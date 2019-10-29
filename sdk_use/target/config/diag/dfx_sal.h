//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : dfx_sal.h
//  版 本 号   : V1.0
//  作    者   : shenhankun/00130424
//  生成日期   : 2011-11-10
//  功能描述   : SAL 诊断包ID和对应参数结构定义, 仅供SAL内部使用
//
//  函数列表   : 无
//  修改历史   :
//  1.日    期 : 2011-11-10
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

#ifndef __DFX_SAL_H__
#define __DFX_SAL_H__
#include <hi_ft_nv.h>
#include <dfx_sys.h>
//*****************************************************************************

#define SAL_SUPPORT_SYS_ERR_INFO_API

//*****************************************************************************
// 命令ID定义 [0x7000, 0x8000)
//*****************************************************************************

#define ID_DIAG_CMD_REMOTE                                   (0x7000)
#define ID_DIAG_CMD_REMOTE_USR                               (0x7001)
#define ID_DIAG_CMD_REMOTE_END                               (0x7002)
#define ID_DIAG_CMD_SYS_ERR_INFO                             (0x7003) // syserr, IND --> DIAG_CMD_SDM_CTX_S
#define ID_DIAG_CMD_SYS_ERR_INFO_CLS                         (0x7004) // clssyserr
#define ID_DIAG_CMD_SYS_ERR_DUMP                             (0x7005) // syserr, IND --> DIAG_CMD_SDM_CTX_S
#define ID_DIAG_CMD_DO_SYS_ERR                               (0x7006) // dosyserr
#define ID_DIAG_CMD_DO_FRAME_ERR                             (0x7007) // DIAG error report
#define ID_DIAG_CMD_DUMP_FILE                                (0x7008)
#define ID_DIAG_CMD_DUMP_MEM2K_FILE                          (0x7009)
#define ID_DIAG_CMD_SYS_CMD_END                              (0x700A)


#define ID_DIAG_CMD_DIAG_SDM                                 (0x7010) //  ,,DIAG_CMD_SAL_DIAG_SDM_S
#define ID_DIAG_CMD_DIAG_DFX_SET_LC_SWT                      (0x7011)
#define ID_DIAG_CMD_SAL_SDM                                  (0x7012) //  ,,DIAG_CMD_SAL_SYS_SDM_S

#define ID_DIAG_CMD_ETH_CFG_SET                              (0x7013)
#define ID_DIAG_CMD_ETH_CFG_GET                              (0x7014)

#define ID_DIAG_CMD_SAL_DEMO                                 (0x7020) // Demo REQ
#define ID_DIAG_CMD_SAL_DEMO_ERR                             (0x7021) // Demo ERR REQ reserved
#define ID_DIAG_CMD_SAL_DEMO_END                             (0x7025)

#define ID_DIAG_CMD_SYS_ERR_DUMP_FINISH                      (0x7030) // syserr dump finish report
#define ID_DIAG_CMD_DUMP_FINISH                              (0x7031)

#define ID_DIAG_CMD_FILE_TRANSMIT_START                      (0x7050) // HI_FILE_TRANSMIT_REQ_S, ,
#define ID_DIAG_CMD_FILE_TRANSMIT                            (0x7051) // , , HI_FILE_TRANSMIT_BLK_IND_S
#define ID_DIAG_CMD_FILE_TRANSMIT_END                        (0x7052) // , , ,
#define ID_DIAG_CMD_FILE_RESULT                              (0x7053) // ,,HI_FILE_TRANSMIT_RESULT_S

//ICC同计量
#define ID_DIAG_CMD_ICC_STAT                                 (0x7060)

#define HI_DCID_QRY_CFG                                      (0x7080) // syscmdq <cmd_id>
#define HI_DCID_SET_CFG                                      (0x7081) // syscmd <cmd_id> <param>  HI_DCID_SET_CFG_S
#define ID_DIAG_CMD_SAL_SET_SDM_MAC                          (0x7082) //
#define ID_DIAG_CMD_SAL_RD_MEM_2K                            (0x7083) // dump 2KB
#define ID_DIAG_CMD_SAL_RD_MEM_SMALL                         (0x7084) // dump
#define ID_DIAG_CMD_SAL_SET                                  (0x7090) // HI_DIAG_CMD_SAL_CFG_SET_S
#define ID_DIAG_CMD_SAL_SET_END                              (0x709A) // reserved

// Demo : 0x7100 -- 0x7200, 0x7300--0x7350

// IND
#define ID_DIAG_CMD_SAL_SDM_DIAG_CHL                         (0x7900)

#define ID_DIAG_CMD_DUMP_FLASH_DIAG_START                    (0x7A00)
#define ID_DIAG_CMD_DUMP_FLASH_DIAG_END                      (0x7A01)
#define ID_DIAG_CMD_DUMP_FLASH_ERR                           (0x7A02)
#define ID_DIAG_CMD_DUMP_FLASH_NO_MSG                        (0x7A03)
#define ID_DIAG_CMD_CLEAR_FLASH_DIAG_START                   (0x7A04)
#define ID_DIAG_CMD_CLEAR_FLASH_DIAG_END                     (0x7A05)

// reserved
// 0x7B00 -- 0x8000

#define DIAG_CTRL_MULTI_UNIT_PACKET    0xFFFF
#define DIAG_CTRL_AUTO_ACK_PACKET      0xFFFE
//*****************************************************************************


//*****************************************************************************
#define SAL_BUILD_VER_PRODUCT_LEN_MAX   52
#if !defined(PRODUCT_CFG_HSO)
#if (SAL_BUILD_VER_PRODUCT_LEN_MAX < HI_BUILD_VER_PRODUCT_LEN_MAX)
#error "SAL_BUILD_VER_PRODUCT_LEN_MAX != HI_BUILD_VER_PRODUCT_LEN_MAX"
#endif
#endif
#define SAL_BUILD_VER_BOOT_LEN_MAX   52

#if defined (SAL_HAVE_SIMU_WIN_VER)
#define PRODUCT_CFG_DFX_TASKNUM_MAX     16
#endif
//*****************************************************************************


//*****************************************************************************
typedef enum
{
    DfxAckCmdDefault    = 0,
    DfxAckCmdString1    = 1,
    DfxReportPrint2     = 2,
    DfxReportPrint3     = 3,
    DfxReportLayerMsg4  = 4,
    DfxReportLogMsg5    = 5,
    DfxReportLocalCmd6  = 6,
    DfxReportSysMsg7    = 7,
    DfxSendFrameTest    = 0xFF
}HI_SAL_DFX_DEMO_E;
//*****************************************************************************

//*****************************************************************************
typedef struct
{
    HI_U32 aulReport2PortOkCnt[SAL_DIAG_SDM_CAT_MAX];
    HI_U32 aulReport2PortFailCnt[SAL_DIAG_SDM_CAT_MAX];
    HI_U32 aulReport2PortFailRc[SAL_DIAG_SDM_CAT_MAX];
    HI_U32 aulSend2QFailCnt[SAL_DIAG_SDM_CAT_MAX];
    HI_U32 aulFreeAddrFailCnt[SAL_DIAG_SDM_CAT_MAX];
    HI_U32 aulMemAddrStartQ[SAL_DIAG_SDM_CAT_MAX];
    HI_U32 aulMemAddrEndQ[SAL_DIAG_SDM_CAT_MAX];
    HI_U32 ulConnectedPortNum;
    HI_U32 aulDynMemReportIndCnt[3];
    HI_U32 aulCallApiOkCnt[SAL_DIAG_SDM_CAT_MAX];
    HI_U32 aulCallApiCnt[SAL_DIAG_SDM_CAT_MAX];
    HI_U32 aulCallApiFailCnt[SAL_DIAG_SDM_CAT_MAX];
    HI_U32 aulCallApiFailRc[SAL_DIAG_SDM_CAT_MAX];
    HI_U32 aulIndCallApiAOER[4];
    HI_U32 aulSysMsgRptCallApiAOER[4];
    HI_U32 aulSysEvtRptCallApiAOER[4];
    HI_U32 aulSysStatRptCallApiAOER[4];
    HI_U32 aulNdmPkCallApiAOER[4];
    HI_U32 aulUserCmdRegCnt[2];
    HI_U32 aulParamCmdRegCnt[2];
    HI_U32 aulSysCmdRegCnt[2];
    HI_U32 aulNvRegCnt[2];
    HI_U32 aulStatRegCnt[2];
    HI_U32 aulTaskRunCnt[7];
    HI_U32 aulDropTaskCnt[3];
    HI_U32 aulLFTaskRunStat[4];
} DIAG_CMD_SAL_DIAG_SDM_S;
//*****************************************************************************

//*****************************************************************************
// 最大支持的模块个数
#define SYS_DFX_MOD_INIT_MAX  24

typedef struct
{
    HI_DSID_SYS_MOD_INIT_S astModInit[SYS_DFX_MOD_INIT_MAX];
    HI_U8  aucFailId[32];
    HI_U32 aulDiag[16];
    HI_U32 aulDms[24];
    HI_U32 aulErr[16];
    HI_U16 usHwChipType;
    HI_U16 usHwProductType;
    HI_U32 ulCurrentTimeStamp;
    HI_U32 ulOutcomingTrafficTime[3]; // tcp and mac
    HI_U32 aulEnableInt[2];
    HI_U32 aulDisableInt[2];
    HI_U32 aulSalEnableInt[2];
    HI_U32 aulSalDisableInt[2];
    HI_U32 aulDmsLFStat[2];
    HI_U32 aulUserDataStat[8];
    HI_U32 aulData[4];
    HI_U32 aulSysRebootCnt;
} DIAG_CMD_SAL_SYS_SDM_S;
//*****************************************************************************

typedef struct
{
    HI_U32 swt; // 1: Open;
} DIAG_CMD_DIAG_DFX_SET_LC_SWT_STRU;

typedef struct
{
    HI_U8  ip_addr[4];
    HI_U8  net_mask[4];
    HI_U8  mac_addr[8]; // 2bytes to padding
} DIAG_CMD_ETH_CONFIG_STRU;
//*****************************************************************************


//*****************************************************************************
#define HI_DIAG_VER_PRODUCT_NAME_MAX_SIZE              30
#define HI_DIAG_VER_PRODUCT_BUILD_DATE_MAX_SIZE        10
#define HI_DIAG_VER_PRODUCT_BUILD_TIME_MAX_SIZE        8
#define HI_DIAG_VER_PRODUCT_BUILD_HINT_MAX_SIZE        8
#define HI_DIAG_VER_PRODUCT_BUILD_SPACE1_MAX_SIZE      1
#define HI_DIAG_VER_PRODUCT_BUILD_SPACE2_MAX_SIZE      1
#define HI_DIAG_VER_PRODUCT_BUILD_SEP_MAX_SIZE         1
#define HI_DIAG_VER_PRODUCT_BUILD_TYPE_SIZE            1
#define HI_DIAG_VER_PRODUCT_BUILD_OS_SIZE              1
#define HI_DIAG_VER_FULL_PRODUCT_NAME_MAX_SIZE         64
#define HI_DIAG_VER_FULL_NV_VER_NAME_MAX_SIZE          32
#define HI_DIAG_VER_FULL_HW_INFO_MAX_SIZE              32
#define HI_DIAG_VER_FULL_DEV_VER_NAME_MAX_SIZE         100
#define HI_DIAG_VER_PRODUCT_BUILD_SEP3_MAX_SIZE        1
#define HI_DIAG_VER_PRODUCT_BUILD_WD_SIZE              1
#define HI_DIAG_VER_CHIP_NAME_MAX_SIZE                 4

//精简结构，远程查询版本号命令(mini_ver)结构体，成员由VER结构体提取
typedef struct
{
    HI_CHAR Product[HI_DIAG_VER_FULL_PRODUCT_NAME_MAX_SIZE];
    HI_CHAR DEV[HI_DIAG_VER_FULL_DEV_VER_NAME_MAX_SIZE];
    HI_CHAR BOOT[64];
	HI_CHAR NV[HI_DIAG_VER_FULL_NV_VER_NAME_MAX_SIZE];
}DIAG_CMD_SOFT_NEW_VER_STRU;

typedef struct
{
    HI_CHAR Product[HI_DIAG_VER_FULL_PRODUCT_NAME_MAX_SIZE];
    HI_CHAR Modem[8];
    HI_CHAR DEV[HI_DIAG_VER_FULL_DEV_VER_NAME_MAX_SIZE];
    HI_CHAR BOOT[64];
    HI_CHAR HW[HI_DIAG_VER_FULL_HW_INFO_MAX_SIZE];
    HI_CHAR OS[36];
    HI_CHAR OSA[32];
    HI_CHAR SAL[36];
    HI_CHAR DRV[36];
    HI_CHAR PHY[36];
    HI_CHAR MAC[36];
    HI_CHAR APP[36];
} DIAG_CMD_SOFT_VER_STRU;


#define HI_USER_VER_LENGTH		(32)
#define HI_MANU_CODE_LENGTH		(3)
#define HI_CHIP_CODE_LENGTH		(3)
#define HI_USER_DATE_LENGTH		(3)

typedef struct
{
    HI_U8 ucInterfaceVer;
    HI_U8 ucStruSize;
    HI_CHAR szUserVersion[HI_USER_VER_LENGTH];
    HI_U16 usSwVersion;
    HI_CHAR szManuCode[HI_MANU_CODE_LENGTH];
    HI_CHAR szChipCode[HI_CHIP_CODE_LENGTH];
    HI_U8 aucDate[HI_USER_DATE_LENGTH];
    HI_U8 reserved[23];
} DIAG_CMD_USER_VER_IND_STRU;

typedef struct
{
    HI_U8 ucInterfaceVer;
    HI_U8 ucStruSize;
    HI_CHAR szSdkVersion[HI_DIAG_VER_FULL_PRODUCT_NAME_MAX_SIZE];
    HI_U8 reserved[10];
} DIAG_CMD_SDK_VER_IND_STRU;


//*****************************************************************************


//*****************************************************************************
//增加FEM命令所用到的结构体
typedef struct
{
    HI_U32  ftm;
} DIAG_CMD_FTM_CHANGE_STRU;

typedef struct
{
    HI_U32 ulId;
    HI_U8 aulData[80];
}HI_DCID_SET_CFG_S;

typedef enum
{
    HI_DIAG_CMD_CFG_UNKNOWN,
    HI_DIAG_CMD_CFG_SET_ALL_TCP_PORT_CLOSE,
    HI_DIAG_CMD_CFG_SET_DIAG_CHL,
    HI_DIAG_CMD_CFG_SET_CLEAN_DIAG_CHL_CNT,
    HI_DIAG_CMD_CFG_SET_CLEAN_DMS_CHL_CNT
}HI_DIAG_CMD_SAL_CFG_SET_E;

typedef struct
{
    HI_U32 ulCfgId;
    HI_U32 ulData;
}HI_DIAG_CMD_SAL_CFG_SET_S;
//*****************************************************************************

#ifdef PRODUCT_CFG_OS_NU
//*****************************************************************************
#define ERR_UNKNOWN_FUNC                                0
#define ERR_NU_Send_To_Mailbox                          1
#define ERR_NU_Create_Driver                            2
#define ERR_NU_Obtain_Semaphore                         3
#define ERR_POSIX_Thread_Cancel_Run                     4
#define ERR_posix_init_devices                          5
#define ERR_flockfile                                   6
#define ERR_NU_Release_Semaphore                        7
#define ERR_funlockfile                                 8
#define ERR_DMC_Create_Memory_Pool                      9
#define ERR_INCT_Sys_Mem_Pools_Initialize               10
#define ERR_NU_IPC_Allocate_Buffer                      11
#define ERR_NU_IPC_Deallocate_Buffer                    13
#define ERR_POSIX_SYS_FS_Ioctl                          14
#define ERR_POSIX_SYS_RTL_Malloc                        15
#define ERR_NU_Allocate_Memory                          16
#define ERR_NU_Delete_Queue                             17
#define ERR__posix_remove_message_queue                 18
#define ERR_NU_Retrieve_Events                          19
#define ERR_NETD_Init                                   20
#define ERR_NMI_Initialize                              21
#define ERR_NMI_MW_Registration                         22
#define ERR_NMI_MW_Initialization                       23
#define ERR_NU_IPC_Open                                 24
#define ERR_POSIX_SYS_FS_Open                           25
#define ERR_POSIX_SYS_PROC_Get_Pid                      26
#define ERR_FindTid                                     27
#define ERR__posix_thread_destructor                    28
#define ERR_POSIX_SYS_TC_Pthread_Exit                   29
#define ERR_POSIX_SYS_TC_Thread_Detach                  30
#define ERR_POSIX_SYS_TC_Pthread_Join                   31
#define ERR_NU_Semaphore_Information                    32
#define ERR_POSIX_SYS_TC_Pthread_Mutex_Destroy          33
#define ERR_NU_Create_Semaphore                         34
#define ERR_POSIX_SYS_TC_Pthread_Mutex_Init             35
#define ERR_POSIX_SYS_TC_Pthread_Mutex_Lock             36
#define ERR_pthread_mutex_trylock                       37
#define ERR_pthread_mutex_unlock                        38
#define ERR_NU_Task_Information                         39
#define ERR_sched_rr_get_interval                       40
#define ERR_NU_IPC_Receive                              41
#define ERR_POSIX_SYS_FS_Read                           42
#define ERR_POSIX_SYS_SM_Sem_Post                       43
#define ERR_SHD_Close_Session                           44
#define ERR_SHC_Shell                                   45
#define ERR_NU_Deallocate_Memory                        46
#define ERR_SHC_Logout                                  47
#define ERR_NU_SD_Init_Port                             48
#define ERR_SDC_Driver                                  49
#define ERR_NMI_Init_Status_Set                         50
#define ERR_SHELL_NMI_Init_Cb                           51
#define ERR_SHELL_Cleanup                               52
#define ERR_SHELL_Init                                  53
#define ERR_task_0                                      54
#define ERR_NU_Send_To_Queue                            55
#define ERR_POSIX_Signal_Handle                         56
#define ERR_TCCT_Schedule                               57
#define ERR_TCCT_Check_Stack                            58
#define ERR_TCC_Unhandled_Interrupt                     59
#define ERR_NU_Control_Timer                            60
#define ERR_POSIX_SYS_TIC_Timer_Delete                  61
#define ERR_POSIX_SYS_TIC_Tmr_Delete                    62
#define ERR_NU_Timer_Information                        63
#define ERR_POSIX_SYS_TIC_Timer_Gettime                 64
#define ERR_TMF_Get_Remaining_Time                      65
#define ERR_POSIX_SYS_TIC_Timer_Settime                 66
#define ERR_NU_Reset_Timer                              67
#define ERR_TCCT_Create_HISR                            68
#define ERR_TMIT_Initialize                             69
#define ERR_POSIX_SYS_FS_Write                          70
#define ERR_NU_IPC_Send                                 71
//*****************************************************************************
#endif // PRODUCT_CFG_OS_NU

//*****************************************************************************
#define HI_SYS_ERR_SIGNATURE   HI_MAKE_IDENTIFIER('s','y','s','E')
#define HI_SYS_ERR_VER                5
#define HI_SYS_CRASH_NUM_OFFSET       3
#define HI_SYS_REBOOT_NUM_OFFSET      4
#define HI_SYS_SDM_NUM_OFFSET         5
#define HI_SYS_RST_NUM_OFFSET         6
#define HI_SYS_BN_OFFSET              sizeof(DIAG_CMD_SDM_CTX_S)-12

#define HI_SYS_ESP_DATA_MAX_NUM       64
#define HI_SYS_RST_DATA_MAX_NUM       60
//*****************************************************************************

//*****************************************************************************
typedef struct
{
    HI_U32 tc_entry;
    HI_U32 tc_status;
    HI_U32 tc_stack_pointer;
    HI_U32 tc_scheduled;
    HI_U32 tc_stack_start;
    HI_U32 tc_stack_size;
}HI_SYS_TASK_INFO_S;

typedef struct
{
    HI_U32 q_id;
    HI_U32 q_max_num;
    HI_U32 q_used_num;
}HI_SYS_Q_INFO_S;

typedef struct
{
    HI_SYS_TASK_INFO_S tsk[HI_SYS_TSK_MAX_NUM];
    HI_SYS_TASK_INFO_S htsk[HI_SYS_HTSK_MAX_NUM];
    HI_SYS_Q_INFO_S    mq[HI_SYS_MQ_MAX_NUM];
    HI_SYS_Q_INFO_S    aq[HI_SYS_AQ_MAX_NUM];
}HI_SYS_OS_RES_BASIC_INFO_S;

#define HI_SYS_TSK_STK_DUMP_MAX_SIZE  68

typedef struct
{
    HI_U32 tc_entry;
    HI_U32 tstk[HI_SYS_TSK_STK_DUMP_MAX_SIZE];
}HI_SYS_TASK_TTSTK_INFO_S;

typedef struct
{
    HI_SYS_TASK_TTSTK_INFO_S tsk[HI_SYS_TSK_MAX_NUM];
    HI_SYS_TASK_TTSTK_INFO_S htsk[HI_SYS_HTSK_MAX_NUM];
}HI_SYS_OS_RES_TTSTK_INFO_S;

typedef struct
{
    HI_SYS_OS_RES_BASIC_INFO_S stBasicInfo;
    HI_SYS_OS_RES_TTSTK_INFO_S stTStkInfo;
}HI_SYS_OS_RES_INFO_S;
//*****************************************************************************
//精简结构，远程查询版本号命令(mini_syserr)结构体，成员由syserr结构体提取
typedef struct
{
    HI_U32  run_tsec;
    HI_S32  eid;
    HI_U32  rid;
    HI_CHAR tname[16];
    HI_U8   rst_data;
    HI_U8   reserved[3];
    HI_U32  crash_tsec;
    HI_CHAR prod_ver[SAL_BUILD_VER_PRODUCT_LEN_MAX];
}DIAG_CMD_NEW_SYSERR_QRY_STU;
//*****************************************************************************
#if (defined(PRODUCT_CFG_HAVE_FEATURE_SYS_ERR_INFO) && defined(SAL_SUPPORT_SYS_ERR_INFO_API)) || defined(PRODUCT_CFG_HSO)
typedef struct
{
    HI_U32  sig_s;              //0x0
    HI_U32  ver;                //0x4
    HI_U32  crash_num;          //0x8
    HI_U32  reboot_num;         //0xc
    HI_U32  sdm_num;            //0x10
    HI_U32  rst_num;            //0x14
    HI_U32  run_tsec;           //0x18
    HI_S32  eid;                //0x1c
    HI_U32  rid[2];             //0x20
    HI_S32  fid[2];             //0x28
    HI_S32  evn;                //0x30
    HI_U32  esp;                //0x34
    HI_U32  sp;                 //0x38
    HI_U32  epc;                //0x3c
    HI_S32  vn;                 //0x40
    HI_S32  type;               //0x44
    HI_U32  oerr[3];            //0x48
    HI_U32  tid;                //0x54
    HI_U32  tcsc;               //0x58
    HI_U32  tcsp;               //0x5c
    HI_CHAR tname[16];          //0x60
    HI_U32  tstatus;            //0x70
    HI_U32  crash_tsec;         //0x74
    HI_U32  rst_tsec;           //0x78
    HI_U32  tstksize;           //0x7c
    HI_U32  tstk[2];            //0x80
    HI_U32  dbg_data[20];       //0x88
    HI_U32  user_data[10];      //0xd8
    HI_U8   rst_data[HI_SYS_RST_DATA_MAX_NUM];  //0x100
    HI_CHAR err_exp[64];        //0x13c
    HI_CHAR err_file[64];       //0x17c
    HI_CHAR err_func[64];       //0x1bc
    HI_U32  err_ln;             //0x1fc
    HI_U32  val[4];             //0x200
    HI_U32  ttfa[16];           //0x210
    HI_U32  r[15];              //0x250
    HI_U32  cpsr;               //0x28c
    HI_CHAR prod_ver[SAL_BUILD_VER_PRODUCT_LEN_MAX];    //0x290
    HI_CHAR cfg_ver[SAL_BUILD_VER_PRODUCT_LEN_MAX];     //0x2c4
    HI_CHAR boot_ver[SAL_BUILD_VER_BOOT_LEN_MAX];       //0x2f8
    HI_U32  esp_data[HI_SYS_ESP_DATA_MAX_NUM];          //0x32c
    HI_CHAR buildtype[2];       //0x42c
    HI_CHAR ostype[2];          //0x42e
    HI_CHAR wd[2];              //0x430
    HI_U8   reserved[2];        //0x432
    HI_U32  wdctrl;             //0x434
    HI_U32  init_phase_no;      //0x438
    HI_U32  check_sum;          //0x43c
    HI_U32  bn;                 //0x440
    HI_U32  size;               //0x444
    HI_U32  sig_e;              //0x448
}DIAG_CMD_SDM_CTX_S;


//*****************************************************************************
#endif

#ifdef HI_SAL_DEV_DEBUG
#define HI_SAL_ASSERT(exp)  HI_SDM_VERIFY_EXPR(exp, 0, 0)
#else
#define HI_SAL_ASSERT(exp)  HI_ASSERT(exp)
#endif

#define HI_SYS_NDM_EVT_MAX   256
#if !defined(PRODUCT_CFG_HSO)
#if defined (HI_DEID_INIT_MAX) && (HI_SYS_NDM_EVT_MAX <= HI_DEID_INIT_MAX)
#error "HI_SYS_NDM_EVT_MAX <= HI_DEID_INIT_MAX"
#endif
#endif
typedef struct
{
    HI_U8  aucInitEvtId[HI_SYS_NDM_EVT_MAX];
    HI_U16 auInitEvtIdTime[HI_SYS_NDM_EVT_MAX];
    HI_U16 auInitEvtIdRc[HI_SYS_NDM_EVT_MAX];
} HI_SYS_EVT_SDM_S;
typedef struct
{
    HI_U16 exe_high;
    HI_U16 exc_high;
}DIAG_CMD_SAL_STACK_PEEK_INFO_S;

typedef struct
{
     DIAG_CMD_SAL_STACK_PEEK_INFO_S stack_peek[PRODUCT_CFG_DFX_TASKNUM_MAX];
}DIAG_CMD_SAL_STACK_PEEK_SDM_S;

typedef struct
{
    HI_U32 ulMagic;
}DIAG_CMD_SDM_POWER_CUT_CTX_S;

typedef struct
{
    DIAG_CMD_SDM_CTX_S stSdmCtx;
    DIAG_CMD_SDM_POWER_CUT_CTX_S stPowerCutCtx; 
    HI_U8 pad[28];
    DIAG_CMD_SAL_SYS_SDM_S stSysCtx;
    DIAG_CMD_SAL_DIAG_SDM_S stDiagCtx;
    HI_SYS_EVT_SDM_S stEvtCtx;

}SDM_SYS_ERR_CTX_S;


#define HI_SYS_EVT_SDM_INIT_DEFINED1 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
#define HI_SYS_EVT_SDM_INIT_DEFINED  {HI_SYS_EVT_SDM_INIT_DEFINED1,HI_SYS_EVT_SDM_INIT_DEFINED1}

//*****************************************************************************
#define DIAG_CMD_SDM_CTX_PADING                 32
#define DIAG_CMD_SDM_CTX_MAX_SIZE               (sizeof(DIAG_CMD_SDM_CTX_S) + DIAG_CMD_SDM_CTX_PADING)
#define SAL_SP_DATA_DEFINED                     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
#define SAL_CHECK_SYS_ERR_INIFO_MAX_SIZE()      HI_SAL_ASSERT((sizeof(DIAG_CMD_SDM_CTX_S) <= PRODUCT_CFG_FLASH_BLOCK_SIZE))
#define SAL_CRASH_INFO_MAX_SIZE                 (PRODUCT_CFG_FLASH_BLOCK_SIZE)
#define SAL_CRASH_HRD_INFO_MAX_SIZE             (PRODUCT_CFG_FLASH_BLOCK_SIZE)
#define SAL_CRASH_STK_DUMP_MAX_SIZE             (PRODUCT_CFG_FLASH_BLOCK_SIZE)
#define SAL_CRASH_DATA_MAX_SIZE                 (PRODUCT_CFG_FLASH_BLOCK_SIZE)
#define SAL_SYS_POWER_CUT_INFO_PTR()            (&g_stPowerCutStatus)

#if defined(PRODUCT_CFG_TASK_INT_SYS_ERR_INFO)&&defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#define SAL_CRASH_DATA2_MAX_SIZE                (PRODUCT_CFG_FLASH_BLOCK_SIZE)
#endif
#if defined(PRODUCT_CFG_HAVE_FEATURE_SYS_ERR_INFO)
#define SAL_SYS_ERR_POWER_CUT_PTR()             ((DIAG_CMD_SDM_POWER_CUT_CTX_S*)(&g_aucSysErrVal[HI_FIELDOFFSET(SDM_SYS_ERR_CTX_S,stPowerCutCtx)]))    
#define SAL_SYS_ERR_PTR()                       ((DIAG_CMD_SDM_CTX_S*)(&g_aucSysErrVal[0]))
#define SAL_SET_SYS_ERR_PTR_M(m,v)              (SAL_SYS_ERR_PTR()->m = v)
#define SAL_CPY_SYS_ERR(m,v)                    (hi_void)memcpy_s(SAL_SYS_ERR_PTR()->m, sizeof(SAL_SYS_ERR_PTR()->m), v, sizeof(SAL_SYS_ERR_PTR()->m));
#else
#define SAL_SET_SYS_ERR_PTR_M(m,v)
#define SAL_CPY_SYS_ERR(m,v)
#endif
#if defined(PRODUCT_CFG_TASK_INT_SYS_ERR_INFO)&&defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#define SAL_TASK_INT_SYS_ERR_PTR()              (&g_Isr_stat)
#endif
#define SAL_SYS_INIT_NO(x)                       SAL_SET_SYS_ERR_PTR_M(init_phase_no,x)
#define SAL_SYS_INIT_TID(x)
#define SAL_SYS_INIT_MODE0(x)
#define SAL_SYS_INIT_MODE1(x)

#define SAL_SYS_USED_MAX_SIZE                   DIAG_CMD_SDM_CTX_MAX_SIZE + sizeof(DIAG_CMD_SAL_SYS_SDM_S) + sizeof(DIAG_CMD_SAL_DIAG_SDM_S) + sizeof(HI_SYS_EVT_SDM_S) + sizeof(DIAG_CMD_SAL_STACK_PEEK_SDM_S)

#define SAL_SYS_BUFFER_SYS_SDM_OFFSET          (DIAG_CMD_SDM_CTX_MAX_SIZE)
#define SAL_SYS_BUFFER_DIAG_SDM_OFFSET         (SAL_SYS_BUFFER_SYS_SDM_OFFSET  + sizeof(DIAG_CMD_SAL_SYS_SDM_S))
#define SAL_SYS_BUFFER_EVT_OFFSET              (SAL_SYS_BUFFER_DIAG_SDM_OFFSET + sizeof(DIAG_CMD_SAL_DIAG_SDM_S))
#define SAL_SYS_STACK_PEEK_OFFSET              (SAL_SYS_BUFFER_EVT_OFFSET + sizeof(HI_SYS_EVT_SDM_S))

#define SAL_SDM_VAL_SIZE()                     DIAG_CMD_SDM_CTX_MAX_SIZE+sizeof(DIAG_CMD_SAL_SYS_SDM_S)+sizeof(DIAG_CMD_SAL_DIAG_SDM_S)+sizeof(HI_SYS_EVT_SDM_S) + sizeof(DIAG_CMD_SAL_STACK_PEEK_SDM_S)
#define SAL_CHK_VALID_CRASH_DATA(pstCtx)       ((pstCtx->sig_s == HI_SYS_ERR_SIGNATURE) && (pstCtx->sig_e == HI_SYS_ERR_SIGNATURE) && (pstCtx->size == sizeof(DIAG_CMD_SDM_CTX_S)))
#ifdef HI_SAL_DEV_DEBUG
#define CHECK_SYS_VAL_SIZE()                    HI_SDM_VERIFY_EXPR((SAL_SDM_VAL_SIZE() < PRODUCT_CFG_FLASH_BLOCK_SIZE), SAL_SDM_VAL_SIZE(), 0)
#else
#define CHECK_SYS_VAL_SIZE()                    HI_SDM_VERIFY_EXPR((SAL_SDM_VAL_SIZE() < PRODUCT_CFG_FLASH_BLOCK_SIZE), SAL_SDM_VAL_SIZE(), 0)
#endif
//*****************************************************************************


//*****************************************************************************
// sal inside interface.
#if defined(PRODUCT_CFG_HAVE_FEATURE_SYS_ERR_INFO)
HI_EAPI HI_VOID HI_SYS_SystemError(HI_VOID);
HI_EAPI HI_VOID HI_SYS_SaveRstInfo(HI_IN HI_U8 aulRst[HI_SYS_RST_DATA_MAX_NUM]);
#else
HI_PRVL HI_VOID HI_SYS_SystemError(HI_VOID){};
HI_PRVL HI_VOID HI_SYS_SaveRstInfo(HI_IN HI_U8 aulRst[HI_SYS_RST_DATA_MAX_NUM]) {}
#endif
//*****************************************************************************


//*****************************************************************************
typedef struct
{
    HI_U32 demo_32_011;
    HI_U8  demo_8_012[4];
    HI_U32 demo_32_cnt013;
}HSO_SYS_MSG_DEMO1_STRU;

typedef struct
{
    HI_U32 demo_32_021;
    HI_U8  demo_8_022;
    HI_U8  demo_8_023[3];
    HI_U32 demo_32_cnt024;
}HSO_SYS_MSG_DEMO2_STRU;

typedef struct
{
    HI_U32 demo_32_011;
    HI_U8  demo_8_012[4];
    HI_U32 demo_32_cnt013;
}HSO_LAYER_MSG_DEMO1_STRU;

typedef struct
{
    HI_U32 demo_32_021;
    HI_U8 demo_8_022;
    HI_U8 demo_8_023;
    HI_U8 demo_8_024;
    HI_U8 demo_8_025;
    HI_U32 demo_32_cnt026;
}HSO_LAYER_MSG_DEMO2_STRU;

typedef struct
{
    HI_U16 status;
    HI_U8 enable;
    HI_U8 flag;
    HI_U32 size;
    HI_U32 cnt;
} HSO_DEMO2_STRU;

//*****************************************************************************

typedef struct
{
    HI_U32 data0;
}DIAG_LOG1_MSG_S;

typedef struct
{
    HI_U32 data0;
    HI_U32 data1;
}DIAG_LOG2_MSG_S;

typedef struct
{
    HI_U32 data0;
    HI_U32 data1;
    HI_U32 data2;
}DIAG_LOG3_MSG_S;

typedef struct
{
    HI_U32 data0;
    HI_U32 data1;
    HI_U32 data2;
    HI_U32 data3;
}DIAG_LOG4_MSG_S;

typedef struct
{
    HI_U32 data0;
    HI_U32 data1;
    HI_U32 data2;
    HI_U32 data3;
    HI_U32 data4;
}DIAG_LOG5_MSG_S;

typedef struct
{
    HI_U32 data0;
    HI_U32 data1;
    HI_U32 data2;
    HI_U32 data3;
}DIAG_LOG_MSG_S;
//*****************************************************************************

//****************************************************************************
// 参数说明参考  << Hi3911V100 抄控器软件接口设计说明书.docx >>
//*****************************************************************************

// APP/MAC的内存统计(模块/子模块级别)
typedef struct
{
    HI_U16 usModId;                       // 模块ID
    HI_U16 usFailCnt;
    HI_U32 ulStcRamUsed;                  // 该模块静态内存占用字节
    HI_U32 ulDynRamUsed;                  // 该模块动态内存占用字节
    HI_U32 ulDynRamUsedPeek;              // 占用动态内存大小的峰值
    HI_U32 ulDynRamUsedPeekTime;          // 记录到 ulRamUsedPeek 的时间戳
    HI_U32 ulDynRam[2];
    HI_U32 aulDynRamAllocFreeCnt[2];      // 分配/释放计数
    HI_U32 aulDynRamAllocFreeFailCnt[2];  // 分配/释放错误次数
    HI_U32 aulDynRamAllocLast[2];         // 最后一次分配内存大小和时间(ms)
} HI_DBG_MEM_MOD_STAT_IND_S;

// APP/MAC的内存统计(系统级别)
typedef struct
{
    HI_U32 aulStcRam[4];                  // 系统静态内存初始大小, 系统静态内存剩余字节, 实际占用大小
    HI_U32 aulDynRam[3];                  // 系统动态内存初始大小, 系统动态内存剩余字节, 实际占用大小
    HI_U32 ulDynRamUsedPeek;              // 系统动态内存占用大小的峰值
    HI_U32 ulDynRamUsedPeekTime;          // 记录到 ulRamUsedPeek 的时间戳
    HI_U32 aulDynRamAllocFreeCnt[2];      // 系统动态内存分配/释放计数
    HI_U32 aulDynRamAllocFreeFailCnt[2];  // 系统动态内存分配/释放错误次数
} HI_DBG_MEM_SUM_STAT_IND_S;


typedef struct
{
    HI_U32 id;
} HI_DBG_STAT_Q_S;


typedef struct
{
    HI_U32 ulOption;
} HI_DBG_DUMP_DIAG_ITEMS_REQ_S;

typedef struct
{
    HI_U32 ulOption;
    HI_CHAR szName[64];
} HI_DBG_DFX_TASK_TEST_S;


/*单板与HSO交互TCP层结构和定义 ST*/
typedef struct
{
	HI_U32 magicNum;//0x54635072
	HI_U32 attribute;
	HI_U16 cmdId;
	HI_U16 unSupportDiag;//0xEEEE考虑将来兼容性，以后由于扩展该结构传递给DIAG层，DIAG层根据报文格式丢弃该报文
	HI_U32 crc;
	HI_U16  reserver;
	HI_U16 dataSize;
    HI_U8 data[0];
} HI_TCP_PACKET_REQ_S;//最小要大于28字节
#define HI_TCP_REQ_MAGIC_NUM 0x54635072
#define HI_TCP_REQ_ID_GRAB 0x0001
#define HI_TCP_REQ_UNSUPPORT_DIAG 0xEEEE
/*单板与HSO交互TCP层结构和定义 END*/

/************************************************HSO安全连接 START************************/
typedef struct
{
    HI_U8 challenge[CHANLLENGE_COMMON_SIZE];//可以没有，单板和HSO预先协商好。
    HI_U8 usOldPwdCP[CHANLLENGE_COMMON_SIZE];//旧密码CP值 
    HI_U8 usNewPwdSTK[CHANLLENGE_COMMON_SIZE];//新密码STK
    HI_U32 crc;//新密码的CRC，0表示不支持CRC校验。
    HI_U8 reserved[4];
} HI_PWD_MODIFY_REQ_S;


#define HI_PWD_MODIFY_ERR_OK 0//修改密码成功
#define HI_PWD_MODIFY_ERR_SYS_CALL 1//系统错误
#define HI_PWD_MODIFY_ERR_NOT_SUPPORT 2//不支持修改密码
#define HI_PWD_MODIFY_ERR_PASS_WORD_WRONG 3//原密码错误
#define HI_PWD_MODIFY_ERR_ACCESS_ACCOUNT_LOCK 4//账号锁定
#define HI_PWD_MODIFY_ERR_CHALLENGE_WRONG 5//密码校验序列不正确(或过期)

typedef struct
{
    HI_U32 errId;//HI_PWD_MODIFY_ERR_XXX
    HI_U32 data[5];//当errId为HI_PWD_MODIFY_ERR_PASS_WORD_WRONG时data[0]表示还有几次机会账号被锁定(0xFFFFFFFF表示不支持锁定)；当errId为HI_PWD_MODIFY_ERR_ACCESS_ACCOUNT_LOCK时data[0]表示账号剩余锁定时间(单位s)，其它情况无意义
} HI_PWD_MODIFY_IND_S;
typedef struct
{
    HI_U32 pad;//
}HI_PWD_MODIFY_GET_CHALLENGE_REQ_S;//获取密码校验序列REQ结构体

typedef struct
{
    HI_U32 ret;//错误码
    HI_U8 challenge[CHANLLENGE_COMMON_SIZE];//密码校验序列
    HI_U8 ver;
    HI_U8 pad;
    HI_U16 cnt; //迭代次数
    HI_U8 salt[CHANLLENGE_SALT_SIZE]; //盐值128bit

    HI_U16 usNewCnt;//新迭代次数
    HI_U8 ucNewSalt[CHANLLENGE_SALT_SIZE];//新盐值
    HI_U16 pad1;
    
    HI_U32 crc;
} HI_PWD_MODIFY_GET_CHALLENGE_IND_S;//获取密码校验序列IND结构体

typedef struct
{
    HI_U8 aucRand[32];//用来存储challenge的随机数
    HI_U8 verNum;//默认写0x1 
    HI_U8 reserved;
    HI_U16 usCnt;		// 计算口令存储信息的PBKDF2迭代次数，旧版本此位置为0
    HI_U8  aucSalt[16];		// 计算口令存储信息的盐值，旧版本此位置为0
    HI_U32 aulHashCrc;//保持最后一个，计算前面成员的CRC校验值。
} HI_TCP_AUTH_CHALLENGE_S;  //发送挑战值接口




/************************************************HSO安全连接 END************************/
HI_U32 HI_SYS_ERR_CLear(HI_VOID);
HI_U32 HI_SYS_ERR_Init(HI_VOID);

//*****************************************************************************
#endif //__DFX_SAL_H__

