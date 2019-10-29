//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : os_res_types.h
//  �� �� ��   : V1.0 
//  ��    ��   : shenhankun/00130424
//  ��������   : 2011-09-06
//  ��������   : ��Դ�����ͷ�ļ�
//               ע��: ��Ҫ����/������Դ����, ��Ҫ�޸ĸ��ļ�, ��ֱ�Ӵ�plc/os_res_cfg.h �ļ�������Ӻ͵���
//  �����б�   : ��
//  �޸���ʷ   : 
//  1.��    �� : 2011-09-06
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

#define SAL_HAVE_RES_TYPES 
#include <hi_res_types.h>
#undef SAL_HAVE_RES_TYPES 

#define SAL_Q_DEFINED(name,size)                                                              SAL_RES_Q_DEFINED(OsRes, name, size)
#define TASK_CFG_DEFINED(name, size, ucPriority, pfnTaskInit, pfnTaskBody, pszFileName)       SAL_RES_TASK_CFG_DEFINED(OsRes, name, size, ucPriority, pfnTaskInit, pfnTaskBody, pszFileName)
#define INIT_PROC_CFG_DEFINED(name, size, ucPriority, pfnTaskInit, pfnTaskBody, pszFileName)  SAL_RES_INIT_PROC_CFG_DEFINED(OsRes, name, size, ucPriority, pfnTaskInit, pfnTaskBody, pszFileName)
#define SAL_TASK_CFG_VAR                                                                      SAL_RES_TASK_CFG_VAR(OsRes)
#define SAL_Q_CFG_VAR                                                                         SAL_RES_Q_CFG_VAR(OsRes)

#define SC_Q_DEFINED(name,size)                                                                  SAL_RES_Q_DEFINED(ScRes, name, size)
#define SC_TASK_CFG_DEFINED(name, size, ucPriority, pfnTaskInit, pfnTaskBody, pszFileName)       SAL_RES_TASK_CFG_DEFINED(ScRes, name, size, ucPriority, pfnTaskInit, pfnTaskBody, pszFileName)
#define SC_INIT_PROC_CFG_DEFINED(name, size, ucPriority, pfnTaskInit, pfnTaskBody, pszFileName)  SAL_RES_INIT_PROC_CFG_DEFINED(ScRes, name, size, ucPriority, pfnTaskInit, pfnTaskBody, pszFileName)
#define SC_TASK_CFG_VAR                                                                          SAL_RES_TASK_CFG_VAR(ScSc)

typedef struct
{
    TIMER_HANDLE_T hTimer;
    HI_U32 ulQueueId;
    HI_U32 bAppQueue;
    SAL_EVT_ENUM enEvtObjId;
    HI_U32 ulTaskMsg; // 0 ��Ч
    HI_U32 ulTaskEvt; // 0 ��Ч
    HI_U32 ulTimeId; // SAL_INVALID_TIMER_ID ��Ч 
    HI_U32 ulTimeOption;
}TASK_TIMER_HANDLE_T;

#define SAL_HAVE_RES_CFG_TYPES
#include SC_RES_CFG_FILE
#undef SAL_HAVE_RES_CFG_TYPES
#include SAL_MODEM_RES_CFG_FILE

typedef struct  
{
    HI_U32 ahMdSem[EN_SAL_SEM_MAX];
    HI_U32 ahMdEvt[EN_SAL_EVENT_MAX];
    HI_U32 ahMdQueue[EN_SAL_Q_MAX];
    MEM_POOL_HANDLE_T ahMdMemPool[EN_SAL_MEMPOOL_MAX];
    HI_U32 ahMdTask[EN_SAL_TASK_MAX];
    HI_U32 ahScTask[EN_SC_TASK_MAX];
    TASK_TIMER_HANDLE_T ahTaskTimer[EN_SAL_TASK_TIMER_MAX];
    TIMER_HANDLE_T ahTimer[EN_SAL_OS_TIMER_MAX]; // Task �û���ʱ��(call-back)
    HI_VOID_CALLBACK_F apfnTimer[EN_SAL_OS_TIMER_MAX];
    TIMER_HANDLE_T ahHrTimer[EN_SAL_HR_TIMER_MAX]; // HR Timer �û���ʱ��
    HI_VOID_CALLBACK_F apfnHrTimer[EN_SAL_HR_TIMER_MAX];
}SAL_RES_PREDEFINE_STRU;

#if defined(HAVE_PCLINT_CHECK)
//*****************************************************************************
SAL_RES_PREDEFINE_STRU g_stOsaResPredefined;
HI_U32 g_CFG_OSsaResPredefinedSize      = sizeof(SAL_RES_PREDEFINE_STRU);
HI_CONST HI_U32 g_CFG_OS_Q_MAX          = EN_SAL_Q_MAX;
HI_CONST HI_U32 g_CFG_OS_SEM_MAX        = EN_SAL_SEM_MAX;
HI_CONST SAL_EVT_ENUM g_CFG_OS_EVT_MAX        = EN_SAL_EVENT_MAX;
HI_CONST HI_U32 g_CFG_OS_TSK_MAX        = EN_SAL_TASK_MAX;
HI_CONST HI_U32 g_CFG_OS_ScTSK_MAX      = EN_SC_TASK_MAX;
HI_CONST HI_U32 g_CFG_OS_TASK_TIMER_MAX = EN_SAL_TASK_TIMER_MAX;
HI_CONST HI_U32 g_CFG_OS_TIMER_MAX      = EN_SAL_OS_TIMER_MAX;
HI_CONST HI_U32 g_CFG_HR_TIMER_MAX      = EN_SAL_HR_TIMER_MAX;
HI_PCSTR g_CFG_OS_TIMER_NAME_DEFINE[]   = SAL_OS_TIMER_NAME_DEFINE;
//*****************************************************************************
#endif

//*****************************************************************************
// ȫ���ź���������Ӧ�ú�DEV���ź���/Task Stack
//*****************************************************************************

#if !defined(SAL_HAVE_NO_RES_DEFINED)

//*****************************************************************************
// �ڴ������
// ���´������벻Ҫ�޸ģ���Ҫ��ɾ�ڴ�أ��� plc/sal_res_cfg.h �ļ����޸�
//*****************************************************************************
#ifdef SAL_HAVE_MEM_POOL_DEFINED 
#define SAL_HAVE_MEM_POOL_GLOBALS 
#define SAL_HAVE_MEM_POOL_DECL 
#include SAL_MODEM_RES_CFG_FILE
HI_PRV MEM_POOL_ITEM_STRU  g_stSalResMemPool[EN_SAL_MEMPOOL_MAX] =
{
#undef SAL_HAVE_MEM_POOL_DECL 
#include SAL_MODEM_RES_CFG_FILE
};
#undef SAL_HAVE_MEM_POOL_GLOBALS
#endif // SAL_HAVE_MEM_POOL_DEFINED
//*****************************************************************************


//*****************************************************************************
// ��������
// ���´������벻Ҫ�޸ģ���Ҫ��ɾ������ plc/sal_res_cfg.h �ļ����޸�
//*****************************************************************************
#define SAL_HAVE_TASK_STACK_GLOBALS
#define SAL_HAVE_TASK_CFG_DECL 
#include SAL_MODEM_RES_CFG_FILE
#undef SAL_HAVE_TASK_CFG_DECL 
#define SAL_HAVE_TASK_CFG_FUNC_DECL 
#include SAL_MODEM_RES_CFG_FILE
#undef SAL_HAVE_TASK_CFG_FUNC_DECL
#if defined(SAL_HAVE_SIMU_WIN_VER) && defined(SAL_HAVE_EXPORT_DLL)
HI_PRV HI_SYS_TASK_CTX_CFG_S  SAL_TASK_CFG_VAR[EN_SAL_TASK_MAX] = 
#else
HI_PRV HI_SYS_TASK_CTX_CFG_S  SAL_TASK_CFG_VAR[EN_SAL_TASK_MAX] = 
#endif
{
#include SAL_MODEM_RES_CFG_FILE
};
HI_PCSTR g_CFG_OsResSemName[EN_SAL_SEM_MAX]   = SAL_SEM_NAME_DEFINE;
HI_PCSTR g_CFG_OsResEvtName[EN_SAL_EVENT_MAX] = SAL_EVT_NAME_DEFINE;
#undef SAL_HAVE_TASK_STACK_GLOBALS

#if defined(SAL_HAVE_SIMU_WIN_VER)
#undef SAL_HAVE_EXPORT_DLL
#endif 
#define SAL_HAVE_TASK_STACK_GLOBALS
#define SAL_HAVE_TASK_CFG_DECL 
#include SC_RES_CFG_FILE
#undef SAL_HAVE_TASK_CFG_DECL 
#define SAL_HAVE_TASK_CFG_FUNC_DECL 
#include SC_RES_CFG_FILE
#undef SAL_HAVE_TASK_CFG_FUNC_DECL
HI_PRV HI_SYS_TASK_CTX_CFG_S  SC_TASK_CFG_VAR[EN_SC_TASK_MAX] = 
{
#include SC_RES_CFG_FILE
};
#undef SAL_HAVE_TASK_STACK_GLOBALS
#if defined(SAL_HAVE_SIMU_WIN_VER)
#define SAL_HAVE_EXPORT_DLL
#endif 
//*****************************************************************************


//*****************************************************************************
// ��Ϣ��������
// ���´������벻Ҫ�޸ģ���Ҫ��ɾ��Ϣ���У��� plc/sal_res_cfg.h �ļ����޸�
//*****************************************************************************
#define SAL_HAVE_QUEUE_GLOBALS
#define SAL_HAVE_Q_CFG_DECL
#include SAL_MODEM_RES_CFG_FILE
#undef SAL_HAVE_Q_CFG_DECL 
HI_PRV HI_SYS_QUEUE_MSG_CFG_S  SAL_Q_CFG_VAR[EN_SAL_Q_MAX] = 
{
#include SAL_MODEM_RES_CFG_FILE
};
#undef SAL_HAVE_QUEUE_GLOBALS
//*****************************************************************************

//*****************************************************************************
#if defined(SAL_HAVE_SIMU_WIN_VER) && defined(SAL_HAVE_DLL_IMPORT_FUNC) 
HI_U32 dmsPowerOn(HI_PVOID, HI_U32);
HI_VOID TestMain(HI_VOID);
#endif

//*****************************************************************************
SAL_RES_PREDEFINE_STRU g_stOsaResPredefined;
HI_U32 g_CFG_OSsaResPredefinedSize      = sizeof(SAL_RES_PREDEFINE_STRU);
HI_CONST HI_U32 g_CFG_OS_Q_MAX          = EN_SAL_Q_MAX;
HI_CONST HI_U32 g_CFG_OS_SEM_MAX        = EN_SAL_SEM_MAX;
HI_CONST SAL_EVT_ENUM g_CFG_OS_EVT_MAX        = EN_SAL_EVENT_MAX;
HI_CONST HI_U32 g_CFG_OS_TSK_MAX        = EN_SAL_TASK_MAX;
HI_CONST HI_U32 g_CFG_OS_ScTSK_MAX      = EN_SC_TASK_MAX;
HI_CONST HI_U32 g_CFG_OS_TASK_TIMER_MAX = EN_SAL_TASK_TIMER_MAX;
HI_CONST HI_U32 g_CFG_OS_TIMER_MAX      = EN_SAL_OS_TIMER_MAX;
HI_CONST HI_U32 g_CFG_HR_TIMER_MAX      = EN_SAL_HR_TIMER_MAX;
HI_PCSTR g_CFG_OS_TIMER_NAME_DEFINE[]   = SAL_OS_TIMER_NAME_DEFINE;
HI_SYS_QUEUE_MSG_CFG_S* g_CFG_OS_QUEUE  = SAL_Q_CFG_VAR;
HI_SYS_TASK_CTX_CFG_S* g_CFG_OS_TASK    = SAL_TASK_CFG_VAR;
HI_SYS_TASK_CTX_CFG_S* g_CFG_SC_TASK    = SC_TASK_CFG_VAR;
//*****************************************************************************

#else

//*****************************************************************************
HI_EXTERN SAL_RES_PREDEFINE_STRU g_stOsaResPredefined ;
HI_EXTERN HI_U32 g_CFG_OSsaResPredefinedSize          ;
HI_EXTERN HI_PCSTR g_CFG_OsResSemName[]               ;
HI_EXTERN HI_PCSTR g_CFG_OsResEvtName[]               ;
HI_EXTERN HI_CONST HI_U32 g_CFG_OS_Q_MAX              ;
HI_EXTERN HI_CONST HI_U32 g_CFG_OS_SEM_MAX            ;
HI_EXTERN HI_CONST SAL_EVT_ENUM g_CFG_OS_EVT_MAX            ;
HI_EXTERN HI_CONST HI_U32 g_CFG_OS_TSK_MAX            ;
HI_EXTERN HI_CONST HI_U32 g_CFG_OS_ScTSK_MAX          ;
HI_EXTERN HI_CONST HI_U32 g_CFG_OS_TASK_TIMER_MAX     ;
HI_EXTERN HI_CONST HI_U32 g_CFG_OS_TIMER_MAX          ;
HI_EXTERN HI_PCSTR g_CFG_OS_TIMER_NAME_DEFINE[]       ;
HI_EXTERN HI_CONST HI_U32 g_CFG_HR_TIMER_MAX          ;
HI_EXTERN HI_SYS_QUEUE_MSG_CFG_S* g_CFG_OS_QUEUE      ;
HI_EXTERN HI_SYS_TASK_CTX_CFG_S* g_CFG_OS_TASK        ;
HI_EXTERN HI_SYS_TASK_CTX_CFG_S* g_CFG_SC_TASK        ;
//*****************************************************************************

#endif

