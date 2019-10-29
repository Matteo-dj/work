//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : os_res_cfg.h
//  版 本 号   : V1.0
//  作    者   : shenhankun/00130424
//  生成日期   : 2011-10-18
//  功能描述   : MDM内部接口定义，引用该文件必须使用 # include  <sal_inf.h>  不要直接 include 该文件
//
//  函数列表   : SAL_TASK_ID_ENUM
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


#if !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
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
#if !defined(PRODUCT_CFG_FEATURE_REMOTE_COMMUNICATION_TERMINAL)
    EN_SAL_TASK_SAL_APP_UART_RX,   // APP接收
#if defined(PRODUCT_CFG_SUPPORT_IR)
    EN_SAL_TASK_SAL_APP_IR_RX,     // APP IR 接收
#endif
#endif//PRODUCT_CFG_FEATURE_REMOTE_COMMUNICATION_TERMINAL
    EN_SAL_TASK_SAL_TCP_RX,        // TCP接收
    EN_SAL_TASK_SAL_DIAG_UART_RX,  // DIAG的UART接收
    EN_SAL_TASK_SAL_DIAG,          // DIAG发送异步处理
    EN_SAL_TASK_SAL_DIAG_RX,       // DIAG接收异步处理
#if defined(PRODUCT_CFG_DFX_SAVE_FLASH_USE_TASK)
    EN_SAL_TASK_SAL_DIAG_LF,       // DIAG保存到Flash任务
#endif
#if defined(PRODUCT_CFG_SUPPORT_UART2)&&!defined(PRODUCT_CFG_FEATURE_REMOTE_COMMUNICATION_TERMINAL)&&!defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    EN_APP_EQUIP_UART2,
#endif
    EN_SAL_TASK_CMN_INIT,
    EN_SAL_TASK_PHY_INIT,          // PHY初始化
    EN_SAL_TASK_DRV_INIT,          // DRV 初始化
#if defined(PRODUCT_CFG_TASK_STACK_SIZE_APP_UPG) && defined(PRODUCT_CFG_TASK_PRIO_APP_UPG)
    EN_SAL_TASK_APP_UPG,          // APP UPG
#endif

#ifdef PRODUCT_CFG_ONLINE_TEST_MODE
    EN_SAL_TASK_MAC_GT_TX,   // MAC 发送机
    EN_SAL_TASK_MAC_DFX,  // MAC 可维可测
    EN_SAL_TASK_MAC_RSB,  // MAC 重组器
#endif

#if defined(PRODUCT_CFG_FEATURE_MAC_TASK)
    EN_SAL_TASK_MAC_INIT,          // MAC初始化
#ifndef PRODUCT_CFG_ONLINE_TEST_MODE
    EN_SAL_TASK_MAC_RSB,  // MAC 重组器
    EN_SAL_TASK_MAC_TX,   // MAC 发送机
#endif
    //EN_SAL_TASK_MAC_CS,   // MAC 汇聚和分段
    EN_SAL_TASK_MAC_NM_MSG,     // MAC 网络管理消息进程
    EN_SAL_TASK_MAC_NM_EVENT,   // MAC 网络管理事件进程
#if defined(PRODUCT_CFG_FEATURE_NDC_CHL)
    EN_SAL_TASK_MAC_NDC,  // MAC-NDC
#endif
/* BEGIN: Added by x00180816, 2014/3/12   问题单号:delete pkc task*/
#if defined(PRODUCT_CFG_FEATURE_NDC_CHL_PK_FC)
    EN_SAL_TASK_MAC_NDCPKFC,  // MAC-NDC PK FC
#endif
/* END:   Added by x00180816, 2014/3/12 */
#ifndef PRODUCT_CFG_ONLINE_TEST_MODE
    EN_SAL_TASK_MAC_DFX,  // MAC 可维可测
#endif
#endif

#if defined(PRODUCT_CFG_FEATURE_APP)
    EN_SAL_TASK_APP_INIT,          // APP初始化
#endif


#if !defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    EN_SAL_TASK_LONG_TIME_DIAG_CMD,
#endif
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
// 格式: 模块名称, 任务栈大小, HI_MAKEU32(优先级,索引), 任务初始化入口, 任务体入口, 库名称(可选)
// 索引支持 1--90
//*****************************************************************************

// 以下为基础任务 ----
#if !defined(PRODUCT_CFG_FEATURE_REMOTE_COMMUNICATION_TERMINAL)
TASK_CFG_DEFINED(SAL_APP_UART_RX, PRODUCT_CFG_TASK_STACK_SIZE_SAL_APP_UART_RX+_SEP_SPACE, HI_MAKEU32(PRODUCT_CFG_TASK_PRIO_SAL_APP_UART_RX, 1) , \
                SAL_AppUartRxTaskInit,  SAL_AppUartRxTaskBody,  HI_DLL_LIB_NAME("sal"))

#if defined(PRODUCT_CFG_SUPPORT_IR)
TASK_CFG_DEFINED(SAL_APP_IR_RX,  PRODUCT_CFG_TASK_STACK_SIZE_SAL_APP_UART_RX+_SEP_SPACE, HI_MAKEU32(PRODUCT_CFG_TASK_PRIO_SAL_APP_UART_RX, 2) , \
                 SAL_AppIrRxTaskInit, SAL_AppIrRxTaskBody, HI_DLL_LIB_NAME("sal"))
#endif
#endif

TASK_CFG_DEFINED(SAL_TCP_RX,  PRODUCT_CFG_TASK_STACK_SIZE_SAL_TCP_RX+_SEP_SPACE, HI_MAKEU32(PRODUCT_CFG_TASK_PRIO_SAL_TCP_RX,3), \
                 SAL_TcpRxTaskInit, SAL_TcpRxTaskBody, HI_DLL_LIB_NAME("sal"))


TASK_CFG_DEFINED(SAL_DIAG_UART_RX, PRODUCT_CFG_TASK_STACK_SIZE_SAL_DIAG_UART_RX+_SEP_SPACE, HI_MAKEU32(PRODUCT_CFG_TASK_PRIO_SAL_DIAG_UART_RX,5), \
                 SAL_DiagUartTaskInit, SAL_DiagUartTaskBody, HI_DLL_LIB_NAME("sal"))

TASK_CFG_DEFINED(SAL_DIAG_TX, PRODUCT_CFG_TASK_STACK_SIZE_SAL_DIAG+_SEP_SPACE, HI_MAKEU32(PRODUCT_CFG_TASK_PRIO_SAL_DIAG, 6), \
                 SAL_DiagTxTaskInit, SAL_DiagTxTaskBody, HI_DLL_LIB_NAME("sal"))

TASK_CFG_DEFINED(SAL_DIAG_RX, PRODUCT_CFG_TASK_STACK_SIZE_SAL_DIAG+_SEP_SPACE, HI_MAKEU32(PRODUCT_CFG_TASK_PRIO_SAL_TCP_RX-1, 7), \
                 SAL_DiagRxTaskInit, SAL_DiagRxTaskBody, HI_DLL_LIB_NAME("sal"))

#if defined(PRODUCT_CFG_DFX_SAVE_FLASH_USE_TASK)
TASK_CFG_DEFINED(SAL_DIAG_LF, PRODUCT_CFG_TASK_STACK_SIZE_SAL_DIAG_LF+_SEP_SPACE, HI_MAKEU32(PRODUCT_CFG_TASK_PRIO_SAL_DIAG_LF, 8), \
                 SAL_DiagLFTaskInit,  SAL_DiagLFTaskBody,  HI_DLL_LIB_NAME("sal"))
#endif

#if defined(PRODUCT_CFG_SUPPORT_UART2) && !defined(PRODUCT_CFG_FEATURE_REMOTE_COMMUNICATION_TERMINAL) && !defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
TASK_CFG_DEFINED(APP_EQUIP_UART2, PRODUCT_CFG_TASK_STACK_SIZE_APP_EQUIP +_SEP_SPACE, HI_MAKEU32(PRODUCT_CFG_TASK_PRIO_APP_EQUIP, 14),\
                 SAL_EquipTaskInit, SAL_EquipTaskBody, HI_DLL_LIB_NAME("sal"))
#endif
// 以上任务顺序不可调整 ---


// 以下为业务相关的任务 ---
INIT_PROC_CFG_DEFINED(CMN_INIT     , 0, HI_MAKEU32(0,10),  CMN_Init     ,  HI_NULL  ,  HI_DLL_LIB_NAME("cmn"))
INIT_PROC_CFG_DEFINED(PHY_INIT     , 0, HI_MAKEU32(0,11),  PHY_Init     ,  HI_NULL  ,  HI_DLL_LIB_NAME("phy"))
INIT_PROC_CFG_DEFINED(DRV_DFX_INIT , 0, HI_MAKEU32(0,12),  DRV_Init     ,  HI_NULL  ,  HI_DLL_LIB_NAME("drv"))
#if defined(PRODUCT_CFG_TASK_STACK_SIZE_APP_UPG) && defined(PRODUCT_CFG_TASK_PRIO_APP_UPG)
INIT_PROC_CFG_DEFINED(APP_UPG, 0, HI_MAKEU32(0, 13),UPG_Init,HI_NULL, HI_DLL_LIB_NAME("upg"))
#endif

#ifdef PRODUCT_CFG_ONLINE_TEST_MODE
TASK_CFG_DEFINED(MAC_DFX, PRODUCT_CFG_TASK_STACK_SIZE_MAC_DFX+_SEP_SPACE, HI_MAKEU32(PRODUCT_CFG_TASK_PRIO_MAC_DFX, 38), \
                 MAC_DfxTaskInit, MAC_DfxTaskBody, HI_DLL_LIB_NAME("mac,mac_st,mac_ut"))
TASK_CFG_DEFINED(MAC_GT_TX, PRODUCT_CFG_TASK_STACK_SIZE_MAC_TX+_SEP_SPACE, HI_MAKEU32(PRODUCT_CFG_TASK_PRIO_MAC_TX,32), \
                 MAC_GtTxTaskInit, MAC_GtTxTaskBody, HI_DLL_LIB_NAME("mac,mac_st,mac_ut"))
TASK_CFG_DEFINED(MAC_RSB, PRODUCT_CFG_TASK_STACK_SIZE_MAC_RSB+_SEP_SPACE, HI_MAKEU32(PRODUCT_CFG_TASK_PRIO_MAC_RSB, 31), \
                 MAC_RsbTaskInit, MAC_RsbTaskBody, HI_DLL_LIB_NAME("mac,mac_st,mac_ut"))
#endif

#if defined(PRODUCT_CFG_FEATURE_MAC_TASK)
INIT_PROC_CFG_DEFINED(MAC_INITI, 0, HI_MAKEU32(0, 30), MAC_Init, HI_NULL, HI_DLL_LIB_NAME("mac"))

#ifndef PRODUCT_CFG_ONLINE_TEST_MODE
TASK_CFG_DEFINED(MAC_RSB, PRODUCT_CFG_TASK_STACK_SIZE_MAC_RSB+_SEP_SPACE, HI_MAKEU32(PRODUCT_CFG_TASK_PRIO_MAC_RSB, 31),\
                 MAC_RsbTaskInit, MAC_RsbTaskBody, HI_DLL_LIB_NAME("mac,mac_st,mac_ut"))
TASK_CFG_DEFINED(MAC_TX, PRODUCT_CFG_TASK_STACK_SIZE_MAC_TX+_SEP_SPACE, HI_MAKEU32(PRODUCT_CFG_TASK_PRIO_MAC_TX,32), \
                 MAC_TxTaskInit, MAC_TxTaskBody, HI_DLL_LIB_NAME("mac,mac_st,mac_ut"))
#endif
TASK_CFG_DEFINED(MAC_NM_MSG, PRODUCT_CFG_TASK_STACK_SIZE_MAC_NM_MSG+_SEP_SPACE, HI_MAKEU32(PRODUCT_CFG_TASK_PRIO_MAC_NM_MSG, 33), \
                 MAC_NmMsgTaskInit, MAC_NmMsgTaskBody,  HI_DLL_LIB_NAME("mac,mac_st,mac_ut"))
TASK_CFG_DEFINED(MAC_NM_EVENT, PRODUCT_CFG_TASK_STACK_SIZE_MAC_NM_EVT+_SEP_SPACE, HI_MAKEU32(PRODUCT_CFG_TASK_PRIO_MAC_NM_EVT, 34), \
                 MAC_NmEvtTaskInit, MAC_NmEvtTaskBody, HI_DLL_LIB_NAME("mac,mac_st,mac_ut"))

#if defined(PRODUCT_CFG_FEATURE_NDC_CHL)
TASK_CFG_DEFINED(MAC_NDC, PRODUCT_CFG_TASK_STACK_SIZE_MAC_NDC+_SEP_SPACE, HI_MAKEU32(PRODUCT_CFG_TASK_PRIO_MAC_NDC, 36), \
                 MAC_NdcTaskInit, MAC_NdcTaskBody, HI_DLL_LIB_NAME("mac,mac_st,mac_ut"))
#endif
/* BEGIN: Added by x00180816, 2014/3/12   问题单号:delete pkc task*/
#if defined(PRODUCT_CFG_FEATURE_NDC_CHL_PK_FC)
TASK_CFG_DEFINED(MAC_NDCPKFC, PRODUCT_CFG_TASK_STACK_SIZE_MAC_NDC_PKFC+_SEP_SPACE, HI_MAKEU32(PRODUCT_CFG_TASK_PRIO_MAC_NDC_PKFC, 37), \
                 MAC_NdcPkfcTaskInit, MAC_NdcPkfcTaskBody, HI_DLL_LIB_NAME("mac,mac_st,mac_ut"))
#endif
/* END:   Added by x00180816, 2014/3/12 */
#ifndef PRODUCT_CFG_ONLINE_TEST_MODE
TASK_CFG_DEFINED(MAC_DFX, PRODUCT_CFG_TASK_STACK_SIZE_MAC_DFX+_SEP_SPACE, HI_MAKEU32(PRODUCT_CFG_TASK_PRIO_MAC_DFX, 38), \
                 MAC_DfxTaskInit, MAC_DfxTaskBody, HI_DLL_LIB_NAME("mac,mac_st,mac_ut"))
#endif
#endif // PRODUCT_CFG_FEATURE_MAC_TASK

#if defined(PRODUCT_CFG_FEATURE_APP)
INIT_PROC_CFG_DEFINED(APP_INIT, 0, HI_MAKEU32(0, 50), APP_Init, HI_NULL, HI_DLL_LIB_NAME("app"))
#endif




// 业务相关的任务 ---
#if defined(PRODUCT_CFG_FEATURE_MAC_TASK)
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
#if defined(PRODUCT_CFG_FEATURE_MULTI_NETWORK_MODE)
TASK_CFG_DEFINED(RADIO_TX_TASK, 4096+_SEP_SPACE, HI_MAKEU32(PRODUCT_CFG_TASK_PRIO_RADIO_TX, 92), \
                 MAC_RfTxTaskInit, MAC_RfTxTaskBody, HI_DLL_LIB_NAME("sal"))
#endif
#endif
#endif//PRODUCT_CFG_FEATURE_MAC_TASK
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
TASK_CFG_DEFINED(DIAG_CMD_ASYNC, PRODUCT_CFG_TASK_STACK_SIZE_DIAG_ASYNC+_SEP_SPACE, HI_MAKEU32(PRODUCT_CFG_TASK_DIAG_ASYNC, 81), \
                 SAL_DiagAsyncTaskInit, SAL_DiagAsyncTaskBody, HI_DLL_LIB_NAME("sal"))
#endif
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
    EN_SAL_QUEUE_NDC,
/* BEGIN: Added by x00180816, 2014/3/12   问题单号:delete pkc task*/
#if defined(PRODUCT_CFG_FEATURE_NDC_CHL_PK_FC)
    EN_SAL_QUEUE_NDC_PK_FC,
#endif
/* END:   Added by x00180816, 2014/3/12 */
    EN_SAL_QUEUE_DFX,       // 3. 可维可测任务用于接收MSP和各模块的发来消息的队列
    EN_SAL_QUEUE_RSB,
    /* BEGIN: Added by likunhe/00194117, 2014/3/4   PN:DTS2014032708937*/
#ifdef PRODUCT_CFG_PRODUCT_TYPE_CCO
    EN_SAL_QUEUE_FA,        // MAC 频段自适应模块的消息队列
#endif
    /* END:   Added by likunhe/00194117, 2014/3/4 */
    EN_SAL_QUEUE_SDM,
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    EN_SAL_QUEUE_ASYNC_DIAG,
#endif
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
SAL_Q_DEFINED(DIAG_TX,            PRODUCT_CFG_Q_DIAG_TX_MSG_NUM)
#if defined(PRODUCT_CFG_DFX_SAVE_FLASH_USE_TASK) && defined(PRODUCT_CFG_DFX_KEY_MESSAGE_SAVE_FLASH)
SAL_Q_DEFINED(DIAG_TX_LF,         HI_DIAG_BUFFERING_LOCAL_NUM)
#endif
#if defined(PRODUCT_CFG_TASK_STACK_SIZE_APP_UPG) && defined(PRODUCT_CFG_TASK_PRIO_APP_UPG)
SAL_Q_DEFINED(APP_UPG,            PRODUCT_CFG_Q_APP_UPG_MSG_NUM)
#endif
SAL_Q_DEFINED(MAC_NM,             PRODUCT_CFG_Q_MAC_NM_MSG_NUM)
//SAL_Q_DEFINED(MAC_CS,             PRODUCT_CFG_Q_MAC_CS_MSG_NUM)
SAL_Q_DEFINED(MAC_NDC,            PRODUCT_CFG_Q_MAC_NDC_MSG_NUM)
/* BEGIN: Added by x00180816, 2014/3/12   问题单号:delete pkc task*/
#if defined(PRODUCT_CFG_FEATURE_NDC_CHL_PK_FC)
SAL_Q_DEFINED(MAC_NDCPKFC,        PRODUCT_CFG_Q_MAC_NDC_PK_FC_MSG_NUM)
#endif
/* END:   Added by x00180816, 2014/3/12 */
SAL_Q_DEFINED(MAC_DFX,            PRODUCT_CFG_Q_MAC_DFX_MSG_NUM)
SAL_Q_DEFINED(MAC_RSB,            PRODUCT_CFG_Q_MAC_RSB_MSG_NUM)
/* BEGIN: Added by likunhe/00194117, 2014/3/4   PN:DTS2014032708937*/
#ifdef PRODUCT_CFG_PRODUCT_TYPE_CCO
SAL_Q_DEFINED(MAC_FA,             PRODUCT_CFG_Q_MAC_FA_MSG_NUM)
#endif
/* END:   Added by likunhe/00194117, 2014/3/4 */
SAL_Q_DEFINED(SDM_ENG,            PRODUCT_CFG_Q_SDM_ENG_MSG_NUM)
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
SAL_Q_DEFINED(ASYNC_DIAG,         PRODUCT_CFG_Q_DIAG_ASYNC_MSG_NUM)
#endif

/* END:   Added by b00208046, 2014/1/4 */
#endif // SAL_HAVE_QUEUE_GLOBALS
//*****************************************************************************


#ifdef SAL_HAVE_RES_CFG_TYPES
//*****************************************************************************
// 信号量配置
// 注意枚举顺和配置定义行严格一致
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
    EN_SAL_SEM_SEG,               //  分段模块缓存队列保护信号量
    EN_SAL_SEM_SEG_ROUTE_TABLE,     // 路由模块路由表操作
    EN_SAL_SEM_MAC_TIMER,

    EN_SAL_SEM_MAC_NM_REJION,
    EN_SAL_SEM_MAC_BUSINESS_PERIOD,     //业务模块申请TDMA和BIND CSMA时隙时同步信号量
    /* BEGIN: Added by likunhe/00194117, 2014/2/18   PN:DTS2014032708937*/
    EN_SAL_SEM_FA_PRE_WHIST_LIST,       //存储的前一个白名单的保护信号量
    /* END:   Added by likunhe/00194117, 2014/2/18 */
    /* BEGIN: Added by zhangguobin, 2014/8/4   PN:DTS2014120901643 */
    EN_SAL_PLC_SWITCH_MAPPING_TABLE,
    EN_SAL_PLC_SWITCH_FRAGMENT_QUEUE,
    /* END:   Added by zhangguobin, 2014/8/4 */

    EN_SAL_SEM_BQ_I2C_BUS,
    EN_SAL_SEM_UPG_STATUS,
/* BEGIN: Added by b00208046, 2014/3/13   问题单号:DTS2014031300959 */
#if defined(PRODUCT_CFG_UFC_FLASH_MANAGE)
    EN_SAL_SEM_UFC_FLASH,
#endif
/* END:   Added by b00208046, 2014/3/13 */
    EN_SAL_SEM_MAC_NM_BACKUP_MAC,       //修改备份MAC地址的保护信号量
#ifdef PRODUCT_CFG_STORAGE_COLLECT_DATA
    EN_SAL_SEM_SD_DC_FOLLOW,
#endif
#ifdef PRODUCT_CFG_ONLINE_TEST_MODE
	EN_SAL_SEM_GT_TX_QUEUE,
#endif
    /* BEGIN: Added by zhangguobin, 2015/1/23   PN:Dual characteristics of development */
    EN_SAL_SEM_RF_TX_QUEUE,
    /* END:   Added by zhangguobin, 2015/1/23 */
    EN_SAL_SEM_PLC_TX_QUEUE,
    EN_SAL_SEM_MAX,
} SAL_SEM_ENUM;
//*****************************************************************************

#if defined(PRODUCT_CFG_DFX_KEY_MESSAGE_SAVE_FLASH) || defined(PRODUCT_CFG_DIAG_IND_SAVE_FLASH)
#define SEM_DMS_LF_NAME   "DMS_LF",
#else
#define SEM_DMS_LF_NAME
#endif
/* BEGIN: Added by b00208046, 2014/3/13   问题单号:DTS2014031300959 */
#if defined(PRODUCT_CFG_UFC_FLASH_MANAGE)
#define SEM_UFC_FLASH_NAME "UFC_FLASH",
#else
#define SEM_UFC_FLASH_NAME
#endif
/* END:   Added by b00208046, 2014/3/13 */

#if defined(PRODUCT_CFG_STORAGE_COLLECT_DATA)
#define SEM_SD_DC_FOLLOW_NAME "SD_DC_FOLLOW",
#else
#define SEM_SD_DC_FOLLOW_NAME
#endif

#if defined (PRODUCT_CFG_ONLINE_TEST_MODE)
#define SEM_GT_TX "SEM_GT_TX",
#else
#define SEM_GT_TX
#endif



// up to 16 bytes.
#define SAL_SEM_NAME_DEFINE {\
    "NVM"                ,  \
    "TCP_RX"             ,  \
    "UART_RX"            ,  \
    "DIAG_TX"            ,  \
    "DIAG_CONNECT"       ,  \
    SEM_DMS_LF_NAME         \
    "DMS"                ,  \
    "NM_AD_STA_LST"      ,  \
    "RT_DL"              ,  \
    "RT_TRAN_TBL"        ,  \
    "RSB_BUF_LIST"       ,  \
    "SEG"                ,  \
    "RT_TABLE"           ,  \
    "MAC_TIMER"          ,  \
    "NM_REJION"          ,  \
    "MAC_BUSI_PERIOD"    ,  \
    "FA_WHITE_LIST"      ,  \
    "PLC_SWITCH_MPT"     ,  \
    "PLC_SWITCH_FG"      ,  \
    "SEM_DRV_BQ_I2C_BUS" ,  \
    "UPG_STATUS"         ,  \
    SEM_UFC_FLASH_NAME      \
    "MAC_BACKUP_MAC"     ,\
	SEM_SD_DC_FOLLOW_NAME   \
	SEM_GT_TX\
	"RF_TX"             ,   \
	"PLC_TX_QUEUE",    \
    }
   // 增加 信号量名称  "MAC_TIMER"
//*****************************************************************************


#if defined (PRODUCT_CFG_ONLINE_TEST_MODE)
#define GT_TX "GT_TX",
#else
#define GT_TX "TX",
#endif

//*****************************************************************************
// 事件配置
// 注意枚举顺和配置定义行严格一致
//*****************************************************************************
typedef enum
{
#ifdef PRODUCT_CFG_ONLINE_TEST_MODE
	EN_SAL_EVENT_GT_TX     ,        // 发送机任务里的事件:
#else
    EN_SAL_EVENT_TX     ,        // 发送机任务里的事件:
#endif
    EN_SAL_EVENT_RSB    ,       // 接收机的HISR通过事件通知重组任务重组
    EN_SAL_EVENT_NM     ,        // 网络管理模块用于接收信标帧的事件
    EN_SAL_EVENT_IO     ,
    EN_SAL_EVENT_SDM    ,
    EN_SAL_EVENT_RF_TX  ,
    EN_SAL_EVENT_CIPHER ,
    EN_SAL_EVENT_MAX
} SAL_EVT_ENUM;
#define SAL_EVT_NAME_DEFINE { \
    GT_TX\
    "RSB"  , \
    "NM"   , \
    "IO"   , \
    "SDM"  , \
    "RFTX" , \
    "CIPHER"\
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
    EN_SAL_OS_TIMER_LD_FILE,
    EN_SAL_OS_TIMER_MAX
} SAL_OS_TIMER_ENUM;
#define SAL_OS_TIMER_NAME_DEFINE { \
    "OS_TIMER_STAT_RPT", \
    "OS_TIMER_LD_FILE", \
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
    EN_APP_TASK_TIMER_UPG_11,
    EN_APP_TASK_TIMER_UPG_12,

    EN_SAL_TASK_TIMER_MAX
} SAL_TASK_TIMER_ENUM;
#define SAL_TASK_TIMER_NAME_DEFINE { \
    "UPG_1_TIMER"  ,\
    "UPG_2_TIMER"  ,\
    "UPG_3_TIMER"  ,\
    "UPG_4_TIMER"  ,\
    "UPG_5_TIMER"  ,\
    "UPG_6_TIMER"  ,\
    "UPG_7_TIMER"  ,\
    "UPG_8_TIMER"  ,\
    "UPG_9_TIMER"  ,\
    "UPG_10_TIMER" ,\
    "UPG_11_TIMER" ,\
    "UPG_12_TIMER" ,\
     " "           \
    }


//*****************************************************************************
#endif // SAL_HAVE_RES_TYPES


//*****************************************************************************
// For TEST MODE ONLY
//
// PRODUCT_CFG_EQUIP_TEST_MODE
#else
#include "os_res_cfg_equip.h"
#endif
//PRODUCT_CFG_EQUIP_TEST_MODE
//*****************************************************************************


