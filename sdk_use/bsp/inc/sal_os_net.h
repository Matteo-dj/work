#ifndef __SAL_OS_NET_H__
#define __SAL_OS_NET_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <sal_os_net_api.h>
#include <netif.h>
#include <hi_mdm.h>

typedef struct
{
    HI_PVOID pBuf;//bufָ��
    HI_U32 ulBufLen;//buf����
}HI_NET_IF_BUF_S;//����buf


#define HI_NET_INVALID_HANDLE 0xffffffff//�Ƿ����
#define HI_NET_IF_MAX_COUNT 3//֧����������
#define OUT_BUF_MAX_COUT 32//ÿ����������ɶ��ٸ�pbuf���
#define HI_NET_IF_INITED_MAGIC 0x9810//ħ����,��ʾ������ʼ���

typedef struct
{
    HI_U32 alloced:1;//����Դ�ѱ�����
    HI_U32 inited:16;//HI_NET_IF_INITED_MAGIC:��ʼ���ɹ�,������ʼ��ʧ��
    HI_U32 pad:15;
    
    HI_U32 handle;//�û�handle
    HI_CHAR name[HI_NET_MAX_NAME_LEN];//��������:����netif->name������������������
    
    hi_net_link_out pfnLinkOut;//��·�㷢�����ݻص�ָ�룬����ETH����
    hi_net_set_mac pfnSetMac;//����MAC��ַ�ص�ָ��
    hi_net_ip_out pfnIpOut;//IP�㷢�����ݻص�ָ�룬���ڷ�ETH����
    struct netif netif;//����ײ�Э��ջ
}HI_NET_IF_ADAPTER_S;//��������ṹ��

typedef struct
{
    
    LOS_DL_LIST  list;//����
    int b_used;//�Ƿ�ռ��
    ip_addr_t ip_addr;//Ŀ��IP
    ip_addr_t netmask;//��������
    ip_addr_t gw;//����
    struct netif * netif;    
}HI_IP_ROUTE_ITEM_S;//·�ɱ���


//rt���
#define HI_IP_TOUTE_TBL_SIZE 10//���·����Ŀ��
HI_EXTERN HI_IP_ROUTE_ITEM_S g_ip_route_tbl[HI_IP_TOUTE_TBL_SIZE];//·�ɱ���Դ
HI_EXTERN LOS_DL_LIST g_ip_route_head;//·�ɱ�ͷ
HI_EXTERN HI_NET_IF_ADAPTER_S * net_if_handle2adapter(HI_U32 handle);
HI_EXTERN struct etharp_entry* etharp_get_arp_tbl(void);
HI_EXTERN HI_U32 SAL_DelArp(HI_U8 ipAddr[4]);
HI_EXTERN HI_U32 SAL_ArpGetTblSize(HI_VOID);
HI_EXTERN HI_U32 SAL_ArpGetInfo(HI_ARP_ITEM_S  ItemInfo[],HI_U32 *count);
#if HI_NET_MAX_HWADDR_LEN!=NETIF_MAX_HWADDR_LEN//TODO :�ظ�����
#error "HI_NET_MAX_HWADDR_LEN!=NETIF_MAX_HWADDR_LEN"
#endif
#endif
