/*----------------------------------------------------------------------------
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/
#include "los_event.h"
#include "los_printf.h"
#include "hisoc/uart.h"
#include "hisoc/sys_ctrl.h"
#ifdef LOSCFG_KERNEL_RUNSTOP
#include "los_runstop.h"
#endif
#ifdef LOSCFG_KERNEL_SCATTER
#include "los_scatter.h"
#endif
#ifdef LOSCFG_LIB_LIBC
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "sys/types.h"
#include "sys/time.h"
#include "unistd.h"
#include "fcntl.h"
#include "sys/statfs.h"
#include "localtime.h"
#include "securec.h"
#include "dirent.h"
#endif
#ifdef LOSCFG_FS_VFS
#include "fs/fs.h"
#endif
#ifdef LOSCFG_NET_LWIP_SACK
#include "lwip/tcpip.h"
#include "lwip/netif.h"
#ifdef LOSCFG_DRIVERS_HIETH_SF
#include "eth_drv.h"
#endif
#endif
#ifdef LOSCFG_DRIVERS_UART
#include "console.h"
#include "uart_dev.h"
#endif
#ifdef LOSCFG_SHELL
#include "shell.h"
#include "show.h"
#endif

#define REG_BASE_SCTL 0x101e0000
#define REG_SC_WDG_RST 0x3C

#define sysctrl_base_addr REG_BASE_SCTL
#define SC_CTRL   REG_SC_CTRL
#define SC_WDG_RST_CTRL   REG_SC_WDG_RST
void Watchdog_Disable(void)
{
    UINT32 dirvalue = 0;

    // clk
    READ_UINT32(dirvalue, sysctrl_base_addr + SC_CTRL);
    dirvalue &= (~(UINT32)(0x1 << 23));
    WRITE_UINT32(dirvalue, sysctrl_base_addr + SC_CTRL);

    // reset
    READ_UINT32(dirvalue, sysctrl_base_addr + SC_WDG_RST_CTRL);
    dirvalue &= (~(UINT32)(0x1 << 3));
    WRITE_UINT32(dirvalue, sysctrl_base_addr + SC_WDG_RST_CTRL);
}

#include "lwip/sockets.h"
#include "lwip/api_shell.h"
#include "lwip/tcpip.h"

#ifdef LOSCFG_NET_LWIP_SACK
#include "lwip/tcpip.h"
#ifdef LOSCFG_DRIVERS_HIETH_SF
#include "eth_drv.h"
#endif
#endif

#ifdef LOSCFG_NET_LWIP_SACK
int secure_func_register(void)
{
    int ret;
    STlwIPSecFuncSsp stlwIPSspCbk = {0};
    stlwIPSspCbk.pfMemset_s = memset_s;
    stlwIPSspCbk.pfMemcpy_s = memcpy_s;
    stlwIPSspCbk.pfStrNCpy_s = strncpy_s;
    stlwIPSspCbk.pfStrNCat_s = strncat_s;
    stlwIPSspCbk.pfStrCat_s = strcat_s;
    stlwIPSspCbk.pfMemMove_s = memmove_s;
    stlwIPSspCbk.pfSnprintf_s = snprintf_s;
    stlwIPSspCbk.pfRand = rand;
    ret = lwIPRegSecSspCbk(&stlwIPSspCbk);
    if (ret != 0)
    {
        PRINT_ERR("\n***lwIPRegSecSspCbk Failed***\n");
        return -1;
    }

    PRINTK("\nCalling lwIPRegSecSspCbk\n");
    return ret;
}
#endif


#define PORT 5685
#define MAX_SIZE 1024
void dump_pkt(char* data, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        printf("%c", *(data + i));
    }
    printf("\r\n");
}
void tcp_respon(int sockfd)
{
    int n, i;
    char msg[MAX_SIZE] = {0};
    while (1)
    {
        printf("waiting for recv\n");
        memset_s(msg, MAX_SIZE, 0, MAX_SIZE);
        n = recv(sockfd, msg, MAX_SIZE, 0);
        if (n < 0)
        {
            printf("no recv msg\n");
            continue;
        }
        else if (n == 0)
        {
            (void)close(sockfd);
            printf("client disconnect\n");
            break;
        }
        msg[n] = '\0';
        dump_pkt(msg, n);

        printf("I have recevied %s,length %d\n", msg, n);

        n = strlen(msg);
        for (i = 0; i < n; ++i)
        {
            if (msg[i] >= 'a' && msg[i] <= 'z')
            {
                msg[i] = msg[i] + 'A' - 'a';
            }
            else if (msg[i] >= 'A' && msg[i] <= 'Z')
            {
                msg[i] = msg[i] + 'a' - 'A';
            }
        }
        //sleep(60);
        if (send(sockfd, msg, n, 0) < 0)
        {
            printf("send msg failed!");
            continue;
        }

        printf("send msg:%s\n", msg);
    }
}

int tcp_server_test(void)
{
    int sockfd;
    int ret = -1;
    struct sockaddr_in seraddr;
    struct sockaddr_in cliaddr;
    u32_t cliaddr_size = (u32_t)sizeof(cliaddr);
    int reuse;
    printf("tcp_server_test\r\n");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
                   (const char*) &reuse, sizeof( reuse ) ) != 0 )
    {
        (void)close(sockfd);
        printf("set SO_REUSEADDR failed\n");
        return -1;
    }

    memset_s(&seraddr, sizeof(seraddr), 0, sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    seraddr.sin_port = htons(PORT);

    ret = bind(sockfd, (struct sockaddr*)&seraddr, sizeof(seraddr));
    if (ret < 0)
    {
        printf("bind ip and port failed");
        (void)close(sockfd);
        return -1;
    }

    ret = listen(sockfd, 5);
    if (ret < 0)
    {
        close(sockfd);
        printf("Bad listen\n");
        return -1;
    }

    memset_s(&cliaddr, sizeof(struct sockaddr_in), 0, sizeof(struct sockaddr_in));
    printf("waiting for accept\n");
    ret = (int)accept(sockfd, (struct sockaddr*)&cliaddr, &cliaddr_size);
    printf("accept ret=%d\n", ret);
    if (ret < 0)
    {
        (void)close(sockfd);
        printf("Bad accept\n");
        return -1;
    }
    //	close(sockfd);
    //	return 0;
    tcp_respon(ret);

    return 0;
}


__attribute__((weak)) void app_init(void)
{
    VOID* MemPool = OS_SYS_MEM_ADDR;
#ifdef LOSCFG_SHELL
    extern unsigned int osShellInit(const char*);
    osShellInit(TTY_DEVICE);
#endif

    secure_func_register();
    tcpip_init(NULL, NULL);
    //hisi_eth_init();
    //tcp_server_test();
    extern void  SAL_ApplicationInitialize(void* mem_pool, void* uncached_mem_pool, unsigned int ulSize);
    SAL_ApplicationInitialize(MemPool, NULL, 0);

    return;
}
