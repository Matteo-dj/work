//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_cco_searchmeter.h
//  �� �� ��   : V1.0
//  ��    ��   :
//  ��������   :
//  ��������   : �ѱ�ģ�麯�����ӿڶ���
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2012-05-18
//    ��    �� : fengxiaomin/f00209182
//    �޸����� : �����ļ�
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   :
// SUBSYSTEM :
// MODULE    :
// OWNER     :
//*****************************************************************************


#ifndef __MRS_SRV_CCO_SEARCHMETER_H__
#define __MRS_SRV_CCO_SEARCHMETER_H__

HI_START_HEADER

#define SERIES_FIND_INVALID_NODE_MAX_COUNT  108         //�����ҵ���Ч�ڵ����
#define SCAN_MAP_CYCLE_TIMER_VAL            (1000 * 5)  //�ѱ�ʱ������ɨ��MAP��ʱ��
#define MRS_SEARCH_REPORT_NODE_NUM          1           //ÿ֡�ѱ����ϱ�ʱ�Ľڵ�����
#define MRS_COLLECTOR_DEFAULT_PROTOCOL      0           //�ɼ���Ĭ��ͨ��Э������
#define MRS_SM_START_NOTIFY_STA_NUM         (1)         // ��δ������վ�����Ϊ��ô��ʱ����ʼ֪ͨ

typedef enum
{
    MRS_CCO_SEARCH_FINISH_NORMAL,
    MRS_CCO_SEARCH_FINISH_TIMEOUT,
    MRS_CCO_SEARCH_FINISH_STOP,
    MRS_CCO_SEARCH_FINISH_ROUTE_RESET,
    MRS_CCO_SEARCH_FINISH_ROUTE_RESUME,
} MRS_CCO_SERACH_FINISH_REASON_E;


//*****************************************************************************
// ��������: mrsCcoSearchMeterInit
// ��������: �ѱ�ģ���ʼ��
//
// ����˵��:
//   pstCcoCtx [in/out] ��ǰ״̬
//
// �� �� ֵ:
//   HI_ERR_SUCCESS �ɹ�
//   0xFFFFFFFF��ַӳ��������ȡʧ��
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/f00209182 [2012-05-24]
//*****************************************************************************
HI_U32 mrsCcoSearchMeterInit(MRS_CCO_SRV_CTX_STRU * pstCcoCtx, HI_U32 ulSearchMode);


//*****************************************************************************
// ��������: mrsCcoSearchReportSlaveNodeInfo
// ��������: �ϱ�����Ϣ������
//
// ����˵��:
//   This function has no arguments.
//
// �� �� ֵ:
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/f00209182 [2012-05-18]
//*****************************************************************************
HI_VOID mrsCcoSearchReportSlaveNodeInfo(HI_VOID);


//*****************************************************************************
// ��������: mrsReportTimeOutFun
// ��������: �ϱ���ʱ������
//
// ����˵��:
//   pstCcoCtx [in/out] ��ǰ״̬
//
// �� �� ֵ:
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/f00209182 [2012-05-22]
//*****************************************************************************
HI_U32 mrsReportTimeOutFun(HI_VOID*);


//*****************************************************************************
// ��������: mrsAckFun
// ��������: �ϱ�ȷ�ϴ�����
//
// ����˵��:
//   This function has no arguments.
//
// �� �� ֵ:
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/f00209182 [2012-05-22]
//*****************************************************************************
HI_VOID mrsAckFun(HI_VOID * pAckParam);

//*****************************************************************************
// ��������: mrsStopSearchMeter
// ��������: ֹͣ�ѱ�����
//
// ����˵��:
//   This function has no arguments.
//
// �� �� ֵ:
//   HI_ERR_SUCCESS ���ҵ���Ч�ڵ�
//   0xFFFFFFFF��ַӳ��������ȡʧ��
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/f00209182 [2012-05-24]
//*****************************************************************************
HI_U32 mrsStopSearchMeter(HI_VOID);


//AFN06 F3 �����ϱ�·�ɹ����䶯��Ϣ
HI_U32 mrs3762_ReportAFN06_F3(HI_U8 status);

HI_VOID mrsReportAFN06F3AckProcess(HI_VOID *param);

/* BEGIN: PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
HI_VOID mrsSearchMeterPrepare(MRS_CCO_SRV_CTX_STRU* pstCcoCtx);

HI_U32 mrsSearchMeterProc(MRS_CCO_SRV_CTX_STRU* pstCcoCtx);
/* END:   PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */

HI_U32 mrsCcoStartSearchMeter(MRS_CCO_SRV_CTX_STRU * cco);

HI_U32 mrsCcoStopSearchMeter(MRS_CCO_SRV_CTX_STRU * cco);

HI_U32 mrsCcoSearchMeterTimeoutProcess(HI_VOID);

HI_U32 mrsCcoSendSearchMeterPlcFrame(MRS_CCO_SRV_CTX_STRU * cco);

HI_U32 mrsCcoSearchMeterGetMeterlist(MRS_CCO_SRV_CTX_STRU *cco);

HI_U32 mrsCcoSearchMeterTopoQueryProc(MRS_CCO_SRV_CTX_STRU *cco);

HI_U32 mrsCmdCcoGetMeterList(HI_U16 id, HI_PVOID req_packet, HI_U16 req_packet_size);

HI_U32 mrsReportTotalTimeOutFun(HI_VOID * param);


HI_VOID mrsCcoSmRestoreWhitelistSwitch(HI_VOID);

HI_VOID mrsCcoSmWhitelistSwitchTimerReset(HI_VOID);

HI_U32 mrsCcoSearchMeterNotifyStaBindNetwork(MRS_CCO_SRV_CTX_STRU *pstCcoCtx);

HI_END_HEADER



#endif //__MRS_SRV_CCO_SEARCHMETER_H__

