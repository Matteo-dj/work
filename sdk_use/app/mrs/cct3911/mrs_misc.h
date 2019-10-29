/**
 ******************************************************************************
 * @file       mrs_misc.h
 * @brief      通道头文件
 * @details    This file including all API functions's declare of mrs_misc.h.
 * @copyright 
 ******************************************************************************
 */

#ifndef _MRS_MISC_H_
#define _MRS_MISC_H_ 

#include "hi_types.h"

/*-----------------------------------------------------------------------------
 Section: Macro Definitions
 ----------------------------------------------------------------------------*/
//正确返回值
#undef OK
#define OK	0

#define MISC_RPT_NUM                8       //上报缓冲报文数量
#define MISC_RPT_CNT                3       //上报次数
#define MISC_RPT_BUF_LEN            40      //上报报文长度

#define CLT_FNAME_SIZE              8

#define PARAM_CLT_ID                0x8010
#define PARAM_TEI_ID                0x8011
#define PARAM_IP_ID                 0x8012

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
/*  信号量资源*/
typedef enum
{
    MISC_SEM_MBUF = 0, 
    MISC_SEM_COLLECT, 
    MISC_SEM_MAX,
} mrs_misc_sem_e;

/*  消息队列资源*/
typedef enum
{
    MISC_QUEUE_CHAN = 0, 
    MISC_QUEUE_MAX,
} mrs_misc_queue_e;

/*  业务信息记录结构体*/
typedef struct
{
    HI_U8  bsotype;
    HI_U8  n;           /**<检测次数*/
    HI_U16 cmdId;
    HI_U32 t;
    HI_U32 timeout;
} mrs_misc_bsn_t;

typedef enum
{
    BAT_UPG_START = 0,  //升级启动
    BAT_UPG_STATE,      //查询升级状态
    BAT_UPG_SUCCESS,    //升级成功
    BAT_UPG_FAIL,       //升级失败
} mrs_misc_batupg_e;

/* 上报信息类型*/
typedef enum
{
    MISC_RPT_REBOOT = 0,
    MISC_RPT_TF,
    MISC_RPT_WRTIME,
    MISC_RPT_RDTIME,
    MISC_RPT_BATUPG,
    MISC_RPT_BATSTAT,
    MISC_RPT_NTB,
    MISC_RPT_MAX,
} mrs_misc_rpt_e;

/* 上报信息缓冲区*/
typedef struct
{
    HI_U8 chantype;
    HI_U8 rpt_type;
    HI_U8 rpt_cnt;
    HI_U8 len;
    HI_U8 t;
    HI_U8 buf[MISC_RPT_BUF_LEN];
} mrs_misc_rptbuf_t;

/* 上报信息记录*/
typedef struct
{
    HI_U8 cur_wpos;
    HI_U8 cur_rpos;
    HI_U8 flag[MISC_RPT_NUM];
    mrs_misc_rptbuf_t rptbuf[MISC_RPT_NUM];
} mrs_misc_rpt_t;

/*采数信息*/
typedef struct
{
    HI_U8 clt_fname[CLT_FNAME_SIZE];    /**< 采数文件名*/
    HI_U8 agc;                          /**< AGC增益*/     
    HI_U8 res[3];
} collect_info_t;

/* 参数结构体*/
typedef struct
{
    collect_info_t clt_info;            /**< 采数信息*/               
    HI_U16 tei;                         /**< tei号*/  
    HI_U8 ip;                           /**< ip */  
    HI_U8 res;
} mrs_misc_param_t;

/*-----------------------------------------------------------------------------
 Section: Global Variables
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Function Prototypes
 ----------------------------------------------------------------------------*/
void Mrs_Misc_BatUpg_set(HI_U8 state);
HI_U8 Mrs_Misc_BatUpg_get(void);

extern void  
Mrs_Misc_Bsn_Clr(HI_U8 bsotype);

extern void  
Mrs_Misc_Bsn_Set(HI_U8 bsotype, HI_U16 cmdId, HI_U32 t, 
    HI_U32 timeout);

extern mrs_misc_bsn_t* 
Mrs_Misc_Bsn_Get(HI_U8 bsotype);

extern void 
Mrs_Misc_Bsn_Check(HI_U8 bsotype);

HI_BOOL Mrs_Misc_GetConnectStat(HI_VOID);

HI_U32 Mrs_Misc_BqUpg(HI_VOID);

HI_U32 Mrs_Misc_CollectNtb(void);

extern void 
Mrs_Misc_Ntb_Check(void);

extern void 
Mrs_Misc_Check_Time_Set(void);

extern void 
Mrs_Misc_Time_Check(void);

extern void 
Mrs_Misc_Time_Request_Set(HI_U32 t);
extern void 
Mrs_Misc_Time_Request(void);

HI_BOOL 
Mrs_Misc_LinkTo9G25(void);

extern void 
Mrs_Misc_Reboot_Report(void);

extern HI_U32 
Mrs_Misc_Sem_Wait(mrs_misc_sem_e n, HI_U32 ulTimeOut);

extern HI_U32 
Mrs_Misc_Sem_Signal (mrs_misc_sem_e n);

extern HI_U32 
Mrs_Misc_Msg_Send(mrs_misc_queue_e n, HI_SYS_QUEUE_MSG_S* pstMsg, 
    HI_U32 ulTimeOut);

extern HI_U32 
Mrs_Misc_Msg_Wait(mrs_misc_queue_e n, HI_SYS_QUEUE_MSG_S* pstMsg, 
    HI_U32 ulTimeOut);

HI_U32 Mrs_Misc_MallocMem(HI_U32 ulSize);
HI_U32 Mrs_Misc_FreeMem(HI_VOID);

extern HI_U32 
Mrs_Misc_QueryTfStatusEnable(void);

extern HI_U32 
Mrs_Misc_QueryTfStatusDisable(void);

extern HI_U8 
Mrs_Misc_Collect_MainPhase_Get(void);

extern HI_U8 
Mrs_Misc_Collect_StartType_Get(void);

extern void  
Mrs_Misc_Collect_AgcGain_Set(HI_S32 AgcGain);

extern HI_S32  
Mrs_Misc_Collect_AgcGain_Get(void);

extern HI_U32 
Mrs_Misc_Collect_Real(void);

extern HI_U32 
Mrs_Misc_Collect_Save_Start(void);

extern HI_U32 
Mrs_Misc_Collect_Save_Stop(void);

HI_U32 Mrs_Misc_SendSignalStart(HI_VOID);
HI_U32 Mrs_Misc_SendSignalToMode0(HI_VOID);
HI_U32 Mrs_Misc_SendSignalToMode1(HI_VOID);
HI_U32 Mrs_Misc_SendSignalToMode2(HI_VOID);

extern void 
Mrs_Misc_Reboot(void);

HI_U8 
Mrs_Misc_Rpt_Cnt(HI_U8 rpt_type);

extern void 
Mrs_Misc_Rpt_Clr(HI_U8 n);

extern HI_U32 
Mrs_Misc_Rpt_Write(HI_U8 chantype, HI_U8 rpt_type, 
    HI_U8* buf, HI_U8 len);

extern mrs_misc_rptbuf_t*  
Mrs_Misc_Rpt_Read(HI_U8 n);

HI_U16 Mrs_Misc_Tei_Get(void);
HI_S16 Mrs_Misc_Tei_Set(HI_U16 tei);

HI_U8 Mrs_Misc_Ip_Get(void);
HI_S8 Mrs_Misc_Ip_Set(HI_U8 ip);

extern int mrs_misc_init(void);

#endif /* _MRS_MISC_H_ */

/*----------------------------End of mrs_misc.h-------------------------------*/
