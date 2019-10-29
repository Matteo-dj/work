//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2014, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_fw_proto_csg.c
//  �� �� ��   : V1.0 
//  ��    ��   : cuiate/c00233580
//  ��������   : 2015-12-09
//  ��������   : TODO: ...
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2015-12-09
//    ��    �� : cuiate/c00233580
//    �޸����� : �����ļ� 
//
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_fw_proto376_2.h"
#include "mrs_fw_proto_csg.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_fw_proto376_2_frame.h"
#include "mrs_fw_proto_csg_echo.h"
HI_START_HEADER

#if ((defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)) && defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))


HI_U32 MRS_ProtoCsgDec(HI_IN HI_PBYTE pucDatagram, HI_U16 usDatagramSize, HI_OUT MRS_CSG_FRAME_STRU** ppstFrame)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_CSG_FRAME_STRU stFrame;
    HI_PBYTE pBuffer = pucDatagram;

    if (!pucDatagram)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("Invalid Param"));
        return HI_ERR_FAILURE;
    }

    ulRet = mrsToolsCheck3762Frame(pucDatagram, usDatagramSize, MRS_CCO_FRAME_TYPE_CSG);
    if (HI_ERR_SUCCESS != ulRet)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_002, HI_DIAG_MT("[CSG] Frame check Err"));
        return HI_ERR_BAD_DATA;
    }

    (hi_void)memset_s(&stFrame, sizeof(stFrame), 0, sizeof(stFrame));

    pBuffer += MRS_CSG_FRAME_HEAD_SIZE + MRS_CSG_FRAME_LEN_SIZE;
    stFrame.ucDir = (HI_U8)(((*pBuffer) >> 7) & 0x01);
    stFrame.ucPrm = (HI_U8)(((*pBuffer) >> 6) & 0x01);
    stFrame.ucAddrFlag = (HI_U8)(((*pBuffer) >> 5) & 0x01);
    pBuffer++;

    if (PROTOCSG_CTRL_DIR_BIT_UL == stFrame.ucDir)
    {
        // ����֡������
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_003, HI_DIAG_MT("[CSG] Frame dir err"));
        return HI_ERR_BAD_DATA;
    }

    if (usDatagramSize < MRS_CSG_FRAME_MIN_SIZE + HI_METER_ADDR_LEN * 2 * stFrame.ucAddrFlag)
    {
        return HI_ERR_BAD_DATA;
    }

    if (stFrame.ucAddrFlag)
    {
        (hi_void)memcpy_s(stFrame.aucSrcAddr, HI_METER_ADDR_LEN, pBuffer, HI_METER_ADDR_LEN);
        pBuffer += HI_METER_ADDR_LEN;
        (hi_void)memcpy_s(stFrame.aucDstAddr, HI_METER_ADDR_LEN, pBuffer, HI_METER_ADDR_LEN);
        pBuffer += HI_METER_ADDR_LEN;
    }

    stFrame.ucAfn = *pBuffer++;
    stFrame.ucSeq = *pBuffer++;
    (hi_void)memcpy_s(&stFrame.ulDI, sizeof(stFrame.ulDI), pBuffer, sizeof(stFrame.ulDI));
    pBuffer += sizeof(stFrame.ulDI);
    stFrame.usAppDataLen = usDatagramSize - MRS_CSG_FRAME_MIN_SIZE - HI_METER_ADDR_LEN * 2 * stFrame.ucAddrFlag;

    // AFN = DI1
    if (stFrame.ucAfn != (HI_U8)(stFrame.ulDI >> 8))
    {
        return HI_ERR_BAD_DATA;
    }

    *ppstFrame = (MRS_CSG_FRAME_STRU *)mrsToolsMalloc(sizeof(MRS_CSG_FRAME_STRU) + stFrame.usAppDataLen);
    if (*ppstFrame == HI_NULL)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_004, HI_DIAG_MT("[CSG] malloc failed."));
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memcpy_s(*ppstFrame, sizeof(stFrame), &stFrame, sizeof(stFrame));
    if (stFrame.usAppDataLen > 0)
    {
        (hi_void)memcpy_s((*ppstFrame)->aucAppData, stFrame.usAppDataLen, pBuffer, stFrame.usAppDataLen);
    }

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_005, HI_DIAG_MT("[CSG] frame decode success."));
    return HI_ERR_SUCCESS;
}


//*****************************************************************************
// ��������: MRS_ProtoCsgEncode
// ��������: һ�����׵���֡����
//
// ����˵��:
//   pucContent [in/out] ���ݵ�Ԫ������
//
//   usLen [in] ���ݵ�Ԫ����
//
//   param [in/out] ���д������Ĳ���
//
//
// �� �� ֵ:
//
// ����Ҫ��:
// ���þ���:
// ��    ��: niesongsong/nKF62735 [2011-12-17]
//*****************************************************************************
HI_U32 MRS_ProtoCsgEncode(HI_U8 * pucContent, HI_U16 usLen, HI_VOID * param)
{
    AFN_DI_UL_PARAM * pstAfnParam = param;
    MRS_CSG_FRAME_STRU * pstAfnFrame = HI_NULL;
    MRS_CSG_IND_FRAME_STRU stIndFrame = {0};
    
    if ((pstAfnParam == HI_NULL) || (pstAfnParam->pstFrame == HI_NULL) || (pucContent == HI_NULL))
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_006, HI_DIAG_MT("Invalid Param"));
        return HI_ERR_BAD_DATA;
    }

    pstAfnFrame = pstAfnParam->pstFrame;

    (hi_void)memset_s(&stIndFrame, sizeof(stIndFrame), 0, sizeof(MRS_CSG_IND_FRAME_STRU));
   
    stIndFrame.ucAddrFlag = pstAfnFrame->ucAddrFlag;
    stIndFrame.ucPrm = (~pstAfnFrame->ucPrm);
    stIndFrame.ucAfn = pstAfnFrame->ucAfn;
    stIndFrame.ulDI = pstAfnFrame->ulDI;
    stIndFrame.ucSeq = pstAfnFrame->ucSeq;

    if(stIndFrame.ucAddrFlag == 1)
    {
        (hi_void)memcpy_s(stIndFrame.aucSrcAddr, HI_METER_ADDR_LEN, pstAfnFrame->aucDstAddr, HI_METER_ADDR_LEN);
        (hi_void)memcpy_s(stIndFrame.aucDstAddr, HI_METER_ADDR_LEN, pstAfnFrame->aucSrcAddr, HI_METER_ADDR_LEN);
    }
    
    stIndFrame.usAppDataLen = usLen;
    stIndFrame.pAppData = pucContent;

    return mrsCreateCsgFrame(&stIndFrame, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
}



HI_U32 mrsCreateCsgFrame(MRS_CSG_IND_FRAME_STRU *pstFrame, HI_OUT HI_PBYTE *ppOutBuffer, HI_OUT HI_U16 *pusBufferLen)
{
    HI_U16 usBufLen = MRS_CSG_FRAME_MIN_SIZE;
    HI_U16 usOffset = 0;
    HI_U8 *pBuf = HI_NULL;

    if (!pstFrame || !ppOutBuffer || !pusBufferLen)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_007, HI_DIAG_MT("Invalid Param"));
        return HI_ERR_INVALID_PARAMETER;
    }
    
    if (1 == pstFrame->ucAddrFlag)
    {
        // �е�ַ��
        usBufLen += HI_METER_ADDR_LEN * 2;
    }

    usBufLen += pstFrame->usAppDataLen;
    
    pBuf = mrsToolsMalloc(usBufLen);
    if (!pBuf)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_008, HI_DIAG_MT("malloc failed"));
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memset_s(pBuf, usBufLen, 0, usBufLen);

    // ��ʼ�ַ�
    pBuf[usOffset++] = PROTO3762_START_FLG;

    // ����
    (hi_void)memcpy_s(pBuf + usOffset, usBufLen - usOffset, &usBufLen, 2);
    usOffset += 2;

    // ������  !!! ����֡  ����Ҫ��ַ��
    pBuf[usOffset++] = PROTOCSG_CTRL_DIR_UL | ((pstFrame->ucPrm & 1) << 6) | ((pstFrame->ucAddrFlag & 1) << 5);

    // A��ַ��
    if (1 == pstFrame->ucAddrFlag)
    {
        (hi_void)memcpy_s(pBuf + usOffset, usBufLen - usOffset, pstFrame->aucSrcAddr, HI_METER_ADDR_LEN);
        usOffset += HI_METER_ADDR_LEN;
        (hi_void)memcpy_s(pBuf + usOffset, usBufLen - usOffset, pstFrame->aucDstAddr, HI_METER_ADDR_LEN);
        usOffset += HI_METER_ADDR_LEN;
    }

    // AFN
    pBuf[usOffset++] = pstFrame->ucAfn;

    //SEQ
    pBuf[usOffset++] = pstFrame->ucSeq; // ֡���к�
    
    //DI ���ݱ�ʶ����
    (hi_void)memcpy_s(pBuf + usOffset, usBufLen - usOffset, &pstFrame->ulDI, sizeof(pstFrame->ulDI));
    usOffset += sizeof(pstFrame->ulDI);

    if ((pstFrame->usAppDataLen > 0) && (pstFrame->pAppData))
    {
        (hi_void)memcpy_s(pBuf + usOffset, usBufLen - usOffset, pstFrame->pAppData, pstFrame->usAppDataLen);
        usOffset += pstFrame->usAppDataLen;
    }
    
    // CS
    pBuf[usOffset++] = mrsToolsCalcCheckSum(pBuf + 3, usBufLen - 5);

    // �����ַ�
    pBuf[usOffset++] = PROTO3762_END_FLG;

   *ppOutBuffer = pBuf;
   *pusBufferLen = usBufLen;

    return HI_ERR_SUCCESS;
}


#endif

HI_END_HEADER
