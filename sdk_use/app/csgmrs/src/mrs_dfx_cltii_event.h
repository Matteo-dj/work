//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2015, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_dfx_cltii_event.h
//  �� �� ��   : V1.0 
//  ��    ��   : cuiate/00233580
//  ��������   : 2015-07-21
//  ��������   : II���¹ҵ���¼�ά����Ϣ �������ӿ�����
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2015-07-21
//    ��    �� : cuiate/00233580
//    �޸����� : �����ļ� 
//
//*****************************************************************************

#ifndef _MRS_DFX_CLTII_EVENT_H_
#define _MRS_DFX_CLTII_EVENT_H_
HI_START_HEADER
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)


HI_U32 mrsDfxGetCltiiEventStatus(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

HI_U32 mrsDfxGetCltiiEventInfo(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

HI_U32 mrsDfxCltiiEventInit(HI_VOID);

HI_VOID mrsDfxCltiiEventMonitorMeterInf(HI_U8 *pucMeter, HI_U8 ucProtocol);

HI_VOID mrsDfxCltiiEventMonitorStart(HI_VOID);

HI_VOID mrsDfxCltiiEventMonitorEnd(HI_VOID);

HI_VOID mrsDfxCltiiEventLastCount(HI_U8 ucCount);

HI_VOID mrsDfxCltiiEventLastReported(HI_U8 ucCount);

HI_VOID mrsDfxCltiiEventLastCleared(HI_VOID);

#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_END_HEADER
#endif


