//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : dfx_sys.h
//  �� �� ��   : V1.0
//  ��    ��   : shenhankun/00130424
//  ��������   : 2011-11-10
//  ��������   : ϵͳ����ϰ�ID�Ͷ�Ӧ�����ṹ����
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


#ifndef __DFX_SYS_H__
#define __DFX_SYS_H__

#include "hi_types.h"
#include "hi_mdm_types.h"
#include <hi_ft_nv.h>
#include "hi_phy_nv.h"
#include "hi_phy_nv_v200.h"

HI_START_HEADER


//*****************************************************************************
// ϵͳ�¼�����Ϣ��ͳ������Χ����, ��ϸ�μ� �ĵ� <<Hi3911V100 ���ϵͳ����Ϸ������˵����.docx>>
//*****************************************************************************
#define HI_DEID_MIN  0
#define HI_DEID_MAX  1024
#define HI_DMID_MIN  0x0001
#define HI_DMID_MAX  0xFFFF
#define HI_DSID_MIN  0xA000
#define HI_DSID_MAX  0xF000
#define HI_DCID_MIN  0xA000
#define HI_DCID_MAX  0xB000
//*****************************************************************************


//*****************************************************************************
// ϵͳ����ʼ���¼� HI_DEID_SYS_INIT_E��ϵͳ�������¼�(��ϵͳ����)
//*****************************************************************************

typedef enum
{
    HI_DEID_SAL_HARDWARE_INIT_OK      = 0,
    HI_DEID_SAL_HARDWARE_INIT_ERR     = 1,
    HI_DEID_SAL_PHASE1_INIT_OK        = 2,
    HI_DEID_SAL_PHASE1_INIT_ERR       = 3,
    HI_DEID_SAL_START_PHASE2_INIT     = 4,
    HI_DEID_SAL_START_PHASE2_INIT_ERR = 5,
    HI_DEID_SAL_CTX_HEAP_ERR          = 6,
    HI_DEID_SAL_CTX_INIT_START        = 7,
    HI_DEID_SAL_CTX_INIT_OK           = 8,
    HI_DEID_SAL_MEM_INIT_OK           = 9,
    HI_DEID_SAL_QES_INIT_OK           = 10,
    HI_DEID_SAL_TASKS_INIT_OK         = 11,
    HI_DEID_SYS_INIT_OK               = 12,
    HI_DEID_SAL_NV_INIT_ERR           = 13,
    HI_DEID_SAL_NV_INIT_OK            = 14,
    HI_DEID_FTM_DEVICE_CFG_ERR        = 15,
    HI_DEID_SYS_CRASH                 = 16,
    HI_DEID_SYS_CRASH_MON_START       = 17,
    HI_DEID_SYS_APP_TASK_INIT_END     = 18,
    HI_DEID_SYS_TASK_START_END        = 19,
    HI_DEID_SAL_MAX                   = 20,

    HI_DEID_DRV_INIT_MIN        = 21,
    HI_DEID_DRV_FLASH_INIT_OK   = 22,               //FLASH��ʼ�����
    HI_DEID_DRV_UART_INIT_OK    = 23,               //UART��ʼ�����
    HI_DEID_DRV_HRTIMER_INIT_OK = 24,           //HRTIMER��ʼ�����
    HI_DEID_DRV_HRTIMER_DESTROY_OK = 25,        //HRTIMERȥ��ʼ��
    HI_DEID_DRV_ETH_INIT_OK = 26,               //ETH��ʼ�����
    HI_DEID_DRV_ETH_REMOVE_OK = 27,         //ETHȥ��ʼ��
    HI_DEID_DRV_UPG_INIT_OK = 28,               //UPG��ʼ�����
    HI_DEID_DRV_UPG_START_OK = 29,              //UPG��ʼ����
    HI_DEID_DRV_UPG_BACKUP_OK = 30,             //UPG�汾����
    HI_DEID_DRV_UPG_STOP_OK = 31,               //UPG����ֹͣ
    HI_DEID_DRV_UPG_TIMEOUT_FINISH_OK = 32, //UPG��ʱ����
    HI_DEID_DRV_UPG_APPCALL_FINISH_OK = 33, //UPG�ص�����
    HI_DEID_DRV_DFX_INIT_OK = 34,               //DFX��ʼ�����

    HI_DEID_DRV_SPI_INIT_OK = 35,
    HI_DEID_DRV_CRC_INIT_OK = 36,
    HI_DEID_DRV_GPIO_INIT_OK = 37,
    HI_DEID_DRV_CIPHER_INIT_OK = 38,
    HI_DEID_DRV_LED_INIT_OK = 39,
    HI_DEID_DRV_IR_INIT_OK  = 40,
    HI_DEID_DRV_I2C_INIT_OK = 41,
    HI_DEID_DRV_DMA_INIT_OK = 42,
    HI_DEID_DRV_UPG_INIT_FAIL = 43,             //UPG ʧ��
    HI_DEID_DRV_NV_UNALIGN = 44,
    HI_DEID_DRV_INIT_MAX    = 50,

    //***********BEGIN OF MAC DEF********************************
    // ���¶���MACЭ��ջ��ģ���ʼ���¼�
    //*******************************************
    HI_DEID_MAC_INIT_MIN    = 51,
    HI_DEID_MAC_PHY_INIT_OK    = 52  ,
    HI_DEID_MAC_PHY_INIT_NG    = 53  ,
    HI_DEID_MAC_BEACON_INIT_OK  = 54    ,
    HI_DEID_MAC_BEACON_INIT_NG  = 55    ,
    HI_DEID_MAC_CVG_INIT_OK     = 56    ,
    HI_DEID_MAC_CVG_INIT_NG     = 57    ,
    HI_DEID_MAC_MAC_BUFFER_INIT_OK  = 58 ,
    HI_DEID_MAC_MAC_BUFFER_INIT_NG  = 59 ,
    HI_DEID_MAC_DFX_INIT_OK         = 60 ,
    HI_DEID_MAC_DFX_INIT_NG         = 61 ,
    HI_DEID_MAC_LOG_INIT_OK         = 62 ,
    HI_DEID_MAC_LOG_INIT_NG         = 63 ,
    HI_DEID_MAC_SGM_INIT_OK         = 64 ,
    HI_DEID_MAC_SGM_INIT_NG         = 65 ,
    HI_DEID_MAC_NM_ASSOC_INIT_OK    = 66 ,
    HI_DEID_MAC_NM_ASSOC_INIT_NG    = 67 ,
    HI_DEID_MAC_NM_COO_INIT_OK      = 68 ,
    HI_DEID_MAC_NM_COO_INIT_NG      = 69 ,
    HI_DEID_MAC_NM_TDMA_INIT_OK     = 72 ,
    HI_DEID_MAC_NM_TDMA_INIT_NG     = 73 ,
    HI_DEID_MAC_NM_TOPO_INIT_OK     = 74 ,
    HI_DEID_MAC_NM_TOPO_INIT_NG     = 75 ,
    HI_DEID_MAC_SD_INIT_OK          = 76 ,
    HI_DEID_MAC_SD_INIT_NG          = 77 ,
    HI_DEID_MAC_ROUTE_INIT_OK       = 78 ,
    HI_DEID_MAC_ROUTE_INIT_NG       = 79 ,
    HI_DEID_MAC_RSB_INIT_OK         = 80 ,
    HI_DEID_MAC_RSB_INIT_NG         = 81 ,
    HI_DEID_MAC_RX_INIT_OK          = 82 ,
    HI_DEID_MAC_RX_INIT_NG          = 83 ,
    HI_DEID_MAC_TX_INIT_OK          = 84 ,
    HI_DEID_MAC_TX_INIT_NG          = 85,
    HI_DEID_MAC_INIT_MAX    = 150,
    //*****************END OF MAC DEF***************************

    HI_DEID_APP_INIT_MIN    = 151,
    HI_DEID_APP_MRS_FW_INIT_OK          ,
    HI_DEID_APP_MRS_FW_INIT_FAIL        ,
    HI_DEID_APP_376_2_PROTO_INIT_OK     ,
    HI_DEID_APP_376_2_PROTO_INIT_FAIL   ,
    HI_DEID_APP_645_PROTO_INIT_OK       ,
    HI_DEID_APP_645_PROTO_INIT_FAIL     ,
    HI_DEID_APP_CCO_CTX_INIT_OK         ,
    HI_DEID_APP_CCO_CTX_INIT_FAIL       ,
    HI_DEID_APP_STA_CTX_INIT_OK         ,
    HI_DEID_APP_STA_CTX_INIT_FAIL       ,
    HI_DEID_APP_QUEUE_INIT_OK           ,
    HI_DEID_APP_QUEUE_INIT_FAIL         ,
    HI_DEID_APP_MAC_LIST_INIT_OK        ,
    HI_DEID_APP_MAC_LIST_INIT_FAIL      ,
    HI_DEID_APP_MAC_MAP_INIT_OK         ,
    HI_DEID_APP_MAC_MAP_INIT_FAIL       ,
    HI_DEID_APP_SES_INIT_OK             ,
    HI_DEID_APP_SES_INIT_FAIL           ,
    HI_DEID_APP_READ_MREC_OK            ,
    HI_DEID_APP_READ_MREC_FAIL          ,
    HI_DEID_APP_READ_NV_OK              ,
    HI_DEID_APP_READ_NV_FAIL            ,
    HI_DEID_APP_START_MRS_OK            ,
    HI_DEID_APP_INIT_MAX           = 200,

    // System Init Event
    HI_DEID_SYS_INIT_FLASH_CFG_ERR  = 201, // Flash���ô���,Block��С���ú��豸��һ��
    HI_DEID_SYS_INIT_OPEN_DEV_ERR   = 202, // ���豸ʧ��
    HI_DEID_SYS_INIT_DEV_CFG_ERR    = 203, // ��ȡ�豸����ʧ��
    HI_DEID_SYS_INIT_HW_CFG_ERR     = 204, // ��ȡӲ������ʧ��
    HI_DEID_SYS_INIT_CFG_MEM_ERR    = 205, // ��ȡ�����ڴ����, ʹ��Ĭ���ڴ�, ������С���еĵ���ģʽ
    HI_DEID_SYS_INIT_MEM_FAIL       = 206,
    HI_DEID_SYS_INIT_R_UPG_CFG_FAIL = 207,
    HI_DEID_SYS_INIT_VERIFY_MSG     = 208,

    HI_DEID_SYS_PLC_MODE            = 220,

    //USR��ʼ���¼� [230,255)
    HI_DEID_USR_INIT_EVNET_START             = 230,
    HI_DEID_USR_INIT_EVNET_END               = 255,
    HI_DEID_INIT_MAX = 255// ���벻�ܴ��� 256
}HI_DEID_SYS_INIT_E;

//*****************************************************************************
// ϵͳ������/�澯/��Ҫ���¼����� [256, 1024)
//*****************************************************************************
#define HI_DEID_SAL_SYS_EVT_START  (HI_DEID_INIT_MAX+1) // 256
typedef enum
{
    // ϵͳ�¼�: [256, 306)
    HI_DEID_SAL_SYS_MIN = HI_DEID_SAL_SYS_EVT_START, // 256
    HI_DEID_SAL_DIAG_DUMP_END,
    HEID_SYS_STATIC_MEM_NOT_ENOUGH,
    HEID_SYS_DIAG_FRM_ERR = 304,
    // TODO
    HI_DEID_SAL_SYS_MAX = HI_DEID_SAL_SYS_EVT_START + 49,

    // DRV �¼�: [306, 512)
    HI_DEID_DRV_SYS_MIN = HI_DEID_SAL_SYS_EVT_START + 50,
    HI_DEID_DRV_UPG_MSG,                //UPG��ʼ���ϱ���Ϣ
    HI_DEID_DRV_UPG_UPDATE_COMPLETE,    //UPG �������
    HI_DEID_DRV_UPG_UPDATE_CRC_OK,  //UPGУ��ͨ��
    HI_DEID_DRV_UPG_UPDATE_NOTIFY,  //UPG������ɻص�
    HI_DEID_DRV_UPG_MALLOC_ERR,     //UPG MALLOC���ɹ�
    HI_DEID_DRV_FLASH_WAIT_TIMEOUT,     //flash �ȴ�оƬ���г�ʱ
    HI_DEID_DRV_FLASH_OP_ADDR_BEYOND,   //flash ������ַԽ��
    HI_DEID_DRV_SYS_MAX = HI_DEID_SAL_SYS_EVT_START + 255,

    // [512, 768)
    HI_DEID_MAC_SYS_MIN = HI_DEID_SAL_SYS_EVT_START + 256,
    // TODO
    //��������¼�����
    HI_DEID_MAC_STA_JOIN_EVALUATE_PAHSE_FAIL = 512,
    HI_DEID_MAC_STA_SEND_CHOSE_PROXY_MSG     = 513,
    HI_DEID_MAC_STA_JOIN_NETWORK_OK          = 515,
    HI_DEID_MAC_STA_JOIN_NETWORK_FAIL        = 516,
    HI_DEID_MAC_STA_CHOSE_PROXY_FAIL         = 517,
    HI_DEID_MAC_STA_LEAVE_NETWORK            = 518,
    HI_DEID_MAC_CCO_JUDGE_STA_LEAVE_NETWORK  = 519,
    HI_DEID_MAC_CCO_NETWORK_FORM_OK          = 520,
    HI_DEID_MAC_CCO_START_NETWORK_FORM       = 521,
    HI_DEID_MAC_ALLOC_MAC_BUFFER_FAIL        = 522,
    HI_DEID_MAC_HAVE_A_CIRCLE                = 523,
    //HI_DEID_MAC_SET_FREQ_MODE                = 530,
    HI_DEID_MAC_CCO_CHANGE_FREQ_MODE         = 531,
    HI_DEID_MAC_STA_RCV_BEACON_SOF_AT_PRESENT_FREQ_MODE = 532,
    HI_DEID_MAC_STA_RCV_CHANGE_FREQ_MODE_NOTIFY = 533,


    HI_DEID_MAC_SEG_FRAME_IN_QUEUE_EVENT  = 523,
    HI_DEID_MAC_SEG_0_RPI_TxQueue_OUTFLOW = 524,
    HI_DEID_MAC_SEG_1_RPI_TxQueue_OUTFLOW = 525,
    HI_DEID_MAC_SEG_2_RPI_TxQueue_OUTFLOW = 526,
    HI_DEID_MAC_SEG_3_RPI_TxQueue_OUTFLOW = 527,
    HI_DEID_MAC_SEG_CLEAN_TXQUEUE         = 528,

    HI_DEID_MAC_RSB_TIMEOUT               = 529,
    HI_DEID_MAC_NTB_BCN_INTERVAL_ERR      = 538,
    HI_DEID_MAC_NTB_BCN_MULTI_NET_SNID       = 540,

#ifdef PRODUCT_CFG_ONLINE_TEST_MODE
    HI_DEID_MAC_GT_DFX_WAIT_3911_BCN_TIMEOUT = 900,
    HI_DEID_MAC_GT_DFX_SEND_BCN              = 901,
    HI_DEID_MAC_GT_DFX_SEND_SOF              = 902,
    HI_DEID_MAC_GT_DFX_START_SEND_SOF_TIMER  = 903,
    HI_DEID_MAC_GT_DFX_START_SEND_BCN_TIMER  = 904,
    HI_DEID_MAC_GT_DFX_SEND_BCN_TIMEOUT      = 905,
    HI_DEID_MAC_GT_DFX_SEND_SOF_TIMEOUT      = 906,
    HI_DEID_MAC_GT_NTB_STAT_WND_TIMEOUT      = 907,
    HI_DEID_MAC_GT_NTB_BUILD_GOOD_SYNC       = 908,
    HI_DEID_MAC_GT_NTB_BUILD_DEATIL_STAT     = 909,
    HI_DEID_MAC_GT_NTB_SWITCH_PREAMBLE       = 910,
#endif

    HI_DEID_MAC_SYS_MAX = HI_DEID_SAL_SYS_EVT_START + 512,

    // APP�¼� [768, 960)
    HI_DEID_APP_SYS_MIN = HI_DEID_SAL_SYS_EVT_START + 512,
    HI_DEID_APP_GET_METER_ADDR_FAIL,
    EID_APP_SEND_MSG_FAIL,

    //USRϵͳ�¼�[960,1024)
    HI_DEID_USR_SYS_EVENT_START = 960,
    HI_DEID_USR_SYS_EVENT_END = 1023,


    HI_DEID_ERR_MAX     = 1024
}HI_DEID_SYS_E;


//*****************************************************************************
#define HI_STAT_ID_NUM_MAX       (HI_DSID_MAX-HI_DFX_STAT_ID_BASE_SYS)
//*****************************************************************************

//*****************************************************************************
// SAL ͳ��������
//*****************************************************************************
// ��Χ [A000, A050)
//*****************************************************************************
#define HI_DSID_SAL_DEVIO             0xA001  // HI_DSID_SAL_DEVIO_S
#define HI_DSID_SYS_MOD_INIT          0xA002  // HI_DSID_SYS_MOD_INIT_S
#define HI_DSID_SYS_INIT_EVT          0xA003  // HI_DSID_SYS_INIT_EVT_S
// TODO
//*****************************************************************************
typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 ulTotalCnt;
    HI_U32 ulUsedSize;
    HI_U32 ulFlashAvailSize;//����
    HI_U32 ulFlashAddr;//����
    HI_U16 usType;
    HI_U16 reserved;
    HI_U32 aulDumpKeyMsgFlashIo[20];
}HI_DSID_SAL_DUMP_KEY_MSG_S;
#define HI_DSID_SAL_DUMP_KEY_MSG_S_INIT_DEFINED1  0,0,0,0,0,0

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 flash_total_size;
    HI_U32 flash_sector_size;
    HI_U32 flash_cfg_sector_size;
    HI_U32 nv_addr;
    HI_U32 nv_block_num;
    HI_U32 nv_block_size;
    HI_U32 pad[2];
}HI_DSID_SAL_DEVIO_S;
#define HI_DSID_SAL_DEVIO_S_INIT_DEFINED1   0,0,0,0,0,0

typedef struct
{
    HI_U32  ulTimsStamp[2];
    HI_U8   ucModId;
    HI_U8   ucReserved;
    HI_U8   ucDropId;
    HI_U8   ucIdx;
    HI_CHAR szModId[HI_SYS_MOD_NAME_LEN];
}HI_DSID_SYS_MOD_INIT_S;
#define HI_DSID_SYS_MOD_INIT_S_INIT_DEFINED   {0,0},0,{0,0,}

typedef struct
{
    HI_U32 ulEvt;
    HI_U16 usTimsStamp;
    HI_U16 usRc;
}HI_DSID_SYS_INIT_EVT_S;

//*****************************************************************************
// DRVͳ��������
//*****************************************************************************
// ��Χ [0xA050, 0xA150)
//*****************************************************************************
#define HI_DSID_DRV_XXX      0xA050  // HI_DSID_DRV_XXX_S
#define HI_DSID_DRV_FLASH_INFO      0xA051  // FLASHͳ����Ϣ
#define HI_DSID_DRV_SPI_INFO        0xA054  // SPIͳ����Ϣ
#define HI_DSID_DRV_HRT_INFO        0xA057  //HRTͳ����Ϣ
#define HI_DSID_DRV_ETH_INFO        0xA058  //ETHͳ����Ϣ
#define HI_DSID_DRV_RF_ERR          0xA059  //RF GPIO SPI ͳ����Ϣ


// TODO
//*****************************************************************************


//*****************************************************************************
// �ṹ����
//*****************************************************************************
typedef struct
{
    HI_OBJ64_S stObjCnt;
}HI_DSID_DRV_XXX_S;
typedef struct sfc_op
{
    HI_U8  uciftype;
    HI_U8  uccmd;
    HI_U8  ucdummy;
    HI_U8  PAD;
    HI_U32 ulsize;
    HI_U32 ulclock;
} SFC_OPERATION_S;
typedef struct sfc_dfx_info
{
    HI_U32  pname;
    HI_U32  ulchipselect;
    HI_U32  ulchipsize;
    HI_U32  ulerasesize;
    HI_U32  piobase;
    HI_U32  uladdrcycle;
    SFC_OPERATION_S  read[1];
    SFC_OPERATION_S  write[1];
    SFC_OPERATION_S  erase[8];
} HI_DSID_DRV_FLASH_INFO_S;

typedef struct
{
    HI_U32 read;
    HI_U32 write;
    HI_U32 int_read;
    HI_U32 int_write;
    HI_U32 err_overrun;
    HI_U32 err_parity;
    HI_U32 err_frame;
    HI_U32 err_break;
	HI_U32 err_busy;
    HI_U32 tx_time_us; //����ʱϵͳʱ��ֵ
    HI_U32 tx_timer_us;//����ʱ��ʱ��������ֵ
    HI_U32 tx2rx_max_us;//����ת�����ʱ��
    HI_U32 tx2rx_max_us_timestamp;//����ת�����ʱ��ķ���ʱ��
}HI_DSID_DRV_UART_INFO_S;

typedef struct
{
    HI_U32 ulOldAddr;//HSO��ռ������ռIP��ַ
    HI_U32 ulNewAddr;//HSO��ռ����ռIP��ַ
    HI_U32 second;//HSO��ռ��ʱ��(s)
}HI_DSID_SAL_HSO_GRAB_CONNECT_INFO_S;
#define HI_DSID_SAL_HSO_GRAB_CONNECT_INFO_COUNT 3//���ɼ�¼����ռ��¼����

typedef struct spi_init_info
{
    HI_U32 baudrate;  //������
    HI_U32 framemode; //����ģʽ
    HI_U32 datawidth; //����λ��
    HI_U32 spo;       //moto��spiģʽʱSPICLKOUT��λ
    HI_U32 sph;       //moto��spiģʽʱSPICLKOUT����
} HI_SPI_DEV_ATTR_S;
typedef struct spi_dfx_info
{
    HI_U32      id;
    HI_U32      int_vector;
    HI_SPI_DEV_ATTR_S spi_attr;
    HI_U32      buf_size;
    HI_U32      reg_base;
    HI_U32      rxbuf_p;
    HI_U32      rxbuf_read;
    HI_U32      rxbuf_write;
    HI_S32      rxbuf_status;
    HI_U32      txbuf;
    HI_U32      txbuf_read;
    HI_U32      txbuf_write;
    HI_S32      txbuf_status;
    HI_U32      err_buf_full;
    HI_U32      err_runout;
} HI_DSID_DRV_SPI_INFO_S;

#if defined(PRODUCT_CFG_SUPPORT_UPG)
typedef struct
{
    HI_U32 data[20];
}HI_UPG_TIMER_HANDLE_T;
#endif
typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 ulRxCnt;   //���հ��ĸ���
    HI_U32 ulTxCnt;  //���Ͱ��ĸ���
    HI_U32  ulTxErrCnt; //����ʧ�ܼ���
    HI_U16   BufTotal;    //�ܵ�BUFFER����
    HI_U16   BufUsed;   //��ǰʹ�ø���
}HI_DSID_DRV_ETH_INFO_S;

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 ulTimerAddCnt; //���TIMER�ĸ���
    HI_U32 ulTimerDelCnt; //ɾ��TIMER�ĸ���
    HI_U32 ulTimerTimeoutCnt;    //����TIEMR�ĸ���
}HI_DSID_DRV_HRT_INFO_S;

//*****************************************************************************
// DRV��Ϣ����
//*****************************************************************************
// ��Χ [0x0150, 0x0250)
//*****************************************************************************
#define HI_DMID_DRV_UPG_ERR_READ_NV                 0x0150      //UPG��NV����
#define HI_DMID_DRV_UPG_ERR_WRITE_NV                0x0151      //UPGдNV����
#define HI_DMID_DRV_UPG_ERR_READ_FLASH              0x0152  //UPG��FLASH����
#define HI_DMID_DRV_UPG_ERR_WRITE_FLASH             0x0153  //UPGдFLASH����
#define HI_DMID_DRV_UPG_ERR_BUSY                    0x0154  //UPG ��������
#define HI_DMID_DRV_UPG_ERR_START_PARA              0x0155  //UPG START ��������
#define HI_DMID_DRV_UPG_ERR_UPDATE_PARA     0x0156  //UPG UPDATE ��������
#define HI_DMID_DRV_UPG_ERR_UPDATE_VERSION      0x0157  //UPG �������ļ���汾����
#define HI_DMID_DRV_UPG_ERR_UPDATE_CRC            0x0158    //UPG CRCУ���
#define HI_DMID_DRV_FLASH_PROBE         0x0170  // FLASHоƬ��Ϣ̽��
#define HI_DMID_DRV_FLASH_OP            0x0171  // FLASH������Ϣ

#define HI_DMID_DRV_UART_CHANGE_PARA    0x0180  // UART�����޸�

#define HI_DMID_DRV_SPI_TIMEOUT         0x0190  // SPI������ʱ
//*****************************************************************************


//*****************************************************************************
// �ṹ����
//*****************************************************************************
#if defined(PRODUCT_CFG_SUPPORT_UPG)
typedef struct
{
    HI_U32 ulNvId;
    HI_U32 ulErrNo;
}HI_DMID_DRV_UPG_ERR_READ_NV_S;

typedef struct
{
    HI_U32 ulNvId;
    HI_U32 ulErrNo;
}HI_DMID_DRV_UPG_ERR_WRITE_NV_S;

typedef struct
{
    HI_U32 ulFlashAddr;
    HI_S32 ulFlashSize;
    HI_U32 ulErrNo;
}HI_DMID_DRV_UPG_ERR_READ_FLASH_S;

typedef struct
{
        HI_U32 ulFlashAddr;
        HI_S32 ulFlashSize;
        HI_U32 ulErrNo;
}HI_DMID_DRV_UPG_ERR_WRITE_FLASH_S;

typedef struct
{
    HI_U32 ulStartTimes;
    HI_U32 ulStatus;
}HI_DMID_DRV_UPG_ERR_BUSY_S;


typedef struct
{
        HI_U32 ulOption;
        HI_U32 ulUpgId;
        HI_U32 ulFileLen;
        HI_U32 ulBlkNum;
        HI_U32 ulUpgTimeWindow;
}HI_DMID_DRV_UPG_ERR_START_PARA_S;

typedef struct
{
    HI_U32 ulStartUpgId;
    HI_U32 ulUpgId;
    HI_U16 usBlkIdx;
    HI_U16 usBlkSize;
    HI_U8* pucFileBlk;
}HI_DMID_DRV_UPG_ERR_UPDATE_PARA_S;



typedef struct
{
    HI_U32 ulMagic;

    HI_UPG_FILE_VER_S  ulRunTimeFileVer;
    HI_UPG_FILE_VER_S  ulBackUpFileVer;


}HI_DMID_DRV_UPG_ERR_UPDATE_VERSION_S;

typedef struct
{
    HI_U32 ulNewCrcData;
    HI_U32 ulFileCrcData;
}HI_DMID_DRV_UPG_ERR_UPDATE_CRC_S;
#endif
typedef struct
{
    HI_U8       ucId[3];
    HI_U8       PAD;
    HI_CHAR     cName[10];
}HI_DMID_DRV_FLASH_PROBE_S;
typedef struct
{
    HI_U8       uctype;
    HI_U8       PAD[3];
    HI_U32      uladdr;
    HI_U32      ullen;
}HI_DMID_DRV_FLASH_OP_S;

typedef struct
{
    HI_U32  ulport;
    HI_U32  ulbaud_rate;
    HI_U32  uldata_bits;
    HI_U32  ulstop_bits;
    HI_U32  ulparity;
}HI_DMID_DRV_UART_CHANGE_PARA_S;

typedef struct
{
    HI_U32 ulport;
    HI_U32 uldirection;
    HI_U32 ultimeout;
}HI_DMID_DRV_SPI_TIMEOUT_S;

//*****************************************************************************
// MACͳ��������
//*****************************************************************************
// ��Χ [A250, A450)
// 1�� ����ҵ����Ϣ  [0xA250~0xA270)
// 2,  ·��          [0xA270~0xA290)
// 3,  ���ͻ�        [0xA290~0xA2B0)
// 4,  �ֶ���        [0xA2B0~0xA2D0)
// 5,  PHY           [0xA2D0~0xA2F0)
// 6,  ������        [0xA2F0~0xA310)
// 7,  beacon        [0xA310~0xA330)
// 8,  conv          [0xA330~0xA350)
// 9,  RX            [0xA350~0xA370)
// 10, NTB           [0xA370~0xA390)
// 11, ������        [0xA390~0xA3A0)
// 12, ���������ο���[0xA3A0~0xA3B0)
// 13, DFX           [0xA3B0~0xA3C0)
// 14, PLC_IP        [0xA3C0~0xA3D0)
// 15, FA            [0xA3D0~0xA3E0)
// 16, sounding      [0xA3E0~0xA3F0)
// 17  ����          [0xA3F0~0xA450)
//*****************************************************************************
//*****************************************************************************
// TODO
// ע��ID�ŵĶ���һ��Ҫ���� �ĵ� Ҫ�������塣
// ID:HI_DSID_XXX_XXX;
// ��Ӧ�Ľṹ��: HI_DSID_XXX_XXX_S;
//*****************************************************************************

//PHY��ͳ��������
#define HI_DSID_PHY_OFFSET                          0xA2D0
#define HI_DSID_PHY_TX_ERR                          (HI_DSID_PHY_OFFSET + 0x00)
#define HI_DSID_PHY_L1COMMON_ERR                    (HI_DSID_PHY_OFFSET + 0x01)
#define HI_DSID_PHY_BITLOADING_ERR                  (HI_DSID_PHY_OFFSET + 0x02)
#define HI_DSID_PHY_OM_ERR                          (HI_DSID_PHY_OFFSET + 0x03)
#define HI_DSID_PHY_AGC_ERR                         (HI_DSID_PHY_OFFSET + 0x04)
#define HI_DSID_PHY_NBI_ERR                         (HI_DSID_PHY_OFFSET + 0x05)
#define HI_DSID_PHY_IMP_ERR                         (HI_DSID_PHY_OFFSET + 0x06)
#define HI_DSID_PHY_SYMBOL_ALIGN_ERR                (HI_DSID_PHY_OFFSET + 0x07)
#define HI_DSID_PHY_CHANNEL_INFO_ERR                (HI_DSID_PHY_OFFSET + 0x08)
#define HI_DSID_PHY_CLOCK_PREAMBLE_ERR              (HI_DSID_PHY_OFFSET + 0x09)
#define HI_DSID_PHY_CLOCK_RX_END_ERR                (HI_DSID_PHY_OFFSET + 0x0a)
#define HI_DSID_PHY_NTB_ERR                         (HI_DSID_PHY_OFFSET + 0x0b)
#define HI_DSID_PHY_TX_SEM_STATISTIC                 (HI_DSID_PHY_OFFSET + 0x0c)
#define HI_DSID_OM_SOUND_SNR_IND                    (HI_DSID_PHY_OFFSET + 0x1b)
#define HI_DSID_OM_PREAMBLE_SNR_IND                 (HI_DSID_PHY_OFFSET + 0x1c)
#define HI_DSID_OM_TX_TONE_MAP_IND                  (HI_DSID_PHY_OFFSET + 0x1d)
#define HI_DSID_OM_BITLOADING_PB136_IND             (HI_DSID_PHY_OFFSET + 0x1e)
#define HI_DSID_OM_BITLOADING_PB520_IND             (HI_DSID_PHY_OFFSET + 0x1f)

#define HI_DSID_PHY_OM_LEVEL0_MAX_NUM                5           //PHY���쳣��Ϣ�ı�����������

//�ṹ�嶨��
//L1COMMON�ڲ��Ŀ�ά�ɲ��������
typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 NvIdError;
    HI_U32 AfeError;
    HI_U32 NvReadError;
    HI_U32 InnerQueryIdError;
    HI_U32 SpiTimeOutError;
    HI_U32 GetHwIdError;                //��ȡӲ��IDʧ�ܵĴ���
    HI_U32 PowerStepError;              //�������ַ��书�ʴ���Ĵ���
    HI_U32 LastHwId;                    //���һ�λ�ȡ��Ӳ��ID

    HI_U16 malloc_err_cnt;              //�ڴ����ʧ�ܼ���
    HI_U16 pad1;

    HI_U32 pad[3];
} HI_DSID_PHY_L1COMMON_ERR_S;

//TX�ڲ��Ŀ�ά�ɲ����
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U32 CopyNumError;
    HI_U32 TryFreqError;                //Ƶ�κŴ���ļ���
    HI_U32 FreqNotAllowedCnt;           //�������л�Ƶ�εĴ���
    HI_U32 FreqSameCnt;                 //���м��Ƶ��һ�£�δ�л��Ĵ���

    HI_U32 CarrierNumError;
    HI_U32 RoboSymbolNumError;

    HI_U32 RoboQamError;
    HI_U32 PayloadGiError;
    HI_U32 FcGiError;
    HI_U32 NoRoboCodeError;

    HI_U32 RiError;
    HI_U32 CpNumError;
    HI_U32 FcQamError;
    HI_U32 NoRoboQamTypeError;

    HI_U32 NonRoboFixedQamError;
    HI_U32 ToneMapError;
    HI_U32 TxEnError;
    HI_U32 PayloadModeError;

    HI_U32 pad3;                        //Ƶ��ͨ��1������Ƶ��ͨ���������õĴ���
    HI_U32 freqChangeCnt;               //Ƶ���л�����

    HI_U32 freqChlChangeCnt;            //����Ƶ��ͨ���л�����
    HI_U16 pad1;                        //Ƶ����ϴ������
    HI_U8  curTxFreq;                   //��ǰ�ķ���Ƶ��
    HI_U8  curChl1Freq;                 //���м��ͨ��1��Ƶ�κ�

    HI_U8  pad2[8];                     //��ǰ�Ľ���Ƶ�����

    HI_U32 freqUnsupportCnt;            //���ͱ���ʱ��֧�ֵ�Ƶ�μ���

    // plc_sc ͳ������Ϣ
    HI_U32 PlcScTxOverIntCnt;
    HI_U32 PlcScRxOverIntCnt;
    HI_U32 PlcScCheckOkCnt;
    HI_U32 PlcScCheckFailCnt;
    HI_U32 PlcScSenseIntCnt;
    HI_U32 PlcScCrcErrCnt;
} HI_DSID_PHY_TX_ERR_S;

//Bitloading���ڲ��Ŀ�ά�ɲ����
typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 MaxBitError;
    HI_U32 BitloadingToneMapError;
} HI_DSID_PHY_BITLOADING_ERR_S;

//OM���ڲ��������
typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 ReqIdError;                  //HSO�·��Ĳ�ѯID����
    HI_U32 ReportError;                 //��ά�ɲ���Ϣ�ϱ�����
}HI_DSID_PHY_OM_ERR_S;

//AGC�쳣
typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 ReportCnt;                   //�ϱ�����ͳ��
    HI_U32 AdAvgPower;                  //ADƽ�������쳣
    HI_U32 SatRate;                     //������
    HI_U32 AfePower;                    //AFE���źŹ���
    HI_U32 PowerAfterFilter;            //�˲����źŵĹ���
}HI_DSID_PHY_AGC_ERR_S;

//NBI�쳣
typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 ReportCnt;                   //�ϱ�����ͳ��
    HI_U32 NbiCnt;                      //����NBI��
    HI_U32 NbiOutOfNf;                  //����NF��������NBI
}HI_DSID_PHY_NBI_ERR_S;

//����ͬ���쳣
typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 ReportCnt;                   //�ϱ�����ͳ��
    HI_U32 FirstToMaxPathLen;           //�׾�����󾶵ľ���
    HI_U32 PataLen;                     //������
}HI_DSID_PHY_SYMBOL_ALIGN_ERR_S;

//�ŵ������쳣
typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 ReportCnt;                   //�ϱ�����ͳ��
    HI_U32 Par;                         //�����
    HI_U32 AvgSnr;                      //ƽ��snr
}HI_DSID_PHY_CHANNEL_INFO_ERR_S;

//Clock�쳣Preamble�ϱ�
typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 ReportCnt;
    HI_U32 PreambleFreqErr;             //PreambleƵƫ
    HI_U32 PllFreqErr;                  //Pll��Ƶƫ
    HI_U32 PreambleCarrierNum;          //Preamble�ز�����
    HI_U32 CntSingu;                    //clk_estģ��������쳣����
}HI_DSID_PHY_CLOCK_PREAMBLE_ERR_S;

//Clock�쳣��������ж��ϱ�
typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 ReportCnt;                   //�ϱ�����ͳ��
    HI_U32 PilotFreqErr;                //PilotƵƫ
    HI_U32 SoundFreqErr;                //Sounding��Ƶƫ
    HI_U32 PilotCarrierNum;             //Pilot�ز���
}HI_DSID_PHY_CLOCK_RX_END_ERR_S;

//NTB�쳣
typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 ReportCnt;                   //�ϱ�����ͳ��
    HI_U32 BtsLtmrFreqErr;              //����������BTS��LTmr�����õ�ƫ��
    HI_U32 DeltaFreqErr;                //NTB��ǰ������Ƶƫ��
    HI_U32 CntSingu4NTB;                //��ǰ״̬�µ��쳣ֵ����
}HI_DSID_PHY_NTB_ERR_S;

//IMP�쳣
typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 ReportCnt;                   //�ϱ�����ͳ��
    HI_U32 ImpCnt;                      //ÿ��ͳ������StatiPeriod�ϱ�
}HI_DSID_PHY_IMP_ERR_S;

//����ʱ��ȡ�ź������쳣ͳ��
typedef struct
{
    HI_U32 CollectDataLockCnt;                    //������������
    HI_U32 CollectDataUnlockCnt;                  //������������

    HI_U32 TxEnConflict;                          //���ͳ�ͻ����
    HI_U32 pad1;                  //�����з�����ɼ���

    HI_U32 TxSuccessCnt;
    HI_U32 pad2;                           //������ɼ���
    HI_U32 BeaconErrCnt;
    HI_U32 SofErrCnt;

    HI_U32 pad3[4];
    HI_U32 pad4[3];
}HI_DSID_PHY_TX_SEM_STATISTIC_S;

//�������ģ��ͳ��������
#define NM_STATISTIC_HEAD_OFFSET 0xA250
//#define HI_DSID_MAC_NM_FA_STAT_INFO                   (NM_STATISTIC_HEAD_OFFSET + 0x02)

//������ģ��ͳ��������
#define RSB_STATISTIC_HEAD_OFFSET 0xA2F0
#define HI_DSID_MAC_RSB_MPDU_INFO_CNT      (RSB_STATISTIC_HEAD_OFFSET)
#define HI_DSID_MAC_RSB_NODE_INFO_CNT      (RSB_STATISTIC_HEAD_OFFSET + 0x01)
#define HI_DSID_MAC_RSB_ABNORMITY_INFO     (RSB_STATISTIC_HEAD_OFFSET + 0x03)

//�ֶ���ģ��ͳ��������
#define SEG_STATISTIC_HEAD_OFFSET 0xA2B0
#define HI_DSID_SEG_FRAME_CNT_IN_DIFF_TXQUEUE       SEG_STATISTIC_HEAD_OFFSET
#define HI_DSID_MAC_SEG_ABNORMITY_INFO              (SEG_STATISTIC_HEAD_OFFSET + 0x01)      //�ֶ�ģ���쳣

//���ͻ�ͳ��������
#define TX_STATISTIC_HEAD_OFFSET 0xA290
#define HI_DSID_MAC_TX_MPDU_SEND_COUNT      TX_STATISTIC_HEAD_OFFSET
#define HI_DSID_MAC_TX_ALL_QUEUE_INFO       TX_STATISTIC_HEAD_OFFSET + 0x1
#define HI_DSID_MAC_TX_QUEUE_INFO           TX_STATISTIC_HEAD_OFFSET + 0x2
#define HI_DSID_MAC_TX_ABNORMITY_INFO       (TX_STATISTIC_HEAD_OFFSET + 0x03)
#define HI_DSID_MAC_TX_EVENT_INFO           (TX_STATISTIC_HEAD_OFFSET + 0x04)

// ������ģ��ͳ����
#define NDM_STATISTIC_HEAD_OFFSET 0xA390
#define HI_DSID_MAC_NDM_CONNECT_STAT_INFO   NDM_STATISTIC_HEAD_OFFSET
#define HI_DSID_MAC_NDM_ABNORMITY_INFO      (NDM_STATISTIC_HEAD_OFFSET + 0x01)      //���������쳣��Ϣ
#define HI_DSID_MAC_NDM_STAT_INFO           (NDM_STATISTIC_HEAD_OFFSET + 0x02)      //���������쳣��Ϣ
#define HI_DSID_MAC_NDM_REMOTE_INFO         (NDM_STATISTIC_HEAD_OFFSET + 0x03)      //��������Զ��ͨ����Ϣ
#define HI_DSID_MAC_NDM_HEARTBEAT_INFO      (NDM_STATISTIC_HEAD_OFFSET + 0x04)      //������������ͳ����Ϣ
#define HI_DSID_MAC_NDM_METER_CHECK_INFO    (NDM_STATISTIC_HEAD_OFFSET + 0x05)      //�������ͳ����Ϣ
//���������ο���ͨ��ͳ����
#define HI_DSID_MAC_MDM_REGIST_INFO         (NDM_STATISTIC_HEAD_OFFSET + 0x10)      //���������ο���ͨ��ע��
//����������NDCͨ��ͳ����
#define HI_DSID_MAC_NDC_STAT_INFO           (NDM_STATISTIC_HEAD_OFFSET + 0x11)      //����������ͨ��ע��
#define HI_DSID_MAC_NFC_STAT_INFO           (NDM_STATISTIC_HEAD_OFFSET + 0x12)      //��������������ͨ����Ϣ
#define HI_DSID_MAC_NDC_OBJ_STAT_INFO       (NDM_STATISTIC_HEAD_OFFSET + 0x13)      //���������û�
#define HI_DSID_MAC_NDC_SEARCH_STAT_INFO    (NDM_STATISTIC_HEAD_OFFSET + 0x14)      //����������ͨ����Ϣע��
//dfxģ��ͳ����
#define DFX_STATISTIC_HEAD_OFFSET           0xA3B0
#define HI_DSID_MAC_DFX_SYS_ERR             (DFX_STATISTIC_HEAD_OFFSET + 0x00)     //dfxģ���ڲ�ͳ��
#define HI_DSID_MAC_DFX_STAT_INFO           (DFX_STATISTIC_HEAD_OFFSET + 0x01)     //dfxģ��ҵ������ͳ��


//PLC��IPģ���ͳ����
#define PLC_IP_STATISTIC_HEAD_OFFSET        0xA3C0
#define HI_DSID_MAC_PLC_IP_STAT_INFO        (PLC_IP_STATISTIC_HEAD_OFFSET + 0x00)   //plc_ipģ���ҵ��ͳ��

//rxģ��ͳ��������
#define RX_STATISTIC_HEAD_OFFSET            0xA350
#define HI_DSID_MAC_RX_ABNORMITY_INFO       (RX_STATISTIC_HEAD_OFFSET + 0x00)
#define HI_DSID_MAC_RX_ITRUPT_INFO          (RX_STATISTIC_HEAD_OFFSET + 0x01)
#define HI_DSID_MAC_RX_STAT_INFO            (RX_STATISTIC_HEAD_OFFSET + 0x02)

//faģ���ͳ����
#define FA_STATISTIC_HEAD_OFFSET            0xA3D0
#define HI_DSID_MAC_FA_STAT_INFO            (FA_STATISTIC_HEAD_OFFSET + 0x00)       //faģ�������ͳ����
#define HI_DSID_MAC_FA_APP_INFO             (FA_STATISTIC_HEAD_OFFSET + 0x01)       //faģ����APP��������ͳ����

//faģ����APP��������ͳ����
typedef struct
{
    HI_U16 app_start_cnt;
    HI_U16 app_stop_cnt;
    HI_U16 app_realtime_enable_cnt;
    HI_U16 app_realtime_disable_cnt;

    HI_BOOL app_realtime_state;
    HI_U8  pad1;
    HI_U16 app_unexpect_cmd_cnt;

    HI_U32 app_set_realtime_timestamp;

    HI_U16 pad2;
    HI_U16 GetCalendarTime_err_cnt;                 //��ȡʵʱʱ��ʧ��

    HI_U32 check_realtime_timer_success_cnt;        //���ʵʱʱ�ӳɹ�����
    HI_U32 pad3[7];
}HI_DSID_MAC_FA_APP_INFO_S;

//faģ�������ͳ����
typedef struct
{
    HI_U32 check_ping_timer_cnt;                    //�����Ա��ĳ�ʱ�Ķ�ʱ��
    HI_U32 check_realtime_timer_cnt;                //���ʵʱʱ�ӵĶ�ʱ��
    HI_U32 affirm_network_state_timer_cnt;          //ȷ������״̬�Ķ�ʱ��
    HI_U32 affirm_offline_timer_cnt;                //ȷ��վ�����ߵĶ�ʱ��

    HI_U16 unknown_timer_cnt;                       //δ֪�Ķ�ʱ��
    HI_U16 unexpect_state_timer_cnt;                //״̬������Ԥ�ڵĶ�ʱ���ص�

    HI_U32 nm_notify_callback_cnt;                  //����֪ͨ�ص��ļ���
    HI_U32 nm_notify_fa_white_list_change_cnt;      //�������仯
    HI_U32 nm_notify_fa_mac_chl_state_cnt;          //macͨ��״̬�仯
    HI_U32 nm_notify_fa_sta_join_cnt;               //��վ������

    HI_U32 nm_notify_fa_freq_change_cnt;            //Ƶ�α��֪ͨ

    HI_U16 unknown_nm_notify_cnt;                   //δ֪������֪ͨ
    HI_U8  nm_notify_last_whist_list_change_reason; //������һ��֪ͨ�������仯��ԭ��
    HI_U8  nm_notify_last_freq;                     //������һ��֪ͨ��Ƶ��

    HI_U8  nm_notify_last_chl_state;                //������һ��֪ͨ��ͨ��״̬
    HI_U8  recover_unexpect_err;                    //�����ָ�ͨ��״̬�Ĵ���
    HI_U16 stop_evaluate_err_cnt;                   //ֹͣƵ������Ӧʧ��

    HI_U16 calc_cur_freq_reslut_addr_err_cnt;       //�ڴ��ַ�Ƿ�
    HI_U16 handle_rcv_ping_pkt_msg_addr_err_cnt;    //�ڴ��ַ�Ƿ�
    HI_U16 handle_check_ping_timer_addr_err_cnt;    //�ڴ��ַ�Ƿ�
    HI_U16 handle_rcv_ping_pkt_msg_state_err_cnt;   //�ڷǱ��Ĳ���״̬�յ����Ա��ĵļ���

    HI_U16 handle_rcv_ping_pkt_msg_rcv_more_delay_cnt;      //���ճɹ�����ʱ������ֵ
    HI_U16 handle_rcv_ping_pkt_msg_unexpect_tei_cnt;        //���յĲ��Ա��ĵĲ��ڽ��ն���
    HI_U16 handle_rcv_ping_pkt_msg_unexpect_turn_cnt;       //���յĲ��Ա��ĵ��ִβ�����Ԥ��
    HI_U16 handle_check_ping_timer_tei_miss_cnt;    //teiȱʧ

    HI_U32 handle_check_ping_timer_overtime_cnt;    //Ӧ��ʱ�Ĵ���

    HI_U16 send_fa_msg_err_cnt;                     //����FA����Ϣʧ��
    HI_U16 start_fa_timer_err_cnt;                  //������ʱ��ʧ��
    HI_U16 whist_list_diff_num;                     //�������仯�ĸ���
    HI_U16 app_set_business_state_cnt;              // ���õ�appҵ�����

    HI_U8  app_set_business_state;                  //���õ�appҵ��״̬
    HI_U8  pad;
    HI_U16 freq_index_err_cnt;                      //Ƶ�κŴ���

    HI_U16 chl_state_notify_err_cnt;                //ͨ��״̬֪ͨ�������
    HI_U16 malloc_fail_cnt;                         //���䶯̬�ڴ�ʧ��

    HI_U16 save_evaluate_result_err_cnt;            //�洢�������ʧ�ܴ���
    HI_U16 set_mac_chl_state_err_cnt;
}HI_DSID_MAC_FA_STAT_INFO_S;

//Soundingģ���ͳ����
#define SD_STATISTIC_HEAD_OFFSET            0xA3E0
#define HI_DSID_MAC_SD_START_INFO           (SD_STATISTIC_HEAD_OFFSET + 0x00)       //sounding����ͳ����

//sounding����ͳ����
typedef struct
{
    HI_U32 initial_success_cnt;
    HI_U32 initial_fail_cnt;

    HI_U32 dynamic_success_cnt;
    HI_U32 dynamic_fail_cnt;

    HI_U8  phy_in;
    HI_U8  old_phy_in;
    HI_U8  phy_in_last_success;
    HI_U8  old_phy_in_last_success;

    HI_U32 update_rcv_tonemap_item_cnt;
    HI_U32 send_rcv_tonemap_success_cnt;        //���ͱ��˽���ʱʹ�õ�ToneMap�ɹ�����
    HI_U32 update_tx_tonemap_item_cnt;          //���·���ToneMap

    HI_U16 rcv_tx_tonemap_bit_sum_invalid_cnt;
    HI_U16 tx_tonemap_next_hop_err_cnt;

    HI_U32 robo_success_in_norobo_cnt;
    HI_U32 norobo_success_cnt;

    HI_U32 pad2[10];
}HI_DSID_MAC_SD_START_INFO_S;

//plc_ipģ���ҵ��ͳ��
typedef struct
{
    HI_U32 mac_output_cnt;

    HI_U16 mac_output_no_initilization_cnt;
    HI_U16 mac_output_status_err_cnt;
    HI_U16 mac_output_data_len_err_cnt;
    HI_U16 mac_output_sem_cs_buf_list_err_cnt;

    HI_U16 mac_output_plc_mac_start_err_cnt;
    HI_U16 pad;

    HI_U32 mac_output_mac_buff_err_cnt;

    HI_U32 send_mpdu_to_ip_cnt;
    HI_U32 send_mpdu_to_ip_netPktGet_buff_err_cnt;

    HI_U32 send_mpdu_to_ip_br_not_my_ip_cnt;
    HI_U32 send_mpdu_to_ip_br_ip_cnt;
    HI_U32 send_mpdu_to_ip_br_mac_cnt;
    HI_U32 send_mpdu_to_ip_my_mac_cnt;
    HI_U32 send_mpdu_to_ip_not_my_mac_cnt;
}HI_DSID_MAC_PLC_IP_STAT_INFO_S;

//�ֶ��쳣ͳ��
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U32 pri0_tx_queue_overflow_cnt;
    HI_U32 pri1_tx_queue_overflow_cnt;
    HI_U32 pri2_tx_queue_overflow_cnt;
    HI_U32 pri3_tx_queue_overflow_cnt;
    HI_U32 upg_tx_queue_overflow_cnt;
    HI_U32 discard_cnt;
}HI_DSID_MAC_SEG_ABNORMITY_INFO_S;

//�����쳣ͳ��
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U32 old_rcv_half_new_rcv_cnt;            //��һ�ν���δ��ɣ����µ�FC����
    HI_U32 rcv_fc_no_buff_cnt;                  //�յ�FC������buff����
    HI_U32 rcv_end_half_sof_cnt;                //sofδ�������������յ���������ж�
    HI_U32 decode_result_err_cnt;               //��ȡ�������������
}HI_DSID_MAC_RX_ABNORMITY_INFO_S;

//RX�ж�ͳ��
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U32 itrupt_cnt;                      //�ж�����ͳ��
    HI_U32 pcs_itrupt_cnt;
    HI_U32 fc_itrupt_cnt;
    HI_U32 pb_itrupt_cnt;

    HI_U32 rx_end_itrupt_cnt;
    HI_U32 tx_end_itrupt_cnt;
    HI_U32 snr_itrupt_cnt;
    HI_U32 pad0;

    HI_U32 rx_wvf_interrupt_cnt;
    HI_U32 rx_para_ok_interrupt_cnt;
    HI_U32 pad;
    HI_U32 tx_write_conllison_cnt;
    HI_U32 pad1;
    HI_U32 tx_collision_rx_cnt;
    HI_U32 tx_send_start_cnt;
    HI_U32 tx_send_end_cnt;
    HI_U32 cp_num_itrupt_cnt;               //V100���ز�������ط�ά���ϱ��ж�
    HI_U32 tx_frame_err_cnt;
    HI_U32 tx_frame_suc_cnt;
}HI_DSID_MAC_RX_ITRUPT_INFO_S;

//RX����״̬ͳ��
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U32 short_pb_ok_cnt;
    HI_U32 long_ok_cnt[4];

    HI_U32 short_pb_err_cnt;
    HI_U32 long_pb_err_cnt[4];
}HI_DSID_MAC_RX_STAT_INFO_S;

//�����쳣ͳ��
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U32 unknown_event_cnt;                   //δ֪�¼�
    HI_U32 malloc_err_cnt;                     //��ȡ�ڴ�ʧ��

}HI_DSID_MAC_TX_ABNORMITY_INFO_S;

//�����¼�ͳ��
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U32 tx_event_cnt;                //�����¼�����
    HI_U32 tx_tdma_period_refreshed;
    HI_U32 tx_mpdu_queued_in;
    HI_U32 tx_cts_received;

    HI_U32 tx_period_timeout;
    HI_U32 tx_sof_sack_received;
    HI_U32 tx_sound_sack_received;
    HI_U32 tx_backoff_end;

    HI_U32 tx_vcs_state_changed;
    HI_U32 tx_wait_response_timeout;
    HI_U32 tx_send_success;
    HI_U32 tx_send_fail;

    HI_U32 tx_phase_timeout;
    HI_U32 tx_rts_period_timeout;
    HI_U32 tx_buffer_clear;
    HI_U32 tx_test_msdu_send;

    HI_U32 tx_multi_send;
    HI_U32 tx_over_current;
    HI_U32 unknown_event_cnt;

    HI_U32 tx_sta_pin_set_high_cnt;
    HI_U32 tx_sta_pin_set_low_cnt;

}HI_DSID_MAC_TX_EVENT_INFO_S;


//�������ģ��ͳ�����ṹ�嶨��

#define LEAVE_TIME_NUM 5//20
#define CHANGE_PROXY_NUM 5//20
#define LITTLE_BEAT_TIME_NUM 10//20
#define PATH_DIFFICULT_TIME_NUM 10//20
#define RELAY_MAC_NUM 1//32
#define HARD_RCV_FIRST_LEVEL_BEA_TIME 24//48
#define FORM_TIME_NUM 10//20
#define JOIN_COUNT_ONE_FORM_NUM   FORM_TIME_NUM//20
#define LAST_JOIN_STA_NUM 10//150
#define ASSOC_FAIL_REASON_NUM 5
#define ASSOC_TROUBLE_STA_NUM 5//80
#define PATH_TROUBLE_STA_NUM 10//30
#define ASSOC_OBJECT_NUM DFX_ASSOC_OBJ_COUNT//5//20
#define CHANGE_PROXY_NUM 5//20
#define CHANGE_REASON_NUM 5//20
#define LEAVE_TIME_NUM 5//20

//��������������ʱ��ͳ����
typedef struct
{
    HI_U16 pad1[7];
    HI_U16 malloc_memory_err;

    HI_U8  ndm_mac[4][HI_PLC_MAC_ADDR_LEN];

    HI_U16 ndm_tei;
    HI_U16 ndm_tei_err_cnt;

    HI_U16 ndm_mac_duplicate_cnt;
    HI_U16 pad2[6];
    HI_U16 send_nm_query_cnt;                       //���͵Ĳ�ѯ�������Ա��ĵ�ͳ��

    HI_U16 send_nm_query_success_cnt;               //���͵Ĳ�ѯ�������Ա��ĳɹ���ͳ��
    HI_U16 get_ndm_proxy_err_cnt;                   //��ȡ�������Ĵ���վ��ʧ��

    HI_U16 find_join_sta_err_cnt;                   //��ѯ����վ��ʧ��
    HI_U16 rcv_nm_attr_cnt;                         //���յ����������Իظ�����
    HI_U16 rcv_nm_attr_success_cnt;                 //��ѯ�������Գɹ�����
    HI_U16 rcv_query_nm_attr_cnt;                   //���յ����������Բ�ѯ���ļ���

    HI_U16 send_nm_attr_cnt;                        //�ظ��������Բ�ѯ�ļ���
    HI_U16 rcv_hso_nm_query_cnt;                    //����HSO�Ĳ�ѯ�������Ա��ĵ�ͳ��

    HI_U32 rcv_nm_attr_success_time;                //��������ѯ�������Գɹ���ʱ��
    HI_U16 query_connect_info_malloc_err_cnt;       //�ϱ�����������Ի�ȡ�ڴ�ʧ��
    HI_U8  search_net_state;
    HI_U8  pad;

    HI_U16 get_base_info_err_cnt;                   //��ȡ���������Ϣʧ��
    HI_U16 rpt_base_info_err_cnt;                   //�ϱ����������Ϣʧ��
    HI_U32 clear_cur_disc_sta_cnt;                  //��������ǰ�������õĴ���ͳ��

    HI_U16 ndm_proxy_tei;                           //��������ǰ�Ĵ���tei
    HI_U16 send_ndm_attr_br_cnt;                    //���ͳ��������Ա��ĵ�ͳ��
    HI_U16 rcv_ndm_attr_br_cnt;                     //���ճ��������Ա��ĵ�ͳ��
    HI_U16 send_ndm_attr_br_err_cnt;                //���ͳ��������Ա���ʱ�����ͳ��
    HI_U16 search_net_start_cnt;                    //������������
    HI_U16 search_net_end_cnt;                      //����ֹͣ����
}HI_DSID_MAC_NDM_CONNECT_STAT_INFO_S;

//�������쳣��Ϣͳ��
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U32 send_msg_err_cnt;
    HI_U32 pad2;

    HI_U8  refresh_disc_static_mem_err_cnt;
    HI_U8  nfc_topo_static_mem_err_cnt;
    HI_U16 refresh_disc_start_timer_err_cnt;

    HI_U16 heartbeat_start_timer_err_cnt;
    HI_U16 nm_attr_query_start_timer_err_cnt;
}HI_DSID_MAC_NDM_ABNORMITY_INFO_S;

//��������ҵ��ͳ����Ϣ
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U32 tx_chl_type_no_proxy_path_cnt;
    HI_U32 tx_chl_type_local_broadcast_all_cnt;
    HI_U32 tx_chl_type_proxy_path_cnt;
    HI_U32 tx_chl_type_local_broadcast_cnt;
    HI_U32 tx_chl_type_unkown_cnt;

    HI_U32 rx_chl_type_no_proxy_path_cnt;
    HI_U32 rx_chl_type_local_broadcast_all_cnt;
    HI_U32 rx_chl_type_proxy_path_cnt;
    HI_U32 rx_chl_type_local_broadcast_cnt;
    HI_U16 rx_chl_type_unkown_cnt;
    HI_U16 rx_chl_duplicate_pk_cnt;         //���յ��ظ�����

    HI_U32 tx_remote_req_cnt;               //����Զ������ĸ���
    HI_U32 rx_remote_rsp_cnt;               //���յ�Զ��Ӧ�����
    HI_U32 hso_remote_req_cnt;              //HSO���͵�Զ���������
    HI_U32 hso_remote_rsp_cnt;              //HSO���յ�Զ��Ӧ�����

    HI_U32 hso_req_rsp_err_cnt;
    HI_U32 refresh_disc_cnt;                //ˢ�³������ķ����б�Ĵ���
    HI_U16 refresh_disc_malloc_fail_cnt;    //ˢ�³������ķ����б�Ĵ���
    HI_U16 refresh_disc_clean_path_fail_cnt;
    HI_U16 refresh_disc_msg_fail_cnt;
    HI_U16 hso_cmd_callback_malloc_fail_cnt;

    HI_U16 hso_cmd_callback_ndm_notify_msg_fail_cnt;
    HI_U16 hso_cmd_callback_para_err_cnt;
    HI_U32 hso_cmd_callback_impl_cnt;
    HI_U16 hso_cmd_callback_impl_mac_buffer_fail_cnt;
    HI_U16 hso_cmd_callback_impl_packet_frame_fail_cnt;
    HI_U16 hso_cmd_callback_impl_output_pk_msg_fail_cnt;
    HI_U16 MAC_SendMessage_set_white_list_cnt;

    HI_U32 MAC_SendMessage_cnt;
    HI_U16 MAC_SendMessage_msg_fail_cnt;
    HI_U16 MAC_SendMessage_msg_unknown_cnt;
    HI_U32 NvChangedNotify_cnt;                             //NVˢ�»ص�����
    HI_U32 MAC_SendMessage_unknown_id;                      //δ֪ID
}HI_DSID_MAC_NDM_STAT_INFO_S;

//������Զ��ͨ����ͳ����Ϣ
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U32 fragment_output_success_cnt;
    HI_U32 fragment_output_fail_cnt;
    HI_U32 fragment_output_mac_buff_fail_cnt;
    HI_U32 fragment_output_sleep_cnt;

    HI_U32 pipe_fragment_output_cnt;
    HI_U32 pipe_fragment_output_malloc_fail_cnt;
    HI_U32 pipe_fragment_output_msg_fail_cnt;
    HI_U32 pipe_output_cnt;

    HI_U32 pipe_output_mac_buffer_fail_cnt;
    HI_U32 pipe_output_success_cnt;
    HI_U32 pipe_output_fail_cnt;
    HI_U32 pk_parse_transf_target_addr_fail_cnt;

    HI_U32 pk_dump_pk_to_cvg_success_cnt;
    HI_U32 dump_pk_to_cvg_fail_cnt;
    HI_U32 pipe_send_ack_not_need_cnt;
    HI_U32 ack_parse_transf_target_addr_fail_cnt;

    HI_U32 ack_dump_pk_to_cvg_success_cnt;
    HI_U32 output_pk_fail_cnt;
    HI_U16 get_pipe_ctx_by_obj_fail_cnt;
    HI_U16 get_pipe_ctx_by_obj_unknown_id;
}HI_DSID_MAC_NDM_REMOTE_INFO_S;

//��������������ͳ����Ϣ
typedef struct
{
    HI_U32 handle_connect_msg_cnt;                      //���յ����ܷ��͵����ӱ�����Ϣ�ļ���
    HI_U16 handle_connect_msg_para_err_cnt;             //���յ�mac bufferΪ��ָ��
    HI_U16 handle_connect_msg_unknown_pkt_type_cnt;     //δ֪�ı�������

    HI_U32 send_heartbeat_cnt;
    HI_U16 send_heartbeat_mac_buffer_fail_cnt;
    HI_U16 send_heartbeat_send_msdu_to_tx_fail_cnt;

    HI_U16 heartbeat_timer_msg_fail_cnt;
    HI_U16 restart_connect_monitor_timer_fail_cnt;

    HI_U32 rcv_heartbeat_cnt;
    HI_U32 hande_heartbeat_connect_cnt;
    HI_U32 connect_monitor_timer_callback_cnt;

    HI_U32 ndm_fix_connect_cmd_cnt;
    HI_U32 ndm_clear_connect_cmd_cnt;

    HI_U8  ndm_fix_connect_mac[6];
    HI_U8  ndm_phase[4];
    HI_U8  is_ndm_connect;
    HI_U8  pad;
}HI_DSID_MAC_NDM_HEARTBEAT_INFO_S;

// �������ͳ����Ϣ
typedef struct
{
    HI_U32 pad1;
    HI_U32 send_ndm_meter_check_frame_cnt;
    HI_U32 rcv_ndm_unexpect_meter_check_frame_cnt;
    HI_U32 rcv_ndm_meter_check_frame_cnt;

    HI_U32 rcv_from_up_645_pk_cnt;
    HI_U32 rcv_from_up_645_pk_first_time;
    HI_U32 rcv_from_up_645_pk_last_time;
    HI_U32 rcv_from_down_645_pk_cnt;
    HI_U32 rcv_from_down_645_pk_first_time;
    HI_U32 rcv_from_down_645_pk_last_time;
    HI_U32 ndm_meter_check_connect_time;

    HI_U8  pad;           //����ɾ��
    HI_U8  ndm_meter_check_connect_flag;
    HI_U16 rcvd;

    HI_U8  meter_check_frame[16];                   //ǿ�����ӵ��������֡
    HI_U32 pad3;
}HI_DSID_MAC_NDM_METER_CHECK_INFO_S;

//NDCͨ��ͳ����Ϣ
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U8  AppInitSuccessCnt;
    HI_U8  AppInitFailCnt;
    HI_U16 HandleNmReqFailCnt;

    HI_U32 HandleNmReqNormalSuccessCnt;
    HI_U32 HandleNmReqTopSuccessCnt;
    HI_U16 TopMallocFailCnt;                    //������������ʱ��ȡ����ʧ��
    HI_U16 TopMsgFailCnt;                       //������������ʱ������Ϣʧ��

    HI_U32 MsgSendExpectObjIdCnt[5];
    HI_U32 MsgSendLocalMacCnt;
    HI_U32 MsgSendInvalidParaCnt;
    HI_U32 MsgSendRemoteReqCnt;
    HI_U32 MsgSendRemoteRspCnt;
    HI_U32 MsgSendUnknownMacCnt;

    HI_U16 MsgSendRemoteReqMallocFailCnt;
    HI_U16 MsgSendRemoteReqMsgFailCnt;

    HI_U32 MsgSendRemoteReqNullParaCnt;

    HI_U16 GetAppRcvFuncNullCnt;
    HI_U16 GetAppRcvFuncUnknownObjIdCnt;

    HI_U32 GetAppRcvFuncExpectObjIdCnt[5];

    HI_U32 NdcRxProcReomoteCnt;
    HI_U32 NdcRxProcLocalCnt;
    HI_U32 NdcRx2AppCnt;
    HI_U32 AppDataTransmitDLCnt;

    HI_U32 AppDataTransmitULCnt;

    HI_U32 ReportChlStatusCnt;

    HI_U8  ChlStatus;
    HI_U8  AppInitClearSuccessCnt;
    HI_U8  GetAppRcvFuncLastObjId;
    HI_U8  Pad2;
}HI_DSID_MAC_MDM_REGIST_INFO_S;

// ������NDCͨ��ͳ����
typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 NdcSendMsgFailCnt;
    HI_U32 NdcMallocFailCnt;
    HI_U32 NdcGetMacBufFailCnt;
    HI_U32 NdcInvalidCommandCnt;
    HI_U32 NdcReceiveCommandCnt;
    HI_U32 NdcSendTransCommandCnt;
    HI_U32 NdcReceiveTransCommandCnt;//ͨ��NDMͨ����������
    HI_U32 NdcSetTransCmdToMsduCnt;   //�����շ�ͨ����MSDU����
    HI_U32 NdcReceiveMsduFailCnt;
    HI_U32 NdcReceiveMsduCnt;
    HI_U32 NdcLocalBroadcastCnt;
    HI_U32 NdcUnicastCnt;
    HI_U32 NdcSendMsduFailCnt;
    HI_U32 NdcMacDevIdConfigCnt;
    HI_U32 NdcInnerCmdCallbackOverCnt;
    HI_U32 NdcOuterCmdCallbackOverCnt;
    HI_U32 NdcInnerConnectReqCnt;//�����ڲ���������
    HI_U32 NdcInnerConnectCnfCnt;//�����ڲ�����ȷ��

    HI_U8   NdcPlcIp[HI_IPV4_ADDR_LEN];//plc��ip��ַ

    HI_U8   NdcVer;
    HI_U8   NdcRcvDevIdReqCnt;   // ���ռ�����ID�������
    HI_U8   NdcDevId[6];

    HI_U8   NdcBcdBinId[5];
    HI_U8   NdcProdType;
    HI_U8   NdcProdTypeStaType;
    HI_U8   NdcRcvDevIdRspCnt;   // ���ռ�����ID��Ӧ���ĸ���

    HI_U32 NdcInvalidMsgCnt;
    HI_U32 NdcMsgIDInvalidCnt;
    HI_U32 NdcMsgIDCmdRcvCnt;
    HI_U32 NdcMsgIDCmdSendCnt;
    HI_U32 NdcMsgIDDevIdCfgCnt;
}HI_DSID_MAC_NDC_STAT_INFO_S;
// ����������ͳ�Ʋ���
typedef struct
{
    HI_U32 NdcMsgIDFcMonInfoCnt;
    HI_U32 NdcMsgIDBcnMonInfoCnt;
    HI_U32 NdcMsgIDFreqChangeCnt;
    HI_U32 NdcMsgIDFreqMonTimeoutCnt;
    HI_U32 NdcMsgIDConnMonTimeoutCnt;
    HI_U32 NdcMsgIDConnAbortCnt;
    HI_U32 NdcMsgIDFoundPreCcoCnt;
    HI_U32 NdcMsgIDNvCallbackCnt;
    HI_U32 NdcMsgIDShortScanTimeoutCnt;
    HI_U32 NdcMsgIDFreqChangeTimeoutCnt;
    HI_U32 NdcMsgIDSnidChangeCnt;
    HI_U32 NdcMsgIDSearchFinishCnt;
    HI_U32 NdcMsgIDConnectTimeoutCnt;
    HI_U32 NdcMsgIDIdleTimeoutCnt;
    HI_U32 NdcMsgIDReportTimeoutCnt;
    HI_U32 NdcRCVFcCnt;
    HI_U32 NdcRCVBcnCnt;
    HI_U32 NdcRCVFreqChangeCnt;
    HI_U32 NdcOuterNetworkSearchConnCnt;
    HI_U32 NdcActionSearchCnt;
    HI_U32 NdcActionSearchQryCnt;
    HI_U32 NdcActionConnectCnt;
    HI_U32 NdcActionDisConnectCnt;
    HI_U32 NdcActionCloseCnt;
    HI_U32 NdcActionParaInvalidCnt;
    HI_U32 NdcInvalidActionCnt;
    HI_U32 NdcSetNdmChlNvCnt;
    HI_U32 NdcMsgFcExpireCnt;
    HI_U32 NdcMsgPbExpireCnt;

    HI_U8  NdcCurFreq;
    HI_U8  NdcCurStatus;
    HI_U8  NdcAccessMode;
    HI_U8  NdcClearMsgCnt;

    HI_U32 NdcMsgIdExpireCheckTimeout;
}HI_DSID_MAC_NDC_SEARCH_STAT_INFO_S;

//������ͨ������������Ϣ
typedef struct
{
    HI_U32 HandleReqMsgCnt;
    HI_U32 HandleReqMsgRejectCnt;
    HI_U32 HandleReqMsgAcceptCnt;

    HI_U32 SendReqMsg2NdmNotifyCnt;

    HI_U16 SendReqMsg2NdmNotifyMsgErrCnt;
    HI_U16 SendReqMsg2NdmNotifyParaErrCnt;

    HI_U16 SendReqMsg2NdmNotifyMallocErrCnt;
    HI_U16 StopRspWaitTimerFailCnt;

    HI_U32 HandleRspMsgCnt;
    HI_U32 HandleRspTopoSuccessCnt;

    HI_U16 HandleRspMsgParaErrCnt;
    HI_U16 HandleRspUnexpectPktCnt;
    HI_U16 HandleRspReqFailCnt;
    HI_U16 ReqWaitTimerMsgFailCnt;

    HI_U32 SaveTopoPktNodeSuccessCnt;

    HI_U16 SaveTopoPktTooLargeCnt;
    HI_U16 SaveTopoPktGetStaValidErrCnt;

    HI_U32 SaveTopoPktNodeDuplicateCnt;

    HI_U32 ReportTopoToHsoCnt;
    HI_U16 ReportTopoToHsoPktFailCnt;
    HI_U16 ReportTopoToHsoMallocFailCnt;

    HI_U16 StopReqWaitTimerFailCnt;
    HI_U16 handle_query_topo_fc_para_err_cnt;

    HI_U32 handle_query_topo_fc_cnt;
    HI_U32 handle_query_topo_fc_new_req_cnt;

    HI_U32 report_topo_fc_ReportFragmentPacket_success_cnt;
    HI_U32 report_topo_fc_node_cnt;
    HI_U16 report_topo_fc_ReportFragmentPacket_fail_cnt;
    HI_U16 report_topo_fc_malloc_fail_cnt;

    HI_U16 report_topo_fc_GetStaValid_fail_cnt;
    HI_U16 HandleReqMsgMallocErrCnt;

    HI_U32 send_remote_pkt_to_fc_cnt;

    HI_U32 ReqWaitTimerHandlerCnt;
    HI_U32 RspWaitTimerHandlerCnt;

    HI_U16 HandleRspWaitTimerFailCnt;
    HI_U16 report_topo_fc_no_pkt_cnt;
}HI_DSID_MAC_NFC_STAT_INFO_S;

//����ģ��Ķ��ο����ӿ�ͳ����
typedef struct
{
    HI_U32 objCmdCnt;

    HI_U8  searchNetCtrlObjId;              //��ǰ�����������û�ID
    HI_U8  lastFreq;
    HI_U8  pad[2];

    HI_U32  lastSnid;

    HI_U32 closeWithInvalidSnid;
    HI_U32 invalidObjCmdCnt;                //�Ƿ��û�������
}HI_DSID_MAC_NDC_OBJ_STAT_INFO_S;

typedef struct
{
    HI_U8    sta_mac[6];
    HI_U16  proxy_tei;
    HI_U32  join_time;
}LAST_JOIN_STA_INFO_S;


typedef struct
{
    HI_U8    sta_mac[6];
    HI_U16  leave_count;
    HI_U16  change_proxy_count;
    HI_U16  change_proxy_mac[CHANGE_PROXY_NUM][6];
    HI_U16  rvsd;
    HI_U32  leave_time[LEAVE_TIME_NUM];
}ASSOC_TROUBLE_STA_INFO_S;


typedef struct _path_trouble_sta_info_st_
{
    HI_U8    sta_mac[6];
    HI_U8    little_heart_beat_count;
    HI_U8    path_difficult_count;
    HI_U32  little_beat_time[LITTLE_BEAT_TIME_NUM];
    HI_U32  path_difficult_time[PATH_DIFFICULT_TIME_NUM];
}PATH_TROUBLE_STA_INFO_S;


typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 change_topo_times;
    HI_U32 last_forming_time;
    HI_U32 disable_relay_num;

    HI_U8  relay_mac[RELAY_MAC_NUM][6];
    HI_U8 last_join_sta_mac[6];

    HI_U32 max_pure_relay_num;
    HI_U32 disable_sta_num;
    HI_U32 hard_rcv_fist_level_bea_now;
    HI_U32 hard_rcv_fist_level_bea_time[HARD_RCV_FIRST_LEVEL_BEA_TIME];
    HI_U32 total_form_count;
    HI_U32 form_time[FORM_TIME_NUM];
    HI_U32 join_count_one_form[JOIN_COUNT_ONE_FORM_NUM];

    LAST_JOIN_STA_INFO_S  last_join_sta[LAST_JOIN_STA_NUM];
    ASSOC_TROUBLE_STA_INFO_S  assoc_trouble_sta_info[ASSOC_TROUBLE_STA_NUM];
    PATH_TROUBLE_STA_INFO_S    path_troble_sta_info[PATH_TROUBLE_STA_NUM];
}HI_DSID_MAC_CCO_NETWORK_STATISTIC_INFO_S;

//��������Ƶ������Ӧ������ص�ͳ����
typedef struct
{
    HI_U16 set_chl_state_msg_err_cnt;                   //����ͨ��״̬����Ϣʧ�ܴ���
    HI_U16 set_chl_state_unexpect_cnt;                  //����ͨ��״̬������Ԥ�ڵĴ���

    HI_U16 refresh_topo_msg_err_cnt;                    //���͸������˵���Ϣʧ�ܴ���
    HI_U16 pre_white_list_sem_fail_cnt;                 //��ǰһ�����������ź�������ʧ��

    HI_U16 malloc_fail_cnt;                             //���������ʱ��ȡ�ڴ�ʧ��
    HI_U16 save_pre_white_list_cover_cnt;               //����ǰһ��������ʱ���ָ���

    HI_U16 compare_white_list_change_null_cnt;          //�Ƚϰ�����ʱ���ֿ�ָ��
    HI_U16 hso_set_white_list_msg_err_cnt;              //HSO���ð���������Ϣʧ��
}HI_DSID_MAC_NM_FA_STAT_INFO_S;

typedef struct _DFX_ASSOC_OBJECT_STRUCT_
{
    HI_U16 tei;
    HI_U8 success_ratio;
    HI_U8 phy_in;

    HI_U16  rcv_beacon_count;
    HI_U16  assoc_req_count;
    HI_U32  first_assoc_bpc;
}DFX_ASSOC_OBJECT_STRUCT;


#define DFX_ASSOC_OBJ_COUNT 10
#define DFX_PROXY_SUM 5
typedef struct _DFX_STA_ASSOC_INFO_
{
    HI_U8 send_success_ratio;
    HI_U8  mac_rejoin_reason;
    HI_U8 pad[2];

    HI_U16 tx_resend_count;//tx
    HI_U16 assoc_req_count;

    HI_U16 rcv_beacon_count;
    HI_U16 rcv_beacon_count_of_proxy[DFX_PROXY_SUM];

    DFX_ASSOC_OBJECT_STRUCT assoc_object[DFX_ASSOC_OBJ_COUNT];
}DFX_STA_ASSOC_INFO;

#define DFX_RCV_BEACON_BPC_COUNT 20
typedef struct _DFX_PROXY_ASSOC_INFO_
{
    HI_U8 success_ratio_uplink;
    HI_U8 success_ratio_downlink;
    HI_U16 tei;
    HI_U16 send_beacon_count;
    HI_U16 rcv_beacon_count;
}DFX_PROXY_ASSOC_INFO;

//������ģ��ͳ�����ṹ�嶨��

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 g_get_buffer_cnt;
    HI_U32 g_set_evt_cnt;
    HI_U32 g_rcv_evt_cnt;
    HI_U32 g_dlv_cov_cnt;
}HI_DSID_MAC_RSB_MPDU_INFO_CNT_S;

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 mpdu_frame_count;
    HI_U32 lid_entry_count;
    HI_U32 stei_head_node_count;
    HI_U32 lid_head_node_count;
    HI_U32 not_rsb_list_count;
    HI_U32 cannot_rsb_list_count;
}HI_DSID_MAC_RSB_NODE_INFO_CNT_S;

//������쳣ͳ��
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U32 no_unrsb_free_node_cnt;
    HI_U32 notify_rsb_err_cnt;
    HI_U32 dlv_cvg_err_cnt;
}HI_DSID_MAC_RSB_ABNORMITY_INFO_S;

//�ֶ���ģ��ͳ�����ṹ�嶨��

typedef struct _phase_queue_cnt_info_st_
{
    HI_U8 q_all_frame_count_in_pri_zero;
    HI_U8 q_all_frame_count_in_pri_one;
    HI_U8 q_all_frame_count_in_pri_two;
    HI_U8 q_all_frame_count_in_pri_three;
}phase_queue_cnt_info_st;

typedef struct
{
    HI_OBJ64_S stObjCnt;
    phase_queue_cnt_info_st phase_a_queue_frame_cnt;
    phase_queue_cnt_info_st phase_b_queue_frame_cnt;
    phase_queue_cnt_info_st phase_c_queue_frame_cnt;
} HI_DSID_SEG_FRAME_CNT_IN_DIFF_TXQUEUE_S;


typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 rts_cts_count;
    HI_U32 sack_count;
    HI_U32 beacon_count;
    HI_U32 sounding_count;
    HI_U32 buffer_used_size;
    HI_U32 buffer_packet_count;
    HI_U32 unicast_count;
    HI_U32 broadcast_count;
    HI_U32 short_packet_count;
    HI_U32 long_packet_count;
    HI_U32 resend_count;
    HI_U32 buffer_overflow_count;
    HI_U32 collision_count;
}HI_DSID_MAC_TX_MPDU_SEND_COUNT_S;

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U8  all_frame_count;                       //��������������֡�ĸ���;
    HI_U8  highest_priority;                      //�����б��ĵ�������ȼ�;
    HI_U16 rsvd;
}HI_DSID_MAC_TX_ALL_QUEUE_INFO_S;

typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U8   lid;                    //���ӱ�ʶ��;
    HI_U8   q_frame_count;          //�Ӷ����д����͵ı�����Ŀ
    HI_U8   q_all_frame_count;      //�Ӷ��������еı�����Ŀ
    HI_U8   rsvd;
}HI_DSID_MAC_TX_QUEUE_INFO_S;
//DFXģ��ͳ��������
//dfxģ���ڲ�����ͳ��
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U32 get_sys_status_err_cnt;              //��NV��ȡϵͳ����ģʽ����
    HI_U32 sys_status;                          //ϵͳ�Ĺ���ģʽ
    HI_U32 mpdu_node_null_cnt;                  //mpduָ��Ϊ��
    HI_U32 send_msg_err_cnt;                    //������Ϣʧ��
    HI_U32 reg_msdu_feature_err_cnt;            //ע�ᱨ������ʧ��
    HI_U32 para_null_err_cnt;                   //����Ϊ��ָ��
}HI_DSID_MAC_DFX_SYS_ERR_S;

//dfxģ������ͳ��
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U32 query_request_cnt;                   //HSO��ѯͳ��
    HI_U32 feature_match_cnt;                   //����ƥ��ɹ�
    HI_U32 no_feature_match_cnt;
    HI_U32 send_eng_event_err_cnt;

    HI_U32 msg_nv_refresh_notify_cnt;
    HI_U32 msg_dfx_ctrl_pk_cnt;
    HI_U32 msg_dfx_connect_pk_cnt;              //�յ����ܵĳ�����ͨ�����ӱ���
    HI_U32 msg_dfx_ndm_notify_cnt;

    HI_U32 msg_dfx_query_notify_cnt;
    HI_U32 msg_dfx_mgt_msg_cnt;
    HI_U32 msg_unknown_cnt;
    HI_U32 topo_success_cnt;

    HI_U32 discover_list_success_cnt;
    HI_U32 discover_list_sm_success_cnt;
    HI_U32 router_table_success_cnt;
    HI_U32 all_sta_info_success_cnt;

    HI_U32 other_query_success_cnt;

    HI_U16 topo_fail_cnt;
    HI_U16 discover_list_fail_cnt;
    HI_U16 discover_list_sm_fail_cnt;
    HI_U16 router_table_fail_cnt;

    HI_U16 other_query_fail_cnt;
    HI_U16 all_sta_info_fail_cnt;
    HI_U32 violence_sta_qry_success_cnt;
    HI_U32 violence_sta_qry_fail_cnt;

    HI_U32 fl_topo_ref_qry_success_cnt;
    HI_U32 fl_topo_ref_qry_fail_cnt;

    HI_U32 fl_topo_rec_qry_success_cnt;
    HI_U32 fl_topo_rec_qry_fail_cnt;

    HI_U32 usFragmentCount;
    HI_U32 avail_buf_length;

}HI_DSID_MAC_DFX_STAT_INFO_S;

//*****************************************************************************
// MAC��Ϣ����
//*****************************************************************************
// ��Χ [0500, 0900)
// 1�� ����ҵ����Ϣ  [0x500~0x564)
// 2,  ·��          [0x564~0x5c8)
// 3,  ���ͻ�        [0x5c8~0x62c)
// 4,  �ֶ���        [0x62c~0x690)
// 5,  PHY           [0x690~0x6D0)
// 6,  ������        [0x6D0~0x710)
// 7,  beacon        [0x710~0x750)
// 8,  conv          [0x750~0x790)
// 9,  RX            [0x790~0x7D0)
// 10, NTB           [0x7D0~0x810)
// 11, ������        [0x810~0x830)
// 12, Ƶ������Ӧ    [0x830~0x850)
// 13, ����ģ��      [0x851~0x870]
// 14, EMCģ��       [0x871~0x890]
// 15, �͹���        [0x891~0x8A0]
// 16, ����          [0x8A1~0x900)
//*****************************************************************************
// TODO
// ע��ID�ŵĶ���һ��Ҫ���� �ĵ� Ҫ�������塣
// ID:HI_DMID_XXX_XXX;
// ��Ӧ�Ľṹ��: HI_DMID_XXX_XXX_S;

//**************************************************
// ����ģ����Ϣ��ʱ
// ����ģ����Ϣ��Χ [0x851~0x870]
#define HI_DMID_SWITCH_OFFSET                   (0x851)
#define HI_DMID_SWITCH_ETH_PK                   (HI_DMID_SWITCH_OFFSET + 0)
#define HI_DMID_SWITCH_ARP_PK                   (HI_DMID_SWITCH_OFFSET + 1)
#define HI_DMID_SWITCH_IP_PK                    (HI_DMID_SWITCH_OFFSET + 2)
#define HI_DMID_SWITCH_TCP_PK                   (HI_DMID_SWITCH_OFFSET + 3)
#define HI_DMID_SWITCH_UDP_PK                   (HI_DMID_SWITCH_OFFSET + 4)
#define HI_DMID_SWITCH_ICMP_PK                  (HI_DMID_SWITCH_OFFSET + 5)
#define HI_DMID_SWITCH_IGMP_PK                  (HI_DMID_SWITCH_OFFSET + 6)

typedef struct
{
    HI_U8  destination[6];
    HI_U8  source[6];
    HI_U16 type;
    HI_U16 link_layer;
}HI_ETHERNET_HEADER_STRU;

typedef struct
{
    HI_U16 hardware_type;
    HI_U16 protocol_type;

    HI_U8  hardware_size;
    HI_U8  protocol_size;
    HI_U16 opcode;

    HI_U8  sender_mac_address[6];
    HI_U8  sender_ip_address[4];
    HI_U8  target_mac_address[6];
    HI_U8  target_ip_address[4];
}HI_ARP_STRU;

typedef struct
{
    HI_U8  version : 4;
    HI_U8  header_length : 4;
    HI_U8  type_of_service;
    HI_U16 total_length;

    HI_U16 identifier;
    HI_U16 rsvd : 1;
    HI_U16 ip_df : 1;
    HI_U16 ip_mf : 1;
    HI_U16 fragment_offset : 13;

    HI_U8  ttl;
    HI_U8  protocol;
    HI_U16 header_checksum;

    HI_U8  source_ip_address[4];
    HI_U8  dest_ip_address[4];
}HI_IP_HEADER_STRU;

typedef struct
{
    HI_U8  type;
    HI_U8  code;
    HI_U16 checksum;
}HI_ICMP_HEADER_STRU;

typedef struct
{
    HI_U8  version : 4;
    HI_U8  type : 4;
    HI_U8  rsvd;
    HI_U16 checksum;

    HI_U8 group_address[4];
}HI_IGMP_HEADER_STRU;

typedef struct
{
    HI_U16 source_port;
    HI_U16 destination_port;

    HI_U32 sequence_number;
    HI_U32 ack_number;

    HI_U16 header_length : 4;
    HI_U16 rsvd : 6;
    HI_U16 urg : 1;
    HI_U16 ack : 1;
    HI_U16 psh : 1;
    HI_U16 rst : 1;
    HI_U16 syn : 1;
    HI_U16 fin : 1;
    HI_U16 window_size;

    HI_U16 checksum;
    HI_U16 urgent_pointer;
}HI_TCP_HEADER_STRU;

typedef struct
{
    HI_U16 source_port;
    HI_U16 destination_port;
    HI_U16 length;
    HI_U16 checksum;
}HI_UDP_HEADER_STRU;

typedef struct
{
    HI_ETHERNET_HEADER_STRU eth_header;     // ETH֡ͷ(16byte)
    HI_ARP_STRU arp_header;                 // ARP����(28byte)
}DIAG_CMD_ARP_STRU;

#define PS_DFX_TCP_DATA_SIZE            (72)
typedef struct
{
    HI_ETHERNET_HEADER_STRU eth_header;     // ETH֡ͷ(16byte)
    HI_IP_HEADER_STRU ip_header;            // IP����ͷ(20byte)
    HI_TCP_HEADER_STRU tcp_header;          // TCP����ͷ(20byte)
    HI_U8 data[PS_DFX_TCP_DATA_SIZE];
}DIAG_CMD_TCP_STRU;

#define PS_DFX_UDP_DATA_SIZE            (84)
typedef struct
{
    HI_ETHERNET_HEADER_STRU eth_header;     // ETH֡ͷ(16byte)
    HI_IP_HEADER_STRU ip_header;            // IP����ͷ(20byte)
    HI_UDP_HEADER_STRU udp_header;          // UDP����ͷ(8byte)
    HI_U8 data[PS_DFX_UDP_DATA_SIZE];
}DIAG_CMD_UDP_STRU;

typedef struct
{
    HI_ETHERNET_HEADER_STRU eth_header;     // ETH֡ͷ(16byte)
    HI_IP_HEADER_STRU ip_header;            // IP����ͷ(20byte)
    HI_ICMP_HEADER_STRU icmp_header;        // ICMP����ͷ(4byte)
}DIAG_CMD_ICMP_STRU;

typedef struct
{
    HI_ETHERNET_HEADER_STRU eth_header;     // ETH֡ͷ(16byte)
    HI_IP_HEADER_STRU ip_header;            // IP����ͷ(20byte)
    HI_IGMP_HEADER_STRU igmp_header;        // IGMP����ͷ(8byte)
}DIAG_CMD_IGMP_STRU;

#define PS_DFX_ETH_DATA_SIZE            (112)
typedef struct
{
    HI_ETHERNET_HEADER_STRU eth_header;     // ETH֡ͷ(16byte)
    HI_U8 data[PS_DFX_ETH_DATA_SIZE];
}DIAG_CMD_ETH_STRU;

#define PS_DFX_IP_DATA_SIZE             (92)
typedef struct
{
    HI_ETHERNET_HEADER_STRU eth_header;     // ETH֡ͷ(16byte)
    HI_IP_HEADER_STRU ip_header;            // IP����ͷ(20byte)
    HI_U8 data[PS_DFX_IP_DATA_SIZE];
}DIAG_CMD_IP_STRU;


//Ƶ������Ӧ��Ϣ����
//Ƶ������Ӧ    [0X830~0X850)
#define HI_DMID_FA_OFFSET                           0x830
#define HI_DMID_FA_START_EVALUATE                   (HI_DMID_FA_OFFSET + 0x00)  //��������
#define HI_DMID_FA_STOP_EVALUATE                    (HI_DMID_FA_OFFSET + 0x01)  //ֹͣ����
#define HI_DMID_FA_EVALUATE_STATE                   (HI_DMID_FA_OFFSET + 0x02)  //�������м�״̬

#define HI_DMID_TF_RECEIVE_NTB                      (HI_DMID_FA_OFFSET + 0x03)  //̨��ʶ�����NTB�ϱ�
#define HI_DMID_TF_DETECT_SEQUENCE                  (HI_DMID_FA_OFFSET + 0x04)  //̨��ʶ���������ϱ�
#define HI_DMID_TF_RESULT_REQ                       (HI_DMID_FA_OFFSET + 0x05)  //CCO���͵�ʶ������ѯ
#define HI_DMID_TF_RESULT_RSP                       (HI_DMID_FA_OFFSET + 0x06)  //CCO�յ���ʶ����Ӧ��
#define HI_DMID_TF_SOFT_INFO                        (HI_DMID_FA_OFFSET + 0x07)  //̨��ʶ��soft��Ϣ
#define HI_DMID_TF_SOFT_SEQ_INFO                    (HI_DMID_FA_OFFSET + 0x08)  //̨��ʶ��soft��Ϣ

#define HI_DMID_PI_ZERO_CROSS_NTB_REQ               (HI_DMID_FA_OFFSET + 0x09)  //CCO���͵Ĺ���NTB��ѯ
#define HI_DMID_PI_ZERO_CROSS_NTB_RSP               (HI_DMID_FA_OFFSET + 0x0A)  //CCO�յ��Ĺ���NTB��ѯ��Ӧ��
#define HI_DMID_TF_START                            (HI_DMID_FA_OFFSET + 0x0B)  //̨��ʶ������
#define HI_DMID_TF_STOP                             (HI_DMID_FA_OFFSET + 0x0C)  //̨��ʶ��ֹͣ
#define HI_DMID_TF_SEND_SEQ                         (HI_DMID_FA_OFFSET + 0x0D)  //����̨�����з���
#define HI_DMID_TF_QUERY_ONE_TURN_OVER              (HI_DMID_FA_OFFSET + 0x0E)  //��ѯһ�ֽ���
#define HI_DMID_TF_SWITCH_PHASE                     (HI_DMID_FA_OFFSET + 0x0F)  //�������з���
#define HI_DMID_PI_CCO_STA_NTB_DIFF_REPORT          (HI_DMID_FA_OFFSET + 0x10)   //CCO��STA NTB��ֵͳ���ϱ�
#define HI_DMID_TF_NDM_REAL_NOISE_REPORT            (HI_DMID_FA_OFFSET + 0x11)   // NDM�ϱ�ʵʱ�Ŷ�ֵ
#define HI_DMID_TF_RESULT_REOPRT                    (HI_DMID_FA_OFFSET + 0x12)     // ����ϱ�
#define HI_DMID_TX_COLLISION_TEST_INFO              (HI_DMID_FA_OFFSET + 0x13)
#define HI_DMID_TF_NTB_POWER_REOPRT                 (HI_DMID_FA_OFFSET + 0x14)     // v200���ڽ���ϱ�
#define HI_DMID_TF_LINE_DATA_REOPRT                 (HI_DMID_FA_OFFSET + 0x15)     // V200��·��Ϣ�ϱ�
#define HI_DMID_TF_NOISE_REPORT                     (HI_DMID_FA_OFFSET + 0x16)     // V200�����ϱ�
#define HI_DMID_TF_CCO_PHASE_CHECK_REPORT           (HI_DMID_FA_OFFSET + 0x17)     // V200��CCO������
#define HI_DMID_NTB_FREQERR                               (HI_DMID_FA_OFFSET + 0x18)
#define HI_DMID_TX_COLLISION_RX_INFO               (HI_DMID_FA_OFFSET + 0x19)
#define HI_DMID_TFP_IDENITFY_RESULT                 (HI_DMID_FA_OFFSET + 0x1A)     // �ϱ�����վ��ʶ����
#define HI_DMID_TFP_NEIGHBOR_INFO                   (HI_DMID_FA_OFFSET + 0x1B)     // �ϱ�ÿ��վ�㷽�������
#define HI_DMID_TFP_SET_CMD_MODE                    (HI_DMID_FA_OFFSET + 0x1C)     // ������ֹͣ��Ϣ
#define HI_DMID_PN_RESULT                           (HI_DMID_FA_OFFSET + 0x1D)     // ����߷���ʶ����Ϣ
#define HI_DMID_NOTIFY_START_COLLECT_NTB_INFO       (HI_DMID_FA_OFFSET + 0x1E)     // CCO֪ͨSTA����NTB���ݲɼ�


typedef struct
{
    HI_U32 send_dt;
    HI_U32 send_tmi;
    HI_U32 current_send_ntb;
    HI_U32 need_duration;

    HI_U32 pad[4];
}HI_DMID_TX_TEST_INFO_S;
typedef struct
{
    HI_U32 tx_buffer_conlilision_ntb;

    HI_U32 pad[2];
}HI_DMID_TX_COLLISION_TEST_INFO_S;
//NDM��λ�л���Ϣ
typedef struct
{
    HI_U8  send_phase_cnt;                  //������λ����
    HI_U8  send_edge_cnt;                   //�����ؼ���
    HI_U8  pad1[2];

    HI_U32 time_stamp;              //ʱ�������λs

    HI_U32 pad[4];
}HI_DMID_TF_SWITCH_PHASE_S;


//����̨�����з�����Ϣ
typedef struct
{
    HI_U32 net_identify_cnt;               //ʶ���ִ�
    HI_U32 time_stamp;                      //��λ:s

    HI_U32 pad2[3];
}HI_DMID_TF_SEND_SEQ_S;

//��ѯһ�ֽ�����Ϣ
typedef struct
{
    HI_U8  pad1;
    HI_U8  query_turn_cnt;
    HI_U16 cur_turn_timeout_sta_cnt;        //���ֳ�ʱ��վ����

    HI_U16 cur_turn_identifying_sta_cnt;    //��������ʶ���վ����
    HI_U16 cur_turn_fail_sta_cnt;           //����ʶ��ʧ��:��ѯ��ʱվ����

    HI_U32 pad[4];
}HI_DMID_TF_QUERY_ONE_TURN_OVER_S;

//̨��ʶ��������Ϣ
typedef struct
{
    HI_U8  reason;                          //������ԭ��
    HI_U8  identify_control_mode;           //̨��ʶ��Ŀ���ģʽ
    HI_BOOL is_network_formed;              //�����Ƿ����
    HI_BOOL is_business_busy;               //APPҵ���Ƿ�æ

    HI_U8  pad1;
    HI_U8  pad[3];

    HI_U32 ret_result;                      //�����Ľ��

    HI_U32 start_bpc;                       //����̨��ʶ��ʱ��bpc
    HI_U32 pad2[3];
}HI_DMID_TF_START_S;

//̨��ʶ��ֹͣ��Ϣ
typedef struct
{
    HI_U8  stop_type;                       //ֹͣ������,0:����ֹͣ,1:�쳣ֹͣ
    HI_U8  reason;                          //ֹͣ��ԭ��
    HI_U8  identify_control_mode;           //̨��ʶ��Ŀ���ģʽ
    HI_U8  pad;

    HI_U32 stop_bpc;                        //ֹ̨ͣ��ʶ��ʱ��bpc
    HI_U32 pad2[3];
}HI_DMID_TF_STOP_S;

typedef struct
{
    HI_U8 index;
    HI_U8 alg_index;
    HI_U8 limit_index;
    HI_U8 pad;

    HI_U8 detect_sequence[48];
}HI_DMID_TF_DETECT_SEQUENCE_S;

#define TF_ZERO_CROSS_REPORT_NTB_MAX_CNT      27           //һ���ϱ�NTB������

typedef struct
{
    HI_U8 index;
    HI_U8 identify_result;
    HI_U8 transformer_index;
    HI_U8 phase_index;

    HI_U8 rx_gear;                     //ndm-ndm�Ľ��ջ���λ
    HI_U8 pad1[3];
    HI_U16 wobble_scope;
    HI_U16 noise_scope;

    HI_U32 ntb_data[TF_ZERO_CROSS_REPORT_NTB_MAX_CNT];
}HI_DMID_TF_RECEIVE_NTB_S;

//Ϊͳһ����ģ�����ά�ɲ�궨��
#define TF_ZERO_CROSS_ALGORITHM_CNT             2
#define TF_ZERO_CROSS_NOISE_LIMIT_CNT           7
#define TF_ZERO_CROSS_TH_CNT (TF_ZERO_CROSS_NOISE_LIMIT_CNT * TF_ZERO_CROSS_ALGORITHM_CNT)

#define TF_NDM_WINDOW_MAX_CNT                   1       // ��ndm-ndm�ĵ�Ե�ʶ�𴰿ڸ�Ϊ1��

#define TF_STA_WINDOW_MAX_CNT                   4
typedef struct
{
    HI_U16 window_soft_val[TF_NDM_WINDOW_MAX_CNT][TF_ZERO_CROSS_TH_CNT];//TF_ZERO_CROSS_NOISE_LIMIT_CNT * TF_ZERO_CROSS_ALGORITHM_CNT
}HI_DMID_TF_SOFT_VAL_S;

typedef struct
{
    HI_U8 seq_phase[TF_NDM_WINDOW_MAX_CNT][TF_ZERO_CROSS_TH_CNT];//TF_ZERO_CROSS_NOISE_LIMIT_CNT * TF_ZERO_CROSS_ALGORITHM_CNT
    HI_U8 seq_index[TF_NDM_WINDOW_MAX_CNT][TF_ZERO_CROSS_TH_CNT];//TF_ZERO_CROSS_NOISE_LIMIT_CNT * TF_ZERO_CROSS_ALGORITHM_CNT
}HI_DMID_TF_SOFT_SEQ_INFO_S;

//̨��ʶ��������
typedef struct
{
    HI_U16 tei;
    HI_U8  mac[6];

    HI_U8  identify_turn_cnt;           //��վ�����ܹ�ȷ�Ͻ��յ���̨��ʶ����ʼ�ִκ�
    HI_U8  query_timeout_cnt;           //��ѯ��ʱ����
    HI_U8  pad1[2];

    HI_U32 pad2[4];
}HI_DMID_TF_RESULT_REQ_S;

//̨��ʶ����Ӧ��
typedef struct
{
    HI_U16 tei;
    HI_U8  mac[6];

    HI_U8  identify_state;                                  //ʶ��״̬
    HI_U8  tf_result;                                       //̨��ʶ����
    HI_U8  identify_state_err_cnt;                          //ʶ��״̬�������
    HI_U8  seq_type;                                         // ��������

    HI_U32 identify_success_bpc;                            //ʶ��ɹ�ʱ��bpc

    HI_U8  noise_limit_index;                               //ʶ��ɹ�ʱ����������
    HI_U8  alg_index                : 2;                    //ʶ��ɹ�ʱ�ļ���㷨
    HI_U8  rcv_level                : 2;                    //ʶ��ɹ�ʱ�Ĺ�����λ
    HI_U8  pad2                     : 4;
    HI_U16 soft_val[TF_ZERO_CROSS_NOISE_LIMIT_CNT];         //��ͬȥ�����޼��ɹ�ʱ����ֵ��

    HI_U8  seq_index[TF_ZERO_CROSS_NOISE_LIMIT_CNT];        //��ͬȥ�����޼�����к�
    HI_BOOL is_exist_diff_seq;                              //��ͬ�����Ƿ���ڲ�ͬ�������

    HI_U16 signal_mean;                                     //�źž�ֵ
    HI_U16 noise_mean;                                      //������ֵ
    HI_U16 other_signal_mean;                               //�Ǳ�̨��������źž�ֵ
    HI_U16 pad;

    HI_U32 other_snid : 24;
    HI_U32 other_freq : 8;

    HI_U16 noise_avg[2];                                //����������ֵ
}HI_DMID_TF_RESULT_RSP_S;

//��λʶ��������
typedef struct
{
    HI_U16 tei;
    HI_U8  phase;                                   //ʶ�������λ
    HI_U8  state;                                   //��λʶ��״̬

    HI_U8  timeout_cnt;                             //ʶ��ʱ����
    HI_U8  fail_cnt;                                //ʶ��ʧ�ܴ���(�յ�Ӧ�𣬵�δʶ�����λ)
    HI_U8  pn_result;                               // ʶ��������
    HI_U8  pn_state;                                // ʶ���״̬

    HI_U32 time_stamp;                              //ʱ�䣬��λms

    HI_U8  phase_b;
    HI_U8  phase_c;
    HI_U8  pn_result_b;
    HI_U8  pn_result_c;
    
    HI_U32 cur_ntb;                                 // ��ǰ��NTB
    HI_U32 start_collect_ntb;                       // ��ʼ�ɼ���NTB

    HI_U8 identify_mode;
    HI_U8 pad[3];
}HI_DMID_PI_ZEROC_CROSS_NTB_REQ_S;

#define HI_DMID_PI_ZEROC_CROSS_NTB_RSP_NTB_NUM_MAX      26      //��λʶ����Ӧ���NTB����

//��λʶ����Ӧ��
typedef struct
{
    HI_U16 tei;
    HI_U8  index;                                               //���
    HI_U8  ntb_num;

    HI_U8  phase_cnt[4];                                        //ʶ������ÿ����λ��ͳ��ֵ
    HI_U32 time_stamp;                                          //ʱ���

    HI_U32 cco_cur_ntb;                                         //CCO�˻��������NTB
    HI_U8  phase_cnt2[4];                                       //�����½���ʶ�������
    HI_U8  identify_mode;                                       // ʶ��ģʽ
    HI_U8  edge_mode;                                           // ����
    HI_U8  phase_state;                                         // ��λʶ��״̬
    HI_U8  pn_state;                                            // ���ʶ��״̬

    HI_U32 ntb[HI_DMID_PI_ZEROC_CROSS_NTB_RSP_NTB_NUM_MAX];     //���յ��Ĺ���NTB
}HI_DMID_PI_ZEROC_CROSS_NTB_RSP_S;

typedef struct
{
    HI_U8 mac[6];
    HI_U16 tei;

    HI_U16 cco_sta_ntb_diff_cnt;
    HI_U8 pn_result;
    HI_U8 identify_mode;

    HI_U32 cco_sta_ntb_diff[18];
    HI_U32 first_sta_ntb;
    HI_U32 last_cco_ntb;
}HI_DMID_PI_CCO_STA_NTB_DIFF_S;

typedef struct
{
    HI_U16 tei;               //tei
    HI_U8 up_ntb_cnt;         //STAƥ��CCO�������ܸ���
    HI_U8 period_num;         //�������ڸ���

    HI_U8 right_period_num;   //��������ڸ��������ڸ���
    HI_U8 up_ntb_index_last;  //����һ��������Index
    HI_U8 pn_result;          //��λʶ����
    HI_U8 identify_mode;      // ʶ��ģʽ
}HI_DMID_PN_RESULT_S;

typedef struct
{
    HI_U8 broadcast_cnt;
    HI_U8 identify_mode;
    HI_U8 collect_num;
    HI_U8 is_increase_seq;

    HI_U32 start_collect_ntb;
    HI_U32 cur_ntb;

    HI_U8 dest_mac[6];
    HI_U8 pad2[2];
}HI_DMID_CCO_NOTIFY_STA_COLLECT_NTB_INFO_S;


typedef struct
{
    HI_U32 real_noise;
    HI_U32 pad;
}HI_DMID_TF_NDM_REAL_NOISE_S;

typedef struct
{
    HI_U32 report_time;
    HI_U32 power_period_cnt;

    HI_BOOL is_success;
    HI_BOOL is_code_crash;
    HI_U8 seq_type;                          // ��������0:13bit,1:127bit
    HI_U8 best_voltage;                       // ��õ�ѹ�� 0-7

    HI_U8 best_line;                          // 0-31����·
    HI_U8 noise_limit_index;
    HI_U8 alg_index;
    HI_U8 phase_index;

    HI_U8 seq_index;
    HI_U8 level;                                    // ʶ��ɹ�ʱ��λ
    HI_U16 soft_value;

    HI_U16  wobble_scope;                     //�Ŷ����Ⱦ�ֵ
    HI_U16  noise_scope;                      //�������Ⱦ�ֵ

    HI_U16 normalized_wobble_scope;               // ��һ���Ŷ�ֵ
    HI_U16 pad;

    HI_U8 cco_mac[6];
    HI_U8 sta_v200_seq_index[5];              // V200��staʶ���5����������
    HI_U8 pad1[13];
}HI_DMID_TF_RESULT_REOPRT_S;

// ÿ���ж��ϱ�����
typedef struct
{
    HI_U32 ntb_result;
    HI_U32 power_period_cnt;
    HI_U8 t_in_valid_cnt;
    HI_U8 pad[3];
}HI_DMID_TF_NTB_POWER_STRU;

// ÿ·��оƬ�ϱ�����
typedef struct
{
    HI_U8 line_index;
    HI_BOOL is_valid;                  // �����Ƿ���Ч
    HI_U8 win_use_index;
    HI_U8 max_soft_cnt;

    HI_U16 seq_index;                 // ����id
    HI_U16 soft_value;                 // ��ֵ
    HI_U16 smean;                    // �ź�ֵ
    HI_U16 noise;                    // ����ֵ

    HI_BOOL is_nevol;               // �Ƚϱ����������Ƿ�����
    HI_U8 seq_type;                 // ��������
    HI_U8 pad[2];

    HI_U32 power_period_cnt;               // ���μ���
    HI_U32 ntb;
}HI_DMID_TF_LINE_DATA_STRU;

// �ϱ���8·����
typedef struct
{
    HI_U16 noise[8];
}HI_DMID_TF_NOISE_REPORT_STRU;

// �ϱ�CCO����λ���NTB����
typedef struct
{
    HI_U8 phase_seq[3];
    HI_U8 pad;
    HI_U32 ntb[30];
}HI_DMID_TF_CCO_PHASE_CHECK_NTB_STRU;

// ��ӡÿ��վ����
typedef struct
{
    HI_U16 tei;
    HI_U8 mac[6];

    HI_U32 nid;

    HI_U8 identify_result;
    HI_U8 identify_state;
    HI_U8 belong_freq;
    HI_U8 pad;

    HI_U32 belong_nid;
}HI_DMID_TFP_IDENITFY_RESULT_STRU;

// ��ӡSTA��Χ15������ʶ����Ϣ
typedef struct
{
    HI_U32 nid;

    HI_BOOL is_in_use;
    HI_U8 nid_index;
    HI_U8 pad[2];

    HI_U16 cur_sync_ntb_cnt;
    HI_U16 sync_win_sum_cnt;

    HI_U32 sync_ntb_sum_cnt;

    HI_U32 cur_var;             // ���η���
    HI_U32 cur_var_ratio;       // ���η����ֵ

    HI_U32 var_ratio_sum;       // �ۼƷ����ֵ��

    HI_U32 var_ratio;

    HI_S32 sta_cco_ntb_diff;   // ÿ�����ڵ�STA_CCO_DIFF��ƽ��ֵ

}HI_DMID_TFP_NEIGHBOR_ENTRY_STRU;

#define HI_DMID_TFP_NEIGHBOR_NUM_MAX      3      // �����ϱ���������Ϣ����

typedef struct
{
    HI_U16 tei;
    HI_U8 phase;
    HI_U8 index;

    HI_U8 neighbor_num;
    HI_U8 pad[3];

    HI_DMID_TFP_NEIGHBOR_ENTRY_STRU neighbor_entry[HI_DMID_TFP_NEIGHBOR_NUM_MAX];
}HI_DMID_TFP_NEIGHBOR_INFO_STRU;


// ��ӡÿ��������ֹͣ��Ϣ
#define HI_DMID_TFP_DATA_NUM                  25

typedef struct
{
    HI_U8 mode;                       // ������ֹͣ
    HI_U8 pad[3];

    HI_U32 data[HI_DMID_TFP_DATA_NUM];
}HI_DMID_TFP_SET_CMD_MODE_STRU;


//ĳ��Ƶ�ε��������
typedef struct
{
    HI_U32 avg_success_rate     : 10;                   //ƽ����ͨ��,����:1/1000
    HI_U32 node_num             : 10;                   //���߽ڵ��������֧��1024��
    HI_U32 avg_delay            : 12;                   //ƽ����ʱ�����֧��4095ms
}HI_MAC_FA_FREQ_RESULT_STRU;

//����������������Ϣ����
typedef struct
{
    HI_U8  cur_evaluate_mode;                                   //��ǰ������ģʽ
    HI_U8  start_reason;                                        //��������Ƶ��������ԭ��
    HI_U8  init_freq;                                           //��������ǰ�Ĺ���Ƶ��
    HI_U8  auto_fail_cnt;                                       //�Լ�����������ָ��Ĵ���

    HI_U32 start_time_stamp;                                    //����ʱ�䣬��λ:ms
    HI_U32 toatl_run_time;                                      //����ģʽ��Ҫ��������ʱ�䣬��λ:s

    HI_U8  business_state;                                      //ҵ��״̬
    HI_U8  chl_state;                                           //ͨ����״̬
    HI_U8  pad[2];
}HI_DMID_FA_START_EVALUATE_S;

//ֹͣƵ����������Ϣ����
typedef struct
{
    HI_U8  cur_evaluate_mode;                                   //��ǰ������ģʽ
    HI_U8  start_reason;                                        //��������Ƶ��������ԭ��
    HI_U8  stop_reason;                                         //����ֹͣƵ��������ԭ��
    HI_U8  init_freq;                                           //��������ǰ�Ĺ���Ƶ��

    HI_U8  cur_freq;                                            //��ǰƵ��
    HI_U8  obj_freq;                                            //Ŀ��Ƶ��
    HI_BOOL is_next_evaluate_mode_full;
    HI_BOOL is_set_evaluate_mode_full;                          //�������ڼ䣬����ԭ�򴥷����������ı�ʶ

    HI_U32 stop_time_stamp;                                     //ֹͣʱ�䣬��λ:ms

    HI_U8  evaluate_state;                                      //ֹͣ����ǰ������״̬
    HI_U8  auto_fail_cnt;                                       //�Լ�����������ָ��Ĵ���
    HI_U8  freq_evaluated_cnt;                                  //��������Ƶ����
    HI_U8  affirm_network_state_cnt;                            //ȷ���������״̬�Ĵ���

    HI_U16 node_num;                                            //��CCO���ȫ��վ����
    HI_U16 ping_send_node_cnt;                                  //�����ѷ��Ͳ��Ա��ĵ�վ����

    HI_U16 ping_next_tei;                                       //��һ�β��Ե���ʼվ��TEI����
    HI_U8  ping_turn_cnt;                                       //ping�����Ե��ִμ���
    HI_U8  pad;

    HI_MAC_FA_FREQ_RESULT_STRU freq_result[FREQ_TRY_NUM];       //Ƶ�������Ľ��
}HI_DMID_FA_STOP_EVALUATE_S;

//Ƶ������״̬����Ϣ����
typedef struct
{
    HI_U8  evaluate_state;                                      //��ǰ����״̬
    HI_U8  cur_freq;                                            //��ǰƵ��
    HI_U8  level_num;                                           //����㼶��
    HI_U8  affirm_network_state_cnt;                            //����״̬��ȷ�ϴ���

    HI_U16 node_num;                                            //����ڵ�����������CCO
    HI_U16 proxy_num;                                           //����վ����

    HI_U32 handle_check_ping_timer_overtime_cnt;                //���Ա��ĳ�ʱ�Ĵ���
    HI_U32 tx_resend_cnt;                                       //���ͻ��ط���ndm���ĵ�����

    HI_U32 tx_send_fail_cnt;                                    // ���ͻ�����ʧ�ܵ�ndm���ĵ�����
    HI_U32 tx_send_timeout_cnt;                                 //���ͻ����ͳ�ʱ��ndm���ĵ�����

    HI_U16 level_node_num[15];                                  //���㼶��վ����
    HI_U16 phase_cnt[3];                                        //����λ��վ����

    HI_MAC_FA_FREQ_RESULT_STRU freq_result[FREQ_TRY_NUM];       //Ƶ�������Ľ��
}HI_DMID_FA_EVALUATE_STATE_S;

//**************************************************

#define HI_DMID_RT_OFFSET                           0x564
#define HI_DMID_RT_PATH_TABLE                       (HI_DMID_RT_OFFSET + 0x00)  // ·����Ϣ���ϱ�
#define HI_DMID_RT_MULTI_PATH                       (HI_DMID_RT_OFFSET + 0x01)  // ��·��ѡ�����ϱ�
#define HI_DMID_RT_EVALUATE_PROXY_RESULT            (HI_DMID_RT_OFFSET + 0x02)  // ������������ϱ�
#define HI_DMID_RT_BETTER_PROXY_RESULT              (HI_DMID_RT_OFFSET + 0x03)  // ���ڸ��ô������ϱ�
#define HI_DMID_RT_DOWN_PATH_CHANGE                 (HI_DMID_RT_OFFSET + 0x04)  // ����·���仯�ϱ�
#define HI_DMID_RT_LEAVE_NETWORK                    (HI_DMID_RT_OFFSET + 0x05)  // վ�������ϱ�
#define HI_DMID_RT_ROUTE_CYCLE                      (HI_DMID_RT_OFFSET + 0x06)  // ·��������Ϣ
typedef struct _rt_channel_stat_st3_
{
    HI_U32 channel_info;                        // �ŵ�����
    HI_U32 stat_cnt;                            // ���ո���
}rt_channel_stat_st3;

typedef struct _rt_proxy_comm_st3_
{
    HI_U32 comm_rate;
    HI_U32 channel_info;
    HI_U32 down_comm_rate;
}rt_proxy_comm_st3;
// ·����Ϣ���ϱ��ṹ
typedef struct
{
    HI_U16 tei;                                     // վ��TEI��ַ
    HI_U16 pad;
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];                 // վ��MAC��ַ
    HI_U8 role;                                     // վ���ɫ
    HI_U8 level;                                    // վ��㼶

    HI_U8 phase;                                    // վ����λ
    HI_U8 tm_capacity;                              // վ�㸺������
    HI_U8 up_channel_update_flag;
    HI_U8 pad1;

    rt_channel_stat_st3 last_down_channel_stat;      // �ϴ�����ͨ����ͳ��
    rt_channel_stat_st3 last_up_channel_stat;        // �ϴ�����ͨ����ͳ��
    rt_channel_stat_st3 cur_down_channel_stat;       // ��ǰ����ͨ����ͳ��
    HI_U8 comm_rate;                               // ������ͨ����
    HI_U8 down_comm_rate;                     //����ͨ�ųɹ���
    HI_U8 up_comm_rate;
    HI_U8 proxy_proxy_comm_rate;
    HI_U32 send_pk_cnt;                             // ���з������ݸ���

    HI_U32 send_fail_cnt;                           // ����ʧ�ܴ���
    HI_U32 send_success_cnt;                        // ���ͳɹ�����
    HI_U8  level_relation;                          // �㼶��ϵ
    HI_U8  rsvd;
    HI_U16 proxy_tei;                               // ������TEI
    rt_proxy_comm_st3 proxy_channel_stat;            // ����������ͨ������Ϣ

    HI_U32 rcv_beacon_cnt;                          // ���ո�վ��BEACON֡�ĸ���
    HI_U8  channel_info;                            // �ŵ�����(beacon֡����)
    HI_U8  par;                                     // �����(SOF֡)
    HI_S16 snr;                                     // ƽ�������(SOF֡)
}HI_DMID_RT_PATH_TABLE_ST;

// ��·��ѡ�����ϱ�
typedef struct
{
    HI_U32 bpc;
    HI_U16 proxy_path;
    HI_U16 up_up_level_path;
    HI_U16 up_leval_path;
    HI_U16 same_level_path;
}HI_DMID_RT_MULTI_PATH_RESULT_ST;

// ������������ϱ�
typedef struct
{
    HI_U32 bpc;
    HI_U16 proxy_candidate[5];
    HI_U16 proxy_cndd_cnt;
}HI_DMID_RT_EVALUATE_PROXY_RESULT_ST;

// ���ڸ��ô������ϱ�
typedef struct
{
    HI_U32 bpc;
    HI_U16 proxy_candidate[5];
    HI_U16 prxoy_cndd_cnt;
}HI_DMID_RT_BETTER_PROXY_RESULT_ST;

// ����·���仯�ϱ�
typedef struct
{
    HI_U16 odtei;
    HI_U16 rsvd;
    HI_U16 next_hop[4];
}HI_DMID_RT_DOWN_PATH_CHANGE_ST;

// վ��������Ϣ�ϱ�
typedef struct
{
    HI_U32 bpc;                         // վ������ʱ�����ű�����
    HI_U16 proxy_tei;                   // վ������ʱ������TEI
    HI_U16 channel_info;                // վ������ʱ�븸������ŵ�����
    HI_U32 comm_rate;                   // վ������ʱ�븸�����ͨ����
} HI_DMID_RT_LEAVE_NETWORK_ST;

typedef struct
{
    HI_U32 route_cycle_cnt;
}HI_DMID_RT_CYCLE_INFO_ST;

#define HI_DMID_NTB_OFFSET                          0x7D0
#define HI_DMID_BCN_OFFSET                          0x710
//�ŵ�����ģ��Ķ���
#define HI_DMID_CPA_OFFSET                          0x850
#define HI_DMID_CPA_CARRIER_SENSE_MODE_SWITCH       (HI_DMID_CPA_OFFSET + 0x00)

//�ز�����ģʽ�л�
typedef struct
{
    HI_U16 tail_timer_rx_success_rate;           //�ɹ��ʼ�������ڽ��ճɹ���
    HI_U16 recover_timer_rx_success_rate;        //�ز��������޻ָ���������ڽ��ճɹ���

    HI_U8  previous_mode;                        //�ز�������ǰһ�߳ɹ���ģʽ
    HI_U8  current_mode;                         //�ز������ĵ�ǰģʽ
    HI_U16 rsd;
}HI_DMID_CPA_CARRIER_SENSE_MODE_CHANGE_S;

//PHYģ��Ķ���
#define HI_DMID_PHY_OFFSET                          0x690
#define HI_DMID_PHY_AGC_ERR                         (HI_DMID_PHY_OFFSET + 0x00)
#define HI_DMID_PHY_NBI_ERR                         (HI_DMID_PHY_OFFSET + 0x01)
#define HI_DMID_PHY_IMP_ERR                         (HI_DMID_PHY_OFFSET + 0x02)
#define HI_DMID_PHY_CHANNEL_INFO_ERR                (HI_DMID_PHY_OFFSET + 0x03)
#define HI_DMID_PHY_CLOCK_PREAMBLE_ERR              (HI_DMID_PHY_OFFSET + 0x04)
#define HI_DMID_PHY_CLOCK_RX_END_ERR                (HI_DMID_PHY_OFFSET + 0x05)
#define HI_DMID_PHY_NTB_ERR                         (HI_DMID_PHY_OFFSET + 0x06)
#define HI_DMID_PHY_SYMBOL_ALIGN_ERR                (HI_DMID_PHY_OFFSET + 0x07)
#define HI_DMID_PHY_OVER_CURRENT                    (HI_DMID_PHY_OFFSET + 0x08)

#define HI_DMID_PHY_STATE_IND                       (HI_DMID_PHY_OFFSET + 0x09)
#define HI_DMID_PHY_SNAP1_IND                       (HI_DMID_PHY_OFFSET + 0x0a)
#define HI_DMID_PHY_SNAP2_IND                       (HI_DMID_PHY_OFFSET + 0x0b)
#define HI_DMID_PHY_SNAP3_IND                       (HI_DMID_PHY_OFFSET + 0x0c)
#define HI_DMID_PHY_SNAP4_IND                       (HI_DMID_PHY_OFFSET + 0x0d)
#define HI_DMID_PHY_SNAP5_IND                       (HI_DMID_PHY_OFFSET + 0x0e)
#define HI_DMID_PHY_SNAP6_IND                       (HI_DMID_PHY_OFFSET + 0x0f)
#define HI_DMID_PHY_SNAP7_IND                       (HI_DMID_PHY_OFFSET + 0x10)
#define HI_DMID_PHY_AGC_DCOC_IND                    (HI_DMID_PHY_OFFSET + 0x11)
#define HI_DMID_PHY_SYMBOL_ALIGN_IND                (HI_DMID_PHY_OFFSET + 0x12)
#define HI_DMID_PHY_CLOCK_IND                       (HI_DMID_PHY_OFFSET + 0x13)
#define HI_DMID_PHY_IMP_IND                         (HI_DMID_PHY_OFFSET + 0x14)
#define HI_DMID_PHY_CARRIER_SENSE_IND               (HI_DMID_PHY_OFFSET + 0x15)
#define HI_DMID_PHY_CARRIER_SENSE_PEAK_IND          (HI_DMID_PHY_OFFSET + 0x16)
#define HI_DMID_PHY_FC_IND                          (HI_DMID_PHY_OFFSET + 0x17)
#define HI_DMID_PHY_NTB_IND                         (HI_DMID_PHY_OFFSET + 0x18)
#define HI_DMID_PHY_TX_IND                          (HI_DMID_PHY_OFFSET + 0x19)
#define HI_DMID_PHY_NBI_NF_IND                      (HI_DMID_PHY_OFFSET + 0x1a)
#define HI_DMID_PHY_NBI_STATUS_IND                  (HI_DMID_PHY_OFFSET + 0x1b)
#define HI_DMID_PHY_PREAMBLE_SNR_IND                (HI_DMID_PHY_OFFSET + 0x1c)
#define HI_DMID_PHY_PREAM_STATE_IND                 (HI_DMID_PHY_OFFSET + 0x1d)
#define HI_DMID_PHY_IMP_FCPL_STAT_IND               (HI_DMID_PHY_OFFSET + 0x1e)
//AGC�쳣
typedef struct
{
    HI_U16 AdAvgPower;                  //ADƽ�������쳣
    HI_U16 SatRate;                     //������

    HI_U32 AfePower;                    //AFE���źŹ���
    HI_U16 PowerAfterFilter;            //�˲����źŵĹ���
    HI_U16 Rsd1;
}HI_DMID_PHY_AGC_ERR_S;

//NBI�쳣
typedef struct
{
    HI_U8  NbiCnt;                      //����NBI��
    HI_U8  NbiOutOfNf;                  //����NF��������NBI
    HI_U16 Rsd1;                        //����

    HI_U32 rpt_nbi0_pow : 27;           // Define the struct C_rpt_nbi0_status0
    HI_U32 reserved_0 : 4;
    HI_U32 rpt_nbi0_en : 1;

    HI_U32 rpt_nbi0_end : 9;            // Define the struct C_rpt_nbi0_status1
    HI_U32 reserved_1 : 7;
    HI_U32 rpt_nbi0_start : 9;
    HI_U32 reserved_2 : 7;

    HI_U32 rpt_nbi1_pow : 27;           // Define the struct C_rpt_nbi1_status0
    HI_U32 reserved_3 : 4;
    HI_U32 rpt_nbi1_en : 1;

    HI_U32 rpt_nbi1_end : 9;            // Define the struct C_rpt_nbi1_status1
    HI_U32 reserved_4 : 7;
    HI_U32 rpt_nbi1_start : 9;
    HI_U32 reserved_5 : 7;

    HI_U32 rpt_nbi2_pow : 27;           // Define the struct C_rpt_nbi2_status0
    HI_U32 reserved_6 : 4;
    HI_U32 rpt_nbi2_en : 1;

    HI_U32 rpt_nbi2_end : 9;            // Define the struct C_rpt_nbi2_status1
    HI_U32 reserved_7 : 7;
    HI_U32 rpt_nbi2_start : 9;
    HI_U32 reserved_8 : 7;

    HI_U32 rpt_nbi3_pow : 27;           // Define the struct C_rpt_nbi3_status0
    HI_U32 reserved_9 : 4;
    HI_U32 rpt_nbi3_en : 1;

    HI_U32 rpt_nbi3_end : 9;            // Define the struct C_rpt_nbi3_status1
    HI_U32 reserved_10 : 7;
    HI_U32 rpt_nbi3_start : 9;
    HI_U32 reserved_11 : 7;

    HI_U32 rpt_nbi4_pow : 27;           // Define the struct C_rpt_nbi4_status0
    HI_U32 reserved_12 : 4;
    HI_U32 rpt_nbi4_en : 1;

    HI_U32 rpt_nbi4_end : 9;            // Define the struct C_rpt_nbi4_status1
    HI_U32 reserved_13 : 7;
    HI_U32 rpt_nbi4_start : 9;
    HI_U32 reserved1 : 7;

    HI_U32 rpt_nbi5_pow : 27;           // Define the struct C_rpt_nbi5_status0
    HI_U32 reserved_14 : 4;
    HI_U32 rpt_nbi5_en : 1;

    HI_U32 rpt_nbi5_end : 9;            // Define the struct C_rpt_nbi5_status1
    HI_U32 reserved_15 : 7;
    HI_U32 rpt_nbi5_start : 9;
    HI_U32 reserved_16 : 7;

    HI_U32 rpt_nbi6_pow : 27;           // Define the struct C_rpt_nbi6_status0
    HI_U32 reserved_17 : 4;
    HI_U32 rpt_nbi6_en : 1;

    HI_U32 rpt_nbi6_end : 9;            // Define the struct C_rpt_nbi6_status1
    HI_U32 reserved_18 : 7;
    HI_U32 rpt_nbi6_start : 9;
    HI_U32 reserved_19 : 7;
}HI_DMID_PHY_NBI_ERR_S;

//IMP�쳣
typedef struct
{
    HI_U16 ImpCnt;                      //ÿ��ͳ������StatiPeriod�ϱ�
    HI_U16 Rsd1;                        //����
}HI_DMID_PHY_IMP_ERR_S;

//����ͬ���쳣
typedef struct
{
    HI_U16 FirstToMaxPathLen;           //�׾�����󾶵ľ���
    HI_U16 PataLen;                     //������
}HI_DMID_PHY_SYMBOL_ALIGN_ERR_S;

//�ŵ������쳣
typedef struct
{
    HI_U8  Par;                         //�����
    HI_U8  Rsd1;                        //����
    HI_S16 AvgSnr;                      //ƽ��snr
}HI_DMID_PHY_CHANNEL_INFO_ERR_S;

//Clock�쳣��������ж��ϱ�
typedef struct
{
    HI_S16 PilotFreqErr;                //PilotƵƫ
    HI_S16 SoundFreqErr;                //Sounding��Ƶƫ

    HI_U16 PilotCarrierNum;             //Pilot�ز���
    HI_U16 Rsd1;                        //����
}HI_DMID_PHY_CLOCK_RX_END_ERR_S;

//Clock�쳣Preamble�ϱ�
typedef struct
{
    HI_S16 PreambleFreqErr;             //PreambleƵƫ
    HI_S16 PllFreqErr;                  //Pll��Ƶƫ

    HI_U16 PreambleCarrierNum;          //Preamble�ز�����
    HI_U8  CntSingu;                    //clk_estģ��������쳣����
    HI_U8  Rsd1;                        //����
}HI_DMID_PHY_CLOCK_PREAMBLE_ERR_S;

//NTB�쳣
typedef struct
{
    HI_S16 BtsLtmrFreqErr;              //����������BTS��LTmr�����õ�ƫ��
    HI_S16 DeltaFreqErr;                //NTB��ǰ������Ƶƫ��

    HI_U8  CntSingu4NTB;                //��ǰ״̬�µ��쳣ֵ����
    HI_U8  Rsd1;
    HI_U16 Rsd2;
}HI_DMID_PHY_NTB_ERR_S;
//�����ش�����������
typedef struct
{
    HI_S8  AfePower;            //AFE�ķ��书��
    HI_S8  TxPower;             //PHY�ķ��书��
    HI_U8  TxState;             //����״̬
    HI_U8  Rsd;                 //����

    HI_U32 RescentCnt;          //�ش�����
}HI_DMID_PHY_OVER_CURRENT_S;


//RXģ��Ķ���
#define HI_DMID_MAC_RX_OFFSET                   0x790
#define HI_DMID_MAC_FREE_MPDU_BUFFER            (HI_DMID_MAC_RX_OFFSET + 0x00)       //���ջ��ͷ�MAC_BUFFER
#define HI_DMID_MAC_GET_MPDU_BUFFER             (HI_DMID_MAC_RX_OFFSET + 0x01)       //���ջ���ȡMAC BUFFER
#define HI_DMID_MAC_RECV_END                    (HI_DMID_MAC_RX_OFFSET + 0x02)       //�������֪ͨ������
#define HI_DMID_MAC_SEND_SACK                   (HI_DMID_MAC_RX_OFFSET + 0x03)       //����SACK֡
#define HI_DMID_MAC_SEND_CTS                    (HI_DMID_MAC_RX_OFFSET + 0x04)       //����CTS֡
#define HI_DMID_MAC_SEND_SOUNDING_SACK          (HI_DMID_MAC_RX_OFFSET + 0x05)       //����SOUNDING SACK֡
#define HI_DMID_MAC_SET_COLISION_FLAG           (HI_DMID_MAC_RX_OFFSET + 0x06)       //���ó�ͻ��־
#define HI_DMID_MAC_RCV_SOUNDING_SACK           (HI_DMID_MAC_RX_OFFSET + 0x07)       //�յ�SOUNDING SACK֡
#define HI_DMID_MAC_SEND_SUCCESS                (HI_DMID_MAC_RX_OFFSET + 0x08)       //֪ͨ���ͻ����ͳɹ�
#define HI_DMID_MAC_VCS_CHANGE                  (HI_DMID_MAC_RX_OFFSET + 0x09)       //֪ͨ���ͻ���·�仯
#define HI_DMID_MAC_GENERATE_SOUNDING_SACK      (HI_DMID_MAC_RX_OFFSET + 0x0a)       //����SOUNDING SACK֡
#define HI_DMID_MAC_HANDLE_SOUNDING             (HI_DMID_MAC_RX_OFFSET + 0x0b)       //����SOUNDING���
#define HI_DMID_MAC_HANDLE_BEACON               (HI_DMID_MAC_RX_OFFSET + 0x0c)       //����BEACON֡
#define HI_DMID_MAC_SOFT_NTB_CALC               (HI_DMID_MAC_RX_OFFSET + 0x0d)       //�������NTB�Ľ��
#define HI_DMID_MAC_RCV_FC_OK                   (HI_DMID_MAC_RX_OFFSET + 0x0e)       //���յ�FC��ȷ
#define HI_DMID_MAC_RCV_FC_ERR                  (HI_DMID_MAC_RX_OFFSET + 0x0f)       //���յ�FC����

#define HI_DMID_MAC_RX_BEACON_INFO              (HI_DMID_MAC_RX_OFFSET + 0x10)
#define HI_DMID_MAC_RX_SOF_INFO                 (HI_DMID_MAC_RX_OFFSET + 0x11)
#define HI_DMID_MAC_RX_SACK_INFO                (HI_DMID_MAC_RX_OFFSET + 0x12)
#define HI_DMID_MAC_RX_COORDIANTE_INFO          (HI_DMID_MAC_RX_OFFSET + 0x13)
#define HI_DMID_MAC_RX_ERR_INFO                 (HI_DMID_MAC_RX_OFFSET + 0x14)


//����FC�����ϱ�
typedef struct
{
    HI_U32 ntb;
    HI_U8  freq;
    HI_U8  pad[3];

    HI_U32 fc[4];
}HI_DMID_MAC_RCV_FC_OK_S;

typedef struct
{
    HI_U32 ntb;
}HI_DMID_MAC_RCV_FC_ERR_S;

//���ջ��ͷ�MAC_BUFFER
typedef struct
{
    HI_U32 bpc;       //�ű�����
    HI_U32 ntb;       //�ͷ�ʱ��
    HI_U32 sof_addr;  //�ͷŵ�ַ
}HI_DMID_MAC_FREE_MPDU_BUFFER_S;

//���ջ���ȡMAC BUFFER
typedef struct
{
    HI_U32 bpc;                 //�ű�����
    HI_U32 ntb;                 //�ͷ�ʱ��
    HI_U32 sof_addr;            //�ͷŵ�ַ

    HI_U16 stei;              //���ݷ���վ��TEI
    HI_U8  lid;               //���ݱ������ȼ�
    HI_U8  resend_packet;     //�Ƿ�Ϊ�ش�����
}HI_DMID_MAC_GET_MPDU_BUFFER_S;

//�������֪ͨ������
typedef struct
{
    HI_U16 stei;              //���ݷ���վ��TEI
    HI_U8  lid;               //���ݱ������ȼ�
    HI_U8  resend_packet;     //�Ƿ�Ϊ�ش�����

    HI_U32 sof_addr;          //��ȡ��MAC BUFFER��ַ
    HI_U32 ntb;               //�ͷ�ʱ��

    HI_U16 packet_index;
    HI_U16 source_type;

    HI_U16 ctei;
    HI_U16 ostei;

    HI_U16 odtei;
    HI_U16 msdu_size;

    HI_U8  broadcast_flag;
    HI_U8  hop_cnt;
    HI_U8  pad[2];
}HI_DMID_MAC_RECV_END_S;

//����SACK֡
typedef struct
{
    HI_U16 dtei;          //Ŀ��վ��TEI
    HI_U8  sack_result;   //SACK������Я��PB����
    HI_U8  rsvd;
}HI_DMID_MAC_SEND_SACK_S;

typedef struct
{
    HI_U16 dtei;     //Ŀ��վ��TEI
    HI_U16 dur;      //RTC/CTS����ʱ��
}HI_DMID_MAC_SEND_CTS_S;

//����SOUNDING SACK֡
typedef struct
{
    HI_U16 dtei;     //Ŀ��վ��TEI
    HI_U16 rsvd;
}HI_DMID_MAC_SEND_SOUNDING_SACK_S;

typedef struct
{
    HI_U32 bpc;     //�ű�����
    HI_U32 ntb;     //��ͻʱ��
}HI_DMID_MAC_SET_COLISION_FLAG_S;

typedef struct
{
    HI_U32 bpc;     //�ű�����
    HI_U32 ntb;     //����ʱ��
    HI_U16 stei;    //Դ��ַ
    HI_U16 rsvd;
}HI_DMID_MAC_RCV_SOUNDING_SACK_S;

typedef struct
{
    HI_U32 bpc;     //�ű�����
    HI_U32 ntb;     //��ͻʱ��
}HI_DMID_MAC_SEND_SUCCESS_S;

typedef struct
{
    HI_U32 bpc;     //�ű�����
    HI_U32 ntb;     //��ͻʱ��
}HI_DMID_MAC_VCS_CHANGE_S;


typedef struct
{
    HI_U32 bpc;     //�ű�����
    HI_U32 ntb;     //��ͻʱ��
}HI_DMID_MAC_GENERATE_SOUNDING_SACK_S;

typedef struct
{
    HI_U32 bpc;     //�ű�����
    HI_U32 ntb;     //��ͻʱ��

    HI_U16 stei;
    HI_U8  tmi;
    HI_U8  rsvd;
}HI_DMID_MAC_HANDLE_SOUNDING_S;

typedef struct
{
    HI_U32 bpc;     //�ű�����
    HI_U32 ntb;     //��ͻʱ��
    HI_U16 stei;    //BEACON֡��Դ��ַ
}HI_DMID_MAC_HANDLE_BEACON_S;

typedef struct
{
    HI_U32 current_bts;     // ��ǰ����ͬ��֡������ʱ�ӻ�ֵ
    HI_U32 current_ltmr;    // ��ǰ����ͬ��֡�ı���ʱ�Ӽ���

    HI_U32 front_bts;       // �ϴ�ͬ��֡�������׼ʱ��ֵ
    HI_U32 front_ltmr;      // �ϴ�ͬ��֡�ı���ʱ�Ӽ���

    HI_U32 cur_ntb;         // ��ӡʱ��NTB

    HI_S16 clk_freqerr;     // ��ǰʹ�õ�Ƶƫ
    HI_S16 calc_freqerr;    // ��ǰ�������Ƶƫ(�����˲����Ƶƫֵ)
    HI_S16 cur_freqerr;     // ���ε�Ƶƫ
    HI_U16 pad1;

    HI_U8  state;           // ͬ��״̬
    HI_U8  synch_snid;      // ͬ��վ��������
    HI_U16 synch_tei;       // ͬ��վ����ն��豸��ʶ

    HI_U8  synch_mac[6];    // ͬ�������MAC��ַ
    HI_U16 pad2;
}DIAG_CMD_NTB_SOFT_CALC_REPORT_STRU;

typedef struct
{
    HI_U16 ostei;
    HI_U16 proxy_tei;
    HI_U32 bpc;
    HI_U16 send_discovery_pk_cnt;
    HI_U16 rsvd;
    HI_U32 to_proxy_comm_rate;
    HI_U32 to_proxy_channel_info;
    HI_U32 to_proxy_down_comm_rate;
    HI_BOOL  proxy_changeing;
    HI_BOOL  calc_proxy_rate_ok;
    HI_U16    new_proxy_tei;
}HI_DMID_MAC_RT_DISCOVER_LIST_HEAD_S;

typedef struct
{
    HI_U16 ostei;
    HI_U16 rsvd;
    HI_U16 next_hop[4];
    HI_U16 route_type[4];
}HI_DMID_MAC_RT_DISCOVER_LIST_ROUTE_S;

typedef struct
{
    HI_U16   ostei;
    HI_U16   discover_tei;
    HI_U16   last_down_stat_cnt_tmp;
    HI_U8     comm_rate;
    HI_U8     calc_proxy_rate_ok;
    HI_U8     to_cco_near;
    HI_U8     rsvd[3];
}HI_DMID_MAC_RT_DISCOVER_LIST_ITEM_S;

//���������Ϣ����
#define NM_MESSAGE_HEAD_OFFSET  0x500
#define HI_DMID_MAC_STA_JOIN_FORM_NUM_CHANGE                       (NM_MESSAGE_HEAD_OFFSET)
#define HI_DMID_MAC_CCO_RCV_STA_ASSOC_REQ_STA                      (NM_MESSAGE_HEAD_OFFSET + 0x01)
#define HI_DMID_MAC_STA_CHOSE_PROXY_RESULT                         (NM_MESSAGE_HEAD_OFFSET + 0x02)
#define HI_DMID_MAC_STA_JOIN_ITS_CHILD                             (NM_MESSAGE_HEAD_OFFSET + 0x03)
#define HI_DMID_MAC_SET_WORK_FREQ                                  (NM_MESSAGE_HEAD_OFFSET + 0x04)     //���ù���Ƶ��
#define HI_DMID_MAC_CREATE_NETWORK_MODULE_MSDU                     (NM_MESSAGE_HEAD_OFFSET + 0x05)
#define HI_DMID_MAC_RCV_NETWORK_MODULE_MSDU                        (NM_MESSAGE_HEAD_OFFSET + 0x06)
#define HI_DMID_MAC_STA_JOIN_OK                                    (NM_MESSAGE_HEAD_OFFSET + 0x07)
#define HI_DMID_MAC_STA_JOIN_FAIL                                  (NM_MESSAGE_HEAD_OFFSET + 0x08)
#define HI_DMID_MAC_CCO_SOME_STA_JOIN_OK                           (NM_MESSAGE_HEAD_OFFSET + 0x09)
#define HI_DMID_MAC_CCO_SOME_STA_JOIN_FAIL                         (NM_MESSAGE_HEAD_OFFSET + 0x0A)
#define HI_DMID_MAC_STA_START_CHANGE_PROXY                         (NM_MESSAGE_HEAD_OFFSET + 0x0B)
#define HI_DMID_MAC_NEW_CHANGE_PROXY_ITEM_DFX                      (NM_MESSAGE_HEAD_OFFSET + 0x0C)
#define HI_DMID_MAC_STA_HANDLE_CHANGE_PROXY_CNF                    (NM_MESSAGE_HEAD_OFFSET + 0x0D)
#define HI_DMID_MAC_CCO_DELETE_STA_INFO                            (NM_MESSAGE_HEAD_OFFSET + 0x0E)
#define HI_DMID_MAC_CCO_REPORT_VIOLENCE_STA                        (NM_MESSAGE_HEAD_OFFSET + 0x0F)
#define HI_DMID_MAC_CCO_HEART_BEAT_CHECK_WARNING                   (NM_MESSAGE_HEAD_OFFSET + 0x10)
#define HI_DMID_MAC_STA_LEAVE_NETWORK                              (NM_MESSAGE_HEAD_OFFSET + 0x11)
#define HI_DMID_MAC_CCO_JUDGE_STA_LEAVE_NETWORK                    (NM_MESSAGE_HEAD_OFFSET + 0x12)
#define HI_DMID_MAC_CCO_START_NETWORK_FORM                         (NM_MESSAGE_HEAD_OFFSET + 0x13)
#define HI_DMID_MAC_CCO_START_SOME_LEVEL_JOIN                      (NM_MESSAGE_HEAD_OFFSET + 0x14)
#define HI_DMID_MAC_CCO_LAST_LEVEL_JOIN_COUNT                      (NM_MESSAGE_HEAD_OFFSET + 0x15)
#define HI_DMID_MAC_CCO_NETWORK_FORM_OK_INFO                       (NM_MESSAGE_HEAD_OFFSET + 0x16)

//#define HI_DMID_MAC_STA_FREQ_MODE_PRESENT                          NM_MESSAGE_HEAD_OFFSET+0x17
//#define HI_DMID_MAC_STA_RCV_BEACON_SOF_AT_PRESENT_FREQ_MODE        NM_MESSAGE_HEAD_OFFSET+0x18
#define HI_DMID_MAC_STA_RCV_CHANGE_FREQ_MODE_NOTIFY                (NM_MESSAGE_HEAD_OFFSET + 0x19)
#define HI_DMID_MAC_CCO_RCV_STA_ASSOC_REQ_PROXY                    (NM_MESSAGE_HEAD_OFFSET + 0x1A)
#define HI_DMID_MAC_CCO_RCV_STA_ASSOC_OBJECT_REQ_STA               (NM_MESSAGE_HEAD_OFFSET + 0x1B)
#define HI_DMID_MAC_SET_SNID                                       (NM_MESSAGE_HEAD_OFFSET + 0x1C)     //����SNID

#define HI_DMID_MAC_RT_DISCOVER_LIST_HEAD                          (NM_MESSAGE_HEAD_OFFSET + 0x20)
#define HI_DMID_MAC_RT_DISCOVER_LIST_ROUTE                         (NM_MESSAGE_HEAD_OFFSET + 0x21)
#define HI_DMID_MAC_RT_DISCOVER_LIST_ITEM                          (NM_MESSAGE_HEAD_OFFSET + 0x22)
#define HI_DMID_MAC_NEW_CHANGE_PROXY_DFX                           (NM_MESSAGE_HEAD_OFFSET + 0x23)
#define HI_DMID_MAC_TDMA_ALLOC_INFO                                (NM_MESSAGE_HEAD_OFFSET + 0x24)
#define HI_DMID_MAC_NDC_ENTRY_INFO                                 (NM_MESSAGE_HEAD_OFFSET + 0x25)
#define HI_DMID_MAC_BEACON_PAYLOAD_INFO                            (NM_MESSAGE_HEAD_OFFSET + 0x26)
#define HI_DMID_MAC_RX_REPORT_INFO                                 (NM_MESSAGE_HEAD_OFFSET + 0x28)
#define HI_DMID_MAC_TX_REPORT_INFO                                 (NM_MESSAGE_HEAD_OFFSET + 0x29)
#define HI_DMID_MAC_SLOT_INFO                                      (NM_MESSAGE_HEAD_OFFSET + 0x2A)
#define HI_DMID_MAC_HEARTBEAT_INFO                                 (NM_MESSAGE_HEAD_OFFSET + 0x2B)
#define HI_DMID_MAC_PLC_SWITCH                                     (NM_MESSAGE_HEAD_OFFSET + 0x2C)
#define HI_DMID_MAC_RX_INFO                                        (NM_MESSAGE_HEAD_OFFSET + 0x2D)
#define HI_DMID_MAC_DFX_INFO                                       (NM_MESSAGE_HEAD_OFFSET + 0x2E)
#define HI_DMID_MAC_PTPC_INFO                                      (NM_MESSAGE_HEAD_OFFSET + 0x2F)
#define HI_DMID_MAC_PTPC_STATE_CHANGE                              (NM_MESSAGE_HEAD_OFFSET + 0x30)
#define HI_DMID_MAC_PTPC_APP_PKT_FROM_PHY                          (NM_MESSAGE_HEAD_OFFSET + 0x31)
#define HI_DMID_MAC_PTPC_APP_PKT_FROM_APP                          (NM_MESSAGE_HEAD_OFFSET + 0x32)
#define HI_DMID_MAC_PTPC_SEND_SEARCH_FRAME                         (NM_MESSAGE_HEAD_OFFSET + 0x33)
#define HI_DMID_MAC_PTPC_RCV_SEARCH_FRAME                          (NM_MESSAGE_HEAD_OFFSET + 0x34)
#define HI_DMID_MAC_PTPC_SEND_SYNC_FRAME                           (NM_MESSAGE_HEAD_OFFSET + 0x35)
#define HI_DMID_MAC_SND_INFO                                       (NM_MESSAGE_HEAD_OFFSET + 0x36)
#define HI_DMID_MAC_SC_RCV_INFO                                    (NM_MESSAGE_HEAD_OFFSET + 0x37)
#define HI_DMID_MAC_SC_SND_INFO                                    (NM_MESSAGE_HEAD_OFFSET + 0x38)
#define HI_DMID_MAC_NETWORK_CONFLICT_REPORT                        (NM_MESSAGE_HEAD_OFFSET + 0x40)
#define HI_DMID_MAC_NETWORK_CONFLICT_CHOOSE_SNID                   (NM_MESSAGE_HEAD_OFFSET + 0x41)
#define HI_DMID_MAC_QUERY_NEIGHBOR_NETWORK_INFO                    (NM_MESSAGE_HEAD_OFFSET + 0x42)

#define HI_DMID_MAC_STA_CANDIDATE_PROXY                            (NM_MESSAGE_HEAD_OFFSET + 0x43)
#define HI_DMID_MAC_STA_PATH_NODE                                  (NM_MESSAGE_HEAD_OFFSET + 0x44)
#define HI_DMID_MAC_STA_MMIB_INFO                                  (NM_MESSAGE_HEAD_OFFSET + 0x45)
#define HI_DMID_MAC_STA_ATTENUATION_INFO                           (NM_MESSAGE_HEAD_OFFSET + 0x46)

#define HI_DMID_MAC_SC_SWITCH_STATUS                               (NM_MESSAGE_HEAD_OFFSET + 0x47)
#define HI_DMID_MAC_SC_NM_TDMA                                     (NM_MESSAGE_HEAD_OFFSET + 0x48)

#define HI_DMID_MAC_SC_PASSIVE_ASSOC                               (NM_MESSAGE_HEAD_OFFSET + 0x49)
#define HI_DMID_MAC_SC_DIRECTIONAL_ASSOC                           (NM_MESSAGE_HEAD_OFFSET + 0x4A)
#define HI_DMID_MAC_SC_ACTIVE_ASSOC		                           (NM_MESSAGE_HEAD_OFFSET + 0x4B)
#define HI_DMID_MAC_SC_NETWORK_SEARCH		                       (NM_MESSAGE_HEAD_OFFSET + 0x4C)
#define HI_DMID_MAC_SC_STATION_SEARCH		                       (NM_MESSAGE_HEAD_OFFSET + 0x4D)
#define HI_DMID_MAC_SC_STATION_DETECT		                       (NM_MESSAGE_HEAD_OFFSET + 0x4E)
#define HI_DMID_MAC_SC_ASSOCING				                       (NM_MESSAGE_HEAD_OFFSET + 0x4F)

#define HI_DMID_MAC_MF_NETWORK_DETECT							   (NM_MESSAGE_HEAD_OFFSET + 0x50)
#define HI_DMID_MAC_MF_NETWORK_DETECT_HANDLE					   (NM_MESSAGE_HEAD_OFFSET + 0x51)
#define HI_DMID_MAC_MF_NETWORK_DETECT_RESPONSE					   (NM_MESSAGE_HEAD_OFFSET + 0x52)
#define HI_DMID_MAC_MF_NETWORK_DETECT_RESPONSE_HANDLE			   (NM_MESSAGE_HEAD_OFFSET + 0x53)
#define HI_DMID_MAC_MF_STATION_DETECT							   (NM_MESSAGE_HEAD_OFFSET + 0x54)
#define HI_DMID_MAC_MF_STATION_DETECT_HANDLE					   (NM_MESSAGE_HEAD_OFFSET + 0x55)
#define HI_DMID_MAC_MF_STATION_DETECT_RESPONSE					   (NM_MESSAGE_HEAD_OFFSET + 0x56)

#define HI_DMID_MAC_MF_FREQ_SWITCH                                 (NM_MESSAGE_HEAD_OFFSET + 0x58)
#define HI_DMID_MAC_MF_FREQ_SWITCH_HANDLE						   (NM_MESSAGE_HEAD_OFFSET + 0x59)
#define HI_DMID_MAC_MF_FREQ_SWITCH_GATHERED                        (NM_MESSAGE_HEAD_OFFSET + 0x5E)
#define HI_DMID_MAC_MF_FREQ_SWITCH_GATHERED_HANDLE				   (NM_MESSAGE_HEAD_OFFSET + 0x5F)
#define HI_DMID_MAC_MF_FREQ_SWITCH_CONFIRM						   (NM_MESSAGE_HEAD_OFFSET + 0x60)
#define HI_DMID_MAC_MF_FREQ_SWITCH_CONFIRM_HANDLE				   (NM_MESSAGE_HEAD_OFFSET + 0x61)
#define HI_DMID_MAC_STA_REPORT_NETWORK_MODE                        (NM_MESSAGE_HEAD_OFFSET + 0x5A)
#define HI_DMID_MAC_STA_REPORT_ACCESS_NETWORK                      (NM_MESSAGE_HEAD_OFFSET + 0x5B)
#define HI_DMID_MAC_STA_REPORT_ASSOC_PROXY                         (NM_MESSAGE_HEAD_OFFSET + 0x5C)
#define HI_DMID_MAC_STA_REPORT_FREQ_BLIND_CHECK                    (NM_MESSAGE_HEAD_OFFSET + 0x5D)
#define HI_DMID_MAC_FAST_NOTICE_INFO                               (NM_MESSAGE_HEAD_OFFSET + 0x5E)  

//*****************************************************************************

// �����ģ����Ϣ����  [0x750~0x790)
#define HI_DMID_MAC_SEND_MSDU_TO_APP  (0x750)
#define HI_DMID_MAC_SEND_MSDU_TO_IP   (0x751)
#define HI_DMID_MAC_RCV_MSDU_FROM_APP (0x752)
#define HI_DMID_MAC_RCV_MSDU_FROM_IP  (0x753)

typedef struct
{
    HI_U32 bpc;
    HI_U32 ntb;
    HI_U32 msdu_size;
}HI_DMID_MAC_RCV_MSDU_FROM_APP_S;

typedef struct
{
    HI_U32 bpc;
    HI_U32 ntb;

    HI_U8  oda[6];
    HI_U8  osa[6];
    HI_U8  dst_ip[4];
    HI_U8  src_ip[4];

    HI_U16 stei;
    HI_U16 ostei;

    HI_U16 odtei;
    HI_U16 ctei;

    HI_U16 pad1;
    HI_U16 pad;

    HI_U16 packet_index;
    HI_U16 msdu_size;
}HI_DMID_MAC_RCV_MSDU_FROM_IP_S;


//������ģ����Ϣ�ṹ�嶨��
#define HI_DMID_MAC_RSB_MPDU_INFO                (0x6D0)

//�ֶ���ģ����Ϣ�ṹ�嶨��
#define HI_DMID_MAC_SEG_FRAME_INFO_IN_QUEUE                (0x62c)

typedef struct
{
    HI_U32     ats;
    HI_U32     timeout;

    HI_U16     stei;
    HI_U16     ptcl_ver;

    HI_U16     ctei;
    HI_U16     ostei;

    HI_U16     odtei;
    HI_U8      lid;
    HI_U8      pad1;

    HI_U8      broadcast_flag;
    HI_U8      hop_cnt;
    HI_U16     msdu_size;

    HI_U8      pb_count;
    HI_U8      eks;
    HI_U16     pb_size;

    HI_U16     rf_mpdu_sequence;
    HI_U16     source_type;

    HI_U32     snid;

    HI_U8      pad;
    HI_U8      localbroadcast_max_send_time;
    HI_U16     packet_index;
}HI_DMID_MAC_SEG_FRAME_INFO_IN_QUEUE_S;

//���ͻ�ģ����Ϣ����
#define TX_MESSAGE_HEAD_OFFSET                      0x5c8
#define HI_DMID_MAC_TX_MPDU_FLAG_SET                TX_MESSAGE_HEAD_OFFSET + 0x01
#define HI_DMID_MAC_TX_BROADCAST_PHASE_NEXTHOP      TX_MESSAGE_HEAD_OFFSET + 0x02
#define HI_DMID_MAC_TX_UNICAST_NEXTHOP              TX_MESSAGE_HEAD_OFFSET + 0x03
#define HI_DMID_MAC_TX_NEXTHOP_PHASE_INFO           TX_MESSAGE_HEAD_OFFSET + 0x04
#define HI_DMID_MAC_TX_MSDU_MAC_BUFFER_INFO         TX_MESSAGE_HEAD_OFFSET + 0x05
#define HI_DMID_MAC_TX_ROUTE_FEEDBACK_INFO          TX_MESSAGE_HEAD_OFFSET + 0x06

#define HI_DMID_MAC_SND_BEACON_INFO                 (TX_MESSAGE_HEAD_OFFSET + 0x07)
#define HI_DMID_MAC_SND_SOF_INFO                    (TX_MESSAGE_HEAD_OFFSET + 0x08)
#define HI_DMID_MAC_SND_SACK_INFO                   (TX_MESSAGE_HEAD_OFFSET + 0x09)
#define HI_DMID_MAC_SND_COORDIANTE_INFO             (TX_MESSAGE_HEAD_OFFSET + 0x0A)


//*****************************************************************************
// �ṹ����
//*****************************************************************************
// TODO
//*****************************************************************************
//���������Ϣ�ṹ�嶨��
typedef struct
{
    HI_U16 tei;
    HI_U16 proxy;
}HI_DMID_MAC_STA_JOIN_ITS_CHILD_S;

typedef struct
{
    HI_U8 new_form_serial_number;
    HI_U8 old_form_ferial_number;
    HI_U16 rsvd;
}HI_DMID_MAC_STA_JOIN_FORM_NUM_CHANGE_S;

typedef struct
{
    HI_U8 mac[6];
    HI_U8 send_success_ratio;
    HI_U8 mac_rejoin_reason;
    HI_U16 tx_resend_count;//tx
    HI_U16 assoc_req_count;
    HI_U16 rcv_beacon_count;
    HI_U16 rcv_beacon_count_of_proxy[DFX_PROXY_SUM];
    HI_U32 end_sequence;
}HI_DMID_MAC_CCO_RCV_STA_ASSOC_REQ_STA_S;

typedef struct
{
    HI_U8 mac[6];
    HI_U16 pad;
    DFX_PROXY_ASSOC_INFO proxy_assoc_info;
}HI_DMID_MAC_CCO_RCV_STA_ASSOC_REQ_PROXY_S;

typedef struct
{
    HI_U8 mac[6];
    HI_U16 pad;
    DFX_ASSOC_OBJECT_STRUCT assoc_object;
}HI_DMID_MAC_CCO_RCV_STA_ASSOC_OBJECT_REQ_STA_S;


typedef struct
{
    HI_U16 old_proxy;
    HI_U16 chose_proxy_tei[5];
}HI_DMID_MAC_STA_CHOSE_PROXY_RESULT_S;


typedef struct
{
    HI_U8 osa[6];
    HI_U8 oda[6];

    HI_U16 mmtype;
    HI_U16 ostei;

    HI_U16 odtei;
    HI_U8  freq;
    HI_U8  pad1;

    HI_U16 ptcl_ver;
    HI_U16 pad2;

    HI_U32 snid;
    HI_U32 bpc;
}HI_DMID_MAC_CREATE_NETWORK_MODULE_MSDU_S;

typedef struct
{
    HI_U8 osa[6];
    HI_U8 oda[6];

    HI_U16 mmtype;
    HI_U16 ostei;

    HI_U16 odtei;
    HI_U16 ptcl_ver;

    HI_U8  mac_head_ver;
    HI_U8  freq;
    HI_U16 pad;

    HI_U32 snid;

    HI_U32 bpc;
}HI_DMID_MAC_RCV_NETWORK_MODULE_MSDU_S;

typedef struct
{
    HI_U8 my_level;
    HI_U8 my_ability;
    HI_U8 my_mac[6];

    HI_U8 my_snid;
    HI_BOOL my_phase_mode;
    HI_U8 my_phase;
    HI_U8 form_serial_number;

    HI_U16 my_tei;
    HI_U16 my_proxy_tei;
}HI_DMID_MAC_STA_JOIN_OK_S;

typedef struct
{
    HI_U32 tdma_state;              // ʱ϶״̬������״̬���ܼ������űꡢ�ܼ�����
    HI_U32 current_bpc;             // ��ǰBPC
    HI_U32 starttime;               // �ű����ڿ�ʼʱ��

    HI_U32 beacon_period_len;       // �ű����ڳ���
    HI_U32 proxy_beacon_cnt;        // �����ű����
    HI_U32 all_beacon_cnt;          // �ű����

    HI_U32 phase_a_cp;              // ��λA CSMAʱ϶����
    HI_U32 phase_b_cp;              // ��λB CSMAʱ϶����
    HI_U32 phase_c_cp;              // ��λC CSMAʱ϶����

    HI_U32 phase_a_bcp;             // ��λA ��CSMAʱ϶����
    HI_U32 phase_b_bcp;             // ��λB ��CSMAʱ϶����
    HI_U32 phase_c_bcp;             // ��λC ��CSMAʱ϶����
}HI_DMID_MAC_TDMA_ALLOC_INFO_S;

//���ù���Ƶ��
typedef struct
{
    HI_U16 reason;                  //����ԭ��
    HI_U8  freq;                    //���ú�Ĺ���Ƶ��
    HI_U8  pad;
}HI_DMID_MAC_SET_WROK_FREQ_S;

typedef struct
{
    HI_U32 snid;

    HI_U8  freq;
    HI_U8  pad;
    HI_U16 ptcl_ver;                //Э��汾��
}HI_DMID_MAC_SET_SNID_S;

typedef struct
{
    HI_U32 heartbeat_cycle_cnt;                     // �������ڼ���
    HI_U32 rcv_heartbeat_cnt;                         // �����������ĸ���
    HI_U16 offline_sta_num;                            // ����վ�����
    HI_U16 false_offline_sta_num;                  // ������վ�����
}HI_DMID_MAC_HEARTBEAT_INFO_S;

typedef struct
{
    HI_U32 bpc;
    HI_U32 starttime;                              // ���ʱ�� ��λ:1΢��
    HI_U32 duration;                               // ����ʱ�� ��λ:1΢��

    HI_U16 tei;                                    // ʱ϶��Ӧ��tei
    HI_U8  slot_type;                              // ʱ϶����
    HI_U8  phase;                                  // ʱ϶��λ(CSMAʱ϶����)
    HI_U8  lid;                                    // ���ȼ�
    HI_U8  slot_source;                     //ʱ϶��Դ
    HI_U8  pad[2];
}HI_DMID_MAC_SLOT_INFO_S;

typedef struct
{
    HI_U8 dev_ver;
    HI_U8 rsvd;
    HI_U8 dev_name[6];
    HI_U8 dev_ip[4];
}HI_DMID_MAC_NDC_ENTRY_INFO_S;

typedef struct
{
    HI_U16 stei;
    HI_U16 pb_size;

    HI_U8  bt;
    HI_U8  nbe;
    HI_U8  station_entry_len;
    HI_U8  para_entry_len;

    HI_U8  ndc_entry_len;
    HI_U8  freqerr_notify_len;
    HI_U16 tdma_entry_len;
}HI_DMID_MAC_BEACON_PAYLOAD_INFO_S;

typedef struct
{
    HI_U8 fail_reason;
    HI_U8 rsvd[3];
}HI_DMID_MAC_STA_JOIN_FAIL_S;

typedef struct
{
    HI_U8 level;
    HI_U8 ability;
    HI_U8 mac[6];

    HI_U16 tei;
    HI_U16 proxy_tei;

    HI_U16 ptcl_ver;
    HI_U16 chip_ver;

    HI_CHAR chip_code[2];           //оƬ����
    HI_U8  pad[2];
}HI_DMID_MAC_CCO_SOME_STA_JOIN_OK_S;

typedef struct
{
    HI_U8 fail_reason;
    HI_U8 mac[6];
    HI_U8 rsvd;

    HI_U16 ptcl_ver;
    HI_U16 chip_ver;

    HI_CHAR chip_code[2];           //оƬ����
    HI_U8  pad[2];
}HI_DMID_MAC_CCO_SOME_STA_JOIN_FAIL_S;

typedef struct
{
    HI_U16 old_proxy;
    HI_U16 chose_proxy_tei[5];
}HI_DMID_MAC_STA_START_CHANGE_PROXY_S;

typedef struct
{
    HI_BOOL result;
    HI_U8 rsvd[3];
}HI_DMID_MAC_STA_HANDLE_CHANGE_PROXY_CNF_S;

typedef struct
{
    HI_U8 mac[6];
    HI_BOOL is_relay_ability;
    HI_U8   product_type;

    HI_U32 proxy_change_cnt;                        // ����������
    HI_U32 leave_cnt;                               // ���ߴ���
}HI_DMID_MAC_CCO_DELETE_STA_INFO_S;

typedef struct
{
    HI_U8 mac[6];
    HI_U8 pad[2];
}HI_DMID_MAC_CCO_REPORT_VIOLENCE_STA_S;
#define DFX_RF_PATH_TABLE_MAX_NUM 12

typedef struct
{
	HI_U8 current_network_mode;
	HI_U8 next_network_mode;
	HI_U8 pad[2];
}HI_DMID_MAC_STA_REPORT_NETWORK_MODE_S;

typedef struct
{
	HI_U32 access_nid;
	HI_U8  access_freq;
	HI_S8  snr;
	HI_U16 pad;
}HI_DMID_MAC_STA_REPORT_ACCESS_NETWORK_S;

typedef struct
{
	HI_U32 bpc;
	HI_U16 proxy_tei[5];
}HI_DMID_MAC_STA_REPORT_ASSOC_PROXY_S;

typedef struct
{
	HI_U8  current_freq;
	HI_U8  next_freq;
	HI_U8  pad[2];
}HI_DMID_MAC_STA_REPORT_FREQ_BLIND_CHECK_S;


typedef struct
{
    HI_U16 tei;
    HI_U8  path_comm_rate;
    HI_U8  avg_rssi;
}dfx_rf_rt_path_comm_rate_st;

 #define DFX_RT_SLIDE_WINDOW_CNT 6
typedef struct
{
    HI_U16    tei;
    HI_U8     level;
    HI_U8     ability;
    HI_U8     comm_rate;                                            //վ����ú�ѡվ���ͨ�ųɹ���
    HI_U8     down_rate;                                            //վ����ոú�ѡվ��ı��ĳɹ���
    HI_U16    rsvd;
    HI_U16    slid_window_down[DFX_RT_SLIDE_WINDOW_CNT];
    HI_U16    slid_window_up[DFX_RT_SLIDE_WINDOW_CNT];
    HI_U16    slid_window_senk_pk_cnt[DFX_RT_SLIDE_WINDOW_CNT];
}HI_DMID_MAC_NEW_CHANGE_PROXY_DFX_INFO_S;


typedef struct
{
    HI_BOOL result;
    HI_U8   reason;
    HI_U16  new_proxy;

    HI_U16  req_change_tei;
    HI_U8   new_level;                      //������վ����²㼶
    HI_U8   pad;

    HI_U16  old_proxy;
    HI_U16  chose_proxy_tei[5];

    HI_U32  end_sequence;
    HI_DMID_MAC_NEW_CHANGE_PROXY_DFX_INFO_S  old_proxy_info;
    dfx_rf_rt_path_comm_rate_st rf_proxy_info[DFX_RF_PATH_TABLE_MAX_NUM];
}HI_DMID_MAC_NEW_CHANGE_PROXY_DFX_S;

typedef struct
{
    HI_U16 req_change_tei;
    HI_U16 rsvd;
    HI_DMID_MAC_NEW_CHANGE_PROXY_DFX_INFO_S  new_proxy_info;
}HI_DMID_MAC_NEW_CHANGE_PROXY_DFX_ITEM_S;

typedef struct
{
    HI_U8 mac[6];
    HI_U16 rsvd;
}HI_DMID_MAC_CCO_HEART_BEAT_CHECK_WARNING_S;

typedef struct
{
    HI_U8 reason;
    HI_U8 rsvd[3];
}HI_DMID_MAC_STA_LEAVE_NETWORK_S;

typedef struct
{
    HI_U8 mac[6];
    HI_U16 tei;
    HI_U16 proxy_tei;
    HI_U16 rsvd;
}HI_DMID_MAC_CCO_JUDGE_STA_LEAVE_NETWORK_S;

typedef struct
{
    HI_U8 max_level;
    HI_U8 rsvd[3];
    HI_U16 child_sum;
    HI_U16 pco_sum;
    HI_U32 cur_bpc;
}HI_DMID_MAC_CCO_START_NETWORK_FORM_S;

typedef struct
{
    HI_U8 level;
    HI_U8 rsvd;
    HI_U16 child_sum;
    HI_U32 cur_bpc;
}HI_DMID_MAC_CCO_START_SOME_LEVEL_JOIN_S;

typedef struct
{
    HI_U8 last_level;
    HI_U8 rsvd;
    HI_U16 last_level_join_child_sum;
    HI_U32 cur_bpc;
}HI_DMID_MAC_CCO_LAST_LEVEL_JOIN_COUNT_S;

typedef struct
{
    HI_U8 max_level;
    HI_U8 rsvd[3];
    HI_U16 pco_sum;
    HI_U16 child_sum;
    HI_U32 cur_bpc;
}HI_DMID_MAC_CCO_NETWORK_FORM_OK_INFO_S;

typedef struct
{
    HI_U8  conflict_cco_mac[6];
    HI_U16 neighbor_snid;                   //�ھ������SNIDλͼ

    HI_U8  sta_mac[6];
    HI_U16 sta_tei;

    HI_U8  conflict_type;
    HI_U8  pad;
    HI_U16 ptcl_ver;
}HI_DMID_MAC_NETWORK_CONFLICT_REPORT_INFO_S;

typedef struct
{
    HI_U16 ptcl_ver;
    HI_U16 pad1;

    HI_U32 free_snid_list[15];
    HI_U32 change_snid;
}HI_DMID_MAC_NETWORK_CONFLICT_CHOOSE_NEW_SNID_S;

typedef struct
{
    HI_U32 snid[16];

    HI_U8  sta_mac[6];
	HI_U16 ptcl_ver;

    HI_U16 sta_tei;
    HI_U16 pad1;
}HI_DMID_MAC_QUERY_NEIGHBOR_NETWORK_INFO_S;

typedef struct
{
    HI_U16 tei;
    HI_U8  role;                                     // վ���ɫ
    HI_U8  level;                                    // վ��㼶

    HI_U8  min_multiproxy_comm_rate;                 // ·����Сͨ����
    HI_U8  proxy_comm_rate;			                 // �����վ�������ͨ����
    HI_U8  proxy_down_comm_rate;		             // �����վ�������ͨ����
    HI_U8  down_comm_rate;                           // ����ͨ�ųɹ���

    HI_U8  up_comm_rate;			                 // ����ͨ�ųɹ���
    HI_U8  comm_rate;
    HI_U8  level_relation;                           // �㼶��ϵ
    HI_U8  attenuation;                              //��¼�ź�˥��ֵ����Ȩ�����Ľ����

    HI_U8  mmib;                                     //��¼�ŵ�����ֵ����Ȩ�����Ľ����
    HI_U8  proxy_mmib;                               //��¼����վ����ŵ�����ֵ��
    HI_U16 cur_down_rcv_cnt;		                 // ��ǰ�������н��ո���(����ǰͳ�ƽ����ű�֡�����ܻᳬ��255����˲���16λ)

    HI_U32 send_fail_cnt;                            // ����ʧ�ܴ���
    HI_U32 send_success_cnt;                         // ���ͳɹ�����
    HI_U32 rcv_beacon_cnt;                           // ���ո�վ��BEACON֡�ĸ���
    HI_U32 rcv_cnt;                                  // ���յ���վ�㷢����վ��ĵ�������(DTEI == my_tei)

}HI_DMID_MAC_STA_CANDIDATE_PROXY_S;

typedef struct
{
    HI_U16      tei;
    HI_U8        pb_count;
    HI_U8        fc_crc;

    HI_U8        pb_crc[4];

    HI_U8        fc_mmib;
    HI_U8        final_value;
    HI_U8        rsvd[2];

    HI_U8        pb_mmib[4];

}HI_DMID_MAC_STA_MMIB_INFO_S;


typedef struct
{
    HI_U16 tei;
    HI_U16 preamble_sat_num;

    HI_U8   attenuation;
    HI_U8   final_value;
    HI_U8   rsvd[2];
}HI_DMID_MAC_STA_ATTENUATION_INFO_S;

// �����ģ����Ϣ�ṹ�嶨��
typedef struct
{
    HI_U32 bpc;
    HI_U32 ntb;
    HI_U32 msdu_size;
}HI_DMID_MAC_SEND_MSDU_TO_APP_S;

typedef struct
{
    HI_U32 bpc;
    HI_U32 ntb;

    HI_U8  oda[6];
    HI_U8  osa[6];
    HI_U8  dst_ip[4];
    HI_U8  src_ip[4];

    HI_U16 stei;
    HI_U16 ostei;

    HI_U16 odtei;
    HI_U16 ctei;

    HI_U16 pad1;
    HI_U16 pad;

    HI_U16 packet_index;
    //HI_U32 msdu_size;
    HI_U16 msdu_size;
}HI_DMID_MAC_SEND_MSDU_TO_IP_S;


//������ģ����Ϣ�ṹ�嶨��
typedef struct
{
    HI_U32 ats;

    HI_U16 ctei;
    HI_U16 odtei;
    HI_U16 ostei;
    HI_U16 stei;

    HI_U8  hop_cnt;
    HI_U8  lid;
    HI_U16 msdu_size;

    HI_U16 packet_index;
    HI_U16 source_type;

    HI_U16 mmtype;
    HI_U8  broadcast_flag;
    HI_U8  pad2;
} HI_DMID_MAC_RSB_MPDU_INFO_S;


//���ͻ�ģ����Ϣ����
typedef struct
{
    HI_U32 cur_ntb;
    HI_U16 mpdu_index;
    HI_U8   send_flag;
    HI_U8   discard_flag;
}HI_DMID_MAC_TX_MPDU_FLAG_SET_S;

typedef struct
{
    HI_U16 dtei;
    HI_U8   phase;
    HI_U8   result;
}HI_DMID_MAC_TX_BROADCAST_PHASE_NEXTHOP_S;

typedef struct
{
    HI_U16 odtei;
    HI_U16 dtei;
    HI_U8   result;
    HI_U8   rsvd[3];
}HI_DMID_MAC_TX_UNICAST_NEXTHOP_S;

typedef struct
{
    HI_U16 dtei;
    HI_U8   phase;
    HI_U8   rsvd;
}HI_DMID_MAC_TX_NEXTHOP_PHASE_INFO_S;

typedef struct
{
    HI_U32     ats;
    HI_U32     timeout;

    HI_U16     stei;
    HI_U16     selected_nexthop_byctei;

    HI_U16     ctei;
    HI_U16     ostei;

    HI_U16     odtei;
    HI_U8       lid;
    HI_U8       encryption_flag;

    HI_U8      broadcast_flag;
    HI_U8      hop_cnt;
    HI_U16    msdu_size;

    HI_U8      pb_count;
    HI_U8      eks;
    HI_U16    pb_size;

    HI_U16    mpdu_index;
    HI_U16    source_type;

    HI_U8      snid;
    HI_U8      rsvd;
    HI_U16    packet_index;
}HI_DMID_MAC_TX_MSDU_MAC_BUFFER_INFO_S;

typedef struct
{
    HI_U16 mpdu_index;
    HI_U16 dtei;
    HI_U32 result;
    HI_U16 odtei;
    HI_U16 rsvd;
}HI_DMID_MAC_TX_ROUTE_FEEDBACK_INFO_S;

#define EMC_MESSAGE_HEAD_OFFSET                 (0x871)
#define HI_DMID_MAC_EMC_NBI_TABLE_REPORT_INFO   EMC_MESSAGE_HEAD_OFFSET + 0x00
#define HI_DMID_MAC_EMC_NBI_POWER_REPORT_INFO   EMC_MESSAGE_HEAD_OFFSET + 0x01
#define HI_DMID_MAC_EMC_POWER_CONTROL_PK_INFO   EMC_MESSAGE_HEAD_OFFSET + 0x02
#define HI_DMID_MAC_EMC_POWER_CONTROL_RESULT    EMC_MESSAGE_HEAD_OFFSET + 0x03
#define HI_DMID_MAC_EMC_WHITE_NOISE_INFO        EMC_MESSAGE_HEAD_OFFSET + 0x04
#define HI_DMID_MAC_EMC_AVOID_FREQ_INFO         EMC_MESSAGE_HEAD_OFFSET + 0x05      //���õ��ز�
#define HI_DMID_MAC_EMC_RECOVERY_FREQ_INFO      EMC_MESSAGE_HEAD_OFFSET + 0x06  //�ָ����ز�
#define HI_DMID_MAC_EMC_HEST_INFO               EMC_MESSAGE_HEAD_OFFSET + 0x07
#define HI_DMID_MAC_EMC_LAB_INFO               EMC_MESSAGE_HEAD_OFFSET + 0x08
#define HI_DFX_MRS_BUF_MAX_LEN                  20

#define HI_DMID_MAC_EMC_POWER_CONTROL_BASIC     EMC_MESSAGE_HEAD_OFFSET + 0x10

typedef struct
{
    HI_U8  emc_inter_type;  		// NBIƵ����ϱ�ģʽ
    HI_U8  pad[3];
    HI_U32 emc_nbi_table[16];			// NBIƵ�������
}HI_DMID_MAC_NBI_TABLE_REPORT_INFO_STRU;

typedef struct
{
    HI_U8  nbi_power_mode;          // NBI�����ϱ�ģʽ
    HI_U8  max_power;               // �����ֵ
    HI_U8  min_power;               // ��С����ֵ
    HI_U8  diff;                    // ��ֵ

    HI_U8  nbi_power[32];           // NBI����ͳ�Ʊ�
}HI_DMID_MAC_NBI_POWER_REPORT_INFO_STRU;

#define EMC_POWER_CONTROL_PK_PERIODIC               (1)     // ������
#define EMC_POWER_CONTROL_PK_ACTIVE                 (2)     // ��������
#define EMC_POWER_CONTROL_PK_PASSIVE                (3)     // ��������
#define EMC_POWER_CONTROL_PK_RCV                    (4)     // ���յ����ʿ��Ʊ���
#define EMC_POWER_CONTROL_RCV_FRAME_ATTENUATION     (5)     // ����֡ʱ���ŵ�˥��ֵ
typedef struct
{
    HI_U8  pk_type;
    HI_U8  channel_attenuation;
    HI_U16 dtei;

    HI_U8 emc_pream_sat_num;
    HI_U8 emc_pk_pow_ctrl_switch;
    HI_U8 emc_pow_ctrl_switch;
    HI_U8 emc_pow_ctrl_start;
}HI_DMID_MAC_POWER_CONTROL_PACKET_INFO_STRU;

typedef struct
{
    HI_U8  power_control_type;
    HI_U8  attenuation;
    HI_S8  emc_powindex_before_adj;
    HI_U8  emc_AFE_before_adj;

    HI_S8  emc_powindex_after_adj;
    HI_U8  emc_AFE_after_adj;
    HI_U8  pad[2];
}HI_DMIMD_MAC_POWER_CONTORL_INFO_STRU;

typedef struct
{
    HI_U8  emc_lab_scene;
    HI_U8  emc_1vs1_scene;
    HI_U8  emc_awgn_scene;
    HI_U8  emc_freq_pow_scene;

    HI_U8  emc_freq_pow_sum;
    HI_U8  pad[3];
}HI_DMIMD_LAB_INFO_STRU;


typedef struct
{
    HI_U8  white_noise_value;
    HI_U8  emc_awgn_conti_num;
    HI_U8  emc_nonawgn_conti_num;
    HI_U8  pad;
}HI_DMID_MAC_WHITE_NOISE_INFO_STRU;

typedef struct
{
    HI_U16 nbi_start;
    HI_U16 nbi_end;

    HI_U32 last_detect_time;
}HI_DMID_MAC_FREQ_AVOID_RESULT_INFO_STRU;

typedef struct
{
    HI_U32 max_hest_value;
    HI_U32 min_hest_value;

    HI_U32 exce_time;
    HI_U16 carrier_num;
    HI_U8  hest_continue_detect_cnt;
    HI_U8  pad;
}HI_DMID_MAC_HEST_INFO_STRU;

typedef struct
{
    HI_U8  ucModeFlag;        //flag of mode
    HI_U8  ucCurMode ;
    HI_U8  padding[2];           //Ԥ��

    HI_U32 ulEnterTimeStamp;  //timestamp of enter mode
    HI_U32 ulExitTimeStamp;   //timestamp of exit mode

}HI_DSID_APP_CSG_TEST_MODE_INF_S;

typedef struct
{
    HI_U32 ulCnt;              //Counts
    HI_U8 ucMode;
    HI_U8 pad[3];             
    HI_U32 ulLatestTS;         // time stamp of latest 
    
    HI_U32 ulPbbErrCnt;       // uncorrect PBB  cnt
    HI_U8 ucPayloadType;
    HI_U8 pad1[3];
    HI_U32 ulPbbErrTS;        // time stamp of err pbb 
    
    HI_U32 ulParseErrAppCnt;  //  cnt of parse err app data
    HI_U32 ulRet;
    HI_U32 ulParseErrTS;      // time stamp of err pbb 

    HI_DSID_APP_CSG_TEST_MODE_INF_S stModeInfo[3];
    
    HI_U8 aucAppData[HI_DFX_MRS_BUF_MAX_LEN];

    HI_U8 aucPayLoadData[2*HI_DFX_MRS_BUF_MAX_LEN];

}HI_DSID_APP_CSG_PYSICAL_MODE_INF_S_CSG;

typedef struct
{
    HI_U32 ulCnt;              //Counts
    HI_U8 ucMode;
    HI_U8 pad[3];
    HI_U32 ulLatestTS;         // time stamp of latest

    HI_U32 ulPbbErrCnt;       // uncorrect PBB  cnt
    HI_U8 ucPayloadType;
    HI_U8 pad1[3];
    HI_U32 ulPbbErrTS;        // time stamp of err pbb

    HI_U32 ulParseErrAppCnt;  //  cnt of parse err app data
    HI_U32 ulRet;
    HI_U32 ulParseErrTS;      // time stamp of err pbb

    HI_U8 ucInvalidPortNum;            // port NUM
    HI_U8 ucInvalidTestId;             //
    HI_U16 usPayloadLen;

    HI_U32 ulEnterTs;          // timespan of enter time
    HI_U32 ulEnterCnt;        // The cnts of Enter  INTO MODE

    HI_DSID_APP_CSG_TEST_MODE_INF_S stModeInfo[6];

    HI_U8 aucAppData[HI_DFX_MRS_BUF_MAX_LEN];

    HI_U8 aucPayLoadData[2*HI_DFX_MRS_BUF_MAX_LEN];

}HI_DSID_APP_CSG_PYSICAL_MODE_INF_S;



// NTB ģ����Ϣ���壬��Χ [0x7D0~0X810)
#define HI_DMID_MAC_NTB_BASE                            (0x7D0)
#define HI_DMID_MAC_NTB_SYNC_PARAM                      HI_DMID_MAC_NTB_BASE+0x1
#define HI_DMID_MAC_NTB_FIRST_CYC_SYNC_STA              HI_DMID_MAC_NTB_BASE+0x2
#define HI_DMID_MAC_NTB_FIRST_CYC_CANDI_IS_BETTER       HI_DMID_MAC_NTB_BASE+0x4
#define HI_DMID_MAC_NTB_FIRST_CYC_ADD_CANDI             HI_DMID_MAC_NTB_BASE+0x6
#define HI_DMID_MAC_NTB_FIRST_CYC_REPLACE_CANDI         HI_DMID_MAC_NTB_BASE+0x7
#define HI_DMID_MAC_NTB_UPD_CANDI_SYNC_STA              HI_DMID_MAC_NTB_BASE+0x8
#define HI_DMID_MAC_NTB_CONFIG_SYNC_STA                 HI_DMID_MAC_NTB_BASE+0x9
#define HI_DMID_MAC_NTB_SET_BEST_CANDI_AS_SYNC_STA      HI_DMID_MAC_NTB_BASE+0xA
#define HI_DMID_MAC_NTB_NO_SYNC_STA                     HI_DMID_MAC_NTB_BASE+0xB
#define HI_DMID_MAC_NTB_CANDI_STA_ERROR                 HI_DMID_MAC_NTB_BASE+0xC
#define HI_DMID_MAC_NTB_SWITCH_SYNC_STA                 HI_DMID_MAC_NTB_BASE+0xD
#define HI_DMID_MAC_NTB_RCV_SYNC_STA_ONE_BCN            HI_DMID_MAC_NTB_BASE+0xE
#define HI_DMID_MAC_NTB_NEIGHBOR_FREQERR                HI_DMID_MAC_NTB_BASE+0xF
#define HI_DMID_MAC_NTB_RESET_REASON                    HI_DMID_MAC_NTB_BASE+0x10
#define HI_DMID_MAC_NTB_SYNC_STA_EXCEPTION              HI_DMID_MAC_NTB_BASE+0x11
#define HI_DMID_MAC_NTB_SET_PROXY_CANDI_AS_SYNC_STA     HI_DMID_MAC_NTB_BASE+0x12
#define HI_DMID_MAC_NTB_LONG_BCN_INTERVAL               HI_DMID_MAC_NTB_BASE+0x13
#define HI_DMID_MAC_NTB_RCV_CANDI_STA_ONE_BCN           HI_DMID_MAC_NTB_BASE+0x14
#define HI_DMID_MAC_NTB_CANDI_STA_EXCEPTION             HI_DMID_MAC_NTB_BASE+0x15
#define HI_DMID_MAC_NTB_CYCLE                           HI_DMID_MAC_NTB_BASE+0x16
// NTB ģ����Ϣ�ṹ�嶨��
typedef struct
{
    HI_U16 tei;
    HI_U8  rsv;
    HI_U8  freq;                //PHY�Ĳ��м��Ƶ����Դ���

    HI_S16 snr;
    HI_U8  bt;
    HI_U8  level;

    HI_U32 snid;

    HI_U32 CBTSn;
    HI_U32 LTmrn_08;
    HI_U32 LTmrn_32;

    HI_U32 time_stamp;
} HI_DMID_MAC_NTB_SYNC_PARAM_S;

typedef struct
{
	HI_U32 tei                      :12;
	HI_U32 level                    :4;
	HI_U32 bt                       :2;
	HI_U32 is_ntb_param_avail       :1;
	HI_U32 has_enough_freqerr       :1;
	HI_U32 avail_freqerr_pos        :4;
	HI_U32 has_calc_freqerr         :1; // �Ƿ���������ЧƵƫ
    HI_U32 freq                     :5;
    HI_U32 pad                      :2;

    HI_U32 snid                     :24;
    HI_U32 pad1                     :8;

	HI_S16 snr;
	HI_U16 freqerr_exception_cnt;

	HI_U32 rcv_bcn_cnt;
	HI_S16 freqerr_output;
    HI_U16 pad2;

	HI_U32 freqerr_offset_high;
	HI_U32 freqerr_offset_low;
	HI_U32 CBTSn_front;
    HI_U32 LTmrn_front_08;
	HI_U32 LTmrn_front_32;

	HI_U32 last_rcv_timestamp;
	HI_U32 ntb;
}HI_DMID_MAC_NTB_SYNC_STA_S;

typedef struct
{
    HI_U16 tei  :12;
    HI_U16 rsv  :4;
    HI_S16 freqerr;

    HI_U32 snid;

    HI_U8  report_state;           //�ϱ��׶Σ�1:pb��ȷ��2:pb����
    HI_U8  freq;
    HI_S16 rx_cubic;
    HI_U32 bpc;

} HI_DMID_MAC_NTB_NEIGHBOR_FREQERR_S;

// ��Χ[0x891~8A0]
#define MAC_LOW_POWER_HEAD_OFFSET       (0x891)
#define HI_DMID_MLP_PHY_SLEEP_INFO      (MAC_LOW_POWER_HEAD_OFFSET + 0x00)
#define HI_DMID_MLP_STATE_SWITCH_INFO   (MAC_LOW_POWER_HEAD_OFFSET + 0x01)

typedef struct
{
    HI_BOOL phy_work_status;
    HI_U8   pad[3];
    HI_U32  ntb;
    HI_U32  sleep_cycle;
}HI_DMID_MLP_PHY_SLEEP_INFO_S;
typedef struct
{
    HI_U8 low_power_state;
    HI_U8 pad[3];
}HI_DMID_MLP_STATE_SWITCH_INFO_S;

//*****************************************************************************
// APPͳ��������
//*****************************************************************************
// ��Χ [A450, A850)
//*****************************************************************************
#define HI_DSID_APP_UART_MSG_SEND_INF       0xA450      // UART_CHL_INF_S (ok)
#define HI_DSID_APP_UART_MSG_PROC_INF       0xA451      // UART_CHL_INF_S (ok)
#define HI_DSID_APP_PLC_MSG_SEND_INF        0xA452      // PLC_CHL_INF_S (ok)
#define HI_DSID_APP_PLC_MSG_PROC_INF        0xA453      // PLC_CHL_INF_S (ok)
#define HI_DSID_APP_REFUSE_CMD_INF          0xA454      // HI_DSID_APP_REFUSE_CMD_INF_S
#define HI_DSID_APP_TOPO_CHANGE_INF         0xA455      // HI_DSID_APP_TOPO_CHANGE_INF_S (ok)
#define HI_DSID_APP_MSG_FILTER_INF          0xA456      // HI_DSID_APP_MSG_FILTER_INF_S
#define HI_DSID_APP_MAC_LIST_CONT           0xA457      // HI_DSID_APP_MAC_LIST_CONT_S
#define HI_DSID_APP_MAC_MAP_CONT            0xA458      // HI_DSID_APP_MAC_MAP_CONT_S
#define HI_DSID_APP_MREC_STATUS             0xA459      // HI_DSID_APP_MREC_STATUS_S
#define HI_DSID_APP_MREC_INF                0xA45A      // HI_DSID_APP_MREC_INF_S
#define HI_DSID_APP_HEARTBEAT_INF           0xA45B      // HI_DSID_APP_HEARTBEAT_INF_S (ok)
#define HI_DSID_APP_CCO_RUN_STATUS          0xA45C      // HI_DSID_APP_CCO_RUN_STATUS_S (ok)
#define HI_DSID_APP_RM_INF                  0xA45D      // HI_DSID_APP_RM_INF_S (ok)
#define HI_DSID_APP_VM_CHL_INF              0xA45E      // HI_DSID_APP_VM_CHL_INF_S (ok)
#define HI_DSID_APP_STA_STATUS              0xA45F      // HI_DSID_APP_STA_STATUS_S (ok)
#define HI_DSID_APP_MSG_INF                 0xA460      // HI_DSID_APP_MSG_INF_S
#define HI_DSID_APP_PLC_BC_INF              0xA461      // HI_DSID_APP_PLC_BC_INF_S (ok)
#define HI_DSID_APP_RM_QUEUE_INF            0xA462      // HI_DSID_APP_RM_QUEUE_INF_S (ok)
#define HI_DSID_APP_REMOTE_UPG_INF          0xA480      // HI_DSID_APP_REMOTE_UPG_INF (ok)
#define HI_DSID_APP_CCO_EVT_INF             0xA465      // HI_DSID_APP_CCO_EVT_INF_S (ok)
#define HI_DSID_APP_RM_RX_TX_INF            0xA466      // HI_DSID_APP_RM_RX_TX_INF (ok)
#define HI_DSID_APP_CCO_GET_TIME_INF        0xA467      // HI_DSID_APP_CCO_GET_TIME_INF (ok)
#define HI_DSID_APP_STA_EVT_INF             0xA468      // HI_DSID_APP_STA_EVT_INF (ok)
#define HI_DSID_APP_CCO_REPORT_QUEUE_INF    0xA469      // HI_DSID_APP_CCO_REPORT_QUEUE_INF_S(ok)
#define HI_DSID_APP_CCO_EVT_BAK_INF         0xA470      // HI_DSID_APP_CCO_EVT_INF_S(ok)
#define HI_DSID_APP_CCO_MEMORY_INC_INF      0xA471      // HI_DSID_APP_CCO_FLASH_INC_INF_S(ok)
#define HI_DSID_APP_CCO_NODE_STATECHG_INF   0xA472      // HI_DSID_APP_CCO_NODE_STATECHG_INF_S(ok)
#define HI_DSID_APP_STA_EVT_MONITOR_INF     0xA475      // HI_DSID_APP_STA_EVT_MONITOR_INF (ok)
#define HI_DSID_APP_CCO_DETECT_RM_INF       0xA476      // HI_DSID_APP_CCO_DETECT_RM_INF_S(ok)

#define HI_DSID_APP_PLC_FRAME_STAT_INF      0xA477      // HI_DSID_APP_PLC_FRAME_STAT_INF_S(ok)
#define HI_DSID_APP_CSG_PYSICAL_MODE_INF    0xA481      // HI_DSID_APP_CSG_PYSICAL_MODE_INF_S(ok)
#define HI_DSID_APP_CCO_POWER_ON_OFF_INF    0xA482      // HI_DSID_APP_CCO_POWER_ON_OFF_INF_S(ok)
#define HI_DSID_APP_STA_POWER_ON_OFF_INF    0xA483      // HI_DSID_APP_STA_POWER_ON_OFF_INF_S(ok)

// UPG ʹ�õķ�Χ 0XA500~0XA600
#define HI_DSID_APP_UPG_CONTINUS_START_NV   0xA500 // HI_DSID_APP_UPG_CONTINUS_START_NV_S

#define HI_DSID_APP_UPG_CONTINUS_TOPO_NOTIFY_INFO 0xA501

#define HI_DSID_APP_UPG_CCO_STAT            0xA502    // HI_DSID_APP_UPG_CCO_STAT_S
#define HI_DSID_APP_UPG_STA_STAT            0xA503    // HI_DSID_APP_UPG_STA_STAT_S
#define HI_DSID_APP_UPG_CCO_STAT1           0xA504
#define HI_DSID_APP_UPG_CCO_STAT2           0xA505
#define HI_DSID_APP_UPG_STA_STAT1           0XA506 //HI_DSID_APP_UPG_STA_STAT1_S
#define HI_DSID_APP_UPG_CCO_STAT3           0XA507 //HI_DSID_APP_UPG_CCO_STAT3_S
#define HI_DSID_APP_UPG_STA_STAT2           0XA508 //HI_DSID_APP_UPG_STA_STAT2_S
#define HI_DSID_APP_UPG_CCO_STAT4           0XA509 //HI_DSID_APP_UPG_CCO_STAT4_S
#define HI_DSID_APP_UPG_CCO_STAT5           0XA50A //HI_DSID_APP_UPG_CCO_STAT5_S
#define HI_DSID_APP_UPG_BC_SND_INFO         0XA50B //HI_DSID_APP_UPG_BC_SND_INFO_S
#define HI_DSID_APP_UPG_U2B_CHECK           0XA50C //HI_DSID_APP_UPG_CHECK_S
#define HI_DSID_APP_UPG_U2B_PROCESS_ITEM    0XA50D //HI_DSID_APP_UPG_U2B_PROCESS_ITEM_S
#define HI_DSID_APP_UPG_U2B_SND_INFO        0XA50E //HI_DSID_APP_UPG_SND_INFO_S
#define HI_DSID_APP_UPG_UC_CHECK            0XA50F //HI_DSID_APP_UPG_CHECK_S
#define HI_DSID_APP_UPG_UC_PROCESS_ITEM     0XA510 //HI_DSID_APP_UPG_UC_PROCESS_ITEM_S
#define HI_DSID_APP_UPG_UC_SND_INFO         0XA511 //HI_DSID_APP_UPG_SND_INFO_S
#define HI_DSID_APP_UPG_CCO_QUERY_INFO      0XA512 //HI_DSID_APP_UPG_CCO_QUERY_INFO_S
#define HI_DSID_APP_UPG_STA_QUERY_INFO      0XA513 //HI_DSID_APP_UPG_STA_QUERY_INFO_S
#define HI_DSID_APP_UPG_STA_DEVICE_INFO     0XA514 //HI_DSID_APP_UPG_STA_DEVICE_INFO_S
#define HI_DSID_APP_UPG_LIST_INFO           0xA515 //HI_DSID_APP_UPG_LIST_INFO_S
#define HI_DSID_APP_UPG_CCO_SDK_STAT        0xA516 //HI_DSID_APP_UPG_CCO_SDK_STAT_S
#define HI_DSID_APP_UPG_STA_SDK_STAT        0xA517 //HI_DSID_APP_UPG_STA_SDK_STAT_S

#define HI_DSID_APP_EQT_CHL_STAT            0xA600    // HI_DSID_APP_EQT_CHL_STAT_S


//*****************************************************************************
// �ṹ����
//*****************************************************************************
#define METER_ADDR_LEN  6    // ���ַ����
#define MAX_METER_NUM   32   // ���ı��ַ����
#define MAX_QUEUE_NUM   3    // ����Ԫ��������
#define MAX_BUF_LEN     64   // ��������󳤶�
#define MAX_LAST_NUM    16   // �����ʷ��¼����
#define MAX_LAST_NUM_1  8    // �����ʷ��¼����1
#define MAX_LAST_NUM_2  4    // �����ʷ��¼����2
#define MAX_RM_POOL_NUM 2    // ��������ظ���

typedef struct
{
    HI_U8 addr[METER_ADDR_LEN];    // ���ַ
}METER_ADDR; // ���ַ

typedef struct
{
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];    // Mac��ַ
}MAC_ADDR; // Mac��ַ


typedef struct
{
    HI_U32 rx_total_len;        // ���յ������ֽ���
    HI_U32 rx_last_time;        // ���һ�ν���ʱ��
    HI_U32 rx_ok_len;           // ���յ���ȷ���ֽ���(����������֡)
    HI_U32 rx_err_len;          // ���յ�������ֽ���(��������֡ʧ��ʱ�������ֽ���)
    HI_U32 rx_ok_num;           // ���յ�����֡�ĸ���
    HI_U32 rx_err_num;          // ���յ�����֡�ĸ���
    HI_U32 rx_err_cause;        // ������һ�δ���ԭ��
    HI_U32 rx_buf_tail;         // ���������ݽ���λ��
    HI_U8 rx_buf[MAX_BUF_LEN];  // �����յ�������(ѭ������-rx_buf_tailΪ��ǰ���ǵ�β��)
    HI_U32 tx_total_len;        // �ܷ������ֽ���
    HI_U32 tx_last_time;        // ���һ�η���ʱ��
    HI_U32 tx_ok_len;           // ���ͳɹ����ֽ���
    HI_U32 tx_fail_len;         // ����ʧ�ܵ��ֽ���(����DMS�ӿڷ���ʧ�ܺ��ͳ��)
    HI_U32 tx_total_num;        // �ܷ���֡����
    HI_U32 tx_ok_num;           // ���ͳɹ���֡����
    HI_U32 tx_fail_num;         // ����ʧ�ܵ�֡����(����DMS�ӿڷ���ʧ�ܺ��ͳ��)
    HI_U32 tx_err_cause;        // ���һ�η���ʧ��ԭ��
    HI_U32 tx_last_len;         // ��һ֡���͵����ݳ���
    HI_U8 tx_buf[MAX_BUF_LEN];  // ��һ֡���͵���������
}UART_CHL_INF_S;    // Uartͨ��ͳ��

typedef struct
{
    HI_U32 rx_total_num;       // �ܽ��հ�����
    HI_U32 rx_time;            // ��һ�ν�������ʱ��
    HI_U16 rx_id;              // ��һ�ν��յ�id
    HI_U8 src_mac[HI_PLC_MAC_ADDR_LEN];          // ��һ�ν��յ�Դmac
    HI_U32 rx_len;             // ��һ�ν������ݳ���
    HI_U8 rx_buf[MAX_BUF_LEN]; // ��һ�ν�����������(����)
    HI_U32 rx_err_num;         // ���մ��������
    HI_U32 rx_err_time;            // ��һ�ν��մ���֡����ʱ��
    HI_U16 rx_err_id;              // ��һ�ν��մ���֡��id
    HI_U8 err_src_mac[HI_PLC_MAC_ADDR_LEN];          // ��һ�ν��մ���֡��Դmac
    HI_U32 rx_err_cause;           // ��һ�ν��ܵĴ���֡ԭ��
    HI_U32 rx_err_len;             // ��һ�ν��մ���֡���ݳ���
    HI_U8 rx_err_buf[MAX_BUF_LEN]; // ��һ�ν��մ���֡��������(����)
    HI_U32 tx_total_num;       // �ܷ��Ͱ�����
    HI_U32 tx_time;            // ��һ�η��͵�����ʱ��
    HI_U16 tx_id;              // ��һ�η��͵�id
    HI_U8 dest_mac[HI_PLC_MAC_ADDR_LEN];         // ��һ�η��͵�Ŀ��mac
    HI_U32 tx_len;             // ��һ�η��͵����ݳ���
    HI_U8 tx_buf[MAX_BUF_LEN]; // ��һ�η��͵���������(����)
    HI_U32 tx_fail_num;        // ����ʧ�ܰ�����
    HI_U32 tx_fail_time;            // ��һ�η��͵�����ʱ��
    HI_U16 tx_fail_id;              // ��һ�η��͵�id
    HI_U8 fail_dest_mac[HI_PLC_MAC_ADDR_LEN];         // ��һ�η��͵�Ŀ��mac
    HI_U32 tx_err_cause;           // ��һ�ν��ܵĴ���֡ԭ��
    HI_U32 tx_fail_len;             // ��һ�η��͵����ݳ���
    HI_U8 tx_fail_buf[MAX_BUF_LEN]; // ��һ�η��͵���������(����)
}PLC_CHL_INF_S;     // PLCͨ������ͳ��

typedef struct
{
    UART_CHL_INF_S uart_msg_send_inf;
    UART_CHL_INF_S uart_msg_proc_inf;
    PLC_CHL_INF_S plc_msg_send_inf;
    PLC_CHL_INF_S plc_msg_proc_inf;
}HI_DSID_APP_CHL_RX_TX_INF_S; // ͨ���շ�ͳ��

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 heart_num;    // ��������
    HI_U32 last_time[MAX_LAST_NUM];    // ��ʷ����ʱ��
    HI_U32 timeout_num;  // ������ʱ����
    HI_U32 timeout_time[MAX_LAST_NUM]; // ��ʷ������ʱʱ��
}HI_DSID_APP_HEARTBEAT_INF_S;    // ����ͳ��

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U8 status[MAX_LAST_NUM];          // CCO״̬
    HI_U32 last_time[MAX_LAST_NUM];      // ��ʷ״̬�ı�ʱ��
    HI_U32 map_ready_time;  // ӳ���ά�����ʱ��
    HI_U32 map_last_time;   // ���һ��ӳ���ά�����ʱ��
}HI_DSID_APP_CCO_RUN_STATUS_S; // CCO����״̬

typedef struct
{
    HI_U8 route;            // CCO·��״̬
    HI_U8 LR_status;        // CCO���ֳ�״̬
    HI_U16 meter_num;       // ��������
    HI_U32 LR_req_cnt;      // �ֳ�ѯ�ʶ������
    HI_U32 LR_ack_cnt;      // �ֳ�Ӧ��������
    HI_U32 LR_plc_tx;       // �ֳ�PLC���ʹ���
    HI_U32 LR_plc_rx;       // �ֳ�PLC���մ���
    HI_U32 LR_meter_tx;     // �ֳ�����ʹ���
    HI_U32 LR_meter_rx;     // �ֳ������մ���
    HI_U32 LR_plc_try_cnt;  // �ֳ�PLC�ط�����
    HI_U32 LR_plc_seq_err;  // �ֳ�PLC֡��Ź��ڶ�������
    HI_U32 LR_plc_len_err;  // �ֳ�PLC֡�������ȴ������
    HI_U32 LR_plc_exceed_cnt;  // �ֳ�PLC֡���չ��ڴ���(CCO��û�д��ڽ���PLC״̬)
    HI_U32 LR_plc_timeout;  // �ֳ�PLC��ʱ����
    HI_U32 LR_up_data_cnt;  // �ϱ��������ݴ���
    HI_U32 LR_up_data_try_cnt; // �ط��ϱ��������ݴ���
    HI_U32 LR_ok_cnt;       // �ֳ��ɹ�����(�ϱ��������ɹ����յ�ȷ��֡)
    HI_U32 LR_fail_cnt;     // �ֳ�ʧ�ܴ���(PLC����������Դ���)
}APP_LR_INF_S;

typedef struct
{
    HI_U32 XR_status;       // �泭CCO״̬
    HI_U32 XR_uart_rx;      // �泭���ڽ��մ���(CCOΪ���ն���֡; STAΪ���յ��֡)
    HI_U32 XR_uart_tx;      // �泭���ڷ��ʹ���(CCOΪ���Ͷ���֡; STAΪ���͵��֡)
    HI_U32 XR_plc_tx;       // �泭PLC���ʹ���
    HI_U32 XR_plc_rx;       // �泭PLC���մ���
    HI_U32 XR_nak_num;      // �泭���ϴ���
    HI_U8 XR_nak[MAX_LAST_NUM]; // ���MAX_LAST_NUM�η���ԭ��
    HI_U32 XR_plc_try_cnt;  // �泭PLC�ط�����
    HI_U32 XR_plc_seq_err;  // �泭PLC֡��Ź��ڶ�������
    HI_U32 XR_plc_len_err;  // �泭PLC֡�������ȴ������
    HI_U32 XR_plc_exceed_cnt;   // �泭PLC֡���չ��ڴ���(CCO��û�д��ڽ���PLC״̬)
    HI_U32 XR_plc_timeout;  // �泭PLC��ʱ����
    HI_U32 XR_ok_cnt;       // �泭�ɹ�����
    HI_U32 XR_fail_cnt;     // �泭ʧ�ܴ���
}APP_XR_INF_S;

typedef struct
{
    HI_U32 PR_uart_rx;      // �������ڽ��մ���(CCOΪ���ն���֡; STAΪ���յ��֡)
    HI_U32 PR_uart_tx;      // �������ڷ��ʹ���(CCOΪ���Ͷ���֡; STAΪ���͵��֡)
    HI_U32 PR_plc_tx;       // ����PLC���ʹ���
    HI_U32 PR_plc_rx;       // ����PLC���մ���
    HI_U32 PR_nak_num;      // �������ϴ���
    HI_U8 PR_nak[MAX_LAST_NUM]; // ���MAX_LAST_NUM�η���ԭ��
    HI_U32 PR_plc_try_cnt;  // ����PLC�ط�����
    HI_U32 PR_plc_seq_err;  // ����PLC֡��Ź��ڶ�������
    HI_U32 PR_plc_len_err;  // ����PLC֡�������ȴ������
    HI_U32 PR_plc_timeout;  // ����PLC��ʱ����
    HI_U32 PR_ok_cnt;       // �����ɹ�����
    HI_U32 PR_fail_cnt;     // ����ʧ�ܴ���
}APP_PR_INF_S;

typedef struct
{
    HI_U32 ulPoolId;        // �����ID(����ص��ڴ��ַ)
    HI_U32 ulPoolMode;      // ����ع���ģʽ(MRS_RM_NORMAL_MODE : MRS_RM_BROADCAST_MODE)
    HI_U32 ulParallelNum;   // ��ǰ������
    HI_U32 ulItemNum;       // ��ǰ������������
    HI_U32 ulJoinFailCnt;   // ���뻺���ʧ�ܴ���
    HI_U32 ulJoinFailCause[MAX_LAST_NUM];   // ���MAX_LAST_NUM�μ��뻺���ʧ��ԭ��
}APP_RM_POOL_INF_S;

typedef struct
{
    HI_U32 ulPlcSeq;
    HI_U32 ulTimerStatus;
    APP_RM_POOL_INF_S stPoolInf[MAX_RM_POOL_NUM];
    APP_LR_INF_S LR_inf;    // �ֳ�(����)������Ϣ
    APP_XR_INF_S XR_inf;    // �泭������Ϣ
    APP_PR_INF_S PR_inf;    // ����������Ϣ
}HI_DSID_APP_RM_INF_S; // ���������Ϣͳ��

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 change_num;              // ģ��ͨ���ı����
    HI_U8 status[MAX_LAST_NUM];     // ģ��ͨ��״̬(����ʷ״̬)
    HI_U32 last_time[MAX_LAST_NUM]; // ģ��ͨ��״̬�ı�ʱ��(��ʷ)
}HI_DSID_APP_VM_CHL_INF_S; // ģ��ͨ�������Ϣ

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 status;              // STA״̬
    HI_U32 state_time;                 // STA״̬�ı�ʱ��
    HI_U32 vm_chg_num;                 // �����ı����
    HI_U8 vm_flg[MAX_LAST_NUM];        // ������־(����ʷ״̬)
    HI_U32 vm_time[MAX_LAST_NUM];      // �����״̬�ı�ʱ��(��ʷ)
    HI_U8 report_flg[MAX_LAST_NUM];    // �����ϱ�״̬(����ʷ״̬)
    HI_U32 report_time[MAX_LAST_NUM];  // �����ϱ�״̬�ı�ʱ��(��ʷ)
    HI_U8 change_flg[MAX_LAST_NUM];    // �ı��ϱ�״̬(����ʷ״̬)
    HI_U32 change_time[MAX_LAST_NUM];  // �ı��ϱ�״̬�ı�ʱ��(��ʷ)
}HI_DSID_APP_STA_STATUS_S; // STA״̬ͳ��


typedef struct
{
    HI_U32 delay_20ms_cnt;      // ��Ϣ������ʱ��20ms����
    HI_U32 delay_50ms_cnt;      // ��Ϣ������ʱ��50ms����
    HI_U32 delay_100ms_cnt;     // ��Ϣ������ʱ��100ms����
    HI_U32 delay_200ms_cnt;     // ��Ϣ������ʱ��200ms����
    HI_U32 delay_300ms_cnt;     // ��Ϣ������ʱ��300ms����
    HI_U32 delay_500ms_cnt;     // ��Ϣ������ʱ��500ms����
    HI_U32 delay_1000ms_cnt;    // ��Ϣ������ʱ��1000ms����
    HI_U32 delay_2000ms_cnt;    // ��Ϣ������ʱ��2000ms����
    HI_U32 delay_5000ms_cnt;    // ��Ϣ������ʱ��5000ms����
    HI_U32 delay_10000ms_cnt;   // ��Ϣ������ʱ��10000ms����
    HI_U32 delay_30000ms_cnt;   // ��Ϣ������ʱ��30000ms����
    HI_U32 other_cnt;           // ����δ����������ʱ���޵Ĵ���
}APP_MSG_DLY_LEVEL_CNT_S;

typedef struct
{
    HI_U32 curr_msg_num;        // ��ǰ��Ϣ�������
    HI_U32 max_msg_num;         // ���������
    HI_U32 max_mag_num_time;    // ��������ȷ���ʱ��
    HI_U32 total_tx_cnt;        // ��Ϣ�ܷ��ʹ���
    HI_U32 tx_no_timer_cnt;     // ��Ϣ���ʹ���(������ʱ����Ϣ)
    HI_U32 total_rx_cnt;        // ��Ϣ�ܽ��մ���
    HI_U32 rx_no_timer_cnt;     // ��Ϣ���մ���(������ʱ����Ϣ)
    HI_U32 tx_fail_cnt;         // ��Ϣ����ʧ�ܴ���
    HI_U32 last_tx_id[MAX_LAST_NUM_2];  // ��һ����Ϣ����ID
    HI_U32 last_rx_id[MAX_LAST_NUM_2];  // ��һ����Ϣ�����ID
    HI_U32 last_tx_time;    // ��һ����Ϣ����ʱ��
    HI_U32 last_rx_time;    // ��һ����Ϣ����ʱ��
    HI_U32 last_msg_delay[MAX_LAST_NUM_2];  // ��ʷ��Ϣ������
    HI_U32 max_msg_delay;   // ������Ϣ������
    HI_U32 max_delay_id;    // ���������Ϣ������ʱ��ID
    HI_U32 max_delay_time;  // ���������Ϣ������ʱ��
    HI_U32 max_delay_queue_num;             // ���������Ϣ������ʱ�������
    HI_U32 max_msg_proc;        // �����Ϣ�����ʱ
    HI_U32 max_msg_proc_id;     // �����Ϣ�����ʱID
    HI_U32 max_msg_proc_time;   // �����Ϣ�����ʱ����ʱ��
    HI_U32 last_fail_id[MAX_LAST_NUM];      // ��ʷ��Ϣ����ʧ��ID
    HI_U32 last_fail_time[MAX_LAST_NUM];    // ��ʷ��Ϣ����ʧ��ʱ��
    APP_MSG_DLY_LEVEL_CNT_S delay_cnt;      // ��Ϣ������ʱͳ��
}HI_DSID_APP_MSG_INF_S;     // Ӧ��������Ϣ����ͳ��

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 tx_ok_cnt;    // ʹ�ù㲥��ʽ���ͳɹ�����
    HI_U32 last_tx_time[MAX_LAST_NUM];  // ��ʷ��ʹ�ù㲥���ͳɹ�ʱ��
    HI_U32 tx_fail_cnt;     // ʹ�ù㲥��ʽ����ʧ�ܴ���
    HI_U32 fail_tx_time[MAX_LAST_NUM];  // ��ʷ��ʹ�ù㲥����ʧ��ʱ��
    HI_U32 rx_cnt;    // ���յ��Ĺ㲥֡�ܴ���
    HI_U32 last_rx_time[MAX_LAST_NUM];  // ��ʷ�Ͻ��յ��Ĺ㲥֡ʱ��
    HI_U32 bc_rm_cnt;       // ʹ�ù㲥��ʽ�������
    HI_U32 bc_rm_ok_cnt;    // ʹ�ù㲥��ʽ����ɹ�����
    HI_U32 bc_rm_non_bc_resp_cnt;   // ʹ�ù㲥��ʽ�����յ��ǹ㲥��ʽ��Ӧ����
    HI_U32 bc_rm_fail_cnt;  // ʹ�ù㲥��ʽ����ʧ�ܴ���(��ʱ)
}HI_DSID_APP_PLC_BC_INF_S;    // Ӧ�ò�PLC�㲥֡�շ�ͳ��

typedef struct
{
    HI_U8 curr_num;          // ��ǰ�������
    HI_U8 max_num;           // ���������
    HI_U16 first_id;         // ����ID
    HI_U32 lock_time;        // ��������ʱ��(ms)
    HI_U32 last_lock_time;   // ��һ�ζ�������ʱ��(ms)
    HI_U32 max_lock_time;    // ����������ʱ��(ms)
    HI_U32 queue_full_cnt;   // ����������
    HI_U32 queue_full_time[MAX_LAST_NUM];   // ����������ʱ��
}HI_DSID_APP_RM_QUEUE_INF_S;  // Ӧ�ò���������Ϣͳ��

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U8 afn;  // �����������afn
    HI_U8 fn;   // �����������fn
    HI_U8 type; // ����ԭ��
}REFUSE_CMD_INF;    // ����������Ϣ

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 total_cnt;
    REFUSE_CMD_INF refust_cmd_list[10]; // ���10�η���֡��Ϣ
}HI_DSID_APP_REFUSE_CMD_INF_S;    // ��������ͳ��(376.2����֡)

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 last_time[MAX_LAST_NUM];     // ��ʷ�ı�ʱ��
    HI_U8 chl_status[MAX_LAST_NUM];     // ��ʷͨ��״̬
    HI_U32 change_num;        // ͨ���������
    HI_U32 first_time;        // ��һ�������ɹ�ʱ��
    HI_U32 chl_change_true;   // TRUE����
    HI_U32 chl_change_false;  // FALSE����
    HI_U32 last_true_time[MAX_LAST_NUM];    // ���һ��ͨ�����Ϊ����ʱ��
    HI_U32 last_false_time[MAX_LAST_NUM];   // ���һ��ͨ�����Ϊ������ʱ��
    HI_U32 max_interval;      // ���α仯�����
    HI_U32 min_interval;      // ���α仯��С���
    HI_U32 msg_tx_fail_num;   // ͨ�������Ϣ����ʧ�ܴ���
    HI_U32 msg_fail_time[MAX_LAST_NUM];     // ��ʷ��Ϣ����ʧ��ʱ��
    HI_U32 msg_fail_cause[MAX_LAST_NUM];    // ��ʷ��Ϣ����ʧ��ԭ��
}HI_DSID_APP_TOPO_CHANGE_INF_S;   // Topo�仯ͳ��

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 rx_msg_cnt;          // ���յ��ı����ܴ���
    HI_U32 mag_filter_cnt;      // ���Ĺ����ܴ���
    HI_U32 bc_filter_cnt;       // �㲥���Ĺ����ܴ���
    HI_U32 normal_filter_cnt;   // �ǹ㲥���Ĺ����ܴ���
    HI_U16 id[20];              // ���20�α����˵�id
}HI_DSID_APP_MSG_FILTER_INF_S;    // ���Ĺ���ͳ��

typedef struct
{
    HI_U32 ulVer;
    HI_U32 ulUartTxNum;
    HI_U32 ulUartRxNum;
    HI_U16 usUartTxDenyNum;
    HI_U16 usUartRxDenyNum;
    HI_U16 usUartTimeOutNum;
    HI_U16 usUartRxErrDataNum;
    HI_U32 ulPlcTxNum;
    HI_U32 ulPlcRxNum;
    HI_U16 usPlcRxExceedNum;
    HI_U16 usPlcTxFailNum;
    HI_U32 ulPlcTimeOutNum;
    HI_U32 ulTestUartTxNum;
    HI_U32 ulTestUartRxNum;
    HI_U16 usTestUartTxDenyNum;
    HI_U16 usTestPlcTxFailNum;
    HI_U32 ulTestPlcTxNum;
    HI_U32 ulTestPlcRxNum;
    HI_U32 ulTestPlcTimeOutNum;
    HI_U16 usTestPlcRxExceedNum;
    HI_U16 usTestPlcRxBcNum;
}HI_DSID_APP_RM_RX_TX_INF_S;

typedef struct
{
    HI_U8  ucCurrGetTimeStatus; //�ϵ���ȡʱ��״̬ 0:δ��ȡ;1:�ѻ�ȡ���;2:���ڻ�ȡ
    HI_U8  ucGetTimeSuccFlag;   //�ϵ���ȡʱ���Ƿ�ɹ� 1:�ɹ�,���״̬����ucCurrGetTimeStatus�ж�
    HI_U8  ucReserved[2];
    HI_U8  ucFirstClock[6];
    HI_U8  ucLastClock[6];
    HI_U16 usGetTimeSendCnt;
    HI_U16 usFirstClockFailCnt;
}HI_DSID_APP_CCO_GET_TIME_INF_S;

typedef struct
{
    HI_U8  ucCurrRetry;
    HI_U8  ucJoinCcoQueueMallocFailCnt;
    HI_U16 usCurrItem;
    HI_U8  reserved[3];
    HI_U8  ucCurrAfn;
    HI_U16 usCurrFn;
    HI_U16 usJoinCcoQueueFailCnt;
    HI_U32 ulJoinCcoQueueCnt;
    HI_U32 ulCcoReportCnt;
    HI_U32 ulCcoRxAckCnt;
    HI_U32 ulCcoRxDenyCnt;
    HI_U32 ulCcoReportTimeOutCnt;
}HI_DSID_APP_CCO_REPORT_QUEUE_INF_S;

typedef struct
{
    HI_U32  ulTime;
    HI_BOOL bACDFlg;
    HI_U8   ucJoinCcoQueueFailCnt;
    HI_U8   ucJoinCcoQueueMallocFailCnt;
    HI_U8   ucJoinEvtRepListMallocFailCnt;
    HI_U8   reserved;
    HI_U8   ucEvtListItemNum;  //CCO�ϱ������¼���������Ŀ����
    HI_U16  usEvtQueueItemNum; //CCO������Ŀ����
    HI_U16  ucCcoRxCompatibilityErrCnt;
    HI_U16  usCcoReportTimeoutCnt;//�ܳ�ʱ����ͳ��(�����ط�)
    HI_U32  ulCcoReportEvtCnt;//�ϱ��¼�֡ͳ��(�����ط�)
    HI_U32  ulCcoEvtJoinQueueTimeSec[10]; //��λΪ��
    HI_U32  ulCcoRxAckCnt;
    HI_U32  ulCcoRxEvtPlcCnt;
    HI_U32  ulCcoRxEvtPlcTimeSec[10]; //��λΪ��
    HI_U32  ulCcoTxAckPlcCnt;
    HI_U32  ulCcoFilterPlcCnt;
    HI_U32  ulCcoEvtRxProtoErrCnt;
    HI_U32  ulCcoEvtRxReportDisableCnt;
    HI_U32  ulCcoTxForbidPlcCnt;
    HI_U32  ulCcoTxFullPlcCnt;
    HI_U32  ulCcoTxAllowPlcCnt;
    HI_U32  ulCcoForbidRxEvtCnt;
    HI_U32  ulCcoFilterReportedEvtCnt;
    HI_U32  ulCcoEvtReplaceReportedHeadCnt;
    HI_U32  ulEvtReportedReplaceSameAddrCnt;
    HI_U32  ulEvtReportedAgedCnt;
    HI_U32  ulEvtQueueFront;
    HI_U32  ulEvtQueueRear;
}HI_DFX_APP_CCO_EVT_INF_S;

typedef struct
{
    HI_DFX_APP_CCO_EVT_INF_S stDfxCcoEvtInf[2];
}HI_DSID_APP_CCO_EVT_INF_S;

typedef HI_DSID_APP_CCO_EVT_INF_S HI_DSID_APP_CCO_EVT_BAK_INF_S;

typedef struct
{
    HI_U32 ulTotalSize;
    HI_U32 ulLastIncSize;
    HI_U16 ulIncMallocErrCount;
    HI_U16 ulIncMallocCount;
} HI_DFX_APP_CCO_MEMORY_INC_INF_S;

typedef struct
{
    HI_DFX_APP_CCO_MEMORY_INC_INF_S stDfxCcoMemIncInf[7];
} HI_DSID_APP_CCO_MEMORY_INC_INF_S;

typedef struct
{
    HI_U8 ucCurrRetry;
    HI_U8 ucEvtWaitProcStat;
    HI_U16 usEvtReportSeq;

    HI_U8 ucHardEvtFlg;
    HI_U8 ucCurrEvtFlag;

	HI_U8  ucMeterEvtType;  //0:Ĭ��ֵ  1:���֧�������ϱ�״̬��  2:���֧�������ϱ�״̬��
	HI_U8  ucRespDenyCnt;
    HI_U16 usMeterEvtQueryCnt;
    HI_U16 usMeterEvtRespCnt;

    HI_U16 usEvtPinHighCnt;
    HI_U16 usEvtPinLowCnt;
    HI_U32 ulEvtPinHighTimeStamp[5];
    HI_U32 ulEvtPinLowTimeStamp[5];

    HI_U32 ulPlcEvtReportTotalCnt;
    HI_U32 usPlcEvtReportTimeOutCnt;
    HI_U32 ulPlcEvtReportTimeStamp[10];

    HI_U16 ulPlcEvtAckCnt;
    HI_U16 usPlcEvtFullCnt;
    HI_U32 ulPlcEvtAckTimeStamp[5];
    HI_U32 ulPlcEvtFullTimeStamp[5];
}HI_DSID_APP_STA_EVT_INF_S;

typedef struct
{
    HI_U8   ucCurrStatus;  //��ǰ���״̬ 0-����  1-���ڼ��
	HI_U8   padding[3];
    HI_U16  usMonEvtTimes; //��ص��¼��Ĵ���
	HI_BOOL bWaitPlcFlag;  //�ȴ�PLCͨ�����÷����¼��ϱ�֡��־
    HI_U8  ucPlcQuery645times; //�ȴ�PLC���ú��յ�CCO��ȷ�Ϻ�,����ܽ���Ϊ��,��ֱ�Ӳ�ѯ645֡����
    HI_U32  ulMonEvtTimeStamp[10];//���10�μ�ص��¼���ʱ���
	HI_U32  ulMonEvtStartTimeStamp[10];//���10�μ�ؿ�ʼ��ʱ���
	HI_U32  ulMonEvtEndTimeStamp[10];//���10�μ�ؽ�����ʱ���
}HI_DSID_APP_STA_EVT_MONITOR_INF_S;

typedef struct
{
    HI_U16 usRxPlcLenErrCnt;
    HI_U16 usRxPlcCltPowerOffCnt;
    HI_U16 usCurrPowerOffMeterCnt;
    HI_U16 usFilterPowerOffMeterCnt;
    HI_U8  ucCurrPowerStatus;
    HI_U8  ucCurrPowerOffPlcTxTurn;
    HI_U16 usTxOtherPowerOffPlcCnt;
    HI_U32 ulCltPowerOnPlcTxTime;
    HI_U16 usTxOtherPowerOffPlcTryCnt;
    HI_U8  ucPowerOffOnFlag;
    HI_U8  ucPowerOnReportMeterNum;
    HI_U8  ucCltPowerOnTxPlcTimes;
    HI_U8  pad[7];
}HI_DSID_APP_STA_POWER_ON_OFF_INF_S;

typedef enum
{
    APP_ONE_KEY_DIAG_VER_R0 = 0,
    APP_ONE_KEY_DIAG_VER_R1,
    APP_ONE_KEY_DIAG_VER_R2,
}HI_DSID_APP_DIAG_VER_ENUM;

#define HI_DFX_MRS_SAVE_ENTRY_NUM     (20)

typedef struct
{
    HI_U32  ulMACReportTS;
    HI_U8   aucMac[HI_PLC_MAC_ADDR_LEN];
    HI_U8   ucStateChange;
    HI_U8   ucOfflineReason;
    HI_U32  ulOfflineDuration;
} HI_DFX_MAC_REPORT_ENTRY;

// ��ǰ�ṹ��С380�ֽ�
typedef struct
{
    HI_U32  ulMACReportEntryCnt;
    HI_U32  ulOnlineToOfflineEntryCnt;
    HI_U32  ulOfflineToOnlineEntryCnt;
    HI_U32  ulInFrameQueueEntryNum;
    HI_U32  ulJoinCcoQueueEntryCnt;
    HI_U32  ulFrameCnt;
    HI_U32  ulFrameQueueLen;
    HI_U32  ulJoinCcoQueueFrameCnt;
    HI_U32  ulInCcoQueueFrameNum;
    HI_U32  ulReportRxRespFrameCnt;
    HI_U32  ulReportTimeoutFrameCnt;
    HI_U32  ulJoinCcoQueueLTS;
    HI_U32  ulReportRxRespLTS;
    HI_U32  ulReportTimeoutLTS;
    HI_U32  ulInCcoQueueEntryNum;
    HI_DFX_MAC_REPORT_ENTRY  astEntry[HI_DFX_MRS_SAVE_ENTRY_NUM];
} HI_DSID_APP_CCO_NODE_STATECHG_INF_S;

typedef struct
{
	HI_U8 ucEnable;
	HI_U8 ucDetectStatus;//0-���� 1-���������δ���� 2-̽�Ⳮ����
	HI_U8 ucJoinWhiteFlag; // 0-��������������  1-������������
	HI_U8 ucAllowJoinWhiteReason;//0-Ĭ�� 1-�Ż���� 2-20����һֱ���������� 3-����������״̬,�ⲿȥʹ�ܺ�ָ�������״̬
	HI_U32 ulAllowJoinWhiteTime;//��������������ʱ���
	HI_U32 ulForbidJoinWhiteTime;//��ֹ�����������ʱ���
	HI_U8 ucForbidJoinWhiteReason;//������״̬��Ϊ��ֹ��ԭ�� 0-Ĭ�� 1-plcͨ�������� 2-�ⲿȥʹ��
	HI_U8 ucSetWaitTopoFlag; // 0-δ֪  1-��Ҫ�ڱ���֪ͨMACʱ����20���ӵȴ�վ������,�����ɿձ�Ϊ�ǿ�ʱ�����øñ�־ 2-���������֪ͨMACʱ����,֮�����һ������ϸñ�־����Ϊ1
	HI_U8 ucRmFlag; //0-����̽��͸������ 1-����̽��͸��,�ѷ���͸��֡
	HI_U8 ucExternFlag; //�ⲿ����̽�Ⳮ�����ʹ�� 0-�ⲿ�޲���  1-�ⲿ�в���
	HI_U8 ucOriginalEnable; //�ⲿʹ��ǰ��ԭʼ����
	HI_U8 ucCurrTurn;//��ǰ̽�Ⳮ������
	HI_U16 usDetectWhiteNum;//̽�������׷�ӵİ���������
	HI_U8 ucTopoStatus;
	HI_U8 uc97DIIndex;//��ǰ����97���DIָ��
	HI_U8 reserved;
    HI_U8 ucDetectEndReason[5];
	HI_U32 ulDetectStartTime[5];//���5��̽����ƿ�ʼ��ʱ��
	HI_U32 ulDetectEndTime[5];//���5��̽����ƽ�����ʱ��
	HI_U32 ulEvalutedTime;  //�����Ż����ʱ��
	HI_U32 ulPlcDisconnectTime;  //Plcͨ����Ϊ������ʱ��
	HI_U32 ulArchivesClearTime;  //�����������յ�ʱ��
	HI_U32 ulArchives0To1Time;  //�����ɿձ�Ϊ�ǿյ�ʱ��
	HI_U32 ulExternDisableTime; //�ⲿȥʹ��̽�⿪��ʱ��
	HI_U32 ulExternResumeTime; //�ⲿ�ָ�ʹ��̽�⿪��ʱ��
	HI_U16 usBlackListNum;  //����������
	HI_U16 usDetectWhiteValidNum;//̽�������׷�ӵİ�������Ч����
	HI_U8 ucRefreshWhiteFlag; //ˢ�°�������ʱ����ر�־
	HI_U8  reserved2[15];
}HI_DSID_APP_CCO_DETECT_RM_INF_S;

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U16 total_num;   // ��ǰ�ڵ�������
    HI_U32 modify_cnt;  // ���޸Ĵ���
    HI_U32 add_cnt;  // ��Ӵ���
    HI_U32 del_cnt;  // ɾ������
    HI_U32 init_cnt;  // ��ʼ������
}HI_DSID_APP_MREC_STATUS_S;   // ��ǰ����״̬

typedef struct
{
    HI_U8 mac[6];
    HI_U8 padding[2];
}HI_DSID_APP_MAC_LIST_CONT_S;   // MacList����

//MAC��ַ��Ϣ
typedef struct
{
    HI_U8   ucMacAddr[HI_PLC_MAC_ADDR_LEN];   //MAC��ַ,6�ֽڣ�������ǰ��
    HI_U8   report;         //�Ѿ��ϱ�������
    HI_U8   retries;        //���Դ���
}DFX_MRS_MAC_INFO;

//�����Ϣ
typedef struct
{
    HI_U8   ucMeterAddr[HI_METER_ADDR_LEN];   //����ַ,6�ֽ�
    HI_U8   ucProtocol;    //��Լ����
    HI_U8   ucDeviceType;   //�豸����
}DFX_MRS_METER_INFO;

typedef struct
{
    HI_U16          ttl;        //ӳ���ϵTTL
    HI_U8           report;     //�ϱ���־
    HI_U8           lock;       //XXX
    DFX_MRS_MAC_INFO    mac;        //MAC
    DFX_MRS_METER_INFO  meter;      //�����Ϣ
}HI_DSID_APP_MAC_MAP_CONT_S;    // Macӳ�������

typedef struct
{
    HI_BOOL bValidFlg;    // ��Ч��־ HI_TRUE : �˴ӽڵ���Ч HI_FALSE : �˴ӽڵ���Ч
    HI_U8 ucProtocolType; // ��Լ���� 00:����(δ֪) 01:97-645 02:07-645 ����:δ֪
    HI_U16 usNodeSeq;    // �����дӽڵ�(���)���
    HI_U8 ucNodeAddr[6]; // �ӽڵ��ַ(����ַ)
    HI_BOOL bSaveValidFlg; //�ڵ���Ϣ������Ч��־(HI_TRUE:�ýڵ���ϢӦ�ô��� HI_FALSE:�ýڵ���Ϣ��Ӧ�ô���)
    HI_U8 ucCheckSum;             // У����
}HI_DSID_APP_MREC_INF_S;    // ��������

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 ulSign;
    HI_U32 ulOption;    //����ѡ��
    HI_U32 ulUpgId;     //����ID
    HI_U32 ulFileLen;   //�����ļ��ĳ���
    HI_U32 ulBlkNum;    //���512�ֽ�Ϊһ����λ
    HI_U32 ulUpgTimeWindow; //����ʱ�䴰
    HI_U32 ulLastBlockId;   //��һ��BLOCK��ID
    HI_U32 bWaitMaxUpgTime;
}HI_DSID_APP_UPG_CONTINUS_START_NV_S;

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 ulSign;
    HI_U32 ulSign_2;
    HI_U32 ulSign_3;
    HI_U32 entry_timestamp;
    HI_U32 notify_timestamp;
    HI_U32 from;
}HI_DSID_APP_UPG_CONTINUS_TOPO_NOTIFY_INFO_S;

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 ulClearMrsUpgTime; //MRS�յ����һ�μ����������װ����ʱ��ʱ��
    HI_U32 ulMrsUpgStartTime; //MRS���һ�ο�ʼ��������������ȷ�ĵ�һ֡��װ�ļ�����ʱ��
    HI_U32 ulMrsUpgEndTime;   //MRSģ����������ʱ�䣬����������ɽ���������ԭ���µ���������
    HI_U8  ucMrsRxClearUpgStatus; //MRSģ����յ������װ����ʱ��״̬
    HI_U8 ucMrsUpgStatus;  //MRSģ��Զ������״̬
    HI_U16 usMrsFrameErrFlag;  //MRSģ������װ�����н��յ��ı����д���ı�ʶ
    HI_U8 ucMrsRxErrUpgStatus;//MRSģ���յ�������״̬���Ƕ��ο����ӿ��е�4��ֵ
    HI_U8 ucMrsReleaseMemFail;//MRSģ���ͷ������ļ��ڴ�ʧ�ܵı�־
    HI_U8 ucStartUpgRxStopUpg;//������������ʱ�յ�ֹͣ�����������
    HI_U8 ucFailedReason;   // ����ʧ��ԭ��
    HI_U8 ucMrsGetUpgMomeryFail; //MRSģ���ȡ�����ļ��ڴ�ʧ�ܵı�־
    HI_U8 ucUpgFileType; //�����ļ�����
    HI_U8 ucUpgStartRet; //MRSģ����UPGģ�鷢�͵Ŀ�ʼ�������
    HI_U8 ucUpgStopRet;  //MRSģ����UOGģ�鷢�͵�ֹͣ�������
    HI_U32 ulMrsUpgLenErrSize; //MRSģ��������װ�����������ļ����ȴ����ֵ
    HI_U16 usMrsRxUpgFrameNum; //����������MRSģ����յ�����������ȷ֡��
    HI_U16 usMrsTxUpgFrameNum; //����������MRSģ�鷢�͸���������֡��
    HI_U32 ulUpgStartTime; //MRSģ����UPGģ�鷢�͵Ŀ�ʼ����ʱ��
    HI_U32 ulUpgStopTime;  //MRSģ����UPGģ�鷢�͵�ֹͣ����ʱ��
    HI_U32 ulMrsMsgRxStartTime; //Mrs���տ�ʼ����Ӧ��֡��ʱ���
    HI_U32 ulMrsMsgRxStartRet;  //Mrs���տ�ʼ����Ӧ��֡�ķ���ֵ
    HI_U16 usMrsMsgRxTotalTimes; //Զ������������MRSģ����յ���ȷ��Ϣ�ܴ���
    HI_U16 usMrsMsgTxMrsTimes; //Զ������������MRSģ�鷢�͸�MRS�Լ�����Ϣ����
    HI_U16 usMsgTxUpgTimes; //Զ������������MRSģ�鷢�͸�UPGģ�����Ϣ����
    HI_U16 usMsgRxUpgTimes; //Զ������������MRSģ����յ�UPGģ�����Ϣ����
    HI_U16 usMsgRxUpgIdErrTimes; //Զ������������MRSģ����յ�UPGģ�����Ϣ�ж���ID������ID������4�Ĵ�����
    HI_U8 ucMsgTxDrvTimes; //Զ������������MRSģ�鷢�͸�DRV_UPGģ�����Ϣ����
    HI_U8 ucMsgRxDrvTimes; //Զ������������MRSģ����յ�DRV_UPGģ�����Ϣ����
    HI_U16 usMsgTxFailId;//Զ������������MRSģ�鷢�͵�ʧ����ϢID
    HI_U8 ucMsgTxFailNum;//Զ������������MRSģ�鷢�͵�ʧ����Ϣ����
    HI_U8 usMrsMsgRxIdErrTimes;//Զ������������MRSģ����յ�����ϢID��������ֹͣ������״̬���ļ����ID�Ĵ�����
    HI_U32 pMsgFailRet[4];//���4����Ϣ����ʧ�ܵķ���ֵ
    HI_U32 ulIndRxStatus;
    HI_U32 ulIndUpgId;
    HI_U32 ulUpgId;
    HI_U32 ulIndUpgTs;
    HI_U8 ucPeriod;
    HI_U8 ucReserved3[3];
}HI_DSID_APP_REMOTE_UPG_INF_S;

typedef struct
{
    HI_U32 ulSmValidFrame;
    HI_U32 ulSmInvalidFrame;
    HI_U32 ulMapValidFrame;
    HI_U32 ulMapInvalidFrame;
    HI_U32 reserved[12];
} HI_DSID_APP_PLC_FRAME_STAT_INF_S;

typedef struct
{
    HI_U16 ucPlcRxLenErrCnt;
    HI_U16 usTxAckRxPowerOnCnt;
    HI_U16 usRxPowerOffCnt;
    HI_U16 usCurrPowerOnMeterCnt;
    HI_U16 usCurrPowerOffMeterCnt;
    HI_U16 usRxFilterPowerOffMeterCnt;
    HI_U16 usRxFilterPowerOnMeterCnt;
    HI_U16 usReportPowerOnMeterCnt;
    HI_U16 usReportPowerOffMeterCnt;
    HI_U16 usReportPowerOnFrameCnt;
    HI_U16 usReportPowerOffFrameCnt;
    HI_U8  pad[6];
}HI_DSID_APP_CCO_POWER_ON_OFF_INF_S;

// EQTͳ����
typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 timestamp;
    HI_U32 eqt_timer_cnt[4]; // [0] ָ������Timer ����ִ����Ϻ�ֹͣtimer���� [1] Reset timer cnt. [2] stop timer counter
    HI_U32 eqt_timer_val[4]; // [0] max counter of timer [1] execute cnt before reset timer. [2] execute cnt before stop timer [3] execute cnt in timer call-back
    HI_U32 eqt_eqx_port_reset[4]; // [0] �˿ڸı��ܴ���, [1]-[3] UART �ڸı����, ����EQMCͨ�� �ɹ�,ʧ��,�ڴ治��,�����ܴ��� ��¼,
    HI_U32 eqt_eqx_port_reset_val[6];
    HI_U32 hso_chl_rx_ok_cnt;  // �ɹ�����HSO���ĵĸ���
    HI_U32 hso_chl_rx_chksum_err_cnt; // CRCУ������HSO���ĸ���
    HI_U32 hso_chl_rx_id_err_cnt; // ��֧�ֵ�HSO���ĸ���
    HI_U32 hso_chl_sndtask_cnt[2]; // ����HSO���ĸ�������ɹ�/ʧ�ܸ���
    HI_U32 hso_chl_sndtask_nomem_err_cnt;
    HI_U32 hso_chl_not_find_req_proc_cnt;
    HI_U32 hso_chl_sndport_err_cnt; // ���͸�HSO�������
    HI_U32 snd_ecmsg_cnt[2];          // ���͸���������ɹ��ʹ���ļ���
    HI_U32 eqt_ecmsg_proc_exec_cnt;   // ec msg �ܴ������ִ�д���
    HI_U32 eqt_hsoreq_proc_exec_cnt;  // hso req �ܴ������ִ�д���
    HI_U32 eqt_eqmc_rcv_cnt[4];       // ����EQMCͨ�����ݼ��� [3]�����ܴ���
    HI_U32 eqt_eqmc_snd_cnt[4];       // ����EQMCͨ�� �ɹ�,ʧ��,�ڴ治��,�����ܴ��� ��¼,
    HI_U32 eqt_eqmc_rcv_len[4];       //
    HI_U32 eqt_mc_parse_err_cnt;
} HI_DSID_APP_EQT_CHL_STAT_S;


#define NETWORK_NOTIFY_REC_TIMES        (5)
#define HI_UPG_PROC_DETAIL_CNT          (3)
#define UPG_CTX_STATUS_CHANGE_REC_TIMES (5)
#define UPG_CCO_REC_SND_QUERY_CNT       (3) // ��¼CCO���Ͳ�ѯ�Ĵ���
#define UPG_CCO_REC_QUERY_EXC_STA_CNT   (3) //CCO��¼��ѯ�쳣վ�����
#define UPG_STA_REC_REPLY_STATUS_CNT    (3) //STA��¼Ӧ��״̬��ѯ����
#define HI_DSID_APP_UPG_UC_CHECK_S     HI_DSID_APP_UPG_CHECK_S
#define HI_DSID_APP_UPG_U2B_CHECK_S    HI_DSID_APP_UPG_CHECK_S
#define HI_DSID_APP_UPG_U2B_SND_INFO_S HI_DSID_APP_UPG_SND_INFO_S
#define HI_DSID_APP_UPG_UC_SND_INFO_S  HI_DSID_APP_UPG_SND_INFO_S


typedef struct
{
    HI_U32 formed_sta_cnt;
    HI_U32 formed_LTS;
} HI_UPG_NETWORK_FORMED_STRU;


typedef struct
{
    HI_U8  proc_mac[HI_PLC_MAC_ADDR_LEN];
    HI_U16 snd_status_cnt;
    HI_U16 snd_data_LSN;
    HI_U16 rcv_all_status_reply;
} HI_UPG_U2B_PROC_DETAIL;


typedef struct
{
    HI_U8  proc_mac[HI_PLC_MAC_ADDR_LEN];
    HI_U16 snd_status_cnt;
    HI_U16 snd_data_LSN;
    HI_U16 rcv_status_reply;
} HI_UPG_UC_PROC_DETAIL;


typedef struct
{
    HI_U32 changed_LTS;
    HI_U8 upg_ctx_status;
    HI_U8 upg_status;
    HI_U8 from;
    HI_U8 reserved;
} HI_UPG_CTX_STATUS_CHANGE_STRU;

typedef struct
{
    HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN]; //����Ϊ����״̬�쳣վ��MAC��ַ
    HI_U16 reserved;
    HI_U32 ulSetExceptionTimeStamp; //վ�㱻��Ϊ����״̬�쳣ʱ���
}HI_UPG_CCO_SET_QUERY_EXCEPTION_INFO;

typedef struct
{
    HI_U16 usSetQueryExceptionCnt; //����Ϊ�����쳣�ܵ�վ�����
    HI_U16 reserved;
    HI_UPG_CCO_SET_QUERY_EXCEPTION_INFO struDetailClient[UPG_CCO_REC_QUERY_EXC_STA_CNT]; //���3�α���Ϊ�����쳣վ����Ϣ
}HI_UPG_CCO_QUERY_EXCEPTION_STRU;

typedef struct
{
    HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN]; //���Ͳ�ѯվ���MAC��ַ
    HI_U16 reserved;
    HI_U32 ulSndTimeStamp; //���Ͳ�ѯ��ʱ���
    HI_U32 ulSndRet;       //����״̬��ѯ�ķ���ֵ
}HI_UPG_CCO_QUERY_CLIENT_INFO;

typedef struct
{
    HI_U16 sta_complete_cnt;
    HI_U8  mac_LTS[HI_PLC_MAC_ADDR_LEN];
} HI_UPG_RCV_STA_COMPLETED_STRU;

typedef struct
{
    HI_U16  state_completed_cnt;
    HI_U16  state_ready_cnt;
    HI_U16  state_upgrading_cnt;
    HI_U16  state_exception_cnt;
}HI_UPG_CLIENT_STATE_INFO;

typedef struct
{
   HI_UPG_CLIENT_STATE_INFO stClientState;//�ж����������Ƿ�ִ����ʱ����Ϣ�б��и�״̬վ�����
   HI_U32 ulTimeStamp;            //�ж����������Ƿ�ִ�����ʱ���
}HI_UPG_CHECK_COMPLETE_INFO;//12�ֽ�

typedef struct
{
    HI_U16 usTei;//վ���TEI
    HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN]; //վ��MAC��ַ
}HI_UPG_STA_INFO;//8�ֽ�

typedef struct
{
    HI_UPG_STA_INFO stProcessItem;//վ��MAC��TEI��Ϣ
    HI_U32 ulTimeStamp; //ʱ���
}HI_UPG_ITEM_DETAIL; //12�ֽ�

typedef struct
{
    HI_U16 usTimeStamp; //�����ϵ�����ǰ��ʱ�ȴ���ʱ���
    HI_U8 ucTimerCnt;//��ǰ��������ʱ������
    HI_U8 reserved;
}HI_UPG_BSP_WAIT_INFO;//�ϵ�����ǰ��������ʱ������ʱ������

typedef struct
{
    HI_UPG_STA_INFO stProxyItem;//����ڵ���Ϣ
    HI_UPG_STA_INFO stItem;//�ӽڵ���Ϣ
    HI_U32 ulTimeStamp;
}HI_UPG_U2B_PROCESS_TABLE_ITEM;//20�ֽ�

typedef struct
{
   HI_U16 usSndTotalLoopCnt;//���͵�������
   HI_U16 usSndCntLatestLoop; //���һ�ַ��ͳɹ��Ĵ���
   HI_U16 usSndFullCntLatestLoop; //���һ�ַ��Ͷ������Ĵ���
   HI_U16 usSndFailCntLatestLoop; //���һ�ַ���ʧ�ܵĴ���(�ǳɹ����Ƕ�����)

   HI_U32 ulSndLoopStartTimeStamp; //���һ�ַ��Ϳ�ʼʱ���
   HI_U32 ulSndLoopEndTimeStamp;//���һ�ַ��ͽ�����ʱ���
}HI_UPG_SND_INFO;//���Է�����Ϣ 16�ֽ�

typedef struct
{
    HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN];//վ���MAC��ַ
    HI_U16 usSndPacketCnt;//�������ݰ��ĸ���
    HI_U32 ulTimeStamp;//ʱ���
}HI_UPG_PROCESS_ITEM_SND_INFO;//12�ֽ�

typedef struct
{
    HI_U32 ulRcvFileSize; //վ��Ӧ���ѯʱ�ϱ��Ľ��������ļ���С
    HI_U32 ulReplyTimeStamp; //վ��Ӧ��״̬��ѯ��ʱ���
    HI_U32 ulReplyRet;       //վ��Ӧ��״̬��ѯ�ķ���ֵ
}HI_UPG_STA_REPLY_QUERY_INFO;


typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U16 rcv_blk_cnt;         // �ѽ�����������֡����[����]ok
    HI_U16 rcv_blk_LSN;         // ���һ�ν������ݵ�˳���[����]ok
    HI_U32 rcv_blk_LTS;         // ���һ�ν������ݵ�ʱ���[����]ok

    HI_U16 snd_blk_cnt;          //ʵ����Ч�������ݱ�������ok
    HI_U16 snd_blk_LSN;          //���һ�η����������ݵ����ok
    HI_U16 snd_blk_fail_cnt;     //��������ʱ����fail�Ĵ���ok
    HI_U16 snd_blk_full_cnt;     //��������ʱ����full�Ĵ���ok
    HI_U32 snd_blk_LTS;          //���һ�η����������ݳɹ���ʱ���ok

    HI_U16 snd_status_cnt;       //����״̬��ѯ�Ĵ���ok
    HI_U16 snd_status_fail_cnt;  //����״̬��ѯʧ�ܴ���ok
    HI_U16 snd_status_full_cnt;  //����״̬��ѯ���������Ĵ���ok
    HI_U8  snd_status_mac[HI_PLC_MAC_ADDR_LEN];   //��ǰ����״̬��ѯվ���MAC��ַ ok
    HI_U32 snd_status_LTS;       //���һ�η���״̬��ѯ��ʱ���ok
    HI_U16 rcv_status_reply_cnt; //���յ�״̬��ѯ�ظ��Ĵ���,��������������ok
    HI_U16 rcv_status_reply_discard_cnt; //���յ�״̬��ѯ�ظ��������� ok
    HI_U32 rcv_status_LTS; //���һ�γɹ�������յ�վ���ѯ�ظ���ʱ���
    HI_U16 rcv_status_report_cnt; //CCO���յ�STA�����ϱ����ܴ���
    HI_U16 snd_affirm_cnt;       //�������֪ͨ�Ĵ��� ok
    HI_U16 snd_affirm_fail_cnt;  //�������֪ͨʧ�ܵĴ��� ok
    HI_U16 snd_affirm_full_cnt;  //�������֪ͨ���������Ĵ��� ok
    HI_U32 snd_affirm_LTS;       //���һ�η������֪ͨ��ʱ��� ok

    HI_U32 snd_stop_LTS;         //���һ�η���ֹͣ������ʱ���ok
    HI_U32 snd_stop_req_upg_id; //���һ�η���ֹͣ����ʱֹͣ����֡��UPG ID ok
    HI_U16 snd_stop_cnt;         //����ֹͣ�����Ĵ���ok
    HI_U16 snd_stop_fail_cnt;    //����ֹͣ����ʧ�ܵĴ���ok
    HI_U16 snd_stop_full_cnt;    //����ֹͣ�������������Ĵ���ok

    HI_U16 snd_start_cnt;        //���Ϳ�ʼ�����Ĵ���ok
    HI_U16 snd_start_fail_cnt;   //���Ϳ�ʼ����ʧ�ܵĴ���ok
    HI_U16 snd_start_full_cnt;   //���Ϳ�ʼ�������������Ĵ���ok
    HI_U32 snd_start_LTS;        //���һ�η��Ϳ�ʼ������ʱ���ok
    HI_U16 stat_rcv_reply_start_discard_cnt; //�ظ���ʼ������������ OK
    HI_U16 stat_rcv_reply_start_cnt;      //��ǰ�����ظ���ʼ�����Ĵ������������������� ok
    HI_U32 stat_rcv_reply_start_LTS; //���һ�γɹ�������յ���START֡ʱ��� ok

    HI_U32 upg_finish_TS;        //ȫ����������ʱ���
} HI_DSID_APP_UPG_CCO_STAT_S;//(0xa502)


//cco�������ͳ����
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U32 bc_entry_LTS;             //�㲥���Խ���ʱ���ok
    HI_U32 u2b_entry_LTS;            //U2B���Խ���ʱ���ok
    HI_U32 uc_entry_LTS;             //�������Խ���ʱ���ok

    HI_U16 bc_loop;                  //�㲥�����ѷ�������ok
    HI_U16 u2b_proc_cnt;             //U2B�����Ѵ���������ok
    HI_U16 uc_proc_cnt;              //���������Ѵ���վ�����ok
    HI_U8 ucFinishTimerCnt;      //�����½���ǰ����ʱ���ĸ���
    HI_U8 ucPlcMode;             //PLCģʽ

    HI_UPG_U2B_PROC_DETAIL u2b_proc_detail[HI_UPG_PROC_DETAIL_CNT]; //u2b���Ե�ǰ�����ϸ�����ݣ���¼���3·����ok
    HI_UPG_UC_PROC_DETAIL  uc_proc_detail[HI_UPG_PROC_DETAIL_CNT];  //�������Ե�ǰ�����ϸ�����ݣ���¼���3·����ok
} HI_DSID_APP_UPG_CCO_STAT1_S;//(0xa504)


//ccoͨ����ȫ��״̬���ͳ����
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U16 stat_proc_cnt;        //��ǰ���������еĸ���ok
    HI_U16 stat_verfiy_cnt;      //��ǰ��֤�еĸ���ok
    HI_U16 stat_succ_cnt;        //��ǰ�ɹ��ĸ���ok
    HI_U16 stat_fail_cnt;        //��ǰʧ�ܵĸ���ok

    HI_U32 network_notify_times;     //ͨ���仯���� ok
    HI_UPG_NETWORK_FORMED_STRU network_notify_st[NETWORK_NOTIFY_REC_TIMES];//���5��ͨ���仯��� ok
    HI_UPG_CTX_STATUS_CHANGE_STRU upg_ctx_change[UPG_CTX_STATUS_CHANGE_REC_TIMES]; //ȫ��״̬�����¼ ok
} HI_DSID_APP_UPG_CCO_STAT2_S;//(0xa505)

//ȫ������CCO���Ͳ�ѯ����վ������״̬�쳣��Ϣ
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_UPG_CCO_QUERY_CLIENT_INFO struCcoQueryClientDetail[UPG_CCO_REC_SND_QUERY_CNT]; //CCO���3�η��Ͳ�ѯ��Ϣ
    HI_UPG_CCO_QUERY_EXCEPTION_STRU struCcoSetExceDetail; //CCO����վ������״̬�쳣��Ϣ

}HI_DSID_APP_UPG_CCO_STAT3_S;//96�ֽ�(0xa507)

typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_UPG_RCV_STA_COMPLETED_STRU bcRcvCompleteDetail;//�㲥���Խ��յ�վ���������վ�����
    HI_UPG_RCV_STA_COMPLETED_STRU u2bRcvCompleteDetail;//U2B���Խ��յ�վ���������վ�����
    HI_UPG_RCV_STA_COMPLETED_STRU ucRcvCompleteDetail;//�������Խ��յ�վ���������վ�����
}HI_DSID_APP_UPG_CCO_STAT4_S;//32�ֽ�(0xa509)

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_UPG_CHECK_COMPLETE_INFO stCheckInfo[3];//վ��״̬��Ϣ

    HI_UPG_ITEM_DETAIL stBcAddProcessItemInfo;//�㲥������Ӵ����վ����Ϣ
    HI_UPG_ITEM_DETAIL stBcRemoveProcessItemInfo;//�㲥�����Ƴ������վ����Ϣ
    HI_UPG_BSP_WAIT_INFO stBspWaitInfo; //�ϵ�����ǰ��������ʱ��ʱ��������Ϣ
    HI_U8 ucLargeProxy;              //�Ƿ���ڴ����:55���ڴ����22�����ڴ������Լ�����
    HI_U8 ucLargeProxyMac[3];
    HI_U16 usQueryMaxException;      //��ѯ�ﵽ����������Ϊ����״̬�쳣վ�����
    HI_U16 usUnknownStatusException; //����״̬δ֪����Ϊ����״̬�쳣վ�����
    HI_U16 usU2BDataMaxException;    //U2B���������ﵽ���ޱ���Ϊ����״̬�쳣վ�����
    HI_U16 usUCDataMaxException;     //UC���������ﵽ���ޱ���Ϊ����״̬�쳣վ�����
    HI_U16 usStatusReplyDiscardPara; //������β��Զ���״̬��ѯӦ��
    HI_U16 usStatusReplyDiscardUpgId; //��������ID���Զ���״̬��ѯӦ��
    HI_U32 ulVerifyRet; //�����ļ�У����
    HI_U32 ulWriteRet;  //�����ļ�дFlash���
}HI_DSID_APP_UPG_CCO_STAT5_S; //�㲥���Դ������վ�����ͳ���� 96�ֽ�(0xa50a)

typedef struct
{
    HI_UPG_SND_INFO stSndStartInfo; //�������Ϳ�ʼ����֡����Ϣ
    HI_UPG_ITEM_DETAIL stLastSndStartClientInfo;//���һ�η��Ϳ�ʼ����֡�ɹ���վ����Ϣ

    HI_UPG_SND_INFO stSndStatusInfo;//�������Ͳ�ѯ֡����Ϣ
    HI_UPG_ITEM_DETAIL stLastSndStatusClientInfo;//���һ�η���״̬��ѯ֡��վ����Ϣ

    HI_UPG_SND_INFO stSndBlkInfo;//��������֡����Ϣ
}HI_DSID_APP_UPG_SND_INFO_S;//�������Է�����Ϣ 72�ֽ�(0xa50e/0xa511)

typedef struct
{
    HI_U32 ulFileCrc;
    HI_U32 ulFileLen;
    HI_U32 ulTxPlcCnt;
    HI_U32 ulRxPlcCnt;
    HI_U32 ulLastTxTime; //��λ:����
    HI_U32 ulLastRxTime; //��λ:����
    HI_U16 usTopoNum;
    HI_U16 usCurrent;
    HI_U16 usTxPlcFailCnt;
    HI_U16 usRxPlcErrCnt;
    HI_U32 ulFinishTime;
    HI_U16 usResultSucCnt;
    HI_U16 usResultFailCnt;
    HI_U8 ucStatus;
    HI_U8 ucType;
    HI_U8 file_crc_from;
    HI_U8 write_backup_flash : 1; /* �Ƿ�д������FLASH */
    HI_U8 chip_mode : 2; /* оƬģʽ */
    HI_U8 reserved : 5;
}HI_DSID_APP_UPG_CCO_QUERY_INFO_S;//(0xa512)

typedef struct
{
    HI_U32 ulOption;
    HI_U32 ulFileCrc;
    HI_U32 ulFileLen;
    HI_U32 ulRxInfoPlcTime[2]; //��λ:����
    HI_U32 ulTxInfoPlcTime[2]; //��λ:����
    HI_U32 ulTxPlcCnt;
    HI_U32 ulRxPlcCnt;
    HI_U8 ucRxPlcErrCnt;
    HI_U8 ucTxPlcFailCnt;
    HI_U8 file_crc_from;
    HI_U8 reserved[5];
}HI_DSID_APP_UPG_STA_QUERY_INFO_S;//(0xa513)

typedef struct
{
    HI_U8  ucUpgStatus;
    HI_U8  ucEndReason; //0:Ĭ��ֵ 1:�ɹ� 2:��ʱ 3:UPG��ѯ��mrs״̬Ϊʧ�� 4:UPG��ѯ��mrs״̬Ϊ����
    HI_U16 usErrCode;
    HI_U8  ucSendStartCnt;
    HI_U8  ucStartRespCnt;
    HI_U8  ucSendStopCnt;
    HI_U8  ucStopRespCnt;
    HI_U32 ulSendStatusCnt;
    HI_U32 ulStatusRespCnt;
    HI_U8  ucRxStatus;
    HI_U8  ucReserved[3];
}HI_DSID_APP_UPG_STA_DEVICE_INFO_S;//(0xa514)

typedef struct
{
    HI_U16 usRcvStaNum;//�ѽ���STA�ܸ���
    HI_U16 usTotalStaNum;   //STA�ܸ���
    HI_U8 ucRcvPacket;      //�ѽ���֡����
    HI_U8 ucTotalPacket;    //֡�ܸ���
    HI_U8 bCcoInList : 1;   //�б����Ƿ����CCO
    HI_U8 bStaInList : 1;   //�б����Ƿ������CCO���������
    HI_U8 bOptionMatchList : 1;//����ѡ�����б��Ƿ�ƥ��
    HI_U8 ulUpgOption : 2; //CCO����STA��ʽ
    HI_U8 pad1 : 3;
    HI_U8 user_proto_type;//�û�Э������
    HI_U32 ulTimerPeriod; //��ʱ������
    HI_U32 ulStartRcvTimerTimeStamp;   //�������ļ���ʱ��ʱ��ʱ���
    HI_U32 ulActRcvTimerTimeStamp;    //��Ӧ�����ļ���ʱ��ʱ��ʱ���
    HI_U32 ulStartListTimerTimeStamp; //�������б�ʱ��ʱ��ʱ���
    HI_U32 ulActListTimerTimeStamp;   //��Ӧ�����б�ʱ��ʱ��ʱ���
    HI_U32 ulListMemSize;  //�б��ڴ��С
    HI_U32 ulReleaseMemTimeStamp; //�ͷ��б��ڴ�ʱ���
    HI_U32 ulWriteListRet; //��Listд��FLASH������
    HI_U32 ulGetListRet;   //��FLASH�л�ȡ�б������
    HI_U32 ulAllStaInTopoTimeStamp;  //����STA����ʱ���
    HI_U16 usStatusNotRight; //STA�ϱ���bitmapΪȫFF,״̬������Ԥ�ڸ���
    HI_U16 pad4;
    HI_U32 set_user_proto_timeStamp;//�����û�Э��ʱ���
    HI_U32 pad3;
}HI_DSID_APP_UPG_LIST_INFO_S;//(0xa515)

typedef struct
{
    HI_UPG_SND_INFO stSndStartBcInfo; //�㲥�����Ϳ�ʼ������Ϣ
    HI_DSID_APP_UPG_SND_INFO_S stSndInfo; //�㲥���Է�����Ϣ
    HI_U16 usBcNeedSndBlkCntLatestLoop;//���һ����Ҫ�������ݰ��ĸ���:����һ�η�����������ǰ��ʼǰ���
    HI_U16 usProtoVerFromMac;
}HI_DSID_APP_UPG_BC_SND_INFO_S;//�㲥���Է���ͳ����Ϣ 92�ֽ�(0xa50b)

typedef struct
{
    HI_UPG_CHECK_COMPLETE_INFO stCheckInfo[3];//վ��״̬��Ϣ
    HI_UPG_PROCESS_ITEM_SND_INFO stProcessItemSndInfo[HI_UPG_PROC_DETAIL_CNT];//��������3��վ����Ҫ�������ݰ���Ϣ :����һ�η�����������ǰ��ʼǰ���
}HI_DSID_APP_UPG_CHECK_S;//�������վ�����ͳ���� 72�ֽ� (0xa50c/0xa50f)

typedef struct
{
    HI_UPG_U2B_PROCESS_TABLE_ITEM stU2bAddProcessItemInfo[HI_UPG_PROC_DETAIL_CNT]; //U2B�������3����Ӵ����վ����Ϣ ok
    HI_UPG_ITEM_DETAIL stU2bRemoveProcessItemInfo[HI_UPG_PROC_DETAIL_CNT];  //U2B�������3���Ƴ������վ����Ϣ ok
}HI_DSID_APP_UPG_U2B_PROCESS_ITEM_S;//U2b��������Ƴ��������Ϣ 96�ֽ�(0xa50d)

typedef struct
{
    HI_UPG_ITEM_DETAIL stUcAddProcessItemInfo[HI_UPG_PROC_DETAIL_CNT];//�����������3����Ӵ����վ����Ϣ ok
    HI_UPG_ITEM_DETAIL stUcRemoveProcessItemInfo[HI_UPG_PROC_DETAIL_CNT];//�����������3���Ƴ������վ����Ϣ ok
    HI_U32 obtain_cache_time;
    HI_U32 obtain_cache_addr;
    HI_U32 obtain_cache_size;
    HI_U32 release_cache_time;
    HI_U32 release_cache_addr;
    HI_U32 reserved;    
}HI_DSID_APP_UPG_UC_PROCESS_ITEM_S;//������������Ƴ��������Ϣ 96�ֽ�(0xa510)

typedef struct
{
    HI_U8 ucRegCbCnt;
    HI_U8 ucRcvStartReqCnt;   //���տ�ʼ������Ϣ����
    HI_U8 ucCbStartCnt;       //���ÿ�ʼ�����ص�����
    HI_U8 reg_protocol_type;  //�û�ע���Э������
    HI_U32 ulLastRcvStartTime;
    HI_U32 ulLastCbStartTime;

    HI_U8 ucRcvStatusReqCnt;  //��������״̬��ѯ��Ϣ����
    HI_U8 ucCbBsStatusCnt;   //���û�������״̬�ص�����
    HI_U8 ucCbStatusCnt;     //��������״̬�ص�����
    HI_U8 ucCbStatStatusCnt; //��������ͳ����Ϣ�ص�����
    HI_U32 ulLastRcvStatusTime;
    HI_U32 ulLastCbStatusTime;

    HI_U8 ucRcvStopReqCnt; //����ֹͣ������Ϣ����
    HI_U8 ucCbStopCnt;     //����ֹͣ�����ص�����
    HI_U16 rcv_transfile_cnt;
    HI_U32 ulLastRcvStopTime;
    HI_U32 ulLastCbStopTime;
    HI_U32 reg_protocol_time; //ע��Э������ʱ���
    HI_U32 last_rcv_transfile_time;
    HI_U32 pad;
}HI_DSID_APP_UPG_CCO_SDK_STAT_S;//(0xa516 44�ֽ�)

typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U16 rcv_blk_cnt;         // �ѽ�����������֡����ok
    HI_U16 rcv_blk_LSN;         // ���һ�ν������ݵ�˳���ok
    HI_U32 rcv_blk_LTS;         // ���һ�ν������ݵ�ʱ���ok
    HI_U32 check_rcv_block_LRC; //���һ��У���������ķ���ֵ  OK
    HI_U32 check_rcv_block_LTS; //���һ��У����������ʱ���  OK
    HI_U32 cache_rcv_block_LRC;//���һ�λ�����յ������ݷ���ֵ OK
    HI_U32 cache_rcv_block_LTS;//���һ�λ�����յ�������ʱ��� OK
    HI_U32 last_Edge_Id;      //���һ�θ�HSO�ϱ��ı߽�ֵ OK
    HI_U32 last_Edge_Id_LTS;   //���һ�θ�HSO�ϱ��ı߽�ֵ��ʱ��� OK
    HI_U32 verify_upg_file_LRC;//�����ļ�У���� OK
    HI_U32 verify_upg_file_LTS;//�����ļ�У��ʱ���OK

    HI_U16 rcv_start_cnt;       // ���յ�start֡��ok
    HI_U16 rcv_start_LRC;       // ���һ�ε���start��������ֵok
    HI_U32 rcv_start_LTS;       // ���һ�ε���start�����ɹ���ʱ���ok
    HI_U16 usRcvU2bBlkCnt;      // ���յ�U2B���Ĳ�ת���Ĵ���

    HI_U16 reply_start_cnt;     //Ӧ��START֡���� ok
    HI_U32 reply_start_LTS;     //���һ�γɹ�Ӧ��START֡��ʱ��� ok
    HI_U16 reply_start_fail_cnt; //Ӧ��START֡ʧ�ܴ��� ok
    HI_U16 reply_start_full_cnt; //Ӧ��START֡���������� ok
    HI_U32 reply_start_LRC;  //���һ��Ӧ��ʼ����֡�ķ���ֵ ok

    HI_U8 ucErrorParaStart;  //���յ�����ʼ����֡����
    HI_U8 ucDiscardRcvStart; //���յ���ʼ����֡���������
    HI_U8 ucStdMallocFail;   //����ģʽ�����ڴ�ʧ�ܴ���
    HI_U8 ucAffirmDiscardCnt; //����ȫ�����֪ͨ�Ĵ���
    HI_U8 reserved3[2];
    HI_U8 bUpgradeSelf;     //�Ƿ���Ҫ���Լ�����:55��Ҫ���Լ�����/22����Ҫ���Լ�����
    HI_U8 ucPlcMode;        //PLCЭ��ģʽ

} HI_DSID_APP_UPG_STA_STAT_S;//(0xa503)

typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U32 rcv_status_LTS;      // ���һ���յ�״̬��ѯ֡��ʱ���ok
    HI_U16 rcv_status_cnt;      // ���յ�״̬��ѯ֡�Ĵ���ok
    HI_U16 rcv_start_FRC;       // ��һ�ε��ÿ�ʼ���������ķ���ֵ

    HI_U32 ulWriteRet;         //дFlash���
    HI_U32 reply_status_LTS;    //���һ��Ӧ��״̬��ѯ��ʱ��� ok
    HI_U16 reply_status_cnt;    // Ӧ��״̬��ѯ֡�Ĵ���ok
    HI_U16 reply_status_fail_cnt; //Ӧ��״̬��ѯ֡ʧ�ܴ��� ok
    HI_U16 reply_status_full_cnt; //Ӧ��״̬��ѯ֡���������� ok
    HI_U8  reply_status_upg_status; //���һ��Ӧ��״̬��ѯʱ����״̬ ok
    HI_U8  reply_status_report_cnt; //STA�����ϱ�״̬����

    HI_U32 finish_timer_set_LTS; //���һ������finish��affirm��ʱ��ʱ��� ok
    HI_U32 finish_timer_period; //���һ������finish��affirm��ʱ����ʱ������ ok

    HI_U32 rcv_affirm_LTS;      //���һ�ν��յ����֪ͨ��ʱ��� ok
    HI_U16 rcv_affirm_cnt;      //���յ����֪ͨ������Ĵ��� ok

    HI_U16 rcv_stop_cnt;        //���յ�ֹͣ������Ӧ����ok
    HI_U16 rcv_stop_denied_cnt; //�ܾ�ֹͣ�������� ok
    HI_U16 padding;
    HI_U32 rcv_stop_LRC;        //���һ���յ�ֹͣ�����ķ���ֵ
    HI_U32 rcv_stop_LTS;        //���һ��ֹͣ������ʱ���ok
    HI_U32 rcv_stop_upg_id;      //���յ�ֹͣ����ʱģ�鵱ǰ������UPG ID ok
    HI_U32 rcv_stop_req_upg_id;  //���յ�ֹͣ����ʱ����ֹͣ������UPG ID ok
} HI_DSID_APP_UPG_STA_STAT1_S;//(0xa506)

//վ��Ӧ��״̬��ѯ��Ϣ
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_UPG_STA_REPLY_QUERY_INFO struStaReplyQueryDetail[UPG_STA_REC_REPLY_STATUS_CNT];//STAվ�����3��Ӧ��״̬��ѯ��Ϣ

}HI_DSID_APP_UPG_STA_STAT2_S;//44�ֽ�(0xa508)

typedef struct
{
    HI_U8 ucRegCbCnt;
    HI_U8 ucRcvStartReqCnt;   //���տ�ʼ������Ϣ����
    HI_U8 ucCbStartCnt;       //���ÿ�ʼ�����ص�����
    HI_U8 reg_protocol_type; //�û�ע���Э������
    HI_U32 ulLastRcvStartTime;
    HI_U32 ulLastCbStartTime;

    HI_U8 ucRcvStatusReqCnt;  //��������״̬��ѯ��Ϣ����
    HI_U8 ucCbBsStatusCnt;   //���û�������״̬�ص�����
    HI_U8 ucCbStatusCnt;     //��������״̬�ص�����
    HI_U8 pad2;
    HI_U32 ulLastRcvStatusTime;
    HI_U32 ulLastCbStatusTime;

    HI_U8 ucRcvStopReqCnt; //����ֹͣ������Ϣ����
    HI_U8 ucCbStopCnt;     //����ֹͣ�����ص�����
    HI_U16 pad3;
    HI_U32 ulLastRcvStopTime;
    HI_U32 ulLastCbStopTime;

    HI_U8 ucRcvDeviceStartIndCnt;    //�����豸������ʼӦ����Ϣ����
    HI_U8 ucCbDeviceStartCnt;      //�����豸������ʼ����ص�����
    HI_U16 pad4;
    HI_U32 ulLastRcvDeviceStartTime;
    HI_U32 ulLastCbDeviceStartTime;

    HI_U8 ucRcvDeviceStatusIndCnt;  //�����豸����״̬Ӧ����Ϣ����
    HI_U8 ucCbDeviceStatusCnt;     //�����豸����״̬����ص�����
    HI_U16 pad5;
    HI_U32 ulLastRcvDeviceStatusTime;
    HI_U32 ulLastCbDeviceStatusTime;

    HI_U8 ucRcvDeviceStopIndCnt;   //�����豸����ֹͣӦ����Ϣ����
    HI_U8 ucCbDeviceStopCnt;     //�����豸����ֹͣ����ص�����
    HI_U16 rcv_transfile_cnt;
    HI_U32 ulLastRcvDeviceStopTime;
    HI_U32 ulLastCbDeviceStopTime;
    HI_U32 last_rcv_transfile_time;
    HI_U32 reg_protocol_time;//�û�ע��Э���ʱ���
    HI_U32 pad;
}HI_DSID_APP_UPG_STA_SDK_STAT_S;//(0xa517 84�ֽ�)

//*****************************************************************************
// APP��Ϣ����
//*****************************************************************************
// ��Χ [0B00, 0F00)
//*****************************************************************************
#define HI_DMID_APP_MAC_LIST_CHANGE             0x0B00      // HI_DMID_APP_MAC_LIST_CHANGE_S
#define HI_DMID_APP_MAC_MAP_CHANGE              0x0B01      // HI_DMID_APP_MAC_MAP_CHANGE_S
#define HI_DMID_APP_CHL_RX_UART                 0x0B02      // HI_DMID_APP_CHL_RX_UART_S
#define HI_DMID_APP_CHL_TX_UART                 0x0B03      // HI_DMID_APP_CHL_TX_UART_S
#define HI_DMID_APP_CHL_RX_PLC                  0x0B04      // HI_DMID_APP_CHL_RX_PLC_S
#define HI_DMID_APP_CHL_TX_PLC                  0x0B05      // HI_DMID_APP_CHL_TX_PLC_S
#define HI_DMID_APP_CHL_STATUS                  0x0B06      // HI_DMID_APP_CHL_STATUS_S
#define HI_DMID_APP_TIMER_START                 0x0B07      // HI_DMID_APP_TIMER_START_S
#define HI_DMID_APP_TIMER_STOP                  0x0B08      // HI_DMID_APP_TIMER_STOP_S
#define HI_DMID_APP_TIMER_ACTIV                 0x0B09      // HI_DMID_APP_TIMER_ACTIV_S
#define HI_DMID_APP_RX_FRAME_COMPLETE           0x0B0A      // HI_DMID_APP_RX_FRAME_COMPLETE_S
#define HI_DMID_APP_376_2_DEC_COMPLETE          0x0B0B      // HI_DMID_APP_376_2_DEC_COMPLETE_S
#define HI_DMID_APP_645_DEC_COMPLETE            0x0B0C      // HI_DMID_APP_645_DEC_COMPLETE_S
#define HI_DMID_APP_SES_ENC_COMPLETE            0x0B0D      // HI_DMID_APP_SES_ENC_COMPLETE_S
#define HI_DMID_APP_SES_CHECK_COMPLETE          0x0B0E      // HI_DMID_APP_SES_CHECK_COMPLETE_S
#define HI_DMID_APP_TX_GET_ADDR_PLC             0x0B0F      // HI_DMID_APP_TX_GET_ADDR_PLC_S
#define HI_DMID_APP_RX_GET_ADDR_PLC             0x0B10      // HI_DMID_APP_RX_GET_ADDR_PLC_S
#define HI_DMID_APP_FIND_MAC_MAP_OK             0x0B11      // HI_DMID_APP_FIND_MAC_MAP_OK_S
#define HI_DMID_APP_FIND_MAC_MAP_FAIL           0x0B12      // HI_DMID_APP_FIND_MAC_MAP_FAIL_S
#define HI_DMID_APP_INQ_METER                   0x0B13      // HI_DMID_APP_INQ_METER_S
#define HI_DMID_APP_INQ_ANS_METER               0x0B14      // HI_DMID_APP_INQ_ANS_METER_S
#define HI_DMID_APP_TX_RM_PLC_CCO               0x0B15      // HI_DMID_APP_TX_RM_PLC_CCO_S
#define HI_DMID_APP_RX_RM_PLC_CCO               0x0B16      // HI_DMID_APP_RX_RM_PLC_CCO_S
#define HI_DMID_APP_TX_RM_PLC_STA               0x0B17      // HI_DMID_APP_TX_RM_PLC_STA_S
#define HI_DMID_APP_RX_RM_PLC_STA               0x0B18      // HI_DMID_APP_RX_RM_PLC_STA_S
#define HI_DMID_APP_START_SCAN_METER            0x0B19      // HI_DMID_APP_START_SCAN_METER_S
#define HI_DMID_APP_REP_SCAN_METER              0x0B1A      // HI_DMID_APP_REP_SCAN_METER_S
#define HI_DMID_APP_TX_RM_STA                   0x0B1B      // HI_DMID_APP_TX_RM_STA_S
#define HI_DMID_APP_RX_RM_STA                   0x0B1C      // HI_DMID_APP_RX_RM_STA_S
#define HI_DMID_APP_REP_RM                      0x0B1D      // HI_DMID_APP_REP_RM_S
#define HI_DMID_APP_TX_HEARTBEAT                0x0B1E      // HI_DMID_APP_TX_HEARTBEAT_S
#define HI_DMID_APP_UPD_MADDR                   0x0B1F      // HI_DMID_APP_UPD_MADDR_S
#define HI_DMID_APP_QUE_STATUS                  0x0B20      // HI_DMID_APP_QUE_STATUS_S
#define HI_DMID_APP_MACTOPO_STATUS              0x0B21      // HI_DMID_APP_MACTOPO_STATUS_S


//*****************************************************************************
// �ṹ����
//*****************************************************************************
#define APP_MAX_BUF_LEN 96     // ����ϱ�����������

typedef struct
{
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];    // �����ı��Mac��ַ
    HI_S16 index;   // Mac����
    HI_U8 option;   // ��������
}HI_DMID_APP_MAC_LIST_CHANGE_S; // MacList�ı�

typedef struct
{
    HI_U8 addr[METER_ADDR_LEN]; // �����ı�ı��ַ
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];    // �����ı��Mac��ַ
    HI_U8 option;   // ��������
}HI_DMID_APP_MAC_MAP_CHANGE_S;  // MacMap�ı�

typedef struct
{
    HI_U16 len;     // ���յ������ݳ���
    HI_U8 data[APP_MAX_BUF_LEN];   // ���յ�����������
}HI_DMID_APP_CHL_RX_UART_S; // ����ͨ�����ݽ���

typedef struct
{
    HI_U16 len;     // ���͵����ݳ���
    HI_U8 data[APP_MAX_BUF_LEN];   // ���͵���������
}HI_DMID_APP_CHL_TX_UART_S; // ����ͨ�����ݷ���

typedef struct
{
    HI_U16 id;      // ���յ�PLC֡��ID
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];    // ԴMac
    HI_U16 len;     // ���յ�PLC֡�ṹ�ĳ���
    HI_U8 data[APP_MAX_BUF_LEN];   // ���յ�PLC֡�ṹ������
}HI_DMID_APP_CHL_RX_PLC_S;  // PLCͨ�����ݽ���

typedef struct
{
    HI_U16 id;      // ����PLC֡��ID
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];    // Ŀ��Mac
    HI_U16 len;     // ����PLC֡�ĳ���
    HI_U8 data[APP_MAX_BUF_LEN];   // ����PLC֡������
}HI_DMID_APP_CHL_TX_PLC_S;  // PLCͨ�����ݷ���

typedef struct
{
    HI_U8 type;     // ͨ������
    HI_U8 state;    // ͨ��״̬
}HI_DMID_APP_CHL_STATUS_S;  // ��ǰͨ��״̬֪ͨ

typedef struct
{
    HI_U16 id;  // ��ʱ��ID
    HI_U32 value; // ��ʱ��ֵ
    HI_U8 option; // ��ʱ������
}HI_DMID_APP_TIMER_START_S; // ��ʱ������

typedef struct
{
    HI_U16 id;  // ��ʱ��ID
}HI_DMID_APP_TIMER_STOP_S;  // ��ʱ��ֹͣ

typedef struct
{
    HI_U16 id;  // ��ʱ��ID
    HI_U32 p;   // ��ʱ���ص�����ָ��
}HI_DMID_APP_TIMER_ACTIV_S; // ��ʱ������

typedef struct
{
    HI_U16 len;     // ���ݳ���
    HI_U8 data[APP_MAX_BUF_LEN];   // ��������
}HI_DMID_APP_RX_FRAME_COMPLETE_S;   // ���յ�����֡

typedef struct
{
    HI_U8 ctrl;     // ������
    HI_U8 afn;      // ������
    HI_U8 fn;       // ��Ϣ��Ԫ
}HI_DMID_APP_376_2_DEC_COMPLETE_S;  // 376.2֡�ṹ�������

typedef struct
{
    HI_U8 protocol; // ��Լ����
    HI_U8 ctrl;     // ������
    HI_U32 id;      // ����ID
}HI_DMID_APP_645_DEC_COMPLETE_S;    // 645֡�ṹ�������

typedef struct
{
    HI_U32 id;  // �ỰID
    HI_U32 sn;  // �ỰSN
    HI_U8 cnt;  // CNT��־
    HI_U8 acd;  // ACD��־
}HI_DMID_APP_SES_ENC_COMPLETE_S;    // �Ự��֯���

typedef struct
{
    HI_U32 id[2];   // �ỰID ([0]֡ [1]����)
    HI_U32 sn[2];   // �ỰSN ([0]֡ [1]����)
    HI_U8 cnt;      // CNT��־
    HI_U8 acd;      // �Ự��Чԭ��
}HI_DMID_APP_SES_CHECK_COMPLETE_S;  // �Ự������

typedef struct
{
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];    // ��Ӧ��MAC��ַ
    HI_U16 len;     // PLC֡����
    HI_U8 data[APP_MAX_BUF_LEN];   // PLC֡�ṹ����
}HI_DMID_APP_TX_GET_ADDR_PLC_S; // ���ͻ�ȡ����ַPLC֡

typedef struct
{
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];    // ��Ӧ��MAC��ַ
    HI_U8 num;  // ��Ӧ�ı��ַ����
    METER_ADDR meter[MAX_METER_NUM];    // ��Ӧ�ı��ַ�б�
    HI_U16 len; // PLC֡����
    HI_U8 data[APP_MAX_BUF_LEN];    // PLC֡�ṹ����
}HI_DMID_APP_RX_GET_ADDR_PLC_S; // ���ջ�ȡ����ַPLC֡

typedef struct
{
    HI_U8 addr[METER_ADDR_LEN];     // ���ַ
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];        // ��Ӧ��MAC��ַ
}HI_DMID_APP_FIND_MAC_MAP_OK_S; // ����MACӳ���ɹ�

typedef struct
{
    HI_U8 addr[METER_ADDR_LEN];     // ���ַ
    HI_U8 result;     // ʧ��ԭ��
}HI_DMID_APP_FIND_MAC_MAP_FAIL_S;   // ����MACӳ���ʧ��

typedef struct
{
    HI_U8 addr[METER_ADDR_LEN];     // ��ǰ����ڵ�ĵ���ַ
    HI_U16 index;   // ��ǰ����ڵ�����
    HI_U16 len; // ֡����
    HI_U8 data[APP_MAX_BUF_LEN];   // ֡����
}HI_DMID_APP_INQ_METER_S;   // ���Ͷ���ѯ��֡

typedef struct
{
    HI_U8 addr[METER_ADDR_LEN];     // Ӧ�����ı��ַ
    HI_U8 read_flg;     // Ӧ�����ĳ�����־
    HI_U16 len;     // Ӧ�����ı��ĳ���
    HI_U8 data[APP_MAX_BUF_LEN];   // Ӧ�����ı�������
}HI_DMID_APP_INQ_ANS_METER_S;   // ���ն���ѯ��Ӧ��֡

typedef struct
{
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];     // Ŀ��Mac
    HI_U16 len;     // ����PLC�ṹ����
    HI_U8 data[APP_MAX_BUF_LEN];   // ����PLC�ṹ����
}HI_DMID_APP_TX_RM_PLC_CCO_S;   // ���Ͷ���PLC֡(CCO)

typedef struct
{
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];     // ԴMac
    HI_U16 len;     // ����PLC�ṹ����
    HI_U8 data[APP_MAX_BUF_LEN];   // ����PLC�ṹ����
}HI_DMID_APP_RX_RM_PLC_CCO_S;   // ���ն���PLC֡(CCO)

typedef struct
{
    HI_U16 len;     // ����PLC�ṹ����
    HI_U8 data[APP_MAX_BUF_LEN];   // ����PLC�ṹ����
}HI_DMID_APP_TX_RM_PLC_STA_S;   // ���Ͷ���PLC֡(STA)

typedef struct
{
    HI_U16 len;     // ����PLC�ṹ����
    HI_U8 data[APP_MAX_BUF_LEN];   // ����PLC�ṹ����
}HI_DMID_APP_RX_RM_PLC_STA_S;   // ���ն���PLC֡(STA)

typedef struct
{
    HI_U8 second;
    HI_U8 minute;
    HI_U8 hour;
    HI_U8 day;
    HI_U8 month;
}APP_TIME_S;

typedef struct
{
    APP_TIME_S start_time;  // �ѱ�ʼʱ��
    HI_U16 duration;    // �ѱ����ʱ��
    HI_U8 retry;    // �ӽڵ��ط�����
    HI_U8 slice;    // ����ȴ�ʱ��Ƭ����
}HI_DMID_APP_START_SCAN_METER_S;    // �����ѱ�

typedef struct
{
    HI_U8 num;  // �ϱ��Ĵӽڵ�����
    METER_ADDR meter[MAX_METER_NUM]; // �ӽڵ��ַ
    HI_U8 protocol;     // ��Լ����
    HI_U16 index;   // �ӽڵ����
    HI_U8 type;     // �豸����
}HI_DMID_APP_REP_SCAN_METER_S;  // �ϱ��ѱ���Ϣ

typedef struct
{
    HI_U8 from; // ������Դ
    HI_U16 len; // ���ݳ���
    HI_U8 data[APP_MAX_BUF_LEN];   // ��������
}HI_DMID_APP_TX_RM_STA_S;    // ���Ͷ�������(STA)

typedef struct
{
    HI_U16 len; // ���ݳ���
    HI_U8 data[APP_MAX_BUF_LEN];   // ��������
}HI_DMID_APP_RX_RM_STA_S;   // ���ն�������(STA)

typedef struct
{
    HI_U8 addr[METER_ADDR_LEN]; // �ϱ��ı��ַ
    HI_U16 len; // �ϱ���֡����
    HI_U8 data[APP_MAX_BUF_LEN];   // �ϱ���֡����
}HI_DMID_APP_REP_RM_CCO_S;  // �ϱ���������(CCO)

typedef struct
{
    HI_U16 len; // ����֡����
    HI_U8 data[APP_MAX_BUF_LEN]; // ����֡����
}HI_DMID_APP_TX_HEARTBEAT_S;    // ��������֡

typedef struct
{
    HI_U8 num;  // ��ǰ���ַ����
    METER_ADDR meter[MAX_METER_NUM];    // ���ַ�б�(���32��Ԫ��)
}HI_DMID_APP_UPD_MADDR_S;   // ���µ���ַ�б�

typedef struct
{
    HI_U8 num;  // ��ǰ����Ԫ�ظ���
    HI_U32 id[MAX_QUEUE_NUM];   // ����ID�б�
    HI_U8 form; // ������Դ
    HI_U8 top_state;    // ����״̬(�Ƿ�����)
}HI_DMID_APP_QUE_STATUS_S;  // ��ǰ����״̬

typedef struct
{
    HI_U16 num;     // ��ǰMacTopo����
    MAC_ADDR mac_list[20];  // ǰ20��Topo��Mac��ַ
}HI_DMID_APP_MACTOPO_STATUS_S;  // ��ǰMacTopo��״̬


//*****************************************************************************
// System ͳ��������
//*****************************************************************************
// ��Χ [0xB000, 0xB200)
//*****************************************************************************
#define HI_DSID_UPG_TM                  0xB000    // HI_DSID_UPG_TM_S
#define HI_DSID_UPG_BLK_TM              0xB001    // HI_DSID_UPG_BLK_TM_S
#define HI_DSID_UPG_BROSTART_STATUS     0xB002    // HI_UPG_STAT_INFO_IND_S

#define HI_DSID_WD_INFO                 0xB100    // HI_DSID_WD_INFO_S
#define HI_DSID_DIAG_CHL                0xB101    // HI_DSID_DIAG_CHL_S
#define HI_DSID_LOGMSG                  0xB102    // HI_DSID_LOGMSG_S
#define HI_DSID_DIAG_BUF_STAT           0xB103    //HI_DIAG_BUF_STAT_S

//
// ������¼
//

// ������ʱͳ��
typedef struct
{
    HI_U32 idx;              // ������
    HI_U32 tm;               // �����������ʱ
    HI_U32 crc;              // ����������У���ʱ, -- APP,DRV,MAC?

    HI_U32 start;             // ���������鿪ʼ
    HI_U32 end;               // �������������
}HI_DSID_UPG_BLK_TM_MAX_S;

#if defined(PRODUCT_CFG_DFX_PLC_FRM_STAT)
typedef struct
{
    HI_OBJ64_S objcnt;

    // ������Ϣ
    HI_U8  eth_ip[4];
    HI_U32 init_start;
    HI_U32 phase_start[2];
    HI_U32 init_end;
    HI_U32 netformed_first_tm;
    HI_U32 netformed_last_tm;
    HI_U32 netformed_num;
    HI_U32 netformed_ok;
}HI_DSID_UPG_TM_S;
#endif

//*****************************************************************************
// Time stat. helper
//*****************************************************************************
#if !defined(PRODUCT_CFG_HSO)
HI_EAPI HI_U32 HI_SYS_GetCurrentTaskSchCnt(HI_VOID);
#if defined(PRODUCT_CFG_DFX_PLC_FRM_STAT)
HI_EAPI HI_DSID_UPG_TM_S* HI_SYS_DfxPlcFrmStat(HI_VOID);
#endif

//*****************************************************************************
#define HI_SYS_TSK_TM_S(x) do { \
}while(0)
#define HI_SYS_TSK_PROC_WT(enCause)    HI_SYS_RstResume(enCause)  // HI_SYS_RST_RESUME_CAUSE_E
#define HI_SYS_TSK_TM_E(x) do { \
}while(0)
//*****************************************************************************

//*****************************************************************************
#define HI_SYS_DIAG_NEED_WT_CNT_MAX   50
#define HI_SYS_RXTX_NEED_WT_CNT_MAX   100


#if defined(PRODUCT_CFG_SUPPORT_TSK_WT_DIAG)
#define HI_SYS_DIAG_TX_TSK_WT_INIT()   HI_U32 __cnt = 0
#define HI_SYS_DIAG_TX_TSK_WT() do {\
                 if(__cnt ++ >= HI_SYS_DIAG_NEED_WT_CNT_MAX)\
                 {\
                    __cnt = 0;\
                    HI_SYS_TSK_PROC_WT(HI_SYS_RST_RESUME_DIAG);\
                 }\
             }while(0)
#else
#define HI_SYS_DIAG_TX_TSK_WT_INIT()
#define HI_SYS_DIAG_TX_TSK_WT()
#endif

#if defined(PRODUCT_CFG_SUPPORT_TSK_WT_HISR_RX)
#define HI_SYS_HISR_RX_TSK_WT() do {\
                 static HI_U32 ___g_tx_cnt = 0;\
                 if(___g_tx_cnt ++ >= HI_SYS_RXTX_NEED_WT_CNT_MAX)\
                 {\
                    ___g_tx_cnt = 0;\
                    HI_SYS_TSK_PROC_WT(HI_SYS_RST_RESUME_HISR_RX);\
                 }\
             }while(0)
#else
#define HI_SYS_HISR_RX_TSK_WT()
#endif

#if defined(PRODUCT_CFG_SUPPORT_TSK_WT_TX)
#define HI_SYS_TX_TSK_WT() do {\
                 static HI_U32 ___g_tx_cnt = 0;\
                 if(___g_tx_cnt ++ >= HI_SYS_RXTX_NEED_WT_CNT_MAX)\
                 {\
                    ___g_tx_cnt = 0;\
                    HI_SYS_TSK_PROC_WT(HI_SYS_RST_RESUME_TX);\
                 }\
             }while(0)
#else
#define HI_SYS_TX_TSK_WT()
#endif

//*****************************************************************************
#endif


//
// ÿ����Ĵ����ʱ
//
#define HI_DSID_UPG_BLK_ARQ_CNT    30
#if defined(PRODUCT_CFG_DFX_PLC_FRM_BLK_STAT)
typedef struct
{
    HI_OBJ64_S objcnt;

    HI_U16 blk_req_cnt; //  hso�Ŀ��Զ��ش�����    -- APP
    HI_U16 blk_arq_cnt; // ���Զ��ش�����(CCo-STA) -- APP
    HI_U16 blk_idx;     // �����                  -- APP
    HI_U16 blk_size;    // ���С                  -- APP

    // ���� ������������ʼ��¼
    HI_U16 app_start;  // APP���������鿪ʼ -- APP
    HI_U16 app_end;    // APP������������� -- APP

    HI_U16 drv_start;  // DRV���������鿪ʼ -- DRV
    HI_U16 drv_end;    // DRV������������� -- DRV

    HI_U16 mac_start;      // MAC���������鿪ʼ -- MAC
    HI_U16 mac_end;        // MAC������������� -- MAC
    HI_U16 mac_arq_cnt;    // �ش�����
    HI_U8 mac_pb_type;     // 0: 136; 1:520
    HI_U8 mac_pb_num;      // pb�����

    HI_U32 blk_err;        // �鴫������� -- APP
}HI_DSID_UPG_BLK_TM_S;
#endif


#if defined(PRODUCT_CFG_DFX_PLC_FRM_BLK_STAT)
#define HI_DSID_UPG_BLK_NUM_MAX    700
#endif

#define HI_DSID_UPG_BLK_CURRENT    0xFFFF

//****************************************************************************

// DIAG ��������������
#define SAL_DIAG_SDM_CAT_MAX     7

#define HI_DFX_SYS_MODEM_Q_NUM_MAX   10
#define HI_DFX_SYS_MODEM_E_NUM_MAX   10
#define HI_DFX_SYS_APP_Q_NUM_MAX     3
#define HI_DFX_SYS_Q_NAME_LEN        20
#define HI_DFX_SYS_TASK_NUM_MAX      20
#define HI_DFX_RST_RESUME_CAUSE_MAX  12
#define HI_DFX_OS_QRY_STK_SIZE_MAX   64

typedef struct
{
    HI_CHAR szTaskName[16];
    HI_CHAR szTaskStatus[12];
    HI_U8   priority;
    HI_U8   idx;
    HI_U8   reserved[2];
    HI_U32  task_status;
    HI_U32  stack_base;
    HI_U32  stack_size;
    HI_U32  minimum_stack;
    HI_U32  scheduled_count;
    HI_U32  time_slice;
    HI_U32  tsp;
    HI_U32  aulStk[HI_DFX_OS_QRY_STK_SIZE_MAX];
}HI_DFX_OS_TASK_INFO_S;

typedef struct
{
    HI_U32 aulTotal[2];
    HI_U32 aulMQMsgNum[HI_DFX_SYS_MODEM_Q_NUM_MAX];
    HI_U32 aulMQMsgTotalSize[HI_DFX_SYS_MODEM_Q_NUM_MAX];
    HI_U32 aulAQMsgCnt[HI_DFX_SYS_APP_Q_NUM_MAX];
    HI_U32 aulAQMsgTotalSize[HI_DFX_SYS_APP_Q_NUM_MAX];
    HI_U8  bQDyn;
    HI_U8  bAppStart;
    HI_U16 usQCreateErrNum;
    HI_U32 aulFlashStat[8];
    HI_U32 aulMMsgTimerStat[7];
    HI_U32 aulAMsgTimerStat[7];
    HI_U32 aulHrTimerStat[2];
    HI_U32 aulOsTimerStat[7];
    HI_U32 aulOsUsrTimerStat[4];
    HI_U32 aulMQMsgNumUsed[HI_DFX_SYS_MODEM_Q_NUM_MAX];
    HI_U32 aulAQMsgNumUsed[HI_DFX_SYS_APP_Q_NUM_MAX];
    HI_U32 aulMQNumW[HI_DFX_SYS_MODEM_Q_NUM_MAX];
    HI_U32 aulMQNumR[HI_DFX_SYS_MODEM_Q_NUM_MAX];
    HI_U32 aulAQNumW[HI_DFX_SYS_APP_Q_NUM_MAX];
    HI_U32 aulAQNumR[HI_DFX_SYS_APP_Q_NUM_MAX];
    HI_U32 aulMENumW[HI_DFX_SYS_MODEM_E_NUM_MAX];
    HI_U32 aulMENumR[HI_DFX_SYS_MODEM_E_NUM_MAX];
}HI_DSID_OS_RES_S;

typedef struct
{
    HI_U32 ulTimeOutCfg;
    HI_U32 aulCnt[2];
    HI_U32 aulResetCnt[2];
    HI_U32 aulEnable[2];
    HI_U32 aulDisable[2];
    HI_U32 aulRstMon[2];
    HI_U32 aulRstResumeCnt[HI_DFX_RST_RESUME_CAUSE_MAX];
    HI_U32 reserved[2];
}HI_DSID_WD_INFO_S;

typedef struct
{
    HI_U32 aulPhyPortConnectxCnt[3];
    HI_U32 aulPhyPortDisConnectxCnt[3];
    HI_U32 aulConnectxCnt[3];
    HI_U32 aulDisConnectxCnt[3];
    HI_U32 aulDiagTcp2DmsTxCnt[3];
    HI_U32 aulDiagUart2DmsTxCnt[3];
    HI_U32 aulTcpPortTxCnt[2];
    HI_U32 aulTxTaskRunCnt[2];
    HI_U32 aulTxTaskDisconnectWaitCnt[2];
    HI_U32 aulTcpPortRxCnt[2];
    HI_U32 ulTcpPortAcceptCnt;
    HI_U32 ulTcpPortCloseCnt;
    HI_U32 ulTxTaskQMsgUsedCnt;
    HI_U32 ulTxTaskQMsgTotalCnt;
    HI_U32 aulTxQBlkLeftCnt[SAL_DIAG_SDM_CAT_MAX];
    HI_U32 aulTxQBlkReqTooLargeCnt[SAL_DIAG_SDM_CAT_MAX];
    HI_U32 aulReqBadCnt[8];
    HI_U32 ulGoodPacketReqCnt;
    HI_U32 aulReqCnt[6];
    HI_U32 aulReqLen[4];
    HI_U32 aulDiagRx[2];
    HI_U32 aulUserReqAckCnt[8];
    HI_U32 aulUserCmdLast[3];
    HI_U32 aulRxLast[6];
    HI_U32 aulNdcReq[2];
    HI_U32 aulNdcIndI[3];
    HI_U32 aulNdcIndD[3];
    HI_U32 aulNdcIndMdm[2];
}HI_DSID_DIAG_CHL_S;

typedef struct
{
    HI_U32 aulMacStatus[3];
    HI_U32 aulMacTxCnt[2];
    HI_U32 aulSimMacTxCnt[2];
    HI_U32 ulMacTxInputErrCnt;
    HI_U32 aulTcpTxCnt[2];
    HI_U32 aulDiagUartTxCnt[2];
    HI_U32 aulAppUartTxCnt[2];
    HI_U32 aulIrUartTxCnt[2];
    HI_U32 aulAppSimuUartTxCnt[2];
    HI_U32 aulTxRc[7];
    HI_U32 aulNvReadCnt[4];
    HI_U32 aulNvWriteCnt[3];
    HI_U32 aulNvFacWriteCnt[3];
    HI_U32 aulDiagNvWriteCnt[4];
    HI_U32 aulDiagNvReadCnt[4];
    HI_U32 aulMacRxCnt[8];
    HI_U32 aulDmsForwarding[2];
    HI_U32 aulDIRemoteChl[8];
    HI_U32 ulMacForceSndCnt;
    HI_U32 ulDmsUartSetup[4];
}HI_DSID_DMS_CHL_S;

typedef struct
{
    HI_U32 ulTimeStampSec;
    HI_DSID_DIAG_CHL_S   stDiagChl;
    HI_DSID_DMS_CHL_S    stDmsChl;
    HI_DSID_WD_INFO_S    stWd;
    HI_SYS_STATUS_INFO_S stSysStatus;
    HI_DSID_OS_RES_S     stOsRes;
}HI_DSID_SYS_QRY_S;

typedef struct
{
    HI_U32 ulModId;
    HI_U32 usRc;
    HI_U32 usDataType;
    HI_U32 id;
    HI_U32 cnt[2];
}HI_DSID_LOGMSG_S;

//****************************************************************************

//*****************************************************************************
// ��������: HI_SYS_DfxPlcFrmStat
// ��������: ��ȡ������¼��ͳ��������
//
// ����˵��:
//   ��
//
// �� �� ֵ:
//    HI_DSID_UPG_TM_S*����,����ֱ�ӶԳ�Ա��ֵ.
//
// ����Ҫ��:
// ���þ���:
//*****************************************************************************
#if defined(PRODUCT_CFG_DFX_PLC_FRM_STAT)
HI_EAPI HI_DSID_UPG_TM_S* HI_SYS_DfxPlcFrmStat(HI_VOID);

#define HI_SYS_DfxPlcFrmStat_TM(m)       do { HI_SYS_DfxPlcFrmStat()->m = HI_MDM_GetMilliSeconds(); } while(0)
#define HI_SYS_DfxPlcFrmStat_ZERO(m)     do { HI_SYS_DfxPlcFrmStat()->m = 0; } while(0)
#define HI_SYS_DfxPlcFrmStat_SET(m,val)  do { HI_SYS_DfxPlcFrmStat()->m = val;  } while(0)
#define HI_SYS_DfxPlcFrmStat_GET(m)      HI_SYS_DfxPlcFrmStat()->m
#define HI_SYS_DfxPlcFrmStat_INC(m)      do { if(HI_SYS_DfxPlcFrmStat()->m == 0xFFFFFFFF) HI_SYS_DfxPlcFrmStat()->m = 0; HI_SYS_DfxPlcFrmStat()->m++; } while(0)
#define HI_SYS_DfxPlcFrmStat_SET_ETH_IP(val)                        memcpy_s(HI_SYS_DfxPlcFrmStat()->eth_ip,4, val, 4)
#define HI_SYS_DfxPlcFrmStat_SET_UpgDroppedBlks(_asl_dropped_blks)  memcpy_s(HI_SYS_DfxPlcFrmStat()->dropped_blks,10,_asl_dropped_blks, 10)
#else
#define HI_SYS_DfxPlcFrmStat_TM(m)
#define HI_SYS_DfxPlcFrmStat_TM_ZERO(m)
#define HI_SYS_DfxPlcFrmStat_SET(m,val)
#define HI_SYS_DfxPlcFrmStat_GET(m)      1
#define HI_SYS_DfxPlcFrmStat_INC(m)
#define HI_SYS_DfxPlcFrmStat_SET_ETH_IP(val)
#define HI_SYS_DfxPlcFrmStat_SET_UpgDroppedBlks(_asl_dropped_blks)
#endif

//*****************************************************************************
// ��������: HI_SYS_DfxPlcFrmBlkStat
// ��������: ��ȡ�����鴫���ʱ���¼��ͳ��������
//
// ����˵��:
//   usBlkIdx [in] usBlkIdx ����С�� HI_DSID_UPG_BLK_NUM_MAX,
//                 ��� usBlkIdx���� HI_DSID_UPG_BLK_CURRENT, ��ʾ��ȡ��ǰ��
//
// �� �� ֵ:
//    HI_DSID_UPG_BLK_TM_S*����,����ֱ�ӶԳ�Ա��ֵ.
//    HI_NULL: �޶�Ӧ��
//
// ����Ҫ��:
// ���þ���:
//*****************************************************************************
#if defined(PRODUCT_CFG_DFX_PLC_FRM_BLK_STAT)
HI_EAPI HI_DSID_UPG_BLK_TM_S* HI_SYS_DfxPlcFrmBlkStat(HI_U16 usBlkIdx);
#define HI_SYS_DfxPlcFrmBlkStat_TM(idx,m)       do { HI_SYS_DfxPlcFrmBlkStat(idx)->m = HI_MDM_GetMilliSeconds(); }while(0)
#define HI_SYS_DfxPlcFrmBlkStat_SET(idx,m,val)  do { HI_SYS_DfxPlcFrmBlkStat(idx)->m = val; }while(0)
#define HI_SYS_DfxPlcFrmBlkStat_GET(idx,m)      HI_SYS_DfxPlcFrmBlkStat(idx)->m
#define HI_SYS_DfxPlcFrmBlkStat_INC(idx,m)      do { HI_SYS_DfxPlcFrmBlkStat(idx)->m++; }while(0)
#else
#define HI_SYS_DfxPlcFrmBlkStat_TM(idx,m)       do { } while(0)
#define HI_SYS_DfxPlcFrmBlkStat_SET(idx,m,val)  do { } while(0)
#define HI_SYS_DfxPlcFrmBlkStat_GET(idx,m)      0
#define HI_SYS_DfxPlcFrmBlkStat_INC(idx,m)      do { } while(0)
#endif

//*****************************************************************************


//*****************************************************************************
// ϵͳ��Ϣ����
//*****************************************************************************
// ��Χ [0x2000, 0x2100)
//*****************************************************************************
#define HI_DMID_UPG_REQ_START              0x2000  // HI_DMID_UPG_REQ_START_S           ������ʼ����
#define HI_DMID_UPG_REQ_STOP               0x2001  // HI_DMID_UPG_REQ_STOP_S            ����ֹͣ����
#define HI_DMID_UPG_REQ_VER_QRY            0x2002  // HI_DMID_UPG_REQ_VER_QRY_S         �����汾��ѯ����
#define HI_DMID_UPG_REQ_STATUS             0x2003  // HI_DMID_UPG_REQ_STATUS_S          ����״̬��ѯ����
#define HI_DMID_UPG_BROSTART_START         0x2010  // HI_DMID_UPG_BROSTART_START_S      ȫ����������          -- CCO
#define HI_DMID_UPG_BROSTART_STOP          0x2011  // HI_DMID_UPG_BROSTART_STOP_S       ȫ������ֹͣ          (����ÿ�ֿ�ʼ�ϱ�) -- CCO
#define HI_DMID_UPG_BROSTART_TRANSMIT      0x2012  // HI_DMID_UPG_BROSTART_TRANSMIT_S   ȫ�������鴫��       (����ÿ�ֿ�ʼ�ϱ�) -- CCO
#define HI_DMID_UPG_BROSTART_STATUS_IND    0x2013  // HI_DMID_UPG_BROSTART_STATUS_IND_S ȫ������״̬�ϱ�       -- CCO(���ϱ�ͳ���������ϱ����нڵ�״̬)
#define HI_DMID_UPG_BROSTART_RESULT        0x2014  // HI_DMID_UPG_BROSTART_RESULT_S     ȫ���������ͽ��
#define HI_DMID_UPG_UNICAST_START          0x2020  // HI_DMID_UPG_UNICAST_START_S       STA��������           -- STA
#define HI_DMID_UPG_UNICAST_STOP           0x2021  // HI_DMID_UPG_UNICAST_STOP_S        STA����ֹͣ           (����ÿ�ֿ�ʼ�ϱ�) -- STA
#define HI_DMID_MAC_PHY_DRV_REG_INIT       0x2022  //
#define HI_DMID_MAC_PHY_DRV_REG_ERR        0x2023  //
#define HI_DMID_KDBG_RC                    0x2030  // HI_DMID_KDBG_RC_S
#define HI_DMID_KDBG_RPT                   0x2031  // HI_DMID_KDBG_RPT_S

#define HI_DMID_MAC2DMS_FRAME              0x2050
#define HI_DMID_DMS2MAC_FRAME              0x2051
#define HI_DMID_MAC2DMS_MSG                0x2052

typedef struct
{
    HI_U32 ulUpgId;
}HI_DMID_UPG_REQ_START_S;


typedef struct
{
    HI_U32 ulUpgId;
}HI_DMID_UPG_REQ_STOP_S;


typedef struct
{
    HI_U32 ulUpgId;
}HI_DMID_UPG_REQ_VER_QRY_S;

typedef struct
{
    HI_U32 ulUpgId;
}HI_DMID_UPG_REQ_STATUS_S;


typedef struct
{
    HI_U32 ulUpgId;
}HI_DMID_UPG_BROSTART_START_S;

typedef struct
{
    HI_U32 ulUpgId;
}HI_DMID_UPG_BROSTART_STOP_S;

typedef struct
{
    HI_U32 ulUpgId;
}HI_DMID_UPG_BROSTART_TRANSMIT_S;

typedef struct
{
    HI_U32 ulUpgId;
}HI_DMID_UPG_BROSTART_STATUS_IND_S;

typedef struct
{
    HI_U32 ulUpgId;
}HI_DMID_UPG_BROSTART_RESULT_S;


typedef struct
{
    HI_U32 ulUpgId;
}HI_DMID_UPG_UNICAST_START_S;

typedef struct
{
    HI_U32 ulUpgId;
}HI_DMID_UPG_UNICAST_STOP_S;

typedef struct
{
    HI_U32 ulUpgId;
}HI_DMID_UPG_UNICAST_TRANSMIT_S;
//*****************************************************************************



//*****************************************************************************
// System ���������
//*****************************************************************************
// ��Χ [0xA001, 0xB000)
// ������ӱ����ǰ�˳�����, �Ҷ��ڴ�СС�� HI_DCID_SYS_CMD_FIXED_LEN
//*****************************************************************************
#define HI_DCID_UPG_CHL_CFG          0xA000    // HI_DCID_UPG_CHL_CFG_S
#define HI_DCID_SAL_DBG_SWT_CFG      0xA001    // HI_DCID_SAL_DBG_SWT_CFG_S
#define HI_DCID_STR                  0xA010
#define HI_DCID_SYS_NUM              4         // �����������,�����������Ҫ�����ú궨���С,���±���SAL

#define HI_DCID_SYS_CMD_FIXED_LEN    64        // ��������ĳ������Ϊ64
#define HI_DCID_SYS_BUFFER_LEN       (HI_DCID_SYS_CMD_FIXED_LEN*HI_DCID_SYS_NUM) // ������������ݽṹ��С�ܺ�

//*****************************************************************************
typedef HI_NV_FTM_UPG_CFG_S HI_DCID_UPG_CHL_CFG_S;

typedef struct
{
    HI_U8 cfg[8];   // [0]: mac->dms; [1]: dms->mac; [2]: dump send 2 tcp port time
    HI_U32 data[8]; // [0]: cmd id
}HI_DCID_SAL_DBG_SWT_CFG_S;
//*****************************************************************************

//*****************************************************************************
// ��Χ [0xB050, 0xB200)
//*****************************************************************************
// HI_MDM_EQT_CHL_PK               0xB000    // HI_MDM_EQT_CHL_PK_REQ_S  HI_MDM_EQT_CHL_PK_IND_S
#define HI_SYS_RESERVED            0xB001    // REQ, ACK, IND?


//*****************************************************************************
// ��ȡ������������ �� HI_DCID_UPG_CHL_CFG_S HI_DCID_SAL_DBG_CFG_S
// ulSysCid: ���������,��Χ [0xA001, 0xB000)
//*****************************************************************************
HI_EAPI HI_PVOID HI_SYS_GetSysCmdCfg(HI_U32 ulSysCid);
HI_EAPI HI_U16   HI_SYS_GetSysCmdCfgLen(HI_U32 ulSysCid);

//*****************************************************************************
// ϵͳ���� ע��
// ulSysCid: ���������,��Χ [0xB000, 0xB050)
// usCfgLen: ȡ֮ 0
//*****************************************************************************
HI_EAPI HI_U32 HI_SYS_RegisterSysCmdCfg(HI_U32 ulSysCid, HI_U16 usCfgLen);

//*****************************************************************************
// ϵͳ��������
// ulSysCid: ���������,��Χ [0xA001, 0xB000)
// pCfgData: ����buffer
// usCfgLen: ����buffer����
// bForce:  HI_TRUE: д��NV; HI_FALSE: ��д��NV,������
//*****************************************************************************
HI_EAPI HI_U32 HI_SYS_SetSysCmdCfg(HI_U32 ulSysCid, HI_U8* pCfgData, HI_U16 usCfgLen, HI_BOOL bForce);

#define GET_HI_DCID_UPG_CHL_CFG_S()       ((HI_DCID_UPG_CHL_CFG_S*)HI_SYS_GetSysCmdCfg(HI_DCID_UPG_CHL_CFG))
#define GET_HI_DCID_SAL_DBG_CFG_S()       ((HI_DCID_SAL_DBG_SWT_CFG_S*)HI_SYS_GetSysCmdCfg(HI_DCID_SAL_DBG_SWT_CFG))

//*****************************************************************************


//*****************************************************************************
// CMN ͳ����
// CMN  [0xA150, 0xA250)
//*****************************************************************************
#define HI_DSID_SYS_EVT_MSG           0xA150
#define HI_DSID_SYS_PERIOD_MSG        0xA151
#define HI_DSID_SYS_SDM_MSG           0xA152
#define HI_DSID_SYS_MRS_MSG           0xA153
#define HI_DSID_SYS_FA_MSG            0xA154
#define HI_DSID_SYS_BAT_MSG           0xA155
#define HI_DSID_SYS_NMLV_MSG          0xA156
#define HI_DSID_SYS_FPGA_MSG		  0xA157
#define HI_DSID_SYS_SD_MODE_DFX       0xA160
#define HI_DSID_SYS_DIAG_ASYNC        0xA161

#include <dfx_sys_sdm.h>
//*****************************************************************************


//*****************************************************************************
// CTS
//*****************************************************************************
#define HI_DBG_CTS_TEST              0x500C  // HI_DBG_CTS_Q_S, ,
#define HI_DBG_CTS_MAC_TEST     0x500D  // HI_DBG_CTS_MAC_TEST



//*****************************************************************************
typedef struct
{
    HI_U32 ulData;
}HI_DEMO_CMD_S;


#define HI_DBG_DEMO_TEST              0x500e  

//*****************************************************************************
HI_END_HEADER
#endif //__DFX_SYS_H__


