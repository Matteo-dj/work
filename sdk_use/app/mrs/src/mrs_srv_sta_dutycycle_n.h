//*****************************************************************************
//
//                  版权所有 (C), 1998-2015, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_sta_dutycycle_n.h
//  版 本 号   : V1.0 
//  作    者   : cuiate/00233580
//  生成日期   : 2015-08-27
//  功能描述   : STA占空比策略: 内部接口和声明
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2015-08-27
//    作    者 : cuiate/00233580
//    修改内容 : 创建文件 
//
//*****************************************************************************

#ifndef _MRS_SRV_STA_DUTYCYCLE_N_H_
#define _MRS_SRV_STA_DUTYCYCLE_N_H_

HI_START_HEADER
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
//*****************************************************************************
//*****************************************************************************

#define MRS_STA_NV_DUTY_CYCLE_INVALID_INDEX         (-1)

#define MRS_PROTO645_DI_DATE_TIME   (0x0400010C)
#define MRS_PROTO645_DI_POWER_INF   (0x03110001)


#define MRS_STA_DUTY_CYCLE_SHORT_DURATION_DEFAULT   (30)
#define MRS_STA_DUTY_CYCLE_SHORT_TIMES_DEFAULT      (10)
#define MRS_STA_DUTY_CYCLE_LONG_DURATION_DEFAULT    (72)
#define MRS_STA_DUTY_CYCLE_LONG_TIMES_DEFAULT       (30)


typedef HI_U32 (*MRS_STA_DUTY_CYCLE_CMD_RX_FUNC)(HI_U8 *pFrame, HI_U16 usFrameLength);
typedef HI_U32 (*MRS_STA_DUTY_CYCLE_CMD_TIMEOUT)(HI_VOID);

typedef struct
{
    HI_U32 ulDi;
    MRS_STA_DUTY_CYCLE_CMD_RX_FUNC pRxFunc;
    MRS_STA_DUTY_CYCLE_CMD_TIMEOUT pTimeoutFunc;
} MRS_STA_DUTY_CYCLE_CMD_OBJ;


HI_PRV MRS_SRV_STA_DUTY_CYCLE_CONTEXT *mrsStaDutyCycleCtx(HI_VOID);
#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
HI_PRV HI_VOID mrsStaDutyCycleLoadCfgInf(MRS_SRV_STA_DUTY_CYCLE_CONTEXT *pstCtx, NV_APP_STA_DUTY_CYCLE_CFG_INFO *pstCfgInf);

HI_PRV HI_VOID mrsStaDutyCycleLoadMeterInf(MRS_SRV_STA_DUTY_CYCLE_CONTEXT *pstCtx, HI_NV_FTM_COLLECT_PARAM_FILE_S *pstCltParam);
#endif
HI_PRV HI_U32 mrsStaDutyCycleGenFrame(HI_U32 ulDi, HI_PBYTE *ppFrame, HI_U16 *pusLength);

HI_PRV HI_U32 mrsStaDutyCycleSendFrame(HI_PBYTE pFrame, HI_U16 usFrameLength);

HI_PRV HI_U32 mrsStaDutyCycleMeterQry(HI_U32 ulDi);

HI_PRV HI_U32 mrsStaDutyCycleDateTimeQry(HI_VOID);

HI_PRV HI_U32 mrsStaDutyCycleDateTimeFrameRx(HI_U8 *pFrame, HI_U16 usFrameLength);

HI_PRV HI_U32 mrsStaDutyCycleDateTimeTimeout(HI_VOID);

HI_PRV HI_U32 mrsStaDutyCyclePowerInfQry(HI_VOID);

HI_PRV HI_U32 mrsStaDutyCyclePowerInfFrameRx(HI_U8 *pFrame, HI_U16 usFrameLength);

HI_PRV HI_U32 mrsStaDutyCyclePowerInfTimeout(HI_VOID);

HI_PRV HI_VOID mrsStaDutyCycleClearResetTimeInf(MRS_SRV_STA_DUTY_CYCLE_CONTEXT *pstCtx);

HI_PRV HI_VOID mrsStaDutyCycleSaveResetTime(MRS_SRV_STA_DUTY_CYCLE_CONTEXT *pstCtx);

HI_PRV HI_BOOL mrsStaDutyCycleIsLowestStrategy(HI_VOID); 

HI_PRV HI_BOOL mrsStaDutyCycleIsPowerOffReboot(HI_VOID);

HI_PRV HI_U32 mrsStaDutyCycleSetStrategy(HI_VOID);

HI_PRV HI_U32 mrsStaDutyCycleStrategyDecrease(HI_VOID);

HI_PRV HI_VOID mrsStaDutyCycleSaveCfgIndex(MRS_SRV_STA_DUTY_CYCLE_CONTEXT *pstCtx);

HI_PRV HI_U32 mrsStaDutyCycleResetTimeProc(HI_VOID);

HI_PRV HI_U32 mrsStaDutyCycleMeterChgNotify(HI_BOOL bChg, HI_U8 ucSlaveFlag, HI_U32 ulParam);

//*****************************************************************************
//*****************************************************************************
#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_END_HEADER

#endif //_MRS_SRV_STA_DUTYCYCLE_N_H_

