//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_fw_proto_645.c
//  �� �� ��   : V1.0 
//  ��    ��   : shenhankun/00130424
//  ��������   : 2011-10-25
//  ��������   : 645Э��
//               
//  �����б�   : 
//  �޸���ʷ   : 
//  1.��    �� : 2011-10-25
//    ��    �� : shenhankun/00130424
//    �޸����� : �����ļ� 
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   : 
// SUBSYSTEM :
// MODULE    :  
// OWNER     :  
//*****************************************************************************
#include "mrs_common.h"
#include "mrs_fw_proto376_2.h"
#include "mrs_fw_proto645.h"
#include "mrs_fw_n.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_parallel_cco.h"
#include "mrs_srv_list.h"
#include "mrs_srv_cco_event.h"
#include "mrs_srv_cco.h"
#include "mrs_cmd_msg.h"
#include "mrs_fw_log.h"
#include "hi_mdm.h"


HI_PRV HI_BOOL mrsJudgeIsCtrlCmd(HI_U8 *);


HI_PRV MRS_645_PROTO_CTX_STRU* g_pst645ProtoCtx[MRS_645BUF_MAX] = {HI_NULL, HI_NULL};
HI_PRV HI_U32 mrs645ProtoBuffer(MRS_645_PROTO_CTX_STRU * pstCtx,MRS_ONE_RAW_FRAME_STRU * pstFrame);


HI_VOID mrs645SetContext(HI_U32 buf_645, HI_U8 buf_id)
{ 
    g_pst645ProtoCtx[buf_id % MRS_645BUF_MAX] = (MRS_645_PROTO_CTX_STRU*)buf_645;
    return;
}

MRS_645_PROTO_CTX_STRU * mrs645GetContext(HI_U8 buf_id)
{
    return g_pst645ProtoCtx[buf_id % MRS_645BUF_MAX];
}


// ��ʼ��
HI_U32 mrs645ProtoInit(HI_IN HI_OUT MRS_645_PROTO_CTX_STRU* pstCtx)
{
    MRS_645_PROTO_CTX_STRU * pst645Ctx = pstCtx;
    
    if (pstCtx == HI_NULL) 
    {
        return HI_ERR_BAD_DATA;
    }

    pst645Ctx->usDagramSize = 0;
    pst645Ctx->usRcvBufferSize = 0;
    pst645Ctx->pucRcvBuffer = mrsToolsMalloc(MRS_PROTO_MR_DATAGRAM_BUF_MAX);
    
    if (pst645Ctx->pucRcvBuffer == HI_NULL) 
    {
        return HI_ERR_MALLOC_FAILUE;
    }
    
    (hi_void)memset_s(pst645Ctx->pucRcvBuffer, MRS_PROTO_MR_DATAGRAM_BUF_MAX, 0, MRS_PROTO_MR_DATAGRAM_BUF_MAX);

    return HI_ERR_SUCCESS;
}

HI_U32 mrs645ProtoBuffer(MRS_645_PROTO_CTX_STRU * pstCtx,MRS_ONE_RAW_FRAME_STRU * pstFrame)
{
    HI_PBYTE pucInputBuffer = pstCtx->pucRcvBuffer;
    HI_U16   usInputLen = pstCtx->usRcvBufferSize;   
    HI_U32 ret = HI_ERR_SUCCESS;
    
    while (usInputLen > 0)
    {
        HI_PBYTE pucFrameHead;
        pucFrameHead = (HI_PBYTE)mrsToolsStreamFindByte(pucInputBuffer,usInputLen,0x68);
        
        if (pucFrameHead != HI_NULL) 
        {//�ҵ�֡
            HI_U16 usFrameLen = 0;
            HI_U8 ucFrameLen = 0;
            
            usInputLen = (HI_U16)((usInputLen) - (pucFrameHead - pucInputBuffer));
            pucInputBuffer = pucFrameHead;
            
            if (usInputLen < 12) // ���С����С֡��-�ƶ�֡ͷ��������ͷ-��������
            {//��������
                ret = HI_ERR_CONTINUE;
                int ret1 = 0;
                ret1 = memmove_s(pstCtx->pucRcvBuffer, MRS_PROTO_MR_DATAGRAM_BUF_MAX, pucInputBuffer, usInputLen);
                pstCtx->usRcvBufferSize = usInputLen;
                if(ret1 != 0){
                    break;
                }
                break;
            }
            
            if (pucInputBuffer[7] != 0x68) // �ڶ���֡��־
            {
                pucInputBuffer ++;
                usInputLen --;
                HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_002, HI_DIAG_MT("not find 2nd 0x68"));
                continue;
            }

            usFrameLen = (HI_U16)(pucInputBuffer[9] + 12);

            //����֡
            if (usFrameLen > MRS_PROTO645_DATAGRAM_LEN_MAX) 
            {
                pucInputBuffer ++;
                usInputLen --;
                continue;
            }
            ucFrameLen = (HI_U8)usFrameLen;
            
            //������
            if (usInputLen >= ucFrameLen)
            {
                if (mrsToolsCheck645Frame(pucInputBuffer,ucFrameLen) == HI_ERR_SUCCESS) 
                {//�ɹ���645֡
                    HI_PBYTE bf645 = mrsToolsMalloc(ucFrameLen);
                    if (bf645 == HI_NULL)
                    {
                        pstCtx->usRcvBufferSize = 0;
                        return HI_ERR_MALLOC_FAILUE;
                    } 

                    pstFrame->pucDatagram = bf645;
                    pstFrame->usDatagramSize = ucFrameLen;
                    (hi_void)memcpy_s(bf645, ucFrameLen, pucInputBuffer, ucFrameLen);                    
                    pstCtx->usRcvBufferSize = 0;
                    return HI_ERR_SUCCESS;
                }
                else
                {//����
                    pucInputBuffer++;
                    usInputLen--;
                    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_005, HI_DIAG_MT("frame err"));
                }                
            } 
            else
            {
                int ret1 = 0;
                ret1 = memmove_s(pstCtx->pucRcvBuffer, MRS_PROTO_MR_DATAGRAM_BUF_MAX, pucInputBuffer, usInputLen);
                pstCtx->usRcvBufferSize = usInputLen;
                if(ret1 != 0){
                    return HI_ERR_CONTINUE;
                }
                return HI_ERR_CONTINUE;
            }
        }
        else 
        {//û���ҵ�֡ͷ
            pstCtx->usRcvBufferSize = 0;
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_006, HI_DIAG_MT("not find 0x68"));
            return HI_ERR_BAD_DATA;
        }
    }
    return ret;
}

//*****************************************************************************
// ��������: mrs645ProtoStream2Buffer
// ��������: ��������ַ�������ֶ�����
//           
// ����˵��:
//   pucStream [in] �����ַ���
//   usStreamLen [in] �����ַ�����С
//   ucBufId [in] ������ID
//
// �� �� ֵ: 
//    HI_ERR_SUCCESS : �ɹ���ȡ������645֡ 
//    HI_ERR_CONTINUE : δ���ҵ��κε�����֡,��Ҫ�����ȴ�����
//    ����Ϊ������ 
//
// ���þ���: TODO: ...
// ��    ��: cuiate/00233580 [2017-07-06]
//*****************************************************************************
HI_U32 mrs645ProtoStream2Buffer(HI_PBYTE pucStream, HI_U16 usStreamLen, HI_U8 ucBufId)
{
    MRS_645_PROTO_CTX_STRU * pst645Ctx = mrs645GetContext(ucBufId);

    // �жϻ������Ƿ�����
    if ((usStreamLen + pst645Ctx->usRcvBufferSize) > MRS_PROTO_MR_DATAGRAM_BUF_MAX)
    {
        mrs645ProtoStreamInputCheck(HI_TRUE, ucBufId);
        return HI_ERR_BAD_DATA;
    }

    (hi_void)memcpy_s(pst645Ctx->pucRcvBuffer + pst645Ctx->usRcvBufferSize,MRS_PROTO_MR_DATAGRAM_BUF_MAX-pst645Ctx->usRcvBufferSize, pucStream, usStreamLen);
    pst645Ctx->usRcvBufferSize = (HI_U16)(pst645Ctx->usRcvBufferSize + usStreamLen);

    return HI_ERR_SUCCESS;
}
//*****************************************************************************
// ��������: mrs645ProtoStreamInput
// ��������: ��������ַ�������ֶ�����, �ж�֡�����Բ����������645֡
//           
// ����˵��:
//   pucStream [in] �����ַ���
//   usStreamLen [in] �����ַ�����С
//   pstFrame [out] ������ɸ�����645֡, ʹ���߲����ͷ�pstFrame.pucDatagram��ָ��.
//
// �� �� ֵ: 
//    HI_ERR_SUCCESS : �ɹ���ȡ������645֡ 
//    HI_ERR_CONTINUE : δ���ҵ��κε�����֡,��Ҫ�����ȴ�����
//
// ����Ҫ��: ʹ���߲����ͷ�pstFrame.pucDatagram��ָ��.
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-29]
//*****************************************************************************
HI_U32 mrs645ProtoStreamInput(HI_PBYTE pucStream, HI_U16 usStreamLen,
                                        HI_OUT MRS_ONE_RAW_FRAME_STRU* pstFrame, HI_U8 buf_id)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

    ulRet = mrs645ProtoStream2Buffer(pucStream, usStreamLen, buf_id);
    if (HI_ERR_SUCCESS == ulRet)
    {
        MRS_645_PROTO_CTX_STRU * pst645Ctx = mrs645GetContext(buf_id);
        ulRet = mrs645ProtoBuffer(pst645Ctx, pstFrame);
    }

    return ulRet;
}


HI_U32 mrs645ProtoStreamInputCheck(HI_BOOL bDiscardOldStream, HI_U8 buf_id)
{
    MRS_645_PROTO_CTX_STRU * pst645Ctx = HI_NULL;
    pst645Ctx = mrs645GetContext(buf_id);

    if(pst645Ctx)
    {
        if(bDiscardOldStream)
        {
            pst645Ctx->usRcvBufferSize = 0;
        }
    }    
    
    return HI_ERR_SUCCESS;
}


#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)    
HI_U32 MRS_Proto645Enc( HI_OUT HI_U8 ucDatagram[MRS_PROTO645_DATAGRAM_LEN_MAX], 
                       HI_OUT HI_U16* pusDatagramSize, HI_IN MRS_645_FRAME_STRU* pstFrame )
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_PBYTE pOutBuffer = (HI_PBYTE)ucDatagram;
    HI_U8    ucDataRealmLen = 0;

    if ((pusDatagramSize == HI_NULL) || (pstFrame == HI_NULL)) 
    {
        return HI_ERR_BAD_DATA;
    }
    
    //����
    ucDataRealmLen = pstFrame->ucDataRealmLen;
    
    //֡ͷ
    *pOutBuffer++ = 0x68;

    //��ַ��6���ֽ�
    (hi_void)memcpy_s(pOutBuffer, HI_METER_ADDR_LEN, pstFrame->ucAddr, HI_METER_ADDR_LEN);
    pOutBuffer += HI_METER_ADDR_LEN;

    //֡ͷ
    *pOutBuffer++ = 0x68;
    
    //������
    *pOutBuffer++ = (HI_U8)(((pstFrame->stCtrl.ucDir & 0x01) << 7)
                         + ((pstFrame->stCtrl.ucSlaveFlag & 0x01) << 6)
                         + ((pstFrame->stCtrl.ucFrameFlag & 0x01) << 5)
                         + ((pstFrame->stCtrl.ucFn & 0x1F)));
    //����
    *pOutBuffer++ = ucDataRealmLen;
    *pusDatagramSize = (HI_U16)(12 + ucDataRealmLen);

    //������
    if (ucDataRealmLen != 0) 
    {
        (hi_void)memcpy_s(pOutBuffer, ucDataRealmLen, pstFrame->ucDataRealm, ucDataRealmLen);
        pOutBuffer += ucDataRealmLen;
    }

    if ((*pusDatagramSize) > MRS_PROTO645_DATAGRAM_LEN_MAX)
    {
        return HI_ERR_FAILURE;
    }
    //У����
    *pOutBuffer++ = mrsToolsCalcCheckSum(ucDatagram,(HI_U16)((*pusDatagramSize) - 2));

    //֡β
    *pOutBuffer++ = 0x16;

    return ret;    
}
#endif

HI_U32 mrsGen645Frame(HI_IN MRS_PROTO645_FRAME_INF *pFrameInf, HI_OUT HI_U8 **ppFrame, HI_OUT HI_U16 *pFrameLen)
{
    HI_U16 usBufferLength = MRS_645_FRAME_LENGTH_MIN + pFrameInf->ucDiSize + pFrameInf->ucDataLength;
    HI_U8 *pBuffer = HI_NULL;
    HI_U16 usOffset = 0;

    pBuffer = mrsToolsMalloc(usBufferLength);
    if (!pBuffer)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pBuffer, usBufferLength, 0, usBufferLength);

    
    // HEAD
    pBuffer[usOffset++] = MRS_645_FRAME_START_FLG;

    // ADDR
    (hi_void)memcpy_s(pBuffer + usOffset, usBufferLength - usOffset, pFrameInf->aucAddr, HI_METER_ADDR_LEN);
    usOffset += HI_METER_ADDR_LEN;

    // HEAD 2
    pBuffer[usOffset++] = MRS_645_FRAME_START_FLG;

    // CTRL
    pBuffer[usOffset++] = pFrameInf->ucCtrl;

    // LENGTH
    pBuffer[usOffset++] = pFrameInf->ucDiSize + pFrameInf->ucDataLength;

    // DI
    if (pFrameInf->ucDiSize > 0)
    {
        (hi_void)memcpy_s(pBuffer + usOffset, usBufferLength - usOffset, &(pFrameInf->ulDi), pFrameInf->ucDiSize);
        mrs645DataEncode(pBuffer + usOffset, pFrameInf->ucDiSize);
        usOffset += pFrameInf->ucDiSize;
    }

    // DATA
    if (pFrameInf->ucDataLength > 0)
    {
        (hi_void)memcpy_s(pBuffer + usOffset, usBufferLength - usOffset, pFrameInf->pData, pFrameInf->ucDataLength);
        mrs645DataEncode(pBuffer + usOffset, pFrameInf->ucDataLength);
        usOffset += pFrameInf->ucDataLength;
    }

    // CS
    pBuffer[usOffset] = mrsToolsCalcCheckSum(pBuffer, usOffset);
    usOffset++;

    // END
    pBuffer[usOffset++] = MRS_645_FRAME_END_FLG;

    *ppFrame = pBuffer;
    *pFrameLen = usBufferLength;

    return HI_ERR_SUCCESS;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
// ���������ĳЩ�����ʱ�䳤������ʱʱ��ı�־
HI_VOID mrsCtrlCmdProc(HI_U8 *pDataBuff, HI_U16 usDataLen)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    PLC_OVERTIME_INF_STRU * TimeOutInf = mrsGetPlcOverTimeInfAfn13();
    HI_U16 pos = 0;
    HI_U16 f_len = 0;

    if (TimeOutInf == HI_NULL)
    {
        return;
    }

    ret = mrsFind645Frame(pDataBuff, (HI_S16)usDataLen, &pos, &f_len);    
    if (ret != HI_ERR_SUCCESS)
    {
        TimeOutInf->IsCtrlCmdFlg = 0;
        return;
    }

    TimeOutInf->IsCtrlCmdFlg = MRS_645_IS_CTRL_CMD(pDataBuff[pos + MRS_645_FRAME_CTRL_OFFSET]);
    
    return;
}
#endif


HI_U32 MRS_Proto645Dec( HI_IN HI_PBYTE pucDatagram, HI_U16 usDatagramSize, HI_OUT MRS_645_FRAME_STRU* pstFrame )
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_PBYTE pInBuffer = HI_NULL;
    HI_U8    ucDataRealmLen = 0;
    HI_U16 pos = 0;
    HI_U16 frame_len = 0;

    if ((pucDatagram == HI_NULL) || (pstFrame == HI_NULL)) 
    {
        return HI_ERR_BAD_DATA;
    }

    ret = mrsFind645Frame(pucDatagram, (HI_S16)usDatagramSize, &pos, &frame_len);
    if (ret != HI_ERR_SUCCESS) 
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_009, HI_DIAG_MT("frame 645 err"));
        return HI_ERR_BAD_DATA;
    }
    pInBuffer = pucDatagram + pos;

    //��ַ��6���ֽ�
    (hi_void)memcpy_s(pstFrame->ucAddr, sizeof(pstFrame->ucAddr), (HI_PBYTE)pInBuffer + 1, HI_METER_ADDR_LEN);

    //������
    pstFrame->stCtrl.ucDir = (HI_U8)((pInBuffer[8] & 0x80) >> 7);
    pstFrame->stCtrl.ucSlaveFlag = (HI_U8)((pInBuffer[8] & 0x40) >> 6);
    pstFrame->stCtrl.ucFrameFlag = (HI_U8)((pInBuffer[8] & 0x20) >> 5);
    pstFrame->stCtrl.ucFn = (HI_U8)(pInBuffer[8] & 0x1F);
        
    //����
    pInBuffer += 9;
    pstFrame->ucDataRealmLen = *pInBuffer++;
    ucDataRealmLen = pstFrame->ucDataRealmLen;

    if (ucDataRealmLen > sizeof(pstFrame->ucDataRealm))
    {
        return HI_ERR_BAD_DATA;
    }
    
    //Ӧ�ó�������
    if (ucDataRealmLen != 0) 
    {
        (hi_void)memcpy_s(pstFrame->ucDataRealm, sizeof(pstFrame->ucDataRealm), pInBuffer, ucDataRealmLen);
    }

    //��ȡ�汾��
    pstFrame->ucVer = mrsToolsGet645Ver(pstFrame->stCtrl.ucFn);

    return ret;
}


// �ڻ������в�����Ч��645֡�����֡ͷƫ�Ƽ�֡��
HI_U32 mrsFind645Frame(HI_IN HI_PBYTE in_buf, HI_IN HI_S16 in_len, 
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
        {//�ҵ�֡
            HI_U16 frame_len = 0;
            
            in_len -= (HI_S16)(head - buf);
            buf = head;
            
            if (in_len < 12) 
            {
                HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_012, HI_DIAG_MT("buflen not enough, waiting [%d]"), (HI_U16)in_len);
                ret = HI_ERR_BAD_DATA;
                break;
            }

            frame_len = (HI_U16)(buf[9] + 12);

            //����֡
            if (frame_len > MRS_PROTO645_DATAGRAM_LEN_MAX) 
            {
                buf++;
                in_len--;
                HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_013, HI_DIAG_MT("frame too long"));
                continue;
            }

            if(in_len >= frame_len)
            {
                if(HI_ERR_SUCCESS == mrsToolsCheck645Frame(buf,frame_len))
                {
                    *start = (HI_U16)(buf - in_buf);
                    *out_len = frame_len;

                    ret = HI_ERR_SUCCESS;
                    break;
                }
                else
                {
                    buf++;
                    in_len--;
                    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_014, HI_DIAG_MT("frame chk err, continue!"));
                    continue;
                }
            }
            else
            {
                buf++;
                in_len--;
                HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_015, HI_DIAG_MT("buflen not enough, continue [%d]-[%d]"), (HI_U16)in_len, frame_len);
                continue;
            }
                   
        }
        else 
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_016, HI_DIAG_MT("not find 0x68"));
            ret = HI_ERR_BAD_DATA;
            break;
        }
    }

    return ret;
}


// �жϻ��������Ƿ�Ϊ�ѿ�����-(Ҫ�����Ϊ����645֡)
HI_BOOL mrsJudgeIsCtrlCmd(HI_U8 *pData645)
{
    if(pData645)
    {
        HI_U8 ucCtrlWord = 0;

        ucCtrlWord = pData645[MRS_645_FRAME_CTRL_OFFSET];
        
        if (((ucCtrlWord >= MRS_645_FRAME_CONTROL_WRITE_DATA) && (ucCtrlWord <= MRS_645_FRAME_CONTROL_WRITE_MAX))
             || (ucCtrlWord == MRS_645_FRAME_CONTROL_SAFE))
        {
            return HI_TRUE;
        }
    }

    return HI_FALSE;
}


// ͳ�ƻ�������645֡��Ϣ
HI_VOID mrsCountBuf645Inf(HI_IN HI_PBYTE buf, HI_IN HI_U16 len, HI_OUT MRS_645BUF_INF *inf)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_PBYTE tmpbuf = buf;
    HI_S16 tmplen = (HI_S16)len;
    HI_U16 pos = 0;
    HI_U16 frame_len = 0;
    
    if((!buf)||(!inf))
    {
        return;
    }

    (hi_void)memset_s(inf, sizeof(MRS_645BUF_INF), 0, sizeof(MRS_645BUF_INF));
    
    while(tmplen > 0)
    {
        ret = mrsFind645Frame(tmpbuf, tmplen, &pos, &frame_len);
        if(HI_ERR_SUCCESS == ret)
        {
            if(inf->pucHead == HI_NULL)
            {
                inf->pucHead = tmpbuf + pos;
                inf->pucAddr = &inf->pucHead[MRS_645_FRAME_METERADD_OFFSET];
            }

            inf->ucNum++;
            inf->bCtrlCmd |= mrsJudgeIsCtrlCmd(tmpbuf + pos);
            
            tmpbuf += (pos + frame_len);
            tmplen -= (HI_S16)(pos + frame_len);
        }
        else
        {
            break;
        }
    }

    inf->ulCrc = HI_MDM_Crc32(0, buf, len);

    return;
}


HI_VOID mrs645DataDecode(HI_U8 * buf, HI_U32 len)
{
    while(len--)
    {
        buf[len] -= MRS_645_FRAME_HEX33;
    }
    return;    
}


HI_VOID mrs645DataEncode(HI_U8 * buf, HI_U32 len)
{
    while(len--)
    {
        buf[len] += MRS_645_FRAME_HEX33;
    }
    return;    
}


// ����ַԤ����: ����֡97���ַ��λ�ֽ�AAH->00H
HI_VOID mrsMeterAddrULPreProc(HI_OUT HI_U8 *aucDstAddr, HI_IN HI_U8 *aucSrcAddr, HI_IN HI_U8 ucProtocol, HI_IN HI_U8 ucCtrl)
{
    if (ucCtrl & MRS_645_FRAME_CONTROL_DIR_UP)
    {
        mrsMeterAddrPreProc(aucDstAddr, aucSrcAddr, ucProtocol);
    }
    else
    {
        (hi_void)memcpy_s(aucDstAddr, HI_METER_ADDR_LEN, aucSrcAddr, HI_METER_ADDR_LEN);
    }
}

// ����ַԤ����: 97���ַ��λ�ֽ�AAH->00H
HI_VOID mrsMeterAddrPreProc(HI_OUT HI_U8 *aucDstAddr, HI_IN HI_U8 *aucSrcAddr, HI_IN HI_U8 ucProtocol)
{
    if (METER_PROTO_645_1997 != ucProtocol)
    {
        (hi_void)memcpy_s(aucDstAddr, HI_METER_ADDR_LEN, aucSrcAddr, HI_METER_ADDR_LEN);
    }
    else
    {
        HI_S32 i;

        for (i = HI_METER_ADDR_LEN - 1; i >= 0; i--)
        {
            if (aucSrcAddr[i] == MRS_645_METER_ADDR_WILDCARDS)
            {
                aucDstAddr[i] = 0;
                continue;
            }

            break;
        }

        for (; i >= 0; i--)
        {
            aucDstAddr[i] = aucSrcAddr[i];
        }
    }
}


// ��ַpDstAddr�Ƿ���pSrcAddrƥ��(ͨ���0xAA)
// ȥ��pSrcAddr��λ�ֽڵ�AAH֮���õ�λ�ֽں�pDstAddr�Ƚ�(����עpDstAddr��λ�ֽڵ�AAH)
// ����698֮�󣬰��հ��ֽڱȽ�
HI_BOOL mrsMeterAddrMatch(HI_U8 *pSrcAddr, HI_U8 *pDstAddr)
{
    HI_S8 i = 0;
    HI_S8 sAddrLength = HI_METER_ADDR_LEN * 2;

    for (i = sAddrLength - 1; i >= 0; i--)
    {
        if (((pSrcAddr[i/2] >> (hi_u8)(4 * (hi_u8)(i % 2))) & 0xF) != 0xA)
        {
            break;
        }
    }

    for (; i >= 0; i--)
    {
        if (((pSrcAddr[i/2] >> (hi_u8)(4 * (hi_u8)(i % 2))) & 0xF) != ((pDstAddr[i/2] >> (HI_U8)(4 * (i % 2))) & 0xF))
        {
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}



#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
HI_U32 mrsIsSameAsBufferBbu(HI_U16 usDatagramSize, HI_U8 *pucDatagram, HI_U8 buf_id)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_645_PROTO_CTX_STRU * pst645Ctx = mrs645GetContext(buf_id);

    if ((usDatagramSize > MRS_PROTO_MR_DATAGRAM_BUF_MAX) 
        || !pst645Ctx
        || !(pst645Ctx->pucRcvBuffer)
        || !pucDatagram)
    {
        return HI_ERR_INVALID_PARAMETER;
    }


    if (pst645Ctx->usRcvBufferSize != usDatagramSize)
    {
        ret = HI_ERR_FAILURE;
    }
    else if (0 != memcmp(pucDatagram, pst645Ctx->pucRcvBuffer, usDatagramSize))
    {
        ret = HI_ERR_FAILURE;
    }
    return ret;
}

HI_U32 mrsAddFrameToBufferBbu(HI_U16 usDatagramSize, HI_U8 *pucDatagram, HI_U8 buf_id)
{
    MRS_645_PROTO_CTX_STRU * pst645Ctx = mrs645GetContext(buf_id);

    if ((usDatagramSize > MRS_PROTO_MR_DATAGRAM_BUF_MAX) 
        || !pst645Ctx
        || !(pst645Ctx->pucRcvBuffer)
        || !pucDatagram)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    mrsClear645ProtoBufferBbu(buf_id);    
    // ���հ汾����Ϊҵ�񴮿�ʱ����������֡�뵱ǰ��ȡ����֡��һ�£���������µ�֡��������
    (hi_void)memcpy_s(pst645Ctx->pucRcvBuffer, MRS_PROTO_MR_DATAGRAM_BUF_MAX, pucDatagram, usDatagramSize);
    pst645Ctx->usRcvBufferSize = usDatagramSize;
    return HI_ERR_SUCCESS;
}

HI_VOID mrsClear645ProtoBufferBbu(HI_U8 buf_id)
{
    MRS_645_PROTO_CTX_STRU * pst645Ctx = mrs645GetContext(buf_id);
    if (pst645Ctx && pst645Ctx->pucRcvBuffer)
    {
        pst645Ctx->usRcvBufferSize = 0;
        (hi_void)memset_s(pst645Ctx->pucRcvBuffer, MRS_PROTO_MR_DATAGRAM_BUF_MAX, 0, MRS_PROTO_MR_DATAGRAM_BUF_MAX);
    }
}
#endif


HI_U32 mrsGetFrame645MeterAddr(HI_U8 *pucFrame, HI_U16 usFrameLength, HI_U8 *pucMeterAddr)
{
    HI_U16 usPos = 0;
    HI_U16 usLength = 0;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    ulRet = mrsFind645Frame(pucFrame, (HI_S16)usFrameLength, &usPos, &usLength);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memcpy_s(pucMeterAddr, HI_METER_ADDR_LEN, pucFrame + usPos + MRS_645_FRAME_METERADD_OFFSET, HI_METER_ADDR_LEN);

    return HI_ERR_SUCCESS;
}


