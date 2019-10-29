#include <hi_mdm.h>

typedef struct
{
    HI_BOOL aucRemoteFlag;
    HI_U8 aucDesMacAddr[HI_PLC_MAC_ADDR_LEN];
    HI_U8 pad;
}HI_MDM_NM_CTRL_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
}HI_MDM_TEST_NV_PARAM_S;

typedef struct
{
    HI_U16 minId;
    HI_U16 maxId;
}HI_MDM_TEST_NV_Change_PARAM_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
}HI_MDM_TEST_PARAM_S;

typedef struct
{
    HI_U32 reboot_reason;
}HI_MDM_Reboot_PARAM_S;

typedef struct
{
    HI_U8 RstDelayID;
    HI_U32 time;
}HI_MDM_RstDelay_PARAM_S;

typedef struct
{
    HI_U32 reboot_reason;
}HI_MDM_TEST_REBOOT_PARAM_S;

typedef struct
{
    HI_U32 ulmiltime;
}HI_MDM_Sleep_PARAM_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
    HI_U32 ulPeriod;
    HI_U8 mod;
}HI_MDM_Timer_PARAM_S;

typedef struct
{
    HI_U32 timeout;
}HI_MDM_WaitSem_PARAM_S;
typedef struct
{
    HI_U8 option;
}HI_MDM_GetEthConfig_S;
typedef struct
{
	HI_U8 option;
    HI_SYS_ETH_CONFIG_S stEthCfgInfo;
}HI_MDM_SetEthConfig_S;
typedef struct
{
    HI_U8 option;
}HI_MDM_OpenSdCardFile_S;
typedef struct
{
    HI_U8 option;
}HI_MDM_CloseSdCardFile_S;
typedef struct
{
    HI_U8 option;
}HI_MDM_ReadSdCardFile_S;
typedef struct
{
    HI_U8 option;
}HI_MDM_WriteSdCardFile_S;
typedef struct
{
    HI_U8 option;
}HI_SSI_Init_S;
typedef struct
{
    HI_U8 option;
}HI_MDM_CIPHER_Open_S;
typedef struct
{
    HI_U8 option;
	HI_U8 daes_alg;
	HI_U8 daes_work_mode;
	HI_U8 daes_bit_width;
	HI_U8 daes_key_length;
}HI_MDM_CIPHER_ConfigAES_S;
typedef struct
{
    HI_U8 option;
	HI_U8 daes_alg;
	HI_U8 daes_work_mode;
	HI_U8 daes_bit_width;
	HI_U8 daes_key_length;
}HI_MDM_CIPHER_ConfigDES_S;
typedef struct
{
    HI_U8 option;
}HI_SSI_DevOpen_S;
typedef struct
{
    HI_U8 option;
}HI_SSI_DevClose_S;
typedef struct
{
    HI_U8 option;
	HI_U8 dataWidth;
	HI_U8 framMode;
	HI_U8 spo;
	HI_U8 sph;
	HI_U8 endian;
	HI_U8 waitVal;
	HI_U8 waitEn;
	HI_U32 baudRate;
}HI_SSI_SetMasterCfg_S;
typedef struct
{
    HI_U8 option;
}HI_SSI_SendData_S;
typedef struct
{
    HI_U8 option;
}HI_SSI_RcvData_S;
typedef struct
{
    HI_U8 option;
}HI_SSI_SendRcvData_S;
typedef struct
{
    HI_U8 option;
	char offset;
	char whence;
}HI_MDM_IseekSdCardFile_S;
typedef struct
{
    HI_U8 option;
}HI_MDM_UpgradeCellChip_S;
typedef struct
{
    HI_U8 option;
}HI_MDM_ReadCellChipVersion_S;
typedef struct
{
    HI_U8 option;
}HI_MDM_EnableWifi_S;
typedef struct
{
    HI_U8 option;
}HI_MDM_SetWifiSsid_S;
typedef struct
{
    HI_U8 option;
}HI_MDM_GetWifiSsid_S;
typedef struct
{
    HI_U8 option;
}HI_MDM_EnableBeep_S;
typedef struct
{
    HI_U8 option;
}HI_MDM_PrintLattice_S;
typedef struct
{
    HI_U8 option;
}HI_MDM_EnableKey_S;
typedef struct
{
    HI_U32 option;
    HI_U8   data[8];
    HI_U32 timeout;
}HI_MDM_SendMessage_PARAM_S;

typedef struct
{
//    HI_SDM_ENG_EVT_S event; //HI_SDM_ENG_EVT_S deleted
}HI_MDM_SendEngEvent_PARAM_S;

typedef struct
{
    HI_BOOL bFlag;
}HI_MDM_WriteMrsLog_PARAM_S;

typedef struct
{
    HI_U8 ulOption;
}HI_MDM_CRC_PARAM_S;

typedef struct
{
    HI_U8 ulOption;
    HI_U32 timeS;
}HI_MDM_SetTime_PARAM_S;

typedef struct
{
    HI_U8 ulOption;
    HI_SYS_CALENDAR_TIME_S calendarTime;
}HI_MDM_SetCalendarTime_PARAM_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
    HI_U8  status;
    HI_U8  rsvd[3];
}HI_MDM_NM_Set_SetUeStatus_PARAM_S;

typedef struct
{
//    HI_MDM_NM_CTRL_S ctrl;
    HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN];
    HI_U8 rsvd[2];
}HI_MDM_NM_SetPlcMac_PARAM_S;
typedef struct
{
	HI_U8 opt;
    HI_U8 freq_index;
    HI_U8 snid;	
    HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN];
    HI_U8 rsvd[7];
}HI_MDM_SetNdmNtbSynchStation_S;
typedef struct
{
    HI_U8 opt;
    HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN];
    HI_U8 rsvd;
}HI_MDM_SetCommunicationMeterAddr_S;
typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;

    HI_U8  aucDevIdBcd[6];
    HI_U8 rsvd[2];
}HI_MDM_NM_SetDEVID_PARAM_S;

typedef struct
{
    HI_U8 aucMacAddr[HI_PLC_MAC_ADDR_LEN];
    HI_U8 aucIpAddr[HI_IPV4_ADDR_LEN];
}HI_MDM_NM_GetAddr_PARAM0_S;

typedef struct
{
   HI_U8 aucMacAddr[HI_PLC_MAC_ADDR_LEN];
   HI_U8 aucIpAddr[HI_IPV4_ADDR_LEN];
}HI_MDM_NM_GetAddr_PARAM2_S;

typedef struct
{
   HI_U32 ulDataSize;
   HI_U8 auData[1024];
}HI_MDM_NM_GetAddr_PARAM1_S;

typedef struct
{
    HI_U8 aucMacAddr[HI_PLC_MAC_ADDR_LEN];
    HI_U8 aucIpAddr[HI_IPV4_ADDR_LEN];
    HI_U8 aucNetMask[HI_IPV4_ADDR_LEN];
}HI_MDM_NM_GetPlcEth_PARAM_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
}HI_MDM_NM_Get_MySnid_PARAM_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
    HI_MDM_NM_SNID_SET_REQ_S set_snid;
}HI_MDM_NM_Set_MySnid_PARAM_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
}HI_MDM_NM_Get_FreqPresent_PARAM_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
    HI_MDM_NM_FREQ_SET_REQ_S set_freq;
}HI_MDM_NM_Set_Freq_PARAM_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
}HI_MDM_NM_Get_Ver_PARAM_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
}HI_MDM_NM_Get_StaticProxy_PARAM_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
    HI_MDM_NM_STATIC_PROXY_SET_REQ_S set_staticproxy;
}HI_MDM_NM_Set_StaticProxy_PARAM_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
    HI_MDM_NM_TOPO_REQ_S topo;
}HI_MDM_NM_Get_TopoInfo_PARAM_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
    HI_MDM_NM_ROUTER_INFO_REQ_S get_route;
}HI_MDM_NM_Get_RouteInfo_PARAM_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
    HI_MDM_NM_DISCOVER_LIST_REQ_S get_discover;
}HI_MDM_NM_Get_DiscoverList_PARAM_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
    HI_MDM_NM_WHITE_LIST_QRY_REQ_S get_blackwhite;
}HI_MDM_NM_Get_BlackWhiteList_PARAM_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
    HI_U16 usOption;
    HI_U16 usId;
    HI_U16 usTotalItemNum;
    HI_U16 usItemNum;
    HI_MDM_NM_WHITE_LIST_ITEM_S astItem;
}HI_MDM_NM_Set_BlackWhiteList_PARAM_S_1;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
    HI_U16 usOption;
    HI_U16 usId;
    HI_U16 usTotalItemNum;
    HI_U16 usItemNum;
    HI_MDM_NM_WHITE_LIST_ITEM_S astItem[2];
}HI_MDM_NM_Set_BlackWhiteList_PARAM_S_2;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
    HI_U8 usOption;
}HI_MDM_MsgSend_PARAM_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
    HI_U8 usOption;
	HI_U8 aucDesMacAddr[HI_PLC_MAC_ADDR_LEN];
}HI_MDM_MsgSend_PARAM2_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
}HI_MDM_NM_Get_NodeInfo_PARAM_S;


typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
}HI_MDM_NM_Get_WhiteList_Switch_PARAM_S;


typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
    HI_MDM_NM_WHITE_LIST_SWITCH_SET_REQ_S whitelist_switch;
}HI_MDM_NM_Set_WhiteList_Switch_PARAM_S;

typedef struct
{
    HI_U8 option;
}HI_MDM_Get_WhiteList_Switch_PARAM_S;

typedef struct
{
	HI_U8 option;
	HI_U8 pad;
    HI_U16 delaytime;
}HI_MDM_SetRebootDelay_S;

typedef struct
{
    HI_U8 isOn;
    HI_U8 isPermanent;
    HI_U32 reason;
}HI_MDM_Set_WhiteList_Switch_PARAM_S;

typedef struct
{
    HI_U8 mac[6];
	HI_U16 pad;
}HI_MDM_QueryMacAttr_S;

typedef struct
{
    HI_U8 mac[6];
	HI_U8 type;
}HI_MDM_SetMacAddr_S;

typedef struct
{
	HI_U8 type;
}HI_MDM_SetSTAProductType_S;

typedef struct
{
    HI_U32 reason;
	HI_U16 list_num;
	HI_U16 pad;
	HI_MAC_ENTRY_STRU  mac_list[2];
}HI_MDM_SetValidMacList_S;

/*
typedef struct
{
    HI_U8 start;
}HI_MDM_SetValidMacList_S;

typedef struct
{
    HI_U8 start;
}HI_MDM_SetVldMacList_S;
*/

typedef struct
{
    HI_U8 option;
}HI_MDM_RegisterReportTopoChangeNode_S;

typedef struct
{
    HI_U8 option;
}HI_MDM_RegisterReportTransformerListRefresh_S;

typedef struct
{
    HI_U8 option;
}HI_MDM_RegisterReportPhaseListRefresh_S;

typedef struct
{
    HI_U8 option;
}HI_MDM_SetTransformerIdentify_S;

typedef struct
{
	HI_U8 option;
	HI_U8 pad;
    HI_U16 num;
}HI_MDM_QueryTransformerList_S;

typedef struct
{
	HI_U8 option;
	HI_U8 pad;
    HI_U16 num;
}HI_MDM_QueryBlackList_S;

typedef struct
{
    HI_U8 lid;
	HI_U8 pad[3];
}HI_MDM_QureySendStatus_S;

typedef struct
{
    HI_U32 cmd;
}HI_MDM_EvaluateFreq_S;

typedef struct
{
    HI_U8 status;
	HI_U8 pad[3];
}HI_MDM_SetSearchMeterStatus_S;

typedef struct
{
    HI_U8 CtrlFlag;
    HI_U8 SetFreq;
	HI_U8 pad[2];
}HI_MDM_SearchNetworkCtrl_s;
typedef struct
{
    HI_U8 option;
    HI_U8 NetDev;
	HI_U8 aucDstAddr[4];
    HI_U8 aucNetMask[4];
    HI_U8 aucGateway[4];
    HI_U8 pad[2];
}HI_MDM_AddRoute_s;
typedef struct
{
    HI_U8 option;
	HI_U8 aucDstAddr[4];
    HI_U8 pad[3];
}HI_MDM_DelRoute_s;
typedef struct
{
    HI_U8 status;
	HI_U8 pad[3];
}HI_MDM_SetReadMeterAddrStatus_S;

typedef struct
{
    HI_U8 ontime;
	HI_U8 pad[3];
}HI_MDM_Set485Led_S;

typedef struct
{
    HI_S8 PowerStep0;
    HI_S8 PowerStep1;
    HI_S8 PowerStep2;
	//HI_U8 pad;
    HI_S8 PowerStep3;
    HI_S8 PowerStep4;
}HI_MDM_WritePowerStep_S;

typedef struct
{
    HI_S8 PowerStep0;
    HI_S8 PowerStep1;
    HI_S8 PowerStep2;
	//HI_U8 pad;
    HI_S8 PowerStep3;
    HI_S8 PowerStep4;
}HI_MDM_SetPowerStep_S;

typedef struct
{
    HI_U8 AfePgaGain;
	HI_U8 pad[3];
}cts_MDM_WriteAfePgaGain_S;

typedef struct
{
    HI_U8 AfePgaGain;
	HI_U8 pad[3];
}cts_MDM_SetAfePgaGain_S;

typedef struct
{
    HI_U8 list_num;
    HI_U8 clear_flag;
    HI_U8 type;
    HI_U8 reserved;
}cts_MDM_SetBackupMacAddr_S;

typedef struct
{
    HI_U8 phase_cnt;
	HI_U8 default_phase;
	HI_U8 pad[2];
}cts_MDM_WritePhaseMode_S;

typedef struct
{
    HI_U16 usPacketId;
    HI_U16 usPacketSize;
    HI_U8 aucMacAddr[HI_PLC_RELAY_NUM_MAX][HI_PLC_MAC_ADDR_LEN];
    HI_U32 ulSn;
}cts_MDM_SendNdmReqFrame_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
    HI_MDM_NET_SEARCH_CONN_REQ_S para;
}HI_MDM_SEARCH_CONN_PARAM_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
    HI_MDM_APPDATA_TRANSMIT_UL_S para;
}HI_MDM_APPDATA_TRANSMIT_PARAM_S;

typedef struct
{
    HI_U8 mod;
}HI_MDM_DevCfg_PARAM_S;
typedef struct
{
    HI_U8 mod;
}HI_MDM_Chl_Test_PARAM_S;
typedef struct
{
    HI_U8 option;
	HI_U8 id;
}HI_MDM_RegisterNdmPingFunc_S;
typedef struct
{
    HI_U8 option;
	HI_U8 id;
	HI_U8 paload;
	HI_U16 paloadsize;
}HI_MDM_SendNdmPing_S;
typedef struct
{
    HI_U8 option;
	HI_U8 duty_enable;
	HI_U8 duty_percent;
	HI_U32 max_sendlength;
}HI_MDM_SetMacDutyCycleCtrlParam_S;
typedef struct
{
    HI_U8 option;
	HI_U8 ucLevel;
}HI_MDM_CollectNtb_S;
typedef struct
{
    HI_U32 ulUpgId;
    HI_U32 ulOption;
    HI_U32 ulUpgTimeWindow;
	HI_U32 ulUpdateOption;
}UPG_Send_MdmUpgStart_PARAM_S;

typedef struct
{
  HI_MDM_UPG_STOP_REQ_S  stop_req;
}UPG_Send_MdmUpgStop_PARAM_S;

typedef struct
{
  HI_MDM_UPG_STATUS_REQ_S  status_req;
}UPG_Send_MdmUpgStatus_PARAM_S;

typedef struct
{
    HI_U32 ulUpgId;
    HI_U32 ulFileSize;    
}HI_MDM_UPG_ObtainMem_PARAM_S;

typedef struct
{
    HI_U32 ulUpgId;
}HI_MDM_UPG_ReleaseMem_PARAM_S;

typedef struct
{
    HI_U32 ulUpgId;
    HI_U32 ulFileSize; 
//    HI_U32 ulUpdateFileSize;
}UPG_File_Load_PARAM_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
}HI_MDM_Mem_Test_PARAM_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
}HI_MDM_STR_OPT_TEST_PARAM_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
    HI_U8 OptChoice[1];
}HI_MDM_SOCKET_TCP_TEST_PARAM_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
    HI_U8 OptChoice[1];
}HI_MDM_SOCKET_UDP_TEST_PARAM_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
    HI_U8 OptChoice[1];
}HI_MDM_SOCKET_CLIENT_TEST_PARAM_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
    HI_U8 OptChoice[1];
}HI_MDM_GPIO_FUN_TEST_PARAM_S;

typedef struct
{
//    HI_MDM_NM_CTRL_S ctrl;
    HI_U8 OptChoice[1];
}HI_MDM_I2C_TEST_PARAM_S;
typedef struct
{
//    HI_MDM_NM_CTRL_S ctrl;
    HI_U8 OptChoice;
	HI_U8 enIoNum;
	HI_U8 enDir;
}HI_MDM_IO_EXP_Set_S;
typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
    HI_U8 OptChoice[1];
}HI_MDM_LED_TEST_PARAM_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
    HI_U8 OptChoice[1];
}HI_MDM_UART_TEST_PARAM_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
    HI_U8 OptChoice[1];
}HI_MDM_GPIO_INTERR_TEST_PARAM_S;

typedef struct
{
    HI_MDM_NM_CTRL_S ctrl;
    HI_U8 OptChoice[1];
}HI_MDM_FLASH_OPT_TEST_PARAM_S;
