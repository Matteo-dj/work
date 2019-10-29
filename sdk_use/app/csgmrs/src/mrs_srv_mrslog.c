
#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_fw_proto376_2.h"
#include "mrs_fw_proto645.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_proto376_2_frame.h"
#include "dfx_sys_sdm.h"
#include "mrs_fw_proto645.h"
#include "mrs_time.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_cco_broadcast.h"
#include "mrs_srv_list.h"
#include "mrs_srv_cco_event.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_res.h"
#include "mrs_srv_mrslog.h"
#include "dfx_app.h"
#include "mrs_srv_cco_archives.h"

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)&&defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))
#include "mrs_fw_proto_csg.h"
#endif

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(MRS_LOG_STA_SWITCH)) && defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)

//#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
//#define MRS_LOG_DEBUG
//#endif

#define DAYFREEZE_TIME_DI (0x05060001 + 0x33333333)
#define DAYFREEZE_DATA_DI (0x05060101 + 0x33333333)

#define DAYFREEZE_MAX_REC_DAY (60)  // STA端日冻结最大记录天数

#define DAYFREEZE_TIME_READING_FLG_BIT   0   // 日冻结时间抄读中标志位
#define DAYFREEZE_DATA_READING_FLG_BIT   1   // 日冻结数据抄读中标志位
#define DAYFREEZE_TIME_READ_END_FLG_BIT  2   // 日冻结时间已抄读标志位
#define DAYFREEZE_DATA_READ_END_FLG_BIT  3   // 日冻结数据已抄读标志位

#define DAYFREEZE_TIME_READ_SUCC_FLG_BIT 0   // 日冻结时间抄读成功标志位
#define DAYFREEZE_DATA_READ_SUCC_FLG_BIT 1   // 日冻结数据抄读成功标志位

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#define MRS_LOG_METER_FILE_CHANGE   (0x8000)
#endif

typedef struct
{
    HI_U32 sysTime;
    HI_U8  meter[HI_METER_ADDR_LEN];
    HI_U8  causeSet;
    HI_U8  causeLatest; 
    HI_U8  frame[HI_SDM_ENG_FRAME_LEN_MAX]; // 31
/* BEGIN: liuxipeng/KF54842 2014/2/20   问题单号: DTS2014022003115 */
    HI_U8  ucBroadcast;       // PLC广播标志:使用广播地址发送PLC帧时将此标志置"HI_TRUE"否则置"HI_FALSE"
/* END  : liuxipeng/KF54842 2014/2/20   问题单号: DTS2014022003115 */
    HI_U32 dayFreezeRead;
    HI_U32 dayFreezeSucc;
    HI_U32 mrTotal;  
    HI_U32 mrSucc;
    HI_U8  dayFreezeRmFlg;    // 日冻结数据抄读进行中标志(D0:表示日冻结时间, D1表示日冻结数据) 每完成一次PLC抄读后清除
                              // 日冻结数据已抄标志(当次有效)(D2:表示日冻结时间, D3表示日冻结数据) 每完成一次日冻结抄读后清除
    HI_U8  dayFreezeSuccFlg;  // 日冻结数据抄读成功标志(D0:表示日冻结时间, D1表示日冻结数据) 每成功抄读一次日冻结数据(时标+数据)后清除
    HI_U16 bitMap;            // 日冻结数据在多帧645缓存中的位置 (0:当前未抄读日冻结数据 其它:当前正在抄读日冻结数据)
} MRS_SRV_MRSLOG_METER_ITEM;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
typedef struct
{
    HI_BOOL valid;
    HI_U8   retry;
    HI_U8   retryMax;
    HI_BOOL reqFlg;
    HI_U8   firstgettimeStatus; //0:未获取, 1:已获取, 2:正在获取
    HI_U8   reserved[3];
} MRS_SRV_CCO_MRSLOG_CLOCK;
#endif

typedef struct
{
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    HI_U32 lastTime;
#endif
    HI_U32 dayFreezeReadTotal;
    HI_U32 dayFreezeReadSucc;

    HI_U16 dayFreezeMeterTotal;
    HI_U16 dayFreezeMeterSucc;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    HI_SDM_ENG_MRS_MR_STATS_S xrStats;
    HI_SDM_ENG_MRS_MR_STATS_S lrprStats;

    HI_U16 lowRatioNv;
#endif

    HI_U16 meterCount;
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    MRS_SRV_MRSLOG_METER_ITEM item[PRODUCT_CFG_MRS_MAX_NODE_NUM];
#else
    HI_U16 pad2;
    MRS_SRV_MRSLOG_METER_ITEM item[PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM];
#endif

} MRS_SRV_MRSLOG_CONTEXT;

MRS_SRV_MRSLOG_CONTEXT g_mrsLogCtx = {0};

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
MRS_SRV_CCO_MRSLOG_CLOCK g_clock = {0};
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#define MRS_LOG_MAX_ITEM_NUM PRODUCT_CFG_MRS_MAX_NODE_NUM
#else
#define MRS_LOG_MAX_ITEM_NUM PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#define MRS_FIRST_ACQUIRE_TIME_IDLE     (0)
#define MRS_FIRST_ACQUIRE_TIME_END      (1)
#define MRS_FIRST_ACQUIRE_TIME_DOING    (2)
#endif

#define MRS_LOG_REQ_CLOCK_MAX_RETRY (3)
#define MRS_LOG_TIME_2000_1_1       (946684800)  // 2000-01-01 0:00:00
#define MRS_LOG_HOURS_PER_DAY       (24)
#define MRS_LOG_MINS_PER_HOUR       (60)
#define MRS_LOG_SECS_PER_MIN        (60)
#define MRS_LOG_SECS_PER_HOUR       (MRS_LOG_MINS_PER_HOUR * MRS_LOG_SECS_PER_MIN)
#define MRS_LOG_MS_PER_SEC          (1000)
#define MRS_LOG_MS_PER_MIN          (MRS_LOG_SECS_PER_MIN * MRS_LOG_MS_PER_SEC)
#define MRS_LOG_LRPR_MAX_MR_COUNT   (65535)
#define MRS_LOG_DIFF_THRESHOLD_MS   (300)        // 5min
#define MRS_LOG_DIFF_THRESHOLD_MIN  (MRS_LOG_DIFF_THRESHOLD_MS/MRS_LOG_SECS_PER_MIN)
#define MRS_LOG_REQ_CLOCK_HH        (22)
#define MRS_LOG_REQ_CLOCK_MM_MIN    (00)
#define MRS_LOG_REQ_CLOCK_MM_MAX    (02)

#define GetMrsLogCtx()              ((MRS_SRV_MRSLOG_CONTEXT *)&g_mrsLogCtx)

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#define GetMrsLogClock()            ((MRS_SRV_CCO_MRSLOG_CLOCK *)&g_clock)

#define MrsLogIsClockValid()        (GetMrsLogClock()->valid)
#define MrsLogClockSetValid(x)      do { GetMrsLogClock()->valid = (x); } while (0)
#define MrsLogClockRetryLimit()     (GetMrsLogClock()->retry < GetMrsLogClock()->retryMax)
#define MrsLogClockRetryInc()       do { GetMrsLogClock()->retry++; } while (0)
#define MrsFristGetTimeStatus()         (GetMrsLogClock()->firstgettimeStatus)
#endif

#define FRAME_TYPE_DAYFREEZE_TIME (1)    // 取值必须为1
#define FRAME_TYPE_DAYFREEZE_DATA (2)    // 取值必须为2
#define FRAME_TYPE_DENY           (3)
#define FRAME_TYPE_OTHER          (0)

HI_VOID initMrsLogCtx(HI_VOID);

HI_VOID initMrsLogCtx(HI_VOID)
{
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    MRS_SRV_MRSLOG_CONTEXT * ctx = GetMrsLogCtx();
    HI_U16 ratioNv = ctx->lowRatioNv;
    HI_U32 lastTime = ctx->lastTime;
#endif

    (hi_void)memset_s(GetMrsLogCtx(), sizeof(MRS_SRV_MRSLOG_CONTEXT), 0, sizeof(MRS_SRV_MRSLOG_CONTEXT));

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    ctx->lowRatioNv = ratioNv;
    ctx->lastTime = lastTime;
#endif
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#define initMrsLogClock() \
    do { \
        MRS_SRV_CCO_MRSLOG_CLOCK * clock = GetMrsLogClock(); \
        (hi_void)memset_s(clock, sizeof(MRS_SRV_CCO_MRSLOG_CLOCK), 0, sizeof(MRS_SRV_CCO_MRSLOG_CLOCK)); \
        clock->retryMax = MRS_LOG_REQ_CLOCK_MAX_RETRY; \
        clock->firstgettimeStatus = MRS_FIRST_ACQUIRE_TIME_IDLE; \
    } while (0)
#endif


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_U32 mrsCheckProtoVer(HI_VOID);

HI_U32 mrsLogSendAfn14F2(HI_VOID);

HI_U32 mrsCcoSetClockPre(HI_SYS_CALENDAR_TIME_S *tm);

HI_U32 mrsCcoRequestClock(HI_VOID);

HI_U32 mrsCcoRequestClockStartup(HI_VOID);

HI_U32 mrsCcoRequestClockRuntime(HI_VOID);

HI_VOID mrsLogClearLrprStats(HI_VOID);

/* BEGIN: 问题单号: DTS2014012700106 Added by liuxipeng/lkf54842 2014/01/27 */
HI_VOID mrsLogClearTxRxCount(MRS_SRV_MRSLOG_CONTEXT *ctx, HI_U32 tmHour);
/* END  : 问题单号: DTS2014012700106 Added by liuxipeng/lkf54842 2014/01/27 */

HI_VOID mrsLogXrToCache(MRS_SRV_MRSLOG_CONTEXT *ctx);

HI_VOID mrsLogLrprToCache(MRS_SRV_MRSLOG_CONTEXT *ctx, HI_U32 tmHour);

HI_VOID mrsLogLowRatioToCache(MRS_SRV_MRSLOG_CONTEXT *ctx, HI_U32 tmHour);

HI_VOID mrsLogDayFreezeToCache(MRS_SRV_MRSLOG_CONTEXT *ctx);

HI_VOID mrsLogToCache(HI_U32 tmHour);

HI_BOOL mrsCheckDayFreezeTime(HI_U8 bcdTime[6], HI_SYS_CALENDAR_TIME_S* sysTime);

HI_U32 mrsLogGetSaveHour(HI_SYS_CALENDAR_TIME_S *tm);

HI_U32 mrsLogClockTimerStart(HI_U32 peroid);

HI_U32 mrsLogClockTimerStop(HI_VOID);

HI_BOOL mrsLogNeedRequestClock(HI_VOID);
#endif

HI_PRV HI_U8 mrsLogGetFrameType(HI_PBYTE pData);

HI_PRV HI_U32 mrsLogInsertNode(HI_IN HI_U8 * addr, HI_OUT HI_S16 * index);

HI_PRV HI_U32 mrsLogFindNode(HI_IN HI_U8 * addr, HI_OUT HI_S16 * index);


HI_U32 mrsInitMrsLogModule(HI_VOID)
{
    initMrsLogCtx();

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    initMrsLogClock();
    mrsCcoRequestClock();
#endif

    return HI_ERR_SUCCESS;
}


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_U32 mrsLogClockTimerStart(HI_U32 peroid)
{
    return MRS_StartTimer(MRS_CCO_TIMER_ACQUIRE_CLOCK, peroid, HI_SYS_TIMER_ONESHOT);
}


HI_U32 mrsLogClockTimerStop(HI_VOID)
{
    return MRS_StopTimer(MRS_CCO_TIMER_ACQUIRE_CLOCK);
}


HI_U32 mrsCcoRequestClock(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    ret = mrsCheckProtoVer();
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }
#endif
    mrsLogClockTimerStart(MRS_CCO_TIME_STARTUP_CLOCK);

    return ret;
}


HI_U32 mrsCheckProtoVer(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU * ccoCtx = mrsCcoGetContext();

    if (ccoCtx->ucProtoVer == PROTO_VER_3762_2009)
    {
        return HI_ERR_NOT_SUPPORT;
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsLogSendAfn14F2(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    HI_PBYTE buf = HI_NULL;
    HI_U16 bufLen = 0;
    
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    MRS_CSG_IND_FRAME_STRU stFrame = {0};
#else
    PROTO376_2_FRAME_AFN_AND_DATA_STRU afnData = {0};
#endif
        
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
    // 江苏版本，不回复，清buffer
    if (mrsToolIsBbu()
        && (!mrsIsSimuChlOpened()))
    {
        return HI_ERR_SKIP;
    }
#endif 

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    (hi_void)memset_s(&stFrame, sizeof(stFrame), 0, sizeof(stFrame));
    stFrame.ucAfn = MRS_AFN(0x06);
    stFrame.ulDI = 0xE8060601;
    stFrame.ucPrm = 1;
    stFrame.ucSeq =  ++(mrsCcoGetContext()->ucSeq);
    ret = mrsCreateCsgFrame(&stFrame, &buf, &bufLen);
#else
    (hi_void)memset_s(&afnData, sizeof(afnData), 0, sizeof(afnData));
    afnData.ucAFN = MRS_AFN(0x14);
    afnData.ucDT[0] = MRS_AFN_FN_LO(2);
    afnData.ucDT[1] = MRS_AFN_FN_HI(2);
    ret = mrsCreate3762UpFrame(&afnData, &buf, &bufLen, HI_NULL);
#endif

    if (HI_ERR_SUCCESS == ret)
    {
        ret = MRS_SendMrData(buf, bufLen, HI_DMS_CHL_UART_PORT_APP);
        mrsDfxGetTimeTotalCnt();
    }

    mrsToolsFree(buf);

    return ret;
}


HI_U32 mrsCcoSetClock(HI_U8 clockBuf[6])
{
    HI_SYS_CALENDAR_TIME_S tm;
    HI_U32 ret = HI_ERR_SUCCESS;

    if (!clockBuf)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    mrsLogClockTimerStop();

    (hi_void)memset_s(&tm, sizeof(tm), 0, sizeof(tm));
    MRS_BCD2Time(clockBuf, &tm);

    ret = mrsCcoSetClockPre(&tm);
    if (HI_ERR_SUCCESS == ret)
    {
        HI_MDM_SetCalendarTime(&tm);
        MrsLogClockSetValid(HI_TRUE);
        mrsGetTimeEndPro();
        mrsDfxGetTimeSucc(HI_TRUE);
        mrsDfxPowerOnGetTime(clockBuf);
        mrsStartMrsLogTimer();
    }

    if (MrsLogIsClockValid())
    {
        mrsLogClockTimerStart(MRS_CCO_TIME_CHECK_CLOCK);
    }
    else if (MrsLogClockRetryLimit())
    {
        mrsLogClockTimerStart(MRS_CCO_TIME_ACQUIRE_CLOCK);
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCcoSetClockPre(HI_SYS_CALENDAR_TIME_S *tm)
{
    HI_BOOL writeFlag = HI_FALSE;
    HI_BOOL enableSwitch = HI_FALSE;
    HI_U32 tmHour = 0;

    if (!MrsLogIsClockValid())
    {
        return HI_ERR_SUCCESS;
    }

    do
    {
        HI_U32 timeCur = 0;
        HI_U32 timeInput = 0;
        HI_U32 timeDiff = 0;
        HI_U32 lastTime = 0;

        HI_MDM_MakeCalendarToSecond(tm, &timeInput);

        timeCur = HI_MDM_GetRealTime();

        timeDiff = MRS_ABS_DIFF(timeInput, timeCur);
        if (timeDiff > MRS_LOG_DIFF_THRESHOLD_MS)
        {
            HI_SYS_CALENDAR_TIME_S tmpTm;
            (hi_void)memset_s(&tmpTm, sizeof(tmpTm), 0, sizeof(tmpTm));
            HI_MDM_MakeSecondToCalendar(timeCur, &tmpTm);
            tmHour = (HI_U32)tmpTm.hour;
            writeFlag = HI_TRUE;
            enableSwitch = HI_TRUE;
            break;
        }

        lastTime = GetMrsLogCtx()->lastTime;
        if ((MRS_ABS_DIFF(timeInput, lastTime) <= MRS_LOG_DIFF_THRESHOLD_MS)
            || (MRS_ABS_DIFF(timeCur, lastTime) <= MRS_LOG_DIFF_THRESHOLD_MS))
        {
            // 5分钟之内刚存储过，此次不存
            break;
        }

        if ((tm->min <= MRS_LOG_DIFF_THRESHOLD_MIN) 
            || (tm->min >= (MRS_LOG_MINS_PER_HOUR - MRS_LOG_DIFF_THRESHOLD_MIN)))
        {
            writeFlag = HI_TRUE;
            tmHour = mrsLogGetSaveHour(tm);
            enableSwitch = (tmHour == 0) ? HI_TRUE : HI_FALSE;
        }
    } while (0);

    if (writeFlag)
    {
        mrsLogToCache(tmHour);
        HI_MDM_WriteMrsLog(enableSwitch);
        if (enableSwitch)
        {
            initMrsLogCtx();
        }
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsLogGetSaveHour(HI_SYS_CALENDAR_TIME_S *tm)
{
    if (tm->min >= (MRS_LOG_MINS_PER_HOUR - MRS_LOG_DIFF_THRESHOLD_MIN))
    {
        return ((HI_U32)tm->hour + 1) % MRS_LOG_HOURS_PER_DAY;
    }

    return (HI_U32)tm->hour;
}


HI_U32 mrsCcoRequestClockTimeout(HI_VOID)
{
    if (!MrsLogIsClockValid())
    {
        return mrsCcoRequestClockStartup();
    }
    
    return mrsCcoRequestClockRuntime();
}

HI_VOID mrsGetTimeEndPro(HI_VOID)
{
    HI_U32 status = mrsCcoGetState();

    if ((MRS_FIRST_ACQUIRE_TIME_DOING == MrsFristGetTimeStatus())
        && (status & MRS_CCO_STATUS_NORMAL))
    {
        // 延时MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE时间后重新激活读表模块
        MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER, MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE * 3, HI_SYS_TIMER_ONESHOT);
    }
    
    mrsSetFristGetTimeFlg(MRS_FIRST_ACQUIRE_TIME_END);
}

HI_U32 mrsCcoRequestClockStartup(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    if (!MrsLogClockRetryLimit())
    {
        MrsLogClockSetValid(HI_FALSE);
        mrsGetTimeEndPro();
        mrsDfxGetTimeFailCnt();

        return HI_ERR_FAILURE;
    }

    if (GetMrsLogClock()->retry)
    {
        mrsDfxGetTimeFailCnt();
    }
    
    ret = mrsLogSendAfn14F2();
    if (HI_ERR_SUCCESS == ret)
    {
        MrsLogClockRetryInc();
    }

    mrsLogClockTimerStart(MRS_CCO_TIME_ACQUIRE_CLOCK);

    return ret;
}

HI_U32 mrsCcoRequestClockRuntime(HI_VOID)
{
    if (mrsLogNeedRequestClock())
    {
        if (!GetMrsLogClock()->reqFlg)
        {
            mrsLogSendAfn14F2();
            GetMrsLogClock()->reqFlg = HI_TRUE;
        }
    }
    else
    {
        GetMrsLogClock()->reqFlg = HI_FALSE;
    }

    mrsLogClockTimerStart(MRS_CCO_TIME_CHECK_CLOCK);

    return HI_ERR_SUCCESS;
}

HI_BOOL mrsLogNeedRequestClock(HI_VOID)
{
    HI_SYS_CALENDAR_TIME_S tm = { 0 };

    if (HI_ERR_SUCCESS != HI_MDM_GetCalendarTime(&tm))
    {
        return HI_FALSE;
    }

    return (HI_BOOL)((tm.hour == MRS_LOG_REQ_CLOCK_HH)
        && (tm.min <= MRS_LOG_REQ_CLOCK_MM_MAX));
}


HI_U32 mrsStartMrsLogTimer(HI_VOID)
{
    HI_U32 time = HI_MDM_GetRealTime();
    HI_U32 peroid = time % MRS_LOG_SECS_PER_HOUR;
    peroid = MRS_LOG_SECS_PER_HOUR - peroid;

    if (peroid <= MRS_LOG_DIFF_THRESHOLD_MS)
    {
        peroid += MRS_LOG_SECS_PER_HOUR;
    }

    MRS_StartTimer(MRS_CCO_TIMER_MRS_LOG, peroid * 1000, HI_SYS_TIMER_ONESHOT);

    return HI_ERR_SUCCESS;
}


HI_U32 mrsLogTimerProc(HI_VOID)
{
    HI_SYS_CALENDAR_TIME_S tm = { 0 };
    HI_U32 tmHour = 0;
    
    HI_MDM_GetCalendarTime(&tm);
    tmHour = mrsLogGetSaveHour(&tm);
    mrsLogToCache(tmHour);

    HI_MDM_WriteMrsLog((HI_BOOL)(0 == tmHour));

    if (0 == tmHour)
    {
        initMrsLogCtx();        
    }
    
    mrsStartMrsLogTimer();

    return HI_ERR_SUCCESS;
}


//透抄抄读数据统计 存储周期24h
HI_VOID mrsLogXrComRx(HI_VOID)
{
    HI_SDM_ENG_MRS_MR_STATS_S *xr = &(GetMrsLogCtx()->xrStats);

    xr->statsType = SDM_MRS_MR_STATS_TYPE_XR;
    xr->comRx++;
}


HI_VOID mrsLogXrComTx(HI_VOID)
{
    HI_SDM_ENG_MRS_MR_STATS_S *xr = &(GetMrsLogCtx()->xrStats);

    xr->statsType = SDM_MRS_MR_STATS_TYPE_XR;
    xr->comTx++;
}


HI_VOID mrsLogXrPlcRx(HI_VOID)
{
    HI_SDM_ENG_MRS_MR_STATS_S *xr = &(GetMrsLogCtx()->xrStats);

    xr->statsType = SDM_MRS_MR_STATS_TYPE_XR;
    xr->plcRx++;
}


HI_VOID mrsLogXrPlcTx(HI_VOID)
{
    HI_SDM_ENG_MRS_MR_STATS_S *xr = &(GetMrsLogCtx()->xrStats);
    
    xr->statsType = SDM_MRS_MR_STATS_TYPE_XR;
    xr->plcTx++;
}


//轮抄/并发抄抄读数据统计，存储周期1h
HI_VOID mrsLogLrprComRx(HI_SDM_ENG_MRS_MR_STATS_TYPE_E statsType)
{
    HI_SDM_ENG_MRS_MR_STATS_S *lrpr = &(GetMrsLogCtx()->lrprStats);

    if (lrpr->statsType != statsType)
    {
        mrsLogClearLrprStats();
    }
    
    lrpr->statsType = statsType;
    if (lrpr->comRx < MRS_LOG_LRPR_MAX_MR_COUNT)
    {
        lrpr->comRx++;
    }
}


HI_VOID mrsLogLrprComTx(HI_SDM_ENG_MRS_MR_STATS_TYPE_E statsType)
{
    HI_SDM_ENG_MRS_MR_STATS_S *lrpr = &(GetMrsLogCtx()->lrprStats);

    if (lrpr->statsType != statsType)
    {
        mrsLogClearLrprStats();
    }
    
    lrpr->statsType = statsType;
    if (lrpr->comTx < MRS_LOG_LRPR_MAX_MR_COUNT)
    {
        lrpr->comTx++;
    }
}


HI_VOID mrsLogLrprPlcRx(HI_SDM_ENG_MRS_MR_STATS_TYPE_E statsType)
{
    HI_SDM_ENG_MRS_MR_STATS_S *lrpr = &(GetMrsLogCtx()->lrprStats);

    if (lrpr->statsType != statsType)
    {
       mrsLogClearLrprStats();
    }
    
    lrpr->statsType = statsType;
    if (lrpr->plcRx < MRS_LOG_LRPR_MAX_MR_COUNT)
    {
        lrpr->plcRx++;    
    }
}


HI_VOID mrsLogLrprPlcTx(HI_SDM_ENG_MRS_MR_STATS_TYPE_E statsType)
{
    HI_SDM_ENG_MRS_MR_STATS_S *lrpr = &(GetMrsLogCtx()->lrprStats);

    if (lrpr->statsType != statsType)
    {
        mrsLogClearLrprStats();
    }
    
    lrpr->statsType = statsType;
    if (lrpr->plcTx < MRS_LOG_LRPR_MAX_MR_COUNT)
    {
        lrpr->plcTx++;
    }
}


//清空内存中轮抄/并发抄读统计数据
HI_VOID mrsLogClearLrprStats(HI_VOID)
{
    (hi_void)memset_s(&(GetMrsLogCtx()->lrprStats), sizeof(GetMrsLogCtx()->lrprStats), 0, sizeof(GetMrsLogCtx()->lrprStats));
}

/* BEGIN: 问题单号: DTS2014012700106 Added by liuxipeng/lkf54842 2014/01/27 */
//清空内存中每四小时统计的抄读次数
HI_VOID mrsLogClearTxRxCount(MRS_SRV_MRSLOG_CONTEXT *ctx, HI_U32 tmHour)
{
    if(0 == (tmHour % 4))
    {
        HI_U16 i = 0;
        
        for(i = 0; i < ctx->meterCount; i++)
        {
            ctx->item[i].mrTotal = 0;
            ctx->item[i].mrSucc = 0;
        }
    }

    return;
}
/* END  : 问题单号: DTS2014012700106 Added by liuxipeng/lkf54842 2014/01/27 */

//上下文中数据存入缓存
HI_VOID mrsLogToCache(HI_U32 tmHour)
{
    MRS_SRV_MRSLOG_CONTEXT *ctx = GetMrsLogCtx();
    
    if (tmHour >= MRS_LOG_HOURS_PER_DAY)
    {
        return;
    }

    //轮抄/并发抄抄读数据存入缓存，存储周期1小时
    // TODO: tm_hour 0~23
    mrsLogLrprToCache(ctx, tmHour);
    mrsLogClearLrprStats();
    
    //低于抄通率门限电表统计，存储周期4小时
    mrsLogLowRatioToCache(ctx, tmHour);
/* BEGIN: 问题单号: DTS2014012700106 Added by liuxipeng/lkf54842 2014/01/27 */    
    mrsLogClearTxRxCount(ctx, tmHour);
/* END  : 问题单号: DTS2014012700106 Added by liuxipeng/lkf54842 2014/01/27 */
    
    //透抄抄读数据统计，存储周期24小时
    mrsLogXrToCache(ctx);

    // 日冻结数据存入缓存，存储周期24小时
    mrsLogDayFreezeToCache(ctx);
}


//轮抄/并发抄抄读数据存入缓存，存储周期1小时
HI_VOID mrsLogLrprToCache(MRS_SRV_MRSLOG_CONTEXT *ctx, HI_U32 tmHour)
{
    HI_SDM_ENG_MRS_MR_STATS_S *stats = HI_SDM_MRS_LOG_GPTR->lrprStats.stats;

    if (tmHour == 0)
    {
        stats += HI_SDM_ENG_LRPR_IN_DAY - 1;
    }
    else 
    {
        stats += tmHour - 1;
    }

    (hi_void)memcpy_s(stats, sizeof(HI_SDM_ENG_MRS_MR_STATS_S), &(ctx->lrprStats), sizeof(ctx->lrprStats));
    stats->valid = HI_TRUE;

    stats->meterFileCnt = mrsSrvArchivesMeterNum();
    if (mrsSrvArchivesIsChanged())
    {
        stats->meterFileCnt |= MRS_LOG_METER_FILE_CHANGE;
        mrsSrvArchivesChangedFlagClear();
    }
}

//透抄抄读数据统计，存储周期24小时
HI_VOID mrsLogXrToCache(MRS_SRV_MRSLOG_CONTEXT *ctx)
{
    ctx->xrStats.valid = HI_TRUE;
    (hi_void)memcpy_s(&(HI_SDM_MRS_LOG_GPTR->xrStats), sizeof(HI_SDM_MRS_LOG_GPTR->xrStats), &(ctx->xrStats), sizeof(ctx->xrStats));
}

//日冻结数据存入缓存，存储周期24小时
HI_VOID mrsLogDayFreezeToCache(MRS_SRV_MRSLOG_CONTEXT *ctx)
{
    HI_U16 itemNum = 0;
    HI_U16 i = 0;

    HI_U32 succ = 0;
    HI_U32 total = 0;

    (hi_void)memset_s(&(HI_SDM_MRS_LOG_GPTR->dayFreeze), sizeof(HI_SDM_MRS_LOG_GPTR->dayFreeze), 0, sizeof(HI_SDM_MRS_LOG_GPTR->dayFreeze));

    //日冻结数据存入缓存，存储周期24小时
    ctx->lastTime = HI_MDM_GetRealTime();

    succ = ctx->dayFreezeMeterSucc;
    total = ctx->dayFreezeMeterTotal;
/* BEGIN: 问题单号: DTS2014012700106 Added by liuxipeng/lkf54842 2014/01/27 */    
    HI_SDM_MRS_LOG_GPTR->dayFreeze.ratio.meter_ratio = (HI_U16) (total? (succ * 10000 / total) : 10000);
/* END  : 问题单号: DTS2014012700106 Added by liuxipeng/lkf54842 2014/01/27 */

    succ = ctx->dayFreezeReadSucc;
    total = ctx->dayFreezeReadTotal;
/* BEGIN: 问题单号: DTS2014012700106 Added by liuxipeng/lkf54842 2014/01/27 */    
    HI_SDM_MRS_LOG_GPTR->dayFreeze.ratio.total_ratio = (HI_U16)(total? (succ * 10000 / total) : 10000);
/* END  : 问题单号: DTS2014012700106 Added by liuxipeng/lkf54842 2014/01/27 */

    HI_SDM_MRS_LOG_GPTR->dayFreeze.failCount = ctx->dayFreezeMeterTotal - ctx->dayFreezeMeterSucc;
    HI_SDM_MRS_LOG_GPTR->dayFreeze.sysTime = ctx->lastTime;

    for (i = 0; (i < ctx->meterCount) && (HI_SDM_MRS_LOG_GPTR->dayFreeze.failCount != 0); i++)
    {
        /* BEGIN:PN:DTS2014011609254    MODIFY\ADD  by chenli/00238131 at 2014/1/16 */
        if ((itemNum > HI_SDM_MRS_LOG_GPTR->dayFreeze.failCount) || (itemNum >= HI_SDM_ENG_METER_REC_MAX))
        {
            break;
        }
        /* END: PN:DTS2014011609254    MODIFY\ADD  by chenli/00238131  at 2014/1/16 */
        if ((ctx->item[i].dayFreezeSucc == 0) && (ctx->item[i].causeSet != 0))
        {
            HI_SDM_ENG_DAY_FREEZE_FAIL_ITEM * item =(HI_SDM_ENG_DAY_FREEZE_FAIL_ITEM *) &(HI_SDM_MRS_LOG_GPTR->dayFreeze.item[itemNum]);

            item->causeLatest = ctx->item[i].causeLatest;
            item->causeSet    = ctx->item[i].causeSet;
            item->sysTime     = ctx->item[i].sysTime;
            (hi_void)memcpy_s(item->frame, HI_SDM_ENG_FRAME_LEN_MAX, ctx->item[i].frame, HI_SDM_ENG_FRAME_LEN_MAX);
            (hi_void)memcpy_s(item->meter, HI_SDM_ENG_METER_ADDR_LEN, ctx->item[i].meter, HI_SDM_ENG_METER_ADDR_LEN);

            itemNum++;                
        }
    }

/* BEGIN: PN: DTS2014012604132 MODIFY\ADD\DEL by cuiate/00233580 at 2014/1/26 */
/* END:   PN: DTS2014012604132 MODIFY\ADD\DEL by cuiate/00233580 at 2014/1/26 */
}


//低于抄通率门限电表数据，存储周期4小时
HI_VOID mrsLogLowRatioToCache(MRS_SRV_MRSLOG_CONTEXT *ctx, HI_U32 tmHour)
{
    HI_U16 lowRatioNum = 0;
    HI_U16 i = 0;
    HI_U8 partNum = 0;
    HI_SDM_ENG_MRS_LOW_RATIO_PART_S *part = HI_NULL;

    //遍历查找抄通率低的电表日志按时间缓存
    partNum = ((0 == tmHour) ? (HI_SDM_ENG_LOW_RATIO_IN_DAY - 1) : (HI_U8)((tmHour - 1) / 4));
    
    part = &(HI_SDM_MRS_LOG_GPTR->lowRatioStats.part[partNum]);
    (hi_void)memset_s(part, sizeof(HI_SDM_ENG_MRS_LOW_RATIO_PART_S), 0, sizeof(HI_SDM_ENG_MRS_LOW_RATIO_PART_S));
    for (i = 0; i < ctx->meterCount; i++)
    {
        HI_SDM_ENG_MRS_LOW_RATIO_ITEM_S *item = HI_NULL;

    /* BEGIN: 问题单号: DTS2014012700106 Added by liuxipeng/lkf54842 2014/01/27 */
        if(0 == ctx->item[i].mrTotal)
        {
            // 除零保护
            continue;
        }
    /* END  : 问题单号: DTS2014012700106 Added by liuxipeng/lkf54842 2014/01/27 */
    
        if (((ctx->item[i].mrSucc * 10000) / (ctx->item[i].mrTotal)) >= ctx->lowRatioNv)
        {
            continue;
        }

        lowRatioNum = part->meterCount++;
        if (lowRatioNum >= HI_SDM_ENG_METER_REC_MAX)
        {
            continue;
        }

        item = &(part->item[lowRatioNum]);
        (hi_void)memcpy_s(item->meter, HI_SDM_ENG_METER_ADDR_LEN, ctx->item[i].meter, HI_SDM_ENG_METER_ADDR_LEN);
        item->ratio = (HI_U8)(ctx->item[i].mrSucc * 100 / ctx->item[i].mrTotal);
        part->valid = HI_TRUE;
    }
}


HI_VOID mrsLogSetLowRatioThreshold(HI_U16 lowRatio)
{
    if ((lowRatio == 0) || (lowRatio > 10000))
    {
        GetMrsLogCtx()->lowRatioNv = 9500;
    }
    else
    {
        GetMrsLogCtx()->lowRatioNv = lowRatio;
    }
}
#endif


// 获取645帧类型
HI_U8 mrsLogGetFrameType(HI_PBYTE pData)
{
    HI_U32 di = 0;
        
    if(pData[MRS_645_FRAME_CTRL_OFFSET] & MRS_645_FRAME_CONTROL_DENY_MASK)
    {
        return FRAME_TYPE_DENY;
    }
    
    if((pData[MRS_645_FRAME_CTRL_OFFSET] & MRS_645_FRAME_CONTROL_MASK) != MRS_645_FRAME_CONTROL_READ_DATA)
    {
        return FRAME_TYPE_OTHER;
    }

    (hi_void)memcpy_s(&di, sizeof(di), pData + MRS_645_FRAME_DATA_OFFSET, sizeof(di));

    if(DAYFREEZE_TIME_DI == di)
    {
        return FRAME_TYPE_DAYFREEZE_TIME;
    }
    else if(DAYFREEZE_DATA_DI == di)
    {
        return FRAME_TYPE_DAYFREEZE_DATA;        
    }
    else
    {
        return FRAME_TYPE_OTHER;
    }
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
// 检查日冻结时间是否与当前系统时间一致
HI_BOOL mrsCheckDayFreezeTime(HI_U8 bcdTime[6], HI_SYS_CALENDAR_TIME_S* sysTime)
{
    if(((BCD2INT(bcdTime[5]) + MRS_YEAR_BASE) != (HI_S32)sysTime->year) // 检查年份
        || (BCD2INT(bcdTime[4]) != (HI_S32)sysTime->month)              // 检查月份
        || (BCD2INT(bcdTime[3]) != (HI_S32)sysTime->day))               // 检查日期
    {
        return HI_FALSE;
    }
    else
    {
        return HI_TRUE;
    }
}
#endif

HI_U32 mrsLogInsertNode(HI_IN HI_U8 * addr, HI_OUT HI_S16 * index)
{
    HI_S16 i = 0;
    MRS_SRV_MRSLOG_CONTEXT *logCtx = GetMrsLogCtx();
/* BEGIN: PN: DTS2015042907341 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/13 */
    HI_U8 aucAddr[HI_METER_ADDR_LEN] = {0};

    mrsMeterAddrPreProc(aucAddr, addr, MRS_PROTO645_VER_1997);
    for(i = 0; i < logCtx->meterCount; i++)
    {
        if(mrsToolsMemEq(logCtx->item[i].meter, aucAddr, MRS_645_FRAME_METERADD_LEN))
        {
            *index = i;
            return HI_ERR_SUCCESS;
        }
    }

    if(logCtx->meterCount < MRS_LOG_MAX_ITEM_NUM)
    {
        (hi_void)memcpy_s(logCtx->item[logCtx->meterCount].meter, MRS_645_FRAME_METERADD_LEN, aucAddr, MRS_645_FRAME_METERADD_LEN);
        *index = (HI_S16)logCtx->meterCount;
        logCtx->meterCount++;
        return HI_ERR_SUCCESS;
    }
    else
    {
        *index = -1;
        return HI_ERR_FULL;
    }
/* END:   PN: DTS2015042907341 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/13 */
}

HI_U32 mrsLogFindNode(HI_IN HI_U8 * addr, HI_OUT HI_S16 * index)
{
    HI_S16 i = 0;
    MRS_SRV_MRSLOG_CONTEXT *logCtx = GetMrsLogCtx();
/* BEGIN: PN: DTS2015042907341 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/13 */
    HI_U8 aucAddr[HI_METER_ADDR_LEN] = {0};

    mrsMeterAddrPreProc(aucAddr, addr, MRS_PROTO645_VER_1997);
    for(i = 0; i < logCtx->meterCount; i++)
    {
        if(mrsToolsMemEq(logCtx->item[i].meter, aucAddr, MRS_645_FRAME_METERADD_LEN))
        {
            *index = i;
            return HI_ERR_SUCCESS;
        }
    }
/* END:   PN: DTS2015042907341 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/13 */

    return HI_ERR_NOT_FOUND;
}


/* BEGIN: liuxipeng/KF54842 2014/2/20   问题单号: DTS2014022003115 */
// 外场可维可测645帧信息发送统计
HI_VOID mrsLogFrameInfStatTx(HI_PBYTE pData, HI_U16 dataLen, HI_U8 ucBcFlg)
/* END  : liuxipeng/KF54842 2014/2/20   问题单号: DTS2014022003115 */
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 frameType = FRAME_TYPE_OTHER;
    HI_S16 nodeIndex = -1;
    MRS_SRV_MRSLOG_CONTEXT *logCtx = GetMrsLogCtx();
    HI_PBYTE tmpbuf = pData;
    HI_S16 tmplen = (HI_S16)dataLen;
    HI_U16 pos = 0;
    HI_U16 frame_len = 0;
    HI_U8 frameIndex = 0;
        
    if(!pData)
    {
        return;
    }
    
    while(tmplen > 0)
    {
        ret = mrsFind645Frame(tmpbuf, tmplen, &pos, &frame_len);
        if(HI_ERR_SUCCESS == ret)
        {
            if(-1 == nodeIndex)
            {
                ret = mrsLogInsertNode(tmpbuf + pos + MRS_645_FRAME_METERADD_OFFSET, &nodeIndex);
                if(HI_ERR_SUCCESS != ret)
                {
                    return;
                }

                if (nodeIndex < 0)
                {
                    return;
                }

                if (nodeIndex >= MRS_LOG_MAX_ITEM_NUM)
                {
                    return;
                }

                // 清除上一次抄读日冻结数据项标志
                BIT8_CLR(logCtx->item[nodeIndex].dayFreezeRmFlg, DAYFREEZE_TIME_READING_FLG_BIT);
                BIT8_CLR(logCtx->item[nodeIndex].dayFreezeRmFlg, DAYFREEZE_DATA_READING_FLG_BIT);

                logCtx->item[nodeIndex].bitMap = 0;
            /* BEGIN: liuxipeng/KF54842 2014/2/20   问题单号: DTS2014022003115 */    
                logCtx->item[nodeIndex].ucBroadcast = ucBcFlg;
            /* END  : liuxipeng/KF54842 2014/2/20   问题单号: DTS2014022003115 */
            }

            logCtx->item[nodeIndex].mrTotal++;  // 统计抄读过的数据项个数(用于抄通率统计)
            
            frameType = mrsLogGetFrameType(tmpbuf + pos);

            if((FRAME_TYPE_DAYFREEZE_TIME == frameType)
                || (FRAME_TYPE_DAYFREEZE_DATA == frameType))
            {
                logCtx->item[nodeIndex].dayFreezeRmFlg |= frameType;        // 置本次抄读日冻结数据项标志
                logCtx->item[nodeIndex].dayFreezeRmFlg |= (HI_U8)(frameType << 2);   // 置本轮抄读日冻结数据项标志
                BIT16_SET(logCtx->item[nodeIndex].bitMap, frameIndex);
            }

            frameIndex++;
            tmpbuf += (pos + frame_len);
            tmplen -= (HI_S16)(pos + frame_len);
        }
        else
        {
            break;
        }
    }

    if(nodeIndex >= 0)
    {
        if(BIT8_GET(logCtx->item[nodeIndex].dayFreezeRmFlg, DAYFREEZE_TIME_READ_END_FLG_BIT)
            && BIT8_GET(logCtx->item[nodeIndex].dayFreezeRmFlg, DAYFREEZE_DATA_READ_END_FLG_BIT))
        {
            logCtx->dayFreezeReadTotal++;
            
            if(0 == logCtx->item[nodeIndex].dayFreezeRead++)
            {
                logCtx->dayFreezeMeterTotal++;  // 统计抄读日冻结数据电表的数量
            }
            
            // 清除本轮抄读日冻结数据项标志
            BIT8_CLR(logCtx->item[nodeIndex].dayFreezeRmFlg, DAYFREEZE_TIME_READ_END_FLG_BIT);
            BIT8_CLR(logCtx->item[nodeIndex].dayFreezeRmFlg, DAYFREEZE_DATA_READ_END_FLG_BIT);
        }
    }
    
    return;
}


// 外场可维可测645帧信息接收统计
HI_VOID mrsLogFrameInfStatRx(HI_PBYTE pDataRx, HI_U16 dataLenRx, HI_PBYTE pDataTx, HI_U16 dataLenTx, HI_U16 respFlg)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 frameType = FRAME_TYPE_OTHER;
    HI_S16 nodeIndex = -1;
    MRS_SRV_MRSLOG_CONTEXT *logCtx = GetMrsLogCtx();
    HI_PBYTE tmpbuf = pDataRx;
    HI_S16 tmplen = (HI_S16)dataLenRx;
    HI_U16 pos = 0;
    HI_U16 frame_len = 0;
    HI_U8 frameIndex = 0;
    HI_U8 * destAddr = HI_NULL;
    HI_U32 tTime = 0;
    HI_SYS_CALENDAR_TIME_S stTime = {0};
        
    if((!pDataRx) || (!dataLenRx))
    {
        return;
    }    

    destAddr = mrsLogPlcOverTimeStat(pDataTx, dataLenTx, respFlg, &nodeIndex);
    if(destAddr == HI_NULL)
    {
        return;
    }

    if(nodeIndex == -1)
    {
        return;
    }

    tTime = HI_MDM_GetRealTime();
    HI_MDM_MakeSecondToCalendar(tTime, &stTime);

    while(tmplen > 0)
    {            
        ret = mrsFind645Frame(tmpbuf, tmplen, &pos, &frame_len);
        if(HI_ERR_SUCCESS == ret)
        {
            HI_U8 * frameHead = HI_NULL;
            HI_U8 * MeterAddr = HI_NULL;

            while((respFlg != 0) && (!(respFlg & (0x01 << frameIndex))))
            {                
                frameIndex++;
            }
            
            frameHead = tmpbuf + pos;
            MeterAddr = frameHead + MRS_645_FRAME_METERADD_OFFSET;
            
            logCtx->item[nodeIndex].mrSucc++;   // 统计抄读成功的数据项个数

            frameType = mrsLogGetFrameType(frameHead);

            switch(frameType)
            {
            case FRAME_TYPE_DENY:
                if(logCtx->item[nodeIndex].bitMap & (0x01 << frameIndex))
                {
                    logCtx->item[nodeIndex].sysTime = tTime;
                    logCtx->item[nodeIndex].causeLatest = MRS_DAY_FREEZE_FAIL_DENY;
                    logCtx->item[nodeIndex].causeSet   |= MRS_DAY_FREEZE_FAIL_DENY;
                    (hi_void)memset_s(logCtx->item[nodeIndex].frame, sizeof(logCtx->item[nodeIndex].frame), 0, sizeof(logCtx->item[nodeIndex].frame));
                    (hi_void)memcpy_s(logCtx->item[nodeIndex].frame, HI_SDM_ENG_FRAME_LEN_MAX, frameHead,
                                        MRS_MIN(HI_SDM_ENG_FRAME_LEN_MAX,frame_len));
                }
                break;
                
            case FRAME_TYPE_DAYFREEZE_TIME:
            case FRAME_TYPE_DAYFREEZE_DATA:
                if(!mrsToolsMemEq(MeterAddr, destAddr, MRS_645_FRAME_METERADD_LEN))
                {
                    logCtx->item[nodeIndex].sysTime = tTime;
                    logCtx->item[nodeIndex].causeLatest = MRS_DAY_FREEZE_FAIL_ADDRERR;
                    logCtx->item[nodeIndex].causeSet   |= MRS_DAY_FREEZE_FAIL_ADDRERR;

                    (hi_void)memset_s(logCtx->item[nodeIndex].frame, sizeof(logCtx->item[nodeIndex].frame), 0, sizeof(logCtx->item[nodeIndex].frame));
                    (hi_void)memcpy_s(logCtx->item[nodeIndex].frame, HI_SDM_ENG_FRAME_LEN_MAX, MeterAddr, MRS_645_FRAME_METERADD_LEN);
 
                    break;  // 检测到地址错误后不进行后续错误检查
                }

                if(HI_FALSE == mrsToolsCheckBCDEx(&frameHead[MRS_645_FRAME_DATA_OFFSET + 4],
                                                    frameHead[MRS_645_FRAME_LENGTH_OFFSET] - 4))
                {
                    // 数据内容非法    
                    logCtx->item[nodeIndex].sysTime = tTime;
                    logCtx->item[nodeIndex].causeLatest = MRS_DAY_FREEZE_FAIL_INVALID;
                    logCtx->item[nodeIndex].causeSet   |= MRS_DAY_FREEZE_FAIL_INVALID;

                    (hi_void)memset_s(logCtx->item[nodeIndex].frame, sizeof(logCtx->item[nodeIndex].frame), 0, sizeof(logCtx->item[nodeIndex].frame));

                    // 保存原始数据 645帧第二个0x68开始截取
                    (hi_void)memcpy_s(logCtx->item[nodeIndex].frame, HI_SDM_ENG_FRAME_LEN_MAX, &frameHead[MRS_645_FRAME_2ND_HEAD_OFFSET],
                                        MRS_MIN(HI_SDM_ENG_FRAME_LEN_MAX, frame_len - MRS_645_FRAME_2ND_HEAD_OFFSET));

                    break;  // 检测到数据内容非法后不进行后续的错误检查
                }
                
                if(FRAME_TYPE_DAYFREEZE_TIME == frameType)
                {
                    HI_U8 meterTime[6] = {0};   // SS:MM:HH:DD:MM:YY

                    (hi_void)memcpy_s(&meterTime[1], sizeof(meterTime)-1, &frameHead[MRS_645_FRAME_DATA_OFFSET + 4], 5);
                    mrs645DataDecode(&meterTime[1], 5);
                    
                #if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
                    if(MrsLogIsClockValid())
                    {
                        if((HI_ERR_SUCCESS != MRS_TimeIsValid(meterTime))
                            || (HI_FALSE == mrsCheckDayFreezeTime(meterTime, &stTime)))
                        {
                            // 日冻结时间非法或是与系统时钟不同步
                            logCtx->item[nodeIndex].sysTime = tTime;
                            logCtx->item[nodeIndex].causeLatest = MRS_DAY_FREEZE_FAIL_CLOCKERR;
                            logCtx->item[nodeIndex].causeSet   |= MRS_DAY_FREEZE_FAIL_CLOCKERR;

                            (hi_void)memset_s(logCtx->item[nodeIndex].frame, sizeof(logCtx->item[nodeIndex].frame), 0, sizeof(logCtx->item[nodeIndex].frame));
                            (hi_void)memcpy_s(logCtx->item[nodeIndex].frame, HI_SDM_ENG_FRAME_LEN_MAX, frameHead,
                                                MRS_MIN(HI_SDM_ENG_FRAME_LEN_MAX, frame_len));

                            break;  // 检测到日冻结时标非法后不进行后续的错误检查
                        }
                    }
                    else
                #endif        
                    {
                        if(tTime < MRS_LOG_TIME_2000_1_1)   // 系统时间未进行过设置 将日冻结日期与系统时间同步
                        {
                            HI_SYS_CALENDAR_TIME_S tempTime = {0};

                            MRS_BCD2Time(meterTime, &tempTime);
                            
                            stTime.year = tempTime.year;
                            stTime.month = tempTime.month;
                            stTime.day = tempTime.day;

                            HI_MDM_SetCalendarTime(&stTime);
                        }                    
                    }
                    
                }
                
                logCtx->item[nodeIndex].dayFreezeSuccFlg |= frameType;
                
                break;
                
            case FRAME_TYPE_OTHER:
            default:
                break;
            }
    
            frameIndex++;
            tmpbuf += (pos + frame_len);
            tmplen -= (HI_S16)(pos + frame_len);
        }
        else
        {
            break;
        }
    }

    if(nodeIndex >= 0)
    {
        if(BIT8_GET(logCtx->item[nodeIndex].dayFreezeSuccFlg, DAYFREEZE_TIME_READ_SUCC_FLG_BIT)
            && BIT8_GET(logCtx->item[nodeIndex].dayFreezeSuccFlg, DAYFREEZE_DATA_READ_SUCC_FLG_BIT))
        {
            logCtx->dayFreezeReadSucc++;
            
            if(0 == logCtx->item[nodeIndex].dayFreezeSucc++)
            {
                logCtx->dayFreezeMeterSucc++;  // 统计抄读成功日冻结数据电表的数量
            }
            
            logCtx->item[nodeIndex].dayFreezeSuccFlg = 0x00;   // 清除本轮抄读日冻结数据项成功标志
        }

        // 清除上一次抄读日冻结数据项标志
        BIT8_CLR(logCtx->item[nodeIndex].dayFreezeRmFlg, DAYFREEZE_TIME_READING_FLG_BIT);
        BIT8_CLR(logCtx->item[nodeIndex].dayFreezeRmFlg, DAYFREEZE_DATA_READING_FLG_BIT);
        
        logCtx->item[nodeIndex].bitMap = 0;
    /* BEGIN: liuxipeng/KF54842 2014/2/20   问题单号: DTS2014022003115 */    
        logCtx->item[nodeIndex].ucBroadcast = HI_FALSE;
    /* END  : liuxipeng/KF54842 2014/2/20   问题单号: DTS2014022003115 */    
    }
    
    return;
}

// 外场可维可测645帧超时统计
HI_U8 * mrsLogPlcOverTimeStat(HI_PBYTE pData, HI_U16 dataLen, HI_U16 respFlg, HI_S16 *index)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 frameType = FRAME_TYPE_OTHER;
    HI_S16 nodeIndex = -1;
    MRS_SRV_MRSLOG_CONTEXT *logCtx = GetMrsLogCtx();
    HI_PBYTE tmpbuf = pData;
    HI_S16 tmplen = (HI_S16)dataLen;
    HI_U16 pos = 0;
    HI_U16 frame_len = 0;
    HI_U16 frameIndex = 0;
    HI_U8 * destAddr = HI_NULL;
    HI_U8 len = 0;
        
    if((!pData) || (!dataLen))
    {
        return HI_NULL;
    } 
    
    while(tmplen > 0)
    {            
        HI_U8 * frameHead = HI_NULL;
        
        ret = mrsFind645Frame(tmpbuf, tmplen, &pos, &frame_len);
        if(HI_ERR_SUCCESS == ret)
        {            
            frameHead = tmpbuf + pos;            

            if(-1 == nodeIndex)
            {    
                destAddr = frameHead + MRS_645_FRAME_METERADD_OFFSET;
                
                ret = mrsLogFindNode(destAddr, &nodeIndex);
                if(HI_ERR_SUCCESS != ret)
                {
                    return HI_NULL;
                }

                if (nodeIndex < 0)
                {
                    return HI_NULL;
                }

                if(!(BIT8_GET(logCtx->item[nodeIndex].dayFreezeRmFlg, DAYFREEZE_TIME_READING_FLG_BIT))
                    && !(BIT8_GET(logCtx->item[nodeIndex].dayFreezeRmFlg, DAYFREEZE_DATA_READING_FLG_BIT)))
                {
                    break; // 当前没有进行日冻结数据抄读
                }

                if(logCtx->item[nodeIndex].bitMap
                    != (logCtx->item[nodeIndex].bitMap & respFlg))
                {
                    // 日冻结数据超时
                    logCtx->item[nodeIndex].sysTime = HI_MDM_GetRealTime();
                /* BEGIN: liuxipeng/KF54842 2014/2/20   问题单号: DTS2014022003115 */    
                    if(logCtx->item[nodeIndex].ucBroadcast)
                    {
                        logCtx->item[nodeIndex].causeLatest = MRS_DAY_FREEZE_FAIL_NO_JOIN_NET;
                        logCtx->item[nodeIndex].causeSet   |= MRS_DAY_FREEZE_FAIL_NO_JOIN_NET;
                    }
                    else
                    {
                        logCtx->item[nodeIndex].causeLatest = MRS_DAY_FREEZE_FAIL_TIMEOUT;
                        logCtx->item[nodeIndex].causeSet   |= MRS_DAY_FREEZE_FAIL_TIMEOUT;
                    }
                /* END  : liuxipeng/KF54842 2014/2/20   问题单号: DTS2014022003115 */
                    (hi_void)memset_s(logCtx->item[nodeIndex].frame, sizeof(logCtx->item[nodeIndex].frame), 0, sizeof(logCtx->item[nodeIndex].frame));
                }
                else
                {
                    if(index != HI_NULL)
                    {
                        *index = nodeIndex;
                    }
                    
                    return destAddr;
                }
            }

            frameType = mrsLogGetFrameType(frameHead);
            
            if((FRAME_TYPE_DAYFREEZE_TIME == frameType)
                || (FRAME_TYPE_DAYFREEZE_DATA == frameType))
            {
                if(!(respFlg & (0x01 << frameIndex)))
                {
                    HI_U8 remainLen = 0;

                    remainLen = (HI_SDM_ENG_FRAME_LEN_MAX > len)? (HI_SDM_ENG_FRAME_LEN_MAX - len) : 0;
                        
                    // 保存原始数据 645帧第二个0x68开始截取
                    (hi_void)memcpy_s(logCtx->item[nodeIndex].frame + len, remainLen,
                                    &frameHead[MRS_645_FRAME_2ND_HEAD_OFFSET],
                                    MRS_MIN(remainLen, frame_len - MRS_645_FRAME_2ND_HEAD_OFFSET));
                    len += (HI_U8)(frame_len - MRS_645_FRAME_2ND_HEAD_OFFSET); 
                }                
            }

            frameIndex++;
            tmpbuf += (pos + frame_len);
            tmplen -= (HI_S16)(pos + frame_len);
        }
        else
        {
            break;
        }
    }
    
    if((respFlg == 0) && (nodeIndex >= 0))
    {
        // 清除上一次抄读日冻结数据项标志
        BIT8_CLR(logCtx->item[nodeIndex].dayFreezeRmFlg, DAYFREEZE_TIME_READING_FLG_BIT);
        BIT8_CLR(logCtx->item[nodeIndex].dayFreezeRmFlg, DAYFREEZE_DATA_READING_FLG_BIT);
        
        logCtx->item[nodeIndex].bitMap = 0;
    /* BEGIN: liuxipeng/KF54842 2014/2/20   问题单号: DTS2014022003115 */    
        logCtx->item[nodeIndex].ucBroadcast = HI_FALSE;
    /* END  : liuxipeng/KF54842 2014/2/20   问题单号: DTS2014022003115 */    
    }

    if(index != HI_NULL)
    {
        *index = nodeIndex;
    }

    return destAddr;
}


#if defined(MRS_LOG_STA_SWITCH)

// ************************************************
// **************STA日冻结数据记录*****************
// ************************************************
typedef struct
{
    HI_U16 meterCount;
    HI_U16 pad;
    MRS_SRV_MRSLOG_DAYFREEZE_ITEM item[PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM];
} MRS_SRV_MRSLOG_STA_DAYFREEZE_REC_DAY;

typedef struct
{
    HI_U16 dayIndex;
    HI_U16 pad;
    MRS_SRV_MRSLOG_STA_DAYFREEZE_REC_DAY dayFreeze[DAYFREEZE_MAX_REC_DAY];
} MRS_SRV_MRSLOG_STA_DAYFREEZE_REC;

MRS_SRV_MRSLOG_STA_DAYFREEZE_REC g_mrsLogStaRec;

MRS_SRV_MRSLOG_STA_DAYFREEZE_REC_DAY * GetStaRecCurrDay(HI_BOOL nextDayFlg);
HI_VOID mrsLogStaRefreshDayFreezeRec(MRS_SRV_MRSLOG_CONTEXT *ctx, HI_BOOL nextDayFlg);

MRS_SRV_MRSLOG_STA_DAYFREEZE_REC_DAY * GetStaRecCurrDay(HI_BOOL nextDayFlg)
{
    MRS_SRV_MRSLOG_STA_DAYFREEZE_REC_DAY *dayFreezeItem = HI_NULL;

    dayFreezeItem = &g_mrsLogStaRec.dayFreeze[g_mrsLogStaRec.dayIndex % DAYFREEZE_MAX_REC_DAY];
    
    (hi_void)memset_s((HI_U8 *)dayFreezeItem, sizeof(MRS_SRV_MRSLOG_STA_DAYFREEZE_REC_DAY), 0, sizeof(MRS_SRV_MRSLOG_STA_DAYFREEZE_REC_DAY));

    if(nextDayFlg)
    {
        g_mrsLogStaRec.dayIndex++;
    }
    
    return dayFreezeItem;
}

HI_VOID mrsLogStaRefreshDayFreezeRec(MRS_SRV_MRSLOG_CONTEXT *ctx, HI_BOOL nextDayFlg)
{
    MRS_SRV_MRSLOG_STA_DAYFREEZE_REC_DAY *dayFreezeItem = HI_NULL;
    HI_U16 i = 0;
    HI_U16 j = 0;

    dayFreezeItem = GetStaRecCurrDay(nextDayFlg);
            
    for(i = 0; i < MRS_MIN(ctx->meterCount, PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM); i++)
    {
        if((ctx->item[i].dayFreezeSucc == 0) && (ctx->item[i].causeSet != 0))
        {
            dayFreezeItem->item[j].sysTime = ctx->item[i].sysTime;
            (hi_void)memcpy_s(dayFreezeItem->item[j].meter, HI_SDM_ENG_METER_ADDR_LEN, ctx->item[i].meter, HI_SDM_ENG_METER_ADDR_LEN);
            dayFreezeItem->item[j].causeSet = ctx->item[i].causeSet;

            j++;
        }
    }

    dayFreezeItem->meterCount = j;

    return;
}       

HI_VOID mrsLogStaDayfreezeRecTimerProc(HI_VOID)
{
    MRS_SRV_MRSLOG_CONTEXT *logCtx = GetMrsLogCtx();
    static HI_S32 tDay = 1;
    static HI_S32 count = 0;
    HI_SYS_CALENDAR_TIME_S stTime = {0};
    HI_BOOL diffDayFlg = HI_FALSE;

    count++;
    
    HI_MDM_GetCalendarTime(&stTime);
    
    if(tDay != stTime.day)
    {
        tDay = stTime.day;
        
        if((stTime.hour == 0)
            && (count > (MRS_LOG_SECS_PER_HOUR * 1000 / MRS_STA_TIME_DAYFREEZE_REC)))
        {
            diffDayFlg = HI_TRUE;
        }
    }
    
    mrsLogStaRefreshDayFreezeRec(logCtx, diffDayFlg);

    if(diffDayFlg)
    {
        initMrsLogCtx();
    }

    return;
}

// ************************************************
// **************STA日冻结数据记录END**************
// ************************************************

#endif

#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)

HI_U32 mrsLogSendDayfreezeRecToHso(HI_U16 usId, HI_IN HI_PVOID pCmdParam,
                                                    HI_U16 usCmdParamSize, HI_U32 ulOption)
{
#if defined(MRS_LOG_STA_SWITCH)
    DIAG_CMD_MRS_DAYFREEZE_REC_REQ_STRU * param = HI_NULL;
    DIAG_CMD_MRS_DAYFREEZE_REC_IND_STRU * dayFreezeRec = HI_NULL;
    MRS_SRV_MRSLOG_STA_DAYFREEZE_REC_DAY * dayFreezeItem = HI_NULL;
    HI_U16 len = 0;
    HI_U16 index = 0;
    HI_U16 i = 0;
    HI_U16 j = 0;
    
    if(usId != ID_DIAG_CMD_MRS_DAYFREEZE_REC)
    {
        return HI_ERR_INVALID_ID;
    }

    if(!pCmdParam)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    MRS_NOT_USED(usCmdParamSize);
    
    param = (DIAG_CMD_MRS_DAYFREEZE_REC_REQ_STRU *)pCmdParam;
    index = (HI_U16)((param->dayIndex - 1) % DAYFREEZE_MAX_REC_DAY);
    len = sizeof(DIAG_CMD_MRS_DAYFREEZE_REC_IND_STRU);

    dayFreezeRec = (DIAG_CMD_MRS_DAYFREEZE_REC_IND_STRU *)mrsToolsMalloc(len);
    if(!dayFreezeRec)
    {
        return HI_ERR_NO_MORE_MEMORY;
    }

    dayFreezeItem = &g_mrsLogStaRec.dayFreeze[index];
    j = 0;

    do
    {
        (hi_void)memset_s(dayFreezeRec, len, 0, len);
        
        dayFreezeRec->currDayIndex = g_mrsLogStaRec.dayIndex + 1;
        dayFreezeRec->currIndex = (HI_U16)param->dayIndex;
        dayFreezeRec->arrayIndex = (HI_U16)(index + 1);
        dayFreezeRec->meterCount = (HI_U8)dayFreezeItem->meterCount;

        for(i = 0; (i < DAYFREEZE_MAX_PACK_NUM) && (j < dayFreezeItem->meterCount); i++,j++)
        {
            dayFreezeRec->item[i].sysTime = dayFreezeItem->item[j].sysTime;

            (hi_void)memcpy_s(dayFreezeRec->item[i].meter, HI_SDM_ENG_METER_ADDR_LEN,
                                dayFreezeItem->item[j].meter,
                                HI_SDM_ENG_METER_ADDR_LEN);

            dayFreezeRec->item[i].causeSet = dayFreezeItem->item[j].causeSet;
            dayFreezeRec->Count++;
        }
       
        HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)dayFreezeRec, len, HI_TRUE);
        
    }while(j < dayFreezeItem->meterCount);


    mrsToolsFree(dayFreezeRec);

#else

    MRS_NOT_USED(usId);
    MRS_NOT_USED(pCmdParam);
    MRS_NOT_USED(usCmdParamSize);
    MRS_NOT_USED(ulOption);
    
#endif
    
    return HI_ERR_SUCCESS;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_BOOL mrsGetCcoTimeFrist(HI_VOID)
{    
    HI_U32 ret = HI_ERR_SUCCESS;

    ret = mrsCheckProtoVer();
    if (HI_ERR_SUCCESS != ret)
    {
        return HI_FALSE;
    }

    if (MrsFristGetTimeStatus() == MRS_FIRST_ACQUIRE_TIME_END)
    {
        return HI_FALSE;
    }

    if (MrsFristGetTimeStatus() == MRS_FIRST_ACQUIRE_TIME_DOING)
    {
        return HI_TRUE;
    }

    mrsSetFristGetTimeFlg(MRS_FIRST_ACQUIRE_TIME_DOING);
    mrsLogClockTimerStart(MRS_CCO_TIME_ACTIVE_ACQUIRE_CLOCK);

    return HI_TRUE;
}

HI_VOID mrsSetFristGetTimeFlg(HI_U8 ucFlag)
{
    GetMrsLogClock()->firstgettimeStatus = ucFlag;
    mrsDfxCurrGetTimeStatus(ucFlag);
}

HI_BOOL mrsCcoClockStatusAllow(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    ret = mrsCheckProtoVer();
    if (HI_ERR_SUCCESS != ret)
    {
        return HI_TRUE;
    }
    
    if (MrsFristGetTimeStatus() == MRS_FIRST_ACQUIRE_TIME_DOING)
    {
        return HI_FALSE;
    }
    
    return HI_TRUE;
}
#endif
