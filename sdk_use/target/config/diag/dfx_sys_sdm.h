//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : dfx_sys_sdm.h
//  �� �� ��   : V1.0
//  ��    ��   : shenhankun/00130424
//  ��������   : 2011-11-10
//  ��������   : ϵͳ����Ͻӿڶ���
//
//  �����б�   : ��
//  �޸���ʷ   :
//  1.��    �� : 2012-08-20
//    ��    �� : shenhankun/00130424
//    �޸����� : �����ļ�
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   :
// SUBSYSTEM :
// MODULE    :
// OWNER     :
//*****************************************************************************

#ifndef __DFX_SYS_SDM_H__
#define __DFX_SYS_SDM_H__

#include "hi_types.h"
#include "hi_mdm_types.h"
#include <hi_ft_nv.h>
#include <dfx_sys.h>
/* BEGIN: Added by likunhe/00194117, 2014/6/10   PN:DTS2014060603580*/
#include "hi_phy_nv.h"
#include "hi_phy_nv_v200.h"
/* END:   Added by likunhe/00194117, 2014/6/10 */

HI_START_HEADER
//*****************************************************************************


//*****************************************************************************
// SDM interface
//*****************************************************************************

#define HI_DFX_RD_2K_MEM_SIZE       2*1024
#define HI_DFX_RD_SMALL_MEM_SIZE    90
#define NM_STAT_SOFT_RESET_INFO_NUM     (5)        // ͳ������������������������Χѭ��ʹ��

/* BEGIN: Added by likunhe/00194117, 2014/6/10   PN:DTS2014060603580*/
#define HI_SDM_FA_EVALUATE_RESULT_FREQ_NUM_MAX      3           //Ƶ�������вδ洢��Ƶ�θ������ֵ
/* END:   Added by likunhe/00194117, 2014/6/10 */

typedef struct
{
    HI_U32 ulAddr;
    HI_U32 ulSize; // *2KB
} HI_DFX_RD_MEM_S;

typedef struct
{
    HI_U32 ulAddr;
}HI_SDM_KLG_MACBUFFER_S;

typedef struct
{
    HI_U32  snid;                        // ����ǰ��snid

    HI_U8  soft_reset_reason;           // MAC������ԭ��
    HI_U8  freq;                        // ����ǰ��Ƶ��
    HI_U8  pad[2];                         // ���ڼ�¼����ϸ�Ŀ�ά�ɲ�
    HI_U32 reset_sys_time;              // MAC�������¼�
}DIAG_CMD_SOFT_RESET_INFO_STRU;

typedef struct
{
    HI_U8  index;                       // ����������
    HI_U8  pad[3];
    DIAG_CMD_SOFT_RESET_INFO_STRU soft_reset_info[NM_STAT_SOFT_RESET_INFO_NUM];
}DIAG_CMD_SOFT_RESET_STRU;

//*****************************************************************************

typedef struct
{
    HI_U32 last_fail_reason;
    HI_U16 call_total_times;
    HI_U16 call_fail_times;
}HI_DIAG_SYNC_STAT_S;
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && !defined(PRODUCT_CFG_EQUIP_TEST_MODE))
HI_EXTERN HI_DIAG_SYNC_STAT_S g_diag_sysnc_stat;
#endif

//*****************************************************************************
// ϵͳ�� ���浽Flash�Ĺؼ���Ϣ
//*****************************************************************************
typedef struct
{
    HI_U32 cnt[10];
    HI_U32 size[10];
}HI_SYS_DFX_NM_ERR_SIZE_LOG_S;

typedef struct
{
    HI_U32 ulMagic;
    HI_U32 ulBootCnt;
    HI_U32 hi_sys_upg_start;
    HI_U32 hi_sys_upg_stop;  //stop �Ĵ���
    HI_U32 nv_init_Phase1;
    HI_U32 nv_init_Phase2;

    HI_U8 hi_sys_upg_update;
    HI_U8 hi_sys_upg_sta_update;//dai shuo
    HI_U8 hi_sys_upg_crc_ok;
    HI_U8 hi_sys_upg_finish;//����ǰ

    HI_U8 hi_sys_upg_ok;// ������
    HI_U8 application_init;
    HI_U8 mac_close_led;
    HI_U8 wdg_enable;

    HI_U8 init_end;
    HI_U8 MAC_RsbTaskInit_ok;
    HI_U8 MAC_TxTaskInit_ok;
    HI_U8 MAC_CsTaskInit_ok;

    HI_U8 MAC_NmMsgTaskInit_ok;
    HI_U8 MAC_NmEvtTaskInit_ok;
    HI_U8 MAC_DfxTaskInit_ok;
    HI_U8 temp;
}HI_SDM_KLG_INIT_S;

typedef enum
{
    HI_RUNSTATUS_10         = 10, // 10 �յ�PLC
    HI_RUNSTATUS_20         = 20, // 20 ����PLC��Ϣ
    HI_RUNSTATUS_30         = 30, // 30 ������
    HI_RUNSTATUS_40         = 40, // 40 �������
    HI_RUNSTATUS_50         = 50, // 50 ��������Ϣ
    HI_RUNSTATUS_60         = 60, // 60 �����ڷ�����Ϣ
    HI_RUNSTATUS_70         = 70, // 70 ���յ����ڱ��ģ���������
    HI_RUNSTATUS_80         = 80, // 80 �����ڽ�����Ϣ
    HI_RUNSTATUS_90         = 90, // 90 ����PLC��Ϣ������
    HI_RUNSTATUS_100        = 100 // 100 ��Ӧ��Ϣ������PLC
}HI_APP_RUNSTATUS;

typedef struct
{
    HI_U32 ulMagic;
    HI_U32 IsNetConnected; // �Ƿ�������
    HI_U32 runStatus;      // ��ǰ����״̬
    HI_U32 runTimeStamp;   // ����״̬ˢ��ʱ��

    //PLCͨ·
    HI_U32 plcMsgId;     // ���һ�ν��յ��ı������͡����ȣ�ʱ��
    HI_U32 plcMsgLen;    // ���һ�ν��յ��ı��ĳ���
    HI_U32 plcMsgTimeStamp; // ���һ�ν��յ��ı���ʱ��
    HI_U32 plcTotalRxBlk; // PLC���յ���֡����


    //UARTͨ·
    HI_U32 uartMsgLen;    // ���һ�ν��յ��ı��ĳ���
    HI_U32 uartMsgTimeStamp; // ���һ�ν��յ��ı���ʱ��

    // ��ǰ����״̬

    // �쳣���
    // ��ʼ��ʧ�ܡ��ڴ�����ʧ��
    //HI_U32 InitFailed;
    //HI_U32 MallocFailedCnt;

    // ���͸�PLCʧ��
    HI_U32 plcSendFailedCnt;
    HI_U32 plcSendMsgFailedCnt;
    // ���ʹ���ʧ��
    HI_U32 uartSendFailedCnt;
    HI_U32 uartSendMsgFailedCnt;

    // ��ǰ��Ϣ����Ԫ�ظ���
    HI_U32 MsgQueenCnt;

} HI_SDM_KLG_APP_S;

typedef enum
{
    HI_SMID_BEACON_FC            = 0,
    HI_SMID_BEACON_PAYLOAD       ,
    HI_SMID_BEACON_FC_B          ,
    HI_SMID_BEACON_PAYLOAD_B     ,
    HI_SMID_BEACON_FC_C          ,
    HI_SMID_BEACON_PAYLOAD_C     ,
    HI_SMID_BEACON_SLOT_MGR_HEAD ,
    HI_SMID_NDM_FRG_RSB_LINK     ,
    HI_SMID_DIAG_BUFFER_PRINT    ,
    HI_SMID_DIAG_BUFFER_MSG      ,
    HI_SMID_DIAG_BUFFER_CMD      ,
    HI_SMID_DIAG_BUFFER_DATA     ,
    HI_SMID_DIAG_BUFFER_EVT      ,
    HI_SMID_DIAG_BUFFER_LOCAL    ,
    HI_SMID_DIAG_BUFFER_PROXY    ,
    HI_SMID_MAC_BUFFER_FREE_LIST    ,
    HI_SMID_MRS_FW_CTX,
    HI_SMID_MRS_IO_CTX,
    HI_SMID_MRS_376PROTO            ,
    HI_SMID_MRS_CCO_CTX            ,
    HI_SMID_MRS_CCO_SRV_METER_INFO            ,
    HI_SMID_MRS_CCO_SRV_METER_FLAG            ,
    HI_SMID_MRS_CCO_SRV_METER_RPT_INFO            ,
    HI_SMID_MRS_COLLECTOR_CTX            ,
    HI_SMID_MAC_PK_FILTER_V1            ,
    HI_SMID_MAC_PK_FILTER_V2            ,
    HI_SMID_MAC_CCO_IP_ENTRY            ,
    HI_SMID_MAC_STA_IP_ENTRY            ,
    HI_SMID_MAC_ROUTE_PATH_TABLE            ,
    HI_SMID_MAC_ROUTE_TABLE            ,
    HI_SMID_UPG_FW_CTX,
    HI_SMID_CCO_UPG_CTX            ,
    HI_SMID_STA_UPG_CTX            ,
    HI_SMID_PLC_MAC_SWITCH          ,

    HI_SMID_MAX = 36
}HI_SMID_E;

#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER) || defined(PRODUCT_CFG_HSO)
#define HI_DFX_SYS_INFO_ERR_FLASH_ERR    1
typedef struct
{
    HI_U32 ulAdd;
    HI_U32 ulSize;
}HI_SDM_GMEM_INFO_S;

typedef struct
{
    HI_U8 *fc;
    HI_U8 *payload;
    HI_U16 pb_size;
    HI_U16 rsvd;
} HI_MAC_BCN_FRM_S;

#define HI_DFX_SAVE_ID_MAC_REJOIN      1
#define HI_DFX_SAVE_ID_NM_NET_FORMED   2
#define HI_DFX_SAVE_ID_TX_RX_PERIOD    3

typedef struct
{
    HI_U8 aucMTskId[HI_SYS_MTSK_MAX_NUM];
    HI_U8 aucMTskStkErr[HI_SYS_MTSK_MAX_NUM];
    HI_U8 aucATskId[HI_SYS_ATSK_MAX_NUM];
    HI_U8 aucATskStkErr[HI_SYS_ATSK_MAX_NUM];
}HI_SYS_TASK_STK_ERR_S;

#if defined(PRODUCT_CFG_SUPPORT_SDM_KLG) || defined(PRODUCT_CFG_SUPPORT_SDM_TM_KLG)
#define HI_SDM_KLG_SYS_MAGIC_CODE 0xA1B2C3D4

typedef struct
{
    HI_U32 ulMagic;
    HI_U32 ulBootCnt;   // boot counter
    HI_U16 bStartup;
    HI_U16 bCloseLed;
    HI_U32 ulRunTimeStamp; // sec

    HI_U32 ulBootTime0; // sec
    HI_U32 ulBootTime1; // sec
    HI_U32 ulRebootCnt; // reboot counter by HI_SYS_ReBoot().
    HI_U32 ulRebootId;
    HI_U32 ulRebootTime; // sec

    HI_U32 ulInt1Cnt[2]; // d/e
    HI_U32 ulInt2Cnt[2]; // d/e
    HI_U32 aulUpgCnt[2]; // ok/err
    HI_U32 ulErr; // flash read err

    HI_U32 ulOsScheduleCnt;
    HI_U32 ulOsTimerRunCnt;
    HI_U32 ulHrLisrTimerCnt;
    HI_U32 ulHrHisrTimerCnt;

    HI_SYS_STATUS_IND_S           stSysStatusInd;
    HI_U32 ulWdEnable;
    HI_U8 aucDbgSync[4];

#if defined(PRODUCT_CFG_SUPPORT_SDM_KLG)
    HI_SYS_TASK_STK_ERR_S stTskStkErr;

    HI_U32 tx_mac_led_on_cnt;
    HI_U32 tx_mac_led_off_cnt;
    HI_U32 tx_app_led_on_cnt;
    HI_U32 tx_app_led_off_cnt;

    HI_U32 rx_mac_led_on_cnt;
    HI_U32 rx_mac_led_off_cnt;
    HI_U32 rx_app_led_on_cnt;
    HI_U32 rx_app_led_off_cnt;

    HI_U32 mac_rejoin_cnt;
    HI_U8 save_id;

    HI_SYS_DFX_NM_ERR_SIZE_LOG_S  stErrSizeLog;
    HI_SDM_GMEM_INFO_S            stGmemInfo[HI_SMID_MAX];
    HI_MAC_BCN_FRM_S              bcn_frm[3];
    HI_U32 ulMacBufferHead;
#endif
} HI_SDM_KLG_SYS_S;
#endif
//*****************************************************************************

//*****************************************************************************
#define HI_SYS_DFX_nm_create_assoc_cnf_OFFSET       0
#define HI_SYS_DFX_nm_create_assoc_ind_OFFSET       1

#if defined(PRODUCT_CFG_SUPPORT_SDM_TM_KLG)
HI_EXTERN HI_SDM_KLG_SYS_S                  g_stSdmKlgSys;
#endif

#if defined(PRODUCT_CFG_SUPPORT_SDM_KLG)
HI_EXTERN HI_SDM_KLG_INIT_S                 g_stSdmKlgInit;
HI_EXTERN HI_SDM_KLG_APP_S                  g_stSdmKlgApp;
HI_EXTERN HI_SDM_KLG_MACBUFFER_S            g_stSdmMacBuffer[];

#define SET_g_stSdmSklgInfo_m(m,v)          g_stSdmKlgSys.m = v
#define SET_g_stSdmSklgInfo_m_inc(m)        g_stSdmKlgSys.m++
#define SET_g_stSdmSklgInit_m(m,v)          g_stSdmKlgInit.m = v
#define SET_g_stSdmSklgInit_m_inc(m)        g_stSdmKlgInit.m ++
#define SET_g_stSdmSklgApp_m(m,v)           g_stSdmKlgApp.m = v
#define SET_g_stSdmSklgApp_m_inc(m)         g_stSdmKlgApp.m++
#else
#define SET_g_stSdmSklgInfo_m(m,v)
#define SET_g_stSdmSklgInfo_m_inc(m)
#define SET_g_stSdmSklgInit_m(m,v)
#define SET_g_stSdmSklgInit_m_inc(m)
#define SET_g_stSdmSklgApp_m(m,v)
#define SET_g_stSdmSklgApp_m_inc(m)
#endif


//*****************************************************************************
// SDM�����
//*****************************************************************************
#define HI_DSID_SDM_BASE           0xB110    // HI_DSID_SDM_CHL_S

#define HI_DSID_SDM_BSP            0xB110    // ,,HI_DSID_SDM_BSP_S,
#define HI_DSID_SDM_CHL            0xB111    // ,,HI_DSID_SDM_CHL_S
#define HI_DSID_SDM_RX             0xB112    // ,,HI_DSID_SDM_RX_S
#define HI_DSID_SDM_TX             0xB113    // ,,HI_DSID_SDM_TX_S
#define HI_DSID_SDM_BCN            0xB114    // ,,HI_DSID_SDM_BCN_S
#define HI_DSID_SDM_NM             0xB115    // ,,HI_DSID_SDM_NM_S
#define HI_DSID_SDM_RT             0xB116    // ,,HI_DSID_SDM_RT_S
#define HI_DSID_SDM_MR             0xB117    // ,,HI_DSID_SDM_MR_S
#define HI_DSID_SDM_DUMP           0xB118    // HI_DSID_SDM_DUMP_S, ,HI_DSID_SDM_XX_S
#define HI_DSID_SDM_EVT            0xB119
#define HI_DSID_SDM_DATA_DUMP      0xB120    // HI_SDM_DATA_DUMP_REQ_S,,HI_SDM_DATA_DUMP_IND_S

#define HI_DSID_SDM_EVT_SEARCH     0xB121
#define HI_DSID_SDM_STORAGE_HEAD   0xB122

#define HI_DSID_SDM_RAM_ERASE      0xB123

/* BEGIN: Added by b00208046, 2013/12/16   PN:DTS2013122602557 */
//0xB130-0xB139 �����ά�ɲ�ʹ��
#define HI_DSID_SDM_MRS_BASE       0xB130
#define HI_DSID_SDM_MLG_LRPR       0xB130
#define HI_DSID_SDM_MLG_LOW_RATIO  0xB131
#define HI_DSID_SDM_MLG_XR         0xB132
#define HI_DSID_SDM_MLG_DAY_FREEZE 0xB133
#define HI_DSID_SDM_MRS_END        0xB133
/* END:   Added by b00208046, 2013/12/16 */
/* BEGIN: Added by b00208046, 2014/3/17   ���ⵥ��:DTS2014012702376 */
#define HI_DSID_SDM_FA             0xB135
#define HI_DSID_SDM_BAT            0xB136
/* END:   Added by b00208046, 2014/3/17 */
#define HI_DSID_SDM_KLG_DUMP       0xB150    // HI_SDM_KLG_DUMP_S
#define HI_DSID_SDM_KLG_SYS        0xB151    // HI_SDM_KLG_SYS_S
#define HI_DSID_SDM_KLG_INIT       0xB152    // HI_SDM_KLG_INIT_S
#define HI_DSID_SDM_KLG_APP        0xB153    // HI_SDM_KLG_APP_S
#define HI_DSID_SDM_KLG_OS         0xB154    // HI_SDM_KLG_OS_S
#define HI_DSID_SDM_KLG_SYSERR     0xB155
#define HI_DSID_SDM_KLG_ALL        0xB156
//*****************************************************************************
// sdm test �����
//*****************************************************************************
#define HI_DSID_SDM_TEST_BASE      0xB160
#define HI_DSID_SDM_TEST_SEVT      0xB161
#define HI_DSID_SDM_TEST_REVT      0xB162
//....����
#define HI_DSID_SDM_TEST_END       0xB169
//*****************************************************************************

//*****************************************************************************

typedef struct
{
    HI_U32 ret;
}HI_SDM_ERASE_RAM_IND_S;

//ħ����
#define MAGIC_NUM_PERIOD 0x5245504d//MPER
#define MAGIC_NUM_EVT    0x54564553//SEVT
#define MAGIC_NUM_MRS    0x53524d4d//MMRS
#define MAGIC_NUM_FA     0x3141464D//MFA1
#define MAGIC_NUM_BAT    0x5441424D//MBAT
#define MAGIC_NUM_NMLV   0x4E4d4c56//NMLV
#if ((!defined(PRODUCT_CFG_BOARD_TYPE_ASIC)) && (defined(PRODUCT_CFG_HW_CHECK)))
#define MAGIC_NUM_FPGA   0x41475046//FPGA
#endif
/* BEGIN: Modified by b00208046, 2014/3/17   ���ⵥ��:DTS2014012702376 */
typedef struct
{
    HI_U32  id;
    HI_U32  hex[6];
    HI_U32  decimal_0;
    HI_U32  decimal_1;
    HI_U32  decimal_2;
    HI_U32  decimal_3;
    HI_U32  decimal_4;
    HI_U32  decimal_5;
} HI_DSID_SDM_TEST_S;//TEST ����
/* END:   Modified by b00208046, 2014/3/17 */
typedef struct
{
    HI_U32 idx;
    HI_U32 magic;
    HI_U32 sec:6;
    HI_U32 min:6;
    HI_U32 hour:5;
    HI_U32 day:5;
    HI_U32 month:4;
    HI_U32 year:6;
    HI_U32 macbpc;
} HI_SDM_ENG_STORAGE_S;

/* BEGIN: Modified by b00208046, 2014/3/17   ���ⵥ��:DTS2014012702376 */
typedef enum
{
    SDM_BSP = 0, // HI_DSID_SDM_BSP_S
    SDM_CHL,     // HI_DSID_SDM_CHL_S
    SDM_RX,      // HI_DSID_SDM_RX_S
    SDM_TX,      // HI_DSID_SDM_TX_S
    SDM_BCN,     // HI_DSID_SDM_BCN_S
    SDM_NM,      // HI_DSID_SDM_NM_S
    SDM_RT,      // HI_DSID_SDM_RT_S
    SDM_MR = 7,  // HI_DSID_SDM_MR_S
    SDM_EVT,     // HI_SDM_ENG_EVT_S

    /* BEGIN: Added by b00208046, 2013/12/16   PN:DTS2013122602557 */
    //�����������
    SDM_MRS_LRPR_STATS = 100,
    SDM_MRS_LOW_RATIO  ,
    SDM_MRS_XR_STATS   ,
    SDM_MRS_DAY_FREEZE_STATS,
    SDM_MRS_ALL,

    SDM_FA  = 120,
    SDM_BAT = 130,
    /* END:   Added by b00208046, 2013/12/16 */

}HI_SDM_CMD_ID_E;//��С������0-255
/* END:   Modified by b00208046, 2014/3/17 */

//*****************************************************************************
typedef enum
{
  HI_SDM_ENG_EVT_VT_ID_NORMAL = 0,
  HI_SDM_ENG_EVT_VT_ID_WIFI_OFF,
  HI_SDM_ENG_EVT_VT_ID_WIFI_ON,
  HI_SDM_ENG_EVT_VT_ID_SD_DC_START,

  HI_SDM_ENG_EVT_VT_ID_NAC_ETH = 32,
  HI_SDM_ENG_EVT_VT_ID_NAC_UART,
  HI_SDM_ENG_EVT_VT_ID_NAC_UART_RX,
  HI_SDM_ENG_EVT_VT_ID_NAC_BQ_UPG,
  HI_SDM_ENG_EVT_VT_ID_NAC_SD_CARD,
  HI_SDM_ENG_EVT_VT_ID_NAC_PLC_SENSITIVE,
  HI_SDM_ENG_EVT_VT_ID_NAC_KEY0, //��������
  HI_SDM_ENG_EVT_VT_ID_NAC_WRITE_BACK_NV,
#if ((!defined(PRODUCT_CFG_BOARD_TYPE_ASIC)) && (defined(PRODUCT_CFG_HW_CHECK)))
  HI_SDM_ENG_EVT_VT_ID_FPGA_CTRL,
#endif

  // TODO:
}HI_SDM_ENG_EVT_VT_ID_E;

/* BEGIN: Modified by b00208046, 2014/3/17   ���ⵥ��:DTS2014012702376 */
typedef struct
{
    HI_SDM_ENG_EVT_VT_ID_E id;
    HI_U32 data[3];//���ԭ�е�ulTemperature,ulVoltage,reserve

} HI_SDM_ENG_EVT_VT_S;

#if ((!defined(PRODUCT_CFG_BOARD_TYPE_ASIC)) && (defined(PRODUCT_CFG_HW_CHECK)))
typedef struct
{
    HI_SDM_ENG_EVT_VT_ID_E id;
	HI_S32 temperature;
	HI_U32 temperatureAlarmTimes;
	HI_U8 fanAlarmTimes;
	HI_U8 activeRstTimes;// ���峤ʱ���¶��쳣�������޷�����������24Сʱ����������
    HI_U8 rsv[2];
} HI_SDM_ENG_EVT_FPGA_S;
#endif

typedef struct
{
    HI_SDM_ENG_EVT_VT_ID_E id;
    HI_U8  flag;
    HI_U8  err_code;
    HI_U16 reserved;
    HI_U32 data[2];
} HI_SDM_ENG_EVT_NAC_S;
typedef struct
{
    HI_U8   bq_ver_old;//BQ�ļ��а汾��
    HI_U8   bq_ver_new;//������BQоƬ�ж����汾��
    HI_U16  reserved;
    HI_U32  ret;//HI_ERR_XXX
} HI_SDM_ENG_EVT_NAC_BQUPG_S;//��ά�ɲ���Ϣ BQ����˽��//default value:  0x0000a1a1
typedef struct
{
    HI_U8   rxDataNum;//UARTУ��ɹ�����
    HI_U8   reserved1;
    HI_U16  reserved2;
    HI_U32  ret;//UART���Դ����� HI_ERR_XXX
} HI_SDM_ENG_EVT_NAC_UART_S;//FLASH��ά�ɲ�˽�нṹ(����) default value: 0x00000003
typedef struct
{
    HI_SDM_ENG_EVT_VT_ID_E id;
    HI_CHAR rxDada1[4];
    HI_CHAR rxDada2[4];
    HI_CHAR rxDada3[4];
} HI_SDM_ENG_EVT_NAC_UART_RX_S; //default value: 0x63614e23(#Nac) 0x63614e23 0x63614e23
typedef struct
{
    HI_CHAR rxData[4];
    HI_U32  ret;
} HI_SDM_ENG_EVT_NAC_SDCARD_S;//default value: 0x54534554
typedef struct
{
    HI_U32  rxBeaconNum;//�յ���Beacon�ĸ���(��ΪbIsBeacon�ĸ�ֵ����)
    HI_U8   bIsBeacon;//MAC����״̬ HI_TRUE���Լ������ԣ�HI_FALSE�ȴ������˳�
    HI_U8   txFailNumber;//����ʧ�ܴ���
    HI_U8   txNumber;//���ʹ���
    HI_U8   reserved;
} HI_SDM_ENG_EVT_NAC_PLC_S;//default value: 0x0000000n  0x00nn0001
typedef struct
{
    HI_SDM_ENG_EVT_VT_ID_E id;
    HI_U8  isKeyInterrupt;
    HI_U8  isResponceKey;
    HI_U8  isPressDown;
    HI_U8  isPressUp;
    HI_U32 data[2];
} HI_SDM_ENG_EVT_NAC_KEY0_S;//default value: 0x01000101
/* END:   Modified by b00208046, 2014/3/17 */
//*****************************************************************************


//*****************************************************************************




// storage access id
typedef enum
{
  SDM_KLG_FLASH_B0 = 0, // 'crash' SDM_KLG flash storage region.
  SDM_KLG_FLASH_B1,     // 'period' SDM_KLG flash storage region.
  SDM_KLG_FLASH_B2,     // 'initializtion' SDM_KLG flash storage region.
  SDM_KLG_FLASH_B3,     // 'reboot by user(NOT rst mgr)' SDM_KLG flash storage region.
  SDM_KLG_RAM     = 8   // SDM KLG in RAM.
}HI_SDM_KLG_SA_ID_E;

typedef enum
{
  SDM_KLG_SYS = 0,
  SDM_KLG_INIT,
  SDM_KLG_APP,
  SDM_KLG_OS,
  SDM_KLG_SYSERR
}HI_SDM_KLG_ID_E;

typedef struct
{
    HI_SDM_KLG_SA_ID_E said;
    HI_SDM_KLG_ID_E scid;
    HI_U32 ulEnableSdmKlg; // ��ǰSDM KLG����ʹ��,�յ�HI_DSID_SDM_KLG_DUMP�����Ĭ�Ϲر�SDM KLG
} HI_SDM_KLG_DUMP_S;

/* BEGIN: Added by b00208046, 2014/1/20   ���ⵥ��:DTS2014010400315 ��ص�����ά�ɲ�*/


//SDM�����ڲ�ʹ�ýṹ��:�������ΪHSO����
/* BEGIN: Added by b00208046, 2014/3/17   ���ⵥ��:DTS2014012702376 */
typedef struct
{
    //HI_SDM_ENG_EVT_CAUSE_E enSysCause;
    HI_U32 aulData[HI_SYS_MSG_PARAM_NUM_MAX_AULDATA];
}HI_SDM_ENG_EVT_PRV_S;
/* END:   Added by b00208046, 2014/3/17 */

HI_EXTERN HI_U32 HI_SDM_SaveCycleEvent(HI_SDM_ENG_EVT_VT_S* pstEvt,HI_BOOL beSync);
#if ((!defined(PRODUCT_CFG_BOARD_TYPE_ASIC)) && (defined(PRODUCT_CFG_HW_CHECK)))
HI_EXTERN HI_U32 HI_SDM_SaveCycleEventFpga(HI_SDM_ENG_EVT_FPGA_S* pstEvt,HI_BOOL beSync);
#endif
/* END:   Added by b00208046, 2014/1/20 */
#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER) || defined(PRODUCT_CFG_HSO)

// storage access id
typedef enum
{
  SDM_FLASH = 0,
  SDM_RAM
}HI_SDM_SA_ID_E;

typedef struct
{
    HI_SDM_SA_ID_E  said;  // storage access id
    HI_SDM_CMD_ID_E scid;  // sdm command id.
    HI_U32 offset;    // reserved
    HI_U32 num;
} HI_DSID_SDM_DUMP_S;

typedef struct
{
    HI_U32 ulId;
    HI_U32 cnt;
    HI_U32 local_time;
    HI_U32 net_time;
    HI_U32 status;
}HI_DSID_SDM_HRD_S;

typedef struct
{
    HI_U32          usSaveTime;                 //�ѷ���
    HI_U32          ulFlashSize;                //flash size
    HI_U32          ulFlashAddr;                //flash basic addr
    HI_U32          pSdmSaveBuffer;             //���ڴ洢�ṹָ��
    HI_U32          magicNum;                   //����ħ����
    HI_U16          ucSysCause;                 //�ѷ���
    HI_U16          sdm_content_arrtibute;       //SDM_CONTENT_ATTRIBUTE_E
    HI_U16          ulSdmSaveBufferSize;        //���ڴ洢�ṹ��С
    HI_U16          ulPerBlockSize;             //ulSdmPeriodSaveBufferSize ���� ulSdmPeriodSaveBufferSize+magic+logic

    HI_U16          ulCurrentWriteBlockIdx;     //��¼��Ҫд���FLASHƫ�ƿ�š�
    HI_U16          ulCurrentLogicBlockIdx;     //��¼��Ҫд���INDEX�š�
    HI_U16          ulMaxWriteBlockIdx;         //���ÿ�
    HI_U16          writeFailTimes;

    HI_U8           sdm_content_type;            //SDM_CONTENT_TYPE_E ����ID
    HI_U8           sdm_blk_id;                  //����flashID,�Թ���FLASH��Ч
    HI_U8           bCreatedSem:1;               //Ϊ1��ʾ�ź��������ɹ�����֧��ͬ�������ķ���������
    HI_U8           bInited:1;                   //Ϊ1��ʾ��ʼ�����
    HI_U8           cur_stat_reserver:6;
    HI_U8           reserver_2;

    HI_U32    		semHandle;                   //�ź���
    HI_U32          semWaitTime;                 //��ȡ�ź�����ʱʱ��ms
}SDM_ENG_CTX_S;
typedef struct
{
    HI_U32 ulWriteFail;
    HI_U32 ulReadFail;
    HI_U32 ulMallocFail;
    HI_U32 MsgSendFailTimes;
    HI_U32 MsgSendSucessTimes;
    HI_U32 MsgReceiveTimes;
}SDM_ENG_DSID_MSG_S;
typedef struct
{
    HI_U32 tx_send_sof_cnt;                       // ���ͻ�����Sof���ĸ���
    HI_U32 tx_send_sack_cnt;                      // ���ͻ�����Sack���ĸ���
    HI_U32 tx_send_beacon_cnt;                    // ���ͻ�����Beacon���ĸ���
    HI_U16 tx_send_sound_cnt;                     // ���ͻ�����Sound���ĸ���
    HI_U16 tx_send_sound_ack_cnt;                 // ���ͻ�����Sound ack���ĸ���
    HI_U16 tx_send_freq_cnt;                      // ���ͻ�����Freq���ĸ���
    HI_U16 tx_send_coordinate_cnt;                // ���ͻ�����Coordinate���ĸ���
    HI_U16 rx_receive_sound_cnt;            // ���ջ�����sound����
    HI_U16 rx_receive_sound_ack_cnt;        // ���ջ�����sound sack����
    HI_U16 rx_receive_freq_cnt;             // ���ջ�����freq����
    HI_U16 rx_receive_coordinate_cnt;       // ���ջ�����coordinate����
    HI_U32 rx_receive_beacon_cnt;              // ���ջ�����Beacon����
    HI_U32 rx_receive_sof_cnt;              // ���ջ�����sof����
    HI_U32 rx_receive_sack_cnt;             // ���ջ�����sack����
    HI_U32 rx_get_macbuffer_success_cnt;    // ���ջ���ȡ����ɹ�����
    HI_U32 rx_get_macbuffer_fail_cnt;       // ���ջ���ȡ����ʧ������
    HI_U16 rx_rcv_not_finished_cnt;         // ���ջ�����δ����ͷŻ�������
    HI_U16 rx_discard_packet_cnt;           // ���ջ�������������
    HI_U32 rsb_receive_cnt;                 // ����ģ�鴦��������
    HI_U32 rsb_reassemble_timeout_cnt;      // ����ģ�����鳬ʱ������������
    HI_U16 rsb_reassemble_fail_cnt;         // ����ģ������ʧ�ܶ�����������
    HI_U16 rsb_crc_fail_cnt;                // ����ģ��У��ʧ�ܶ�����������
    HI_U32 rsb_not_receive_cnt;             // ����ģ��Ǳ�վ�㶪����������
    HI_U32 rsb_abnormity_cnt;               // ����ģ�鱨���쳣������������
    HI_U32 rsb_filter_cnt;                  // ����ģ����˱�������
    HI_U32 rsb_deliver_cnt;                 // ����ģ��ַ���������
    HI_U32 cvg_receive_delivery_cnt;        // ���ģ����շַ���������
    HI_U32 cvg_discard_unicast_cnt;         // ���ģ�鶪��������������
    HI_U32 cvg_transmit_unicast_cnt;        // ���ģ��ת��������������
    HI_U32 cvg_discard_broadcast_cnt;       // ���ģ�鶪���㲥��������
    HI_U32 cvg_transmit_broadcast_cnt;      // ���ģ��ת���㲥��������
    HI_U32 cvg_deliver_app_cnt;             // ���ģ��ַ�APP��������
    HI_U32 cvg_deliver_app_fail_cnt;        // ���ģ��ַ�APP����ʧ������
    HI_U32 cvg_deliver_ip_cnt;              // ���ģ��ַ�IP��������
    HI_U32 cvg_deliver_ip_size_fail_cnt;    // ���ģ��ַ�IP���ı��ĳ���ʧ������
    HI_U32 cvg_deliver_ip_buffer_fail_cnt;  // ���ģ��ַ�IP�������뻺��ʧ������
    HI_U32 cvg_deliver_ip_discard_broadcast_cnt;  // ���ģ��ַ�IP���Ķ����㲥��������
    HI_U32 cvg_deliver_nm_cnt;                    // ���ģ��ַ�NM��������
    HI_U32 cvg_deliver_nm_fail_cnt;               // ���ģ��ַ�NM����ʧ������
    HI_U32 cvg_deliver_invalid_type_cnt;          // ���ģ��ַ�δ֪���ͱ�������
    HI_U32 cvg_receive_transmit_cnt;              // ���ģ�����ת����������
    HI_U32 cvg_receive_transmit_fail_cnt;         // ���ģ�����ת������ʧ������
    HI_U32 cvg_receive_nm_cnt;                    // ���ģ�����NM��������
    HI_U32 cvg_receive_app_cnt;                   // ���ģ�����APP��������
    HI_U16 cvg_receive_app_no_buffer_cnt;         // ���ģ�����APP�����򻺴治�㴦��ʧ������
    HI_U16 cvg_receive_app_queue_overflow_cnt;    // ���ģ�����APP���Ķ������ʧ������
    HI_U32 cvg_receive_ip_cnt;                    // ���ģ�����IP��������
    HI_U32 cvg_receive_ip_no_buffer_cnt;          // ���ģ�����IP�����򻺴治��ʧ������
    HI_U32 cvg_handle_cnt;                        // ���ģ�鴦��������
    HI_U32 cvg_handle_transmit_cnt;               // ���ģ�鴦��ת����������
    HI_U32 cvg_handle_nm_cnt;                     // ���ģ�鴦��NM��������
    HI_U32 cvg_handle_ip_cnt;                     // ���ģ�鴦��IP��������
    HI_U16 cvg_handle_app_cnt;                    // ���ģ�鴦��APP��������
    HI_U16 cvg_handle_invalid_cnt;                // ���ģ�鴦��δ֪���ͱ�������
    HI_U32 cvg_handle_fail_cnt;                   // ���ģ�鴦����ʧ������
    HI_U32 cvg_handle_transmit_route_fail_cnt;    // ���ģ�鴦��ת����������·��ʧ������
    HI_U32 cvg_handle_nm_route_fail_cnt;          // ���ģ��NM��������·��ʧ������
    HI_U32 cvg_handle_app_route_fail_cnt;         // ���ģ��APP��������·��ʧ������
    HI_U32 cvg_handle_ip_route_fail_cnt;          // ���ģ��IP��������·��ʧ������
    HI_U32 cvg_segment_fail_cnt;                  // ���ģ��ֶδ���ʧ������
    HI_U16 pad;                                   // ���ģ��ֶ�����MAC֡ʧ������
    HI_U16 cvg_segment_split_mac_fail_cnt;        // ���ģ��ֶηָ�MACʧ������
    HI_U32 cvg_segment_set_fc_fail_cnt;           // ���ģ��ֶ�����FCʧ������
    HI_U32 cvg_segment_queue_overflow_cnt;        // ���ģ��ֶζ�����ʧ������
    HI_U32 segment_sof_cnt;                       // �ֶ�ģ����ձ�������
    HI_U32 sgm_mpdu_send_success_cnt;             // �ֶ�ģ�����ͳɹ�������������
    HI_U32 sgm_mpdu_send_fail_cnt;                // �ֶ�ģ������ʧ�ܶ�����������
    HI_U32 sgm_mpdu_send_timeout_cnt;             // �ֶ�ģ�����ĳ�ʱ������������
    HI_U32 sgm_mpdu_reset_discard_cnt;            // �ֶ�ģ����ϵͳ������������������
} HI_MAC_INTERFACE_STATISTICS_S;

typedef struct
{
    // ͨ�������ͳ����
    HI_U32 first_notify_time;     // ������ɺ��һ��֪ͨʱ��
    HI_U32 last_true_time;        // ���һ��ͨ���ı�Ϊ������Ϣʱ��
    HI_U32 last_false_time;       // ���һ��ͨ���ı�Ϊ��������Ϣʱ��
    HI_U32 chl_status;            // ��ǰͨ��״̬
    HI_U16 chl_change_true;       // ͨ�����ô���
    HI_U16 chl_change_false;      // ͨ�������ô���
    HI_U32 uart_rx_total_len[2];  // ���ڽ����ֽ���(������Ϣ/������Ϣ)
    HI_U16 uart_rx_ok_num[2];     // ���ڽ�������֡��(������Ϣ/������Ϣ)
    HI_U32 uart_rx_last_time[2];  // �������һ�ν���ʱ��(������Ϣ/������Ϣ)
    HI_U32 uart_tx_total_len[2];  // ���ڷ����ֽ���(������Ϣ/������Ϣ)
    HI_U16 uart_tx_ok_num[2];     // ���ڷ��ͳɹ�֡����������Ϣ/������Ϣ��
    HI_U32 uart_tx_last_time[2];  // �������һ�η���ʱ��(������Ϣ/������Ϣ)
    HI_U16 uart_tx_fail_num[2];   // ���ڷ���ʧ�ܴ���(������Ϣ/������Ϣ)
    HI_U32 uart_tx_err_cause[2];  // �������һ�η���ʧ��ԭ��(������Ϣ/������Ϣ)
    HI_U16 plc_rx_total_num[2];   // PLC����֡����(������Ϣ/������Ϣ)
    HI_U32 plc_rx_last_time[2];   // PLC���һ�ν���ʱ��(������Ϣ/������Ϣ)
    HI_U16 plc_rx_last_id[2];     // PLC���һ�ν���ID(������Ϣ/������Ϣ)
    HI_U32 plc_rx_last_len[2];    // PLC���һ�ν��հ�����(������Ϣ/������Ϣ)
    HI_U16 plc_tx_total_num[2];   // PLC����֡����(������Ϣ/������Ϣ)
    HI_U32 plc_tx_last_time[2];   // PLC���һ�η���ʱ��(������Ϣ/������Ϣ)
    HI_U16 plc_tx_last_id[2];     // PLC���һ�η���ID(������Ϣ/������Ϣ)
    HI_U32 plc_tx_last_len[2];    // PLC���һ�η��Ͱ�����(������Ϣ/������Ϣ)
    HI_U16 plc_tx_fail_num[2];    // PLC����ʧ�ܴ���(������Ϣ/������Ϣ)
    HI_U32 plc_tx_fail_len[2];    // PLC�������һ��ʧ�ܰ�����(������Ϣ/������Ϣ)
    HI_U32 plc_tx_err_cause[2];   // PLC�������һ��ʧ��ԭ��(������Ϣ/������Ϣ)
}HI_MRS_INTERFACE_STATISTICS_S;    // MRS �ⲿ�ӿ�ͳ����

//
// �˵��� �ӿڼ��� ���� MRS-IP-MAC-PHY
//
typedef struct
{
    HI_DSID_SDM_HRD_S hrd;

    // ETH �շ�����
    // --> *�ռ���(�ж�), ����(����)
    // TCP���ռ���, TCP���͸�MAC����
    // SOCKET���Ӹ���

    // ETH �շ�����
    // --> ,
    HI_U32 ulEthRxCnt; //*�ռ���(�ж�)
    HI_U32 ulEthTxCnt;// ���Ͱ�(����)
    HI_U32 ulTcpRxCnt; // TCP���ռ���
    HI_U32 ulTcpTxCnt;// TCP���ͼ���

    HI_U32 ulTcpInErrCnt; //���յ���TCP����������ȷ
    HI_U32 ulIpFragFailsCnt; //ip��Ƭ����

    HI_U32 ulTcpEstabResetsCnt; //����rese������
    HI_U32 ulTcpListenCnt;   //���Ӵ�����
    HI_U32 ulTcpAcceptCnt;   //
    HI_U32 ulTcpLinkFailCnt;

    // MAC�շ�����
    // *IP�����ո���, �ֶμ���, ���͸�PHY����
    HI_MAC_INTERFACE_STATISTICS_S stMacHLInfo; // MAC һ��ͳ����Ϣ,����ͨ·
    HI_U32 eth_to_mac_cnt;
    HI_U32 eth_to_cvg_cnt;              //��̫������۵�IP����
    HI_U32 tx_ip_packet_cnt;            //��۽�����̫����IP����
    HI_U32 sem_to_tx_cnt;               //�ֶ�֪ͨ���ͻ�����
    HI_U32 short_packet_cnt;            //���ͻ����̰���
    HI_U32 long_packet_cnt;             //���ͻ���������
    HI_U32 tx_end_itrupt_cnt;           //��������ж���
    HI_U32 sof_sack_rcv_cnt;            //�յ��Զ˵�sack��

    // *����IP������, PHY���ո���(OK/ERR)
    HI_U32 fc_ok_cnt;                   //fc��ȷ��
    HI_U32 fc_err_cnt;                  //fc������
    HI_U32 pb_ok_cnt;                   //pb��ȷ��
    HI_U32 pb_err_cnt;                  //pb������
    HI_U32 rx_beacon_cnt;               //�յ�beacon��
    HI_U32 rx_end_itrupt_cnt;           //��������ж���
    HI_U32 rx_to_rsb_cnt;               //���ջ�֪ͨ�ظ���
    HI_U32 rx_ip_packet_cnt;            //��۷��͸���̫����IP����
    HI_U32 cvg_to_eth_cnt;              //���֪ͨ��̫�����¼���
    
    // MRS �ⲿ�ӿ�ͳ����
    HI_MRS_INTERFACE_STATISTICS_S mrs_chl_info;
} HI_DSID_SDM_CHL_S;

typedef struct
{
    HI_DSID_SDM_HRD_S hrd;

    // TODO: ҵ���ֳ��ؼ���Ϣ����
    HI_U32 rx_fc_cnt;
    HI_U32 rx_fc_crc_success_cnt;
    HI_U32 rx_fc_crc_err_cnt;
    HI_U32 rx_false_frame;
    HI_U32 rx_beacon_fc_recv_cnt;
    HI_U32 rx_beacon_fc_discard_cnt;
    HI_U32 rx_beacon_pb_crc_err_cnt;
    HI_U32 rx_beacon_pb_crc_success_cnt;
    HI_U32 rx_sof_broadcast_cnt;
    HI_U32 rx_sof_unicast_cnt;
    HI_U32 rx_sof_resend_cnt;
    HI_U32 rx_sof_136_pb_crc_err_cnt;
    HI_U32 rx_sof_136_pb_crc_success_cnt;
    HI_U32 rx_sof_520_pb_crc_err_cnt;
    HI_U32 rx_sof_520_pb_crc_success_cnt;
    HI_U32 detect_nbi_cnt;
    HI_U16 nbi_start[3];
    HI_U16 nbi_end[3];
    HI_U16 imp_noise_cnt;
    HI_U16 rsvd;
} HI_DSID_SDM_RX_S;


typedef struct
{
    HI_DSID_SDM_HRD_S hrd;

    // TODO: ҵ���ֳ��ؼ���Ϣ����
    HI_U32 tx_sof_unicast_cnt;
    HI_U32 tx_sof_broadcast_cnt;
    HI_U32 tx_sof_resend_cnt;
    HI_U32 tx_sof_136_pb_cnt;
    HI_U32 tx_sof_520_pb_cnt;
    HI_U32 tx_tdma_refresh_evt_cnt;
    HI_U32 tx_mpdu_queued_in_evt_cnt;
    HI_U32 tx_period_timeout_evt_cnt;
    HI_U32 tx_sack_received_evt_cnt;
    HI_U32 tx_sound_ack_received_evt_cnt;
    HI_U32 tx_backoff_end_evt_cnt;
    HI_U32 tx_vcs_state_changed_evt_cnt;
    HI_U32 tx_wait_response_timeout_evt_cnt;
    HI_U32 tx_send_success_evt_cnt;
    HI_U32 tx_buffer_clear_evt_cnt;
    HI_U32 tx_multi_send_evt_cnt;
    HI_U32 tx_mem_malloc_fail_cnt;
    HI_U32 tx_get_slot_fail_cnt;
    HI_U32 tx_send_vcs_busy_cnt;
    HI_U32 tx_send_all_queue_null_cnt;
    HI_U32 tx_send_queue_null_cnt;
    HI_U32 tx_start_hr_timer_fail_cnt;
    HI_U32 tx_stop_hr_timer_fail_cnt;

    HI_U32 tx_get_all_msdu_cnt;                   // ���ͻ���ȡ��������
    HI_U32 sof_wait_sack_send_success;            // �ȴ�SACK SOF���ͳɹ�����
    HI_U32 sof_not_wait_sack_send_success;        // ����Ҫ�ȴ�SACK���ķ��ͳɹ�����
    HI_U32 sof_resend_exclude_fix_resend;         // �����ش��������������̶��ش�

    /* BEGIN: Added by zhangguobin, 2015/6/25   ���ⵥ��:˫ģ-��ά�ɲ� */
    HI_U32 beacon_enqueue_cnt;          // BEACON֡����и���
	HI_U32 sof_enqueue_cnt;             // SOF֡����и���
    HI_U32 sack_enqueue_cnt;            // SACK֡����и���
    HI_U32 link_test_cnt;               // ��·���Ա��Ķ��и���

	HI_U32 send_beacon_cnt;             // ����BEACON֡����
	HI_U32 send_sof_cnt;                // ����SOF֡����
	HI_U32 send_sack_cnt;               // ����SACK֡����
	HI_U32 send_short_beacon_cnt;       //���Ͷ�beacon���ĸ���
    HI_U32 send_link_test_cnt;          //������·���Ա��ĸ���

	HI_U32 send_beacon_fragment_cnt;	// ����BEACON֡��Ƭ����
    HI_U32 send_sof_fragment_cnt;		// ����SOF֡��Ƭ����

	HI_U32 backoff_success_cnt;			// �˱ܳɹ�����
	HI_U32 backoff_fail_cnt;			// �˱�ʧ�ܴ���

    HI_U32 send_timeout_cnt;            // ���ͳ�ʱ����
    HI_U16 over_queue_threshold_cnt;    // ��������ˮ�߱��Ķ����ܼ���;
    HI_U16 packet_timeout_err_cnt;      // ���ĳ�ʱ����

    HI_U32 send_command_cnt;            // �����������
    HI_U32 write_tx_buf_cnt;            // д���ͻ������
    HI_U32 sack_timout_cnt;             // SACK��ʱ����
    HI_U32 send_beacon_pb_fail_cnt;     //����beacon pb��ʧ�ܼ���

    HI_U16 write_tx_buf_fail;           // дBUFFʧ�ܼ���
    HI_U8  rf_chip_reset_cnt;           // RFоƬ��������
    HI_U8  rf_send_timeout_reset_cnt;   // ���ͳ�ʱ��������

    HI_U32 rcv_beacon_cnt;              // ����BEACON֡����
	HI_U32 rcv_sof_cnt;                 // ����SOF֡����
	HI_U32 rcv_short_beacon_cnt;        // ���ն�beacon���ĸ���
	HI_U32 rcv_link_test_cnt;           // ���ն���·��ⱨ�ĸ���

	HI_U32 rcv_beacon_fragment_cnt;		// ����BEACON֡��Ƭ����
	HI_U32 rcv_sof_fragment_cnt;		// ����SOF֡��Ƭ����

    HI_U32 beacon_crc_fail_cnt;         // BEACON֡У��ʧ�ܸ���
    HI_U32 short_beacon_fail_cnt;       // ��BEACON֡ʧ�ܸ���
    HI_U32 link_test_fail_cnt;          // ����·��ⱨ��ʧ�ܸ���

	HI_U32 rcv_incomplete_cnt;          // ���ղ��������ĸ���
	HI_U32 rcv_beacon_incomplete_cnt;   // �����ű�֡����������
	HI_U32 rcv_invalid_cnt;             // �����쳣����

    HI_U32 interrupt_cnt;               // �жϸ���
	HI_U32 preamble_interrupt_cnt;		// ǰ���жϸ���
	HI_U32 send_complete_interrupt_cnt;	// ��������жϸ���
	HI_U32 rcv_interrupt_cnt;			// �����жϸ���
    HI_U32 error_cmd_cnt;               // �����������
    HI_U32 fifo_flow_cnt;               // �շ������������
	HI_U32 rcv_sof_incomplete_cnt;      // �����ű�֡����������
    HI_U32 rcv_crc_fail_cnt;            // �ж��ϱ�crc�������
    HI_U32 rcv_rebuild_fail;
    HI_U32 rcv_rebuild_incomplete;
    HI_U32 rcv_return_sack_packe;
    HI_U32 rcv_pkt_length_err;			 // ���ĳ��ȴ���
    HI_U16 rcv_pkt_size_too_short_err;
    HI_U16 rcv_pkt_size_too_long_err;
    HI_U32 rcv_no_self_pkt_cnt;

	HI_U8  switch_to_rf_by_blink_check_cnt;			// ����ä���л���RFģʽ�Ĵ���
	HI_U8  switch_to_rf_by_low_comm_rate_cnt;		// ���ڵ�ͨ�����л���RFģʽ�Ĵ���
	HI_U8  switch_to_plc_by_not_join_network_cnt;	// ����RF�޷����������л���PLCģʽ����
	HI_U8  switch_to_plc_by_not_rcv_beacon_cnt;		// ����RF�޷����յ��ű�֡�л���PLCģʽ����

    HI_U8  switch_to_plc_by_rf_link_exception;      // ����RF��·ͨ���쳣�л���PLCģʽ����
    HI_U8  network_mode_switch_cmd_cnt;             // ����ģʽ�л������л�����
    HI_U16  rf_rout_evaluation_num;                 //RF��·����������

    HI_U32 rcv_self_snid_short_beacon_cnt;          //���յ��Լ�������ű����

	HI_U16 sub_network_snid_conflict_cnt;			// ������ų�ͻ����
	HI_U16 neighbor_network_snid;					// �ھ������
    /* END:   Added by zhangguobin, 2015/6/25 */
    HI_U32 rf_mode_change_comm_rate[12];

	HI_U8  switch_mf_to_plc_by_high_comm_rate_cnt;
	HI_U8  pad[3];
} HI_DSID_SDM_TX_S;

typedef struct
{
    HI_DSID_SDM_HRD_S hrd;

    // TODO: ҵ���ֳ��ؼ���Ϣ����
    HI_U32 bcn_slot_table_update_cnt;
    HI_U32 bcn_last_update_slot_table_time;
    HI_U32 bcn_last_update_slot_table_bpc;
    HI_U32 bcn_last_rcv_beacon_time;
    HI_U32 bcn_last_rcv_beacon_bpc;
    HI_U32 bcn_discard_beacon_slot_cnt;
    HI_U32 bcn_discard_bind_csma_slot_cnt;
    HI_U32 bcn_discard_csma_slot_cnt;
    HI_U32 bcn_discard_tdma_slot_cnt;
    HI_U32 bcn_give_beacon_slot_cnt;
    HI_U32 bcn_give_bind_csma_slot_cnt;
    HI_U32 bcn_give_csma_slot_cnt;
    HI_U32 bcn_give_tdma_slot_cnt;
    HI_U32 bcn_get_repeat_slot_cnt;
    HI_U32 bcn_give_oneself_slot_cnt;
    HI_U32 tx_get_slot_cnt;
    HI_U32 tx_get_slot_fail_cnt;
    HI_U32 phs_evaluate_a_cnt;
    HI_U32 phs_evaluate_b_cnt;
    HI_U32 phs_evaluate_c_cnt;
    HI_U32 synch_timeout_reset_cnt;
    HI_U32 exception_reset_cnt;
    HI_U32 exception_jump_reset_cnt;
    HI_U32 cylce_exception_reset_cnt;
    HI_U32 freqerr_exception_cnt;
    HI_U32 save_freqerr_success_cnt;
    HI_U32 save_freqerr_fail_cnt;
    HI_U32 soft_reset_cnt;
    HI_S16 freqerr_current;
    HI_U16 synch_tei;
    HI_U32  synch_snid;
    HI_U32 chip_state1_cnt;
    HI_U32 chip_state2_cnt;
    HI_U32 chip_state3_cnt;
    HI_U32 freqerr_cnt[36];
} HI_DSID_SDM_BCN_S;

typedef struct _dfx_switch_freq_info_
{
    HI_U16 freq_total_cnt;
    HI_U16 reason;
    HI_U32 last_time;       //��Ƶ�γ���ʱ��
}dfx_switch_freq_info;

typedef struct _dfx_switch_snid_info_
{
    HI_U16 snid_total_cnt;
    HI_U16 status;
    HI_U32 fc_success_cnt;
    HI_U32 beacon_pb_success_cnt;
    //HI_U32 sof_pb_success_cnt;
}dfx_switch_snid_info;

typedef struct _dfx_rt_info_
{
    HI_U16 rt_discover_station_cnt;        // ����վ�����
    HI_U16 rt_near_station_cnt;            // ��CCO��������վ�����
    HI_U16 rt_upper_comm_rate_station_cnt; // ͨ��������վ�����
    HI_U16 rt_middle_comm_rate_station_cnt;// ͨ�����������м�վ�����
    HI_U16 rt_lower_comm_rate_station_cnt; // ͨ��������վ�����
    HI_U16 rt_rcv_discover_pk_cnt;         // ���շ����б������1�����ڣ�
    HI_U16 rt_rcv_bea_cnt;         // �����ű������1�����ڣ�
    HI_U8   rt_proxy_up_comm_rate;           // �����վ������ͨ����
    HI_U8   rt_proxy_down_comm_rate;         // �����վ������ͨ����
    HI_U16 rt_rcv_proxy_beacon_cnt;        // ���մ���վ���ű�֡������1�����ڣ�
    HI_U16 rt_rcv_proxy_discover_pk_cnt;   // ���մ���վ�㷢���б������1�����ڣ�
    HI_U8   proxy_mac[6];
    HI_U8   chose_proxy_mac[6];
    HI_U16   change_proxy_reason;
    HI_BOOL proxy_is_static;
    HI_U8       rsvd;
}dfx_rt_info;


#define DFX_RT_INDEX_MAX 10
/* BEGIN: Deleted by likunhe/00194117, 2014/6/10   ���ⵥ��:DTS2014060603580*/
//#define FREQ_TRY_VALID_NUM          3           //ʵ�ʿ���Ƶ����
/* END: Deleted by likunhe/00194117, 2014/6/10 */

//������ԭ�����λ
enum
{
    PROXY_RATE_IS_LOW = 1<<0,
    PROXY_RATE_IS_MIDDLE = 1<<1,
    PROXY_RATE_IS_UPPER = 1<<2,
    PROXY_IS_FAR_TO_CCO = 1<<3,
    MULTI_PROXY_COMMRATE_BAD = 1<<4,
    HALF_WINDOW_COMM_BAD  = 1<<5,
    NEW_PROXY_LEVEL_IS_SMALL  = 1<<6,
    NEW_PROXY_IS_STA = 1<<7,
    NEW_PROXY_RATE_IS_BIG = 1<<8,
    CHANGE_TO_STATIC_PROXY = 1<<9,
};


typedef struct
{
    HI_DSID_SDM_HRD_S hrd;

    HI_U32  switch_freq_last_systime;         // ���һ����Ƶ��ϵͳʱ��
    dfx_switch_freq_info  switch_freq_info[FREQ_TRY_NUM];// Ƶ���л���¼

    HI_U8   freq_now;                         // ��ǰƵ��
    HI_U8   freq_last;                        // �ϴ�Ƶ��
    HI_U16  last_change_reason;               // �ϴ��л�Ƶ��ԭ��
    HI_U32  switch_snid_last_systime;         // ���һ���л�SNIDϵͳʱ��

    dfx_switch_snid_info  switch_snid_info[15];// SNID�л���¼

    HI_U32  join_first_systime;               // ��һ�ι�������ϵͳʱ��
    HI_U32  join_last_systime;                // ����ϵͳʱ��

    HI_U16  not_bigger_enough_bpc_cnt;          // �޸���bpc�������������
    HI_U16  rcv_bea_period_count;               // ���ռ���beacon���ڵ��ű�
    HI_U16  chose_proxy_fail_cnt;               // ѡ�����ʧ�ܴ���
    HI_U16  assoc_req_cnt;                      // �����������
    HI_U16  cco_reject_cnt;                     // CCO�ܾ��Ĵ���
    HI_U16  cco_reject_reason;                  // CCO�ܾ���ԭ��

    HI_U32   mac_reset_cnt;                     // MAC����������

    HI_U16  join_not_white_cnt;            // ���ڰ������������
    HI_U16  join_level_overflow_cnt;       // �����������
    HI_U16  join_proxy_overflow_cnt;       // ����������
    HI_U16  join_sta_overflow_cnt;         // ��վ��������
    HI_U16  join_total_overflow_cnt;       // �����������
    HI_U16  join_repeat_mac_cnt;           // �ظ�mac����
    HI_U16  join_illege_proxy_cnt;         // �������ڴ���
    HI_U16  join_end_sequence_err;         // end_sequence�������

    HI_U32  cco_recv_req_cnt;              // CCO�յ������ܴ���

    DIAG_CMD_SOFT_RESET_STRU soft_reset;

    HI_U32 nm_handle_assoc_req_cnt;
    HI_U32 nm_handle_assoc_cnf_cnt;
    HI_U32 nm_create_assoc_ind_01_cnt;
    HI_U32 nm_create_assoc_ind_03_cnt;
    HI_U32 nm_create_change_proxy_cnf_cnt;
    HI_U32 nm_handle_change_proxy_cnf_cnt;
    HI_U32 dfx_ndm_handle_hso_cmd_callback_cnt;
    HI_U32 dfx_ndm_pipe_output_cnt;
    HI_U32 dfx_ndm_packet_frame_copy_payload_list_cnt;
    HI_U32 dfx_ndm_handle_packet_cnt;
    HI_U32 dfx_ndm_pipe_send_ack_back_cnt;
    HI_U32 dfx_ndm_pipe_fragment_output_cnt;
    HI_U32 dfx_ndm_fill_reply_payload_cnt;
    HI_U32 Ndc_DRemoteSend_cnt;
    HI_U32 nm_write_nv_fail;

    HI_U8  cco_mac[HI_PLC_MAC_ADDR_LEN];            // ���������CCO MAC��ַ
    HI_U8  lock_status;                             // ����״̬
    HI_U8  lock_status_identify;                    // ̨��ʶ����������״̬

    HI_U32 lock_begin_time;                         // ������ʼʱ��
    HI_U32 lock_duration;                           // ����ʱ��
    HI_U32 reject_time;                             // ��������ܾ�ʱ��

    HI_U16 lock_cnt;                                // ��������
    HI_U16 reject_cnt;                              // ��������ܾ�����
    HI_U16 lock_timeout_cnt;                        // ������ʱ��ʱ
    HI_U16 lock_fail_by_not_join_cnt;               // δ������������ʧ��
    HI_U16 send_msg_fail_cnt;                       // ��Ϣ����ʧ��
    HI_U16 unlock_cnt_by_identify;                  // ̨��ʶ���������(ά��)

    HI_U8  lock_freq_identify;                      // ��������Ƶ�κ�
    HI_U8  lock_snid_identify;                      // �������������
    HI_U8  cco_mac_identify[HI_PLC_MAC_ADDR_LEN];   // ̨��ʶ�����������CCO MAC��ַ

    HI_U32 lock_begin_time_by_identify;             // ������ʼʱ�䣬��λS
    HI_U32 lock_freqsnid_duration_by_identify;      // ����ʱ������λS
    HI_U32 lock_ccomac_duration_by_identify;        // ����CCO MACʱ������λS
    HI_U32 unlock_time_by_identify;                 // ����ʱ��(ά��)
    HI_U16 lock_freqsnid_cnt_by_identify;           // ̨��ʶ������Ƶ�κ�����Ŵ���(ά��)
    HI_U16 lock_ccomac_cnt_by_identify;             // ̨��ʶ������CCO MAC����(ά��)

    HI_U32 lock_freq : 8;                           // ���������Ƶ��
    HI_U32 lock_snid : 24;                          // ���������SNID
} HI_DSID_SDM_NM_S;

typedef struct
{
    HI_DSID_SDM_HRD_S hrd;

    dfx_rt_info  rt_dfx_info[DFX_RT_INDEX_MAX];
    HI_U8    rt_dfx_index;
    HI_U8    rsvd2[2];
    HI_U8   my_static_proxy_exist;
    HI_U8   static_proxy_mac[HI_PLC_MAC_ADDR_LEN];
    HI_U16 rt_fast_proxy_change_cnt;       // ���ٴ���·�ɱ������
    HI_U16 rt_window_proxy_change_cnt;     // �����ڱ������
    HI_U16  lock_static_proxy_cnt;
    HI_U16  unlock_static_proxy_cnt;
    HI_U16  choose_static_proxy_fail_cnt;
    HI_U16  rf_choose_proxy_fial_cnt;
    HI_U16  pad;
} HI_DSID_SDM_RT_S;

typedef struct
{
    HI_DSID_SDM_HRD_S hrd;

    // MRSģ���ڲ����ͳ����
    HI_U8 cco_status;                 // CCO����״̬(�������泭���ѱ�ӳ���)
    HI_U8 sta_status;                 // STA����״̬(��ʼ��������)
    HI_U8 route_ctrl;                 // ·�ɿ���״̬
    HI_U8 vm_flg;                     // �����״̬
    HI_U32 vm_change_num;             // �����ı����
    HI_U32 map_ready_time;            // ӳ���ά�����ʱ��
    HI_U32 heart_num;                 // ��������
    HI_U32 heart_last_time;           // ��һ������ʱ��
    HI_U32 heart_timeout_num;         // ������ʱ����
    HI_U32 heart_timeout_time;        // ��һ��������ʱʱ��
    HI_U32 meter_num;                 // ��������
    HI_U32 LR_req_cnt;                // �ֳ�ѯ�ʶ������
    HI_U32 LR_ack_cnt;                // �ֳ�Ӧ��������
    HI_U32 LR_plc_tx;                 // �ֳ�PLC���ʹ���
    HI_U32 LR_plc_rx;                 // �ֳ�PLC���մ���
    HI_U32 LR_meter_tx;               // �ֳ�����ʹ���
    HI_U32 LR_meter_rx;               // �ֳ������մ���
    HI_U32 LR_plc_timeout;            // �ֳ�PLC��ʱ����
    HI_U32 LR_ok_cnt;                 // �ֳ��ɹ�����
    HI_U32 LR_fail_cnt;               // �ֳ�ʧ�ܴ���
    HI_U32 XR_uart_rx;                // �泭���ڽ��մ���(CCOΪ���ն���֡; STAΪ���յ��֡)
    HI_U32 XR_uart_tx;                // �泭���ڷ��ʹ���(CCOΪ���Ͷ���֡; STAΪ���͵��֡)
    HI_U32 XR_plc_tx;                 // �泭PLC���ʹ���
    HI_U32 XR_plc_rx;                 // �泭PLC���մ���
    HI_U32 XR_nak_num;                // �泭���ϴ���
    HI_U8 XR_nak[4];                  // ���4�η���ԭ��
    HI_U32 XR_plc_timeout;            // �泭PLC��ʱ����
    HI_U32 XR_ok_cnt;                 // �泭�ɹ�����
    HI_U32 XR_fail_cnt;               // �泭ʧ�ܴ���
    HI_U32 PR_uart_rx;                // �������ڽ��մ���(CCOΪ���ն���֡; STAΪ���յ��֡)
    HI_U32 PR_uart_tx;                // �������ڷ��ʹ���(CCOΪ���Ͷ���֡; STAΪ���͵��֡)
    HI_U32 PR_plc_tx;                 // ����PLC���ʹ���
    HI_U32 PR_plc_rx;                 // ����PLC���մ���
    HI_U32 PR_pool_num;               // ������Ԫ�ظ���
    HI_U32 PR_nak_num;                // �������ϴ���
    HI_U8 PR_nak[4];                  // ���4�η���ԭ��
    HI_U32 PR_plc_timeout;            // ����PLC��ʱ����
    HI_U32 PR_ok_cnt;                 // �����ɹ�����
    HI_U32 PR_fail_cnt;               // ����ʧ�ܴ���
    HI_U32 msg_curr_num;              // ��ǰ��Ϣ�������
    HI_U32 msg_max_num;               // �����Ϣ�������
    HI_U32 msg_max_num_time;          // �����Ϣ������ȷ���ʱ��
    HI_U32 msg_total_tx_cnt;          // ��Ϣ�ܷ��ʹ���
    HI_U32 msg_total_rx_cnt;          // ��Ϣ�ܽ��մ���
    HI_U32 msg_last_tx_time;          // ��һ����Ϣ����ʱ��
    HI_U32 msg_last_rx_time;          // ��һ����Ϣ����ʱ��
    HI_U32 msg_last_tx_id;            // ��һ����Ϣ����ID
    HI_U32 msg_last_rx_id;            // ��һ����Ϣ����ID
    HI_U32 msg_tx_fail_cnt;           // ��Ϣ����ʧ�ܴ���
    HI_U32 msg_max_delay;             // ������Ϣ������
    HI_U32 msg_max_delay_id;          // ���������Ϣ������ʱ��ID
    HI_U32 msg_max_delay_time;        // ���������Ϣ������ʱ��
    HI_U32 msg_max_delay_queue_num;   // ���������Ϣ������ʱ�������
    HI_U32 vchl_change_num;           // ģ��ͨ���ı����
    HI_U8 vchl_status[4];             // ģ��ͨ��״̬(����ʷ״̬)
    HI_U32 vchl_last_change_time[4];  // ģ��ͨ��״̬�ı�ʱ��(��ʷ)
    HI_U8 rm_queue_curr_num;          // ������е�ǰ���
    HI_U8 rm_queue_max_num;           // �������������
    HI_U16 rm_queue_first_id;         // ������ж���ID
    HI_U32 rm_queue_lock_time;        // ������ж�������ʱ��
    HI_U32 rm_queue_max_lock_time;    // ������ж����������ʱ��
    HI_U32 timer_fail_cnt;            // ��ʱ������ʧ�ܴ���
    HI_U32 timer_fail_val[4];         // ���4�ζ�ʱ������ʧ��ʱ�Ķ�ʱֵ
    HI_U32 timer_fail_id[4];          // ���4�ζ�ʱ������ʧ��ʱ�Ķ�ʱ��ID
    HI_U32 timer_val_zero_cnt;        // ��ʱ����ʱֵΪ0�Ĵ���
    HI_U32 timer_val_zero_id[4];      // ���4�ζ�ʱ��ֵΪ0�Ķ�ʱ��ID
    HI_U32 timer_val_zero_time[4];    // ���4�ζ�ʱ��ֵΪ0�ķ���ʱ��
    // TODO: ҵ���ֳ��ؼ���Ϣ����
} HI_DSID_SDM_MR_S;

typedef struct
{
    HI_DSID_SDM_HRD_S hrd;

    // TODO: ҵ���ֳ��ؼ���Ϣ����
} HI_DSID_SDM_BOOT_S;


typedef struct
{
    HI_DSID_SDM_HRD_S hrd;
    HI_UPG_FILE_VER_S st_code_ver;      //����汾��Ϣ
    HI_UPG_FILE_VER_S st_nv_ver;        //NV�汾��Ϣ
    HI_U32 ul_boot_ver[2];              //BOOT�汾��Ϣ
    HI_DSID_DRV_UART_INFO_S st_uart[4];
    HI_U32 ul_ir_rx_cnt;                //IR�����ַ�����
    HI_U32 ul_ir_tx_cnt;                //IR�����ַ�����
    HI_U32 ul_ir_parity_err_cnt;        //IR����������żУ��������
    HI_U32 ul_ir_frame_err_cnt;         //IR ��������֡��ʽ�������
    HI_U32 ul_ir_overrun_err_cnt;       //IR ����FIFO����������
    HI_U32 ul_ir_buffer_full_cnt;    //IR ����BUFFER���������
    HI_U32 ul_eth_lisr_int_cnt;         //ETH�жϼ���
    HI_U32 ul_eth_lisr_int_fault;       //ETH�жϴ������
    HI_U32 ul_eth_rx_int_cnt;           //ETH �����жϼ���
    HI_U32 ul_eth_tx_int_cnt;           //ETH �����жϼ���
    HI_U32 ul_eth_pkt_feed_cnt;         //ETH Ӳ��buffer������
    HI_U32 ul_eth_pkt_release_cnt;      //ETH Ӳ��buffer �ͷż���
    HI_U32 ul_eth_pkt_alloc_fail_cnt;   //ETH ����buffer ����ʧ�ܼ���
    HI_U32 ul_eth_pkt_jobadd_err_cnt;   //ETH JOBADD������հ��������
    HI_U32 ul_eth_pkt_rx_len_err_cnt;   //ETH ����֡���������
    HI_U32 ul_eth_tx_not_ready_cnt;     //ETH ����̫æ�����ܼ�ʱ���͵Ĵ���
    HI_U32 ul_wdg_tick_cnt;             //���Ź��߹�����
    HI_DSID_SAL_HSO_GRAB_CONNECT_INFO_S st_hso_grab_connect[HI_DSID_SAL_HSO_GRAB_CONNECT_INFO_COUNT];//HSO��ռ����ռ˫����IP��ַ��ʱ��
    HI_U32 ulHtcpTempAddr;//HSO��ռ:�м�̬����ռ��ַ
    HI_U16 us_hso_grab_count;//HSO��ռ�ܴ���
    HI_U16 rst_data;
    HI_U32 ul_plc2eth_err_cnt;     //
    HI_U32 ul_eth2plc_err_cnt;     //
    HI_U16 us_cipher_int_cnt;       //cipher�жϴ�������
    HI_U16 us_cipher_errbus_cnt;    //cipher���ߴ������
    HI_U16 us_cipher_errlen_cnt;    //cipher���ȴ������
    HI_U16 us_cipher_errcb_cnt;     //�û��ص������������
} HI_DSID_SDM_BSP_S;

typedef struct
{
    HI_DSID_SDM_HRD_S hrd;

    // TODO: ҵ���ֳ��ؼ���Ϣ����
} HI_DSID_SDM_EVT_MAC_S;

typedef struct
{
    HI_DSID_SDM_HRD_S hrd;

    // TODO: ҵ���ֳ��ؼ���Ϣ����
} HI_DSID_SDM_EVT_MR_S;

// cmd: HI_DSID_SDM_DUMP REQ
typedef struct
{
    HI_U16 usCheckSum;
    HI_U16 usProdType;
    HI_U8 ucSdmId;
    HI_U8 ucOpt;
    HI_U8 ucMac[6];//Զ�̵���ʱĿ��mac��ַ
    HI_U32 ulAddr;//�������ݵ�FLASH��ʼ��ַ
    HI_U32 ulSize;//�������ݵĴ�С,ȫFF��ʾ��ն�Ӧ����
} HI_SDM_DATA_DUMP_REQ_S;

typedef struct
{
    HI_U16 usCheckSum;
    HI_U16 usRc;//������
    HI_U32 ulAddr;//Ӧ������ݶ�ӦFLASH��ַ
    HI_U32 ulSize;//Ӧ������ݶ�ӦFLASH��С
    HI_U32 bFinish:1;//�������
    HI_U32 reserved:31;
    HI_U8  aucData[0];//����
} HI_SDM_DATA_DUMP_IND_S;

typedef struct
{
    HI_U32 timestamp;
} HI_SDM_KLG_OS_S;

typedef enum
{
    HI_SDM_TEST_FRME_SWT_NORMAL = 0,
    HI_SDM_TEST_FRME_SWT_ON = 0x01,
    HI_SDM_TEST_FRME_SWT_OFF = 0x02
} HI_SDM_TEST_FRME_SWT_E;

//��Ҫ���ݰ�����ע��
typedef struct
{
    HI_U32 key_event;           //HI_SDM_ENG_EVT_CAUSE_E�У�Ŀǰʹ��HI_SDM_ENG_EVT_CAUSE_MAC_DISCARD_MSDU
    HI_U32 feature_value[2];    //���������ֶε�ֵ����Ӧoffset������2��
    HI_U16 feature_offset[2];   //���������ֶ�ƫ�ƣ���msduͷ���offset��������2��
    HI_U8  feature_size[2];     //���������ֶεĴ�С����������Ϊ4
    HI_U16 save_offset[2];      //Ҫ����ı����е��ֶ�ƫ�ƣ���msduͷ���offset��������2��
    HI_U8  save_size[2];        //Ҫ����ı����е��ֶεĴ�С����Ӧoffset������2��������ֵ֮�Ͳ�����6(�ֽ�)
}HI_SDM_RegFeature_S;
//*****************************************************************************

//*****************************************************************************


#define HI_SDM_ENG_MRS_FLASH_SIZE       (PRODUCT_CFG_CCO_MRLOG_SECTOR_NUM*PRODUCT_CFG_FLASH_BLOCK_SIZE)

#if ((defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)) && defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG) || defined(PRODUCT_CFG_HSO))


#define HI_SDM_ENG_LRPR_IN_DAY          (24)    // �ֳ���������������ͳ�ƴ�����ÿСʱͳ��1�Σ�ÿ�칲24��
#define HI_SDM_ENG_METER_ADDR_LEN       (3)     // �洢���ַ�ĳ��ȣ�ֻ�洢ĩ3�ֽ�
#define HI_SDM_ENG_FRAME_LEN_MAX        (31)    // ����ʧ��ʱ�洢���ĵ���󳤶ȣ�32�ֽ�
#define HI_SDM_ENG_METER_REC_MAX        (50)    // ��¼������������ÿ���洢���޶�������50����
                                                // �ն��᳭��ʧ�ܵĵ������50����ÿʱ�γ����ɹ��ʵ������޵ı�����50��
#define HI_SDM_ENG_LOW_RATIO_IN_DAY     (6)     // ���ڳ������޵��Ĵ�����ÿ4СʱΪ1��ʱ�Σ�ÿ��6��

#define MRS_DAY_FREEZE_FAIL_TIMEOUT     0x01   // �ն������ݳ�����ʱ
#define MRS_DAY_FREEZE_FAIL_ADDRERR     0x02   // �ն������ݳ�����ַ����
#define MRS_DAY_FREEZE_FAIL_DENY        0x04   // �ն������ݳ������ط���֡
#define MRS_DAY_FREEZE_FAIL_INVALID     0x08   // �ն������ݳ����������ݷǷ�
#define MRS_DAY_FREEZE_FAIL_CLOCKERR    0x10   // �ն������ݳ���ʱ�Ӵ���
/* BEGIN: liuxipeng/KF54842 2014/2/20   ���ⵥ��: DTS2014022003115 */
#define MRS_DAY_FREEZE_FAIL_NO_JOIN_NET 0x20   // �ն������ݳ�����ʱ-δ����
/* END  : liuxipeng/KF54842 2014/2/20   ���ⵥ��: DTS2014022003115 */

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_HSO))
typedef enum _HI_SDM_ENDG_MRS_STATS_TYPE_E_
{
    SDM_MRS_MR_STATS_TYPE_XR,
    SDM_MRS_MR_STATS_TYPE_LR,
    SDM_MRS_MR_STATS_TYPE_PR
}HI_SDM_ENG_MRS_MR_STATS_TYPE_E;

typedef struct
{
    HI_U8  statsType;   // ͳ�Ƶĳ������ͣ�1-�ֳ� 2-������
    HI_U8  valid;       // ��Ч�ԣ�1-��Ч������-��Ч
    HI_U16 meterFileCnt;// �����е������
    HI_U16 comRx;       // ���ڽ��ձ����ܴ���
    HI_U16 comTx;       // ���ڷ��ͱ����ܴ���
    HI_U16 plcRx;       // PLC���ճ����ܴ���
    HI_U16 plcTx;       // PLC���ͳ����ܴ���
} HI_SDM_ENG_MRS_MR_STATS_S;

typedef struct
{
    HI_U8  statsType;   // ͳ�Ƶĳ������ͣ�0-͸��
    HI_U8  valid;       // ��Ч�ԣ�1-��Ч������-��Ч
    HI_U16 pad;         // ����ֶ�
    HI_U16 comRx;       // ���ڽ��ձ����ܴ���
    HI_U16 comTx;       // ���ڷ��ͱ����ܴ���
    HI_U16 plcRx;       // PLC���ճ����ܴ���
    HI_U16 plcTx;       // PLC���ͳ����ܴ���
} HI_SDM_ENG_MRS_XR_STATS_S;

typedef struct
{
    HI_SDM_ENG_MRS_MR_STATS_S stats[HI_SDM_ENG_LRPR_IN_DAY];
} HI_SDM_ENG_MRS_LRPR_STATS_S;


typedef struct
{
    HI_U8 meter[HI_SDM_ENG_METER_ADDR_LEN];     // ����ַĩ3�ֽ�
    HI_U8 ratio;                                // �����ɹ����������֣�98.76%��¼Ϊ98
} HI_SDM_ENG_MRS_LOW_RATIO_ITEM_S;

typedef struct
{
    HI_U8 valid;            // ��Ч�ԣ�1-��Ч������-��Ч
    HI_U8 pad;              // ����ֶ�
    HI_U16 meterCount;      // �����ɹ��ʵ������޵������

    // �����ɹ��ʵ������޵����Ϣ��������50��ʱ����ʵ�������¼������50��ʱ��ֻ��¼50�������Ϣ
    HI_SDM_ENG_MRS_LOW_RATIO_ITEM_S item[HI_SDM_ENG_METER_REC_MAX];
} HI_SDM_ENG_MRS_LOW_RATIO_PART_S;

typedef struct
{
    // ��ʱ��ͳ�Ƶĳ����ɹ��ʵ������޵����Ϣ��ÿ���6��ʱ�Σ�ÿʱ��4Сʱ��
    HI_SDM_ENG_MRS_LOW_RATIO_PART_S part[HI_SDM_ENG_LOW_RATIO_IN_DAY];
} HI_SDM_ENG_MRS_LOW_RATIO_S;

typedef struct
{
    HI_U16 total_ratio;  // ���峭���ɹ��ʰٷֱȣ�9876��ʾ�ɹ���98.76%
    HI_U16 meter_ratio;  // �����ɹ����İٷֱȣ�9876��ʾ�ɹ���98.76%
} HI_SDM_ENG_DAY_FREEZE_RATIO_S;

typedef struct
{
    HI_U32 sysTime;                         // ���һ�γ���ʧ��UTCʱ��
    HI_U8 meter[HI_SDM_ENG_METER_ADDR_LEN]; // ʧ�ܱ��ַ��ĩ3���ֽ�
    HI_U8 causeSet;                         // ����ʧ��ԭ�򲢼������ʧ��ԭ��ļ���
    HI_U8 causeLatest;                      // ���һ�γ���ʧ��ԭ��
                                            // 0x01:��ʱ
                                            // 0x02:���ַ����
                                            // 0x04:������֡
                                            // 0x08:645Ӧ���������ݷǷ�
                                            // 0x10:ʱ�Ӳ���
    HI_U8 frame[HI_SDM_ENG_FRAME_LEN_MAX];  // ���һ��ʧ��ʱ��Ӧ��ʧ�ܱ���
                                            // 0x01:������645����
                                            // 0x02:�����б��ַ
                                            // 0x04:�����֡
                                            // 0x08:��645֡��������Ҫ��DI�������
                                            // 0x10:��ʱ�䱨��
} HI_SDM_ENG_DAY_FREEZE_FAIL_ITEM;

typedef struct
{
    HI_SDM_ENG_DAY_FREEZE_RATIO_S ratio;    // �ն��᳭������ɹ���ͳ��
    HI_U32 sysTime;                         // ϵͳUTCʱ��
    HI_U16 failCount;                       // �ն��᳭��ʧ�ܵ������
    HI_U16 pad;                             // ����ֶ�

    // �ն��᳭��ʧ�ܵ����Ϣ��������50��ʱ����ʵ�������¼������50��ʱ��ֻ��¼50�������Ϣ
    HI_SDM_ENG_DAY_FREEZE_FAIL_ITEM item[HI_SDM_ENG_METER_REC_MAX];
} HI_SDM_ENG_DAY_FREEZE_STATS_S;


typedef struct
{
    HI_SDM_ENG_STORAGE_S stStorageHrd;          // ���ڴ洢ͷ�ṹ
    HI_SDM_ENG_MRS_LRPR_STATS_S lrprStats;      // �ֳ�/��������������ͳ��
    HI_SDM_ENG_MRS_LOW_RATIO_S lowRatioStats;   // ���ڳ�ͨ�����޵��ͳ��
    HI_SDM_ENG_MRS_XR_STATS_S xrStats;          // ͸����������ͳ��
    HI_SDM_ENG_DAY_FREEZE_STATS_S dayFreeze;    // �ն�������ͳ��
} HI_SDM_ENG_MRS_LOG_S;
#endif

#endif

/******************************************************************************
Ƶ������Ӧ
*******************************************************************************/

/* BEGIN: Added by likunhe/00194117, 2014/3/18   PN:DTS2014032708937*/
//Ƶ����������洢�ṹ
typedef struct
{
    HI_U32 id                   : 4;					//����
    HI_U32 start_reason         : 8;				    //������ԭ��
    HI_U32 stop_reason          : 8;		            //ֹͣ��ԭ��
    HI_U32 init_freq            : 3;			        //ԭ����Ƶ��
    HI_U32 obj_freq             : 3;			        //��������ʱ�����Ĺ���Ƶ��
    HI_U32 state                : 5;                    //����״̬
    HI_U32 pad                  : 1;					//����

    /* BEGIN: Modified by likunhe/00194117, 2014/6/10   ���ⵥ��:DTS2014060603580*/
    //HI_MAC_FA_FREQ_RESULT_STRU freq_result[3];                   //Ƶ���������
    HI_MAC_FA_FREQ_RESULT_STRU freq_result[HI_SDM_FA_EVALUATE_RESULT_FREQ_NUM_MAX];     //Ƶ���������
    /* END:   Modified by likunhe/00194117, 2014/6/10 */
}HI_SDM_FA_EVALUATE_RESULT_STRU;
/* END:   Added by likunhe/00194117, 2014/3/18 */


//*****************************************************************************


//*****************************************************************************
#define HI_DSID_SDM_BSP_MAX_SIZE      (512)
#define HI_DSID_SDM_CHL_MAX_SIZE      (512)
#define HI_DSID_SDM_RX_MAX_SIZE       (512)
#define HI_DSID_SDM_TX_MAX_SIZE       (512)
#define HI_DSID_SDM_BCN_MAX_SIZE      (512)
#define HI_DSID_SDM_NM_MAX_SIZE       (512)
#define HI_DSID_SDM_RT_MAX_SIZE       (512)
#define HI_DSID_SDM_MR_MAX_SIZE       (496)
//#define HI_DSID_SDM_MR_MAX_SIZE       (512-sizeof(HI_SDM_ENG_STORAGE_S))
#define HI_DSID_SDM_ITEM_MAX_SIZE     (512)

#define HI_SDM_CHECK_PARAM()  do {\
    HI_SDM_VERIFY_EXPR((sizeof(HI_DSID_SDM_BSP_S) <= HI_DSID_SDM_BSP_MAX_SIZE), sizeof(HI_DSID_SDM_BSP_S), 0);\
    HI_SDM_VERIFY_EXPR((sizeof(HI_DSID_SDM_CHL_S) <= HI_DSID_SDM_CHL_MAX_SIZE), sizeof(HI_DSID_SDM_CHL_S), 1);\
    HI_SDM_VERIFY_EXPR((sizeof(HI_DSID_SDM_RX_S)  <= HI_DSID_SDM_RX_MAX_SIZE),  sizeof(HI_DSID_SDM_RX_S),  2);\
    HI_SDM_VERIFY_EXPR((sizeof(HI_DSID_SDM_TX_S)  <= HI_DSID_SDM_TX_MAX_SIZE),  sizeof(HI_DSID_SDM_TX_S),  3);\
    HI_SDM_VERIFY_EXPR((sizeof(HI_DSID_SDM_BCN_S) <= HI_DSID_SDM_BCN_MAX_SIZE), sizeof(HI_DSID_SDM_BCN_S), 4);\
    HI_SDM_VERIFY_EXPR((sizeof(HI_DSID_SDM_NM_S)  <= HI_DSID_SDM_NM_MAX_SIZE),  sizeof(HI_DSID_SDM_NM_S),  5);\
    HI_SDM_VERIFY_EXPR((sizeof(HI_DSID_SDM_RT_S)  <= HI_DSID_SDM_RT_MAX_SIZE),  sizeof(HI_DSID_SDM_RT_S),  6);\
    HI_SDM_VERIFY_EXPR((sizeof(HI_DSID_SDM_MR_S)  <= HI_DSID_SDM_MR_MAX_SIZE),  sizeof(HI_DSID_SDM_MR_S),  7);\
}while(0)


#define HI_LEAVE_PLC_MAC_ADDR_OFFSET    (2)
/* BEGIN: Added by zhangguobin, 2015/3/26   ���ⵥ��:CQ_MODIFY */
#define HI_LEAVE_PLC_MAC_ADDR_LEN	(HI_PLC_MAC_ADDR_LEN - 2)
typedef struct
{
	HI_U8  mac[HI_LEAVE_PLC_MAC_ADDR_LEN];	// վ��MAC��ַ(Ϊ�˽�ʡ�洢�ռ䣬MAC��ַ���洢��4���ֽ�)

	HI_U32 leave_start_time;				// ���߿�ʼʱ�䣨13������Ϊ����ʱ�䣬09������Ϊϵͳ����ʱ�䣩
	HI_U32 leave_time : 16;					// ����ʱ������λ�����ӣ�65535���ӣ��ɱ�ʾ45�죬��վ������ʱ�䳬��45�죬��洢�����
	HI_U32 leave_reason : 4;				// ����ԭ��
	HI_U32 sw_reset_reason : 6;			    // ������ԭ��
	HI_U32 hw_reset_reason : 6;				// Ӳ����ԭ��
}HI_SDM_LEAVE_INFO_S;

#define HI_SDM_MAX_LEAVE_ENTRY_NUM  ((PRODUCT_CFG_FLASH_BLOCK_SIZE - sizeof(HI_SDM_ENG_STORAGE_S)) / sizeof(HI_SDM_LEAVE_INFO_S))
typedef struct
{
    HI_SDM_ENG_STORAGE_S stStorageHrd;
    HI_SDM_LEAVE_INFO_S leaveinfo[HI_SDM_MAX_LEAVE_ENTRY_NUM];
}HI_SDM_ENG_NMLV_LOG_S;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
HI_EXTERN HI_SDM_ENG_NMLV_LOG_S g_stSdmEngNmlvLog;
HI_EXTERN HI_SDM_ENG_NMLV_LOG_S* HI_SDM_NMLV_LOG_GPTR;
#define HI_SDM_NMLV_LOG_GPTR     ((HI_SDM_ENG_NMLV_LOG_S*)&g_stSdmEngNmlvLog)
#endif
/* END:   Added by zhangguobin, 2015/3/26 */


//*****************************************************************************

//*****************************************************************************
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
HI_EXTERN HI_SDM_ENG_MRS_LOG_S  g_stSdmEngMrsLog;
HI_EXTERN HI_SDM_ENG_MRS_LOG_S* HI_SDM_MRS_LOG_GPTR;
#define HI_SDM_MRS_LOG_GPTR     ((HI_SDM_ENG_MRS_LOG_S*)&g_stSdmEngMrsLog)
#endif



typedef enum
{
    SD_DRV_DFX_API_GET_FIRMWARE,
    SD_DRV_DFX_API_CHECK_EXIST,
    SD_DRV_DFX_API_GET_DISK_SIZE,
    SD_DRV_DFX_API_DISK_FORMAT,
    SD_DRV_DFX_API_GET_DIR_FIRST,
    SD_DRV_DFX_API_GET_DIR_NEXT,
    SD_DRV_DFX_API_MAKE_DIR,
    SD_DRV_DFX_API_ENTER_DIR,
    SD_DRV_DFX_API_BACK_ROOT,
    SD_DRV_DFX_API_MAKE_FILE,
    SD_DRV_DFX_API_REMOVE_FILE,
    SD_DRV_DFX_API_FILE_OPEN,
    SD_DRV_DFX_API_FILE_CLOSE,
    SD_DRV_DFX_API_FILE_GET_LEN,
    SD_DRV_DFX_API_FILE_READ,
    SD_DRV_DFX_API_FILE_CLEAR,
    SD_DRV_DFX_API_FILE_APPEND,
    SD_DRV_DFX_API_NOTIFY_CUR_TIME,
    SD_DRV_DFX_API_NOTIFY_GET_DIR_NUM,
    SD_DRV_DFX_API_DISK_OTHER_MAX,// TODO:���õĽӿ�ID��ʱ�������棬������ͳ��

    SD_DRV_DFX_API_REMOVE_DIR,
    SD_DRV_DFX_API_FILE_SEEK,
    SD_DRV_DFX_API_DIRECTORY_RENAME,
}SD_DRV_DFX_API_E;

typedef struct
{
    HI_U16 usIdleMaxWaitTime;
    HI_U16 usReadyMaxWaitTime;
    HI_U8 ucIdleTimeOutTimes;
    HI_U8 ucReadyTimeOutTimes;
    HI_U8 reserver[2];

}SD_DRV_API_DFX_S;

//*****************************************************************************


//*****************************************************************************
#define DEAD_CHECK

typedef enum    _DFX_PK_SIZE_EXCEPTION_POS_E_
{
    nm_handle_assoc_req_ERR_FUNC = 0 ,
    nm_handle_assoc_cnf_ERR_FUNC ,
    nm_create_assoc_ind_01_ERR_FUNC ,
    nm_create_assoc_ind_02_ERR_FUNC ,
    nm_create_assoc_ind_03_ERR_FUNC ,
    nm_create_change_proxy_cnf_ERR_FUNC ,
    nm_handle_change_proxy_cnf_ERR_FUNC ,
    nm_create_change_key_ind_ERR_FUNC ,
    //plc_mac_output_NU_ERR_FUNC ,
    dfx_ndm_handle_hso_cmd_callback_ERR_FUNC ,
    dfx_ndm_packet_frame_ERR_FUNC ,
    dfx_ndm_packet_frame_copy_payload_list_ERR_FUNC ,
    dfx_ndm_handle_packet_ERR_FUNC ,
    dfx_ndm_pipe_send_ack_back_ERR_FUNC ,
    dfx_ndm_pipe_fragment_output_ERR_FUNC ,
    dfx_ndm_fill_reply_payload_ERR_FUNC ,
    //Ndc_DRemoteSend_ERR_FUNC
}DFX_PK_SIZE_EXCEPTION_POS_E;

#define MAX_PK_EXCEPTION_CNT 15//(17-2)

typedef struct
{
    HI_U16  type;                    // 0:INDICATION , 1: CNF, 2: REQ, 3:RSP
    HI_U16  channelType;             //������ͨ��������
    HI_U16  usCmdId;                 // ����ID
    HI_U16  pad;

    HI_U32 payload_size;            //���Ĵ�С
}DFX_NDM_PK_EXCEPTION_INFO_S;
typedef struct
{
    HI_U16  pk_mmtype;              // ����mtype
    HI_U16  ostei;                  // ����mmtype
    HI_U16  odtei;                  // ��������
    HI_U16  pad;

    HI_U32  payload_size;           //���Ĵ�С
} DFX_NM_PK_EXCEPTION_INFO_S;

typedef union
{
    DFX_NDM_PK_EXCEPTION_INFO_S ndm_pk_excpetion_info;
    DFX_NM_PK_EXCEPTION_INFO_S nm_pk_excpetion_info;
}DFX_PK_EXCEPTION_INFO_U;
typedef struct _dfx_mac_check_pk_err_status_
{
    DFX_PK_SIZE_EXCEPTION_POS_E err_pos; // �쳣����λ��
    DFX_PK_EXCEPTION_INFO_U err_pk_info; // �쳣������Ϣ
}DFX_MAC_CHECK_PK_ERR_STATUS_S;

#define flash_block_size 4096
#define SDM_PK_INFO_SIZE sizeof(DFX_MAC_CHECK_PK_ERR_STATUS_S)
#define MAX_SDM_PK_NUM (flash_block_size/SDM_PK_INFO_SIZE)


typedef struct
{
    HI_U8 aucPkBuffer[MAX_SDM_PK_NUM][SDM_PK_INFO_SIZE];
} HI_SDM_MAC_ERR_PK_CACHE_S;


HI_EXTERN HI_SDM_MAC_ERR_PK_CACHE_S g_stMac_check_pk_info;
HI_EXTERN HI_U32 g_sdm_index;


//*****************************************************************************
// �����ӿڶ���
//*****************************************************************************
HI_EXTERN HI_VOID HI_SDM_KlgPkinfoSave(DFX_MAC_CHECK_PK_ERR_STATUS_S *pk_exception_info);

//*****************************************************************************

HI_EXTERN HI_VOID HI_SDM_KlgCrashSave(HI_VOID);

#if defined(PRODUCT_CFG_SUPPORT_SDM_TM_KLG)
HI_EXTERN HI_VOID HI_SDM_KlgRebootSave(HI_U32 ulId);
#endif

#if defined(PRODUCT_CFG_SUPPORT_SDM_KLG)
HI_EXTERN HI_VOID HI_SDM_KlgCtxInit(HI_VOID);
HI_EXTERN HI_VOID HI_SDM_KlgInitPhase1(HI_VOID);
HI_EXTERN HI_VOID HI_SDM_KlgInitSave(HI_BOOL bReboot);
HI_EXTERN HI_VOID HI_SDM_KlgChkErrSave(HI_U32 id);
HI_EXTERN HI_VOID HI_SDM_KlgRebootSave(HI_U32 ulId);
HI_EXTERN HI_VOID HI_SDM_KlgRebootInit(HI_VOID);
HI_EXTERN HI_VOID HI_SDM_KlgOsSave(HI_PVOID pBuffer, HI_U32 ulSize);
HI_EXTERN HI_VOID HI_SDM_KlgTskPeriodSave(HI_VOID);
HI_EXTERN HI_VOID HI_SDM_KlgChkMacBufferValid(HI_PVOID pAnyMacBuf, HI_U32 ulUsedSize, HI_U32 ulLn, HI_PCSTR pszFunc);
HI_EXTERN HI_VOID HI_SDM_KlgCheckMacBufferInit(HI_VOID);
HI_EXTERN HI_BOOL HI_SDM_KlgIsEnable(HI_VOID);
HI_EXTERN HI_VOID HI_SDM_KlgSetEnable(HI_BOOL bEnable);
#define HI_SDM_KLG_CHK_MAC_BUF_PTR(pAnyMacBuf, ulUsedSize)      HI_SDM_KlgChkMacBufferValid(pAnyMacBuf, ulUsedSize, __LINE__, __FUNCTION__)
#define HI_SDM_KLG_NM_KEY_INFO(offset, v)                       g_stSdmKlgSys.stErrSizeLog.size[offset] = v
#define HI_SDM_KLG_NM_KEY_INFO_INC(offset,v)                    HI_SAL_INC_U32(g_stSdmKlgSys.stErrSizeLog.cnt[offset]); HI_SDM_KLG_NM_KEY_INFO(offset, v)
#define HI_SDM_KLG_INFO_INC(offset,m)                           HI_SAL_INC_U32(g_stSdmKlgSys.m[offset])
#else
#define HI_SDM_KlgCtxInit()                 do {} while(0)
#define HI_SDM_KlgInitPhase1()              do {} while(0)
#define HI_SDM_KlgInitSave(bReboot)         do {} while(0)
//#define HI_SDM_KlgBufListSave()             do {} while(0)
#define HI_SDM_KlgCrashSave()               do {} while(0)
#define HI_SDM_KlgChkErrSave(id)            do {} while(0)
#if !defined(PRODUCT_CFG_SUPPORT_SDM_TM_KLG)||defined(PRODUCT_CFG_FEATURE_ST_PHY)
#define HI_SDM_KlgRebootSave(ulId)          do {} while(0)
#endif
#define HI_SDM_KlgRebootInit()              do {} while(0)
#define HI_SDM_KlgOsSave(pBuffer, ulSize)   do {} while(0)
#define HI_SDM_KlgTskPeriodSave()           do {} while(0)
#define HI_SDM_KlgCheckMacBufferInit()      do {} while(0)
#define HI_SDM_KlgIsEnable()                HI_FALSE
#define HI_SDM_KlgSetEnable(bEnable)        do {} while(0)
#define HI_SDM_KLG_CHK_MAC_BUF_PTR(pAnyMacBuf, ulUsedSize)                 do {} while(0)
#define HI_SDM_KLG_NM_KEY_INFO(offset, v)                                  do {} while(0)
#define HI_SDM_KLG_NM_KEY_INFO_INC(offset,v)                               do {} while(0)
#define HI_SDM_KLG_INFO_INC(offset,m)                                      do {} while(0)
#endif




#if defined(PRODUCT_CFG_SUPPORT_SDM_ENG)
HI_EXTERN HI_U32 HI_SDM_RegisterMsduSaveFeature(HI_SDM_RegFeature_S *pstFeature);
#else
HI_PRVL HI_U32 HI_SDM_RegisterMsduSaveFeature(HI_SDM_RegFeature_S *pstFeature)  { return HI_ERR_NOT_SUPPORT; }
#endif
//*****************************************************************************


//*****************************************************************************
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)&& defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
HI_EXTERN HI_U32 HI_SDM_WriteMrsLog(HI_BOOL isIdxAdd);
#else
HI_PRVL HI_U32 HI_SDM_WriteMrsLog(HI_BOOL isIdxAdd)  { HI_UNREF_PARAM(isIdxAdd); return HI_ERR_NOT_SUPPORT; }
#endif
//*****************************************************************************
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_EXTERN HI_U32 HI_SDM_WriteNmlvLog(HI_BOOL isIdxAdd);
HI_EXTERN HI_VOID HI_SDM_NmlvInit(HI_U32 address,HI_U32 size);
#else
HI_PRVL HI_U32 HI_SDM_WriteNmlvLog(HI_BOOL isIdxAdd)  { HI_UNREF_PARAM(isIdxAdd); return HI_ERR_NOT_SUPPORT; }
HI_PRVL HI_VOID HI_SDM_NmlvInit(HI_U32 address,HI_U32 size){ HI_UNREF_PARAM(address); HI_UNREF_PARAM(size); return; }
#endif


#if defined(PRODUCT_CFG_CHECK_TASK_STACK_ERR)
HI_EXTERN HI_VOID HI_SDM_TskStakChkErr(HI_SYS_TASK_STK_ERR_S* pstStkErr);
#else
HI_PRVL HI_VOID HI_SDM_TskStakChkErr(HI_SYS_TASK_STK_ERR_S* pstStkErr) {HI_UNREF_PARAM(pstStkErr);}
#endif


#if defined(PRODUCT_CFG_CHECK_TASK_STACK_ERR_V2)
HI_EXTERN HI_VOID HI_SDM_CheckAllStack(HI_VOID);
#else
HI_PRVL HI_VOID HI_SDM_CheckAllStack(HI_VOID) {}
#endif

#if defined(PRODUCT_CFG_HAVE_OS_ALL_TASK_INFO)
HI_EXTERN HI_PVOID HI_SDM_GetOsKrlInfo(HI_VOID);
HI_EXTERN HI_U32 HI_SDM_GetOsKrlInfoSize(HI_VOID);
#else
HI_PRVL HI_PVOID HI_SDM_GetOsKrlInfo(HI_VOID)         { return HI_NULL; }
HI_PRVL HI_U32 HI_SDM_GetOsKrlInfoSize(HI_VOID)      { return HI_ERR_NOT_SUPPORT; }
#endif
HI_VOID HI_SDM_QueryOsKrlBasicInfo(HI_VOID);
HI_VOID HI_SDM_SetBootVer(HI_CHAR* pszBootVer);
#endif // PRODUCT_CFG_CORE_TYPE_BOOTLOADER
//*****************************************************************************


//*****************************************************************************
// HI_SDM_CheckTestFrameSwt
//*****************************************************************************
HI_SDM_TEST_FRME_SWT_E HI_SDM_CheckTestFrameSwt (HI_VOID);

//*****************************************************************************
//
// ulSdmId ȡֵΪ
//
// HI_DSID_SDM_CHL
// HI_DSID_SDM_RX
// HI_DSID_SDM_TX
// HI_DSID_SDM_BCN
// HI_DSID_SDM_NM
// HI_DSID_SDM_MR
// HI_DSID_SDM_KLG_ALL
// HI_DSID_SDM_KLG_SYSERR
//
// ģ���е����ؼ���Ϣ
HI_U32 HI_SDM_ExportData(HI_U32 ulSdmId, HI_PVOID pData, HI_U32 ulDataSize);
HI_U32 HI_SDM_NdmImportData(HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN], HI_U32 ulSdmId, HI_PVOID pData, HI_U32 ulDataSize);
//*****************************************************************************

typedef struct
{
    HI_U32 ulTxOkNum;
    HI_U32 ulTxErrNum;
    HI_U32 ulRxOkNum;
    HI_U32 ulRxErrNum;
} HI_SDM_TEST_FRAME_INFO_S;


HI_U32 HI_SDM_SendTestFrame(HI_PVOID pFrame, HI_U32 ulFrameSize, HI_U8 ucFrameFlag, HI_U8 ucOption);
HI_U32 HI_SDM_GetTestFrameInfo (HI_SDM_TEST_FRAME_INFO_S* pstTestFrameInfo);

#endif // PRODUCT_CFG_CORE_TYPE_BOOTLOADER
//*****************************************************************************




//*****************************************************************************
HI_END_HEADER
#endif //__DFX_SYS_H__


