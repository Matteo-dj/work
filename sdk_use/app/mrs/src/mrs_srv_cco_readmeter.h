//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cco_readmeter.h
//  版 本 号   : V1.0 
//  作    者   : liuxipeng/KF54842
//  生成日期   : 2011-12-13
//  功能描述   : 读表模块函数及接口定义
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2011-12-13
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




#ifndef __MRS_SRV_CCO_READMETER_H__
#define __MRS_SRV_CCO_READMETER_H__

HI_START_HEADER

//激活轮抄读表模块
#define mrsAcivReadMeter()                                              \
    do {                                                                \
            HI_SYS_QUEUE_MSG_S stMsg = { 0 };                           \
            stMsg.ulMsgId = ID_MRS_MSG_MR_TIMER;                        \
            stMsg.ulParam[0] = EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER;    \
            mrsSendMessage2Queue(&stMsg);                               \
    }while(0)

#define MRS_READ_FAILED_CCO 0x00    // 抄读失败
#define MRS_READ_SUCCESS_CCO 0x01   // 抄读成功
#define MRS_ALLOW_READ_CCO 0x02     // 可以抄读
#define MRS_READ_METER_FLAG_BIT  0x01   //抄读标识为:获取Flag的低位


//*****************************************************************************
// 函数名称: mrsReadMeterStateProcess
// 功能描述: 读表状态处理
//           
// 参数说明:
//   pstCcoCtx [IN/OUT] 读表状态信息
//
// 返 回 值: 
//    HI_TRUE 等待状态
//    HI_FALSE 持续执行
//
// 调用要求: 
// 调用举例: 
// 作    者: liuxipeng/KF54842 [2011-12-20]
//*****************************************************************************
HI_BOOL mrsReadMeterStateProcess(HI_INOUT MRS_CCO_SRV_CTX_STRU* pstCcoCtx);

//*****************************************************************************
// 函数名称: mrsReadterModelCco
// 功能描述: 读表模块处理入口函数
//           
// 参数说明:
//  This function has no arguments.
//
// 返 回 值: 
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liuxipeng/KF54842 [2011-12-15]
//*****************************************************************************
HI_VOID mrsReadterModelCco(HI_VOID *);

//*****************************************************************************
// 函数名称: mrsCreatReadMeterQueueCco
// 功能描述: 创建读表队列(单向环形链表)
//           
// 参数说明:
//  This function has no arguments.
// 返 回 值: 
//    HI_ERR_SUCCESS 创建成功
//    HI_ERR_NOT_FOUND 没有找到电表
//    HI_ERR_NOT_ENOUGH_MEMORY 内存不足
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liuxipeng/KF54842 [2011-12-14]
//*****************************************************************************
HI_U32 mrsCreatReadMeterQueueCco(HI_VOID);

//*****************************************************************************
// 函数名称: mrsClearReadMeterQueueCco
// 功能描述: 清除整个读表队列 
//           
// 参数说明:
//  This function has no arguments.
//
// 返 回 值: 
//    无
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liuxipeng/KF54842 [2011-12-14]
//*****************************************************************************
HI_VOID mrsClearReadMeterQueueCco(HI_VOID);


//*****************************************************************************
// 函数名称: mrsInquireReadMeter
// 功能描述: 询问是否抄表
//           
// 参数说明:
//  This function has no arguments.
//
// 返 回 值: 
//    无
// 调用要求: 
// 调用举例: 
// 作    者: liuxipeng/KF54842 [2011-12-16]
//*****************************************************************************
HI_VOID mrsInquireReadMeter(HI_VOID);

//*****************************************************************************
// 函数名称: mrsAssembleReadMeterData
// 功能描述:组织PLC读表数据帧
//           
// 参数说明:
//   无
// 返 回 值: 
//    无
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liuxipeng/KF54842 [2011-12-16]
//*****************************************************************************
HI_VOID mrsAssembleReadMeterData(HI_VOID);

//*****************************************************************************
// 函数名称: mrsTransmit14F1FramePlc
// 功能描述: 转发PLC读表数据
//           
// 参数说明:
//   无
// 返 回 值: 
//    无
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liuxipeng/KF54842 [2011-12-16]
//*****************************************************************************
HI_VOID mrsTransmit14F1FramePlc(HI_VOID);

//*****************************************************************************
// 函数名称: mrsUpReterMetreData
// 功能描述: 上报读表数据
//           
// 参数说明:
//    * pstUpReadMeter [in] 待上传读表数据的结构
//
// 返 回 值: 
//    HI_ERR_SUCCESS 上传数据成功
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liuxipeng/KF54842 [2011-12-17]
//*****************************************************************************
HI_VOID mrsUpReterMetreData(HI_VOID);

//*****************************************************************************
// 函数名称: mrsReadMeterStateInit
// 功能描述: 读表模块状态初始化
//           
// 参数说明:
//    无
// 返 回 值: 
//    无
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liuxipeng/KF54842 [2011-12-16]
//*****************************************************************************
HI_VOID mrsReadMeterStateInit(HI_VOID);

//*****************************************************************************
// 函数名称: mrsReadMeterStaTimerProcess
// 功能描述: 定抄读表状态下STA通讯定时器超时处理
//           
// 参数说明:
//   pstCcoCtx [in/out] 当前状态
//
// 返 回 值: 
//    无
//
// 调用要求: 
// 调用举例: 
// 作    者: liuxipeng/KF54842 [2012-06-08]
//*****************************************************************************
HI_VOID mrsReadMeterStaTimerProcess( MRS_CCO_SRV_CTX_STRU* pstCcoCtx );

//*****************************************************************************
// 函数名称: mrsReadMeterMusterTimerProcess
// 功能描述: 定抄读表状态下集中器通讯定时器超时处理
//           
// 参数说明:
//   pstCcoCtx [in/out] 当前状态
//
// 返 回 值: 
//    无
//
// 调用要求: 
// 调用举例: 
// 作    者: liuxipeng/KF54842 [2012-06-08]
//*****************************************************************************
HI_U32 mrsReadMeterMusterTimerProcess(HI_VOID*);

//*****************************************************************************
// 函数名称: mrsReadMeterActiveTimer
// 功能描述: 读表模块激活定时器处理
//           
// 参数说明:
//   pstCcoCtx [in/out] 当前状态
//
// 返 回 值: 
//    无
//
// 调用要求: 
// 调用举例: 
// 作    者: liuxipeng/KF54842 [2012-06-08]
//*****************************************************************************
HI_VOID mrsReadMeterActiveTimer( MRS_CCO_SRV_CTX_STRU* pstCcoCtx );

HI_VOID mrsReadWaterMeterActiveTimer( MRS_CCO_SRV_CTX_STRU* pstCcoCtx );

HI_U32 mrsReadMeterFirstRunProcess(MRS_CCO_SRV_CTX_STRU* pstCcoCtx);

HI_U32 mrsMeterInfChangeProcess(MRS_CCO_SRV_CTX_STRU* pstCcoCtx);

HI_VOID mrsWaitAskReplyProcess(MRS_CCO_SRV_CTX_STRU* pstCcoCtx, HI_BOOL *bLoopFlg);

HI_VOID mrsWaitPlcReplyProcess(MRS_CCO_SRV_CTX_STRU* pstCcoCtx, HI_BOOL *bLoopFlg);

HI_VOID mrsWaitUpDataAckProcess(MRS_CCO_SRV_CTX_STRU* pstCcoCtx, HI_BOOL *bLoopFlg);

HI_VOID mrsWaterMeterModelRxRespProc(HI_VOID * pAckParam);
HI_U32 mrsWaterMeterModelAllTimeOutProc(HI_VOID * pAckParam);


//*****************************************************************************
// 函数名称: mrsReadMeterFindValidNode
// 功能描述: 查找有效节点：如果查找成功，直接返回；否则将当前处理节点信息置为空
//           
// 参数说明:
//   pstCcoCtx [in/out] 当前状态
//
// 返 回 值: 
//   HI_ERR_SUCCESS 查找到有效节点
//   HI_ERR_NOT_FOUND 没有查找到有效节点
//   0xFFFFFFFF地址映射表错误或读取失败
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2012-05-26]
//*****************************************************************************
HI_U32 mrsReadMeterFindValidNode(MRS_CCO_SRV_CTX_STRU * pstCcoCtx);

//*****************************************************************************
// 函数名称: mrsCurrNodeValid
// 功能描述: 判断抄表过程中当前节点是否有效
//
// 参数说明:
//
// 返 回 值:
//   HI_FALSE 当前节点无效
//   HI_TRUE 当前有效节点有效
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2012-09-04]
//*****************************************************************************
HI_BOOL mrsCurrNodeValid(HI_VOID);
//*****************************************************************************
// 函数名称: mrsReadMeterReceivePlcProcess
// 功能描述: 接收读表PLC数据处理
//
// 参数说明:
//          pstData 转发的数据内容
//          pstCcoCtx 全局上下文
// 返 回 值:
//          参见错误列表
//
// 调用要求: 无
// 调用举例: 无
// 作    者: liuxipeng/lKF54842 [2012-09-17]
//*****************************************************************************
HI_U32 mrsReadMeterReceivePlcProcess(MRS_DATA_TRANSMIT_AFN14_UP_STRU * pstData,
                                                MRS_CCO_SRV_CTX_STRU * pstCcoCtx,HI_U8 ucMacAdd[HI_PLC_MAC_ADDR_LEN]);

HI_VOID mrsSetReadMeterState(MRS_MRV_CCO_READMETER_STATE_ENUM state);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
HI_VOID mrsInitMrStateBbu(HI_VOID);
#endif

HI_U32 mrsReadMeterTotalMusterTimerProcess(HI_VOID *param);
HI_VOID mrsReadMeterResume(HI_VOID);

HI_VOID mrsReadWaterMeterStateInit(HI_VOID);

HI_END_HEADER
 


#endif //__MRS_SRV_CCO_READMETER_H__


