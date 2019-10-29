#include "hieth.h"
#include "mdio.h"

static int wait_mdio_ready(struct hieth_netdev_local *ld)
{
    int timeout_us = 1000;

    while (--timeout_us && !test_mdio_ready(ld))
        LOS_Udelay(1);

    return timeout_us;
}

int hieth_mdio_read(struct hieth_netdev_local *ld, unsigned int phy_addr, unsigned int regnum)
{
    int val = 0;

    HISI_NET_LOCK_GET(&hieth_glb_reg_lock);

    if (!wait_mdio_ready(ld)) {
        goto error_exit;
    }

    mdio_start_phyread(ld, phy_addr, regnum);

    if (wait_mdio_ready(ld))
        val = mdio_get_phyread_val(ld);
    else
    {
        hieth_debug(LOS_ERR_LEVEL, "Read mdio failed\n");
    }

error_exit:

    HISI_NET_LOCK_PUT(&hieth_glb_reg_lock);


    return val;
}

int hieth_mdio_write(
        struct hieth_netdev_local *ld,
        int phy_addr,
        int regnum,
        int val)
{
    int ret = 0;

    HISI_NET_LOCK_GET(&hieth_glb_reg_lock);

    if (!wait_mdio_ready(ld)) {
        hieth_debug(LOS_ERR_LEVEL, "mdio busy\n");
        ret = -1;
        goto error_exit;
    }

    mdio_phywrite(ld, phy_addr, regnum, val);

error_exit:

    HISI_NET_LOCK_PUT(&hieth_glb_reg_lock);

    return ret;
}

int hieth_mdio_reset(struct hieth_netdev_local *ld)
{
    mdio_reg_reset(ld);

    return 0;
}

int hieth_mdio_init(struct hieth_netdev_local *ld)
{
    (void)hieth_mdio_reset(ld);

    return 0;
}

void hieth_mdio_exit(struct hieth_netdev_local *ld)
{
}

/* vim: set ts=8 sw=8 tw=78: */
