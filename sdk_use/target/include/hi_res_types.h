//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : hi_res_types.h
//  版 本 号   : V1.0 
//  作    者   : shenhankun/00130424
//  生成日期   : 2011-10-18
//  功能描述   : TODO: ...
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2011-10-18
//    作    者 : shenhankun/00130424
//    修改内容 : 创建文件 
//
//*****************************************************************************

#define CHECK_RET_LOG(x,sz) {if(!x) {SAL_FL1("[SAL] e : %s not found. \n", sz); } }

#ifndef SAL_HAVE_RES_TYPES
# define TASK_CFG_DEFINED_INIT(file_name, proc_name) proc_name = (HI_PVOID)HAL_GetProcAddress(file_name, #proc_name);
# undef MEM_POOL_DEFINED
# ifdef SAL_HAVE_MEM_POOL_DECL
#  define MEM_POOL_DEFINED(res_name, name, size)     HI_PRV HI_BYTE g_uc ## res_name ## MemPool ## name[size];
# else
#  define MEM_POOL_DEFINED(res_name, name, size)     { (HI_U32)g_uc ## res_name ## MemPool ## name, size, # name},
# endif
#endif // SAL_HAVE_RES_TYPES

#ifndef SAL_HAVE_RES_TYPES

# undef SAL_RES_TASK_CFG_DEFINED
# ifdef SAL_HAVE_TASK_CFG_DECL
#if defined(PRODUCT_CFG_OS_DYN_TASK_STACK_BUFFER)
# define SAL_RES_TASK_CFG_DEFINED(res_name, name, size, ucPriority, pfnTaskInit, pfnTaskBody, pszFileName)    HI_PRV HI_BYTE g_uc ## res_name ## TaskStack ## name[1];
#else
# define SAL_RES_TASK_CFG_DEFINED(res_name, name, size, ucPriority, pfnTaskInit, pfnTaskBody, pszFileName)    HI_PRV HI_BYTE g_uc ## res_name ## TaskStack ## name[size];
# endif
# elif defined(SAL_HAVE_TASK_CFG_FUNC_DECL) && !defined(SAL_HAVE_EXPORT_DLL)
#  define SAL_RES_TASK_CFG_DEFINED(res_name, name, size, ucPriority, pfnTaskInit, pfnTaskBody, pszFileName)    HI_PUBLIC HI_VOID pfnTaskInit(HI_VOID); HI_PUBLIC HI_VOID pfnTaskBody(HI_VOID);
# elif defined(SAL_HAVE_TASK_CFG_FUNC_DECL) && defined(SAL_HAVE_EXPORT_DLL)
#  define SAL_RES_TASK_CFG_DEFINED(res_name, name, size, ucPriority, pfnTaskInit, pfnTaskBody, pszFileName)    HI_PRV HI_PVOID pfnTaskInit = NULL; HI_PRV HI_PVOID pfnTaskBody = NULL;
# elif defined(SAL_HAVE_TASK_CFG_INI_DECL) && defined(SAL_HAVE_EXPORT_DLL)
#  define SAL_RES_TASK_CFG_DEFINED(res_name, name, size, ucPriority, TaskInit, TaskBody, pszFileName)          g_st ## res_name ## TasksCfg[i].pfnTaskInit = (HI_PVOID)HAL_GetProcAddress(pszFileName, # TaskInit); CHECK_RET_LOG(g_st ## res_name ## TasksCfg[i].pfnTaskInit, # TaskInit) g_st ## res_name ## TasksCfg[i].pfnTaskBody = (HI_PVOID)HAL_GetProcAddress(pszFileName, # TaskBody);CHECK_RET_LOG(g_st ## res_name ## TasksCfg[i].pfnTaskBody, # TaskBody) i++;
# elif defined(SAL_HAVE_TASK_CFG_INI_DECL) && !defined(SAL_HAVE_EXPORT_DLL)
#  define SAL_RES_TASK_CFG_DEFINED(res_name, name, size, ucPriority, pfnTaskInit, pfnTaskBody, pszFileName)
# elif defined(SAL_HAVE_EXPORT_DLL)
#  define SAL_RES_TASK_CFG_DEFINED(res_name, name, size, ucPriority, pfnTaskInit, pfnTaskBody, pszFileName)    { (HI_PCSTR)# name, (HI_PVOID)g_uc ## res_name ## TaskStack ## name, size, ucPriority, (HI_VOID_CALLBACK_F)0, (HI_VOID_CALLBACK_F)0, # pfnTaskInit, # pfnTaskBody},
# else
#  define SAL_RES_TASK_CFG_DEFINED(res_name, name, size, ucPriority, pfnTaskInit, pfnTaskBody, pszFileName)    { (HI_PCSTR)# name, (HI_PVOID)g_uc ## res_name ## TaskStack ## name, size, ucPriority, (HI_VOID_CALLBACK_F)pfnTaskInit, (HI_VOID_CALLBACK_F)pfnTaskBody, # pfnTaskInit, # pfnTaskBody },
# endif

# undef SAL_RES_INIT_PROC_CFG_DEFINED
# ifdef SAL_HAVE_TASK_CFG_DECL
#  define SAL_RES_INIT_PROC_CFG_DEFINED(res_name, name, size, ucPriority, pfnTaskInit, pfnTaskBody, pszFileName)    
# elif defined(SAL_HAVE_TASK_CFG_FUNC_DECL) && !defined(SAL_HAVE_EXPORT_DLL)
#  define SAL_RES_INIT_PROC_CFG_DEFINED(res_name, name, size, ucPriority, pfnTaskInit, pfnTaskBody, pszFileName)    HI_PUBLIC HI_VOID pfnTaskInit(HI_VOID);
# elif defined(SAL_HAVE_TASK_CFG_FUNC_DECL) && defined(SAL_HAVE_EXPORT_DLL)
#  define SAL_RES_INIT_PROC_CFG_DEFINED(res_name, name, size, ucPriority, pfnTaskInit, pfnTaskBody, pszFileName)    HI_PRV HI_PVOID pfnTaskInit = NULL;
# elif defined(SAL_HAVE_TASK_CFG_INI_DECL) && defined(SAL_HAVE_EXPORT_DLL)
#  define SAL_RES_INIT_PROC_CFG_DEFINED(res_name, name, size, ucPriority, TaskInit, TaskBody, pszFileName)          g_st ## res_name ## TasksCfg[i].pfnTaskInit = (HI_PVOID)HAL_GetProcAddress(pszFileName, # TaskInit); CHECK_RET_LOG(g_st ## res_name ## TasksCfg[i].pfnTaskInit, # TaskInit) g_st ## res_name ## TasksCfg[i].pfnTaskBody = HI_NULL; i++;
# elif defined(SAL_HAVE_TASK_CFG_INI_DECL) && !defined(SAL_HAVE_EXPORT_DLL)
#  define SAL_RES_INIT_PROC_CFG_DEFINED(res_name, name, size, ucPriority, pfnTaskInit, pfnTaskBody, pszFileName)
# elif defined(SAL_HAVE_EXPORT_DLL)
#  define SAL_RES_INIT_PROC_CFG_DEFINED(res_name, name, size, ucPriority, pfnTaskInit, pfnTaskBody, pszFileName)    { (HI_PCSTR)# name, (HI_PVOID)HI_NULL, size, ucPriority, (HI_VOID_CALLBACK_F)0, (HI_VOID_CALLBACK_F)0, # pfnTaskInit, HI_NULL},
# else
#  define SAL_RES_INIT_PROC_CFG_DEFINED(res_name, name, size, ucPriority, pfnTaskInit, pfnTaskBody, pszFileName)    { (HI_PCSTR)# name, (HI_PVOID)HI_NULL, size, ucPriority, (HI_VOID_CALLBACK_F)pfnTaskInit, (HI_VOID_CALLBACK_F)0 },
# endif

#  define SAL_RES_TASK_CFG_VAR(res_name)   g_st ## res_name ## TasksCfg
#  define SAL_RES_Q_CFG_VAR(res_name)      g_st ## res_name ## QueuesCfg
#endif // SAL_HAVE_RES_TYPES


#ifndef SAL_HAVE_RES_TYPES
# undef SAL_RES_Q_DEFINED
#if defined(PRODUCT_CFG_OS_DYN_QUEUE_BUFFER)
# ifdef SAL_HAVE_Q_CFG_DECL
#  define SAL_RES_Q_DEFINED(res_name, name, size)     HI_PRV HI_SYS_QUEUE_MSG_S g_st ## res_name ## Queue##name[1];
# else
#  define SAL_RES_Q_DEFINED(res_name, name, size)     { (HI_SYS_QUEUE_MSG_S*)g_st ## res_name ## Queue ## name, (HI_PCSTR)# name, size},
# endif
#else
# ifdef SAL_HAVE_Q_CFG_DECL
#  define SAL_RES_Q_DEFINED(res_name, name, size)     HI_PRV HI_SYS_QUEUE_MSG_S g_st ## res_name ## Queue##name[size];
# else
#  define SAL_RES_Q_DEFINED(res_name, name, size)     { (HI_SYS_QUEUE_MSG_S*)g_st ## res_name ## Queue ## name, (HI_PCSTR)# name, size},
# endif
#endif 

#endif // SAL_HAVE_RES_TYPES

