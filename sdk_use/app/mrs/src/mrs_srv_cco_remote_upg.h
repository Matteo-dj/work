//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_cco_remote_upg.h
//  �� �� ��   : V1.0 
//  ��    ��   : fengxiaomin/f00209182
//  ��������   : 2014-1-9
//  ��������   : Զ������ģ�麯�����ӿڶ���
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2014-1-9
//    ��    �� : fengxiaomin/f00209182 
//    �޸����� : �����ļ� 
//
//*****************************************************************************
#ifndef __MRS_SERVICE_CCO_REMOTE_UPG_H__
#define __MRS_SERVICE_CCO_REMOTE_UPG_H__
HI_START_HEADER


/* BEGIN: Added by fengxiaomin/00209182, 2014/1/9   ���ⵥ�ź�:DTS2014012607684 */
#define MRS_CCO_UPG_ERROR_SEG          (0x0000FFFF)  //����α�ʶ
#define MRS_CCO_SINGAL_UPG_DATA_LEN_MAX   (1024) //ÿ������������󳤶�

// Fn(2)+�ļ���ʶ(1)+�ļ�����(1)+�ļ�ָ��(1)+�ܶ���n(2)+��i�α�ʶ(4)+���ݳ���lf(2)+����(lf)
#define MRS_CCO_UPG_FIX_DATA_OFFSET    (13)    // Զ����������������������ƫ�Ƴ���
#define MRS_CCO_UPG_DATA_LEN_OFFSET    (11)  //Զ���������������ݳ���ƫ�Ƴ���
#define MRS_CCO_UPG_CURRENT_SEG_OFFSET   (7) // Զ�����������������������жα�ʶλ�õ�ƫ�Ƴ���
#define MRS_CCO_UPG_TOTAL_SEG_OFFSET   (5)   //Զ�������������ܶ���ƫ�Ƴ���
#define MRS_CCO_UPG_FILE_PROPERTY_OFFSET  (3) //Զ�������������ļ�����ƫ�Ƴ���
#define MRS_CCO_UPG_FILE_FLAG_OFFSET   (2)   //Զ�������������ļ���ʶƫ�Ƴ���

#define MRS_CCO_REMOTE_UPG_ID          HI_MDM_OBJ_ID_MRS  //MRSģ������ID
#define MRS_CCO_REMOTE_UPG_OBJ_ID 	   HI_MDM_OBJ_ID_MRS  //MRS��վ��������

#define MRS_CCO_UPGRADE_IDLE_PERIOD         (0) //MRS�������н׶�
#define MRS_CCO_UPGRADE_LOCAL_LOAD_PERIOD   (1) // MRS���������б�����װ�׶�
#define MRS_CCO_UPGRADE_LOCAL_UPG_PERIOD    (2) // MRS���������б��������׶�

//�ļ���ʶ
#define MRS_CCO_CLEAR_UPG_LOAD_FILE   (0x00) //�����װ�ļ�
#define MRS_CCO_LOCAL_CCO_UPGRADE     (0x03) //����CCO��������
#define MRS_CCO_BOARDCAST_UPGRADE      (0x07) //ȫ������
#define MRS_CCO_STA_UPGRADE           (0x08) //ȫ��STA����

#define MRS_CCO_UPG_END_FRAME         (0x01) //����֡
//�����ļ�����
#define MRS_CCO_UPGRADE_FILE_CODE     (1)
#define MRS_CCO_UPGRADE_FILE_NV       (2)
#define MRS_CCO_UPGADE_FILE_BOOT      (3)

//�ļ�����־
#define MRS_CCO_UPG_FILE_CHECK_FINISH       (1)  //�����ļ�������
#define MRS_CCO_UPG_FILE_NEED_CHECK         (0)  //�����ļ���Ҫ���
#define MRS_CCO_UPG_FILE_CHECK_RESPONSION_OR_TIMEOUT      (1)  //�����ļ��������Ӧ��ʱ
#define MRS_CCO_UPG_FILE_CHECK_NEED_RESPONSION  (0)  //�����ļ����δ��Ӧ

//CCO�յ������װ��־
#define MRS_CCO_UPG_CLEAR_FILE_COMMAND_RX          (1) //CCO���յ������װ����
#define MRS_CCO_UPG_CLEAR_FILE_COMMAND_NON_RX      (0) //CCOû�н��յ������װ����
/* BEGIN: Modified by fengxiaomin/00209182, 2014/5/6   ���ⵥDTS2014050504643 */
#define MRS_CCO_UPG_CLEAR_FILE_COMMAND_RESPONSION_OR_TIMEOUT  (1) //CCO���յ������װ������Ѿ��յ���Ӧ
/* END:   Modified by fengxiaomin/00209182, 2014/5/6 */
#define MRS_CCO_UPG_CLEAR_FILE_COMMAND_NEED_RESPONSION  (0) //CCO���յ������װ�����δ��Ӧ
#define MRS_CCO_RESP_ACK_FALG   (1)
#define MRS_CCO_RESP_DENY_FALG  (0)
//���������׶�����������־
#define MRS_CCO_NEED_START_UPG_PLC_BAD         (1)
#define MRS_CCO_NEED_START_UPG_PLC_OK          (0)

/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014031100268 */
//����������־
#define MRS_CCO_ALLOW_STOP_UPG  (0)
#define MRS_CCO_DENY_STOP_UPG   (1)
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */

//��Ϣ��غ�
#define MRS_CCO_UPG_FILE_CHECK_MSG_MAX_TIMES_MAX       (5)  //�����ļ���Ϣ��ʱ����ʹ������ֵ
#define MRS_CCO_UPG_FILE_CHECK_MSG_MAX_TIMES_DEFAULT   (2)  //�����ļ���Ϣ��ʱ����ʹ���Ĭ��ֵ
#define MRS_CCO_UPG_FILE_CHECK_MSG_MAX_TIMES_MIN       (1)  //�����ļ���Ϣ��ʱ����ʹ�����Сֵ

#define MRS_CCO_REMOTE_UPG_START_FIX_BLKNUM  (1)
#define MRS_CCO_REMOTE_UPG_START_FIX_INDEX   (1)
//#define MRS_CCO_UPG_CMD_FAIL_MSG_TINE_INTERVAL_MAX      (60) //���������ط�ʱ�������ֵ����λΪ��
//#define MRS_CCO_UPG_CMD_FAIL_MSG_TINE_INTERVAL_DEFAULT  (10) //���������ط�ʱ����Ĭ��ֵ����λΪ��
//#define MRS_CCO_UPG_CMD_FAIL_MSG_TINE_INTERVAL_MIN      (1) //���������ط�ʱ������Сֵ����λΪ��

#define MRS_CCO_UPG_CMD_MSG_MAX_TIMES_MAX         (50)  //������������ʹ������ֵ
#define MRS_CCO_UPG_CMD_MSG_MAX_TIMES_DEFAULT     (6)  //������������ʹ���Ĭ��ֵ
#define MRS_CCO_UPG_CMD_MSG_MAX_TIMES_MIN         (1)  //������������ʹ�����Сֵ
/* BEGIN: PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
#define MRS_CCO_UPG_CMD_WAIT_PLC_AVAILABLE_TIME   (180)
/* END:   PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */

#define MRS_CCO_UPG_FILE_RECEIVE_TIME_OUT_MAX       (2 * 60 * 60)  //������װ�׶������ļ������ճ�ʱʱ�����ֵ����λΪ��
#define MRS_CCO_UPG_FILE_RECEIVE_TIME_OUT_DEFAULT   (600)  //������װ�׶������ļ������ճ�ʱʱ��Ĭ��ֵ����λΪ��
#define MRS_CCO_UPG_FILE_RECEIVE_TIME_OUT_MIN       (60)  //������װ�׶������ļ������ճ�ʱʱ����Сֵ����λΪ��

#define MRS_CCO_UPG_STATUS_QUERY_TIME_MAX       (1 * 60 * 60)   //MRSѯ������״̬ʱ�������ֵ����λΪ��
#define MRS_CCO_UPG_STATUS_QUERY_TIME_DEFAULT   (60)   //MRSѯ������״̬ʱ����Ĭ��ֵ����λΪ��
#define MRS_CCO_UPG_STATUS_QUERY_TIME_MIN       (5)   //MRSѯ������״̬ʱ������Сֵ����λΪ��

#define MRS_CCO_UPG_STATUS_QUERY_TIME_OUT     (2)   //MRSѯ������״̬��ʱʱ��,��λΪ��
#define MRS_CCO_UPG_STATUS_QUERY_POWERON_TIMES_MAX       (10)   //MRSѯ������״̬�������ֵ
#define MRS_CCO_UPG_STATUS_QUERY_POWERON_TIMES_DEFAULT   (5)   //MRSѯ������״̬����Ĭ��ֵ
#define MRS_CCO_UPG_STATUS_QUERY_POWERON_TIMES_MIN       (1)  //MRSѯ������״̬������Сֵ

typedef NV_APP_MRS_UPG_PARAM_STRU MRS_CCO_REMOTE_UPG_PARAM_S;

typedef struct
{
    MRS_CCO_REMOTE_UPG_PARAM_S stUpgParam;
    HI_U8 reserved;
    HI_U8 ucUpgPeriod;      // mrs���������Ľ׶�
    HI_U8 ucUpgFileFlag;   // �ļ���ʶ
    HI_U8 ucFileHeadCheckFlag:2;  //CCO���յ��������ļ�ͷ����־����  0--δ��飬1--�Ѽ�飬2--�����Ϊ����
    HI_U8 ucResered:2;

    HI_U8 ucStartUpgFlag:1; //CCO���뱾�������׶κ��Ƿ�����������־���������Ż�δ��ɣ�ֵΪ1���������Ż���ɣ�ֵΪ0
    HI_U8 ucClearFileRxFlag:1;//CCO�����������н��յ������װ��־ 0--δ�յ���1--�յ�
    HI_U8 ucClearFileRxResp:1;//CCO�����������н��յ������װ���Ӧ��־ 0--δ��Ӧ��1--�ѻ�Ӧ
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014031100268 */
    HI_U8 ucAllowStopUpgFlag:1;//CCO�������������Ƿ�����ֹͣ������־ 0--����1--������
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
    HI_U32 ulTotalSegNum; // �ܶ���
    HI_U16 usSingleSegLen;// ÿ�γ���
    HI_U16 usEndSegLen; //�����ļ��������һ�������������������ݳ���
    HI_U8 ucFrameSeq;
    HI_U8 ucCurrCheckMsgTimes;  //��ǰ��Ϣ���ʹ���
    HI_U8 ucCurrStopUpgMsgTimes;  //��ǰ��Ϣ���ʹ���
    HI_U8 ucCurrStatusMsgTimes; //��ǰ״̬��ѯ��Ϣ���ʹ���
    MRS_3762_FRAME_STRU  *pstFrame;
	HI_U32 ulClearFileSeg; // �����װ�����еĶκ�
    HI_U32 ulCurrSegNum;  // ��ǰ���յ��Ķκ�
    HI_U32 ulUpgradeFileLen; // �����ļ�����
    HI_U8 ucUpgId;
    HI_U8 reserved1[3];
    HI_MDM_UPG_START_REQ_S *pstUpgFileInfo;//�����ļ��Ļ�����ָ��
}MRS_REMOTE_UPG_MODULE;

HI_U32 mrsCcoRemoteUpgInit(HI_VOID);
MRS_REMOTE_UPG_MODULE* mrsGetRemoteUpgCtx(HI_VOID);
HI_U32 mrsRemoteUpgMsgRcv(HI_U32 ulId, HI_PBYTE pucPacket, HI_U16 usPacketSize);
HI_VOID mrsUpgStatusMsgRx(HI_MDM_UPG_STATUS_BS_IND_S *pstInd);
HI_VOID mrsUpgStopMsgRx(HI_MDM_UPG_STOP_IND_S *pstInd);
HI_VOID mrsUpgLocalPro(MRS_REMOTE_UPG_MODULE *pstMrsUpgCtx);
HI_U32 mrsInitRemoteUpgNvDefault(NV_APP_MRS_UPG_PARAM_STRU *pstNvCfg);
HI_VOID mrsAFN15F1ReportAck(HI_U32 ulSeg, HI_U8 ucFrameSeq);
HI_VOID mrsRemoteUpgReset(MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx);
HI_U32 mrsRemoteUpgStart(HI_U16 ulId, HI_PVOID pData, HI_U32 ulFileSize);
HI_U32 mrsRemoteUpgStop(HI_U16 ulId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize);
HI_U32 mrsRemoteUpgStatus(HI_VOID);
HI_BOOL mrsIsUpgFileFlagValid(HI_U8 ucFileFlag, HI_U8 ucFileProperty);
HI_BOOL mrsUpgFileLenValid(HI_U8 *pContent, HI_U8 ucDataLenOff, HI_U8 ucCurrSegOff);
HI_BOOL mrsUpgFileStartLenValid(HI_U32 ulTotalSegNum, HI_U16 usDataLen, HI_U8 ucFileFlag);
HI_BOOL mrsUpgFileEndLenValid(HI_U32 ulTotalSegNum, HI_U16 usDataLen, HI_U8 ucFileFlag);
HI_BOOL mrsIsCcoStatePermitUpg(HI_VOID);
HI_U32 mrsRemoteUpgClear(MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx);
HI_U32 mrsRemoteUpgStartFramePro(HI_U8 * pContent);
HI_U32 mrsRemoteUpgMiddleFramePro(HI_U8 * pContent);
HI_U32 mrsRemoteUpgEndFramePro(HI_U8 * pContent);
HI_U32 mrsGetUpgType(HI_U8 ucFileFlag);
HI_U32 mrsUpgFileCheck(MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx);
HI_U32 mrsUpgMiddleEndFrameValid(HI_U8 * pContent, HI_U8 ucDataLenOff, HI_U8 ucCurrSegOff);
HI_VOID mrsUpgStopTimeOutPro(HI_VOID);
HI_VOID mrsUpgStatusTimeOutPro(HI_VOID);
HI_VOID mrsRemoteUpgRxFilePro(HI_VOID);
HI_VOID mrsUpgRxFileTimeOutPro(HI_VOID);
HI_BOOL mrsIsAllowCcoReset(HI_VOID);
HI_BOOL mrsIsCcoUpgStatus(HI_VOID);
HI_VOID mrsStartQueryUpgStatus(HI_U8 ucPowerFlag, MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx);
HI_BOOL mrsIsSegValid(HI_U8 * pContent, HI_U8 ucSegOffset, MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx);
HI_VOID mrsPowerOnGetUpgStatus(HI_VOID);
HI_U32 mrsRemoteUpgMsgProc(HI_SYS_QUEUE_MSG_S* pstMsg);
HI_BOOL mrsIsMdmUpgIdValid(HI_U32 ulMdmMsgId);
HI_VOID mrsPowerOnStatusTimeOut(HI_VOID);
 HI_U32 mrsUpgRespClearFile(HI_U8 ucFlag, MRS_REMOTE_UPG_MODULE *pstMrsUpgCtx);
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014032809465*/
HI_U32 mrsUpgStatusEndPro(MRS_REMOTE_UPG_MODULE *pstMrsUpgCtx);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
HI_U8 mrsGetRemoteUpgStatus(HI_VOID);
HI_U32 mrsRemoteUpgFrameRxPro(HI_U8 *pContent, HI_U32 ulSeg, HI_U16 usTotalSegNum);
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014032809465 */
HI_U32 mrsRemoteUpgFree(MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */

/* END:   Added by fengxiaomin/00209182, 2014/1/16 */

HI_END_HEADER
#endif //__MRS_SERVICE_H__

