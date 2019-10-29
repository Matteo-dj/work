/**
 ******************************************************************************
 * @file      mrs_bsn.c
 * @brief     业务处理
 * @details   This file including all API functions's implement of mrs_bsn.c.
 * @copyright 
 ******************************************************************************
 */
#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined (PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_CCT3911)
/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/

#include "hi_types.h"
#include "hi_key.h"
#include "hi_ft_nv.h"
#include "hi_share_memory.h"

#include "hi_mdm_sys.h"
#include "hi_mdm_task.h"
#include "hi_mdm_time.h"
//#include "hi_mdm_dc.h"
#include "hi_mdm_mac.h"

//#include "mrs_srv_ndm_collectdata.h"
#include "mrs_srv_ndm_sendsignal.h"
#include "mrs_fw_tools.h"
#include "mrs_common.h"
#include "mrs_fw_n.h"

#include "mrs_cfg.h"
#include "mrs_bsn.h"

/*-----------------------------------------------------------------------------
 Section: Constant Definitions
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 Section: Global Variables
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Local Variables
 ----------------------------------------------------------------------------*/
/* 业务对象处理列表 */
static mrs_bso_t *mrs_bso_list[CHAN_MAX][BSO_MAX_NUM] = {{NULL}, {NULL}};

/* 业务对象实例*/
static mrs_bso_t mrs_bso_inst[CHAN_MAX][BSO_MAX_NUM];

/*-----------------------------------------------------------------------------
 Section: Local Function Prototypes
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Global Function Prototypes
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Function Definitions
 ----------------------------------------------------------------------------*/
static HI_VOID mrs_bsn_recvmsg(HI_SYS_QUEUE_MSG_S queue_msg)
{
    mrs_bso_t *bso = NULL;
    mrs_chan_t *pchan = NULL;
    HI_U8 chantype;
    HI_U8 rpt_type;

    chantype = (queue_msg.ulMsgId & 0xFF) - 1;
    rpt_type = ((queue_msg.ulMsgId & 0xFF00) >> 8);
    if (rpt_type > 0)//主动上报
    {
        HI_U8 rpt_len = 0;
        rpt_type -= 1;
        if (MISC_RPT_TF == rpt_type)
        {
            rpt_len = sizeof(HI_MDM_QUERY_NDM2NDM_TF_STATUS_IND_STRU);
            Mrs_Misc_Rpt_Write(chantype, rpt_type, (HI_U8* )queue_msg.ulParam, 
                rpt_len);
        }

        printf("Msg recv: Chantype[%d] rpt type[%d] len[%d] ok\r\n", 
            chantype, rpt_type, rpt_len);

        return;
    }

    int len = queue_msg.ulParam[0];
    HI_U8* pbuf = (HI_U8* )queue_msg.ulParam[1];
    HI_U8 bsotype = PTL_BSOTYPE_GET(pbuf[1]);
    
    if ((chantype >= CHAN_MAX) || (bsotype >= BSO_TYPE_MAX) || 
        (bsotype == BSO_TYPE_BDCOLLECT))
    {
        //释放内存
        mlib_free(pbuf);
        return;
    }

    if (bsotype != BSO_TYPE_REPORT)
    {
        //获取业务处理对象
        bso = mrs_bsolist_get(mrs_bso_list[chantype], bsotype);

        //业务对像在列表中, 先删除
        if (bso)
        {
            mrs_bsolist_del(mrs_bso_list[chantype], bso);           
        }

        //创建对象
        mrs_bso_inst[chantype][bsotype].chantype = chantype;
        mrs_bsolist_ins(mrs_bso_list[chantype], mrs_bso_instance(bsotype, 
            &mrs_bso_inst[chantype][bsotype]));
    }

    bso = mrs_bsolist_get(mrs_bso_list[chantype], bsotype);
    if (bso)
    {
        //业务数据接收处理
        pchan = mrs_chan_ctx(chantype);
        bso->proc(pchan->fd, pbuf, len, bso);
    }

    //释放内存
    mlib_free(pbuf);
    return;
}

static HI_VOID mrs_bsn_report(void)
{
    int i = 0, j = 0;
    mrs_bso_t *bso = NULL;
    
    for (i = 0;i < CHAN_MAX;i ++)
    {
        bso = mrs_bsolist_get(mrs_bso_list[i], BSO_TYPE_REPORT);
        if (!bso)
        {
            mrs_misc_rptbuf_t* rptbuf = NULL;
            for (j = 0;j < MISC_RPT_NUM;j ++)
            {
                rptbuf = Mrs_Misc_Rpt_Read(j);
                if (!rptbuf)
                {
                    continue;
                }

                if (rptbuf->chantype == i)
                {
                    mrs_bso_inst[i][BSO_TYPE_REPORT].chantype = i;
                    mrs_bso_inst[i][BSO_TYPE_REPORT].misc_use = j;
                    mrs_bsolist_ins(mrs_bso_list[i], mrs_bso_instance(
                        BSO_TYPE_REPORT, &mrs_bso_inst[i][BSO_TYPE_REPORT]));
                    break;
                }
            }
        }
    }
}

static HI_VOID mrs_bsn_run(void)
{
    int i = 0, j = 0;
    mrs_bso_t *bso = NULL;
    mrs_chan_t *pchan = NULL;
    
    for (i = 0;i < CHAN_MAX;i ++)
    {
        for (j = 0;j < BSO_MAX_NUM;j ++)
        {
            bso = mrs_bsolist_get(mrs_bso_list[i], j);
            if (!bso)
            {
                continue;
            }

            pchan = mrs_chan_ctx(i);
            bso->run(pchan->fd, bso);

            //状态= 0 从业务列表中删除业务对象
            if ((bso->run_state == 0) && (bso->proc_state == 0))
            {
                mrs_bsolist_del(mrs_bso_list[i], bso);
            }
        }
    }
}

static HI_VOID mrs_bsn_static(void)
{
    int i = 0;
    for (i = 0;i < CHAN_MAX;i ++)
    {
        HI_U32 chan_static = mrs_chan_static_get(i);

        if (chan_static > 0)
        {
            printf("mrs_bsn: chan type[%d] static[%d]\r\n", i, 
                chan_static);
            mrs_chan_static_clr(i);
        }
    }
}

/**
 ******************************************************************************
 * @brief   协议处理任务
 * @return  None
 ******************************************************************************
 */
static HI_VOID mrs_bsn_task(HI_U32 ulParam)
{
    HI_U8 i, j;
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_SYS_QUEUE_MSG_S queue_msg = {0};
    mrs_bso_t *bso = NULL;
    mrs_chan_t *pchan = NULL;

    printf("mrs_bsn_task OK\n");

    for (;;)
    {
        //TODO 接收消息
        ret = Mrs_Misc_Msg_Wait(MISC_QUEUE_CHAN, &queue_msg, 100);
 
        if (HI_ERR_SUCCESS == ret)
        {
            mrs_bsn_recvmsg(queue_msg);
        }

        //检测是否有上报报文        
        mrs_bsn_report();
        
        //依次处理各通道业务列表中的所有业务
        mrs_bsn_run();

        Mrs_Misc_Ntb_Check();

        //检测上位机命令和实际动作是否一致
        //0. 时钟同步请求
        Mrs_Misc_Time_Request();
        
        //1. 台区识别命令
        
        //2. 存储采数命令
        Mrs_Misc_Bsn_Check(BSO_TYPE_COLLECT);
        
        //3. 信号发送命令
        Mrs_Misc_Bsn_Check(BSO_TYPE_SG);

        //4. 参数检测

        //5. 时钟检测
        Mrs_Misc_Time_Check();

        mrs_bsn_static();
    }
}

/**
 ******************************************************************************
 * @brief   初始化业务报文处理(BSN)模块
 * @retval  OK   : 成功
 * @retval  ERROR: 失败
 ******************************************************************************
 */
int 
mrs_bsn_init(void)
{
    HI_U32 TaskId;
    HI_MDM_TASK_Create(&TaskId, "NDM_BSN", mrs_bsn_task, 0, 2048, 26);
    return OK;
}

/*---------------------------------mrs_bsn.c-------------------------------------*/
#endif