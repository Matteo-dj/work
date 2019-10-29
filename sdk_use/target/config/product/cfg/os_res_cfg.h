//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : os_res_cfg.h
//  �� �� ��   : V1.0
//  ��    ��   : shenhankun/00130424
//  ��������   : 2011-10-18
//  ��������   : MDM�ڲ��ӿڶ��壬���ø��ļ�����ʹ�� # include  <sal_inf.h>  ��Ҫֱ�� include ���ļ�
//
//  �����б�   : SAL_TASK_ID_ENUM
//  �޸���ʷ   :
//  1.��    �� : 2011-10-18
//    ��    �� : shenhankun/00130424
//    �޸����� : �����ļ�
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
// �ڴ��
//*****************************************************************************
#ifdef SAL_HAVE_RES_CFG_TYPES
// �ڴ�� ID  ����
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
// ��������
// ע��ö��˳�� TASK_CFG_DEFINED/INIT_PROC_CFG_DEFINED ���ö�������Ҫ�ϸ�һ��
//*****************************************************************************
#ifdef SAL_HAVE_RES_CFG_TYPES
typedef enum
{
#if !defined(PRODUCT_CFG_FEATURE_REMOTE_COMMUNICATION_TERMINAL)
    EN_SAL_TASK_SAL_APP_UART_RX,   // APP����
#if defined(PRODUCT_CFG_SUPPORT_IR)
    EN_SAL_TASK_SAL_APP_IR_RX,     // APP IR ����
#endif
#endif//PRODUCT_CFG_FEATURE_REMOTE_COMMUNICATION_TERMINAL
    EN_SAL_TASK_SAL_TCP_RX,        // TCP����
    EN_SAL_TASK_SAL_DIAG_UART_RX,  // DIAG��UART����
    EN_SAL_TASK_SAL_DIAG,          // DIAG�����첽����
    EN_SAL_TASK_SAL_DIAG_RX,       // DIAG�����첽����
#if defined(PRODUCT_CFG_DFX_SAVE_FLASH_USE_TASK)
    EN_SAL_TASK_SAL_DIAG_LF,       // DIAG���浽Flash����
#endif
#if defined(PRODUCT_CFG_SUPPORT_UART2)&&!defined(PRODUCT_CFG_FEATURE_REMOTE_COMMUNICATION_TERMINAL)&&!defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    EN_APP_EQUIP_UART2,
#endif
    EN_SAL_TASK_CMN_INIT,
    EN_SAL_TASK_PHY_INIT,          // PHY��ʼ��
    EN_SAL_TASK_DRV_INIT,          // DRV ��ʼ��
#if defined(PRODUCT_CFG_TASK_STACK_SIZE_APP_UPG) && defined(PRODUCT_CFG_TASK_PRIO_APP_UPG)
    EN_SAL_TASK_APP_UPG,          // APP UPG
#endif

#ifdef PRODUCT_CFG_ONLINE_TEST_MODE
    EN_SAL_TASK_MAC_GT_TX,   // MAC ���ͻ�
    EN_SAL_TASK_MAC_DFX,  // MAC ��ά�ɲ�
    EN_SAL_TASK_MAC_RSB,  // MAC ������
#endif

#if defined(PRODUCT_CFG_FEATURE_MAC_TASK)
    EN_SAL_TASK_MAC_INIT,          // MAC��ʼ��
#ifndef PRODUCT_CFG_ONLINE_TEST_MODE
    EN_SAL_TASK_MAC_RSB,  // MAC ������
    EN_SAL_TASK_MAC_TX,   // MAC ���ͻ�
#endif
    //EN_SAL_TASK_MAC_CS,   // MAC ��ۺͷֶ�
    EN_SAL_TASK_MAC_NM_MSG,     // MAC ���������Ϣ����
    EN_SAL_TASK_MAC_NM_EVENT,   // MAC ��������¼�����
#if defined(PRODUCT_CFG_FEATURE_NDC_CHL)
    EN_SAL_TASK_MAC_NDC,  // MAC-NDC
#endif
/* BEGIN: Added by x00180816, 2014/3/12   ���ⵥ��:delete pkc task*/
#if defined(PRODUCT_CFG_FEATURE_NDC_CHL_PK_FC)
    EN_SAL_TASK_MAC_NDCPKFC,  // MAC-NDC PK FC
#endif
/* END:   Added by x00180816, 2014/3/12 */
#ifndef PRODUCT_CFG_ONLINE_TEST_MODE
    EN_SAL_TASK_MAC_DFX,  // MAC ��ά�ɲ�
#endif
#endif

#if defined(PRODUCT_CFG_FEATURE_APP)
    EN_SAL_TASK_APP_INIT,          // APP��ʼ��
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
// ��˳��ִ�г�ʼ����� ������ ������
// ����ʵ����Ϊ  HI_EAPI HI_VOID ��������(HI_VOID) { ... }
// ��ʽ: ģ������, ����ջ��С, HI_MAKEU32(���ȼ�,����), �����ʼ�����, ���������, ������(��ѡ)
// ����֧�� 1--90
//*****************************************************************************

// ����Ϊ�������� ----
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
// ��������˳�򲻿ɵ��� ---


// ����Ϊҵ����ص����� ---
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
/* BEGIN: Added by x00180816, 2014/3/12   ���ⵥ��:delete pkc task*/
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




// ҵ����ص����� ---
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
// ��Ϣ��������
// ע��ö��˳�����ö������ϸ�һ��
//*****************************************************************************
#ifdef SAL_HAVE_RES_CFG_TYPES
// ����ID����
typedef enum
{
#if !defined(PRODUCT_CFG_FEATURE_DIAG_RX)
    EN_SAL_QUEUE_DIAG_RX,   // DIAG���ն���
#endif
    EN_SAL_QUEUE_DIAG_ACK_RX, // DIAG��ACK���ն���
    EN_SAL_QUEUE_DIAG_TX,   // DIAG���Ͷ���
    #if defined(PRODUCT_CFG_DFX_SAVE_FLASH_USE_TASK) && defined(PRODUCT_CFG_DFX_KEY_MESSAGE_SAVE_FLASH)
    EN_SAL_QUEUE_DIAG_TX_LF,  // DIAG���͵�Flash�Ķ���
    #endif
#if defined(PRODUCT_CFG_TASK_STACK_SIZE_APP_UPG) && defined(PRODUCT_CFG_TASK_PRIO_APP_UPG)
    EN_SAL_QUEUE_APP_UPG,
#endif
    EN_SAL_QUEUE_NM,        // 1. ��������������ڽ�����Ϣ����Ϣ����
    //EN_SAL_QUEUE_CS,        // 2. ��۷ֶ��������ڽ�����Ϣ����Ϣ����
    EN_SAL_QUEUE_NDC,
/* BEGIN: Added by x00180816, 2014/3/12   ���ⵥ��:delete pkc task*/
#if defined(PRODUCT_CFG_FEATURE_NDC_CHL_PK_FC)
    EN_SAL_QUEUE_NDC_PK_FC,
#endif
/* END:   Added by x00180816, 2014/3/12 */
    EN_SAL_QUEUE_DFX,       // 3. ��ά�ɲ��������ڽ���MSP�͸�ģ��ķ�����Ϣ�Ķ���
    EN_SAL_QUEUE_RSB,
    /* BEGIN: Added by likunhe/00194117, 2014/3/4   PN:DTS2014032708937*/
#ifdef PRODUCT_CFG_PRODUCT_TYPE_CCO
    EN_SAL_QUEUE_FA,        // MAC Ƶ������Ӧģ�����Ϣ����
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
// ���д�С����
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
/* BEGIN: Added by x00180816, 2014/3/12   ���ⵥ��:delete pkc task*/
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
// �ź�������
// ע��ö��˳�����ö������ϸ�һ��
//*****************************************************************************
typedef enum
{
    EN_SAL_SEM_NVM,      // NV��д������֧��
    EN_SAL_SEM_TCP_RX,   // TCP��˿�֧��
    EN_SAL_SEM_UART_TX,  // ���Ӷ�UART��д����
    EN_SAL_SEM_DIAG_TX,
    EN_SAL_SEM_DIAG_CONNECT,
    #if defined(PRODUCT_CFG_DFX_KEY_MESSAGE_SAVE_FLASH) || defined(PRODUCT_CFG_DIAG_IND_SAVE_FLASH)
    EN_SAL_SEM_DMS_LF,
    #endif
    EN_SAL_SEM_DMS,               // �˿ڷ���
    EN_SAL_SEM_NM_ADDED_STA_LIST, //  ��������վ����Ϣ����
    EN_SAL_SEM_ROUTE_DL,          //  ·��ģ��ķ����б�
    EN_SAL_SEM_ROUTE_TRANS_TABLE, //  ·��ת����Ϣ��
    EN_SAL_SEM_RSB_BUF_LIST,      //  ����ģ��
    EN_SAL_SEM_SEG,               //  �ֶ�ģ�黺����б����ź���
    EN_SAL_SEM_SEG_ROUTE_TABLE,     // ·��ģ��·�ɱ����
    EN_SAL_SEM_MAC_TIMER,

    EN_SAL_SEM_MAC_NM_REJION,
    EN_SAL_SEM_MAC_BUSINESS_PERIOD,     //ҵ��ģ������TDMA��BIND CSMAʱ϶ʱͬ���ź���
    /* BEGIN: Added by likunhe/00194117, 2014/2/18   PN:DTS2014032708937*/
    EN_SAL_SEM_FA_PRE_WHIST_LIST,       //�洢��ǰһ���������ı����ź���
    /* END:   Added by likunhe/00194117, 2014/2/18 */
    /* BEGIN: Added by zhangguobin, 2014/8/4   PN:DTS2014120901643 */
    EN_SAL_PLC_SWITCH_MAPPING_TABLE,
    EN_SAL_PLC_SWITCH_FRAGMENT_QUEUE,
    /* END:   Added by zhangguobin, 2014/8/4 */

    EN_SAL_SEM_BQ_I2C_BUS,
    EN_SAL_SEM_UPG_STATUS,
/* BEGIN: Added by b00208046, 2014/3/13   ���ⵥ��:DTS2014031300959 */
#if defined(PRODUCT_CFG_UFC_FLASH_MANAGE)
    EN_SAL_SEM_UFC_FLASH,
#endif
/* END:   Added by b00208046, 2014/3/13 */
    EN_SAL_SEM_MAC_NM_BACKUP_MAC,       //�޸ı���MAC��ַ�ı����ź���
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
/* BEGIN: Added by b00208046, 2014/3/13   ���ⵥ��:DTS2014031300959 */
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
   // ���� �ź�������  "MAC_TIMER"
//*****************************************************************************


#if defined (PRODUCT_CFG_ONLINE_TEST_MODE)
#define GT_TX "GT_TX",
#else
#define GT_TX "TX",
#endif

//*****************************************************************************
// �¼�����
// ע��ö��˳�����ö������ϸ�һ��
//*****************************************************************************
typedef enum
{
#ifdef PRODUCT_CFG_ONLINE_TEST_MODE
	EN_SAL_EVENT_GT_TX     ,        // ���ͻ���������¼�:
#else
    EN_SAL_EVENT_TX     ,        // ���ͻ���������¼�:
#endif
    EN_SAL_EVENT_RSB    ,       // ���ջ���HISRͨ���¼�֪ͨ������������
    EN_SAL_EVENT_NM     ,        // �������ģ�����ڽ����ű�֡���¼�
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
// ��ʱ������
// ע��ö��˳�����ö������ϸ�һ��
//*****************************************************************************
#define  SAL_INVALID_TIMER_ID  (0xFFFFFFFF)

//*****************************************************************************
// OS��ʱ������(call-back)
// ע��ö��˳�����ö������ϸ�һ��
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
// �߾��ȶ�ʱ������
// ע��ö��˳�����ö������ϸ�һ��
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
// ����ʱ������(message)
// ע��ö��˳�����ö������ϸ�һ��
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


