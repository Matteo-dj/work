
#include "mrs_common.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_proto188.h"

HI_CPP_START
#if defined(MRS_SRV_PROTOCOL_CJT188)

HI_U32 mrsToolsCheck188Frame(HI_PBYTE ptr,HI_U16 len)
{
    HI_U8 checkSum;

    //帧长检测
    if ((!ptr) || 
        (len > MRS_PROTO188_DATAGRAM_LEN_MAX) || 
        (len < MRS_PROTO188_DATAGRAM_HEADER_LEN_MAX))
    {
        return HI_ERR_BAD_DATA;
    }
    
    //帧标志检查
    if ((ptr[0] != 0x68) || (ptr[len-1] != 0x16)) 
    {
        return HI_ERR_BAD_DATA;
    }
        //校验和检查
    checkSum = mrsToolsCalcCheckSum(ptr,(HI_U16)(len-2));
    if (checkSum != ptr[len - 2])
    {
        return HI_ERR_BAD_DATA;
    }
    
    return HI_ERR_SUCCESS;
}

HI_U32 mrsFind188Frame(HI_IN HI_PBYTE in_buf, HI_IN HI_S16 in_len, 
                            HI_OUT HI_U16 *start, HI_OUT HI_U16 *out_len)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_PBYTE buf = in_buf;

    if((!in_buf) || (!start) || (!out_len))
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    while (in_len > 0)
    {
        HI_PBYTE head = HI_NULL;
        head = (HI_PBYTE)mrsToolsStreamFindByte(buf,(HI_U16)in_len,0x68);
        
        if (head != HI_NULL) 
        {//找到帧
            HI_U16 frame_len = 0;
            
            in_len -= (HI_S16)(head - buf);
            buf = head;
            
            if (in_len < 13) //cjt188 min is 13.
            {
                ret = HI_ERR_BAD_DATA;
                break;
            }

            frame_len = (HI_U16)(buf[10] + 13);

            //超长帧
            if (frame_len > MRS_PROTO188_DATAGRAM_LEN_MAX) 
            {
                buf++;
                in_len--;
                continue;
            }

            if(in_len >= frame_len)
            {
                if(HI_ERR_SUCCESS == mrsToolsCheck188Frame(buf,frame_len))
                {
                    *start = (HI_U16)(buf - in_buf);
                    *out_len = frame_len;

                    //mrsProtoPrint0(MRS_LOG_INFO,"找到了一个完整645帧!");
                    ret = HI_ERR_SUCCESS;
                    break;
                }
                else
                {
                    buf++;
                    in_len--;
                    continue;
                }
            }
            else
            {
                buf++;
                in_len--;
                continue;
            }
                   
        }
        else 
        {
            ret = HI_ERR_BAD_DATA;
            break;
        }
    }

    return ret;
}

HI_U32 MRS_Proto188Dec( HI_IN HI_PBYTE pucDatagram, HI_U16 usDatagramSize, HI_OUT MRS_188_FRAME_STRU* pstFrame )
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_PBYTE pInBuffer = pucDatagram;
    HI_U8    ucDataRealmLen = 0;
    HI_U16 pos = 0;
    HI_U16 frame_len = 0;

    if ((pucDatagram == HI_NULL) || (pstFrame == HI_NULL)) 
    {
        return HI_ERR_BAD_DATA;
    }

    ret = mrsFind188Frame(pucDatagram, (HI_S16)usDatagramSize, &pos, &frame_len);
    if (ret != HI_ERR_SUCCESS) 
    {
        return HI_ERR_BAD_DATA;
    }
    pInBuffer = pucDatagram + pos;

	pstFrame->ucType = pInBuffer[1];
    //地址域6个字节
    (hi_void)memcpy_s(pstFrame->ucAddr,sizeof(pstFrame->ucAddr),(HI_PBYTE)pInBuffer + 2,7);

    //控制域
    pstFrame->ucCtrl = pInBuffer[9];
        
    //长度
    pInBuffer += 10;
    pstFrame->ucDataRealmLen = *pInBuffer++;
    ucDataRealmLen = pstFrame->ucDataRealmLen;

    if (ucDataRealmLen > sizeof(pstFrame->ucDataRealm))
    {
        return HI_ERR_BAD_DATA;
    }
    
    //应用程序数据
    if (ucDataRealmLen != 0) 
    {
        (hi_void)memcpy_s(pstFrame->ucDataRealm,MRS_PROTO188_DATAGRAM_DATA_LEN_MAX, pInBuffer, ucDataRealmLen);
    }

    return ret;
}

HI_U32 mrsFind188MeterAddr(HI_PBYTE pucBuffer, HI_U16 usLength, HI_U8 *pucMeterAddr)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U16 usStartPos = 0;
    HI_U16 usFrmLength = 0;
    HI_U8 *pucFrame = HI_NULL;

    if (!pucMeterAddr)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    ulRet = mrsFind188Frame(pucBuffer, usLength, &usStartPos, &usFrmLength);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    pucFrame = pucBuffer + usStartPos;
    mrsToolsMemZero_s(pucMeterAddr,HI_METER_ADDR_LEN, HI_METER_ADDR_LEN);
	mrsToolsMemcpy_s(pucMeterAddr,HI_METER_ADDR_LEN,(HI_PBYTE)pucFrame + 2,HI_METER_ADDR_LEN);
    
    return HI_ERR_SUCCESS;
}

HI_U8 mrsGetT188ProMeterType(HI_PBYTE pucBuffer, HI_U16 usLength)
{
	return pucBuffer[1];
}
HI_U8 *mrsGetT188ProMeterAddress(HI_PBYTE pucBuffer, HI_U16 usLength)
{
	return (pucBuffer+2);
}
HI_U8 *mrsGetT188ProData(HI_PBYTE pucBuffer, HI_U16 usLength)
{
	return (pucBuffer+11);
}
HI_U8 mrsGetT188ProDataLen(HI_PBYTE pucBuffer, HI_U16 usLength)
{
	return *(pucBuffer+10);
}

#endif
HI_CPP_END
