
#ifndef __MRS_SRV_MRSLOG_H__
#define __MRS_SRV_MRSLOG_H__


HI_START_HEADER

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA) && defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
#define MRS_LOG_STA_SWITCH
#endif


#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(MRS_LOG_STA_SWITCH)) && defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
//透抄抄读数据统计 存储周期24h
HI_VOID mrsLogXrComRx(HI_VOID);
HI_VOID mrsLogXrComTx(HI_VOID);
HI_VOID mrsLogXrPlcRx(HI_VOID);
HI_VOID mrsLogXrPlcTx(HI_VOID);

//轮抄/并发抄抄读数据统计，存储周期1h
HI_VOID mrsLogLrprComRx(HI_SDM_ENG_MRS_MR_STATS_TYPE_E statsType);
HI_VOID mrsLogLrprComTx(HI_SDM_ENG_MRS_MR_STATS_TYPE_E statsType);
HI_VOID mrsLogLrprPlcRx(HI_SDM_ENG_MRS_MR_STATS_TYPE_E statsType);    
HI_VOID mrsLogLrprPlcTx(HI_SDM_ENG_MRS_MR_STATS_TYPE_E statsType); 

//设置抄通率门限
HI_VOID mrsLogSetLowRatioThreshold(HI_U16 lowRatio);
#endif

HI_U32 mrsInitMrsLogModule(HI_VOID);

HI_U32 mrsDeinitMrsLogModule(HI_VOID);


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_U32 mrsCcoSetClock(HI_U8 clockBuf[6]);

HI_U32 mrsCcoRequestClockTimeout(HI_VOID);

HI_U32 mrsLogTimerProc(HI_VOID);

HI_U32 mrsStartMrsLogTimer(HI_VOID);
#endif

// 外场可维可测645帧信息发送统计
/* BEGIN: liuxipeng/KF54842 2014/2/20   问题单号: DTS2014022003115 */
HI_VOID mrsLogFrameInfStatTx(HI_PBYTE pData, HI_U16 dataLen, HI_U8 ucBcFlg);
/* END  : liuxipeng/KF54842 2014/2/20   问题单号: DTS2014022003115 */

// 外场可维可测645帧信息接收统计
HI_VOID mrsLogFrameInfStatRx(HI_PBYTE pDataRx, HI_U16 dataLenRx, HI_PBYTE pDataTx, HI_U16 dataLenTx, HI_U16 respFlg);

// 外场可维可测645帧超时统计
HI_U8 * mrsLogPlcOverTimeStat(HI_PBYTE pData, HI_U16 dataLen, HI_U16 respFlg, HI_S16 *index);

#if defined(MRS_LOG_STA_SWITCH)
// STA端日冻结统计量定时冻结定时器处理
HI_VOID mrsLogStaDayfreezeRecTimerProc(HI_VOID);
#endif

#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)

HI_U32 mrsLogSendDayfreezeRecToHso(HI_U16 usId, HI_IN HI_PVOID pCmdParam,
                                                    HI_U16 usCmdParamSize, HI_U32 ulOption);
HI_VOID mrsSetFristGetTimeFlg(HI_U8 ucFlag);
HI_BOOL mrsGetCcoTimeFrist(HI_VOID);
HI_VOID mrsGetTimeEndPro(HI_VOID);
HI_BOOL mrsCcoClockStatusAllow(HI_VOID);

HI_END_HEADER

#endif


