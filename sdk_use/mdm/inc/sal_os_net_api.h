
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
��·��output����
pBuf���������bufָ������
ulen���������buf��������
pBufCout����������ɼ���buf���
totalSize������ulen[0]+ulen[1]+...ulen[pBufCout-1]
*/
typedef HI_U32  (*hi_net_link_out)(HI_PVOID pBuf[],HI_U32 ulen[],HI_U32 pBufCout,HI_U32 totalSize);
/*ip��output����*/
typedef HI_U32  (*hi_net_ip_out)(HI_PVOID pBuf[],HI_U32 ulen[],HI_U32 pBufCout,HI_U32 totalSize,HI_U32 next_ip);
/*����mac��ַ����*/
typedef HI_U32 (*hi_net_set_mac)(HI_U8  *addr, HI_U8 len);
/*��ȡ·����Ϣ����*/
typedef HI_U32 (*print_rt_fn)(char *ucIfName,HI_U8 ip_addr[4],HI_U8 net_mask[4],HI_U8 g_w[4],HI_U32 ulUsrData);

#define HI_NET_MAX_HWADDR_LEN 6//�Ӳ����ַ
#define HI_NET_MAX_NAME_LEN 8//�netif����

typedef struct
{
    HI_U8 ip_addr[4];//IP
    HI_U8 net_mask[4];//netmask

    HI_U16 default_mtu;//mtu
    HI_U8 hwaddr[HI_NET_MAX_HWADDR_LEN];//mac

    hi_net_ip_out pfnIpOut;//IP������ص��ӿ�
    hi_net_set_mac pfnSetMac; //����mac�ص��ӿ�

    HI_U8 hwaddr_len;//Ӳ����ַ����
    HI_U8 pad1[3];
}HI_IP_NET_IF_INIT_CFG_S;//��ETH������ʼ������


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
}HI_ETH_NET_IF_INIT_CFG_S;//ETH������ʼ������

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
// ��������: SAL_CreateIpNetIf
// ��������: ������ETH����ʵ��
//
// ����˵��:
//      name:��������
//      pCfg:��ʼ������
// �� �� ֵ:
//      HI_NET_INVALID_HANDLE���Ƿ����
//      ��������������ɹ�
// ����Ҫ��:
//
//*****************************************************************************
HI_U32 SAL_CreateIpNetIf(HI_CHAR name[HI_NET_MAX_NAME_LEN],HI_IP_NET_IF_INIT_CFG_S *pCfg);
//*****************************************************************************
// ��������: SAL_CreateEthNetIf
// ��������: ����ETH����ʵ��
//
// ����˵��:
//      name:��������
//      pCfg:��ʼ������
// �� �� ֵ:
//      HI_NET_INVALID_HANDLE���Ƿ����
//      ��������������ɹ�
// ����Ҫ��:
//
//*****************************************************************************
HI_U32 SAL_CreateEthNetIf(HI_CHAR name[HI_NET_MAX_NAME_LEN],HI_ETH_NET_IF_INIT_CFG_S *pCfg);

//*****************************************************************************
// ��������: SAL_SetLinkDown
// ��������: �������ӶϿ�
//
// ����˵��:
//      handle:�豸���
// �� �� ֵ:
//      HI_ERR_SUCCESS:�ɹ�
//      ���� ʧ��
// ����Ҫ��:
//
//*****************************************************************************
HI_U32 SAL_SetLinkDown(HI_U32 handle);

//*****************************************************************************
// ��������: SAL_SetLinkUp
// ��������: ����������ɽ
//
// ����˵��:
//      handle:�豸���
// �� �� ֵ:
//      HI_ERR_SUCCESS:�ɹ�
//      ���� ʧ��
// ����Ҫ��:
//
//*****************************************************************************
HI_U32 SAL_SetLinkUp(HI_U32 handle);

//*****************************************************************************
// ��������: SAL_RcvNetData
// ��������: �����ܵ������ݷ��͸�Э��ջ
//
// ����˵��:
//      handle:�豸���
//      pRcvData:����ָ��
//      ulDataLen:���ݳ���
// �� �� ֵ:
//      HI_ERR_SUCCESS:�ɹ�
//      ���� ʧ��
// ����Ҫ��:
//
//*****************************************************************************
HI_U32 SAL_RcvNetData(HI_U32 handle,HI_CHAR * pRcvData,HI_U32 ulDataLen);

//*****************************************************************************
// ��������: SAL_Add_Route
// ��������: ���·��
//
// ����˵��:
//      ucIfName:�豸����
//      ip_addr:Ŀ���ַ
//      net_mask:���ݳ���
//      g_w:����
// �� �� ֵ:
//      HI_ERR_SUCCESS:�ɹ�
//      ���� ʧ��
// ����Ҫ��:
//
//*****************************************************************************
HI_U32 SAL_Add_Route(HI_PCSTR ucIfName, HI_U8 ip_addr[HI_IPV4_ADDR_LEN], HI_U8 net_mask[HI_IPV4_ADDR_LEN],HI_U8 g_w[HI_IPV4_ADDR_LEN]);
HI_U32 SAL_routeAdd(HI_U8 *destAddr, HI_U8 *netMask, HI_U8 *gateway, HI_PCSTR pszName);
//*****************************************************************************
// ��������: SAL_routeDelete
// ����������ɾ��·��
//
// ����˵��:
//      ip_addr:Ŀ���ַ
// �� �� ֵ:
//      HI_ERR_SUCCESS:�ɹ�
//      ���� ʧ��
// ����Ҫ��:
//
//*****************************************************************************
HI_U32 SAL_routeDelete(HI_U8 ip_addr[4]);

//*****************************************************************************
// ��������: SAL_RouteGet
// ��������:��ȡ·����Ϣ
//
// ����˵��:
//      pfnPrint:��SAL_RouteGet�ӿ���ѭ�����ö�θú�����·����Ϣ�ϱ����û�
//      ulUsrData:�û�����
// �� �� ֵ:
//      HI_ERR_SUCCESS:
//      ���� :�ӿڴ����ص��������ش���,����������ʱ�ײ㲻�����ϱ�ʣ��·����Ϣ��ֱ�ӷ���
// ����Ҫ��:
//
//*****************************************************************************
HI_U32 SAL_RouteGet(print_rt_fn pfnPrint,HI_U32 ulUsrData);

#endif

