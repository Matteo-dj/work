#ifndef __SAL_OS_SYS_H__
#define __SAL_OS_SYS_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <netif.h>
#include <sal_os_net_api.h>

#if HI_NET_MAX_HWADDR_LEN!=NETIF_MAX_HWADDR_LEN//TODO :重复定义
#error "HI_NET_MAX_HWADDR_LEN!=NETIF_MAX_HWADDR_LEN"
#endif

/*下面导出函数仅OSA层可用*/

    /*lint -e10 -e40*/
    /*lint +e10 +e40*/
HI_EXTERN HI_U32 ms2systick (HI_IN HI_U32 ulms, HI_IN HI_BOOL include0);
HI_EXTERN HI_U32 SAL_IsTmLegal(HI_IN HI_SYS_CALENDAR_TIME_S * calendarTime);
HI_EXTERN struct netif * ip_route_find(ip_addr_t *dest);
HI_EXTERN HI_U32 ip_route_add(ip_addr_t* ip_address, ip_addr_t* netmask, ip_addr_t* gw, struct netif * pstnetif);
#endif
