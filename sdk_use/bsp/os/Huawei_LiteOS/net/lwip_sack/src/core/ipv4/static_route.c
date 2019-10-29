#include "lwip/static_route.h"

struct ip4_route_entry {    
    ip_addr_t rt_dst;
    ip_addr_t rt_genmask;
    ip_addr_t rt_gateway;
    struct netif * netif;
};

static struct ip4_route_entry static_route_table[LWIP_IPV4_NUM_ROUTE_ENTRIES];

u32_t lwip_htonl(u32_t n)
{
  return ((n & 0xff) << 24) | ((n & 0xff00) << 8) |
         ((n & 0xff0000UL) >> 8) |((n & 0xff000000UL) >> 24);
}

#define IP_LOWER_NETMASK(ipaddr, netmask) ((ip4_addr4(ipaddr) <= ip4_addr4(netmask)) && (ip4_addr3(ipaddr) <= ip4_addr3(netmask)) \
                                        && (ip4_addr2(ipaddr) <= ip4_addr2(netmask)) && (ip4_addr1(ipaddr) <= ip4_addr1(netmask)))
                                            
err_t lwip_ip4_add_route_entry(ip_addr_t *ipaddr, ip_addr_t *netmask, ip_addr_t *gateway, char *ifname)
{
    struct netif *netif;
    int i;
    char netif_name[IFNAMSIZ];
    
    if (NULL == ipaddr || NULL == netmask  || NULL == gateway) {
        return ERR_ARG;
    }

    if(ip_addr_netmask_valid(netmask) == 0 ||  !IP_LOWER_NETMASK(ipaddr, netmask))
    {
        return ERR_ARG;
    }
    
    netif = netif_find(ifname);
    if (NULL == netif)
    {
        return ERR_RTE;
    }

    for (i = 0; i < LWIP_IPV4_NUM_ROUTE_ENTRIES; i++) {
        if(NULL != static_route_table[i].netif) {
            MEMSET_S(netif_name, sizeof(netif_name), 0, sizeof(netif_name));             
            (void)SNPRINTF_S(netif_name, IFNAMSIZ, (IFNAMSIZ-1), "%s", static_route_table[i].netif->name);
            if (ip_addr_cmp(ipaddr, &static_route_table[i].rt_dst)
                && ip_addr_cmp(netmask, &static_route_table[i].rt_genmask)
                && ip_addr_cmp(gateway, &static_route_table[i].rt_gateway)
                && !STRNCMP(netif_name, ifname, IFNAMSIZ)) {
                return ERR_OK;
            }
        } else {
            break;
        }
    }

    if (i == LWIP_IPV4_NUM_ROUTE_ENTRIES)
    {
        LWIP_DEBUGF(IP_DEBUG | LWIP_DBG_LEVEL_WARNING, ("static table is full\n"));
        return ERR_IF;
    }

    ip_addr_copy(static_route_table[i].rt_dst, *ipaddr);
    ip_addr_copy(static_route_table[i].rt_genmask, *netmask);
    ip_addr_copy(static_route_table[i].rt_gateway, *gateway);
    static_route_table[i].netif = netif;
    
    return ERR_OK;
    
}

err_t lwip_ip4_remove_route_entry(ip_addr_t *ipaddr, char *ifname)
{
    struct netif *netif;
    int i;
    int ret = ERR_NODEVADDR;
    char netif_name[IFNAMSIZ];
    
    if (NULL == ipaddr) {
        return ERR_ARG;
    }
    
    for (i = 0; i < LWIP_IPV4_NUM_ROUTE_ENTRIES; i++) {
        netif = static_route_table[i].netif;
        if (NULL != netif) {
            (void)MEMSET_S(netif_name, sizeof(netif_name), 0, sizeof(netif_name));             
            (void)SNPRINTF_S(netif_name, IFNAMSIZ, (IFNAMSIZ-1), "%s", static_route_table[i].netif->name);
            if (ip_addr_cmp(ipaddr, &static_route_table[i].rt_dst) 
                &&(NULL == ifname || !STRNCMP(netif_name, ifname, IFNAMSIZ))) {      
                (void)MEMSET_S(&static_route_table[i], sizeof(struct ip4_route_entry), 0, sizeof(struct ip4_route_entry));
                ret = ERR_OK;
            }
        }
    }

    return ret;
}

struct netif *ip4_static_route_lookup(ip_addr_t *dest, ip_addr_t *nxhp)
{
    int i;
    int index = -1;
    struct netif *netif;

    for (i = 0; i < LWIP_IPV4_NUM_ROUTE_ENTRIES; i++) {
        netif = static_route_table[i].netif;
        if (NULL != netif) {
            if (ip_addr_netcmp(dest, &static_route_table[i].rt_dst, &static_route_table[i].rt_genmask)) {      
                if (index == -1 || static_route_table[i].rt_genmask.addr > static_route_table[index].rt_genmask.addr) { 
                    index = i;                    
                }
            }
        }
    }
    if (index != -1) {
        if (NULL != nxhp) {
            if (ip_addr_cmp(&static_route_table[index].rt_gateway, &static_route_table[index].netif->ip_addr)) {
                ip_addr_copy(*nxhp, *dest);
            } else {
                ip_addr_copy(*nxhp, static_route_table[index].rt_gateway);
            }			
        }
        return static_route_table[index].netif;
    } else {
        return NULL;
    }
}

err_t lwip_route_table_show(char *printf_buf, unsigned int buf_len)
{
    int i;
    int ret;
    struct netif *netif;
    char *tmp = printf_buf;
    char dst[16];
    char gw[16];
    char mask[16];

    if (NULL == printf_buf) {
        return ERR_ARG;
    }
    
    ret = SNPRINTF_S(tmp, buf_len, (buf_len-1), "\n%-24s%-24s%-24s%-24s\n","Destination", "Gateway", "Genmask", "Iface"); 
    if((ret <= 0) || ((unsigned int)ret >= buf_len))
        return ERR_ARG;
    tmp += ret;
    buf_len -= (unsigned int)ret;

    for (i = 0; i < LWIP_IPV4_NUM_ROUTE_ENTRIES; ++i) {
        netif = static_route_table[i].netif;
        if (NULL != netif)
        {
            if(NULL != ipaddr_ntoa_r(&static_route_table[i].rt_dst, dst, 16)
                && NULL != ipaddr_ntoa_r(&static_route_table[i].rt_gateway, gw, 16)
                && NULL != ipaddr_ntoa_r(&static_route_table[i].rt_genmask, mask, 16)){
                ret = SNPRINTF_S(tmp, buf_len, (buf_len-1), "%-24s%-24s%-24s%s\n", dst,gw,mask, 
                                 static_route_table[i].netif->name);
                if((ret <= 0) || ((unsigned int)ret >= buf_len))
                    return ERR_ARG;
                tmp += ret;
                buf_len -= (unsigned int)ret; 
            }         
        }
    }
    return ERR_OK;
}

