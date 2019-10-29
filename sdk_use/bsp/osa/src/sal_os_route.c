#include <hi_types.h>
#include <hi_mdm_types.h>
#include <netif.h>
#include <los_list.h>
#include "sal_os_net.h"
#include "hi_stdlib.h"

HI_IP_ROUTE_ITEM_S g_ip_route_tbl[HI_IP_TOUTE_TBL_SIZE];/*路由表资源*/
LOS_DL_LIST g_ip_route_head;/*路由表头*/

/*申请路由表中一项*/
HI_IP_ROUTE_ITEM_S * malloc_route_item(HI_VOID)
{
    int i = 0;
    for(i=0;i<HI_IP_TOUTE_TBL_SIZE;i++)
    {
        if(g_ip_route_tbl[i].b_used==0)
        {
            g_ip_route_tbl[i].b_used=1;
            return &g_ip_route_tbl[i];
        }
    }

    return HI_NULL;
}

/*释放路由表中一项*/
HI_VOID free_route_item(HI_IP_ROUTE_ITEM_S *item)
{
    memset_s((HI_VOID *)item, sizeof(HI_IP_ROUTE_ITEM_S), 0, sizeof(HI_IP_ROUTE_ITEM_S));
}

/*比较2个路由项优先级高低*/
int ip_route_is_pri_higher(HI_IP_ROUTE_ITEM_S * pHighItem,HI_IP_ROUTE_ITEM_S * pLowItem)
{
    if(pHighItem->netmask.addr > pLowItem->netmask.addr)
    {
        return 1;
    }
    return 0;
}

/*添加一个路由项*/
HI_U32 ip_route_add(ip_addr_t* ip_address, ip_addr_t* netmask, ip_addr_t* gw, struct netif * pstnetif)
{
    HI_IP_ROUTE_ITEM_S *pEntry = HI_NULL;
    LOS_DL_LIST *pNode  = HI_NULL;
    // TODO:路由表合法性判断
	HI_IP_ROUTE_ITEM_S *pNewEntry = malloc_route_item();
    
    if(pNewEntry==0)
    {
        return HI_ERR_FAILURE;
    }

    pNewEntry->netif = pstnetif;
    ip_addr_copy(pNewEntry->ip_addr,*ip_address);
    ip_addr_copy(pNewEntry->netmask,*netmask);
    ip_addr_copy(pNewEntry->gw,*gw);
    //printf("add_gw[0x08%x]\n",*gw);

        
    for (pNode = g_ip_route_head.pstNext; pNode != &g_ip_route_head; pNode = pNode->pstNext)
    {
        pEntry = LOS_DL_LIST_ENTRY(pNode,HI_IP_ROUTE_ITEM_S,list);
        
        if(ip_route_is_pri_higher(pNewEntry,pEntry))
        {
            //printf("insert[0x%08x]\n",(&(pEntry->list))->pstPrev);
            LOS_ListTailInsert(&(pEntry->list),&(pNewEntry->list));
            return HI_ERR_SUCCESS;
        }
    }

    LOS_ListTailInsert(&g_ip_route_head,&(pNewEntry->list));
    return HI_ERR_SUCCESS;
}

/*删除一个路由项*/
int ip_route_delete(ip_addr_t* ip_address)
{
    HI_IP_ROUTE_ITEM_S *pEntry = HI_NULL;

    LOS_DL_LIST *pNode  = HI_NULL;

    for (pNode = g_ip_route_head.pstNext; pNode != &g_ip_route_head; pNode = pNode->pstNext)
    {
        pEntry = LOS_DL_LIST_ENTRY(pNode,HI_IP_ROUTE_ITEM_S,list);
        if(ip_addr_cmp(&(pEntry->ip_addr),ip_address))
        {
            LOS_ListDelete(&(pEntry->list));
            free_route_item(pEntry);
            return 0;
        }
    }
    return -1;
}

/*
路由表匹配
dest：输入时为目的地址，输出时为下一条地址
返回值：目的网卡
*/
struct netif * ip_route_find(ip_addr_t *dest)
{
    HI_IP_ROUTE_ITEM_S *pEntry = HI_NULL;

    LOS_DL_LIST *pNode  = HI_NULL;

    for (pNode = g_ip_route_head.pstNext; pNode != &g_ip_route_head; pNode = pNode->pstNext)
    {
        pEntry = LOS_DL_LIST_ENTRY(pNode,HI_IP_ROUTE_ITEM_S,list);
        if(ip_addr_netcmp(dest,&(pEntry->ip_addr),&(pEntry->netmask)))
        {
            if(!ip_addr_cmp(&(pEntry->gw),&(pEntry->netif->ip_addr)))
            {
                ip_addr_copy(*dest,pEntry->gw);//iplite_ip4_get_addr_type ʱdo
            }
            return pEntry->netif;
        }
    }

    return HI_NULL;
}

/*获取路由表信息，维测用*/
HI_U32 SAL_RouteGet(print_rt_fn pfnPrint,HI_U32 ulUsrData)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_IP_ROUTE_ITEM_S *pEntry = HI_NULL;
    LOS_DL_LIST *pNode  = HI_NULL;
    HI_CHAR netif_name[20];

    for (pNode = g_ip_route_head.pstNext; pNode != &g_ip_route_head; pNode = pNode->pstNext)
    {
        pEntry = LOS_DL_LIST_ENTRY(pNode,HI_IP_ROUTE_ITEM_S,list);
        int ret1 = 0;
        ret1 = snprintf_s(netif_name, sizeof(netif_name), sizeof(netif_name) - 1, "%s%u", pEntry->netif->name, pEntry->netif->num);
        ret = pfnPrint(netif_name,ip4_addrs(&(pEntry->ip_addr)),ip4_addrs(&(pEntry->netmask)),ip4_addrs(&(pEntry->gw)),ulUsrData);
        if(ret)
        {
            return ret;
        }
        if(ret1 < 0){
            continue;
        }
    }
    return ret;
}
/*路由表初始化*/
void ip_route_init(HI_VOID)
{
    g_ip_route_head.pstNext = &g_ip_route_head;
    g_ip_route_head.pstPrev= &g_ip_route_head;
}

