//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_fw_nv.h
//  �� �� ��   : V1.0
//  ��    ��   : fengxiaomin/f00209182
//  ��������   : 2014-7-17
//  ��������   : NV����ش��������ӿڶ���
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2014-7-17
//    ��    �� : fengxiaomin/f00209182
//    �޸����� : �����ļ�
//
//*****************************************************************************
#ifndef __MRS_FW_NV_H__
#define __MRS_FW_NV_H__
HI_START_HEADER

#define MRS_SM_BC_SEND_INTERVAL     (30)    // 3000ms����3��
#define MRS_SM_BC_LOOP_MAX          (5)     // 5��
#define MRS_SM_QUERY_MAX_WAY        (10)    // 10������
#define MRS_SM_QUERY_INTERVAL       (2)     // 2s
#define MRS_SM_QUERY_FORCE_TIMES    (4)
#define MRS_SM_QUERY_EXCEPT_TIMES   (5)
#define MRS_SM_TOPO_QUERY_INTERVAL  (5)     // 5min
#define MRS_SM_TOPO_STABLE_TIMES    (6)     // ����6��δ���
/* BEGIN: PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/11 */
#define MRS_SM_REPORT_AA_2_MASTER   (0)
/* END:   PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/11 */

#define MRS_SM_INVALID_FRM_LEN_07   (1)
#define MRS_SM_INVALID_FRM_LEN_97   (2)

#define MRS_SM_DURATION_DEFALUT             (180)   // �ѱ�Ĭ�ϳ�ʱʱ������λmin
#define MRS_SM_BIND_CMD_INTERVAL            (5)     // �����������ļ������λs
#define MRS_SM_CHECK_MAIN_NODE_ADDR         (1)     // ���ڵ��ַ�仯ʱ�Ƿ�����̨��ʶ��
#define MRS_SM_ALLOW_SM_IN_TF               (1)     // ̨��ʶ�����Ƿ������ѱ�
#define MRS_SM_CHECK_MAIN_MODE_TIMEOUT      (300)   // ������ڵ��ַ��ʱʱ�䣬��λs
#define MRS_SM_REPORT_WAIT_LOCK_MSG         (0)     // �ѱ��ϱ��Ƿ�ȴ���������
#define MRS_SM_BIND_CMD_SEND_MAX_WAY        (3)
#define MRS_SM_BIND_CMD_SEND_MAX_TIMES      (6)
#define MRS_SM_LEAVE_TIME_LEN_TH            (6*60) // STA����ʱ�����ޣ���λmin
#define MRS_SM_CLOSE_WHITE_WHEN_SNR_OPEN    (1)     // SNR�򿪵�ʱ���Ƿ�رհ�����

#define MRS_TF_QUERY_INTERVAL               (5)
#define MRS_TF_QUERY_ROUND_INTERVAL         (300)
#define MRS_TF_QUERY_BEGIN_DELAY_TIME       (1800)
#define MRS_TF_QUERY_MAX_WAY                (3)
#define MRS_TF_QUERY_MAX_TRY                (50)

#define MRS_DETECT_BAUDRATE_LOOP_MAX          (4)            // ̽�Ⲩ�����ִ�
#define MRS_DETECT_BAUDRATE_TIMEOUT           (1800)         // ̽�Ⲩ����: ��ͨ�ŵ�ַ���γ�ʱʱ��, ��λ:����
#define MRS_DETECT_BAUDRATE_PERIOD            (60)           // ̽�Ⲩ����: ����, ��λ: ��
#define MRS_CLTI_SM_QUERY_INTERVAL            (30)           // �ѱ������: ��ѯ�ѱ����ļ�� ��λ:��
#define MRS_CLTI_READ_POSITIVE_A_MAX          (4)            // �ϵ�������: ��NV�д洢�ı��ַ�������й���������
#define MRS_CLTI_READ_POSITIVE_A_INTERVAL     (2)            // �ϵ�������: ��NV�д洢�ı��ַ�������й���ʱ���� ��λ:��
#define MRS_CLTII_SM_DETECT_TIMEOUT           (50)           // II���ѱ���̽���ѱ���֡��ʱʱ��, ��λ:�ٺ���

#define MRS_CLTII_SM_EXPLORE_BYTE_COUNT_MAX_DEFAULT       (0)                      // �����ֽ�������Ĭ��ֵ0
#define MRS_CLTII_SM_EXPLORE_BYTE_COUNT_MAX_UPPERBOUND    (HI_METER_ADDR_LEN - 1)  // �����ֽ������޵����Ϸ�ֵ

#define MRS_SM_FRAME_TIMEOUT        (500)
#define MRS_SM_BYTE_TIMEOUT         (60)
#define MRS_SM_MAX_METERMUM         (MRS_MAX_METERNUM)
#define MRS_SM_DI_VER2007           (0x04000401)
#define MRS_SM_DI_VER1997           (0xC032)
#define MRS_SM_DI_VER1997B          (0x9010)
#define MRS_SM_DI_VER1997C          (0x901F)
#define MRS_SM_SEARCH_BC_ENABLE     (HI_TRUE)
#define MRS_SM_SEARCH_RETRY         (1)

#define MRS_SM_SEARCH_PERIOD        (60 * 24)   // 24h, ��λmin

#define MRS_CCO_EVT_REPORT_RETRY_TIMES_MAX  (2)
#define MRS_CCO_EVT_REPORT_TIMEOUT          (3000) //��λ:����
#define MRS_CCO_EVT_FILTER_AGED_TIME        (30)   //��λ:����
#define MRS_CCO_EVT_BUFFER_SIZE             (200)
#define MRS_CCO_EVT_STAT_BAK_TIME           (720) //��λ:����

#define MRS_STA_EVT_REPORT_TIMEOUT_DEFAULT  (3)  //��λ:��
#define MRS_STA_EVT_REPORT_TIMES_MAX        (41)
#define MRS_STA_EVT_REPORT_TIMEOUT_MAX      (60) //��λ:��
#define MRS_STA_CONTINUOUS_DENY_TIMES_MAX   (5)

#define MRS_STA_EVT_MONITOR_INTERVAL_DEFAULT  (5)  //��λ:����


#define MRS_SM_00_TO_DEFAULT(x,d)   do { if((x) == 0) {(x) = (d);} } while(0)

#define MRS_NODE_STATE_CHG_REPORT_ENABLE_DEFAULT             (1)    // Ĭ�������ϱ�
#define MRS_NODE_STATE_CHG_REPORT_REASON_POWER_OFF_DEFAULT   (1)    // Ĭ�������ϱ�ͣ��ԭ��
#define MRS_NODE_STATE_CHG_REPORT_REASON_PLC_CHL_DEFAULT     (1)    // Ĭ�������ϱ��ŵ�ԭ��
#define MRS_NODE_STATE_CHG_REPORT_CACHE_NUM_MAX_DEFAULT      (30)   // �ϱ�ǰ������������
#define MRS_NODE_STATE_CHG_REPORT_CACHE_TIMEOUT_DEFAULT      (50)   // ���������ȴ���ʱ�䣬��λ: 100����
#define MRS_NODE_STATE_CHG_REPORT_FRAME_TIMEOUT_DEFAULT      (10000) // �ϱ����������ȴ���ʱ�䣬��λ: ����
#define MRS_NODE_STATE_CHG_REPORT_FRAME_MAX_TRY_DEFAULT      (2)    // �ϱ�3762֡������Դ���
#define MRS_NODE_STATE_CHG_REPORT_CACHE_NUM_MAX_LOWERBOUND   (1)    // �ϱ�ǰ����������������
#define MRS_NODE_STATE_CHG_REPORT_CACHE_NUM_MAX_UPPERBOUND   (80)   // �ϱ�ǰ���������������ޣ��ɱ��ĳ���1024�Ƴ�
#define MRS_POWEROFF_DETECT_MAX_TRIES_DEFAULT               (5)     // ͣ��̽������Դ���
#define MRS_POWEROFF_DETECT_INTERVAL_DEFAULT                (3)     // ͣ��̽��㲥���ķ��ͼ��
#define MRS_POWEROFF_RECHECK_MAX_TRIES_DEFAULT              (10)    // ͣ�縴������Դ���
#define MRS_POWEROFF_RECHECK_INTERVAL_DEFAULT               (30)    // ͣ�縴�˹㲥���ķ��ͼ��
#define MRS_POWEROFF_REVIEW_SCORE_FULL_DEFAULT              (7)     // ͣ���⸴�����ַ���
#define MRS_POWEROFF_WRONG_REPORT_REDUCE_SCORE_DEFAULT      (3)     // ��ʱ�����ַ���
#define MRS_POWEROFF_MISS_REPORT_PLUS_SCORE_DEFAULT         (1)     // ©��ʱ���ӷַ���
#define MRS_POWEROFF_CORRECT_REPORT_PLUS_SCORE_DEFAULT      (1)     // ��ȷ�ϱ�ʱ���ӷַ���
#define MRS_POWEROFF_NONE_PASS_DETECT_PLUS_SCORE_DEFAULT    (1)     // ������վ��û��̽��ɹ��ļ�¼ʱ���ӷַ���
#define MRS_POWEROFF_NONE_PASS_DETECT_PERIOD_DEFAULT        (24*60*60)  // վ���Ƿ���̽��ɹ���¼ʶ������
#define MRS_POWEROFF_REPORT_MAX_DELAY_TIME_DEFAULT          (5*60)  // ��ȷ�ϱ����ϱ�ʱ��-ͣ��ʱ��ܴ�ʱ��
#define MRS_POWEROFF_MAC_REPORT_CACHE_MAX_NUM_DEFAULT       (30)    // MAC�ϱ�����������
#define MRS_POWEROFF_MAC_REPORT_CACHE_MAX_TIME_DEFAULT      (2)     // MAC��������ʱ��
#define MRS_POWEROFF_SCALE_POWER_OFF_DETECT_DEFAULT         (48)     // �ϱ�ͣ��վ����Ŀ�ﵽ10��ʱ��̽��ʱ������

// Local: STA/CltII <--> Meter, CltI Module <--> CltI
#define MRS_LOCAL_RM_FRAME_HEAD_DEFAULT   (1800)  // Frame head timeout default
#define MRS_LOCAL_RM_FRAME_HEAD           (1000)  // Frame head timeout
#define MRS_LOCAL_RM_FRAME_INTERVAL       (200)   // Frame Interval timeout
#define MRS_LOCAL_RM_CTRL_CMD_DELAY       (2000)  // Correction for CTRL command
#define MRS_LOCAL_RM_METER_INTERVAL       (50)    // Interval between two 645 frame
#define MRS_LOCAL_RM_QUEUE_MAX            (16)    // Max queue length of STA/CltII
#define MRS_LOCAL_RM_METER_TIMEOUT_RETRY  (0)     // Timeout retry of STA/CltII
#define MRS_LOCAL_RM_METER_DENY_RETRY     (0)     // Deny retry of STA/CltII

// CltI: CltI Module <--> CltI <--> Meter
#define MRS_CLTI_RM_FRAME_HEAD            (3600)  // Frame head timeout
#define MRS_CLTI_RM_FRAME_INTERVAL        (200)   // Frame Interval timeout
#define MRS_CLTI_RM_CTRL_CMD_DELAY        (2000)  // Correction for CTRL command
#define MRS_CLTI_RM_METER_INTERVAL        (50)    // Interval between two 645 frame
#define MRS_CLTI_RM_QUEUE_MAX             (16)    // Max queue length of CLTI Module
#define MRS_CLTI_RM_METER_TIMEOUT_RETRY   (0)     // Timeout retry of CltI
#define MRS_CLTI_RM_METER_DENY_RETRY      (0)     // Deny retry of CltI


HI_U32 mrsNvChangeMsgNotify(HI_U16 usId);
HI_U32 mrsNvChangeMsgProc(HI_SYS_QUEUE_MSG_S* pstMsg);
HI_U32 mrsNvStaSmCfg(HI_U16 usId);
HI_U32 mrsNvStaSmCfgExp(HI_U16 usId);
HI_U32 mrsStaSearchFtmNvInit(HI_VOID);

//*****************************************************************************
// ��������: MRS_GetMrNvCfg
// ��������: ��ȡ������ص�NV������
//
// ����˵��:
//   pstCfg [out] NV_ASL_MR_CFG_STRU�ṹ
//
// �� �� ֵ:
//    TODO: ...
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-10]
// ��    ��: cuiate/00233580     [2013-03-18]
//*****************************************************************************
HI_U32 mrsInitAppMrNvInf(HI_VOID);
HI_U32 mrsInitStaMrNvInf(HI_VOID);
HI_U32 mrsNvCcoFtParam(HI_U16 usId);
HI_U32 mrsNvCcoSearchMeterCfg(HI_U16 usId);
HI_U32 mrsNvCcoSearchMeterCfgExp(HI_U16 usId);
HI_U32 mrsNvRmSyncCfg(HI_U16);
//Nv��ı�֪ͨ
HI_U32 mrsNvDutFlagCfg(HI_U16 usId);
HI_U32 mrsNvRemoteUpgInfCfg(HI_U16 usId);
HI_U32 mrsNvSimuStateCfg(HI_U16 usId);
HI_U32 mrsNvMaxTurnNumCfg(HI_U16 usId);
HI_U32 mrsNvInitBaudRateCfg(HI_U16 usId);
HI_U32 mrsNvCltIIRefresh(HI_U16 usId);
// ��ȡ�¼����NV��
HI_U32 mrsSrvGetEvtNvCfg(NV_APP_MRS_EVT_CFG_STRU *);
HI_U32 mrsNvStaEvtCfg(HI_U16 usId);
HI_U32 mrsNvCcoEvtCfg(HI_U16 usId);
HI_U32 mrsNvUserVerCfg(HI_U16 usId);

HI_U32 mrsGetUserVerNvCfg(NV_MRS_USER_VER_S* pstVer);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_U32 mrsNVSetUserVerInfo(NV_MRS_USER_VER_S * pstInfo);
HI_U32 mrsNVSetszId(HI_CHAR  szId[3]);
HI_U32 mrsNVGetszId(HI_NV_FTM_MANUFACOTR_ID_S * pstInfo);
HI_U32 mrsNVSetClientId(HI_CHAR szId[3]);
HI_U32 mrsNVGetClientId(HI_VOID *pstInfo);

#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_U32 mrsNvNodeStateChgReportCfg(HI_U16 usId);
HI_U32 mrsCcoSetProto(HI_U8 ucProto);
HI_U32 mrsNvSetUpgId(HI_U8 ucUpgId);
HI_U32 mrsCcoReloadNvVerCfg(HI_VOID);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_U32 mrsNvStaRmCfg(HI_U16 usId);
HI_U32 mrsStaReloadNvVerCfg(HI_VOID);
#endif

HI_U32 mrsNvUserCfg(HI_U16 usId);
HI_END_HEADER
#endif //__MRS_SERVICE_H__

