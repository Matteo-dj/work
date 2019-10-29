//*****************************************************************************
//
//                  版权所有 (C), 2001-2014, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_capture.c
//  版 本 号   : V1.0 
//  作    者   : fengxiaomin/f00209182
//  生成日期   : 2016-09-26
//  功能描述   : TODO: ...
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2016-09-26
//    作    者 : fengxiaomin/f00209182
//    修改内容 : 创建文件 
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
#include "mrs_srv_csg_plc.h"
#include "mrs_srv_csg_plc_sta.h"


HI_START_HEADER

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
#define MRS_PLC_FRAME_COM_OFFSET  (0)
#define MRS_PLC_FRAME_ID_OFFSET   (1)
#define MRS_PLC_RESP_FLAG_OFFSET  (5)
#define MRS_PLC_WORK_FLAG_OFFSET  (6)
#define MRS_PLC_FRAME_SEQ_OFFSET  (8)
#define MRS_PLC_TEST_ID_OFFSET    (12)
#define MRS_PLC_TEST_LEN_OFFSET   (14)
#define MRS_PLC_TEST_FREQ_OFFSET  (16)

#define MRS_PLC_TEST_RESP_FLAG    (0x20)
#define MRS_PLC_FRAME_COM_VALUSE  (0x11)
#define MRS_PLC_FRAME_ID_VALUSE   (0x0101)

#define MAC_TYPE_LL_PK            (0)         // MAC HEAD(32byte) 和 ETH HEAD(18byte)为标准协议结构
#define MAC_TYPE_SS_PK            (1)         // MAC HEAD(12byte) 和 ETH HEAD(2byte)

#define MAC_HEAD_LONG_SIZE        (32)
#define MAC_HEAD_SHORT_SIZE       (12)

#define MSDU_HEAD_LONG_SIZE       (18)
#define MSDU_HEAD_SHORT_SIZE      (2)

#define MAC_PB_HEAD_LEN           (4)
#define MAC_HEAD_TYPE_MASK        (0x01)
#define MSDU_HEAD_APP_TYPE        (0x01)
#define MAC_HEAD_MSDU_LEN_OFFSET  (2)

HI_PRV HI_U32 mrsStaCaptureFunc(HI_MDM_CAPTURE_PKT_PBB_TYPE_EN ePayloadType, HI_U8 *pucFcData, HI_U8 *pucPayloadeData);

HI_U32 mrsSrvCaptureInit(HI_VOID)
{
    #if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    HI_U8 ucTimerId = MRS_CCO_TIMER_ENTER_TESTMODE;
    #elif defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    HI_U8 ucTimerId = MRS_STA_TIMER_ENTER_TESTMODE;
    #endif
    
    MRS_StartTimer(ucTimerId, MRS_SEC_TO_MS(MRS_CSG_PHYSICAL_TEST_MODE_LISTEN), HI_SYS_TIMER_ONESHOT);//设置10秒定时器 nv 读取
	HI_MDM_SetCaptureMode(HI_MAC_CAPTURE_SINGLE_PB_MODE);

    //注册抓包函数
    return HI_MDM_RegisterCapturePktFunction(mrsStaCaptureFunc);
}

HI_U32 mrsSrvCaptureDeinit(HI_VOID)
{
    HI_U8 ucMode = HI_MDM_GetPhysicalTestMode();
    if ((ucMode != PHYSICAL_TEST_MODE_LOOPBACK) && (ucMode != PHYSICAL_TEST_MODE_TRANSIMIT))
    {
		HI_MDM_SetCaptureMode(HI_MAC_NO_CAPTURE_MODE);
	}

    return HI_MDM_CancelCapturePktFunction();
}

HI_U32 mrsParseAppData(HI_U8 *pstPbData, HI_U8 **ppstData, HI_U16 *pusLen)
{
    HI_U8 mac_head_type = 0;
    HI_U16 msdu_type = 0;
    HI_U8 app_offset = 0;
    HI_U16 msdu_len = 0;
	HI_U8 *pb = pstPbData + MAC_PB_HEAD_LEN;

    mac_head_type = pb[0] & MAC_HEAD_TYPE_MASK;
    (hi_void)memcpy_s(&msdu_len, sizeof(HI_U16), &pb[MAC_HEAD_MSDU_LEN_OFFSET], sizeof(HI_U16));
    if (MAC_TYPE_LL_PK == mac_head_type)
    {
        app_offset = MAC_HEAD_LONG_SIZE + MSDU_HEAD_LONG_SIZE;
        (hi_void)memcpy_s(&msdu_type, sizeof(HI_U16), &pb[app_offset - sizeof(HI_U16)], sizeof(HI_U16));
        msdu_len -= MSDU_HEAD_LONG_SIZE;
    }
    else if (MAC_TYPE_SS_PK == mac_head_type)
    {
        app_offset = MAC_HEAD_SHORT_SIZE + MSDU_HEAD_SHORT_SIZE;
        msdu_type = pb[app_offset - sizeof(HI_U8)];
        msdu_len -= MSDU_HEAD_SHORT_SIZE;
    }

    if (MSDU_HEAD_APP_TYPE == msdu_type)
    {
        *ppstData = pb + app_offset;
        *pusLen = msdu_len;
        return HI_ERR_SUCCESS;
    }

    return HI_ERR_FAILURE;
}

HI_U32 mrsStaCaptureFunc(HI_MDM_CAPTURE_PKT_PBB_TYPE_EN ePayloadType, HI_U8 *pucFcData, HI_U8 *pucPayloadeData)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 *pAppData = HI_NULL;
    HI_U16 usLen = 0;
    HI_U16 usTestDataLen = 0;
    HI_U16 usIdValue = 0;
    HI_U8 ucMode = HI_MDM_GetPhysicalTestMode();
    HI_U8 ucFreq = 0;

    HI_UNREF_PARAM(pucFcData);
    mrsDfxTestModeCnt(ucMode);

    do 
    {
        if (PBB_CORRECT_TYPE != ePayloadType)
        {
            mrsDfxTestModeErrPbbCnt((HI_U8)ePayloadType);
            return HI_ERR_SKIP;
        }

        ulRet = mrsParseAppData(pucPayloadeData, &pAppData, &usLen);
        if (HI_ERR_SUCCESS != ulRet)
        {
            mrsDfxTestModeErrParseCnt(ulRet);
            return HI_ERR_SKIP;
        }

        if (ucMode == PHYSICAL_TEST_MODE_TRANSIMIT)//物理层转发
        {
            mrsDfxTestModeAppBuf(pAppData, usLen);
            ulRet = MRS_SendMrData(pAppData, usLen, HI_DMS_CHL_UART_PORT_APP); //将MSDU的payload 转给业务串口
            break;
        }

        (hi_void)memcpy_s(&usIdValue, sizeof(HI_U16), pAppData + MRS_PLC_FRAME_ID_OFFSET, sizeof(HI_U16));
        if ((pAppData[MRS_PLC_FRAME_COM_OFFSET] != MRS_PLC_FRAME_COM_VALUSE) || (usIdValue != MRS_PLC_FRAME_ID_VALUSE))
        {
            return HI_ERR_INVALID_PARAMETER;
        }

        (hi_void)memcpy_s(&usTestDataLen, sizeof(HI_U16), &pAppData[MRS_PLC_TEST_LEN_OFFSET], sizeof(HI_U16));
        ucFreq = pAppData[MRS_PLC_TEST_FREQ_OFFSET] & MRS_PLC_TEST_FREQ_MASK;

        if ((MRS_CSG_PLC_SRV_ID_CMD_TEST == pAppData[MRS_PLC_WORK_FLAG_OFFSET]) 
            && (APP_CSG_MODE_PHYSICAL_MAX > pAppData[MRS_PLC_TEST_ID_OFFSET]) 
            && (HI_MDM_GetSeconds() < MRS_CSG_PHYSICAL_TEST_MODE_LISTEN))
        {
            #if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
            HI_U8 ucTimerId = MRS_CCO_TIMER_MODE_PHYSICAL_TEST;
            #elif defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
            HI_U8 ucTimerId = MRS_STA_TIMER_MODE_PHYSICAL_TEST;
            #endif

            switch(pAppData[MRS_PLC_TEST_ID_OFFSET])
            {
                case APP_CSG_MODE_PHYSICAL_LOOPBACK://物理层回环测试模式
                    if(MRS_PLC_TEST_LEN_OFFSET + sizeof(usTestDataLen) + usTestDataLen == usLen)
                    {
						HI_MDM_EnterPhyTestMode(PHYSICAL_TEST_MODE_LOOPBACK);
						HI_MDM_SetWorkFreqCsg(ucFreq);
						HI_MDM_SetCaptureMode(HI_MAC_INTERCEPT_REVERSE_LOOP_MODE);
                        mrsDfxTestModeEnter(APP_CSG_MODE_PHYSICAL_LOOPBACK, HI_MDM_GetPhysicalTestMode());
                        MRS_StartTimer(ucTimerId, MRS_MIN_TO_MS(MRS_TEST_MODE_DURATION), HI_SYS_TIMER_ONESHOT);//设置10分钟定时器 nv 读取
                    }
                    break;

                case APP_CSG_MODE_PHYSICAL_TRANSIMIT://物理层透明转发模式
					HI_MDM_EnterPhyTestMode(PHYSICAL_TEST_MODE_TRANSIMIT);
					HI_MDM_SetWorkFreqCsg(ucFreq);
					HI_MDM_SetCaptureMode(HI_MAC_CAPTURE_SINGLE_PB_MODE);
                    mrsDfxTestModeEnter(APP_CSG_MODE_PHYSICAL_TRANSIMIT, HI_MDM_GetPhysicalTestMode());
                    MRS_StartTimer(ucTimerId, MRS_MIN_TO_MS(MRS_TEST_MODE_DURATION), HI_SYS_TIMER_ONESHOT);//设置10分钟定时器 nv 读取
                    break;

                default:
                    break;
            }

        }
        else
        {
            return HI_ERR_INVALID_PARAMETER;
        }
    }while(HI_FALSE); 

    mrsDfxRefreshTestMode(HI_MDM_GetPhysicalTestMode());

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    if ((pAppData[MRS_PLC_RESP_FLAG_OFFSET] & MRS_PLC_TEST_RESP_FLAG) ==  MRS_CSG_PLC_CTRL_RESP_BIT_Y)
    {
        HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN]= {0};
        HI_U16 usSeq = 0;

        (hi_void)memcpy_s(&usSeq, sizeof(usSeq), &pAppData[MRS_PLC_FRAME_SEQ_OFFSET], sizeof(usSeq));
        if (HI_ERR_SUCCESS == ulRet)
        {
            ulRet = mrsCsgPlcAckFrame(ucMac, usSeq, HI_FALSE);
        }
        else
        {
            ulRet = mrsCsgPlcDenyFrame(ucMac, usSeq, HI_FALSE, mrsStaGetPLCDenyCode(ulRet));
        }
    }
#endif
    return ulRet;
}
#endif
#endif

HI_END_HEADER
