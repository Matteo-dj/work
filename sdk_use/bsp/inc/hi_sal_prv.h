//****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//****************************************************************************
//  文 件 名   : hi_sal_prv.h
//  版 本 号   : V1.0
//  作    者   : 沈汉坤/00130424
//  生成日期   : 2011年10月10日
//  功能描述   : SAL层内引用的私有接口, 外部模块不能直接引用!!!
//               该接口仅供 hi_sal.h 引用
//  函数列表   : 
//  修改历史   : 
//  1.日    期 : 2011年10月10日
//    作    者 : 沈汉坤/00130424
//    修改内容 : 创建文件
//
//****************************************************************************

#ifndef __HI_SAL_PRV_H__
#define __HI_SAL_PRV_H__
//****************************************************************************

//****************************************************************************
// 函数名称: HI_SYS_FlashIsBadBlock
// 功能描述: 检测指定编号的Flash块是否为坏块(V1版本不支持)
//           
// 参数说明:
//   ulSectorSn [in] : Flash块序号
// 
// 返 回 值: 
//    HI_TRUE  : 表示坏块
//    HI_FALSE : 表示没有坏块
//****************************************************************************
HI_EAPI HI_BOOL HI_SYS_FlashIsBadBlock(HI_U32 ulSectorAddress);
//****************************************************************************
#if !defined(PRODUCT_CFG_FEATURE_UT)
HI_EAPI HI_U32 HI_SYS_ControlTaskTimerPrv(HI_U32 ulTimerId, HI_U32 ulPeriod, HI_BOOL bStart, HI_BOOL bAppLayer);
HI_EAPI HI_U32 HI_NVM_WritePrv(HI_U16 usId, HI_CONST HI_PVOID pItemData, HI_U16 ulItemDataLen);
HI_EAPI HI_U32 HI_NVM_GetItemLengthPrv(HI_U16 usId, HI_OUT HI_U16* pusItemLength);
HI_EAPI HI_U32 HI_NVM_ReadPrv(HI_U16 usId, HI_OUT HI_PVOID pItemData, HI_U16 usItemDataLen, HI_BOOL bIfFailUseDefault);
HI_EAPI HI_U32 HI_NVM_WriteFactoryPrv(HI_U16 usId, HI_CONST HI_PVOID pData,HI_U16 usDataLen);
HI_EAPI HI_U32 HI_NVM_RegisterChangedNoitfyProcPrv(HI_U16 usMinId, HI_U16 usMaxId, HI_NVM_CHANGED_NOTIFY_CALLBACK_F pfnChanged);
HI_EAPI HI_U32  HI_DMS_SetForwardingPrv(HI_U16 usPortNum, HI_BOOL bForwarding);
HI_EAPI HI_BOOL HI_DMS_IsForwardingPrv(HI_U16 usPortNum);
HI_EAPI HI_U32 HI_DIAG_RegisterConnectPrv(HI_DIAG_CONNECT_F pfnUserConnect);
HI_EAPI HI_U32 HI_DIAG_ReportAirMessagePrv(HI_DIAG_AIR_MSG_S* pRptMessage, HI_U8 ucType);
HI_EAPI HI_U32 HI_DIAG_ReportLayerMessagePrv(HI_DIAG_LAYER_MSG_S* pRptMessage, HI_U8 ucType);
HI_EAPI HI_U32 HI_DIAG_ReportLayerMsgPrv(HI_U16 usId, HI_U16 usSrcModId, HI_U16 usDestModId, HI_PVOID pData, HI_U16 usDataSize);
HI_EAPI HI_U32 HI_DIAG_ReportUserPlaneMessagePrv(HI_DIAG_USERPLANE_MSG_S* pstMsg, HI_U32 ulSn);
HI_EAPI HI_U32 HI_DIAG_IsUserPlaneMessageEnablePrv(HI_U32 ulModId, HI_U32 ulId);
HI_EAPI HI_U32 HI_DIAG_ReportCommandPrv(HI_U16 usId, HI_U16 usDataSize, HI_PVOID pData);
HI_EAPI HI_BOOL HI_DIAG_IsCommandReportEnalbePrv(HI_VOID);
HI_EAPI HI_U32 HI_DIAG_RegisterCommandPrv(HI_CONST HI_DIAG_CMD_REG_OBJ_S* pstCmdTbl, HI_U16 usCmdNum, HI_U8 ucInstanceId);
HI_EAPI HI_U32 HI_DIAG_RegisterDRemoteCommandPrv(HI_CONST HI_DIAG_CMD_F pfnProcess);
HI_EAPI HI_U32 HI_DIAG_SendAckPacketPrv(HI_U32 ulId, HI_IN HI_PVOID pBuffer, HI_U16 usBufferSize, HI_BOOL bSync, HI_HANDLE hAckInstance);
HI_EAPI HI_U32 HI_DIAG_ReportPacketPrv(HI_U32 ulId, HI_U8 ucInstanceId, HI_PBYTE pucPacket, HI_U16 usPacketSize, HI_BOOL bSync);
HI_EAPI HI_U32 HI_DIAG_RegistDiagCmdNotifyPrv(HI_U32 ulId, HI_DIAG_CMD_NOTIFY_F * func);
HI_EAPI HI_U32 HI_DIAG_ReportStatObjPrv(HI_U16 usId, HI_PVOID pStatObj, HI_U16 usStatObjSize);
HI_EAPI HI_U32 HI_DIAG_ReportInitEvtPrv(HI_U16 usEvtId, HI_U8 ucOption, HI_U16 usRc);
HI_EAPI HI_U32 HI_DIAG_ReportSysEventPrv(HI_U16 usSysEvtId, HI_U8 ucOption);
HI_EAPI HI_U32 HI_DIAG_ReportSysCriticalEventPrv(HI_U32 ulEvtId, HI_U32 ulError);
HI_EAPI HI_U32 HI_DIAG_ReportSysMessagePrv(HI_U16 usId, HI_U16 usSrcModId, HI_U16 usDestModId, HI_PVOID pPacket, HI_U16 usPacketSize, HI_U32 ulOption);
#if defined(PRODUCT_CFG_FEATURE_NDM) && !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
HI_EAPI HI_U32 HI_DIAG_RegisterSysStatPrv(HI_CONST HI_DIAG_SYS_STAT_OBJ_S* pstSysStatObjTbl, HI_U16 usObjNum);
HI_EAPI HI_U32 HI_DIAG_SendNdmReqFramePrv(HI_U32 ulId, HI_U16 usPacketId, HI_PBYTE pucPacket, HI_U16 usPacketSize, HI_U8 aucMacAddr[HI_PLC_RELAY_NUM_MAX][HI_PLC_MAC_ADDR_LEN], HI_U32 ulSn);
#endif
HI_EAPI HI_VOID HI_SYS_DisableMMU(HI_VOID) ;

HI_EAPI HI_VOID HI_SYS_RebootPrv(HI_U32 id);
HI_EAPI HI_U32 HI_DIAG_RegisterParallelCmdPrv(HI_U16 , HI_U16, HI_DIAG_GET_CMD_PK_CACHE_F);
HI_EAPI HI_VOID  HI_SYS_FatalVerifyPrv(HI_PCSTR szExp, HI_U32 ulUsrData1, HI_U32 ulUsrData2, HI_PCSTR pszFile, HI_U32 ulLn, HI_PCSTR pszFunc);
HI_EAPI HI_VOID HI_SYS_PtrFatalVerifyPrv(HI_PCSTR szExp, HI_U32 ulUsrData1, HI_U32 ulUsrData2, HI_U32 ulUsrData3, HI_U32 ulUsrData4, HI_U32 ulLn, HI_PCSTR pszFunc);
HI_EAPI HI_VOID HI_SYS_TaskStackErrAssertPrv(HI_PCSTR pszFile, HI_U32 ulLineNo, HI_PCSTR pszFunction);
HI_EAPI HI_U32 HI_DIAG_ReportBufferByHexPrv(HI_U32 ulId, HI_U8* pucBuffer, HI_U32 ulLen, HI_PCSTR pszFile, HI_U32 ulLineNo);
HI_EAPI HI_BOOL HI_DIAG_IsConnectPrv(HI_VOID);
HI_EAPI HI_U32 HI_DIAG_IsPrintfEnablePrv(HI_U32 ulId);
HI_EAPI HI_U32 HI_DIAG_IsLayerMessageEnablePrv(HI_U16 usId, HI_U16 usSrcModId, HI_U16 usDestModId, HI_U8 ucType);
#elif defined(PRODUCT_CFG_FEATURE_UT) && !defined(PRODUCT_CFG_HAVE_CUSTOM_UT_STUB)
HI_PRVL HI_U32 HI_SYS_ControlTaskTimerPrv(HI_U32 ulTimerId, HI_U32 ulPeriod, HI_BOOL bStart, HI_BOOL bAppLayer) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_NVM_WritePrv(HI_U16 usId, HI_CONST HI_PVOID pItemData, HI_U16 ulItemDataLen) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_NVM_GetItemLengthPrv(HI_U16 usId, HI_OUT HI_U16* pusItemLength) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_NVM_ReadPrv(HI_U16 usId, HI_OUT HI_PVOID pItemData, HI_U16 usItemDataLen, HI_BOOL bIfFailUseDefault) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_NVM_WriteFactoryPrv(HI_U16 usId, HI_CONST HI_PVOID pData,HI_U16 usDataLen) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_NVM_RegisterChangedNoitfyProcPrv(HI_U16 usMinId, HI_U16 usMaxId, HI_NVM_CHANGED_NOTIFY_CALLBACK_F pfnChanged) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32  HI_DMS_SetForwardingPrv(HI_U16 usPortNum, HI_BOOL bForwarding) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_BOOL HI_DMS_IsForwardingPrv(HI_U16 usPortNum) {return HI_FALSE;}
HI_PRVL HI_U32 HI_DIAG_RegisterConnectPrv(HI_DIAG_CONNECT_F pfnUserConnect) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DIAG_ReportAirMessagePrv(HI_DIAG_AIR_MSG_S* pRptMessage, HI_U8 ucType) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DIAG_ReportLayerMessagePrv(HI_DIAG_LAYER_MSG_S* pRptMessage, HI_U8 ucType) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_VOID HI_SYS_DisableMMU(HI_VOID) {return ;}
HI_PRVL HI_U32 HI_SYS_MakeScondToCalendar(HI_IN HI_U32 second,HI_OUT HI_SYS_CALENDAR_TIME_S * calendarTime){return HI_ERR_NOT_SUPPORT;}
//HI_EAPI HI_U32 HI_DIAG_ReportUserPlaneMessagePrv(HI_DIAG_USERPLANE_MSG_S* pstMsg, HI_U32 ulSn) {return HI_ERR_NOT_SUPPORT;}
//HI_EAPI HI_U32 HI_DIAG_IsUserPlaneMessageEnablePrv(HI_U32 ulModId, HI_U32 ulId) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DIAG_ReportCommandPrv(HI_U16 usId, HI_U16 usDataSize, HI_PVOID pData) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_BOOL HI_DIAG_IsCommandReportEnalbePrv(HI_VOID) {return HI_FALSE;}
HI_PRVL HI_U32 HI_DIAG_RegisterCommandPrv(HI_CONST HI_DIAG_CMD_REG_OBJ_S* pstCmdTbl, HI_U16 usCmdNum, HI_U8 ucInstanceId) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DIAG_RegisterDRemoteCommandPrv(HI_CONST HI_DIAG_CMD_F pfnProcess) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DIAG_SendAckPacketPrv(HI_U32 ulId, HI_IN HI_PVOID pBuffer, HI_U16 usBufferSize, HI_BOOL bSync, HI_HANDLE hAckInstance) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DIAG_ReportPacketPrv(HI_U32 ulId, HI_U8 ucInstanceId, HI_PBYTE pucPacket, HI_U16 usPacketSize, HI_BOOL bSync) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DIAG_RegistDiagCmdNotifyPrv(HI_U32 ulId, HI_DIAG_CMD_NOTIFY_F * func){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DIAG_ReportStatObjPrv(HI_U16 usId, HI_PVOID pStatObj, HI_U16 usStatObjSize) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DIAG_ReportInitEvtPrv(HI_U16 usEvtId, HI_U8 ucOption, HI_U16 usRc) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DIAG_ReportSysEventPrv(HI_U16 usSysEvtId, HI_U8 ucOption) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DIAG_ReportSysCriticalEventPrv(HI_U32 ulEvtId, HI_U32 ulError) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DIAG_ReportSysMessagePrv(HI_U16 usId, HI_U16 usSrcModId, HI_U16 usDestModId, HI_PVOID pPacket, HI_U16 usPacketSize, HI_U32 ulOption) {return HI_ERR_NOT_SUPPORT;}
#if defined(PRODUCT_CFG_FEATURE_NDM) && !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
HI_PRVL HI_U32 HI_DIAG_RegisterSysStatPrv(HI_CONST HI_DIAG_SYS_STAT_OBJ_S* pstSysStatObjTbl, HI_U16 usObjNum) {return HI_ERR_NOT_SUPPORT;}
#endif
HI_PRVL HI_VOID HI_SYS_RebootPrv(HI_U32 id) {}
HI_PRVL HI_U32 HI_DIAG_RegisterParallelCmdPrv(HI_U16 p1, HI_U16 p2, HI_DIAG_GET_CMD_PK_CACHE_F pfn) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_VOID HI_SYS_FatalVerifyPrv(HI_PCSTR szExp, HI_U32 ulUsrData1, HI_U32 ulUsrData2, HI_PCSTR pszFile, HI_U32 ulLn, HI_PCSTR pszFunc) { }
HI_PRVL HI_VOID HI_SYS_PtrFatalVerifyPrv(HI_PCSTR szExp, HI_U32 ulUsrData1, HI_U32 ulUsrData2, HI_U32 ulUsrData3, HI_U32 ulUsrData4, HI_U32 ulLn, HI_PCSTR pszFunc) { }
HI_PRVL HI_VOID HI_SYS_TaskStackErrAssertPrv(HI_PCSTR pszFile, HI_U32 ulLineNo, HI_PCSTR pszFunction) {return ;}
HI_PRVL HI_U32 HI_DIAG_ReportBufferByHexPrv(HI_U32 ulId, HI_U8* pucBuffer, HI_U32 ulLen, HI_PCSTR pszFile, HI_U32 ulLineNo) {return HI_ERR_NOT_SUPPORT;}
//HI_PRVL HI_U32 HI_DIAG_RegisterSysStat(HI_CONST HI_DIAG_SYS_STAT_OBJ_S* pstSysStatObjTbl, HI_U16 usObjNum){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DIAG_UpdateSysStatCounter(HI_U32* pulSysStatObj) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_BOOL HI_DIAG_IsConnectPrv(HI_VOID) {return HI_FALSE;}
HI_PRVL HI_U32 HI_DIAG_IsPrintfEnablePrv(HI_U32 ulId) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DIAG_IsLayerMessageEnablePrv(HI_U16 usId, HI_U16 usSrcModId, HI_U16 usDestModId, HI_U8 ucType) {return HI_ERR_NOT_SUPPORT;}
#endif
//*****************************************************************************

//*****************************************************************************
// 函数名称: HI_DIAG_LogMsg4
// 功能描述: 
//           Report the UserPlane message to 'User Plane Message Viewer' on PC side.
// 参数说明: 
//           id  [in] LOG消息ID, 即为函数的ID, 显示前缀为函数名称
//           ulModId [in] 低4bit为模块ID, 高12bit为LOG消息的路径序号(函数内部的路径点序号,在同一个函数内部必须唯一)
//           usDataType [in] 数据类型, 取值如下
//                           HI_DIAG_USERPLANE_MSG_DT_LBUF  : 本地buffer, 由DIAG内部的缓存拷贝在后台完成上报(默认)
//                           HI_DIAG_USERPLANE_MSG_DT_GBUF  : buffer为全局的指针,DIAG内部不需要缓存.
//                           HI_DIAG_USERPLANE_MSG_DT_DBUF  : buffer为动态分配的地址,由DIAG发送完毕数据后, 自动释放
//           d0  [in] U32整形数据0
//           d1  [in] U32整形数据1
//           d2  [in] U32整形数据2
//           d3  [in] U32整形数据3
// 返 回 值: 
//           HI_ERR_SUCCESS           表示成功
//           HI_ERR_NO_INITILIZATION  设备未初始化
//           HI_ERR_QUEUE_FULL        OS队列满，分配失败
//           HI_ERR_FAILURE           未知错误
//           HI_ERR_CFG_NOT_ALLOW 日志不允许上报
//           HI_ERR_INVALID_PARAMETER 传参非法
//           HI_ERR_NOT_ENOUGH_MEMORY 无可用的缓存块使用
// 调用要求: 1) 动态分配的内存由调用者释放
//           2) 在.c源代码文件头中加入定义以下语句
//         
// 作    者: 郭成钢/00149569/20090903
//*****************************************************************************
#if defined(PRODUCT_CFG_DIAG_SHELL_API) && !defined(PRODUCT_CFG_FEATURE_UT)
HI_EAPI HI_U32 HI_DIAG_LogMsg0(HI_U32 ulModId, HI_U32 id);
HI_EAPI HI_U32 HI_DIAG_LogMsg1(HI_U32 ulModId, HI_U32 id, HI_U32 d0);
HI_EAPI HI_U32 HI_DIAG_LogMsg2(HI_U32 ulModId, HI_U32 id, HI_U32 d0, HI_U32 d1);
HI_EAPI HI_U32 HI_DIAG_LogMsg3(HI_U32 ulModId, HI_U32 id, HI_U32 d0, HI_U32 d1, HI_U32 d2);
HI_EAPI HI_U32 HI_DIAG_LogMsg4(HI_U32 ulModId, HI_U32 id, HI_U32 d0, HI_U32 d1, HI_U32 d2, HI_U32 d3);
HI_EAPI HI_U32 HI_DIAG_LogBuffer(HI_U32 ulModId, HI_U32 id, HI_U16 usDataType, HI_PVOID pBuffer, HI_U16 usSize);
#elif defined(PRODUCT_CFG_FEATURE_UT) && !defined(PRODUCT_CFG_HAVE_CUSTOM_UT_STUB)
HI_PRVL HI_U32 HI_DIAG_LogMsg0(HI_U32 ulModId, HI_U32 id) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DIAG_LogMsg1(HI_U32 ulModId, HI_U32 id, HI_U32 d0) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DIAG_LogMsg2(HI_U32 ulModId, HI_U32 id, HI_U32 d0, HI_U32 d1) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DIAG_LogMsg3(HI_U32 ulModId, HI_U32 id, HI_U32 d0, HI_U32 d1, HI_U32 d2) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DIAG_LogMsg4(HI_U32 ulModId, HI_U32 id, HI_U32 d0, HI_U32 d1, HI_U32 d2, HI_U32 d3)    {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DIAG_LogBuffer(HI_U32 ulModId, HI_U32 id, HI_U16 usDataType, HI_PVOID pBuffer, HI_U16 usSize)  {return HI_ERR_NOT_SUPPORT;}
#endif
//****************************************************************************

//****************************************************************************
#if defined(PRODUCT_CFG_FEATURE_UT) && !defined(PRODUCT_CFG_HAVE_CUSTOM_UT_STUB)
HI_PRVL HI_U32 HI_DMS_ChlSendMessage(HI_U16 usPortNum, HI_IN HI_DMS_PORT_CTRL_OPT_ID_E eMsgId, HI_IN HI_PVOID pInputParam, HI_OUT HI_PVOID pOutputParam) {return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DMS_ChlSetupInstance(HI_CONST HI_DMS_CHL_PORT_OBJ_S* pstChlPort, HI_U16 usPortNum, HI_U16 usInstanceId) {return HI_ERR_NOT_SUPPORT;}
//HI_EAPI HI_U32 HI_DMS_ChlTxFrm(HI_U16 usPortNum, HI_IN HI_DMS_CHL_TX_DATA_S* pFrameData, HI_IN HI_DMS_CHL_TX_CTRL_S* pFrameCtrl) {return HI_ERR_NOT_SUPPORT;}
#else
HI_EAPI HI_U32 HI_DMS_ChlSetupInstance(HI_CONST HI_DMS_CHL_PORT_OBJ_S* pstChlPort, HI_U16 usPortNum, HI_U16 usInstanceId);
HI_EAPI HI_U32 HI_DMS_ChlSendMessage(HI_U16 usPortNum, HI_IN HI_DMS_PORT_CTRL_OPT_ID_E eMsgId, HI_IN HI_PVOID pInputParam, HI_OUT HI_PVOID pOutputParam);
HI_EAPI HI_U32 HI_DMS_ChlTxFrm(HI_U16 usPortNum, HI_IN HI_DMS_CHL_TX_DATA_S* pFrameData, HI_IN HI_DMS_CHL_TX_CTRL_S* pFrameCtrl);
#endif
//****************************************************************************
#if defined(PRODUCT_CFG_FEATURE_UT)
#define HI_SYS_DisableInterrupts()
#define HI_SYS_EnableInterrupts()
#define HI_SYS_INTS_LOCK()
#define HI_SYS_INTS_UNLOCK()
#endif
//*****************************************************************************
// 检测所有DIAG通道的端口是否可用

// usPort: HI_DMS_CHL_PORT_ANY 
//         HI_DMS_CHL_TCP_PORT_DIAG/HI_DMS_CHL_UART_PORT_DIAG/HI_DMS_CHL_MAC_PORT_DIAG
//*****************************************************************************
HI_EAPI HI_BOOL HI_DIAG_IsPortAvailable(HI_U16 usPortNum);
//****************************************************************************

//*****************************************************************************
// 注册DIAG通道连接和端口通知处理
// 当DIAG通道连接或是断开, DIAG能回调 pfnUserConnect
//*****************************************************************************
HI_PRVL HI_U32 HI_DIAG_RegisterConnect(HI_DIAG_CONNECT_F pfnUserConnect)
{
    if(!HI_IS_VALID_CODE_ADDR(pfnUserConnect))
    {
        return HI_ERR_INVALID_CODE_ADDR;
    }
    else
    {
        return HI_DIAG_RegisterConnectPrv(pfnUserConnect);
    }
}
//*****************************************************************************



//****************************************************************************
// Include the lib to linker for Windows
// 沈汉坤/00130424(2011-10-10)
//****************************************************************************
#if defined(PRODUCT_CFG_OS_WIN) && !defined(PRODUCT_CFG_NO_SAL_LIB_DLL) && !defined(PRODUCT_CFG_HSO)
#if !defined(HAVE_SAL_SRC) && !defined(_LIB)
#undef __MSS_LIBNAME1__ 
#define __MSS_LIBNAME1__   "sal" 
#ifdef _DEBUG
#define __MSS_LIB__        __MSS_LIBNAME1__##"_debug.lib"
#else  
#define __MSS_LIB__        __MSS_LIBNAME1__##"_release.lib"
#endif 
#pragma comment(lib,  __MSS_LIB__)
#if !defined(PRODUCT_CFG_MSVC_HIDE_AUTOLINK_OUTPUT_INFO)
#pragma message("Automatically linking with " __MSS_LIB__)
#endif 
#endif //  HAVE_ODA_SRC
#endif 

#if defined SAL_HAVE_STATIC_LIB
#pragma comment(lib, "shlwapi.lib") 
#pragma comment(lib, "advapi.lib") 
#endif 
//****************************************************************************

#include <hi_sal_nv.h>
HI_U32 HI_SYS_TaskDropCfg(HI_CONST HI_U8 auTaskDropIdxRange0[HI_NV_SAL_TASK_DROP_IDX_RANGE_MAX], HI_CONST HI_U8 auTaskDropIdxRange1[HI_NV_SAL_TASK_DROP_IDX_RANGE_MAX]);


//****************************************************************************
#endif // __HI_SAL_PRV_H__

