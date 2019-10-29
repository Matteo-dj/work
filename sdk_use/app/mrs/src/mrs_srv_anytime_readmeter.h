//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_anytime_readmeter.h
//  版 本 号   : V1.0 
//  作    者   : liuxipeng/KF54842
//  生成日期   : 2012-06-09
//  功能描述   : 读表模块函数及接口定义
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2012-06-09
//    作    者 : liuxipeng/KF54842
//    修改内容 : 创建文件 
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   : 
// SUBSYSTEM :
// MODULE    :  
// OWNER     :  
//*****************************************************************************

#ifndef __MRS_SRV_CCO_ANY_TIME_H__
#define __MRS_SRV_CCO_ANY_TIME_H__


HI_START_HEADER

/* 问题单号: DTS2014010608635 BEGIN: Added by liuxipeng/lkf54842, 2014/01/07 */
// 点抄读表帧来源 
#define	 XR_FROM_AFN02_F1  0 // 来自于 "转发通讯协议数据帧"
#define	 XR_FROM_AFN13_F1  1 // 来自于 "监控载波从节点"
#define	 XR_FROM_NDM_AFN13_F1  2 // 来自于 "来源于NDM的PLC帧包含的监控载波从节点"
/* 问题单号: DTS2014010608635 END:   Added by liuxipeng/lkf54842, 2014/01/07 */

//*****************************************************************************
// 函数名称: mrsAnyTimeReadMeterInit
// 功能描述: 随抄读表初始化
//           
// 参数说明:
//   无
//
// 返 回 值: 
//   无
//
// 调用要求: 
// 调用举例: 
// 作    者: liuxipeng/KF54842 [2012-06-09]
//*****************************************************************************
HI_VOID mrsAnyTimeReadMeterInit(HI_VOID);

//*****************************************************************************
// 函数名称: mrsAnyTimeReadMeterPlcTimeOutProcess
// 功能描述: 随抄读表Plc帧超时处理
//           
// 参数说明:
//   pstCcoCtx 全局上下文
//
// 返 回 值: 
//   无
//
// 调用要求: 
// 调用举例: 
// 作    者: liuxipeng/KF54842 [2012-06-09]
//*****************************************************************************
HI_VOID mrsAnyTimeReadMeterPlcTimeOutProcess(HI_IN HI_OUT MRS_CCO_SRV_CTX_STRU * pstCcoCtx);

//*****************************************************************************
// 函数名称: mrsAnyTimeReadMeterServiceEndProcess
// 功能描述: 随抄读表服务结束定时器超时处理
//           
// 参数说明:
//   pstCcoCtx 全局上下文
//
// 返 回 值: 
//   无
//
// 调用要求: 
// 调用举例: 
// 作    者: liuxipeng/KF54842 [2012-06-09]
//*****************************************************************************
HI_VOID mrsAnyTimeReadMeterServiceEndProcess(HI_IN HI_OUT MRS_CCO_SRV_CTX_STRU * pstCcoCtx);
//*****************************************************************************
// 函数名称: mrsAnyTimeReadMeterReceivePlcProcess
// 功能描述: 随抄读表Plc帧接收处理
//           
// 参数说明:
//   pstData PLC数据转发帧结构
//
// 返 回 值: 
//   无
//
// 调用要求: 
// 调用举例: 
// 作    者: liuxipeng/KF54842 [2012-06-09]
//*****************************************************************************
HI_U32 mrsAnyTimeReadMeterReceivePlcProcess(HI_IN MRS_DATA_TRANSMIT_AFN13_UP_STRU * pstData, HI_U8 ucMacAdd[HI_PLC_MAC_ADDR_LEN]);
//*****************************************************************************
// 函数名称: mrsAnyTimeReadMeterPlcChlStatusProcess
// 功能描述: 随抄读表通道改变消息处理
//           
// 参数说明:
//   pstCcoCtx 全局上下文
//
// 返 回 值: 
//   无
//
// 调用要求: 
// 调用举例: 
// 作    者: liuxipeng/KF54842 [2012-06-09]
//*****************************************************************************
HI_U32 mrsAnyTimeReadMeterPlcChlStatusProcess(HI_IN HI_OUT MRS_CCO_SRV_CTX_STRU * pstCcoCtx);

//*****************************************************************************
// 函数名称: mrsAnyTimeReadMeterProcess
// 功能描述: 随抄读表处理入口
//           
// 参数说明:
//   pstAddr 地址域
//   ucProtocol 规约类型
//   pDataBuff 待抄读的数据
//   usDataLen 待抄读数据的长度
// 
// 返 回 值: 
//   无
//
// 调用要求: 
// 调用举例: 
// 作    者: liuxipeng/KF54842 [2012-06-09]
//*****************************************************************************
HI_U32 mrsAnyTimeReadMeterProcess(HI_IN MRS_3762_USER_DATA_HRD_STRU * pstUsrDataHrd, HI_IN HI_U8 ucProtocol,
                                                    HI_IN HI_U8 * pDataBuff, HI_IN HI_U16 usDataLen);

HI_U32 mrsAnyTimeReadMeterProcessDelay(HI_IN HI_U8 * pDataBuff, HI_IN HI_U16 usDataLen);

HI_VOID mrsSetXRStatus(MRS_ANYTIME_RM_STATE_ENUM state);

HI_U32 mrsAFN13F1CreateAckFrame(HI_U8 **ppBuf, HI_U16 *pLen, HI_U8 aucMacAdd[HI_PLC_MAC_ADDR_LEN]);

HI_VOID mrsAnyTimeReadMeterFinishProc(HI_VOID);

HI_VOID mrsAnyTimeReadMeterServiceTimeout(MRS_CCO_SRV_CTX_STRU * pstCcoCtx);

HI_VOID mrsAnyTimeReadMeterFailEndProc(MRS_CCO_SRV_CTX_STRU * pstCcoCtx);

HI_U32 mrsCreateReadMeterFrame376_2(HI_IN HI_U8 ucProtocol, HI_IN HI_U8 * pDataBuff, HI_IN HI_U16 usDataLen,
                                                  HI_OUT HI_PBYTE * ppOutBuff, HI_OUT HI_U16 * usOutLen, HI_U8 ucMacAdd[HI_PLC_MAC_ADDR_LEN]);

HI_END_HEADER

#endif //__MRS_SRV_CCO_ANY_TIME_H__

