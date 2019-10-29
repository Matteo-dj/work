/**
 ******************************************************************************
 * @file      mrs_eth.c
 * @brief     网口通道处理
 * @details   This file including all API functions's implement of eth.c.
 * @copyright  
 ******************************************************************************
 */
#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined (PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_CCT3911)
/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include "hi_stdlib.h"
#include "hi_socket.h"
#include "hi_mdm_task.h"
#include "hi_mdm_network.h"
#include "securec.h"
#include "mrs_cfg.h"

/*-----------------------------------------------------------------------------
 Section: Macro Definitions
 ----------------------------------------------------------------------------*/
#define NONE_BLOCK 1
#define BLOCK 0

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Global Variables
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Local Variables
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 Section: Local Function Prototypes
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 Section: Global Function Prototypes
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Function Definitions
 ----------------------------------------------------------------------------*/
static HI_VOID mrs_eth_ip_get(struct sockaddr_in *server_addr)
{
    HI_U8 ip;

    ip = Mrs_Misc_Ip_Get();
    if (ip != (HI_U8)ERROR)
    {
        char ipbuf[20] = {0};
        (hi_void)sprintf_s(ipbuf, sizeof(ipbuf), "172.16.%d.45", ip);
        inet_aton(ipbuf, &server_addr->sin_addr);
        printf("[OK][mrs_eth]:ip = %s\r\n", ipbuf);
    }
    else
    {
        Mrs_Misc_Ip_Set(192);
        inet_aton("172.16.192.45", &server_addr->sin_addr);
        printf("[ERR][mrs_eth]:ip = 172.16.192.45\r\n");
    }
}

/**
 ******************************************************************************
 * @brief      网口通道数据读取接口
 * @param[in]  fd ：网口通道句柄
  * @param[in]  *ptl_chk：报文检测对象
 * @retval   HI_S32 : 数据长度
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
HI_S32 mrs_eth_read(HI_S32 fd, void *ptl_chk)
{
    HI_S32 ret;
    mrs_ptl_chk_t *pchk = (mrs_ptl_chk_t *)ptl_chk;
    
    if ((pchk->in_len >= PTL_INBUF_LEN) 
        || (pchk->in_len < 0))
    {
        pchk->in_len = 0;
    }
            
    ret = recv(fd, pchk->pin + pchk->in_len, PTL_INBUF_LEN 
        - pchk->in_len, 0);

    if (ret > 0)
    {
        pchk->in_len += ret;
    }

    return ret;
}

HI_S32 mrs_eth_connect(mrs_chan_t *pchan, struct sockaddr_in server_addr)
{
    HI_S32 select_ret;
    fd_set fdset; 
    struct timeval tv; 
    
    pchan->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (pchan->fd == HI_SYS_INVALID_SOCKET)
    {
        printf("[ERR][%s %u]\r\n",__func__,__LINE__);
        return ERROR;
    }

    HI_BOOL bConnect = HI_FALSE;
    HI_S32 iOpt = NONE_BLOCK;
    ioctl(pchan->fd, FIONBIO, (void*)&iOpt);

    if (connect(pchan->fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        HI_S32 error_code = -1;
        HI_S32 len = 4;
    
        tv.tv_sec = (HI_S32)5; 
        tv.tv_usec = 0; 
        (void)memset_s((void*)(&fdset), sizeof(*(&fdset)),0, sizeof(*(&fdset))); 
        FD_SET((HI_U32)pchan->fd, &fdset);
        select_ret = select(pchan->fd + 1, NULL, &fdset, NULL, &tv);
        if (select_ret > 0) 
        { 
            (HI_VOID)getsockopt(pchan->fd, SOL_SOCKET, SO_ERROR, &error_code, (socklen_t*)&len); 
            bConnect = ((error_code == 0) ? HI_TRUE: HI_FALSE);
        }
        else 
        {
            bConnect = HI_FALSE; 
        }
    }
    else
    {
        bConnect = HI_TRUE; 
    }

    if (bConnect)
    {
        iOpt = BLOCK;
        ioctl(pchan->fd, FIONBIO, (void *)&iOpt);
    }

    if (!bConnect)
    {
        printf("[ERR][%s %u]\r\n",__func__,__LINE__);
        close(pchan->fd);
        pchan->fd = -1;
        return ERROR;
    }

    printf("[OK][%s %u]\r\n",__func__,__LINE__);
    return OK;
}

/**
 ******************************************************************************
 * @brief   网口数据处理任务
 * @return  None
 ******************************************************************************
 */
static HI_VOID mrs_eth_task(HI_U32 ulParam)
{   
    HI_S32 select_ret;
    fd_set fdset; 
    struct timeval tv; 
    mrs_chan_t *pchan = NULL;
    mrs_ptl_chk_t *pchk = NULL;
    struct sockaddr_in server_addr;
    
    (hi_void)memset_s(&server_addr,sizeof( struct sockaddr_in), 0,sizeof( struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(65530);
    mrs_eth_ip_get(&server_addr);

    pchan = mrs_chan_ctx(CHAN_ETH_9G25);
    printf("mrs_eth_task OK\n");

    for (;;)
    {
        if (!Mrs_Misc_LinkTo9G25())
        {
            HI_MDM_Sleep(30);
            continue;
        }

        if (pchan->fd == HI_SYS_INVALID_SOCKET)
        {
            if (mrs_eth_connect(pchan, server_addr) == ERROR)
            {
                HI_MDM_Sleep(2000);
                continue;
            }
        }

        tv.tv_sec = (HI_S32)3; 
        tv.tv_usec = 0; 
        (void)memset_s((void*)(&fdset), sizeof(*(&fdset)),0, sizeof(*(&fdset))); 
        FD_SET((HI_U32)pchan->fd, &fdset);

        select_ret = select(pchan->fd + 1, &fdset, NULL, NULL, &tv);
        if (select_ret > 0) 
        { 
            if (FD_ISSET((HI_U32)pchan->fd, &fdset))
            {
                HI_S32 rdlen;
                pchk = &pchan->ptl_chk;

                rdlen = mrs_eth_read(pchan->fd, pchk);
                if (rdlen <= 0)
                {
                    close(pchan->fd);
                    pchan->fd = -1;
                    continue;
                }
            }
        }

        //解包
        mrs_chan_ptl_unpack(CHAN_ETH_9G25);
    }
}

/**
 ******************************************************************************
 * @brief   初始化网口模块,创建网口任务
 * @retval  OK   : 成功
 * @retval  ERROR: 失败
 ******************************************************************************
 */
HI_S32 
mrs_eth_init(void)
{
    HI_SYS_ETH_CONFIG_S EthCfgInfo;  

    if (HI_ERR_SUCCESS == HI_MDM_GetEthConfig(&EthCfgInfo))
    {
        printf("ip: %d.%d.%d.%d\n",EthCfgInfo.ip_addr[0],EthCfgInfo.ip_addr[1],
            EthCfgInfo.ip_addr[2],EthCfgInfo.ip_addr[3]);
        printf("net_mask: %d.%d.%d.%d\n",EthCfgInfo.net_mask[0],EthCfgInfo.net_mask[1],
            EthCfgInfo.net_mask[2],EthCfgInfo.net_mask[3]);
        printf("gateway: %d.%d.%d.%d\n",EthCfgInfo.gateway[0],EthCfgInfo.gateway[1],
            EthCfgInfo.gateway[2],EthCfgInfo.gateway[3]);
        printf("mac_addr: %02X:%02X:%02X:%02X:%02X:%02X\n",
            EthCfgInfo.mac_addr[0],EthCfgInfo.mac_addr[1],
            EthCfgInfo.mac_addr[2],EthCfgInfo.mac_addr[3],
            EthCfgInfo.mac_addr[4],EthCfgInfo.mac_addr[5]);
    }

    HI_U32 TaskId;
    mrs_chan_t* pchan = NULL;

    pchan = mrs_chan_ctx(CHAN_ETH_9G25);
    pchan->ptl_chkfrm = mrs_ptl_chkfrm;
    (hi_void)memset_s(&pchan->ptl_chk, sizeof(mrs_ptl_chk_t), 0, sizeof(mrs_ptl_chk_t));

    mrs_ptl_chkfrm_init(pchan->ptype, &pchan->ptl_chk, mrs_chan_read_cb);

    HI_MDM_TASK_Create(&TaskId, "NDM_ETH", mrs_eth_task, 0, 1024, 26);

    return OK;
}

/*---------------------------------eth.c-------------------------------------*/
#endif