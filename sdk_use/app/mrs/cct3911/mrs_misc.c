/**
 ******************************************************************************
 * @file      mrs_misc.c
 * @brief     
 * @details   This file including all API functions's implement of mrs_chan.c.
 * @copyright 
 ******************************************************************************
 */
#include <hi_types.h>
#include <hi_mdm_types.h>

#if defined (PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_CCT3911)

/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include "mrs_sdsave.h"
#include "mrs_cfg.h"

#include "hi_types.h"
#include "hi_config.h"
#include "hi_key.h"
#include "hi_ft_nv.h"
#include "hi_share_memory.h"
#include "hi_algorithm.h"
#include "hi_socket.h"

#include "hi_mdm_phy.h"
#include "hi_mdm_sys.h"
#include "hi_mdm_task.h"
#include "hi_mdm_time.h"
#include "hi_mdm_datacollect.h"
#include "hi_mdm_mac.h"
#include "securec.h"

#include "mrs_srv_ndm_collectdata.h"
#include "mrs_srv_ndm_sendsignal.h"
#include "mrs_fw_tools.h"
#include "mrs_common.h"
#include "app_ndm_msg.h"
//#include "mrs_fw_n.h"

//TODO 简单接口先封装起来，后面再详细看(wangchao 00230251)
#define mrsSendMessage(a,b,c,d,e)   APP_NdmMessageSend(MRS_NDM_MSG_Q_1,a,b,c,d,e)

//#include "app_ndm_msg.h"

/*-----------------------------------------------------------------------------
 Section: Macro Definitions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 Section: Local Variables
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 Section: Global Variables
 ----------------------------------------------------------------------------*/
/*  信号量资源*/
static HI_U32 misc_semId[MISC_SEM_MAX] = {0};

/*  消息队列资源*/
static HI_U32 misc_queueId[MISC_QUEUE_MAX] = {0};

/*  业务信息记录*/
static mrs_misc_bsn_t misc_bsn_list[BSO_TYPE_MAX];

/* 上报信息记录*/
static mrs_misc_rpt_t the_misc_rpt = {0,0,{0},{{0}}};

/* 参数信息*/
static mrs_misc_param_t the_misc_param = {{0},0,0,0};

static HI_U32 the_check_time = 0;

/* 复位后请求时间,同步3911时钟, 
  * 请求不成功,间隔时间一直请求
  * 非0 : Hi3911已和9G25通讯上
*/
static HI_BOOL the_time_request = FALSE;

// 采数增益
static HI_S32 the_AgcGain = 0;

//共享内存分配
HI_U32 the_Misc_RamAddr = (HI_U32)HI_NULL;

//升级标志
static HI_U8 the_batupg_state = 0;

/*-----------------------------------------------------------------------------
 Section: Local Function Prototypes
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Global Function Prototypes
 ----------------------------------------------------------------------------*/
extern HI_U32 tf_enable_rx_mode_switch(HI_IN HI_BOOL mode_switch);

/*-----------------------------------------------------------------------------
 Section: Function Definitions
 ----------------------------------------------------------------------------*/
void Mrs_Misc_BatUpg_set(HI_U8 state)
{
    the_batupg_state = state;
}

HI_U8 Mrs_Misc_BatUpg_get(void)
{
    return the_batupg_state;
}

void  
Mrs_Misc_Bsn_Clr(HI_U8 bsotype)
{
    if (bsotype >= BSO_TYPE_MAX)
    {
        return;
    }
    
    misc_bsn_list[bsotype].bsotype = BSO_TYPE_MAX;
    misc_bsn_list[bsotype].n = 0;
    misc_bsn_list[bsotype].cmdId = 0;
    misc_bsn_list[bsotype].t = 0;
}

void  
Mrs_Misc_Bsn_Set(HI_U8 bsotype, HI_U16 cmdId, HI_U32 t, 
    HI_U32 timeout)
{
    if (bsotype >= BSO_TYPE_MAX)
    {
        return;
    }
    
    misc_bsn_list[bsotype].bsotype = bsotype;
    misc_bsn_list[bsotype].n = 0;
    misc_bsn_list[bsotype].cmdId = cmdId;
    misc_bsn_list[bsotype].t = t;
    misc_bsn_list[bsotype].timeout = timeout;
}

mrs_misc_bsn_t* 
Mrs_Misc_Bsn_Get(HI_U8 bsotype)
{
    if (bsotype >= BSO_TYPE_MAX)
    {
        return NULL;
    }
    
    return &misc_bsn_list[bsotype];
}

static void 
Mrs_Misc_Collect_Check(void)
{
    if ((misc_bsn_list[BSO_TYPE_COLLECT].cmdId == BSO_CMD_CLT_SAVE_START) 
        && (Mrs_Misc_Collect_MainPhase_Get() == MRS_DC_SD_MAIN_PHASE_STOPPED))
    {
        if (misc_bsn_list[BSO_TYPE_COLLECT].n >= 3)
        {
            printf("[%s %u]: Reboot\r\n",__func__,__LINE__);
            Mrs_Misc_Reboot();
        }
        misc_bsn_list[BSO_TYPE_COLLECT].n ++;
        Mrs_Misc_Collect_Save_Start();
        printf("[%s][%d]: Collect_Save_Start\r\n", __func__, misc_bsn_list[BSO_TYPE_COLLECT].n);
    }
    else if ((misc_bsn_list[BSO_TYPE_COLLECT].cmdId == BSO_CMD_CLT_SAVE_STOP) 
            && (Mrs_Misc_Collect_MainPhase_Get() == MRS_DC_SD_MAIN_PHASE_STARTED))
    {
        if (misc_bsn_list[BSO_TYPE_COLLECT].n >= 3)
        {
            printf("[%s %u]: Reboot\r\n",__func__,__LINE__);
            Mrs_Misc_Reboot();
        }
        misc_bsn_list[BSO_TYPE_COLLECT].n ++;
        Mrs_Misc_Collect_Save_Stop();
        printf("[%s][%d]: Collect_Save_Stop\r\n", __func__, misc_bsn_list[BSO_TYPE_COLLECT].n);
    }
}

void 
Mrs_Misc_Bsn_Check(HI_U8 bsotype)
{
    if (bsotype != misc_bsn_list[bsotype].bsotype)
    {
        return;
    }
    
    HI_U32 t = HI_MDM_GetMilliSeconds();
    if (abs(t - misc_bsn_list[bsotype].t) < misc_bsn_list[bsotype].timeout)
    {
        return;
    }

    misc_bsn_list[bsotype].t = t;
    
    switch (bsotype)
    {            
        case BSO_TYPE_COLLECT:
            Mrs_Misc_Collect_Check();
            break;
            
        case BSO_TYPE_SG:
            if (misc_bsn_list[BSO_TYPE_SG].cmdId == BSO_CMD_SG1)
            {
                //复位 
                printf("[%s %u]: Reboot\r\n",__func__,__LINE__);
                Mrs_Misc_Reboot();
            }
            break;
        default:
            break;
    }

    return;
}

HI_BOOL Mrs_Misc_GetConnectStat(HI_VOID)
{
    mrs_chan_t *pchan = mrs_chan_ctx(CHAN_ETH_9G25);

    if (pchan->fd >= 0)
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

HI_U32 Mrs_Misc_BqUpg(HI_VOID)
{
    HI_U8 batupg_state;
    HI_U32 t1 = HI_MDM_GetSeconds();
    HI_U32 t2 = 0;
    
    Mrs_Misc_BatUpg_set(BAT_UPG_START);

    printf("[%s %u]: batupg start t = %d\r\n", __func__, __LINE__, t1);

    for (;;)
    {
        if (abs(HI_MDM_GetSeconds() - t1) >= 30) // 30秒
        {
            break;
        }

        HI_MDM_Sleep(100);

        batupg_state = Mrs_Misc_BatUpg_get();
        if ((batupg_state == BAT_UPG_START) 
            || (batupg_state == BAT_UPG_STATE))
        {
            if (abs(HI_MDM_GetSeconds() - t2) < 3)
            {
                continue;
            }
            
            HI_U8 rpt_type = ((batupg_state == BAT_UPG_START) 
                ? MISC_RPT_BATUPG : MISC_RPT_BATSTAT);
            
            Mrs_Misc_Rpt_Write(CHAN_LSP_9G25, rpt_type, NULL, 0);
            t2 = HI_MDM_GetSeconds();
        }
        else
        {
            break;
        }
    }

    batupg_state = Mrs_Misc_BatUpg_get();
    printf("[%s %u]: batupg state = %d t = %d\r\n", __func__, __LINE__, 
        batupg_state, HI_MDM_GetSeconds());

    return ((batupg_state == BAT_UPG_SUCCESS) ? HI_ERR_SUCCESS 
        : HI_ERR_FAILURE);
}

static HI_VOID Mrs_Misc_NtbCallback(HI_U32 ulRet, HI_U8 ucLevel, 
    HI_U32 szNtb[EQUIP_ZERO_NTB_NUM])
{
    HI_U8 nbt_num = 0;
    
    printf("Mrs_Misc_NtbCallback: ret = 0x%X,level = %d\r\n",ulRet,ucLevel);
    for (int i = 0;i < EQUIP_ZERO_NTB_NUM; i ++)
    {
        if (szNtb[i] > 0)
        {
            nbt_num ++;
        }
    }

    if (nbt_num >= 3)
    {
        HI_U8 ntb = 1;
        Mrs_Misc_Rpt_Write(CHAN_LSP_9G25, MISC_RPT_NTB, &ntb, 1);
        printf("[%s %u]\r\n",__func__,__LINE__);
    }
}

HI_U32 Mrs_Misc_CollectNtb(void)
{
    return HI_MDM_CollectNtb(HI_TRUE, EQUIP_ZERO_CROSS_LEVEL_1M, PHASE_A, EQUIP_ZERO_CROSS_CRICUIT_RISE, Mrs_Misc_NtbCallback);
}

void 
Mrs_Misc_Ntb_Check(void)
{
    //Hi3911和9G25未通讯上,不检测
    if (!the_time_request)
    {
        return;
    }

    static HI_U32 chk_cnt = 0;
    static HI_U32 pre_time = 0;
    HI_U32 cur_time = 0;

    if (chk_cnt >= 1)   //最多检测1次
    {
        return;
    }

    cur_time = HI_MDM_GetSeconds();
    if (pre_time == 0)
    {
        pre_time = cur_time;
    }
    
    if (abs(cur_time - pre_time) < 10)
    {
        return;
    }

    HI_U32 ret = HI_ERR_SYSTEM_CALL_ERROR;
    ret = Mrs_Misc_CollectNtb();
    if (HI_ERR_SUCCESS != ret)
    {
        printf("ZeroCross Register fail, ret = 0x%X\r\n", ret);
    }

    chk_cnt ++;
    pre_time = cur_time;
}

void 
Mrs_Misc_Check_Time_Set(void)
{
    if (!the_time_request)
    {
        return;
    }

    the_check_time = HI_MDM_GetRealTime();
}

void 
Mrs_Misc_Time_Check(void)
{
    HI_U32 cur_time = HI_MDM_GetRealTime();

    //Hi3911和9G25未通讯上,不检测
    if (!the_time_request)
    {
        return;
    }

    if (the_check_time == 0)
    {
        the_check_time = cur_time;
    }

    if (abs(cur_time - the_check_time) >= 3)
    {
        Mrs_Misc_Rpt_Write(CHAN_LSP_9G25, MISC_RPT_WRTIME, NULL, 0);
        printf("[%s %u]\r\n",__func__,__LINE__);
    }

    the_check_time = cur_time;
}

void 
Mrs_Misc_Time_Request_Set(HI_U32 t)
{
    HI_MDM_SetRealTime(t);
    the_time_request = TRUE;
    printf("[%s %u]: %u %u\r\n",__func__,__LINE__, 
        t, HI_MDM_GetRealTime());
}

void 
Mrs_Misc_Time_Request(void)
{
    if (the_time_request)
    {
        return;

    }

    static HI_U32 pre_time = 0;
    HI_U32 cur_time = HI_MDM_GetSeconds();

    if (pre_time == 0)
    {
        pre_time = cur_time;
    }

    if (abs(cur_time - pre_time) < 5)
    {
        return;
    }

    //上报请求报文
    Mrs_Misc_Rpt_Write(CHAN_LSP_9G25, MISC_RPT_RDTIME, NULL, 0);
    pre_time = cur_time;
}

HI_BOOL 
Mrs_Misc_LinkTo9G25(void)
{
    return the_time_request;
}

void 
Mrs_Misc_Reboot_Report(void)
{
    /*  复位上报*/
    Mrs_Misc_Rpt_Write(CHAN_LSP_9G25, MISC_RPT_REBOOT, NULL, 0);
}

HI_U32 
Mrs_Misc_Sem_Wait(mrs_misc_sem_e n, HI_U32 ulTimeOut)
{
    return HI_MDM_SEM_Wait(misc_semId[n], ulTimeOut);
}

HI_U32 
Mrs_Misc_Sem_Signal(mrs_misc_sem_e n)
{
    return HI_MDM_SEM_Signal(misc_semId[n]);
}

HI_U32 
Mrs_Misc_Msg_Send(mrs_misc_queue_e n, HI_SYS_QUEUE_MSG_S* pstMsg, 
    HI_U32 ulTimeOut)
{
    return HI_MDM_MSG_Send(misc_queueId[n], pstMsg, ulTimeOut);
}

HI_U32 
Mrs_Misc_Msg_Wait(mrs_misc_queue_e n, HI_SYS_QUEUE_MSG_S* pstMsg, 
    HI_U32 ulTimeOut)
{
    return HI_MDM_MSG_Wait(misc_queueId[n], pstMsg, ulTimeOut);
}

HI_U32 Mrs_Misc_MallocMem(HI_U32 ulSize)
{
    if (the_Misc_RamAddr == (HI_U32)HI_NULL)
    {
        the_Misc_RamAddr = (HI_U32)HI_SHARE_MEM_GetMem(ulSize);
        if (the_Misc_RamAddr == (HI_U32)HI_NULL)
        {
            return HI_ERR_NOT_ENOUGH_MEMORY;
        }
    }

    return HI_ERR_SUCCESS;
}

HI_U32 Mrs_Misc_FreeMem(HI_VOID)
{
    if (the_Misc_RamAddr)
    {
        HI_SHARE_MEM_FreeMem((HI_PVOID)the_Misc_RamAddr);
        the_Misc_RamAddr = (HI_U32)HI_NULL;
    }

    return HI_ERR_SUCCESS;
}

static HI_U32 
Mrs_Misc_QueryTfStatusCallBack(HI_MDM_QUERY_NDM2NDM_TF_STATUS_IND_STRU *tf_status)
{
    HI_SYS_QUEUE_MSG_S queue_msg = {0};
    
    printf("[%s]\r\n", __func__);
    printf("Status: %d\r\n", tf_status->ucStatus);
    printf("TfResult: %d\r\n", tf_status->ucTfResult);
    printf("SerialNum: %d\r\n", tf_status->ucSerialNum);
    printf("Phase: %d\r\n", tf_status->ucPhase);
    printf("Level: %d\r\n", tf_status->ucLevel);
    printf("wobble_scope: %d\r\n", tf_status->wobble_scope);
    printf("noise_scope: %d\r\n", tf_status->noise_scope);

    HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("【TF】Mrs_Misc_QueryTfStatusCallBack=%d"));
    
    queue_msg.ulMsgId = (CHAN_ETH_9G25 + 1) | ((MISC_RPT_TF + 1 ) << 8);
    (hi_void)memcpy_s(queue_msg.ulParam, 16, tf_status, sizeof(HI_MDM_QUERY_NDM2NDM_TF_STATUS_IND_STRU));

    return HI_ERR_SUCCESS;
}

HI_U32 
Mrs_Misc_QueryTfStatusEnable(void)
{
    return tf_enable_rx_mode_switch(TRUE);
}

HI_U32 
Mrs_Misc_QueryTfStatusDisable(void)
{
    return tf_enable_rx_mode_switch(FALSE);
}

HI_U8 
Mrs_Misc_Collect_MainPhase_Get(void)
{
    MRS_DC_SD_MAIN_CTRL_CTX_STRU * MainCtrlCtx = MRS_NDM_DC_SD_GET_MAIN_CTRL_CTX();
    return MainCtrlCtx->ucMainPhase;
}

HI_U8 
Mrs_Misc_Collect_StartType_Get(void)
{
    MRS_DC_SD_MAIN_CTRL_CTX_STRU * MainCtrlCtx = MRS_NDM_DC_SD_GET_MAIN_CTRL_CTX();
    return MainCtrlCtx->ucStartType;
}

void  
Mrs_Misc_Collect_AgcGain_Set(HI_S32 AgcGain)
{
    the_AgcGain = AgcGain;
}

HI_S32  
Mrs_Misc_Collect_AgcGain_Get(void)
{
    return the_AgcGain;
}

HI_U32 
Mrs_Misc_Collect_Real(void)
{
    HI_U32 ulErrCode = HI_ERR_SUCCESS;
    DC_INPUT_PARA_STRU DcInParam;
    HI_BUF_S stBuf;

    (hi_void)memset_s(&DcInParam, sizeof(DcInParam), 0, sizeof(DcInParam));
    (hi_void)memset_s(&stBuf, sizeof(stBuf), 0, sizeof(stBuf));
    
    DcInParam.ulStartAddr = (HI_U32)the_Misc_RamAddr;
    DcInParam.ulEndAddr   = (HI_U32)the_Misc_RamAddr + 0x200000;
    DcInParam.lAgcGain    = Mrs_Misc_Collect_AgcGain_Get();
    DcInParam.ulAfterPointlen = MRS_DC_SD_DEFAULT_AFTER_POINT_LEN;
    DcInParam.enDataSrc   = EN_TRIG_AD_12_BIT;
    DcInParam.enTrigerType = EN_TRIG_TYPE_NOISE;
    DcInParam.ulWaitEnvOkTime = 1000;
    DcInParam.ulWaitResultTimeOut = 1000;

    ulErrCode = HI_DC_StartNormalDataCollect(&DcInParam, &stBuf);

    if (HI_ERR_SUCCESS != ulErrCode)
    {
        (hi_void)memset_s(&stBuf, sizeof(stBuf), 0, sizeof(stBuf));
    }

    // 实时采数结果
    printf("COLLECT_REAL AgcGain[%d]: [%u][%u][%u][%u][%u]\n",
        DcInParam.lAgcGain, stBuf.bufAddr[0],stBuf.bufAddr[1],
        stBuf.bufSize[0],stBuf.bufSize[1],ulErrCode);

    mrs_clt_info_t clt_info;

    clt_info.clt_type = CLT_TYPE_REAL;
    clt_info.file_type = MRS_DC_SD_SAVE_FILE_TYPE_N;
    clt_info.hi_buf = (void *)&stBuf;

    Mrs_Sdsave_FileWrite(BSO_TYPE_BDCOLLECT, &clt_info);
    return HI_ERR_SUCCESS;
}

HI_U32 
Mrs_Misc_Collect_Save_Start(void)
{
    if (Mrs_Misc_Collect_MainPhase_Get() == MRS_DC_SD_MAIN_PHASE_STOPPED)
    {
        mrsSendMessage(EN_MRS_FW_MSG_NDM_KEY, HI_DRV_KEY_NUM_1, 0, 
            HI_DRV_KEY_PRESS_TIME_PRECISION_MS, 0);
    }
    else if (Mrs_Misc_Collect_MainPhase_Get() == MRS_DC_SD_MAIN_PHASE_STARTED)
    {
        //如果采数启动中先停止
        mrsSendMessage(EN_MRS_FW_MSG_NDM_KEY, HI_DRV_KEY_NUM_1, 0, 
            HI_DRV_KEY_PRESS_TIME_PRECISION_MS, 0);
    }
    
    return HI_ERR_SUCCESS;
}

HI_U32 
Mrs_Misc_Collect_Save_Stop(void)
{
    if (Mrs_Misc_Collect_MainPhase_Get() == MRS_DC_SD_MAIN_PHASE_STARTED)
    {
        mrsSendMessage(EN_MRS_FW_MSG_NDM_KEY, HI_DRV_KEY_NUM_1, 0, 
            HI_DRV_KEY_PRESS_TIME_PRECISION_MS, 0);
    }
    
    return HI_ERR_SUCCESS;
}

HI_U32 Mrs_Misc_SendSignalStart(HI_VOID)
{
    // Generate signal file.
    HI_MDM_CreateTestSignalFile((HI_U8 *)the_Misc_RamAddr, MRS_SEND_SIGNAL_FILE_SIZE);
    HI_MDM_PHY_SetWorkCtrlFlag(PHY_WORK_CTRL_DISABLE);
    // Start send signal
    HI_MDM_PHY_StartSendSignal((HI_U8 *)the_Misc_RamAddr, MRS_SEND_SIGNAL_FILE_SIZE);

    return HI_ERR_SUCCESS;
}

HI_U32 Mrs_Misc_SendSignalToMode0(HI_VOID)
{
    (HI_VOID)HI_MDM_SetAfePgaGain(MRS_PHY_AFE_ANALOG_TX_DB_0);
    (HI_VOID)HI_MDM_LineDriverDisable();

    return HI_ERR_SUCCESS;
}

HI_U32 Mrs_Misc_SendSignalToMode1(HI_VOID)
{
    (HI_VOID)HI_MDM_SetAfePgaGain(MRS_PHY_AFE_ANALOG_TX_DB_12);
    (HI_VOID)HI_MDM_LineDriverEnable();

    return HI_ERR_SUCCESS;
}

HI_U32 Mrs_Misc_SendSignalToMode2(HI_VOID)
{
    (HI_VOID)HI_MDM_SetAfePgaGain(MRS_PHY_AFE_ANALOG_TX_DB_6);
    (HI_VOID)HI_MDM_LineDriverEnable();

    return HI_ERR_SUCCESS;
}

void 
Mrs_Misc_Reboot(void)
{
    /*释放资源*/
    mrs_chan_t *pchan = mrs_chan_ctx(CHAN_ETH_9G25);

    HI_MDM_Sleep(100);

    if (pchan->fd >= 0)
    {
        close(pchan->fd);

    }

    HI_MDM_Sleep(30);
    HI_MDM_Reboot(HI_SYS_REBOOT_CAUSE_USR0);
}

HI_U8 
Mrs_Misc_Rpt_Cnt(HI_U8 rpt_type)
{
    if ((rpt_type == MISC_RPT_REBOOT) || (rpt_type == MISC_RPT_TF) 
        || (rpt_type == MISC_RPT_WRTIME) || (rpt_type == MISC_RPT_NTB)) 
    {
        return MISC_RPT_CNT;
    } 
    else if ((rpt_type == MISC_RPT_RDTIME) || (rpt_type == MISC_RPT_BATUPG) 
        || (rpt_type == MISC_RPT_BATSTAT)) 
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void 
Mrs_Misc_Rpt_Clr(HI_U8 n)
{
    if (n >= MISC_RPT_NUM)
    {
        return;
    }
    
    the_misc_rpt.flag[n] = 0;
}

HI_U32 
Mrs_Misc_Rpt_Write(HI_U8 chantype, HI_U8 rpt_type, 
    HI_U8* buf, HI_U8 len)
{
    if ((chantype >= CHAN_MAX) || (rpt_type >= MISC_RPT_MAX) 
        || (len > MISC_RPT_BUF_LEN))
    {
        return HI_ERR_INVALID_TYPE;
    }

    if (the_misc_rpt.cur_wpos >= MISC_RPT_NUM)
    {
        the_misc_rpt.cur_wpos = 0;
    }

    mrs_misc_rptbuf_t* rptbuf = &the_misc_rpt.rptbuf[the_misc_rpt.cur_wpos];

    if (buf && (len > 0))
    {
        (hi_void)memcpy_s(rptbuf->buf, MISC_RPT_BUF_LEN, buf, len);
    }
    rptbuf->t = 0;
    rptbuf->len = len;
    rptbuf->rpt_cnt = 0;
    rptbuf->rpt_type = rpt_type;
    rptbuf->chantype = chantype;

    the_misc_rpt.flag[the_misc_rpt.cur_wpos ++] = 1;
    
    return HI_ERR_SUCCESS;
}

mrs_misc_rptbuf_t*  
Mrs_Misc_Rpt_Read(HI_U8 n)
{
    if (n >= MISC_RPT_NUM)
    {
        return NULL;
    }

    if (the_misc_rpt.flag[n] == 0)
    {
        return NULL;
    }

    mrs_misc_rptbuf_t* rptbuf = &the_misc_rpt.rptbuf[n];

    if ((rptbuf->chantype >= CHAN_MAX) || (rptbuf->rpt_type 
        >= MISC_RPT_MAX) || (rptbuf->len > MISC_RPT_BUF_LEN))
    {
        the_misc_rpt.flag[n] = 0;
        return NULL;
    }
    
    return rptbuf;
}

HI_U16 Mrs_Misc_Tei_Get(void)
{
    HI_U32 ret;
    HI_U32 tei;
    
    ret = HI_MDM_GetNdmTei(&tei);
    if (HI_ERR_SUCCESS != ret)
    {
        printf("[ERR][%s %u]\r\n",__func__,__LINE__);
        return (HI_U16)ERROR;
    }

    if ((tei < 1017) || (tei > 1020))   
    {
        printf("[ERR][%s %u]\r\n",__func__,__LINE__);
        return (HI_U16)ERROR;
    }
    
    return (HI_U16)tei;
}

HI_S16 Mrs_Misc_Tei_Set(HI_U16 tei)
{
    HI_U32 ret;

    if ((tei < 1017) || (tei > 1020))   
    {
        printf("[ERR][%s %u]\r\n",__func__,__LINE__);
        return ERROR;
    }
    
    ret = HI_MDM_SetNdmTei(tei);
    if (HI_ERR_SUCCESS != ret)
    {
        printf("[ERR][%s %u]\r\n",__func__,__LINE__);
        return ERROR;
    }

    return OK;
}

HI_U8 Mrs_Misc_Ip_Get(void)
{
    HI_U32 ret;
    HI_U8 ip_addr[HI_IPV4_ADDR_LEN] = {172,16,192,41};   /**< IP地址 */
    HI_SYS_ETH_CONFIG_S EthCfgInfo;

    ret = HI_MDM_GetEthConfig(&EthCfgInfo);
    if (HI_ERR_SUCCESS != ret)
    {
        printf("[ERR][%s %u]\r\n",__func__,__LINE__);
        return (HI_U8)ERROR;
    }

    ip_addr[2] = EthCfgInfo.ip_addr[2];

    if (0 != memcmp(ip_addr, EthCfgInfo.ip_addr, HI_IPV4_ADDR_LEN))
    {
        printf("[ERR][%s %u]\r\n",__func__,__LINE__);
        return (HI_U8)ERROR;
    }

    return ip_addr[2];
}

HI_S8 Mrs_Misc_Ip_Set(HI_U8 ip)
{
    HI_U32 ret;
    HI_U8 ip_addr[HI_IPV4_ADDR_LEN] = {172,16,192,41};   /**< IP地址 */
    HI_SYS_ETH_CONFIG_S EthCfgInfo;

    ret = HI_MDM_GetEthConfig(&EthCfgInfo);
    if (HI_ERR_SUCCESS != ret)
    {
        return ERROR;
    }

    ip_addr[2] = ip;

    if (0 != memcmp(ip_addr, EthCfgInfo.ip_addr, HI_IPV4_ADDR_LEN))
    {
        (hi_void)memcpy_s(EthCfgInfo.ip_addr, HI_IPV4_ADDR_LEN, ip_addr, HI_IPV4_ADDR_LEN);
        (hi_void)memcpy_s(EthCfgInfo.gateway, HI_IPV4_ADDR_LEN, ip_addr, HI_IPV4_ADDR_LEN);
        ret = HI_MDM_SetEthConfig(&EthCfgInfo);
        if (HI_ERR_SUCCESS != ret)
        {
            return ERROR;
        }
    }
    
    return OK;
}

/**
 ******************************************************************************
 * @brief    资源初始化
 * @retval  OK   : 成功
 * @retval  ERROR: 失败
 ******************************************************************************
 */
int mrs_misc_init(void)
{
    int i;
    HI_U32 ret = OK;
    HI_U32 Hi_Err;
    HI_CHAR szbuf[20];
    
    /*  信号量初始化*/
    for (i = 0;i < MISC_SEM_MAX;i ++)
    {
        (hi_void)sprintf_s(szbuf, sizeof(szbuf), "%s%d", "misc_sem", i);
        Hi_Err = HI_MDM_SEM_BCreate(&misc_semId[i], szbuf, HI_SEM_ONE);
        if (HI_ERR_SUCCESS != Hi_Err)
        {
            printf("[%s]: Sem[%d] create Err[%d]!!!\r\n", __func__, i, Hi_Err);
            ret = ERROR;
        }
    }

    /*  消息队列初始化*/
    for (i = 0;i < MISC_QUEUE_MAX;i ++)
    {
        (hi_void)sprintf_s(szbuf, sizeof(szbuf), "%s%d", "misc_queue", i);
        Hi_Err = HI_MDM_MSG_CreateQueue(&misc_queueId[i], szbuf, 8);
        if (HI_ERR_SUCCESS != Hi_Err)
        {
            printf("[%s]: Queue[%d] create Err[%d]!!!\r\n", __func__, i, Hi_Err);
            ret = ERROR;
        }
    }

    for (i = 0;i < BSO_TYPE_MAX;i ++)
    {
        misc_bsn_list[i].bsotype = BSO_TYPE_MAX;
    }

    /*  台区识别注册*/
    Hi_Err = HI_MDM_RegisterQueryNdm2NdmTfStatus(Mrs_Misc_QueryTfStatusCallBack);
    if (HI_ERR_SUCCESS != Hi_Err)
    {
        printf("tf Register Err[%d]\r\n", Hi_Err);
        ret = ERROR;
    }

    Mrs_Misc_Reboot_Report();
    return ret;
}

/*---------------------------------chan.c-------------------------------------*/
#endif
