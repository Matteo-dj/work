//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cco_broadcast.h
//  版 本 号   : V1.0 
//  作    者   : cuiate/00233580
//  生成日期   : 2013-02-20
//  功能描述   : 广播函数及接口定义
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2013-02-20
//    作    者 : cuiate/00233580
//    修改内容 : 创建文件 
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


HI_END_HEADER


#endif //__MRS_SRV_CCO_BROADCAST_H__

