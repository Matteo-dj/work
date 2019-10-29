//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : sal_dfx_sys_val.h
//  版 本 号   : V1.0 
//  作    者   : shenhankun/00130424
//  生成日期   : 2011-12-24
//  功能描述   : 系统级统计量定义文件
//               
//  函数列表   : 
//  修改历史   : 
//  1.日    期 : 2011-12-24
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

#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)     
#if defined(SAL_DFX_SYS_VAL_DECLARE)
HI_EXTERN HI_DSID_SAL_DUMP_KEY_MSG_S              g_stSalStatDumpKeyMsg;
HI_EXTERN HI_DSID_SAL_DEVIO_S                     g_stSalStatDevIo;
#ifdef PRODUCT_CFG_HAVE_FEATURE_SYS_ERR_INFO
#define g_pstStatSysModInit                       ((DIAG_CMD_SAL_SYS_SDM_S*)&(g_aucSysErrVal[DIAG_CMD_SDM_CTX_MAX_SIZE]))->astModInit
#endif
#define SAL_STAT_aulDumpKeyMsgFlashIoInc(x)       g_stSalStatDumpKeyMsg.aulDumpKeyMsgFlashIo[x]++;
#define SAL_STAT_aulDumpKeyMsgFlashIo(x,v)        g_stSalStatDumpKeyMsg.aulDumpKeyMsgFlashIo[x]=v;
#define SAL_STAT_aulDumpKeyMsgFlashIoIncV(x,v)    g_stSalStatDumpKeyMsg.aulDumpKeyMsgFlashIo[x]+=v; // HI_DIAG_UpdateSysStatCounter(&g_stSalStatDumpKeyMsg);
#define SAL_STAT_SetSalStatDumpKeyMsg(x,v)        g_stSalStatDumpKeyMsg.x = (v);
#define SAL_STAT_SetStatDevIo(x,v)                g_stSalStatDevIo.x = (v);
#define SAL_STAT_GetFlashSdramSpace(_total,_secsize)  _secsize = g_stSalStatDevIo.flash_sector_size/1024; \
                                                                  _total   = g_stSalStatDevIo.flash_total_size/1024/1024;
#ifdef PRODUCT_CFG_HAVE_FEATURE_SYS_ERR_INFO

#define SAL_SetStatSysModInit_S(id, sn, name)  do {\
    if(i < SYS_DFX_MOD_INIT_MAX)\
    {\
        g_pstStatSysModInit[id].ulTimsStamp[0] = HI_MDM_GetMilliSeconds();\
        g_pstStatSysModInit[id].ulTimsStamp[1] = 0xCCCCCCCC;\
        g_pstStatSysModInit[id].ucModId     = (HI_U8)id;\
        g_pstStatSysModInit[id].ucReserved  = (HI_U8)0xEE;\
        g_pstStatSysModInit[id].ucDropId    = (HI_U8)0xFF;\
        g_pstStatSysModInit[id].ucIdx       = (HI_U8)sn;\
        if(name) (hi_void)strncpy_s(g_pstStatSysModInit[id].szModId,HI_SYS_MOD_NAME_LEN, name, HI_SYS_MOD_NAME_LEN - 1);\
        else g_pstStatSysModInit[id].szModId[0] = 0;\
    }\
}while(0)

#define SAL_SetStatSysModInit_E(id)    do {\
    if(i < SYS_DFX_MOD_INIT_MAX)\
    {\
            g_pstStatSysModInit[id].ulTimsStamp[1] = HI_MDM_GetMilliSeconds();\
    }\
}while(0)

#define SAL_SetStatSysModDrop(id)  do {\
    if(i < SYS_DFX_MOD_INIT_MAX)\
    {\
       g_pstStatSysModInit[id].ucDropId = (HI_U8)id;\
    }\
}while(0)

#else
#define SAL_SetStatSysModInit_S(id, sn, name)  
#define SAL_SetStatSysModInit_E(id)      
#define SAL_SetStatSysModDrop(id)                 
#endif 
#else
// Stat. Define
HI_DSID_SAL_DUMP_KEY_MSG_S  g_stSalStatDumpKeyMsg = {HI_OBJ64_S_INIT_DEFINED, HI_DSID_SAL_DUMP_KEY_MSG_S_INIT_DEFINED1, {0,0,}};
HI_DSID_SAL_DEVIO_S         g_stSalStatDevIo      = {HI_OBJ64_S_INIT_DEFINED, HI_DSID_SAL_DEVIO_S_INIT_DEFINED1};

HI_CONST HI_DIAG_SYS_STAT_OBJ_S g_stSalStatObjTbl[] = {
{HI_DSID_SAL_DEVIO,           HI_SYS_DMID_BUSINESS_L0,   HI_DSID_TYPE_QRY,      1,                    (HI_U16)sizeof(g_stSalStatDevIo),         (HI_PVOID)&g_stSalStatDevIo},
#ifdef PRODUCT_CFG_HAVE_FEATURE_SYS_ERR_INFO
{HI_DSID_SYS_MOD_INIT,        HI_SYS_DMID_BUSINESS_L0,   HI_DSID_TYPE_CUSTOM,   SYS_DFX_MOD_INIT_MAX, (HI_U16)sizeof(HI_DSID_SYS_MOD_INIT_S),   (HI_PVOID)g_pstStatSysModInit},
#endif 
// TODO... 
};

// Initialize G-STAT object entry.
#define SAL_SYS_INIT_STAT_OBJ_TBL()\
{\
    HI_DIAG_RegisterSysStat(g_stSalStatObjTbl, HI_ARRAY_COUNT(g_stSalStatObjTbl));\
}

#endif 

#else
#define SAL_STAT_aulDumpKeyMsgFlashIoInc(x)       do { } while(0)
#define SAL_STAT_aulDumpKeyMsgFlashIo(x,v)        do { } while(0)
#define SAL_STAT_aulDumpKeyMsgFlashIoIncV(x,v)    do { } while(0)
#define SAL_STAT_SetSalStatDumpKeyMsg(x,v)        do { } while(0)
#define SAL_STAT_SetStatDevIo(x,v)                do { } while(0)
#endif

#include "sal_dfx_sys_val_usr.h"

