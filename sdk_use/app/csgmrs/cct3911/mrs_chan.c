/**
 ******************************************************************************
 * @file      mrs_chan.c
 * @brief     通道处理
 * @details   This file including all API functions's implement of mrs_chan.c.
 * @copyright 
 ******************************************************************************
 */
#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined (PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_CCT3911)

/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include "mrs_cfg.h"
#include "hi_types.h"
#include "hi_socket.h"
#include "hi_mdm_types.h"
#include "hi_mdm_uart.h"

/*-----------------------------------------------------------------------------
 Section: Macro Definitions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 Section: Global Variables
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 Section: Local Variables
 ----------------------------------------------------------------------------*/
static mrs_chan_t mrs_chan_list[CHAN_MAX] = 
{
    {-1, PTL_68, {0}, NULL},
    {-1, PTL_68, {0}, NULL},
};

static HI_U32 mrs_chan_static[CHAN_MAX] = {0,0};

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
void mrs_chan_static_clr(HI_U8 chantype)
{
    if (chantype >= CHAN_MAX)
    {
        return;
    }

    mrs_chan_static[chantype] = 0;
}

HI_U32 mrs_chan_static_get(HI_U8 chantype)
{
    if (chantype >= CHAN_MAX)
    {
        return 0;
    }

    return mrs_chan_static[chantype];
}

/**
 ******************************************************************************
 * @brief      获取通道数据处理句柄
 * @param[in]  n ：通道号
 * @retval   通道数据处理句柄
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
mrs_chan_t* mrs_chan_ctx(HI_U8 n)
{
    return &mrs_chan_list[n];
}

/**
 ******************************************************************************
 * @brief      通道接收到合法报文后的回调函数
  * @param[in]  *ptl_chk：报文检测对象
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
void mrs_chan_read_cb(void *ptl_chk)
{
    mrs_ptl_chk_t *pchk = (mrs_ptl_chk_t *)ptl_chk;
    HI_S32 pre_len = pchk->pout - pchk->pin;

    if (pchk->in_len > (pchk->out_len + pre_len))
    {
        pchk->in_len -= (pchk->out_len + pre_len);
        (hi_void)memmove_s(pchk->pin, PTL_INBUF_LEN, pchk->pout + pchk->out_len, 
            pchk->in_len);
    }
    else
    {
        pchk->in_len = 0;
    }
}

void mrs_chan_ptl_unpack(HI_U8 chantype)
{
    if (chantype >= CHAN_MAX)
    {
        return;
    }

    HI_U32 ret;
    int fn;
    HI_U8* pbuf = NULL;
    HI_SYS_QUEUE_MSG_S queue_msg = {0};

    mrs_chan_t *pchan = mrs_chan_ctx(chantype);
    mrs_ptl_chk_t *pchk = &pchan->ptl_chk;

    for (;;)
    {
        //报文检测
        fn = pchan->ptl_chkfrm(pchk);

        //未检测到合格报文退出
        if (fn < 0)
        {
            break;
        }

        pbuf = mlib_malloc(TEMP_256);
        if (!pbuf)
        {
            printf("[%s]: Mlib_malloc Err!!!\r\n",__func__);
            break;
        }

        (hi_void)memcpy_s(pbuf, TEMP_256, pchk->pout, pchk->out_len);

        queue_msg.ulMsgId = chantype + 1;
        queue_msg.ulParam[0] = pchk->out_len;
        queue_msg.ulParam[1] = (HI_U32)pbuf;

        ret = Mrs_Misc_Msg_Send(MISC_QUEUE_CHAN, &queue_msg, 0);
        if (HI_ERR_SUCCESS == ret)
        {
        }
        else
        {
            mlib_free(pbuf);
            pbuf = NULL;
            printf("[%s]: Msg send Err!!!\r\n",__func__);
        }

        //收到合法报文后的回调处理
        pchk->cb(pchk);
    }
}

int mrs_chan_write(HI_U8 n, int fd, 
    HI_U8* buf, int len)
{
    if (n == CHAN_ETH_9G25)
    {
        return comm_wr(fd, buf, len);
    } 
    else if (n == CHAN_LSP_9G25)
    {
        //TODO
        return HI_MDM_UART_Write(fd, buf, len);
    }

    return ERROR;
}

int mrs_chan_writes(HI_U8 n, HI_U8 wrnum, int fd, 
    HI_U8* buf, int len)
{
    int i;
    
    if (n == CHAN_ETH_9G25)
    {
        for (i = 0;i < wrnum;i ++)
        {
            if (comm_wr(fd, buf, len) == len)
            {
                //TODO延时
                return OK;
            }
        }

        mrs_chan_static[n] ++;
    } 
    else if (n == CHAN_LSP_9G25)
    {
        for (i = 0;i < wrnum;i ++)
        {
            if (HI_MDM_UART_Write(fd, buf, len) == len)
            {
                //TODO延时
                return OK;
            }
        }
    }

    return ERROR;
}

/*---------------------------------chan.c-------------------------------------*/
#endif