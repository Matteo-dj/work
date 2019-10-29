/**
 ******************************************************************************
 * @file       mrs_bso_collect.c
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
#include "mrs_srv_ndm_collectdata.h"
#include "mrs_cfg.h"

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
typedef enum
{
    BSO_COLLECT_NONE = 0, 
    BSO_COLLECT_RUN, 
    BSO_COLLECT_REAL_START,
    BSO_COLLECT_REAL_STOP,
    BSO_COLLECT_SAVE_START,
    BSO_COLLECT_SAVE_STOP,
} mrs_bso_clt_state_e;

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
static void  
bsocollect_real_start(int fd, mrs_bso_t* mrs_bso)
{
    PACK_MALLOC2;
    HI_U8 resp = BSO_RESP_OK;

    //����洢������,��ֹͣ;
    Mrs_Misc_Collect_Save_Stop();
    HI_MDM_Sleep(500);

    if (Mrs_Misc_MallocMem(MRS_DC_SD_DEFAULT_NV_VALUE_RAM_SIZE) 
        != HI_ERR_SUCCESS)
    {
        resp = BSO_RESP_ERR;
    }

    load_uint8(ctxt, &clen, resp);
    wrlen = mrs_ptl68_pack(mrs_ptl68_ctrlcode_set(1, 0, 
        BSO_TYPE_COLLECT), BSO_CMD_CLT_REAL_START, 
        ctxt, clen, wrbuf, BSO_WRBUF_LEN);
    
    mrs_chan_writes(mrs_bso->chantype, 1, fd, wrbuf, wrlen);
    if (resp == BSO_RESP_OK)
    {
        Mrs_Misc_Collect_Real();
        Mrs_Misc_FreeMem();
    }
    else
    {
        printf("[%s %u]: Reboot\r\n",__func__,__LINE__);
        Mrs_Misc_Reboot();
    }
    //����ִ��ʱ��> 3��,�����һ��
    Mrs_Misc_Check_Time_Set();
    mrs_bso_clr(mrs_bso);
}

/**
 ******************************************************************************
 * @brief      ����ҵ�����ݷ��ʹ���
 * @param[in]  fd �����
 * @param[in]  pri ��ҵ�����
 * @retval     STATUS  ��OK-�ɹ�, ERROR-ʧ��
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
static HI_S32 
mrs_bsocollect_run(HI_S32 fd, mrs_bso_t* mrs_bso)
{
    switch (mrs_bso->run_state)
    {
        case BSO_COLLECT_REAL_START:
            bsocollect_real_start(fd, mrs_bso);
            break;

        case BSO_COLLECT_REAL_STOP:
            {
                PACK_MALLOC2;

                load_uint8(ctxt, &clen, BSO_RESP_OK);
                wrlen = mrs_ptl68_pack(mrs_ptl68_ctrlcode_set(1, 0, 
                    BSO_TYPE_COLLECT), BSO_CMD_CLT_REAL_STOP, 
                    ctxt, clen, wrbuf, BSO_WRBUF_LEN);
                
                mrs_chan_writes(mrs_bso->chantype, 1, fd, wrbuf, wrlen);
                mrs_bso_clr(mrs_bso);
            }
            break;
            
        case BSO_COLLECT_SAVE_START:
            {
                PACK_MALLOC2;
                
                load_uint8(ctxt, &clen, BSO_RESP_OK);
                wrlen = mrs_ptl68_pack(mrs_ptl68_ctrlcode_set(1, 0, 
                    BSO_TYPE_COLLECT), BSO_CMD_CLT_SAVE_START, 
                    ctxt, clen, wrbuf, BSO_WRBUF_LEN);
                
                mrs_chan_writes(mrs_bso->chantype, 1, fd, wrbuf, wrlen);
                Mrs_Misc_Collect_Save_Start();
                Mrs_Misc_Bsn_Set(BSO_TYPE_COLLECT, BSO_CMD_CLT_SAVE_START, 
                    HI_MDM_GetMilliSeconds(), 3000);
                mrs_bso_clr(mrs_bso);
            }
            break;

        case BSO_COLLECT_SAVE_STOP:
            {
                PACK_MALLOC2;

                load_uint8(ctxt, &clen, BSO_RESP_OK);
                wrlen = mrs_ptl68_pack(mrs_ptl68_ctrlcode_set(1, 0, 
                    BSO_TYPE_COLLECT), BSO_CMD_CLT_SAVE_STOP, 
                    ctxt, clen, wrbuf, BSO_WRBUF_LEN);
                
                mrs_chan_writes(mrs_bso->chantype, 1, fd, wrbuf, wrlen);
                Mrs_Misc_Collect_Save_Stop();
                Mrs_Misc_Bsn_Set(BSO_TYPE_COLLECT, BSO_CMD_CLT_SAVE_STOP, 
                    HI_MDM_GetMilliSeconds(), 3000);
                mrs_bso_clr(mrs_bso);
            }
            break;
            
        default:
            break;
    }
    
    return OK;
}

/**
 ******************************************************************************
 * @brief      ����ҵ�����ݽ��մ���
 * @param[in]  fd �����
 * @param[in]  buf ���������ݻ�����
 * @param[in]  len ���������ݳ���
 * @param[in]  pri ��ҵ�����
 * @retval     STATUS  ��OK-�ɹ�, ERROR-ʧ��
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
static HI_S32 
mrs_bsocollect_proc(HI_S32 fd, HI_U8 *buf, HI_S32 len, 
    mrs_bso_t* mrs_bso)
{
    HI_U16 cmdId;
    HI_U8 *data = &buf[PTL_DATA_OFT_HEAD];

    (hi_void)memcpy_s(&cmdId, PTL_CMDID_SIZE, data, PTL_CMDID_SIZE);
    
    if (BSO_CMD_CLT_REAL_START == cmdId)
    {
        HI_S32 AgcGain = 0;
        if (data[2])
        {
            AgcGain = -12;
        }
        Mrs_Misc_Collect_AgcGain_Set(AgcGain);
        mrs_bso->run_state = BSO_COLLECT_REAL_START;
    }
    else if (BSO_CMD_CLT_REAL_STOP == cmdId)
    {
        mrs_bso->run_state = BSO_COLLECT_REAL_STOP;
    }
    else if (BSO_CMD_CLT_SAVE_START == cmdId)
    {
        HI_S32 AgcGain = 0;
        if (data[2])
        {
            AgcGain = -12;
        }
        Mrs_Misc_Collect_AgcGain_Set(AgcGain);
        mrs_bso->run_state = BSO_COLLECT_SAVE_START;
    }
    else if (BSO_CMD_CLT_SAVE_STOP == cmdId)
    {
        mrs_bso->run_state = BSO_COLLECT_SAVE_STOP;
    }
    else
    {
        mrs_bso_clr(mrs_bso);
    }

    return OK;
}

/**
 ******************************************************************************
 * @brief      ����ҵ�����ʵ����
 * @param[in]  mrs_bso_t *mrs_bso  ��ҵ�����
 * @retval     mrs_bso_t*  �����ص�ҵ�����
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
mrs_bso_t* 
mrs_bsocollect_getinstanc(mrs_bso_t* mrs_bso)
{   
    mrs_bso->bsotype = BSO_TYPE_COLLECT;
    mrs_bso->t = HI_MDM_GetMilliSeconds();
    mrs_bso->wcnt = 0;

    mrs_bso->run_state = BSO_COLLECT_RUN;
    mrs_bso->proc_state = BSO_COLLECT_RUN;
    
    mrs_bso->run = mrs_bsocollect_run;
    mrs_bso->proc = mrs_bsocollect_proc;
    
    return mrs_bso;
}

/*-------------------------------End of mrs_bso_collect.c--------------------------------*/
#endif