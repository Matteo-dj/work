#ifndef _RADIO_COMM_H_
#define _RADIO_COMM_H_               
#include <hi_types.h>
#include <drv_dfx.h>
#include <hi_driver.h>

HI_GPIO_VALUE_E radio_nirq_level(HI_VOID);
HI_U32 radio_comm_GetResp(HI_U8 byteCount, HI_U8* pData);
HI_U32 radio_comm_SendCmd(HI_U8 byteCount, HI_U8* pData);
HI_U32 radio_comm_ReadDataWaiteCts(HI_U8 cmd, HI_U8 byteCount, HI_U8* pData);
HI_U32 radio_comm_ReadDataNoCts(HI_U8 cmd, HI_U8 byteCount, HI_U8* pData);
HI_U32 radio_comm_WriteDataWaiteCts(HI_U8 cmd, HI_U8 byteCount, HI_U8* pData);
HI_U32 radio_comm_WriteDataNoCts(HI_U8 cmd, HI_U8 byteCount, HI_U8* pData);
HI_U32 radio_comm_SendCmdGetResp(HI_U8 cmdByteCount,HI_U8* pCmdData,HI_U8 respByteCount,HI_U8* pRespData);
HI_U32 radio_comm_PollCTS(hi_void);
#ifdef PRODUCT_CFG_SUPPORT_RF_DEBUG
HI_VOID radio_backup_cmd_order(HI_VOID);
HI_VOID radio_init_cmd_err(HI_VOID);
#endif//PRODUCT_CFG_SUPPORT_RF_DEBUG
#endif//_RADIO_COMM_H_