//*****************************************************************************
//
//                  版权所有 (C), 2017-2027, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_fw_proto698_45.c
//  版 本 号   : V1.0 
//  作    者   : Cui Ate/c00233580
//  生成日期   : 2017-06-28
//  功能描述   : TODO: ...
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2017-06-28
//    作    者 : Cui Ate/c00233580
//    修改内容 : 创建文件 
//
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_fw_proto645.h"
#include "mrs_fw_proto698_45.h"
#include "mrs_fw_tools.h"
HI_CPP_START


#define TABLE_SIZE      (256)
#define CRC_BITS        (8)
#define MASK_LE         (0x0001)
#define CRC_16_POLY     (0x8408)    // x^16 + x^12 + x^5 + 1

HI_U16 g_ausCrc16Tab[TABLE_SIZE] = {0};


HI_VOID mrs698Crc16Init(HI_VOID)
{
    HI_REG HI_U16 i, j, usCrc;

    for (i = 0; i < TABLE_SIZE; i++)
    {
        usCrc = i;

        for (j = 0; j < CRC_BITS; j++)
        {
            usCrc = (usCrc >> 1) ^ ((usCrc & MASK_LE) ? CRC_16_POLY : 0);
        }

        g_ausCrc16Tab[i] = usCrc;
    }
}


HI_U16 mrs698Crc16(HI_REG HI_U16 usInitCrc, HI_REG HI_U8 * pBuf, HI_REG HI_S32 sLength)
{
    while (sLength--)
    {
        usInitCrc = (usInitCrc >> 8) ^ g_ausCrc16Tab[(usInitCrc ^ *pBuf++) & 0xFF];
    }

    usInitCrc ^= 0xFFFF;

    return usInitCrc;
}


HI_U32 mrsCheck698Frame(HI_PBYTE pucFrame, HI_U16 usLength)
{
    HI_U16 usFrameLength = 0;
    HI_U16 usHeadLength = MRS_698_FIXED_HEAD_LEN;
    HI_U16 usHCS = 0, usFCS = 0;
    HI_U16 usCalcCS = 0;

    if (!pucFrame || MRS_698_FRAME_MIN_LEN > usLength)
    {
        return HI_ERR_BAD_DATA;
    }

    usFrameLength = MRS_COMBIN16(pucFrame[1], pucFrame[2]);
    usFrameLength += 2; // head(68) + tail(16)
    if ((usFrameLength != usLength) || (MRS_FRAME_HEAD != pucFrame[0]) || (MRS_FRAME_TAIL != pucFrame[usLength - 1]))
    {
        return HI_ERR_BAD_DATA;
    }

    usHeadLength += (pucFrame[4] & 0xF) + 1;
    usHCS = MRS_COMBIN16(pucFrame[usHeadLength + 1], pucFrame[usHeadLength + 2]);
    usCalcCS = mrs698Crc16(MRS_698_CRC_INIT, pucFrame + 1, usHeadLength);
    if (usHCS != usCalcCS)
    {
        return HI_ERR_BAD_DATA;
    }

    usFCS = MRS_COMBIN16(pucFrame[usFrameLength - 3], pucFrame[usFrameLength - 2]);
    usCalcCS = mrs698Crc16(MRS_698_CRC_INIT, pucFrame + 1, usFrameLength - 4);
    if (usFCS != usCalcCS)
    {
        return HI_ERR_BAD_DATA;
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsFind698Frame(HI_PBYTE pucInBuffer, HI_U16 usLength, HI_U16 *pusStart, HI_U16 *pusFrameLength)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_PBYTE pucBuffer = pucInBuffer;
    HI_S32 nDataLength = usLength;

    if (!pucBuffer || !pusStart || !pusFrameLength)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    while (nDataLength > 0)
    {
        HI_PBYTE pucFrmHead = (HI_PBYTE)mrsToolsStreamFindByte(pucBuffer, (HI_U16)nDataLength, MRS_FRAME_HEAD);
        HI_U16 usFrameLength = 0;

        if (!pucFrmHead)
        {
            return HI_ERR_BAD_DATA;
        }

        nDataLength -= (HI_S32)(pucFrmHead - pucBuffer);
        pucBuffer = pucFrmHead;

        if (nDataLength < MRS_698_FRAME_MIN_LEN)
        {
            return HI_ERR_BAD_DATA;
        }

        usFrameLength = MRS_COMBIN16(pucBuffer[1], pucBuffer[2]);
        usFrameLength += 2; // head(68) + tail(16)
        if ((HI_S32)usFrameLength > nDataLength)
        {
            pucBuffer++;
            nDataLength--;
            continue;
        }

        ulRet = mrsCheck698Frame(pucBuffer, usFrameLength);
        if (HI_ERR_SUCCESS != ulRet)
        {
            pucBuffer++;
            nDataLength--;
            continue;
        }

        *pusStart = (HI_U16)(pucBuffer - pucInBuffer);
        *pusFrameLength = usFrameLength;

        return HI_ERR_SUCCESS;
    }

    return HI_ERR_NOT_FOUND;
}


HI_U32 mrsFind698MeterAddr(HI_PBYTE pucBuffer, HI_U16 usLength, HI_U8 *pucMeterAddr)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U16 usStartPos = 0;
    HI_U16 usFrmLength = 0;
    HI_U8 ucAddrLen = 0;
    HI_U8 ucDstStart = 0;
    HI_U8 ucSrcStart = 0;
    HI_U8 *pucFrame = HI_NULL;

    if (!pucMeterAddr)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    ulRet = mrsFind698Frame(pucBuffer, usLength, &usStartPos, &usFrmLength);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    pucFrame = pucBuffer + usStartPos;
    ucAddrLen = (pucFrame[MRS_698_FRAME_SA_OFFSET] & 0x0F) + 1;
    if ((pucFrame[MRS_698_FRAME_ADDR_OFFSET] & 0x0F) == 0x0F)
    {
        ucSrcStart = 1;
    }

    (hi_void)memset_s(pucMeterAddr, HI_METER_ADDR_LEN, 0, HI_METER_ADDR_LEN);
    for (; (ucDstStart < HI_METER_ADDR_LEN * 2) && (ucSrcStart < ucAddrLen * 2); ucDstStart++, ucSrcStart++)
    {
        pucMeterAddr[ucDstStart/2] |= (HI_U8)((pucFrame[5 + ucSrcStart/2] >> (4 * (ucSrcStart % 2))) << (4 * (ucDstStart % 2)));
    }

    return HI_ERR_SUCCESS;
}

HI_U32 mrsFind698Dir(HI_IN HI_PBYTE buf, HI_IN HI_U16 len, HI_OUT HI_U8 *dir)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U16 start_pos = 0;
    HI_U16 frame_len = 0;
    HI_U8 *frame = HI_NULL;    

    if (HI_NULL == dir)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    ret = mrsFind698Frame(buf, len, &start_pos, &frame_len);
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }
    frame = buf + start_pos;
    *dir = (HI_U8)((frame[MRS_698_FRAME_CTRL_OFFSET] & 0x80) >> 7);

    return HI_ERR_SUCCESS;
}

HI_CPP_END
