#ifndef __SAL_OS_NET_H__
#define __SAL_OS_NET_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <sal_os_net_api.h>
#include <netif.h>
#include <hi_mdm.h>

typedef struct
{
    HI_PVOID pBuf;//buf指针
    HI_U32 ulBufLen;//buf长度
}HI_NET_IF_BUF_S;//数据buf


#define HI_NET_INVALID_HANDLE 0xffffffff//非法句柄
#define HI_NET_IF_MAX_COUNT 3//支持网卡个数
#define OUT_BUF_MAX_COUT 32//每个报文最多由多少个pbuf组成
#define HI_NET_IF_INITED_MAGIC 0x9810//魔术字,表示网卡初始完成

typedef struct
{
    HI_U32 alloced:1;//该资源已被申请
    HI_U32 inited:16;//HI_NET_IF_INITED_MAGIC:初始化成功,其它初始化失败
    HI_U32 pad:15;
    
    HI_U32 handle;//用户handle
    HI_CHAR name[HI_NET_MAX_NAME_LEN];//网卡名称:由于netif->name不可以任意命名增加
    
    hi_net_link_out pfnLinkOut;//链路层发送数据回调指针，用于ETH网卡
    hi_net_set_mac pfnSetMac;//设置MAC地址回调指针
    hi_net_ip_out pfnIpOut;//IP层发送数据回调指针，用于非ETH网卡
    struct netif netif;//适配底层协议栈
}HI_NET_IF_ADAPTER_S;//网卡适配结构体

typedef struct
{
    
    LOS_DL_LIST  list;//链表
    int b_used;//是否被占用
    ip_addr_t ip_addr;//目标IP
    ip_addr_t netmask;//子网掩码
    ip_addr_t gw;//网关
    struct netif * netif;    
}HI_IP_ROUTE_ITEM_S;//路由表项


//rt相关
#define HI_IP_TOUTE_TBL_SIZE 10//最多路由条目数
HI_EXTERN HI_IP_ROUTE_ITEM_S g_ip_route_tbl[HI_IP_TOUTE_TBL_SIZE];//路由表资源
HI_EXTERN LOS_DL_LIST g_ip_route_head;//路由表头
HI_EXTERN HI_NET_IF_ADAPTER_S * net_if_handle2adapter(HI_U32 handle);
HI_EXTERN struct etharp_entry* etharp_get_arp_tbl(void);
HI_EXTERN HI_U32 SAL_DelArp(HI_U8 ipAddr[4]);
HI_EXTERN HI_U32 SAL_ArpGetTblSize(HI_VOID);
HI_EXTERN HI_U32 SAL_ArpGetInfo(HI_ARP_ITEM_S  ItemInfo[],HI_U32 *count);
#if HI_NET_MAX_HWADDR_LEN!=NETIF_MAX_HWADDR_LEN//TODO :重复定义
#error "HI_NET_MAX_HWADDR_LEN!=NETIF_MAX_HWADDR_LEN"
#endif
#endif
