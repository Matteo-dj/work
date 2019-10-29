/**
 ******************************************************************************
 * @file       mrs_bso_sg.c
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
#include "mrs_srv_ndm_sendsignal.h"
#include "mrs_cfg.h"

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
typedef enum
{
    BSO_SG_NONE = 0,
    BSO_SG_RUN,
    BSO_SG0,
    BSO_SG1,
    BSO_SG2,
    BSO_SG1_STOP,
} mrs_bso_sg_state_e;

/*-----------------------------------------------------------------------------
 Section: Constant Definitions
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
void Mrs_Misc_SendSignalToMode(HI_BOOL sg_type)
{
    if (sg_type)
    {
        Mrs_Misc_SendSignalToMode1();
    }
    else
    {
        Mrs_Misc_SendSignalToMode2();
    }
}

static HI_U8 
bsosg_run(HI_BOOL sg_type, int fd, mrs_bso_t* bso)
{
    PACK_MALLOC2;
    HI_U8 resp = BSO_RESP_OK;
    HI_U8 flag = 0;
    HI_U8 cmdId = (sg_type ? BSO_CMD_SG1 : BSO_CMD_SG2);
    HI_U32 timeout = (sg_type ? 60000 : 0xFFFFFFFF);

    do
    {
        mrs_misc_bsn_t* bsn = Mrs_Misc_Bsn_Get(BSO_TYPE_SG);
        if ((bsn->bsotype == BSO_TYPE_SG) && ((bsn->cmdId == BSO_CMD_SG1) 
            || (bsn->cmdId == BSO_CMD_SG2)))
        {
            resp = BSO_RESP_ERR;
            break;
        }

        if (Mrs_Misc_MallocMem(MRS_SEND_SIGNAL_FILE_SIZE) != HI_ERR_SUCCESS)
        {
            resp = BSO_RESP_ERR;
            break;
        }

        Mrs_Misc_SendSignalStart();
        Mrs_Misc_SendSignalToMode(sg_type);
        Mrs_Misc_Bsn_Set(BSO_TYPE_SG, cmdId, HI_MDM_GetMilliSeconds(), timeout);
        //信号发送执行时间> 3秒,需调用一次
        Mrs_Misc_Check_Time_Set();
    } while(0);

    flag = ((resp != BSO_RESP_OK) ? 1 : 0);
    load_uint8(ctxt, &clen, resp);
    wrlen = mrs_ptl68_pack(mrs_ptl68_ctrlcode_set(1, flag, BSO_TYPE_SG), 
        cmdId, ctxt, clen, wrbuf, BSO_WRBUF_LEN);
    
    mrs_chan_writes(bso->chantype, 1, fd, wrbuf, wrlen);
    return resp;
}

static HI_S32 
mrs_bsosg_run(int fd, mrs_bso_t* bso)
{
    switch (bso->run_state)
    {
        case BSO_SG0:
            {
                PACK_MALLOC2;

                load_uint8(ctxt, &clen, BSO_RESP_OK);
                wrlen = mrs_ptl68_pack(mrs_ptl68_ctrlcode_set(1, 0, BSO_TYPE_SG), 
                    BSO_CMD_SG0, ctxt, clen, wrbuf, BSO_WRBUF_LEN);
                
                mrs_chan_writes(bso->chantype, 1, fd, wrbuf, wrlen);

                Mrs_Misc_Bsn_Clr(BSO_TYPE_SG);
                Mrs_Misc_SendSignalToMode0();
                Mrs_Misc_FreeMem();
                
                //复位   
                printf("[%s %u]: Reboot\r\n",__func__,__LINE__);
                Mrs_Misc_Reboot();
                mrs_bso_clr(bso);
            }
            break;

        case BSO_SG1:
            if (bsosg_run(TRUE, fd, bso) != BSO_RESP_OK)
            {
                printf("[%s %u]: Reboot\r\n",__func__,__LINE__);
                Mrs_Misc_Reboot();
            }
            mrs_bso_clr(bso);
            break;

        case BSO_SG2:
            if (bsosg_run(FALSE, fd, bso) != BSO_RESP_OK)
            {
                printf("[%s %u]: Reboot\r\n",__func__,__LINE__);
                Mrs_Misc_Reboot();
            }
            mrs_bso_clr(bso);
            break;

        case BSO_SG1_STOP:
            {
                PACK_MALLOC2;

                load_uint8(ctxt, &clen, BSO_RESP_OK);
                wrlen = mrs_ptl68_pack(mrs_ptl68_ctrlcode_set(1, 0, BSO_TYPE_SG), 
                    BSO_CMD_SG1_STOP, ctxt, clen, wrbuf, BSO_WRBUF_LEN);

                mrs_chan_writes(bso->chantype, 1, fd, wrbuf, wrlen);

                Mrs_Misc_Bsn_Clr(BSO_TYPE_SG);
                Mrs_Misc_SendSignalToMode0();
                Mrs_Misc_FreeMem();

                //复位
                printf("[%s %u]: Reboot\r\n",__func__,__LINE__);
                Mrs_Misc_Reboot();
                mrs_bso_clr(bso);
            }
            break;
            
        default:
            break;
    }
    
    return OK;
}

static HI_S32 
mrs_bsosg_proc(HI_S32 fd, HI_U8 *buf, int len, 
    mrs_bso_t* mrs_bso)
{
    HI_U16 cmdId;
    HI_U8 *data = &buf[PTL_DATA_OFT_HEAD];

    (hi_void)memcpy_s(&cmdId, PTL_CMDID_SIZE, data, PTL_CMDID_SIZE);
    
    if (BSO_CMD_SG0 == cmdId)
    {
        mrs_bso->run_state = BSO_SG0;
    }
    else if (BSO_CMD_SG1 == cmdId)
    {
        mrs_bso->run_state = BSO_SG1;
    }
    else if (BSO_CMD_SG2 == cmdId)
    {
        mrs_bso->run_state = BSO_SG2;
    }
    else if (BSO_CMD_SG1_STOP == cmdId)
    {
        mrs_bso->run_state = BSO_SG1_STOP;
    }
    else
    {
        mrs_bso_clr(mrs_bso);
    }

    return OK;
}

mrs_bso_t* 
mrs_bsosg_getinstanc(mrs_bso_t* mrs_bso)
{
    mrs_bso->bsotype = BSO_TYPE_SG;
    mrs_bso->t = HI_MDM_GetMilliSeconds();
    mrs_bso->wcnt = 0;
    
    mrs_bso->run = mrs_bsosg_run;
    mrs_bso->proc = mrs_bsosg_proc;
    
    mrs_bso->run_state = BSO_SG_RUN;
    mrs_bso->proc_state = BSO_SG_RUN;

    return mrs_bso;
}

/*-------------------------------End of mrs_bso_sg.c--------------------------------*/
#endif