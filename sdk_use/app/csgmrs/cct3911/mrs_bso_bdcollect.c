/**
 ******************************************************************************
 * @file       mrs_bso_bdcollect.c
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
 Section: Macro Definitions
 ----------------------------------------------------------------------------*/
#define BSO_FRAME_MAX_NUM            2400
#define BSO_TRANS_MAX_NUM            600

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
typedef enum
{
    BSO_BDCOLLECT_NONE = 0, 
    BSO_BDCOLLECT_RUN, 
    BSO_BDCOLLECT_TRANS,
} mrs_bso_bdclt_state_e;

typedef struct
{
    HI_U16 frm_num;                       /**< 帧总数*/
    HI_S16 trans_num;                     /**< 请求次数*/
    HI_S32 frm_sn;                   
    HI_U32 t;
    HI_U32 tv;
} bso_bdclt_buf_t;

/*-----------------------------------------------------------------------------
 Section: Global Variables
 ----------------------------------------------------------------------------*/
/* None */

/*-----------------------------------------------------------------------------
 Section: Local Variables
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 Section: Local Function Prototypes
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 Section: Function Definitions
 ----------------------------------------------------------------------------*/
static HI_S32 
bsobdcollect_trans(HI_S32 fd, mrs_bso_t* mrs_bso)
{
    bso_bdclt_buf_t *bso_buf = NULL;
    bso_buf = (bso_bdclt_buf_t *)mrs_bso->pbuf;

    if (bso_buf->trans_num <= 0)
    {
        printf("[mrs_bsobdcollect_run]: trans over\r\n");
        return ERROR;
    }

    if (bso_buf->frm_sn >= bso_buf->frm_num)
    {
        HI_U32 t = HI_MDM_GetMilliSeconds();
        if (abs(t - bso_buf->tv) >= 2000)
        {
            printf("[mrs_bsobdcollect_run]: wait timeout\r\n");
            return ERROR;
        }
        
        return OK;
    }

    int i = 0;
    HI_U16 wrlen = 0;
    HI_U16 packlen = 0, crc16 = 0;
    mrs_clt_info_t* clt_info = (mrs_clt_info_t* )mrs_bso->param;
    HI_BUF_S* HiBuf = (HI_BUF_S* )clt_info->hi_buf;
    HI_U8* wrbuf = mlib_malloc(TEMP_1024);
    HI_U8* pbuf = NULL;
    
    if (!wrbuf)
    {
        printf("[%s]: mlib_malloc error\r\n",__func__);
        return ERROR;
    }
 
    for (i = bso_buf->frm_sn;i < bso_buf->frm_num;i ++)
    {    
        int len = 0;
        packlen = 0;
        collect_data_pack(HiBuf, i, &pbuf, &packlen);

        wrlen = 0;
        load_uint8(wrbuf, &wrlen, PTL_HEAD);      //报文头
        load_uint8(wrbuf, &wrlen, mrs_ptl68_ctrlcode_set(1, 0, 
            BSO_TYPE_BDCOLLECT));      //控制码
        load_uint16(wrbuf, &wrlen, 10 + packlen);         //数据长度
        load_uint16(wrbuf, &wrlen, BSO_CMD_CLT_TRANS_TODO);        //报文命令
        load_uint32(wrbuf, &wrlen, bso_buf->t);
        load_uint16(wrbuf, &wrlen, bso_buf->frm_num);//帧总数
        load_uint16(wrbuf, &wrlen, i);//帧序号

        len = 14;
        if (mrs_chan_write(mrs_bso->chantype, fd, wrbuf, len) != len)
        {
             break;
        }

        (hi_void)memcpy_s(wrbuf + wrlen, 1024, pbuf, packlen);
        wrlen += packlen;

        crc16 = get_crc16(wrbuf, wrlen, 0xFFFF);
        load_uint16(wrbuf, &wrlen, crc16);        //cs
        load_uint8(wrbuf, &wrlen, PTL_END);       //报文尾

        len = packlen + 3;
        if (mrs_chan_write(mrs_bso->chantype, fd, wrbuf + 14, len) != len)
        {
             break;
        }
    }

    mlib_free(wrbuf);

    bso_buf->frm_sn = i;
    if (bso_buf->frm_sn >= bso_buf->frm_num)
    {
        HI_U32 t = HI_MDM_GetMilliSeconds();
        printf("[mrs_bsobdcollect_run]: trans t[%d] = %d ok \n", 
            bso_buf->trans_num, abs(t - bso_buf->t));
        bso_buf->tv = HI_MDM_GetMilliSeconds();
    }

    bso_buf->trans_num --;
    return OK;
}

/**
 ******************************************************************************
 * @brief      采数业务数据发送处理
 * @param[in]  fd ：句柄
 * @param[in]  pri ：业务对象
 * @retval     STATUS  ：OK-成功, ERROR-失败
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
static HI_S32 
mrs_bsobdcollect_run(HI_S32 fd, mrs_bso_t* mrs_bso)
{
    bso_bdclt_buf_t *bso_buf = NULL;
    
    if (!mrs_bso->pbuf)
    {
        mrs_bso_clr(mrs_bso);
        return ERROR;
    }

    bso_buf = (bso_bdclt_buf_t *)mrs_bso->pbuf;

    switch (mrs_bso->run_state)
    {  
        case BSO_BDCOLLECT_RUN:
            {
                PACK_MALLOC2;
                
                mrs_clt_info_t* clt_info = (mrs_clt_info_t* )mrs_bso->param;
                HI_BUF_S* HiBuf = (HI_BUF_S* )clt_info->hi_buf;

                load_uint32(ctxt, &clen, bso_buf->t);
                load_uint8(ctxt, &clen, clt_info->clt_type);
                load_uint8(ctxt, &clen, clt_info->file_type);
                load_uint32(ctxt, &clen, HiBuf->bufSize[0]);//bufsize0
                load_uint32(ctxt, &clen, HiBuf->bufSize[1]);//bufsize1

                wrlen = mrs_ptl68_pack(mrs_ptl68_ctrlcode_set(1, 0, 
                    BSO_TYPE_BDCOLLECT), BSO_CMD_CLT_TRANS_RUN, 
                    ctxt, clen, wrbuf, 100);

                if (ERROR == mrs_bso_wr(fd, wrbuf, wrlen, 1, 2000, mrs_bso))
                {
                    mrs_bso_clr(mrs_bso);
                    return ERROR;
                }
            }
            break;

        case BSO_BDCOLLECT_TRANS:
            if (bsobdcollect_trans(fd, mrs_bso) == ERROR)
            {
                mrs_bso_clr(mrs_bso);
                return ERROR;
            }
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
 * @param[in]  pri ：业务对象
 * @retval     STATUS  ：OK-成功, ERROR-失败
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
static HI_S32 
mrs_bsobdcollect_proc(HI_S32 fd, HI_U8 *buf, HI_S32 len, 
    mrs_bso_t* mrs_bso)
{
    //HI_U32 t;
    HI_U16 cmdId;
    HI_U8* data = &buf[PTL_DATA_OFT_HEAD];
    bso_bdclt_buf_t *bso_buf = NULL;
    
    if (!mrs_bso->pbuf)
    {
        mrs_bso_clr(mrs_bso);
        return ERROR;
    }
    
    bso_buf = (bso_bdclt_buf_t *)mrs_bso->pbuf;

    (hi_void)memcpy_s(&cmdId, PTL_CMDID_SIZE, data, PTL_CMDID_SIZE);
    //收到9G25的传输状态帧则直接退出
    if (cmdId == BSO_CMD_CLT_TRANS_STATE)
    {
        printf("mrs_bsobdcollect_proc: trans state[%d]\n", 
            buf[PTL_TXT_OFT_HEAD]);
        mrs_bso_clr(mrs_bso);
        return ERROR;
    }

    switch (mrs_bso->run_state)
    {
        case BSO_BDCOLLECT_RUN: 
            {
                if (cmdId == BSO_CMD_CLT_TRANS_RUN)
                {
                    bso_buf->trans_num = BSO_TRANS_MAX_NUM;
                    bso_buf->frm_sn = 0;
                    mrs_bso->run_state = BSO_BDCOLLECT_TRANS;
                }
                else
                {
                    printf("mrs_bsobdcollect_proc: run Fail\n");
                    mrs_bso_clr(mrs_bso);
                    return ERROR;
                }
            }
            break;
            
        case BSO_BDCOLLECT_TRANS: 
            {
                //TODO
            }
            break;

        default:
            break;
    }

    return OK;
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
mrs_bsobdcollect_getinstanc(mrs_bso_t* mrs_bso)
{   
    mrs_bso->bsotype = BSO_TYPE_BDCOLLECT;
    mrs_bso->t = HI_MDM_GetMilliSeconds();
    mrs_bso->wcnt = 0;

    mrs_bso->run_state = BSO_BDCOLLECT_RUN;
    mrs_bso->proc_state = BSO_BDCOLLECT_RUN;
    
    mrs_bso->run = mrs_bsobdcollect_run;
    mrs_bso->proc = mrs_bsobdcollect_proc;

    mrs_bso->pbuf = (HI_U8 *)mlib_malloc(TEMP_256);
    if (!mrs_bso->pbuf)
    {
        mrs_bso_clr(mrs_bso);
        return NULL;
    }

    mrs_clt_info_t* clt_info = (mrs_clt_info_t* )mrs_bso->param;
    HI_BUF_S* HiBuf = (HI_BUF_S* )clt_info->hi_buf;
    
    bso_bdclt_buf_t *bso_buf = NULL;
    bso_buf = (bso_bdclt_buf_t *)mrs_bso->pbuf;

    bso_buf->frm_num = collect_frm_num(HiBuf->bufSize, NULL, NULL);
    bso_buf->trans_num = 0;
    bso_buf->frm_sn = 0;
    bso_buf->t = HI_MDM_GetMilliSeconds();
    
    printf("mrs_bsobdcollect_getinstanc: frm_num = %d\n", 
        bso_buf->frm_num);
    
    return mrs_bso;
}

/*-------------------------------End of mrs_bso_bdcollect.c--------------------------------*/
#endif