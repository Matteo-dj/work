/**
 ******************************************************************************
 * @file       mrs_bso_misc.c
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
#include "hi_mdm_time.h"
#include "mrs_cfg.h"

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
typedef enum
{
    BSO_MISC_NONE = 0, 
    BSO_MISC_RUN,
    BSO_MISC_HEARTBEAT,
    BSO_MISC_TIME,
    BSO_MISC_NTB,
} mrs_bso_misc_state_e;

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
mrs_bsomisc_run(int fd, mrs_bso_t* mrs_bso)
{
    switch (mrs_bso->run_state)
    {   
        case BSO_MISC_HEARTBEAT:
            {
                PACK_MALLOC2;

                load_uint8(ctxt, &clen, BSO_RESP_OK);
                wrlen = mrs_ptl68_pack(mrs_ptl68_ctrlcode_set(1, 0, 
                    BSO_TYPE_MISC), BSO_CMD_MISC_HEARTBEAT, 
                    ctxt, clen, wrbuf, BSO_WRBUF_LEN);
 
                mrs_chan_writes(mrs_bso->chantype, 1, fd, wrbuf, wrlen);
                mrs_bso_clr(mrs_bso);
            }
            break;

        case BSO_MISC_TIME:
            {
                PACK_MALLOC2;

                load_uint8(ctxt, &clen, BSO_RESP_OK);
                wrlen = mrs_ptl68_pack(mrs_ptl68_ctrlcode_set(1, 0, 
                    BSO_TYPE_MISC), BSO_CMD_MISC_TIME, 
                    ctxt, clen, wrbuf, BSO_WRBUF_LEN);
 
                mrs_chan_writes(mrs_bso->chantype, 1, fd, wrbuf, wrlen);
                mrs_bso_clr(mrs_bso);
            }
            break;

        case BSO_MISC_NTB:
            {
                PACK_MALLOC2;

                load_uint8(ctxt, &clen, BSO_RESP_OK);
                wrlen = mrs_ptl68_pack(mrs_ptl68_ctrlcode_set(1, 0, 
                    BSO_TYPE_MISC), BSO_CMD_MISC_NTB, 
                    ctxt, clen, wrbuf, BSO_WRBUF_LEN);
 
                mrs_chan_writes(mrs_bso->chantype, 1, fd, wrbuf, wrlen);
                Mrs_Misc_CollectNtb();
                mrs_bso_clr(mrs_bso);
            }
            break;

        default:
            break;
    }
    
    return OK;
}

static HI_S32 
mrs_bsomisc_proc(int fd, HI_U8 *buf, int len, 
    mrs_bso_t* mrs_bso)
{
    HI_U16 cmdId;
    HI_U8 *data = &buf[PTL_DATA_OFT_HEAD];

    (hi_void)memcpy_s(&cmdId, PTL_CMDID_SIZE, data, PTL_CMDID_SIZE);

    if (BSO_CMD_MISC_HEARTBEAT == cmdId)
    {
        mrs_bso->run_state = BSO_MISC_HEARTBEAT;
    }
    else if (BSO_CMD_MISC_TIME == cmdId)
    {
        HI_U32 t;
        (hi_void)memcpy_s(&t, sizeof(HI_U32), data + 2, sizeof(HI_U32));
        Mrs_Misc_Time_Request_Set(t);
        //时间更新,需调用一次
        Mrs_Misc_Check_Time_Set();
        mrs_bso->run_state = BSO_MISC_TIME;
    }
    else if (BSO_CMD_MISC_NTB == cmdId)
    {
        mrs_bso->run_state = BSO_MISC_NTB;
    }
    else
    {
        mrs_bso_clr(mrs_bso);
    }

    return OK;
}

mrs_bso_t* 
mrs_bsomisc_getinstanc(mrs_bso_t* mrs_bso)
{
    mrs_bso->bsotype = BSO_TYPE_MISC;
    mrs_bso->t = HI_MDM_GetMilliSeconds();
    mrs_bso->wcnt = 0;
    
    mrs_bso->run = mrs_bsomisc_run;
    mrs_bso->proc = mrs_bsomisc_proc;
    
    mrs_bso->run_state = BSO_MISC_RUN;
    mrs_bso->proc_state = BSO_MISC_RUN;
    
    return mrs_bso;
}

/*-------------------------------End of mrs_bso_misc.c--------------------------------*/
#endif