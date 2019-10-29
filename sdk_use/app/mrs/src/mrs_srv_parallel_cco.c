//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_parallel_cco.c
//  版 本 号   : V1.0 
//  作    者   : liuxipeng/KF54842
//  生成日期   : 2012-12-22
//  功能描述   : 并发读表模块函数实现
//               
//  函数列表   :
//  修改历史   : 
//  1.日    期 : 2012-12-22
//    作    者 : liuxipeng/KF54842
//    修改内容 : 创建文件 
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   : 
// SUBSYSTEM :
// MODULE    :  
// OWNER     :  
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_proto376_2_frame.h"
#include "mrs_dfx.h"
#include "mrs_srv_mrslog.h"
#include "mrs_srv_readmeter_manage.h"
#include "mrs_srv_parallel_cco.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_fw_proto698_45.h"
#include "mrs_fw_proto645.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_list.h"
#include "mrs_srv_cco_event.h"
#include "mrs_srv_cco.h"
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

// 并发读表初始化(必须在读表管理模块初始化后执行)
HI_U32 mrsParallelReadMeterInit(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;    

    ret = mrsRmRegisterStrategy(MRS_RM_TYPE_AFNF1H_F1, 
                                EN_MRS_RETRY_CFG_03, 
                                EN_MRS_RM_RETRY_STRATEGY_02,
                                EN_MRS_PARALLEL_POOL_LOW,
                                mrsParallelPlcRespProc);    

    return ret;
}

// 并发读表PLC应答处理
HI_U32 mrsParallelPlcRespProc(P_MRS_RM_REQ_DATA pstData)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    PROTO376_2_FRAME_PRE_STRU stFrame;
    HI_U8 *pucOutBuf = HI_NULL;
    HI_U16 usOutLen = 0;

    if(!pstData)
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memset_s(&stFrame, sizeof(stFrame), 0, sizeof(stFrame));

    stFrame.ucModuleFlag = 1;
    stFrame.ucSeq = pstData->ucMrSeq;
    stFrame.ucIs645 = 1;
    stFrame.ucProtocol = pstData->ucProtocol;
    (hi_void)memcpy_s(stFrame.ucSrcAdd, HI_METER_ADDR_LEN, pstData->ucAddr, HI_METER_ADDR_LEN);

    stFrame.stAfnData.usDataLen = pstData->usDataLen;
    stFrame.stAfnData.pData = pstData->pucData;
    
    stFrame.stAfnData.ucAFN   = MRS_AFN((pstData->usRmType & 0xFF00) >> 8);     // AFN = F1H Fn=1
    stFrame.stAfnData.ucDT[0] = MRS_AFN_FN_LO(pstData->usRmType & 0x00FF);
    stFrame.stAfnData.ucDT[1] = MRS_AFN_FN_HI(pstData->usRmType & 0x00FF);
    stFrame.ucRelayLevel = mrsMapGetLevel(stFrame.ucSrcAdd, pstData->ucMacAdd);
    if (pstData->pucData && pstData->usDataLen)
    {
        stFrame.ucPhase = mrsCcoGetStaPhase(mrsCcoGetSrcMac());
        mrsCcoSetCtxPhase(stFrame.ucPhase);
    }

    ret = mrsCreate3762Frame(&stFrame, &pucOutBuf, &usOutLen, HI_NULL);
    if(ret == HI_ERR_SUCCESS)
    {
        ret = MRS_SendMrData(pucOutBuf, usOutLen, HI_DMS_CHL_UART_PORT_APP);
        mrsDfxPrUartTx();
    }

    mrsToolsFree(pucOutBuf);

    return ret;
}

// 集中器下行并发数据处理
HI_U32 mrsParallelReadMeterProc(HI_IN MRS_3762_FRAME_STRU * frame_data,
                                            HI_IN HI_U8 protocol, HI_IN HI_U8 * buf, HI_IN HI_U16 buf_len)
{
    MRS_RM_REQ_DATA stData = {0};

    (hi_void)memset_s(&stData, sizeof(stData), 0, sizeof(stData));
    if ((frame_data->strUsrDataHrd.uInfoRealm.stInfRealmDL.ucModuleFlag == 1) && mrsToolsNormalAddr(frame_data->strUsrDataHrd.stAddrRealm.ucDestAdd))
    {
        stData.ucAddr = frame_data->strUsrDataHrd.stAddrRealm.ucDestAdd;
    }
    else //没有地址域
    {
        if (mrsCheck69845Proto(protocol))
        {
            HI_U8 aucMeter[HI_METER_ADDR_LEN] = {0};

            if (mrsFind698MeterAddr(buf, buf_len, aucMeter) == HI_ERR_SUCCESS)
            {
                (hi_void)memcpy_s(frame_data->strUsrDataHrd.stAddrRealm.ucDestAdd, HI_METER_ADDR_LEN, aucMeter, HI_METER_ADDR_LEN);
            }
            else
            {
                return HI_ERR_BAD_DATA;
            }

            stData.ucAddr = frame_data->strUsrDataHrd.stAddrRealm.ucDestAdd;            
        }
        else if (mrsCheck645Proto(protocol))
        {
            MRS_645BUF_INF inf = {0};
            // 获取业务读表缓冲区中645帧信息
            mrsCountBuf645Inf(buf, buf_len, &inf);
            if (inf.ucNum == 0)
            {
                return HI_ERR_BAD_DATA;
            }
            
            (hi_void)memcpy_s(frame_data->strUsrDataHrd.stAddrRealm.ucDestAdd, HI_METER_ADDR_LEN, inf.pucAddr, HI_METER_ADDR_LEN);
  
            stData.ucAddr = frame_data->strUsrDataHrd.stAddrRealm.ucDestAdd; 
        }
        else //透明协议
        {
            HI_U8 aucMeter[HI_METER_ADDR_LEN] = {0};
            MRS_645BUF_INF inf = {0};

            if (mrsFind698MeterAddr(buf, buf_len, aucMeter) == HI_ERR_SUCCESS)
            {
                (hi_void)memcpy_s(frame_data->strUsrDataHrd.stAddrRealm.ucDestAdd, HI_METER_ADDR_LEN, aucMeter, HI_METER_ADDR_LEN);
            }
            else
            {
                mrsCountBuf645Inf(buf, buf_len, &inf);
                if (inf.ucNum != 0)
                {
                    (hi_void)memcpy_s(frame_data->strUsrDataHrd.stAddrRealm.ucDestAdd, HI_METER_ADDR_LEN, inf.pucAddr, HI_METER_ADDR_LEN);
                }
            }
            
            stData.ucAddr = frame_data->strUsrDataHrd.stAddrRealm.ucDestAdd; 
        }
    }
    
    stData.usRmType   = MRS_RM_TYPE_AFNF1H_F1;
    stData.ucProtocol = protocol;
    stData.ucMrSeq    = frame_data->strUsrDataHrd.uInfoRealm.stInfRealmDL.ucSeq;
    stData.usDataLen  = buf_len;
    stData.pucData    = buf;

	//异常电表处理
	if(HI_ERR_SUCCESS != mrsParallelFailMeterBack(&stData))
	{
		return HI_ERR_SUCCESS;
	}
	
    return mrsRmReqJoinPool(&stData);
}

#endif


