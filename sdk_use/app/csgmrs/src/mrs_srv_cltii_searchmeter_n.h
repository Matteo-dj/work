//*****************************************************************************
//
//                  版权所有 (C), 1998-2013, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cltii_searchmeter_n.h
//  版 本 号   : V1.0 
//  作    者   : cuiate/00233580
//  生成日期   : 2014-08-13
//  功能描述   : II采搜表内部函数及接口声明
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2014-08-13
//    作    者 : cuiate/00233580
//    修改内容 : 创建文件 
//
//*****************************************************************************

#ifndef _MRS_SRV_CLTII_SEARCHMETER_N_H_
#define _MRS_SRV_CLTII_SEARCHMETER_N_H_
HI_START_HEADER
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)


//*****************************************************************************
//*****************************************************************************

#define MRS_SM_ADDR_DEFAULT         "\xAA\xAA\xAA\xAA\xAA\xAA"
#define MRS_SM_ADDR_BROADCAST       "\x99\x99\x99\x99\x99\x99"

#define MRS_SM_FIRST_METER_MAXRETRY (3)

// 上电之后，如果启动15秒之内未搜到表，继续搜表流程
#define MRS_SM_STARTUP_TIMESTAMP    (15)     // 15秒

// 上电之后，给电表预留8秒的启动时间
#define MRS_SM_STARTUP_WAIT_METER   (8)      // 8秒

/* BEGIN: PN: DTS2015042907633 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
/* BEGIN: PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/11 */
#define MRS_645_DI_METER_ID         "\x65\xF3"      // 0xC032
#define MRS_645_DI_USER_ID          "\x66\xF3"      // 0xC033
#define MRS_645_DI_DEVICE_ID        "\x67\xF3"      // 0xC034
/* END:   PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/11 */
/* END:   PN: DTS2015042907633 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */


typedef struct 
{
    HI_U32  ulDiIn;
    HI_U8   ucProtocol;
    HI_BOOL bResult;
    HI_U16  pad;
    HI_U32  ulDiOut;
} MRS_SEARCH_ITEM_EXTRA;


//*****************************************************************************
//*****************************************************************************

HI_PRV HI_U32 mrsCltiiSearchStartPortal(MRS_STA_SEARCH_CTX* pstCtx);

HI_PRV HI_VOID  mrsCltiiSearchResetRunning(MRS_STA_SEARCH_CTX* pstCtx);

HI_PRV HI_BOOL mrsCltiiSearchIsNeedDetect(MRS_STA_SEARCH_CTX* pstCtx);

HI_PRV HI_BOOL mrsCltiiSearchIsAllDetected(MRS_STA_SEARCH_CTX* pstCtx);

HI_PRV HI_U32 mrsCltiiSearchDetectStart(MRS_STA_SEARCH_CTX* pstCtx);

HI_PRV HI_U32 mrsCltiiSearchDetectProc(MRS_STA_SEARCH_CTX* pstCtx);

HI_PRV HI_U32 mrsCltiiVMSearch(HI_VOID);

HI_PRV HI_U32 mrsCltiiSearchCtxInit(MRS_STA_SEARCH_CTX * pstCtx);

HI_PRV HI_U32 mrsCltiiCheckNvMeterParam(MRS_STA_SEARCH_CTX * pstCtx);

HI_PRV HI_U32 mrsCltiiInitFirstFrame(MRS_STA_SEARCH_CTX * pstCtx);

HI_PRV HI_U32 mrsCltiiInitSearchFrame(MRS_STA_SEARCH_CTX * pstCtx, HI_U32 ulDI, HI_U8 ucProtocol);

HI_PRV HI_U32 mrsCltiiCreateSearchFrame(MRS_STA_SEARCH_CTX * pstCtx, HI_U8 *pucMeter);

HI_PRV HI_U32 mrsCltiiSearchSendFrame(MRS_STA_SEARCH_CTX *pstCtx);

HI_PRV HI_U32 mrsCltiiSearchLoopStart(MRS_STA_SEARCH_CTX *pstCtx);

HI_PRV HI_U32 mrsCltiiSearchLoopStartProc(MRS_STA_SEARCH_CTX *pstCtx);

/* BEGIN: PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/12 */
HI_PRV HI_U32 mrsCltiiSearchLoopRxMeter(MRS_STA_SEARCH_CTX *pstCtx, HI_U8 *pucMeter, HI_U8 ucProtocol);
/* END:   PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/12 */

HI_PRV HI_U32 mrsCltiiSearchLoopRxMeterProc(MRS_STA_SEARCH_CTX *pstCtx, HI_U8 *pucMeter, HI_U8 ucProtocol);

HI_PRV HI_U32 mrsCltiiSearchParseRx(MRS_STA_SEARCH_CTX *pstCtx);

HI_PRV HI_U32 mrsCltiiSearchParseRxProc(MRS_STA_SEARCH_CTX *pstCtx);

HI_PRV HI_U32 mrsCltiiSearchSaveVerifyParams(MRS_STA_SEARCH_CTX *pstCtx, HI_U8 *pucMeter, HI_U8 ucProtocol);

HI_PRV HI_BOOL mrsCltiiSearchIsNeedVerify(MRS_STA_SEARCH_CTX *pstCtx);

HI_PRV HI_BOOL mrsCltiiSearchIsAllVerified(MRS_STA_SEARCH_CTX* pstCtx);

HI_PRV HI_VOID mrsCltiiSearchSaveLoopParams(MRS_STA_SEARCH_CTX* pstCtx);

HI_PRV HI_VOID mrsCltiiSearchRestoreLoopParams(MRS_STA_SEARCH_CTX* pstCtx);

HI_PRV HI_VOID mrsCltiiSearchClearLoopParams(MRS_STA_SEARCH_CTX* pstCtx);

HI_PRV HI_U32 mrsCltiiSearchVerifyStart(MRS_STA_SEARCH_CTX *pstCtx);

HI_PRV HI_U32 mrsCltiiSearchVerifyProc(MRS_STA_SEARCH_CTX *pstCtx);

HI_PRV HI_U32 mrsCltiiSearchVerifyFinish(MRS_STA_SEARCH_CTX *pstCtx);

HI_PRV HI_VOID mrsCltiiSearchClearVerifyParams(MRS_STA_SEARCH_CTX *pstCtx);

HI_PRV HI_BOOL mrsCltiiSearchAllowExplore(MRS_STA_SEARCH_CTX* pstCtx);

HI_PRV HI_U32 mrsCltiiSearchExploreStart(MRS_STA_SEARCH_CTX* pstCtx);

HI_PRV HI_VOID mrsCltiiSearchSaveExploreParams(MRS_STA_SEARCH_CTX* pstCtx);

HI_PRV HI_U8 mrsCltiiSearchGetAddrAvailMaskByteNum(HI_U8* pucAddr, HI_U8 ucProtocol);

HI_PRV HI_VOID mrsCltiiSearchExploreDepthIncrease(MRS_STA_SEARCH_CTX *pstCtx, HI_U8 *pucAddr, HI_U8 ucProtocol);

HI_PRV HI_VOID mrsCltiiSearchExploreFinish(MRS_STA_SEARCH_CTX *pstCtx);

HI_PRV HI_VOID mrsCltiiSearchClearExploreParams(MRS_STA_SEARCH_CTX *pstCtx);

HI_PRV HI_BOOL mrsCltiiSearchCheckInvalidFrame(MRS_STA_SEARCH_CTX *pstCtx, HI_U16 usLen, HI_U8 ucProtocol);

HI_PRV HI_U32 mrsCltiiSearchFinish(MRS_STA_SEARCH_CTX *pstCtx);

HI_PRV HI_U32 mrsCltiiSearchCheck(MRS_STA_SEARCH_CTX *pstCtx);

HI_PRV HI_U32 mrsCltiiSearchCheckTryFirst(MRS_STA_SEARCH_CTX *pstCtx);

HI_PRV HI_U32 mrsCltiiSearchCheckDetect(MRS_STA_SEARCH_CTX *pstCtx);

HI_PRV HI_U32 mrsCltiiSearchCheckVerify(MRS_STA_SEARCH_CTX *pstCtx);

HI_PRV HI_U32 mrsCltiiSearchCheckNormal(MRS_STA_SEARCH_CTX *pstCtx);

HI_PRV HI_BOOL mrsCltiiSearchCheckChange(MRS_METER_LIST_S * pstMeterList, MRS_SEARCH_METERLIST_STRU *pstSmList);

HI_PRV HI_U32 mrsCltiiSearchEventInsert(MRS_STA_SEARCH_CTX *pstCtx, MRS_SEARCH_EVENT_ITEM *pstEventItem);

HI_PRV HI_BOOL mrsCltiiSearchWaitMeterStartup(HI_VOID);

HI_PRV HI_U32 mrsCltiiSearchNext(MRS_STA_SEARCH_CTX *pstCtx);

HI_PRV HI_U32 mrsCltiiSearchNextLoop(MRS_STA_SEARCH_CTX *pstCtx);

HI_PRV HI_BOOL mrsCltiiSearchIsMaskMeterAddr(HI_U8 *pucAddr);

HI_PRV HI_BOOL mrsCltiiSearchIsBcMeterAddr(HI_U8 *pucAddr);

HI_PRV HI_BOOL mrsCltiiSearchIsBcSearchEnable(MRS_STA_SEARCH_CTX *pstCtx);

HI_PRV HI_VOID mrsCltiiSearchAddrIncrease(MRS_STA_SEARCH_CTX *pstCtx, HI_U8 *pucAddr);

HI_PRV HI_VOID mrsCltiiSearchAddrDepthIncrease(HI_U8* pucAddr, HI_U8 ucProtocol);

HI_PRV HI_U8 mrsCltiiSearchAddrByteIncrease(HI_U8 ucByte, HI_U8 ucProtocol);

HI_PRV HI_U32 mrsCltiiSearchGetNodeDI(MRS_STA_SEARCH_CTX* pstCtx, NV_APP_MRS_SM_NODE* pstNode);

HI_PRV HI_U32 mrsCltiiSearchGetNodeDI97(MRS_STA_SEARCH_CTX* pstCtx, NV_APP_MRS_SM_NODE* pstNode);

HI_PRV HI_U8 mrsCltiiSearchGetNodeBaudRateIndex(HI_U8 ucParam);

HI_PRV HI_U16 mrsCltiiSearchGetDI97(MRS_STA_SEARCH_CTX *pstCtx, HI_U8 ucProtocol);

HI_PRV HI_U8 mrsCltiiSearchMaskByte(HI_U8 ucProtocol);

HI_PRV HI_U32 mrsCltiiGetMeterAddr(HI_U8 *pFrame, HI_U16 usFrameLen, HI_U8 ucProtocol, HI_U8 *pMeter);

HI_PRV HI_U16 mrsCltiiGetDetectBaudrate(MRS_SRV_QUEUE *pQueue);

HI_PRV HI_U32 mrsCltiiInsertSearchItem(MRS_SRV_QUEUE *pQueue, HI_U32 ulDi, HI_U8 ucProtocol);

HI_PRV HI_BOOL mrsCltiiSearchItemVisit(HI_VOID *pItem, HI_VOID *pParam);

HI_PRV HI_BOOL mrsCltiiSearchItemVisit97(HI_VOID *pItem, HI_VOID *pParam);

HI_PRV HI_U16 mrsCltiiGetSearchBaudRate(MRS_STA_SEARCH_CTX *pstCtx);


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)

HI_U32 mrsCsgCltiiStartDetectList(APP_CSG_CMD_DL_METERLIST_STRU *pstMlist);

HI_U32 mrsCsgCltiiDetectProc(MRS_STA_SEARCH_CTX *pstCtx);

HI_U32 mrsCsgCltiiSearchItemInit(MRS_SRV_QUEUE *pQueue);

#endif

//*****************************************************************************
//*****************************************************************************


#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_END_HEADER
#endif // _MRS_SRV_CLTII_SEARCHMETER_N_H_


