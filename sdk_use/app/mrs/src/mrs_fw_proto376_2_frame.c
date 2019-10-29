//*****************************************************************************
//
//                  版权所有 (C), 1998-2013, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_fw_proto376_2_frame.c
//  版 本 号   : V1.0 
//  作    者   : cuiate/00233580
//  生成日期   : 2013-03-11
//  功能描述   : 376.2组帧接口
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2013-03-11
//    作    者 : cuiate/00233580
//    修改内容 : 创建文件 
//
//  2.日    期 : 2013-07-11
//    作    者 : cuiate/00233580
//    修改内容 : 修改文件 
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   : 
// SUBSYSTEM :
// MODULE    :  
// OWNER     :  
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_fw.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_proto376_2_frame.h"
#include "mrs_srv_common.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_parallel_cco.h"
#include "mrs_srv_list.h"
#include "mrs_srv_cco_event.h"
#include "mrs_srv_cco.h"
#include "mrs_fw_proto645.h"

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_NDM))
#define PROTO376_2_HEAD_SIZE        (1)
#define PROTO376_2_LENGTH_SIZE      (2)
#define PROTO376_2_CTRL_SIZE        (1)
#define PROTO376_2_R_SIZE           (6)
#define PROTO376_2_ADDR_SIZE        (12)
#define PROTO376_2_AFN_SIZE         (1)
#define PROTO376_2_DT_SIZE          (2)
#define PROTO376_2_CS_SIZE          (1)
#define PROTO376_2_END_SIZE         (1)

#define PROTO376_2_FIXED_SIZE   \
    (PROTO376_2_HEAD_SIZE + PROTO376_2_LENGTH_SIZE + PROTO376_2_CTRL_SIZE + PROTO376_2_R_SIZE \
    + PROTO376_2_AFN_SIZE + PROTO376_2_DT_SIZE + PROTO376_2_CS_SIZE + PROTO376_2_END_SIZE)


typedef enum
{
    PROTO376_2_COMM_FORM_RESERVED = 0,
    PROTO376_2_COMM_FORM_CENTRALIZED,
    PROTO376_2_COMM_FORM_DISTRIBUTED,
    PROTO376_2_COMM_FORM_BROADBAND,
} PROTO376_2_COMM_FORM_E;

typedef enum
{
    PROTO376_2_CTRL_DIR_DL,
    PROTO376_2_CTRL_DIR_UL
} PROTO376_2_CTRL_DIR_E;

#define PROTO376_2_CTRL_DIR     (PROTO376_2_CTRL_DIR_UL << 7)

HI_U32 mrsCreate3762Frame(HI_IN PROTO376_2_FRAME_PRE_STRU *pstFrame, 
          HI_OUT HI_PBYTE *ppOutBuffer, HI_OUT HI_U16 *pusBufferLen, HI_IN HI_U8 ucMeterAddr[])
{
    HI_U16 usBufLen = PROTO376_2_FIXED_SIZE;
    HI_U16 usOffset = 0;
    HI_U8 *pBuf = HI_NULL;
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    MRS_CCO_SRV_CTX_STRU *pCtx = mrsCcoGetContext();
#endif

    HI_UNREF_PARAM(ucMeterAddr);

    if (!pstFrame || !ppOutBuffer || !pusBufferLen)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if (1 == pstFrame->ucModuleFlag)
    {
        // 有地址域
        usBufLen += HI_METER_ADDR_LEN * 2;
    }

    usBufLen += pstFrame->stAfnData.usDataLen;
    if (pstFrame->ucIs645)
    {
        usBufLen += 2;  // 规约类型1 + 长度1
        if (MRS_AFN(0xF1) == pstFrame->stAfnData.ucAFN)
        {
            usBufLen++;
        }
    }

    pBuf = mrsToolsMalloc(usBufLen);
    if (!pBuf)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memset_s(pBuf, usBufLen, 0, usBufLen);

    // 起始字符
    pBuf[usOffset++] = PROTO3762_START_FLG;

    // 长度
    (hi_void)memcpy_s(pBuf + usOffset, usBufLen - usOffset, &usBufLen, 2);
    usOffset += 2;

    // 控制域
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    if (pCtx->ucProtoVer == PROTO_VER_3762_2009)
    {
        pBuf[usOffset++] = PROTO376_2_CTRL_DIR | ((pstFrame->ucPrm&1) << 6) | PROTO376_2_COMM_FORM_CENTRALIZED;
    }
    else
#endif
    {
        pBuf[usOffset++] = PROTO376_2_CTRL_DIR | ((pstFrame->ucPrm&1) << 6) | PROTO376_2_COMM_FORM_BROADBAND;
    }

    // 信息域
    pBuf[usOffset++] = (HI_U8)((pstFrame->ucRelayLevel << 4) |((pstFrame->ucModuleFlag & 0x01) << 2));    //通讯模块标识 - 1
    pBuf[usOffset++] = 0;
    pBuf[usOffset++] = (HI_U8)(pstFrame->ucPhase & 0x0F);;
    pBuf[usOffset++] = 0;
    pBuf[usOffset++] = pstFrame->ucEvtFlg;
    pBuf[usOffset++] = pstFrame->ucSeq; // 帧序列号

    // 地址域
    if (1 == pstFrame->ucModuleFlag)
    {
        (hi_void)memcpy_s(pBuf + usOffset, usBufLen - usOffset, pstFrame->ucSrcAdd, HI_METER_ADDR_LEN);
        usOffset += HI_METER_ADDR_LEN;
        (hi_void)memcpy_s(pBuf + usOffset, usBufLen - usOffset, pstFrame->ucDstAdd, HI_METER_ADDR_LEN);
        usOffset += HI_METER_ADDR_LEN;
    }

    // AFN
    pBuf[usOffset++] = pstFrame->stAfnData.ucAFN;

    // DT
    (hi_void)memcpy_s(pBuf + usOffset, usBufLen - usOffset, pstFrame->stAfnData.ucDT, 2);
    usOffset += 2;

    // 用户数据单元
    if (pstFrame->ucIs645)
    {
        pBuf[usOffset++] = pstFrame->ucProtocol;
        if (MRS_AFN(0xF1) == pstFrame->stAfnData.ucAFN)
        {
            pBuf[usOffset++] = (HI_U8)(pstFrame->stAfnData.usDataLen & 0xff);
            pBuf[usOffset++] = (HI_U8)((pstFrame->stAfnData.usDataLen >> 8) & 0xff);
        }
        else
        {
            pBuf[usOffset++] = (HI_U8)pstFrame->stAfnData.usDataLen;
        }
    }

    if (pstFrame->stAfnData.usDataLen > 0)
    {
        (hi_void)memcpy_s(pBuf + usOffset, usBufLen - usOffset, pstFrame->stAfnData.pData, pstFrame->stAfnData.usDataLen);
        usOffset += pstFrame->stAfnData.usDataLen;
    }
    
    // CS
    pBuf[usOffset++] = mrsToolsCalcCheckSum(pBuf + 3, usBufLen - 5);

    // 结束字符
    pBuf[usOffset++] = PROTO3762_END_FLG;

    *ppOutBuffer = pBuf;
    *pusBufferLen = usBufLen;

    return HI_ERR_SUCCESS;
}

/*****************************************************************
FunctionName  :  mrsCreate3762UpFrame
Parameters    :  pstAfnData
              ppOutBuf
              pusBufLen
Return        :    
Description   :  

Author        :  c00233580
Creation time :  2013/3/13
*****************************************************************/
HI_U32 mrsCreate3762UpFrame(HI_IN PROTO376_2_FRAME_AFN_AND_DATA_STRU * pstAfnData, 
        HI_OUT HI_PBYTE *ppOutBuf, HI_OUT HI_U16 *pusBufLen, HI_IN HI_U8 ucMeterAddr[], HI_U8 aucMacAdd[HI_PLC_MAC_ADDR_LEN])
{
    PROTO376_2_FRAME_PRE_STRU stFrame;
    MRS_CCO_SRV_CTX_STRU *pCtx = mrsCcoGetContext();

    if (!pstAfnData || !ppOutBuf || !pusBufLen)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    (hi_void)memset_s(&stFrame, sizeof(stFrame), 0, sizeof(stFrame));
    stFrame.ucPrm = 1;
    stFrame.ucSeq = ++(pCtx->ucSeq);
    if ((pstAfnData->ucAFN == MRS_CCO_PROTOCOL_AFN_06) && (pstAfnData->ucDT[0] == 2 &&(pstAfnData->ucDT[1] == 0)))
    {
        stFrame.ucRelayLevel = mrsMapGetLevel(ucMeterAddr, aucMacAdd);
    }
    
    (hi_void)memcpy_s(&stFrame.stAfnData, sizeof(stFrame.stAfnData), pstAfnData, sizeof(PROTO376_2_FRAME_AFN_AND_DATA_STRU));
    
    if (pstAfnData->ucPhaseFlag)
    {
        stFrame.ucPhase = mrsCcoGetStaPhase(pCtx->aucSrcMac);
        pCtx->ucPhase = stFrame.ucPhase;
    }
    
    return mrsCreate3762Frame(&stFrame, ppOutBuf, pusBufLen, ucMeterAddr);
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
HI_U32 mrsCreateFrame3762Bbu(HI_U16 usDatagramSize, HI_PBYTE pucDatagram, MRS_ONE_RAW_FRAME_STRU** ppstFrame)
{
    if (!pucDatagram)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    // 如果是广播，表地址/报文长度/控制字检查
    if(mrsToolsCheckBroadCast(pucDatagram, usDatagramSize))
    {   
        // 广播校时
        return mrsCreateBroadCastFrame3762_Down_Bbu(usDatagramSize, pucDatagram, ppstFrame); 
    }
    else
    {    
        // AFN 13F1s
        return mrsCreateAFN13F1Frame3762_Dwon_Bbu(usDatagramSize, pucDatagram, ppstFrame); 
    }
}

HI_U32 mrsCreateAFN13F1Frame3762_Dwon_Bbu(HI_U16 usDatagramSize, HI_PBYTE pucDatagram, MRS_ONE_RAW_FRAME_STRU** ppstFrame)
{
    HI_U8 ucProtocol = 0;
    HI_U16 usBufLen = PROTO376_2_FIXED_SIZE;
    HI_U16 usOffset = 0;
    HI_PBYTE pBuf = HI_NULL;
    MRS_ONE_RAW_FRAME_STRU * pstTempFrame = HI_NULL;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    
    if (!pucDatagram)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    // 有地址域
    usBufLen += HI_METER_ADDR_LEN * 2;

    //如果是13规约 +2
    if (pstCcoCtx->ucProtoVer != PROTO_VER_3762_2009)
    {
        usBufLen += 1;
    }
    usBufLen += 3;

    // 645报文长度
    usBufLen += usDatagramSize;
    pstTempFrame = mrsToolsMalloc(sizeof(MRS_ONE_RAW_FRAME_STRU) + usBufLen);
    
    if (!pstTempFrame)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memset_s(pstTempFrame, sizeof(MRS_ONE_RAW_FRAME_STRU) + usBufLen, 0, sizeof(MRS_ONE_RAW_FRAME_STRU) + usBufLen);
    
    pBuf = (HI_PBYTE)(pstTempFrame) + sizeof(MRS_ONE_RAW_FRAME_STRU);
    pstTempFrame->usDatagramSize = usBufLen;
    pstTempFrame->pucDatagram = pBuf;

    // 起始字符
    pBuf[usOffset++] = PROTO3762_START_FLG;

    // 长度
    (hi_void)memcpy_s(pBuf + usOffset, usBufLen - usOffset, &usBufLen, 2);
    usOffset += 2;

    // 控制域
    if (pstCcoCtx->ucProtoVer == PROTO_VER_3762_2009)
    {
        pBuf[usOffset++] = PROTO376_2_COMM_FORM_CENTRALIZED;
    }
    else
    {
        pBuf[usOffset++] = PROTO376_2_COMM_FORM_BROADBAND;
    }

    // 信息域
    pBuf[usOffset++] = (HI_U8)((0x01) << 2);    //通讯模块标识 - 1
    pBuf[usOffset++] = 0;
    pBuf[usOffset++] = 0;
    pBuf[usOffset++] = 0;
    pBuf[usOffset++] = 0;
    pBuf[usOffset++] = 0; // 帧序列号

    // 地址域
    (hi_void)memset_s(pBuf + usOffset, usBufLen - usOffset, 0,HI_METER_ADDR_LEN);    // 源地址 填充0
    usOffset += HI_METER_ADDR_LEN;    
    (hi_void)memcpy_s(pBuf + usOffset, usBufLen - usOffset, pucDatagram + 1, HI_METER_ADDR_LEN);    // 目的地址
    usOffset += HI_METER_ADDR_LEN;

    // AFN 13F1
    pBuf[usOffset++] = 0x13;

    // DT 13F1
    pBuf[usOffset++] = 0x1;
    pBuf[usOffset++] = 0x0;

    // 用户数据单元
    ucProtocol = mrsToolsGet645Ver(pucDatagram[MRS_645_FRAME_CTRL_OFFSET]);
    pBuf[usOffset++] = ucProtocol;          // 规约类型
    if (pstCcoCtx->ucProtoVer != PROTO_VER_3762_2009)       // 13规约
    {
        pBuf[usOffset++] = 0x0;     // 通信延时相关性标志
    }
    pBuf[usOffset++] = 0x0;                    // 从节点数量为0
    pBuf[usOffset++] = (HI_U8)usDatagramSize;         // 645长度

    //645报文
    (hi_void)memcpy_s(pBuf + usOffset, usBufLen - usOffset, pucDatagram, usDatagramSize);    // 目的地址
    usOffset += usDatagramSize;

    // CS
    pBuf[usOffset++] = mrsToolsCalcCheckSum(pBuf + 3, usBufLen - 5);

    // 结束字符
    pBuf[usOffset++] = PROTO3762_END_FLG;    

    *ppstFrame = pstTempFrame;

    return HI_ERR_SUCCESS;    
}


HI_U32 mrsCreateBroadCastFrame3762_Down_Bbu(HI_U16 usDatagramSize, HI_PBYTE pucDatagram, MRS_ONE_RAW_FRAME_STRU** ppstFrame)
{
    HI_U8 ucProtocol = 0;
    HI_U16 usBufLen = PROTO376_2_FIXED_SIZE;
    HI_U16 usOffset = 0;
    HI_PBYTE pBuf = HI_NULL;
    MRS_ONE_RAW_FRAME_STRU * pstTempFrame = HI_NULL;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    
    if (!pucDatagram)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    // 有地址域
    usBufLen += HI_METER_ADDR_LEN * 2;

    // 控制字、长度
    usBufLen += 2;

    // 645报文长度
    usBufLen += usDatagramSize;
    pstTempFrame = mrsToolsMalloc(sizeof(MRS_ONE_RAW_FRAME_STRU) + usBufLen);
    
    if (!pstTempFrame)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memset_s(pstTempFrame, sizeof(MRS_ONE_RAW_FRAME_STRU) + usBufLen, 0, sizeof(MRS_ONE_RAW_FRAME_STRU) + usBufLen);
    
    pBuf = (HI_PBYTE)(pstTempFrame) + sizeof(MRS_ONE_RAW_FRAME_STRU);
    pstTempFrame->usDatagramSize = usBufLen;
    pstTempFrame->pucDatagram = pBuf;

    // 起始字符
    pBuf[usOffset++] = PROTO3762_START_FLG;

    // 长度
    (hi_void)memcpy_s(pBuf + usOffset, usBufLen - usOffset, &usBufLen, 2);
    usOffset += 2;

    // 控制域
    if (pstCcoCtx->ucProtoVer == PROTO_VER_3762_2009)
    {
        pBuf[usOffset++] = PROTO376_2_COMM_FORM_CENTRALIZED;
    }
    else
    {
        pBuf[usOffset++] = PROTO376_2_COMM_FORM_BROADBAND;
    }

    // 信息域
    pBuf[usOffset++] = (HI_U8)((0x01) << 2);    //通讯模块标识 - 1
    pBuf[usOffset++] = 0;
    pBuf[usOffset++] = 0;
    pBuf[usOffset++] = 0;
    pBuf[usOffset++] = 0;
    pBuf[usOffset++] = 0; // 帧序列号

    // 地址域
    (hi_void)memset_s(pBuf + usOffset, usBufLen - usOffset,0, HI_METER_ADDR_LEN);    // 源地址 填充0
    usOffset += HI_METER_ADDR_LEN;    
    (hi_void)memcpy_s(pBuf + usOffset, usBufLen - usOffset, pucDatagram + 1, HI_METER_ADDR_LEN);    // 目的地址
    usOffset += HI_METER_ADDR_LEN;

    // AFN 05F3
    pBuf[usOffset++] = 0x5;
    
    // DT 05F3
    pBuf[usOffset++] = 0x4;
    pBuf[usOffset++] = 0x0;

    // 用户数据单元
    ucProtocol = mrsToolsGet645Ver(pucDatagram[MRS_645_FRAME_CTRL_OFFSET]);
    pBuf[usOffset++] = ucProtocol;          // 规约类型

    pBuf[usOffset++] = (HI_U8)usDatagramSize;         // 645长度

    //645报文
    (hi_void)memcpy_s(pBuf + usOffset, usBufLen - usOffset, pucDatagram, usDatagramSize);    // 目的地址
    usOffset += usDatagramSize;

    // CS
    pBuf[usOffset++] = mrsToolsCalcCheckSum(pBuf + 3, usBufLen - 5);

    // 结束字符
    pBuf[usOffset++] = PROTO3762_END_FLG;    

    *ppstFrame = pstTempFrame;

    return HI_ERR_SUCCESS;    
}
#endif
#endif

#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)
HI_U32 mrsCreate3762FrameDown(HI_IN PROTO376_2_FRAME_PRE_STRU *pstFrame, 
          HI_OUT HI_PBYTE *ppOutBuffer, HI_OUT HI_U16 *pusBufferLen, HI_IN HI_U8 ucMeterAddr[])
{
    HI_U16 usBufLen = PROTO376_2_FIXED_SIZE;
    HI_U16 usOffset = 0;
    HI_U8 *pBuf = HI_NULL;
    MRS_CCO_SRV_CTX_STRU *pCtx = mrsCcoGetContext();
    
    if (!pstFrame || !ppOutBuffer || !pusBufferLen)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if (1 == pstFrame->ucModuleFlag)
    {
        // 有地址域
        usBufLen += HI_METER_ADDR_LEN * 2;
    }

    usBufLen += pstFrame->stAfnData.usDataLen;
    if (pstFrame->ucIs645)
    {
        usBufLen += 2;  // 规约类型1 + 长度1
        if (MRS_AFN(0xF1) == pstFrame->stAfnData.ucAFN)
        {
            usBufLen++;
        }
    }

    pBuf = mrsToolsMalloc(usBufLen);
    if (!pBuf)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    mrsToolsMemZero_s(pBuf, usBufLen, usBufLen);

    // 起始字符
    pBuf[usOffset++] = PROTO3762_START_FLG;

    // 长度
    mrsToolsMemcpy_s(pBuf + usOffset, usBufLen - usOffset, &usBufLen, 2);
    usOffset += 2;

    // 控制域
    if (pCtx->ucProtoVer == PROTO_VER_3762_2009)
    {
        pBuf[usOffset++] = ((pstFrame->ucPrm&1) << 6) | PROTO376_2_COMM_FORM_CENTRALIZED;
    }
    else
    {
        pBuf[usOffset++] = ((pstFrame->ucPrm&1) << 6) | PROTO376_2_COMM_FORM_BROADBAND;
    }

    // 信息域
    pBuf[usOffset++] = (HI_U8)((pstFrame->ucRelayLevel << 4) |((pstFrame->ucModuleFlag & 0x01) << 2));    //通讯模块标识 - 1
    pBuf[usOffset++] = 0;
    pBuf[usOffset++] = (HI_U8)(pstFrame->ucPhase & 0x0F);;
    pBuf[usOffset++] = 0;
    pBuf[usOffset++] = pstFrame->ucEvtFlg;
    pBuf[usOffset++] = pstFrame->ucSeq; // 帧序列号

    // 地址域
    if (1 == pstFrame->ucModuleFlag)
    {
        mrsToolsMemcpy_s(pBuf + usOffset, usBufLen - usOffset, pstFrame->ucSrcAdd, HI_METER_ADDR_LEN);
        usOffset += HI_METER_ADDR_LEN;
        mrsToolsMemcpy_s(pBuf + usOffset, usBufLen - usOffset, pstFrame->ucDstAdd, HI_METER_ADDR_LEN);
        usOffset += HI_METER_ADDR_LEN;
    }

    // AFN
    pBuf[usOffset++] = pstFrame->stAfnData.ucAFN;

    // DT
    mrsToolsMemcpy_s(pBuf + usOffset, usBufLen - usOffset, pstFrame->stAfnData.ucDT, 2);
    usOffset += 2;

    // 用户数据单元
    if (pstFrame->ucIs645)
    {
        pBuf[usOffset++] = pstFrame->ucProtocol;
        if (MRS_AFN(0xF1) == pstFrame->stAfnData.ucAFN)
        {
            pBuf[usOffset++] = (HI_U8)(pstFrame->stAfnData.usDataLen & 0xff);
            pBuf[usOffset++] = (HI_U8)((pstFrame->stAfnData.usDataLen >> 8) & 0xff);
        }
        else
        {
            pBuf[usOffset++] = (HI_U8)pstFrame->stAfnData.usDataLen;
        }
    }

    if (pstFrame->stAfnData.usDataLen > 0)
    {
        mrsToolsMemcpy_s(pBuf + usOffset, usBufLen - usOffset, pstFrame->stAfnData.pData, pstFrame->stAfnData.usDataLen);
        usOffset += pstFrame->stAfnData.usDataLen;
    }
    
    // CS
    pBuf[usOffset++] = mrsToolsCalcCheckSum(pBuf + 3, usBufLen - 5);

    // 结束字符
    pBuf[usOffset++] = PROTO3762_END_FLG;

    *ppOutBuffer = pBuf;
    *pusBufferLen = usBufLen;

    return HI_ERR_SUCCESS;
}

/*****************************************************************
FunctionName  :  mrsCreate3762UpFrame
Parameters    :  pstAfnData
              ppOutBuf
              pusBufLen
Return        :    
Description   :  

Author        :  c00233580
Creation time :  2013/3/13
*****************************************************************/
HI_U32 mrsCreate3762DownFrame(HI_IN PROTO376_2_FRAME_AFN_AND_DATA_STRU * pstAfnData, 
        HI_OUT HI_PBYTE *ppOutBuf, HI_OUT HI_U16 *pusBufLen, HI_IN HI_U8 ucMeterAddr[], HI_U8 aucMacAdd[HI_PLC_MAC_ADDR_LEN])
{
    PROTO376_2_FRAME_PRE_STRU stFrame;
    MRS_CCO_SRV_CTX_STRU *pCtx = mrsCcoGetContext();

    if (!pstAfnData || !ppOutBuf || !pusBufLen)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    mrsToolsMemZero_s(&stFrame, sizeof(stFrame), sizeof(stFrame));
    stFrame.ucPrm = 1;
    stFrame.ucSeq = ++(pCtx->ucSeq);
    if ((pstAfnData->ucAFN == MRS_CCO_PROTOCOL_AFN_06) && (pstAfnData->ucDT[0] == 2 &&(pstAfnData->ucDT[1] == 0)))
    {
        stFrame.ucRelayLevel = mrsMapGetLevel(ucMeterAddr, aucMacAdd);
    }
    
    mrsToolsMemcpy_s(&stFrame.stAfnData, sizeof(stFrame.stAfnData), pstAfnData, sizeof(PROTO376_2_FRAME_AFN_AND_DATA_STRU));
    
    if (pstAfnData->ucPhaseFlag)
    {
        stFrame.ucPhase = mrsCcoGetStaPhase(pCtx->aucSrcMac);
        pCtx->ucPhase = stFrame.ucPhase;
    }
    
    return mrsCreate3762FrameDown(&stFrame, ppOutBuf, pusBufLen, ucMeterAddr);
}
#endif

#endif // CCO

