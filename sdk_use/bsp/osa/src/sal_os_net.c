#include <hi_types.h>
#include <hi_mdm_types.h>
#include <sal_os_net.h>
#include <sal_common.h>//�Ȱ���sal_common.h�ڰ���dfx_sal.h������ܱ���ͨ��

#include <dfx_sal.h>
#include <sockets.h>
#include "lwip/static_route.h"
#include <ip_addr.h>
#include <hi_stdlib.h>
#include <opt.h>
#include <sal_inf.h>

HI_PRV HI_U32 sal_route_add_prv(HI_PCSTR ucIfName,ip_addr_t* addrIp, ip_addr_t* addrMask, ip_addr_t* addrGw);


HI_PRV HI_U32 sal_route_delete_prv(ip_addr_t* addrIp);



HI_NET_IF_ADAPTER_S g_net_if_adapter_array[HI_NET_IF_MAX_COUNT];

#define HI_HANDLE_OFFSET 0x1241644a//�����±��handle��ת��ֵ

/*handleת������ƽṹ*/
HI_NET_IF_ADAPTER_S * net_if_handle2adapter(HI_U32 handle)
{
    HI_U32 ulArrayId =  handle-HI_HANDLE_OFFSET;
    if(ulArrayId >= HI_NET_IF_MAX_COUNT)
    {
        return HI_NULL;
    }

    if(g_net_if_adapter_array[ulArrayId].inited != HI_NET_IF_INITED_MAGIC)
    {
        return HI_NULL;
    }

    return &(g_net_if_adapter_array[ulArrayId]);
}

/*����ת������ƽṹ*/
HI_NET_IF_ADAPTER_S * net_if_name2adapter(HI_CHAR name[HI_NET_MAX_NAME_LEN])
{
    HI_U32 i = 0;    
    HI_NET_IF_ADAPTER_S *pAdapter = HI_NULL;
    for(i=0;i<HI_NET_IF_MAX_COUNT;i++)
    {
        pAdapter = &g_net_if_adapter_array[i];
        if(strncmp(name,pAdapter->name,HI_NET_MAX_NAME_LEN) && pAdapter->inited == HI_NET_IF_INITED_MAGIC)
        {
            return pAdapter;
        }
    }
    return HI_NULL;
}
/*����תnetif*/
struct netif* net_if_name2netIf(HI_CHAR name[HI_NET_MAX_NAME_LEN])
{
    HI_U32 i = 0;    
    HI_NET_IF_ADAPTER_S *pAdapter = HI_NULL;

    for(i=0;i<HI_NET_IF_MAX_COUNT;i++)
    {
        pAdapter = &g_net_if_adapter_array[i];
        if(strncmp(name,pAdapter->name,HI_NET_MAX_NAME_LEN) && pAdapter->inited == HI_NET_IF_INITED_MAGIC)
        {
            return &(pAdapter->netif);
        }
    }
    return HI_NULL;
}

/*����������ƽṹ*/
HI_PRV HI_NET_IF_ADAPTER_S * net_if_malloc_adapter(HI_VOID)
{
    HI_U32 i = 0;    
    HI_NET_IF_ADAPTER_S *pAdapter = HI_NULL;
    for(i=0;i<HI_NET_IF_MAX_COUNT;i++)
    {
        pAdapter = &g_net_if_adapter_array[i];
        if(pAdapter->alloced == HI_FALSE)
        {
            pAdapter->alloced = HI_TRUE;
            pAdapter->handle = i+HI_HANDLE_OFFSET;
            return pAdapter;
        }
    }
    return HI_NULL;
}

#define net_if_netif2adapter(netif) (netif->state)

/*�������ӶϿ�*/
HI_U32 SAL_SetLinkDown(HI_U32 handle)
{
    HI_NET_IF_ADAPTER_S * pAdapter = net_if_handle2adapter(handle);

    if(pAdapter==HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    netif_set_link_down(&(pAdapter->netif));

    return HI_ERR_SUCCESS;
}

/*������������*/
HI_U32 SAL_SetLinkUp(HI_U32 handle)
{

    HI_NET_IF_ADAPTER_S * pAdapter = net_if_handle2adapter(handle);

    if(pAdapter==HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    netif_set_link_up(&(pAdapter->netif));

    return HI_ERR_SUCCESS;
}


//�����ݷ��͸�Э��ջ
HI_U32 SAL_RcvNetData(HI_U32 handle,HI_CHAR * pRcvData,HI_U32 ulDataLen)
{
    HI_NET_IF_ADAPTER_S * pAdapter = net_if_handle2adapter(handle);
    HI_U32 ulPadSize = 0;
    struct pbuf *pNetIfBuf = HI_NULL;
    struct netif * pNetIf= HI_NULL;
    HI_U32 foward_size = 0;

    //net_debug_printf_data("SAL_RcvNetData",pRcvData,ulDataLen);

    if(pAdapter==HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    pNetIf = &(pAdapter->netif);

    if(pNetIf->flags & (NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET))
    {
        ulPadSize = ETH_PAD_SIZE;
    }
    else
    {
        foward_size = sizeof(struct eth_hdr);//�������ETHͷ��ת����ETH�������ETHͷʹ��
    }

    pNetIfBuf = pbuf_alloc(PBUF_RAW,(HI_U16)(ulDataLen+ulPadSize+foward_size),PBUF_RAM);
    if(pNetIfBuf == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }

    if(ulPadSize+foward_size!=0)
    {
        (HI_VOID)pbuf_header(pNetIfBuf, (HI_S16)(0-(ulPadSize+foward_size)));
    }

    memcpy_s(pNetIfBuf->payload, ulDataLen, pRcvData, ulDataLen);
    if(ulPadSize!=0)
    {
        (HI_VOID)pbuf_header(pNetIfBuf, (HI_S16)ulPadSize);
    }


    if(pNetIf->input)
    {   
        
        //printf("pNetIf->input \n");
        if(pNetIf->input(pNetIfBuf, pNetIf)!=ERR_OK)
        {
            
            //printf("pNetIf->input not_ok \n");
            pbuf_free(pNetIfBuf);
            return HI_ERR_FAILURE;
        }
       
    }
    else
    {
        pbuf_free(pNetIfBuf);
        return HI_ERR_FAILURE;

    }
    
    return HI_ERR_SUCCESS;
    
}

//����̫���ӿڣ����۲�Ӧ�õ��õ�.��ֹ��ָ����������
HI_PRV HI_VOID  net_if_drv_out_put_nothing(struct netif *netif, struct pbuf *p)
{
    //printf("net_if_drv_out_put_nothing should not go \n");
    return;
}

//����liwp����·�����
HI_PRV HI_VOID  net_if_link_out_put(struct netif *netif, struct pbuf *p)
{
    HI_NET_IF_ADAPTER_S *pAdapter = HI_NULL;
    HI_PVOID pBufs[OUT_BUF_MAX_COUT]={0};
    HI_U32 ulBufLens[OUT_BUF_MAX_COUT]={0};
    HI_U32 ulBufCout = 0;
    HI_U32 ulTotalLen = 0;
    struct pbuf * q = HI_NULL;
    hi_net_link_out pfnLinkOut = HI_NULL;  

    if (netif == HI_NULL)
    {
        return;
    }
    
    pAdapter = net_if_netif2adapter(netif);
    if((pAdapter == HI_NULL) || (pAdapter->pfnLinkOut == HI_NULL))
    {
        return;
    }
    
    pfnLinkOut = pAdapter->pfnLinkOut;
    
   
    for (q  = p; q  != NULL; q  = q ->next)
    {
        if(ulBufCout>=OUT_BUF_MAX_COUT)
        {
            return ;
        }
        ulTotalLen+=q->len;
        pBufs[ulBufCout] = q->payload;
        ulBufLens[ulBufCout] = q->len;
        ulBufCout++;    
    }


    pfnLinkOut(pBufs,ulBufLens,ulBufCout,ulTotalLen);

    return;
}

extern struct ip_addr_t * ip_route_find_gw(struct netif *netif, ip_addr_t *dest);
//����lwip��IP�����
HI_PRV HI_VOID  net_if_ip_out_put(struct netif *netif, struct pbuf *p,ip_addr_t* ipaddr)
{
    HI_NET_IF_ADAPTER_S *pAdapter = HI_NULL;
    HI_PVOID pBufs[OUT_BUF_MAX_COUT]={0};
    HI_U32 ulBufLens[OUT_BUF_MAX_COUT]={0};
    HI_U32 ulBufCout = 0;
    HI_U32 ulTotalLen = 0;
    struct pbuf * q = HI_NULL;
    hi_net_ip_out pfnIpOut = HI_NULL;    

    if (netif == HI_NULL)
    {
        return;
    }
    
    pAdapter = (HI_NET_IF_ADAPTER_S *)net_if_netif2adapter(netif);
    if((pAdapter == HI_NULL) || (pAdapter->pfnIpOut == HI_NULL))
    {
        return;
    }
    
    pfnIpOut = pAdapter->pfnIpOut;
    
    for (q  = p; q  != NULL; q  = q ->next)
    {
        if(ulBufCout>=OUT_BUF_MAX_COUT)
        {
            return ;
        }
        ulTotalLen+=q->len;
        pBufs[ulBufCout] = q->payload;
        ulBufLens[ulBufCout] = q->len;
        ulBufCout++;    
    }

    pfnIpOut(pBufs,ulBufLens,ulBufCout,ulTotalLen,ipaddr->addr);
    return;
}

//����lwip������mac��ַ
HI_PRV HI_U8 net_if_set_hw_addr(struct netif *netif, HI_U8 *addr, HI_U8 len)
{
    hi_net_set_mac pfnSetMac = HI_NULL;
    HI_NET_IF_ADAPTER_S *pAdapter = (HI_NET_IF_ADAPTER_S *)net_if_netif2adapter(netif);
    pfnSetMac =  pAdapter->pfnSetMac;
    if(pfnSetMac)
    {
        pfnSetMac(addr,len);
    }
    return 0;
}

//��ʼ������
HI_PRV HI_U32 hi_create_net_if(HI_CHAR name[HI_NET_MAX_NAME_LEN],HI_NET_IF_INIT_CFG_S *pCfg)
{
    HI_NET_IF_ADAPTER_S *pAdapter = HI_NULL;    
    struct netif * pNetIf = HI_NULL;
    HI_U32 nameLen =  0;
    ip_addr_t ipaddr, netmask, gw;

    //1.�жϲ����Ϸ���
    if(name==HI_NULL||pCfg==HI_NULL)
    {
        return HI_NET_INVALID_HANDLE;
    }

    nameLen = strlen((HI_CONST HI_CHAR *)name);
    if(nameLen==0||nameLen>=HI_NET_MAX_NAME_LEN)
    {
        return HI_NET_INVALID_HANDLE;
    }

    pAdapter = net_if_name2adapter(name);
    if(pAdapter!=HI_NULL)
    {
        //�����ظ�
        return HI_NET_INVALID_HANDLE;
    }
   
    //2.������е�netif��Դ    
    pAdapter = net_if_malloc_adapter();
    if(pAdapter==HI_NULL)
    {
        //��Դ����
        return HI_NET_INVALID_HANDLE;
    }

    //3.���adpater
    memcpy_s(pAdapter->name, sizeof(pAdapter->name), name, HI_NET_MAX_NAME_LEN);

    //4.��ʼ������lwip netif
    pNetIf = &(pAdapter->netif);
    pNetIf->state = pAdapter;//netifָ��adapter
    pNetIf->link_layer_type = ETHERNET_DRIVER_IF;//linktype
    pNetIf->hwaddr_len = ETHARP_HWADDR_LEN;//�����ַ����
    pNetIf->mtu =  pCfg->default_mtu;
    snprintf_s(pNetIf->name, sizeof(pNetIf->name), sizeof(pNetIf->name) - 1, "%s", "eth1");
    
    //pNetIf->input = tcpip_input;���뺯��,�ڲ���ֵ
    //��ip,gw,netmaskת����lwip�ø�ʽ
    IP4_ADDR(&gw, pCfg->gw[0], pCfg->gw[1], pCfg->gw[2], pCfg->gw[3]);
    IP4_ADDR(&ipaddr, pCfg->ip_addr[0], pCfg->ip_addr[1], pCfg->ip_addr[2], pCfg->ip_addr[3]);
    IP4_ADDR(&netmask, pCfg->net_mask[0], pCfg->net_mask[1], pCfg->net_mask[2], pCfg->net_mask[3]);

    //����netif���Ժ��������
    if(pCfg->bEth)
    {
        pNetIf->flags = NETIF_FLAG_ETHARP|NETIF_FLAG_ETHERNET|NETIF_FLAG_BROADCAST;

        //������·OUT�ӿ�
        pNetIf->drv_send = net_if_link_out_put;
        pAdapter->pfnLinkOut = pCfg->pfnLinkOut;
    }
    else
    {   
        pNetIf->flags = NETIF_FLAG_BROADCAST;

        //����IP��out�ӿ�
        pNetIf->output = (netif_output_fn)net_if_ip_out_put;
        pAdapter->pfnIpOut = pCfg->pfnIpOut;

        //��ֹ��һ�����õ�,������·��ӿ�
        pNetIf->drv_send = (drv_send_fn)net_if_drv_out_put_nothing;
        pAdapter->pfnLinkOut = HI_NULL;        
    }

    //��������Ӳ����ַ�ӿ�
    pNetIf->drv_set_hwaddr = net_if_set_hw_addr;//���������ַ�ص�
    pAdapter->pfnSetMac = pCfg->pfnSetMac;
    netif_add(pNetIf,&ipaddr,&netmask,&gw);
    netif_set_up(pNetIf);
    pAdapter->inited = HI_NET_IF_INITED_MAGIC;
    return pAdapter->handle;
}





//��ʼ����̫������
HI_U32 SAL_CreateIpNetIf(HI_CHAR name[HI_NET_MAX_NAME_LEN],HI_IP_NET_IF_INIT_CFG_S *pCfg)
{
    HI_NET_IF_INIT_CFG_S stInitCfg;

    memset_s(&stInitCfg, sizeof(stInitCfg), 0, sizeof(stInitCfg));

    stInitCfg.bEth = HI_FALSE;
    (hi_void)memcpy_s(stInitCfg.ip_addr, sizeof(stInitCfg.ip_addr), pCfg->ip_addr, sizeof(stInitCfg.ip_addr));
    (hi_void)memcpy_s(stInitCfg.net_mask, sizeof(stInitCfg.net_mask), pCfg->net_mask, sizeof(stInitCfg.net_mask));
    (hi_void)memcpy_s(stInitCfg.hwaddr, sizeof(stInitCfg.hwaddr), pCfg->hwaddr, sizeof(stInitCfg.hwaddr));
    stInitCfg.default_mtu = pCfg->default_mtu;
    stInitCfg.pfnIpOut = pCfg->pfnIpOut;
    stInitCfg.pfnSetMac = pCfg->pfnSetMac;
    stInitCfg.hwaddr_len = pCfg->hwaddr_len;
    return hi_create_net_if(name,&stInitCfg);
}

//��ʼ������̫������
HI_U32 SAL_CreateEthNetIf(HI_CHAR name[HI_NET_MAX_NAME_LEN],HI_ETH_NET_IF_INIT_CFG_S *pCfg)
{    
    HI_NET_IF_INIT_CFG_S stInitCfg;

    memset_s(&stInitCfg, sizeof(stInitCfg), 0, sizeof(stInitCfg));

    stInitCfg.bEth = HI_TRUE;
    memcpy_s(stInitCfg.ip_addr, sizeof(stInitCfg.ip_addr), pCfg->ip_addr, sizeof(stInitCfg.ip_addr));
    memcpy_s(stInitCfg.net_mask, sizeof(stInitCfg.net_mask), pCfg->net_mask, sizeof(stInitCfg.net_mask));
    memcpy_s(stInitCfg.gw, sizeof(stInitCfg.gw), pCfg->gw, sizeof(stInitCfg.gw));
    memcpy_s(stInitCfg.hwaddr, sizeof(stInitCfg.hwaddr), pCfg->hwaddr, sizeof(stInitCfg.hwaddr));
    stInitCfg.default_mtu = pCfg->default_mtu;
    stInitCfg.pfnLinkOut = pCfg->pfnLinkOut;
    stInitCfg.pfnSetMac = pCfg->pfnSetMac;
    stInitCfg.hwaddr_len = pCfg->hwaddr_len;
    return hi_create_net_if(name,&stInitCfg);

}

//TODO:PLC�����õ�,�ȴ��ׯʹ�ÿ���ͨ��
HI_U32 SAL_FindTargetAddr(HI_IN HI_U32 ipAddr)
{
    // TODO: ΪʲôҪ�������
    return 1;
    //ip_addr_t addrIp;
    //addrIp.addr = ipAddr;
    //return netif_find_by_ipaddr((ipAddr));
}

//*****************************************************************************
// ��������:  SAL_ArpGetTblSize
// ��������: ��ȡARP������
// ����˵��:
//
// �� �� ֵ:
//      ARP�ܱ�����    
//*****************************************************************************
HI_U32 SAL_ArpGetTblSize(HI_VOID)
{
    return ARP_TABLE_SIZE;
}


//*****************************************************************************
// ��������:  SAL_ArpGetInfo
// ��������: ��ȡARP��Ϣ
// ����˵��:
//      arpInfo: ARP��������, ����arp��Ϣ
//      fromSq:��ʼ�±�
//      count:��ȡ��Ϣ��Ŀ��
// �� �� ֵ:
//    HI_ERR_SUCCESS : �ɹ�
//    ����ֵ����
//*****************************************************************************
HI_U32 SAL_ArpGetInfo(HI_ARP_ITEM_S  ItemInfo[],HI_U32 *count)
{
    HI_U32 i = 0, j = 0;
    struct etharp_entry* pArpTbl = etharp_get_arp_tbl();

    if ((ItemInfo == HI_NULL) || (pArpTbl == HI_NULL) || HI_NULL == count)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    for (i = 0; i < ARP_TABLE_SIZE; i++)
    {
        if ((pArpTbl[i].state == ETHARP_STATE_STABLE)
            || (pArpTbl[i].state == ETHARP_STATE_STABLE_REREQUESTING)
#if ETHARP_SUPPORT_STATIC_ENTRIES
            || (pArpTbl[i].state == ETHARP_STATE_STATIC)
#endif
           )
        {
            (hi_void)memcpy_s(ItemInfo[j].ipAddr, sizeof(ItemInfo[j].ipAddr), &pArpTbl[i].ipaddr.addr, sizeof(pArpTbl[i].ipaddr.addr));
            (hi_void)memcpy_s(ItemInfo[j].macAddr, sizeof(ItemInfo[j].macAddr), pArpTbl[i].ethaddr.addr, sizeof(ItemInfo[j].macAddr));
            (hi_void)memcpy_s(ItemInfo[j].name, sizeof(ItemInfo[j].name), pArpTbl[i].netif->name, sizeof(ItemInfo[j].name));
            j++;
        }
    }

    *count = j;
    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// ��������:  SAL_DelArp
// ��������:ɾ��һ��arp��Ϣ
// ����˵��:
//      ipAddr:Ҫɾ����arp ��ַ
// �� �� ֵ:
//      HI_ERR_SUCCESS:ɾ���ɹ�
//      ������ʾɾ��ʧ��
//*****************************************************************************
HI_U32 SAL_DelArp(HI_U8 ipAddr[4])
{
    HI_U32 ulRet = HI_ERR_FAILURE;
    struct netif *pstNetif;

    //to do need specify the eth name   
    pstNetif = netif_find("eth0");
    if(NULL == pstNetif)
    {
        return HI_ERR_FAILURE;
    } 

    ulRet = (HI_U32)etharp_delete_arp_entry(pstNetif, (ip_addr_t *)ipAddr);
    if(HI_ERR_SUCCESS == ulRet)
    { 
        return HI_ERR_SUCCESS;
    }
    else
    {
        return HI_ERR_FAILURE;
    }
    
}


typedef struct
{
    HI_U32 ulIp;
    HI_U32 ulNetMask;
    HI_U8  aucMac[8]; // 2bytes to padding
}SAL_IP_MASK_MAC_PRV_S;

HI_PRV HI_U32 set_get_ip(HI_CHAR *ucIfName,SAL_IP_MASK_MAC_PRV_S* pstSetCfg,SAL_IP_MASK_MAC_PRV_S* pstGetCfg,HI_BOOL bSet)
{
    struct ifreq stIfreq;
    HI_S32 fd=HI_SYS_INVALID_SOCKET;
    ip_addr_t route_ip,route_mask,route_gw;
    HI_U32 ret = HI_ERR_FAILURE;

    memset_s(&stIfreq, sizeof(stIfreq), 0, sizeof(stIfreq));
	HI_U32 len = strlen(ucIfName);
    strncpy_s(stIfreq.ifr_name, sizeof(stIfreq.ifr_name), ucIfName, HI_MIN(len, sizeof(stIfreq.ifr_name) - 1));
    
    //����fd ���ڻ�ȡ������Ϣ
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == HI_SYS_INVALID_SOCKET)
    {
        goto end;
    }

    //��ȡIP��ַ
    if(ioctl(fd, (HI_S32)SIOCGIFADDR, &stIfreq) < 0)
    {
        goto end;
    }  
    pstGetCfg->ulIp = ((struct sockaddr_in*)&(stIfreq.ifr_addr))->sin_addr.s_addr;

    //��ȡnet_mask    
    if (ioctl(fd, (HI_S32)SIOCGIFNETMASK, &stIfreq) < 0)
    {
        goto end;

    }
    pstGetCfg->ulNetMask = ((struct sockaddr_in*)&(stIfreq.ifr_addr))->sin_addr.s_addr;

    //��ȡmac��ַ
    if (ioctl(fd, SIOCGIFHWADDR, &stIfreq) < 0)
    {
        goto end;

    }
    memcpy_s(pstGetCfg->aucMac, 6, (u8_t *)stIfreq.ifr_hwaddr.sa_data, 6);

    if(bSet)
    {
        //�޸�IP
        ((struct sockaddr_in*)&(stIfreq.ifr_addr))->sin_addr.s_addr = pstSetCfg->ulIp;    
        if (ioctl(fd, SIOCSIFADDR, &stIfreq)<0)
        {
            goto end;
        }
        
        //�޸�net_mask
        ((struct sockaddr_in*)&(stIfreq.ifr_addr))->sin_addr.s_addr = pstSetCfg->ulNetMask;
        if(ioctl(fd, SIOCSIFNETMASK, &stIfreq)<0)
        {
            goto end;
        }
        
        //ɾ����·��
        route_ip.addr=(pstGetCfg->ulIp&pstGetCfg->ulNetMask);
        sal_route_delete_prv(&route_ip);
        
        //����µ�·��
        route_mask.addr=pstSetCfg->ulNetMask;
        route_gw.addr=pstSetCfg->ulIp;
        route_ip.addr=(pstSetCfg->ulNetMask)&(pstSetCfg->ulIp);
        sal_route_add_prv(ucIfName,&route_ip,&route_mask,&route_gw);
            
        //�޸�mac��ַ
        memcpy_s((u8_t *)stIfreq.ifr_hwaddr.sa_data,6,pstSetCfg->aucMac,6);
        if(ioctl(fd, SIOCSIFHWADDR, &stIfreq)<0)
        {
            goto end;
        }
        
    }
    
    ret=HI_ERR_SUCCESS;
end:
    if(fd!=HI_SYS_INVALID_SOCKET)
    {
        close(fd);
    }
    return ret;
}


HI_U32 SAL_SetEthCfg(HI_IN char * ucIfName, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize)
{
    SAL_IP_MASK_MAC_PRV_S stGetCfg;
    if (usCmdParamSize != sizeof(DIAG_CMD_ETH_CONFIG_STRU)||pCmdParam==HI_NULL||ucIfName==HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    return set_get_ip(ucIfName,pCmdParam,&stGetCfg,HI_TRUE);

}

HI_U32 SAL_GetEthCfg(HI_IN char * ucIfName,HI_IN HI_PVOID pCmdParam ,HI_U16 usCmdParamSize)
{
    if (usCmdParamSize != sizeof(DIAG_CMD_ETH_CONFIG_STRU)||pCmdParam==HI_NULL||ucIfName==HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    return set_get_ip(ucIfName,HI_NULL,pCmdParam,HI_FALSE);
}

HI_U32 SAL_SetMtu(HI_IN const char * ucIfName,HI_U32 ulMtuSize)
{
    struct netif *stNetif = HI_NULL;

    /* Get a pointer to the device */
    stNetif = netif_find(ucIfName);
    
    if (stNetif != HI_NULL)
    {
        netif_set_mtu(stNetif,(HI_U16)ulMtuSize);
    }

    return HI_ERR_SUCCESS;

}


HI_PRV HI_U32 sal_route_add_prv(HI_PCSTR ucIfName,ip_addr_t* addrIp, ip_addr_t* addrMask, ip_addr_t* addrGw)
{
    struct netif *pstNetif = HI_NULL;
    pstNetif = netif_find(ucIfName);
    
    PRINTK("[%s][ucIfName=%s][ip_addr=0x%08x][net_mask=0x%08x][g_w=0x%08x][pstNetif=0x%08x]\n",__func__,ucIfName,addrIp->addr,addrMask->addr,addrGw->addr,pstNetif);
    if(pstNetif == HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    //    return ip_route_add(addrIp,addrMask,addrGw,pstNetif);
    if (lwip_ip4_add_route_entry(addrIp,addrMask,addrGw,(HI_CHAR*)ucIfName))
    {
        return HI_ERR_FAILURE;
    }
    
    return HI_ERR_SUCCESS;
}

HI_PRV HI_U32 sal_route_delete_prv(ip_addr_t* addrIp)
{
    if (!addrIp)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    PRINTK("[%s][ip_addr=0x%08x]\n",__func__,addrIp->addr);
    //    return ip_route_delete(addrIp);
    lwip_ip4_remove_route_entry(addrIp, NULL);
    return HI_ERR_SUCCESS;
}

//���·�ɽӿ�
HI_U32 SAL_Add_Route(HI_PCSTR ucIfName, HI_U8 arrayIp[HI_IPV4_ADDR_LEN], HI_U8 arrayMask[HI_IPV4_ADDR_LEN],HI_U8 arrayGw[HI_IPV4_ADDR_LEN])
{
    //HI_U32 ret = HI_ERR_SUCCESS;
    ip_addr_t addrIp, addrMask, addrGw;
    IP4_ADDR(&addrGw, arrayGw[0], arrayGw[1], arrayGw[2], arrayGw[3]);
    IP4_ADDR(&addrIp, arrayIp[0], arrayIp[1], arrayIp[2], arrayIp[3]);
    IP4_ADDR(&addrMask, arrayMask[0], arrayMask[1], arrayMask[2], arrayMask[3]);
    return sal_route_add_prv(ucIfName,&addrIp,&addrMask,&addrGw);
    //printf("SAL_Add_Route[ucIfName=%s][ip_addr=0x%08x][net_mask=0x%08x][g_w=0x%08x][ret=0x%08x]\n",ucIfName,addrIp.addr,addrMask.addr,addrGw.addr,ret);
    //return ret;

}

//���·�ɽӿڣ�������ӿ�ͬ����
HI_U32 SAL_routeAdd(HI_U8 *arrayIp, HI_U8 *arrayMask, HI_U8 *arrayGw, HI_PCSTR ucIfName)
{
    //HI_U32 ret = HI_ERR_SUCCESS;
    ip_addr_t addrIp, addrMask, addrGw;
    IP4_ADDR(&addrGw, arrayGw[0], arrayGw[1], arrayGw[2], arrayGw[3]);
    IP4_ADDR(&addrIp, arrayIp[0], arrayIp[1], arrayIp[2], arrayIp[3]);
    IP4_ADDR(&addrMask, arrayMask[0], arrayMask[1], arrayMask[2], arrayMask[3]);
    return sal_route_add_prv(ucIfName,&addrIp,&addrMask,&addrGw);
    //printf("SAL_routeAdd[ucIfName=%s][ip_addr=0x%08x][net_mask=0x%08x][g_w=0x%08x][ret=0x%08x]\n",ucIfName,addrIp.addr,addrMask.addr,addrGw.addr,ret);
}

//ɾ��·�ɽӿ�
HI_U32 SAL_routeDelete(HI_U8 ip_address[4])
{
    ip_addr_t ipaddr;
    
    IP4_ADDR(&ipaddr, ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
    return sal_route_delete_prv(&ipaddr);
}

HI_U32 SAL_route_table_show(HI_CHAR *buf, HI_U32 buf_len) 
{
    lwip_route_table_show(buf, buf_len);
    return HI_ERR_SUCCESS;
}

HI_U32 SAL_SetDeafultGateway(HI_PCSTR pszName, HI_U8 ucGateway[HI_IPV4_ADDR_LEN])
{
	struct netif* netif = netif_find(pszName);
	if (netif != NULL) 
	{
		IP4_ADDR(&netif->gw, ucGateway[0], ucGateway[1], ucGateway[2], ucGateway[3]);
		return HI_ERR_SUCCESS;
	}
	   
	return HI_ERR_FAILURE;
}
HI_U32 SAL_GetDeafultGateway(HI_PCSTR pszName, HI_U8 ucGateway[HI_IPV4_ADDR_LEN])
{
	struct netif* netif = netif_find(pszName);
	
	if (netif != NULL) 
	{
		*(HI_U32*)ucGateway = netif->gw.addr;
		return HI_ERR_SUCCESS;
	}
	
	return HI_ERR_FAILURE;
}	


