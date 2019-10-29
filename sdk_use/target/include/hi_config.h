//*****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : sal_config.h
//  版 本 号   : V1.0
//  作    者   : 沈汉坤/00130424
//  生成日期   : 2011年09月06日
//  功能描述   : 全局配置定义文件
//  函数列表   :
//  修改历史   :
//  1.日    期 : 2011年09月06日
//    作    者 : 沈汉坤/00130424
//    修改内容 : 创建文件
//
//*****************************************************************************

#ifndef __HI_CONFIG_H__
#define __HI_CONFIG_H__
//*****************************************************************************
#include <hi_types.h>

//*****************************************************************************
// 全局模块/子模块ID设置
// 注意增减模块个数后，需要增减 HI_MOD_MAX_NUM
// SAL : [200, 300)
// BSP : [300, 400)   -- [12c, 190)
// MAC : [500, 600)   -- [1F4, 258)
// APP : [800, 900)   -- [320, 384)
// USER: [900, 1000)  -- [384, 3E8)
//*****************************************************************************
#ifndef PRODUCT_CFG_DIAG_MODULE_ID_MAX_NUM
#define PRODUCT_CFG_DIAG_MODULE_ID_MAX_NUM 60
#endif
#define HI_MOD_MAX_NUM           (PRODUCT_CFG_DIAG_MODULE_ID_MAX_NUM)  // 模块个数
#define HI_MOD_ID_STD            (0)
#define HI_MOD_ID_DFX            (10)
#define HI_MOD_ID_CMN_CFG        (11)

// SAL: [200,  300)
#define HI_MOD_ID_SAL            (200)
#define HI_MOD_ID_SAL_SYS        (201)
#define HI_MOD_ID_SAL_DMS        (202)
#define HI_MOD_ID_SAL_NVM        (203)
#define HI_MOD_ID_SAL_DIAG       (204)
#define HI_MOD_ID_SAL_DFX        (205)
#define HI_MOD_ID_SAL_ST         (206)
#define HI_MOD_ID_SAL_UP_API     (207) // User palne API Req
#define HI_MOD_ID_SAL_API        (208)
#define HI_MOD_ID_SAL_ICC        (209)

// BSP: [300, 400)
#define HI_MOD_ID_DRV            (300) //0x12C
#define HI_MOD_ID_DRV_FLASH      (301) //0x12D
#define HI_MOD_ID_DRV_SERIAL     (302) //0x12E
#define HI_MOD_ID_DRV_ETH        (303) //0x12F
#define HI_MOD_ID_DRV_HRTIMER    (304) //0x130
#define HI_MOD_ID_DRV_UPG        (305) //0x131 
#define HI_MOD_ID_DRV_DFX        (306) //0x132
#define HI_MOD_ID_DRV_SPI        (307) //0x133
#define HI_MOD_ID_DRV_POWER_SAVE (308) //0x134
#define HI_MOD_ID_DRV_CRC        (309) //0x135
#define HI_MOD_ID_DRV_GPIO       (310) //0x136
#define HI_MOD_ID_DRV_CIPHER     (311) //0x137
#define HI_MOD_ID_DRV_WDG        (312) //0x138
#define HI_MOD_ID_DRV_LED        (313) //0x139
#define HI_MOD_ID_DRV_IR         (314) //0x13a
#define HI_MOD_ID_DRV_I2C        (315) //0x13b
#define HI_MOD_ID_DRV_DMA        (316) //0x13c
#define HI_MOD_ID_DRV_AUTOTEST   (317) //0x13d
#define HI_MOD_ID_DRV_TF         (318) //0x13e
#define HI_MOD_ID_DRV_RTC        (319) //0x13f
#define HI_MOD_ID_DRV_SD_DC      (320) //0x140
#define HI_MOD_ID_DRV_RF         (321) //0x141
#define HI_MOD_ID_SYS_CPUP       (322) //0x142
#define HI_MOD_ID_DRV_ENV        (323) //0x143

// MAC: [500, 600)
#define HI_MOD_ID_MAC            (500) // 1F4
#define HI_MOD_ID_MAC_CVG        (501) // 1F5
#define HI_MOD_ID_MAC_SGM        (502) // 1F6
#define HI_MOD_ID_MAC_TX         (503) // 1F7
#define HI_MOD_ID_MAC_RTS_CTS    (504) // 1F8
#define HI_MOD_ID_MAC_SACK       (505) // 1F9
#define HI_MOD_ID_MAC_RX         (506) // 1FA
#define HI_MOD_ID_MAC_RSB        (507) // 1FB
#define HI_MOD_ID_MAC_ROUTE      (508) // 1FC
#define HI_MOD_ID_MAC_SD         (509) // 1FD
#define HI_MOD_ID_MAC_BCN        (510) // 1FE
#define HI_MOD_ID_MAC_DFX        (511) // 1FF
#define HI_MOD_ID_MAC_ASSOC      (512) // 200
#define HI_MOD_ID_MAC_COO        (513) // 201
#define HI_MOD_ID_MAC_KEY        (514) // 202
#define HI_MOD_ID_MAC_TDMA       (515) // 203
#define HI_MOD_ID_MAC_TOPO       (516) // 204
#define HI_MOD_ID_MAC_PHY_DRV    (517) // 205
#define HI_MOD_ID_MAC_NTB        (518) // 206
#define HI_MOD_ID_MAC_CPA        (519) // 207
#if defined(PRODUCT_CFG_FEATURE_NDC_CHL)
#define HI_MOD_ID_MAC_NDC         (520)// 208
#endif
#define HI_MOD_ID_MAC_FA         (521)
#define HI_MOD_ID_MAC_SWITCH     (522)
#define HI_MOD_ID_MAC_PTPC       (523)
#define HI_MOD_ID_MAC_EMC        (524)
#define HI_MOD_ID_MAC_RX_SC		 (525)
#define HI_MOD_ID_MAC_TX_SC      (526)
#define HI_MOD_ID_MAC_SC_ASSOC   (527)
#define HI_MOD_ID_MAC_SUB_TOPO   (528)
#define HI_MOD_ID_MAC_SUB_TDMA   (529)

// APP : [800, 900)
#define HI_MOD_ID_APP            (800)  // 320
#define HI_MOD_ID_APP_CHL_RX     (801)  // 321
#define HI_MOD_ID_APP_CHL_TX     (802)  // 322
#define HI_MOD_ID_APP_PRO_DEC    (803)  // 323
#define HI_MOD_ID_APP_SESSION    (804)  // 324
#define HI_MOD_ID_APP_MAC_LIST   (805)  // 325
#define HI_MOD_ID_APP_MAC_MAP    (806)  // 326
#define HI_MOD_ID_APP_ANY_MR     (807)  // 327
#define HI_MOD_ID_APP_LOCAL_MR   (808)  // 328
#define HI_MOD_ID_APP_SCH_METER  (809)  // 329
#define HI_MOD_ID_APP_UPG        (810)  // 32A
#define HI_MOD_ID_APP_MR_QUEUE   (811)  // 32B
#define HI_MOD_ID_APP_COMMON     (812)  // 32C
#define HI_MOD_ID_APP_STA_QUEUE  (813)  // 32D

// EQT : [850,860)
#define HI_MOD_ID_EQT            (850)
#define HI_MOD_ID_EQT_MAIN       (851)
#define HI_MOD_ID_EQT_SRV        (852)
#define HI_MOD_ID_EQT_SRV_SIMU   (853)
#define HI_MOD_ID_NDM_SRV        (854)

// USER : [900, 1000)
#define HI_MOD_ID_USR            (900) // 384
#define HI_MOD_ID_USR_1          (901)


#define HI_MODE_ID_MAX           (0x0000FFFF)
#define HI_MODE_ID_DIAG_MSG      (0x0000FFF0)
#define HI_MODE_ID_DIAG_FLASH    (0x0000FFFE)
#define HI_MODE_ID_INVALID       (0xFFFFFFFF)
#define HI_MODE_ID_MASK(x)       (x&0x0FFF)


// Module name size in bytes
#define HI_SYS_MOD_NAME_LEN  16
#ifdef __NEW_FILE_ID__
#define HI_DIAG_LOG_MSG_MOD_ID    __NEW_MOD_ID__
#define HI_DIAG_LOG_MSG_FILE_ID   __NEW_FILE_ID__ 
#else
#define HI_DIAG_LOG_MSG_MOD_ID 0
#define HI_DIAG_LOG_MSG_FILE_ID 0
#endif


//*****************************************************************************

//*****************************************************************************
// NV范围
// PHY_DRV:  [0x0001, 0x0100)
// SAL:      [0x0100, 0x0200)
// MAC:      [0x0200, 0x0400)
// APP:      [0x0400, 0x0600)
// USER:     [0x8000, 0x9000) *
// FT:       [0x0F00, 0x1F00)
//*****************************************************************************
#define HI_NV_PHY_ID_START      0x0001
#define HI_NV_PHY_ID_END        0x0100
#define HI_NV_SAL_ID_START      0x0100
#define HI_NV_SAL_ID_END        0x0200
#define HI_NV_MAC_ID_START      0x0200
#define HI_NV_MAC_ID_END        0x0400
#define HI_NV_APP_ID_START      0x0400
#define HI_NV_APP_ID_END        0x0600
#define HI_NV_USER_ID_START     0x8000
#define HI_NV_USER_ID_END       0x9000
//*****************************************************************************


//*****************************************************************************
// 统计量
// SAL	[0xA000, 0xA050)
// DRV	[0xA050, 0xA150)
// CMN	[0xA150, 0xA250)
// MAC	[0xA250, 0xA450)
// APP	[0xA450, 0xA850)
// 预留[0xA850, 0xB000)
// 预留[0xB200, 0xEE00)
//USR [0xEE00, 0xF000)

//*****************************************************************************
#define HI_GOBJ_STAT_ID_MIN   0xA000
#define HI_GOBJ_STAT_ID_MAMX  0xF000


//*****************************************************************************
// 日志消息(UserPlane)
// OS/DRV/SAL (0x0,    0x500)
// PHY        (0x500,  0x1000)
// MAC        (0x1000, 0x8000)
// 预留       (0x8000, 0xF000)
// APP        (0xF000, 0xF800)
// 预留       [0xF800, 0xFFFFFFFF)
//*****************************************************************************


//*****************************************************************************
// 命令和消息ID DIAG+MDM
// MAC        (0x1000, 0x2000)
// DRV        (0x2000, 0x2100)
// NDM        [0x3400, 0x3600)
// MDM        [0x3600, 0x3800)
// NDC        [0x3800, 0x3900)
// r1         [0x3900, 0x5000)
// DCHL       [0x5000, 0x5600)
// APP        [0x5600, 0x6600)
// r2         [0x6600, 0x7000)
// SAL        [0x7000, 0x8000)
// r3         [0x8000, 0xA000)
// CTS        [0xF000, 0xF500)
// r4         [0xF500, 0xFFFF)
//*****************************************************************************
#define HI_GOBJ_PHY_ID_MIN      0x0001
#define HI_GOBJ_PHY_ID_MAX      0x1000
#define HI_GOBJ_MAC_ID_MIN      0x1000
#define HI_GOBJ_MAC_ID_MAX      0x2000
#define HI_GOBJ_DRV_ID_MIN      0x2000
#define HI_GOBJ_DRV_ID_MAX      0x2100
#define HI_GOBJ_NDM_ID_MIN      0x3400
#define HI_GOBJ_NDM_ID_MAX      0x3600
#define HI_GOBJ_MDM_ID_MIN      0x3600
#define HI_GOBJ_MDM_ID_MAX      0x3800
#define HI_GOBJ_NDC_ID_MIN      0x3800
#define HI_GOBJ_NDC_ID_MAX      0x3900
#define HI_GOBJ_APP_ID_MIN      0x5600
#define HI_GOBJ_APP_ID_MAX      0x6600
#define HI_GOBJ_CTS_ID_MIN      0xF000
#define HI_GOBJ_CTS_ID_MAX      0xF500


//*****************************************************************************
// 全局内存大小设置
//*****************************************************************************

#define HI_TOTAL_HEAP_SIZE                    (8*1024*1024) // 缺省最大内存大小
#define HI_TOTAL_HEAP_SIZE_2M                 (2*1024*1024)

#define HI_PLATFORM_ASIC_V1R1      (1)    //8M V100程序
#define HI_PLATFORM_ASIC_V1R2      (2)    //2M V100程序
#define HI_PLATFORM_ASIC_V200_8M   (3)    //8M V200程序
#define HI_PLATFORM_ASIC_V200_2M   (4)    //2M V200程序

#define HI_DEFAULT_STATIC_MEM_SIZE            (PRODUCT_CFG_DEFAULT_STATIC_MEM_SIZE)    // 缺省的静态内存池大小
#define HI_DEFAULT_DYN_MEM_SIZE               (PRODUCT_CFG_DEFAULT_DYN_MEM_SIZE)       // 缺省的内存池分配大小

#define HI_DIAG_MODE_DEFAULT_DYN_MEM_SIZE     (16*1024)    // DIAG模式下缺省的内存分配大小
#define HI_DIAG_MODE_DEFAULT_STATIC_MEM_SIZE  (32*1024)    // DIAG模式下缺省的静态内存大小

#define HI_DEFALT_STATIC_MEM_SIZE_MAX         (0x16a000)
#define HI_DEFALT_DYN_MEM_SIZE_MAX            (0x411000)

// HI_DEFAULT_DYN_MEM_SIZE+HI_DEFAULT_STATIC_MEM_SIZE 必须小于　SAL_DEFAULT_STATIC_MEM_SIZE+HI_DEFAULT_DYN_MEM_SIZE
#define HI_DEFAULT_DYN_MEM_SIZE_MIN           (200*1024)   // 必须小于 HI_DEFAULT_DYN_MEM_SIZE
#define HI_DEFAULT_STATIC_MEM_SIZE_MIN        (200*1024)   // 必须小于 HI_DEFAULT_STATIC_MEM_SIZE

#define HI_HEAP_MIN_ALLOCATION_SIZE           (64) // min. allocation memory size

#define HI_POWER_ON_MEM_POOL_SIZE             (100*1024+50*1024) // 上电后 内存池, 初始化结束后释放,必须小于 HI_DEFAULT_DYN_MEM_SIZE_MIN
#define HI_POWER_ON_CACHE_SIZE                (100*1024) // 必须小于 HI_POWER_ON_MEM_POOL_SIZE

#define HI_MILLISECOND_PER_TICK               (10)  // 10ms/tick
#define HI_SYS_TICKS_PER_SECOND               (100) // ticks/sec
#define HI_SYS_MILLISECOND_PER_TICK           HI_MILLISECOND_PER_TICK

// DIAG包缓存单包最大大小设置
#ifdef PRODUCT_CFG_DIAG_BUFFERING_MSG_MAX_LEN
#define HI_DIAG_BUFFERING_MSG_MAX_LEN           PRODUCT_CFG_DIAG_BUFFERING_MSG_MAX_LEN
#else
#define HI_DIAG_BUFFERING_MSG_MAX_LEN           132
#endif
#ifdef PRODUCT_CFG_DIAG_BUFFERING_CMD_MAX_LEN
#define HI_DIAG_BUFFERING_CMD_MAX_LEN           PRODUCT_CFG_DIAG_BUFFERING_CMD_MAX_LEN
#else
#define HI_DIAG_BUFFERING_CMD_MAX_LEN           132
#endif
#ifdef PRODUCT_CFG_DIAG_BUFFERING_DATA_MAX_LEN
#define HI_DIAG_BUFFERING_DATA_MAX_LEN          PRODUCT_CFG_DIAG_BUFFERING_DATA_MAX_LEN
#else
#define HI_DIAG_BUFFERING_DATA_MAX_LEN          132
#endif
#define HI_DIAG_BUFFERING_PRINT_MAX_LEN         132 // 包括文件名/行号/字符串的总长度
#define HI_DIAG_BUFFERING_LOCAL_MAX_LEN         64
#define HI_DIAG_BUFFERING_EVT_MAX_LEN           8
#define HI_DIAG_BUFFERING_PROXY_MAX_LEN         132


// DAIG 通道 中 缓存队列 大小设置
#define HI_DIAG_BUFFERING_PRINT_NUM             (PRODUCT_CFG_DIAG_BUFFERING_PRINT_MAX_NUM) // 打印
#define HI_DIAG_BUFFERING_MSG_NUM               (PRODUCT_CFG_DIAG_BUFFERING_MSG_MAX_NUM)   // 消息
#define HI_DIAG_BUFFERING_CMD_NUM               (PRODUCT_CFG_DIAG_BUFFERING_CMD_MAX_NUM)   // 命令
#define HI_DIAG_BUFFERING_DATA_NUM              (PRODUCT_CFG_DIAG_BUFFERING_DATA_MAX_NUM)  // 用户面数据
#define HI_DIAG_BUFFERING_EVT_NUM               (PRODUCT_CFG_DIAG_BUFFERING_EVT_MAX_NUM)   // 事件
#if defined(PRODUCT_CFG_DIAG_BUFFERING_STAT_MAX_NUM)
#define HI_DIAG_BUFFERING_STAT_NUM              (PRODUCT_CFG_DIAG_BUFFERING_STAT_MAX_NUM)
#else
#define HI_DIAG_BUFFERING_STAT_NUM              (1)
#endif
#if defined(PRODUCT_CFG_DFX_KEY_MESSAGE_SAVE_FLASH)
#define HI_DIAG_BUFFERING_LOCAL_NUM             (PRODUCT_CFG_DIAG_BUFFERING_LOCAL_MAX_NUM) // 本地
#else
#define HI_DIAG_BUFFERING_LOCAL_NUM             1
#endif
#define HI_DIAG_BUFFERING_PROXY_NUM             (PRODUCT_CFG_DIAG_BUFFERING_PROXY_MAX_NUM) // 代理
#define HI_DIAG_BUFFERING_TOTAL_NUM             (HI_DIAG_BUFFERING_PRINT_NUM+\
                                                 HI_DIAG_BUFFERING_MSG_NUM+\
                                                 HI_DIAG_BUFFERING_CMD_NUM+\
                                                 HI_DIAG_BUFFERING_DATA_NUM+\
                                                 HI_DIAG_BUFFERING_EVT_NUM+\
                                                 HI_DIAG_BUFFERING_STAT_NUM+\
                                                 HI_DIAG_BUFFERING_PROXY_NUM)

#define HI_SYS_MEM_DYN_ALLOC_LIMIT_SIZE         (512) // 仅超过该值才使用动态分配
#define HI_DIAG_SPY_TIME_OUT_TICKS              (5*1000) // ticks
//*****************************************************************************


//*****************************************************************************
// 全局静态设置
//*****************************************************************************
#define HI_SYS_MSG_PARAM_NUM_MAX               4 // 必须 >= HI_SYS_MSG_PARAM_NUM_MIN(4)
//HI_SYS_MSG_PARAM_NUM_MAX-1,由于HISO不支持宏-1，用HI_SYS_MSG_PARAM_NUM_MIN替代HI_SYS_MSG_PARAM_NUM_MIN-1
/* BEGIN: Modified by b00208046, 2014/3/17   问题单号:DTS2014012702376 */
#define HI_SYS_MSG_PARAM_NUM_MAX_AULDATA       2
/* END:   Modified by b00208046, 2014/3/17 */
#define HI_SYS_MSG_PARAM_NUM_MIN               4
#define HI_SYS_MSG_PARAM_SIZE                  HI_SYS_MSG_PARAM_NUM_MAX
#define HI_PLC_MAC_ADDR_LEN                    6
#define HI_METER_ADDR_LEN                      6
#define HI_IPV4_ADDR_LEN                       4
#define HI_ETH_MAC_ADDR_LEN                    6
#define HI_PLC_RELAY_NUM_MAX                   3

#define HI_USER_VER_LENGTH              (32)

// 接收DIAG包最大大小，包括通道帧头和payload
#define HI_DIAG_PACKET_REQ_MAX_SIZE           (2*1024)  // 2KB, 最大16KB字节
#define HI_DIAG_PACKET_REQ_CACHE_MAX_SIZE     (2*HI_DIAG_PACKET_REQ_MAX_SIZE)  // 4KB, 最大16KB字节
#define HI_DIAG_PACKET_ACK_MAX_SIZE           (2*1024)  // 2KB, 最大16KB字节

#define HI_DIAG_FILENAME_MAX_LEN              (32)
#define HI_DIAG_ERR_REPORT_TIME_LIMIT         (2000)  // ms

#define HI_DMS_MAC_PORT_TX_TIME_OUT          (0)  // s
#define HI_DMS_TCP_PORT_TX_TIME_OUT          (0)  // s
#define HI_DMS_UART_CCO_PORT_TX_TIME_OUT     (0)  // s
#define HI_DMS_UART_STA_PORT_TX_TIME_OUT     (0)  // s
#define HI_DMS_UART_DIAG_PORT_TX_TIME_OUT    (4)  // s

// 任务等等异常
#define HI_SYS_TASK_WAIT_EXCEPTION_TIME_OUT  10000 // ms task wait 异常等待 Sleep
//*****************************************************************************

#if !defined(HI_HAVE_CROSS_COMPILER_DIAB_AS)

#if defined(PRODUCT_CFG_HSO)
#undef PRODUCT_CFG_MCORE_RAM_LOW_ADDR
#define PRODUCT_CFG_MCORE_RAM_LOW_ADDR       0x0
#endif

//*****************************************************************************
#define HI_SYS_FLASH_MAX_ADDR                (PRODUCT_CFG_MCORE_FLASH_MAX_ADDR)
#define HI_SYS_RAM_MAX_ADDR                  (PRODUCT_CFG_MCORE_RAM_HIGH_ADDR)
#define HI_SYS_RAM_ADDR_LIMIT                (PRODUCT_CFG_MCORE_RAM_LOW_ADDR)
#define HI_ADDR_RAM_BASE                     ((HI_U32)(HI_SYS_RAM_ADDR_LIMIT))

// 可读写的起始地址
#if defined(PRODUCT_CFG_PLATFORM_WINDOWS) || defined(PRODUCT_CFG_FEATURE_UT)
#define HI_ADDR_USER_WR                    (0x00001000)
#else
#define HI_ADDR_USER_WR                    (0x10000000)
#endif

#if defined(PRODUCT_CFG_PLATFORM_WINDOWS) || defined(PRODUCT_CFG_FEATURE_UT)
#define HI_ADDR_USER_RAM_START             (0x00001000)
#define HI_ADDR_USER_REG                   (0x00001000)
#undef HI_ADDR_RAM_BASE
#define HI_ADDR_RAM_BASE                   (0x00001000)
#define HI_ADDR_USER_RAM_MAX               0xFFFFFFFF
#define SET_RAM_ADDR(x)                    if(x == 0)  x = 0x02000000
#else
#if defined(PRODUCT_CFG_HSO)
#define HI_ADDR_USER_RAM_START             (0x00001000)
#define HI_ADDR_USER_REG                   (0x00001000)
#define HI_ADDR_USER_RAM_MAX               0xFFFFFFFF
#define SET_RAM_ADDR(x)                    if(x == 0) x = 0x02000000
#else
#if defined(PRODUCT_CFG_OS_NU)
#if defined(HI_HAVE_CROSS_COMPILER_ARM_ARMCC)
HI_EXTERN HI_U32 Image$$text$$Limit;
#endif
#define HI_ADDR_USER_RAM_START                   (HI_U32)((HI_U32*)(&Image$$text$$Limit))
#elif defined(HI_HAVE_CROSS_COMPILER_DIAB)
HI_EXTERN HI_U32 _wrs_kernel_text_end;
#define HI_ADDR_USER_RAM_START                   (HI_U32)((HI_U32*)(&_wrs_kernel_text_end))
#else
#define HI_ADDR_USER_RAM_START                   ((HI_U32)(HI_ADDR_RAM_BASE))
#endif
#define HI_ADDR_USER_REG                         (PRODUCT_CFG_MCORE_REG_BASE_ADDR)
#define HI_ADDR_USER_RAM_MAX                     (PRODUCT_CFG_MCORE_RAM_MEM_MAX_ADDR)
#define SET_RAM_ADDR(x)
#endif
#endif

#define HI_ADDR_USER_RAM                    HI_ADDR_USER_RAM_START
#define HI_ADDR_CODE_RAM                    HI_SYS_RAM_ADDR_LIMIT

#if !defined(PRODUCT_CFG_FEATURE_UT)
#if defined(HAVE_PCLINT_CHECK) // 底层接口适配, 不参与PCLINT检查
#define HI_SYS_ADDR_USER_RAM               (HI_SYS_RAM_ADDR_LIMIT)
#else
#define HI_SYS_ADDR_USER_RAM               (((HI_ADDR_USER_RAM) < HI_SYS_RAM_ADDR_LIMIT)?(HI_SYS_RAM_ADDR_LIMIT):(HI_ADDR_USER_RAM))
#endif
#else
#define HI_SYS_ADDR_USER_RAM               (HI_ADDR_USER_RAM_START)
#endif

#if !defined(PRODUCT_CFG_FEATURE_UT)
#if defined(PRODUCT_CFG_CHECK_CODE_ADDR)
#define HI_IS_VALID_RAM_ADDR(_x)           (((uintptr_t)(_x) > (HI_U32)HI_ADDR_USER_RAM_START) && ((uintptr_t)(_x) < (HI_U32)HI_ADDR_USER_RAM_MAX))
#else
#define HI_IS_VALID_RAM_ADDR(_x)           (((uintptr_t)(_x) > (HI_U32)HI_ADDR_RAM_BASE) && ((uintptr_t)(_x) < (HI_U32)HI_ADDR_USER_RAM_MAX))
#endif
#else
#define HI_IS_VALID_RAM_ADDR(_x)           (_x)
#endif

#if !defined(PRODUCT_CFG_FEATURE_UT)
#if defined(PRODUCT_CFG_CHECK_CODE_ADDR)
#define HI_IS_VALID_CODE_ADDR(_x)          (((uintptr_t)(_x)) > HI_ADDR_CODE_RAM)
#else
#define HI_IS_VALID_CODE_ADDR(_x)          (((uintptr_t)(_x)) > HI_SYS_RAM_ADDR_LIMIT)
#endif
#else
#define HI_IS_VALID_CODE_ADDR(_x)         (_x)
#endif

#if defined(PRODUCT_CFG_MCORE_FLH_LOW_ADDR)
#define HI_IS_VALID_FLH_ADDR(_x)           (((uintptr_t)(_x) >= (HI_U32)PRODUCT_CFG_MCORE_FLH_LOW_ADDR) && ((uintptr_t)(_x) < (HI_U32)HI_ADDR_USER_RAM_MAX))
#else
#define HI_IS_VALID_FLH_ADDR(_x)           (_x)
#endif

//*****************************************************************************

//*****************************************************************************
#if defined(PRODUCT_CFG_CHECK_TASK_STACK_ERR)
#define HI_SYS_TASK_STK_SEP_SPACE_SIZE   8 // 必须小于 HI_SYS_TASK_STK_MIN_SIZE,且为4的倍数
#else
#define HI_SYS_TASK_STK_SEP_SPACE_SIZE   0
#endif

#define HI_SYS_TASK_STK_SEP_SPACE_SIZE_HALF   HI_SYS_TASK_STK_SEP_SPACE_SIZE/2

#define HI_SYS_TASK_STK_MIN_SIZE        128

#define HI_SYS_TASK_STK_WD1  0xA1A2A3A4
#define HI_SYS_TASK_STK_WD2  0xB1B2B3B4

#define HI_SYS_TSK_MAX_NUM            20 // 必须等于 HI_SYS_MTSK_MAX_NUM + HI_SYS_ATSK_MAX_NUM 之和, 由于HSO解析问题,这里不使用四则运算表达式
#define HI_SYS_HTSK_MAX_NUM           4
#define HI_SYS_MQ_MAX_NUM             10
#define HI_SYS_AQ_MAX_NUM             1

#define HI_SYS_MTSK_MAX_NUM           18
#define HI_SYS_ATSK_MAX_NUM           2
#define HI_DIAG_DBG_SYNC_SIZE_MAX     3
//*****************************************************************************

//*****************************************************************************
// NV 配置
//*****************************************************************************
#define HI_NV_USE_DYN_ALLOC_BUF
#ifdef PRODUCT_CFG_NV_FLASH_LAYOUT_V2
#define HI_NV_ITEM_NUM_MAX                    PRODUCT_CFG_NV_FLASH_ITEM_NUM_MAX_V2  // 最大NV项个数
#else
#define HI_NV_ITEM_NUM_MAX                    PRODUCT_CFG_NV_FLASH_ITEM_NUM_MAX  // 最大NV项个数
#endif

#define HI_NV_ITEM_MAXSIZE                    PRODUCT_CFG_NV_FLASH_ITEM_SIZE_MAX // 每个NV项占空间最大值，单位字节

#ifndef PRODUCT_CFG_SUPPORT_UPG
#define HI_NV_FLASH_BLOCK_NUM                 PRODUCT_CFG_NV_FLASH_BLOCK_NUM   //所需总的块数
#define HI_NV_FLASH_BLOCK_SIZE                PRODUCT_CFG_NV_FLASH_BLOCK_SIZE  //块的大小
#else
#define HI_NV_FLASH_BLOCK_NUM                 (PRODUCT_CFG_NV_MAX_SIZE/PRODUCT_CFG_FLASH_BLOCK_SIZE)   //所需总的块数
#define HI_NV_FLASH_BLOCK_SIZE                PRODUCT_CFG_FLASH_BLOCK_SIZE                             //块的大小
#endif

#define HI_NV_FILE_USED_SECTOR_NUM                  (PRODUCT_CFG_NV_FILE_SIZE_MAX/HI_NV_FLASH_BLOCK_SIZE)

#define HI_NV_SECTION_FACTORY_ID                    3
#define HI_NV_MAX_SECTION_NUM                       4

//存放初始文件的块起始编号
#define HI_NV_SECTOR_FOR_INIT_FILE_ID               0//45

//烧写区的块数
#ifdef PRODUCT_CFG_NV_FLASH_LAYOUT_V2
#define HI_NV_FILE_SECTOR_NUM                       HI_NV_FILE_USED_SECTOR_NUM
#else
#define HI_NV_FILE_SECTOR_NUM                       1//1//2//10//5
#endif

// NVM可以使用的flash的起始地址(Table), 非烧写区
#ifdef PRODUCT_CFG_NV_FLASH_LAYOUT_V2
#define HI_NV_SECTOR_START_ID                       HI_NV_FILE_USED_SECTOR_NUM   //因为是ID号是从零开始
#else
#define HI_NV_SECTOR_START_ID                       1//4//2//10//5
#endif

// 存放数据的sector的起始编号
#ifdef PRODUCT_CFG_NV_FLASH_LAYOUT_V2
#define HI_NV_SECTOR_FOR_DATA_ID                    (4+HI_NV_SECTOR_START_ID)
#else
#define HI_NV_SECTOR_FOR_DATA_ID                    (3+HI_NV_SECTOR_START_ID)
#endif

// 预留空间的起始编号
#ifdef PRODUCT_CFG_NV_FLASH_LAYOUT_V2
#define HI_NV_SECTOR_FOR_RESERVE_BEGIN             HI_NV_FLASH_BLOCK_NUM //没留预留空间
#else
#define HI_NV_SECTOR_FOR_RESERVE_BEGIN              8//6//9//35//25//30
#endif

//存放上版本初始文件备份的块起始编号
#ifdef PRODUCT_CFG_NV_FLASH_LAYOUT_V2
#define HI_NV_SECTOR_FOR_BAK_FILE_BEGIN             HI_NV_FLASH_BLOCK_NUM
#else
#define HI_NV_SECTOR_FOR_BAK_FILE_BEGIN             8//6//9//40//30//35
#endif

//存放上版本工作数据的块起始编号
#ifdef PRODUCT_CFG_NV_FLASH_LAYOUT_V2
#define HI_NV_SECTOR_FOR_BAK_DATA_BEGIN             HI_NV_FLASH_BLOCK_NUM
#else
#define HI_NV_SECTOR_FOR_BAK_DATA_BEGIN             8//6//9//50//35//40
#endif

//存放生产NV备份文件的起始块编号
#ifdef PRODUCT_CFG_NV_FLASH_LAYOUT_V2
#define HI_NV_SECTOR_FOR_FACTORY_NV_BEGIN           HI_NV_FLASH_BLOCK_NUM
#else
#define HI_NV_SECTOR_FOR_FACTORY_NV_BEGIN           8//6//9//55//35//40
#endif

//最后一块的编号
#ifdef PRODUCT_CFG_NV_FLASH_LAYOUT_V2
#define HI_NV_SECTOR_END_ID                         HI_NV_FLASH_BLOCK_NUM
#else
#define HI_NV_SECTOR_END_ID                         8// 6//9//59//39//49
#endif

//*****************************************************************************


//*****************************************************************************
// 通道设置
//*****************************************************************************
#if defined(PRODUCT_CFG_FEATURE_FRAME_OPT_V2)
#define HI_DMS_CHL_UART_PORT_MIN                    (0) // min.
#define HI_DMS_CHL_UART_PORT_APP                    (0) // APP通道支持的UART端口号, 必须小于 HI_DMS_CHL_UART_PORT_NUM_MAX
#define HI_DMS_CHL_UART_PORT_DIAG                   (1) // DIAG通道支持的UART端口号, 必须小于 HI_DMS_CHL_UART_PORT_NUM_MAX
#if defined(PRODUCT_CFG_SUPPORT_IR)
#if !defined(PRODUCT_CFG_SUPPORT_UART2)
#define HI_DMS_CHL_UART_PORT_IR                     (2) // IR
#define HI_DMS_CHL_UART_PORT_MAX                    (2) // max.
#else
#define HI_DMS_CHL_UART_PORT_UART2                  (2) // UART2
#define HI_DMS_CHL_UART_PORT_IR                     (3) // IR
#define HI_DMS_CHL_UART_PORT_MAX                    (3) // max.
#endif//PRODUCT_CFG_SUPPORT_UART2
#else
#if !defined(PRODUCT_CFG_SUPPORT_UART2)
#define HI_DMS_CHL_UART_PORT_MAX                    (1) // max.
#else
#define HI_DMS_CHL_UART_PORT_UART2                  (2) // UART2
#define HI_DMS_CHL_UART_PORT_MAX                    (2) // max.
#endif
#endif

#if !defined(PRODUCT_CFG_SUPPORT_UART2)
#define HI_DMS_CHL_UART_PORT_NUM_MAX                (3)
#define HI_DMS_CHL_UART_PORT_NUM_MAX2               (3)
#else
#define HI_DMS_CHL_UART_PORT_NUM_MAX                (4)
#define HI_DMS_CHL_UART_PORT_NUM_MAX2               (3)
#endif

#define HI_DMS_CHL_MAC_PORT_MIN                     (0x10)
#define HI_DMS_CHL_MAC_PORT_APP                     (0x10)
#define HI_DMS_CHL_MAC_PORT_DIAG                    (0x11)
#if defined(PRODUCT_CFG_SUPPORT_UPG_SEPARATE)
#define HI_DMS_CHL_MAC_PORT_UPG                     (0x12)
#define HI_DMS_CHL_MAC_PORT_MAX                     (0x12)
#else
#define HI_DMS_CHL_MAC_PORT_MAX                     (0x11)
#endif

#define HI_DMS_CHL_DIAG_PORT_MAX                    (30003)  // max.
#define HI_DMS_CHL_FLASH_PORT_DAIG                  (0xE000) // 本地存储端口
#else
#define HI_DMS_CHL_DIAG_PORT_MAX                    (40200)  // max.
#define HI_DMS_CHL_UART_PORT_MIN                    (0)
#define HI_DMS_CHL_UART_PORT_MAX                    (3)
#define HI_DMS_CHL_MAC_PORT_MIN                     (0xF000)
#define HI_DMS_CHL_MAC_PORT_MAX                     (0xF100)

#define HI_DMS_CHL_UART_PORT_APP                    (0) // APP通道支持的UART端口号, 必须小于 HI_DMS_CHL_UART_PORT_NUM_MAX
#define HI_DMS_CHL_UART_PORT_DIAG                   (1) // DIAG通道支持的UART端口号, 必须小于 HI_DMS_CHL_UART_PORT_NUM_MAX
#if defined(PRODUCT_CFG_SUPPORT_IR)
#define HI_DMS_CHL_UART_PORT_IR                     (2) // IR
#define HI_DMS_CHL_UART_PORT_NUM_MAX                (3) // UART最大个
#else
#define HI_DMS_CHL_UART_PORT_NUM_MAX                (2) // UART最大个
#endif

#define HI_DMS_CHL_FLASH_PORT_DAIG                  (0xE000) // 本地存储端口
#define HI_DMS_CHL_MAC_PORT_APP                     (0xF000)
#define HI_DMS_CHL_MAC_PORT_DIAG                    (0xF001)
#define HI_DMS_CHL_MAC_PORT_UPG                     (0xF002)
#endif // PRODUCT_CFG_FEATURE_FRAME_OPT_V2

#define HI_DMS_CHL_TCP_PORT_MIN                     (PRODUCT_CFG_TCP_PORT_MIN)
#define HI_DMS_CHL_TCP_PORT_DIAG                    (PRODUCT_CFG_TCP_PORT_DIAG)
#define HI_DMS_CHL_TCP_PORT_MAX                     (PRODUCT_CFG_TCP_PORT_MAX)

#define HI_DMS_CHL_PORT_ANY                         (0xFFF0)
#define HI_DMS_CHL_PORT_NA                          (0xFFF1)
#define HI_DMS_CHL_PORT_INVALID                     (0xFFFF)

#define HI_DMS_CHL_UART_BLOCK_WRITE_TIMEOUT         10   // ms block写超时
#define HI_DMS_CHL_EXCEPTION_POLLING_WAIT           5000 // ms task read dev 异常等待Sleep

#define HI_DMS_CHL_FLASH_BLOCK_WRITE_TIMEOUT        10   // ms block写超时
#define HI_DMS_CHL_FLASH_BLOCK_READ_TIMEOUT         5

#define HI_DMS_UART_EXCEPTION_POLLING_WAIT          1000 // ms UART异常等待Sleep

#define HI_DMS_CHL_CONNECT_DETECT_NUM               3 // 通道连接后心跳方式检测客户端的失败的次数

#define HI_DMS_CHL_REPORT_PACKET_TIMEOUT            10   // 批量上报延时

#define HI_DIAG_MODULE_NUM                      (HI_MOD_MAX_NUM) // 配置的模块个数
#define HI_DIAG_USER_PLANE_ID_NUM               (HI_MOD_MAX_NUM)  // 配置的用户面包个数
#ifndef PRODUCT_CFG_DIAG_MSG_CFG_MAX_NUM
#define PRODUCT_CFG_DIAG_MSG_CFG_MAX_NUM 10
#endif
#define HI_DIAG_MSG_ID_NUM                      (PRODUCT_CFG_DIAG_MSG_CFG_MAX_NUM) // 配置的消息个数

#define HI_DIAG_WAIT_MESSAGE_TIMEOUT            (50)  // ms
#define HI_DIAG_WAI_DIAG_MESSAGE_TIMEOUT        (100)  // 上报 DIAG包的最小周期 ms
#define HI_DIAG_RESEND_COUNT                    (1)
#define HI_DMS_FLASH_EMPTY_DATA                 (0xFFFFFFFF)
#define HI_DMS_LF_PADDING                       (0xA1A2A3A4)
#define HI_SYS_TASK_WAIT_EXCEPTION_TIME_OUT      10000 // ms task wait 异常等待 Sleep
#define HI_SYS_TASK_DROP_POLLING_WAIT            1000*60*60 // ms

#define HI_DMS_TCP_SND_TIME_MAX_CHK              (5000)

// NDM
#define HI_ND_NDM_START_DEFAULT                     0x3400
#define HI_ND_NDM_END_DEFAULT                       0x3600

//*****************************************************************************


// 缺省波特率设置
#define HI_DMS_UART_BAUDRATE_MIN                   (1200)

#define HI_DMS_CCO_APP_UART_DEFAULT_BAUDRATE       (9600)
#define HI_DMS_CCO_APP_UART_DEFAULT_DATABIT        (8)
#define HI_DMS_CCO_APP_UART_DEFAULT_STOP           (1)
#define HI_DMS_CCO_APP_UART_DEFAULT_PARITY         (2)
#define HI_DMS_CCO_APP_UART_DEFAULT_FLOWCTRL       (0)

#define HI_DMS_STA_APP_UART_DEFAULT_BAUDRATE       (2400)
#define HI_DMS_STA_APP_UART_DEFAULT_DATABIT        (8)
#define HI_DMS_STA_APP_UART_DEFAULT_STOP           (1)
#define HI_DMS_STA_APP_UART_DEFAULT_PARITY         (2)
#define HI_DMS_STA_APP_UART_DEFAULT_FLOWCTRL       (0)

#define HI_DMS_DIAG_UART_DEFAULT_BAUDRATE          (115200)
#define HI_DMS_DIAG_UART_DEFAULT_BAUDRATE2         (115200*2)
#define HI_DMS_DIAG_UART_DEFAULT_DATABIT           (8)
#define HI_DMS_DIAG_UART_DEFAULT_STOP              (1)
#define HI_DMS_DIAG_UART_DEFAULT_PARITY            (0)
#define HI_DMS_DIAG_UART_DEFAULT_FLOWCTRL          (0)

#if defined(PRODUCT_CFG_SUPPORT_UART2)
#define HI_DMS_EQUIP_UART_DEFAULT_BAUDRATE          (115200)
#define HI_DMS_EQUIP_UART_DEFAULT_BAUDRATE2         (115200*2)
#define HI_DMS_EQUIP_UART_DEFAULT_DATABIT           (8)
#define HI_DMS_EQUIP_UART_DEFAULT_STOP              (1)
#define HI_DMS_EQUIP_UART_DEFAULT_PARITY            (2)
#define HI_DMS_EQUIP_UART_DEFAULT_FLOWCTRL          (0)
#endif
//*****************************************************************************


//*****************************************************************************
// DFX ID Definition to HSO
//*****************************************************************************
#define HI_DFX_ID_BASE_PHY              0x0050  // PHY的HSO包ID范围: [0x0050, 0x1000) [0x2100, 0x2900)
#define HI_DFX_ID_BASE_MAC              0x1000  // MAC的HSO包ID范围: [0x1000, 0x2000) [0x2900, 0x3000) [0x3000, 0x4000)
#define HI_DFX_ID_BASE_APP              0x5000  // APP的HSO包ID范围: [0x5600, 0x5E00) [0x5E00, 0x6600)
#define HI_DFX_ID_BASE_SAL              0x7000  // SAL的HSO包ID范围: [0x7000, 0x8000)
#define HI_DFX_ID_BASE_SYS              0x8000  // SYS的HSO包ID范围: [0x8000, 0xA000)

// 模块层间、空口消息件范围
#define HI_DFX_MSG_ID_BASE_PHY          0x0050  // PHY: [0x50，0x1000)
#define HI_DFX_MSG_ID_BASE_MAC          0x1000  // MAC:(0x1000,0x5000)
#define HI_DFX_MSG_ID_BASE_APP          0x5000  // APP:(0x5000,0x7000)
#define HI_DFX_MSG_ID_BASE_SAL          0x7000  // SAL的HSO包ID范围: [0x7000, 0x8000)
#define HI_DFX_MSG_ID_BASE_SYS          0x8000  // SYS的HSO包ID范围: [0x8000, 0xA000)

// 模块事件范围
#define HI_DFX_EVT_ID_BASE_SAL          0x0800  // SAL[0x0800, 0x1000)
#define HI_DFX_EVT_ID_BASE_MAC          0x1000  // MAC:(0x1000,0x2000)
#define HI_DFX_EVT_ID_BASE_APP          0x2000  // APP:(0x2000,0x3000)

// 系统诊断ID定义范围
#define HI_DFX_STAT_ID_BASE_SYS         0xA000  // SYS STAT的包ID范围: [0xA000, 0xF000)
#define HI_DFX_STAT_ID_MAX_SYS          0xF000
#define HI_DFX_EVT_ID_BASE_SYS          0x0000  // SYS EVT的HSO包ID范围:  [0x0000, 0x400)
#define HI_DFX_EVT_ID_MAX_SYS           0x0400
//*****************************************************************************


//*****************************************************************************
// OS 设置
//*****************************************************************************
#define HI_SYS_TASK_TIME_SLICE                  (1) // tick number
//*****************************************************************************

//*****************************************************************************
#if defined(PRODUCT_CFG_FEATURE_POSIX_SOCKET) || defined(PRODUCT_CFG_FEATURE_NU_SOCKET)
#define PRODUCT_CFG_FEATURE_SOCKET
#endif
//*****************************************************************************

#define HI_APP_TASK_PRIO_RANGE_START   PRODUCT_CFG_TASK_PRIO_APP_RANGE_START
#define HI_APP_TASK_PRIO_RANGE_END     PRODUCT_CFG_TASK_PRIO_APP_RANGE_END

#define PRODUCT_CFG_FEATURE_DIAG_RX
//*****************************************************************************

//*****************************************************************************
// DIAG 系统级消息、统计、事件 设置
//*****************************************************************************
#define HI_SYS_COMMAND_RPT_MSG        0xfffffff0
#define HI_SYS_DIAG_BUSINIESS         0xffffff10
#define HI_SYS_DIAG_NET               0xffffff20
#define HI_SYS_DIAG_MSG_LEVEL0        0xffffff00
#define HI_SYS_DIAG_MSG_LEVEL1        0xffffff01
#define HI_SYS_DIAG_MSG_LEVEL2        0xffffff02
#define HI_SYS_DIAG_STAT_LEVEL0       0xffffff00
#define HI_SYS_DIAG_STAT_LEVEL1       0xffffff04
#define HI_SYS_DIAG_STAT_LEVEL2       0xffffff08
#define HI_SYS_DIAG_MASK              0x000000FF

// 等级定义
#define HI_SYS_DMID_BUSINESS_L0           ((HI_SYS_DIAG_BUSINIESS|HI_SYS_DIAG_MSG_LEVEL0)&HI_SYS_DIAG_MASK)
#define HI_SYS_DMID_BUSINESS_L1           ((HI_SYS_DIAG_BUSINIESS|HI_SYS_DIAG_MSG_LEVEL1)&HI_SYS_DIAG_MASK)
#define HI_SYS_DMID_BUSINESS_L2           ((HI_SYS_DIAG_BUSINIESS|HI_SYS_DIAG_MSG_LEVEL2)&HI_SYS_DIAG_MASK)
#define HI_SYS_DMID_NET_L0                ((HI_SYS_DIAG_NET|HI_SYS_DIAG_MSG_LEVEL0)&HI_SYS_DIAG_MASK)
#define HI_SYS_DMID_NET_L1                ((HI_SYS_DIAG_NET|HI_SYS_DIAG_MSG_LEVEL1)&HI_SYS_DIAG_MASK)
#define HI_SYS_DMID_NET_L2                ((HI_SYS_DIAG_NET|HI_SYS_DIAG_MSG_LEVEL2)&HI_SYS_DIAG_MASK)
#define HI_SYS_DSID_BUSINESS_L0           ((HI_SYS_DIAG_BUSINIESS|HI_SYS_DIAG_STAT_LEVEL0)&HI_SYS_DIAG_MASK)
#define HI_SYS_DSID_BUSINESS_L1           ((HI_SYS_DIAG_BUSINIESS|HI_SYS_DIAG_STAT_LEVEL1)&HI_SYS_DIAG_MASK)
#define HI_SYS_DSID_BUSINESS_L2           ((HI_SYS_DIAG_BUSINIESS|HI_SYS_DIAG_STAT_LEVEL2)&HI_SYS_DIAG_MASK)
#define HI_SYS_DSID_NET_L0                ((HI_SYS_DIAG_BUSINIESS|HI_SYS_DIAG_STAT_LEVEL0)&HI_SYS_DIAG_MASK)
#define HI_SYS_DSID_NET_L1                ((HI_SYS_DIAG_BUSINIESS|HI_SYS_DIAG_STAT_LEVEL1)&HI_SYS_DIAG_MASK)
#define HI_SYS_DSID_NET_L2                ((HI_SYS_DIAG_BUSINIESS|HI_SYS_DIAG_STAT_LEVEL2)&HI_SYS_DIAG_MASK)
//*****************************************************************************


//*****************************************************************************
// 其它
//*****************************************************************************
#define HI_STR_NOT_FOUND_CMD               "D-FRM(%X) NOT SUPPORT"
//*****************************************************************************


//*****************************************************************************
// 版本兼容相关宏定义
//*****************************************************************************
#define HI_SYS_RSD_INVALID_VALUE              (0)

//	bit31-bit29表示V版本 ：最大可以表示7个版本, 1:表示100; 2表示200 , [0,6]
//	bit28-bit26表示R版本 ：最大可以表示7个版本, [0,6]
//	bit25-bit20表示C版本 ：最大可以表示64个版本, [0,63]
//	bit19-bit18预留
//	bit17-bit8表示B版本  : 最大可以表示1024个版本, [0,1023]
//	bit7-bit0表示SPC版本 : 最大可以表示256个版本, [0,255]
#define HI_SYS_INF_VER(_v,_r,_c,_b,_spc)        ((_v&0x07)<<29) | ((_r&0x07) << 26) | ((_c&0x3F) << 20) | ((_b&0x03FF) << 8)  | (_spc&0x0FF)

#define HI_SYS_INF_VER_V(_ver)                  (((_ver)>>29)&0x07)
#define HI_SYS_INF_VER_R(_ver)                  (((_ver)>>26)&0x07)
#define HI_SYS_INF_VER_C(_ver)                  (((_ver)>>20)&0x3F)
#define HI_SYS_INF_VER_B(_ver)                  (((_ver)>>8)&0x03FF)
#define HI_SYS_INF_VER_SPC(_ver)                (((_ver))&0x0FF)

#define HI_SYS_INF_V100R001C00_BSPC(_b,_spc)    HI_SYS_INF_VER(1,1,1,_b, _spc)

// CMP.VER: VRCB define.
#define HI_SYS_INF_CMP_VER_V  100
#define HI_SYS_INF_CMP_VER_R  1
#define HI_SYS_INF_CMP_VER_C  1
#define HI_SYS_INF_CMP_VER_B  19
//*****************************************************************************


//*****************************************************************************
// Task Trace Interface Macros
#define HI_SYS_TT_FUNC_MAX_NUM        16
//*****************************************************************************

#if defined(PRODUCT_CFG_SUPPORT_CONFIG_VARIABLE)
#if defined (PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined (PRODUCT_CFG_PRODUCT_TYPE_STA) || defined (PRODUCT_CFG_PRODUCT_TYPE_NDM)
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_UPG1_REAL_FLASH_ADDR;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_UPG2_REAL_FLASH_ADDR;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_NV_FLASH_ADDR;
#elif defined (PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
HI_EXTERN HI_U32 PRODUCT_CFG_UPG1_REAL_FLASH_ADDR;
HI_EXTERN HI_U32 PRODUCT_CFG_UPG2_REAL_FLASH_ADDR;
HI_EXTERN HI_U32 PRODUCT_CFG_NV_FLASH_ADDR;
HI_EXTERN HI_U32 PRODUCT_CFG_UPG1_FILE_FLASH_ADDR;
HI_EXTERN HI_U32 PRODUCT_CFG_UPG2_FILE_FLASH_ADDR;
#endif
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_UPG_BITMAP_LEN;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_UPG_CRC_BLK_SIZE;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_UPG_NVFILE_FLASH_SIZE;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_BOOT_FLASH_SIZE;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_BOOT_VERSION_ADDR;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_NV_FILE_SIZE_MAX;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_UPG_FILE_MAX_SIZE;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_CRASH_FLASH_ADDR;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_UPGFILE_CACHE_CCO_FLASH_ADDR;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_UPGFILE_CACHE_CCO_FLASH_SIZE;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_UPGFILE_CACHE_CCO_RAM_SIZE;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_UPG_LIST_CCO_FLASH_ADDR;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_UPG_LIST_CCO_FLASH_SIZE; 
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_USERDATA_CCO_BIDB_FLASH_ADDR;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_USERDATA_CCO_APP_FLASH_ADDR;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_USERDATA_CCO_USER_FLASH_ADDR;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_DIAG_CCO_FLASH_ADDR;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_DIAG_STA_FLASH_ADDR;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_ETH_PHY_ADDR_CFG;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_DEFAULT_STATIC_MEM_SIZE;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_DEFAULT_DYN_MEM_SIZE;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_NDM_UPG_FILE_MAX_SIZE;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_CCO_UPG_FILE_MAX_SIZE;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_STA_UPG_FILE_MAX_SIZE;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_WD_INT_PROC_ENABLE;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_POWER_DOWN_ETH_PHY_ENABLE;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_SDM_ENG_EVT_FLASH_SIZE;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_BOOT_DYNAMIC_ADDR_END;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_CCO_PERIOD_SIZE;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_STA_PERIOD_SIZE;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_SDM_BAT_FLASH_ST_ADDR;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_SDM_BAT_FLASH_END_ADDR;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_NV_FLASH_ITEM_SIZE_MAX;
HI_EXTERN HI_CONST HI_U32 PRODUCT_CFG_MCORE_RAM_HIGH_ADDR;
HI_EXTERN HI_CONST HI_U16 PRODUCT_CFG_TCP_PORT_MIN;
HI_EXTERN HI_CONST HI_U16 PRODUCT_CFG_TCP_PORT_DIAG;
HI_EXTERN HI_CONST HI_U16 PRODUCT_CFG_TCP_PORT_MAX;
HI_EXTERN HI_CONST HI_U8  PRODUCT_CFG_CHIP_MODE;
HI_EXTERN HI_CONST HI_U16 PRODUCT_CFG_DIAG_COMPANY_ID;
HI_EXTERN HI_CONST HI_U16 PRODUCT_CFG_CMD_NV_READ_PLC;
HI_EXTERN HI_CONST HI_U16 PRODUCT_CFG_CMD_NV_WRITE_PLC;
HI_EXTERN HI_CONST HI_U16 PRODUCT_CFG_MMTYPE_DFX_MGT_PV100;
HI_EXTERN HI_CONST HI_U16 PRODUCT_CFG_MMTYPE_NDM_CONNECT_PK;

HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_UNKOWN;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_C01;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_C02;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_C03;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_C04;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_C05;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_C06;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_C07;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_C08;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_C11;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_C12;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_C13;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_C14;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_C15;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_C16;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_C17;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_C18;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_R21;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_R22;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_R23;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_R24;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_R25;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_R26;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_R27;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_R28;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_T31;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_T32;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_T33;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_T34;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_R35;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_R36;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_R37;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_R38;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_D41;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_D42;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_D43;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_D44;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_D45;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_D46;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_D47;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_D48;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_D51;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_D52;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_D53;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_D54;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_D55;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_D56;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_D57;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_D58;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_D61;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_D62;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_D63;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_D64;
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M101;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M102;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M103;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M104;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M105;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M106;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M107;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M108;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M111;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M112;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M113;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M114;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M115;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M116;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M117;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M118;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M119;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M120;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M121;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M122;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M123;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M124;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M125;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M126;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M131;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M132;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M133;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M134;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M135;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M136;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M137;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_M138;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_ANY;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_TYPE_VALUE_MAX;	
HI_EXTERN HI_CONST HI_U32 HI_FTM_PRODUCT_BOOTID_BASE_VALUE;
#if defined(PRODUCT_CFG_AUTO_RESET_SYSTEM_TIMEOUT)
HI_EXTERN HI_CONST HI_U32 g_ulAutoResetSystemTimeOutCfg;
#endif
#endif // PRODUCT_CFG_SUPPORT_CONFIG_VARIABLE


//*****************************************************************************
#define HI_FLASH_USER_VALID_ADDR_MIN    (PRODUCT_CFG_CRASH_FLASH_ADDR+PRODUCT_CFG_CRASH_MAX_SIZE)
//*****************************************************************************
#endif // HI_HAVE_CROSS_COMPILER_DIAB_AS



//*****************************************************************************
#endif // __HI_CONFIG_H__

