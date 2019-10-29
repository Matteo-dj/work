/**
 ******************************************************************************
 * @file       mrs_bso_param.c
 * @version   
 * @brief  
 * @details  
 *
 * @copyright  
 ******************************************************************************
 */
#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined (PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_CCT3911)

/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include "mrs_cfg.h"
#include "hi_mdm_time.h"
#include "hi_socket.h"
#include "hi_mdm_task.h"
#include "hi_mdm_network.h"

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
typedef enum
{
    BSO_PARAM_NONE = 0,
    BSO_PARAM_RUN, 
    BSO_PARAM_RD_ALL, 
    BSO_PARAM_WR_IP,  
    BSO_PARAM_WR_TEI, 
    BSO_PARAM_RD_IP, 
    BSO_PARAM_RD_TEI, 
    BSO_PARAM_RD_IP3911,
} mrs_bso_param_state_e;

/*-----------------------------------------------------------------------------
 Section: Global Variables
 ----------------------------------------------------------------------------*/
/* None */

/*-----------------------------------------------------------------------------
 Section: Local Variables
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 Section: Local Function Prototypes
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 Section: Function Definitions
 ----------------------------------------------------------------------------*/
static HI_S32 
mrs_bsoparam_run(int fd, mrs_bso_t* mrs_bso)
{
    switch (mrs_bso->run_state)
    {
        case BSO_PARAM_WR_IP:
            {
                PACK_MALLOC2;
                HI_U8 resp = mrs_bso->misc_use;
                
                load_uint8(ctxt, &clen, resp);
                wrlen = mrs_ptl68_pack(mrs_ptl68_ctrlcode_set(1, 0, 
                    BSO_TYPE_PARAM), BSO_CMD_PARAM_WR_IP, 
                    ctxt, clen, wrbuf, BSO_WRBUF_LEN);

                mrs_chan_writes(mrs_bso->chantype, 1, fd, wrbuf, wrlen);

                printf("[%s %u]: Reboot\r\n",__func__,__LINE__);
                Mrs_Misc_Reboot();
                mrs_bso_clr(mrs_bso);
            }
            break;

        case BSO_PARAM_WR_TEI:
            {
                PACK_MALLOC2;
                HI_U8 resp = mrs_bso->misc_use;
                
                load_uint8(ctxt, &clen, resp);
                wrlen = mrs_ptl68_pack(mrs_ptl68_ctrlcode_set(1, 0, 
                    BSO_TYPE_PARAM), BSO_CMD_PARAM_WR_TEI, 
                    ctxt, clen, wrbuf, BSO_WRBUF_LEN);

                mrs_chan_writes(mrs_bso->chantype, 1, fd, wrbuf, wrlen);
                mrs_bso_clr(mrs_bso);
            }
            break;

        case BSO_PARAM_RD_IP:
            {
                PACK_MALLOC2;
                HI_U8 ip;

                ip = Mrs_Misc_Ip_Get();
                load_uint8(ctxt, &clen, ip);
                wrlen = mrs_ptl68_pack(mrs_ptl68_ctrlcode_set(1, 0, 
                    BSO_TYPE_PARAM), BSO_CMD_PARAM_RD_IP, 
                    ctxt, clen, wrbuf, BSO_WRBUF_LEN);

                mrs_chan_writes(mrs_bso->chantype, 1, fd, wrbuf, wrlen);
                mrs_bso_clr(mrs_bso);
            }
            break;

        case BSO_PARAM_RD_TEI:
            {
                PACK_MALLOC2;
                HI_U16 tei;

                tei = Mrs_Misc_Tei_Get();
                load_uint16(ctxt, &clen, tei);
                wrlen = mrs_ptl68_pack(mrs_ptl68_ctrlcode_set(1, 0, 
                    BSO_TYPE_PARAM), BSO_CMD_PARAM_RD_TEI, 
                    ctxt, clen, wrbuf, BSO_WRBUF_LEN);

                mrs_chan_writes(mrs_bso->chantype, 1, fd, wrbuf, wrlen);
                mrs_bso_clr(mrs_bso);
            }
            break;

        case BSO_PARAM_RD_IP3911:
            {
                PACK_MALLOC2;

                HI_U8 ip[HI_IPV4_ADDR_LEN] = {0};
                HI_SYS_ETH_CONFIG_S EthCfgInfo;
                (hi_void)memset_s(&EthCfgInfo, sizeof(HI_SYS_ETH_CONFIG_S), 0, sizeof(HI_SYS_ETH_CONFIG_S));
                
                HI_MDM_GetEthConfig(&EthCfgInfo);
                (hi_void)memcpy_s(ip, HI_IPV4_ADDR_LEN, EthCfgInfo.ip_addr, HI_IPV4_ADDR_LEN);

                load_buf(ctxt, &clen, ip, HI_IPV4_ADDR_LEN);
                wrlen = mrs_ptl68_pack(mrs_ptl68_ctrlcode_set(1, 0, 
                    BSO_TYPE_PARAM), BSO_CMD_PARAM_RD_IP3911, 
                    ctxt, clen, wrbuf, BSO_WRBUF_LEN);

                mrs_chan_writes(mrs_bso->chantype, 1, fd, wrbuf, wrlen);
                mrs_bso_clr(mrs_bso);
            }
            break;
            
        default:
            break;
    }
    
    return OK;
}

static HI_S32 
mrs_bsoparam_proc(int fd, HI_U8 *buf, int len, 
    mrs_bso_t* mrs_bso)
{
    HI_U16 cmdId;
    HI_U8 *data = &buf[PTL_DATA_OFT_HEAD];
    
    (hi_void)memcpy_s(&cmdId, PTL_CMDID_SIZE, data, PTL_CMDID_SIZE);

    if (BSO_CMD_PARAM_WR_IP == cmdId)
    {
        mrs_bso->misc_use = ((Mrs_Misc_Ip_Set(data[2]) < 0) ? BSO_RESP_ERR : BSO_RESP_OK);
        mrs_bso->run_state = BSO_PARAM_WR_IP;
    }
    else if (BSO_CMD_PARAM_WR_TEI == cmdId)
    {
        HI_U16 tei;
        (hi_void)memcpy_s(&tei, 2, &data[2], 2);
        mrs_bso->misc_use = ((Mrs_Misc_Tei_Set(tei) < 0) ? BSO_RESP_ERR : BSO_RESP_OK);
        mrs_bso->run_state = BSO_PARAM_WR_TEI;
    }
    else if (BSO_CMD_PARAM_RD_IP == cmdId)
    {
        mrs_bso->run_state = BSO_PARAM_RD_IP;
    }
    else if (BSO_CMD_PARAM_RD_TEI == cmdId)
    {
        mrs_bso->run_state = BSO_PARAM_RD_TEI;
    }
    else if (BSO_CMD_PARAM_RD_IP3911 == cmdId)
    {
        mrs_bso->run_state = BSO_PARAM_RD_IP3911;
    }
    else
    {
        mrs_bso_clr(mrs_bso);
    }

    return OK;
}

mrs_bso_t* 
mrs_bsoparam_getinstanc(mrs_bso_t* mrs_bso)
{
    mrs_bso->bsotype = BSO_TYPE_PARAM;
    mrs_bso->t = HI_MDM_GetMilliSeconds();
    mrs_bso->misc_use = 0;
    mrs_bso->wcnt = 0;

    mrs_bso->run_state = BSO_PARAM_RUN;
    mrs_bso->proc_state = BSO_PARAM_RUN;
    
    mrs_bso->run = mrs_bsoparam_run;
    mrs_bso->proc = mrs_bsoparam_proc;
    
    return mrs_bso;
}

/*-------------------------------End of mrs_bso_param.c--------------------------------*/
#endif