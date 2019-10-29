//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_cco_broadcast.h
//  �� �� ��   : V1.0 
//  ��    ��   : cuiate/00233580
//  ��������   : 2013-02-20
//  ��������   : �㲥�������ӿڶ���
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2013-02-20
//    ��    �� : cuiate/00233580
//    �޸����� : �����ļ� 
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   : 
// SUBSYSTEM :
// MODULE    :  
// OWNER     :  
//*****************************************************************************

#ifndef __MRS_SRV_CCO_BROADCAST_H__
#define __MRS_SRV_CCO_BROADCAST_H__


HI_START_HEADER


HI_U32 mrsBroadcastProcess(HI_IN HI_U8 * pDataBuff, HI_IN HI_U16 usDataLen);

HI_VOID mrsBroadcastReset(HI_VOID);

HI_U32 mrsSendBroadcastFramePlc(HI_VOID);

HI_VOID mrsBroadcastRegFinishNotify(HI_VOID (*pfnBroadcastFinishNotify)(HI_VOID*), HI_VOID *pParam);


HI_END_HEADER


#endif //__MRS_SRV_CCO_BROADCAST_H__
