//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2014, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_capture.c
//  �� �� ��   : V1.0
//  ��    ��   : fengxiaomin/f00209182
//  ��������   : 2016-09-26
//  ��������   : TODO: ...
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2016-09-26
//    ��    �� : fengxiaomin/f00209182
//    �޸����� : �����ļ�
//
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_fw_n.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_srv_res.h"
#include "mrs_srv_common.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_sta_baudrate.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_sta.h"
#include "mrs_time.h"
#include "hi_mdm.h"
#include "hi_app.h"
#include "mrs_srv_capture.h"
#include "app_mrs.h"
#include "mrs_dfx_clt.h"
#include "mrs_srv_baudrate_manage.h"
#include "mrs_prepare_test_mode.h"

HI_START_HEADER

#ifdef PRODUCT_CFG_TTCN

HI_BOOL g_mrs_test_mode_enable;

HI_PRV HI_U32 mrsCaptureFunc(HI_MDM_CAPTURE_PKT_PBB_TYPE_EN ePayloadType, HI_U8 *pucFcData, HI_U8 *pucPayloadeData);
HI_PRV HI_VOID mrsCaptureMsduFunc(HI_MDM_CAPTURE_MSDU_INFO_ST *CaptureMsduInfo);

HI_VOID mrsSrvCaptureInit(HI_VOID)
{
    mrs_set_test_mode_enable(HI_FALSE);

    #if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    // ����30�붨ʱ�� nv ��ȡ
    MRS_StartTimer(MRS_STA_TIMER_ENTER_TESTMODE, MRS_SEC_TO_MS(MRS_STD_PHYSICAL_TEST_MODE_LISTEN), HI_SYS_TIMER_ONESHOT);
    #endif

    #if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    // ����30�붨ʱ�� nv ��ȡ
    MRS_StartTimer(MRS_CCO_TIMER_ENTER_TESTMODE, MRS_SEC_TO_MS(MRS_STD_PHYSICAL_TEST_MODE_LISTEN), HI_SYS_TIMER_ONESHOT);
    #endif
    // ע��ץ������
    HI_MDM_RegisterCapturePktFunction(mrsCaptureFunc);
    HI_MDM_SetCaptureMode(HI_MAC_CAPTURE_SINGLE_PB_MODE);
}

HI_U32 mrsSrvCaptureDeinit(HI_VOID)
{
	HI_MDM_UnregisterCaptureMsduInterface(mrsCaptureMsduFunc);

	if(HI_MDM_GetPhysicalTestMode() == PHYSICAL_TEST_MODE_NONE)
	{
        HI_MDM_SetCaptureMode(HI_MAC_NO_CAPTURE_MODE);
        HI_MDM_SetInterceptMsdu(HI_FALSE);
	}

    return HI_MDM_CancelCapturePktFunction();
}

HI_U32 mrsCheckPhyTest(HI_U8 *pucFcData, HI_U8 *pucPayloadeData)
{
    HI_U32 ulRet = HI_ERR_FAILURE;
    HI_U16 usPayloadLen = 0;
    HI_U16 usTotalLen = HI_FC_SIZE;
    HI_U8 ucLen = 0;
    HI_U8 * payload = HI_NULL;

	if (pucFcData == HI_NULL)
	{
		return ulRet;
	}

    // �ű�֡�����͵������ŵ�
	if ((pucFcData[0]&0x07) == 0)
	{
		return ulRet;
	}

	usPayloadLen = HI_MDM_Get_PayloadLen((pucFcData[0]&0x07));
	usTotalLen += usPayloadLen;

    payload = mrsToolsMalloc(usTotalLen);
    if (payload == HI_NULL)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(payload, usTotalLen, 0, usTotalLen);
    (hi_void)memcpy_s(payload, usTotalLen, pucFcData, HI_FC_SIZE);

    if (pucPayloadeData != HI_NULL)
    {
        (hi_void)memcpy_s(payload + HI_FC_SIZE, usPayloadLen, pucPayloadeData, usPayloadLen);
        mrsDfxStaTestModePayloadBuf(payload + HI_FC_SIZE, usPayloadLen);
    }

    if (usTotalLen > HI_DMS_FRM_MAX_PAYLOAD_SIZE)
    {
        ucLen = (HI_U8)(usTotalLen - HI_DMS_FRM_MAX_PAYLOAD_SIZE);
        ulRet = MRS_SendMrData(payload, HI_DMS_FRM_MAX_PAYLOAD_SIZE, HI_DMS_CHL_UART_PORT_APP);
        if (ulRet == HI_ERR_SUCCESS)
        {
            ulRet = MRS_SendMrData(payload + HI_DMS_FRM_MAX_PAYLOAD_SIZE,ucLen, HI_DMS_CHL_UART_PORT_APP);
        }
    }
    else
    {
        ulRet = MRS_SendMrData(payload, usTotalLen, HI_DMS_CHL_UART_PORT_APP);
    }

    mrsToolsFree(payload);

    return ulRet;
}

HI_U32 mrsCheckMsduTest(HI_MDM_CAPTURE_MSDU_INFO_ST *CaptureMsduInfo)
{
    HI_U32 ulMsduLength = 0;
    HI_U8 * payload = HI_NULL;
    HI_U32 ulRet = 0;
    HI_U16 usLen = 0;

    if (HI_NULL == CaptureMsduInfo || 0 == CaptureMsduInfo->msdu_size || HI_NULL == CaptureMsduInfo->msdu_addr)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    ulMsduLength = CaptureMsduInfo->msdu_size;
    payload = CaptureMsduInfo->msdu_addr;
    if (ulMsduLength > HI_DMS_FRM_MAX_PAYLOAD_SIZE)
    {
        usLen = (HI_U16)(ulMsduLength - HI_DMS_FRM_MAX_PAYLOAD_SIZE);
        ulRet = MRS_SendMrData(payload, HI_DMS_FRM_MAX_PAYLOAD_SIZE, HI_DMS_CHL_UART_PORT_APP);
        if (ulRet == HI_ERR_SUCCESS)
        {
            ulRet = MRS_SendMrData(payload + HI_DMS_FRM_MAX_PAYLOAD_SIZE, usLen, HI_DMS_CHL_UART_PORT_APP);
        }
    }
    else
    {
        ulRet = MRS_SendMrData(payload, (HI_U16)ulMsduLength, HI_DMS_CHL_UART_PORT_APP);
    }

    return ulRet;

}

HI_U32 mrsCaptureFunc(HI_MDM_CAPTURE_PKT_PBB_TYPE_EN ePayloadType, HI_U8 *pucFcData, HI_U8 *pucPayloadeData)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 *pAppData = HI_NULL;
    HI_U16 usLen = 0;
    mrs_app_test_fream_st test_fream = {0};

    HI_U16 pktId = 0;
	HI_U8 ucTestMode = HI_MDM_GetPhysicalTestMode();

    // ��ǰģʽΪ͸�����յ���FC+PB����FC��PB�͵������ŵ�
    if (ucTestMode == PHYSICAL_TEST_MODE_APP_TO_COM_LOOP)
    {
        return mrsCheckPhyTest(pucFcData, pucPayloadeData);
    }
    // ��ǰģʽΪ͸�����յ�MSDU�������ŵ������˳�������MSDUץ���ӿڸ���MSDU͸���������ŵ�
    if (ucTestMode == PHYSICAL_TEST_MODE_EXIT)
    {
        return HI_ERR_SKIP;
    }

    mrsDfxStaTestModeCnt(ucTestMode);
    if (PBB_CORRECT_TYPE != ePayloadType)
    {
        mrsDfxStaTestModeErrPbbCnt((HI_U8)ePayloadType);
        return HI_ERR_SKIP;
    }

    if((HI_NULL == pucFcData) || (HI_NULL == pucPayloadeData))
    {
        return HI_ERR_SKIP;
    }

    ulRet = mrsSrvParseAppData(pucPayloadeData, &pAppData, &usLen);
    if((HI_NULL == pAppData) || (HI_ERR_SUCCESS != ulRet))
    {
        mrsDfxStaTestModeErrParseCnt(ulRet);
        return HI_ERR_SKIP;
    }

    // ��鱨�����ݺϷ���
    (hi_void)memcpy_s(&pktId, sizeof(pktId), pAppData + MRS_HEAD_PKT_ID_OFFSET, sizeof(HI_U16));
    if ((MRS_APP_PORT_NUM != pAppData[MRS_HEAD_PORT_OFFSET]) || (pktId != MRS_PKT_ID_PLC_TEST))
    {
        mrsDfxStaTestModeInvalid(pAppData[MRS_HEAD_PORT_OFFSET],MRS_PKT_ID_PLC_TEST);
        return HI_ERR_INVALID_PARAMETER;
    }

    (hi_void)memcpy_s(&test_fream, sizeof(test_fream), pAppData + MRS_PLC_APP_HEAD_LEN, sizeof(test_fream));

    // ����ͨ�Ų��Ա���ʱ��������ޣ����ڽ������ģʽ
    if (HI_MDM_GetSeconds() > MRS_STD_PHYSICAL_TEST_MODE_LISTEN)
    {
        return HI_ERR_SKIP;
    }

    switch(test_fream.test_mode)
    {
        case PHYSICAL_TEST_MODE_APP_TO_COM:
            break;

        case PHYSICAL_TEST_MODE_APP_TO_PLC:
            break;
        // ͸��SOF��FC + PB�������ŵ�
        case PHYSICAL_TEST_MODE_APP_TO_COM_LOOP:
            HI_MDM_EnterPhyTestMode(PHYSICAL_TEST_MODE_APP_TO_COM_LOOP);
            mrsDfxStaTestModeEnter(PHYSICAL_TEST_MODE_APP_TO_COM_LOOP,ucTestMode);
            MRS_PrepareUartConfig(MRS_SRV_BAUD_RATE_115200, MRS_SRV_UART_PARITY_NONE);

			HI_MDM_SetCaptureMode(HI_MAC_INTERCEPT_MULTI_PB_MODE);

            #if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
            MRS_StartTimer(MRS_STA_TIMER_PHYSICAL_TESTMODE, MRS_MIN_TO_MS(test_fream.test_para), HI_SYS_TIMER_ONESHOT);
            #elif defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
            MRS_StartTimer(MRS_CCO_TIMER_PHYSICAL_TESTMODE, MRS_MIN_TO_MS(test_fream.test_para), HI_SYS_TIMER_ONESHOT);
            #endif
            break;

        // ���������ش�����ģʽ
        case PHYSICAL_TEST_MODE_APP_TO_PLC_LOOP:
            HI_MDM_EnterPhyTestMode(PHYSICAL_TEST_MODE_APP_TO_PLC_LOOP);
            // ����MAC����ջ��������ػ�����ģʽ
			HI_MDM_SetCaptureMode(HI_MAC_INTERCEPT_LOOP_MODE);

            #if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
            MRS_StartTimer(MRS_STA_TIMER_TESTMODE, MRS_MIN_TO_MS(test_fream.test_para), HI_SYS_TIMER_ONESHOT);
            #endif
            #if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
            MRS_StartTimer(MRS_CCO_TIMER_TESTMODE, MRS_MIN_TO_MS(test_fream.test_para), HI_SYS_TIMER_ONESHOT);
            #endif
            break;

        // MSDU͸��������
        case PHYSICAL_TEST_MODE_EXIT:
            HI_MDM_EnterPhyTestMode(PHYSICAL_TEST_MODE_EXIT);
            mrsDfxStaTestModeEnter(PHYSICAL_TEST_MODE_EXIT, ucTestMode);
            // ע��ץȡMSDU���ĵ������ŵ�
			HI_MDM_RegisterCaptureMsduInterface(mrsCaptureMsduFunc);
			// ����MSDU�������ϱ���Ӧ�ò����������
            HI_MDM_SetInterceptMsdu(HI_TRUE);
            MRS_PrepareUartConfig(MRS_SRV_BAUD_RATE_115200, MRS_SRV_UART_PARITY_NONE);

            #if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
            MRS_StartTimer(MRS_STA_TIMER_MSDU_TESTMODE, MRS_MIN_TO_MS(test_fream.test_para), HI_SYS_TIMER_ONESHOT);
            #elif defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
            MRS_StartTimer(MRS_CCO_TIMER_MSDU_TESTMODE, MRS_MIN_TO_MS(test_fream.test_para), HI_SYS_TIMER_ONESHOT);
            #endif
            break;
        // ����Ƶ��
        case PHYSICAL_TEST_MODE_SET_FREQ:
            mrs_set_test_mode_enable(HI_TRUE);
            #if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
            HI_MDM_SetPhyLowPowerSwtich(HI_FALSE);
            #endif
            HI_MDM_SetWorkFreq((HI_U8)(test_fream.test_para));
            break;

        // ����TONEMASK
        case PHYSICAL_TEST_MODE_SET_TONE_MASK_MODE:
            #if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
            HI_MDM_SetPhyLowPowerSwtich(HI_FALSE);
            #endif
            HI_MDM_SetToneMaskMode((HI_U8)(test_fream.test_para));
            break;

        default:  // ���ع㲥����
            return HI_ERR_SUCCESS;
    }

    return ulRet;
}

HI_VOID mrsCaptureMsduFunc(HI_MDM_CAPTURE_MSDU_INFO_ST *CaptureMsduInfo)
{
	HI_U8 ucTestMode = HI_MDM_GetPhysicalTestMode();

    if (ucTestMode == PHYSICAL_TEST_MODE_EXIT)
    {
        mrsCheckMsduTest(CaptureMsduInfo);
    }
}

HI_U32 mrsSrvParseAppData(HI_U8 *pstPbData, HI_U8 **ppstData, HI_U16 *usLen)
{
    HI_U8 is_mac_existed = 0;
    HI_U8 msdu_type = 0;
    HI_U8 app_offset = 0;
    HI_U16 msdu_len = 0;
    HI_U8 *pbb_buf = pstPbData + MRS_PBH_SIZE_PSG;

    msdu_type = pbb_buf[MRS_HEAD_MSDU_TYPE_OFFSET_PSG];
    // ��Ӧ�ñ��ģ����˳�
    if(MRS_MSDU_TYPE_APP_PSG != msdu_type)
    {
        return HI_ERR_FAILURE;
    }

    msdu_len = pbb_buf[MSDU_LEN_OFFSET] + (((HI_U16)(pbb_buf[MSDU_LEN_OFFSET + 1] & 0x07))<<0x08);
    is_mac_existed = pbb_buf[MAC_ADD_FLAG_OFFSET] & 0x08;
    if(HI_TRUE == is_mac_existed)
    {
        app_offset = MRS_LONG_HEAD_SIZE_PSG;
    }
    else
    {
        app_offset = MRS_SHORT_HEAD_SIZE_PSG;
    }
    *ppstData = pbb_buf + app_offset;
    *usLen = msdu_len;

    return HI_ERR_SUCCESS;
}

HI_BOOL mrs_get_test_mode_enable(HI_VOID)
{
    return g_mrs_test_mode_enable;
}

HI_VOID mrs_set_test_mode_enable(HI_BOOL enable)
{
    g_mrs_test_mode_enable = enable;
}

#endif
HI_END_HEADER
