/**
 ******************************************************************************
 * @file       mrs_bso_report.c
 * @version   
 * @brief
 * @details  
 *
 * @copyright  
 ******************************************************************************
 */

#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined (PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_CCT3911)

/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include "hi_mdm_time.h"
#include "mrs_cfg.h"

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
typedef enum
{
    BSO_REPORT_NONE = 0, 
    BSO_REPORT_RUN, 
} mrs_bso_rpt_state_e;

typedef HI_U16 (*bso_rpt_run)(mrs_misc_rptbuf_t*, HI_U16, HI_U8*);
typedef void (*bso_rpt_proc)(HI_U8, HI_U8*);

typedef struct
{
    HI_U8 type;
    HI_U16 cmdId;
    bso_rpt_run runfun;
    bso_rpt_proc procfun;
} mrs_bso_rpt_t;

/*-----------------------------------------------------------------------------
 Section: Global Variables
 ----------------------------------------------------------------------------*/
/* None */

/*-----------------------------------------------------------------------------
 Section: Local Variables
 ----------------------------------------------------------------------------*/
HI_U16 bso_rpt_run_comm1(mrs_misc_rptbuf_t* rptbuf, HI_U16 cmdId, HI_U8* wrbuf);
HI_U16 bso_rpt_run_comm2(mrs_misc_rptbuf_t* rptbuf, HI_U16 cmdId, HI_U8* wrbuf);
HI_U16 bso_rpt_run_wrtime(mrs_misc_rptbuf_t* rptbuf, HI_U16 cmdId, HI_U8* wrbuf);

void bso_rpt_proc_rdtime(HI_U8 rpt_sn, HI_U8* data);
void bso_rpt_proc_batupg(HI_U8 rpt_sn, HI_U8* data);
void bso_rpt_proc_batstat(HI_U8 rpt_sn, HI_U8* data);

mrs_bso_rpt_t the_bso_rpt[MISC_RPT_MAX] = {
    {MISC_RPT_REBOOT,  BSO_CMD_REPORT_REBOOT,  bso_rpt_run_comm1,  NULL},
    {MISC_RPT_TF,      BSO_CMD_REPORT_TF,      bso_rpt_run_comm2,  NULL},
    {MISC_RPT_WRTIME,  BSO_CMD_REPORT_WRTIME,  bso_rpt_run_wrtime, NULL},
    {MISC_RPT_RDTIME,  BSO_CMD_REPORT_RDTIME,  bso_rpt_run_comm1,  bso_rpt_proc_rdtime},
    {MISC_RPT_BATUPG,  BSO_CMD_REPORT_BATUPG,  bso_rpt_run_comm1,  bso_rpt_proc_batupg},
    {MISC_RPT_BATSTAT, BSO_CMD_REPORT_BATSTAT, bso_rpt_run_comm1,  bso_rpt_proc_batstat},
    {MISC_RPT_NTB,     BSO_CMD_REPORT_NTB,     bso_rpt_run_comm2,  NULL},
};

/*-----------------------------------------------------------------------------
 Section: Local Function Prototypes
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 Section: Function Definitions
 ----------------------------------------------------------------------------*/
HI_U16 bso_rpt_run_comm1(mrs_misc_rptbuf_t* rptbuf, HI_U16 cmdId, HI_U8* wrbuf)
{
    HI_U16 wrlen;
    HI_U16 clen = 0;
    HI_U8* ctxt = wrbuf + PTL_TXT_OFT_HEAD;
    
    load_uint8(ctxt, &clen, BSO_RESP_OK);
    return mrs_ptl68_pack(mrs_ptl68_ctrlcode_set(1, 0, 
        BSO_TYPE_REPORT), cmdId, ctxt, clen, 
        wrbuf, BSO_WRBUF_LEN);
}

HI_U16 bso_rpt_run_comm2(mrs_misc_rptbuf_t* rptbuf, HI_U16 cmdId, HI_U8* wrbuf)
{
    HI_U16 wrlen;
    HI_U16 clen = 0;
    HI_U8* ctxt = wrbuf + PTL_TXT_OFT_HEAD;
    
    load_buf(ctxt, &clen, rptbuf->buf, rptbuf->len);
    return mrs_ptl68_pack(mrs_ptl68_ctrlcode_set(1, 0, 
        BSO_TYPE_REPORT), cmdId, ctxt, clen, 
        wrbuf, BSO_WRBUF_LEN);
}

HI_U16 bso_rpt_run_wrtime(mrs_misc_rptbuf_t* rptbuf, HI_U16 cmdId, HI_U8* wrbuf)
{
    HI_U16 clen = 0;
    HI_U8* ctxt = wrbuf + PTL_TXT_OFT_HEAD;
    
    HI_U32 t = HI_MDM_GetRealTime();
    load_uint32(ctxt, &clen, t);
    printf("[%s %u]: report time = %u\r\n",__func__,__LINE__,t);
    return mrs_ptl68_pack(mrs_ptl68_ctrlcode_set(1, 0, 
        BSO_TYPE_REPORT), cmdId, ctxt, clen, 
        wrbuf, BSO_WRBUF_LEN);
}

void bsoreport_run(int fd, mrs_bso_t *mrs_bso)
{
    int i;
    PACK_MALLOC1;
                
    mrs_misc_rptbuf_t* rptbuf = Mrs_Misc_Rpt_Read(mrs_bso->misc_use);
    if (!rptbuf)
    {
        mrs_bso_clr(mrs_bso);
        return;
    }

    for (i = 0;i < MISC_RPT_MAX;i ++)
    {
        if (rptbuf->rpt_type == the_bso_rpt[i].type)
        {
            wrlen = the_bso_rpt[i].runfun(rptbuf, 
                the_bso_rpt[i].cmdId, wrbuf);
            break;
        }
    }

    if (i >= MISC_RPT_MAX)
    {
        Mrs_Misc_Rpt_Clr(mrs_bso->misc_use);
        mrs_bso_clr(mrs_bso);
        return;
    }

    if (ERROR == mrs_bso_wr(fd, wrbuf, wrlen, 1, 2000, mrs_bso))
    {
        printf("[%s %u]: report type = %d cnt = %d\r\n",__func__,
            __LINE__,rptbuf->rpt_type,rptbuf->rpt_cnt);
        rptbuf->rpt_cnt ++;
        if (rptbuf->rpt_cnt >= Mrs_Misc_Rpt_Cnt(rptbuf->rpt_type))
        {
            Mrs_Misc_Rpt_Clr(mrs_bso->misc_use);
        }
        mrs_bso_clr(mrs_bso);
    }
}

void bso_rpt_proc_comm(HI_U8 rpt_sn, HI_U8* data)
{
    
}

void bso_rpt_proc_rdtime(HI_U8 rpt_sn, HI_U8* data)
{
    HI_U32 t;
    (hi_void)memcpy_s(&t, sizeof(HI_U32), data + 2, sizeof(HI_U32));
    Mrs_Misc_Time_Request_Set(t);
    //时间更新,需调用一次
    Mrs_Misc_Check_Time_Set();
    Mrs_Misc_Rpt_Clr(rpt_sn);
}

void bso_rpt_proc_batupg(HI_U8 rpt_sn, HI_U8* data)
{
    Mrs_Misc_BatUpg_set(BAT_UPG_STATE);
    printf("[%s %u]\r\n", __func__, __LINE__);
    Mrs_Misc_Rpt_Clr(rpt_sn);
}

void bso_rpt_proc_batstat(HI_U8 rpt_sn, HI_U8* data)
{
    if ((data[2] == BAT_UPG_SUCCESS) || (data[2] == BAT_UPG_FAIL))
    {
        Mrs_Misc_BatUpg_set(data[2]); 
        printf("[%s %u]\r\n", __func__, __LINE__);
    }
    Mrs_Misc_Rpt_Clr(rpt_sn);
}

/**
 ******************************************************************************
 * @brief      采数业务数据发送处理
 * @param[in]  fd ：句柄
 * @param[in]  mrs_bso ：业务对象
 * @retval     STATUS  ：OK-成功, ERROR-失败
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
static HI_S32 
mrs_bsoreport_run(int fd, mrs_bso_t *mrs_bso)
{
    switch (mrs_bso->run_state)
    {
        case BSO_REPORT_RUN:
            bsoreport_run(fd, mrs_bso);
            break;
            
        default:
            break;
    }
    
    return OK;
}

/**
 ******************************************************************************
 * @brief      采数业务数据接收处理
 * @param[in]  fd ：句柄
 * @param[in]  buf ：接收数据缓冲区
 * @param[in]  len ：接收数据长度
 * @param[in]  mrs_bso ：业务对象
 * @retval     STATUS  ：OK-成功, ERROR-失败
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
static HI_S32 
mrs_bsoreport_proc(int fd, HI_U8 *buf, int len, 
    mrs_bso_t *mrs_bso)
{
    int i;
    HI_U16 cmdId;
    HI_U8 *data = &buf[PTL_DATA_OFT_HEAD];
    
    (hi_void)memcpy_s(&cmdId, PTL_CMDID_SIZE, data, PTL_CMDID_SIZE);

    mrs_misc_rptbuf_t* rptbuf = Mrs_Misc_Rpt_Read(mrs_bso->misc_use);
    if (!rptbuf)
    {
        mrs_bso_clr(mrs_bso);
        return ERROR;
    }

    for (i = 0;i < MISC_RPT_MAX;i ++)
    {
        if ((cmdId == the_bso_rpt[i].cmdId) 
            && (rptbuf->rpt_type == the_bso_rpt[i].type))
        {
            if (the_bso_rpt[i].procfun)
            {
                the_bso_rpt[i].procfun(mrs_bso->misc_use, data);
            }

            Mrs_Misc_Rpt_Clr(mrs_bso->misc_use);
            mrs_bso_clr(mrs_bso);
            return OK;
        }
    }

    mrs_bso_clr(mrs_bso);
    return ERROR;
}

/**
 ******************************************************************************
 * @brief      采数业务对象实例化
 * @param[in]  mrs_bso_t *mrs_bso  ：业务对象
 * @retval     mrs_bso_t*  ：返回的业务对象
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
mrs_bso_t* 
mrs_bsoreport_getinstanc(mrs_bso_t* mrs_bso)
{
    mrs_bso->bsotype = BSO_TYPE_REPORT;

    mrs_misc_rptbuf_t* rptbuf = Mrs_Misc_Rpt_Read(mrs_bso->misc_use);
    if (rptbuf && (rptbuf->rpt_cnt == 0))
    {
        mrs_bso->t = HI_MDM_GetMilliSeconds();
        mrs_bso->wcnt = 0;
    }
    
    mrs_bso->run_state = BSO_REPORT_RUN;
    mrs_bso->proc_state = BSO_REPORT_RUN;
    
    mrs_bso->run = mrs_bsoreport_run;
    mrs_bso->proc = mrs_bsoreport_proc;

    return mrs_bso;
}

/*-------------------------------End of mrs_bso_report.c--------------------------------*/
#endif