//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : os_res_cfg_equip.h
//  版 本 号   : V1.0
//  作    者   : shenhankun/00130424
//  生成日期   : 2011-10-18
//  功能描述   : For Test Mode
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2011-10-18
//    作    者 : shenhankun/00130424
//    修改内容 : 创建文件
//
//*****************************************************************************

//*****************************************************************************
#ifndef __HI_MDM_H__
#error "please include hi_sal.h instead of this file"
#endif
//*****************************************************************************


#define SAL_DUMMY()


//*****************************************************************************
// 内存池
//*****************************************************************************
#ifdef SAL_HAVE_RES_CFG_TYPES
// 内存池 ID  定义
typedef enum
{
    EN_SAL_MEMPOOL_STATIC,
    EN_SAL_MEMPOOL_DYN,
    EN_SAL_MEMPOOL_MAX
} SAL_MEMPOOL_ENUM;
#endif // SAL_HAVE_RES_CFG_TYPES
#ifdef SAL_HAVE_MEM_POOL_GLOBALS
#include <hi_res_types.h>
MEM_POOL_DEFINED(STATIC,  EN_SAL_MEMPOOL_APP_SZIE)
MEM_POOL_DEFINED(DYN,     EN_SAL_MEMPOOL_APP_SZIE)
#endif
//*****************************************************************************



//*****************************************************************************
// 任务配置
// 注意枚举顺和 TASK_CFG_DEFINED/INIT_PROC_CFG_DEFINED 配置定义行需要严格一致
//*****************************************************************************
#ifdef SAL_HAVE_RES_CFG_TYPES
typedef enum
{
    EN_SAL_TASK_EQUIP_0,
    EN_SAL_TASK_EQUIP_1,
    EN_SAL_TASK_EQUIP_2,
    EN_SAL_TASK_EQUIP_3,

    EN_SAL_TASK_MAX
} SAL_TASK_ID_ENUM;
//*****************************************************************************

#endif // SAL_HAVE_RES_CFG_TYPES

#define _SEP_SPACE   HI_SYS_TASK_STK_SEP_SPACE_SIZE

#ifdef SAL_HAVE_TASK_STACK_GLOBALS
#include <hi_res_types.h>
//*****************************************************************************

//*****************************************************************************
// 按顺序执行初始化入口 、创建 任务体
// 函数实现体为  HI_EAPI HI_VOID 函数名称(HI_VOID) { ... }
//*****************************************************************************
TASK_CFG_DEFINED(EQUIP_0          ,  PRODUCT_CFG_TASK_STACK_SIZE_EQUIP_0+_SEP_SPACE,     PRODUCT_CFG_TASK_PRIO_EQUIP_0,    EQUIP_TestTaskInit    ,  EQUIP_TestTaskBody    ,  HI_DLL_LIB_NAME("mac,mac_st,mac_ut"))
TASK_CFG_DEFINED(EQUIP_1          ,  PRODUCT_CFG_TASK_STACK_SIZE_EQUIP_1+_SEP_SPACE,     PRODUCT_CFG_TASK_PRIO_EQUIP_1,    EQUIP_Uart0TaskInit   ,  EQUIP_Uart0TaskBody   ,  HI_DLL_LIB_NAME("mac,mac_st,mac_ut"))
TASK_CFG_DEFINED(EQUIP_2          ,  PRODUCT_CFG_TASK_STACK_SIZE_EQUIP_2+_SEP_SPACE,     PRODUCT_CFG_TASK_PRIO_EQUIP_2,    EQUIP_Uart1TaskInit   ,  EQUIP_Uart1TaskBody   ,  HI_DLL_LIB_NAME("mac,mac_st,mac_ut"))
TASK_CFG_DEFINED(EQUIP_3          ,  PRODUCT_CFG_TASK_STACK_SIZE_EQUIP_3+_SEP_SPACE,     PRODUCT_CFG_TASK_PRIO_EQUIP_3,    EQUIP_Uart2TaskInit   ,  EQUIP_Uart2TaskBody   ,  HI_DLL_LIB_NAME("mac,mac_st,mac_ut"))

#endif // SAL_HAVE_TASK_STACK_GLOBALS
//*****************************************************************************


//*****************************************************************************
// 消息队列配置
// 注意枚举顺和配置定义行严格一致
//*****************************************************************************
#ifdef SAL_HAVE_RES_CFG_TYPES
// 队列ID定义
typedef enum
{
#if !defined(PRODUCT_CFG_FEATURE_DIAG_RX)
    EN_SAL_QUEUE_DIAG_RX,   // DIAG接收队列
#endif
    EN_SAL_QUEUE_DIAG_ACK_RX, // DIAG的ACK接收队列
    EN_SAL_QUEUE_DIAG_TX,   // DIAG发送队列
    #if defined(PRODUCT_CFG_DFX_SAVE_FLASH_USE_TASK) && defined(PRODUCT_CFG_DFX_KEY_MESSAGE_SAVE_FLASH)
    EN_SAL_QUEUE_DIAG_TX_LF,  // DIAG发送到Flash的队列
    #endif
#if defined(PRODUCT_CFG_TASK_STACK_SIZE_APP_UPG) && defined(PRODUCT_CFG_TASK_PRIO_APP_UPG)
    EN_SAL_QUEUE_APP_UPG,
#endif
    EN_SAL_QUEUE_NM,        // 1. 网络管理任务用于接收消息的消息队列
    //EN_SAL_QUEUE_CS,        // 2. 汇聚分段任务用于接收消息的消息队列
    EN_SAL_QUEUE_DFX,       // 3. 可维可测任务用于接收MSP和各模块的发来消息的队列
    EN_SAL_Q_MAX
} SAL_QUEUE_ID_ENUM;
#endif // SAL_HAVE_RES_CFG_TYPES
#ifdef SAL_HAVE_QUEUE_GLOBALS
#include <hi_res_types.h>
// 队列大小定义
SAL_DUMMY()
#if !defined(PRODUCT_CFG_FEATURE_DIAG_RX)
SAL_Q_DEFINED(DIAG_RX,            PRODUCT_CFG_Q_DIAG_RX_MSG_NUM)
#endif
SAL_Q_DEFINED(DIAG_ACK_RX,        PRODUCT_CFG_Q_DIAG_RX_MSG_NUM)
SAL_Q_DEFINED(DIAG_TX,            HI_DIAG_BUFFERING_TOTAL_NUM)
#if defined(PRODUCT_CFG_DFX_SAVE_FLASH_USE_TASK) && defined(PRODUCT_CFG_DFX_KEY_MESSAGE_SAVE_FLASH)
SAL_Q_DEFINED(DIAG_TX_LF,         HI_DIAG_BUFFERING_LOCAL_NUM)
#endif
#if defined(PRODUCT_CFG_TASK_STACK_SIZE_APP_UPG) && defined(PRODUCT_CFG_TASK_PRIO_APP_UPG)
SAL_Q_DEFINED(APP_UPG,            PRODUCT_CFG_Q_APP_UPG_MSG_NUM)
#endif
SAL_Q_DEFINED(MAC_NM,             PRODUCT_CFG_Q_MAC_NM_MSG_NUM)
//SAL_Q_DEFINED(MAC_CS,             PRODUCT_CFG_Q_MAC_CS_MSG_NUM)
SAL_Q_DEFINED(MAC_DFX,            PRODUCT_CFG_Q_MAC_DFX_MSG_NUM)
#endif // SAL_HAVE_QUEUE_GLOBALS
//*****************************************************************************


#ifdef SAL_HAVE_RES_CFG_TYPES
//*****************************************************************************
// 信号量配置
// 注意枚举顺序和配置名字定义行严格一致!!!
//*****************************************************************************
typedef enum
{
    EN_SAL_SEM_NVM,      // NV读写多任务支持
    EN_SAL_SEM_TCP_RX,   // TCP多端口支持
    EN_SAL_SEM_UART_TX,  // 增加对UART口写保护
    EN_SAL_SEM_DIAG_TX,
    EN_SAL_SEM_DIAG_CONNECT,
    #if defined(PRODUCT_CFG_DFX_KEY_MESSAGE_SAVE_FLASH) || defined(PRODUCT_CFG_DIAG_IND_SAVE_FLASH)
    EN_SAL_SEM_DMS_LF,
    #endif
    EN_SAL_SEM_DMS,               // 端口访问
    EN_SAL_SEM_NM_ADDED_STA_LIST, //  自组网的站点信息链表
    EN_SAL_SEM_ROUTE_DL,          //  路由模块的发现列表
    EN_SAL_SEM_ROUTE_TRANS_TABLE, //  路由转发信息表
    EN_SAL_SEM_RSB_BUF_LIST,      //  重组模块
    EN_SAL_SEM_SEG,      //  分段模块缓存队列保护信号量
    EN_SAL_SEM_SEG_ROUTE_TABLE,     // 路由模块路由表操作
    /* BEGIN: Added by likunhe/00194117, 2012/11/2   问题单号:DTS2012081602262 */
    /* BEGIN: Deleted by likunhe/00194117, 2013/7/19   问题单号:DTS2013071702738*/
    //EN_SAL_SEM_SEG_PHY_TX_BUF,              // phy的发送缓存的保护信号量
    /* END: Deleted by likunhe/00194117, 2013/7/19 */
    /* END:   Added by likunhe/00194117, 2012/11/2 */
/* BEGIN: Added by j00216219, 2012/12/27   问题单号:DTS2012121106949 */
    EN_SAL_SEM_MAC_TIMER,
/* END:   Added by j00216219, 2012/12/27 */

    EN_SAL_SEM_MAC_NM_REJION, // BEGIN : DTS2012122101463, ADDED by s00130424  2013-01-18: MAC重入网保护
    EN_SAL_SEM_MAC_BUSINESS_PERIOD,     //业务模块申请TDMA和BIND CSMA时隙时同步信号量
    /* BEGIN: Added by j00216219, 2014/5/14   PN:DTS2014063001626*/
    EN_SAL_SEM_MAC_NM_BACKUP_MAC,       //修改备份MAC地址的保护信号量
    /* END:   Added by j00216219, 2014/5/14 */
    EN_SAL_SEM_MAX
} SAL_SEM_ENUM;
//*****************************************************************************

#if defined(PRODUCT_CFG_DFX_KEY_MESSAGE_SAVE_FLASH) || defined(PRODUCT_CFG_DIAG_IND_SAVE_FLASH)
#define SEM_DMS_LF_NAME   "DMS_LF",
#else
#define SEM_DMS_LF_NAME
#endif

// up to 16 bytes.
#define SAL_SEM_NAME_DEFINE {\
    "NVM"                ,  \
    "TCP_RX"             ,  \
    "UART_TX"            ,  \
    "DIAG_TX"            ,  \
    "DIAG_CONNECT"       ,  \
    SEM_DMS_LF_NAME         \
    "DMS"                ,  \
    "NM_AD_STA_LST"      ,  \
    "RT_DL"              ,  \
    "RT_TRAN_TBL"        ,  \
    "RSB_BUF_LIST"       ,  \
    "SEG"       ,  \
    "RT_TABLE"           ,  \
    "MAC_TIMER"          ,  \
    "NM_REJION"          ,  \
    "MAC_BUSI_PERIOD"    ,  \
    "MAC_BACKUP_MAC"     , \
    }
/* BEGIN: Added by j00216219, 2012/12/27   问题单号:DTS2012121106949 */
   // 增加 信号量名称  "MAC_TIMER"
/* END:   Added by j00216219, 2012/12/27 */
//*****************************************************************************


//*****************************************************************************
// 事件配置
// 注意枚举顺序和配置名字定义行严格一致!!!
//*****************************************************************************
typedef enum
{
    EN_SAL_EVENT_TX,        // 发送机任务里的事件:
    EN_SAL_EVENT_RSB,       // 接收机的HISR通过事件通知重组任务重组
    EN_SAL_EVENT_NM,        // 网络管理模块用于接收信标帧的事件
/* BEGIN: Added by 张娜00193780, 2013/2/16   PN:DTS2013020508134 */
    EN_SAL_EVENT_EQUIP_UART0,
    EN_SAL_EVENT_EQUIP_UART1,
    EN_SAL_EVENT_EQUIP_UART2,
    EN_SAL_EVENT_EQUIP_TASK_BEGIN,
    EN_SAL_EVENT_EQUIP_STA_OK,
    EN_SAL_EVENT_EQUIP_UART_MAC,
/* END:   Added by 张娜00193780, 2013/2/16 */
    EN_SAL_EVENT_MAX
} SAL_EVT_ENUM;
/* BEGIN: Added by 张娜00193780, 2013/2/16   PN:DTS2013020508134 */
#define SAL_EVT_NAME_DEFINE { \
    "CS", \
    "TX", \
    "RSB", \
    "NM" \
    "EQUIP_UART0", \
    "EQUIP_UART1", \
    "EQUIP_UART2", \
    "EQUIP_TASK_BEGIN", \
    "EQUIP_STA_OK", \
    "EQUIP_UART_MAC",\
    }
//*****************************************************************************


//*****************************************************************************
// 定时器配置
// 注意枚举顺和配置定义行严格一致
//*****************************************************************************
#define  SAL_INVALID_TIMER_ID  (0xFFFFFFFF)

//*****************************************************************************
// OS定时器配置(call-back)
// 注意枚举顺和配置定义行严格一致
//*****************************************************************************
typedef enum
{
    EN_SAL_OS_TIMER_STAT_RPT,
    EN_SAL_OS_TIMER_MAX
} SAL_OS_TIMER_ENUM;
#define SAL_OS_TIMER_NAME_DEFINE { \
    "OS_TIMER_STAT_RPT", \
    }


//*****************************************************************************
// 高精度定时器配置
// 注意枚举顺和配置定义行严格一致
//*****************************************************************************
typedef enum
{
    EN_SAL_HR_TIMER_RESERVED,
    EN_SAL_HR_TIMER_MAX
} SAL_HR_TIMER_ENUM;
#define SAL_HR_TIMER_NAME_DEFINE { \
    "HR_TIMER_RESERVED" \
    }


//*****************************************************************************
// 任务定时器配置(message)
// 注意枚举顺和配置定义行严格一致
//*****************************************************************************
#define  SAL_INVALID_TASK_TIMER_ID  (0xFFFFFFFF)
/* 问题单号：DTS2012123000452 MODIFY\ADD BEGAN by db  at 2013-01-28 */
typedef enum
{
    EN_APP_TASK_TIMER_UPG_1,
    EN_APP_TASK_TIMER_UPG_2,
    EN_APP_TASK_TIMER_UPG_3,
    EN_APP_TASK_TIMER_UPG_4,
    EN_APP_TASK_TIMER_UPG_5,
    EN_APP_TASK_TIMER_UPG_6,
    EN_APP_TASK_TIMER_UPG_7,
    EN_APP_TASK_TIMER_UPG_8,
    EN_APP_TASK_TIMER_UPG_9,
    EN_APP_TASK_TIMER_UPG_10,

    EN_SAL_TASK_TIMER_MAX
} SAL_TASK_TIMER_ENUM;
#define SAL_TASK_TIMER_NAME_DEFINE { \
    "TIMER_UPG_1",\
    "TIMER_UPG_2",\
    "TIMER_UPG_3",\
    "TIMER_UPG_4",\
    "TIMER_UPG_5",\
    "TIMER_UPG_6",\
    "TIMER_UPG_7",\
    "TIMER_UPG_8",\
    "TIMER_UPG_9",\
    "TIMER_UPG_10",\
    " "\
    }
/* 问题单号：DTS2012123000452 MODIFY\ADD END by db  at 2013-01-28 */


//*****************************************************************************
#endif // SAL_HAVE_RES_TYPES


