/**********************************************************************************
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************/

/**********************************************************************************
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which
 * might include those applicable to Huawei LiteOS of U.S. and the country in which you
 * are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance
 * with such applicable export control laws and regulations.
 **********************************************************************************/

#include "lwip/opt.h"

#if (LWIP_ENABLE_LOS_SHELL_CMD == 1) || defined(LOSCFG_PLATFORM_HI3911)
#include "lwip/sockets.h"
#include "lwip/inet.h"
#include "lwip/inet_chksum.h"
#include "lwip/icmp.h"
#include "securec.h"
extern int tcpip_init_finish;
extern UINT32 GetTimer2Value(void);
#include "lwip/api_shell.h"
#if (LWIP_ENABLE_LOS_SHELL_CMD == 1)
#include "lwip/tcpip.h"
#include "lwip/netif.h"
#include "lwip/netdb.h"
#include "lwip/stats.h"
#include "lwip/err.h"
#include "netif/etharp.h"
#include "lwip/ip_addr.h"
#include "los_sys.h"
#include "los_config.h"
#include <string.h>
#include "limits.h"
#include <stdlib.h>
#include <stdio.h>
#include "lwip/api_shell.h"
#include "lwip/tftpc.h"
#include "lwip/sntp.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "lwip/udp.h"
#include "lwip/tcp_impl.h"

#ifdef LOSCFG_SHELL
#include "shcmd.h"
#include "shell.h"
#endif

extern struct etharp_entry arp_table[ARP_TABLE_SIZE];
extern const char * const tcp_state_str[];

#define IFCONFIG_OPTION_SET_IP          (1)
#define IFCONFIG_OPTION_SET_NETMASK     (1 << 1)
#define IFCONFIG_OPTION_SET_GW          (1 << 2)
#define IFCONFIG_OPTION_SET_HW          (1 << 3)
#define IFCONFIG_OPTION_SET_UP          (1 << 4)
#define IFCONFIG_OPTION_SET_DOWN        (1 << 5)

#define NETSTAT_ENTRY_SIZE 120
#define MAX_NETSTAT_ENTRY (NETSTAT_ENTRY_SIZE * (MEMP_NUM_TCP_PCB + MEMP_NUM_UDP_PCB  + MEMP_NUM_TCP_PCB_LISTEN + 1))
s8_t g_netstat_buf[MAX_NETSTAT_ENTRY+1] = {0};

#define PRINT_BUF_LEN   1024
#define MAX_MACADDR_STRING_LENGTH    18 /* including NULL */

#define CONVERT_STRING_TO_HEX(_src, _dest)    \
{                                           \
    const char *_srcString = (char *)_src;    \
    _dest = 0;                                  \
    while (*_srcString)                         \
    {                                           \
        _dest = (_dest << 4) & 0xFF;            \
        if ((*_srcString >= 48) && (*_srcString <= 57))    /* between 0 to 9 */  \
            _dest |= (*_srcString - 48);                     \
        else if ((*_srcString >= 65 && *_srcString <= 70)) /* between A to F */  \
            _dest |= ((*_srcString - 65) + 10);              \
        else if ((*_srcString >= 97 && *_srcString <= 102))/* between a to f */  \
            _dest |= ((*_srcString - 97) + 10);              \
        else break;                             \
        ++_srcString;                           \
    }                                           \
}

struct ifconfig_option {
    char            iface[IFNAMSIZ];
    unsigned int    option;
    unsigned int    ip_addr;
    unsigned int    netmask;
    unsigned int    gw;
    unsigned char   ethaddr[6];
    /* when using telnet, printf to the telnet socket will result in system  */
    /* deadlock. So we cahe the prinf data to a buf, and when the tcpip      */
    /* callback returns, then printf the data out to the telnet socket       */
    sys_sem_t       cb_completed;
    char            cb_print_buf[PRINT_BUF_LEN];
    unsigned int    print_len;
};

struct netstat_data {
    s8_t *netstat_out_buf;
    int netstat_out_buf_len;
    int netstat_out_buf_updated_len;
    sys_sem_t cb_completed;
};

struct if_cmd_data {
  char *if_name;
  err_t err;
  sys_sem_t cb_completed;
};

LWIP_STATIC int print_netif(struct netif *netif, char *print_buf, unsigned int buf_len)
{
    int i,ret;
    char *tmp = print_buf;

    if(netif->link_layer_type == LOOPBACK_IF)
      ret = SNPRINTF_S(tmp, buf_len, (buf_len-1), "%s\t", netif->name);
    else
      ret = SNPRINTF_S(tmp, buf_len, (buf_len-1), "%s%u\t", netif->name, netif->num);
    if((ret <= 0) || ((unsigned int)ret >= buf_len))
        goto out;
    tmp += ret;
    buf_len -= (unsigned int)ret;

    ret = SNPRINTF_S(tmp, buf_len, (buf_len-1), "ip:%s ", ipaddr_ntoa(&netif->ip_addr)); //CSEC_FIX_2302
    if((ret <= 0) || ((unsigned int)ret >= buf_len)) //CSEC_FIX_2302
        goto out;;
    tmp += ret;
    buf_len -= (unsigned int)ret;

    ret = SNPRINTF_S(tmp, buf_len, (buf_len-1), "netmask:%s ", ipaddr_ntoa(&netif->netmask)); //CSEC_FIX_2302
    if((ret <= 0) || ((unsigned int)ret >= buf_len)) //CSEC_FIX_2302
        goto out;
    tmp += ret;
    buf_len -= (unsigned int)ret;

    ret = SNPRINTF_S(tmp, buf_len, (buf_len-1), "gateway:%s\n", ipaddr_ntoa(&netif->gw)); //CSEC_FIX_2302
    if((ret <= 0) || ((unsigned int)ret >= buf_len)) //CSEC_FIX_2302
        goto out;
    tmp += ret;
    buf_len -= (unsigned int)ret;

    ret = SNPRINTF_S(tmp, buf_len, (buf_len-1), "\tHWaddr "); //CSEC_FIX_2302
    if((ret <= 0) || ((unsigned int)ret >= buf_len)) //CSEC_FIX_2302
        goto out;
    tmp += ret;
    buf_len -= (unsigned int)ret;

    for(i = 0; i < netif->hwaddr_len - 1; i++)
    {
        ret = SNPRINTF_S(tmp, buf_len, (buf_len-1), "%02x:", netif->hwaddr[i]); //CSEC_FIX_2302
        if((ret <= 0) || ((unsigned int)ret >= buf_len)) //CSEC_FIX_2302
            goto out;
        tmp += ret;
        buf_len -= (unsigned int)ret;
    }

    ret = SNPRINTF_S(tmp, buf_len, (buf_len-1), "%02x", netif->hwaddr[i]); //CSEC_FIX_2302
    if((ret <= 0) || ((unsigned int)ret >= buf_len)) //CSEC_FIX_2302
        goto out;
    tmp += ret;
    buf_len -= (unsigned int)ret;


    ret = SNPRINTF_S(tmp, buf_len, (buf_len-1), " MTU:%d %s", netif->mtu,
        netif->flags&NETIF_FLAG_UP?"Runing":"Stop"); //CSEC_FIX_2302
    if((ret <= 0) || ((unsigned int)ret >= buf_len)) //CSEC_FIX_2302
        goto out;
    tmp += ret;
    buf_len -= (unsigned int)ret;

    if(netif_default == netif)
    {
      ret = SNPRINTF_S(tmp, buf_len, (buf_len-1), " %s", "Default"); //CSEC_FIX_2302
      if((ret <= 0) || ((unsigned int)ret >= buf_len)) //CSEC_FIX_2302
          goto out;
      tmp += ret;
      buf_len -= (unsigned int)ret;
    }

    ret = SNPRINTF_S(tmp, buf_len, (buf_len-1), " %s\n",
        netif->flags&NETIF_FLAG_LINK_UP?"Link UP":"Link Down");//CSEC_FIX_2302
    if((ret <= 0) || ((unsigned int)ret >= buf_len)) //CSEC_FIX_2302
        goto out;
    tmp += ret;

out:
    return (int)(tmp - print_buf);
}

LWIP_STATIC void lwip_ifconfig_show_internal(void *arg)
{
    struct netif *netif;
    struct ifconfig_option *ifconfig_cmd = (struct ifconfig_option *)arg;
    int ret;

    if(netif_list == NULL)
    {
        ret = SNPRINTF_S(ifconfig_cmd->cb_print_buf, PRINT_BUF_LEN - ifconfig_cmd->print_len,
                  ((PRINT_BUF_LEN - ifconfig_cmd->print_len) -1), "Device not init\n"); //CSEC_FIX_2302
        if((ret > 0) && ((unsigned int)ret < (PRINT_BUF_LEN - ifconfig_cmd->print_len))) //CSEC_FIX_2302
            ifconfig_cmd->print_len += (unsigned int)ret;

        sys_sem_signal(&ifconfig_cmd->cb_completed);
        return;
    }

    if(ifconfig_cmd->iface[0] == '\0')
    {
        /* display all netif */
        for(netif = netif_list; netif != NULL; netif = netif->next) {
            ret = print_netif(netif,
                ifconfig_cmd->cb_print_buf + ifconfig_cmd->print_len,
                PRINT_BUF_LEN - ifconfig_cmd->print_len);
            ifconfig_cmd->print_len += (unsigned int)ret;
        }
    }
    else
    {
        netif = netif_find(ifconfig_cmd->iface);
        if(netif == NULL)
        {
            ret = SNPRINTF_S(ifconfig_cmd->cb_print_buf+ ifconfig_cmd->print_len, (PRINT_BUF_LEN - ifconfig_cmd->print_len),
                ((PRINT_BUF_LEN - ifconfig_cmd->print_len) - 1), "Device not found\n"); //CSEC_FIX_2302
            if((ret > 0) && ((unsigned int)ret < (PRINT_BUF_LEN - ifconfig_cmd->print_len))) //CSEC_FIX_2302
                ifconfig_cmd->print_len += (unsigned int)ret;

            sys_sem_signal(&ifconfig_cmd->cb_completed);
            return;
        }

        ret = print_netif(netif,
            ifconfig_cmd->cb_print_buf + ifconfig_cmd->print_len,
            PRINT_BUF_LEN - ifconfig_cmd->print_len);
        ifconfig_cmd->print_len += (unsigned int)ret;
    }
    sys_sem_signal(&ifconfig_cmd->cb_completed);
}

LWIP_STATIC void lwip_ifconfig_internal(void *arg)
{
    struct ifconfig_option *ifconfig_cmd;
    struct netif *netif;
    ip_addr_t ip_addr;
    ip_addr_t netmask;
    ip_addr_t gw;
    int ret;

    ifconfig_cmd = (struct ifconfig_option *)arg;
    netif = netif_find(ifconfig_cmd->iface);
    if(netif == NULL)
    {
        ret = SNPRINTF_S(ifconfig_cmd->cb_print_buf + ifconfig_cmd->print_len, PRINT_BUF_LEN - ifconfig_cmd->print_len,
            ((PRINT_BUF_LEN - ifconfig_cmd->print_len) - 1), "Device not found\n"); //CSEC_FIX_2302
        if((ret > 0) && ((unsigned int)ret < (PRINT_BUF_LEN - ifconfig_cmd->print_len))) //CSEC_FIX_2302
            ifconfig_cmd->print_len += (unsigned int)ret;

        goto out;
    }

    if(ifconfig_cmd->option & IFCONFIG_OPTION_SET_UP)
    {
        netif_set_up(netif);
        goto out;
    }
    else if(ifconfig_cmd->option & IFCONFIG_OPTION_SET_DOWN)
    {
        netif_set_down(netif);
        goto out;
    }

    if(ifconfig_cmd->option & IFCONFIG_OPTION_SET_IP)
    {
        ip_addr.addr = ifconfig_cmd->ip_addr;
        netif_set_down(netif);
        netif_set_ipaddr(netif, &ip_addr);
        netif_set_up(netif);
    }

    if(ifconfig_cmd->option & IFCONFIG_OPTION_SET_NETMASK)
    {
        netmask.addr = ifconfig_cmd->netmask;
        netif_set_netmask(netif, &netmask);
    }

    if(ifconfig_cmd->option & IFCONFIG_OPTION_SET_GW)
    {
        gw.addr = ifconfig_cmd->gw;
        netif_set_gw(netif, &gw);
    }

    if(ifconfig_cmd->option & IFCONFIG_OPTION_SET_HW)
    {
        if (netif->drv_set_hwaddr)
        {
          if(netif->drv_set_hwaddr(netif, ifconfig_cmd->ethaddr, 6))
          {
            ret = SNPRINTF_S(ifconfig_cmd->cb_print_buf + ifconfig_cmd->print_len,
                PRINT_BUF_LEN - ifconfig_cmd->print_len, ((PRINT_BUF_LEN - ifconfig_cmd->print_len) - 1),
                "set hw address fail\n"); //CSEC_FIX_2302
            if((ret > 0) && ((unsigned int)ret < (PRINT_BUF_LEN - ifconfig_cmd->print_len))) //CSEC_FIX_2302
                ifconfig_cmd->print_len += (unsigned int)ret;
          }
        }
        else
        {
          ret = SNPRINTF_S(ifconfig_cmd->cb_print_buf + ifconfig_cmd->print_len,
              PRINT_BUF_LEN - ifconfig_cmd->print_len, ((PRINT_BUF_LEN - ifconfig_cmd->print_len) - 1),
              "drv_set_hwaddr callback have not set.\n"); //CSEC_FIX_2302
          if((ret > 0) && ((unsigned int)ret < (PRINT_BUF_LEN - ifconfig_cmd->print_len))) //CSEC_FIX_2302
              ifconfig_cmd->print_len += (unsigned int)ret;
        }
    }
out:
    sys_sem_signal(&ifconfig_cmd->cb_completed);
}


void lwip_printsize(size_t  size)
{
    static const char *SIZES[] = { "B", "KB", "MB", "GB" };
    size_t divis = 0;
    size_t rem = 0;

    while ((size >= 1024) && (divis < ((sizeof SIZES / sizeof *SIZES) - 1)))
    {
        rem = (size % 1024);
        divis++;
        size /= 1024;
    }

    /*lint -e661*/
    PRINTK("(%.1f %s) \r\n", (float)size + (float)rem / 1024.0, SIZES[divis]);
    /*lint +e661*/
}

LWIP_STATIC void lwip_ifconfig_usage(const char * cmd)
{
    PRINTK("Usage:"\
        "\n%s -a [Interface [address]] [netmask <address>] [gateway <address>]"\
        "[hw ether <address>] [up | down]\n",
        cmd);
}

/*lint -e838*/
u32_t lwip_ifconfig(int argc, char **argv)
{
    int i = 0;
    static struct ifconfig_option ifconfig_cmd;
    unsigned int stat_err_cnt;
    unsigned int stat_drop_cnt;
    unsigned int stat_rx_or_tx_cnt;
    unsigned int stat_rx_or_tx_bytes;
#if LWIP_ARP
    unsigned int old_ipaddr;
#endif

    if(!tcpip_init_finish)
    {
        PRINTK("%s: tcpip_init have not been called\n", __FUNCTION__);
        return 2;
    }
    /* To support "ifconfig -a" command
         RX packets:XXXX errors:X dropped:X overruns:X bytes:XXXX (Human readable format)
         TX packets:XXXX errors:X dropped:X overruns:X bytes:XXXX (Human readable format)

         Below is assumed for 'overrun' stat.
         Linux Kernel:
                RX: FIFO overrun
                    Data structure: net_device->stats->rx_fifo_errors
                    Flag which is marked when FIFO overrun: ENRSR_FO

                Function: ei_receive->ENRSR_FO

                TX: A "FIFO underrun" occurred during transmit.
                    Data structure: net_device->stats->tx_fifo_errors
                    Flag which is marked when FIFO underrun: ENTSR_FU

                Function: ei_tx_intr->ENTSR_FU

        LWIP:
            So in our case,
            while receiving a packet RX case, if the buffer is full (trypost - it is sys_mbox_trypost)
            the error will be returned, we can consider that an overflow has happend.
            So this can be RX overrun.

            But while transmitting a packet TX case, underrun cannot happen because it block on the
            message Q if it is full (NOT trypost - it is sys_mbox_post). So TX overrun is always 0.
    */
    if (argc)
    {
        if (strcmp("-a", argv[0]) == 0)
        {
#if LWIP_STATS
            stat_rx_or_tx_cnt     = lwip_stats.ip.recv;
            stat_err_cnt          = (lwip_stats.ip.ip_rx_err
                                                        + lwip_stats.ip.lenerr
                                                        + lwip_stats.ip.chkerr
                                                        + lwip_stats.ip.opterr
                                                        + lwip_stats.ip.proterr);
            stat_drop_cnt         = (lwip_stats.ip.drop + lwip_stats.link.link_rx_drop);
            stat_rx_or_tx_bytes = lwip_stats.ip.ip_rx_bytes;

            PRINTK("RX packets:%u\t errors:%u\t dropped:%u\t overrun:%d\t bytes:%u ",
            stat_rx_or_tx_cnt,
            stat_err_cnt,
            stat_drop_cnt,
            lwip_stats.ip.link_rx_overrun,
            stat_rx_or_tx_bytes);

            /* Print in Human readable format of the incoming bytes */
            lwip_printsize(lwip_stats.ip.ip_rx_bytes);


            stat_rx_or_tx_cnt     = (lwip_stats.ip.fw + lwip_stats.ip.xmit);
            stat_err_cnt          = (lwip_stats.ip.rterr + lwip_stats.ip.ip_tx_err);
            stat_drop_cnt         = lwip_stats.link.link_tx_drop; /* IP layer drop stat param is not maintained, failure at IP is considered in 'errors' stat */
            stat_rx_or_tx_bytes   = lwip_stats.ip.ip_tx_bytes;

            PRINTK("TX packets:%u\t errors:%u\t dropped:%u\t overrun:0\t bytes:%u",
                stat_rx_or_tx_cnt,
                stat_err_cnt,
                stat_drop_cnt,
                stat_rx_or_tx_bytes);

            /* Print in Human readable format of the outgoing bytes */
            lwip_printsize(lwip_stats.ip.ip_tx_bytes);
#endif /* LWIP_STATS */
            return 0;
        }
    }

    (void)MEMSET_S(&ifconfig_cmd, sizeof(ifconfig_cmd), 0, sizeof(ifconfig_cmd)); //CSEC_FIX_2302
    if (sys_sem_new(&ifconfig_cmd.cb_completed, 0) != ERR_OK) {
        PRINTK("%s: sys_sem_new fail\n", __FUNCTION__);
    }

    i        = 0;
    /* Get the interface */
    if(argc > 0)
    {
        if (strlen(argv[i]) < IFNAMSIZ)
        {
            STRNCPY_S(ifconfig_cmd.iface, IFNAMSIZ, argv[i], (strlen(argv[i])));
        }
        else
        {
            sys_sem_free(&ifconfig_cmd.cb_completed);
            PRINTK("ifconfig : interface name is too big");
            return 0;
        }
        i++;
        argc--;
        if(argc == 0)
        {
            /* no more arguments, show the interface state. */
            tcpip_callback(lwip_ifconfig_show_internal, &ifconfig_cmd);
            sys_arch_sem_wait(&ifconfig_cmd.cb_completed, 0);
            sys_sem_free(&ifconfig_cmd.cb_completed);
            PRINTK("%s", ifconfig_cmd.cb_print_buf);
            return 0;
        }
    }
    else
    {
        /* no more arguments, show all the interface state. */
        tcpip_callback(lwip_ifconfig_show_internal, &ifconfig_cmd);
        sys_arch_sem_wait(&ifconfig_cmd.cb_completed, 0);
        sys_sem_free(&ifconfig_cmd.cb_completed);
        ifconfig_cmd.cb_print_buf[PRINT_BUF_LEN -1] = '\0';
        PRINTK("%s", ifconfig_cmd.cb_print_buf);

        return 0;
    }

    /* ifup/ifdown */
    if(strcmp("up", argv[i]) == 0)
    {
        ifconfig_cmd.option |= IFCONFIG_OPTION_SET_UP;
        /* setup the interface, other arguments is ignored. */
        tcpip_callback(lwip_ifconfig_internal, &ifconfig_cmd);
        sys_arch_sem_wait(&ifconfig_cmd.cb_completed, 0);
        sys_sem_free(&ifconfig_cmd.cb_completed);
        ifconfig_cmd.cb_print_buf[PRINT_BUF_LEN -1] = '\0';
        PRINTK("%s", ifconfig_cmd.cb_print_buf);
        return 0;
    }
    else if(strcmp("down", argv[i]) == 0)
    {
        ifconfig_cmd.option |= IFCONFIG_OPTION_SET_DOWN;
        /* setdown the interface, other arguments is ignored. */
        tcpip_callback(lwip_ifconfig_internal, &ifconfig_cmd);
        sys_arch_sem_wait(&ifconfig_cmd.cb_completed, 0);
        sys_sem_free(&ifconfig_cmd.cb_completed);
        ifconfig_cmd.cb_print_buf[PRINT_BUF_LEN -1] = '\0';
        PRINTK("%s", ifconfig_cmd.cb_print_buf);
        return 0;
    }

    /* check if set the ip address. */
#if LWIP_ARP
    old_ipaddr = ipaddr_addr(ipaddr_ntoa(&netif_find(ifconfig_cmd.iface)->ip_addr));
#endif
    ifconfig_cmd.ip_addr = ipaddr_addr(argv[i]);
    if(ifconfig_cmd.ip_addr != IPADDR_NONE)
    {
        /* set the ip address */
        ifconfig_cmd.option |= IFCONFIG_OPTION_SET_IP;
        i++;
        argc--;
        /* there is no more arguments */
        if(argc == 0)
        {
#if LWIP_ARP
            err_t err;
            unsigned int uwRet = 0;
            uwRet = LOS_SemCreate(0, &uarpsemlock);
            if (uwRet)
            {
                PRINT_ERR("%s: LOS_SemCreate failed[0x%x].\n", __FUNCTION__, uwRet);
                return 1;
            }
#endif
            tcpip_callback(lwip_ifconfig_internal, &ifconfig_cmd);
            sys_arch_sem_wait(&ifconfig_cmd.cb_completed, 0);
            sys_sem_free(&ifconfig_cmd.cb_completed);
            ifconfig_cmd.cb_print_buf[PRINT_BUF_LEN -1] = '\0';
            PRINTK("%s", ifconfig_cmd.cb_print_buf);

#if LWIP_ARP
            err = LOS_SemPend(uarpsemlock, 2 * LOSCFG_BASE_CORE_TICK_PER_SECOND);
            uwRet = LOS_SemDelete(uarpsemlock);
            if (uwRet)
            {
                PRINT_ERR("%s: LOS_SemDelete failed[0x%x].\n", __FUNCTION__, uwRet);
                return 1;
            }

            if (err == 0)
            {
                PRINT_ERR("%s: Ip conflict!\n", __FUNCTION__);
                ifconfig_cmd.ip_addr = old_ipaddr;
                tcpip_callback(lwip_ifconfig_internal, &ifconfig_cmd);
                sys_arch_sem_wait(&ifconfig_cmd.cb_completed, 0);
                sys_sem_free(&ifconfig_cmd.cb_completed);
                ifconfig_cmd.cb_print_buf[PRINT_BUF_LEN -1] = '\0';
                PRINTK("%s", ifconfig_cmd.cb_print_buf);

                return 1;
            }
            else if (err != LOS_ERRNO_SEM_TIMEOUT)
            {
                PRINT_ERR("%s: semaphore pend error [0x%x].\n", __FUNCTION__, err);
                return 1;
            }
#endif
            return 0;
        }
    }

    while(argc > 0)
    {
        /* if set netmask */
        if(strcmp("netmask", argv[i]) == 0 && (argc > 1) &&
            (ipaddr_addr(argv[i+1]) != IPADDR_NONE))
        {
            ifconfig_cmd.netmask = ipaddr_addr(argv[i+1]);
            ifconfig_cmd.option |= IFCONFIG_OPTION_SET_NETMASK;
            i += 2;
            argc -= 2;
        }
        /* if set gateway */
        else if(strcmp("gateway", argv[i]) == 0 && (argc > 1) &&
            (ipaddr_addr(argv[i+1]) != IPADDR_NONE))
        {
            ifconfig_cmd.gw = ipaddr_addr(argv[i+1]);
            ifconfig_cmd.option |= IFCONFIG_OPTION_SET_GW;
            i += 2;
            argc -= 2;
        }
        /* if set HWaddr */
        /*lint -e561*/
        else if(strcmp("hw", argv[i]) == 0 && argc > 2 &&
            strcmp("ether", argv[i+1]) == 0 )
        {
            char *digit = NULL;
            u32_t macaddrlen = strlen(argv[i+2]) + 1;
            char tmpStr[MAX_MACADDR_STRING_LENGTH];
            char *tmpStr1;
            char *saveptr;
            int j;

            if(macaddrlen != MAX_MACADDR_STRING_LENGTH)
            {
                sys_sem_free(&ifconfig_cmd.cb_completed);
                PRINTK("ifconfig : wrong MAC address format\n");
                return 1;
            }

            STRNCPY_S(tmpStr, macaddrlen, argv[i+2], macaddrlen);
            for(j = 0, tmpStr1 = tmpStr; j < 6; j++, tmpStr1=NULL)
            {
                digit = strtok_r(tmpStr1, ":", &saveptr);

                if((digit == NULL) || (strlen(digit) > 2))
                {
                    sys_sem_free(&ifconfig_cmd.cb_completed);
                    PRINTK("ifconfig : wrong MAC address format\n");
                    return 1;
                }

                CONVERT_STRING_TO_HEX(digit, ifconfig_cmd.ethaddr[j]);
            }
            ifconfig_cmd.option |= IFCONFIG_OPTION_SET_HW;
            i += 3;
            argc -= 3;
        }
        /*lint +e561*/
        else
        {
            sys_sem_free(&ifconfig_cmd.cb_completed);
            goto ifconfig_error;
        }
    }
    tcpip_callback(lwip_ifconfig_internal, &ifconfig_cmd);
    sys_arch_sem_wait(&ifconfig_cmd.cb_completed, 0);
    sys_sem_free(&ifconfig_cmd.cb_completed);
    ifconfig_cmd.cb_print_buf[PRINT_BUF_LEN -1] = '\0';
    PRINTK("%s", ifconfig_cmd.cb_print_buf);
    return 0;

ifconfig_error:
    lwip_ifconfig_usage("ifconfig");
    return 1;
}
/*lint +e838*/
#ifdef LOSCFG_SHELL
SHELLCMD_ENTRY(ifconfig_shellcmd, CMD_TYPE_EX, "ifconfig", XARGS, (CMD_CBK_FUNC)lwip_ifconfig);
#endif /* LOSCFG_SHELL */
/* add arp entry to arp cache */
#define ARP_OPTION_ADD      1
/* delete arp entry to arp cache */
#define ARP_OPTION_DEL      2
/* print all arp entry in arp cache */
#define ARP_OPTION_SHOW     3

struct arp_option {
    /* see the ARP_OPTION_ above */
    int             option;
    /* descriptive abbreviation of network interface */
    char            iface[IFNAMSIZ];
    /* ip addr */
    unsigned int    ipaddr;
    /* hw addr */
    unsigned char            ethaddr[6];
    /* when using telnet, printf to the telnet socket will result in system  */
    /* deadlock.so don't do it.cahe the data to prinf to a buf, and when     */
    /* callback returns, then printf the data out to the telnet socket       */
    sys_sem_t       cb_completed;
    char            cb_print_buf[PRINT_BUF_LEN];
    int             print_buf_len;
};

void lwip_arp_show_internal(char *printf_buf, unsigned int buf_len)
{
    u8_t state, i;
    int ret;
    char *tmp = printf_buf;
    ret = SNPRINTF_S(tmp, buf_len, (buf_len-1), "%-24s%-24s%-24s\n","Address", "HWaddress", "Iface"); //CSEC_FIX_2302
    if((ret <= 0) || ((unsigned int)ret >= buf_len)) //CSEC_FIX_2302
        return;
    tmp += ret;
    buf_len -= (unsigned int)ret;

    for (i = 0; i < ARP_TABLE_SIZE; ++i) {
        state = arp_table[i].state;
        /* no empty entry found yet and now we do find one? */
        if (state == ETHARP_STATE_STABLE)
        {
            ret = SNPRINTF_S(tmp, buf_len, (buf_len-1), "%-24s%02X:%02X:%02X:%02X:%02X:%02X       %s%u\n",
                ipaddr_ntoa(&arp_table[i].ipaddr),
                arp_table[i].ethaddr.addr[0], arp_table[i].ethaddr.addr[1],
                arp_table[i].ethaddr.addr[2], arp_table[i].ethaddr.addr[3],
                arp_table[i].ethaddr.addr[4], arp_table[i].ethaddr.addr[5],
                arp_table[i].netif->name, arp_table[i].netif->num);
            if((ret <= 0) || ((unsigned int)ret >= buf_len))
                return;
            tmp += ret;
            buf_len -= (unsigned int)ret;
        }
    }
}

LWIP_STATIC void lwip_arp_internal(void *arg)
{
    struct arp_option *arp_cmd = (struct arp_option *)arg;
    struct netif *netif;
    struct eth_addr ethaddr;
    ip_addr_t ipaddr;
    err_t ret;

    /* find the specified netif by it's name */
    if(arp_cmd->iface[0] == 'd' && arp_cmd->iface[1] == 'e')
        netif = netif_default;
    else
        netif = netif_find(arp_cmd->iface);

    if(arp_cmd->option == ARP_OPTION_SHOW)
    {
        if (netif == NULL)
        {
            SNPRINTF_S(arp_cmd->cb_print_buf, PRINT_BUF_LEN, (PRINT_BUF_LEN-1), "cannot match %s\n",
                                            arp_cmd->iface);
        }
        else
        {
            lwip_arp_show_internal(arp_cmd->cb_print_buf, PRINT_BUF_LEN);
        }
    }
    else if(arp_cmd->option == ARP_OPTION_ADD)
    {
        if(netif == NULL)
        {
            (void)SNPRINTF_S(arp_cmd->cb_print_buf, PRINT_BUF_LEN, (PRINT_BUF_LEN-1),
                "No such device\n"); //CSEC_FIX_2302
            goto out;
        }

        ipaddr.addr = arp_cmd->ipaddr;
        MEMCPY_S(ethaddr.addr, sizeof(ethaddr.addr), arp_cmd->ethaddr, 6); //CSEC_FIX_2302

        ret = etharp_update_arp_entry(netif, &ipaddr, &ethaddr, ETHARP_FLAG_TRY_HARD);
        if(ret == ERR_MEM)
            (void)SNPRINTF_S(arp_cmd->cb_print_buf, PRINT_BUF_LEN, (PRINT_BUF_LEN-1), "Out of memory error\n"); //CSEC_FIX_2302
        else if(ret == ERR_ARG)
            (void)SNPRINTF_S(arp_cmd->cb_print_buf, PRINT_BUF_LEN, (PRINT_BUF_LEN-1), "Illegal argument\n"); //CSEC_FIX_2302
        else
            (void)SNPRINTF_S(arp_cmd->cb_print_buf, PRINT_BUF_LEN, (PRINT_BUF_LEN-1), "Successed\n"); //CSEC_FIX_2302
    }
    /* Must be ARP_OPTION_DEL */
    else
    {
        if(netif == NULL)
        {
            (void)SNPRINTF_S(arp_cmd->cb_print_buf, PRINT_BUF_LEN, (PRINT_BUF_LEN-1), "No such device\n"); //CSEC_FIX_2302
            goto out;
        }

        ipaddr.addr = arp_cmd->ipaddr;
        MEMCPY_S(ethaddr.addr, sizeof(ethaddr.addr), arp_cmd->ethaddr, 6); //CSEC_FIX_2302

        ret = etharp_delete_arp_entry(netif, &ipaddr);
        if(ret == ERR_MEM)
            (void)SNPRINTF_S(arp_cmd->cb_print_buf, PRINT_BUF_LEN, (PRINT_BUF_LEN-1), "Out of memory error\n"); //CSEC_FIX_2302
        else if(ret == ERR_ARG)
            (void)SNPRINTF_S(arp_cmd->cb_print_buf, PRINT_BUF_LEN, (PRINT_BUF_LEN-1), "Illegal argument\n"); //CSEC_FIX_2302
        else
            (void)SNPRINTF_S(arp_cmd->cb_print_buf, PRINT_BUF_LEN, (PRINT_BUF_LEN-1), "Successed\n"); //CSEC_FIX_2302
    }
out:
    sys_sem_signal(&arp_cmd->cb_completed);
}


LWIP_STATIC void lwip_arp_usage(const char * cmd)
{
    PRINTK("Usage:"\
        "\n%s"
        "\n%s [-i IF] -s IPADDR HWADDR"\
        "\n%s [-i IF] -d IPADDR\n",
        cmd, cmd, cmd);
}

u32_t lwip_arp(int argc, char **argv)
{
    int i;
    struct arp_option arp_cmd;
    int ret = 0;

    (void)MEMSET_S(&arp_cmd, sizeof(struct arp_option), 0, sizeof(struct arp_option));
    if(!tcpip_init_finish)
    {
        PRINTK("%s: tcpip_init have not been called\n", __FUNCTION__);
        return LOS_NOK;
    }

    arp_cmd.iface[0] = 'd';
    arp_cmd.iface[1] = 'e';
    arp_cmd.iface[2] = '0';
    arp_cmd.option = ARP_OPTION_SHOW;
    arp_cmd.print_buf_len = 0;
    if (sys_sem_new(&arp_cmd.cb_completed, 0) != ERR_OK) {
        PRINTK("%s: sys_sem_new fail\n", __FUNCTION__);
        return 1;
    }

    i  = 0;
    while(argc > 0)
    {
        if(strcmp("-i", argv[i]) == 0 && (argc > 1))
        {
            /* get the network interface's name */
            if (strlen(argv[i]) < IFNAMSIZ)
            {
                STRNCPY_S(arp_cmd.iface, IFNAMSIZ, argv[i+1], (sizeof(arp_cmd.iface) - 1));
            }
            else
            {
                PRINTK("iface name is big \n");
                goto arp_error;
            }
            i += 2;
            argc -= 2;
        }
        else if(strcmp("-d", argv[i]) == 0 && (argc > 1))
        {
            /* arp delete */
            arp_cmd.option = ARP_OPTION_DEL;
            arp_cmd.ipaddr = inet_addr(argv[i+1]);
            if(arp_cmd.ipaddr == IPADDR_NONE)
            {
                PRINTK("ip address is not correct!\n");
                goto arp_error;
            }

            i += 3;
            argc -= 3;
        }
        else if(strcmp("-s", argv[i]) == 0 && (argc > 2))
        {
            /* arp add */
            char *digit = NULL;
            u32_t macaddrlen = strlen(argv[i+2]) + 1;
            char tmpStr[MAX_MACADDR_STRING_LENGTH];
            char *tmpStr1;
            char *saveptr1;
            int j;

            arp_cmd.option = ARP_OPTION_ADD;
            arp_cmd.ipaddr = inet_addr(argv[i+1]);

            /*lint -e561*/
            if(arp_cmd.ipaddr == IPADDR_NONE ) //CSEC_FIX_2302
            {
                PRINTK("ip address is not correct!\n");
                goto arp_error;
            }

            if(macaddrlen != MAX_MACADDR_STRING_LENGTH)
            {
                PRINTK("wrong MAC address length\n");
                return 1;
            }

            STRNCPY_S(tmpStr, macaddrlen, argv[i+2], macaddrlen);
            for(j = 0, tmpStr1 = tmpStr; j < 6; j++, tmpStr1 = NULL)
            {
                digit = strtok_r(tmpStr1, ":", &saveptr1);
                if((digit == NULL) || (strlen(digit) > 2))
                {
                    PRINTK("MAC address is not correct\n");
                    return 1;
                }

                CONVERT_STRING_TO_HEX(digit, arp_cmd.ethaddr[j]);
            }
            /*lint +e561*/

            i += 3;
            argc -= 3;
        }
        else
        {
            goto arp_error;
        }
    }

    tcpip_callback(lwip_arp_internal, &arp_cmd);
    sys_arch_sem_wait(&arp_cmd.cb_completed, 0);
    sys_sem_free(&arp_cmd.cb_completed);
    arp_cmd.cb_print_buf[PRINT_BUF_LEN - 1] = '\0';
    PRINTK("%s", arp_cmd.cb_print_buf);
    return 0;

arp_error:
    lwip_arp_usage("arp");
    sys_sem_free(&arp_cmd.cb_completed);
    return 1;
}

#ifdef LOSCFG_SHELL
SHELLCMD_ENTRY(arp_shellcmd, CMD_TYPE_EX, "arp",      1, (CMD_CBK_FUNC)lwip_arp);
#endif /* LOSCFG_SHELL */

void ifup_internal(void *arg)
{
    struct netif* netif;
    struct if_cmd_data *ifcmd_data;

    ifcmd_data = (struct if_cmd_data *)arg;
    netif = netif_find(ifcmd_data->if_name);
    if (!netif) {
        ifcmd_data->err = ERR_VAL;
    } else {
        netif_set_up(netif);
        ifcmd_data->err = ERR_OK;
    }

    sys_sem_signal(&ifcmd_data->cb_completed);
    return;
}

void ifdown_internal(void *arg)
{
    struct netif *netif;
    struct if_cmd_data *ifcmd_data;

    ifcmd_data = (struct if_cmd_data *)arg;
    netif = netif_find(ifcmd_data->if_name);
    if (!netif) {
        ifcmd_data->err = ERR_VAL;
    } else {
        netif_set_down(netif);\
        ifcmd_data->err = ERR_OK;
    }

    sys_sem_signal(&ifcmd_data->cb_completed);
    return;
}

#if LWIP_DNS
LWIP_STATIC struct hostent *gethostnameinfo (const char *host)
{
    struct hostent hostbuf, *hp;
    size_t hstbuflen;
    char tmphstbuf[1024];
    int res;
    int herr;

    hstbuflen = 1024;

    res = gethostbyname_r (host, &hostbuf, tmphstbuf, hstbuflen,
            &hp, &herr);
    /*  Check for errors.  */
    if (res || hp == NULL)
        return NULL;
    return hp;
}

LWIP_STATIC unsigned int get_hostip(const char *hname)
{
    unsigned int ip=0;

    struct hostent* pent = gethostnameinfo(hname);
    if( NULL==pent || NULL==pent->h_addr )
        return 0;
    MEMCPY_S( &ip, sizeof(ip), pent->h_addr, 4 ); //CSEC_FIX_2302
    return ip;
}
#endif
/*lint +e838*/
#ifdef LOSCFG_SHELL
SHELLCMD_ENTRY(ping_shellcmd, CMD_TYPE_EX, "ping",     XARGS, (CMD_CBK_FUNC)osShellPing);
#endif /* LOSCFG_SHELL */

#if LWIP_SNTP
u32_t osShellNtpdate(int argc, char **argv)
{
    static int ntp_run = 0;
    unsigned int update_delay;
    int server_num;
    struct timeval time;

    if(!tcpip_init_finish)
    {
        PRINTK("%s: tcpip_init have not been called\n", __FUNCTION__);
        return LOS_NOK;
    }

    if(argc < 1)
    {
        goto usage_return;
    }

    server_num = lwip_sntp_start(argc, argv, &time);
    if(server_num >= 0 && server_num < argc)
    PRINTK("time server %s: %s\n", argv[server_num], ctime((time_t *)&time.tv_sec));
    else
    PRINTK("no server suitable for synchronization found\n");

    return server_num;
usage_return:
    PRINTK("usage:\n");
    PRINTK("ntpdate [SERVER_IP1] [SERVER_IP2] ...\n");
  return LOS_NOK;
}
#ifdef LOSCFG_SHELL
SHELLCMD_ENTRY(ntpdate_shellcmd, CMD_TYPE_EX, "ntpdate",  XARGS, (CMD_CBK_FUNC)osShellNtpdate);
#endif
#endif /* LOSCFG_SHELL */

#ifdef LOSCFG_NET_LWIP_SACK_TFTP
static char *TftpError[] =
{
    "TFTP transfer finish\n",
    "Error while creating UDP socket\n",
    "Error while binding to the UDP socket\n",
    "Error returned by lwip_select() system call\n",
    "Error while receiving data from the peer\n",
    "Error while sending data to the peer\n",
    "Requested file is not found\n",
    "This is the error sent by the server when hostname cannot be resolved\n",
    "Input paramters passed to TFTP interfaces are invalid\n",
    "Error detected in TFTP packet or the error received from the TFTP server\n",
    "Error during packet synhronization while sending or unexpected packet is received\n",
    "File size limit crossed, Max block can be 0xFFFF, each block containing 512 bytes\n",
    "File name lenght greater than 256\n",
    "Hostname IP is not valid\n",
    "TFTP server returned file acess error\n",
    "TFTP server returned error signifying that the DISK is full to write\n",
    "TFTP server returned error signifying that the file exist\n",
    "The source file name do not exisits\n",
    "Memory allocaion failed in TFTP client\n",
    "File open failed\n",
    "File read error\n",
    "File create error\n",
    "File write error\n",
    "Max time expired while waiting for file to be recived\n",
    "Error when the received packet is less than 4bytes(error lenght) or greater than 512bytes\n",
    "Returned by TFTP server for protocol user error\n",
    "The destination file path length greater than 256\n",
    "Returned by TFTP server for undefined transfer ID\n",
    "IOCTL fucntion failed at TFTP client while setting the socket to non-block\n",
};

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))
#endif

u32_t osShellTftp(int argc, char **argv)
{
    u32_t  ulRemoteAddr = IPADDR_NONE;
    u16_t  usTftpServPort = 69;
    u8_t   ucTftpGet = 0;
    s8_t   *szLocalFileName = NULL;
    s8_t   *szRemoteFileName = NULL;
    u32_t  ret;

    int i = 0;

    if(!tcpip_init_finish)
    {
        PRINTK("%s: tcpip_init have not been called\n", __FUNCTION__);
        return LOS_NOK;
    }

    while(i < argc)
    {
        if(strcmp(argv[i], "-p") == 0)
        {
            ucTftpGet = 0;
            i++;
            continue;
        }

        if(strcmp(argv[i], "-g") == 0)
        {
            ucTftpGet = 1;
            i++;
            continue;
        }

        if(strcmp(argv[i], "-l") == 0 && ((i + 1) < argc))
        {
            szLocalFileName = (s8_t *)argv[i+1];
            i += 2;
            continue;
        }

        if(strcmp(argv[i], "-r") == 0 && ((i + 1) < argc))
        {
            szRemoteFileName = (s8_t *)argv[i+1];
            i += 2;
            continue;
        }

        if((i + 1) == argc)
        {
            ulRemoteAddr = inet_addr(argv[i]);
            break;
        }

        goto usage;
    }

    if(ulRemoteAddr == IPADDR_NONE || szLocalFileName == NULL || szRemoteFileName == NULL)
    {
        goto usage;
    }

    if(ucTftpGet)
        ret = lwip_tftp_get_file_by_filename(ntohl(ulRemoteAddr), usTftpServPort,
            TRANSFER_MODE_BINARY, szRemoteFileName, szLocalFileName);
    else
        ret = lwip_tftp_put_file_by_filename(ntohl(ulRemoteAddr), usTftpServPort,
            TRANSFER_MODE_BINARY, szLocalFileName, szRemoteFileName);

    LWIP_ASSERT("TFTP UNKNOW ERROR!", ret < ARRAY_SIZE(TftpError));
    PRINTK("%s", TftpError[ret]);
    if(ret)
        return LOS_NOK;
    else
        return LOS_OK;
usage:
    PRINTK("usage:\nTransfer a file from/to tftp server\n");
    PRINTK("tftp <-g/-p> -l FullPathLocalFile -r RemoteFile Host\n");
    return LOS_NOK;
}
#ifdef LOSCFG_SHELL
SHELLCMD_ENTRY(tftp_shellcmd, CMD_TYPE_EX, "tftp",     XARGS, (CMD_CBK_FUNC)osShellTftp);
#endif /* LOSCFG_SHELL */
#endif /* LOSCFG_NET_LWIP_SACK_TFTP */


#if LWIP_DNS
u32_t osShellDns(int argc, char **argv)
{
    ip_addr_t dns;
    err_t err;
    int i;

    if (0 == tcpip_init_finish)
    {
        PRINTK("%s: tcpip_init have not been called\n", __FUNCTION__);
        return LOS_NOK;
    }

    if(argc == 1 && (strcmp(argv[0], "-a") == 0))
    {
        for(i = 0; i < DNS_MAX_SERVERS; i++)
        {
          err = lwip_dns_getserver((u8_t)i, &dns);
          if (err == ERR_OK) {
              PRINTK("dns%d: %s\n", i+1, ipaddr_ntoa(&dns));
          } else {
              PRINTK("dns: failed\n");
              return LOS_NOK;
          }
        }
        return LOS_OK;
    }
    else if(argc == 2)
    {
        if(sscanf(argv[0], "%d", &i) != 1 || (i <= 0) || (i > DNS_MAX_SERVERS)) //CSEC_FIX_2302
            goto usage;

        dns.addr = ipaddr_addr(argv[1]);
        if(dns.addr == IPADDR_NONE)
        {
            PRINTK("ip address<%s> is wrong\n", argv[i]);
            return LOS_NOK;
        }

        err = lwip_dns_setserver((u8_t)(i - 1), &dns);
        if (err != ERR_OK) {
            PRINTK("dns : failed\n");
            return LOS_NOK;
        }
        return LOS_OK;
    }

usage:
    PRINTK("usage:\n");
    PRINTK("\tdns <1-%d> <IP>\n", DNS_MAX_SERVERS);
    PRINTK("\tdns -a\n");
    return LOS_NOK;
}
#ifdef LOSCFG_SHELL
SHELLCMD_ENTRY(dns_shellcmd, CMD_TYPE_EX, "dns",      XARGS, (CMD_CBK_FUNC)osShellDns);
#endif /* LOSCFG_SHELL */
#endif /* LWIP_DNS */

int netstat_get_udp_sendQLen(struct udp_pcb *udppcb, struct pbuf *udpbuf)
{
    int sendLen = -1;
    s16_t offset = 0, len;
    struct ip_hdr *iphdr;
    struct udp_hdr *udphdr;

    LWIP_ERROR("netstat_get_udp_sendQLen: NULL pcb received\n", (udppcb != NULL), return -1;);
    LWIP_ERROR("netstat_get_udp_sendQLen: NULL pbuf received\n", (udpbuf != NULL), return -1;);

    len = PBUF_LINK_HLEN;
    if(pbuf_header(udpbuf, -len)){
        goto FUNC_OUT;
    }

    offset += len;

    iphdr = (struct ip_hdr *)udpbuf->payload;

    if(!(ip_addr_cmp(&iphdr->dest, &udppcb->remote_ip)
            && (ip_addr_cmp(&iphdr->src, &udppcb->local_ip)))) {
        goto FUNC_OUT;
    }
#if LWIP_UDPLITE
    if((IPH_PROTO(iphdr) != IP_PROTO_UDP) && (IPH_PROTO(iphdr) != IP_PROTO_UDPLITE)) {
#else
    if(IPH_PROTO(iphdr) != IP_PROTO_UDP) {
#endif
        goto FUNC_OUT;
    }

    if((ntohs(IPH_OFFSET(iphdr)) & IP_OFFMASK) != 0)
    {
        goto FUNC_OUT;
    }

    len = IPH_HL(iphdr) * 4;
    if(pbuf_header(udpbuf, -len)) {
        goto FUNC_OUT;
    }

    offset += len;

    udphdr = (struct udp_hdr *)udpbuf->payload;
    if((ntohs(udphdr->dest) == udppcb->remote_port) && (ntohs(udphdr->src) == udppcb->local_port)) {
        sendLen = ntohs(udphdr->len) - UDP_HLEN;
    }

FUNC_OUT:
    (void)pbuf_header(udpbuf, offset);
    return sendLen;
}

int netstat_tcp_recvq(struct tcp_pcb *tpcb)
{
    int retVal = 0;
#if LWIP_SO_RCVBUF
    struct netconn *conn;
#endif

    LWIP_ERROR("netstat_tcp_recvq: Received NULL pcb\n", (tpcb != NULL), return 0;);

#if LWIP_SO_RCVBUF
    conn = (struct netconn *)tpcb->callback_arg;
    if(conn != NULL) {
        SYS_ARCH_GET(conn->recv_avail, retVal);
    }
#endif

   return retVal;
}

int netstat_tcp_sendq(struct tcp_pcb *tpcb)
{
    int retVal = 0;
    struct tcp_seg *useg;

    LWIP_ERROR("netstat_tcp_sendq: Received NULL pcb\n", (tpcb != NULL), return 0;);

    for (useg = tpcb->unacked; useg != NULL; useg = useg->next){
        retVal = retVal + useg->len;
    }

    return retVal;
}

int netstat_udp_recvq(struct udp_pcb *upcb)
{
    int retVal = 0;
#if LWIP_SO_RCVBUF
    struct netconn *conn;
#endif

    LWIP_ERROR("netstat_udp_recvq: Received NULL pcb\n", (upcb != NULL), return 0;);
#if LWIP_SO_RCVBUF
   extern void recv_udp(void *arg, struct udp_pcb *pcb, struct pbuf *p, ip_addr_t *addr, u16_t port);
   if(upcb->recv == (udp_recv_fn)recv_udp){
        conn = (struct netconn *)upcb->recv_arg;
        if(conn != NULL) {
            SYS_ARCH_GET(conn->recv_avail, retVal);
        }
    }
#endif
    return retVal;
}

int netstat_udp_sendq(struct udp_pcb *upcb)
{
    int retLen = 0, ret;
    int arpidx = -1;
    int i;
#if ARP_QUEUEING
    struct etharp_q_entry  *arpq;
#endif

    LWIP_ERROR("netstat_udp_sendq: Received NULL pcb\n", (upcb != NULL), return 0;);

    for (i = 0; i < ARP_TABLE_SIZE; ++i) {
        if (arp_table[i].state != ETHARP_STATE_EMPTY) {
            if (ip_addr_cmp(&upcb->remote_ip, &arp_table[i].ipaddr)) {
                arpidx = i;
                break;
            }
        }
    }

    //arpidx = etharp_find_entry(&upcb->remote_ip, ETHARP_FLAG_FIND_ONLY);
    if(arpidx >= 0) {
#if ARP_QUEUEING
        for(arpq = arp_table[arpidx].q; arpq != NULL; arpq = arpq->next) {
            ret = netstat_get_udp_sendQLen(upcb, arpq->p);
            if(ret >= 0) {
                retLen += ret;
            }
        }
#else
        ret = netstat_get_udp_sendQLen(arp_table[arpidx].q);
        if(ret >= 0) {
            retLen += ret;
        }
#endif
    }

    return retLen;
}

/*lint -e732*/
void netstat_internal(void *ctx)
{
    struct tcp_pcb *tpcb;
    struct udp_pcb *upcb;
    s8_t *entry_buf;
    int entry_buf_len;
    int entry_buf_offset;
    struct netstat_data *ndata = (struct netstat_data *)ctx;
    int iRet;
    s8_t local_ip_port[25] = {0};
    s8_t remote_ip_port[25] = {0};
    int recvQlen = 0;
    int sendQlen = 0;

    entry_buf = ndata->netstat_out_buf;
    entry_buf_len = ndata->netstat_out_buf_len;
    entry_buf_offset = 0;

    if (entry_buf == NULL) {
        goto out;
    }

    iRet = SNPRINTF_S(entry_buf, entry_buf_len, (entry_buf_len-1), "%-8s%-12s%-12s%-24s%-24s%-16s\n",
      "Proto", "Recv-Q", "Send-Q", "Local Address", "Foreign Address", "State");
    if ((iRet <= 0) || (iRet >= entry_buf_len)) {
        goto out;
    }
    entry_buf_len -= iRet;
    entry_buf_offset += iRet;

#if LWIP_UDP
    for (upcb = udp_pcbs; upcb != NULL; upcb = upcb->next) {
        (void)MEMSET_S(local_ip_port, sizeof(local_ip_port), 0, sizeof(local_ip_port));
        (void)MEMSET_S(remote_ip_port, sizeof(remote_ip_port), 0, sizeof(remote_ip_port));
        (void)SNPRINTF_S(local_ip_port, sizeof(local_ip_port), (sizeof(local_ip_port) - 1),"%s:%d", ipaddr_ntoa(&upcb->local_ip), upcb->local_port);
        (void)SNPRINTF_S(remote_ip_port, sizeof(remote_ip_port), (sizeof(remote_ip_port) - 1), "%s:%d", ipaddr_ntoa(&upcb->remote_ip), upcb->remote_port);

        recvQlen = netstat_udp_recvq(upcb);
        sendQlen = netstat_udp_sendq(upcb);

        iRet = SNPRINTF_S((entry_buf + entry_buf_offset), entry_buf_len, (entry_buf_len - 1), "%-8s%-12d%-12d%-24s%-24s%-16s\n",
                  "udp", recvQlen, sendQlen, local_ip_port, remote_ip_port, " ");
        if ((iRet <=0) || (iRet >= entry_buf_len)) {
            goto out;
        }
        entry_buf_len -= iRet;
        entry_buf_offset += iRet;
    }
#endif

#if LWIP_TCP
    for (tpcb = tcp_active_pcbs; tpcb != NULL; tpcb = tpcb->next) {
        (void)MEMSET_S(local_ip_port, sizeof(local_ip_port), 0, sizeof(local_ip_port));
        (void)MEMSET_S(remote_ip_port, sizeof(remote_ip_port), 0, sizeof(remote_ip_port));
        (void)SNPRINTF_S(local_ip_port, sizeof(local_ip_port), (sizeof(local_ip_port) - 1), "%s:%d", ipaddr_ntoa(&tpcb->local_ip), tpcb->local_port);
        (void)SNPRINTF_S(remote_ip_port, sizeof(remote_ip_port), (sizeof(remote_ip_port) - 1), "%s:%d", ipaddr_ntoa(&tpcb->remote_ip), tpcb->remote_port);

        recvQlen = netstat_tcp_recvq(tpcb);
        sendQlen = netstat_tcp_sendq(tpcb);
        iRet = SNPRINTF_S((entry_buf + entry_buf_offset), entry_buf_len, (entry_buf_len-1), "%-8s%-12d%-12d%-24s%-24s%-16s\n",
                  "tcp", recvQlen, sendQlen, local_ip_port, remote_ip_port, tcp_state_str[tpcb->state]);
        if ((iRet <=0) || (iRet >= entry_buf_len)) {
            goto out;
        }
        entry_buf_len -= iRet;
        entry_buf_offset += iRet;
    }

    /* For bound PCBs */
    sendQlen = 0;
    recvQlen = 0;

    for (tpcb = tcp_bound_pcbs; tpcb != NULL; tpcb = tpcb->next) {
        (void)MEMSET_S(local_ip_port, sizeof(local_ip_port), 0, sizeof(local_ip_port));
        (void)MEMSET_S(remote_ip_port, sizeof(remote_ip_port), 0, sizeof(remote_ip_port));
        (void)SNPRINTF_S(local_ip_port, sizeof(local_ip_port), (sizeof(local_ip_port) - 1), "%s:%d", ipaddr_ntoa(&tpcb->local_ip), tpcb->local_port);
        (void)SNPRINTF_S(remote_ip_port, sizeof(remote_ip_port), (sizeof(remote_ip_port) - 1), "%s:%d", ipaddr_ntoa(&tpcb->remote_ip), tpcb->remote_port);

        iRet = SNPRINTF_S((entry_buf + entry_buf_offset), entry_buf_len, (entry_buf_len-1), "%-8s%-12d%-12d%-24s%-24s%-16s\n",
                  "tcp", recvQlen, sendQlen, local_ip_port, remote_ip_port, tcp_state_str[tpcb->state]);
        if ((iRet <=0) || (iRet >= entry_buf_len)) {
            goto out;
        }
        entry_buf_len -= iRet;
        entry_buf_offset += iRet;
    }

    for (tpcb = tcp_tw_pcbs; tpcb != NULL; tpcb = tpcb->next) {
        (void)MEMSET_S(local_ip_port, sizeof(local_ip_port), 0, sizeof(local_ip_port));
        (void)MEMSET_S(remote_ip_port, sizeof(remote_ip_port), 0, sizeof(remote_ip_port));
        (void)SNPRINTF_S(local_ip_port, sizeof(local_ip_port), (sizeof(local_ip_port) - 1),"%s:%d", ipaddr_ntoa(&tpcb->local_ip), tpcb->local_port);
        (void)SNPRINTF_S(remote_ip_port, sizeof(remote_ip_port), (sizeof(remote_ip_port) - 1), "%s:%d", ipaddr_ntoa(&tpcb->remote_ip), tpcb->remote_port);

        recvQlen = netstat_tcp_recvq(tpcb);
        sendQlen = netstat_tcp_sendq(tpcb);

        iRet = SNPRINTF_S((entry_buf + entry_buf_offset), entry_buf_len, (entry_buf_len-1), "%-8s%-12d%-12d%-24s%-24s%-16s\n",
                  "tcp", recvQlen, sendQlen, local_ip_port, remote_ip_port, tcp_state_str[tpcb->state]);
        if ((iRet <=0) || (iRet >= entry_buf_len)) {
            goto out;
        }
        entry_buf_len -= iRet;
        entry_buf_offset += iRet;
    }

    /* For listen PCBs */
    recvQlen = 0;
    sendQlen = 0;

    for (tpcb = tcp_listen_pcbs.pcbs; tpcb != NULL; tpcb = tpcb->next) {
        (void)MEMSET_S(local_ip_port, sizeof(local_ip_port), 0, sizeof(local_ip_port));
        (void)MEMSET_S(remote_ip_port, sizeof(remote_ip_port), 0, sizeof(remote_ip_port));
        (void)SNPRINTF_S(local_ip_port, sizeof(local_ip_port), (sizeof(local_ip_port) - 1), "%s:%d", ipaddr_ntoa(&tpcb->local_ip), tpcb->local_port);
        (void)SNPRINTF_S(remote_ip_port, sizeof(remote_ip_port), (sizeof(remote_ip_port) - 1), "%s:%d", ipaddr_ntoa(&tpcb->remote_ip), tpcb->remote_port);

        iRet = SNPRINTF_S((entry_buf + entry_buf_offset), entry_buf_len, (entry_buf_len-1), "%-8s%-12d%-12d%-24s%-24s%-16s\n",
                  "tcp", recvQlen, sendQlen, local_ip_port, remote_ip_port, tcp_state_str[tpcb->state]);
        if ((iRet <=0) || (iRet >= entry_buf_len)) {
            goto out;
        }
        entry_buf_len -= iRet;
        entry_buf_offset += iRet;
    }
#endif

out:
    ndata->netstat_out_buf_updated_len = entry_buf_offset;
    sys_sem_signal(&ndata->cb_completed);
    return;
}

u32_t osShellNetstat(int argc, char **argv)
{
    struct netstat_data ndata = {0};
    err_t err;

    if (argc > 0)
    {
        PRINTK("\nUsage: netstat\n");
        return LOS_NOK;
    }

    if (0 == tcpip_init_finish)
    {
        PRINTK("%s: tcpip_init have not been called\n", __FUNCTION__);
        return LOS_NOK;
    }

    (void)MEMSET_S(g_netstat_buf, sizeof(g_netstat_buf), 0, sizeof(g_netstat_buf)); //CSEC_FIX_2302
    ndata.netstat_out_buf = g_netstat_buf;
    ndata.netstat_out_buf_len = MAX_NETSTAT_ENTRY;
    ndata.netstat_out_buf_updated_len = 0;

    if (sys_sem_new(&ndata.cb_completed, 0) != ERR_OK) {
        goto err_hand;
    }

    err = tcpip_callback(netstat_internal, &ndata);
    if (err != ERR_OK) {
        sys_sem_free(&ndata.cb_completed);
        goto err_hand;
    }

    sys_arch_sem_wait(&ndata.cb_completed, 0);
    sys_sem_free(&ndata.cb_completed);
    if ((ndata.netstat_out_buf_updated_len > 0) && (ndata.netstat_out_buf_updated_len < MAX_NETSTAT_ENTRY)) {
        PRINTK("%s\n", g_netstat_buf);
        return LOS_OK;
    }

err_hand:
    PRINTK("Error in netstat\n");
    return LOS_NOK;
}
#ifdef LOSCFG_SHELL
SHELLCMD_ENTRY(netstat_shellcmd, CMD_TYPE_EX, "netstat",  XARGS, (CMD_CBK_FUNC)osShellNetstat);
#endif /* LOSCFG_SHELL */

/*lint +e732*/
#endif //LWIP_ENABLE_LOS_SHELL_CMD

/*lint -e838*/
void fill_icmp_data (char* icmp_data, int datalen, int ip_index)
{
    char* datapart;
    int index = 0,count = 0;
    datapart =  (icmp_data + sizeof(struct icmp_echo_hdr));
    while(count < datalen)
    {
      *datapart = index + 'a';
      index = ( index + 1 ) % 23;
      count++;
      datapart++;
    }
}
u32_t osShellPing(struct ping_ctx* ping_content)//(int argc, char **argv)
{
    int sfd;
    struct sockaddr_in to;
    struct pbuf *iecho = NULL;
    struct icmp_echo_hdr* imcp_hdr;
    struct pbuf *pbuf_resp = NULL;
    struct icmp_echo_hdr *iecho_resp_hdr;
    struct ip_hdr *iphdr_resp;
    struct ip_addr dst_ipaddr;
    s16_t ip_hlen;
    fd_set fdReadSet;
    struct timeval stTimeVal;
    u32_t start_time, end_time;
    int ret;
    int return_val = ERR_VAL;
    u32_t i;
    s32_t rtt;
    u16_t icmp_seq = 0x2255;
    char* dst_ip = ping_content->dst_ip;
    s32_t pingcount = ping_content->count;
    s32_t datalen = ping_content->len;

    LWIP_ERROR("ping: tcpip_init have not been called\n", tcpip_init_finish, return LOS_NOK);
    LWIP_ERROR("ping len out of limit.\n", (datalen >= 0 && datalen <= 64992), return  ERR_VAL);
    LWIP_ERROR("ping count is negtive.\n", (pingcount > 0), return  ERR_VAL);
	LWIP_ERROR("ping ip is NULL.\n", (dst_ip != NULL), return  ERR_VAL);

  /* initialize dst IP address */
#if LWIP_DNS
    dst_ipaddr.addr = get_hostip(dst_ip);
#else /* LWIP_DNS */
    dst_ipaddr.addr = inet_addr(dst_ip);
#endif /* LWIP_DNS */

    to.sin_family = AF_INET;
    to.sin_addr.s_addr = dst_ipaddr.addr;
    to.sin_port = 0;

    LWIP_ERROR("ping : invalid ip address.\n", !(to.sin_addr.s_addr == IPADDR_NONE || to.sin_addr.s_addr == IPADDR_ANY), return ERR_VAL);

    sfd = lwip_socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
    LWIP_ERROR("ping : failed, socket creation failed\n", !(sfd == -1), return ERR_VAL);

    pbuf_resp = pbuf_alloc(PBUF_RAW, IP_HLEN + sizeof(struct icmp_echo_hdr) + datalen, PBUF_RAM);
    if (pbuf_resp == NULL)
    {
        LWIP_DEBUGF(SYS_ARCH_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_SERIOUS,("ping : memory allocation failed\n"));
        goto FAILURE;
    }
    iecho = pbuf_alloc(PBUF_RAW, sizeof(struct icmp_echo_hdr) + datalen, PBUF_RAM);
    if (iecho == NULL)
    {
        LWIP_DEBUGF(SYS_ARCH_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_SERIOUS,("ping : iecho memory allocation failed\n"));
        goto FAILURE;
    }
    for (i = 0; i < pingcount; i++)
    {
        icmp_seq++;
        imcp_hdr = (struct icmp_echo_hdr*)iecho->payload;
        (void)MEMSET_S(imcp_hdr, sizeof(struct icmp_echo_hdr) + datalen, 0, sizeof(struct icmp_echo_hdr) + datalen);
        imcp_hdr->type = (u8_t)ICMP_ECHO;
        imcp_hdr->code = 0;
        imcp_hdr->id = (u16_t)LOS_CurTaskIDGet();
        imcp_hdr->seqno = icmp_seq;
        fill_icmp_data (iecho->payload, datalen, 4);
        imcp_hdr->chksum = inet_chksum(imcp_hdr, sizeof(struct icmp_echo_hdr) + datalen);
        ret = lwip_sendto(sfd, imcp_hdr, sizeof(struct icmp_echo_hdr) + datalen, 0, (struct sockaddr *)&to, (socklen_t)sizeof(to));

        if (ret == -1)
        {
            LWIP_DEBUGF(SYS_ARCH_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_SERIOUS,("ping : sending ICMP echo msg failed\n"));
            goto FAILURE;
        }

        /* capture the start tick to calculate round trip time */
        start_time = sys_now();
        /* Wait in select for ICMP response msg */
        FD_ZERO(&fdReadSet);
        FD_SET(sfd, &fdReadSet);
        stTimeVal.tv_sec = LWIP_SHELL_CMD_PING_TIMEOUT/1000;
        stTimeVal.tv_usec = 0;

DO_SELECT:
        ret = lwip_select(sfd+1, &fdReadSet, 0, 0, &stTimeVal);
        if (ret < 0)
        {
            LWIP_DEBUGF(SYS_ARCH_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_SERIOUS,("ping : select failure\n"));
            return_val = ERR_VAL;
        }
        else if (ret == 0)
        {
            LWIP_DEBUGF(SYS_ARCH_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_SERIOUS,("Request timed out.\n"));
            return_val = ERR_TIMEOUT;
            continue;
        }

        /* capture the end tick to calculate round trip time */
        end_time = sys_now();
        rtt = (s32_t)(end_time -start_time);
        ret = lwip_recv(sfd, pbuf_resp->payload, pbuf_resp->len, 0);
        if (ret == -1)
        {
            LWIP_DEBUGF(SYS_ARCH_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_SERIOUS,("ping : receiving ICMP echo response msg failed\n"));
            goto FAILURE;
        }
        if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
        {
            LWIP_DEBUGF(SYS_ARCH_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_SERIOUS,("ping : receiving ICMP echo response msg interrupted\n"));
            continue;
        }

        /* Accessing ip header and icmp header */
        iphdr_resp = pbuf_resp->payload;
        ip_hlen = IPH_HL(iphdr_resp) * 4;
        if (pbuf_header(pbuf_resp, -ip_hlen))
        {
            /* this failure will never happen, but failure handle is written just to be in safe side */
            LWIP_DEBUGF(SYS_ARCH_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_SERIOUS,("ping : memory management failure\n"));
            goto FAILURE;
        }
        iecho_resp_hdr = (struct icmp_echo_hdr *)pbuf_resp->payload;

        /* Reverting back pbuf to its original state */
        if (pbuf_header(pbuf_resp, ip_hlen))
        {
            /* this failure will never happen, but failure handle is written just to be in safe side */
            LWIP_DEBUGF(SYS_ARCH_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_SERIOUS,("ping : memory management failure\n"));
            goto FAILURE;
        }

        if (iphdr_resp->src.addr == to.sin_addr.s_addr)
        {
            if (iecho_resp_hdr->type == (u8_t)ICMP_ER)//ping_content->icmp_type
            {
                ping_content->icmp_type = ICMPH_TYPE(iecho_resp_hdr);
                ping_content->icmp_code = ICMPH_CODE(iecho_resp_hdr);
                ping_content->rtt = rtt;
                ping_content->ttl = iphdr_resp->_ttl;
                return_val = ERR_OK;
            }
            /* If ping self, stack will receive a ICMP_ECHO request message flowing a ICMP_ER reply message,
                and we need reply only, do select again */
            else if(iecho_resp_hdr->type == (u8_t)ICMP_ECHO)
                goto DO_SELECT;
        }
    }

    lwip_close(sfd);
    pbuf_free(pbuf_resp);
    pbuf_free(iecho);
    return return_val;

FAILURE:
    lwip_close(sfd);
    if(iecho != NULL)
    {
        pbuf_free(iecho);
    }
    if (pbuf_resp != NULL)
    {
        pbuf_free(pbuf_resp);
    }

    return ERR_VAL;
}
#endif
