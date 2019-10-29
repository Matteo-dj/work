//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : hi_sal_nv.h
//  版 本 号   : V1.0
//  作    者   : shenhankun/00130424
//  生成日期   : 2011-10-26
//  功能描述   : SAL的NV项设置
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2011-10-26
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


#ifndef __HI_SAL_NV_H__
#define __HI_SAL_NV_H__

//*****************************************************************************
// 物理层 [0x0001-0x0100) SAL层 [0x0100-0x0150) MAC层 [0x0200-0x0400)
// NV ID 定义 [0x0100-0x0150)
//*****************************************************************************
#define  HI_NV_SAL_DCHL_CFG_ID   0x0100 // HI_NV_SAL_DCHL_CFG_ID_S
#define  HI_NV_SAL_DIAG_CONN_PWD_ID   0x0101 // HI_NV_SAL_DIAG_CONN_PWD_ID_S

#define  HI_NV_BSP_IO_REMAP_INIT_CONFIG_ID_1  0x0111            //V200_ASIC_CCO
#define  HI_NV_BSP_IO_REMAP_INIT_CONFIG_ID_2  0x0112            //NDM
#define  HI_NV_BSP_IO_REMAP_INIT_CONFIG_ID_3  0x0113            //RELAY
#define  HI_NV_BSP_IO_REMAP_INIT_CONFIG_ID_4  0x0114            //V200_ASIC_STA
#define  HI_NV_BSP_IO_REMAP_INIT_CONFIG_ID_5  0x0115            //V200_ASIC_IISTA
#define  HI_NV_BSP_IO_REMAP_INIT_CONFIG_ID_6  0x0116            //EQT
#define  HI_NV_BSP_IO_REMAP_INIT_CONFIG_ID_7  0x0117            //Three_Phase
#define  HI_NV_BSP_IO_REMAP_INIT_CONFIG_ID_8  0x0118            //Meter_Check
#define  HI_NV_BSP_IO_REMAP_INIT_CONFIG_ID_9  0x0119            //V100_CCO&V200_FPGA_CCO
#define  HI_NV_BSP_IO_REMAP_INIT_CONFIG_ID_A  0x011A            //V100_STA&V200_FPGA_STA
#define  HI_NV_BSP_IO_REMAP_INIT_CONFIG_ID_B  0x011B            //V100_II_STA
#define  HI_NV_FTM_DEVICE_CFG_ID              0x011C

/* BEGIN: Added by x00180816, 2014/2/28   问题单号:DTS2014012702376 */
#define HI_NV_BSP_TASK_STCSIZE        0x0105
#define HI_NV_BSP_TASK_QUEUE_SIZE     0x0106
/* END:   Added by x00180816, 2014/2/28 */
#define  HI_NV_SAL_MEM_STAT1_CFG_ID   0x0107
/* BEGIN: Added by b00208046, 2014/3/10   问题单号:DTS2014031301020  */
#define  HI_NV_SAL_CONFIG             0x0108// HI_NV_SAL_CONFIG_S
/* END:   Added by b00208046, 2014/3/10 */
#define  HI_NV_SYS_RST_TIMES            0x0109
#define HI_NV_SAL_NETWORK_SECURITY_CONFIG         0x01A0//密码校验功能
//*****************************************************************************

//*****************************************************************************
// NV项数据结构定义
//*****************************************************************************
#define HI_NV_SAL_TASK_DROP_IDX_RANGE_MAX    4

typedef struct
{
    HI_U16 LockTime;//账号锁定时长(秒)
    HI_U8 RetryMax;//密码校验尝试次数
    HI_U8 ChangePassWordEnable:1;//是否支持密码修改 1:支持 0:不支持
    HI_U8 LockLoginEnable:1;//是否支持账号锁定 1:支持 0:不支持
    HI_U8 reserver_1:6;
}HI_NV_SAL_NETWORK_SECURITY_S;

typedef struct
{
    HI_U8  ioInitFunc[48];		//IO功能配置 HI_HW_IO_IDX_MAX
    HI_U32 ioInitDir;			//IO方向配置
    HI_U32 ioInitLvl;			//IO电平配置
    HI_U16 ioExpDir;			//扩展IO方向寄存器
    HI_U16 ioExpOutput;			//扩展IO输出寄存器
}HI_IO_REMAP_INIT_CONFIG_NV_S;

/* BEGIN: Added by x00180816, 2014/2/28   问题单号:DTS2014012702376 */
typedef struct _task_stcsize_item_
{
    HI_U16 exe_high;  /* task execution stack */
    HI_U16 exc_high;  /* task exception stack */
}TASK_STCSIZE_ITEM;

typedef struct _task_stcsize_store_
    {
        TASK_STCSIZE_ITEM item[25];
    }TASK_STCSIZE_NV_S;

typedef struct _task_msg_queue_store_
    {
        HI_U32 MQueue[10];
        HI_U32 AQueue[3];
    }MSG_QUEUESTAT_NV_S;

/* END:   Added by x00180816, 2014/2/28 */
typedef struct _sys_rst_times_store_
{
    HI_U32 rst_times;   //重启次数统计
    HI_U32 ulRelativeRstTimes;//相对重启次数，可以被清空
    HI_U32 reserved[2];
}HI_SYS_RESET_TIMES_S;

typedef struct
{
    HI_U8  bSaveDiagItem2Flash;
    HI_U8  bReport2LocalFlashRamSwt;
    HI_U8  bSaveAllMsgPrint2Flash;
    HI_U8  bDiagChlClearRptCnt;             // DIAG通道连接后自动清除DIAG上报计数

    HI_U8  bResetExIsOn;                    // 管理 主动复位重启是否打开，默认打开(1),关闭(0)
    HI_U8  bRsvSwt1;                        // 未使用开关1
    HI_U8  bRsvSwt2;                        // 未使用开关2
    HI_U8  com1mode;

    // 以下为重启时间配置
    HI_U32 ulAnyMrFailTime;                 // 默认值24小时*60分*60秒           对应 HI_SYS_RST_CON_ANY_MR_FAIL
    HI_U32 ulAnyMrFailScal;                 // 默认值30块表                     对应 HI_SYS_RST_CON_ANY_MR_FAIL
    HI_U32 ulNoFrameTime;                   // 默认值24小时*60分*60秒           对应 HI_SYS_RST_CON_NOT_GET_FARME
    HI_U32 ulNoUartDtaTime;                 // 默认值12小时        对应 HI_SYS_RST_CON_NOT_GET_METER_UART_DATA
    HI_U32 ulMaxProxy;                      // 默认值150个代理以上              对应 HI_SYS_RST_CON_CCO_TOO_MANY_AGENT
    HI_U32 ulMaxProxyTime;                  // 默认值150个代理12小时*60分*60秒  对应 HI_SYS_RST_CON_CCO_TOO_MANY_AGENT
	HI_U32 ulStaNoRx;                       // 默认值30分*60秒                  对应  HI_SYS_RST_CON_STA_NOT_RX
    HI_U32 ulStaNoTx;                       // 默认值90分*60秒                  对应 HI_SYS_RST_CON_STA_NOT_TX
    HI_U32 ulCcoNoRx;                       // 默认值8小时*60分*60秒           对应  HI_SYS_RST_CON_CCO_NOT_RX
    HI_U32 ulCcoNoTx;                       // 默认值8小时*60分*60秒           对应 HI_SYS_RST_CON_CCO_NOT_TX

    HI_U16 usSysSecureStartHour;            // 定期重启时间下限 单位:小时
    HI_U16 usSysSecureEndHour;              // 定期重启时间上限 单位:小时
    HI_U16 usCltIINoUartDateMinute;         // 2天 单位:分钟 HI_SYS_RST_CON_NOT_GET_METER_UART_DATA_CLT_II
    HI_U16 usNoPlcDateMinute;               // 2天 单位:分钟 HI_SYS_RST_CON_NOT_GET_METER_PLC
    HI_U16 usSaveSdmPeriod;                 // 周期存储间隔, 单位10秒, 0表示关闭存储
    HI_U16 usSdmEnable;                     // 是否打开存储功能
    HI_U16 ausNdmCmdIdRange[2];

    HI_U8  ucChlRx[8];                      // [0] DIAG-UART; [1]:DIAG-TCP; [2]:DIAG-MAC; [3]:APP-UART; [4]:APP-MAC; [5]:UPG-MAC. 0: 默认不接收; 1:默认接收
    HI_U8  ucDiagCfg[4];                    // [0] 0: API上报中不检查DIAG的OS队列,1则检查.
                                            // [1] 1: 周期上报统计量, 0不上报;
                                            // [2] 1: 支持Userplane SN;
                                            // [3] 0: Layer Msg 内置开关判断配置; 1:不使用内置开关过滤;
                                            // [4] 0: 打开UserPlane的统计量; 1: 不打开
    HI_U8  ucSwtCfg[4];                     // [0] 0: diag port; 1: uart0 as stream port; 2: uart0 as vx shell
                                            // [1] : 0:PLC MODE; 1:User-defined Mode;
                                            // [2] : 0:DCHL FTO enable; 1: DCHL FTO disable
                                            // [3] : reserved;

    HI_U16 usDiagEvtBlockNum;               // 0xFFFE表示不上报, 0表示使用默认设置
    HI_U16 usDiagPrintBlockNum;
    HI_U16 usDiagMsgBlockNum;
    HI_U16 usDiagCmdBlockNum;
    HI_U16 usDiagUserPlaneBlockNum;
    HI_U16 ulSysFailTimeMinute;                   // 默认值24小时*60分   对应SAL_SYS_RESET_TIME_24_HOURS
    HI_U16 ulMacBufferExceTimeMinute;             // MAC BUFFER异常重启时间上限
    HI_U16 ulMacExceTimeMinute;                   // MAC协议栈异常重启时间上限

    HI_U8 ucDiagPrintBlockSize;
    HI_U8 ucDiagMsgBlockSize;
    HI_U8 ucDiagCmdBlockSize;
    HI_U8 ucDiagUserPlaneBlockSize;
    HI_U8 auTaskDropIdxRange0[HI_NV_SAL_TASK_DROP_IDX_RANGE_MAX];
    HI_U8 auTaskDropIdxRange1[HI_NV_SAL_TASK_DROP_IDX_RANGE_MAX];
}HI_NV_SAL_DCHL_CFG_ID_S;

#define HI_NV_SAL_DYA_MEM_STAT_SIZE         (20)
typedef struct
{
    HI_U32 ultatolDynRamUsedPeek;
    HI_U16 usModId[HI_NV_SAL_DYA_MEM_STAT_SIZE];
    HI_U32 ulDynRamUsedPeek[HI_NV_SAL_DYA_MEM_STAT_SIZE];
}HI_NV_SAL_DYN_MEM_STAT_ID_S;

#define HI_DMS_CHIP_PRODUCT_TYPE_MAX  2
/* BEGIN: Added by b00208046, 2014/3/10   问题单号:DTS2014031301020  */
typedef struct
{
    HI_S32 aslStaticRamSize[HI_DMS_CHIP_PRODUCT_TYPE_MAX]; // 扩大内存大小 默认2MB, CCO,STA,NDM
    HI_S32 aslDynRamSize[HI_DMS_CHIP_PRODUCT_TYPE_MAX]; // 扩大内存大小 默认2MB, CCO,STA,NDM
    HI_S32 tcpRecvBuf;
    HI_S32 tcpSendBuf;
    HI_U8  ucDmsUartDiagRxTimeOut;
    HI_BOOL isMeterCheckProduct;
    HI_U8  bSupportDynLowPower;//是否执行动态低功耗
    HI_U8  reserved;
}HI_NV_SAL_CONFIG_S;
#if (HI_DMS_CHIP_PRODUCT_TYPE_MAX!=2)
#error "HI_DMS_CHIP_PRODUCT_TYPE_MAX!=2"
#endif
/* END:   Added by b00208046, 2014/3/10 */

/**
 * @ingroup hct_dms
 * 调度项取值意义描述
 */
typedef struct
{
    HI_U32 ulBaudRate; /**< 波特率 如:1200,2400,4800,9600,115200*/
    HI_U8 ucDataBit;   /**< 数据位 5,6,7,8*/
    HI_U8 ucStop;      /**< 停止位 1停止位1位，2停止位2位*/
    HI_U8 ucParity;    /**< 奇偶校验 0无校验 1奇校验 2偶校验*/
    HI_U8 ucFlowCtrl;  /**< 流控:不支持*/
}HI_FTM_DEVICE_UART_CFG_S;

typedef struct
{
    HI_FTM_DEVICE_UART_CFG_S astUartCfg[HI_DMS_CHL_UART_PORT_NUM_MAX2];
    HI_U32 ulStaticMemSize;  // 静态内存大小
    HI_U32 ulHeapSize;       // 动态可用的内存大小
}HI_FTM_DEVICE_ITEM_S;

#define HI_NV_FTM_DEVICE_CFG_CCO     0
#define HI_NV_FTM_DEVICE_CFG_STA     1

typedef struct
{
    HI_FTM_DEVICE_ITEM_S astDevItem[HI_DMS_CHIP_PRODUCT_TYPE_MAX]; // 数组偏移取值为 HI_NV_FTM_DEVICE_CFG_XX
}HI_NV_FTM_DEVICE_CFG_S;

typedef struct
{
    HI_FTM_DEVICE_UART_CFG_S astUartCfg[HI_DMS_CHL_UART_PORT_NUM_MAX];
    HI_U32 ulStaticMemSize;  // 静态内存大小
    HI_U32 ulHeapSize;       // 动态可用的内存大小
}DMS_FTM_DEVICE_ITEM_S;

typedef struct
{
    DMS_FTM_DEVICE_ITEM_S astDevItem[HI_DMS_CHIP_PRODUCT_TYPE_MAX]; // 数组偏移取值为 HI_NV_FTM_DEVICE_CFG_XX
}HI_DMS_NV_FTM_DEVICE_CFG_S;


typedef struct
{
    HI_NV_FTM_DEVICE_CFG_S stFtmDevCfg;
    HI_PVOID pCacheAddr;
    HI_U32 ulCacheSize;
}SAL_DEVCIE_INIT_INFO_STRU;


//*****************************************************************************


//*****************************************************************************
HI_EXTERN HI_NV_SAL_DCHL_CFG_ID_S* SAL_NvDchlCfg(HI_VOID);

#define HI_SAL_NVCFG_DIAG_CFG_CHK(idx,m)   (SAL_NvDchlCfg()->ucDiagCfg[idx] == (HI_U8)(m))
#define HI_SAL_NVCFG_SWT_CFG_CHK(idx,m)    (SAL_NvDchlCfg()->ucSwtCfg[idx] == (HI_U8)(m))

#define HI_SAL_NVCFG_UART_DBG_SWT_CFG      0
#define HI_SAL_NVCFG_DEV_MODE_SWT_CFG      1
#define HI_SAL_NVCFG_DCHLTO_SWT_CFG        2

typedef enum
{
  HI_SAL_NVCFG_DIAG_SWT_CFG_0     = 0,
  HI_SAL_NVCFG_DIAG_SWT_CFG_1     = 1,
  HI_SAL_NVCFG_DIAG_SWT_CFG_2     = 2,
  HI_SAL_NVCFG_DIAG_SWT_CFG_LAYER = 3,
  HI_SAL_NVCFG_DIAG_SWT_CFG_USERPLANE_STAT = 4,
  // TODO:
}HI_SAL_NVCFG_DIAG_SWT_CFG_E;

typedef enum
{
  HI_SAL_NVCFG_UART_DBG_SWT_CFG_DIAG         = 0,
  HI_SAL_NVCFG_UART_DBG_SWT_CFG_STREAM_SHELL = 1,
  HI_SAL_NVCFG_UART_DBG_SWT_CFG_VX_SHELL     = 2
  // TODO:
}HI_SAL_NVCFG_UART_DBG_SWT_CFG_E;

typedef enum
{
  HI_SAL_NVCFG_DEV_MODE_SWT_CFG_PLC         = 0, // PLC Device Mode
  HI_SAL_NVCFG_DEV_MODE_SWT_CFG_USR         = 1, // user-defined Device Mode
  // TODO:
}HI_SAL_NVCFG_DEV_MODE_SWT_CFG_E;

#define HI_SAL_NVCFG_SWT_CFG_CHK_UART_DBG(m)   HI_SAL_NVCFG_SWT_CFG_CHK(HI_SAL_NVCFG_UART_DBG_SWT_CFG, m)
#define HI_SAL_IS_PLC_DEVICE_MODE()            HI_SAL_NVCFG_SWT_CFG_CHK(HI_SAL_NVCFG_DEV_MODE_SWT_CFG, HI_SAL_NVCFG_DEV_MODE_SWT_CFG_PLC)
#define HI_SAL_IS_USR_DEVICE_MODE()            HI_SAL_NVCFG_SWT_CFG_CHK(HI_SAL_NVCFG_DEV_MODE_SWT_CFG, HI_SAL_NVCFG_DEV_MODE_SWT_CFG_USR)
#define HI_SAL_IS_DCHL_FTO_DISABLE()           HI_SAL_NVCFG_SWT_CFG_CHK(HI_SAL_NVCFG_DCHLTO_SWT_CFG, 1)

#define HI_SYS_CHK_PLC_MODE() do {\
    if(!HI_SAL_IS_PLC_DEVICE_MODE())\
    {\
        return ;\
    }\
}while(0)
//*****************************************************************************


//*****************************************************************************
#endif //__HI_SAL_NV_H__


