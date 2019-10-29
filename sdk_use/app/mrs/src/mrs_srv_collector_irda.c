//*****************************************************************************
// 
//                  版权所有 (C), 1998-2011, 华为技术有限公司
// 
//*****************************************************************************
//  文 件 名   : mrs_srv_collector_irda.c
//  版 本 号   : V2.0 
//  作    者   : fengxiaomin/f00209182
//  生成日期   : 2012-08-28
//  功能描述   : II型采集器处理函数的实现
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2012-08-28
//    作    者 : fengxiaomin/f00209182
//    修改内容 : 创建文件 
// 
//*****************************************************************************
#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_fw_tools.h"
#include "mrs_srv_common.h"
#include "mrs_fw_nv.h"
#include "mrs_fw_log.h"
#include "mrs_fw_proto645.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_srv_res.h"
#include "mrs_srv_collector_irda.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_sta.h"
#include "mrs_srv_sta_searchmeter.h"
#include "mrs_srv_cltii_event.h"
#include "mrs_srv_collector.h"
#include "mrs_fw_proto698_45.h"


#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

/* BEGIN: PN: DTS2015042907048 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
#define MRS_IRDA_CTRL14_PARAM_EXTRA_LEN (8)

#define MRS_IRDA_CTRL14_PARAM_VALID(len, expect_len) \
    (((len) == (expect_len)) || ((len) == ((expect_len) + MRS_IRDA_CTRL14_PARAM_EXTRA_LEN)))
/* END:   PN: DTS2015042907048 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */


#define MRS_SM_REPORT_MAX_METERNUM      (6)

typedef struct
{
    HI_U32              ulMinDI;
    HI_U32              ulMaxDI;
    MRS_IR_645_DI_FUN   func;
} MRS_IRDA_DI_PAIR_STRU;

typedef struct
{
    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN];
    HI_U8 ucProtocol;
    HI_U8 ucReserved;
} MRS_IRDA_SM_ITEM_STRU;

typedef struct
{
    HI_BOOL bValid;
    HI_U8   ucTotal;
    HI_U8   ucSent;
    HI_U8   ucSeq;
    MRS_IRDA_SM_ITEM_STRU astItem[MRS_MAX_METERNUM];
} MRS_IRDA_SM_REPORT_STRU;


//*****************************************************************************
//全局变量
//*****************************************************************************
HI_PRV MRS_IRDA_SM_REPORT_STRU g_SMReportCtx = {HI_FALSE, 0, 0, 0, {{{0}, 0, 0}}};
//*****************************************************************************

// 厂家软件版本号
HI_U32 mrsIr645_11H_DI04A00101(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// 查询厂家软件版本扩展
HI_U32 mrsIr645_11H_DI04A00102(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// 终端复位次数
HI_U32 mrsIr645_11H_DI04A00201(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// 注册集中器逻辑地址
HI_U32 mrsIr645_11H_DI04A00300(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// II采搜表信息
HI_U32 mrsIr645_11H_DI04A00301(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// 采集器资产号
HI_U32 mrsIr645_11H_DI04A00303(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// 查询终端日历时钟
HI_U32 mrsIr645_11H_DI04A00503(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

//读采集器内电表数据
HI_U32 mrsIr645_11H_DI04A00601(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

//读采集器内电表曲线
HI_U32 mrsIr645_11H_DI04A00701(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// 查询采集器停电次数,采集器（上N次）掉电时刻，结束时刻
HI_U32 mrsIr645_11H_DI04A0080X(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// 查询事件总条数
HI_U32 mrsIr645_11H_DI04A09901(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// 查询事件内容
HI_U32 mrsIr645_11H_DI04A09902(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// MAC地址
HI_U32 mrsIr645_11H_DI04A09903(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);


// 注册集中器逻辑地址
HI_U32 mrsIr645_14H_DI04A00300(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// 采集器资产号
HI_U32 mrsIr645_14H_DI04A00303(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// 硬件初始化
HI_U32 mrsIr645_14H_DI04A00401(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// 数据初始化
HI_U32 mrsIr645_14H_DI04A00402(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// 参数初始化
HI_U32 mrsIr645_14H_DI04A00403(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

//设置终端日历时钟
HI_U32 mrsIr645_14H_DI04A00503(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// MAC地址写入
HI_U32 mrsIr645_14H_DI04A09903(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);


HI_U32 mrsFillProductVer(HI_CHAR * buf, HI_U16 buf_len);
HI_PRV HI_U32 mrsSetProductVerExtend(HI_U8 *pBuf, HI_U16 usBufLen);


#define DI_04A00101     (0x04A00101)
#define DI_04A00102     (0x04A00102)
#define DI_04A00201     (0x04A00201)
#define DI_04A00300     (0x04A00300)
#define DI_04A00301     (0x04A00301)
#define DI_04A00303     (0x04A00303)
#define DI_04A00503     (0x04A00503)
#define DI_04A00601     (0x04A00601)
#define DI_04A00701     (0x04A00701)
#define DI_04A00800     (0x04A00800)
#define DI_04A00801     (0x04A00801)
#define DI_04A0080A     (0x04A0080A)
#define DI_04A09901     (0x04A09901)
#define DI_04A09902     (0x04A09902)
#define DI_04A09903     (0x04A09903)

/* BEGIN: PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */
#define DI_04A09904     (0x04A09904) //II采485透明转发
/* END:   PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */

#define DI_04A00401     (0x04A00401)
#define DI_04A00402     (0x04A00402)
#define DI_04A00403     (0x04A00403)

MRS_IRDA_DI_PAIR_STRU MRS_IRDA_DI_PAIR_MAP_11H[] =
{
    {DI_04A00101, DI_04A00101, mrsIr645_11H_DI04A00101},  // 厂家软件版本号
	{DI_04A00102, DI_04A00102, mrsIr645_11H_DI04A00102},  // 厂家软件版本扩展
    {DI_04A00201, DI_04A00201, mrsIr645_11H_DI04A00201},  // 终端复位次数
    {DI_04A00300, DI_04A00300, mrsIr645_11H_DI04A00300},  // 注册集中器逻辑地址
    {DI_04A00301, DI_04A00301, mrsIr645_11H_DI04A00301},  // II采搜表信息
    {DI_04A00303, DI_04A00303, mrsIr645_11H_DI04A00303},  // 采集器资产号
	{DI_04A00503, DI_04A00503, mrsIr645_11H_DI04A00503},  // 终端日历时钟
	{DI_04A00601, DI_04A00601, mrsIr645_11H_DI04A00601},  // 读采集器内电表数据
	{DI_04A00701, DI_04A00701, mrsIr645_11H_DI04A00701},  // 读采集器内电表曲线
	{DI_04A00800, DI_04A0080A, mrsIr645_11H_DI04A0080X},  // 采集器停电次数,采集器（上N次）掉电时刻，结束时刻
    {DI_04A09901, DI_04A09901, mrsIr645_11H_DI04A09901},  // 查询事件总条数
    {DI_04A09902, DI_04A09902, mrsIr645_11H_DI04A09902},  // 查询事件内容
    {DI_04A09903, DI_04A09903, mrsIr645_11H_DI04A09903},  // MAC地址
};

MRS_IRDA_DI_PAIR_STRU MRS_IRDA_DI_PAIR_MAP_12H[] =
{
    {DI_04A00301, DI_04A00301, mrsIr645_11H_DI04A00301},  // II采搜表信息
};

MRS_IRDA_DI_PAIR_STRU MRS_IRDA_DI_PAIR_MAP_14H[] =
{
    {DI_04A00300, DI_04A00300, mrsIr645_14H_DI04A00300},  // 注册集中器逻辑地址
    {DI_04A00303, DI_04A00303, mrsIr645_14H_DI04A00303},  // 采集器资产号
    {DI_04A00401, DI_04A00401, mrsIr645_14H_DI04A00401},  // 硬件初始化
    {DI_04A00402, DI_04A00402, mrsIr645_14H_DI04A00402},  // 数据初始化
    {DI_04A00403, DI_04A00403, mrsIr645_14H_DI04A00403},  // 参数初始化
	{DI_04A00503, DI_04A00503, mrsIr645_14H_DI04A00503},  // 终端日历时钟
	{DI_04A09903, DI_04A09903, mrsIr645_14H_DI04A09903},  // 写入MAC地址
};

HI_U32 mrsAssembleFrame(HI_VOID **ppFrame, HI_U16 *pFrmLen, HI_U8 *pPayload, HI_U8 ucPayloadLen, 
								 MRS_COLLECTOR_645_FRAME_STRU *p645Frame, HI_U8 ucPreSize);

HI_PRV MRS_IRDA_SM_REPORT_STRU * mrsIrDAReportSMGetContext(HI_VOID)
{
    return &g_SMReportCtx;
}


HI_PRV HI_VOID mrsIrDAReportSMContextReset(HI_VOID)
{
    (hi_void)memset_s(&g_SMReportCtx, sizeof(g_SMReportCtx), 0, sizeof(g_SMReportCtx));
}

/* BEGIN: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/21 */


HI_U32 mrsIrGetTransmit(MRS_COLLECTOR_645_FRAME_STRU * pst645Frame)
{
    HI_U32 ulTransmitType = MRS_CLTII_IR_TRANSMIT_TYPE_DISABLE;
    MRS_COLLECTOR_CTX_STRU *pstCltIICtx = HI_NULL;

    if ((pst645Frame->ucCtrl == MRS_645_FRAME_CONTROL_READ_DATA) 
        && (MRS_COMBIN32(pst645Frame->ucDataRealm[0], pst645Frame->ucDataRealm[1], pst645Frame->ucDataRealm[2], pst645Frame->ucDataRealm[3])) == DI_04A09904)
    {
        ulTransmitType = MRS_CLTII_IR_TRANSMIT_TYPE_SPECIAL;
    }
    else
    {
        pstCltIICtx = mrsGetCollectorCtx();
        //判断NV开关是否打开
        if (pstCltIICtx->ucIrTrasmit645 == HI_TRUE)//NV打开，则转发
        {
            ulTransmitType = MRS_CLTII_IR_TRANSMIT_TYPE_ENABLE;
        }
    }
    
    return ulTransmitType;
}

HI_U32 mrsIrSendQueue(HI_U8 ucSubId,HI_PBYTE pucDatagram,HI_U8 ucDataRealmLen,HI_U8 ucBaudRateIndex)
{ 
    MRS_STA_ITEM *item = HI_NULL;
    MRS_STA_SRV_CTX_STRU * sta = mrsStaGetContext();
    
    item = (MRS_STA_ITEM *)mrsToolsMalloc(sizeof(MRS_STA_ITEM) + ucDataRealmLen);
    if (!item)
    {
        return  HI_ERR_MALLOC_FAILUE;
    }
    
    (hi_void)memset_s(item, sizeof(MRS_STA_ITEM) + ucDataRealmLen, 0, sizeof(MRS_STA_ITEM) + ucDataRealmLen);
    item->id = ID_MRS_CMD_IR_CMD;
    item->buf = (HI_U8 *)item + sizeof(MRS_STA_ITEM);
    item->from = MRS_STA_QUEUE_FROM_IR;
    item->ucbaudrate_index = ucBaudRateIndex;
    item->len = ucDataRealmLen;
    item->time_out = 20; // 2s
    item->sub_id = ucSubId;
    (hi_void)memcpy_s(item->buf, ucDataRealmLen, pucDatagram, ucDataRealmLen);
    item->ucProtocol = METER_PROTO_TRANSPARENT;
    item->bAutoStrategy = HI_TRUE;
    item->ucFrameTimeout = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameInterval(MRS_STA_RM_CFG_LOCAL));

    if (!mrsStaTryEnQueue(&(sta->stQueModule), item))
    {
        mrsToolsFree(item);
    }
    
    mrsStaNotifyQueue();
    return HI_ERR_SUCCESS;
            
}
/* END: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/21 */
#if defined(MRS_SRV_IR_698_45)
HI_U32 mrsIrSend698Queue(HI_U8 ucSubId,HI_PBYTE pucDatagram,HI_U8 ucDataRealmLen,HI_U8 ucBaudRateIndex)
{ 
    MRS_STA_ITEM *item = HI_NULL;
    MRS_STA_SRV_CTX_STRU * sta = mrsStaGetContext();
    
    item = (MRS_STA_ITEM *)mrsToolsMalloc(sizeof(MRS_STA_ITEM) + ucDataRealmLen);
    if (!item)
    {
        return  HI_ERR_MALLOC_FAILUE;
    }
    
    (hi_void)memset_s(item,sizeof(MRS_STA_ITEM) + ucDataRealmLen,0, sizeof(MRS_STA_ITEM) + ucDataRealmLen);
    item->id = ID_MRS_CMD_IR_CMD;
    item->buf = (HI_U8 *)item + sizeof(MRS_STA_ITEM);
    item->from = MRS_STA_QUEUE_FROM_IR;
    item->ucbaudrate_index = ucBaudRateIndex;
    item->len = ucDataRealmLen;
    item->time_out = 20; // 2s
    item->sub_id = ucSubId;
    (hi_void)memcpy_s(item->buf, ucDataRealmLen, pucDatagram, ucDataRealmLen);
	item->ucProtocol = METER_PROTO_698_45;
    item->bAutoStrategy = HI_TRUE;
    item->ucFrameTimeout = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameInterval(MRS_STA_RM_CFG_LOCAL));
    if (!mrsStaTryEnQueue(&(sta->stQueModule), item))
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_004, HI_DIAG_MT("IR STAmrsStaTryEnQueue"));
        mrsToolsFree(item);
    }
    
    mrsStaNotifyQueue();
    return HI_ERR_SUCCESS;
            
}
#endif
#if defined(MRS_SRV_IR_698_45)
#define MRS_STA_PRO_RECV_IR_DATA 64
HI_U8 g_ucIRCom[MRS_STA_PRO_RECV_IR_DATA] = { 0 };
HI_U16 g_ucIRBufSize = 0;
#endif
//*****************************************************************************
// 函数名称: mrsCollectorIrDARx
// 功能描述: 获取从红外接收过来的完整帧
//           
// 参数说明:
//    pstRxData [IN]接收到的红外数据
//                            
// 返 回 值: 
//    成功返回  HI_ERR_SUCCESS
//    失败返回  HI_ERR_FAILURE
//    继续接收数据 HI_ERR_CONTINUE
// 调用要求: 
// 调用举例: 
// 作    者: fengxiaomin/f00209182 [2012-08-28]
//*****************************************************************************
HI_U32 mrsCollectorIrDARx(HI_DMS_CHL_RX_S* pstRxData)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_ONE_RAW_FRAME_STRU stFrame = {0};
    MRS_COLLECTOR_645_FRAME_STRU *pst645Frame = HI_NULL;
    MRS_IR_645_DI_FUN pIrDIProc = HI_NULL;
/* BEGIN: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */
    HI_U32 ulTransmitType = MRS_CLTII_IR_TRANSMIT_TYPE_DISABLE;
    HI_U8 ucSubId = MRS_IR_SUBID_SPECIAL_TRANSMIT;
    HI_PBYTE pucDatagram = HI_NULL;
    HI_U8 ucDataRealmLen = 0;
    HI_U8 ucBaudRateIndex = 0;
/* END: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */
    
    HI_U8 *pData = HI_NULL;
    HI_U16 usDataLen = 0;

    if (pstRxData == HI_NULL)
    {
        ret = HI_ERR_FAILURE;
        return ret;
    }
    
    mrs645ProtoStreamInputCheck((HI_BOOL)pstRxData->bCanDiscardOldData, ID_MRS_IRDA_645BUF);

    // 获取到完整的帧数据
    ret = mrs645ProtoStreamInput(pstRxData->pPayload, pstRxData->usPayloadLen, &stFrame, ID_MRS_IRDA_645BUF);
    if (HI_ERR_SUCCESS != ret)
    {
#if defined(MRS_SRV_IR_698_45)
    	HI_U16 us698pos =0,us698Len=0;

	if((g_ucIRBufSize+ pstRxData->usPayloadLen)>MRS_STA_PRO_RECV_IR_DATA)
        {
            g_ucIRBufSize = 0;
        }
        (hi_void)memcpy_s(g_ucIRCom + g_ucIRBufSize, MRS_STA_PRO_RECV_IR_DATA,pstRxData->pPayload, pstRxData->usPayloadLen);
        g_ucIRBufSize += pstRxData->usPayloadLen;
        
	do
        {
        	ret = mrsFind698Frame(g_ucIRCom, g_ucIRBufSize, &us698pos, &us698Len);
        	if(HI_ERR_SUCCESS != ret)
        	{
            		return ret;
        	} 
		HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1002, HI_DIAG_MT("红外收到698.45数据"), g_ucIRCom+us698pos,us698Len);
            ret = mrsIrSend698Queue(MRS_IR_SUBID_TRANSMIT,g_ucIRCom+us698pos, us698Len,ucBaudRateIndex);
        }while(0);
	g_ucIRBufSize = 0;
#endif
        return ret;
    }
#if defined(MRS_SRV_IR_698_45)
	g_ucIRBufSize = 0;
#endif
    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_011, HI_DIAG_MT("收到红外645帧"));
    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1000, HI_DIAG_MT("数据"), stFrame.pucDatagram, stFrame.usDatagramSize);

    do
    {
        //解析645帧
        pst645Frame = (MRS_COLLECTOR_645_FRAME_STRU *)mrsToolsMalloc(sizeof(MRS_COLLECTOR_645_FRAME_STRU));
        if(pst645Frame == HI_NULL)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("no memory"));
            ret = HI_ERR_FAILURE;
            break;
        }

        (hi_void)memset_s(pst645Frame, sizeof(MRS_COLLECTOR_645_FRAME_STRU), 0, sizeof(MRS_COLLECTOR_645_FRAME_STRU));

        //检查645帧内容是否正确
        ret = mrsIrDA645Dec(stFrame.pucDatagram, stFrame.usDatagramSize, pst645Frame, MRS_CLTII_IR_FROM);
        if (ret == HI_ERR_SUCCESS) 
        {
            pIrDIProc = mrsIrFrameProc(pst645Frame->ucCtrl, pst645Frame->ucDataRealm);
            if (pIrDIProc)
            {
                ret = pIrDIProc(pst645Frame, (HI_PVOID *)&pData, &usDataLen, MRS_PREFIX_FE_SIZE);
                if (ret != HI_ERR_SUCCESS)
                {
                    mrsAssembleDFrame((HI_VOID **)&pData, &usDataLen, MRS_IR_ERR_NOT_EXIST, 
									  pst645Frame, MRS_PREFIX_FE_SIZE);
                }

                break;
            }
        }
/* BEGIN: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */
        ulTransmitType = mrsIrGetTransmit(pst645Frame);
        if (ulTransmitType == MRS_CLTII_IR_TRANSMIT_TYPE_DISABLE)//不需要转发
        {
            break;
        }
      
        if (mrsSrvQueueLength(&(mrsStaGetContext()->stQueModule.stMrQueue)) != 0)//队列不为空
        {
            mrsAssembleDFrame((HI_VOID **)&pData, &usDataLen, MRS_IR_ERR_BUSY, 
							  pst645Frame, MRS_PREFIX_FE_SIZE);
            ret = HI_ERR_BUSY;
            break;
        }
       
        if (ulTransmitType == MRS_CLTII_IR_TRANSMIT_TYPE_SPECIAL)//特殊DI转发
        {
            ucSubId = MRS_IR_SUBID_SPECIAL_TRANSMIT;
            ucBaudRateIndex = pst645Frame->ucDataRealm[MRS_645_FRAME_DATA_DI_SIZE];
            pucDatagram = pst645Frame->ucDataRealm + MRS_645_FRAME_DATA_DI_SIZE + sizeof(HI_U8);
            ucDataRealmLen = (HI_U8)(pst645Frame->ucDataRealmLen - MRS_645_FRAME_DATA_DI_SIZE - sizeof(HI_U8));
           
        }
        else //if(ulTransmitType == MRS_CLTII_IR_TRANSMIT_TYPE_NOMAL)
        {
            ucSubId = MRS_IR_SUBID_TRANSMIT;
            pucDatagram = stFrame.pucDatagram;
            ucDataRealmLen = (HI_U8)stFrame.usDatagramSize; 
            ucBaudRateIndex = 0;//按照默认波特率发送
            
        }
        ret = mrsIrSendQueue(ucSubId, pucDatagram,ucDataRealmLen,ucBaudRateIndex);
/* END: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */
    } while (0);

    if ((usDataLen > 0) && (pst645Frame && !mrsToolsBroadCastAddr(pst645Frame->ucAddr)))
    {
        // 适配红外掌机通讯 延时100ms响应
        HI_MDM_Sleep(100);
        
        MRS_SendMrData(pData, usDataLen, HI_DMS_CHL_UART_PORT_IR);
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1003, HI_DIAG_MT("ANSWER"), pData, usDataLen);
    }

    mrsToolsFree(stFrame.pucDatagram);
    mrsToolsFree(pData);
    mrsToolsFree(pst645Frame);

    return ret;
}

//*****************************************************************************
// 函数名称: mrsIrDA645Dec
// 功能描述: 解析II型采集器红外接收的完整645帧
//           
// 参数说明:
//    pucData     [IN] 645帧
//    usDataSize  [IN] 645帧长度
//    pst645Frame [OUT]645帧结构
//                            
// 返 回 值: 
//    成功返回  HI_ERR_SUCCESS
//    失败返回  HI_ERR_FAILURE
//
// 调用要求: 
// 调用举例: 
// 作    者: fengxiaomin/f00209182 [2012-08-29]
//*****************************************************************************
HI_U32 mrsIrDA645Dec(HI_U8* pucData, HI_U16 usDataSize, MRS_COLLECTOR_645_FRAME_STRU *pst645Frame, HI_U8 ucFrom)
{
    HI_U8 *pucBuffer = pucData;

    HI_UNREF_PARAM(usDataSize);

    if (pucData == HI_NULL || pst645Frame == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }

    pucBuffer++;
    (hi_void)memcpy_s(pst645Frame->ucAddr, HI_METER_ADDR_LEN, pucBuffer, HI_METER_ADDR_LEN); //采集器地址

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1001, HI_DIAG_MT("地址"), pst645Frame->ucAddr, HI_METER_ADDR_LEN);

    pucBuffer += HI_METER_ADDR_LEN;
    pucBuffer++;
    pst645Frame->ucCtrl = *(pucBuffer++); //控制码
    pst645Frame->ucDataRealmLen = *(pucBuffer++);
	if(sizeof(pst645Frame->ucDataRealm) >= pst645Frame->ucDataRealmLen){
		(hi_void)memcpy_s(pst645Frame->ucDataRealm, sizeof(pst645Frame->ucDataRealm), pucBuffer, pst645Frame->ucDataRealmLen);	
	}	 
    mrs645DataDecode(pst645Frame->ucDataRealm, pst645Frame->ucDataRealmLen);

	return mrsIrDACheckAddr(pst645Frame->ucAddr, ucFrom);
}

//*****************************************************************************
// 函数名称: mrsIrDACheckAddr
// 功能描述: II型采集器判断资产号
//           
// 参数说明:
//    aucAddr     [IN] 收到的命令中的地址
//    ucFrom      [IN] 命令来源
//                            
// 返 回 值: 
//    正确返回  HI_ERR_SUCCESS
//    错误返回  HI_ERR_FAILURE
//
// 调用要求: 
// 调用举例: 
// 作    者: fengxiaomin/f00209182 [2015-11-16]
//*****************************************************************************
HI_U32 mrsIrDACheckAddr(HI_U8 aucAddr[], HI_U8 ucFrom)
{
    HI_U8 aucCltAddr[HI_METER_ADDR_LEN] = {0};
	
    mrsIrDAGetCltAddr(aucCltAddr);
	if (MRS_CLTII_IR_FROM == ucFrom)
	{
		if (!mrsMeterAddrMatch(aucAddr, aucCltAddr) && !mrsToolsBroadCastAddr(aucAddr))
		{
			return HI_ERR_FAILURE;
		}
	}
	else
	{
		if (mrsToolsCollectorAddr(aucAddr) || !mrsMeterAddrMatch(aucAddr, aucCltAddr))
		{
			return HI_ERR_FAILURE;
		}
	}

	return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: mrsIrDAGetDIFunc
// 功能描述: II型采集器获取红外命令DI对应的处理函数
//           
// 参数说明:
//    ucCtrl      [IN] 645帧中的控制字
//    pDataRealm  [IN] 645帧中的数据域
//    pFunc       [OUT]获取到的处理函数
//                            
// 返 回 值: 
//    成功返回  HI_ERR_SUCCESS
//    失败返回  HI_ERR_FAILURE
//
// 调用要求: 
// 调用举例: 
// 作    者: fengxiaomin/f00209182 [2015-11-16]
//*****************************************************************************
HI_U32 mrsIrDAGetDIFunc(HI_U8 ucCtrl, HI_U8 *pDataRealm, HI_U32 *pFunc)
{
    MRS_IRDA_DI_PAIR_STRU * pair = HI_NULL;
    HI_S32 size = 0;
    HI_S32 index;
    HI_U32 di = 0;

    if (HI_NULL == pDataRealm)
    {
        return HI_ERR_FAILURE;
    }

    switch (ucCtrl)
    {
        case MRS_645_FRAME_CONTROL_READ_DATA:
            pair = MRS_IRDA_DI_PAIR_MAP_11H;
            size = MRS_TOOLS_ALEN(MRS_IRDA_DI_PAIR_MAP_11H);
            break;

        case MRS_645_FRAME_CONTROL_READ_FOLLOW:
            pair = MRS_IRDA_DI_PAIR_MAP_12H;
            size = MRS_TOOLS_ALEN(MRS_IRDA_DI_PAIR_MAP_12H);
            break;

        case MRS_645_FRAME_CONTROL_WRITE_DATA:
            pair = MRS_IRDA_DI_PAIR_MAP_14H;
            size = MRS_TOOLS_ALEN(MRS_IRDA_DI_PAIR_MAP_14H);
            break;

        default:
            return HI_ERR_NOT_FOUND;
    }

    di = MRS_COMBIN32(pDataRealm[0], pDataRealm[1], pDataRealm[2], pDataRealm[3]);

    for (index = 0; index < size; index++)
    {
        if ((di >= pair[index].ulMinDI) && (di <= pair[index].ulMaxDI))
        {
            *pFunc = (HI_U32)pair[index].func;
            return HI_ERR_SUCCESS;
        }
    }

    return HI_ERR_NOT_FOUND;
}

//*****************************************************************************
// 函数名称: mrsIrFrameProc
// 功能描述: 对每一个接收的完整帧进行相应的处理
//           
// 参数说明:
//    ucCtrl      [IN]645帧中控制字
//    pDataRealm  [IN]645帧中数据域地址
//
// 返 回 值: 
//    找到相应的处理函数返回函数指针
//    找不到相应的处理函数返回0
//
// 调用要求: 
// 调用举例: 
// 作    者: fengxiaomin/f00209182 [2012-08-29]
//*****************************************************************************
MRS_IR_645_DI_FUN mrsIrFrameProc(HI_U8 ucCtrl, HI_U8 *pDataRealm)
{
    MRS_IR_645_DI_FUN pIrDIProc = HI_NULL;

	mrsIrDAGetDIFunc(ucCtrl, pDataRealm, (HI_U32*)&pIrDIProc);
    if ((!pIrDIProc) || (ucCtrl != MRS_645_FRAME_CONTROL_READ_FOLLOW))
    {
        mrsIrDAReportSMContextReset();
    }

    return pIrDIProc;
}


HI_VOID mrsIrDAGetCltAddr(HI_U8 *pAddr)
{
    HI_NV_FTM_COLLECT_PARAM_FILE_S stNv;

    (hi_void)memset_s(&stNv, sizeof(stNv), 0, sizeof(stNv));

    if (HI_ERR_SUCCESS == HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &stNv, sizeof(stNv)))
    {
        (hi_void)memcpy_s(pAddr, HI_METER_ADDR_LEN, stNv.CLTAddress, HI_METER_ADDR_LEN);
    }
    else
    {
        (hi_void)memcpy_s(pAddr, HI_METER_ADDR_LEN, MRS_COLLECTOR_ADDR, HI_METER_ADDR_LEN);
    }
}


//*****************************************************************************
// 函数名称: mrsAssembleFrame
// 功能描述: 组织645回应帧
//           
// 参数说明:
//    ppFrame     [OUT]组织成功的帧
//    pFrmLen     [OUT]组织成功的帧长度
//    pPayload    [IN] 负载
//    ucPayloadLen[IN] 负载数据长度
//    p645Frame   [IN] 接收到的报文
//                            
// 返 回 值: 
//    组帧成功返回  HI_ERR_SUCCESS
//    组帧失败返回  HI_ERR_FAILURE
//
// 调用要求: 
// 调用举例: 
// 作    者: fengxiaomin/f00209182 [2012-08-29]
// 作    者: cuiate/c00233580 [2015-05-30]
//*****************************************************************************
HI_U32 mrsAssembleFrame(HI_VOID **ppFrame, HI_U16 *pFrmLen, HI_U8 *pPayload, HI_U8 ucPayloadLen, 
								 MRS_COLLECTOR_645_FRAME_STRU *p645Frame, HI_U8 ucPreSize)
{
    HI_U16 usBufLength = MRS_645_MIN_LENGTH + ucPreSize + ucPayloadLen;
    HI_U8 * pBuffer = HI_NULL;
    HI_U16 usLength = 0;
    HI_U8 i = 0;

    if (((ucPayloadLen > 0) && (!pPayload)) || (!p645Frame))
    {
        return HI_ERR_FAILURE;
    }

    pBuffer = mrsToolsMalloc(usBufLength);
    if (!pBuffer)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pBuffer, usBufLength, 0, usBufLength);
    // 前导FE
    for (i = 0; i < ucPreSize; i++)
    {
        pBuffer[usLength++] = 0xFE;
    }

    // HEAD
    pBuffer[usLength++] = MRS_645_FRAME_START_FLG;

    // ADDR
    mrsIrDAGetCltAddr(pBuffer + usLength);
    usLength += HI_METER_ADDR_LEN;

    // HEAD 2
    pBuffer[usLength++] = MRS_645_FRAME_START_FLG;

    // CTRL
    pBuffer[usLength++] = p645Frame->ucCtrl | MRS_645_FRAME_CONTROL_DIR_UP;

    // LENGTH
    pBuffer[usLength++] = ucPayloadLen;

    // DATA
    for (i = 0; i < ucPayloadLen; i++)
    {
        pBuffer[usLength++] = pPayload[i] + MRS_645_FRAME_HEX33;
    }

    // CS
    pBuffer[usLength] = mrsToolsCalcCheckSum(pBuffer + ucPreSize, usLength - ucPreSize);
    usLength++;

    // END
    pBuffer[usLength++] = MRS_645_FRAME_END_FLG;

    *ppFrame = pBuffer;
    *pFrmLen = usLength;

    return HI_ERR_SUCCESS;
}


HI_U32 mrsAssembleDFrame(HI_VOID **ppFrame, HI_U16 *pFrmLen, HI_U8 ucErrCode, 
								   MRS_COLLECTOR_645_FRAME_STRU *p645Frame, HI_U8 ucPreSize)
{
    if (!p645Frame)
    {
        return HI_ERR_FAILURE;
    }

    p645Frame->ucCtrl |= MRS_645_FRAME_CONTROL_DENY_MASK;

    return mrsAssembleFrame(ppFrame, pFrmLen, &ucErrCode, sizeof(ucErrCode), p645Frame, ucPreSize);
}


HI_U32 mrsFillProductVer(HI_CHAR * buf, HI_U16 buf_len)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_CHAR * strVer[4] = {MRS_V_VERSION_STRING, MRS_R_VERSION_STRING, MRS_C_VERSION_STRING, MRS_SPC_VERSION_STRING};
    HI_U16 width[4] = {MRS_V_VERSION_WIDTH, MRS_R_VERSION_WIDTH, MRS_C_VERSION_WIDTH, MRS_SPC_VERSION_WIDTH};
    HI_U32 i = 0;
    HI_U16 maxLen = 0;
	NV_MRS_USER_VER_S stVer;

    (hi_void)memset_s(&stVer, sizeof(stVer), 0, sizeof(stVer));
	ulRet = mrsGetUserVerNvCfg(&stVer);
	if (HI_ERR_SUCCESS != ulRet)
	{
		return ulRet;
	}

    for (i = 0; i < MRS_TOOLS_ALEN(width); i++)
    {
        HI_CHAR * temp = HI_NULL;

        maxLen += width[i] + (HI_U16)strlen(strVer[i]);
        temp = mrsFindStrEnd(stVer.aucUserVersion, strVer[i]);
        if (temp && maxLen < buf_len)
        {
            strncat_s(buf, buf_len, strVer[i], strlen(strVer[i]));
            strncat_s(buf, buf_len, temp, width[i]);
        }
    }
    
    return HI_ERR_SUCCESS;
}

// 厂家软件版本号
HI_U32 mrsIr645_11H_DI04A00101(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    HI_U8 payload[36];
	HI_U32 ulDI = DI_04A00101;
	NV_MRS_USER_VER_S stVer;
    (hi_void)memset_s(payload, sizeof(payload), 0, sizeof(payload));
    (hi_void)memcpy_s(payload, sizeof(payload), &ulDI, MRS_SRV_645_DATA_INDEX_SIZE);
    //mrsFillProductVer((HI_CHAR *)(payload + sizeof(HI_U32)), (HI_U16)(sizeof(payload) - sizeof(HI_U32)));
	(hi_void)memset_s(&stVer, sizeof(stVer), 0, sizeof(stVer));
	mrsGetUserVerNvCfg(&stVer);
	(hi_void)memcpy_s(payload+sizeof(HI_U32),HI_USER_VER_LENGTH,stVer.aucUserVersion,HI_USER_VER_LENGTH);
	mrsHexInvert(payload + sizeof(HI_U32), (HI_U16)(sizeof(payload) - sizeof(HI_U32)));
	
    return mrsAssembleFrame(ppOutParam, pOutParamLen, payload, sizeof(payload), 
							(MRS_COLLECTOR_645_FRAME_STRU*)param, ucPreSize);
}

//*****************************************************************************
// 函数名称: mrsSetProductVerExtend
// 功能描述: II采设置扩展的厂家软件版本
//           
// 参数说明:
//    无
//                            
// 返 回 值: 
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求: 
// 调用举例: 
// 作    者: fengxiaomin/f00209182 [2015-10-30]
//*****************************************************************************
HI_U32 mrsSetProductVerExtend(HI_U8 *pBuf, HI_U16 usBufLen)
{
	HI_U32 ulRet = HI_ERR_SUCCESS;
	HI_U8 ucOffset = 0;
	HI_U8 cCltVer[MRS_CLTII_EXTEND_VER_LEN] = {0,0,0,0,'2','C'};
	HI_MDM_SYS_INFO_S *pstVerInfo = HI_NULL;
	
    pstVerInfo = (HI_MDM_SYS_INFO_S *)HI_MDM_GetSysInfo();
	if (!pstVerInfo)
	{
		return HI_ERR_FAILURE;
	}
	
    (hi_void)memcpy_s(pBuf, usBufLen, cCltVer, MRS_CLTII_EXTEND_VER_LEN);
	ucOffset += MRS_CLTII_EXTEND_VER_LEN;
    (hi_void)memcpy_s(&pBuf[ucOffset], usBufLen - ucOffset, pstVerInfo->szManuCode, MRS_MANU_CODE_LEN);
	ucOffset += MRS_MANU_CODE_LEN;
    mrsIntToBcd((HI_U32)pstVerInfo->usDay, &pBuf[ucOffset], sizeof(HI_U8));
    ucOffset++;
    mrsIntToBcd((HI_U32)pstVerInfo->usMonth, &pBuf[ucOffset], sizeof(HI_U8));
    ucOffset++;
    mrsIntToBcd((HI_U32)pstVerInfo->usYear, &pBuf[ucOffset], sizeof(HI_U8));
    ucOffset++;
    (hi_void)memcpy_s(&pBuf[ucOffset], usBufLen - ucOffset, &pstVerInfo->usSwVer, sizeof(pstVerInfo->usSwVer));
	ucOffset += sizeof(pstVerInfo->usSwVer);
	pBuf[ucOffset++] = pstVerInfo->ucBootVer;

	return ulRet;
}

//*****************************************************************************
// 函数名称: mrsIr645_11H_DI04A00102
// 功能描述: 查询厂家软件版本扩展
//           
// 参数说明:
//    无
//                            
// 返 回 值: 
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求: 
// 调用举例: 
// 作    者: fengxiaomin/f00209182 [2015-10-30]
//*****************************************************************************
HI_U32 mrsIr645_11H_DI04A00102(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
	HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 payload[36];
	HI_U32 ulDI = DI_04A00102;

    (hi_void)memset_s(payload, sizeof(payload), 0, sizeof(payload));
    (hi_void)memcpy_s(payload, sizeof(payload), &ulDI, MRS_SRV_645_DATA_INDEX_SIZE);
    ulRet = mrsSetProductVerExtend(payload + sizeof(HI_U32), (HI_U16)(sizeof(payload) - sizeof(HI_U32)));
	if (HI_ERR_SUCCESS != ulRet)
	{
		return ulRet;
	}

    return mrsAssembleFrame(ppOutParam, pOutParamLen, payload, sizeof(payload), 
							(MRS_COLLECTOR_645_FRAME_STRU*)param, ucPreSize);
}

// 终端复位次数
HI_U32 mrsIr645_11H_DI04A00201(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    HI_U8 payload[6] = {0x01, 0x02, 0xA0, 0x04, 0x00, 0x00};
    HI_U16 report_times = 0;    
    HI_U32 real_times = HI_MDM_GetRelativeRstTimes(); 
    
    // 复位次数最大值为9999(红外约束)
    report_times = real_times % 10000;
    mrsIntToBcd(report_times, payload + MRS_SRV_645_DATA_INDEX_SIZE, 2);

    return mrsAssembleFrame(ppOutParam, pOutParamLen, payload, sizeof(payload), 
							(MRS_COLLECTOR_645_FRAME_STRU*)param, ucPreSize);
}


// 注册集中器逻辑地址
HI_U32 mrsIr645_11H_DI04A00300(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    HI_U8 payload[] = {0x00, 0x03, 0xA0, 0x04, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00};
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_NV_FTM_COLLECT_PARAM_FILE_S nv;

    (hi_void)memset_s(&nv, sizeof(nv), 0, sizeof(nv));

    ret = HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &nv, sizeof(nv));
    if (HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memcpy_s(payload + MRS_SRV_645_DATA_INDEX_SIZE, sizeof(payload) - MRS_SRV_645_DATA_INDEX_SIZE, nv.CCTAddress, sizeof(nv.CCTAddress));
    *(payload + MRS_SRV_645_DATA_INDEX_SIZE + sizeof(nv.CCTAddress)) = nv.CCT_register;

    return mrsAssembleFrame(ppOutParam, pOutParamLen, payload, sizeof(payload), 
							(MRS_COLLECTOR_645_FRAME_STRU*)param, ucPreSize);
}


// II采搜表信息
HI_U32 mrsIr645_11H_DI04A00301(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    MRS_COLLECTOR_645_FRAME_STRU* frame = (MRS_COLLECTOR_645_FRAME_STRU*)param;
    MRS_IRDA_SM_REPORT_STRU * sm_report = mrsIrDAReportSMGetContext();
    HI_U8 * payload = HI_NULL;
    HI_U8 payload_len = 0;
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 i;
    HI_U8 this_num;
    HI_U8 seq = 0;
    HI_U8 seq_len = 0;
	HI_U32 ulDI = DI_04A00301;
    HI_U8 ucOffset = 0;

    if (HI_NULL == param)
    {
        return HI_ERR_FAILURE;
    }

    if (frame->ucCtrl == MRS_645_FRAME_CONTROL_READ_DATA)
    {
        MRS_STA_SEARCH_CTX * pstSmCtx = mrsStaSearchCtx();
        MRS_SEARCH_METERLIST_STRU * pstMeterList = &(pstSmCtx->stMeterList);
        MRS_METER_ITEM_S * pItem = HI_NULL;

        pItem = pstMeterList->astMeter;
        for (i = 0; i < pstMeterList->ucMeterNum; i++, pItem++)
        {
            mrsMeterAddrPreProc(sm_report->astItem[i].aucMeterAddr, pItem->ucMeterAddr, pItem->ucProtocol);
            sm_report->astItem[i].ucProtocol = pItem->ucProtocol;
        }
/* BEGIN: PN: DTS2015061608880 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/17 */
        sm_report->ucTotal = pstMeterList->ucMeterNum;

        if (pstMeterList->ucMeterNum > MRS_SM_REPORT_MAX_METERNUM)
        {
            sm_report->bValid = HI_TRUE;
        }
/* END:   PN: DTS2015061608880 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/17 */
    }
    else if (frame->ucCtrl == MRS_645_FRAME_CONTROL_READ_FOLLOW)
    {
        if (HI_TRUE != sm_report->bValid)
        {
            return HI_ERR_FAILURE;
        }

        if (frame->ucDataRealmLen == (MRS_645_FRAME_DATA_DI_SIZE + 1))
        {
            seq = frame->ucDataRealm[MRS_645_FRAME_DATA_DI_SIZE];
            if ((seq == 0) || ((seq != sm_report->ucSeq) && (seq != sm_report->ucSeq + 1)))
            {
                return HI_ERR_FAILURE;
            }

            sm_report->ucSent = (HI_U8)(MRS_SM_REPORT_MAX_METERNUM * seq);
            seq_len = 1;
        }
        else if (frame->ucDataRealmLen != MRS_645_FRAME_DATA_DI_SIZE)
        {
            return HI_ERR_FAILURE;
        }

        if (sm_report->ucSent >= sm_report->ucTotal)
        {
            return HI_ERR_FAILURE;
        }
    }
    else
    {
        return HI_ERR_FAILURE;
    }

    this_num = MRS_MIN(sm_report->ucTotal - sm_report->ucSent, MRS_SM_REPORT_MAX_METERNUM);
    
    payload_len = (HI_U8)(MRS_SRV_645_DATA_INDEX_SIZE + 1 + 7 * this_num + seq_len);
    payload = mrsToolsMalloc(payload_len);
    if (HI_NULL == payload)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memcpy_s(payload, payload_len, &ulDI, MRS_SRV_645_DATA_INDEX_SIZE);
    mrsIntToBcd(sm_report->ucTotal, &payload[4], 1);

    ucOffset = MRS_SRV_645_DATA_INDEX_SIZE + 1;
    for (i = 0; i < this_num; i++)
    {
        (hi_void)memcpy_s(payload + ucOffset, payload_len - ucOffset, sm_report->astItem[i + sm_report->ucSent].aucMeterAddr, HI_METER_ADDR_LEN);
        ucOffset += HI_METER_ADDR_LEN;
        *(payload + ucOffset) = sm_report->astItem[i + sm_report->ucSent].ucProtocol;
        ucOffset++;
    }

    if (seq_len > 0)
    {
        *(payload + payload_len - 1) = seq;
        sm_report->ucSeq = seq;
    }

    sm_report->ucSent += this_num;
    if (sm_report->ucSent < sm_report->ucTotal)
    {
        frame->ucCtrl |= MRS_645_CONTROL_FOLLOWBIT_ONE;
    }

    ret = mrsAssembleFrame(ppOutParam, pOutParamLen, payload, payload_len, frame, ucPreSize);
    mrsToolsFree(payload);

    return ret;
}


// 采集器资产号
HI_U32 mrsIr645_11H_DI04A00303(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    HI_U8 payload[10] = {0x03, 0x03, 0xA0, 0x04, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB};

    mrsIrDAGetCltAddr(payload + MRS_SRV_645_DATA_INDEX_SIZE);

    return mrsAssembleFrame(ppOutParam, pOutParamLen, payload, sizeof(payload), 
							(MRS_COLLECTOR_645_FRAME_STRU*)param, ucPreSize);
}

//*****************************************************************************
// 函数名称: mrsIr645_11H_DI04A00503
// 功能描述: 查询终端日历时钟
//           
// 参数说明:
//    无
//                            
// 返 回 值: 
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求: 
// 调用举例: 
// 作    者: fengxiaomin/f00209182 [2015-10-30]
//*****************************************************************************
HI_U32 mrsIr645_11H_DI04A00503(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    HI_UNREF_PARAM(param);
    HI_UNREF_PARAM(ppOutParam);
    HI_UNREF_PARAM(pOutParamLen);
    HI_UNREF_PARAM(ucPreSize);
    return HI_ERR_NOT_SUPPORT;
}

//*****************************************************************************
// 函数名称: mrsIr645_11H_DI04A00601
// 功能描述: 读采集器内电表数据
//           
// 参数说明:
//    无
//                            
// 返 回 值: 
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求: 
// 调用举例: 
// 作    者: fengxiaomin/f00209182 [2015-11-07]
//*****************************************************************************
HI_U32 mrsIr645_11H_DI04A00601(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    HI_UNREF_PARAM(param);
    HI_UNREF_PARAM(ppOutParam);
    HI_UNREF_PARAM(pOutParamLen);
    HI_UNREF_PARAM(ucPreSize);
    return HI_ERR_NOT_SUPPORT;
}

//*****************************************************************************
// 函数名称: mrsIr645_11H_DI04A00701
// 功能描述: 读采集器内电表曲线
//           
// 参数说明:
//    无
//                            
// 返 回 值: 
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求: 
// 调用举例: 
// 作    者: fengxiaomin/f00209182 [2015-11-07]
//*****************************************************************************
HI_U32 mrsIr645_11H_DI04A00701(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    HI_UNREF_PARAM(param);
    HI_UNREF_PARAM(ppOutParam);
    HI_UNREF_PARAM(pOutParamLen);
    HI_UNREF_PARAM(ucPreSize);
    return HI_ERR_NOT_SUPPORT;
}

//*****************************************************************************
// 函数名称: mrsIr645_11H_DI04A0080X
// 功能描述: 查询采集器停电次数,采集器（上N次）掉电时刻，结束时刻
//           
// 参数说明:
//    无
//                            
// 返 回 值: 
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求: 
// 调用举例: 
// 作    者: fengxiaomin/f00209182 [2015-10-30]
//*****************************************************************************
HI_U32 mrsIr645_11H_DI04A0080X(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    HI_UNREF_PARAM(param);
    HI_UNREF_PARAM(ppOutParam);
    HI_UNREF_PARAM(pOutParamLen);
    HI_UNREF_PARAM(ucPreSize);
    return HI_ERR_NOT_SUPPORT;
}

// 查询事件总条数
HI_U32 mrsIr645_11H_DI04A09901(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    MRS_STA_SEARCH_CTX * pstSmCtx = mrsStaSearchCtx();
    HI_U8 payload[5] = {0x01, 0x99, 0xA0, 0x04, 0x00};

    mrsIntToBcd(pstSmCtx->stEvent.ucNum, payload + MRS_SRV_645_DATA_INDEX_SIZE, 1);

    return mrsAssembleFrame(ppOutParam, pOutParamLen, payload, sizeof(payload), 
							(MRS_COLLECTOR_645_FRAME_STRU*)param, ucPreSize);
}

// 查询事件内容
HI_U32 mrsIr645_11H_DI04A09902(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    MRS_COLLECTOR_645_FRAME_STRU* frame = (MRS_COLLECTOR_645_FRAME_STRU*)param;
    MRS_STA_SEARCH_CTX * pstSmCtx = mrsStaSearchCtx();
    MRS_SEARCH_EVENT_ITEM * item = HI_NULL;
    HI_U8 payload[11] = {0x02, 0x99, 0xA0, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    HI_U8 index = 0;

    if (HI_NULL == param)
    {
        return HI_ERR_FAILURE;
    }

    index = *(frame->ucDataRealm + MRS_SRV_645_DATA_INDEX_SIZE);
    index = (HI_U8)BCD2INT(index);

    if ((index == 0) || (index > pstSmCtx->stEvent.ucNum))
    {
        return HI_ERR_FAILURE;
    }

    item = pstSmCtx->stEvent.astItem;

    *(payload + MRS_SRV_645_DATA_INDEX_SIZE) = *(frame->ucDataRealm + MRS_SRV_645_DATA_INDEX_SIZE);
    (hi_void)memcpy_s(&payload[MRS_SRV_645_DATA_INDEX_SIZE + 1],
        sizeof(payload) - MRS_SRV_645_DATA_INDEX_SIZE - 1,
        &(item[index-1].ulTime), sizeof(item[index-1].ulTime));
    (hi_void)memcpy_s(&payload[MRS_SRV_645_DATA_INDEX_SIZE + 5],
        sizeof(payload) - MRS_SRV_645_DATA_INDEX_SIZE - 5,
        &(item[index-1].usEvent), sizeof(item[index-1].usEvent));

    return mrsAssembleFrame(ppOutParam, pOutParamLen, payload, sizeof(payload), frame, ucPreSize);
}

// MAC地址
HI_U32 mrsIr645_11H_DI04A09903(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    HI_U8 payload[10] = {0x03, 0x99, 0xA0, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    HI_MDM_GetLocalMacAddr(payload + MRS_SRV_645_DATA_INDEX_SIZE);
    return mrsAssembleFrame(ppOutParam, pOutParamLen, payload, sizeof(payload), 
							(MRS_COLLECTOR_645_FRAME_STRU*)param, ucPreSize);
}


/* BEGIN: PN: DTS2015042907048 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
// 注册集中器逻辑地址
HI_U32 mrsIr645_14H_DI04A00300(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    MRS_COLLECTOR_645_FRAME_STRU* frame = (MRS_COLLECTOR_645_FRAME_STRU*)param;
    HI_NV_FTM_COLLECT_PARAM_FILE_S nv;
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U16 offset = 0;

    if ((HI_NULL == param) 
        || !MRS_IRDA_CTRL14_PARAM_VALID(frame->ucDataRealmLen, sizeof(nv.CCTAddress) + MRS_645_FRAME_DATA_DI_SIZE))
    {
        return HI_ERR_FAILURE;
    }

    offset = frame->ucDataRealmLen - sizeof(nv.CCTAddress);

    (hi_void)memset_s(&nv, sizeof(nv), 0, sizeof(nv));
    ret = HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &nv, sizeof(nv));
    if (HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memcpy_s(nv.CCTAddress, sizeof(nv.CCTAddress), frame->ucDataRealm + offset, sizeof(nv.CCTAddress));
    nv.CCT_register = 1;

    ret = HI_MDM_NV_Write(HI_NV_FTM_COLLECT_PARAM_FILE, &nv, sizeof(nv));
    if (HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }

    return mrsAssembleFrame(ppOutParam, pOutParamLen, HI_NULL, 0, frame, ucPreSize);
}
/* END:   PN: DTS2015042907048 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */


/* BEGIN: PN: DTS2015042907048 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
// 采集器资产号
HI_U32 mrsIr645_14H_DI04A00303(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    MRS_COLLECTOR_645_FRAME_STRU* frame = (MRS_COLLECTOR_645_FRAME_STRU*)param;
    HI_NV_FTM_COLLECT_PARAM_FILE_S nv;
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U16 offset = 0;

    if ((HI_NULL == param) 
        || !MRS_IRDA_CTRL14_PARAM_VALID(frame->ucDataRealmLen, sizeof(nv.CLTAddress) + MRS_645_FRAME_DATA_DI_SIZE))
    {
        return HI_ERR_FAILURE;
    }

    offset = frame->ucDataRealmLen - sizeof(nv.CLTAddress);

    if (!mrsToolsNormalAddr(frame->ucDataRealm + offset) && !mrsToolsCollectorAddr(frame->ucDataRealm + offset))
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memset_s(&nv, sizeof(nv), 0, sizeof(nv));
    ret = HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &nv, sizeof(nv));
    if (HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memcpy_s(nv.CLTAddress, sizeof(nv.CLTAddress), frame->ucDataRealm + offset, sizeof(nv.CLTAddress));

    ret = HI_MDM_NV_Write(HI_NV_FTM_COLLECT_PARAM_FILE, &nv, sizeof(nv));
    if (HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }
    
    mrsSetCollectorAddr(nv.CLTAddress);

    return mrsAssembleFrame(ppOutParam, pOutParamLen, HI_NULL, 0, frame, ucPreSize);
}
/* END:   PN: DTS2015042907048 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */


/* BEGIN: PN: DTS2015042907048 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
// 硬件初始化
HI_U32 mrsIr645_14H_DI04A00401(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_COLLECTOR_645_FRAME_STRU* frame = (MRS_COLLECTOR_645_FRAME_STRU*)param;
    static HI_U32 receive = 0;

    if ((HI_NULL == param) 
        || !MRS_IRDA_CTRL14_PARAM_VALID(frame->ucDataRealmLen, MRS_645_FRAME_DATA_DI_SIZE))
    {
        return HI_ERR_FAILURE;
    }

    ret = mrsAssembleFrame(ppOutParam, pOutParamLen, HI_NULL, 0, frame, ucPreSize);
    if (ret == HI_ERR_SUCCESS)
    {
        //启动执行硬件初始化定时器
        if (receive != 0)
        {
            return ret;
        }
        
        MRS_StartTimer(MRS_COLLECTOR_TIMER_IRDA_RESET, MRS_COLLECTOR_TIME_IRDA_RESET, HI_SYS_TIMER_ONESHOT);
        receive = 1;
    }

    return ret;
}
/* END:   PN: DTS2015042907048 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */


/* BEGIN: PN: DTS2015042907048 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
// 数据初始化
HI_U32 mrsIr645_14H_DI04A00402(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    MRS_COLLECTOR_645_FRAME_STRU* frame = (MRS_COLLECTOR_645_FRAME_STRU*)param;
    MRS_STA_SEARCH_CTX * pstSmCtx = mrsStaSearchCtx();
    HI_U32 ret = HI_ERR_SUCCESS;

    if ((HI_NULL == param) 
        || !MRS_IRDA_CTRL14_PARAM_VALID(frame->ucDataRealmLen, MRS_645_FRAME_DATA_DI_SIZE))
    {
        return HI_ERR_FAILURE;
    }

    // 1.清空事件记录
    {
        (hi_void)memset_s(&(pstSmCtx->stEvent), sizeof(pstSmCtx->stEvent), 0, sizeof(pstSmCtx->stEvent));
    }

    // 2.终端复位次数清零
    if (pstSmCtx->stSearchCfg.ucJSAreaVer)
    {
        ret = HI_MDM_ClrRelativeRstTimes();
        if (HI_ERR_SUCCESS != ret)
        {
            return HI_ERR_FAILURE;
        }
    }

    // 3.搜表结果清空
    mrsStaSearchClearMeterList(pstSmCtx);

    return mrsAssembleFrame(ppOutParam, pOutParamLen, HI_NULL, 0, frame, ucPreSize);
}
/* END:   PN: DTS2015042907048 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */


/* BEGIN: PN: DTS2015042907048 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
// 参数初始化
HI_U32 mrsIr645_14H_DI04A00403(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_COLLECTOR_645_FRAME_STRU* frame = (MRS_COLLECTOR_645_FRAME_STRU*)param;
    HI_NV_FTM_COLLECT_PARAM_FILE_S nv;

    if ((HI_NULL == param) 
        || !MRS_IRDA_CTRL14_PARAM_VALID(frame->ucDataRealmLen, MRS_645_FRAME_DATA_DI_SIZE))
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memset_s(&nv, sizeof(nv), 0, sizeof(nv));
    ret = HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &nv, sizeof(nv));
    if (HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memset_s(nv.CCTAddress, sizeof(nv.CCTAddress), 0xFF, sizeof(nv.CCTAddress));
    nv.CCT_register = 0;

    ret = HI_MDM_NV_Write(HI_NV_FTM_COLLECT_PARAM_FILE, &nv, sizeof(nv));
    if (HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }

    return mrsAssembleFrame(ppOutParam, pOutParamLen, HI_NULL, 0, frame, ucPreSize);
}
/* END:   PN: DTS2015042907048 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */

//*****************************************************************************
// 函数名称: mrsIr645_14H_DI04A00503
// 功能描述: 设置终端日历时钟
//           
// 参数说明:
//    无
//                            
// 返 回 值: 
//    成功返回  HI_ERR_SUCCESS
//
// 调用要求: 
// 调用举例: 
// 作    者: fengxiaomin/f00209182 [2015-10-30]
//*****************************************************************************
HI_U32 mrsIr645_14H_DI04A00503(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    HI_UNREF_PARAM(param);
    HI_UNREF_PARAM(ppOutParam);
    HI_UNREF_PARAM(pOutParamLen);
    HI_UNREF_PARAM(ucPreSize);
	return HI_ERR_NOT_SUPPORT;
}

// MAC地址写入
HI_U32 mrsIr645_14H_DI04A09903(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U16 offset = 0;
    MRS_COLLECTOR_645_FRAME_STRU* frame = (MRS_COLLECTOR_645_FRAME_STRU*)param;
	HI_U8 ucPlcMac[HI_PLC_MAC_ADDR_LEN] = {0};

	
    if ((HI_NULL == param) 
        || !MRS_IRDA_CTRL14_PARAM_VALID(frame->ucDataRealmLen, HI_PLC_MAC_ADDR_LEN + MRS_645_FRAME_DATA_DI_SIZE))
    {
        return HI_ERR_FAILURE;
    }

    offset = frame->ucDataRealmLen - HI_PLC_MAC_ADDR_LEN;

    do
    {
        (hi_void)memcpy_s(ucPlcMac,HI_PLC_MAC_ADDR_LEN, frame->ucDataRealm + offset, HI_PLC_MAC_ADDR_LEN);
		
        if (mrsToolsAbnormalMac(ucPlcMac))
        {
			return HI_ERR_FAILURE;
        }

        ret = HI_MDM_SetPlcMac(ucPlcMac);
        if (HI_ERR_SUCCESS != ret)
        {
			return HI_ERR_FAILURE;
        }

        mrsStaSetMeterToMac(ucPlcMac, STA_TYPE_METER, HI_MDM_METER_MAC);
    } while (0);
	
    return mrsAssembleFrame(ppOutParam, pOutParamLen, HI_NULL, 0, frame, ucPreSize);

}



/* BEGIN: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/21 */
HI_U32 mrsCreate645SpecialTransmitUp(HI_PBYTE *ppFrame, HI_PBYTE pbuf, HI_U8 pbuflen,HI_U16 *pusLength)
{
    MRS_PROTO645_FRAME_INF stFrameInf;
    HI_U8 CLTAddress[HI_METER_ADDR_LEN] = {0};

    mrsIrDAGetCltAddr(CLTAddress);
    (hi_void)memset_s(&stFrameInf, sizeof(stFrameInf), 0, sizeof(stFrameInf));
    //采集器地址
    (hi_void)memcpy_s(stFrameInf.aucAddr, HI_METER_ADDR_LEN, CLTAddress, HI_METER_ADDR_LEN);
    stFrameInf.ucCtrl = MRS_645_FRAME_CONTROL_DIR_UP|MRS_645_FRAME_CONTROL_READ_DATA;
    stFrameInf.ucDiSize = MRS_645_FRAME_DATA_DI_SIZE;
    stFrameInf.ulDi = DI_04A09904;
    stFrameInf.pData = pbuf; 
    stFrameInf.ucDataLength = pbuflen;
       
    return mrsGen645Frame(&stFrameInf, ppFrame, pusLength);
    
}


HI_VOID mrsIrTransmitUp(HI_U8 subId,HI_PBYTE pucData, HI_U16 usDataSize)
{
    if (subId == MRS_IR_SUBID_SPECIAL_TRANSMIT)
    {
        HI_PBYTE pFrame = HI_NULL;
        HI_U16 usFrameLength = 0;
        
        if (mrsCreate645SpecialTransmitUp(&pFrame,pucData,(HI_U8)usDataSize, &usFrameLength) == HI_ERR_SUCCESS)
        {
            MRS_SendMrData(pFrame, usFrameLength, HI_DMS_CHL_UART_PORT_IR); 
        }
        
        
        mrsToolsFree(pFrame);
            
    }
    else// if (subId == MRS_IR_SUBID_TRANSMIT)
    {
        MRS_SendMrData(pucData, usDataSize, HI_DMS_CHL_UART_PORT_IR);
    }
}
/* END: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/21 */


#endif // STA

