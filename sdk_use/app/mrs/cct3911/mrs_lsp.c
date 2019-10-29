/**
 ******************************************************************************
 * @file      mrs_lsp.c
 * @brief     串口通道处理
 * @details   This file including all API functions's implement of mrs_lsp.c.
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
#include "hi_mdm_types.h"
#include "hi_mdm_uart.h"
#include "hi_mdm_task.h"
#include "securec.h"
#include "mrs_cfg.h"

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
static HI_S32 mrs_lsp_read(HI_S32 fd, void *ptl_chk)
{
    HI_S32 ret;
    mrs_ptl_chk_t *pchk = (mrs_ptl_chk_t *)ptl_chk;
    
    if (pchk->in_len >= PTL_INBUF_LEN)
    {
        pchk->in_len = 0;
    }
            
    ret = HI_MDM_UART_Read(fd, pchk->pin + pchk->in_len, PTL_INBUF_LEN 
        - pchk->in_len);

    if (ret > 0)
    {
        pchk->in_len += ret;
    }

    return ret;
}

/**
 ******************************************************************************
 * @brief   网口处理任务
 * @return  None
 ******************************************************************************
 */
static HI_VOID mrs_lsp_task(HI_U32 ulParam)
{  
    mrs_chan_t *pchan = mrs_chan_ctx(CHAN_LSP_9G25);
    mrs_ptl_chk_t *pchk = &pchan->ptl_chk;
   
    printf("mrs_lsp_task OK\n");

    for(;;)
    {
        //读数据
        mrs_lsp_read(pchan->fd, pchk);

        //解包
        mrs_chan_ptl_unpack(CHAN_LSP_9G25);
    }
}

/**
 ******************************************************************************
 * @brief   初始化网口模块
 * @retval  OK   : 成功
 * @retval  ERROR: 失败
 ******************************************************************************
 */
HI_S32 
mrs_lsp_init(void)
{
    HI_U32 ret;
    HI_U32 TaskId;
    HI_UART_INIT_TYPE_S UartCfg = {115200, 8, 1, 2};
    mrs_chan_t* pchan = NULL;

    pchan = mrs_chan_ctx(CHAN_LSP_9G25);
    pchan->ptl_chkfrm = mrs_ptl_chkfrm;
    (hi_void)memset_s(&pchan->ptl_chk, sizeof(mrs_ptl_chk_t), 0, sizeof(mrs_ptl_chk_t));

    mrs_ptl_chkfrm_init(pchan->ptype, &pchan->ptl_chk, mrs_chan_read_cb);

    pchan->fd = HI_MDM_UART_Open(HI_UART_2, HI_UART_232);
    if (pchan->fd < 0)
    {
        return ERROR;
    }

    ret = HI_MDM_UART_Ioctl(pchan->fd, UART_CFG_SET_ATTR, (HI_U32)&UartCfg);
    if (ret != HI_ERR_SUCCESS)
    {
        printf("[ERR][%s %u]\r\n",__func__,__LINE__);
        return ERROR;
    }
    
    HI_MDM_TASK_Create(&TaskId, "NDM_LSP", mrs_lsp_task, 0, 1024, 26);

    return OK;
}

/*---------------------------------lsp.c-------------------------------------*/
#endif