//*****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : sal_common.h
//  版 本 号   : V1.0
//  作    者   : 沈汉坤/00130424
//  生成日期   : 2011年09月06日
//  功能描述   : SAL模块内部接口文件
//  函数列表   : 无
//  修改历史   :
//  1.日    期 : 2011年09月06日
//    作    者 : 沈汉坤/00130424
//    修改内容 : 创建文件
//*****************************************************************************

#ifndef __SAL_COMMON_H__
#define __SAL_COMMON_H__

//****************************************************************************
#define SAL_EXCLUDE_RES_CFG_INF_HRD
#include <sal_inf.h>
#include <sal_inf_m.h>
/* BEGIN: Added by b00208046, 2014/11/19   问题单号:DTS2014111902396:新增diagbuf统计量*/
#include <sal_diag.h>
/* END:   Added by b00208046, 2014/11/19 */
#include <mac_inf.h>
#include <hi_sal_nv.h>
#include <hi_ndm.h>
#include <hi_driver.h>
#include <hi_flash.h>
#include <los_exc.h>
#include "mdm_inf.h"
//****************************************************************************

//****************************************************************************
#if defined(PRODUCT_CFG_OS_NU) && !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
#endif

#if defined(PRODUCT_CFG_OS_VX) && !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
#if defined(PRODUCT_CFG_FEATURE_POSIX)
// VX_TODO:
#endif
#if defined(PRODUCT_CFG_FEATURE_POSIX_SOCKET) // OS的头文件没有独立处理
// VX_TODO:
#endif
#endif

//****************************************************************************
#include <dfx_sal.h>
#include <dfx_sys.h>
//****************************************************************************


//****************************************************************************
#define SAL_IS_VALID_HANDLE(hHandle)  ((hHandle).data[0] == (HI_U32)HI_NULL ? HI_FALSE : HI_TRUE)

#define SAL_MODEM_RES_CFG_FILE  "os_res_cfg.h"
#define SC_RES_CFG_FILE         "os_scres_cfg.h"
//****************************************************************************

//****************************************************************************
#define SAL_ERROR_NOT_FIND_LOG0(sz)
#define SAL_ERROR_NOT_FIND_LOG1(s1)
#define SAL_WARNING_NOT_FIND_LOG0(sz)
#define SAL_WARNING_NOT_FIND_LOG1(s1)
#define SAL_INFO_LOG0(sz)
#define SAL_I_LOG1(sz, v1)
#define SAL_E_LOG0(sz)
#define SAL_E_LOG1(sz,v1)
#define SAL_I_LOG2(sz,v1,v2)
#define SAL_E_LOG2(sz,v1,v2)
//****************************************************************************




//****************************************************************************

//****************************************************************************
#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)

#if defined(PRODUCT_CFG_OS_NU) && defined(PRODUCT_CFG_OS_ADP_FUNC)
#include <sal_os_nu_func.h>
#elif defined(PRODUCT_CFG_OS_NU)
#include <sal_os_nu_macro.h>
#elif defined(PRODUCT_CFG_OS_VX) && defined(PRODUCT_CFG_OS_ADP_FUNC)
#include <os_vx_sal_api.h>
#elif defined(PRODUCT_CFG_OS_VX)
#include <sal_os_vx_macro.h>
#endif
#endif // PRODUCT_CFG_CORE_TYPE_BOOTLOADER
//****************************************************************************


//****************************************************************************
#define SAL_MEM_PADDING 4

#define HI_DIAG_PRINT_MOD_MASK    0x00FFFFFF
#define HI_DIAG_PRINT_LEVEL_MASK  0xFF000000

//****************************************************************************


//****************************************************************************
HI_U32 salOsResInit(HI_VOID);
HI_U32 salTaskInit(HI_VOID);
HI_U32 salCtxInit(HI_PVOID pFirstAvailableMemory,HI_U32 aulMemAdd[2], HI_U32 aulMemSize[2]);
HI_U32 salInitHardware(HI_VOID);
HI_U32 salInitPowwerOnMem(HI_PVOID pAddr);
HI_U32 salExitPowwerOnMem(HI_VOID);
HI_PVOID salPowerOnHeapAlloc(HI_U32 ulSize);
HI_U32 salPowerOnHeapFree(HI_PVOID pAddr);
HI_BOOL salGetPhase2Enable(HI_VOID);
HI_BOOL salSetPhase2Enable(HI_BOOL bEnable);
HI_U32 salInitTimer(HI_VOID);
HI_VOID InitTestCfg(HI_VOID);
HI_U32 salStartScTasks(HI_VOID);
HI_U32 salInitSysPhase1(HI_U32 ulOption);
HI_U32 salInitComplete(HI_VOID);
HI_U32 dmsPowerOn(HI_IN HI_OUT HI_PVOID pDevInfo, HI_U32 ulOption);
HI_VOID salInitDfxCtx(HI_VOID);
HI_U32 salSetRstTimes(HI_VOID);
HI_VOID salDfxInit(HI_VOID);
HI_RST_USER_REQ_HANDLE_F* GetSalResetUserReqHandler(HI_VOID);
HI_BOOL SalIsCco(HI_VOID);
HI_BOOL SalIsSta(HI_VOID);
HI_VOID salSetSysStatus(HI_U8 ucSysStatus);
HI_U32 sal_GetFlashInfo(HI_FLASH_INFO_S *pstFlashInfo);
//****************************************************************************


//****************************************************************************
#define SAL_CHECK_IN_RANGE(val, limt)        if(limt <= val) {return HI_ERR_INVALID_PARAMETER;}
#define SAL_CHECK_IN_RANGE_BOOL(val, limt)   if(limt <= val) {return HI_FALSE;}
#define SAL_CHECK_IN_RANGE_PTR(val, limt)    if(limt <= val) {return HI_NULL;}

#define SAL_CHECK_MD_SEM_IN_RANGE() \
        if((pCtx->stMdResSem.ulCnt == 0) || (pCtx->stMdResSem.paHd == 0) )\
        {\
            return HI_ERR_NO_OS_RES_INIT;\
        }\
        if(ulSemId >= pCtx->stMdResSem.ulCnt)\
        {\
            return HI_ERR_INVALID_PARAMETER;\
        }

#define SAL_CHECK_APP_SEM_IN_RANGE() \
        if((pCtx->stAppResSem.ulCnt == 0) || (pCtx->stAppResSem.paHd == 0) )\
        {\
            return HI_ERR_NO_INIT_APP_OS_RES;\
        }\
        if(ulSemId >= pCtx->stAppResSem.ulCnt)\
        {\
            return HI_ERR_INVALID_PARAMETER;\
        }
//****************************************************************************

typedef struct
{
    HI_U32 ulAddress;
    HI_U32 ulPoolSize;
    HI_PCSTR pszName;
} MEM_POOL_ITEM_STRU;


//****************************************************************************
typedef struct
{
    HI_CHAR szVerMagic[60];
    HI_U32 ulMajorMinorVersion; // 主版本号.次版本号
    HI_U32 ulRevisionVersion;   // 修正版本号
    HI_U32 ulBuildVersion;      // 内部版本号
    HI_CHAR* pszBuildDate;
    HI_CHAR* pszBuildTime;
} UE_SOFT_PRIVATE_VER_STRU;

#define MSP_VER_MAGIC "MspMajorMinorRevisionVersionMagicABCD^#12"
#define MSP_LIB_VER(MajorVersion, MinorVersion, RevisionVersion) ((((HI_U16)(MajorVersion))<<16)|((HI_U16)MinorVersion)), (RevisionVersion)
//****************************************************************************

//****************************************************************************
//
// 消息定义
//
#define ID_MSG_DIAG_BASE               0x100

enum
{
    ID_MSG_DIAG_CHLMGR_TX = (ID_MSG_DIAG_BASE +1),// 通道管理命令
    ID_MSG_DIAG_DATA_TX,      // 数据包上报
    ID_MSG_DIAG_DATA_USER_TX, // 用户数据包上报
    ID_MSG_DIAG_SYS_STAT_DATA_TX,
    ID_MSG_DIAG_SYS_EVT_DATA_TX,
    ID_MSG_DIAG_ACK_RX,
    ID_MSG_DIAG_ACK_USER_RX,
    ID_MSG_DIAG_ACK_TX,       // ACK包，需要释放参数指针
    ID_MSG_DIAG_RELAY_FRAME,  // 转发HSO帧
    ID_MSG_DIAG_RX,
    ID_MSG_DIAG_LOCAL_DATA_DUMP,      // DIAG Flash数据包DUMP到HSO
    ID_MSG_DIAG_LOCAL_DATA_SAVE,       // DIAG数据包保存到Flash
    ID_MSG_DIAG_USERPLANE_GDATA_TX,
    ID_MSG_DIAG_USERPLANE_DDATA_TX,
    ID_MSG_DIAG_USERPLANE_DATA64_TX,
    ID_MSG_DIAG_ACK_MDM_RX
};
//****************************************************************************

//****************************************************************************
#define DMS_FRAME_PADDING_SIZE               (0) // 填充字
#define DMS_MAC_FRAME_PAYLOAD_SIZE_MAX       (HI_DMS_FRM_MAX_PAYLOAD_SIZE)

typedef enum
{
    SYM_SYSMODE_NORMAL = 0,
    SYM_CORE_CHILD_ONLINE,
    SYM_CORE_CHILD_UPDATE
} SYM_CORE_CHILD_MODE_ENUM;

#define diag_SetPacketsLostInfo(x)
//****************************************************************************

//****************************************************************************
#define DIAG_MSG_FREE(pstMsg)               HI_MDM_Free(HI_MOD_ID_SAL_DIAG,(HI_PVOID)((pstMsg)->ulParam[0]))
#define DIAG_MSG_FREE0(pstMsg)              HI_MDM_Free(HI_MOD_ID_SAL_DIAG,(HI_PVOID)((pstMsg)->ulParam[0]))
#define DIAG_MSG_FREE1(pstMsg)              HI_MDM_Free(HI_MOD_ID_SAL_DIAG,(HI_PVOID)((pstMsg)->ulParam[1]))
#define DIAG_MSG_FREE2(pstMsg)              HI_MDM_Free(HI_MOD_ID_SAL_DIAG,(HI_PVOID)((pstMsg)->ulParam[2]))

#define DIAG_BUFFERING_MSG_FREE(pstMsg)     diagFreeDiagBuffer((DIAG_BUFFER_ID_ENUM)HI_LOU16((pstMsg)->ulParam[3]), (HI_PVOID)((pstMsg)->ulParam[0]))
//****************************************************************************

//****************************************************************************
HI_U32 salNvAppInit(HI_VOID);
HI_U32 salDiagAppInit(HI_VOID);
#if defined(PRODUCT_CFG_FEATURE_ST) || defined(PRODUCT_CFG_OS_WIN)
HI_U32 salAppTestInit(HI_VOID);
#endif
//****************************************************************************

//****************************************************************************
// 相关宏定义(入参检查、非法定义等), 方便阅读和理解代码，而集中定义的宏
#define SAL_CHECK_NULL_PTR_RET_ERR(p)\
    if(HI_NULL == p)\
    {\
        return HI_ERR_INVALID_PARAMETER;\
    }

#define SAL_CHECK_BAD_PTR_RET_ERR(p)\
    if((HI_U32)(p) < (HI_U32)HI_ADDR_USER_RAM_START)\
    {\
        return HI_ERR_INVALID_PARAMETER;\
    }

#define SAL_CHECK_BAD_PTR_RET_ZERO(p)\
    if((HI_U32)(p) < (HI_U32)HI_ADDR_USER_RAM_START)\
    {\
        return 0;\
    }

#define SAL_OS_RES_IS_INIT(pCtx) (pCtx->bPowerOnProcess || (!pCtx->bOsResInit))

#define SAL_CHECK_INIT_STATUS(pCtx) if(pCtx->bPowerOnProcess)\
    {\
        return HI_ERR_POWER_ON_PROCESS;\
    }\
    \
    if(!pCtx->bOsResInit)\
    {\
        return HI_ERR_NO_OS_RES_INIT;\
    }

#define SAL_CHECK_APP_QUEUE_STATUS_RET(pResQ, ulQueueId, ret) \
        if((pResQ->ulCnt == 0) || (pResQ->paHd == 0) || (pResQ->pastCfg == 0))\
        {\
            ret = HI_ERR_NO_INIT_APP_OS_RES;\
        }\
        else if(ulQueueId >= pResQ->ulCnt)\
        {\
            ret = HI_ERR_INVALID_PARAMETER;\
        }


#define SAL_CHECK_MODEM_QUEUE_PARAM_PTR(ulQueueId) \
        if((SAL_QUEUE_ID_ENUM)ulQueueId >= EN_SAL_Q_MAX)\
        {\
            goto __api_rtn_end;\
        }
//****************************************************************************


//****************************************************************************
// Helper routine
#define INIT_EXCEPTION_POLLING_WAIT()  HI_MDM_Sleep(HI_DMS_CHL_EXCEPTION_POLLING_WAIT)
#define UART_EXCEPTION_POLLING_WAIT()  HI_MDM_Sleep(HI_DMS_UART_EXCEPTION_POLLING_WAIT)
//****************************************************************************

//****************************************************************************

typedef struct
{
    HI_DBG_MEM_SUM_STAT_IND_S stSum;
    HI_DBG_MEM_MOD_STAT_IND_S stModItem[HI_MOD_MAX_NUM];
} SAL_DBG_MEM_STAT_S;


typedef struct
{
    HI_U16 usTotalItems;    // 实际待传输的总个数, 最大限制为 HI_BIDB_ITEM_NUM_MAX
    HI_U16 usTransmitItems; // 当前传输累积个数
    HI_U32 bImproting;      // HI_TRUE 表示正在导入
} SAL_DFX_BIDM_INFO_S;

typedef struct
{
    HI_U32 ulData[32]; // reserved.
    HI_SAL_DFX_HOST_CTRL_S stHostCtl;
    HI_U32 pfnTbl[4];

    HI_CHAR szOsVer[40];
    HI_CHAR szSalVer[40];
    HI_CHAR szDrvVer[40];
    HI_CHAR szPhyVer[40];
    HI_CHAR szMacVer[40];
    HI_CHAR szAppVer[40];
    HI_CHAR szBootVer[64];

#if defined(PRODUCT_CFG_FEATURE_MEM_STAT) && !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
    SAL_DBG_MEM_STAT_S stMemStat;
#endif

#if defined(PRODUCT_CFG_FEATURE_NDM)
    SAL_DFX_BIDM_INFO_S stBidm;
#endif
} HI_SAL_DFX_HOST_S;
//****************************************************************************

//****************************************************************************
#if defined(PRODUCT_CFG_FEATURE_MEM_STAT) && !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
HI_VOID salDfxSetMalloc(HI_U32 ulModId, HI_U32 ulSize, HI_PVOID pMemAllocResult);
HI_VOID salDfxSetStcRam(HI_U16, HI_U32);
HI_VOID salDfxSetFree(HI_U32 ulModId, HI_U32 ulSize, HI_U32 ret);
HI_BOOL salCheckModStcRamFail(HI_U16 usModId);
#if defined(PRODUCT_CFG_FEATURE_MEM_STAT_CHECK_AVAIL_SIZE)
HI_BOOL salDfxChkMemAvail(HI_U32 ulSize, HI_BOOL bStatMem);
#endif
#else
#define salDfxSetMalloc(ulModId, ulSize, pMemAllocResult)  {}
#define salDfxSetStcRam(ulModId, ulSize)                   {}
#define salDfxSetFree(ulModId, ulSize, ret)                {}
#define salCheckModStcRamFail(usModId)                     HI_FALSE
#define salDfxChkMemAvail(ulSize, bStatMem)                HI_TRUE
#endif
//****************************************************************************

//****************************************************************************
HI_SAL_DFX_HOST_S* sal_DfxGetHostCtx(HI_VOID);
#define HOST_CTRL_FN1()   (sal_DfxGetHostCtx())->pfnTbl[0]
#define HOST_CTRL_FN2()   (sal_DfxGetHostCtx())->pfnTbl[1]
#define HOST_CTRL_FN3()   (sal_DfxGetHostCtx())->pfnTbl[2]
#define HOST_CTRL_FN4()   (sal_DfxGetHostCtx())->pfnTbl[3]
//****************************************************************************

//****************************************************************************
#if defined(PRODUCT_CFG_AUTO_RESET_SYSTEM_TIMEOUT)
#define SAL_CHK_WD_ACTIVE()     (g_ulAutoResetSystemTimeOutCfg > 1)
#define SAL_CHK_WD_ACTIVE_TO    (g_ulAutoResetSystemTimeOutCfg)
#endif

#define SDM_MAIN_TASK_INIT_KLG_WD_STAT()  do { \
    SAL_STAT_SYS_WD_SET(ulTimeOutCfg, SAL_CHK_WD_ACTIVE_TO);\
    if(CHECK_WD_TIMEROUT_ENABLE())\
    { \
        SET_g_stSdmSklgInit_m(wdg_enable,1); \
        SET_g_stSdmSklgInfo_m(ulWdEnable,1);\
    }\
	else \
	{\
	    SET_g_stSdmSklgInit_m(wdg_enable,0); \
        SET_g_stSdmSklgInfo_m(ulWdEnable,0);\
	}\
}while(0)

#define WD_TIMEOUT_MIN              5 // ms

#define CHECK_WD_TIMEROUT_ENABLE()  (SAL_CHK_WD_ACTIVE_TO > WD_TIMEOUT_MIN)

#if defined(PRODUCT_CFG_PLATFORM_HI3911)
#define SAL_REST() if(SAL_CHK_WD_ACTIVE()) HI_SYS_RebootPrv(HI_SYS_REBOOT_CAUSE_UNKNOWN)
#else
#define SAL_REST()
#endif
//****************************************************************************

//****************************************************************************
#if defined(PRODUCT_CFG_FEATURE_MEM_STAT) && !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
SAL_DBG_MEM_STAT_S* sal_DfxGetMemStatCtx(HI_VOID);
#endif

#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
DIAG_CMD_SAL_SYS_SDM_S* sal_GetSalSysSdm(HI_VOID);
HI_SYS_EVT_SDM_S* sal_GetSysEvtSdm(HI_VOID);
DIAG_CMD_SAL_DIAG_SDM_S* sal_GetSalDiagSdm(HI_VOID);
#endif
#define SAL_DIAG_SDM_PTR ((DIAG_CMD_SAL_DIAG_SDM_S*)sal_GetSalDiagSdm())
//****************************************************************************

//****************************************************************************
// 公用宏定义
#define SAL_INC_U32(x)                                   (x)++
#define SAL_INC_U16(x)                                   (x)++
#define SAL_DEC_U(x)                                     do { if((x)>0) {(x)--;} } while(0)
//****************************************************************************

//****************************************************************************
// DIAG SDM统计量
 // 添加统计量
#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
#define SAL_SET_SAL_DIAG_SDM_U32(m,v)                      do { SAL_DIAG_SDM_PTR->m = v;   } while(0)
#define SAL_SET_SAL_DIAG_SDM_TIME(m)                       do { SAL_DIAG_SDM_PTR->m = HI_MDM_GetMilliSeconds();   } while(0)
#define SAL_SET_SAL_DIAG_SDM_U32_INC(m)                    do { SAL_INC_U32(SAL_DIAG_SDM_PTR->m);  } while(0)
#define SAL_SET_SAL_DIAG_SDM_U32_INC2(m,_ret)              do { _ret = SAL_INC_U32(SAL_DIAG_SDM_PTR->m);  } while(0)
#define SAL_SET_SAL_DIAG_SDM_U32_DEC(m)                    do { if(SAL_DIAG_SDM_PTR->m) SAL_DIAG_SDM_PTR->m--; } while(0)
#define SAL_SET_SAL_DIAG_SDM_U32_INC_M(m,v)                do { SAL_DIAG_SDM_PTR->m += v;  } while(0)
#define SAL_SET_SAL_DIAG_SDM_U32_INC_RET(m, _ret)          do { if(_ret == HI_ERR_SUCCESS) { SAL_INC_U32(SAL_DIAG_SDM_PTR->m[0]); } else { SAL_INC_U32(SAL_DIAG_SDM_PTR->m[1]); }  } while(0)
#define SAL_SET_SAL_DIAG_SDM_U32_FAIL_INC(m1, m2, _ret)    do { SAL_SET_SAL_DIAG_SDM_U32_INC(m1); SAL_SET_SAL_DIAG_SDM_U32(m2, ret); } while(0)
#define SAL_SET_SAL_DIAG_SDM_U32_BIT(m,n)                  do { HI_SET_BIT_I(SAL_DIAG_SDM_PTR->m, n);  } while(0)
#define SAL_SET_SAL_DIAG_SDM_U32_TSK_DROP(n)               SAL_SET_SAL_DIAG_SDM_U32_BIT(aulDropTaskCnt[2],n)
#else
#define SAL_SET_SAL_DIAG_SDM_U32(m,v)                      do { } while(0)
#define SAL_SET_SAL_DIAG_SDM_TIME(m)                       do { } while(0)
#define SAL_SET_SAL_DIAG_SDM_U32_INC(m)                    do { } while(0)
#define SAL_SET_SAL_DIAG_SDM_U32_INC2(m,ret)               do { } while(0)
#define SAL_SET_SAL_DIAG_SDM_U32_DEC(m)                    do { } while(0)
#define SAL_SET_SAL_DIAG_SDM_U32_INC_M(m,v)                do { } while(0)
#define SAL_SET_SAL_DIAG_SDM_U32_INC_RET(m, _ret)          do { } while(0)
#define SAL_SET_SAL_DIAG_SDM_U32_FAIL_INC(m1, m2, _ret)    do { } while(0)
#define SAL_SET_SAL_DIAG_SDM_U32_BIT(m1, m2, _ret)         do { } while(0)
#define SAL_SET_SAL_DIAG_SDM_U32_TSK_DROP(n)               do { } while(0)
#endif
//****************************************************************************

//****************************************************************************
// 系统状态查询统计量
#if defined(PRODUCT_CFG_SYS_QRY_STAT) && !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
#define SAL_INC_STAT_SYS_QRY_U32_RET(ret,st,m)            do { if(ret){ SAL_INC_U32(g_stStatSysQry.st.m[1]);} else {SAL_INC_U32(g_stStatSysQry.st.m[0]);} }while(0)
#define SAL_INC_STAT_SYS_QRY_U32(st,m)                    do { SAL_INC_U32(g_stStatSysQry.st.m);} while(0)
#define SAL_INC_STAT_SYS_QRY_U16(st,m)                    do { SAL_INC_U16(g_stStatSysQry.st.m);} while(0)
#define SAL_INC_STAT_SYS_QRY_SET_LAST_ERR(st,m,ret)       do { g_stStatSysQry.st.m = ret; } while(0)
#define SAL_SET_STAT_SYS_QRY_U32(st,m,v)                  do { g_stStatSysQry.st.m = v;   } while(0)
#define SAL_SET_STAT_SYS_QRY_U32_INC(st,m,v)              do { g_stStatSysQry.st.m += v;  } while(0)
#define SAL_SET_STAT_SYS_QRY_U32_INC1(st,m)               do { SAL_INC_U32(g_stStatSysQry.st.m);  } while(0)
#define SAL_SET_STAT_SYS_QRY_ZERO(st)                     do {memset_s(&g_stStatSysQry.st, sizeof(g_stStatSysQry.st), 0, sizeof(g_stStatSysQry.st)); } while(0)
#define SAL_SET_STAT_SYS_QRY_ZERO_M(st, m)                do {memset_s(&g_stStatSysQry.st.m,sizeof(g_stStatSysQry.st.m), 0,sizeof(g_stStatSysQry.st.m)); } while(0)
#define SAL_SET_SATT_SYS_QRY_MQ_CNT_W(id)                 do {if(id < HI_DFX_SYS_MODEM_Q_NUM_MAX) {SAL_INC_U32(g_stStatSysQry.stOsRes.aulMQNumW[id]);}}while(0)
#define SAL_SET_SATT_SYS_QRY_AQ_CNT_W(id)                 do {if(id < HI_DFX_SYS_APP_Q_NUM_MAX)   {SAL_INC_U32(g_stStatSysQry.stOsRes.aulAQNumW[id]);}}while(0)
#define SAL_SET_SATT_SYS_QRY_Q_CNT_W(id,app)              do {if(app){SAL_SET_SATT_SYS_QRY_AQ_CNT_W(id);} else {SAL_SET_SATT_SYS_QRY_MQ_CNT_W(id);}}while(0)
#define SAL_SET_SATT_SYS_QRY_MQ_CNT_R(id)                 do {if(id < HI_DFX_SYS_MODEM_Q_NUM_MAX) {SAL_INC_U32(g_stStatSysQry.stOsRes.aulMQNumR[id]);}}while(0)
#define SAL_SET_SATT_SYS_QRY_AQ_CNT_R(id)                 do {if(id < HI_DFX_SYS_APP_Q_NUM_MAX)   {SAL_INC_U32(g_stStatSysQry.stOsRes.aulAQNumR[id]);}}while(0)
#define SAL_SET_SATT_SYS_QRY_Q_CNT_R(id,app)              do {if(app){SAL_SET_SATT_SYS_QRY_AQ_CNT_R(id);} else {SAL_SET_SATT_SYS_QRY_MQ_CNT_R(id);}}while(0)
#define SAL_SET_SATT_SYS_QRY_ME_CNT_W(id)                 do {if(id < HI_DFX_SYS_MODEM_E_NUM_MAX) {SAL_INC_U32(g_stStatSysQry.stOsRes.aulMENumW[id]);}}while(0)
#define SAL_SET_SATT_SYS_QRY_ME_CNT_R(id)                 do {if(id < HI_DFX_SYS_MODEM_E_NUM_MAX) {SAL_INC_U32(g_stStatSysQry.stOsRes.aulMENumR[id]);}}while(0)
#define SAL_SET_SATT_SYS_QRY_AE_CNT_W(id)                 do { }while(0)
#define SAL_SET_SATT_SYS_QRY_AE_CNT_R(id)                 do { }while(0)
#define SAL_SET_SATT_SYS_QRY_E_CNT_W(id,app)              do {if(app){SAL_SET_SATT_SYS_QRY_AE_CNT_W(id);} else {SAL_SET_SATT_SYS_QRY_ME_CNT_W(id);}}while(0)
#define SAL_SET_SATT_SYS_QRY_E_CNT_R(id,app)              do {if(app){SAL_SET_SATT_SYS_QRY_AE_CNT_R(id);} else {SAL_SET_SATT_SYS_QRY_ME_CNT_R(id);}}while(0)
#define SAL_SET_SATT_SYS_QRY_OS_RES_OK_ERR(ret,m,of1,of2) do {if(ret==HI_ERR_SUCCESS) {SAL_INC_STAT_SYS_QRY_U32(stOsRes,m[of1]);} else {SAL_INC_STAT_SYS_QRY_U32(stOsRes,m[of2]);}}while(0)
#define SAL_SET_SATT_SYS_QRY_RES_OK_ERR(ret,st,m,of1,of2) do {if(ret==HI_ERR_SUCCESS) {SAL_INC_STAT_SYS_QRY_U32(st,m[of1]);} else {SAL_INC_STAT_SYS_QRY_U32(st,m[of2]);}}while(0)

#else
#define SAL_INC_STAT_SYS_QRY_U32_RET(ret,st,m)            do {  } while(0)
#define SAL_INC_STAT_SYS_QRY_U32(st,m)                    do {  } while(0)
#define SAL_INC_STAT_SYS_QRY_U16(st,m)                    do {  } while(0)
#define SAL_INC_STAT_SYS_QRY_SET_LAST_ERR(st,m,ret)       do {  } while(0)
#define SAL_SET_STAT_SYS_QRY_U32(st,m,v)                  do {  } while(0)
#define SAL_SET_STAT_SYS_QRY_U32_INC(st,m,v)              do {  } while(0)
#define SAL_SET_STAT_SYS_QRY_U32_INC1(st,m)               do {  } while(0)
#define SAL_SET_STAT_SYS_QRY_ZERO(st)                     do {  } while(0)
#define SAL_SET_STAT_SYS_QRY_ZERO_M(st, m)                do {  } while(0)
#define SAL_SET_SATT_SYS_QRY_MQ_CNT_W(id)                 do {  } while(0)
#define SAL_SET_SATT_SYS_QRY_AQ_CNT_W(id)                 do {  } while(0)
#define SAL_SET_SATT_SYS_QRY_Q_CNT_W(id,app)              do {  } while(0)
#define SAL_SET_SATT_SYS_QRY_MQ_CNT_R(id)                 do {  } while(0)
#define SAL_SET_SATT_SYS_QRY_AQ_CNT_R(id)                 do {  } while(0)
#define SAL_SET_SATT_SYS_QRY_Q_CNT_R(id,app)              do {  } while(0)
#define SAL_SET_SATT_SYS_QRY_ME_CNT_W(id)                 do {  } while(0)
#define SAL_SET_SATT_SYS_QRY_ME_CNT_R(id)                 do {  } while(0)
#define SAL_SET_SATT_SYS_QRY_AE_CNT_W(id)                 do {  } while(0)
#define SAL_SET_SATT_SYS_QRY_AE_CNT_R(id)                 do {  } while(0)
#define SAL_SET_SATT_SYS_QRY_E_CNT_W(id,app)              do {  } while(0)
#define SAL_SET_SATT_SYS_QRY_E_CNT_R(id,app)              do {  } while(0)
#define SAL_SET_SATT_SYS_QRY_OS_RES_OK_ERR(ret,m,of1,of2) do {  } while(0)
#define SAL_SET_SATT_SYS_QRY_RES_OK_ERR(ret,st,m,of1,of2) do {  } while(0)
#endif
//****************************************************************************

//****************************************************************************
// WD 统计量 宏定义
 // 添加统计量
#if defined(PRODUCT_CFG_SYS_WD_STAT) && !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
#define SAL_STAT_SYS_WD_SET(m,v)           do { g_stSysWdStat.m = v; } while(0)
#define SAL_STAT_SYS_WD_ZERO()             do { (hi_void)memset_s(&g_stSysWdStat, sizeof(HI_DSID_WD_INFO_S), 0, sizeof(HI_DSID_WD_INFO_S));} while(0)
#define SAL_STAT_SYS_WD_INC(m)             do { SAL_INC_U32(g_stSysWdStat.m);}while(0)
#define SAL_STAT_SYS_WD_INC_TIME(x)        do { SAL_STAT_SYS_WD_INC(x[0]); SAL_STAT_SYS_WD_SET(x[1],  HI_MDM_GetMilliSeconds()); }while(0)
#else
#define SAL_STAT_SYS_WD_SET(m,v)           do {  } while(0)
#define SAL_STAT_SYS_WD_ZERO()             do {  } while(0)
#define SAL_STAT_SYS_WD_INC(m)             do {  } while(0)
#define SAL_STAT_SYS_WD_INC_TIME(x)        do {  } while(0)
#endif
//****************************************************************************

//****************************************************************************
#define SDM_DMS_ID_NV_INIT                         0
#define SDM_DMS_ID_TCP_D_FRM_CNT                   1
#define SDM_DMS_ID_UART_D_FRM_CNT                  2
// TODO
#define SDM_DMS_ID_4                               4

#define SDM_DMS_ID_TCP_CHL                         5
#define SDM_DMS_ID_TCP_RCV_STATUS                  6
#define SDM_DMS_ID_TCP_CHL_ACCEPT                  7
#define SDM_DMS_ID_TCP_RCV_DATA                    8
#define SDM_DMS_ID_TCP_SND_STATUS                  9

#define SDM_DMS_ID_SELECT_RET                      10
#define SDM_DMS_ID_NOTHING_MONITOR                 11
#define SDM_DMS_ID_SELECT_INPUT1                   12
#define SDM_DMS_ID_SELECT_INPUT2                   13
#define SDM_DMS_ID_TCP_CHL_ACCPET_RET              14
#define SDM_DMS_ID_TC_SCK_CLOSE_RET1               15
#define SDM_DMS_ID_TC_SCK_CLOSE_RET2               16
#define SDM_DMS_ID_TC_SCK_CLOSE_RET3               17
#define SDM_DMS_ID_TC_SCK_CLOSE_RET4               18
#define SDM_DMS_ID_TC_SCK_CLOSE_RET0               19
#define SDM_DMS_ID_TCP_SND_TIME_MAX_CHK_S          20
#define SDM_DMS_ID_TCP_SND_TIME_MAX_CHK            21

#define SDM_DMS_ID_T_C_CNT_B                       22
#define SDM_DMS_ID_T_C_CNT_A                       23
//****************************************************************************

//****************************************************************************
// 初始化过程中, 关键阶段(成功和失败)的统计
#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER) && defined(PRODUCT_CFG_HAVE_FEATURE_SYS_ERR_INFO)
#define SAL_SDM_SET_FAIL_ID(offset)             sal_GetSalSysSdm()->aucFailId[offset]       = 1
#define SAL_SDM_SET_FAIL_ID_RET(ret,offset)     if(ret) sal_GetSalSysSdm()->aucFailId[offset] = 1
#define SAL_SDM_SET_FAIL_ID_ZERO(offset)        sal_GetSalSysSdm()->aucFailId[offset]       = 0
#define SAL_SDM_SET_FAIL_ID_CNT(offset)         sal_GetSalSysSdm()->aucFailId[offset]++
#define SAL_SDM_SET_DIAG_ID(offset, val)        sal_GetSalSysSdm()->aulDiag[offset] = val
#define SAL_SDM_SET_DMS_ID(offset, val)         sal_GetSalSysSdm()->aulDms[offset] = (HI_U32)val
#define SAL_SDM_SET_DMS_ID_INC(offset)          SAL_INC_U32(sal_GetSalSysSdm()->aulDms[offset])
#define SAL_SDM_SET_DMS_ID_PLUS(offset, val)    sal_GetSalSysSdm()->aulDms[offset] += val
#define SAL_SDM_SET_DIAG_ID_PLUS(offset, val)   sal_GetSalSysSdm()->aulDiag[offset] += val
#define SAL_SDM_SET_ERR_ID(offset, val)         sal_GetSalSysSdm()->aulErr[offset] = (HI_U32)val
#define SAL_SDM_SET_DMS_TIME_ZERO(offset)       sal_GetSalSysSdm()->ulOutcomingTrafficTime[offset] = 0;
#define SAL_SDM_SET_DMS_TIME(offset)            sal_GetSalSysSdm()->ulOutcomingTrafficTime[offset] = HI_MDM_GetMilliSeconds();
#define SAL_SDM_SET_VAL(m, val)                 sal_GetSalSysSdm()->m = val
#define SAL_SDM_SET_SYS_TIME(m)                 do { sal_GetSalSysSdm()->m = HI_MDM_GetMilliSeconds(); } while(0)
#define SAL_SDM_SET_SYS_SET(m,v)                do { sal_GetSalSysSdm()->m = v; } while(0)
#define SAL_SDM_SET_SYS_INC(m)                  do { SAL_INC_U32(sal_GetSalSysSdm()->m);}while(0)
#define SAL_SDM_SET_SYS_INC_TIME(x)             do { SAL_SDM_SET_SYS_INC(x[0]); SAL_SDM_SET_SYS_SET(x[1],  HI_MDM_GetMilliSeconds()); }while(0)
#define SAL_SDM_SET_SYS_INC_RET(ret,m,o1,o2)    do { if(ret == HI_ERR_SUCCESS) SAL_SDM_SET_SYS_INC(m[o1]); else SAL_SDM_SET_SYS_INC(m[o2]);}while(0)
#else
#define SAL_SDM_SET_FAIL_ID(offset)            do {} while(0)
#define SAL_SDM_SET_FAIL_ID_RET(ret,offset)    do {} while(0)
#define SAL_SDM_SET_FAIL_ID_ZERO(offset)       do {} while(0)
#define SAL_SDM_SET_FAIL_ID_CNT(offset)        do {} while(0)
#define SAL_SDM_SET_MOD_INIT_ID(offset, val)   do {} while(0)
#define SAL_SDM_SET_DIAG_ID(offset, val)       do {} while(0)
#define SAL_SDM_SET_DMS_ID(offset, val)        do {} while(0)
#define SAL_SDM_SET_DMS_ID_INC(offset)         do {} while(0)
#define SAL_SDM_SET_DMS_ID_PLUS(offset, val)   do {} while(0)
#define SAL_SDM_SET_DIAG_ID_PLUS(offset, val)  do {} while(0)
#define SAL_SDM_SET_ERR_ID(offset, val)        do {} while(0)
#define SAL_SDM_SET_DMS_TIME_ZERO(offset)      do {} while(0)
#define SAL_SDM_SET_DMS_TIME(offset)           do {} while(0)
#define SAL_SDM_SET_VAL(m, val)                do {} while(0)
#define SAL_SDM_SET_SYS_TIME(m)                do {} while(0)
#define SAL_SDM_SET_SYS_SET(m,v)               do {} while(0)
#define SAL_SDM_SET_SYS_INC(m)                 do {} while(0)
#define SAL_SDM_SET_SYS_INC_TIME(x)            do {} while(0)
#define SAL_SDM_SET_SYS_INC_RET(ret,m,o1,o2)   do {} while(0)
#endif
//****************************************************************************

//****************************************************************************
HI_U32 sal_NvFlashErase(HI_U32 ulSectorSn, HI_U32 ulNum);
HI_U32 sal_NvFlashWrite(HI_CONST HI_U8* pAddress, HI_U32 ulSize, HI_U32 * pWrittensize, HI_CONST HI_CHAR* pBuffer);
HI_U32 sal_FlashEraseOneSector(HI_U32 ulAddr);
//****************************************************************************

//****************************************************************************
typedef struct
{
    HI_U32 ulAddr;           // Flash的起始地址
    HI_U32 ulStartSectorId;  // 起始块编号
    HI_U32 ulSectorSize;     // flash的一个sector的大小
    HI_U32 ulSectorNum;      // flash的sector的数量
    HI_U32 ulPageSize;       // 页大小
    HI_U32 ulTotalSize;  //Flash总大小
    HI_U32 ulItemNumPerSector;//每个sector中最多存储的NV_ITEM(TEMP_ITME)个数
} SAL_FLASH_CFG_INFO_STRU;

//****************************************************************************
// 函数名称: sal_FlashGetNvSpace
// 功能描述: 获取NV的Flash空间布局信息
//
// 参数说明:
//   pstLayoutInfo [out] Flash布局信息
//
// 返 回 值:
//    HI_ERR_SUCCESS : 成功
//    其它值为错误码
//
//****************************************************************************
HI_EAPI HI_U32 sal_FlashGetNvSpace(HI_OUT SAL_FLASH_CFG_INFO_STRU* pstLayoutInfo);
HI_EAPI HI_U32 sal_UartAdaptInit(HI_SYS_UART_E enUartPort, HI_U32 ulMaxByteInterval, HI_BOOL bEnable);
HI_EAPI HI_U32 sal_UartOpen(HI_SYS_UART_E enUartPort, HI_UART_MODE_TYPE_E enMode, HI_IN HI_UART_INIT_TYPE_S* pstUartCfg);
HI_EAPI HI_U32 sal_UartClose(HI_SYS_UART_E enUartPort ,HI_UART_MODE_TYPE_E enMode);
HI_EAPI HI_U32 sal_UartRead(HI_SYS_UART_E enUartPort, HI_UART_MODE_TYPE_E enMode,HI_IN HI_U8* pucBuffer, HI_U16 usNumberOfBytesToRead, HI_OUT HI_U16* pusNumberOfBytesRead);
HI_EAPI HI_U32 sal_UartWrite(HI_SYS_UART_E enUartPort, HI_UART_MODE_TYPE_E enMode, HI_IN HI_U8* pucBuffer, HI_U16 usNumberOfBytesToWrite, HI_OUT HI_U16* pusNumberOfBytesWritten);

typedef HI_BOOL (*DIAG_IS_CONNECT_F)(HI_U16 usPortNum); // Phycial channel connection status.
//****************************************************************************


//****************************************************************************
HI_U32  dms_ChlDumpFlash(HI_U16 usSrcPortNum, HI_U16 usDestPortNum, DIAG_IS_CONNECT_F pfnIsChlConnect, HI_BOOL bClear);
HI_U32  dms_CheckLocalPortConnect(HI_VOID);
HI_VOID diag_SetRamLcSwtCfg(HI_BOOL bEnable);
HI_BOOL diag_GetRamLcSwtCfg(HI_VOID);
HI_U32  diag_ResetRamLcSwtCfg(HI_BOOL bBootInit);
HI_U32  diag_ConfigTimer(HI_VOID);
HI_U32  sal_GetRamSizeInfo(HI_U32 aulMemInitSize[2], HI_U32 aulMemAvailSize[2], HI_BOOL bMemInitSizeOnly);
//****************************************************************************


//****************************************************************************
// Useful helper utilities.
//****************************************************************************
#ifdef SAL_HAVE_INSIDE_UTILS_API

#ifndef PRODUCT_CFG_SUPPORT_UPG
#define GET_FLASH_CRASH_HRD_ADDR(ulAddr) ulAddr = PRODUCT_CFG_CRASH_FLASH_ADDR
#else
#define GET_FLASH_CRASH_HRD_ADDR(ulAddr) ulAddr = PRODUCT_CFG_CRASH_FLASH_ADDR
#endif

#ifndef PRODUCT_CFG_SUPPORT_UPG
#define GET_FLASH_CRASH_DATA_ADDR(ulAddr, n) ulAddr = PRODUCT_CFG_CRASH_FLASH_ADDR + n*PRODUCT_CFG_FLASH_BLOCK_SIZE
#else
#define GET_FLASH_CRASH_DATA_ADDR(ulAddr, n) ulAddr = PRODUCT_CFG_CRASH_FLASH_ADDR + n*PRODUCT_CFG_FLASH_BLOCK_SIZE
#endif

#define GET_FLASH_CRASH_DATA1_ADDR(ulAddr)   GET_FLASH_CRASH_DATA_ADDR(ulAddr, 1);
#if defined(PRODUCT_CFG_TASK_INT_SYS_ERR_INFO)&&defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#define GET_FLASH_CRASH_DATA2_ADDR(ulAddr)   GET_FLASH_CRASH_DATA_ADDR(ulAddr, 2);
#endif
#define GET_FLASH_CRASH_DATA3_ADDR(ulAddr)   GET_FLASH_CRASH_DATA_ADDR(ulAddr, 3);

#define CHK_FLASH_CRASH_DATA_SIZE(size)      HI_MIN(PRODUCT_CFG_FLASH_BLOCK_SIZE, size)

#ifndef PRODUCT_CFG_SUPPORT_UPG
#define USERDATA_BIDB_HRD_FLASH_ADDR()  PRODUCT_CFG_USERDATA_CCO_FLASH_ADDR
#else
#define USERDATA_BIDB_HRD_FLASH_ADDR()  PRODUCT_CFG_USERDATA_CCO_BIDB_FLASH_ADDR
#endif

#ifndef PRODUCT_CFG_SUPPORT_UPG
#define USERDATA_BIDB_DATA_FLASH_ADDR()  (PRODUCT_CFG_USERDATA_CCO_FLASH_ADDR + PRODUCT_CFG_FLASH_BLOCK_SIZE)
#else
#define USERDATA_BIDB_DATA_FLASH_ADDR()  (PRODUCT_CFG_USERDATA_CCO_BIDB_FLASH_ADDR + PRODUCT_CFG_FLASH_BLOCK_SIZE)
#endif

#ifndef PRODUCT_CFG_SUPPORT_UPG
#define CHECK_USERDATA_BIDB_FULL()
#else
#define CHECK_USERDATA_BIDB_FULL() \
        if((ulItemOffset+ulItemNum)*usItemSize > (PRODUCT_CFG_USERDATA_CCO_BIDB_MAX_SIZE - PRODUCT_CFG_FLASH_BLOCK_SIZE))\
        {\
            return HI_ERR_FULL;\
        }
#endif

#define SAL_CHECK_USER_FLASH_ADDR(x) if(x < PRODUCT_CFG_CRASH_FLASH_ADDR) return HI_ERR_INVALID_PRODUCT_CONFIG;

#ifndef PRODUCT_CFG_SUPPORT_UPG
#define CHECK_USERDATA_APP_FULL()
#else
#define CHECK_USERDATA_APP_FULL() \
        if((HI_U32)((ulItemOffset+ulItemNum)*usItemSize) > (HI_U32)(PRODUCT_CFG_USERDATA_CCO_APP_MAX_SIZE - PRODUCT_CFG_FLASH_BLOCK_SIZE))\
        {\
            return HI_ERR_FULL;\
        }
#define CHECK_USERDATA_USR_FULL() \
        if((HI_U32)((ulItemOffset+ulItemNum)*usItemSize) > (HI_U32)(PRODUCT_CFG_USERDATA_CCO_USER_MAX_SIZE - PRODUCT_CFG_FLASH_BLOCK_SIZE))\
        {\
            return HI_ERR_FULL;\
        }
#endif

#ifndef PRODUCT_CFG_SUPPORT_UPG
#define USERDATA_APP_HRD_FLASH_ADDR()
#define USERDATA_USR_HRD_FLASH_ADDR()
#else
#define USERDATA_APP_HRD_FLASH_ADDR()  ulHrdAddr = PRODUCT_CFG_USERDATA_CCO_APP_FLASH_ADDR;
#define USERDATA_USR_HRD_FLASH_ADDR()  ulHrdAddr = PRODUCT_CFG_USERDATA_CCO_USER_FLASH_ADDR;
#endif

#ifndef PRODUCT_CFG_SUPPORT_UPG
#define USERDATA_APP_DATA_FLASH_ADDR()  0
#else
#define USERDATA_APP_DATA_FLASH_ADDR()  (PRODUCT_CFG_USERDATA_CCO_APP_FLASH_ADDR + PRODUCT_CFG_FLASH_BLOCK_SIZE)
#endif

// BEGIN : DTS2012122101463, ADDED by s00130424  2013-01-18: 重构代码，增加宏定义
#ifndef PRODUCT_CFG_SUPPORT_UPG
#define DIAG_DUMP_FLASH_ADDR(offset, _chip)  0
#else
#define DIAG_DUMP_FLASH_ADDR(offset, _chip)  PRODUCT_CFG_DIAG_##_chip##_DUMP_FLASH_ADDR*offset;
#endif

#ifdef PRODUCT_CFG_PLATFORM_HI3911
/* BEGIN: Modified by w00190206, 2013/4/12   PN:DTS2013041203404*/
#if defined(PRODUCT_CFG_OS_NU)||defined(PRODUCT_CFG_OS_VX)
/* END:   Modified by w00190206, 2013/4/12 */
#define WR_CRASH_INFO_INIT()                         flash_init_crash()
#else
#define WR_CRASH_INFO_INIT()
#endif
#define WRITE_CRASH_INFO(ulAddr, pItems, usItemSize) ret = flash_write_crash((HI_U32)ulAddr,  (HI_VOID*)pItems, (HI_U32)usItemSize)
#define WRITE_1_to_0(ulAddr, pItems, usItemSize) ret = flash_write_1_to_0((HI_U32)ulAddr,  (HI_VOID*)pItems, (HI_U32)usItemSize)
/* BEGIN: Modified by w00190206, 2013/4/12   PN:DTS2013041203404*/
#if defined(PRODUCT_CFG_OS_NU)||defined(PRODUCT_CFG_OS_VX)
/* END:   Modified by w00190206, 2013/4/12 */
#define READ_CRASH_INFO(ulAddr, pItems, usItemSize)  ret = flash_read_crash((HI_U32)ulAddr, (HI_VOID*)pItems, (HI_U32)usItemSize)
#else
#define READ_CRASH_INFO(ulAddr, pItems, usItemSize)
#endif
#else
#define WR_CRASH_INFO_INIT()
#define WRITE_CRASH_INFO(ulAddr, pItems, usItemSize) ret = HI_MDM_FLASH_WriteSafe((HI_U32)ulAddr, (HI_U32)usItemSize,  HI_NULL, (HI_U8*)pItems, HI_TRUE)
#define READ_CRASH_INFO(ulAddr, pItems, usItemSize)  ret = HI_MDM_FLASH_Read(ulAddr, (HI_U32)usItemSize,  HI_NULL, (HI_U8*)pItems)
#endif

#define WRITE_CRASH_OS_INFO(ulAddr, pItems, usItemSize) ret = HI_MDM_FLASH_WriteSafe((HI_U32)ulAddr, (HI_U32)usItemSize,  HI_NULL, (HI_U8*)pItems, HI_TRUE)
#define READ_CRASH_OS_INFO(ulAddr, pItems, usItemSize)  ret = HI_MDM_FLASH_Read(ulAddr, (HI_U32)usItemSize,  HI_NULL, HI_OUT (HI_U8*)pItems)

#ifndef PRODUCT_CFG_SUPPORT_UPG
#define CHECK_USERDATA_APP_DATA_FLASH_ADDR() return HI_ERR_NOT_SUPPORT
#define CHECK_USERDATA_USR_DATA_FLASH_ADDR() return HI_ERR_NOT_SUPPORT
#else
#define CHECK_USERDATA_APP_DATA_FLASH_ADDR() \
        if(SalIsCco())\
        {\
            ulAddr = (HI_U32)(PRODUCT_CFG_USERDATA_CCO_APP_FLASH_ADDR + PRODUCT_CFG_FLASH_BLOCK_SIZE + ulItemOffset*usItemSize);\
        }\
        else \
        {\
            return HI_ERR_INVALID_CHIP_TYPE;\
        }
#define CHECK_USERDATA_USR_DATA_FLASH_ADDR() \
        if(SalIsCco())\
        {\
            ulAddr = (HI_U32)(PRODUCT_CFG_USERDATA_CCO_USER_FLASH_ADDR + PRODUCT_CFG_FLASH_BLOCK_SIZE + ulItemOffset*usItemSize);\
        }\
        else \
        {\
            return HI_ERR_INVALID_CHIP_TYPE;\
        }
#endif

#ifndef PRODUCT_CFG_SUPPORT_UPG
#define GET_USERDATA_BIDM_DATA_FLASH_ADDR() \
        ulAddr = PRODUCT_CFG_USERDATA_CCO_FLASH_ADDR + PRODUCT_CFG_FLASH_BLOCK_SIZE + ulItemOffset*usItemSize;
#else
#define GET_USERDATA_BIDM_DATA_FLASH_ADDR() \
        if(SalIsCco())\
        {\
            ulAddr = (HI_U32)(PRODUCT_CFG_USERDATA_CCO_BIDB_FLASH_ADDR + PRODUCT_CFG_FLASH_BLOCK_SIZE + ulItemOffset*usItemSize);\
        }\
        else \
        {\
            return HI_ERR_INVALID_CHIP_TYPE;\
        }
#endif


#define CHECK_STA_MODULE_VALID()  if(SalIsSta()) \
        {\
            return HI_ERR_NOT_SUPPORT;\
        }


#define CHK_NO_MORE_DATA(x)     if(ulItemOffset + ulItemNum > GetUserDataItems(ulHrdAddr, x))\
    {\
        return HI_ERR_NO_MORE_DATA;\
    }

#if defined(PRODUCT_CFG_SUPPORT_SDM_ENG)
#define GET_DIAG_FLASH_LEFT_SIZE(total, x) ((total) >= PRODUCT_CFG_DIAG_##x##_FLASH_ADDR)?(((total) - PRODUCT_CFG_DIAG_##x##_FLASH_ADDR)) : 0;
#define GET_DIAG_FLASH_ADDR(total,x)       ((total) >= PRODUCT_CFG_DIAG_##x##_FLASH_ADDR)?PRODUCT_CFG_DIAG_##x##_FLASH_ADDR:0;
#else
#define GET_DIAG_FLASH_LEFT_SIZE(total, x) ((total) > PRODUCT_CFG_DIAG_##x##_FLASH_ADDR+2*PRODUCT_CFG_FLASH_BLOCK_SIZE)?((total) - PRODUCT_CFG_DIAG_##x##_FLASH_ADDR) : 0;
#define GET_DIAG_FLASH_ADDR(total,x)       ((total) > PRODUCT_CFG_DIAG_##x##_FLASH_ADDR+2*PRODUCT_CFG_FLASH_BLOCK_SIZE)?PRODUCT_CFG_DIAG_##x##_FLASH_ADDR:0;
#endif

#ifndef PRODUCT_CFG_SUPPORT_UPG
#define GET_DIAG_FLASH_LAYOUT_INFO(total) \
            pstLayoutInfo->ulSectorSize    = 0;\
            pstLayoutInfo->ulSectorNum     = 0;\
            pstLayoutInfo->ulStartSectorId = 0;\
            pstLayoutInfo->ulPageSize      = ((total) > PRODUCT_CFG_DIAG_FLASH_ADDR+2*PRODUCT_CFG_FLASH_BLOCK_SIZE)?((total) - PRODUCT_CFG_DIAG_FLASH_ADDR) : 0;\
            pstLayoutInfo->ulAddr          = PRODUCT_CFG_DIAG_FLASH_ADDR;
#else
#define GET_DIAG_FLASH_LAYOUT_INFO(total) \
            {HI_FTM_CHIP_TYPE_E enChipType = HI_DMS_GetChipType();\
            pstLayoutInfo->ulSectorSize    = 0;\
            pstLayoutInfo->ulSectorNum     = 0;\
            pstLayoutInfo->ulStartSectorId = 0;\
            pstLayoutInfo->ulPageSize      = 0;\
            pstLayoutInfo->ulTotalSize    = total;\
            if(HI_DMS_IsChipCco(enChipType))\
            {\
                pstLayoutInfo->ulPageSize  = GET_DIAG_FLASH_LEFT_SIZE(total,CCO);\
                pstLayoutInfo->ulAddr      = (HI_U32)GET_DIAG_FLASH_ADDR(total,CCO);\
            }\
            else if(HI_DMS_IsChipSta(enChipType))\
            {\
                pstLayoutInfo->ulPageSize  = GET_DIAG_FLASH_LEFT_SIZE(total,STA);\
                pstLayoutInfo->ulAddr      = (HI_U32)GET_DIAG_FLASH_ADDR(total,STA);\
            }\
            else \
            {\
                return HI_ERR_INVALID_CHIP_TYPE;\
            }}
#endif
#endif // SAL_HAVE_INSIDE_UTILS_API

#define SAL_CHK_INPUT_VAL_RET_U32(_x, _max) do { \
        if(_x >= _max)\
        {\
            return HI_ERR_INVALID_PARAMETER;\
        }\
    }while(0)
//****************************************************************************


//****************************************************************************
// Check parameter utilities
//****************************************************************************
#define CHEK_PTR_ZERO_SIZE(x)          if((HI_U32)x == 0){return HI_NULL;}
#define CHEK_U32_ZERO_SIZE(x)          if((HI_U32)x == 0) { return 0; }
#define CHEK_DYN_MEM_SIZE(x)           if(((HI_U32)(x) == 0) || ((HI_U32)(x) >= HI_MIN(SAL_TOTAL_DYN_MEM_SZIE(),g_use_ram_size))) { return HI_NULL; }

/* BEGIN: Added by b00208046, 2013/7/3   PN:DTS2013062400945 */
#ifdef PRODUCT_CFG_SUPPORT_UPG
#define CHK_VALID_FLASH_ERASE_ADDR(x)  if((HI_U32)(x) < (HI_U32)PRODUCT_CFG_NV_FLASH_ADDR)     { return HI_ERR_ACCESS_DENIED; }
#else
#define CHK_VALID_FLASH_ERASE_ADDR(x)  if((HI_U32)(x) < (HI_U32)PRODUCT_CFG_FLASH_BLOCK_SIZE)  { return HI_ERR_ACCESS_DENIED; }
#endif
/* END:   Added by b00208046, 2013/7/3 */
#define HI_IS_NV_FTM(x)               ((x)>=HI_NV_FTM_ID_START) && ((x)<HI_NV_FTM_ID_END)

#define SAL_MAKE_U32(p)   (((HI_U32)p[0] << 24) + ((HI_U32)p[1] << 16) + ((HI_U32)p[2] << 8) + (HI_U32)p[3])
//****************************************************************************


//****************************************************************************
// DIAG Init Evet Queue Helpers
//****************************************************************************
#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
#if defined(PRODUCT_CFG_HAVE_FEATURE_SYS_ERR_INFO)
#define EVT_BUF_SIZE   HI_SYS_NDM_EVT_MAX

typedef struct
{
    HI_U32 total;
    HI_U32 head;
    HI_U32 tail;
    HI_VOLATILE HI_SYS_EVT_SDM_S *pEvt;
}evt_queue;

HI_EXTERN evt_queue g_stSdmEvtQueue;

HI_PRVL HI_VOID sal_EvtQCfgChk(HI_VOID)
{
    #if defined(HI_DEBUG)
    evt_queue* queue= &g_stSdmEvtQueue;

    HI_ASSERT(queue->pEvt);
    #endif
}

HI_PRVL HI_U8 sal_DiagInitEvtQGet(HI_U16* pusTimeStamp, HI_U16* pusRc)
{
    HI_U8 result;
    SAL_LOCK0();
    evt_queue* queue= &g_stSdmEvtQueue;
    HI_ASSERT(queue->pEvt);

    if (queue->head && (queue->tail == queue->head))
    {
        SAL_UNLOCK0();
        return HI_DEID_INIT_MAX;
    }
    result = queue->pEvt->aucInitEvtId[queue->tail];
    if(pusTimeStamp)
    {
        *pusTimeStamp = queue->pEvt->auInitEvtIdTime[queue->tail];
    }
    if(pusRc)
    {
        *pusRc = queue->pEvt->auInitEvtIdRc[queue->tail];
    }
    queue->tail = (queue->tail + 1) & (EVT_BUF_SIZE - 1);
    SAL_UNLOCK0();
    return result;
}

HI_PRVL HI_U32 sal_DiagInitEvtQAppend(HI_U8 evt, HI_U16 rc)
{
    SAL_LOCK0();
    evt_queue* queue = &g_stSdmEvtQueue;
    HI_S32 head      = 0;
    HI_S32 maxhead   = 0;

    if(queue->total >= EVT_BUF_SIZE)
    {
        SAL_UNLOCK0();
        return HI_ERR_TOO_SMALL_BUFFER;
    }

    head    = queue->head;
    maxhead = (queue->tail - 1) & (EVT_BUF_SIZE - 1);

    queue->pEvt->aucInitEvtId[head]    = evt;
    queue->pEvt->auInitEvtIdTime[head] = (HI_U16)HI_MDM_GetTick();
    queue->pEvt->auInitEvtIdRc[head]   = (HI_U16)rc;
    if (head != maxhead)
    {
        head++;
        queue->total ++;
        head = (HI_U32)head & (EVT_BUF_SIZE - 1);
    }
    queue->head = head;
    SAL_UNLOCK0();
    return HI_ERR_SUCCESS;
}

HI_PRVL HI_VOID sal_DiagInitEvtQReset(void)
{
    SAL_LOCK();
    evt_queue* queue = &g_stSdmEvtQueue;
    queue->tail = 0;
    SAL_UNLOCK();
}
#else
HI_PRVL HI_VOID sal_EvtQCfgChk(HI_VOID) {}
HI_PRVL HI_U8   sal_DiagInitEvtQGet(HI_U16* pusTimeStamp, HI_U16* pusRc)  { return HI_DEID_INIT_MAX;}
HI_PRVL HI_VOID sal_DiagInitEvtQReset(void) {}
HI_PRVL HI_U32  sal_DiagInitEvtQAppend(HI_U8 ev,HI_U16 rc)        { return 0;}
#endif
#endif // #if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
//****************************************************************************


//****************************************************************************
HI_U32 sal_ResetMonitor(HI_VOID);
HI_VOID sal_ResetInit(HI_VOID);
HI_VOID sal_InitBuildVer(HI_VOID);
HI_VOID sal_InitDefaultUserInfo(HI_VOID);
HI_U32 sal_InitProductId(HI_VOID);
HI_U32 sal_InitDchlNvCfg(HI_VOID);
HI_VOID sal_InitSysCmdCfg(HI_VOID);
HI_VOID sal_SetSysStatusNetStatus(HI_U8  ucNetStatus);

//****************************************************************************


//****************************************************************************
#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
HI_PRVL HI_U16 sal_GetDevId16(HI_VOID)
{
    HI_U8 ucPlcMac[HI_PLC_MAC_ADDR_LEN];
    HI_CONST HI_NV_FTM_PRODUCT_ID_S* pstId = SAL_GetProductId();
    (hi_void)memcpy_s(ucPlcMac, HI_PLC_MAC_ADDR_LEN, pstId->ucPlcMac, HI_PLC_MAC_ADDR_LEN);
    return HI_SYS_Crc16(0, (HI_CONST HI_U8 *)ucPlcMac, HI_PLC_MAC_ADDR_LEN);
}
HI_PRVL HI_U16 sal_GetVerId16(HI_SYS_STATUS_IND_S* pstSysStatus)
{
    HI_U8 buf[sizeof(pstSysStatus->ausVer)+sizeof(pstSysStatus->aucVer)];
    (hi_void)memcpy_s(buf, sizeof(pstSysStatus->ausVer)+sizeof(pstSysStatus->aucVer),
        pstSysStatus->ausVer, sizeof(pstSysStatus->ausVer)+sizeof(pstSysStatus->aucVer));
    return HI_SYS_Crc16(0, (HI_CONST HI_U8 *)buf, sizeof(pstSysStatus->ausVer)+sizeof(pstSysStatus->aucVer));
}
#endif
//****************************************************************************


//****************************************************************************
HI_PRVL HI_S32 _Is_Space( int c)
{
    return c == 0x20 || (c >= 0x09 && c <= 0x0D);
}

HI_PRVL HI_S32 _Is_Digit(int c)
{
    return  ((c >= '0') && (c <= '9'));
}

//***********************************
// ASCII stirng to S32
//***********************************
HI_PRVL HI_U32 Str2S32(HI_CONST HI_CHAR *pszString, hi_u32 len)
{
    HI_UNREF_PARAM(len);
    HI_CONST HI_CHAR *ptr = (HI_CONST HI_CHAR *)pszString;
    HI_U32 num = 0;
    HI_S32 c   = *ptr;

    while( _Is_Space(c) )
    {
        c = *++ptr;
    }

    while( _Is_Digit(c) )
    {
        num = (HI_U32)(( 10 * num ) + ( c - '0' ));
        c = *++ptr;
    }

    return ( (HI_U32) num );
}

//****************************************************************************
#if defined(PRODUCT_CFG_HAVE_FEATURE_SYS_ERR_INFO)
HI_VOID sas_SysErrInit(HI_BOOL bInit);
HI_VOID sas_SetSysErrProductId(HI_VOID);
HI_PVOID sas_GetErrR(HI_U32* pulSize); // err in RAM
HI_BOOL sas_CheckSdmSysErrCtx(HI_PVOID pSdmErrCache);
HI_BOOL sas_CheckCSdmSysErrCtx(HI_PVOID pSdmErrCache);
HI_U32 sas_GetSysErrMaxSize(HI_VOID);
HI_VOID sas_SetSdmSysErrRunTime(HI_PVOID pSdmErrCache);
HI_U32 SAL_SdmSysErrSetSpInfo(HI_U32 ulSp, HI_U32 ulPc);
#else
HI_PRVL HI_VOID sas_SysErrInit(HI_BOOL bInit){}
HI_PRVL HI_PVOID sas_GetErrR(HI_U32* pulSize) {*pulSize = 0; return HI_NULL;}
HI_PRVL HI_BOOL sas_CheckSdmSysErrCtx(HI_PVOID pSdmErrCache) {return HI_FALSE;}
HI_PRVL HI_U32 sas_GetSysErrMaxSize(HI_VOID) {return 0;}
HI_PRVL HI_VOID sas_SetSdmSysErrRunTime(HI_PVOID pSdmErrCache){}
HI_PRVL HI_U32 SAL_SdmSysErrSetSpInfo(HI_U32 ulSp, HI_U32 ulPc){return HI_ERR_NOT_SUPPORT;}
#endif
HI_U32 dms_TcpSetKeepAlive(HI_U16 usPortNum, HI_S32 slEnable, HI_S32 slTcpKeepIdle, HI_S32 slTcpKeepCnt);
HI_U32 dms_TcpNoDelay(HI_U16 usPortNum, HI_BOOL bEnable);
HI_U32 dms_TcpRcvFrmTimeOut(HI_U16 usPortNum, HI_U16 usTimeOut);
#if defined(PRODUCT_CFG_DIAG_FRM_ERR_PK_RPT)
HI_U32 diag_ChlErrorReport(HI_U32 ret, HI_U32 ulId);
#else
#define diag_ChlErrorReport(ret, ulId) do { }while(0)
#endif
#if defined(PRODUCT_CFG_DIAG_FRM_ERR_EVT_RPT)
HI_U32 diag_ChlErrorReportEvt(HI_U32 ret, HI_U32 ulId);
#else
#define diag_ChlErrorReportEvt(ret, ulId) do { }while(0)
#endif

typedef enum
{
    DIAG_CHL_PCL_DBG_RPT_OPT_CCO2STA,
    DIAG_CHL_PCL_DBG_RPT_OPT_STA2CCO,
}DIAG_CHL_PCL_DBG_RPT_OPT_E;

#if defined(PRODUCT_CFG_DIAG_PLC_FRM_ERR_PK_RPT)
HI_U32 diag_ChlPlcDbgReport(HI_U32 ret, HI_U32 ulId, HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN], DIAG_CHL_PCL_DBG_RPT_OPT_E eOption);
#else
#define diag_ChlPlcDbgReport(ret, ulId, aucMac, eOption) do { }while(0)
#endif

// ERR REPORT ONLY for DIAG inside
#define DIAG_CHL_ERR_RPT(ulId, ret)             do { if(ret != HI_ERR_SUCCESS) { diag_ChlErrorReportEvt(ret, ulId); diag_ChlErrorReport(ret, ulId); } } while(0)

#define DIAG_CHL_DBG_RPT(ulId, ret, eOption)          do { HI_U8 _aucSdmMac[HI_PLC_MAC_ADDR_LEN]; diag_GetPlcSdmMac(aucSdmMac); diag_ChlPlcDbgReport(ret, ulId, _aucSdmMac, eOption); } while(0)
#define DIAG_CHL_DBG_RPT2STA(ulId, ret, aucSdmMac)    do {diag_ChlPlcDbgReport(ret, ulId, aucSdmMac, DIAG_CHL_PCL_DBG_RPT_OPT_CCO2STA); } while(0)
//****************************************************************************


//****************************************************************************
#define SAL_STUB_INIT() do { }while(0)
//****************************************************************************


//****************************************************************************
#if defined(PRODUCT_CFG_DIAG_IND_SAVE_FLASH)
// 超控器且DUMP的NV项开关打开则认为DIAG通道可用
#define DIAG_DUMP_CHL_ENABLE()             ((diagGetRamDumpMsgPrintFlashSwtCfg()) && diagGetSave2FlashRamSwtCfg())
#define DIAG_DUMP_CHL_CHECK()              (diagGetRamDumpMsgPrintFlashSwtCfg())
#else
#define DIAG_DUMP_CHL_ENABLE()             HI_FALSE
#define DIAG_DUMP_CHL_CHECK()              HI_FALSE
#endif

#define SAL_DIAG_DATA_PK_PADDING       sizeof(HI_U32)

#if defined(PRODUCT_CFG_DIAG_IND_SAVE_FLASH)
#define SAL_DIAG_APPEND_DATA_PK_PADDING(_pBuffer, _usBufferSize) \
do {\
    if((bDumpChl && !bConnect))\
    { HI_U32 __lfpad = HI_DMS_LF_PADDING;\
     (hi_void)memcpy_s(&(((HI_U8*)(_pBuffer))[_usBufferSize]), SAL_DIAG_DATA_PK_PADDING, &__lfpad , SAL_DIAG_DATA_PK_PADDING);\
    }\
}while(0)
#else
#define SAL_DIAG_APPEND_DATA_PK_PADDING(_pBuffer, _usBufferSize)  do { }while(0)
#endif

#if defined(PRODUCT_CFG_DIAG_IND_SAVE_FLASH)
#define SAL_DIAG_DATA_PK_SIZE_ADJUST(_size)    (bDumpChl && !bConnect)?(SAL_DIAG_DATA_PK_PADDING+(_size)):(_size)
#else
#define SAL_DIAG_DATA_PK_SIZE_ADJUST(_size)    _size
#endif

enum
{
    MSG_OPT_DUMP_MSG=1,
    MSG_OPT_CLEAR_MSG=2
};
//****************************************************************************


//****************************************************************************
// Check the pre-defined value.
//****************************************************************************
#if !defined(HI_HAVE_NATIVE_COMPILER_VC)
#define EN_DIAG_BUFFER_ID_MAX 7
#if (EN_DIAG_BUFFER_ID_MAX > SAL_DIAG_SDM_CAT_MAX)
#error "EN_DIAG_BUFFER_ID_MAX > SAL_DIAG_SDM_CAT_MAX"
#endif
#endif
//****************************************************************************


//****************************************************************************
#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
#define SAL_DFX_SYS_VAL_DECLARE
#include "sal_dfx_sys_val.h"
#endif
//****************************************************************************

//****************************************************************************
#if defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
HI_EXTERN HI_U32 FLASH_Read(HI_IN HI_U32 ulByteOffAddr, HI_OUT HI_VOID *buffer, HI_IN HI_U32 ulByteSize);
HI_EXTERN HI_U32 FLASH_Write(HI_IN HI_U32 ulByteOffAddr, HI_IN HI_VOID *buffer, HI_IN HI_U32 ulByteSize, HI_BOOL AutoErase);
HI_EXTERN HI_U32 FLASH_Erase(HI_IN HI_U32 ulByteOffAddr, HI_IN HI_U32 ulByteSize);
#endif
//****************************************************************************



//****************************************************************************
#define SAL_USER_DATA_STAT(m) do {\
    if(HI_USERDATA_ID_APP == ucUserDataId)\
    {\
        SAL_SDM_SET_SYS_INC_RET(ret,aulUserDataStat, m, m+1);\
    }\
    else if(HI_USERDATA_ID_BIDB == ucUserDataId)\
    {\
        SAL_SDM_SET_SYS_INC_RET(ret,aulUserDataStat, m+2, m+3);\
    }\
}while(0)
//****************************************************************************


//****************************************************************************
#if defined(PRODUCT_CFG_KDIAG_CCO_FLASH_ADDR)
#define WRITE_KDIAG2FLASH(x)  WRITE_CRASH_INFO(x, pucBuffer, usBufferSize)
#else
#define WRITE_KDIAG2FLASH(x)
#endif
//****************************************************************************

//*****************************************************************************
#define DMS_CHK_FRM_MAC_PORT(usPortNum) \
    (((HI_U32)usPortNum >= (HI_U32)HI_DMS_CHL_MAC_PORT_MIN) && ((HI_U32)usPortNum <= (HI_U32)HI_DMS_CHL_MAC_PORT_MAX))

#define DMS_CHK_FRM_TCP_PORT(usPortNum) \
    (((HI_U32)usPortNum >= (HI_U32)HI_DMS_CHL_TCP_PORT_MIN) && ((HI_U32)usPortNum <= (HI_U32)HI_DMS_CHL_TCP_PORT_MAX))

#define DMS_CHK_FRM_UART_PORT(usPortNum) \
    ((HI_U32)usPortNum <= (HI_U32)HI_DMS_CHL_UART_PORT_MAX)

#if defined(PRODUCT_CFG_SUPPORT_UPG_SEPARATE)
#define DMS_CHK_OLD_FRM_MAC_PORT(usPortNum) \
    (((HI_U32)usPortNum == (HI_U32)0xF000) || ((HI_U32)usPortNum == (HI_U32)0xF001) || ((HI_U32)usPortNum == (HI_U32)0xF002))
#else
#define DMS_CHK_OLD_FRM_MAC_PORT(usPortNum) \
    (((HI_U32)usPortNum == (HI_U32)0xF000) || ((HI_U32)usPortNum == (HI_U32)0xF001))
#endif
//*****************************************************************************


//*****************************************************************************
// Helper
//*****************************************************************************
#define HI_BIDB_SIGNATURE                HI_MAKE_IDENTIFIER('b','i','D','B')
#define HI_TF_LIST_SIGNATURE                HI_MAKE_IDENTIFIER('T','F','L','1')
#define HI_BLACK_LIST_SIGNATURE                HI_MAKE_IDENTIFIER('B','L','L','1')

typedef struct
{
  HI_U32 ulSignature; // HI_BIDB_SIGNATURE
  HI_U32 ulUserDataId;
  HI_U32 ulItemNum;
  HI_U32 ulItemSize;
  HI_U32 reserved[4];
}FLASH_USERDATA_BIDB_HEADER_S;

#define DB_VAL_MAX  0x00FFFFFF

#define CHECK_VALID_DB(ucUserDataId,ulSignature)  \
    ((stUsrDataHrd.ulSignature == ulSignature) && (stUsrDataHrd.ulUserDataId == ucUserDataId) && \
    stUsrDataHrd.ulItemSize && stUsrDataHrd.ulItemNum && (stUsrDataHrd.ulItemSize < DB_VAL_MAX) && (stUsrDataHrd.ulItemNum < DB_VAL_MAX))
//*****************************************************************************

//****************************************************************************
#define DO_SYS_RST_NEVER_RETURN()          HI_SYS_NEVER_RETURN()
#define DO_SAL_REPORT_SYS_INIT_EVT(ret,x)  do { if(ret == HI_ERR_SUCCESS) {HI_DIAG_ReportSysInitEvt(x);} else {} } while(0)
//****************************************************************************


//****************************************************************************
// Set NV .hupg File Version.
//****************************************************************************
#define SAL_SET_SYSINFO_M(pCtx,m, v)    pCtx->stSysInfo.m = v
#define SAL_SET_SYSSTATUS_M(pCtx,m, v)  pCtx->stSysStatus.m = v
#define SAL_SET_SYSNVVER_M(pCtx,m, v)   pCtx->stSysNvVer.m = v

#define SAL_SET_NV_FILE_VER_BY_CODE(pCtx)  do {\
        pCtx->stSysInfo.ucNvCVer      = pCtx->stSysInfo.aucVer[2];\
        pCtx->stSysInfo.ausNvVer[0]   = pCtx->stSysInfo.ausVer[0];\
        pCtx->stSysInfo.ausNvVer[1]   = pCtx->stSysInfo.ausVer[1];\
        pCtx->stSysStatus.ucNvCVer    = pCtx->stSysInfo.aucVer[2];\
        pCtx->stSysStatus.ausNvVer[0] = pCtx->stSysInfo.ausVer[0];\
        pCtx->stSysStatus.ausNvVer[1] = pCtx->stSysInfo.ausVer[1];\
        pCtx->stSysInfo.usNvVerId     = 0;\
        pCtx->stSysNvVer.ucNvVer[0]   = pCtx->stSysInfo.aucVer[0];\
        pCtx->stSysNvVer.ucNvVer[1]   = pCtx->stSysInfo.aucVer[1];\
        pCtx->stSysNvVer.ucNvVer[2]   = pCtx->stSysInfo.aucVer[2];\
        pCtx->stSysNvVer.ausNvVer[0]  = pCtx->stSysInfo.ausVer[0];\
        pCtx->stSysNvVer.ausNvVer[1]  = pCtx->stSysInfo.ausVer[1];\
        pCtx->stSysNvVer.usNvVerId    = 0;\
}while(0)

// Set NV .hupg File Version.
#if defined(PRODUCT_CFG_SUPPORT_UPG)
#define SAL_SET_NV_FILE_VER(pCtx)  do { \
        SAL_SET_NV_FILE_VER_BY_CODE(pCtx);\
}while(0)
#define SAL_SET_NV_UPG_USER_OPTION(pCtx)    SAL_SET_SYSINFO_M(pCtx,usUpgOption, (HI_U16)(GET_HI_DCID_UPG_CHL_CFG_S()->force_upg_bsn))
#else
#define SAL_SET_NV_FILE_VER(pCtx)           do { }while(0)
#define SAL_SET_NV_UPG_USER_OPTION(pCtx)    do { }while(0)
#endif // #if defined(PRODUCT_CFG_SUPPORT_UPG)

//****************************************************************************


//*****************************************************************************
#if defined(PRODUCT_CFG_OS_NU)
#define SAL_OS_TYPE_NAME           "N"
#elif defined(PRODUCT_CFG_OS_VX)
#define SAL_OS_TYPE_NAME           "V"
#else
#define SAL_OS_TYPE_NAME           "?"
#endif

#define SAL_WD_ENABLE_NAME         "W"
#define SAL_WD_DISABLE_NAME        "0"

#if defined(HI_DEBUG)
#define SAL_BUILD_TYPE_NAME        "D"
#else
#define SAL_BUILD_TYPE_NAME        "R"
#endif
//*****************************************************************************


//*****************************************************************************
#define SAL_CPY_U32(dst, src, cnt) do {\
    HI_U32 k = 0;\
    HI_U32* pdst = (HI_U32*)(dst);\
    HI_U32* psrc = (HI_U32*)(src);\
    for(k = 0; k < cnt; k++)\
    {\
        pdst[k] = psrc[k];\
    }\
}while(0)
//*****************************************************************************

//*****************************************************************************
#define DIAG_ERR_RPT_LEN 48
#if defined(PRODUCT_CFG_DFX_DIAG_CHL_RPT)
HI_EAPI HI_U32 dms_SendDiagDbgPacket2TcpPort(HI_U16 usId, HI_PVOID pData, HI_U16 usDataLen); // send directly to tcp port bypass OS queue.
#else
HI_PRVL HI_U32 dms_SendDiagDbgPacket2TcpPort(HI_U16 usId, HI_PVOID pData, HI_U16 usDataLen) { return HI_ERR_SUCCESS; }
#endif
//*****************************************************************************


//*****************************************************************************
HI_EXTERN HI_U32 dms_CloseAllSockets(HI_U32 ulOption);
//*****************************************************************************


//*****************************************************************************
#if defined(PRODUCT_CFG_SUPPORT_SDM_ENG)
HI_EXTERN HI_VOID HI_SDM_EngMainMonitor(HI_VOID);
/* BEGIN: Deleted by b00208046, 2014/3/17   问题单号:DTS2014012702376 */
/* END: Deleted by b00208046, 2014/3/17 */
#else

HI_PRVL HI_VOID HI_SDM_EngMainMonitor(HI_VOID)    {  }
/* BEGIN: Deleted by b00208046, 2014/3/17   问题单号:DTS2014012702376 */
/* END: Deleted by b00208046, 2014/3/17 */
#endif
//*****************************************************************************

HI_S8 pnv_v200_get_fix_aagc_gain(HI_VOID);
/*****************************************************************************
 函 数 名  : HI_MAC_OpenAfeRxHpf
 功能描述  : 打开AFE的接收高通滤波器
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2014年1月6日
    作    者   : likunhe/00194117
    修改内容   : 新生成函数

*****************************************************************************/
HI_VOID drv_open_afe_rx_hpf(HI_VOID);
HI_BOOL pnv_v200_is_fix_aagc_enable(HI_VOID);
HI_U32 crc32 (HI_U32  crc, const HI_U8* buf, HI_U32 len);
/*****************************************************************************
 函 数 名  : HI_MAC_CloseAfeRxHpf
 功能描述  : 关闭AFE的接收高通滤波器
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2014年1月6日
    作    者   : likunhe/00194117
    修改内容   : 新生成函数

*****************************************************************************/
HI_VOID drv_close_afe_rx_hpf(HI_VOID);
HI_VOID low_power_change_freq(void);
HI_VOID low_power_switch_clk(HI_VOID);
HI_VOID low_power_change_sfc_freq(HI_VOID);
HI_U32 diag_GetSdkProductVer(HI_CHAR szProduct[HI_DIAG_VER_FULL_PRODUCT_NAME_MAX_SIZE]);
HI_U32 diag_GetProductVer(HI_CHAR szProduct[HI_DIAG_VER_FULL_PRODUCT_NAME_MAX_SIZE]);
HI_U32 HI_HRTIMER_GetCurrentVal(HI_VOID);
HI_U32 HI_SYS_StartUserTimerPrv(HI_U16 usTimerId, HI_U32 ulPeriod, HI_U8 ucOption, HI_VOID_CALLBACK_F pfnTimerRoutine);
HI_U32 HI_SYS_StopUserTimerPrv(HI_U16 usTimerId);
HI_U32 SAL_OSGetCurrentTaskPointer(HI_OUT HI_PVOID* pTask);
HI_U32 SAL_route_table_show(HI_CHAR *buf, HI_U32 buf_len);
HI_VOID SAL_OSExcHook(HI_U32 vecNum, EXC_CONTEXT_S* pEsf);
//****************************************************************************
#endif // __SAL_COMMON_H__

