
#ifndef __SAL_OS_NET_API_H__
#define __SAL_OS_NET_API_H__

#include <hi_types.h>
#include <hi_mdm_types.h>


#define HI_IP4_ADDR1(ipaddr) (((HI_U8*)(&ipaddr))[0])
#define HI_IP4_ADDR2(ipaddr) (((HI_U8*)(&ipaddr))[1])
#define HI_IP4_ADDR3(ipaddr) (((HI_U8*)(&ipaddr))[2])
#define HI_IP4_ADDR4(ipaddr) (((HI_U8*)(&ipaddr))[3])


#define HI_IP4_ADDR_U32_TO_ARRAY(ulIp,arrayIp) do{}while(0)

#define HI_IP4_ADDR_STR_TO_ARRAY(strIp,arrayIp,pRet) do{if(strIp){HI_U32 ulIp =ipaddr_addr(strIp); (HI_VOID)memcpy_s(arrayIp, 4, &ulIp, 4);*pRet=HI_ERR_SUCCESS;}else{ret=HI_ERR_FAILURE;}}while(0)


#define HI_IP4_ADDR_ARRAY_TO_U32(arrayIp,ulIp) do{}while(0)
#define HI_IP4_ADDR_STR_TO_U32(strIp,ulIp,pRet) do{}while(0)

#define HI_IP4_ADDR_U32_TO_STR(ulIp,strIp) do{}while(0)
#define HI_IP4_ADDR_ARRAY_TO_STR(arrayIp,strIp) do{}while(0)



/*
链路层output钩子
pBuf：输出数据buf指针数组
ulen：输出数据buf长度数组
pBufCout：输出数据由几个buf组成
totalSize：等于ulen[0]+ulen[1]+...ulen[pBufCout-1]
*/
typedef HI_U32  (*hi_net_link_out)(HI_PVOID pBuf[],HI_U32 ulen[],HI_U32 pBufCout,HI_U32 totalSize);
/*ip层output钩子*/
typedef HI_U32  (*hi_net_ip_out)(HI_PVOID pBuf[],HI_U32 ulen[],HI_U32 pBufCout,HI_U32 totalSize,HI_U32 next_ip);
/*配置mac地址钩子*/
typedef HI_U32 (*hi_net_set_mac)(HI_U8  *addr, HI_U8 len);
/*获取路由信息钩子*/
typedef HI_U32 (*print_rt_fn)(char *ucIfName,HI_U8 ip_addr[4],HI_U8 net_mask[4],HI_U8 g_w[4],HI_U32 ulUsrData);

#define HI_NET_MAX_HWADDR_LEN 6//最长硬件地址
#define HI_NET_MAX_NAME_LEN 8//最长netif名称

typedef struct
{
    HI_U8 ip_addr[4];//IP
    HI_U8 net_mask[4];//netmask

    HI_U16 default_mtu;//mtu
    HI_U8 hwaddr[HI_NET_MAX_HWADDR_LEN];//mac

    hi_net_ip_out pfnIpOut;//IP层输出回调接口
    hi_net_set_mac pfnSetMac; //设置mac回调接口

    HI_U8 hwaddr_len;//硬件地址长度
    HI_U8 pad1[3];
}HI_IP_NET_IF_INIT_CFG_S;//非ETH网卡初始化参数


typedef struct
{
    HI_U8 ip_addr[4];
    HI_U8 net_mask[4];
    HI_U8 gw[4];

    HI_U16 default_mtu;
    HI_U8 hwaddr[HI_NET_MAX_HWADDR_LEN];

    hi_net_link_out pfnLinkOut;
    hi_net_set_mac pfnSetMac;

    HI_U8 hwaddr_len;
    HI_U8 pad1[3];
}HI_ETH_NET_IF_INIT_CFG_S;//ETH网卡初始化参数

typedef struct
{
    HI_U8 ip_addr[4];
    HI_U8 net_mask[4];
    HI_U8 gw[4];

    HI_U16 default_mtu;
    HI_U8 hwaddr[HI_NET_MAX_HWADDR_LEN];

    hi_net_link_out pfnLinkOut;
    hi_net_set_mac pfnSetMac;
    hi_net_ip_out pfnIpOut;

    HI_U8 hwaddr_len;
    HI_U8 bEth;
    HI_U8 pad1[2];

}HI_NET_IF_INIT_CFG_S;





//*****************************************************************************
// 函数名称: SAL_CreateIpNetIf
// 功能描述: 创建非ETH网卡实例
//
// 参数说明:
//      name:网卡名称
//      pCfg:初始化参数
// 返 回 值:
//      HI_NET_INVALID_HANDLE：非法句柄
//      其它：句柄创建成功
// 调用要求:
//
//*****************************************************************************
HI_U32 SAL_CreateIpNetIf(HI_CHAR name[HI_NET_MAX_NAME_LEN],HI_IP_NET_IF_INIT_CFG_S *pCfg);
//*****************************************************************************
// 函数名称: SAL_CreateEthNetIf
// 功能描述: 创建ETH网卡实例
//
// 参数说明:
//      name:网卡名称
//      pCfg:初始化参数
// 返 回 值:
//      HI_NET_INVALID_HANDLE：非法句柄
//      其它：句柄创建成功
// 调用要求:
//
//*****************************************************************************
HI_U32 SAL_CreateEthNetIf(HI_CHAR name[HI_NET_MAX_NAME_LEN],HI_ETH_NET_IF_INIT_CFG_S *pCfg);

//*****************************************************************************
// 函数名称: SAL_SetLinkDown
// 功能描述: 物理连接断开
//
// 参数说明:
//      handle:设备句柄
// 返 回 值:
//      HI_ERR_SUCCESS:成功
//      其它 失败
// 调用要求:
//
//*****************************************************************************
HI_U32 SAL_SetLinkDown(HI_U32 handle);

//*****************************************************************************
// 函数名称: SAL_SetLinkUp
// 功能描述: 物理连接连山
//
// 参数说明:
//      handle:设备句柄
// 返 回 值:
//      HI_ERR_SUCCESS:成功
//      其它 失败
// 调用要求:
//
//*****************************************************************************
HI_U32 SAL_SetLinkUp(HI_U32 handle);

//*****************************************************************************
// 函数名称: SAL_RcvNetData
// 功能描述: 将接受到的数据发送给协议栈
//
// 参数说明:
//      handle:设备句柄
//      pRcvData:数据指针
//      ulDataLen:数据长度
// 返 回 值:
//      HI_ERR_SUCCESS:成功
//      其它 失败
// 调用要求:
//
//*****************************************************************************
HI_U32 SAL_RcvNetData(HI_U32 handle,HI_CHAR * pRcvData,HI_U32 ulDataLen);

//*****************************************************************************
// 函数名称: SAL_Add_Route
// 功能描述: 添加路由
//
// 参数说明:
//      ucIfName:设备名称
//      ip_addr:目标地址
//      net_mask:数据长度
//      g_w:网关
// 返 回 值:
//      HI_ERR_SUCCESS:成功
//      其它 失败
// 调用要求:
//
//*****************************************************************************
HI_U32 SAL_Add_Route(HI_PCSTR ucIfName, HI_U8 ip_addr[HI_IPV4_ADDR_LEN], HI_U8 net_mask[HI_IPV4_ADDR_LEN],HI_U8 g_w[HI_IPV4_ADDR_LEN]);
HI_U32 SAL_routeAdd(HI_U8 *destAddr, HI_U8 *netMask, HI_U8 *gateway, HI_PCSTR pszName);
//*****************************************************************************
// 函数名称: SAL_routeDelete
// 功能描述：删除路由
//
// 参数说明:
//      ip_addr:目标地址
// 返 回 值:
//      HI_ERR_SUCCESS:成功
//      其它 失败
// 调用要求:
//
//*****************************************************************************
HI_U32 SAL_routeDelete(HI_U8 ip_addr[4]);

//*****************************************************************************
// 函数名称: SAL_RouteGet
// 功能描述:获取路由信息
//
// 参数说明:
//      pfnPrint:在SAL_RouteGet接口中循环调用多次该函数将路由信息上报给用户
//      ulUsrData:用户数据
// 返 回 值:
//      HI_ERR_SUCCESS:
//      其它 :接口错误或回调函数返回错误,当发生错误时底层不继续上报剩余路由信息，直接返回
// 调用要求:
//
//*****************************************************************************
HI_U32 SAL_RouteGet(print_rt_fn pfnPrint,HI_U32 ulUsrData);

#endif

