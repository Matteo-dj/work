//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : dfx_sys.h
//  版 本 号   : V1.0
//  作    者   : shenhankun/00130424
//  生成日期   : 2011-11-10
//  功能描述   : 系统级诊断包ID和对应参数结构定义
//
//  函数列表   : 无
//  修改历史   :
//  1.日    期 : 2012-08-20
//    作    者 : shenhankun/00130424
//    修改内容 : 创建文件
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
// 系统事件、消息、统计量范围定义, 详细参见 文档 <<Hi3911V100 软件系统级诊断方案设计说明书.docx>>
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
// 系统级初始化事件 HI_DEID_SYS_INIT_E、系统级错误事件(非系统死机)
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
    HI_DEID_DRV_FLASH_INIT_OK   = 22,               //FLASH初始化完成
    HI_DEID_DRV_UART_INIT_OK    = 23,               //UART初始化完成
    HI_DEID_DRV_HRTIMER_INIT_OK = 24,           //HRTIMER初始化完成
    HI_DEID_DRV_HRTIMER_DESTROY_OK = 25,        //HRTIMER去初始化
    HI_DEID_DRV_ETH_INIT_OK = 26,               //ETH初始化完成
    HI_DEID_DRV_ETH_REMOVE_OK = 27,         //ETH去初始化
    HI_DEID_DRV_UPG_INIT_OK = 28,               //UPG初始化完成
    HI_DEID_DRV_UPG_START_OK = 29,              //UPG开始升级
    HI_DEID_DRV_UPG_BACKUP_OK = 30,             //UPG版本回退
    HI_DEID_DRV_UPG_STOP_OK = 31,               //UPG升级停止
    HI_DEID_DRV_UPG_TIMEOUT_FINISH_OK = 32, //UPG超时重启
    HI_DEID_DRV_UPG_APPCALL_FINISH_OK = 33, //UPG回调重启
    HI_DEID_DRV_DFX_INIT_OK = 34,               //DFX初始化完成

    HI_DEID_DRV_SPI_INIT_OK = 35,
    HI_DEID_DRV_CRC_INIT_OK = 36,
    HI_DEID_DRV_GPIO_INIT_OK = 37,
    HI_DEID_DRV_CIPHER_INIT_OK = 38,
    HI_DEID_DRV_LED_INIT_OK = 39,
    HI_DEID_DRV_IR_INIT_OK  = 40,
    HI_DEID_DRV_I2C_INIT_OK = 41,
    HI_DEID_DRV_DMA_INIT_OK = 42,
    HI_DEID_DRV_UPG_INIT_FAIL = 43,             //UPG 失败
    HI_DEID_DRV_NV_UNALIGN = 44,
    HI_DEID_DRV_INIT_MAX    = 50,

    //***********BEGIN OF MAC DEF********************************
    // 以下定义MAC协议栈各模块初始化事件
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
    HI_DEID_SYS_INIT_FLASH_CFG_ERR  = 201, // Flash配置错误,Block大小配置和设备不一致
    HI_DEID_SYS_INIT_OPEN_DEV_ERR   = 202, // 打开设备失败
    HI_DEID_SYS_INIT_DEV_CFG_ERR    = 203, // 获取设备配置失败
    HI_DEID_SYS_INIT_HW_CFG_ERR     = 204, // 获取硬件配置失败
    HI_DEID_SYS_INIT_CFG_MEM_ERR    = 205, // 获取配置内存错误, 使用默认内存, 进入最小运行的调试模式
    HI_DEID_SYS_INIT_MEM_FAIL       = 206,
    HI_DEID_SYS_INIT_R_UPG_CFG_FAIL = 207,
    HI_DEID_SYS_INIT_VERIFY_MSG     = 208,

    HI_DEID_SYS_PLC_MODE            = 220,

    //USR初始化事件 [230,255)
    HI_DEID_USR_INIT_EVNET_START             = 230,
    HI_DEID_USR_INIT_EVNET_END               = 255,
    HI_DEID_INIT_MAX = 255// 必须不能大于 256
}HI_DEID_SYS_INIT_E;

//*****************************************************************************
// 系统级错误/告警/重要的事件定义 [256, 1024)
//*****************************************************************************
#define HI_DEID_SAL_SYS_EVT_START  (HI_DEID_INIT_MAX+1) // 256
typedef enum
{
    // 系统事件: [256, 306)
    HI_DEID_SAL_SYS_MIN = HI_DEID_SAL_SYS_EVT_START, // 256
    HI_DEID_SAL_DIAG_DUMP_END,
    HEID_SYS_STATIC_MEM_NOT_ENOUGH,
    HEID_SYS_DIAG_FRM_ERR = 304,
    // TODO
    HI_DEID_SAL_SYS_MAX = HI_DEID_SAL_SYS_EVT_START + 49,

    // DRV 事件: [306, 512)
    HI_DEID_DRV_SYS_MIN = HI_DEID_SAL_SYS_EVT_START + 50,
    HI_DEID_DRV_UPG_MSG,                //UPG初始化上报消息
    HI_DEID_DRV_UPG_UPDATE_COMPLETE,    //UPG 升级完成
    HI_DEID_DRV_UPG_UPDATE_CRC_OK,  //UPG校验通过
    HI_DEID_DRV_UPG_UPDATE_NOTIFY,  //UPG升级完成回调
    HI_DEID_DRV_UPG_MALLOC_ERR,     //UPG MALLOC不成功
    HI_DEID_DRV_FLASH_WAIT_TIMEOUT,     //flash 等待芯片空闲超时
    HI_DEID_DRV_FLASH_OP_ADDR_BEYOND,   //flash 操作地址越界
    HI_DEID_DRV_SYS_MAX = HI_DEID_SAL_SYS_EVT_START + 255,

    // [512, 768)
    HI_DEID_MAC_SYS_MIN = HI_DEID_SAL_SYS_EVT_START + 256,
    // TODO
    //网络管理事件定义
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

    // APP事件 [768, 960)
    HI_DEID_APP_SYS_MIN = HI_DEID_SAL_SYS_EVT_START + 512,
    HI_DEID_APP_GET_METER_ADDR_FAIL,
    EID_APP_SEND_MSG_FAIL,

    //USR系统事件[960,1024)
    HI_DEID_USR_SYS_EVENT_START = 960,
    HI_DEID_USR_SYS_EVENT_END = 1023,


    HI_DEID_ERR_MAX     = 1024
}HI_DEID_SYS_E;


//*****************************************************************************
#define HI_STAT_ID_NUM_MAX       (HI_DSID_MAX-HI_DFX_STAT_ID_BASE_SYS)
//*****************************************************************************

//*****************************************************************************
// SAL 统计量定义
//*****************************************************************************
// 范围 [A000, A050)
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
    HI_U32 ulFlashAvailSize;//废弃
    HI_U32 ulFlashAddr;//废弃
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
// DRV统计量定义
//*****************************************************************************
// 范围 [0xA050, 0xA150)
//*****************************************************************************
#define HI_DSID_DRV_XXX      0xA050  // HI_DSID_DRV_XXX_S
#define HI_DSID_DRV_FLASH_INFO      0xA051  // FLASH统计信息
#define HI_DSID_DRV_SPI_INFO        0xA054  // SPI统计信息
#define HI_DSID_DRV_HRT_INFO        0xA057  //HRT统计信息
#define HI_DSID_DRV_ETH_INFO        0xA058  //ETH统计信息
#define HI_DSID_DRV_RF_ERR          0xA059  //RF GPIO SPI 统计信息


// TODO
//*****************************************************************************


//*****************************************************************************
// 结构定义
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
    HI_U32 tx_time_us; //发送时系统时间值
    HI_U32 tx_timer_us;//发送时定时器的设置值
    HI_U32 tx2rx_max_us;//发送转接收最长时间
    HI_U32 tx2rx_max_us_timestamp;//发送转接收最长时间的发送时刻
}HI_DSID_DRV_UART_INFO_S;

typedef struct
{
    HI_U32 ulOldAddr;//HSO抢占：被抢占IP地址
    HI_U32 ulNewAddr;//HSO抢占：抢占IP地址
    HI_U32 second;//HSO抢占：时刻(s)
}HI_DSID_SAL_HSO_GRAB_CONNECT_INFO_S;
#define HI_DSID_SAL_HSO_GRAB_CONNECT_INFO_COUNT 3//最多可记录的抢占记录次数

typedef struct spi_init_info
{
    HI_U32 baudrate;  //波特率
    HI_U32 framemode; //传输模式
    HI_U32 datawidth; //数据位宽
    HI_U32 spo;       //moto的spi模式时SPICLKOUT相位
    HI_U32 sph;       //moto的spi模式时SPICLKOUT极性
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
    HI_U32 ulRxCnt;   //接收包的个数
    HI_U32 ulTxCnt;  //发送包的个数
    HI_U32  ulTxErrCnt; //发送失败计数
    HI_U16   BufTotal;    //总的BUFFER个数
    HI_U16   BufUsed;   //当前使用个数
}HI_DSID_DRV_ETH_INFO_S;

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 ulTimerAddCnt; //添加TIMER的个数
    HI_U32 ulTimerDelCnt; //删除TIMER的个数
    HI_U32 ulTimerTimeoutCnt;    //到期TIEMR的个数
}HI_DSID_DRV_HRT_INFO_S;

//*****************************************************************************
// DRV消息定义
//*****************************************************************************
// 范围 [0x0150, 0x0250)
//*****************************************************************************
#define HI_DMID_DRV_UPG_ERR_READ_NV                 0x0150      //UPG读NV出错
#define HI_DMID_DRV_UPG_ERR_WRITE_NV                0x0151      //UPG写NV出错
#define HI_DMID_DRV_UPG_ERR_READ_FLASH              0x0152  //UPG读FLASH出错
#define HI_DMID_DRV_UPG_ERR_WRITE_FLASH             0x0153  //UPG写FLASH出错
#define HI_DMID_DRV_UPG_ERR_BUSY                    0x0154  //UPG 正在升级
#define HI_DMID_DRV_UPG_ERR_START_PARA              0x0155  //UPG START 参数错误
#define HI_DMID_DRV_UPG_ERR_UPDATE_PARA     0x0156  //UPG UPDATE 参数错误
#define HI_DMID_DRV_UPG_ERR_UPDATE_VERSION      0x0157  //UPG 非升级文件或版本不对
#define HI_DMID_DRV_UPG_ERR_UPDATE_CRC            0x0158    //UPG CRC校验错
#define HI_DMID_DRV_FLASH_PROBE         0x0170  // FLASH芯片信息探测
#define HI_DMID_DRV_FLASH_OP            0x0171  // FLASH操作信息

#define HI_DMID_DRV_UART_CHANGE_PARA    0x0180  // UART参数修改

#define HI_DMID_DRV_SPI_TIMEOUT         0x0190  // SPI操作超时
//*****************************************************************************


//*****************************************************************************
// 结构定义
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
// MAC统计量定义
//*****************************************************************************
// 范围 [A250, A450)
// 1， 组网业务消息  [0xA250~0xA270)
// 2,  路由          [0xA270~0xA290)
// 3,  发送机        [0xA290~0xA2B0)
// 4,  分段器        [0xA2B0~0xA2D0)
// 5,  PHY           [0xA2D0~0xA2F0)
// 6,  重组器        [0xA2F0~0xA310)
// 7,  beacon        [0xA310~0xA330)
// 8,  conv          [0xA330~0xA350)
// 9,  RX            [0xA350~0xA370)
// 10, NTB           [0xA370~0xA390)
// 11, 抄控器        [0xA390~0xA3A0)
// 12, 抄控器二次开发[0xA3A0~0xA3B0)
// 13, DFX           [0xA3B0~0xA3C0)
// 14, PLC_IP        [0xA3C0~0xA3D0)
// 15, FA            [0xA3D0~0xA3E0)
// 16, sounding      [0xA3E0~0xA3F0)
// 17  保留          [0xA3F0~0xA450)
//*****************************************************************************
//*****************************************************************************
// TODO
// 注意ID号的定义一定要按照 文档 要求来定义。
// ID:HI_DSID_XXX_XXX;
// 对应的结构体: HI_DSID_XXX_XXX_S;
//*****************************************************************************

//PHY的统计量定义
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

#define HI_DSID_PHY_OM_LEVEL0_MAX_NUM                5           //PHY的异常信息的保存的最大数量

//结构体定义
//L1COMMON内部的可维可测计数变量
typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 NvIdError;
    HI_U32 AfeError;
    HI_U32 NvReadError;
    HI_U32 InnerQueryIdError;
    HI_U32 SpiTimeOutError;
    HI_U32 GetHwIdError;                //获取硬件ID失败的次数
    HI_U32 PowerStepError;              //配置数字发射功率错误的次数
    HI_U32 LastHwId;                    //最近一次获取的硬件ID

    HI_U16 malloc_err_cnt;              //内存分析失败计数
    HI_U16 pad1;

    HI_U32 pad[3];
} HI_DSID_PHY_L1COMMON_ERR_S;

//TX内部的可维可测变量
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U32 CopyNumError;
    HI_U32 TryFreqError;                //频段号错误的计数
    HI_U32 FreqNotAllowedCnt;           //不允许切换频段的次数
    HI_U32 FreqSameCnt;                 //并行检测频段一致，未切换的次数

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

    HI_U32 pad3;                        //频段通道1因其它频段通道而被设置的次数
    HI_U32 freqChangeCnt;               //频段切换次数

    HI_U32 freqChlChangeCnt;            //发送频段通道切换次数
    HI_U16 pad1;                        //频段组合错误计数
    HI_U8  curTxFreq;                   //当前的发送频段
    HI_U8  curChl1Freq;                 //并行检测通道1的频段号

    HI_U8  pad2[8];                     //当前的接收频段组合

    HI_U32 freqUnsupportCnt;            //发送报文时不支持的频段计数

    // plc_sc 统计量信息
    HI_U32 PlcScTxOverIntCnt;
    HI_U32 PlcScRxOverIntCnt;
    HI_U32 PlcScCheckOkCnt;
    HI_U32 PlcScCheckFailCnt;
    HI_U32 PlcScSenseIntCnt;
    HI_U32 PlcScCrcErrCnt;
} HI_DSID_PHY_TX_ERR_S;

//Bitloading的内部的可维可测变量
typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 MaxBitError;
    HI_U32 BitloadingToneMapError;
} HI_DSID_PHY_BITLOADING_ERR_S;

//OM的内部错误计数
typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 ReqIdError;                  //HSO下发的查询ID错误
    HI_U32 ReportError;                 //可维可测信息上报错误
}HI_DSID_PHY_OM_ERR_S;

//AGC异常
typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 ReportCnt;                   //上报次数统计
    HI_U32 AdAvgPower;                  //AD平均功率异常
    HI_U32 SatRate;                     //饱和率
    HI_U32 AfePower;                    //AFE的信号功率
    HI_U32 PowerAfterFilter;            //滤波后信号的功率
}HI_DSID_PHY_AGC_ERR_S;

//NBI异常
typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 ReportCnt;                   //上报次数统计
    HI_U32 NbiCnt;                      //检测的NBI数
    HI_U32 NbiOutOfNf;                  //超过NF处理带宽的NBI
}HI_DSID_PHY_NBI_ERR_S;

//符号同步异常
typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 ReportCnt;                   //上报次数统计
    HI_U32 FirstToMaxPathLen;           //首径与最大径的距离
    HI_U32 PataLen;                     //径长度
}HI_DSID_PHY_SYMBOL_ALIGN_ERR_S;

//信道质量异常
typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 ReportCnt;                   //上报次数统计
    HI_U32 Par;                         //峰均比
    HI_U32 AvgSnr;                      //平均snr
}HI_DSID_PHY_CHANNEL_INFO_ERR_S;

//Clock异常Preamble上报
typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 ReportCnt;
    HI_U32 PreambleFreqErr;             //Preamble频偏
    HI_U32 PllFreqErr;                  //Pll的频偏
    HI_U32 PreambleCarrierNum;          //Preamble载波个数
    HI_U32 CntSingu;                    //clk_est模块的连续异常次数
}HI_DSID_PHY_CLOCK_PREAMBLE_ERR_S;

//Clock异常接收完成中断上报
typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 ReportCnt;                   //上报次数统计
    HI_U32 PilotFreqErr;                //Pilot频偏
    HI_U32 SoundFreqErr;                //Sounding的频偏
    HI_U32 PilotCarrierNum;             //Pilot载波数
}HI_DSID_PHY_CLOCK_RX_END_ERR_S;

//NTB异常
typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 ReportCnt;                   //上报次数统计
    HI_U32 BtsLtmrFreqErr;              //用相邻两个BTS与LTmr计算获得的偏差
    HI_U32 DeltaFreqErr;                //NTB的前后两次频偏差
    HI_U32 CntSingu4NTB;                //当前状态下的异常值个数
}HI_DSID_PHY_NTB_ERR_S;

//IMP异常
typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 ReportCnt;                   //上报次数统计
    HI_U32 ImpCnt;                      //每个统计周期StatiPeriod上报
}HI_DSID_PHY_IMP_ERR_S;

//发送时获取信号量的异常统计
typedef struct
{
    HI_U32 CollectDataLockCnt;                    //采数锁定计数
    HI_U32 CollectDataUnlockCnt;                  //采数解锁计数

    HI_U32 TxEnConflict;                          //发送冲突计数
    HI_U32 pad1;                  //采数中发送完成计数

    HI_U32 TxSuccessCnt;
    HI_U32 pad2;                           //发送完成计数
    HI_U32 BeaconErrCnt;
    HI_U32 SofErrCnt;

    HI_U32 pad3[4];
    HI_U32 pad4[3];
}HI_DSID_PHY_TX_SEM_STATISTIC_S;

//网络管理模块统计量定义
#define NM_STATISTIC_HEAD_OFFSET 0xA250
//#define HI_DSID_MAC_NM_FA_STAT_INFO                   (NM_STATISTIC_HEAD_OFFSET + 0x02)

//重组器模块统计量定义
#define RSB_STATISTIC_HEAD_OFFSET 0xA2F0
#define HI_DSID_MAC_RSB_MPDU_INFO_CNT      (RSB_STATISTIC_HEAD_OFFSET)
#define HI_DSID_MAC_RSB_NODE_INFO_CNT      (RSB_STATISTIC_HEAD_OFFSET + 0x01)
#define HI_DSID_MAC_RSB_ABNORMITY_INFO     (RSB_STATISTIC_HEAD_OFFSET + 0x03)

//分段器模块统计量定义
#define SEG_STATISTIC_HEAD_OFFSET 0xA2B0
#define HI_DSID_SEG_FRAME_CNT_IN_DIFF_TXQUEUE       SEG_STATISTIC_HEAD_OFFSET
#define HI_DSID_MAC_SEG_ABNORMITY_INFO              (SEG_STATISTIC_HEAD_OFFSET + 0x01)      //分段模块异常

//发送机统计量定义
#define TX_STATISTIC_HEAD_OFFSET 0xA290
#define HI_DSID_MAC_TX_MPDU_SEND_COUNT      TX_STATISTIC_HEAD_OFFSET
#define HI_DSID_MAC_TX_ALL_QUEUE_INFO       TX_STATISTIC_HEAD_OFFSET + 0x1
#define HI_DSID_MAC_TX_QUEUE_INFO           TX_STATISTIC_HEAD_OFFSET + 0x2
#define HI_DSID_MAC_TX_ABNORMITY_INFO       (TX_STATISTIC_HEAD_OFFSET + 0x03)
#define HI_DSID_MAC_TX_EVENT_INFO           (TX_STATISTIC_HEAD_OFFSET + 0x04)

// 抄控器模块统计量
#define NDM_STATISTIC_HEAD_OFFSET 0xA390
#define HI_DSID_MAC_NDM_CONNECT_STAT_INFO   NDM_STATISTIC_HEAD_OFFSET
#define HI_DSID_MAC_NDM_ABNORMITY_INFO      (NDM_STATISTIC_HEAD_OFFSET + 0x01)      //抄控器的异常信息
#define HI_DSID_MAC_NDM_STAT_INFO           (NDM_STATISTIC_HEAD_OFFSET + 0x02)      //抄控器的异常信息
#define HI_DSID_MAC_NDM_REMOTE_INFO         (NDM_STATISTIC_HEAD_OFFSET + 0x03)      //抄控器的远程通道信息
#define HI_DSID_MAC_NDM_HEARTBEAT_INFO      (NDM_STATISTIC_HEAD_OFFSET + 0x04)      //抄控器的心跳统计信息
#define HI_DSID_MAC_NDM_METER_CHECK_INFO    (NDM_STATISTIC_HEAD_OFFSET + 0x05)      //整表测试统计信息
//抄控器二次开发通道统计量
#define HI_DSID_MAC_MDM_REGIST_INFO         (NDM_STATISTIC_HEAD_OFFSET + 0x10)      //抄控器二次开发通道注册
//二代抄控器NDC通道统计量
#define HI_DSID_MAC_NDC_STAT_INFO           (NDM_STATISTIC_HEAD_OFFSET + 0x11)      //二代抄控器通道注册
#define HI_DSID_MAC_NFC_STAT_INFO           (NDM_STATISTIC_HEAD_OFFSET + 0x12)      //抄控器报文流控通道信息
#define HI_DSID_MAC_NDC_OBJ_STAT_INFO       (NDM_STATISTIC_HEAD_OFFSET + 0x13)      //抄控器多用户
#define HI_DSID_MAC_NDC_SEARCH_STAT_INFO    (NDM_STATISTIC_HEAD_OFFSET + 0x14)      //抄控器搜网通道信息注册
//dfx模块统计量
#define DFX_STATISTIC_HEAD_OFFSET           0xA3B0
#define HI_DSID_MAC_DFX_SYS_ERR             (DFX_STATISTIC_HEAD_OFFSET + 0x00)     //dfx模块内部统计
#define HI_DSID_MAC_DFX_STAT_INFO           (DFX_STATISTIC_HEAD_OFFSET + 0x01)     //dfx模块业务流程统计


//PLC的IP模块的统计量
#define PLC_IP_STATISTIC_HEAD_OFFSET        0xA3C0
#define HI_DSID_MAC_PLC_IP_STAT_INFO        (PLC_IP_STATISTIC_HEAD_OFFSET + 0x00)   //plc_ip模块的业务统计

//rx模块统计量定义
#define RX_STATISTIC_HEAD_OFFSET            0xA350
#define HI_DSID_MAC_RX_ABNORMITY_INFO       (RX_STATISTIC_HEAD_OFFSET + 0x00)
#define HI_DSID_MAC_RX_ITRUPT_INFO          (RX_STATISTIC_HEAD_OFFSET + 0x01)
#define HI_DSID_MAC_RX_STAT_INFO            (RX_STATISTIC_HEAD_OFFSET + 0x02)

//fa模块的统计量
#define FA_STATISTIC_HEAD_OFFSET            0xA3D0
#define HI_DSID_MAC_FA_STAT_INFO            (FA_STATISTIC_HEAD_OFFSET + 0x00)       //fa模块的流程统计量
#define HI_DSID_MAC_FA_APP_INFO             (FA_STATISTIC_HEAD_OFFSET + 0x01)       //fa模块与APP交互流程统计量

//fa模块与APP交互流程统计量
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
    HI_U16 GetCalendarTime_err_cnt;                 //获取实时时钟失败

    HI_U32 check_realtime_timer_success_cnt;        //检测实时时钟成功次数
    HI_U32 pad3[7];
}HI_DSID_MAC_FA_APP_INFO_S;

//fa模块的流程统计量
typedef struct
{
    HI_U32 check_ping_timer_cnt;                    //检测测试报文超时的定时器
    HI_U32 check_realtime_timer_cnt;                //检测实时时钟的定时器
    HI_U32 affirm_network_state_timer_cnt;          //确认网络状态的定时器
    HI_U32 affirm_offline_timer_cnt;                //确保站点离线的定时器

    HI_U16 unknown_timer_cnt;                       //未知的定时器
    HI_U16 unexpect_state_timer_cnt;                //状态不符合预期的定时器回调

    HI_U32 nm_notify_callback_cnt;                  //网管通知回调的计数
    HI_U32 nm_notify_fa_white_list_change_cnt;      //白名单变化
    HI_U32 nm_notify_fa_mac_chl_state_cnt;          //mac通道状态变化
    HI_U32 nm_notify_fa_sta_join_cnt;               //有站点入网

    HI_U32 nm_notify_fa_freq_change_cnt;            //频段变更通知

    HI_U16 unknown_nm_notify_cnt;                   //未知的网管通知
    HI_U8  nm_notify_last_whist_list_change_reason; //网管上一次通知白名单变化的原因
    HI_U8  nm_notify_last_freq;                     //网管上一次通知的频段

    HI_U8  nm_notify_last_chl_state;                //网管上一次通知的通道状态
    HI_U8  recover_unexpect_err;                    //紧急恢复通关状态的次数
    HI_U16 stop_evaluate_err_cnt;                   //停止频段自适应失败

    HI_U16 calc_cur_freq_reslut_addr_err_cnt;       //内存地址非法
    HI_U16 handle_rcv_ping_pkt_msg_addr_err_cnt;    //内存地址非法
    HI_U16 handle_check_ping_timer_addr_err_cnt;    //内存地址非法
    HI_U16 handle_rcv_ping_pkt_msg_state_err_cnt;   //在非报文测试状态收到测试报文的计数

    HI_U16 handle_rcv_ping_pkt_msg_rcv_more_delay_cnt;      //接收成功的延时超过阈值
    HI_U16 handle_rcv_ping_pkt_msg_unexpect_tei_cnt;        //接收的测试报文的不在接收队列
    HI_U16 handle_rcv_ping_pkt_msg_unexpect_turn_cnt;       //接收的测试报文的轮次不符合预期
    HI_U16 handle_check_ping_timer_tei_miss_cnt;    //tei缺失

    HI_U32 handle_check_ping_timer_overtime_cnt;    //应答超时的次数

    HI_U16 send_fa_msg_err_cnt;                     //发送FA的消息失败
    HI_U16 start_fa_timer_err_cnt;                  //启动定时器失败
    HI_U16 whist_list_diff_num;                     //白名单变化的个数
    HI_U16 app_set_business_state_cnt;              // 设置的app业务次数

    HI_U8  app_set_business_state;                  //设置的app业务状态
    HI_U8  pad;
    HI_U16 freq_index_err_cnt;                      //频段号错误

    HI_U16 chl_state_notify_err_cnt;                //通道状态通知错误次数
    HI_U16 malloc_fail_cnt;                         //分配动态内存失败

    HI_U16 save_evaluate_result_err_cnt;            //存储评估结果失败次数
    HI_U16 set_mac_chl_state_err_cnt;
}HI_DSID_MAC_FA_STAT_INFO_S;

//Sounding模块的统计量
#define SD_STATISTIC_HEAD_OFFSET            0xA3E0
#define HI_DSID_MAC_SD_START_INFO           (SD_STATISTIC_HEAD_OFFSET + 0x00)       //sounding启动统计量

//sounding启动统计量
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
    HI_U32 send_rcv_tonemap_success_cnt;        //发送本端接收时使用的ToneMap成功计数
    HI_U32 update_tx_tonemap_item_cnt;          //更新发送ToneMap

    HI_U16 rcv_tx_tonemap_bit_sum_invalid_cnt;
    HI_U16 tx_tonemap_next_hop_err_cnt;

    HI_U32 robo_success_in_norobo_cnt;
    HI_U32 norobo_success_cnt;

    HI_U32 pad2[10];
}HI_DSID_MAC_SD_START_INFO_S;

//plc_ip模块的业务统计
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

//分段异常统计
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

//接收异常统计
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U32 old_rcv_half_new_rcv_cnt;            //上一次接收未完成，有新的FC到来
    HI_U32 rcv_fc_no_buff_cnt;                  //收到FC，但无buff可用
    HI_U32 rcv_end_half_sof_cnt;                //sof未接收完整，但收到接收完成中断
    HI_U32 decode_result_err_cnt;               //读取译码结果错误计数
}HI_DSID_MAC_RX_ABNORMITY_INFO_S;

//RX中断统计
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U32 itrupt_cnt;                      //中断总数统计
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
    HI_U32 cp_num_itrupt_cnt;               //V100的载波侦听相关峰维测上报中断
    HI_U32 tx_frame_err_cnt;
    HI_U32 tx_frame_suc_cnt;
}HI_DSID_MAC_RX_ITRUPT_INFO_S;

//RX接收状态统计
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U32 short_pb_ok_cnt;
    HI_U32 long_ok_cnt[4];

    HI_U32 short_pb_err_cnt;
    HI_U32 long_pb_err_cnt[4];
}HI_DSID_MAC_RX_STAT_INFO_S;

//发送异常统计
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U32 unknown_event_cnt;                   //未知事件
    HI_U32 malloc_err_cnt;                     //获取内存失败

}HI_DSID_MAC_TX_ABNORMITY_INFO_S;

//发送事件统计
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U32 tx_event_cnt;                //发送事件计数
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


//网络管理模块统计量结构体定义

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

//抄控器建立连接时的统计量
typedef struct
{
    HI_U16 pad1[7];
    HI_U16 malloc_memory_err;

    HI_U8  ndm_mac[4][HI_PLC_MAC_ADDR_LEN];

    HI_U16 ndm_tei;
    HI_U16 ndm_tei_err_cnt;

    HI_U16 ndm_mac_duplicate_cnt;
    HI_U16 pad2[6];
    HI_U16 send_nm_query_cnt;                       //发送的查询网络属性报文的统计

    HI_U16 send_nm_query_success_cnt;               //发送的查询网络属性报文成功的统计
    HI_U16 get_ndm_proxy_err_cnt;                   //获取抄控器的代理站点失败

    HI_U16 find_join_sta_err_cnt;                   //查询发现站点失败
    HI_U16 rcv_nm_attr_cnt;                         //接收到的网络属性回复报文
    HI_U16 rcv_nm_attr_success_cnt;                 //查询网络属性成功计数
    HI_U16 rcv_query_nm_attr_cnt;                   //接收到的网络属性查询报文计数

    HI_U16 send_nm_attr_cnt;                        //回复网络属性查询的计数
    HI_U16 rcv_hso_nm_query_cnt;                    //接收HSO的查询网络属性报文的统计

    HI_U32 rcv_nm_attr_success_time;                //抄控器查询网络属性成功的时间
    HI_U16 query_connect_info_malloc_err_cnt;       //上报网络基本属性获取内存失败
    HI_U8  search_net_state;
    HI_U8  pad;

    HI_U16 get_base_info_err_cnt;                   //获取网络基本信息失败
    HI_U16 rpt_base_info_err_cnt;                   //上报网络基本信息失败
    HI_U32 clear_cur_disc_sta_cnt;                  //抄控器当前代理不可用的次数统计

    HI_U16 ndm_proxy_tei;                           //抄控器当前的代理tei
    HI_U16 send_ndm_attr_br_cnt;                    //发送抄控器属性报文的统计
    HI_U16 rcv_ndm_attr_br_cnt;                     //接收抄控器属性报文的统计
    HI_U16 send_ndm_attr_br_err_cnt;                //发送抄控器属性报文时错误的统计
    HI_U16 search_net_start_cnt;                    //搜网启动次数
    HI_U16 search_net_end_cnt;                      //搜网停止次数
}HI_DSID_MAC_NDM_CONNECT_STAT_INFO_S;

//抄控器异常信息统计
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

//抄控器的业务统计信息
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
    HI_U16 rx_chl_duplicate_pk_cnt;         //接收的重复报文

    HI_U32 tx_remote_req_cnt;               //发送远程请求的个数
    HI_U32 rx_remote_rsp_cnt;               //接收的远程应答个数
    HI_U32 hso_remote_req_cnt;              //HSO发送的远程请求个数
    HI_U32 hso_remote_rsp_cnt;              //HSO接收的远程应答个数

    HI_U32 hso_req_rsp_err_cnt;
    HI_U32 refresh_disc_cnt;                //刷新抄控器的发现列表的次数
    HI_U16 refresh_disc_malloc_fail_cnt;    //刷新抄控器的发现列表的次数
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
    HI_U32 NvChangedNotify_cnt;                             //NV刷新回调次数
    HI_U32 MAC_SendMessage_unknown_id;                      //未知ID
}HI_DSID_MAC_NDM_STAT_INFO_S;

//抄控器远程通道的统计信息
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

//抄控器心跳机制统计信息
typedef struct
{
    HI_U32 handle_connect_msg_cnt;                      //接收到网管发送的连接报文消息的计数
    HI_U16 handle_connect_msg_para_err_cnt;             //接收的mac buffer为空指针
    HI_U16 handle_connect_msg_unknown_pkt_type_cnt;     //未知的报文类型

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

// 整表测试统计信息
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

    HI_U8  pad;           //可以删除
    HI_U8  ndm_meter_check_connect_flag;
    HI_U16 rcvd;

    HI_U8  meter_check_frame[16];                   //强制连接的整表测试帧
    HI_U32 pad3;
}HI_DSID_MAC_NDM_METER_CHECK_INFO_S;

//NDC通道统计信息
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U8  AppInitSuccessCnt;
    HI_U8  AppInitFailCnt;
    HI_U16 HandleNmReqFailCnt;

    HI_U32 HandleNmReqNormalSuccessCnt;
    HI_U32 HandleNmReqTopSuccessCnt;
    HI_U16 TopMallocFailCnt;                    //发查拓扑命令时获取内在失败
    HI_U16 TopMsgFailCnt;                       //发查拓扑命令时发送消息失败

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

// 抄控器NDC通道统计量
typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 NdcSendMsgFailCnt;
    HI_U32 NdcMallocFailCnt;
    HI_U32 NdcGetMacBufFailCnt;
    HI_U32 NdcInvalidCommandCnt;
    HI_U32 NdcReceiveCommandCnt;
    HI_U32 NdcSendTransCommandCnt;
    HI_U32 NdcReceiveTransCommandCnt;//通过NDM通道发送命令
    HI_U32 NdcSetTransCmdToMsduCnt;   //接收收发通道的MSDU报文
    HI_U32 NdcReceiveMsduFailCnt;
    HI_U32 NdcReceiveMsduCnt;
    HI_U32 NdcLocalBroadcastCnt;
    HI_U32 NdcUnicastCnt;
    HI_U32 NdcSendMsduFailCnt;
    HI_U32 NdcMacDevIdConfigCnt;
    HI_U32 NdcInnerCmdCallbackOverCnt;
    HI_U32 NdcOuterCmdCallbackOverCnt;
    HI_U32 NdcInnerConnectReqCnt;//处理内部连接请求
    HI_U32 NdcInnerConnectCnfCnt;//处理内部连接确认

    HI_U8   NdcPlcIp[HI_IPV4_ADDR_LEN];//plc的ip地址

    HI_U8   NdcVer;
    HI_U8   NdcRcvDevIdReqCnt;   // 接收集中器ID请求个数
    HI_U8   NdcDevId[6];

    HI_U8   NdcBcdBinId[5];
    HI_U8   NdcProdType;
    HI_U8   NdcProdTypeStaType;
    HI_U8   NdcRcvDevIdRspCnt;   // 接收集中器ID回应报文个数

    HI_U32 NdcInvalidMsgCnt;
    HI_U32 NdcMsgIDInvalidCnt;
    HI_U32 NdcMsgIDCmdRcvCnt;
    HI_U32 NdcMsgIDCmdSendCnt;
    HI_U32 NdcMsgIDDevIdCfgCnt;
}HI_DSID_MAC_NDC_STAT_INFO_S;
// 抄控器搜网统计参数
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

//抄控器通道报文流控信息
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

//搜网模块的二次开发接口统计量
typedef struct
{
    HI_U32 objCmdCnt;

    HI_U8  searchNetCtrlObjId;              //当前控制搜网的用户ID
    HI_U8  lastFreq;
    HI_U8  pad[2];

    HI_U32  lastSnid;

    HI_U32 closeWithInvalidSnid;
    HI_U32 invalidObjCmdCnt;                //非法用户的命令
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

//网管中与频段自适应功能相关的统计量
typedef struct
{
    HI_U16 set_chl_state_msg_err_cnt;                   //设置通道状态的消息失败次数
    HI_U16 set_chl_state_unexpect_cnt;                  //设置通道状态不符合预期的次数

    HI_U16 refresh_topo_msg_err_cnt;                    //发送更新拓扑的消息失败次数
    HI_U16 pre_white_list_sem_fail_cnt;                 //对前一个白名单的信号量操作失败

    HI_U16 malloc_fail_cnt;                             //保存白名单时获取内存失败
    HI_U16 save_pre_white_list_cover_cnt;               //保存前一个白名单时出现覆盖

    HI_U16 compare_white_list_change_null_cnt;          //比较白名单时出现空指针
    HI_U16 hso_set_white_list_msg_err_cnt;              //HSO设置白名单的消息失败
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

//重组器模块统计量结构体定义

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

//重组的异常统计
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U32 no_unrsb_free_node_cnt;
    HI_U32 notify_rsb_err_cnt;
    HI_U32 dlv_cvg_err_cnt;
}HI_DSID_MAC_RSB_ABNORMITY_INFO_S;

//分段器模块统计量结构体定义

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
    HI_U8  all_frame_count;                       //队列中所有数据帧的个数;
    HI_U8  highest_priority;                      //队列中报文的最高优先级;
    HI_U16 rsvd;
}HI_DSID_MAC_TX_ALL_QUEUE_INFO_S;

typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U8   lid;                    //连接标识符;
    HI_U8   q_frame_count;          //子队列中待发送的报文数目
    HI_U8   q_all_frame_count;      //子队列中所有的报文数目
    HI_U8   rsvd;
}HI_DSID_MAC_TX_QUEUE_INFO_S;
//DFX模块统计量定义
//dfx模块内部错误统计
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U32 get_sys_status_err_cnt;              //从NV获取系统工作模式错误
    HI_U32 sys_status;                          //系统的工作模式
    HI_U32 mpdu_node_null_cnt;                  //mpdu指针为空
    HI_U32 send_msg_err_cnt;                    //发送消息失败
    HI_U32 reg_msdu_feature_err_cnt;            //注册报文特征失败
    HI_U32 para_null_err_cnt;                   //参数为空指针
}HI_DSID_MAC_DFX_SYS_ERR_S;

//dfx模块流程统计
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U32 query_request_cnt;                   //HSO查询统计
    HI_U32 feature_match_cnt;                   //特征匹配成功
    HI_U32 no_feature_match_cnt;
    HI_U32 send_eng_event_err_cnt;

    HI_U32 msg_nv_refresh_notify_cnt;
    HI_U32 msg_dfx_ctrl_pk_cnt;
    HI_U32 msg_dfx_connect_pk_cnt;              //收到网管的抄控器通道连接报文
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
// MAC消息定义
//*****************************************************************************
// 范围 [0500, 0900)
// 1， 组网业务消息  [0x500~0x564)
// 2,  路由          [0x564~0x5c8)
// 3,  发送机        [0x5c8~0x62c)
// 4,  分段器        [0x62c~0x690)
// 5,  PHY           [0x690~0x6D0)
// 6,  重组器        [0x6D0~0x710)
// 7,  beacon        [0x710~0x750)
// 8,  conv          [0x750~0x790)
// 9,  RX            [0x790~0x7D0)
// 10, NTB           [0x7D0~0x810)
// 11, 抄控器        [0x810~0x830)
// 12, 频段自适应    [0x830~0x850)
// 13, 交换模块      [0x851~0x870]
// 14, EMC模块       [0x871~0x890]
// 15, 低功耗        [0x891~0x8A0]
// 16, 保留          [0x8A1~0x900)
//*****************************************************************************
// TODO
// 注意ID号的定义一定要按照 文档 要求来定义。
// ID:HI_DMID_XXX_XXX;
// 对应的结构体: HI_DMID_XXX_XXX_S;

//**************************************************
// 交换模块消息定时
// 交换模块消息范围 [0x851~0x870]
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
    HI_ETHERNET_HEADER_STRU eth_header;     // ETH帧头(16byte)
    HI_ARP_STRU arp_header;                 // ARP报文(28byte)
}DIAG_CMD_ARP_STRU;

#define PS_DFX_TCP_DATA_SIZE            (72)
typedef struct
{
    HI_ETHERNET_HEADER_STRU eth_header;     // ETH帧头(16byte)
    HI_IP_HEADER_STRU ip_header;            // IP报文头(20byte)
    HI_TCP_HEADER_STRU tcp_header;          // TCP报文头(20byte)
    HI_U8 data[PS_DFX_TCP_DATA_SIZE];
}DIAG_CMD_TCP_STRU;

#define PS_DFX_UDP_DATA_SIZE            (84)
typedef struct
{
    HI_ETHERNET_HEADER_STRU eth_header;     // ETH帧头(16byte)
    HI_IP_HEADER_STRU ip_header;            // IP报文头(20byte)
    HI_UDP_HEADER_STRU udp_header;          // UDP报文头(8byte)
    HI_U8 data[PS_DFX_UDP_DATA_SIZE];
}DIAG_CMD_UDP_STRU;

typedef struct
{
    HI_ETHERNET_HEADER_STRU eth_header;     // ETH帧头(16byte)
    HI_IP_HEADER_STRU ip_header;            // IP报文头(20byte)
    HI_ICMP_HEADER_STRU icmp_header;        // ICMP报文头(4byte)
}DIAG_CMD_ICMP_STRU;

typedef struct
{
    HI_ETHERNET_HEADER_STRU eth_header;     // ETH帧头(16byte)
    HI_IP_HEADER_STRU ip_header;            // IP报文头(20byte)
    HI_IGMP_HEADER_STRU igmp_header;        // IGMP报文头(8byte)
}DIAG_CMD_IGMP_STRU;

#define PS_DFX_ETH_DATA_SIZE            (112)
typedef struct
{
    HI_ETHERNET_HEADER_STRU eth_header;     // ETH帧头(16byte)
    HI_U8 data[PS_DFX_ETH_DATA_SIZE];
}DIAG_CMD_ETH_STRU;

#define PS_DFX_IP_DATA_SIZE             (92)
typedef struct
{
    HI_ETHERNET_HEADER_STRU eth_header;     // ETH帧头(16byte)
    HI_IP_HEADER_STRU ip_header;            // IP报文头(20byte)
    HI_U8 data[PS_DFX_IP_DATA_SIZE];
}DIAG_CMD_IP_STRU;


//频段自适应消息定义
//频段自适应    [0X830~0X850)
#define HI_DMID_FA_OFFSET                           0x830
#define HI_DMID_FA_START_EVALUATE                   (HI_DMID_FA_OFFSET + 0x00)  //启动评估
#define HI_DMID_FA_STOP_EVALUATE                    (HI_DMID_FA_OFFSET + 0x01)  //停止评估
#define HI_DMID_FA_EVALUATE_STATE                   (HI_DMID_FA_OFFSET + 0x02)  //评估的中间状态

#define HI_DMID_TF_RECEIVE_NTB                      (HI_DMID_FA_OFFSET + 0x03)  //台区识别接收NTB上报
#define HI_DMID_TF_DETECT_SEQUENCE                  (HI_DMID_FA_OFFSET + 0x04)  //台区识别检出序列上报
#define HI_DMID_TF_RESULT_REQ                       (HI_DMID_FA_OFFSET + 0x05)  //CCO发送的识别结果查询
#define HI_DMID_TF_RESULT_RSP                       (HI_DMID_FA_OFFSET + 0x06)  //CCO收到的识别结果应答
#define HI_DMID_TF_SOFT_INFO                        (HI_DMID_FA_OFFSET + 0x07)  //台区识别soft信息
#define HI_DMID_TF_SOFT_SEQ_INFO                    (HI_DMID_FA_OFFSET + 0x08)  //台区识别soft信息

#define HI_DMID_PI_ZERO_CROSS_NTB_REQ               (HI_DMID_FA_OFFSET + 0x09)  //CCO发送的过零NTB查询
#define HI_DMID_PI_ZERO_CROSS_NTB_RSP               (HI_DMID_FA_OFFSET + 0x0A)  //CCO收到的过零NTB查询的应答
#define HI_DMID_TF_START                            (HI_DMID_FA_OFFSET + 0x0B)  //台区识别启动
#define HI_DMID_TF_STOP                             (HI_DMID_FA_OFFSET + 0x0C)  //台区识别停止
#define HI_DMID_TF_SEND_SEQ                         (HI_DMID_FA_OFFSET + 0x0D)  //启动台区序列发送
#define HI_DMID_TF_QUERY_ONE_TURN_OVER              (HI_DMID_FA_OFFSET + 0x0E)  //查询一轮结束
#define HI_DMID_TF_SWITCH_PHASE                     (HI_DMID_FA_OFFSET + 0x0F)  //启动序列发送
#define HI_DMID_PI_CCO_STA_NTB_DIFF_REPORT          (HI_DMID_FA_OFFSET + 0x10)   //CCO与STA NTB差值统计上报
#define HI_DMID_TF_NDM_REAL_NOISE_REPORT            (HI_DMID_FA_OFFSET + 0x11)   // NDM上报实时扰动值
#define HI_DMID_TF_RESULT_REOPRT                    (HI_DMID_FA_OFFSET + 0x12)     // 结果上报
#define HI_DMID_TX_COLLISION_TEST_INFO              (HI_DMID_FA_OFFSET + 0x13)
#define HI_DMID_TF_NTB_POWER_REOPRT                 (HI_DMID_FA_OFFSET + 0x14)     // v200周期结果上报
#define HI_DMID_TF_LINE_DATA_REOPRT                 (HI_DMID_FA_OFFSET + 0x15)     // V200线路信息上报
#define HI_DMID_TF_NOISE_REPORT                     (HI_DMID_FA_OFFSET + 0x16)     // V200噪声上报
#define HI_DMID_TF_CCO_PHASE_CHECK_REPORT           (HI_DMID_FA_OFFSET + 0x17)     // V200的CCO相序检测
#define HI_DMID_NTB_FREQERR                               (HI_DMID_FA_OFFSET + 0x18)
#define HI_DMID_TX_COLLISION_RX_INFO               (HI_DMID_FA_OFFSET + 0x19)
#define HI_DMID_TFP_IDENITFY_RESULT                 (HI_DMID_FA_OFFSET + 0x1A)     // 上报单个站点识别结果
#define HI_DMID_TFP_NEIGHBOR_INFO                   (HI_DMID_FA_OFFSET + 0x1B)     // 上报每个站点方差计算结果
#define HI_DMID_TFP_SET_CMD_MODE                    (HI_DMID_FA_OFFSET + 0x1C)     // 启动，停止消息
#define HI_DMID_PN_RESULT                           (HI_DMID_FA_OFFSET + 0x1D)     // 零火线反接识别消息
#define HI_DMID_NOTIFY_START_COLLECT_NTB_INFO       (HI_DMID_FA_OFFSET + 0x1E)     // CCO通知STA进行NTB数据采集


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
//NDM相位切换消息
typedef struct
{
    HI_U8  send_phase_cnt;                  //发送相位计数
    HI_U8  send_edge_cnt;                   //发送沿计数
    HI_U8  pad1[2];

    HI_U32 time_stamp;              //时间戳，单位s

    HI_U32 pad[4];
}HI_DMID_TF_SWITCH_PHASE_S;


//启动台区序列发送消息
typedef struct
{
    HI_U32 net_identify_cnt;               //识别轮次
    HI_U32 time_stamp;                      //单位:s

    HI_U32 pad2[3];
}HI_DMID_TF_SEND_SEQ_S;

//查询一轮结束消息
typedef struct
{
    HI_U8  pad1;
    HI_U8  query_turn_cnt;
    HI_U16 cur_turn_timeout_sta_cnt;        //本轮超时的站点数

    HI_U16 cur_turn_identifying_sta_cnt;    //本轮正在识别的站点数
    HI_U16 cur_turn_fail_sta_cnt;           //本轮识别失败:查询超时站点数

    HI_U32 pad[4];
}HI_DMID_TF_QUERY_ONE_TURN_OVER_S;

//台区识别启动消息
typedef struct
{
    HI_U8  reason;                          //启动的原因
    HI_U8  identify_control_mode;           //台区识别的控制模式
    HI_BOOL is_network_formed;              //组网是否完成
    HI_BOOL is_business_busy;               //APP业务是否繁忙

    HI_U8  pad1;
    HI_U8  pad[3];

    HI_U32 ret_result;                      //启动的结果

    HI_U32 start_bpc;                       //启动台区识别时的bpc
    HI_U32 pad2[3];
}HI_DMID_TF_START_S;

//台区识别停止消息
typedef struct
{
    HI_U8  stop_type;                       //停止的类型,0:正常停止,1:异常停止
    HI_U8  reason;                          //停止的原因
    HI_U8  identify_control_mode;           //台区识别的控制模式
    HI_U8  pad;

    HI_U32 stop_bpc;                        //停止台区识别时的bpc
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

#define TF_ZERO_CROSS_REPORT_NTB_MAX_CNT      27           //一次上报NTB最大个数

typedef struct
{
    HI_U8 index;
    HI_U8 identify_result;
    HI_U8 transformer_index;
    HI_U8 phase_index;

    HI_U8 rx_gear;                     //ndm-ndm的接收机档位
    HI_U8 pad1[3];
    HI_U16 wobble_scope;
    HI_U16 noise_scope;

    HI_U32 ntb_data[TF_ZERO_CROSS_REPORT_NTB_MAX_CNT];
}HI_DMID_TF_RECEIVE_NTB_S;

//为统一功能模块与可维可测宏定义
#define TF_ZERO_CROSS_ALGORITHM_CNT             2
#define TF_ZERO_CROSS_NOISE_LIMIT_CNT           7
#define TF_ZERO_CROSS_TH_CNT (TF_ZERO_CROSS_NOISE_LIMIT_CNT * TF_ZERO_CROSS_ALGORITHM_CNT)

#define TF_NDM_WINDOW_MAX_CNT                   1       // 将ndm-ndm的点对点识别窗口改为1个

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

//台区识别结果请求
typedef struct
{
    HI_U16 tei;
    HI_U8  mac[6];

    HI_U8  identify_turn_cnt;           //该站点所能够确认接收的相台区识别起始轮次号
    HI_U8  query_timeout_cnt;           //查询超时次数
    HI_U8  pad1[2];

    HI_U32 pad2[4];
}HI_DMID_TF_RESULT_REQ_S;

//台区识别结果应答
typedef struct
{
    HI_U16 tei;
    HI_U8  mac[6];

    HI_U8  identify_state;                                  //识别状态
    HI_U8  tf_result;                                       //台区识别结果
    HI_U8  identify_state_err_cnt;                          //识别状态错误计数
    HI_U8  seq_type;                                         // 码字类型

    HI_U32 identify_success_bpc;                            //识别成功时的bpc

    HI_U8  noise_limit_index;                               //识别成功时的噪声门限
    HI_U8  alg_index                : 2;                    //识别成功时的检测算法
    HI_U8  rcv_level                : 2;                    //识别成功时的工作档位
    HI_U8  pad2                     : 4;
    HI_U16 soft_val[TF_ZERO_CROSS_NOISE_LIMIT_CNT];         //不同去噪门限检测成功时的软值和

    HI_U8  seq_index[TF_ZERO_CROSS_NOISE_LIMIT_CNT];        //不同去噪门限检出序列号
    HI_BOOL is_exist_diff_seq;                              //不同门限是否存在不同检出序列

    HI_U16 signal_mean;                                     //信号均值
    HI_U16 noise_mean;                                      //噪声均值
    HI_U16 other_signal_mean;                               //非本台区的最大信号均值
    HI_U16 pad;

    HI_U32 other_snid : 24;
    HI_U32 other_freq : 8;

    HI_U16 noise_avg[2];                                //单窗噪声均值
}HI_DMID_TF_RESULT_RSP_S;

//相位识别结果请求
typedef struct
{
    HI_U16 tei;
    HI_U8  phase;                                   //识别出的相位
    HI_U8  state;                                   //相位识别状态

    HI_U8  timeout_cnt;                             //识别超时次数
    HI_U8  fail_cnt;                                //识别失败次数(收到应答，但未识别出相位)
    HI_U8  pn_result;                               // 识别的零火结果
    HI_U8  pn_state;                                // 识别的状态

    HI_U32 time_stamp;                              //时间，单位ms

    HI_U8  phase_b;
    HI_U8  phase_c;
    HI_U8  pn_result_b;
    HI_U8  pn_result_c;
    
    HI_U32 cur_ntb;                                 // 当前的NTB
    HI_U32 start_collect_ntb;                       // 开始采集的NTB

    HI_U8 identify_mode;
    HI_U8 pad[3];
}HI_DMID_PI_ZEROC_CROSS_NTB_REQ_S;

#define HI_DMID_PI_ZEROC_CROSS_NTB_RSP_NTB_NUM_MAX      26      //相位识别结果应答的NTB个数

//相位识别结果应答
typedef struct
{
    HI_U16 tei;
    HI_U8  index;                                               //编号
    HI_U8  ntb_num;

    HI_U8  phase_cnt[4];                                        //识别结果在每个相位的统计值
    HI_U32 time_stamp;                                          //时间戳

    HI_U32 cco_cur_ntb;                                         //CCO端缓存的最新NTB
    HI_U8  phase_cnt2[4];                                       //基于下降沿识别的数据
    HI_U8  identify_mode;                                       // 识别模式
    HI_U8  edge_mode;                                           // 边沿
    HI_U8  phase_state;                                         // 相位识别状态
    HI_U8  pn_state;                                            // 零火识别状态

    HI_U32 ntb[HI_DMID_PI_ZEROC_CROSS_NTB_RSP_NTB_NUM_MAX];     //接收到的过零NTB
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
    HI_U8 up_ntb_cnt;         //STA匹配CCO上升沿总个数
    HI_U8 period_num;         //完整周期个数

    HI_U8 right_period_num;   //正半轴大于负半轴周期个数
    HI_U8 up_ntb_index_last;  //最新一次上升沿Index
    HI_U8 pn_result;          //相位识别结果
    HI_U8 identify_mode;      // 识别模式
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
    HI_U8 seq_type;                          // 码字类型0:13bit,1:127bit
    HI_U8 best_voltage;                       // 最好电压点 0-7

    HI_U8 best_line;                          // 0-31条线路
    HI_U8 noise_limit_index;
    HI_U8 alg_index;
    HI_U8 phase_index;

    HI_U8 seq_index;
    HI_U8 level;                                    // 识别成功时档位
    HI_U16 soft_value;

    HI_U16  wobble_scope;                     //扰动幅度均值
    HI_U16  noise_scope;                      //噪声幅度均值

    HI_U16 normalized_wobble_scope;               // 归一化扰动值
    HI_U16 pad;

    HI_U8 cco_mac[6];
    HI_U8 sta_v200_seq_index[5];              // V200的sta识别出5个码字序列
    HI_U8 pad1[13];
}HI_DMID_TF_RESULT_REOPRT_S;

// 每次中断上报数据
typedef struct
{
    HI_U32 ntb_result;
    HI_U32 power_period_cnt;
    HI_U8 t_in_valid_cnt;
    HI_U8 pad[3];
}HI_DMID_TF_NTB_POWER_STRU;

// 每路的芯片上报数据
typedef struct
{
    HI_U8 line_index;
    HI_BOOL is_valid;                  // 本次是否有效
    HI_U8 win_use_index;
    HI_U8 max_soft_cnt;

    HI_U16 seq_index;                 // 序列id
    HI_U16 soft_value;                 // 软值
    HI_U16 smean;                    // 信号值
    HI_U16 noise;                    // 噪声值

    HI_BOOL is_nevol;               // 比较本窗口数据是否正常
    HI_U8 seq_type;                 // 码字类型
    HI_U8 pad[2];

    HI_U32 power_period_cnt;               // 本次计数
    HI_U32 ntb;
}HI_DMID_TF_LINE_DATA_STRU;

// 上报的8路噪声
typedef struct
{
    HI_U16 noise[8];
}HI_DMID_TF_NOISE_REPORT_STRU;

// 上报CCO的相位检测NTB数据
typedef struct
{
    HI_U8 phase_seq[3];
    HI_U8 pad;
    HI_U32 ntb[30];
}HI_DMID_TF_CCO_PHASE_CHECK_NTB_STRU;

// 打印每个站点结果
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

// 打印STA周围15个网络识别信息
typedef struct
{
    HI_U32 nid;

    HI_BOOL is_in_use;
    HI_U8 nid_index;
    HI_U8 pad[2];

    HI_U16 cur_sync_ntb_cnt;
    HI_U16 sync_win_sum_cnt;

    HI_U32 sync_ntb_sum_cnt;

    HI_U32 cur_var;             // 本次方差
    HI_U32 cur_var_ratio;       // 本次方差比值

    HI_U32 var_ratio_sum;       // 累计方差比值和

    HI_U32 var_ratio;

    HI_S32 sta_cco_ntb_diff;   // 每个窗口的STA_CCO_DIFF的平均值

}HI_DMID_TFP_NEIGHBOR_ENTRY_STRU;

#define HI_DMID_TFP_NEIGHBOR_NUM_MAX      3      // 单包上报多网络信息个数

typedef struct
{
    HI_U16 tei;
    HI_U8 phase;
    HI_U8 index;

    HI_U8 neighbor_num;
    HI_U8 pad[3];

    HI_DMID_TFP_NEIGHBOR_ENTRY_STRU neighbor_entry[HI_DMID_TFP_NEIGHBOR_NUM_MAX];
}HI_DMID_TFP_NEIGHBOR_INFO_STRU;


// 打印每次启动，停止消息
#define HI_DMID_TFP_DATA_NUM                  25

typedef struct
{
    HI_U8 mode;                       // 启动，停止
    HI_U8 pad[3];

    HI_U32 data[HI_DMID_TFP_DATA_NUM];
}HI_DMID_TFP_SET_CMD_MODE_STRU;


//某个频段的评估结果
typedef struct
{
    HI_U32 avg_success_rate     : 10;                   //平均抄通率,精度:1/1000
    HI_U32 node_num             : 10;                   //上线节点数，最大支持1024个
    HI_U32 avg_delay            : 12;                   //平均延时，最大支持4095ms
}HI_MAC_FA_FREQ_RESULT_STRU;

//启动评估评估的消息定义
typedef struct
{
    HI_U8  cur_evaluate_mode;                                   //当前的评估模式
    HI_U8  start_reason;                                        //本次启动频段评估的原因
    HI_U8  init_freq;                                           //评估启动前的工作频段
    HI_U8  auto_fail_cnt;                                       //自检评估不满足指标的次数

    HI_U32 start_time_stamp;                                    //启动时间，单位:ms
    HI_U32 toatl_run_time;                                      //评估模式需要的总运行时间，单位:s

    HI_U8  business_state;                                      //业务状态
    HI_U8  chl_state;                                           //通道的状态
    HI_U8  pad[2];
}HI_DMID_FA_START_EVALUATE_S;

//停止频段评估的消息定义
typedef struct
{
    HI_U8  cur_evaluate_mode;                                   //当前的评估模式
    HI_U8  start_reason;                                        //本次启动频段评估的原因
    HI_U8  stop_reason;                                         //本次停止频段评估的原因
    HI_U8  init_freq;                                           //评估启动前的工作频段

    HI_U8  cur_freq;                                            //当前频段
    HI_U8  obj_freq;                                            //目标频段
    HI_BOOL is_next_evaluate_mode_full;
    HI_BOOL is_set_evaluate_mode_full;                          //在评估期间，其它原因触发完整评估的标识

    HI_U32 stop_time_stamp;                                     //停止时间，单位:ms

    HI_U8  evaluate_state;                                      //停止评估前的评估状态
    HI_U8  auto_fail_cnt;                                       //自检评估不满足指标的次数
    HI_U8  freq_evaluated_cnt;                                  //已评估的频段数
    HI_U8  affirm_network_state_cnt;                            //确认组网完成状态的次数

    HI_U16 node_num;                                            //除CCO外的全网站点数
    HI_U16 ping_send_node_cnt;                                  //本轮已发送测试报文的站点数

    HI_U16 ping_next_tei;                                       //下一次测试的起始站点TEI索引
    HI_U8  ping_turn_cnt;                                       //ping包测试的轮次计数
    HI_U8  pad;

    HI_MAC_FA_FREQ_RESULT_STRU freq_result[FREQ_TRY_NUM];       //频段评估的结果
}HI_DMID_FA_STOP_EVALUATE_S;

//频段评估状态的消息定义
typedef struct
{
    HI_U8  evaluate_state;                                      //当前评估状态
    HI_U8  cur_freq;                                            //当前频段
    HI_U8  level_num;                                           //网络层级数
    HI_U8  affirm_network_state_cnt;                            //组网状态的确认次数

    HI_U16 node_num;                                            //网络节点数，不包含CCO
    HI_U16 proxy_num;                                           //代理站点数

    HI_U32 handle_check_ping_timer_overtime_cnt;                //测试报文超时的次数
    HI_U32 tx_resend_cnt;                                       //发送机重发的ndm报文的总数

    HI_U32 tx_send_fail_cnt;                                    // 发送机发送失败的ndm报文的总数
    HI_U32 tx_send_timeout_cnt;                                 //发送机发送超时的ndm报文的总数

    HI_U16 level_node_num[15];                                  //各层级的站点数
    HI_U16 phase_cnt[3];                                        //各相位的站点数

    HI_MAC_FA_FREQ_RESULT_STRU freq_result[FREQ_TRY_NUM];       //频段评估的结果
}HI_DMID_FA_EVALUATE_STATE_S;

//**************************************************

#define HI_DMID_RT_OFFSET                           0x564
#define HI_DMID_RT_PATH_TABLE                       (HI_DMID_RT_OFFSET + 0x00)  // 路径信息表上报
#define HI_DMID_RT_MULTI_PATH                       (HI_DMID_RT_OFFSET + 0x01)  // 多路径选择结果上报
#define HI_DMID_RT_EVALUATE_PROXY_RESULT            (HI_DMID_RT_OFFSET + 0x02)  // 代理评估结果上报
#define HI_DMID_RT_BETTER_PROXY_RESULT              (HI_DMID_RT_OFFSET + 0x03)  // 存在更好代理结果上报
#define HI_DMID_RT_DOWN_PATH_CHANGE                 (HI_DMID_RT_OFFSET + 0x04)  // 下行路径变化上报
#define HI_DMID_RT_LEAVE_NETWORK                    (HI_DMID_RT_OFFSET + 0x05)  // 站点离线上报
#define HI_DMID_RT_ROUTE_CYCLE                      (HI_DMID_RT_OFFSET + 0x06)  // 路由周期信息
typedef struct _rt_channel_stat_st3_
{
    HI_U32 channel_info;                        // 信道质量
    HI_U32 stat_cnt;                            // 接收个数
}rt_channel_stat_st3;

typedef struct _rt_proxy_comm_st3_
{
    HI_U32 comm_rate;
    HI_U32 channel_info;
    HI_U32 down_comm_rate;
}rt_proxy_comm_st3;
// 路径信息表上报结构
typedef struct
{
    HI_U16 tei;                                     // 站点TEI地址
    HI_U16 pad;
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];                 // 站点MAC地址
    HI_U8 role;                                     // 站点角色
    HI_U8 level;                                    // 站点层级

    HI_U8 phase;                                    // 站点相位
    HI_U8 tm_capacity;                              // 站点负载能力
    HI_U8 up_channel_update_flag;
    HI_U8 pad1;

    rt_channel_stat_st3 last_down_channel_stat;      // 上次下行通信率统计
    rt_channel_stat_st3 last_up_channel_stat;        // 上次上行通信率统计
    rt_channel_stat_st3 cur_down_channel_stat;       // 当前下行通信率统计
    HI_U8 comm_rate;                               // 上下行通信率
    HI_U8 down_comm_rate;                     //下行通信成功率
    HI_U8 up_comm_rate;
    HI_U8 proxy_proxy_comm_rate;
    HI_U32 send_pk_cnt;                             // 上行发送数据个数

    HI_U32 send_fail_cnt;                           // 发送失败次数
    HI_U32 send_success_cnt;                        // 发送成功次数
    HI_U8  level_relation;                          // 层级关系
    HI_U8  rsvd;
    HI_U16 proxy_tei;                               // 父代理TEI
    rt_proxy_comm_st3 proxy_channel_stat;            // 父代理下行通信率信息

    HI_U32 rcv_beacon_cnt;                          // 接收该站点BEACON帧的个数
    HI_U8  channel_info;                            // 信道质量(beacon帧存在)
    HI_U8  par;                                     // 峰均比(SOF帧)
    HI_S16 snr;                                     // 平均信噪比(SOF帧)
}HI_DMID_RT_PATH_TABLE_ST;

// 多路径选择结果上报
typedef struct
{
    HI_U32 bpc;
    HI_U16 proxy_path;
    HI_U16 up_up_level_path;
    HI_U16 up_leval_path;
    HI_U16 same_level_path;
}HI_DMID_RT_MULTI_PATH_RESULT_ST;

// 代理评估结果上报
typedef struct
{
    HI_U32 bpc;
    HI_U16 proxy_candidate[5];
    HI_U16 proxy_cndd_cnt;
}HI_DMID_RT_EVALUATE_PROXY_RESULT_ST;

// 存在更好代理结果上报
typedef struct
{
    HI_U32 bpc;
    HI_U16 proxy_candidate[5];
    HI_U16 prxoy_cndd_cnt;
}HI_DMID_RT_BETTER_PROXY_RESULT_ST;

// 下行路径变化上报
typedef struct
{
    HI_U16 odtei;
    HI_U16 rsvd;
    HI_U16 next_hop[4];
}HI_DMID_RT_DOWN_PATH_CHANGE_ST;

// 站点离线消息上报
typedef struct
{
    HI_U32 bpc;                         // 站点离线时所处信标周期
    HI_U16 proxy_tei;                   // 站点离线时父代理TEI
    HI_U16 channel_info;                // 站点离线时与父代理的信道质量
    HI_U32 comm_rate;                   // 站点离线时与父代理的通信率
} HI_DMID_RT_LEAVE_NETWORK_ST;

typedef struct
{
    HI_U32 route_cycle_cnt;
}HI_DMID_RT_CYCLE_INFO_ST;

#define HI_DMID_NTB_OFFSET                          0x7D0
#define HI_DMID_BCN_OFFSET                          0x710
//信道参数模块的定义
#define HI_DMID_CPA_OFFSET                          0x850
#define HI_DMID_CPA_CARRIER_SENSE_MODE_SWITCH       (HI_DMID_CPA_OFFSET + 0x00)

//载波侦听模式切换
typedef struct
{
    HI_U16 tail_timer_rx_success_rate;           //成功率监测周期内接收成功率
    HI_U16 recover_timer_rx_success_rate;        //载波侦听门限恢复监测周期内接收成功率

    HI_U8  previous_mode;                        //载波侦听的前一高成功率模式
    HI_U8  current_mode;                         //载波侦听的当前模式
    HI_U16 rsd;
}HI_DMID_CPA_CARRIER_SENSE_MODE_CHANGE_S;

//PHY模块的定义
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
//AGC异常
typedef struct
{
    HI_U16 AdAvgPower;                  //AD平均功率异常
    HI_U16 SatRate;                     //饱和率

    HI_U32 AfePower;                    //AFE的信号功率
    HI_U16 PowerAfterFilter;            //滤波后信号的功率
    HI_U16 Rsd1;
}HI_DMID_PHY_AGC_ERR_S;

//NBI异常
typedef struct
{
    HI_U8  NbiCnt;                      //检测的NBI数
    HI_U8  NbiOutOfNf;                  //超过NF处理带宽的NBI
    HI_U16 Rsd1;                        //保留

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

//IMP异常
typedef struct
{
    HI_U16 ImpCnt;                      //每个统计周期StatiPeriod上报
    HI_U16 Rsd1;                        //保留
}HI_DMID_PHY_IMP_ERR_S;

//符号同步异常
typedef struct
{
    HI_U16 FirstToMaxPathLen;           //首径与最大径的距离
    HI_U16 PataLen;                     //径长度
}HI_DMID_PHY_SYMBOL_ALIGN_ERR_S;

//信道质量异常
typedef struct
{
    HI_U8  Par;                         //峰均比
    HI_U8  Rsd1;                        //保留
    HI_S16 AvgSnr;                      //平均snr
}HI_DMID_PHY_CHANNEL_INFO_ERR_S;

//Clock异常接收完成中断上报
typedef struct
{
    HI_S16 PilotFreqErr;                //Pilot频偏
    HI_S16 SoundFreqErr;                //Sounding的频偏

    HI_U16 PilotCarrierNum;             //Pilot载波数
    HI_U16 Rsd1;                        //保留
}HI_DMID_PHY_CLOCK_RX_END_ERR_S;

//Clock异常Preamble上报
typedef struct
{
    HI_S16 PreambleFreqErr;             //Preamble频偏
    HI_S16 PllFreqErr;                  //Pll的频偏

    HI_U16 PreambleCarrierNum;          //Preamble载波个数
    HI_U8  CntSingu;                    //clk_est模块的连续异常次数
    HI_U8  Rsd1;                        //保留
}HI_DMID_PHY_CLOCK_PREAMBLE_ERR_S;

//NTB异常
typedef struct
{
    HI_S16 BtsLtmrFreqErr;              //用相邻两个BTS与LTmr计算获得的偏差
    HI_S16 DeltaFreqErr;                //NTB的前后两次频偏差

    HI_U8  CntSingu4NTB;                //当前状态下的异常值个数
    HI_U8  Rsd1;
    HI_U16 Rsd2;
}HI_DMID_PHY_NTB_ERR_S;
//过流重传的配置数据
typedef struct
{
    HI_S8  AfePower;            //AFE的发射功率
    HI_S8  TxPower;             //PHY的发射功率
    HI_U8  TxState;             //发送状态
    HI_U8  Rsd;                 //保留

    HI_U32 RescentCnt;          //重传次数
}HI_DMID_PHY_OVER_CURRENT_S;


//RX模块的定义
#define HI_DMID_MAC_RX_OFFSET                   0x790
#define HI_DMID_MAC_FREE_MPDU_BUFFER            (HI_DMID_MAC_RX_OFFSET + 0x00)       //接收机释放MAC_BUFFER
#define HI_DMID_MAC_GET_MPDU_BUFFER             (HI_DMID_MAC_RX_OFFSET + 0x01)       //接收机获取MAC BUFFER
#define HI_DMID_MAC_RECV_END                    (HI_DMID_MAC_RX_OFFSET + 0x02)       //接收完成通知重组器
#define HI_DMID_MAC_SEND_SACK                   (HI_DMID_MAC_RX_OFFSET + 0x03)       //发送SACK帧
#define HI_DMID_MAC_SEND_CTS                    (HI_DMID_MAC_RX_OFFSET + 0x04)       //发送CTS帧
#define HI_DMID_MAC_SEND_SOUNDING_SACK          (HI_DMID_MAC_RX_OFFSET + 0x05)       //发送SOUNDING SACK帧
#define HI_DMID_MAC_SET_COLISION_FLAG           (HI_DMID_MAC_RX_OFFSET + 0x06)       //设置冲突标志
#define HI_DMID_MAC_RCV_SOUNDING_SACK           (HI_DMID_MAC_RX_OFFSET + 0x07)       //收到SOUNDING SACK帧
#define HI_DMID_MAC_SEND_SUCCESS                (HI_DMID_MAC_RX_OFFSET + 0x08)       //通知发送机发送成功
#define HI_DMID_MAC_VCS_CHANGE                  (HI_DMID_MAC_RX_OFFSET + 0x09)       //通知发送机线路变化
#define HI_DMID_MAC_GENERATE_SOUNDING_SACK      (HI_DMID_MAC_RX_OFFSET + 0x0a)       //生成SOUNDING SACK帧
#define HI_DMID_MAC_HANDLE_SOUNDING             (HI_DMID_MAC_RX_OFFSET + 0x0b)       //处理SOUNDING结果
#define HI_DMID_MAC_HANDLE_BEACON               (HI_DMID_MAC_RX_OFFSET + 0x0c)       //处理BEACON帧
#define HI_DMID_MAC_SOFT_NTB_CALC               (HI_DMID_MAC_RX_OFFSET + 0x0d)       //软件计算NTB的结果
#define HI_DMID_MAC_RCV_FC_OK                   (HI_DMID_MAC_RX_OFFSET + 0x0e)       //接收到FC正确
#define HI_DMID_MAC_RCV_FC_ERR                  (HI_DMID_MAC_RX_OFFSET + 0x0f)       //接收到FC错误

#define HI_DMID_MAC_RX_BEACON_INFO              (HI_DMID_MAC_RX_OFFSET + 0x10)
#define HI_DMID_MAC_RX_SOF_INFO                 (HI_DMID_MAC_RX_OFFSET + 0x11)
#define HI_DMID_MAC_RX_SACK_INFO                (HI_DMID_MAC_RX_OFFSET + 0x12)
#define HI_DMID_MAC_RX_COORDIANTE_INFO          (HI_DMID_MAC_RX_OFFSET + 0x13)
#define HI_DMID_MAC_RX_ERR_INFO                 (HI_DMID_MAC_RX_OFFSET + 0x14)


//接收FC内容上报
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

//接收机释放MAC_BUFFER
typedef struct
{
    HI_U32 bpc;       //信标周期
    HI_U32 ntb;       //释放时间
    HI_U32 sof_addr;  //释放地址
}HI_DMID_MAC_FREE_MPDU_BUFFER_S;

//接收机获取MAC BUFFER
typedef struct
{
    HI_U32 bpc;                 //信标周期
    HI_U32 ntb;                 //释放时间
    HI_U32 sof_addr;            //释放地址

    HI_U16 stei;              //数据发送站点TEI
    HI_U8  lid;               //数据报文优先级
    HI_U8  resend_packet;     //是否为重传报文
}HI_DMID_MAC_GET_MPDU_BUFFER_S;

//接收完成通知重组器
typedef struct
{
    HI_U16 stei;              //数据发送站点TEI
    HI_U8  lid;               //数据报文优先级
    HI_U8  resend_packet;     //是否为重传报文

    HI_U32 sof_addr;          //获取的MAC BUFFER地址
    HI_U32 ntb;               //释放时间

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

//发送SACK帧
typedef struct
{
    HI_U16 dtei;          //目的站点TEI
    HI_U8  sack_result;   //SACK报文中携带PB块结果
    HI_U8  rsvd;
}HI_DMID_MAC_SEND_SACK_S;

typedef struct
{
    HI_U16 dtei;     //目的站点TEI
    HI_U16 dur;      //RTC/CTS持续时间
}HI_DMID_MAC_SEND_CTS_S;

//发送SOUNDING SACK帧
typedef struct
{
    HI_U16 dtei;     //目的站点TEI
    HI_U16 rsvd;
}HI_DMID_MAC_SEND_SOUNDING_SACK_S;

typedef struct
{
    HI_U32 bpc;     //信标周期
    HI_U32 ntb;     //冲突时间
}HI_DMID_MAC_SET_COLISION_FLAG_S;

typedef struct
{
    HI_U32 bpc;     //信标周期
    HI_U32 ntb;     //接收时间
    HI_U16 stei;    //源地址
    HI_U16 rsvd;
}HI_DMID_MAC_RCV_SOUNDING_SACK_S;

typedef struct
{
    HI_U32 bpc;     //信标周期
    HI_U32 ntb;     //冲突时间
}HI_DMID_MAC_SEND_SUCCESS_S;

typedef struct
{
    HI_U32 bpc;     //信标周期
    HI_U32 ntb;     //冲突时间
}HI_DMID_MAC_VCS_CHANGE_S;


typedef struct
{
    HI_U32 bpc;     //信标周期
    HI_U32 ntb;     //冲突时间
}HI_DMID_MAC_GENERATE_SOUNDING_SACK_S;

typedef struct
{
    HI_U32 bpc;     //信标周期
    HI_U32 ntb;     //冲突时间

    HI_U16 stei;
    HI_U8  tmi;
    HI_U8  rsvd;
}HI_DMID_MAC_HANDLE_SOUNDING_S;

typedef struct
{
    HI_U32 bpc;     //信标周期
    HI_U32 ntb;     //冲突时间
    HI_U16 stei;    //BEACON帧的源地址
}HI_DMID_MAC_HANDLE_BEACON_S;

typedef struct
{
    HI_U32 current_bts;     // 当前接收同步帧的网络时钟基值
    HI_U32 current_ltmr;    // 当前接收同步帧的本地时钟计数

    HI_U32 front_bts;       // 上次同步帧的网络基准时钟值
    HI_U32 front_ltmr;      // 上次同步帧的本地时钟计数

    HI_U32 cur_ntb;         // 打印时的NTB

    HI_S16 clk_freqerr;     // 当前使用的频偏
    HI_S16 calc_freqerr;    // 当前计算出的频偏(经过滤波后的频偏值)
    HI_S16 cur_freqerr;     // 本次的频偏
    HI_U16 pad1;

    HI_U8  state;           // 同步状态
    HI_U8  synch_snid;      // 同步站点的网络号
    HI_U16 synch_tei;       // 同步站点的终端设备标识

    HI_U8  synch_mac[6];    // 同步对象的MAC地址
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

//网络管理消息定义
#define NM_MESSAGE_HEAD_OFFSET  0x500
#define HI_DMID_MAC_STA_JOIN_FORM_NUM_CHANGE                       (NM_MESSAGE_HEAD_OFFSET)
#define HI_DMID_MAC_CCO_RCV_STA_ASSOC_REQ_STA                      (NM_MESSAGE_HEAD_OFFSET + 0x01)
#define HI_DMID_MAC_STA_CHOSE_PROXY_RESULT                         (NM_MESSAGE_HEAD_OFFSET + 0x02)
#define HI_DMID_MAC_STA_JOIN_ITS_CHILD                             (NM_MESSAGE_HEAD_OFFSET + 0x03)
#define HI_DMID_MAC_SET_WORK_FREQ                                  (NM_MESSAGE_HEAD_OFFSET + 0x04)     //设置工作频段
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
#define HI_DMID_MAC_SET_SNID                                       (NM_MESSAGE_HEAD_OFFSET + 0x1C)     //设置SNID

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

// 汇聚器模块消息定义  [0x750~0x790)
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


//重组器模块消息结构体定义
#define HI_DMID_MAC_RSB_MPDU_INFO                (0x6D0)

//分段器模块消息结构体定义
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

//发送机模块消息定义
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
// 结构定义
//*****************************************************************************
// TODO
//*****************************************************************************
//网络管理消息结构体定义
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
    HI_U32 tdma_state;              // 时隙状态：正常状态、密集发送信标、密集关联
    HI_U32 current_bpc;             // 当前BPC
    HI_U32 starttime;               // 信标周期开始时间

    HI_U32 beacon_period_len;       // 信标周期长度
    HI_U32 proxy_beacon_cnt;        // 代理信标个数
    HI_U32 all_beacon_cnt;          // 信标个数

    HI_U32 phase_a_cp;              // 相位A CSMA时隙长度
    HI_U32 phase_b_cp;              // 相位B CSMA时隙长度
    HI_U32 phase_c_cp;              // 相位C CSMA时隙长度

    HI_U32 phase_a_bcp;             // 相位A 绑定CSMA时隙长度
    HI_U32 phase_b_bcp;             // 相位B 绑定CSMA时隙长度
    HI_U32 phase_c_bcp;             // 相位C 绑定CSMA时隙长度
}HI_DMID_MAC_TDMA_ALLOC_INFO_S;

//设置工作频段
typedef struct
{
    HI_U16 reason;                  //设置原因
    HI_U8  freq;                    //设置后的工作频段
    HI_U8  pad;
}HI_DMID_MAC_SET_WROK_FREQ_S;

typedef struct
{
    HI_U32 snid;

    HI_U8  freq;
    HI_U8  pad;
    HI_U16 ptcl_ver;                //协议版本号
}HI_DMID_MAC_SET_SNID_S;

typedef struct
{
    HI_U32 heartbeat_cycle_cnt;                     // 心跳周期计数
    HI_U32 rcv_heartbeat_cnt;                         // 接收心跳报文个数
    HI_U16 offline_sta_num;                            // 离线站点个数
    HI_U16 false_offline_sta_num;                  // 假离线站点个数
}HI_DMID_MAC_HEARTBEAT_INFO_S;

typedef struct
{
    HI_U32 bpc;
    HI_U32 starttime;                              // 相对时间 单位:1微秒
    HI_U32 duration;                               // 持续时间 单位:1微秒

    HI_U16 tei;                                    // 时隙对应的tei
    HI_U8  slot_type;                              // 时隙类型
    HI_U8  phase;                                  // 时隙相位(CSMA时隙存在)
    HI_U8  lid;                                    // 优先级
    HI_U8  slot_source;                     //时隙来源
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

    HI_CHAR chip_code[2];           //芯片代码
    HI_U8  pad[2];
}HI_DMID_MAC_CCO_SOME_STA_JOIN_OK_S;

typedef struct
{
    HI_U8 fail_reason;
    HI_U8 mac[6];
    HI_U8 rsvd;

    HI_U16 ptcl_ver;
    HI_U16 chip_ver;

    HI_CHAR chip_code[2];           //芯片代码
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

    HI_U32 proxy_change_cnt;                        // 代理变更次数
    HI_U32 leave_cnt;                               // 离线次数
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
    HI_U8     comm_rate;                                            //站点与该候选站点的通信成功率
    HI_U8     down_rate;                                            //站点接收该候选站点的报文成功率
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
    HI_U8   new_level;                      //代理变更站点的新层级
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
    HI_U16 neighbor_snid;                   //邻居网络的SNID位图

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
    HI_U8  role;                                     // 站点角色
    HI_U8  level;                                    // 站点层级

    HI_U8  min_multiproxy_comm_rate;                 // 路由最小通信率
    HI_U8  proxy_comm_rate;			                 // 与代理站点的上行通信率
    HI_U8  proxy_down_comm_rate;		             // 与代理站点的下行通信率
    HI_U8  down_comm_rate;                           // 下行通信成功率

    HI_U8  up_comm_rate;			                 // 上行通信成功率
    HI_U8  comm_rate;
    HI_U8  level_relation;                           // 层级关系
    HI_U8  attenuation;                              //记录信号衰减值，加权计算后的结果。

    HI_U8  mmib;                                     //记录信道质量值，加权计算后的结果。
    HI_U8  proxy_mmib;                               //记录代理站点的信道质量值。
    HI_U16 cur_down_rcv_cnt;		                 // 当前周期下行接收个数(组网前统计接收信标帧，可能会超过255，因此采用16位)

    HI_U32 send_fail_cnt;                            // 发送失败次数
    HI_U32 send_success_cnt;                         // 发送成功次数
    HI_U32 rcv_beacon_cnt;                           // 接收该站点BEACON帧的个数
    HI_U32 rcv_cnt;                                  // 接收到该站点发给本站点的单播报文(DTEI == my_tei)

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

// 汇聚器模块消息结构体定义
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


//重组器模块消息结构体定义
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


//发送机模块消息定义
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
#define HI_DMID_MAC_EMC_AVOID_FREQ_INFO         EMC_MESSAGE_HEAD_OFFSET + 0x05      //避让的载波
#define HI_DMID_MAC_EMC_RECOVERY_FREQ_INFO      EMC_MESSAGE_HEAD_OFFSET + 0x06  //恢复的载波
#define HI_DMID_MAC_EMC_HEST_INFO               EMC_MESSAGE_HEAD_OFFSET + 0x07
#define HI_DMID_MAC_EMC_LAB_INFO               EMC_MESSAGE_HEAD_OFFSET + 0x08
#define HI_DFX_MRS_BUF_MAX_LEN                  20

#define HI_DMID_MAC_EMC_POWER_CONTROL_BASIC     EMC_MESSAGE_HEAD_OFFSET + 0x10

typedef struct
{
    HI_U8  emc_inter_type;  		// NBI频域表上报模式
    HI_U8  pad[3];
    HI_U32 emc_nbi_table[16];			// NBI频域表数据
}HI_DMID_MAC_NBI_TABLE_REPORT_INFO_STRU;

typedef struct
{
    HI_U8  nbi_power_mode;          // NBI功率上报模式
    HI_U8  max_power;               // 最大功率值
    HI_U8  min_power;               // 最小功率值
    HI_U8  diff;                    // 差值

    HI_U8  nbi_power[32];           // NBI功率统计表
}HI_DMID_MAC_NBI_POWER_REPORT_INFO_STRU;

#define EMC_POWER_CONTROL_PK_PERIODIC               (1)     // 周期性
#define EMC_POWER_CONTROL_PK_ACTIVE                 (2)     // 主动发起
#define EMC_POWER_CONTROL_PK_PASSIVE                (3)     // 被动发起
#define EMC_POWER_CONTROL_PK_RCV                    (4)     // 接收到功率控制报文
#define EMC_POWER_CONTROL_RCV_FRAME_ATTENUATION     (5)     // 接收帧时的信道衰减值
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
    HI_U8  padding[2];           //预留

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



// NTB 模块消息定义，范围 [0x7D0~0X810)
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
// NTB 模块消息结构体定义
typedef struct
{
    HI_U16 tei;
    HI_U8  rsv;
    HI_U8  freq;                //PHY的并行检测频段资源编号

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
	HI_U32 has_calc_freqerr         :1; // 是否计算出过有效频偏
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

    HI_U8  report_state;           //上报阶段，1:pb正确，2:pb错误
    HI_U8  freq;
    HI_S16 rx_cubic;
    HI_U32 bpc;

} HI_DMID_MAC_NTB_NEIGHBOR_FREQERR_S;

// 范围[0x891~8A0]
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
// APP统计量定义
//*****************************************************************************
// 范围 [A450, A850)
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

// UPG 使用的范围 0XA500~0XA600
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
// 结构定义
//*****************************************************************************
#define METER_ADDR_LEN  6    // 表地址长度
#define MAX_METER_NUM   32   // 最大的表地址个数
#define MAX_QUEUE_NUM   3    // 队列元素最大个数
#define MAX_BUF_LEN     64   // 缓冲区最大长度
#define MAX_LAST_NUM    16   // 最大历史记录数量
#define MAX_LAST_NUM_1  8    // 最大历史记录数量1
#define MAX_LAST_NUM_2  4    // 最大历史记录数量2
#define MAX_RM_POOL_NUM 2    // 最大读表缓冲池个数

typedef struct
{
    HI_U8 addr[METER_ADDR_LEN];    // 表地址
}METER_ADDR; // 表地址

typedef struct
{
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];    // Mac地址
}MAC_ADDR; // Mac地址


typedef struct
{
    HI_U32 rx_total_len;        // 接收到的总字节数
    HI_U32 rx_last_time;        // 最近一次接收时间
    HI_U32 rx_ok_len;           // 接收到正确的字节数(解析到完整帧)
    HI_U32 rx_err_len;          // 接收到错误的字节数(解析完整帧失败时抛弃的字节数)
    HI_U32 rx_ok_num;           // 接收到完整帧的个数
    HI_U32 rx_err_num;          // 接收到错误帧的个数
    HI_U32 rx_err_cause;        // 接收上一次错误原因
    HI_U32 rx_buf_tail;         // 缓冲区内容结束位置
    HI_U8 rx_buf[MAX_BUF_LEN];  // 最后接收到的内容(循环覆盖-rx_buf_tail为当前覆盖的尾部)
    HI_U32 tx_total_len;        // 总发的总字节数
    HI_U32 tx_last_time;        // 最近一次发送时间
    HI_U32 tx_ok_len;           // 发送成功的字节数
    HI_U32 tx_fail_len;         // 发送失败的字节数(调用DMS接口返回失败后的统计)
    HI_U32 tx_total_num;        // 总发送帧个数
    HI_U32 tx_ok_num;           // 发送成功的帧个数
    HI_U32 tx_fail_num;         // 发送失败的帧个数(调用DMS接口返回失败后的统计)
    HI_U32 tx_err_cause;        // 最近一次发送失败原因
    HI_U32 tx_last_len;         // 上一帧发送的数据长度
    HI_U8 tx_buf[MAX_BUF_LEN];  // 上一帧发送的数据内容
}UART_CHL_INF_S;    // Uart通道统计

typedef struct
{
    HI_U32 rx_total_num;       // 总接收包个数
    HI_U32 rx_time;            // 上一次接收数据时标
    HI_U16 rx_id;              // 上一次接收的id
    HI_U8 src_mac[HI_PLC_MAC_ADDR_LEN];          // 上一次接收的源mac
    HI_U32 rx_len;             // 上一次接收数据长度
    HI_U8 rx_buf[MAX_BUF_LEN]; // 上一次接收数据内容(部分)
    HI_U32 rx_err_num;         // 接收错误包个数
    HI_U32 rx_err_time;            // 上一次接收错误帧数据时标
    HI_U16 rx_err_id;              // 上一次接收错误帧的id
    HI_U8 err_src_mac[HI_PLC_MAC_ADDR_LEN];          // 上一次接收错误帧的源mac
    HI_U32 rx_err_cause;           // 上一次接受的错误帧原因
    HI_U32 rx_err_len;             // 上一次接收错误帧数据长度
    HI_U8 rx_err_buf[MAX_BUF_LEN]; // 上一次接收错误帧数据内容(部分)
    HI_U32 tx_total_num;       // 总发送包个数
    HI_U32 tx_time;            // 上一次发送的数据时标
    HI_U16 tx_id;              // 上一次发送的id
    HI_U8 dest_mac[HI_PLC_MAC_ADDR_LEN];         // 上一次发送的目的mac
    HI_U32 tx_len;             // 上一次发送的数据长度
    HI_U8 tx_buf[MAX_BUF_LEN]; // 上一次发送的数据内容(部分)
    HI_U32 tx_fail_num;        // 发送失败包个数
    HI_U32 tx_fail_time;            // 上一次发送的数据时标
    HI_U16 tx_fail_id;              // 上一次发送的id
    HI_U8 fail_dest_mac[HI_PLC_MAC_ADDR_LEN];         // 上一次发送的目的mac
    HI_U32 tx_err_cause;           // 上一次接受的错误帧原因
    HI_U32 tx_fail_len;             // 上一次发送的数据长度
    HI_U8 tx_fail_buf[MAX_BUF_LEN]; // 上一次发送的数据内容(部分)
}PLC_CHL_INF_S;     // PLC通道接收统计

typedef struct
{
    UART_CHL_INF_S uart_msg_send_inf;
    UART_CHL_INF_S uart_msg_proc_inf;
    PLC_CHL_INF_S plc_msg_send_inf;
    PLC_CHL_INF_S plc_msg_proc_inf;
}HI_DSID_APP_CHL_RX_TX_INF_S; // 通道收发统计

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 heart_num;    // 心跳次数
    HI_U32 last_time[MAX_LAST_NUM];    // 历史心跳时间
    HI_U32 timeout_num;  // 心跳超时次数
    HI_U32 timeout_time[MAX_LAST_NUM]; // 历史心跳超时时间
}HI_DSID_APP_HEARTBEAT_INF_S;    // 心跳统计

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U8 status[MAX_LAST_NUM];          // CCO状态
    HI_U32 last_time[MAX_LAST_NUM];      // 历史状态改变时标
    HI_U32 map_ready_time;  // 映射表维护完成时标
    HI_U32 map_last_time;   // 最近一次映射表维护完成时标
}HI_DSID_APP_CCO_RUN_STATUS_S; // CCO运行状态

typedef struct
{
    HI_U8 route;            // CCO路由状态
    HI_U8 LR_status;        // CCO端轮抄状态
    HI_U16 meter_num;       // 表档案数量
    HI_U32 LR_req_cnt;      // 轮抄询问读表次数
    HI_U32 LR_ack_cnt;      // 轮抄应答读表次数
    HI_U32 LR_plc_tx;       // 轮抄PLC发送次数
    HI_U32 LR_plc_rx;       // 轮抄PLC接收次数
    HI_U32 LR_meter_tx;     // 轮抄电表发送次数
    HI_U32 LR_meter_rx;     // 轮抄电表接收次数
    HI_U32 LR_plc_try_cnt;  // 轮抄PLC重发次数
    HI_U32 LR_plc_seq_err;  // 轮抄PLC帧序号过期丢弃次数
    HI_U32 LR_plc_len_err;  // 轮抄PLC帧解析长度错误次数
    HI_U32 LR_plc_exceed_cnt;  // 轮抄PLC帧接收过期次数(CCO侧没有处于接收PLC状态)
    HI_U32 LR_plc_timeout;  // 轮抄PLC超时次数
    HI_U32 LR_up_data_cnt;  // 上报抄读数据次数
    HI_U32 LR_up_data_try_cnt; // 重发上报抄读数据次数
    HI_U32 LR_ok_cnt;       // 轮抄成功次数(上报集中器成功并收到确认帧)
    HI_U32 LR_fail_cnt;     // 轮抄失败次数(PLC超过最大重试次数)
}APP_LR_INF_S;

typedef struct
{
    HI_U32 XR_status;       // 随抄CCO状态
    HI_U32 XR_uart_rx;      // 随抄串口接收次数(CCO为接收读表帧; STA为接收电表帧)
    HI_U32 XR_uart_tx;      // 随抄串口发送次数(CCO为发送读表帧; STA为发送电表帧)
    HI_U32 XR_plc_tx;       // 随抄PLC发送次数
    HI_U32 XR_plc_rx;       // 随抄PLC接收次数
    HI_U32 XR_nak_num;      // 随抄否认次数
    HI_U8 XR_nak[MAX_LAST_NUM]; // 最近MAX_LAST_NUM次否认原因
    HI_U32 XR_plc_try_cnt;  // 随抄PLC重发次数
    HI_U32 XR_plc_seq_err;  // 随抄PLC帧序号过期丢弃次数
    HI_U32 XR_plc_len_err;  // 随抄PLC帧解析长度错误次数
    HI_U32 XR_plc_exceed_cnt;   // 随抄PLC帧接收过期次数(CCO侧没有处于接收PLC状态)
    HI_U32 XR_plc_timeout;  // 随抄PLC超时次数
    HI_U32 XR_ok_cnt;       // 随抄成功次数
    HI_U32 XR_fail_cnt;     // 随抄失败次数
}APP_XR_INF_S;

typedef struct
{
    HI_U32 PR_uart_rx;      // 并发串口接收次数(CCO为接收读表帧; STA为接收电表帧)
    HI_U32 PR_uart_tx;      // 并发串口发送次数(CCO为发送读表帧; STA为发送电表帧)
    HI_U32 PR_plc_tx;       // 并发PLC发送次数
    HI_U32 PR_plc_rx;       // 并发PLC接收次数
    HI_U32 PR_nak_num;      // 并发否认次数
    HI_U8 PR_nak[MAX_LAST_NUM]; // 最近MAX_LAST_NUM次否认原因
    HI_U32 PR_plc_try_cnt;  // 并发PLC重发次数
    HI_U32 PR_plc_seq_err;  // 并发PLC帧序号过期丢弃次数
    HI_U32 PR_plc_len_err;  // 并发PLC帧解析长度错误次数
    HI_U32 PR_plc_timeout;  // 并发PLC超时次数
    HI_U32 PR_ok_cnt;       // 并发成功次数
    HI_U32 PR_fail_cnt;     // 并发失败次数
}APP_PR_INF_S;

typedef struct
{
    HI_U32 ulPoolId;        // 缓存池ID(缓存池的内存地址)
    HI_U32 ulPoolMode;      // 缓存池工作模式(MRS_RM_NORMAL_MODE : MRS_RM_BROADCAST_MODE)
    HI_U32 ulParallelNum;   // 当前并发数
    HI_U32 ulItemNum;       // 当前读表请求数量
    HI_U32 ulJoinFailCnt;   // 加入缓冲池失败次数
    HI_U32 ulJoinFailCause[MAX_LAST_NUM];   // 最近MAX_LAST_NUM次加入缓冲池失败原因
}APP_RM_POOL_INF_S;

typedef struct
{
    HI_U32 ulPlcSeq;
    HI_U32 ulTimerStatus;
    APP_RM_POOL_INF_S stPoolInf[MAX_RM_POOL_NUM];
    APP_LR_INF_S LR_inf;    // 轮抄(定抄)读表信息
    APP_XR_INF_S XR_inf;    // 随抄读表信息
    APP_PR_INF_S PR_inf;    // 并发读表信息
}HI_DSID_APP_RM_INF_S; // 读表相关信息统计

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 change_num;              // 模拟通道改变次数
    HI_U8 status[MAX_LAST_NUM];     // 模拟通道状态(含历史状态)
    HI_U32 last_time[MAX_LAST_NUM]; // 模拟通道状态改变时刻(历史)
}HI_DSID_APP_VM_CHL_INF_S; // 模拟通道相关信息

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 status;              // STA状态
    HI_U32 state_time;                 // STA状态改变时标
    HI_U32 vm_chg_num;                 // 虚拟表改变次数
    HI_U8 vm_flg[MAX_LAST_NUM];        // 虚拟表标志(含历史状态)
    HI_U32 vm_time[MAX_LAST_NUM];      // 虚拟表状态改变时刻(历史)
    HI_U8 report_flg[MAX_LAST_NUM];    // 主动上报状态(含历史状态)
    HI_U32 report_time[MAX_LAST_NUM];  // 主动上报状态改变时刻(历史)
    HI_U8 change_flg[MAX_LAST_NUM];    // 改变上报状态(含历史状态)
    HI_U32 change_time[MAX_LAST_NUM];  // 改变上报状态改变时刻(历史)
}HI_DSID_APP_STA_STATUS_S; // STA状态统计


typedef struct
{
    HI_U32 delay_20ms_cnt;      // 消息处理延时超20ms次数
    HI_U32 delay_50ms_cnt;      // 消息处理延时超50ms次数
    HI_U32 delay_100ms_cnt;     // 消息处理延时超100ms次数
    HI_U32 delay_200ms_cnt;     // 消息处理延时超200ms次数
    HI_U32 delay_300ms_cnt;     // 消息处理延时超300ms次数
    HI_U32 delay_500ms_cnt;     // 消息处理延时超500ms次数
    HI_U32 delay_1000ms_cnt;    // 消息处理延时超1000ms次数
    HI_U32 delay_2000ms_cnt;    // 消息处理延时超2000ms次数
    HI_U32 delay_5000ms_cnt;    // 消息处理延时超5000ms次数
    HI_U32 delay_10000ms_cnt;   // 消息处理延时超10000ms次数
    HI_U32 delay_30000ms_cnt;   // 消息处理延时超30000ms次数
    HI_U32 other_cnt;           // 其它未超过处理延时门限的次数
}APP_MSG_DLY_LEVEL_CNT_S;

typedef struct
{
    HI_U32 curr_msg_num;        // 当前消息队列深度
    HI_U32 max_msg_num;         // 最大队列深度
    HI_U32 max_mag_num_time;    // 最大队列深度发生时间
    HI_U32 total_tx_cnt;        // 消息总发送次数
    HI_U32 tx_no_timer_cnt;     // 消息发送次数(不含定时器消息)
    HI_U32 total_rx_cnt;        // 消息总接收次数
    HI_U32 rx_no_timer_cnt;     // 消息接收次数(不含定时器消息)
    HI_U32 tx_fail_cnt;         // 消息发送失败次数
    HI_U32 last_tx_id[MAX_LAST_NUM_2];  // 上一次消息发送ID
    HI_U32 last_rx_id[MAX_LAST_NUM_2];  // 上一次消息处理的ID
    HI_U32 last_tx_time;    // 上一次消息发送时标
    HI_U32 last_rx_time;    // 上一次消息处理时间
    HI_U32 last_msg_delay[MAX_LAST_NUM_2];  // 历史消息处理间隔
    HI_U32 max_msg_delay;   // 最大的消息处理间隔
    HI_U32 max_delay_id;    // 发生最大消息处理间隔时的ID
    HI_U32 max_delay_time;  // 发生最大消息处理间隔时标
    HI_U32 max_delay_queue_num;             // 发生最大消息处理间隔时队列深度
    HI_U32 max_msg_proc;        // 最大消息处理耗时
    HI_U32 max_msg_proc_id;     // 最大消息处理耗时ID
    HI_U32 max_msg_proc_time;   // 最大消息处理耗时发生时间
    HI_U32 last_fail_id[MAX_LAST_NUM];      // 历史消息发送失败ID
    HI_U32 last_fail_time[MAX_LAST_NUM];    // 历史消息发送失败时间
    APP_MSG_DLY_LEVEL_CNT_S delay_cnt;      // 消息处理延时统计
}HI_DSID_APP_MSG_INF_S;     // 应用任务消息队列统计

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 tx_ok_cnt;    // 使用广播方式发送成功次数
    HI_U32 last_tx_time[MAX_LAST_NUM];  // 历史上使用广播发送成功时标
    HI_U32 tx_fail_cnt;     // 使用广播方式发送失败次数
    HI_U32 fail_tx_time[MAX_LAST_NUM];  // 历史上使用广播发送失败时标
    HI_U32 rx_cnt;    // 接收到的广播帧总次数
    HI_U32 last_rx_time[MAX_LAST_NUM];  // 历史上接收到的广播帧时标
    HI_U32 bc_rm_cnt;       // 使用广播方式读表次数
    HI_U32 bc_rm_ok_cnt;    // 使用广播方式读表成功次数
    HI_U32 bc_rm_non_bc_resp_cnt;   // 使用广播方式读表收到非广播方式响应次数
    HI_U32 bc_rm_fail_cnt;  // 使用广播方式读表失败次数(超时)
}HI_DSID_APP_PLC_BC_INF_S;    // 应用层PLC广播帧收发统计

typedef struct
{
    HI_U8 curr_num;          // 当前队列深度
    HI_U8 max_num;           // 最大队列深度
    HI_U16 first_id;         // 队首ID
    HI_U32 lock_time;        // 队首锁定时间(ms)
    HI_U32 last_lock_time;   // 上一次队首锁定时间(ms)
    HI_U32 max_lock_time;    // 最大队首锁定时间(ms)
    HI_U32 queue_full_cnt;   // 队列满次数
    HI_U32 queue_full_time[MAX_LAST_NUM];   // 队列满发生时刻
}HI_DSID_APP_RM_QUEUE_INF_S;  // 应用层读表队列信息统计

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U8 afn;  // 被拒收命令的afn
    HI_U8 fn;   // 被拒收命令的fn
    HI_U8 type; // 否认原因
}REFUSE_CMD_INF;    // 拒收命令信息

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 total_cnt;
    REFUSE_CMD_INF refust_cmd_list[10]; // 最近10次否认帧信息
}HI_DSID_APP_REFUSE_CMD_INF_S;    // 拒收命令统计(376.2否认帧)

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 last_time[MAX_LAST_NUM];     // 历史改变时刻
    HI_U8 chl_status[MAX_LAST_NUM];     // 历史通道状态
    HI_U32 change_num;        // 通道变更次数
    HI_U32 first_time;        // 第一次组网成功时间
    HI_U32 chl_change_true;   // TRUE个数
    HI_U32 chl_change_false;  // FALSE个数
    HI_U32 last_true_time[MAX_LAST_NUM];    // 最近一次通道变更为可用时标
    HI_U32 last_false_time[MAX_LAST_NUM];   // 最近一次通道变更为不可用时标
    HI_U32 max_interval;      // 两次变化最大间隔
    HI_U32 min_interval;      // 两次变化最小间隔
    HI_U32 msg_tx_fail_num;   // 通道变更消息发送失败次数
    HI_U32 msg_fail_time[MAX_LAST_NUM];     // 历史消息发送失败时间
    HI_U32 msg_fail_cause[MAX_LAST_NUM];    // 历史消息发送失败原因
}HI_DSID_APP_TOPO_CHANGE_INF_S;   // Topo变化统计

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 rx_msg_cnt;          // 接收到的报文总次数
    HI_U32 mag_filter_cnt;      // 报文过滤总次数
    HI_U32 bc_filter_cnt;       // 广播报文过滤总次数
    HI_U32 normal_filter_cnt;   // 非广播报文过滤总次数
    HI_U16 id[20];              // 最近20次被过滤的id
}HI_DSID_APP_MSG_FILTER_INF_S;    // 报文过滤统计

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
    HI_U8  ucCurrGetTimeStatus; //上电后获取时钟状态 0:未获取;1:已获取完成;2:正在获取
    HI_U8  ucGetTimeSuccFlag;   //上电后获取时钟是否成功 1:成功,结合状态参数ucCurrGetTimeStatus判断
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
    HI_U8   ucEvtListItemNum;  //CCO上报过的事件链表中条目数量
    HI_U16  usEvtQueueItemNum; //CCO队列条目数量
    HI_U16  ucCcoRxCompatibilityErrCnt;
    HI_U16  usCcoReportTimeoutCnt;//总超时次数统计(不含重发)
    HI_U32  ulCcoReportEvtCnt;//上报事件帧统计(不含重发)
    HI_U32  ulCcoEvtJoinQueueTimeSec[10]; //单位为秒
    HI_U32  ulCcoRxAckCnt;
    HI_U32  ulCcoRxEvtPlcCnt;
    HI_U32  ulCcoRxEvtPlcTimeSec[10]; //单位为秒
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

	HI_U8  ucMeterEvtType;  //0:默认值  1:电表支持主动上报状态字  2:电表不支持主动上报状态字
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
    HI_U8   ucCurrStatus;  //当前监控状态 0-空闲  1-正在监控
	HI_U8   padding[3];
    HI_U16  usMonEvtTimes; //监控到事件的次数
	HI_BOOL bWaitPlcFlag;  //等待PLC通道可用发送事件上报帧标志
    HI_U8  ucPlcQuery645times; //等待PLC可用后收到CCO的确认后,如果管脚仍为高,则直接查询645帧次数
    HI_U32  ulMonEvtTimeStamp[10];//最近10次监控到事件的时间戳
	HI_U32  ulMonEvtStartTimeStamp[10];//最近10次监控开始的时间戳
	HI_U32  ulMonEvtEndTimeStamp[10];//最近10次监控结束的时间戳
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

// 当前结构大小380字节
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
	HI_U8 ucDetectStatus;//0-空闲 1-白名单添加未抄表 2-探测抄表中
	HI_U8 ucJoinWhiteFlag; // 0-不允许加入白名单  1-允许加入白名单
	HI_U8 ucAllowJoinWhiteReason;//0-默认 1-优化完成 2-20分钟一直处于组网中 3-本来是允许状态,外部去使能后恢复的允许状态
	HI_U32 ulAllowJoinWhiteTime;//允许加入白名单的时间戳
	HI_U32 ulForbidJoinWhiteTime;//禁止加入白名单的时间戳
	HI_U8 ucForbidJoinWhiteReason;//由允许状态变为禁止的原因 0-默认 1-plc通道不可用 2-外部去使能
	HI_U8 ucSetWaitTopoFlag; // 0-未知  1-需要在表档案通知MAC时设置20分钟等待站点入网,表档案由空变为非空时会设置该标志 2-表档案被清空通知MAC时设置,之后表档案一旦添加上该标志设置为1
	HI_U8 ucRmFlag; //0-不在探测透抄抄表 1-正在探测透抄,已发送透抄帧
	HI_U8 ucExternFlag; //外部操作探测抄表机制使能 0-外部无操作  1-外部有操作
	HI_U8 ucOriginalEnable; //外部使能前的原始开关
	HI_U8 ucCurrTurn;//当前探测抄表轮数
	HI_U16 usDetectWhiteNum;//探测机制中追加的白名单个数
	HI_U8 ucTopoStatus;
	HI_U8 uc97DIIndex;//当前抄读97表的DI指数
	HI_U8 reserved;
    HI_U8 ucDetectEndReason[5];
	HI_U32 ulDetectStartTime[5];//最近5次探测机制开始的时间
	HI_U32 ulDetectEndTime[5];//最近5次探测机制结束的时间
	HI_U32 ulEvalutedTime;  //拓扑优化完成时间
	HI_U32 ulPlcDisconnectTime;  //Plc通道变为不可用时间
	HI_U32 ulArchivesClearTime;  //表档案最近被清空的时间
	HI_U32 ulArchives0To1Time;  //表档案由空变为非空的时间
	HI_U32 ulExternDisableTime; //外部去使能探测开关时间
	HI_U32 ulExternResumeTime; //外部恢复使能探测开关时间
	HI_U16 usBlackListNum;  //黑名单数量
	HI_U16 usDetectWhiteValidNum;//探测机制中追加的白名单有效个数
	HI_U8 ucRefreshWhiteFlag; //刷新白名单定时器监控标志
	HI_U8  reserved2[15];
}HI_DSID_APP_CCO_DETECT_RM_INF_S;

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U16 total_num;   // 当前节点总数量
    HI_U32 modify_cnt;  // 被修改次数
    HI_U32 add_cnt;  // 添加次数
    HI_U32 del_cnt;  // 删除次数
    HI_U32 init_cnt;  // 初始化次数
}HI_DSID_APP_MREC_STATUS_S;   // 当前表档案状态

typedef struct
{
    HI_U8 mac[6];
    HI_U8 padding[2];
}HI_DSID_APP_MAC_LIST_CONT_S;   // MacList内容

//MAC地址信息
typedef struct
{
    HI_U8   ucMacAddr[HI_PLC_MAC_ADDR_LEN];   //MAC地址,6字节，放在最前面
    HI_U8   report;         //已经上报上来了
    HI_U8   retries;        //重试次数
}DFX_MRS_MAC_INFO;

//电表信息
typedef struct
{
    HI_U8   ucMeterAddr[HI_METER_ADDR_LEN];   //电表地址,6字节
    HI_U8   ucProtocol;    //规约类型
    HI_U8   ucDeviceType;   //设备类型
}DFX_MRS_METER_INFO;

typedef struct
{
    HI_U16          ttl;        //映射关系TTL
    HI_U8           report;     //上报标志
    HI_U8           lock;       //XXX
    DFX_MRS_MAC_INFO    mac;        //MAC
    DFX_MRS_METER_INFO  meter;      //电表信息
}HI_DSID_APP_MAC_MAP_CONT_S;    // Mac映射表内容

typedef struct
{
    HI_BOOL bValidFlg;    // 有效标志 HI_TRUE : 此从节点有效 HI_FALSE : 此从节点无效
    HI_U8 ucProtocolType; // 规约类型 00:保留(未知) 01:97-645 02:07-645 其它:未知
    HI_U16 usNodeSeq;    // 表档案中从节点(电表)序号
    HI_U8 ucNodeAddr[6]; // 从节点地址(电表地址)
    HI_BOOL bSaveValidFlg; //节点信息存盘有效标志(HI_TRUE:该节点信息应该存盘 HI_FALSE:该节点信息不应该存盘)
    HI_U8 ucCheckSum;             // 校验码
}HI_DSID_APP_MREC_INF_S;    // 表档案内容

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 ulSign;
    HI_U32 ulOption;    //升级选项
    HI_U32 ulUpgId;     //升级ID
    HI_U32 ulFileLen;   //升级文件的长度
    HI_U32 ulBlkNum;    //块个512字节为一个单位
    HI_U32 ulUpgTimeWindow; //升级时间窗
    HI_U32 ulLastBlockId;   //上一个BLOCK的ID
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
    HI_U32 ulClearMrsUpgTime; //MRS收到最近一次集中器清除下装命令时的时间
    HI_U32 ulMrsUpgStartTime; //MRS最近一次开始升级（即接收正确的第一帧下装文件）的时间
    HI_U32 ulMrsUpgEndTime;   //MRS模块升级结束时间，包括升级完成结束和其它原因导致的升级结束
    HI_U8  ucMrsRxClearUpgStatus; //MRS模块接收到清除下装命令时的状态
    HI_U8 ucMrsUpgStatus;  //MRS模块远程升级状态
    HI_U16 usMrsFrameErrFlag;  //MRS模块在下装过程中接收到的报文中错误的标识
    HI_U8 ucMrsRxErrUpgStatus;//MRS模块收到的升级状态不是二次开发接口中的4个值
    HI_U8 ucMrsReleaseMemFail;//MRS模块释放升级文件内存失败的标志
    HI_U8 ucStartUpgRxStopUpg;//正在启动升级时收到停止升级命令次数
    HI_U8 ucFailedReason;   // 结束失败原因
    HI_U8 ucMrsGetUpgMomeryFail; //MRS模块获取升级文件内存失败的标志
    HI_U8 ucUpgFileType; //升级文件类型
    HI_U8 ucUpgStartRet; //MRS模块向UPG模块发送的开始升级结果
    HI_U8 ucUpgStopRet;  //MRS模块向UOG模块发送的停止升级结果
    HI_U32 ulMrsUpgLenErrSize; //MRS模块升级下装过程中升级文件长度错误的值
    HI_U16 usMrsRxUpgFrameNum; //升级过程中MRS模块接收到集中器的正确帧数
    HI_U16 usMrsTxUpgFrameNum; //升级过程中MRS模块发送给集中器的帧数
    HI_U32 ulUpgStartTime; //MRS模块向UPG模块发送的开始升级时间
    HI_U32 ulUpgStopTime;  //MRS模块向UPG模块发送的停止升级时间
    HI_U32 ulMrsMsgRxStartTime; //Mrs接收开始升级应答帧的时间戳
    HI_U32 ulMrsMsgRxStartRet;  //Mrs接收开始升级应答帧的返回值
    HI_U16 usMrsMsgRxTotalTimes; //远程升级过程中MRS模块接收的正确消息总次数
    HI_U16 usMrsMsgTxMrsTimes; //远程升级过程中MRS模块发送给MRS自己的消息次数
    HI_U16 usMsgTxUpgTimes; //远程升级过程中MRS模块发送给UPG模块的消息次数
    HI_U16 usMsgRxUpgTimes; //远程升级过程中MRS模块接收到UPG模块的消息次数
    HI_U16 usMsgRxUpgIdErrTimes; //远程升级过程中MRS模块接收到UPG模块的消息中对象ID或升级ID不等于4的次数。
    HI_U8 ucMsgTxDrvTimes; //远程升级过程中MRS模块发送给DRV_UPG模块的消息次数
    HI_U8 ucMsgRxDrvTimes; //远程升级过程中MRS模块接收到DRV_UPG模块的消息次数
    HI_U16 usMsgTxFailId;//远程升级过程中MRS模块发送的失败消息ID
    HI_U8 ucMsgTxFailNum;//远程升级过程中MRS模块发送的失败消息总数
    HI_U8 usMrsMsgRxIdErrTimes;//远程升级过程中MRS模块接收到的消息ID不是升级停止、升级状态或文件检查ID的次数。
    HI_U32 pMsgFailRet[4];//最近4次消息发送失败的返回值
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

// EQT统计量
typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_U32 timestamp;
    HI_U32 eqt_timer_cnt[4]; // [0] 指定次数Timer 次数执行完毕后停止timer计数 [1] Reset timer cnt. [2] stop timer counter
    HI_U32 eqt_timer_val[4]; // [0] max counter of timer [1] execute cnt before reset timer. [2] execute cnt before stop timer [3] execute cnt in timer call-back
    HI_U32 eqt_eqx_port_reset[4]; // [0] 端口改变总次数, [1]-[3] UART 口改变次数, 发送EQMC通道 成功,失败,内存不足,发送总次数 记录,
    HI_U32 eqt_eqx_port_reset_val[6];
    HI_U32 hso_chl_rx_ok_cnt;  // 成功接收HSO报文的个数
    HI_U32 hso_chl_rx_chksum_err_cnt; // CRC校验错误的HSO报文个数
    HI_U32 hso_chl_rx_id_err_cnt; // 不支持的HSO报文个数
    HI_U32 hso_chl_sndtask_cnt[2]; // 发送HSO报文给主任务成功/失败个数
    HI_U32 hso_chl_sndtask_nomem_err_cnt;
    HI_U32 hso_chl_not_find_req_proc_cnt;
    HI_U32 hso_chl_sndport_err_cnt; // 发送给HSO错误个数
    HI_U32 snd_ecmsg_cnt[2];          // 发送给主控任务成功和错误的计算
    HI_U32 eqt_ecmsg_proc_exec_cnt;   // ec msg 总处理入口执行次数
    HI_U32 eqt_hsoreq_proc_exec_cnt;  // hso req 总处理入口执行次数
    HI_U32 eqt_eqmc_rcv_cnt[4];       // 接收EQMC通道数据计数 [3]接收总次数
    HI_U32 eqt_eqmc_snd_cnt[4];       // 发送EQMC通道 成功,失败,内存不足,发送总次数 记录,
    HI_U32 eqt_eqmc_rcv_len[4];       //
    HI_U32 eqt_mc_parse_err_cnt;
} HI_DSID_APP_EQT_CHL_STAT_S;


#define NETWORK_NOTIFY_REC_TIMES        (5)
#define HI_UPG_PROC_DETAIL_CNT          (3)
#define UPG_CTX_STATUS_CHANGE_REC_TIMES (5)
#define UPG_CCO_REC_SND_QUERY_CNT       (3) // 记录CCO发送查询的次数
#define UPG_CCO_REC_QUERY_EXC_STA_CNT   (3) //CCO记录查询异常站点个数
#define UPG_STA_REC_REPLY_STATUS_CNT    (3) //STA记录应答状态查询次数
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
    HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN]; //被置为升级状态异常站点MAC地址
    HI_U16 reserved;
    HI_U32 ulSetExceptionTimeStamp; //站点被置为升级状态异常时间戳
}HI_UPG_CCO_SET_QUERY_EXCEPTION_INFO;

typedef struct
{
    HI_U16 usSetQueryExceptionCnt; //被置为升级异常总的站点个数
    HI_U16 reserved;
    HI_UPG_CCO_SET_QUERY_EXCEPTION_INFO struDetailClient[UPG_CCO_REC_QUERY_EXC_STA_CNT]; //最近3次被置为升级异常站点信息
}HI_UPG_CCO_QUERY_EXCEPTION_STRU;

typedef struct
{
    HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN]; //发送查询站点的MAC地址
    HI_U16 reserved;
    HI_U32 ulSndTimeStamp; //发送查询的时间戳
    HI_U32 ulSndRet;       //发送状态查询的返回值
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
   HI_UPG_CLIENT_STATE_INFO stClientState;//判断升级策略是否执行完时，信息列表中各状态站点个数
   HI_U32 ulTimeStamp;            //判断升级策略是否执行完的时间戳
}HI_UPG_CHECK_COMPLETE_INFO;//12字节

typedef struct
{
    HI_U16 usTei;//站点的TEI
    HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN]; //站点MAC地址
}HI_UPG_STA_INFO;//8字节

typedef struct
{
    HI_UPG_STA_INFO stProcessItem;//站点MAC、TEI信息
    HI_U32 ulTimeStamp; //时间戳
}HI_UPG_ITEM_DETAIL; //12字节

typedef struct
{
    HI_U16 usTimeStamp; //结束断电续传前延时等待的时间戳
    HI_U8 ucTimerCnt;//当前已启动定时器个数
    HI_U8 reserved;
}HI_UPG_BSP_WAIT_INFO;//断电续传前已启动延时重启定时器个数

typedef struct
{
    HI_UPG_STA_INFO stProxyItem;//代理节点信息
    HI_UPG_STA_INFO stItem;//子节点信息
    HI_U32 ulTimeStamp;
}HI_UPG_U2B_PROCESS_TABLE_ITEM;//20字节

typedef struct
{
   HI_U16 usSndTotalLoopCnt;//发送的总轮数
   HI_U16 usSndCntLatestLoop; //最近一轮发送成功的次数
   HI_U16 usSndFullCntLatestLoop; //最近一轮发送队列满的次数
   HI_U16 usSndFailCntLatestLoop; //最近一轮发送失败的次数(非成功、非队列满)

   HI_U32 ulSndLoopStartTimeStamp; //最近一轮发送开始时间戳
   HI_U32 ulSndLoopEndTimeStamp;//最近一轮发送结束的时间戳
}HI_UPG_SND_INFO;//策略发送信息 16字节

typedef struct
{
    HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN];//站点的MAC地址
    HI_U16 usSndPacketCnt;//发送数据包的个数
    HI_U32 ulTimeStamp;//时间戳
}HI_UPG_PROCESS_ITEM_SND_INFO;//12字节

typedef struct
{
    HI_U32 ulRcvFileSize; //站点应答查询时上报的接收升级文件大小
    HI_U32 ulReplyTimeStamp; //站点应答状态查询的时间戳
    HI_U32 ulReplyRet;       //站点应答状态查询的返回值
}HI_UPG_STA_REPLY_QUERY_INFO;


typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U16 rcv_blk_cnt;         // 已接收升级数据帧总数[新增]ok
    HI_U16 rcv_blk_LSN;         // 最后一次接收数据的顺序号[新增]ok
    HI_U32 rcv_blk_LTS;         // 最后一次接收数据的时间戳[新增]ok

    HI_U16 snd_blk_cnt;          //实际有效发送数据报文总数ok
    HI_U16 snd_blk_LSN;          //最后一次发送升级数据的序号ok
    HI_U16 snd_blk_fail_cnt;     //发送数据时返回fail的次数ok
    HI_U16 snd_blk_full_cnt;     //发送数据时返回full的次数ok
    HI_U32 snd_blk_LTS;          //最后一次发送升级数据成功的时间戳ok

    HI_U16 snd_status_cnt;       //发送状态查询的次数ok
    HI_U16 snd_status_fail_cnt;  //发送状态查询失败次数ok
    HI_U16 snd_status_full_cnt;  //发送状态查询但队列满的次数ok
    HI_U8  snd_status_mac[HI_PLC_MAC_ADDR_LEN];   //当前发送状态查询站点的MAC地址 ok
    HI_U32 snd_status_LTS;       //最后一次发送状态查询的时间戳ok
    HI_U16 rcv_status_reply_cnt; //接收到状态查询回复的次数,不包括丢弃次数ok
    HI_U16 rcv_status_reply_discard_cnt; //接收到状态查询回复丢弃次数 ok
    HI_U32 rcv_status_LTS; //最后一次成功处理接收到站点查询回复的时间戳
    HI_U16 rcv_status_report_cnt; //CCO接收到STA主动上报的总次数
    HI_U16 snd_affirm_cnt;       //发送完成通知的次数 ok
    HI_U16 snd_affirm_fail_cnt;  //发送完成通知失败的次数 ok
    HI_U16 snd_affirm_full_cnt;  //发送完成通知但队列满的次数 ok
    HI_U32 snd_affirm_LTS;       //最后一次发送完成通知的时间戳 ok

    HI_U32 snd_stop_LTS;         //最后一次发送停止升级的时间戳ok
    HI_U32 snd_stop_req_upg_id; //最后一次发送停止升级时停止升级帧的UPG ID ok
    HI_U16 snd_stop_cnt;         //发送停止升级的次数ok
    HI_U16 snd_stop_fail_cnt;    //发送停止升级失败的次数ok
    HI_U16 snd_stop_full_cnt;    //发送停止升级但队列满的次数ok

    HI_U16 snd_start_cnt;        //发送开始升级的次数ok
    HI_U16 snd_start_fail_cnt;   //发送开始升级失败的次数ok
    HI_U16 snd_start_full_cnt;   //发送开始升级但队列满的次数ok
    HI_U32 snd_start_LTS;        //最后一次发送开始升级的时间戳ok
    HI_U16 stat_rcv_reply_start_discard_cnt; //回复开始升级丢弃次数 OK
    HI_U16 stat_rcv_reply_start_cnt;      //当前升级回复开始升级的次数，不包括丢弃部分 ok
    HI_U32 stat_rcv_reply_start_LTS; //最后一次成功处理接收到的START帧时间戳 ok

    HI_U32 upg_finish_TS;        //全网升级结束时间戳
} HI_DSID_APP_UPG_CCO_STAT_S;//(0xa502)


//cco策略相关统计量
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U32 bc_entry_LTS;             //广播策略进入时间戳ok
    HI_U32 u2b_entry_LTS;            //U2B策略进入时间戳ok
    HI_U32 uc_entry_LTS;             //单播策略进入时间戳ok

    HI_U16 bc_loop;                  //广播策略已发送轮数ok
    HI_U16 u2b_proc_cnt;             //U2B策略已处理代理个数ok
    HI_U16 uc_proc_cnt;              //单播策略已处理站点个数ok
    HI_U8 ucFinishTimerCnt;      //升级下结论前启定时器的个数
    HI_U8 ucPlcMode;             //PLC模式

    HI_UPG_U2B_PROC_DETAIL u2b_proc_detail[HI_UPG_PROC_DETAIL_CNT]; //u2b策略当前处理的细节数据，记录最大3路并发ok
    HI_UPG_UC_PROC_DETAIL  uc_proc_detail[HI_UPG_PROC_DETAIL_CNT];  //单播策略当前处理的细节数据，记录最大3路并发ok
} HI_DSID_APP_UPG_CCO_STAT1_S;//(0xa504)


//cco通道及全网状态变更统计量
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U16 stat_proc_cnt;        //当前升级传输中的个数ok
    HI_U16 stat_verfiy_cnt;      //当前验证中的个数ok
    HI_U16 stat_succ_cnt;        //当前成功的个数ok
    HI_U16 stat_fail_cnt;        //当前失败的个数ok

    HI_U32 network_notify_times;     //通道变化次数 ok
    HI_UPG_NETWORK_FORMED_STRU network_notify_st[NETWORK_NOTIFY_REC_TIMES];//最近5次通道变化情况 ok
    HI_UPG_CTX_STATUS_CHANGE_STRU upg_ctx_change[UPG_CTX_STATUS_CHANGE_REC_TIMES]; //全网状态变更记录 ok
} HI_DSID_APP_UPG_CCO_STAT2_S;//(0xa505)

//全网升级CCO发送查询及置站点升级状态异常信息
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_UPG_CCO_QUERY_CLIENT_INFO struCcoQueryClientDetail[UPG_CCO_REC_SND_QUERY_CNT]; //CCO最近3次发送查询信息
    HI_UPG_CCO_QUERY_EXCEPTION_STRU struCcoSetExceDetail; //CCO设置站点升级状态异常信息

}HI_DSID_APP_UPG_CCO_STAT3_S;//96字节(0xa507)

typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_UPG_RCV_STA_COMPLETED_STRU bcRcvCompleteDetail;//广播策略接收到站点升级完成站点个数
    HI_UPG_RCV_STA_COMPLETED_STRU u2bRcvCompleteDetail;//U2B策略接收到站点升级完成站点个数
    HI_UPG_RCV_STA_COMPLETED_STRU ucRcvCompleteDetail;//单播策略接收到站点升级完成站点个数
}HI_DSID_APP_UPG_CCO_STAT4_S;//32字节(0xa509)

typedef struct
{
    HI_OBJ64_S stObjCnt;
    HI_UPG_CHECK_COMPLETE_INFO stCheckInfo[3];//站点状态信息

    HI_UPG_ITEM_DETAIL stBcAddProcessItemInfo;//广播策略添加处理表站点信息
    HI_UPG_ITEM_DETAIL stBcRemoveProcessItemInfo;//广播策略移除处理表站点信息
    HI_UPG_BSP_WAIT_INFO stBspWaitInfo; //断电续传前已启动延时定时器个数信息
    HI_U8 ucLargeProxy;              //是否存在大代理:55存在大代理；22不存在大代理给自己升级
    HI_U8 ucLargeProxyMac[3];
    HI_U16 usQueryMaxException;      //查询达到最大次数被置为升级状态异常站点个数
    HI_U16 usUnknownStatusException; //升级状态未知被置为升级状态异常站点个数
    HI_U16 usU2BDataMaxException;    //U2B发送轮数达到上限被置为升级状态异常站点个数
    HI_U16 usUCDataMaxException;     //UC发送论述达到上限被置为升级状态异常站点个数
    HI_U16 usStatusReplyDiscardPara; //由于入参不对丢弃状态查询应答
    HI_U16 usStatusReplyDiscardUpgId; //由于升级ID不对丢弃状态查询应答
    HI_U32 ulVerifyRet; //升级文件校验结果
    HI_U32 ulWriteRet;  //升级文件写Flash结果
}HI_DSID_APP_UPG_CCO_STAT5_S; //广播策略处理表中站点相关统计量 96字节(0xa50a)

typedef struct
{
    HI_UPG_SND_INFO stSndStartInfo; //单播发送开始升级帧的信息
    HI_UPG_ITEM_DETAIL stLastSndStartClientInfo;//最后一次发送开始升级帧成功的站点信息

    HI_UPG_SND_INFO stSndStatusInfo;//单播发送查询帧的信息
    HI_UPG_ITEM_DETAIL stLastSndStatusClientInfo;//最后一次发送状态查询帧的站点信息

    HI_UPG_SND_INFO stSndBlkInfo;//发送数据帧的信息
}HI_DSID_APP_UPG_SND_INFO_S;//升级策略发送信息 72字节(0xa50e/0xa511)

typedef struct
{
    HI_U32 ulFileCrc;
    HI_U32 ulFileLen;
    HI_U32 ulTxPlcCnt;
    HI_U32 ulRxPlcCnt;
    HI_U32 ulLastTxTime; //单位:毫秒
    HI_U32 ulLastRxTime; //单位:毫秒
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
    HI_U8 write_backup_flash : 1; /* 是否写备份区FLASH */
    HI_U8 chip_mode : 2; /* 芯片模式 */
    HI_U8 reserved : 5;
}HI_DSID_APP_UPG_CCO_QUERY_INFO_S;//(0xa512)

typedef struct
{
    HI_U32 ulOption;
    HI_U32 ulFileCrc;
    HI_U32 ulFileLen;
    HI_U32 ulRxInfoPlcTime[2]; //单位:毫秒
    HI_U32 ulTxInfoPlcTime[2]; //单位:毫秒
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
    HI_U8  ucEndReason; //0:默认值 1:成功 2:超时 3:UPG查询的mrs状态为失败 4:UPG查询的mrs状态为空闲
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
    HI_U16 usRcvStaNum;//已接收STA总个数
    HI_U16 usTotalStaNum;   //STA总个数
    HI_U8 ucRcvPacket;      //已接收帧个数
    HI_U8 ucTotalPacket;    //帧总个数
    HI_U8 bCcoInList : 1;   //列表中是否包含CCO
    HI_U8 bStaInList : 1;   //列表中是否包含除CCO外的其他点
    HI_U8 bOptionMatchList : 1;//升级选项与列表是否匹配
    HI_U8 ulUpgOption : 2; //CCO升级STA方式
    HI_U8 pad1 : 3;
    HI_U8 user_proto_type;//用户协议类型
    HI_U32 ulTimerPeriod; //定时器周期
    HI_U32 ulStartRcvTimerTimeStamp;   //启接收文件超时定时器时间戳
    HI_U32 ulActRcvTimerTimeStamp;    //响应接收文件超时定时器时间戳
    HI_U32 ulStartListTimerTimeStamp; //启接收列表超时定时器时间戳
    HI_U32 ulActListTimerTimeStamp;   //响应接收列表超时定时器时间戳
    HI_U32 ulListMemSize;  //列表内存大小
    HI_U32 ulReleaseMemTimeStamp; //释放列表内存时间戳
    HI_U32 ulWriteListRet; //将List写入FLASH错误码
    HI_U32 ulGetListRet;   //从FLASH中获取列表错误码
    HI_U32 ulAllStaInTopoTimeStamp;  //所有STA入网时间戳
    HI_U16 usStatusNotRight; //STA上报的bitmap为全FF,状态不符合预期个数
    HI_U16 pad4;
    HI_U32 set_user_proto_timeStamp;//设置用户协议时间戳
    HI_U32 pad3;
}HI_DSID_APP_UPG_LIST_INFO_S;//(0xa515)

typedef struct
{
    HI_UPG_SND_INFO stSndStartBcInfo; //广播发发送开始升级信息
    HI_DSID_APP_UPG_SND_INFO_S stSndInfo; //广播策略发送信息
    HI_U16 usBcNeedSndBlkCntLatestLoop;//最近一轮需要发送数据包的个数:在下一次发送升级数据前开始前清除
    HI_U16 usProtoVerFromMac;
}HI_DSID_APP_UPG_BC_SND_INFO_S;//广播策略发送统计信息 92字节(0xa50b)

typedef struct
{
    HI_UPG_CHECK_COMPLETE_INFO stCheckInfo[3];//站点状态信息
    HI_UPG_PROCESS_ITEM_SND_INFO stProcessItemSndInfo[HI_UPG_PROC_DETAIL_CNT];//处理表最近3个站点需要发送数据包信息 :在下一次发送升级数据前开始前清除
}HI_DSID_APP_UPG_CHECK_S;//处理表中站点相关统计量 72字节 (0xa50c/0xa50f)

typedef struct
{
    HI_UPG_U2B_PROCESS_TABLE_ITEM stU2bAddProcessItemInfo[HI_UPG_PROC_DETAIL_CNT]; //U2B策略最近3次添加处理表站点信息 ok
    HI_UPG_ITEM_DETAIL stU2bRemoveProcessItemInfo[HI_UPG_PROC_DETAIL_CNT];  //U2B策略最近3次移除处理表站点信息 ok
}HI_DSID_APP_UPG_U2B_PROCESS_ITEM_S;//U2b策略添加移除处理表信息 96字节(0xa50d)

typedef struct
{
    HI_UPG_ITEM_DETAIL stUcAddProcessItemInfo[HI_UPG_PROC_DETAIL_CNT];//单播策略最近3次添加处理表站点信息 ok
    HI_UPG_ITEM_DETAIL stUcRemoveProcessItemInfo[HI_UPG_PROC_DETAIL_CNT];//单播策略最近3次移除处理表站点信息 ok
    HI_U32 obtain_cache_time;
    HI_U32 obtain_cache_addr;
    HI_U32 obtain_cache_size;
    HI_U32 release_cache_time;
    HI_U32 release_cache_addr;
    HI_U32 reserved;    
}HI_DSID_APP_UPG_UC_PROCESS_ITEM_S;//单播策略添加移除处理表信息 96字节(0xa510)

typedef struct
{
    HI_U8 ucRegCbCnt;
    HI_U8 ucRcvStartReqCnt;   //接收开始升级消息次数
    HI_U8 ucCbStartCnt;       //调用开始升级回调次数
    HI_U8 reg_protocol_type;  //用户注册的协议类型
    HI_U32 ulLastRcvStartTime;
    HI_U32 ulLastCbStartTime;

    HI_U8 ucRcvStatusReqCnt;  //接收升级状态查询消息次数
    HI_U8 ucCbBsStatusCnt;   //调用基本升级状态回调次数
    HI_U8 ucCbStatusCnt;     //调用升级状态回调次数
    HI_U8 ucCbStatStatusCnt; //调用升级统计信息回调次数
    HI_U32 ulLastRcvStatusTime;
    HI_U32 ulLastCbStatusTime;

    HI_U8 ucRcvStopReqCnt; //接收停止升级消息次数
    HI_U8 ucCbStopCnt;     //调用停止升级回调次数
    HI_U16 rcv_transfile_cnt;
    HI_U32 ulLastRcvStopTime;
    HI_U32 ulLastCbStopTime;
    HI_U32 reg_protocol_time; //注册协议类型时间戳
    HI_U32 last_rcv_transfile_time;
    HI_U32 pad;
}HI_DSID_APP_UPG_CCO_SDK_STAT_S;//(0xa516 44字节)

typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U16 rcv_blk_cnt;         // 已接收升级数据帧总数ok
    HI_U16 rcv_blk_LSN;         // 最后一次接收数据的顺序号ok
    HI_U32 rcv_blk_LTS;         // 最后一次接收数据的时间戳ok
    HI_U32 check_rcv_block_LRC; //最近一次校验升级包的返回值  OK
    HI_U32 check_rcv_block_LTS; //最近一次校验升级包的时间戳  OK
    HI_U32 cache_rcv_block_LRC;//最近一次缓存接收到的数据返回值 OK
    HI_U32 cache_rcv_block_LTS;//最近一次缓存接收到的数据时间戳 OK
    HI_U32 last_Edge_Id;      //最近一次给HSO上报的边界值 OK
    HI_U32 last_Edge_Id_LTS;   //最近一次给HSO上报的边界值的时间戳 OK
    HI_U32 verify_upg_file_LRC;//升级文件校验结果 OK
    HI_U32 verify_upg_file_LTS;//升级文件校验时间戳OK

    HI_U16 rcv_start_cnt;       // 接收到start帧数ok
    HI_U16 rcv_start_LRC;       // 最后一次调用start函数返回值ok
    HI_U32 rcv_start_LTS;       // 最后一次调用start函数成功的时间戳ok
    HI_U16 usRcvU2bBlkCnt;      // 接收到U2B报文并转发的次数

    HI_U16 reply_start_cnt;     //应答START帧次数 ok
    HI_U32 reply_start_LTS;     //最后一次成功应答START帧的时间戳 ok
    HI_U16 reply_start_fail_cnt; //应答START帧失败次数 ok
    HI_U16 reply_start_full_cnt; //应答START帧队列满次数 ok
    HI_U32 reply_start_LRC;  //最后一次应答开始升级帧的返回值 ok

    HI_U8 ucErrorParaStart;  //接收到错误开始升级帧次数
    HI_U8 ucDiscardRcvStart; //接收到开始升级帧不处理次数
    HI_U8 ucStdMallocFail;   //国网模式分配内存失败次数
    HI_U8 ucAffirmDiscardCnt; //丢弃全网完成通知的次数
    HI_U8 reserved3[2];
    HI_U8 bUpgradeSelf;     //是否需要给自己升级:55需要给自己升级/22不需要给自己升级
    HI_U8 ucPlcMode;        //PLC协议模式

} HI_DSID_APP_UPG_STA_STAT_S;//(0xa503)

typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_U32 rcv_status_LTS;      // 最后一次收到状态查询帧的时间戳ok
    HI_U16 rcv_status_cnt;      // 接收到状态查询帧的次数ok
    HI_U16 rcv_start_FRC;       // 第一次调用开始升级函数的返回值

    HI_U32 ulWriteRet;         //写Flash结果
    HI_U32 reply_status_LTS;    //最后一次应答状态查询的时间戳 ok
    HI_U16 reply_status_cnt;    // 应答状态查询帧的次数ok
    HI_U16 reply_status_fail_cnt; //应答状态查询帧失败次数 ok
    HI_U16 reply_status_full_cnt; //应答状态查询帧队列满次数 ok
    HI_U8  reply_status_upg_status; //最后一次应答状态查询时单板状态 ok
    HI_U8  reply_status_report_cnt; //STA主动上报状态次数

    HI_U32 finish_timer_set_LTS; //最后一次设置finish或affirm定时器时间戳 ok
    HI_U32 finish_timer_period; //最后一次设置finish或affirm定时器定时器周期 ok

    HI_U32 rcv_affirm_LTS;      //最后一次接收到完成通知的时间戳 ok
    HI_U16 rcv_affirm_cnt;      //接收到完成通知并处理的次数 ok

    HI_U16 rcv_stop_cnt;        //接收到停止升级响应次数ok
    HI_U16 rcv_stop_denied_cnt; //拒绝停止升级次数 ok
    HI_U16 padding;
    HI_U32 rcv_stop_LRC;        //最后一次收到停止升级的返回值
    HI_U32 rcv_stop_LTS;        //最后一次停止升级的时间戳ok
    HI_U32 rcv_stop_upg_id;      //接收到停止升级时模块当前升级的UPG ID ok
    HI_U32 rcv_stop_req_upg_id;  //接收到停止升级时请求停止升级的UPG ID ok
} HI_DSID_APP_UPG_STA_STAT1_S;//(0xa506)

//站点应答状态查询信息
typedef struct
{
    HI_OBJ64_S stObjCnt;

    HI_UPG_STA_REPLY_QUERY_INFO struStaReplyQueryDetail[UPG_STA_REC_REPLY_STATUS_CNT];//STA站点最后3次应答状态查询信息

}HI_DSID_APP_UPG_STA_STAT2_S;//44字节(0xa508)

typedef struct
{
    HI_U8 ucRegCbCnt;
    HI_U8 ucRcvStartReqCnt;   //接收开始升级消息次数
    HI_U8 ucCbStartCnt;       //调用开始升级回调次数
    HI_U8 reg_protocol_type; //用户注册的协议类型
    HI_U32 ulLastRcvStartTime;
    HI_U32 ulLastCbStartTime;

    HI_U8 ucRcvStatusReqCnt;  //接收升级状态查询消息次数
    HI_U8 ucCbBsStatusCnt;   //调用基本升级状态回调次数
    HI_U8 ucCbStatusCnt;     //调用升级状态回调次数
    HI_U8 pad2;
    HI_U32 ulLastRcvStatusTime;
    HI_U32 ulLastCbStatusTime;

    HI_U8 ucRcvStopReqCnt; //接收停止升级消息次数
    HI_U8 ucCbStopCnt;     //调用停止升级回调次数
    HI_U16 pad3;
    HI_U32 ulLastRcvStopTime;
    HI_U32 ulLastCbStopTime;

    HI_U8 ucRcvDeviceStartIndCnt;    //接收设备升级开始应答消息次数
    HI_U8 ucCbDeviceStartCnt;      //调用设备升级开始请求回调次数
    HI_U16 pad4;
    HI_U32 ulLastRcvDeviceStartTime;
    HI_U32 ulLastCbDeviceStartTime;

    HI_U8 ucRcvDeviceStatusIndCnt;  //接收设备升级状态应答消息次数
    HI_U8 ucCbDeviceStatusCnt;     //调用设备升级状态请求回调次数
    HI_U16 pad5;
    HI_U32 ulLastRcvDeviceStatusTime;
    HI_U32 ulLastCbDeviceStatusTime;

    HI_U8 ucRcvDeviceStopIndCnt;   //接收设备升级停止应答消息次数
    HI_U8 ucCbDeviceStopCnt;     //调用设备升级停止请求回调次数
    HI_U16 rcv_transfile_cnt;
    HI_U32 ulLastRcvDeviceStopTime;
    HI_U32 ulLastCbDeviceStopTime;
    HI_U32 last_rcv_transfile_time;
    HI_U32 reg_protocol_time;//用户注册协议的时间戳
    HI_U32 pad;
}HI_DSID_APP_UPG_STA_SDK_STAT_S;//(0xa517 84字节)

//*****************************************************************************
// APP消息定义
//*****************************************************************************
// 范围 [0B00, 0F00)
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
// 结构定义
//*****************************************************************************
#define APP_MAX_BUF_LEN 96     // 最大上报缓冲区长度

typedef struct
{
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];    // 发生改变的Mac地址
    HI_S16 index;   // Mac索引
    HI_U8 option;   // 操作类型
}HI_DMID_APP_MAC_LIST_CHANGE_S; // MacList改变

typedef struct
{
    HI_U8 addr[METER_ADDR_LEN]; // 发生改变的表地址
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];    // 发生改变的Mac地址
    HI_U8 option;   // 操作类型
}HI_DMID_APP_MAC_MAP_CHANGE_S;  // MacMap改变

typedef struct
{
    HI_U16 len;     // 接收到的数据长度
    HI_U8 data[APP_MAX_BUF_LEN];   // 接收到的数据内容
}HI_DMID_APP_CHL_RX_UART_S; // 抄表通道数据接收

typedef struct
{
    HI_U16 len;     // 发送的数据长度
    HI_U8 data[APP_MAX_BUF_LEN];   // 发送的数据内容
}HI_DMID_APP_CHL_TX_UART_S; // 抄表通道数据发送

typedef struct
{
    HI_U16 id;      // 接收到PLC帧的ID
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];    // 源Mac
    HI_U16 len;     // 接收到PLC帧结构的长度
    HI_U8 data[APP_MAX_BUF_LEN];   // 接收到PLC帧结构的内容
}HI_DMID_APP_CHL_RX_PLC_S;  // PLC通道数据接收

typedef struct
{
    HI_U16 id;      // 发送PLC帧的ID
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];    // 目的Mac
    HI_U16 len;     // 发送PLC帧的长度
    HI_U8 data[APP_MAX_BUF_LEN];   // 发送PLC帧的内容
}HI_DMID_APP_CHL_TX_PLC_S;  // PLC通道数据发送

typedef struct
{
    HI_U8 type;     // 通道类型
    HI_U8 state;    // 通道状态
}HI_DMID_APP_CHL_STATUS_S;  // 当前通道状态通知

typedef struct
{
    HI_U16 id;  // 定时器ID
    HI_U32 value; // 定时器值
    HI_U8 option; // 定时器类型
}HI_DMID_APP_TIMER_START_S; // 定时器启动

typedef struct
{
    HI_U16 id;  // 定时器ID
}HI_DMID_APP_TIMER_STOP_S;  // 定时器停止

typedef struct
{
    HI_U16 id;  // 定时器ID
    HI_U32 p;   // 定时器回调函数指针
}HI_DMID_APP_TIMER_ACTIV_S; // 定时器激活

typedef struct
{
    HI_U16 len;     // 数据长度
    HI_U8 data[APP_MAX_BUF_LEN];   // 数据内容
}HI_DMID_APP_RX_FRAME_COMPLETE_S;   // 接收到完整帧

typedef struct
{
    HI_U8 ctrl;     // 控制字
    HI_U8 afn;      // 功能码
    HI_U8 fn;       // 信息单元
}HI_DMID_APP_376_2_DEC_COMPLETE_S;  // 376.2帧结构解析完成

typedef struct
{
    HI_U8 protocol; // 规约类型
    HI_U8 ctrl;     // 控制字
    HI_U32 id;      // 数据ID
}HI_DMID_APP_645_DEC_COMPLETE_S;    // 645帧结构解析完成

typedef struct
{
    HI_U32 id;  // 会话ID
    HI_U32 sn;  // 会话SN
    HI_U8 cnt;  // CNT标志
    HI_U8 acd;  // ACD标志
}HI_DMID_APP_SES_ENC_COMPLETE_S;    // 会话组织完毕

typedef struct
{
    HI_U32 id[2];   // 会话ID ([0]帧 [1]本地)
    HI_U32 sn[2];   // 会话SN ([0]帧 [1]本地)
    HI_U8 cnt;      // CNT标志
    HI_U8 acd;      // 会话无效原因
}HI_DMID_APP_SES_CHECK_COMPLETE_S;  // 会话检查完毕

typedef struct
{
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];    // 对应的MAC地址
    HI_U16 len;     // PLC帧长度
    HI_U8 data[APP_MAX_BUF_LEN];   // PLC帧结构内容
}HI_DMID_APP_TX_GET_ADDR_PLC_S; // 发送获取电表地址PLC帧

typedef struct
{
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];    // 对应的MAC地址
    HI_U8 num;  // 对应的表地址个数
    METER_ADDR meter[MAX_METER_NUM];    // 对应的表地址列表
    HI_U16 len; // PLC帧长度
    HI_U8 data[APP_MAX_BUF_LEN];    // PLC帧结构内容
}HI_DMID_APP_RX_GET_ADDR_PLC_S; // 接收获取电表地址PLC帧

typedef struct
{
    HI_U8 addr[METER_ADDR_LEN];     // 表地址
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];        // 对应的MAC地址
}HI_DMID_APP_FIND_MAC_MAP_OK_S; // 查找MAC映射表成功

typedef struct
{
    HI_U8 addr[METER_ADDR_LEN];     // 表地址
    HI_U8 result;     // 失败原因
}HI_DMID_APP_FIND_MAC_MAP_FAIL_S;   // 查找MAC映射表失败

typedef struct
{
    HI_U8 addr[METER_ADDR_LEN];     // 当前处理节点的电表地址
    HI_U16 index;   // 当前处理节点的序号
    HI_U16 len; // 帧长度
    HI_U8 data[APP_MAX_BUF_LEN];   // 帧内容
}HI_DMID_APP_INQ_METER_S;   // 发送读表询问帧

typedef struct
{
    HI_U8 addr[METER_ADDR_LEN];     // 应答读表的表地址
    HI_U8 read_flg;     // 应答读表的抄读标志
    HI_U16 len;     // 应答读表的报文长度
    HI_U8 data[APP_MAX_BUF_LEN];   // 应答读表的报文内容
}HI_DMID_APP_INQ_ANS_METER_S;   // 接收读表询问应答帧

typedef struct
{
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];     // 目的Mac
    HI_U16 len;     // 读表PLC结构长度
    HI_U8 data[APP_MAX_BUF_LEN];   // 读表PLC结构内容
}HI_DMID_APP_TX_RM_PLC_CCO_S;   // 发送读表PLC帧(CCO)

typedef struct
{
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];     // 源Mac
    HI_U16 len;     // 读表PLC结构长度
    HI_U8 data[APP_MAX_BUF_LEN];   // 读表PLC结构内容
}HI_DMID_APP_RX_RM_PLC_CCO_S;   // 接收读表PLC帧(CCO)

typedef struct
{
    HI_U16 len;     // 读表PLC结构长度
    HI_U8 data[APP_MAX_BUF_LEN];   // 读表PLC结构内容
}HI_DMID_APP_TX_RM_PLC_STA_S;   // 发送读表PLC帧(STA)

typedef struct
{
    HI_U16 len;     // 读表PLC结构长度
    HI_U8 data[APP_MAX_BUF_LEN];   // 读表PLC结构内容
}HI_DMID_APP_RX_RM_PLC_STA_S;   // 接收读表PLC帧(STA)

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
    APP_TIME_S start_time;  // 搜表开始时间
    HI_U16 duration;    // 搜表持续时间
    HI_U8 retry;    // 从节点重发次数
    HI_U8 slice;    // 随机等待时间片个数
}HI_DMID_APP_START_SCAN_METER_S;    // 启动搜表

typedef struct
{
    HI_U8 num;  // 上报的从节点数量
    METER_ADDR meter[MAX_METER_NUM]; // 从节点地址
    HI_U8 protocol;     // 规约类型
    HI_U16 index;   // 从节点序号
    HI_U8 type;     // 设备类型
}HI_DMID_APP_REP_SCAN_METER_S;  // 上报搜表信息

typedef struct
{
    HI_U8 from; // 数据来源
    HI_U16 len; // 数据长度
    HI_U8 data[APP_MAX_BUF_LEN];   // 数据内容
}HI_DMID_APP_TX_RM_STA_S;    // 发送读表数据(STA)

typedef struct
{
    HI_U16 len; // 数据长度
    HI_U8 data[APP_MAX_BUF_LEN];   // 数据内容
}HI_DMID_APP_RX_RM_STA_S;   // 接收读表数据(STA)

typedef struct
{
    HI_U8 addr[METER_ADDR_LEN]; // 上报的表地址
    HI_U16 len; // 上报的帧长度
    HI_U8 data[APP_MAX_BUF_LEN];   // 上报的帧内容
}HI_DMID_APP_REP_RM_CCO_S;  // 上报读表数据(CCO)

typedef struct
{
    HI_U16 len; // 心跳帧长度
    HI_U8 data[APP_MAX_BUF_LEN]; // 心跳帧内容
}HI_DMID_APP_TX_HEARTBEAT_S;    // 发送心跳帧

typedef struct
{
    HI_U8 num;  // 当前表地址个数
    METER_ADDR meter[MAX_METER_NUM];    // 表地址列表(最大32个元素)
}HI_DMID_APP_UPD_MADDR_S;   // 更新电表地址列表

typedef struct
{
    HI_U8 num;  // 当前队列元素个数
    HI_U32 id[MAX_QUEUE_NUM];   // 队列ID列表
    HI_U8 form; // 数据来源
    HI_U8 top_state;    // 队首状态(是否锁定)
}HI_DMID_APP_QUE_STATUS_S;  // 当前队列状态

typedef struct
{
    HI_U16 num;     // 当前MacTopo个数
    MAC_ADDR mac_list[20];  // 前20个Topo的Mac地址
}HI_DMID_APP_MACTOPO_STATUS_S;  // 当前MacTopo表状态


//*****************************************************************************
// System 统计量定义
//*****************************************************************************
// 范围 [0xB000, 0xB200)
//*****************************************************************************
#define HI_DSID_UPG_TM                  0xB000    // HI_DSID_UPG_TM_S
#define HI_DSID_UPG_BLK_TM              0xB001    // HI_DSID_UPG_BLK_TM_S
#define HI_DSID_UPG_BROSTART_STATUS     0xB002    // HI_UPG_STAT_INFO_IND_S

#define HI_DSID_WD_INFO                 0xB100    // HI_DSID_WD_INFO_S
#define HI_DSID_DIAG_CHL                0xB101    // HI_DSID_DIAG_CHL_S
#define HI_DSID_LOGMSG                  0xB102    // HI_DSID_LOGMSG_S
#define HI_DSID_DIAG_BUF_STAT           0xB103    //HI_DIAG_BUF_STAT_S

//
// 升级记录
//

// 块最大耗时统计
typedef struct
{
    HI_U32 idx;              // 块索引
    HI_U32 tm;               // 处理升级块耗时
    HI_U32 crc;              // 处理升级块校验耗时, -- APP,DRV,MAC?

    HI_U32 start;             // 处理升级块开始
    HI_U32 end;               // 处理升级块结束
}HI_DSID_UPG_BLK_TM_MAX_S;

#if defined(PRODUCT_CFG_DFX_PLC_FRM_STAT)
typedef struct
{
    HI_OBJ64_S objcnt;

    // 基本信息
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
// 每个块的传输记时
//
#define HI_DSID_UPG_BLK_ARQ_CNT    30
#if defined(PRODUCT_CFG_DFX_PLC_FRM_BLK_STAT)
typedef struct
{
    HI_OBJ64_S objcnt;

    HI_U16 blk_req_cnt; //  hso的块自动重传次数    -- APP
    HI_U16 blk_arq_cnt; // 块自动重传次数(CCo-STA) -- APP
    HI_U16 blk_idx;     // 块序号                  -- APP
    HI_U16 blk_size;    // 块大小                  -- APP

    // 处理 处理升级块起始记录
    HI_U16 app_start;  // APP处理升级块开始 -- APP
    HI_U16 app_end;    // APP处理升级块结束 -- APP

    HI_U16 drv_start;  // DRV处理升级块开始 -- DRV
    HI_U16 drv_end;    // DRV处理升级块结束 -- DRV

    HI_U16 mac_start;      // MAC处理升级块开始 -- MAC
    HI_U16 mac_end;        // MAC处理升级块结束 -- MAC
    HI_U16 mac_arq_cnt;    // 重传次数
    HI_U8 mac_pb_type;     // 0: 136; 1:520
    HI_U8 mac_pb_num;      // pb块个数

    HI_U32 blk_err;        // 块传输错误码 -- APP
}HI_DSID_UPG_BLK_TM_S;
#endif


#if defined(PRODUCT_CFG_DFX_PLC_FRM_BLK_STAT)
#define HI_DSID_UPG_BLK_NUM_MAX    700
#endif

#define HI_DSID_UPG_BLK_CURRENT    0xFFFF

//****************************************************************************

// DIAG 缓存类别的最大个数
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
// 函数名称: HI_SYS_DfxPlcFrmStat
// 功能描述: 获取升级记录的统计量对象
//
// 参数说明:
//   无
//
// 返 回 值:
//    HI_DSID_UPG_TM_S*对象,可以直接对成员赋值.
//
// 调用要求:
// 调用举例:
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
// 函数名称: HI_SYS_DfxPlcFrmBlkStat
// 功能描述: 获取升级块传输的时间记录的统计量对象
//
// 参数说明:
//   usBlkIdx [in] usBlkIdx 必须小于 HI_DSID_UPG_BLK_NUM_MAX,
//                 如果 usBlkIdx等于 HI_DSID_UPG_BLK_CURRENT, 表示获取当前块
//
// 返 回 值:
//    HI_DSID_UPG_BLK_TM_S*对象,可以直接对成员赋值.
//    HI_NULL: 无对应块
//
// 调用要求:
// 调用举例:
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
// 系统消息定义
//*****************************************************************************
// 范围 [0x2000, 0x2100)
//*****************************************************************************
#define HI_DMID_UPG_REQ_START              0x2000  // HI_DMID_UPG_REQ_START_S           升级开始请求
#define HI_DMID_UPG_REQ_STOP               0x2001  // HI_DMID_UPG_REQ_STOP_S            升级停止请求
#define HI_DMID_UPG_REQ_VER_QRY            0x2002  // HI_DMID_UPG_REQ_VER_QRY_S         升级版本查询请求
#define HI_DMID_UPG_REQ_STATUS             0x2003  // HI_DMID_UPG_REQ_STATUS_S          升级状态查询请求
#define HI_DMID_UPG_BROSTART_START         0x2010  // HI_DMID_UPG_BROSTART_START_S      全网升级启动          -- CCO
#define HI_DMID_UPG_BROSTART_STOP          0x2011  // HI_DMID_UPG_BROSTART_STOP_S       全网升级停止          (仅在每轮开始上报) -- CCO
#define HI_DMID_UPG_BROSTART_TRANSMIT      0x2012  // HI_DMID_UPG_BROSTART_TRANSMIT_S   全网升级块传输       (仅在每轮开始上报) -- CCO
#define HI_DMID_UPG_BROSTART_STATUS_IND    0x2013  // HI_DMID_UPG_BROSTART_STATUS_IND_S 全网升级状态上报       -- CCO(仅上报统计量，不上报所有节点状态)
#define HI_DMID_UPG_BROSTART_RESULT        0x2014  // HI_DMID_UPG_BROSTART_RESULT_S     全网升级发送结果
#define HI_DMID_UPG_UNICAST_START          0x2020  // HI_DMID_UPG_UNICAST_START_S       STA升级启动           -- STA
#define HI_DMID_UPG_UNICAST_STOP           0x2021  // HI_DMID_UPG_UNICAST_STOP_S        STA升级停止           (仅在每轮开始上报) -- STA
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
// System 配置命令定义
//*****************************************************************************
// 范围 [0xA001, 0xB000)
// 命令添加必须是按顺序添加, 且对于大小小于 HI_DCID_SYS_CMD_FIXED_LEN
//*****************************************************************************
#define HI_DCID_UPG_CHL_CFG          0xA000    // HI_DCID_UPG_CHL_CFG_S
#define HI_DCID_SAL_DBG_SWT_CFG      0xA001    // HI_DCID_SAL_DBG_SWT_CFG_S
#define HI_DCID_STR                  0xA010
#define HI_DCID_SYS_NUM              4         // 所有命令个数,加命令后需要调整该宏定义大小,重新编译SAL

#define HI_DCID_SYS_CMD_FIXED_LEN    64        // 命令参数的长度最大为64
#define HI_DCID_SYS_BUFFER_LEN       (HI_DCID_SYS_CMD_FIXED_LEN*HI_DCID_SYS_NUM) // 所有命令的数据结构大小总和

//*****************************************************************************
typedef HI_NV_FTM_UPG_CFG_S HI_DCID_UPG_CHL_CFG_S;

typedef struct
{
    HI_U8 cfg[8];   // [0]: mac->dms; [1]: dms->mac; [2]: dump send 2 tcp port time
    HI_U32 data[8]; // [0]: cmd id
}HI_DCID_SAL_DBG_SWT_CFG_S;
//*****************************************************************************

//*****************************************************************************
// 范围 [0xB050, 0xB200)
//*****************************************************************************
// HI_MDM_EQT_CHL_PK               0xB000    // HI_MDM_EQT_CHL_PK_REQ_S  HI_MDM_EQT_CHL_PK_IND_S
#define HI_SYS_RESERVED            0xB001    // REQ, ACK, IND?


//*****************************************************************************
// 获取配置命令数据 如 HI_DCID_UPG_CHL_CFG_S HI_DCID_SAL_DBG_CFG_S
// ulSysCid: 配置命令定义,范围 [0xA001, 0xB000)
//*****************************************************************************
HI_EAPI HI_PVOID HI_SYS_GetSysCmdCfg(HI_U32 ulSysCid);
HI_EAPI HI_U16   HI_SYS_GetSysCmdCfgLen(HI_U32 ulSysCid);

//*****************************************************************************
// 系统命令 注册
// ulSysCid: 配置命令定义,范围 [0xB000, 0xB050)
// usCfgLen: 取之 0
//*****************************************************************************
HI_EAPI HI_U32 HI_SYS_RegisterSysCmdCfg(HI_U32 ulSysCid, HI_U16 usCfgLen);

//*****************************************************************************
// 系统命令设置
// ulSysCid: 配置命令定义,范围 [0xA001, 0xB000)
// pCfgData: 配置buffer
// usCfgLen: 配置buffer长度
// bForce:  HI_TRUE: 写入NV; HI_FALSE: 不写入NV,仅缓存
//*****************************************************************************
HI_EAPI HI_U32 HI_SYS_SetSysCmdCfg(HI_U32 ulSysCid, HI_U8* pCfgData, HI_U16 usCfgLen, HI_BOOL bForce);

#define GET_HI_DCID_UPG_CHL_CFG_S()       ((HI_DCID_UPG_CHL_CFG_S*)HI_SYS_GetSysCmdCfg(HI_DCID_UPG_CHL_CFG))
#define GET_HI_DCID_SAL_DBG_CFG_S()       ((HI_DCID_SAL_DBG_SWT_CFG_S*)HI_SYS_GetSysCmdCfg(HI_DCID_SAL_DBG_SWT_CFG))

//*****************************************************************************


//*****************************************************************************
// CMN 统计量
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


