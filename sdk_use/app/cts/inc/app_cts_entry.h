//*****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : app_cts_entry.cfg
//  版 本 号   : V1.0 
//  作    者   : 沈汉坤/00130424
//  生成日期   : 2011年09月06日
//  功能描述   : ASL共用接口定义
//  函数列表   : 无
//  修改历史   : 
//  1.日    期 : 2011年09月06日
//    作    者 : 沈汉坤/00130424
//    修改内容 : 创建文件 
//*****************************************************************************

//*****************************************************************************

//*****************************************************************************
// 
// cts cts_HI_MDM_NM_GetAddr 0
// cts cts_HI_MDM_UPG_START 1
//
//*****************************************************************************
typedef HI_U32 (*HI_CTS_OBJ_PROC_FN)(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);

typedef struct
{
   HI_U8 ucType;
   HI_CHAR* pszName;
   HI_CTS_OBJ_PROC_FN pfnObjProc;
}HI_CTS_OBJ_TBL;

#define HI_CTS_OBJ_TYPE_FUNC  0
#define HI_CTS_OBJ_TYPE_MSG   1
#define HI_CTS_FUNC_NAME(x)   #x, x

typedef struct
{
    HI_CHAR szName[64];
    HI_U32 ulId;
    HI_U32 ulOpt;
    HI_U32 ulDataSize;
    HI_U8 aucData[0];
} HI_CTS_REQ_S;


//*****************************************************************************
// entry 
//*****************************************************************************
HI_U32 cts_HI_MDM_NM_GetAddr(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_HI_MDM_NM_GetPlcEth(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_NM_Get_MySnid(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_NM_Set_MySnid(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_NM_Get_FreqPresent(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_NM_Set_Freq(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_NM_Get_Ver(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_NM_Get_StaticProxy(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_NM_Set_StaticProxy(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_NM_Get_TopoInfo(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_NM_Get_RouteInfo(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_NM_Get_DiscoverList(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_NM_Get_BlackWhiteList(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_NM_Set_BlackWhiteList(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_NM_Get_NodeInfo(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_NM_Get_WhiteListSwitch(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_NM_Set_WhiteListSwitch(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_MDM_Get_WhiteListSwitch(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_MDM_Set_WhiteListSwitch(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_HI_MDM_MsgSend(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_test_nv(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_test_nvChangeNotify(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_HI_MDM_Test(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_test_sys(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_HI_MDM_Reboot(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_HI_MDM_RstDelayEnable(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_HI_MDM_RstDelayDisable(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_HI_MDM_Sleep(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_HI_MDM_Start_Timer(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_HI_MDM_Stop_Timer(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_HI_MDM_WaitSem(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_HI_MDM_SignalSem(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_HI_MDM_SendMessage(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_MDM_SendEngEvent(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_MDM_WriteMrsLog(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_MDM_QueryInfo(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_MDM_QueryMacAttr(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_MDM_SetMacAddr(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_MDM_SetBackupMeterAddr(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_MDM_SetSTAProductType(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_MDM_SetValidMacList(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_MDM_SetMacList(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_MDM_QureySendStatus(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
//HI_U32 cts_MDM_EvaluateFreq(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_MDM_LED_SetSearchMeterStatus(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_MDM_LED_SetReadMeterAddrStatus(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_MDM_LED_Set485Led(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_MDM_SetDutMode(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_MDM_WritePowerStep(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_MDM_WriteAfePgaGain(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_MDM_WritePhaseMode(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_MDM_SendNdmReqFrame(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_Crc(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_SetTime(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_SetCalendarTime(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_SetUeStatus(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_SetPlcMac(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_DevIdConfig(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_NDM_Search(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_APPDATA_TRANSMIT(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_DevCfg(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_Chl_Test(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_UPG_START(HI_U8*, HI_U32, HI_U32, HI_U32);
HI_U32 cts_UPG_Send_MdmUpgStart(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_UPG_Send_MdmUpgFileCheck(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_UPG_Send_MdmUpgStop(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_UPG_Send_MdmUpgStatus(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_UPG_ObtainMem(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_UPG_ReleaseMem(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_Mem_Test(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_STR_OPT_TEST(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_SOCKET_TCP_TEST(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_SOCKET_UDP_TEST(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_GPIO_Fun_Test(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_I2C_Test(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_LED_Test(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_Uart_Test(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_GPIO_Interr_Test(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_Flash_Opt_Test(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_SOCKET_Client_Test(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_RegisterReportTopoChangeNode(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_RegisterReportTransformerListRefresh(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_RegisterReportPhaseListRefresh(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_SetTransformerIdentify(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_QueryTransformerList(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_QueryBlackList(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_UPG_File_Load(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_RegisterNdmPingFunc(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_SendNdmPing(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_U32 cts_HI_MDM_SetMacDutyCycleCtrlParam(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_GetEthConfig(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_SetEthConfig(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_OpenSdCardFile(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_CloseSdCardFile(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_WriteSdCardFile(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_ReadSdCardFile(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_IseekSdCardFile(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_UpgradeCellChip(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_ReadCellChipVersion(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_SetWifiSsid(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_GetWifiSsid(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_EnableBeep(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_PrintLattice(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_EnableKey(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_SSI_Init(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_SSI_DevOpen(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_SSI_DevClose(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_SSI_SetMasterCfg(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_SSI_SendData(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_SSI_RcvData(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_SSI_SendRcvData(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_CIPHER_Open(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_CIPHER_Close(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_CIPHER_GetHandle(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_CIPHER_ReleaseHandle(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_CIPHER_Get_Dev(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_CIPHER_Put_Dev(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_CIPHER_ConfigAES(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_CIPHER_ConfigDES(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_CIPHER_Encrypt(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_CIPHER_Decrypt(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_CIPHER_GetState(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_IO_EXP_Set_Dir(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_IO_EXP_Get_Dir(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_IO_EXP_Set_OutputVal(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_IO_EXP_Get_InputVal(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_IO_EXP_Get_OutputVal(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_SetNdmNtbSynchStation(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_CheckNtbIsSynchSuccess(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_CollectNtb(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_RegisterReportRejectNode(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_RegisterReportTopoStatusChg(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_GetDynMemInfo(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_SetRebootDelay(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_GetRstTimes(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_RegisterCapturePktFunction(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_RegistDiagCmdNotify(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_RunDiagCmd(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_CancelCapturePktFunction(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_SearchNetworkCtrl(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_GetCurrentNetworkStatus(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_SetSendSilenceStatus(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_AddRoute(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_DelRoute(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_SetCommunicationMeterAddr(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_SetPowerSupplyMode(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_SetupUIUChl(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_SendFrameToMac(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_GetCommunicationMeterAddr(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_RstRegisterNotify(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_HI_MDM_EnablePowerCutInfo(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_MDM_SetPowerStep(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
HI_S32 cts_MDM_SetAfePgaGain(HI_U8* pucParam, HI_U32 ulParamSize, HI_U32 ulOpt, HI_U32 uObjId);
//*****************************************************************************


#if 0 //c00375526
//*****************************************************************************
// Configure entry to test
//*****************************************************************************
HI_PRV HI_CONST HI_CTS_OBJ_TBL g_stCtsObjTbl[] = {

    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_NM_GetAddr)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_NM_GetPlcEth)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_NM_Get_MySnid)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_NM_Set_MySnid)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_NM_Get_FreqPresent)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_NM_Set_Freq)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_NM_Get_Ver)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_NM_Get_StaticProxy)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_NM_Set_StaticProxy)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_NM_Get_TopoInfo)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_NM_Get_RouteInfo)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_NM_Get_DiscoverList)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_NM_Get_BlackWhiteList)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_NM_Set_BlackWhiteList)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_NM_Get_NodeInfo)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_NM_Get_WhiteListSwitch)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_NM_Set_WhiteListSwitch)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_MDM_Get_WhiteListSwitch)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_MDM_Set_WhiteListSwitch)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_MsgSend)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_test_nv)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_test_nvChangeNotify)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_Test)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_test_sys)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_Reboot)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_RstDelayEnable)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_RstDelayDisable)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_Sleep)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_Start_Timer)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_Stop_Timer)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_WaitSem)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_SignalSem)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_SendMessage)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_MDM_SendEngEvent)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_MDM_WriteMrsLog)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_MDM_QueryInfo)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_MDM_QueryMacAttr)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_MDM_SetMacAddr)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_MDM_SetBackupMeterAddr)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_MDM_SetSTAProductType)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_MDM_SetValidMacList)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_MDM_SetMacList)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_MDM_QureySendStatus)},
//	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_MDM_EvaluateFreq)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_MDM_LED_SetSearchMeterStatus)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_MDM_LED_SetReadMeterAddrStatus)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_MDM_LED_Set485Led)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_MDM_SetDutMode)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_MDM_WritePowerStep)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_MDM_WriteAfePgaGain)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_MDM_WritePhaseMode)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_MDM_SendNdmReqFrame)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_Crc)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_SetTime)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_SetCalendarTime)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_SetUeStatus)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_SetPlcMac)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_DevIdConfig)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_NDM_Search)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_APPDATA_TRANSMIT)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_DevCfg)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_Chl_Test)},
    {HI_CTS_OBJ_TYPE_MSG,  HI_CTS_FUNC_NAME(cts_HI_MDM_UPG_START)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_UPG_Send_MdmUpgStart)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_UPG_Send_MdmUpgFileCheck)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_UPG_Send_MdmUpgStop)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_UPG_Send_MdmUpgStatus)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_UPG_ObtainMem)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_UPG_ReleaseMem)},
	{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_Mem_Test)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_STR_OPT_TEST)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_SOCKET_TCP_TEST)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_SOCKET_UDP_TEST)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_GPIO_Fun_Test)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_I2C_Test)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_LED_Test)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_Uart_Test)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_GPIO_Interr_Test)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_Flash_Opt_Test)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_SOCKET_Client_Test)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_RegisterReportTopoChangeNode)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_RegisterReportTransformerListRefresh)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_RegisterReportPhaseListRefresh)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_SetTransformerIdentify)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_QueryTransformerList)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_QueryBlackList)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_UPG_File_Load)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_RegisterNdmPingFunc)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_SendNdmPing)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_SetMacDutyCycleCtrlParam)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_GetEthConfig)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_SetEthConfig)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_OpenSdCardFile)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_CloseSdCardFile)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_WriteSdCardFile)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_ReadSdCardFile)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_IseekSdCardFile)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_UpgradeCellChip)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_ReadCellChipVersion)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_SetWifiSsid)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_GetWifiSsid)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_EnableBeep)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_PrintLattice)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_EnableKey)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_SSI_Init)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_SSI_DevOpen)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_SSI_DevClose)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_SSI_SetMasterCfg)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_SSI_SendData)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_SSI_RcvData)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_SSI_SendRcvData)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_CIPHER_Open)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_CIPHER_Close)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_CIPHER_GetHandle)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_CIPHER_ReleaseHandle)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_CIPHER_Get_Dev)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_CIPHER_Put_Dev)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_CIPHER_ConfigAES)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_CIPHER_ConfigDES)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_CIPHER_Encrypt)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_CIPHER_Decrypt)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_CIPHER_GetState)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_IO_EXP_Set_Dir)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_IO_EXP_Get_Dir)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_IO_EXP_Set_OutputVal)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_IO_EXP_Get_InputVal)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_IO_EXP_Get_OutputVal)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_SetNdmNtbSynchStation)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_CheckNtbIsSynchSuccess)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_CollectNtb)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_RegisterReportRejectNode)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_RegisterReportTopoStatusChg)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_GetDynMemInfo)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_SetRebootDelay)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_GetRstTimes)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_RegisterCapturePktFunction)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_RegistDiagCmdNotify)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_RunDiagCmd)},    
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_CancelCapturePktFunction)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_SearchNetworkCtrl)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_GetCurrentNetworkStatus)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_SetSendSilenceStatus)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_AddRoute)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_DelRoute)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_SetCommunicationMeterAddr)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_SetPowerSupplyMode)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_SetupUIUChl)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_SendFrameToMac)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_GetCommunicationMeterAddr)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_RstRegisterNotify)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_EnablePowerCutInfo)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_MDM_SetPowerStep)},
    {HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_MDM_SetAfePgaGain)},    
};
#else

HI_PRV HI_CTS_OBJ_TBL g_stCtsObjTbl[] = {
{HI_CTS_OBJ_TYPE_FUNC, HI_CTS_FUNC_NAME(cts_HI_MDM_Crc)},
};

#endif
//*****************************************************************************

//#define CTS_OBJ_TBL g_stCtsObjTbl

//*****************************************************************************

