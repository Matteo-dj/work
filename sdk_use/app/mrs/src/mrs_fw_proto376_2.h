//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_fw_proto376_2.h
//  版 本 号   : V1.0 
//  作    者   : shenhankun/00130424
//  生成日期   : 2011-11-29
//  功能描述   : TODO: ...
//               
//  函数列表   : mrs3762StreamInput
//  修改历史   : 
//  1.日    期 : 2011-11-29
//    作    者 : shenhankun/00130424
//    修改内容 : 创建文件 
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   : 
// SUBSYSTEM :
// MODULE    :  
// OWNER     :  
//*****************************************************************************

#ifndef __MRS_FW_PROTO376_2_H__
#define __MRS_FW_PROTO376_2_H__
HI_START_HEADER


#define MRS_3762_FRAME_HEAD_SIZE       1   //376.2帧头字节数
#define MRS_3762_FRAME_LEN_SIZE        2   //376.2帧长度字节数
#define MRS_3762_FRAME_CTRL_SIZE       1   //376.2帧控制字字节数
#define MRS_3762_FRAME_INFO_SIZE       6   //376.2帧信息域字节数
#define MRS_3762_FRAME_AFN_SIZE        1   //376.2帧AFN字节数
#define MRS_3762_FRAME_FN_SIZE         2   //376.2帧FN字节数
#define MRS_3762_FRAME_CS_SIZE         1   //376.2帧校验字节数
#define MRS_3762_FRAME_END_SIZE        1   //376.2帧尾字节数

#define MRS_3762_FRAME_MIN_SIZE        (MRS_3762_FRAME_HEAD_SIZE  \
                                       + MRS_3762_FRAME_LEN_SIZE  \
                                       + MRS_3762_FRAME_CTRL_SIZE \
                                       + MRS_3762_FRAME_INFO_SIZE \
                                       + MRS_3762_FRAME_AFN_SIZE  \
                                       + MRS_3762_FRAME_FN_SIZE   \
                                       + MRS_3762_FRAME_CS_SIZE   \
                                       + MRS_3762_FRAME_END_SIZE)   //376.2帧固定最小长度


typedef struct  
{
    HI_U8* pucRcvBuffer;    // 内存分配出来的接收帧buffer
    HI_U16 usRcvBufferSize; // 接收帧实际buffer大小
    HI_U16 usDagramSize;    // 数据理论大小 
} MRS_3762_PROTO_CTX_STRU;

//*****************************************************************************
// 函数名称: mrs3762ProtoInit
// 功能描述: 376.2协议处理上下文初始化
//           
// 参数说明:
//   pstCtx [in/out] MRS_3762_PROTO_CTX_STRU结构
//
// 返 回 值: 
//    HI_ERR_SUCCESS : 成功获取完整的376.2帧 
//    其它为错误码 
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-29]
//*****************************************************************************
HI_U32 mrs3762ProtoInit(HI_IN HI_OUT MRS_3762_PROTO_CTX_STRU* pstCtx);

//*****************************************************************************
// 函数名称: mrs3762ProtoStreamInput
// 功能描述: 对输入的字符流镜像分段重组, 判断帧完整性并获得完整的376.2帧
//           
// 参数说明:
//   pucStream   [in] 输入字符流
//   usStreamLen [in] 输入字符流大小
//   ppstFrame   [out] 输出完整376.2帧, 使用者不能释放pstFrame.pucDatagram该指针.
//
// 返 回 值: 
//    HI_ERR_SUCCESS : 成功获取完整的376.2帧及其基本信息
//    HI_ERR_CONTINUE : 未能找到任何的完整帧,需要继续等待数据
//    其它为错误码 
//
// 调用要求: 使用者不能释放pstFrame.pucDatagram该指针.
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-29]
//*****************************************************************************
HI_U32 mrs3762ProtoStreamInput(HI_IN HI_PBYTE pucStream, HI_U16 usStreamLen, HI_OUT MRS_ONE_RAW_FRAME_STRU** ppstFrame);

HI_U32 mrs3762ProtoStreamInputCheck(HI_BOOL bDiscardOldStream);
HI_VOID mrs3762DiscardOldData(HI_VOID);
HI_VOID mrs3762RealmDlDec(HI_IN HI_PBYTE pInBuffer, HI_OUT MRS_3762_INFO_REALM_DL_STRU * pstInfoRealmDl);

HI_END_HEADER

#endif //__MRS_PROTO376_2_H__
