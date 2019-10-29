//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2014, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_dfx_clt.h
//  �� �� ��   : V1.0 
//  ��    ��   : liupeidong/00238134
//  ��������   : 2014-04-03
//  ��������   : �ɼ����ѱ����ͳ�������ӿڶ���
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2014-04-03
//    ��    �� : liupeidong/00238134
//    �޸����� : �����ļ� 
//
//*****************************************************************************
#ifndef __MRS_DFX_CLT_H__
#define __MRS_DFX_CLT_H__

HI_START_HEADER


#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA) 

HI_U32 mrsDfxGetCltSearchResult(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
HI_U32 mrsDfxGetCltSearchStats(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
HI_U32 mrsDfxGetCltSearchLocal(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
HI_U32 mrsDfxGetCltSearchFrame(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
/* BEGIN: PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/11 */
HI_U32 mrsDfxGetCltSearchLocalEx(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
/* END:   PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/11 */
HI_U32 mrsDfxGetCltLocalMap(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

//�����ʻ���    /100  (12:1200 24:2400 48:4800 96:9600)
#define MRS_DFX_BAUDRATE_100                      (100)

DIAG_CMD_MRS_GET_CLT_SEARCH_STATS_IND_STRU* mrsDfxCltGetSearchStats(HI_VOID);
DIAG_CMD_MRS_GET_CLT_SEARCH_LOCAL_IND_STRU* mrsDfxCltGetSearchLocal(HI_VOID);
MRS_DFX_SEARCH_FRAME_STATS_STRU* mrsDfxCltGetSearchFrame(HI_VOID);
HI_VOID mrsDfxCltResetPlcSearchStats(HI_VOID);
HI_VOID mrsDfxCltResetComSearchStats(HI_VOID);
HI_VOID mrsDfxCltResetSearchLocal(HI_VOID);
HI_VOID mrsDfxCltResetSearchFrame(HI_VOID);

HI_VOID mrsDfxStaSetAddr2Mac(HI_U8*);
HI_VOID mrsDfxCltiSmComFrameTx(HI_U8);
HI_VOID mrsDfxCltiSmComStartRx(HI_VOID);
HI_VOID mrsDfxCltiSmComQueryRx(HI_U8*);
HI_VOID mrsDfxCltSmComQueryResultLTS(HI_VOID);

HI_VOID mrsDfxCltSmPlcStartRx(HI_U32);
HI_VOID mrsDfxCltSmPlcQueryRx(HI_U32);
HI_VOID mrsDfxCltSmPlcQueryTx(HI_VOID);

HI_VOID mrsDfxCltSetBaudRate(HI_U16);

HI_VOID mrsDfxCltSmLocalStartTime(HI_VOID);
HI_VOID mrsDfxCltSmLocalEndTime(HI_VOID);


#define MRS_DFX_SM_ABNORMAL_DEPTH                 (2)
#define MRS_DFX_BYTE_INTERVAL_TIMEOUT             (60)

MRS_CLT_SEARCH_LOCAL_STATS* mrsDfxCltiiGetLocalStatsType(HI_VOID);
HI_VOID mrsDfxCltiiSetLocalTxStats(HI_U8* pucAddr, HI_U8 ucProtocol, HI_U32 ulDI, HI_U32 ulBaudrate);
HI_VOID mrsDfxCltiiSetLocalTxAddrDepth(MRS_CLT_SEARCH_LOCAL_STATS* pstStatsType, HI_U8* pucAddr, HI_U8 ucProtocol);
HI_VOID mrsDfxCltiiSetLocalRxValid645Frame(HI_VOID);
HI_VOID mrsDfxCltiiSetLocalRxInvalidStats(HI_U16 ucLen);
MRS_DFX_SEARCH_FRAME_HEAD* mrsDfxCltiiGetCurrentFrameHead(HI_VOID);
HI_VOID mrsDfxCltiiAppendCurrentFrame(HI_U8* pucAddr, HI_U8 ucProtocol, HI_U32 ulDI, HI_U16 usBaudrate);
HI_VOID mrsDfxCltiiRxFrameCount(HI_VOID);
HI_VOID mrsDfxCltiiSetFrameTimeStats(HI_VOID);
HI_VOID mrsDfxCltiiSetFrameStatsValid(HI_VOID);
HI_VOID mrsDfxCltiiSetFrameStatsInvalid(HI_VOID);
HI_VOID mrsDfxCltiiSetFrameStatsDeny(HI_VOID);
HI_VOID mrsDfxCltiiSetFrameStatsDiscard(HI_VOID);
HI_VOID mrsDfxCltiiSetSmAlarm(HI_U8* pucAbnormal);
HI_VOID mrsDfxCltiiSetSmWarning(HI_U8* pucAbnormal);
HI_VOID mrsDfxCltiiSetSmMultiCltiiConnect(HI_VOID);

/* BEGIN: Modified by cuichunyan/c00276962 2016/11/10  ���ⵥ��:cltii R485 MultiConnect */    
HI_U32 mrsDfxGetRS485MultiConn(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
HI_U32 mrsDfxClrRS485MultiConn(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

/* END  : Modified by cuichunyan/c00276962 2016/11/10  ���ⵥ��:cltii R485 MultiConnect */

#endif

HI_END_HEADER

#endif // __MRS_DFX_CLT_H__


