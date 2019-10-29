
#ifndef __HI_MDM_DFXCHL_H__
#define __HI_MDM_DFXCHL_H__

HI_START_HEADER

#if !defined(PRODUCT_CFG_BUILD_HSO_CODE)

HI_EAPI HI_SAL_DFX_HOST_CTRL_S* HI_MDM_DfxGetHostCtrl(HI_VOID);
HI_EAPI HI_U32 HI_MDM_DfxSetHostCtrl(HI_SAL_DFX_HOST_CTRL_S* pstCtrl, HI_U32 pfnSendMrPacketByPass, HI_U32 pfSendMessageByPass);
HI_EAPI HI_U32 HI_MDM_DfxChlUartRx(HI_U16 usPort, HI_IN HI_PBYTE pDataBuffer, HI_U16 usDataBufferLen);

#endif

HI_END_HEADER
#endif
