//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : sal_dfx_sys_val_usr.h
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

//------------------------------------------------------------------
#if defined(SAL_DFX_SYS_VAL_DECLARE)

#if defined(PRODUCT_CFG_DFX_PLC_FRM_STAT)
HI_EXTERN HI_DSID_UPG_TM_S        g_stSysPlcFrmStat;
#endif
#if defined(PRODUCT_CFG_SYS_QRY_STAT)
HI_EXTERN HI_DSID_SYS_QRY_S       g_stStatSysQry;
#if defined(PRODUCT_CFG_SYS_QRY_HAVE_OS_TASK_INFO)
HI_EXTERN HI_DFX_OS_TASK_INFO_S   g_stStatSysOsQry[];
#endif
#endif
#if defined(PRODUCT_CFG_SYS_WD_STAT) 
HI_EXTERN HI_DSID_WD_INFO_S       g_stSysWdStat;
#endif
#if defined(PRODUCT_CFG_DFX_PLC_FRM_BLK_STAT)
HI_EXTERN HI_DSID_UPG_BLK_TM_S    g_astSysPlcFrmBlkStat[];
#endif
/* BEGIN: Added by b00208046, 2014/11/19   问题单号:DTS2014111902396:新增diagbuf统计量*/
HI_EXTERN HI_DIAG_BUF_STAT_S gDiagBufStat[];
/* END:   Added by b00208046, 2014/11/19 */
#if defined(PRODUCT_CFG_SYS_QRY_LOGMSG)
HI_EXTERN HI_DSID_LOGMSG_S    g_astLogMsgStat[];
#define DIAG_LOG_MSG_STAT_E(idx) do{\
    if(idx < HI_MOD_MAX_NUM)\
    {\
        g_astLogMsgStat[idx].id         = id;\
        g_astLogMsgStat[idx].usDataType = usDataType;\
        g_astLogMsgStat[idx].usRc       = (HI_U16)ret;\
        g_astLogMsgStat[idx].cnt[1]++;\
    }\
}while(0)
#define DIAG_LOG_MSG_STAT_S(idx) do{\
    if(idx < HI_MOD_MAX_NUM)\
    {\
        g_astLogMsgStat[idx].id         = id;\
        g_astLogMsgStat[idx].usDataType = usDataType;\
        g_astLogMsgStat[idx].cnt[0]++;\
    }\
}while(0)
#define DIAG_LOG_MSG_STAT_INIT(ulModId)\
    HI_U32 idx = HI_MOD_MAX_NUM;\
    if(HI_SAL_NVCFG_DIAG_CFG_CHK(HI_SAL_NVCFG_DIAG_SWT_CFG_USERPLANE_STAT, HI_FALSE))\
    {\
        if(ulModId)\
        for(idx = 0; idx < HI_MOD_MAX_NUM; idx++)\
        {\
            if(g_astLogMsgStat[idx].ulModId == 0) \
            {\
                g_astLogMsgStat[idx].ulModId = ulModId;\
                break;\
            }\
        }\
    }
#else // PRODUCT_CFG_SYS_QRY_LOGMSG
#define DIAG_LOG_MSG_STAT_INIT(ulModId)  do { } while(0)
#define DIAG_LOG_MSG_STAT_E(idx)         do { } while(0)
#define DIAG_LOG_MSG_STAT_S(idx)         do { } while(0)
#endif
//------------------------------------------------------------------
#else
#if defined(PRODUCT_CFG_DFX_PLC_FRM_STAT)
HI_DSID_UPG_TM_S        g_stSysPlcFrmStat;
#endif
#if defined(PRODUCT_CFG_DFX_PLC_FRM_BLK_STAT)
HI_DSID_UPG_BLK_TM_S    g_astSysPlcFrmBlkStat[HI_DSID_UPG_BLK_NUM_MAX];
#endif
#if defined(PRODUCT_CFG_SYS_QRY_STAT)
HI_DSID_SYS_QRY_S       g_stStatSysQry;
#if defined(PRODUCT_CFG_SYS_QRY_HAVE_OS_TASK_INFO)
HI_DFX_OS_TASK_INFO_S   g_stStatSysOsQry[HI_DFX_SYS_TASK_NUM_MAX];
#endif
#endif
#if defined(PRODUCT_CFG_SYS_WD_STAT) 
HI_DSID_WD_INFO_S       g_stSysWdStat;
#endif
#if defined(PRODUCT_CFG_SYS_QRY_LOGMSG) 
HI_DSID_LOGMSG_S       g_astLogMsgStat[HI_MOD_MAX_NUM];
#endif // SAL_DFX_SYS_VAL_DECLARE
//------------------------------------------------------------------

HI_CONST HI_DIAG_SYS_STAT_OBJ_S g_stSysStatObjTbl[] = {
#if defined(PRODUCT_CFG_DFX_PLC_FRM_STAT)
{HI_DSID_UPG_TM,      HI_SYS_DMID_BUSINESS_L0,   HI_DSID_TYPE_QRY,    1,                      (HI_U16)sizeof(HI_DSID_UPG_TM_S),     (HI_PVOID)&g_stSysPlcFrmStat},
#endif 
#if defined(PRODUCT_CFG_DFX_PLC_FRM_BLK_STAT)
{HI_DSID_UPG_BLK_TM,  HI_SYS_DMID_BUSINESS_L0,   HI_DSID_TYPE_QRY,    HI_DSID_UPG_BLK_NUM_MAX, (HI_U16)sizeof(HI_DSID_UPG_BLK_TM_S), (HI_PVOID)g_astSysPlcFrmBlkStat},
#endif 
#if defined(PRODUCT_CFG_SYS_QRY_STAT)
{HI_DSID_DIAG_CHL,  HI_SYS_DMID_BUSINESS_L0,     HI_DSID_TYPE_CUSTOM,  1, (HI_U16)sizeof(HI_DSID_SYS_QRY_S),  (HI_PVOID)&g_stStatSysQry},
#endif 
#if defined(PRODUCT_CFG_SYS_QRY_LOGMSG)
{HI_DSID_LOGMSG,  HI_SYS_DMID_BUSINESS_L0,       HI_DSID_TYPE_QRY,  HI_MOD_MAX_NUM, (HI_U16)sizeof(HI_DSID_LOGMSG_S),  (HI_PVOID)g_astLogMsgStat},
#endif 
#if defined(PRODUCT_CFG_SYS_WD_STAT) 
{HI_DSID_WD_INFO,   HI_SYS_DMID_BUSINESS_L0,     HI_DSID_TYPE_CUSTOM,  1, (HI_U16)sizeof(HI_DSID_WD_INFO_S),  (HI_PVOID)&g_stSysWdStat},
#endif
/* BEGIN: Added by b00208046, 2014/11/19   问题单号:DTS2014111902396:新增diagbuf统计量*/
{HI_DSID_DIAG_BUF_STAT,   HI_SYS_DMID_BUSINESS_L0,     HI_DSID_TYPE_CUSTOM,  EN_DIAG_BUFFER_ID_MAX, (HI_U16)sizeof(HI_DIAG_BUF_STAT_S),  (HI_PVOID)gDiagBufStat},
/* END:   Added by b00208046, 2014/11/19 */
// TODO... 
};

#if defined(PRODUCT_CFG_DFX_PLC_FRM_STAT)
#define SAL_SYS_INIT_PLC_FRM_STAT_VAL()  (hi_void)memset_s(&g_stSysPlcFrmStat, sizeof(HI_DSID_UPG_TM_S),0, sizeof(HI_DSID_UPG_TM_S))
#else
#define SAL_SYS_INIT_PLC_FRM_STAT_VAL() 
#endif


#if defined(PRODUCT_CFG_DFX_PLC_FRM_BLK_STAT)
#define SAL_SYS_INIT_PLC_FRM_BLK_STAT_VAL()   (hi_void)memset_s(g_astSysPlcFrmBlkStat, sizeof(HI_DSID_UPG_BLK_TM_S)*HI_DSID_UPG_BLK_NUM_MAX,0,sizeof(HI_DSID_UPG_BLK_TM_S)*HI_DSID_UPG_BLK_NUM_MAX)
#else
#define SAL_SYS_INIT_PLC_FRM_BLK_STAT_VAL() 
#endif 

#if defined(PRODUCT_CFG_SYS_QRY_STAT)
#define SAL_SYS_INIT_SYS_QRY_STAT_VAL() do {memset_s(&g_stStatSysQry,  sizeof(HI_DSID_SYS_QRY_S), 0,  sizeof(HI_DSID_SYS_QRY_S));} while(0) 
#else
#define SAL_SYS_INIT_SYS_QRY_STAT_VAL() do {} while(0) 
#endif

#if defined(PRODUCT_CFG_SYS_QRY_STAT) && defined(PRODUCT_CFG_SYS_QRY_HAVE_OS_TASK_INFO)
#define SAL_SYS_INIT_SYS_QRY_STAT_OSINFO_VAL() do {memset_s(&g_stStatSysOsQry,  HI_DFX_SYS_TASK_NUM_MAX*sizeof(HI_DFX_OS_TASK_INFO_S), 0,  HI_DFX_SYS_TASK_NUM_MAX*sizeof(HI_DFX_OS_TASK_INFO_S));} while(0) 
#else
#define SAL_SYS_INIT_SYS_QRY_STAT_OSINFO_VAL() do {} while(0) 
#endif

#if defined(PRODUCT_CFG_SYS_QRY_STAT) && defined(PRODUCT_CFG_SUPPORT_ASSERT)
#define SAL_SYS_INIT_CHK_LIMITATION()\
HI_ASSERT(HI_ARRAY_COUNT(g_stStatSysQry.stDiagChl.aulTxQBlkLeftCnt) >= EN_DIAG_BUFFER_ID_MAX);\
HI_ASSERT(HI_ARRAY_COUNT(g_stStatSysQry.stDiagChl.aulTxQBlkReqTooLargeCnt) >= EN_DIAG_BUFFER_ID_MAX);
#else
#define SAL_SYS_INIT_CHK_LIMITATION() 
#endif

#if defined(PRODUCT_CFG_SYS_QRY_LOGMSG)
#define SAL_SYS_INIT_SYS_QRY_STAT_LOGMSG_VAL() do {memset_s(&g_astLogMsgStat,  HI_MOD_MAX_NUM*sizeof(HI_DSID_LOGMSG_S), 0,  HI_MOD_MAX_NUM*sizeof(HI_DSID_LOGMSG_S));} while(0) 
#else
#define SAL_SYS_INIT_SYS_QRY_STAT_LOGMSG_VAL() do { } while(0) 
#endif

// Initialize G-STAT object entry.
#define SAL_SYS_INIT_USR_STAT_OBJ_TBL() \
do {\
    SAL_SYS_INIT_CHK_LIMITATION();\
    SAL_SYS_INIT_PLC_FRM_STAT_VAL();\
    SAL_SYS_INIT_PLC_FRM_BLK_STAT_VAL();\
    SAL_STAT_SYS_WD_ZERO();\
    SAL_SYS_INIT_SYS_QRY_STAT_VAL();\
    SAL_SYS_INIT_SYS_QRY_STAT_OSINFO_VAL();\
    SAL_SYS_INIT_SYS_QRY_STAT_LOGMSG_VAL();\
    HI_DIAG_RegisterSysStat(g_stSysStatObjTbl, HI_ARRAY_COUNT(g_stSysStatObjTbl));\
}while(0)


#endif

#else // SAL_DFX_SYS_VAL_DECLARE


#endif 


