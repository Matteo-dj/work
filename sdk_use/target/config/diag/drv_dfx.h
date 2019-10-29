//*****************************************************************************
//
//                  版权所有 (C), 1998-2012, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : drv_dfx.h
//  版 本 号   : V1.0
//  作    者   : chenghuanhuan/00132816
//  生成日期   : 2012-05-16
//  功能描述   : TODO: ...
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2012-05-16
//    作    者 : c00132816
//    修改内容 : 创建文件
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   :
// SUBSYSTEM :
// MODULE    :
// OWNER     :
//*****************************************************************************



#ifndef __DRV_DFX_H__
#define __DRV_DFX_H__
#include <hi_types.h>
#include <dfx_sys.h>


HI_START_HEADER

#define HI_DRV_UPLOAD_DATA_ACK_SIZE        0X2800
#define HI_DRV_UPLOAD_DATA_PACKET_SIZE     0X400

#define ID_DIAG_CMD_DRV_UPLOAD_DATA  0X3000 //HI_DIAG_UPLOAD_IND_S, HI_DIAG_UPLOAD_CNF_S  (DRV->HSO)
#define ID_DIAG_CMD_DRV_UPLOAD_DATA_START  0X300A //HI_DIAG_UPLOAD_START_REQ_S, HI_DIAG_UPLOAD_START_IND_S (HSO->DRV)
#define ID_DIAG_CMD_DRV_UPLOAD_DATA_STOP   0X300B //HI_DIAG_UPLOAD_STOP_REQ_S, HI_DIAG_UPLOAD_STOP_IND_S  (HSO<->DRV)

#define ID_DIAG_CMD_DRV_MEM_READ     0X3001
#define ID_DIAG_CMD_DRV_MEM_WRITE    0X3002
#define ID_DIAG_CMD_DRV_IP_READ      0X3003
#define ID_DIAG_CMD_DRV_IP_WRITE     0X3004
#define ID_DIAG_CMD_DRV_ROUTE        0X3005
#define ID_DIAG_CMD_DRV_ROUTE_ADD    0X3006
#define ID_DIAG_CMD_DRV_HELP         0X3007
#define ID_DIAG_CMD_DRV_ROUTE_DEL    0X3008
#define ID_DIAG_CMD_DRV_SYSTEM_RST   0X3009
#define ID_DIAG_CMD_DRV_OS_SHOW      0X300C
#define ID_DIAG_CMD_DRV_OS_MEM       0X300D
#define ID_DIAG_CMD_DRV_OS_TASK      0X300E
#define ID_DIAG_CMD_DRV_OS_TIMER     0X300F
#define ID_DIAG_CMD_DRV_OS_NETSTAT    0X3010
//增加两条预留命令，用于直接在HSO打印命令
#define ID_DIAG_CMD_DRV_OS_RSV1         0X3011
#define ID_DIAG_CMD_DRV_OS_RSV2         0X3012
#define DIAG_CMD_VER_QRY_1              (0x3013)
#define DIAG_CMD_SYS_QRY_1              (0x3014)


//精简结构查询syserr的命令，支持本地和远程
#define ID_DIAG_CMD_NEW_SYSERR_QRY    (0x3017)
#define ID_DIAG_CMD_NDM_RTC_TIME      (0x3018)

//配置以太网口
#define ID_DIAG_CMD_ETH_CFG           (0x3019)
#define ID_DIAG_CMD_KEY_STAT          (0x3020)

#define ID_DIAG_CMD_SDK_VER_QRY             (0x301A)
#define ID_DIAG_CMD_USER_VER_QRY            (0x301B)//历史版本可用命令

#define ID_DIAG_CMD_DRV_3518_SYSTEM_RST     (0x3021)
#define ID_DIAG_CMD_GET_3518_VER            (0x3022)
#define ID_DIAG_CMD_RF_CMD_STAT             (0x3023)
#define ID_DIAG_CMD_RF_CMD_ORDER            (0x3024)
#define ID_DIAG_CMD_RF_CMD_OPERATE          (0x3025)
#define ID_DIAG_CMD_RF_CMD_OP_ENABLE        (0x3026)
#define ID_DIAG_CMD_FLASH_ERASE_COUNT       (0x3027)
#define ID_DIAG_CMD_DRV_GET_HW_ID           (0X3028)
#define ID_DIAG_CMD_GET_NDM_WORK_MODE       (0X3029)
#define ID_DIAG_CMD_SET_NDM_WORK_MODE       (0X3030)
#define ID_DIAG_CMD_ARP                     (0X3040)
#define ID_DIAG_CMD_NEW_MEM_SHOW            (0X3041)
#define ID_DIAG_CMD_OS_PING                 (0X3042)
#define ID_DIAG_CMD_LOW_POWER_INFO          (0X3050)//查询动态低功耗信息
#define ID_DIAG_CMD_FLASH_PROTECT_OP        (0x3051)

#define ID_DIAG_CPUP_START              (0X3060)
#define ID_DIAG_CPUP_STOP               (0X3061)
#define ID_DIAG_CPUP_REPORT_TOTAL_INFO  (0X3062)
#define ID_DIAG_CPUP_REPORT_ITEM_INFO   (0X3063)

#define ID_DIAG_TRACK_START              (0X3068)
#define ID_DIAG_TRACK_STOP               (0X3069)
#define ID_DIAG_TRACK_REPORT_TOTAL_INFO  (0X306a)
#define ID_DIAG_TRACK_REPORT_ITEM_INFO   (0X306b)


#define ID_DIAG_CMD_CHANGE_PASS_WORD (0x3100)
#define ID_DIAG_CMD_GET_CHALLENGE (0x3101)
#define ID_DIAG_CMD_GET_UART_STAT (0x3102)
/*****注意*****增加命令后相应ID_DIAG_CMD_DRV_OS_MAX值要修改为最后一个命令*/
/* BEGIN: Added by x00180816, 2014/3/5   问题单号:DTS2014012702376 */
#define ID_DIAG_CMD_DRV_OS_MAX   ID_DIAG_CMD_GET_UART_STAT

/* END:   Added by x00180816, 2014/3/5 */
#define DRV_DFX_TASK_UPLOAD     (6) //每次上传task个数
#define DRV_DFX_TASK_MAX        (30)//总共task数
#define DRV_DFX_TIMER_MAX       (40)
#define DRV_DFX_MEM_MAX         (10)
#define DRV_DFX_TRACE_MAX       (10)

#define DRV_DFX_KEY_NUM_MAX     (12)//按键总数

typedef struct _drv_dfx_eth_cfg
{
    HI_U16 key_enable_flag;//按键中断使能标志,分别对应bit0-5
    HI_U16 key_intflag;//电平中断
    HI_U8  key_times[DRV_DFX_KEY_NUM_MAX];//对应按键按下次数
    HI_U8  key_down_err[DRV_DFX_KEY_NUM_MAX];//对应按键按下次数
    HI_U8  key_up_err[DRV_DFX_KEY_NUM_MAX];//对应按键按下次数
}DRV_DFX_KEY_CFG;

typedef enum _dfx_os_obj_class
    {
    dfxOsObjInvalid = 0,
    dfxOsObjCpu,
    dfxOsObjTask,
    dfxOsObjMem,
    dfxOsObjTimer,
    dfxOsObjMax
    } DFX_OS_OBJ_CLASS;


typedef enum {
    upload_data_012 = 12,// 命名方式: 函数名_num
    dfx_task_show_018 = 18,
    upload_data_019 = 19,
    upload_data_config_020,
 } drv_dfx_log_id_enum;

typedef struct _dfx_os_wd_info
    {
    HI_U16  wdId;           /* watchdog ID */
    HI_U8   mode;           /* watchdog mode */
    HI_U8   status;         /* status of watchdog */
    HI_U32  pHandle;        /* 指针，可能存在变化，暂时保留 */ 
    HI_U32 *wdRtn;          /* routine to call upon expiration */
    HI_U32  wdParam;        /* arbitrary parameter to routine */
    HI_U32  remainTicks;    /* number of ticks before expiration */
    } DFX_OS_WD_INFO;

typedef struct _dfx_os_mem_info
    {
    HI_U32 *partId;                 /* memory partition ID */
    HI_U32  numBytesFree;           /* number of Free Bytes in Partition */
    HI_U32  numBlocksFree;          /* number of Free Blocks in Partition */
    HI_U32  maxBlockSizeFree;       /* maximum block size that is free */
    HI_U32  numBytesAlloc;          /* number of Allocated Bytes in Partition */
    HI_U32  numBlocksAlloc;         /* number of Allocated Blocks in Partition */
    HI_U32  maxBytesAlloc;          /* maximum number of Allocated Bytes at any time */

    HI_U32  curBytesAllocInter;     /* current number of bytes allocated for internal use */
    HI_U32  curBlocksAllocInter;    /* current number of blocks allocated for internal use */
    HI_U32  cumBytesAlloc[2];       /* cumulative number of bytes allocated */
    HI_U32  cumBlocksAlloc[2];      /* cumulative number of blocks allocated */
    } DFX_OS_MEM_INFO;

typedef struct _dfx_ndm_rtc_time
{
    HI_U32  ulRtcSecNum;             //相对于1970秒数
    HI_U32  ulRet;
    HI_U32  reserve1;                   //保留位
    HI_U8   reserve2[8];               //保留位
}DRV_DFX_NDM_RTC;

typedef struct _dfx_eth_cfg
{
    HI_SYS_ETH_CONFIG_S stEthcfg; //ip/mac/netmask/gateway配置
    HI_U32 ulRet;  //返回值判断
    HI_U8 ucSetDir;   //读取:HI_FALSE 设置:HI_TRUE
    HI_U8 ucRsv[7];//保留
}DRV_DFX_ETH_CFG;

typedef struct _dfx_os_stack_info
    {
    HI_U32  base;   /* stack base */
    HI_U32  end;    /* stack end */
    HI_U32  size;   /* stack size */
    HI_U32  cur;    /* current stack usage in bytes */
    HI_U32  high;   /* maximum stack usage in bytes */
    HI_U32  margin; /* current stack margin in bytes */
    } DFX_OS_STACK_INFO;

typedef struct _dfx_os_task_info
    {
    char    name[12];       /* task name */
    HI_U32 *entry;          /* 无用，待替换 */ //16
    
    HI_U32 *id;             /* task ID */
    HI_S32  pri;            /* task priority */  //24
    
    char    status[12];     /* task status */
    HI_U32  usedmem;        /* 原有pc值 */ //40
    
    HI_U32  sp;             /* task stack pointer */
    HI_U32  pad1; //48
    DFX_OS_STACK_INFO stack;  /* task execution stack */ //72
    HI_U32  semID;          /* 原有异常栈第一个成员 */
    HI_U32 evtmask;         /* 原有异常栈第二个成员 */
    HI_U32 pc;
    HI_U32 lr;
    HI_U32 cpsr;
    HI_U32 pad2[1]; //96
    HI_U32 *stackTrc[DRV_DFX_TRACE_MAX];   /* task stack trace */ //136
    } DFX_OS_TASK_INFO;
//任务结构体，很多16位占用32位，暂放开用

/* BEGIN: Added by x00180816, 2014/2/28   问题单号:DTS2014012702376 */
typedef struct _dfx_os_task_stc_size
    {
    char    name[12];       /* task name */
    HI_U16 exe_high;  /* task execution stack */
    HI_U16 exc_high;  /* task exception stack */
    } DFX_OS_TASK_STC_SIZE;

typedef struct
{
    HI_U16 exe_high;
    HI_U16 exc_high;
}DFX_OS_TASK_STCSIZE_ITEM_S;

typedef struct
{
    DFX_OS_TASK_STCSIZE_ITEM_S item[25];
}DFX_OS_TASK_STCSIZE_REPORT_S;

typedef struct
{
    HI_U32 MQueue[10];
    HI_U32 AQueue[3];
}DFX_OS_QUEUESTAT_REPORT_S;
/* END:   Added by x00180816, 2014/2/28 */

typedef struct _dfx_os_spy_task
    {
    char    name[12];       /* task name */
    HI_U32 *entry;          /* task entry function */
    HI_U32 *id;             /* task ID of this record entry */
    HI_S32  pri;            /* task priority */
    HI_U32  totalTicks;     /* task ticks since spy start */
    HI_U32  totalPerCent;   /* task percentage since spy start */
    HI_U32  incTicks;       /* task mode ticks since last report */
    HI_U32  incPerCent;     /* task percentage since last report */
    } DFX_OS_SPY_TASK;



typedef struct _dfx_os_spy_info
    {
    HI_U32  totalTicks;             /* all ticks since start */
    HI_U32  totalPerCent;           /* all percentage since start */
    HI_U32  incTicks;               /* all ticks since last report */
    HI_U32  incPerCent;             /* all percentage since last reprot */

    HI_U32  kernelTotalTicks;       /* sum of knl ticks across CPUs since start */
    HI_U32  kernelTotalPerCent;     /* sum of knl percentage across CPUs since start */
    HI_U32  interruptTotalTicks;    /* sum of int ticks across CPUs since start */
    HI_U32  interruptTotalPerCent;  /* sum of int percentage across CPUs since start */
    HI_U32  idleTotalTicks;         /* sum of idle ticks across CPUs since start */
    HI_U32  idleTotalPerCent;       /* sum of idle percentage across CPUs since start */

    HI_U32  kernelIncTicks;         /* CPU kernel mode ticks since last report */
    HI_U32  kernelIncPerCent;       /* CPU kernel mode percentage since last report */
    HI_U32  interruptIncTicks;      /* CPU interrupt mode ticks since last report */
    HI_U32  interruptIncPerCent;    /* CPU interrupt mode percentage since last report */
    HI_U32  idleIncTicks;           /* CPU idle mode ticks since last report */
    HI_U32  idleIncPerCent;         /* CPU idle mode ticks percentage last report */

    HI_U32  nTasks;
    DFX_OS_SPY_TASK taskInfo[DRV_DFX_TASK_MAX];
    } DRV_DFX_SPY_INFO;

typedef struct
    {
    HI_U32  ulCnt;
    HI_U32  bIsLast;
    DFX_OS_WD_INFO astEntrys[DRV_DFX_TIMER_MAX];
    } DRV_DFX_TIMER_INFO;

typedef struct
    {
    HI_U32  ulCnt;
    HI_U32  bIsLast;
    DFX_OS_MEM_INFO astEntrys[DRV_DFX_MEM_MAX];
    } DRV_DFX_MEM_INFO;

typedef struct
    {
    HI_U32  ulCnt;
    HI_U32  bIsLast;
    DFX_OS_TASK_INFO astEntrys[DRV_DFX_TASK_UPLOAD];
    } DRV_DFX_TASK_INFO_UPLOAD;

typedef struct
    {
    HI_U32  ulCnt;
    DFX_OS_TASK_INFO astEntrys[DRV_DFX_TASK_MAX];
    } DRV_DFX_TASK_INFO;

typedef struct
{
    HI_U32  StartAddr;
    HI_U32 	EndAddr;
    HI_U32  AgcGain;   //0xFFFFFFFF, NO fixed AGC
    HI_U32  ulPointNum;
    HI_U32  ulTimeout;
    HI_U16  usDataSrc;
    HI_U16  usTrigerType;
} HI_DIAG_UPLOAD_START_REQ_S;

typedef struct
{
    HI_U32  ulReturn;
    HI_U32  ulTotalLength;
} HI_DIAG_UPLOAD_START_IND_S;

typedef enum
{
    HI_SI4438_CMD_TYPE_POWERUP = 0x0,       // 0  偏移0x0
    HI_SI4438_CMD_TYPE_CFG_INIT,            // 1  偏移0x10
    HI_SI4438_CMD_TYPE_PARTINFO,            // 2  偏移0x20
    HI_SI4438_CMD_TYPE_PARTINFO_PHASE2,     // 3  偏移0x30
    HI_SI4438_CMD_TYPE_START_TX,            // 4  偏移0x40
    HI_SI4438_CMD_TYPE_START_RX,            // 5  偏移0x50
    HI_SI4438_CMD_TYPE_INT_STATUS,	        // 6  偏移0x60
    HI_SI4438_CMD_TYPE_INT_STATUS_PHASE2,   // 7  偏移0x70
    HI_SI4438_CMD_TYPE_SET_PROPERTY,        // 8  偏移0x80
    HI_SI4438_CMD_TYPE_CHANGE_STATE,        // 9  偏移0x90
    HI_SI4438_CMD_TYPE_FIFO_INFO,           // 10 偏移0xa0
    HI_SI4438_CMD_TYPE_FIFO_INFO_PHASE2,    // 11 偏移0xb0
    HI_SI4438_CMD_TYPE_TX_FIFO,             // 12 偏移0xc0
    HI_SI4438_CMD_TYPE_RX_FIFO,             // 13 偏移0xd0
    HI_SI4438_CMD_TYPE_GET_PROPERTY,        // 14 偏移0xe0
    HI_SI4438_CMD_TYPE_GET_PROPERTY_PHASE2, // 15 偏移0xf0
    HI_SI4438_CMD_TYPE_PIN_CFG,             // 16 偏移0x100
    HI_SI4438_CMD_TYPE_FRR_A_READ,          // 17 偏移0x110
    HI_SI4438_CMD_TYPE_MAX = 18,            // HSO不识别枚举，暂时规避
}HI_SI4438_CMD_TYPE_EN;
//#define RF_NIRQ_LOWLEVEL HI_FALSE
//#define RF_NIRQ_HIGHLEVEL HI_TRUE

#ifdef PRODUCT_CFG_SUPPORT_RF_DEBUG

#define CMD_LOG_NUM     (32)    //需要保证是2的n次方
#define TIMES_AREA_1    (500)
#define TIMES_AREA_2    (1000)
#define TIMES_AREA_3    (2000)
#define TIMES_AREA_NUM  (4)     //4个区间用于cts尝试次数统计
#define RF_INPUT_PARAM_NUMBER (4)
#define RF_OUTPUT_PARAM_NUMBER (64)

typedef struct
{
    HI_U32 wait_max;    //等待次数峰值
    HI_U32 wait_total;  //等待总执行次数，用于计算平均
    HI_U32 times;       //执行次数
}HI_SI4438_CMD_TYPE_ST;

typedef struct
{
    HI_SI4438_CMD_TYPE_ST cts_stat[HI_SI4438_CMD_TYPE_MAX];
    HI_U32 init_cmd_err_times;
    HI_U32 cts_times_area[TIMES_AREA_NUM];
}HI_RF_CMD_STAT_ST;

typedef struct
{
    HI_U32 current_point;
    HI_U8 opr_order[CMD_LOG_NUM];
}HI_SI4438_CMD_ORDER_ST;
typedef struct
{
    HI_SI4438_CMD_ORDER_ST current_order;   //当前rf命令执行顺序
    HI_SI4438_CMD_ORDER_ST backup_order;    //rf芯片重启前命令执行顺序
    HI_SI4438_CMD_ORDER_ST first_backup;    //第一次出现问题执行时备份
}HI_RF_CMD_ORDER_ST;
#endif //PRODUCT_CFG_SUPPORT_RF_DEBUG

#ifdef PRODUCT_CFG_FLASH_ERASE_COUNT
#define FLASH_ERASE_STATE_NUM 20  //统计使用频率最高的块数
/* 这里仅支持内部CCO和STA最大2M Flash(共512个4K)的需求，不考虑二次开发和以后扩展，对Flash芯片不作自适应处理 */
#define FLASH_TOTAL_SECTOR_NUM 512

typedef struct
{
    HI_U32 sector_num;   //flash sector
    HI_U32 erase_times;  //对应sector擦除次数
}HI_FLASH_ERASE_INFO_ST;

typedef struct
{
    HI_FLASH_ERASE_INFO_ST max_info[FLASH_ERASE_STATE_NUM];
}HI_DFX_FLASH_ERASE_INFO_ST;

HI_EXTERN HI_U16 g_aulFlashEraseCount[FLASH_TOTAL_SECTOR_NUM];

#endif//PRODUCT_CFG_FLASH_ERASE_COUNT

typedef struct
{
    HI_U32  ndm_work_mode;//设为0代表正常模式，2代表NDM测试模式
    HI_U32  ret;
    HI_U32  reserved;
}HI_DFX_NDM_MODE_CFG;
/* BEGIN: Added by x00180816, 2014/3/3   问题单号:DTS2014012702376 */


HI_U32 dfx_read_task_stcsize_from_nv(HI_VOID);
HI_PVOID DRV_GetOSTimerList(HI_VOID);
HI_U32 DRV_GetOSTimerListSize(HI_VOID);
HI_U32 DRV_GetHrTimerListSize(HI_VOID);

//*****************************************************************************
// 函数名称: new_diag_syserr_qry    newdiagVerQry
// 功能描述: 精简结构查询syserr和ver命令的回调函数
//
// 参数说明:
//
//
// 返 回 值:
//    HI_ERR_SUCCESS:  表示该接口执行成功
//    其它为错误码
//
// 调用要求: 无
// 调用举例:
// 作    者: zhangyang/z00218821 [2014-05-15]
//*****************************************************************************
HI_U32 new_diag_syserr_qry(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
HI_U32 newdiagVerQry(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

/* END:   Added by x00180816, 2014/3/3 */
/*****************************************************************************
 函 数 名  : HI_DRV_get_net_dfx
 功能描述  : 获取网口驱动的可维可测信息
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_DSID_DRV_ETH_INFO_S *
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2013年2月5日
    作    者   : likunhe/00194117
    修改内容   : 新生成函数

*****************************************************************************/
HI_EXTERN HI_DSID_DRV_ETH_INFO_S * HI_DRV_get_net_dfx(HI_VOID);

HI_U32 sdkVerQry(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

#ifdef PRODUCT_CFG_FLASH_PROTECT

typedef enum
{   
    HI_DIAG_FLASH_PROTECT_CMD_QUERY_INFO,    //只查询信息    
    HI_DIAG_FLASH_PROTECT_CMD_UNLOCK     //去掉flash保护功能并上报保护信息   
}HI_DIAG_FLASH_PROTECT_CMD_E;

typedef struct
{
    HI_DIAG_FLASH_PROTECT_CMD_E ulcmdPara;
}HI_DIAG_FLASH_CMD_S;//flash保护维测下发参数

typedef struct
{
    HI_U32  ulRet;               //维测命令中仅HI_FLASH_RemoveProtect接口的返回值
    HI_BOOL bSupportProtect;     //是否启用flash保护标识
    HI_U8   ucStatusVal;         //状态寄存器1值
    HI_U8   reserved[2];
}HI_DIAG_FLASH_PROTECT_INFO_IND_S;//flash保护维测上发的信息

#endif

HI_END_HEADER
#endif //__DRV_DFX_H__


