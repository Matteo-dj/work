#include "eth_phy.h"
#include "eth_phy_dev.h"
#include "hieth.h"
#include "mdio.h"
#include "stdio.h"

#define WAIT_PHY_AUTO_NEG_TIMES    25

bool hieth_get_phy_stat(eth_phy_access_t *f, int *state)
{
    struct hieth_netdev_local *ld = &netdev_local;
    int phy_state;
    int i;

    *state = 0;
    phy_state = hieth_mdio_read(ld, (unsigned int)f->phy_addr, PHY_BMSR);
    if (phy_state >= 0) {
        if (!((UINT32)phy_state & BMSR_AN_COMPLETE)) {
            dbg_phy_printf("waiting for auto-negotiation completed!\n");
            for (i = 0; i < WAIT_PHY_AUTO_NEG_TIMES; i++) {
                phy_state = hieth_mdio_read(ld, (unsigned int)f->phy_addr, PHY_BMSR);
                if ((phy_state >= 0) && ((UINT32)phy_state & BMSR_AN_COMPLETE))
                        break;

                LOS_Msleep(10);
            }
        }
        if ((UINT32)phy_state & BMSR_AN_COMPLETE) {
            if ((UINT32)phy_state & BMSR_LINK)
                *state = (unsigned int)*state|ETH_PHY_STAT_LINK;

            return true;
        }
    }
    return false;
}

/*****************************************************************************
 *
 * Determine link status
 */
int miiphy_link(eth_phy_access_t *f)
{
    struct hieth_netdev_local *ld = &netdev_local;
    int reg;

    reg = hieth_mdio_read(ld, (unsigned int)f->phy_addr, PHY_BMSR);
    if (reg < 0) {
        dbg_phy_printf("PHY_BMSR read failed, assuming no link\n");
        return 0;
    }

    /* Determine if a link is active */
    if (((UINT32)reg & BMSR_LINK) != 0)
        return 1;
    else
        return 0;
}
/*****************************************************************************
 *
 * Return 1 if PHY supports 1000BASE-X, 0 if PHY supports 10BASE-T/100BASE-TX/
 * 1000BASE-T, or on error.
 */
#if (PHY_SPEED_ABILITY == SPEED_1000)   
static int miiphy_is_1000base_x(eth_phy_access_t *f)
{
    //UINT16 exsr;

    struct hieth_netdev_local *ld = &netdev_local;
    int reg;

    reg = hieth_mdio_read(ld, f->phy_addr, PHY_EXSR);
    if (reg < 0) {
        dbg_phy_printf("PHY_EXSR read failed, assume no 1000BASE-X\n");
        return 0;
    }
    return 0 != (reg & (EXSR_1000XF | EXSR_1000XH));
}
#endif
/*****************************************************************************
 *
 * Determine the ethernet speed (10/100/1000).  Return 10 on error.
 */
int miiphy_speed(eth_phy_access_t *f)
{
    int bmcr, anlpar;
    
    struct hieth_netdev_local *ld = &netdev_local;
    
#if (PHY_SPEED_ABILITY == SPEED_1000)  
    int val = hieth_mdio_read(ld, f->phy_addr, PHY_BMSR);
    if (val < 0) {
        dbg_phy_printf("PHY 1000BT status[read PHY_BMSR]\n");
        goto miiphy_read_failed;
    }

    if (val & BMSR_ESTATEN) {
        if (miiphy_is_1000base_x(f)) {
            dbg_phy_printf("----- fn %s %d[val 0x%x]\n", __func__, __LINE__, val);
            return SPEED_1000;
        }
        int btsr = hieth_mdio_read(ld, f->phy_addr, PHY_1000BTSR);
        if (btsr < 0) {
            dbg_phy_printf("PHY 1000BT status[read PHY_1000BTSR]\n");
            goto miiphy_read_failed;
        }

        if (btsr != 0xFFFF && (btsr & (PHY_1000BTSR_1000FD | PHY_1000BTSR_1000HD))) {
            dbg_phy_printf("----- fn %s %d[val 0x%x, btsr 0x%x]\n", __func__, __LINE__, val, btsr);

            return SPEED_1000;
        }
    }
#endif

    /* Check Basic Management Control Register first. */
    bmcr = hieth_mdio_read(ld, (unsigned int)f->phy_addr, PHY_BMCR);
    if (bmcr < 0) {
        dbg_phy_printf("PHY speed read failed[read PHY_BMCR]\n");
        goto miiphy_read_failed;
    }

    dbg_phy_printf("fn %s %d bmcr=0x%x\n", __func__, __LINE__, bmcr);
    if ((UINT32)bmcr & BMCR_AN_ENABLE) {
        anlpar = hieth_mdio_read(ld, (unsigned int)f->phy_addr, PHY_ANLPAR);
        if (anlpar < 0) {
            dbg_phy_printf("PHY AN speed failed[anlpar]\n");
            goto miiphy_read_failed;
        }
        dbg_phy_printf("----- fn %s %d[bmcr 0x%x, anlpar 0x%x]\n", __func__, __LINE__, bmcr, anlpar);
        return ((unsigned int)anlpar & ANLPAR_100) ? SPEED_100 : SPEED_10;
    }

    return ((UINT32)bmcr & BMCR_SPEED100) ? SPEED_100 : SPEED_10;

miiphy_read_failed:
    dbg_phy_printf("read failed, assuming 10BASE-T\n");
    return SPEED_10;
}

/*****************************************************************************
 *
 * Determine full/half duplex.  Return half on error.
 */
int miiphy_duplex(eth_phy_access_t *f)
{
    struct hieth_netdev_local *ld = &netdev_local;
    int bmcr, anlpar;
    
#if (PHY_SPEED_ABILITY == SPEED_1000) 
    int val = hieth_mdio_read(ld, f->phy_addr, PHY_BMSR);
    if (val < 0) {
        dbg_phy_printf("PHY duplex read failed\n");
        goto miiphy_read_failed;
    }

    if (val & BMSR_ESTATEN) {
        int btsr;

        /* Check for 1000BASE-X. */
        if (miiphy_is_1000base_x(f)) {
            /* 1000BASE-X */
            anlpar = hieth_mdio_read(ld, f->phy_addr, PHY_ANLPAR);
            if (anlpar < 0) {
                printf("1000BASE-X PHY AN duplex");
                goto miiphy_read_failed;
            }
        }
        /* No 1000BASE-X, so assume 1000BASE-T/100BASE-TX/
         * 10BASE-T register set.
         */
        /* Check for 1000BASE-T. */
        btsr = hieth_mdio_read(ld, f->phy_addr, PHY_1000BTSR);
        if (btsr < 0) {
            dbg_phy_printf("PHY 1000BT status");
            goto miiphy_read_failed;
        }

        if (btsr != 0xFFFF) {
            if (btsr & PHY_1000BTSR_1000FD) {
                dbg_phy_printf("----- fn %s %d[val 0x%x, btsr 0x%x]\n", __func__, __LINE__, val, btsr);
                return DUPLEX_FULL;
            } else if (btsr & PHY_1000BTSR_1000HD) {
                dbg_phy_printf("----- fn %s %d[val 0x%x, btsr 0x%x]\n", __func__, __LINE__, val, btsr);
                return DUPLEX_HALF;
            }
        }
    }
#endif

    /* Check Basic Management Control Register first. */
    bmcr = hieth_mdio_read(ld, (unsigned int)f->phy_addr, PHY_BMCR);
    if (bmcr < 0) {
        dbg_phy_printf("PHY duplex");
        goto miiphy_read_failed;
    }
    /* Check if auto-negotiation is on. */
    if ((UINT32)bmcr & BMCR_AN_ENABLE) {
        /* Get auto-negotiation results. */
        anlpar = hieth_mdio_read(ld, (unsigned int)f->phy_addr, PHY_ANLPAR);
        if (anlpar < 0) {
            dbg_phy_printf("PHY AN duplex");
            goto miiphy_read_failed;
        }
        return ((unsigned int)anlpar & (ANLPAR_10FD | ANLPAR_TXFD)) ?
            DUPLEX_FULL : DUPLEX_HALF;
    }
    /* Get speed from basic control settings. */

    return ((UINT32)bmcr & BMCR_FULL_DUPLEX) ? DUPLEX_FULL : DUPLEX_HALF;

miiphy_read_failed:
    dbg_phy_printf(" read failed, assuming half duplex\n");
    return DUPLEX_HALF;
}
