//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2016, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_cco_detect_rm.h
//  �� �� ��   : V1.0 
//  ��    ��   : fengxiaomin/00209182
//  ��������   : 2016-01-13
//  ��������   : �ɼ�������ȫ�������ܺ������ӿ�����
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2016-01-13
//    ��    �� : fengxiaomin/00209182
//    �޸����� : �����ļ� 
//
//*****************************************************************************

#ifndef _MRS_SRV_CCO_DETECT_RM_H_
#define _MRS_SRV_CCO_DETECT_RM_H_

HI_START_HEADER

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

//*****************************************************************************
//*****************************************************************************
#define MRS_CCO_DETECT_MAC_NUM_MAX          (100)
#define MRS_CCO_DETECT_MAC_BUFFER_MAX       (MRS_CCO_DETECT_MAC_NUM_MAX * HI_PLC_MAC_ADDR_LEN)


typedef struct 
{
    MRS_SRV_LINK stLink;
    HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN];
	HI_U8 reserved[2];
    HI_U32 ulTimeStamp; //��λ:����
} MRS_CCO_DETECT_BLACK_LIST_ITEM;

typedef struct
{
    HI_U16 usNum;                      // ����Ԫ�ظ���
    HI_U8  aucPad[2];                  // ����λ
    MRS_SRV_LIST stList;              // ָ��MRS_CCO_DETECT_BLACK_LIST_ITEM
} MRS_CCO_DETECT_BLACK_LIST;

typedef struct 
{
    MRS_SRV_NODE stLink;
	HI_BOOL bValid;
    HI_U8 reserved;
    HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN];
    HI_U32 ulTimeStamp;
} MRS_CCO_DETECT_WHITE_QUEUE_ITEM;

typedef struct
{
    HI_U16 usNum;                      // ����Ԫ�ظ���
    HI_U16 usValidNum;                  
    MRS_SRV_QUEUE stQueue;             // ����
} MRS_CCO_DETECT_WHITE_QUEUE;

typedef struct
{
	HI_U8 ucEnable;
	HI_U8 ucTopoWaitMaxTime;//���CCOһֱ������������ֻ��CCOһ��վ��,20���Ӻ��յ��ܾ�����֪ͨ�󽫾ܾ��ɼ���վ��������������,ǰ���Ǳ����ǿ�,��λ����
	HI_U16 usTimeOut; //����mac��ַ�����Ĳɼ�����������������ucWhilteAddMaxTimeʱ����û�м���������Ĳɼ���վ������,ɾ����ӵİ�����
	HI_U8 ucMaxTurn;
	HI_U8 ucJoinNetMaxTime;//̽������л�����׷�Ӱ������е�վ������ʱ��,��ʱ������̽�Ⳮ��,��λ:����
    HI_U8 ucBlackValidTime;//̽������к�������Чʱ�� ��λ:����
	HI_U8 ucStaJoinWhiteTime;//���ܾ���վ����뵽��������ʱ�� ��λ:����
	HI_U16 aus97DI[MRS_BROADCAST_RM_97_DI_NUM];
	HI_U32 ul07DI;
    HI_U8 ucSingleMeterTime; //ÿ���Ԥ��̽��ʱ�� ��λ:��
    HI_U8 ucTurnTime; //���͵���ַ�б�Plc����ʱ���� ��λ:����
    HI_U8 reserved[2];
}MRS_CCO_DETECT_PARAM_S;

typedef struct
{
	HI_U8 ucJoinWhiteFlag : 1; // 0-��������������  1-������������
	HI_U8 ucSetWaitTopoFlag : 2; // 0-δ֪  1-��Ҫ�ڱ���֪ͨMACʱ����20���ӵȴ�վ������,�����ɿձ�Ϊ�ǿ�ʱ�����øñ�־ 2-���������֪ͨMACʱ����,֮�����һ������ϸñ�־����Ϊ1
	HI_U8 ucRmFlag : 1; //0-����̽��͸������ 1-����̽��͸��,�ѷ���͸��֡
	HI_U8 ucExternFlag : 1; //�ⲿ����̽�Ⳮ�����ʹ�� 0-�ⲿ�޲���  1-�ⲿ�в���
	HI_U8 ucRefreshWhiteFlag : 1;  //ˢ�°�������ʱ����ر�־  0-��ʱ���ر�,���ڼ��  1-��ʱ��������־,�ڼ��
	HI_U8 reserved : 2;
	HI_U8 ucStatus;
	HI_U8 ucCurrTurn;
	HI_U8 uc97DIIndex;
	MRS_ARCHIVES_NODE_STRU stNode;
	HI_U16 usSeq; //CCO����̽����ַ�б���ɼ���ģ���plc���
	HI_U8 ucOriginalEnable; //�ⲿʹ��ǰ��ԭʼ����
	HI_U8 ucCacheMacNum;
	HI_U8 pMacBuf[MRS_CCO_DETECT_MAC_BUFFER_MAX]; //��ʱ��ű���վ��mac��ַ������ ����ʱ��20��
}MRS_CCO_DETECT_RUNNING_S;

typedef struct
{
	MRS_CCO_DETECT_RUNNING_S stRunning;
	MRS_CCO_DETECT_WHITE_QUEUE stWhiteListQueue;
	MRS_CCO_DETECT_BLACK_LIST stBlackList;
	MRS_CCO_DETECT_PARAM_S stParam;
}MRS_CCO_DETECT_RM_CTX_S;


MRS_CCO_DETECT_RM_CTX_S* mrsCcoGetDetectRmCtx(HI_VOID);
HI_U8 mrsGetDetectRmEnable(HI_VOID);
HI_U8 mrsGetDetectRmStatus(HI_VOID);
HI_U32 mrsCcoDetectRmInit(HI_VOID);
HI_VOID mrsCcoRejectNodeCallback (HI_IN HI_MDM_NETWORK_REJECT_ENTRY_S *);
HI_BOOL mrsIsDetectRm(HI_VOID);
HI_U32 mrsDetectRmRespProc(HI_U8 *pucMeter);
HI_VOID mrsDetectRmBreakProc(HI_VOID);
HI_VOID mrsResumeDetectRmProc(HI_VOID);
HI_U32 mrsMacJoinWhiteMsgProc(HI_SYS_QUEUE_MSG_S *pstMsg);
HI_U32 mrsDetectWhiteListAddTimeout(HI_VOID);
HI_U32 mrsDetectWhiteListKeepTimeOut(HI_VOID);
MRS_CCO_DETECT_WHITE_QUEUE_ITEM* mrsDetectWhiteItemQuery(HI_U16 usIndex);
HI_VOID mrsDetectWaitTopoInit(HI_VOID);
HI_U32 mrsDetectWaitTopoTimeOut(HI_VOID);
HI_U32 mrsCcoDetectMeterChgToMacProc(HI_VOID);
HI_VOID mrsCcoDetectEmptyArchivesAddProc(HI_VOID);
HI_U32 mrsCcoDetectTopoChgProc(HI_U8 ucStatus);
HI_U32 mrsDetectJoinNetTimeout(HI_VOID);
HI_U32 mrsCcoDetectRmReset(HI_VOID);
HI_VOID mrsCcoDetectPlcDisconnectProc(HI_VOID);
HI_VOID mrsDetectLrRmTimeout(MRS_ARCHIVES_NODE_STRU *pstNode);
HI_U16 mrsGetDetectWhiteListNum(HI_VOID);
HI_U32 mrsDetectRxModeNotify(HI_U8 ucMode);
HI_U32 mrsCcoDetectRmNodeProc(HI_VOID);
HI_U32 mrsCheckDetectEndProc(HI_VOID);
HI_U32 mrsCcoDetectRmTimeoutProc(HI_U8 *pucMeter);
HI_U32 mrsResumeDetectRmSendMsg(HI_VOID);
HI_VOID mrsSetDetectRmFlag(HI_U8 ucFlag);
HI_VOID mrsDetectBlackListTimeout(HI_VOID);
HI_U32 mrsCcoDetectWhiteSetMacList(HI_U16 usNum, HI_MAC_VALID_MAC_LIST_STRU * pstMeterList);
HI_U16 mrsGetDetectWhiteListValidNum(HI_VOID);
HI_U32 mrsDetectWhiteListCheckProc(HI_VOID);
HI_U32 mrsCcoDetectRmStart(HI_VOID);

/**
 @brief  Detect module timout function, called when parallel meter-reading timeout. 
 @brief  CNcomment: ̽��ģ�鳬ʱ����������������ʱʱ���á�CNend
 @param[in] pMeterAddr  Meter address.  CNcomment: ����ַ��CNend
 */
HI_VOID mrsDetectPrRmTimeout(HI_U8 *pucMeter);


//*****************************************************************************
//*****************************************************************************


#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

HI_END_HEADER

#endif // _MRS_SRV_CCO_DETECT_RM_H_


