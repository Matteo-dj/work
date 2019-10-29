#ifndef __ICC_VX_H__
#define __ICC_VX_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined (PRODUCT_CFG_ICC_FMGR_SUPPORT) 
HI_VOID HI_ICC_CheckAllChanConnect(HI_VOID);
HI_U32 HI_ICC_CheckConnectCmd(HI_OUT HI_U32 *ret_result,ICC_CHANNEL_ID_E channelId);
#else
HI_VOID HI_ICC_CheckAllChanConnect(HI_VOID){return;}
HI_U32 HI_ICC_CheckConnectCmd(HI_OUT HI_U32 *ret_result,ICC_CHANNEL_ID_E channelId){return HI_ERR_NOT_SUPPORT;}
#endif
#endif
