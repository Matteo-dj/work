#ifndef _HI_EQT_PC_NDM_H_
#define _HI_EQT_PC_NDM_H_

#include "hi_types.h"

//DiagID for test
#define ID_DIAG_CMD_MDM_TEST_LOG                        (0x5B00)
#define ID_DIAG_CMD_MDM_MODE_QUERY                      (0x5B01)
#define ID_DIAG_CMD_MDM_MODE_SET                        (0x5B02)
#define ID_DIAG_CMD_MDM_MAC_SET                         (0x5B03)
#define ID_DIAG_CMD_MDM_MAC_QUERY                       (0x5B04)
#define ID_DIAG_CMD_MDM_ZERO_CROSS_TEST                 (0x5B05)
#define ID_DIAG_CMD_MDM_PLC_TEST                        (0x5B06)
#define ID_DIAG_CMD_MDM_SSID_SET                        (0x5B07)
#define ID_DIAG_CMD_MDM_SSID_QUERY                      (0x5B08)
#define ID_DIAG_CMD_MDM_BQ_LOAD_FROM_SD                 (0x5B09)
#define ID_DIAG_CMD_MDM_BQ_LOAD_RESULT_QUERY            (0x5B0A)
#define ID_DIAG_CMD_MDM_KEY_TEST                        (0x5B0B)
#define ID_DIAG_CMD_MDM_DISPLAY                         (0x5B0C)
#define ID_DIAG_CMD_MDM_EQT_VERSION                     (0x5B0D)
#define ID_DIAG_CMD_MDM_EQT_TEST_INIT                   (0x5B0E)
#define ID_DIAG_CMD_MDM_NTB_SYNC_SET                    (0x5B0F)
#define ID_DIAG_CMD_MDM_NTB_SYNC_QUERY                  (0x5B10)
#define ID_DIAG_CMD_MDM_TF_CARD_TEST                    (0x5B11)
#define ID_DIAG_CMD_MDM_TEST_ERR                        (0x5BFF)

//抄控器工装测试初始化
typedef struct _DIAG_CMD_EQUIP_TEST_INIT_ACK_STRU_
{
    HI_U32 ret;
    HI_U8 ucRsv[16];
}DIAG_CMD_EQUIP_TEST_INIT_ACK_STRU;
typedef struct _DIAG_CMD_EQUIP_TEST_INIT_STRU_
{
    HI_U8 ucType;
    HI_U8 ucRsv[19];
}DIAG_CMD_EQUIP_TEST_INIT_STRU;


//抄控器工装版本号
typedef struct _DIAG_CMD_EQUIP_VERSION_CTRL_ACK_STRU_
{
    HI_U32 ulBoardVersion;
    HI_U8 ucRsv[16];
}DIAG_CMD_EQUIP_VERSION_CTRL_ACK_STRU;
typedef struct _DIAG_CMD_EQUIP_VERSION_CTRL_STRU_
{
    HI_U8 ucRsv[20];
}DIAG_CMD_EQUIP_VERSION_CTRL_STRU;

//测试模式查询
typedef struct _DIAG_CMD_EQUIP_MODE_QUERY_ACK_STRU_
{
    HI_U32 ret; 
    HI_U32 mode;
    HI_U8 ucRsv[12];
}DIAG_CMD_EQUIP_MODE_QUERY_ACK_STRU;
typedef struct _DIAG_CMD_EQUIP_MODE_QUERY_STRU_
{
    HI_U8 ucRsv[20];
}DIAG_CMD_EQUIP_MODE_QUERY_STRU;


//测试模式设置
typedef struct _DIAG_CMD_EQUIP_MODE_SET_ACK_STRU_
{
    HI_U32 ret; 
    HI_U8 ucRsv[16];
} DIAG_CMD_EQUIP_MODE_SET_ACK_STRU;
typedef struct _DIAG_CMD_EQUIP_MODE_SET_STRU_
{
    HI_U32 mode;
    HI_U8 ucRsv[16];
}DIAG_CMD_EQUIP_MODE_SET_STRU;


//MAC设置
typedef struct _DIAG_CMD_EQUIP_MAC_SET_ACK_STRU_
{
    HI_U32 ret; 
    HI_U8 ucRsv[16];
} DIAG_CMD_EQUIP_MAC_SET_ACK_STRU;
typedef struct _DIAG_CMD_EQUIP_MAC_SET_STRU_
{
    HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN];
    HI_U8 ucRsv[14];
}DIAG_CMD_EQUIP_MAC_SET_STRU;

//MAC读取
typedef struct DIAG_CMD_EQUIP_MAC_QUERY_ACK_STRU_
{
    HI_U32 ret; 
    HI_U8 ucPlcMac[HI_PLC_MAC_ADDR_LEN];
    HI_U8 ucRsv[10];
} DIAG_CMD_EQUIP_MAC_QUERY_ACK_STRU;

typedef struct _DIAG_CMD_EQUIP_MAC_QUERY_STRU_
{
    HI_U8 ucRsv[20];
}DIAG_CMD_EQUIP_MAC_QUERY_STRU;


//过零检测
typedef struct _DIAG_CMD_EQUIP_ZERO_CROSS_ACK_STRU_
{ 
    HI_U32 ret; 
    HI_U8 ucLevel;
    HI_U8 ucRsv[3];
    HI_U32 szNtb [20];
} DIAG_CMD_EQUIP_ZERO_CROSS_ACK_STRU;
typedef struct _DIAG_CMD_EQUIP_ZERO_CROSS_STRU_
{
	HI_U8 ucLevel;
    HI_U8 ucRsv[19];
}DIAG_CMD_EQUIP_ZERO_CROSS_STRU;

//灵敏度
typedef struct _DIAG_CMD_EQUIP_PLC_ACK_STRU_
{
    HI_U32 ret;	
    HI_U8 ucRsv[16];
} DIAG_CMD_EQUIP_PLC_ACK_STRU;
typedef struct _DIAG_CMD_EQUIP_PLC_STRU_
{
    HI_U8 ucMacAddr_ndm[HI_PLC_MAC_ADDR_LEN];
    HI_U8 ucRsv[14];
}DIAG_CMD_EQUIP_PLC_STRU;



//SSID Set
typedef struct _DIAG_CMD_EQUIP_SSID_SET_ACK_STRU_
{
    HI_U32 ret; 
    HI_U8 ucRsv[16];
} DIAG_CMD_EQUIP_SSID_SET_ACK_STRU;
typedef struct _DIAG_CMD_EQUIP_SSID_SET_STRU_
{
    HI_CHAR ucSn[20];
}DIAG_CMD_EQUIP_SSID_SET_STRU;

//SSID Query
typedef struct _DIAG_CMD_EQUIP_SSID_QUERY_ACK_STRU_
{
    HI_U32 ret;
    HI_CHAR ucSn[20]; 
    HI_U8 ucRsv[16];
} DIAG_CMD_EQUIP_SSID_QUERY_ACK_STRU;
typedef struct _DIAG_CMD_EQUIP_SSID_QUERY_STRU_
{
    HI_U8 ucRsv[20];
}DIAG_CMD_EQUIP_SSID_QUERY_STRU;


//BQ_LOAD
typedef struct _DIAG_CMD_EQUIP_BQ_LOAD_ACK_STRU_
{
    HI_U32 ret; 
    HI_U8 ucRsv[16];
} DIAG_CMD_EQUIP_BQ_LOAD_ACK_STRU;
typedef struct _DIAG_CMD_EQUIP_BQ_LOAD_STRU_
{
    HI_U8 ucRsv[20];
}DIAG_CMD_EQUIP_BQ_LOAD_STRU;

//BQ Result Query
typedef struct _DIAG_CMD_EQUIP_BQ_RESULT_QUERY_ACK_STRU_
{
    HI_U32 ret; 
    HI_U8 ucRsv[16];
} DIAG_CMD_EQUIP_BQ_RESULT_QUERY_ACK_STRU;
typedef struct _DIAG_CMD_EQUIP_BQ_RESULT_QUERY_STRU_
{
    HI_U8 ucRsv[20];
}DIAG_CMD_EQUIP_BQ_RESULT_QUERY_STRU;

//按键测试
typedef struct _DIAG_CMD_EQUIP_KEY_TEST_ACK_STRU_
{
    HI_U32 ret; 
    HI_U32 keyResponse;
    HI_U8 ucRsv[12];
} DIAG_CMD_EQUIP_KEY_TEST_ACK_STRU;
typedef struct _DIAG_CMD_EQUIP_KEY_TEST_STRU_
{
    HI_U32 periodTime;
    HI_U32 totalTimes;
    HI_U32 keyMask;
    HI_U8 ucRsv[8];
}DIAG_CMD_EQUIP_KEY_TEST_STRU;

//点阵和LED点灯
typedef struct _DIAG_CMD_EQUIP_LED_TEST_ACK_STRU_
{
    HI_U32 ret; 
    HI_U8 ucRsv[16];
} DIAG_CMD_EQUIP_DISPLAY_ACK_STRU;
typedef struct _DIAG_CMD_EQUIP_LED_TEST_STRU_
{
    HI_U32 state;
    HI_U32 period;
    HI_U8 ucRsv[12];
}DIAG_CMD_EQUIP_DISPLAY_STRU;

//NTB同步设置
typedef struct _DIAG_CMD_EQUIP_NTB_SYNC_SET_ACK_STRU_
{
    HI_U32 ret; 
    HI_U8 ucRsv[16];
} DIAG_CMD_EQUIP_NTB_SYNC_SET_ACK_STRU;
typedef struct _DIAG_CMD_EQUIP_NTB_SYNC_SET_STRU_
{
    HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN];
    HI_U8 ucWorkMode;       
    HI_U8 ucSnid;
    HI_U8 ucRsv[12];
}DIAG_CMD_EQUIP_NTB_SYNC_SET_STRU;

//NTB同步结果查询
typedef struct _DIAG_CMD_EQUIP_NTB_SYNC_QUERY_ACK_STRU_
{
    HI_U32 ret; 
    HI_U8 ucRsv[16];
} DIAG_CMD_EQUIP_NTB_SYNC_QUERY_ACK_STRU;
typedef struct _DIAG_CMD_EQUIP_NTB_SYNC_QUERY_STRU_
{
    HI_U8 ucRsv[20];
}DIAG_CMD_EQUIP_NTB_SYNC_QUERY_STRU;

//TfCard测试
typedef struct _DIAG_CMD_EQUIP_TF_CARD_TEST_ACK_STRU_
{
    HI_U32 ret; 
    HI_U8 ucRsv[16];
} DIAG_CMD_EQUIP_TF_CARD_TEST_ACK_STRU;
typedef struct _DIAG_CMD_EQUIP_TF_CARD_TEST_STRU_
{
    HI_U8 ucRsv[20];
}DIAG_CMD_EQUIP_TF_CARD_TEST_STRU;
#endif
