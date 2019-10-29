
#ifndef     __HI_DRV_COM_H__
#define     __HI_DRV_COM_H__
#include <hi_driver_internal.h>
#include <hi_types.h>

HI_BOOL IsV200Boot(HI_VOID);
HI_VOID HI_COM_SetEnvAddr(HI_VOID);
HI_U32 HI_COM_GetBootVer(HI_U8 acBootVer[3]);

void hisi_eth_init(void);
unsigned int hisi_eth_phy_powerdown(void);
void hisi_eth_prepare_reset(void);
void hisi_eth_finish_reset(void);

unsigned int hisi_eth_send_data(void *pData,HI_U16 usLen);
void hisi_eth_pkt_free(void *pbuf);
void hisi_eth_switch_send2lwip_stack(void *netif, void *pbuf);

#endif  /*__HI_DRV_COM_H__*/
