/**
 ******************************************************************************
 * @file      mrs_sdsave.c
 * @brief     sd存数处理
 * @details   This file including all API functions's implement of mrs_sdsave.c.
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
#include "hi_ft_nv.h"
#include "hi_stdlib.h"
#include "hi_socket.h"
#include "hi_mdm_types.h"
#include "hi_mdm_sys.h"
#include "hi_mdm_time.h"

#include "mrs_sdsave.h"
#include "mrs_cfg.h"

/*-----------------------------------------------------------------------------
 Section: Constant Definitions
 ----------------------------------------------------------------------------*/
#define MRS_SDSAVE_TIMEOUT  12000

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

/*-----------------------------------------------------------------------------
 Section: Local Function Prototypes
 ----------------------------------------------------------------------------*/
static mrs_bso_t mrs_bso_bdinst[] = 
{
    {CHAN_ETH_9G25, BSO_TYPE_BDCOLLECT}, 
};

static mrs_chan_t mrs_sdsave_chan = 
{
    -1, PTL_68, {0}, NULL
};

/*-----------------------------------------------------------------------------
 Section: Global Function Prototypes
 ----------------------------------------------------------------------------*/
extern HI_S32 
mrs_eth_read(HI_S32 fd, void *ptl_chk);

/*-----------------------------------------------------------------------------
 Section: Function Definitions
 ----------------------------------------------------------------------------*/
static HI_S32 Mrs_Sdsave_SockInit(void)
{
    HI_S32 fd = -1;
    HI_S32 iOpt = 1;// NONE_BLOCK
    HI_U8 ip;
    struct sockaddr_in server_addr;

    (hi_void)memset_s(&server_addr,sizeof( struct sockaddr_in), 0,sizeof( struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(65531);

    ip = Mrs_Misc_Ip_Get();
    if (ip != ((HI_U8)ERROR))
    {
        char ipbuf[20] = {0};
        (hi_void)sprintf_s(ipbuf, sizeof(ipbuf), "172.16.%d.45", ip);
        inet_aton(ipbuf, &server_addr.sin_addr);
        printf("[OK][%s %u]: %s\r\n",__func__,__LINE__,ipbuf);
    }
    else
    {
        printf("[ERR][%s %u]\r\n",__func__,__LINE__);
        return ERROR;
    }

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == HI_SYS_INVALID_SOCKET)
    {
        printf("[ERR][%s %u]\r\n",__func__,__LINE__);
        return ERROR;
    }
    
    if (connect(fd, (struct sockaddr*)&server_addr, 
        sizeof(server_addr)) < 0)
    {
        close(fd);
        printf("[ERR][%s %u]\r\n",__func__,__LINE__);
        return ERROR;
    }

    if (ioctl(fd, FIONBIO, (void*)&iOpt) != 0)
    {
        close(fd);
        printf("[ERR][%s %u]\r\n",__func__,__LINE__);
        return ERROR;
    }

    return fd;
}

static mrs_bso_t*  
Mrs_Sdsave_ChanInit(mrs_chan_t* pchan, 
    HI_S32 fd, HI_U8 bsotype, void *param)
{
    int i;
    mrs_ptl_chk_t* pchk = &pchan->ptl_chk;
    mrs_bso_t* bso = NULL;

    for (i = 0;i < ARRAY_SIZE(mrs_bso_bdinst);i ++)
    {
        if (mrs_bso_bdinst[i].bsotype == bsotype)
        {
            break;
        }
    }

    if (i >= ARRAY_SIZE(mrs_bso_bdinst))
    {
        return NULL;
    }

    bso = &mrs_bso_bdinst[i];

    //获取fd
    pchan->fd = fd;
    //获取协议类型
    pchan->ptype = PTL_68;
    //获取通道的报文检测接口
    pchan->ptl_chkfrm = mrs_ptl_chkfrm;
    
    //初始化报文检测对象
    mrs_ptl_chkfrm_init(pchan->ptype, pchk, mrs_chan_read_cb);
    
    //业务对象
    bso->chantype = CHAN_ETH_9G25;
    bso->param = param;
    
    return mrs_bso_instance(bsotype, bso);
}

static HI_S32 
Mrs_Sdsave_ChanProc(mrs_chan_t* pchan, mrs_bso_t* bso)
{
    int fn;
    mrs_ptl_chk_t* pchk = &pchan->ptl_chk;

    if (ERROR == bso->run(pchan->fd, bso))
    {
        printf("[ERR][%s %u]\r\n",__func__,__LINE__);
        return ERROR;
    }
        
    //报文检测
    fn = pchan->ptl_chkfrm(pchk);
    
    //检测到合法报文
    if (fn >= 0)
    {
        if (fn == bso->bsotype)
        {
            //业务数据接收处理
            if (ERROR == bso->proc(pchan->fd, pchk->pout, 
                pchk->out_len, bso))
            {
                printf("[%s %u]\r\n",__func__,__LINE__);
                return ERROR;
            }
        }

        //收到合法报文后的回调函数处理
        pchk->cb(pchk);
    }

    return OK;
}

/**
 ******************************************************************************
 * @brief      写文件
 * @param[in] ptype:  

 * @retval  
 *
 * @details: 调用此接口必须互斥
 *
 * @note
 ******************************************************************************
 */
HI_S32 Mrs_Sdsave_FileWrite(HI_U8 bsotype, void *param)
{
    HI_S32 fd = -1;
    HI_S32 ret;
    HI_U32 t;
    fd_set fdset;
    struct timeval tv; 

    Mrs_Misc_Sem_Wait(MISC_SEM_COLLECT, HI_SYS_WAIT_FOREVER);
    
    fd = Mrs_Sdsave_SockInit();
    if (fd == ERROR)
    {
        Mrs_Misc_Sem_Signal(MISC_SEM_COLLECT);
        return ERROR;
    }

    mrs_bso_t* bso = NULL;
    mrs_chan_t* pchan = &mrs_sdsave_chan;

    bso = Mrs_Sdsave_ChanInit(pchan, fd, bsotype, param);
    if (!bso)
    {
        printf("[ERR][%s %u]\r\n",__func__,__LINE__);
        close(fd);
        Mrs_Misc_Sem_Signal(MISC_SEM_COLLECT);
        return ERROR;
    }

    tv.tv_sec = 0; 
    tv.tv_usec = 10000; 
    t = HI_MDM_GetMilliSeconds();

    for (;;)
    {
        //超时退出
        if (abs(HI_MDM_GetMilliSeconds() - t) >= MRS_SDSAVE_TIMEOUT)
        {
            mrs_bso_clr(bso);
            printf("mrs_sdsave: timeout\r\n");
            break;
        }

        (void)memset_s((void*)(&fdset), sizeof(*(&fdset)),0, sizeof(*(&fdset))); 
        FD_SET((HI_U32)fd, &fdset);

        ret = select(fd + 1, &fdset, NULL, NULL, &tv);
        if(ret < 0) 
        {
            printf("[ERR][%s %u]\r\n",__func__,__LINE__);
            mrs_bso_clr(bso);
            break;
        }
        
        if (FD_ISSET((HI_U32)fd, &fdset))
        {
            HI_S32 rdlen;
            mrs_ptl_chk_t* pchk = &pchan->ptl_chk;
            rdlen = mrs_eth_read(fd, pchk);
            if (rdlen <= 0)
            {
                printf("[ERR][%s %u]\r\n",__func__,__LINE__);
                mrs_bso_clr(bso);
                break;
            }
        }

        if (ERROR == Mrs_Sdsave_ChanProc(pchan, bso))
        {
            break;
        }
    }

    close(fd);
    Mrs_Misc_Sem_Signal(MISC_SEM_COLLECT);
    return OK;
}

/*---------------------------------mrs_sdsave.c-------------------------------------*/
#endif