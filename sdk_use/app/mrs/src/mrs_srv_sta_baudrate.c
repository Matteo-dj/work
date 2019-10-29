//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_sta_baudrate.c
//  �� �� ��   : V1.0 
//  ��    ��   : ������/KF62735
//  ��������   : 2012-07-11
//  ��������   : ���Բ�����
//  �����б�   : NA
//  �޸���ʷ   : 
//  1.��    �� : 2012-07-11
//    ��    �� : ������/KF62735
//    �޸����� : �����ļ� 
//*****************************************************************************
#include "mrs_common.h"
#include "mrs_srv_common.h"
#include "mrs_srv_res.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_srv_sta_baudrate.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_sta.h"
#include "dfx_sys.h"
#include "mrs_srv_io.h"
#include "equip_dut_proc.h"
#include "mrs_srv_sta_searchmeter.h"
#include "hi_mdm.h"
#include "mrs_srv_clti_searchmeter.h"
#include "mrs_dfx_clt.h"
#include "mrs_srv_baudrate_manage.h"
#include "mrs_srv_collector.h"
#include "mrs_srv_sta_meter.h"
#include "mrs_srv_cltii_power_on_off.h"
#include "app_nv.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)


HI_PRV MRS_STA_DETECT_STRU g_stDetectCtx = {0, 0, MRS_SRV_PROTO_645, 0, {0}};

HI_PRV HI_U32 mrsStaDetectEnQueue(HI_U8 *pData, HI_U16 usDataLen, HI_U8 ucProto);
HI_PRV hi_u32 change_baudrate_safe(hi_u32 baudrate, hi_u8 parity);


hi_u32 change_baudrate_safe(hi_u32 baudrate, hi_u8 parity)
{
    hi_u8 test_mode = HI_MDM_GetPhysicalTestMode();

    if ((test_mode != PHYSICAL_TEST_MODE_APP_TO_COM_LOOP) && (test_mode != PHYSICAL_TEST_MODE_EXIT)) {
        MRS_ChangeBaudRate(baudrate, parity);
        mrsDfxCltSetBaudRate((hi_u16)baudrate);
        return HI_ERR_SUCCESS;
    }

    return HI_ERR_FAILURE;
}

MRS_STA_DETECT_STRU* mrsStaDetectGetContext(HI_VOID)
{
    return &g_stDetectCtx;
}

HI_VOID mrsStaDetectBaudRate(HI_VOID)
{
	HI_U16 usSendLen = 0;
	HI_U8* pucSendBuf = HI_NULL;
	MRS_STA_SRV_CTX_STRU * pstSta = mrsStaGetContext();
	MRS_STA_DETECT_STRU* pstDetect = mrsStaDetectGetContext();
	MRS_STA_ITEM  *item;
	MRS_SRV_QUEUE *pstQueue = &pstSta->stQueModule.stMrQueue;
	HI_U8 detectLoopMax = pstSta->detect_loop_max;

	// ��ȡ�׽��
	item = mrsSrvQueueTop(pstQueue);

	if ((item != HI_NULL) && (MRS_STA_ITEM_ID_DETECT == item->id))
	{
		mrsStaTryDeQueue(&pstSta->stQueModule, mrsStaQueueFree);
	}

	if (pstDetect->ucRetry < detectLoopMax * 4)
	{
		//645̽��ɹ�,�����̽��698����
		if ((MRS_SRV_PROTO_698_45 == pstDetect->ucProto) && (pstDetect->uc645AckFlg == DETECT_METER_645ACK_SUCCESS))
		{
			pstDetect->ucRetry = (HI_U8)(detectLoopMax * 2);
			pstDetect->uc645AckFlg = DETECT_METER_PRO_CHANGE_TO_698;
		}
		//645̽��ʧ��,����̽��698
		else if ((MRS_SRV_PROTO_645 == pstDetect->ucProto) && 
				((pstDetect->ucRetry == (detectLoopMax)) || (pstDetect->ucRetry == (detectLoopMax * 3))))
		{
			mrsStaSetFrmProto(MRS_SRV_PROTO_698_45);
			pstDetect->ucProto = MRS_SRV_PROTO_698_45;
		}
		//��һ��698̽��ʧ��,����̽��645
		else if ((MRS_SRV_PROTO_698_45 == pstDetect->ucProto) && 
				(pstDetect->ucRetry == (detectLoopMax * 2)))
		{			
			mrsStaSetFrmProto(MRS_SRV_PROTO_645);
			pstDetect->ucProto = MRS_SRV_PROTO_645;
		}

		if (mrsToolsIsSTA() || mrsToolsIsTSTA())
		{
			P_MRS_SRV_BAUDRATE_CFG_S pstBausRateCfg = mrsSrvGetBaudRateCfg(MRS_SRV_ID_BAUDRATE_DETECT);
			HI_U16 usBaudRateList698Default[] = {MRS_SRV_BAUD_RATE_9600, MRS_SRV_BAUD_RATE_2400};
			HI_U16 usBaudRateList645Default[] = {MRS_SRV_BAUD_RATE_DEFAULT, MRS_SRV_BAUD_RATE_9600};
			HI_U8 ucMaxBaudRateNum = MRS_TOOLS_ALEN(usBaudRateList645Default);
			HI_U16 *usBaudRateList = usBaudRateList645Default;

			if (pstBausRateCfg)
			{
				usBaudRateList = pstBausRateCfg->usBaudRateList;
				ucMaxBaudRateNum = pstBausRateCfg->ucValidNum;
			}

			/* ����698Ĭ��Ϊ9600,��� ���������̽��9600���������л���2400 */
			// 1. δ̽�⵽645��,��Ҫ��ʼ̽��698,����9600������̽��
			// 2. ��9600��������̽�⵽645��,��ʼ̽��698,������9600������̽��
			if(((pstDetect->uc645AckFlg == DETECT_METER_PRO_CHANGE_TO_698)&&(pstSta->usBaudrate == MRS_SRV_BAUD_RATE_9600)) ||
				((pstDetect->uc645AckFlg == DETECT_METER_645ACK_FAIL)&&(pstDetect->ucProto == MRS_SRV_PROTO_698_45)))
			{
				usBaudRateList = usBaudRateList698Default;
				ucMaxBaudRateNum = MRS_TOOLS_ALEN(usBaudRateList698Default);
			}

			pstDetect->usBaudRate = usBaudRateList[(pstDetect->ucRetry / 2) % ucMaxBaudRateNum];
			change_baudrate_safe(pstDetect->usBaudRate, MRS_SRV_UART_PARITY_EVEN);
		}        

		pucSendBuf = mrsGetSendAddrFrame(pstDetect->ucProto, &usSendLen);
		mrsStaDetectEnQueue(pucSendBuf, usSendLen, pstDetect->ucProto);

		pstDetect->ucRetry++;   
		return;
	}

	{
		HI_U8 aucMacAddr[HI_PLC_MAC_ADDR_LEN] = { 0 };

		/* 645̽��ɹ�,698.45����̽��ʧ�ܴ��� */
		if ((MRS_SRV_PROTO_698_45 == pstDetect->ucProto) && (pstDetect->uc645AckFlg == DETECT_METER_PRO_CHANGE_TO_698))
		{
			/* ��698.45������645���Ĵ���̽����� */
			mrsStaSetState(MRS_STA_STATE_MR);
			mrsStaSetFrmProto(MRS_SRV_PROTO_645);
			change_baudrate_safe(pstSta->usBaudrate, MRS_SRV_UART_PARITY_EVEN);
			mrsStaMeterRxNotify(pstSta->aucMeter, pstSta->ucProtocol, HI_FALSE);
			return;
		}

		if (mrsToolsIsSTA() || mrsToolsIsTSTA())
		{
			change_baudrate_safe(MRS_SRV_BAUD_RATE_DEFAULT, MRS_SRV_UART_PARITY_EVEN);
		}

		/* 645��698̽�����ֶ�ʧ��,ʧ�ܴ��� */
		HI_MDM_GetLocalMacAddr(aucMacAddr);
		if (mrsToolsIsTSTA())
		{
			mrsStaSetMeterToMac(aucMacAddr, STA_TYPE_INVALID, HI_MDM_NV_MAC);
		}
#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
		else if (mrsToolsIsCLTI())
		{
			HI_U8 aucCltAddr[HI_METER_ADDR_LEN] = {0};

			mrsStaSetMeterToMac(aucMacAddr, STA_TYPE_CLTI, HI_MDM_NV_MAC);
			mrsStaSetCltiType(HI_TRUE);   //����ΪI������
		    (hi_void)memset_s(aucCltAddr, HI_METER_ADDR_LEN, 0xbb, HI_METER_ADDR_LEN);
			mrsSetCollectorAddr(aucCltAddr);
			mrsCltiTryGetAddrForMAC();
		}
#endif
		else
		{
			mrsStaSetType(STA_TYPE_METER);
			mrsStaSetMeterToMac(aucMacAddr, STA_TYPE_METER, HI_MDM_NV_MAC);
		}

		HI_MDM_DevIdConfig(aucMacAddr);
		HI_MDM_LED_SetReadMeterAddrStatus(LED_READ_METER_FAIL);
	}
    
	mrsStaSetFrmProto(MRS_SRV_PROTO_645);
	pstDetect->ucProto = MRS_SRV_PROTO_645;
    pstDetect->ucRetry = 0;
    MRS_StartTimer(MRS_STA_TIMER_TEST, MRS_SEC_TO_MSEC * (pstSta->detect_period), HI_SYS_TIMER_ONESHOT);

    return;
}

HI_U32 mrsStaDetectEnQueue(HI_U8 *pData, HI_U16 usDataLen, HI_U8 ucProto)
{
    MRS_STA_ITEM * pstItem = HI_NULL;
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();
    hi_u8 test_mode = HI_MDM_GetPhysicalTestMode();

    if ((test_mode == PHYSICAL_TEST_MODE_APP_TO_COM_LOOP) || (test_mode == PHYSICAL_TEST_MODE_EXIT)) {
        return HI_ERR_FAILURE;
    }

    //�����ݷ��͵������Ժ���
    pstItem = mrsToolsMalloc(sizeof(MRS_STA_ITEM) + usDataLen);
    if (HI_NULL == pstItem)
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memset_s(pstItem,sizeof(MRS_STA_ITEM) + usDataLen, 0,sizeof(MRS_STA_ITEM) + usDataLen);

    pstItem->from     = MRS_STA_QUEUE_FROM_STA;
    pstItem->time_out = (HI_U8)MRS_MS_TO_100MS(pstStaCtx->detect_timeout); //��λ:�ٺ���
    pstItem->id       = MRS_STA_ITEM_ID_DETECT;
    pstItem->len      = usDataLen; // ��ȡ���ݳ���
    pstItem->buf      = (HI_U8 *)pstItem + sizeof(MRS_STA_ITEM);   // ƫ�����ݻ�����ָ��
    pstItem->ucProtocol = (MRS_SRV_PROTO_698_45 == ucProto) ? METER_PROTO_698_45 : METER_PROTO_645_2007;
    pstItem->bAutoStrategy = HI_TRUE;
    pstItem->ucFrameTimeout = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameInterval(MRS_STA_RM_CFG_LOCAL));
    (hi_void)memcpy_s(pstItem->buf, usDataLen, pData, usDataLen);     // ��������
    if(!mrsStaTryEnQueue(&pstStaCtx->stQueModule, pstItem))
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("in queue fail"));
        mrsToolsFree(pstItem);
        return HI_ERR_FAILURE;
    }
        
    mrsStaNotifyQueue();
    
    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// ��������: mrsStaSearchMeter
// ��������: ���Ա��ɼ�����������    
// ����˵��:
//   HI_VOID
// �� �� ֵ: 
//   HI_VOID
// ����Ҫ��: ��
// ���þ���: ��
// ��    ��: niesongsong/nkf62735 [2012-05-29]
//*****************************************************************************
HI_VOID mrsStaSearchMeter(HI_VOID)
{
    if (MRS_STA_STATE_INIT != mrsStaGetState())
    {
        return;
    }

#if defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR)

    mrsStaSetState(MRS_STA_STATE_MR);
    HI_MDM_LED_SetReadMeterAddrStatus(LED_READ_METER_SUCCESS);

#else

    if(mrsStaGetSupportVM())
    {
        mrsStaSetState(MRS_STA_STATE_MR);
        MRS_StartTimer(MRS_STA_TIMER_HEART, MRS_STA_TIME_HEART, HI_SYS_TIMER_PERIODIC);
        HI_MDM_LED_SetReadMeterAddrStatus(LED_READ_METER_SUCCESS);

        return;
    }

    if (mrsToolsIsIICollector())
    {
        MRS_COLLECTOR_CTX_STRU *pstCltIICtx = mrsGetCollectorCtx();
        
        mrsStaSetState(MRS_STA_STATE_MR);

        if (pstCltIICtx->ucPowerSearchEnable)
        {
            NV_APP_MRS_EVT_CFG_STRU *powerFailureNvCtrl = mrsGetPowerFailureNvCtrl();

            HI_MDM_LED_SetReadMeterAddrStatus(LED_READ_METER_SUCCESS);
            mrsStaSearchStart(HI_TRUE);
            if (MRS_STA_DETECT_POWER_FAILURE_ENABLE == powerFailureNvCtrl->ucPowerFailureReportEnable)
            {
                mrsCltSetPowerStatus(MRS_CLT_POWER_ON_WAIT_STATUS);
            }
        }
        else
        {
            HI_MDM_LED_SetReadMeterAddrStatus(LED_READ_METER_FAIL);
        }
        
        return;
    }

    mrsStaDetectBaudRate();
#endif
}


HI_VOID mrsStaSearchMeter_698(HI_VOID)
{
    MRS_STA_DETECT_STRU* pstDetect = mrsStaDetectGetContext();

    pstDetect->ucProto = MRS_SRV_PROTO_698_45;
	pstDetect->uc645AckFlg = DETECT_METER_645ACK_SUCCESS;
    mrsStaDetectBaudRate();
}

#endif

