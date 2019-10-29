//*****************************************************************************
//
//                  ��Ȩ���� (C), 2017-2019, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_power_failure.c
//  �� �� ��   : V1.0
//  ��    ��   : jingyankai/00130424
//  ��������   : 2017-8-16
//  ��������   : ͣ���¼�����
//
//  �����б�   :
//  �޸���ʷ   :
//  1.��    �� :
//    ��    �� :
//    �޸����� :
//
//*****************************************************************************
#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_dfx.h"
#include "mrs_fw_proto376_2_frame.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_log.h"
#include "mrs_srv_common.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_parallel_cco.h"
#include "mrs_power_failure.h"
#include "mrs_fw_n.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_cco_event.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_proto_csg.h"
#include "mrs_fw_proto_csg_echo.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

//#define BIT_SET(x,i,b)  ((x)=(HI_U8)((b)?((1<<(i))|(x)):((~(HI_U8)(1<<(i)))&(x))))
#define BIT_SET(x,i)  ((x)=(HI_U8)(((1<<(i))|(x))))
#define BIT_GET(x,i) (((1<<(i)) & (x))?1:0)

HI_VOID mrsSrvBitSetPf(HI_U8 *pBitmap, HI_U16 pos);
HI_BOOL mrsSrvBitTestPf(HI_U8 *pBitmap, HI_U16 pos);
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_VOID mrsStaNormalBitmap(HI_OUT HI_U8 *bitmapOut, HI_U16 bitmapLen);
HI_U16 mrsCalcMinTei(HI_VOID);
HI_U16 mrsCalcMaxTei(HI_VOID);
HI_U32 mrsStaSendPowerFailureFrame(MRS_PLC_FRAME_DATA_S *pstReportData);
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_VOID mrsCcoReportBitmap(HI_VOID);
HI_U16 mrsCcoCalcMinTei(HI_VOID);
HI_U16 mrsCcoCalcMaxTei(HI_VOID);
HI_U32 mrsCcoGetBitmapTeiCnt(HI_U16 bitmapMinTei, HI_U16 bitmapMaxTei);
HI_VOID mrsPowerFailureDfxInfo(HI_VOID);
#endif

/************************************************************
                   ���ļ������ȫ�ֱ���
************************************************************/
NV_APP_MRS_EVT_CFG_STRU g_power_failure_nv = {0};

//ͣ���ϱ���ؿ�����Ϣ
POWER_FAILURE_CTRL_S g_power_failure_ctrl = {
    {0},
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    {0}, {0}, 0, HI_FALSE, 0,
#endif
    HI_FALSE, 0, 0,
};

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
//mac����������״̬��Ϣ
POWER_FAILURE_RCV_MAC_REPORT_S g_rcv_mac_transmit_info = {0};
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
CCO_POWER_FAILURE_STATISTIC_S g_power_failure_mrs_dfx_info = {0};
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
//*****************************************************************************
// ��������: mrsPowerFailureCallback
// ��������: MRS�Ļص�����
//
// ����˵��:
//  This function has no arguments.
//
// �� �� ֵ:
//    TODO: ...
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: j00415756
//*****************************************************************************
HI_U32 mrsPowerFailureCallback(HI_U8 ucType, HI_MDM_POWER_FAILURE_REPORT_ST *pCmdData, HI_U32 ulDataSize)

{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_SYS_QUEUE_MSG_S stMsg = { 0 };

    //id���
    stMsg.ulMsgId = EN_MRS_FW_MSG_POWER_FAILURE;
    stMsg.ulParam[0] = (HI_U32)ucType;
    stMsg.ulParam[1] = (HI_U32)(ulDataSize);
    //����վ��tei,�豸״̬��ʶ�������ܽ���Ϣ
    (hi_void)memcpy_s(&stMsg.ulParam[2], sizeof(HI_U32), pCmdData, sizeof(HI_MDM_POWER_FAILURE_REPORT_ST));


    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_007, HI_DIAG_MT("MRS_GET_DATA_FROM_MAC in call back"),
                    pCmdData->device_power_failure,pCmdData->device_not_pull_reset);
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_008, HI_DIAG_MT("MRS_GET_DATA_FROM_MAC in call back copy"),
                    stMsg.ulParam[2]);
    //���ͽ�������еȴ�����
    ret = mrsSendMessage2Queue(&stMsg);

    return ret;
}
#endif
//λ���㺯��
HI_VOID mrsSrvBitSetPf(HI_U8 *pBitmap, HI_U16 pos)
{
    HI_U8 ucPos = (HI_U8)(pos / 8);
    HI_U8 ucBit = pos % 8;

    BIT_SET(pBitmap[ucPos], ucBit);
}

HI_BOOL mrsSrvBitTestPf(HI_U8 *pBitmap, HI_U16 pos)
{
    HI_U8 ucPos = (HI_U8)(pos / 8);
    HI_U8 ucBit = pos % 8;

    return (HI_BOOL)(1 == BIT_GET(pBitmap[ucPos], ucBit));
}
/*****************************************************************************
 �� �� ��  : mrsGetPowerFailureCtrlInfo
 ��������  : ��ȡͣ���ϱ��¼�����ر���
 �������  : ��
 �� �� ֵ  : power_failure_bitmap_st *
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2017��8��18��
    ��    ��   : test
    �޸�����   : �����ɺ���

*****************************************************************************/
POWER_FAILURE_CTRL_S *mrsGetPowerFailureCtrlInfo(HI_VOID)
{
    return &g_power_failure_ctrl;
}

/*****************************************************************************
 �� �� ��  : mrsGetPowerFailureNvCtrl
 ��������  : ��ȡͣ����Ʊ���
 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2017��8��16��
    ��    ��   : test
    �޸�����   : �����ɺ���

*****************************************************************************/
NV_APP_MRS_EVT_CFG_STRU *mrsGetPowerFailureNvCtrl(HI_VOID)
{
    return &g_power_failure_nv;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
/*****************************************************************************
 �� �� ��  : mrsGetPowerFailureRcv
 ��������  : ��ȡmac������������Ϣ
 �������  : ��
 �� �� ֵ  : POWER_FAILURE_RCV_MAC_REPORT_S *
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2017��9��19��
    ��    ��   : test
    �޸�����   : �����ɺ���

*****************************************************************************/
POWER_FAILURE_RCV_MAC_REPORT_S *mrsGetPowerFailureMacInfo(HI_VOID)
{
    return &g_rcv_mac_transmit_info;
}
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
/*****************************************************************************
 �� �� ��  : mrsGetPowerFailureDfxInfo
 ��������  : ��ȡά����Ϣ����
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : CCO_POWER_FAILURE_STATISTIC_S *
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2017��10��19��
    ��    ��   : test
    �޸�����   : �����ɺ���

*****************************************************************************/
CCO_POWER_FAILURE_STATISTIC_S *mrsGetPowerFailureDfxInfo(HI_VOID)
{
    return &g_power_failure_mrs_dfx_info;
}
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
/*****************************************************************************
 �� �� ��  : mrsHandlePowerFailure
 ��������  : ͣ�紦����ں���
 �������  : HI_IN HI_SYS_QUEUE_MSG_S* pstMsg
 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2017��8��16��
    ��    ��   : j00415756
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_U32 mrsHandlePowerFailure(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U32 eventType = 0;
    POWER_FAILURE_RCV_MAC_REPORT_S *powerFailureRcvInfo = mrsGetPowerFailureMacInfo();
    HI_SYS_QUEUE_MSG_S *rcvPstmsg = HI_NULL;

    if(HI_NULL == pstMsg)
    {
        return ret;
    }

    rcvPstmsg = pstMsg;
    eventType = rcvPstmsg->ulParam[0];
    (hi_void)memcpy_s(powerFailureRcvInfo,sizeof(POWER_FAILURE_RCV_MAC_REPORT_S),
                     &rcvPstmsg->ulParam[2], sizeof(HI_U32));

    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_013, HI_DIAG_MT("MRS_GET_DATA_FROM_MAC"),
                    powerFailureRcvInfo->devicePowerFailure,powerFailureRcvInfo->devicePullReset);

    switch(eventType)
    {
        case HI_MDM_POWER_FAILURE_EVENT_LOCAL:
            ret = mrsHandleLocalPowerFailure();
            break;
        case HI_MDM_POWER_FAILURE_EVNET_POWERON:
            mrsHandlePowerOn();
            break;
        default:
            ret = HI_ERR_FAILURE;
            break;
    }

    return ret;
}

/*****************************************************************************
 �� �� ��  : mrsHandlePowerOn
 ��������  : ͣ���ٻָ��¼�������
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2017��8��18��
    ��    ��   : test
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_VOID mrsHandlePowerOn(HI_VOID)
{
    //osreset������
    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("MRS_REBOOT"));
    HI_MDM_Reboot(HI_SYS_REBOOT_CAUSE_MRS11);

}
#endif

/*****************************************************************************
 �� �� ��  : mrsGetOthersPowerFailureFrameData
 ��������  : ��ȡ����վ���ͣ���ϱ�֡�����������Ϣ
 �������  : MRS_EVENT_INFO_STRU * pstRcvEvtInfo
             HI_U8 *pstRcvData
             HI_BOOL flag
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2017��8��17��
    ��    ��   : test
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_VOID mrsGetOthersPowerFailureFrameData(HI_IN MRS_CSG_PLC_FRAME_HEAD * pstRcvEvtInfo)
{
    HI_U16 i = 0;
    HI_U16 *dataTei = 0;
    HI_U8 rcvTei[DATA_END_TEI_POSITION] = {0};
    HI_U16 teiTemp = 0;
    POWER_FAILURE_CTRL_S *powerFailureBitmap = mrsGetPowerFailureCtrlInfo();

    //��Ҫ��teiת��Ϊλͼ
    for(i = 0; i < DATA_END_TEI_POSITION; i++)
    {
        rcvTei[i] = pstRcvEvtInfo->aucData[SAVE_POSITION + REPORT_EVENT_TYPE + i];
    }
    //��ȡ��Сtei
    dataTei = (HI_U16*)rcvTei;

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_066, HI_DIAG_MT("[PW god, check tei=%d]"),*dataTei);

    //�������е�teiˢ�µ�ȫ��bitmap֮��
    for(i = 0; i < (pstRcvEvtInfo->usDataLength - DATA_FIX_SIZE) * BIT_WIZE; i++)
    {
        if(HI_TRUE == mrsSrvBitTestPf(&pstRcvEvtInfo->aucData[DATA_FIX_SIZE],i))
        {

            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_064, HI_DIAG_MT("[PW god check i=%d]"),i);
            teiTemp = (*dataTei) + i;
            mrsSrvBitSetPf(powerFailureBitmap->power_failure_bitmap, teiTemp);
        }

    }

}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
/*****************************************************************************
 �� �� ��  : mrsHandleOthersPowerFailureFrame
 ��������  : ��������ģ���ͣ���ϱ�֡
 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2017��8��17��
    ��    ��   : test
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_U32 mrsHandleOthersPowerFailureFrame(HI_VOID)
{
    NV_APP_MRS_EVT_CFG_STRU *powerFailureNvCtrl = HI_NULL;
    HI_U32 ret = HI_ERR_SUCCESS;
    POWER_FAILURE_CTRL_S *powerFailureBitmap = mrsGetPowerFailureCtrlInfo();

    powerFailureNvCtrl = mrsGetPowerFailureNvCtrl();
    powerFailureBitmap->isRedayToSend = HI_TRUE;

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_040, HI_DIAG_MT("[PCO-hdl oth pff=%d]"),powerFailureNvCtrl->ucStaConvergeTime);
    MRS_StartTimer(MRS_STA_TIMER_POWERFAILURE, powerFailureNvCtrl->ucStaConvergeTime * MRS_US_TO_S_MASK, HI_SYS_TIMER_ONESHOT);

    return ret;
}

/*****************************************************************************
 �� �� ��  : mrsStaNormalBitmap
 ��������  :������׼��bitmapת��Ϊ��׼bitmap
 �������  : ��
 �������  : ��
 �� �� ֵ  :
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2017��8��19��
    ��    ��   : test
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_VOID mrsStaNormalBitmap(HI_OUT HI_U8 *bitmapOut, HI_U16 bitmapLen)
{
    //���������Ҫת����λͼ����λͼ����
    HI_U16 i = 0;
    HI_U16 j = 0;
    HI_U16 bitmapMinTei = 0;
    HI_U16 bitmapMaxTei = 0;
    HI_U8 *bitmapIn = HI_NULL;
    POWER_FAILURE_CTRL_S *powerFailureBitmapInfo = HI_NULL;

    //ֱ�ӻ�ȡȫ�ֱ������ȡ��Ӧ��Χ�ڵ�tei��ֵ
    powerFailureBitmapInfo = mrsGetPowerFailureCtrlInfo();
	bitmapMinTei = mrsCalcMinTei();
    bitmapMaxTei = mrsCalcMaxTei();
    bitmapIn = powerFailureBitmapInfo->power_failure_bitmap;

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_052, HI_DIAG_MT("[GET_send_bitmap,maxtei]"),bitmapMaxTei);
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_053, HI_DIAG_MT("[GET_send_bitmap,mintei]"),bitmapMinTei);

    for (i = bitmapMinTei, j = 0; i <= bitmapMaxTei && j < bitmapLen * BIT_WIZE; i++,j++)
    {
        //�ж���С���tei����Щtei�����ϵ磬���µ�buf�н����λ��1
        if (HI_TRUE == mrsSrvBitTestPf(bitmapIn, i))
        {
            mrsSrvBitSetPf(bitmapOut, j);
        }
    }
}
/*****************************************************************************
 �� �� ��  : mrsCalcMinTeiDataLen
 ��������  : ������Сtei�Լ����ݳ���
 �������  : HI_OUT HI_U16 *tei
             HI_OUT HI_U16 *dataLen
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2017��8��18��
    ��    ��   : test
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_U16 mrsCalcMinTei(HI_VOID)
{
    HI_U16 i = 0;
    HI_U16 ret = 0;
    HI_U16 bitmapMinTei = 0;

    POWER_FAILURE_CTRL_S *powerFailureBitmapInfo = HI_NULL;
    HI_U8 *bitmapIn = HI_NULL;
    powerFailureBitmapInfo = mrsGetPowerFailureCtrlInfo();
    bitmapIn = powerFailureBitmapInfo->power_failure_bitmap;

    //������Сtei,ֱ�ӷ�������λ��
    for(i = MIN_START_TEI; i < POWER_FAILURE_BITMAP_LEN * BIT_WIZE; i++)
    {
        if (HI_TRUE == mrsSrvBitTestPf(bitmapIn, i))
        {
            bitmapMinTei = i;
            break;
        }
    }

    ret = bitmapMinTei;

    return ret;
}

/*****************************************************************************
 �� �� ��  : mrsCalcMaxTei
 ��������  : �������tei�Լ����ݳ���
 �������  : HI_OUT HI_U16 *tei
             HI_OUT HI_U16 *dataLen
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2017��8��18��
    ��    ��   : test
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_U16 mrsCalcMaxTei(HI_VOID)
{
    HI_U16 i = 0;
    HI_U16 ret = 0;
    HI_U16 bitmapMaxTei = 0;
    HI_U8 *bitmapIn = HI_NULL;
    POWER_FAILURE_CTRL_S *powerFailureBitmapInfo = HI_NULL;

    powerFailureBitmapInfo = mrsGetPowerFailureCtrlInfo();
    bitmapIn = powerFailureBitmapInfo->power_failure_bitmap;

    for(i = (POWER_FAILURE_BITMAP_LEN - 1) * BIT_WIZE; i >= 1; i--)
    {
        if (HI_TRUE == mrsSrvBitTestPf(bitmapIn, i))
        {
            bitmapMaxTei = i;
            break;
        }
    }

    ret = bitmapMaxTei;
    return ret;
}

/*****************************************************************************
 �� �� ��  : mrsCreatePowerFailureFrame
 ��������  : ����ͣ���ϱ�֡
 �������  : HI_U16 tei
 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2017��8��17��
    ��    ��   : test
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_U32 mrsCreatePowerFailureFrame(HI_VOID)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_U16 usPayloadLen = 0;
    HI_U16 pwBitmapMinTei = 0;
    HI_U16 pwBitmapMaxTei = 0;
    HI_U16 usBitmapLen = 0;
    HI_U8 *teiPoint = HI_NULL;
    P_MRS_STA_EVT_CTX pstCtx = mrsStaGetEvtCtx();
    HI_U8 *pucPayload = HI_NULL;
    HI_U8 index = 0;
    POWER_FAILURE_RCV_MAC_REPORT_S *deviceStateInfo = HI_NULL;
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = HI_NULL;
    MRS_PLC_FRAME_DATA_S pstPowerFailureFrame = {0};
    POWER_FAILURE_CTRL_S *powerFailureBitmapInfo = HI_NULL;

    if (HI_NULL == pstCtx)
    {
        return HI_ERR_NO_INITILIZATION;
    }

    //����bitͼ��Ϣ,��ȡ�豸״̬
    powerFailureBitmapInfo = mrsGetPowerFailureCtrlInfo();
    //������Ϣһ�𴫵�����
    deviceStateInfo = mrsGetPowerFailureMacInfo();

    //������С,���tei�����ݳ���
    pwBitmapMinTei = mrsCalcMinTei();
    pwBitmapMaxTei = mrsCalcMaxTei();

    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_050, HI_DIAG_MT("[PCO-tei-range]"), pwBitmapMinTei, pwBitmapMaxTei);

    usBitmapLen = (pwBitmapMaxTei - pwBitmapMinTei + BIT_WIZE) / BIT_WIZE;

    teiPoint = (HI_U8*)(&pwBitmapMinTei);

    //����Ϊ8 + 5 + ����
    usPayloadLen = sizeof(MRS_CSG_PLC_FRAME_HEAD) + DATA_FIX_SIZE + usBitmapLen;
    pucPayload = (HI_U8*)mrsToolsMalloc(usPayloadLen);
    if(HI_NULL == pucPayload)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_009, HI_DIAG_MT("Malloc failed"));
        return HI_ERR_MALLOC_FAILUE;
    }
    (hi_void)memset_s(pucPayload, usPayloadLen, 0, usPayloadLen);

    //���֡�еĹ̶�����
    pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pucPayload;
    pstFrameHead->usFrameType = MRS_CSG_PLC_CTRL_TYPE_REPROT;
    pstFrameHead->usExtendBit = MRS_CSG_PLC_CTRL_EXTEND_BIT_N;
    pstFrameHead->usRespBit = MRS_CSG_PLC_CTRL_RESP_BIT_N;
    pstFrameHead->usPrm = MRS_CSG_PLC_CTRL_PRM_HOST;
    pstFrameHead->usDir = MRS_CSG_PLC_CTRL_DIR_UL;

    pstFrameHead->ucSrvId = MRS_CSG_PLC_SRV_ID_REPORT_EVENT;
    pstFrameHead->ucVersion = MRS_CSG_PLC_PROTO_VERSION;
    pstCtx->usSeq++;
    pstFrameHead->usSeq = pstCtx->usSeq;

    //����ҵ���ģ������� + ���� + ���� (�䳤L, �ϱ����ͣ� tei��bitmap)
    pstFrameHead->usDataLength = DATA_FIX_SIZE + usBitmapLen;

    //�ɱ��������ݿ���
    pstFrameHead->aucData[0] = MRS_EVENT_FUN_REPORT_MODUL;
    pstFrameHead->aucData[SAVE_POSITION] = 0;
    pstFrameHead->aucData[REPORT_EVENT_TYPE] = SINGLE_AMMETER_POWER_FAILURE;
    (hi_void)memcpy_s(&pstFrameHead->aucData[REPORT_EVENT_TYPE + DATA_START_TEI_POSITION], sizeof(HI_U8), teiPoint, sizeof(HI_U8));
    (hi_void)memcpy_s(&pstFrameHead->aucData[REPORT_EVENT_TYPE + DATA_END_TEI_POSITION], sizeof(HI_U8), teiPoint + 1, sizeof(HI_U8));
    //ֱ�ӽ���������
    mrsStaNormalBitmap(&pstFrameHead->aucData[DATA_FIX_SIZE], usBitmapLen);

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1000, HI_DIAG_MT("[PCO-MR]-PW-senddata"), pucPayload, usPayloadLen);

    //���ݴ�idѡ������
    pstPowerFailureFrame.usId = ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC;

    pstPowerFailureFrame.pucPayload = pucPayload;
    pstPowerFailureFrame.usPayloadLen = usPayloadLen;

    //�����ģ��ͣ�磬����ַΪ���ع㲥��ַ
    if(HI_TRUE == deviceStateInfo->devicePowerFailure)
    {
        pstPowerFailureFrame.ucMac[index] = 0;
        (hi_void)memset_s(pstPowerFailureFrame.ucMac + 1, HI_PLC_MAC_ADDR_LEN - 1, 0xFF, HI_PLC_MAC_ADDR_LEN - 1);

        //�����Ϣ����,��η��ͣ����ڲ���ȫ�ֱ�������
        mrsStaSendPowerFailureFrame(&pstPowerFailureFrame);

        mrsToolsFree(pucPayload);

        return ret;
    }

    //����ģ��δͣ�磬������ַ������Ҫ������,ֱ�ӷ��ͳ�ȥ
    (HI_VOID)MRS_SendFrameProc(&pstPowerFailureFrame);

    //STA������ɺ�Ͳ���ȫ������
    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_055, HI_DIAG_MT("[PCO-MR-PW-sendover]"));
    (hi_void)memset_s(powerFailureBitmapInfo, sizeof(POWER_FAILURE_CTRL_S), 0, sizeof(POWER_FAILURE_CTRL_S));
    mrsToolsFree(pucPayload);

    return ret;
}


/*****************************************************************************
 �� �� ��  : mrsStaSendEvtReportPowerFailure
 ��������  : ����ͣ���ϱ�����
 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2017��8��17��
    ��    ��   : test
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_U32 mrsStaSendPowerFailureFrame(MRS_PLC_FRAME_DATA_S *pstReportData)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    NV_APP_MRS_EVT_CFG_STRU *powerFailureNvCtrl = HI_NULL;
    POWER_FAILURE_CTRL_S *powerFailureBitmapInfo = HI_NULL;

    //ֻ�ܷ���
    powerFailureNvCtrl = mrsGetPowerFailureNvCtrl();
    powerFailureBitmapInfo = mrsGetPowerFailureCtrlInfo();

    if (!pstReportData)
    {
        return HI_ERR_BAD_DATA;
    }

    ret = MRS_SendFrameProc(pstReportData);

    //���ͳɹ���ȴ�ʱ���������¼�������
    powerFailureBitmapInfo->sta_send_times++;

    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_032, HI_DIAG_MT("PW_FAILURE_STA,Sendtime,interval")
                        ,powerFailureBitmapInfo->sta_send_times,powerFailureNvCtrl->usSendInterval);

    if(powerFailureBitmapInfo->sta_send_times < powerFailureNvCtrl->ucSendTimes)
    {
         MRS_StartTimer(MRS_STA_TIMER_POWERFAILURE, powerFailureNvCtrl->usSendInterval ,HI_SYS_TIMER_ONESHOT);
    }
    else
    {
        //���ʹ����㹻�󣬲���bitmap��Ϣ
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_045, HI_DIAG_MT("POWER_FAILURE_SendTime_ENOUGH"));
        (hi_void)memset_s(powerFailureBitmapInfo, sizeof(POWER_FAILURE_CTRL_S), 0, sizeof(POWER_FAILURE_CTRL_S));

    }
    mrsDfxStaPlcEvtReportTimeStamp(HI_MDM_GetMilliSeconds());

    return ret;
}
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

/*****************************************************************************
 �� �� ��  : mrsGetReportMacAddr
 ��������  : ��ȡͣ��tei��mac��ַ
 �������  : HI_U16 powerFailureTei
             HI_U8 *macAddrSave
 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2017��8��20��
    ��    ��   : test
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_VOID mrsCcoReportMacAddr(HI_U8 *bitmapIn, HI_U8 *macAddr, HI_U32 macAllocLen,
                                    HI_IN HI_U16 startPos, HI_OUT HI_U16 *nextStartPos)
{
    HI_U16 i = 0, j = 0;
    HI_U16 pwBitmapMaxTei = 0;
    HI_U16 queryIndex = 0;
    HI_U8 cpyMacAddrCnt = 0;
    HI_MAC_NETWORK_TOPO_ENTRY_S *topo_entry = HI_NULL;

    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();


    pwBitmapMaxTei = mrsCcoCalcMaxTei();


    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_033, HI_DIAG_MT("cco report evt in getmactei"),startPos,pwBitmapMaxTei);

    mrsRefreshTopoInfo(pstCcoCtx);
    topo_entry = pstCcoCtx->stTopoInf.entry;

    for(i = startPos; (i <= pwBitmapMaxTei) && (cpyMacAddrCnt < macAllocLen); i++)
    {
        //�������Сtei֮���tei���м��
        if (HI_FALSE == mrsSrvBitTestPf(bitmapIn, i))
        {
            continue;
        }

        for(j = queryIndex; j < HI_MAC_NETWORK_TOPO_ENTRY_MAX; j++)
        {
            //mac��ַ��Ч���ж��ڼ���tei cnt ʱ����ɣ���֤�ռ�����׼ȷ��
            if(topo_entry[j].tei == i)
            {
                HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_034, HI_DIAG_MT("cco report evt ready getmacaddr"),i);
                HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1002, HI_DIAG_MT("cco get macaddr "), topo_entry[j].mac,HI_PLC_MAC_ADDR_LEN);

                queryIndex = j + 1;
                (hi_void)memcpy_s(macAddr + cpyMacAddrCnt, HI_PLC_MAC_ADDR_LEN, topo_entry[j].mac, HI_PLC_MAC_ADDR_LEN);
                mrsHexInvert(macAddr + cpyMacAddrCnt, HI_PLC_MAC_ADDR_LEN);
                cpyMacAddrCnt += HI_PLC_MAC_ADDR_LEN;
                //�Ѿ��ϱ������tei��λ��
                *nextStartPos = i;
                break;
            }
        }
    }

    //teiλ�ú���һλ
    *nextStartPos += 1;
}
/*****************************************************************************
 �� �� ��  : mrsCcoGetBitmapTeiCnt
 ��������  : ��ȡbitmap�е�tei����
 �������  : ��
 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2017��8��19��
    ��    ��   : test
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_U32 mrsCcoGetBitmapTeiCnt(HI_U16 bitmapMinTei, HI_U16 bitmapMaxTei)
{
    HI_U32 ret = 0;
    HI_U16 i = 0, j = 0;
    HI_U16 queryIndex = 0;
    HI_U8 *bitmapIn = HI_NULL;
    HI_U8 invalidMacAddr[HI_PLC_MAC_ADDR_LEN] = {0};
    HI_MAC_NETWORK_TOPO_ENTRY_S *topo_entry = HI_NULL;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    POWER_FAILURE_CTRL_S *ccoReportCtrl = mrsGetPowerFailureCtrlInfo();

    mrsRefreshTopoInfo(pstCcoCtx);
    topo_entry = pstCcoCtx->stTopoInf.entry;

    bitmapIn = ccoReportCtrl->cco_diff_bitmap;

    //��mac��ַ�ļ�飬��֤׼ȷ��
    for(i = bitmapMinTei; i <= bitmapMaxTei; i++)
    {
        if (HI_FALSE == mrsSrvBitTestPf(bitmapIn, i))
        {
            continue;
        }

        for(j = queryIndex; j < HI_MAC_NETWORK_TOPO_ENTRY_MAX; j++)
        {
            //���Ӷ�mac��ַ��Ч�Ե��ж�
            if(topo_entry[j].tei == i && (0 != memcmp(topo_entry[j].mac, invalidMacAddr, HI_PLC_MAC_ADDR_LEN)))
            {
                ret++;
                queryIndex = j + 1;
                break;
            }
        }

    }

    return ret;
}


/*****************************************************************************
 �� �� ��  : mrsCCOCalcMaxTei
 ��������  : �������tei�Լ����ݳ���
 �������  : HI_OUT HI_U16 *tei
             HI_OUT HI_U16 *dataLen
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2017��8��18��
    ��    ��   : test
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_U16 mrsCcoCalcMaxTei(HI_VOID)
{
    HI_U16 i = 0;
    HI_U16 ret = 0;
    HI_U16 bitmapMaxTei = 0;
    HI_U8 *bitmapIn = HI_NULL;
    POWER_FAILURE_CTRL_S *ccoReportCtrl = mrsGetPowerFailureCtrlInfo();


    bitmapIn = ccoReportCtrl->cco_diff_bitmap;

    //������Сtei�Լ�data�������е�������ռ�ռ�Ĵ�С
    for(i = (POWER_FAILURE_BITMAP_LEN - 1) * BIT_WIZE; i >= 1; i--)
    {
        if (HI_TRUE == mrsSrvBitTestPf(bitmapIn, i))
        {
            bitmapMaxTei = i;
            break;
        }
    }

    ret = bitmapMaxTei;
    return ret;
}
/*****************************************************************************
 �� �� ��  : mrsCalcMinTeiDataLen
 ��������  : ������Сtei�Լ����ݳ���
 �������  : HI_OUT HI_U16 *tei
             HI_OUT HI_U16 *dataLen
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2017��8��18��
    ��    ��   : test
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_U16 mrsCcoCalcMinTei(HI_VOID)
{
    HI_U16 i = 0;
    HI_U16 ret = 0;
    HI_U16 bitmapMinTei = 0;

    HI_U8 *bitmapIn = HI_NULL;
    POWER_FAILURE_CTRL_S *ccoReportCtrl = mrsGetPowerFailureCtrlInfo();
    bitmapIn = ccoReportCtrl->cco_diff_bitmap;

    //������Сtei,ֱ�ӷ�������λ��
    for(i = MIN_START_TEI; i < POWER_FAILURE_BITMAP_LEN * BIT_WIZE; i++)
    {
        if (HI_TRUE == mrsSrvBitTestPf(bitmapIn, i))
        {
            bitmapMinTei = i;
            break;
        }

    }

    ret = bitmapMinTei;

    return ret;
}


/*****************************************************************************
 �� �� ��  : mrsCcoCreateReportFrame
 ��������  : cco��Ӧ��ʱ���������ϱ�����
 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2017��8��19��
    ��    ��   : test
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_U32 mrsCcoCreateReportFrame(HI_PBYTE *ppBuf, HI_U16 *usBufLen, HI_IN HI_OUT HI_U16 *startTeiPos)
{

    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_PBYTE pucData = HI_NULL;
    HI_U16 usDataLen = 0;
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    MRS_CSG_IND_FRAME_STRU stIndFrame = {0};
    POWER_FAILURE_CTRL_S *ccoReportCtrl = HI_NULL;
    HI_U16 pwRepDataLen = 0;
    HI_U16 pwBitmapMaxTei = 0;
    HI_U32 bitmapTeiCnt = 0;
    HI_U16 nextStartTeiPos = 0;
    HI_U8 *powerFailureBitmap = HI_NULL;

    ccoReportCtrl = mrsGetPowerFailureCtrlInfo();
    powerFailureBitmap = ccoReportCtrl->cco_diff_bitmap;

    pwBitmapMaxTei = mrsCcoCalcMaxTei();

    //��ȡλͼ�е�tei��Ϣ
    bitmapTeiCnt = mrsCcoGetBitmapTeiCnt(*startTeiPos, pwBitmapMaxTei);

    //��������е����ݳ���,����������ݳ���ʱ����Ҫ����ϱ�
    if(bitmapTeiCnt > MAX_REPORT_TEI_CNT)
    {
       pwRepDataLen = MAX_REPORT_TEI_CNT * HI_PLC_MAC_ADDR_LEN;
    }
    else
    {
       pwRepDataLen = (HI_U16)(bitmapTeiCnt * HI_PLC_MAC_ADDR_LEN);
    }

    usDataLen = CCO_REPORT_EVENT_DATA + pwRepDataLen + 1;
    pucData = (HI_U8*)mrsToolsMalloc(usDataLen);
    if(!pucData)
    {
        ulRet = HI_ERR_MALLOC_FAILUE;
        return ulRet;
    }

    pucData[0] = MRS_DEVICE_TYPE_BROAD_CARRIER_COMM_UNIT; // �豸����
    pucData[1] = MRS_DEVICE_TYPE_COLLECTOR; // ��Լ����
    pucData[2] = (HI_U8)pwRepDataLen + 1;
    pucData[CCO_REPORT_EVENT_DATA] = MRS_COMM_UNIT_TYPE_POWER_FAILURE;

    mrsCcoReportMacAddr(powerFailureBitmap, pucData + CCO_REPORT_EVENT_DATA + 1,
                            pwRepDataLen, *startTeiPos, &nextStartTeiPos);

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_031, HI_DIAG_MT("cco report evt datalen"),pwRepDataLen);
    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1001, HI_DIAG_MT("cco report evt,mac_addr"),pucData + CCO_REPORT_EVENT_DATA + 1,pwRepDataLen);

    //�´ο�ʼ��teiλ��
    *startTeiPos = nextStartTeiPos;

    (hi_void)memset_s(&stIndFrame, sizeof(stIndFrame), 0, sizeof(stIndFrame));
    stIndFrame.ucPrm = PROTOCSG_CTRL_PRM_BIT_REQ;
    stIndFrame.ucAddrFlag = PROTOCSG_CTRL_ADD_BIT_Y;
    stIndFrame.ucSeq = ++pstCcoCtx->ucSeq;

    stIndFrame.ucAfn = MRS_CSG_AFN_REPORT_05H;
    stIndFrame.ulDI = MRS_CSG_DI_REPORT_POWER_FAILURE;
    stIndFrame.usAppDataLen = usDataLen;
    stIndFrame.pAppData = pucData;

    (hi_void)memcpy_s(stIndFrame.aucDstAddr, HI_METER_ADDR_LEN, pstCcoCtx->ucMainNodeAddr, HI_METER_ADDR_LEN);
    mrsHexInvert(stIndFrame.aucDstAddr, sizeof(stIndFrame.aucDstAddr));

    ulRet = mrsCreateCsgFrame(&stIndFrame, ppBuf, usBufLen);

    mrsToolsFree(pucData);

    return ulRet;
}

/*****************************************************************************
 �� �� ��  : mrsCcoRspPowerFailureEvt
 ��������  : cco��Ӧ�¼��ϱ���ʱ��
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2017��8��19��
    ��    ��   : test
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_U32 mrsCcoRspPowerFailureEvt(HI_VOID)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_U8 *pucUpData = HI_NULL;
    HI_U16 usDataLen = 0;
    HI_U32 i = 0;
    HI_U32 pk_num = 0;
    HI_U16 pwBitmapMinTei = 0;
    HI_U16 pwBitmapMaxTei = 0;
    HI_U32 bitmapTeiCnt = 0;
    HI_U16 startTei = 0;
    MRS_EVT_QUEUE_ITEM *pstEvtItem = HI_NULL;
    P_MRS_CCO_EVT_CTX_STRU pstEvtCtx = mrsGetCcoEvtCtx();
    POWER_FAILURE_CTRL_S *powerFailureInfo = mrsGetPowerFailureCtrlInfo();
    NV_APP_MRS_EVT_CFG_STRU *powerFailureNvCtrl = mrsGetPowerFailureNvCtrl();

    //��ȡҪ���͵�bitmap
    mrsCcoReportBitmap();

    //CCO�˼���diffbitmap�е����ֵ����Сֵ
    pwBitmapMinTei = mrsCcoCalcMinTei();
    pwBitmapMaxTei = mrsCcoCalcMaxTei();

    bitmapTeiCnt = mrsCcoGetBitmapTeiCnt(pwBitmapMinTei, pwBitmapMaxTei);
    startTei = pwBitmapMinTei;

    pk_num = (bitmapTeiCnt + MAX_REPORT_TEI_CNT - 1) / MAX_REPORT_TEI_CNT;

    for(i = 0; i < pk_num; i++)
    {
        ret = mrsCcoCreateReportFrame(&pucUpData, &usDataLen, &startTei);

        if(HI_ERR_SUCCESS != ret)
        {
            break;
        }
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_059, HI_DIAG_MT("cco pwevt assemble ok,join evt queue"));
        pstEvtItem = (MRS_EVT_QUEUE_ITEM *)mrsToolsMalloc(sizeof(MRS_EVT_QUEUE_ITEM) + usDataLen);
        if (!pstEvtItem)
        {
            mrsToolsFree(pucUpData);
            return HI_ERR_NO_MORE_MEMORY;
        }

        (hi_void)memset_s(pstEvtItem, sizeof(MRS_EVT_QUEUE_ITEM) + usDataLen, 0, sizeof(MRS_EVT_QUEUE_ITEM) + usDataLen);

        pstEvtItem->usDataLen = usDataLen;
        (hi_void)memcpy_s(pstEvtItem->pucData, pstEvtItem->usDataLen, pucUpData, usDataLen);

        mrsCcoEvtEnQueue(&pstEvtCtx->stEvtQueue, pstEvtItem);
        pstEvtCtx->stEvtQueue.usItemNum++;
        mrsToolsFree(pucUpData);
    }

    pstEvtCtx->bACDFlg = HI_TRUE;
    mrsActiveEvtReport();

    mrsDfxCcoEvtQueueItemNum(pstEvtCtx->stEvtQueue.usItemNum);
    mrsDfxCcoEvtFlag(pstEvtCtx->bACDFlg);

    //ά����Ϣ
    mrsPowerFailureDfxInfo();

    //��������bitmap�����Ž��ձ�ʶ
    (hi_void)memset_s(powerFailureInfo->cco_diff_bitmap, POWER_FAILURE_BITMAP_LEN, 0, POWER_FAILURE_BITMAP_LEN);
    powerFailureInfo->isRedayToSend = HI_FALSE;

    if(HI_FALSE == powerFailureInfo->cco_erease_bitmap_flag)
    {
        //��ʱ����90s���������
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_007, HI_DIAG_MT("cco begin erase data"), powerFailureNvCtrl->ucCcoConvergeTime * MRS_US_TO_S_MASK * CCO_ERASE_BITMAP);
        MRS_StartTimer(MRS_CCO_TIMER_POWERFAILURE_ERASE_DATA, powerFailureNvCtrl->ucCcoConvergeTime * MRS_US_TO_S_MASK * CCO_ERASE_BITMAP, HI_SYS_TIMER_ONESHOT);
        powerFailureInfo->cco_erease_bitmap_flag = HI_TRUE;
    }

    return HI_ERR_SUCCESS;
}

/*****************************************************************************
 �� �� ��  : mrsEraseBitmap
 ��������  : cco��������
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2017��9��15��
    ��    ��   : test
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_VOID mrsEraseBitmap(HI_VOID)
{
    POWER_FAILURE_CTRL_S *powerFailureInfo = mrsGetPowerFailureCtrlInfo();
    HI_U8 ccoTempDiffBitmap[POWER_FAILURE_BITMAP_LEN]= {0};

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_004, HI_DIAG_MT("cco erase data"));

    //�Ȼ�ȡ��������
    mrsCcoReportBitmap();
    (hi_void)memcpy_s(ccoTempDiffBitmap, POWER_FAILURE_BITMAP_LEN, powerFailureInfo->cco_diff_bitmap, POWER_FAILURE_BITMAP_LEN);

    //�ϱ�����ʱ��������90s��,����ȫ�ֱ����е�����
    (hi_void)memset_s(powerFailureInfo, sizeof(POWER_FAILURE_CTRL_S), 0, sizeof(POWER_FAILURE_CTRL_S));

    //�������ݵ�ȫ�ֱ�����
    (hi_void)memcpy_s(powerFailureInfo->power_failure_bitmap, POWER_FAILURE_BITMAP_LEN, ccoTempDiffBitmap, POWER_FAILURE_BITMAP_LEN);
    (hi_void)memcpy_s(powerFailureInfo->cco_diff_bitmap, POWER_FAILURE_BITMAP_LEN, ccoTempDiffBitmap, POWER_FAILURE_BITMAP_LEN);

}

HI_VOID mrsPowerFailureDfxInfo(HI_VOID)
{
    HI_U16 i = 0;
    HI_U16 powerFailureTeiCnt = 0;
    POWER_FAILURE_CTRL_S *powerFailureInfo = mrsGetPowerFailureCtrlInfo();
    CCO_POWER_FAILURE_STATISTIC_S *powerFailureDfxInfo = mrsGetPowerFailureDfxInfo();

    for(i = 0; i < POWER_FAILURE_BITMAP_LEN * BIT_WIZE; i++)
    {
        //ά����Ϣ�洢
        if(HI_TRUE == mrsSrvBitTestPf(powerFailureInfo->cco_diff_bitmap, i))
        {
            mrsSrvBitSetPf(powerFailureDfxInfo->cco_statistic_pwf_bitmap,i);
        }

        if(HI_TRUE == mrsSrvBitTestPf(powerFailureDfxInfo->cco_statistic_pwf_bitmap, i))
        {
            powerFailureTeiCnt++;
        }
    }

    powerFailureDfxInfo->pwf_tei_cnt = powerFailureTeiCnt;
    powerFailureDfxInfo->rcv_pwf_frame_time = HI_MDM_GetSeconds();

    HI_MDM_NM_PowerFailurCcoDfxInfo(powerFailureDfxInfo);
}

/*****************************************************************************
 �� �� ��  : mrsGetReportBitmap
 ��������  : ��ȡδ���͹���bitmap
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2017��9��14��
    ��    ��   : test
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_VOID mrsCcoReportBitmap(HI_VOID)
{
    POWER_FAILURE_CTRL_S *powerFailureInfo = mrsGetPowerFailureCtrlInfo();
    HI_U16 i = 0;

    for(i = 0; i < POWER_FAILURE_BITMAP_LEN * BIT_WIZE; i++)
    {
        //���ϱ����¼�tei,����diff_bitmap��ӦλΪ1
        if(HI_TRUE == mrsSrvBitTestPf(powerFailureInfo->power_failure_bitmap, i)
           && HI_FALSE == mrsSrvBitTestPf(powerFailureInfo->cco_last_send_bitmap, i))
        {
            mrsSrvBitSetPf(powerFailureInfo->cco_diff_bitmap,i);
        }
    }

    (hi_void)memcpy_s(powerFailureInfo->cco_last_send_bitmap, POWER_FAILURE_BITMAP_LEN,
                         powerFailureInfo->power_failure_bitmap, POWER_FAILURE_BITMAP_LEN);
}

/*****************************************************************************
 �� �� ��  : mrsCcoEvtReport
 ��������  : cco�ϱ��¼���������ģ��
 �������  : MRS_EVENT_INFO_STRU *pstEvtTransmit
            ���յ�������֡
 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2017��8��18��
    ��    ��   : test
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_U32 mrsCcoEvtReport(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    NV_APP_MRS_EVT_CFG_STRU *powerFailureNvCtrl = HI_NULL;
    POWER_FAILURE_CTRL_S *powerFailureInfo = HI_NULL;
    powerFailureInfo = mrsGetPowerFailureCtrlInfo();
    powerFailureNvCtrl = mrsGetPowerFailureNvCtrl();

    powerFailureInfo->isRedayToSend = HI_TRUE;

    //30s�Ķ�ʱ�����ڵ��ں󿪷Ž��չ��ܲ���
    ulRet = MRS_StartTimer(MRS_CCO_TIMER_POWERFAILURE, powerFailureNvCtrl->ucCcoConvergeTime * MRS_US_TO_S_MASK, HI_SYS_TIMER_ONESHOT);

    return ulRet;
}
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
/*****************************************************************************
 �� �� ��  : mrsHandleLocalPowerFailure
 ��������  : ������ͣ��
 �������  : HI_U16 pow_failure_tei
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2017��8��16��
    ��    ��   : j00415756
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_U32 mrsHandleLocalPowerFailure(HI_VOID)
{
    //��ȡnv����ȡ����ʱ��������һ��ֵ
    NV_APP_MRS_EVT_CFG_STRU *powerFailureNvCtrl = HI_NULL;
    POWER_FAILURE_CTRL_S *powerFailureInfo = HI_NULL;
    HI_U32 randomWaitSendTime = 0;
    HI_U32 ret = HI_ERR_SUCCESS;
    POWER_FAILURE_RCV_MAC_REPORT_S *rcvMacInfo = HI_NULL;

    powerFailureInfo = mrsGetPowerFailureCtrlInfo();
    powerFailureNvCtrl = mrsGetPowerFailureNvCtrl();
    rcvMacInfo = mrsGetPowerFailureMacInfo();
    randomWaitSendTime = HI_SYS_GetRandNum32AddMac(powerFailureNvCtrl->usRandomMin,powerFailureNvCtrl->usRandomMax);


    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_036, HI_DIAG_MT("APP_RCV_PWF_Evt"),powerFailureNvCtrl->usRandomMin,powerFailureNvCtrl->usRandomMax);
    //���ⲿ���tei��ˢ�µ�ȫ��bitmap֮��
    powerFailureInfo->power_failure_bitmap[rcvMacInfo->usMyTei / BIT_WIZE] |= 1 << (rcvMacInfo->usMyTei % BIT_WIZE);

    //���ö�ʱ������random_wait_timeʱ��� ���ж��Ƿ��յ�����ģ��ı��� �����ϱ�֡������
    MRS_StartTimer(MRS_STA_TIMER_POWERFAILURE, randomWaitSendTime, HI_SYS_TIMER_ONESHOT);

    return ret;
}
#endif

HI_VOID mrsSetStaPowerFailureNvCtrl(NV_APP_MRS_EVT_CFG_STRU *nv)
{

    NV_APP_MRS_EVT_CFG_STRU *powerFailureNvCtrl = mrsGetPowerFailureNvCtrl();
    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_037, HI_DIAG_MT("handle nv"));
    //����������»�ȡ����
    powerFailureNvCtrl->ucStaConvergeTime = nv->ucStaConvergeTime;
    powerFailureNvCtrl->ucCcoConvergeTime = nv->ucCcoConvergeTime;
    powerFailureNvCtrl->usRandomMin = nv->usRandomMin;
    powerFailureNvCtrl->usRandomMax = nv->usRandomMax;
    powerFailureNvCtrl->usSendInterval = nv->usSendInterval;
    powerFailureNvCtrl->ucSendTimes = nv->ucSendTimes;
    powerFailureNvCtrl->ucPowerFailureReportEnable = nv->ucPowerFailureReportEnable;
}

#endif
#endif


