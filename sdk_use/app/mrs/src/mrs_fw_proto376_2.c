//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_fw_proto_376_2.c
//  �� �� ��   : V1.0 
//  ��    ��   : shenhankun/00130424
//  ��������   : 2011-10-25
//  ��������   : 376.2Э��
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
#include "mrs_fw_tools.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_proto376_2_frame.h"
#include "mrs_fw_log.h"

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_NDM))
HI_PRV MRS_3762_PROTO_CTX_STRU* g_pst3762ProtoCtx = HI_NULL;

#define mrs3762SetContext(X) {g_pst3762ProtoCtx=(X);}
#define mrs3762GetContext()  g_pst3762ProtoCtx


HI_VOID mrs3762RealmUlEnc(HI_OUT HI_PBYTE pucOutBuffer, HI_IN MRS_3762_INFO_REALM_UL_STRU * pstInfoRealmUl);
HI_U32 mrs3762ProtoBuffer(HI_IN MRS_3762_PROTO_CTX_STRU * pst3762Ctx, HI_OUT MRS_ONE_RAW_FRAME_STRU** ppstFrame);
HI_VOID mrs3762RealmDlDec(HI_IN HI_PBYTE pInBuffer, HI_OUT MRS_3762_INFO_REALM_DL_STRU * pstInfoRealmDl);


//*****************************************************************************
// ��������: mrs3762ProtoInit
// ��������: 376.2Э�鴦�������ĳ�ʼ��
//           
// ����˵��:
//   pstCtx [in/out] MRS_3762_PROTO_CTX_STRU�ṹ
//
// �� �� ֵ: 
//    HI_ERR_SUCCESS : �ɹ���ȡ������376.2֡ 
//    ����Ϊ������ 
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-29]
//*****************************************************************************
HI_U32 mrs3762ProtoInit(HI_IN HI_OUT MRS_3762_PROTO_CTX_STRU* pstCtx)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    
    if (pstCtx == HI_NULL) 
    {
        return HI_ERR_BAD_DATA;
    }

    pstCtx->usDagramSize = 0;
    pstCtx->usRcvBufferSize = 0;
    pstCtx->pucRcvBuffer = mrsToolsMalloc(PRODUCT_CFG_MRS_3762_BUFFER_BYTE_MAX);
    
    if (pstCtx->pucRcvBuffer == HI_NULL) 
    {
        return HI_ERR_MALLOC_FAILUE;
    }
    
    (hi_void)memset_s(pstCtx->pucRcvBuffer, PRODUCT_CFG_MRS_3762_BUFFER_BYTE_MAX, 0, PRODUCT_CFG_MRS_3762_BUFFER_BYTE_MAX);
    mrs3762SetContext(pstCtx);

    return ret;
}

//*****************************************************************************
// ��������: mrs3762ProtoBuffer
// ��������: TODO: ... 
//           
// ����˵��:
//   pstCtx [in/out] 3762֡�����Ľṹ
//   ppstFrame [out] ���֡
//
// �� �� ֵ: 
//    TODO: ...
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: niesongsong/kf62735 [2011-12-13]
//*****************************************************************************
HI_U32 mrs3762ProtoBuffer(MRS_3762_PROTO_CTX_STRU * pst3762Ctx, MRS_ONE_RAW_FRAME_STRU ** ppstFrame)
{
    HI_PBYTE puc3762InputBuffer = pst3762Ctx->pucRcvBuffer;
    HI_U16   us3762InputLen = pst3762Ctx->usRcvBufferSize;   
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_ONE_RAW_FRAME_STRU * pstTempFrame = HI_NULL;
    
    while (us3762InputLen > 0)
    {
        HI_PBYTE pucFrameHead;
        pucFrameHead = mrsToolsStreamFindByte(puc3762InputBuffer,us3762InputLen,0x68);
        
        if (pucFrameHead != HI_NULL) 
        {//�ҵ�֡
            HI_U16 usFrameLen = 0;
            
            us3762InputLen = (HI_U16)(us3762InputLen - (pucFrameHead - puc3762InputBuffer));
            puc3762InputBuffer = pucFrameHead;
            
            if (us3762InputLen <= 2) 
            {//��������
                ret = HI_ERR_CONTINUE;

                (hi_void)memmove_s(pst3762Ctx->pucRcvBuffer, PRODUCT_CFG_MRS_3762_BUFFER_BYTE_MAX, puc3762InputBuffer, us3762InputLen);
                pst3762Ctx->usRcvBufferSize = us3762InputLen;
                
                break;
            }
            (hi_void)memcpy_s(&usFrameLen, sizeof(usFrameLen), puc3762InputBuffer + 1, sizeof(usFrameLen));
            
            //����֡
            if (usFrameLen > PRODUCT_CFG_MRS_3762_BUFFER_BYTE_MAX) 
            {
                puc3762InputBuffer ++;
                us3762InputLen --;
                continue;
            }
            
            //������
            if (us3762InputLen >= usFrameLen)
            {
                if (mrsToolsCheck3762Frame(puc3762InputBuffer,usFrameLen) == HI_ERR_SUCCESS) 
                {
                    if (pstTempFrame)
                    {
                        mrsToolsFree(pstTempFrame);
                        pstTempFrame = HI_NULL;
                    }

                    //�ɹ���
                    pstTempFrame = mrsToolsMalloc(sizeof(MRS_ONE_RAW_FRAME_STRU) + usFrameLen);
                    if (pstTempFrame)
                    {
                        pstTempFrame->usDatagramSize = usFrameLen;
                        pstTempFrame->pucDatagram = (HI_PBYTE)pstTempFrame + sizeof(MRS_ONE_RAW_FRAME_STRU);
                        (hi_void)memcpy_s(pstTempFrame->pucDatagram, pstTempFrame->usDatagramSize, puc3762InputBuffer, pstTempFrame->usDatagramSize);
                    }

                    puc3762InputBuffer += usFrameLen;
                    us3762InputLen -= usFrameLen;
                    ret = HI_ERR_SUCCESS;

                    if (0 == us3762InputLen)
                    {
                        mrs3762DiscardOldData();
                    }
                }
                else
                {//����
                    puc3762InputBuffer++;
                    us3762InputLen--;
                    ret = HI_ERR_BAD_DATA;
                }                
            } 
            else
            {
                ret = HI_ERR_CONTINUE;
                (hi_void)memmove_s(pst3762Ctx->pucRcvBuffer, PRODUCT_CFG_MRS_3762_BUFFER_BYTE_MAX, puc3762InputBuffer, us3762InputLen);
                pst3762Ctx->usRcvBufferSize = us3762InputLen;
                
                break;
            }
        }
        else 
        {//û���ҵ�֡ͷ
            ret = HI_ERR_BAD_DATA;
            mrs3762DiscardOldData();
            break;
        }
    }

    if (pstTempFrame)
    {
        *ppstFrame = pstTempFrame;
        ret = HI_ERR_SUCCESS;
    }

    return ret;
}

//*****************************************************************************
// ��������: mrs3762ProtoStreamInput
// ��������: ��������ַ����ֶκ�����, �ж�֡�����Բ����������376.2֡
//           
// ����˵��:
//   pucStream [in] �����ַ���
//   usStreamLen [in] �����ַ�����С
//
// �� �� ֵ: 
//    HI_ERR_SUCCESS : �ɹ���ȡ������376.2֡���������Ϣ
//    HI_ERR_CONTINUE : δ���ҵ��κε�����֡,��Ҫ�����ȴ�����
//    ����Ϊ������ 
//
// ����Ҫ��: pstFrame.pucDatagram ����Ҫ�������ͷ��ڴ�ָ��.
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-29]
//*****************************************************************************
HI_U32 mrs3762ProtoStreamInput(HI_IN HI_PBYTE pucStream, HI_IN HI_U16 usStreamLen, HI_OUT MRS_ONE_RAW_FRAME_STRU** ppstFrame)
{
    MRS_3762_PROTO_CTX_STRU * pst3762Ctx = mrs3762GetContext();

    // �жϻ������Ƿ�����
    if ((usStreamLen + pst3762Ctx->usRcvBufferSize) > PRODUCT_CFG_MRS_3762_BUFFER_BYTE_MAX)
    {
        mrs3762DiscardOldData();
    }
    
    //�����ݷ��뻺����
    (hi_void)memcpy_s(pst3762Ctx->pucRcvBuffer + pst3762Ctx->usRcvBufferSize,
        PRODUCT_CFG_MRS_3762_BUFFER_BYTE_MAX - pst3762Ctx->usRcvBufferSize, pucStream, usStreamLen);
    pst3762Ctx->usRcvBufferSize = (HI_U16)(pst3762Ctx->usRcvBufferSize +usStreamLen);

    return mrs3762ProtoBuffer(pst3762Ctx, ppstFrame);
}


HI_U32 mrs3762ProtoStreamInputCheck(HI_BOOL bDiscardOldStream)
{
    if (bDiscardOldStream)
    {
        mrs3762DiscardOldData();
    }
    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// ��������: mrs3762RealmDlDec
// ��������: ����������Ϣ�򵽻�����
//           
// ����˵��:
//   HI_PBYTE pInputBuffer [in] ...
//                             ...
//   MRS_3762_INFO_REALM_DL_STRU * pstInfoRealmDl [in/out] ...
//                                                         ...
//
// �� �� ֵ: 
//    TODO: ...
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: niesongsong/kf62735 [2011-12-13]
//*****************************************************************************
HI_VOID mrs3762RealmDlDec(HI_IN HI_PBYTE pInBuffer, 
                          HI_OUT MRS_3762_INFO_REALM_DL_STRU * pstInfoRealmDl) 
{
    HI_U8 ucURByte1 = pInBuffer[0];
    HI_U8 ucURByte2 = pInBuffer[1];
    HI_U8 ucURByte3 = pInBuffer[2];
    HI_U8 ucURByte4 = pInBuffer[3];
    HI_U8 ucURByte5 = pInBuffer[4];
    HI_U8 ucURByte6 = pInBuffer[5];
    
    pstInfoRealmDl->ucRelayLevel     = (HI_U8)((ucURByte1 >> 4) & 0x0F); // �м̼���-4 
    pstInfoRealmDl->ucClashCheck     = (HI_U8)((ucURByte1 >> 3) & 0x01); // ��ͻ���-1 
    pstInfoRealmDl->ucModuleFlag     = (HI_U8)((ucURByte1 >> 2) & 0x01); // ͨѶģ���ʶ-1 
    pstInfoRealmDl->ucSubsidiaryFlag = (HI_U8)((ucURByte1 >> 1) & 0x01); // �����ڵ��ʶ-1 
    pstInfoRealmDl->ucRouteFlag      = (HI_U8)(ucURByte1 & 0x01);        // ·�ɱ�ʶ-1 
    pstInfoRealmDl->ucCorrErrCode    = (HI_U8)((ucURByte2 >> 4) & 0x0F); // ��������ʶ-4 
    pstInfoRealmDl->ucCHFlag         = (HI_U8)(ucURByte2 & 0x0F);        // �ŵ���ʶ-4 
    pstInfoRealmDl->ucResByte        = ucURByte3;                       // Ԥ��Ӧ���ֽ��� 
    pstInfoRealmDl->usSpeedUnit = (HI_U16)((ucURByte5 >> 7) & 0x01);
    pstInfoRealmDl->usCommSpeed = (HI_U16)(((ucURByte5 << 8) & 0x7F00) + (ucURByte4 & 0x00FF));
    pstInfoRealmDl->ucSeq       = ucURByte6;                            // ֡���к�
    return ;
}

//*****************************************************************************
// ��������: mrs3762RealmUlEnc
// ��������: TODO: ... 
//           
// ����˵��:
//   HI_PBYTE pInBuffer [in] ...
//                          ...
//   MRS_3762_INFO_REALM_UL_STRU * pstInfoRealmUl [in/out] ...
//                                                         ...
//
// �� �� ֵ: 
//    TODO: ...
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: niesongsong/kf62735 [2011-12-13]
//*****************************************************************************
HI_VOID mrs3762RealmUlEnc(HI_OUT HI_PBYTE pucOutBuffer, 
                          HI_IN MRS_3762_INFO_REALM_UL_STRU * pstInfoRealmUl)  
{

    pucOutBuffer[0] = (HI_U8)(((pstInfoRealmUl->ucRelayLevel & 0x0F) << 4)   //�м̼��� - 4 
                           + ((pstInfoRealmUl->ucModuleFlag & 1)  << 2)     //ͨѶģ���ʶ - 1 
                           + ((pstInfoRealmUl->ucRouteFlag & 1)));          //·�ɱ�ʶ - 1 
    pucOutBuffer[1] = (HI_U8)((pstInfoRealmUl->ucCHFlag & 0x0F));            //�ŵ���ʶ - 4 
    pucOutBuffer[2] = (HI_U8)(((pstInfoRealmUl->ucMeterCH & 0x0F) << 4)      //���ͨ������ - 4 
                           + ((pstInfoRealmUl->ucPhaseFlag & 0x0F)));       //ʵ�����߱�־ - 4 
    pucOutBuffer[3] = (HI_U8)(((pstInfoRealmUl->ucResSignal & 0x0F) << 4)    //ĩ��Ӧ���ź�Ʒ�� - 4 
                           + ((pstInfoRealmUl->ucCmdSignal & 0x0F)));       //ĩ�������ź�Ʒ�� - 4 
    pucOutBuffer[4] = pstInfoRealmUl->ucEvtFlg|pstInfoRealmUl->ucResrved;
    pucOutBuffer[5] = pstInfoRealmUl->ucSeq; // ֡���к�

    return ;
}

//*****************************************************************************
// ��������: MRS_Proto3762Dec
// ��������: ����376.2Э��ı���ͷ�����ݵ�Ԫ��ʶ �� ���ݵ�Ԫ �������ݲ�����
//           
// ����˵��:
//   pucDatagram [in] ����
//   usDatagramSize [in] ���Ĵ�С����λ�ֽ�
//   ppstFrame [out] ָ�� MRS_3762_FRAME_STRU �ṹ, ���ڱ���������֡�ṹ��Ϣ. 
//                   ��ָ���ɸú�������, ��Ҫʹ�� HI_SYS_FREE(HI_MOD_ID_APP_MRS_PROTO,ppstFrame) �ͷ�. 
//   
// �� �� ֵ: 
//    HI_ERR_SUCCESS : �ɹ�
//    ����Ϊ������ 
//
// ����Ҫ��: ���յ� ID_MRS_MSG_MR_DATA ��Ϣ�󣬵��øýӿڻ�ȡ 376.2 ֡�ṹ 
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-21]
//*****************************************************************************
HI_U32 MRS_Proto3762Dec( HI_IN HI_PBYTE pucDatagram, HI_U16 usDatagramSize, 
                       HI_OUT MRS_3762_FRAME_STRU** ppstFrame )
{
    HI_U32 ret          = HI_ERR_SUCCESS;
    HI_PBYTE pInBuffer  = 0;
    HI_U16 usAppDataLen = 0;
    HI_U16 usAddrLen    = 0;
    HI_U8 ucDir;
    MRS_3762_FRAME_STRU st3762Frame;

    if (!pucDatagram) 
    {
        return HI_ERR_FAILURE;
    }
    
    //У��֡���˴����Ա�֤������ȷ������������С֡��
    ret = mrsToolsCheck3762Frame(pucDatagram,usDatagramSize);
    if (ret != HI_ERR_SUCCESS) 
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_004, HI_DIAG_MT("376.2 Chk Err "));
        return HI_ERR_BAD_DATA;
    }

    (hi_void)memset_s(&st3762Frame, sizeof(st3762Frame), 0, sizeof(st3762Frame));

    //��ʼ����
    pInBuffer  = pucDatagram;
    // ������
    pInBuffer += MRS_3762_FRAME_HEAD_SIZE + MRS_3762_FRAME_LEN_SIZE;
    ucDir                         = (HI_U8) (((*pInBuffer) >> 7) & 1);
    st3762Frame.stCtrl.ucDir      = ucDir;
    st3762Frame.stCtrl.ucPrm      = (HI_U8) (((*pInBuffer) >> 6) & 1);
    st3762Frame.stCtrl.ucCommForm = (HI_U8) ((*pInBuffer) & 0x3F);
    pInBuffer++;

    if (ucDir == 0)
    { // ����
        MRS_3762_INFO_REALM_DL_STRU* pDl = &(st3762Frame.strUsrDataHrd.uInfoRealm.stInfRealmDL);
        
        mrs3762RealmDlDec(pInBuffer, pDl);
        pInBuffer += MRS_3762_FRAME_INFO_SIZE; /* sizeof(MRS_3762_INFO_REALM_DL_STRU) */   

        // ��ַ��A
        if ((st3762Frame.strUsrDataHrd.uInfoRealm.stInfRealmDL.ucModuleFlag == 1))
        {
            HI_U8 ucRelayLevel = 0;
            
            st3762Frame.strUsrDataHrd.stAddrRealm.ucRelayLevel = pDl->ucRelayLevel;
            
            ucRelayLevel = st3762Frame.strUsrDataHrd.stAddrRealm.ucRelayLevel;

            usAddrLen = (HI_U16) ((ucRelayLevel + 2) * 6);
            
            if ((HI_S16)(usDatagramSize - MRS_3762_FRAME_MIN_SIZE) < usAddrLen)
            {
                // ��ַ�򳤶Ȳ���
                HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_005, HI_DIAG_MT("376.2 addr len too short"));
                return HI_ERR_BAD_DATA;
            }
                 
            // Դ��ַ
            (hi_void)memcpy_s(st3762Frame.strUsrDataHrd.stAddrRealm.ucSrcAdd,
                sizeof(st3762Frame.strUsrDataHrd.stAddrRealm.ucSrcAdd),
                pInBuffer, sizeof(st3762Frame.strUsrDataHrd.stAddrRealm.ucSrcAdd));
            pInBuffer += sizeof(st3762Frame.strUsrDataHrd.stAddrRealm.ucSrcAdd);
            
            // �м̵�ַ
            if (ucRelayLevel <= 15)
            {
                (hi_void)memcpy_s(st3762Frame.strUsrDataHrd.stAddrRealm.ucRelayAdd[0], 
                    sizeof(st3762Frame.strUsrDataHrd.stAddrRealm.ucRelayAdd), pInBuffer, HI_PLC_MAC_ADDR_LEN * ucRelayLevel);
            }
            
            pInBuffer += HI_PLC_MAC_ADDR_LEN * ucRelayLevel;
            
            // Ŀ�ĵ�ַ
            (hi_void)memcpy_s(st3762Frame.strUsrDataHrd.stAddrRealm.ucDestAdd,
                sizeof(st3762Frame.strUsrDataHrd.stAddrRealm.ucDestAdd),
                pInBuffer, sizeof(st3762Frame.strUsrDataHrd.stAddrRealm.ucDestAdd));
            pInBuffer += sizeof(st3762Frame.strUsrDataHrd.stAddrRealm.ucDestAdd);
        }
    }
    else
    { // ���в�����н�֡
        return HI_ERR_BAD_DATA;
    }

    //�ܳ��� = �̶�����+��Ϣ��+��ַ��+Ӧ��������
    usAppDataLen = (usDatagramSize - MRS_3762_FRAME_MIN_SIZE - usAddrLen + MRS_3762_FRAME_FN_SIZE);

    if((HI_S16)usAppDataLen < MRS_3762_FRAME_FN_SIZE)
    {
        return HI_ERR_BAD_DATA;
    }

    st3762Frame.strUsrDataHrd.stAppDataRealm.ucAfn = *pInBuffer++;    
    st3762Frame.strUsrDataHrd.stAppDataRealm.usAppDataLen = usAppDataLen;

    *ppstFrame = mrsToolsMalloc(sizeof(MRS_3762_FRAME_STRU) + usAppDataLen);
    if (*ppstFrame == HI_NULL) 
    {
        //�����ڴ�ʧ��
        return HI_ERR_MALLOC_FAILUE;
    }
    (hi_void)memcpy_s(*ppstFrame, sizeof(MRS_3762_FRAME_STRU) + usAppDataLen, &st3762Frame, sizeof(MRS_3762_FRAME_STRU));

    (hi_void)memcpy_s( (*ppstFrame)->strUsrDataHrd.stAppDataRealm.pucAppDataContent, usAppDataLen,
                pInBuffer, usAppDataLen);

    return ret;
} 


HI_VOID mrs3762DiscardOldData(HI_VOID) 
{
    MRS_3762_PROTO_CTX_STRU *pst3762Ctx= mrs3762GetContext();

    (hi_void)memset_s(pst3762Ctx->pucRcvBuffer, PRODUCT_CFG_MRS_3762_BUFFER_BYTE_MAX, 0, PRODUCT_CFG_MRS_3762_BUFFER_BYTE_MAX);
    pst3762Ctx->usDagramSize = 0;
    pst3762Ctx->usRcvBufferSize = 0;
}

#endif

