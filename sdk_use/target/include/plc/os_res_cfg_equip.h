//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : os_res_cfg_equip.h
//  �� �� ��   : V1.0
//  ��    ��   : shenhankun/00130424
//  ��������   : 2011-10-18
//  ��������   : For Test Mode
//
//  �����б�   : TODO: ...
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
// ��˳��ִ�г�ʼ����� ������ ������
// ����ʵ����Ϊ  HI_EAPI HI_VOID ��������(HI_VOID) { ... }
//*****************************************************************************
TASK_CFG_DEFINED(EQUIP_0          ,  PRODUCT_CFG_TASK_STACK_SIZE_EQUIP_0+_SEP_SPACE,     PRODUCT_CFG_TASK_PRIO_EQUIP_0,    EQUIP_TestTaskInit    ,  EQUIP_TestTaskBody    ,  HI_DLL_LIB_NAME("mac,mac_st,mac_ut"))
TASK_CFG_DEFINED(EQUIP_1          ,  PRODUCT_CFG_TASK_STACK_SIZE_EQUIP_1+_SEP_SPACE,     PRODUCT_CFG_TASK_PRIO_EQUIP_1,    EQUIP_Uart0TaskInit   ,  EQUIP_Uart0TaskBody   ,  HI_DLL_LIB_NAME("mac,mac_st,mac_ut"))
TASK_CFG_DEFINED(EQUIP_2          ,  PRODUCT_CFG_TASK_STACK_SIZE_EQUIP_2+_SEP_SPACE,     PRODUCT_CFG_TASK_PRIO_EQUIP_2,    EQUIP_Uart1TaskInit   ,  EQUIP_Uart1TaskBody   ,  HI_DLL_LIB_NAME("mac,mac_st,mac_ut"))
TASK_CFG_DEFINED(EQUIP_3          ,  PRODUCT_CFG_TASK_STACK_SIZE_EQUIP_3+_SEP_SPACE,     PRODUCT_CFG_TASK_PRIO_EQUIP_3,    EQUIP_Uart2TaskInit   ,  EQUIP_Uart2TaskBody   ,  HI_DLL_LIB_NAME("mac,mac_st,mac_ut"))

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
    EN_SAL_QUEUE_DFX,       // 3. ��ά�ɲ��������ڽ���MSP�͸�ģ��ķ�����Ϣ�Ķ���
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
// �ź�������
// ע��ö��˳����������ֶ������ϸ�һ��!!!
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
    EN_SAL_SEM_SEG,      //  �ֶ�ģ�黺����б����ź���
    EN_SAL_SEM_SEG_ROUTE_TABLE,     // ·��ģ��·�ɱ����
    /* BEGIN: Added by likunhe/00194117, 2012/11/2   ���ⵥ��:DTS2012081602262 */
    /* BEGIN: Deleted by likunhe/00194117, 2013/7/19   ���ⵥ��:DTS2013071702738*/
    //EN_SAL_SEM_SEG_PHY_TX_BUF,              // phy�ķ��ͻ���ı����ź���
    /* END: Deleted by likunhe/00194117, 2013/7/19 */
    /* END:   Added by likunhe/00194117, 2012/11/2 */
/* BEGIN: Added by j00216219, 2012/12/27   ���ⵥ��:DTS2012121106949 */
    EN_SAL_SEM_MAC_TIMER,
/* END:   Added by j00216219, 2012/12/27 */

    EN_SAL_SEM_MAC_NM_REJION, // BEGIN : DTS2012122101463, ADDED by s00130424  2013-01-18: MAC����������
    EN_SAL_SEM_MAC_BUSINESS_PERIOD,     //ҵ��ģ������TDMA��BIND CSMAʱ϶ʱͬ���ź���
    /* BEGIN: Added by j00216219, 2014/5/14   PN:DTS2014063001626*/
    EN_SAL_SEM_MAC_NM_BACKUP_MAC,       //�޸ı���MAC��ַ�ı����ź���
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
/* BEGIN: Added by j00216219, 2012/12/27   ���ⵥ��:DTS2012121106949 */
   // ���� �ź�������  "MAC_TIMER"
/* END:   Added by j00216219, 2012/12/27 */
//*****************************************************************************


//*****************************************************************************
// �¼�����
// ע��ö��˳����������ֶ������ϸ�һ��!!!
//*****************************************************************************
typedef enum
{
    EN_SAL_EVENT_TX,        // ���ͻ���������¼�:
    EN_SAL_EVENT_RSB,       // ���ջ���HISRͨ���¼�֪ͨ������������
    EN_SAL_EVENT_NM,        // �������ģ�����ڽ����ű�֡���¼�
/* BEGIN: Added by ����00193780, 2013/2/16   PN:DTS2013020508134 */
    EN_SAL_EVENT_EQUIP_UART0,
    EN_SAL_EVENT_EQUIP_UART1,
    EN_SAL_EVENT_EQUIP_UART2,
    EN_SAL_EVENT_EQUIP_TASK_BEGIN,
    EN_SAL_EVENT_EQUIP_STA_OK,
    EN_SAL_EVENT_EQUIP_UART_MAC,
/* END:   Added by ����00193780, 2013/2/16 */
    EN_SAL_EVENT_MAX
} SAL_EVT_ENUM;
/* BEGIN: Added by ����00193780, 2013/2/16   PN:DTS2013020508134 */
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
    EN_SAL_OS_TIMER_MAX
} SAL_OS_TIMER_ENUM;
#define SAL_OS_TIMER_NAME_DEFINE { \
    "OS_TIMER_STAT_RPT", \
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
/* ���ⵥ�ţ�DTS2012123000452 MODIFY\ADD BEGAN by db  at 2013-01-28 */
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
/* ���ⵥ�ţ�DTS2012123000452 MODIFY\ADD END by db  at 2013-01-28 */


//*****************************************************************************
#endif // SAL_HAVE_RES_TYPES


