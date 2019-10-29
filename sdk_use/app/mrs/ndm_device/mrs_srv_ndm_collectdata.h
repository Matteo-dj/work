/********************************************************************************
Copyright (C), Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
*********************************************************************************
File Name     : mrs_srv_ndm_collectdata.h
Version       : Initial draft
Author        : HCT
Created Date  : 2016-10-13
Last Modified by: c00233580
Description   : TODO: ...
Function List : TODO: ...
Change History: TODO: ...
********************************************************************************/

#ifndef _MRS_SRV_NDM_COLLECT_DATA_H_
#define _MRS_SRV_NDM_COLLECT_DATA_H_
#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)

#include <hi_cmn_nv.h>
#include <hi_mdm_datacollect.h>
#include "dfx_app.h"

HI_START_HEADER

#define MS_PER_MINUTES                                  (60*1000)
#define SEC_PER_MINUTE                                  (60)

#define MRS_NDM_DC_HSO_MAX_TIME                         (6*60*1000) // Max len of time for hso collect data.

#define MRS_DC_SD_REBOOT_START_RETRY_TIMES              10          // �ϵ�������ೢ��10��

#define MRS_DC_SD_NORMAL_PERIOD_RETRY_TIME              (60*1000)   // ��ͨ���ڲɷ�����Ϣʧ��������ʱʱ��
#define MRS_DC_SD_ZCROSS_RETRY_TIME                     (10*1000)   // ����ɷ���Ϣʧ��������ʱʱ��
#define MRS_DC_SD_WAIT_INTERRUPT_TIME                   (1*1000)    // �ȴ��жϳ�ʱʱ��
#define MRS_DC_SD_WAIT_TS_OVER_TIME                     (10*1000)   // �ȴ�ʱ϶��ʱʱ��
#define MRS_DC_SD_TS_ENOUGHT_TIME                       (20*1000)   // ������Ҫʱ϶ʱ��

#define MRS_DC_SD_NEED_SD_CARD_MIN_SIZE                 (3*1024)    // ����������СSD����С����λKB
#define MRS_DC_SD_SD_CARD_ALERT_SIZE                    (200*1024)  // SD�澯��С��С����λKB

#define MRS_DC_SD_ZCROSS_FILE_COUNT                     10          // ��������ļ�����

#define MRS_DC_SD_KEY_DOWN_ZCROSS_UP_MIN_TIME           (1000)      // ms
#define MRS_DC_SD_KEY_DOWN_ZCROSS_UP_MAX_TIME           (3000)      // ms
#define MRS_DC_SD_DEFAULT_NV_VALUE_RAM_SIZE             0x200000    //Ĭ�ϲ���RAM��С
#define MRS_DC_SD_DEFAULT_BEFORE_POINT_LEN              0x60000     //��ǰ�ɴ�С������
#define MRS_DC_SD_DEFAULT_AFTER_POINT_LEN               0x20000     //���ɴ�С,����
#define MRS_DC_SD_SD_CARD_MAX_ERR_TIMES                 5           // SD��������ڸ�ֵ��ֹͣ����
#define MRS_DC_SD_UNFOLLOW_MIN_PERIOD_TIME              3           // �Ǹ���ģʽ�£����ڲ���С���ڡ�minutes
#define MRS_DC_SD_FOLLOW_MIN_PERIOD_TIME                3           // ����ģʽ�£����ڲ���С���ڡ�minutes
#define MRS_DC_SD_UNFOLLOW_MAX_TIMES_PER_PERIOD         10          // �Ǹ���ģʽ�£����ÿ�ֲɼ�����
#define MRS_DC_SD_FOLLOW_TIMES_PER_PERIOD               1           // ����ģʽ�£����ڲ�ÿ�ֲɼ�����
#define MRS_DC_SD_UNFOLLOW_MIN_TRIGER_TIME              2           // �Ǹ��津���ɣ���С�����洢ʱ�䡣��λ����
#define MRS_DC_SD_FOLLOW_MIN_TRIGER_TIME                2           // ���津���ɣ���С�����洢ʱ�䡣��λ����

#define MRS_DC_SD_UN_FIX_AGCGAIN                        ((HI_S32)0xFFFFFFFF)

#define MRS_DC_SD_MAX_FILE_COUNT_PER_FOLDER             500         // ÿ���ļ�������������ж��ٸ��ļ�
#define MRS_DC_SD_FILE_NAME_MAX_LEN                     30          // SD��֧���ļ�������
#define MRS_DC_SD_MAX_SEQUENCE_FOLDER_NUM               255         // �������ļ������

#define MRS_DC_SD_MAX_SEQ_NUM                           1000        // �����ˮ��
#define MRS_DC_SD_DC_TYPE_BYTE_LEN                      5           // ��������
#define MRS_DC_SD_MAX_NAME_ID                           1000        // ���NameID.
#define MRS_DC_SD_MAX_TIME_SECOND                       1000000     // ���ʱ�䡣��λs
#define MRS_DC_SD_MAX_ERR_CODE                          0x10000     // ��������

#define MRS_DC_SD_BASE_FOLDER_NAME                      "/sd_dc"
#define MRS_DC_SD_BASE_FOLDER_NAME_SLANT_LEN            7                           // "/sd_dc/"
#define MRS_DC_SD_USER_FOLDER_NAME_SLANT_LEN            (NDM_DC_FOLDER_NAME_SIZE+1) // "DC_XXX/"
#define MRS_DC_SD_SEQUENCE_FOLDER_NAME_SLANT_LEN        4                           // "255/"
// �ļ�·�����ȣ��������ļ���
#define MRS_DC_SD_TOTAL_FOLDER_SLANT_LEN \
    (MRS_DC_SD_BASE_FOLDER_NAME_SLANT_LEN + MRS_DC_SD_USER_FOLDER_NAME_SLANT_LEN + MRS_DC_SD_SEQUENCE_FOLDER_NAME_SLANT_LEN)
// �ļ�·�� + �ļ��� ��󳤶�
#define MRS_DC_SD_TOTAL_PATH_NAME_LEN  \
    (MRS_DC_SD_TOTAL_FOLDER_SLANT_LEN + MRS_DC_SD_FILE_NAME_MAX_LEN)

// Data collect msg id
enum
{
    // Hso Dc
    EN_NDM_MSG_DC_HSO_UPLOAD_DATA,
    EN_NDM_MSG_DC_HSO_UPLOAD_START,
    EN_NDM_MSG_DC_HSO_UPLOAD_STOP,

    // Sd Dc
    EN_NDM_MSG_DC_SD_START,                 // ��ʼ����������
    EN_NDM_MSG_DC_SD_NORMAL_PERIOD,         // ��ͨ���ڲ�
    EN_NDM_MSG_DC_SD_NORMAL_TRIGER_MONITOR, // ��ͨ��������ѯ
    EN_NDM_MSG_DC_SD_ZCROSS,                // �����
    EN_NDM_MSG_DC_SD_FOLLOW_TRIGER_MONITOR, // ���津������ѯ
    EN_NDM_MSG_DC_SD_FOLLOW_PERIOD_MONITOR, // �������ڲ���ѯ
    EN_NDM_MSG_DC_STOP,                     // ֹͣ����
    EN_NDM_MSG_DC_REBOOT_START,             // �ϵ�����
    EN_NDM_MSG_DC_CHECK_TIME,               // Уʱ(����3518ʱ��)
};

// Hso collect data control struct
typedef struct
{
    HI_BUF_S strDcInfo;
    HI_PVOID pDCAddr;
} MRS_NDM_DC_HSO_CTRL_STRU;

// SD������������״̬
typedef enum
{
    MRS_DC_SD_MAIN_PHASE_STOPPED,        // ������ֹͣ���ʼ״̬
    MRS_DC_SD_MAIN_PHASE_STARTED,        // �����ѿ�ʼ
    MRS_DC_SD_MAIN_PHASE_STOPPING,       // ����ֹͣ������

    MRS_DC_SD_MAIN_PHASE_STARING_ID_ST,  // ��ʼ״̬��ʼID
    MRS_DC_SD_MAIN_PHASE_STARING_NORMAL = MRS_DC_SD_MAIN_PHASE_STARING_ID_ST, //��ͨ������ʼ��
    MRS_DC_SD_MAIN_PHASE_STARTING_PROMPT_ZCROSS_UP,      //��ʾ�����ز���
    MRS_DC_SD_MAIN_PHASE_STARTING_ZCROSS_UP,             //�����ز�����ʼ��
    MRS_DC_SD_MAIN_PHASE_STARTING_PROMPT_ZCROSS_DOWN,    //��ʾ�½��ز���
    MRS_DC_SD_MAIN_PHASE_STARTING_ZCROSS_DONW,           //�½��ز�����ʼ��
    MRS_DC_SD_MAIN_PHASE_STARTING_REBOOT,                //�ϵ����ɿ�ʼ������
    MRS_DC_SD_MAIN_PHASE_STARING_ID_END = MRS_DC_SD_MAIN_PHASE_STARTING_REBOOT,//��ʼ״̬����ID
} MRS_DC_SD_MAIN_PHASE_E;

// SD������������״̬�仯 ����ԭ��
typedef enum
{
    MRS_DC_SD_CHANGE_PHASE_REASON_KEY_DOWN,     //��������
    MRS_DC_SD_CHANGE_PHASE_REASON_KEY_UP,       //����̧��
    MRS_DC_SD_CHANGE_PHASE_REASON_REBOOT_START, //��������

    MRS_DC_SD_CHANGE_PHASE_REASON_SD_CADR_ERR,  //Sd������
    MRS_DC_SD_CHANGE_PHASE_REASON_COMPLETE,     //�������
    MRS_DC_SD_CHANGE_PHASE_PROCEES_NEED,        //��������ԭ��,���̵����Ӧ�������仯
} MRS_DC_SD_CHANGE_PHASE_REASON_E;

//��������ԭ��
typedef enum
{
    MRS_DC_SD_START_TYPE_KEY_NORNAL,
    MRS_DC_SD_START_TYPE_KEY_ZCROSS_UP,
    MRS_DC_SD_START_TYPE_KEY_ZCROSS_DOWN,
    MRS_DC_SD_START_TYPE_REBOOT,
} MRS_DC_SD_START_TYPE_E;

//�����޷���ʼ����ԭ��
typedef enum
{
    MRS_DC_SD_NOT_START_REASON_NONE,

    MRS_DC_SD_NOT_START_REASON_MEM_GET_FAIL,            // ��ȡ�ڴ�ռ�ʧ��
    MRS_DC_SD_NOT_START_REASON_NV_READ_FAIL,            // ��ȡNVʧ��
    MRS_DC_SD_NOT_START_REASON_NV_WRITE_FAIL,           // дNVʧ��
    MRS_DC_SD_NOT_START_REASON_NV_CONFIG_WRONG,         // NV���ô���
    MRS_DC_SD_NOT_START_REASON_PREPARE_FAIL,            // ����׼��ʧ��
    MRS_DC_SD_NOT_START_REASON_SEND_CHECK_TIME_FAIL,    // ����Уʱ��Ϣʧ��

    MRS_DC_SD_NOT_START_REASON_SD_NOT_EXIST = 0x10, // SD������λ
    MRS_DC_SD_NOT_START_REASON_SD_GET_SIZE_FAIL,
    MRS_DC_SD_NOT_START_REASON_SD_NO_SPACE,         // �ռ䲻�� (����ʹ��)
    MRS_DC_SD_NOT_START_REASON_SD_API_CALL_FAIL,    // ����APIʧ��
    MRS_DC_SD_NOT_START_REASON_REGIST_MAC_FUNC_FAIL = 0x20,//MAC����
    MRS_DC_SD_NOT_START_REASON_ENABLE_FOLLOW_FAIL,  // �ռ伴������ (�ﵽ�澯ֵ)

} MRS_DC_SD_NOT_START_REASON_E;

// ���������������
typedef enum
{
    MRS_DC_SD_ZCROSS_ISR_TYPE_FALL,
    MRS_DC_SD_ZCROSS_ISR_TYPE_RISE,
    MRS_DC_SD_ZCROSS_ISR_TYPE_UNKNOWN,
} MRS_DC_SD_ZCROSS_ISR_TYPE_E;

// SD������ֹͣԭ��
typedef enum
{
    MRS_DC_SD_STOP_TYPE_KEY,                // ����ֹͣ����
    MRS_DC_SD_STOP_TYPE_SD_CARD_ERR_TIMES,  // SD������ֹͣ����
    MRS_DC_SD_STOP_TYPE_COMPLETE,           // ��������Զ�ֹͣ
} MRS_DC_SD_STOP_TYPE_E;

//��ʾID
typedef enum
{
    MRS_DC_SD_DISPLAY_DC_NONE,              // ����״̬����
    MRS_DC_SD_DISPLAY_DC_NORMAL,            // ����״̬����ͨ�������������
    MRS_DC_SD_DISPLAY_DC_ZERO_CROSS_UP,     // ����״̬: ���������ز���
    MRS_DC_SD_DISPLAY_DC_ZERO_CROSS_DOWN,   // ����״̬�������½��ز���

    MRS_DC_SD_DISPLAY_ALERT_FULL,           // SD���ռ䲻��
    MRS_DC_SD_DISPLAY_ALERT_SD_ERR,         // SD������
    MRS_DC_SD_DISPLAY_ALERT_NONE,           // �޸澯
} MRS_DC_SD_DISPLAY_E;

// SD���澯״̬
typedef enum
{
    MRS_DC_SD_ALERT_TYPE_NONE,          // �޸澯
    MRS_DC_SD_ALERT_TYPE_SD_CARD_FULL,  // �ռ䲻��
    MRS_DC_SD_ALERT_TYPE_SD_CARD_ERR,   // SD������
    MRS_DC_SD_ALERT_TYPE_SD_CARD_MAX
} MRS_DC_SD_ALERT_TYPE_E;


#define MRS_DC_SD_DATA_COLLECT_TYPE_NOISE       (0x0001)
#define MRS_DC_SD_DATA_COLLECT_TYPE_FC          (0x0002)
#define MRS_DC_SD_DATA_COLLECT_TYPE_STAT4       (0x0004)
#define MRS_DC_SD_DATA_COLLECT_TYPE_DEFAULT     (0x0008)    /*������DRV�޹� ���ڲ�������*/

//�ļ�����,�����ļ����Ͳ�ͬ�洢��ͬ�ļ���
typedef enum
{
    MRS_DC_SD_SAVE_FILE_TYPE_A,     //ʱ϶����ʱ���㹻
    MRS_DC_SD_SAVE_FILE_TYPE_P,     //ʱ϶����ʱ�䲻��

    MRS_DC_SD_SAVE_FILE_TYPE_D,     //ä��
    MRS_DC_SD_SAVE_FILE_TYPE_F,     //FC
    MRS_DC_SD_SAVE_FILE_TYPE_N,     //����
    MRS_DC_SD_SAVE_FILE_TYPE_S,     //״̬4
    MRS_DC_SD_SAVE_FILE_TYPE_ZCROSS_FALL,//�����½���
    MRS_DC_SD_SAVE_FILE_TYPE_ZCROSS_RISE,//����������
    MRS_DC_SD_SAVE_FILE_TYPE_MAX,
} MRS_DC_SD_SAVE_FILE_TYPE_E;

// ���в�������
#define MRS_DC_SD_DATA_COLLECT_TYPE_ALL             \
    (MRS_DC_SD_DATA_COLLECT_TYPE_NOISE | MRS_DC_SD_DATA_COLLECT_TYPE_FC | \
     MRS_DC_SD_DATA_COLLECT_TYPE_STAT4 | MRS_DC_SD_DATA_COLLECT_TYPE_DEFAULT)
// �Ǹ���: ��������
#define MRS_DC_SD_DATA_COLLECT_TYPE_UNFOLLOW_ALL    \
    (MRS_DC_SD_DATA_COLLECT_TYPE_NOISE | MRS_DC_SD_DATA_COLLECT_TYPE_FC | \
     MRS_DC_SD_DATA_COLLECT_TYPE_STAT4 | MRS_DC_SD_DATA_COLLECT_TYPE_DEFAULT)
// ����: ��������
#define MRS_DC_SD_DATA_COLLECT_TYPE_FOLLOW_ALL      \
    (MRS_DC_SD_DATA_COLLECT_TYPE_NOISE | MRS_DC_SD_DATA_COLLECT_TYPE_DEFAULT)
// ����: ��������
#define MRS_DC_SD_DATA_COLLECT_TYPE_NOISE_ALL       \
    (MRS_DC_SD_DATA_COLLECT_TYPE_NOISE | MRS_DC_SD_DATA_COLLECT_TYPE_DEFAULT)
// ����:��������
#define MRS_DC_SD_DATA_COLLECT_TYPE_TRI_ALL         \
    (MRS_DC_SD_DATA_COLLECT_TYPE_NOISE | MRS_DC_SD_DATA_COLLECT_TYPE_FC | MRS_DC_SD_DATA_COLLECT_TYPE_STAT4)

HI_EXTERN HI_NDM_DATA_COLLECT_STATUS_MSG_S g_mrsNdmDcSdCtrl;

#define MRS_NDM_DC_SD_GET_MAIN_CTRL_CTX()       (&g_mrsNdmDcSdCtrl.stMainCtrlCtx)
#define MRS_NDM_DC_SD_GET_ERR_CTX()             (&g_mrsNdmDcSdCtrl.stErrCtx)
#define MRS_NDM_DC_SD_GET_CONFIG_CTX()          (&g_mrsNdmDcSdCtrl.stConfigCtx)
#define MRS_NDM_DC_SD_GET_STAT_CTX()            (&g_mrsNdmDcSdCtrl.stStatCtx)

/**
 @brief NDM HSO collect data.
    HSO collect data steps:
        1. HSO send cmd ID_DIAG_CMD_DRV_UPLOAD_DATA_START to collect data.
        2. board begin collect data with params from cmd.
            if success, return HI_ERR_SUCCESS and data size to HSO, and then, upload 10k data with mrsNdmMsgDcUploadData function.
            if fail, return err code.
        3. when HSO has received first 10k data, if need more data, HSO send cmd ID_DIAG_CMD_DRV_UPLOAD_DATA for next 10k data.
        4. when board get ID_DIAG_CMD_DRV_UPLOAD_DATA_STOP cmd, stop collect data.
 @attention \n
N/A
 @see \n
N/A
 */
HI_U32 mrsNdmDcHsoDiagCmdProc(HI_U16 usId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

HI_U32 mrsNdmDcMsgProc(HI_U16 usId, HI_PVOID pMsgParam);

HI_U32 mrsNdmDcSendSubMsg(HI_U32 ulMsgId, HI_PVOID pData, HI_U32 ulDataSize, HI_U32 ulOption);

HI_U32 mrsNdmMsgDcUploadDataStart(HI_U16 usId, HI_PVOID pMsgParam);
HI_U32 mrsNdmMsgDcUploadData(HI_U16 usId, HI_PVOID pMsgParam);
HI_U32 mrsNdmMsgDcUploadDataStop(HI_U16 usId, HI_PVOID pMsgParam);

HI_VOID mrsNdmDcHsoTimeout(HI_VOID);

/**
 @brief KEY Collect Data.
 */

/**
 @brief     Handle collect data key up/down event.
 @attention \n
N/A
 @param[in] bIsKeyDown is key down.
 @param[in] uMs key press time, measure in millisecond.
 @see \n
N/A
 */

// HSO�����в�ѯSD������״̬
HI_U32 mrsNdmMsgDcSdReportStat(HI_U16 usId, HI_PVOID pMsgParam);
// HSO->���Կ���̨->���ݲɼ�->�洢�ɼ�����: ����SD����������
HI_U32 mrsNdmMsgDcSdSetConfig(HI_U16 usId, HI_PVOID pMsgParam);
// HSO->���Կ���̨->���ݲɼ�->�洢�ɼ�����: ��ѯSD����������
HI_U32 mrsNdmMsgDcSdQueryConfig(HI_U16 usId, HI_PVOID pMsgParam);

// ������Ϣ����
HI_VOID mrsNdmDcKeyMsgProc(HI_BOOL bIsKeyDown, HI_U32 uMs);
// ���Ͳ������̿�ʼ��Ϣ
HI_U32 mrsNdmDcSdSendStartMsg(MRS_DC_SD_START_TYPE_E eStartType);
// ��ʼ��������
HI_U32 mrsNdmDcSdStart(HI_VOID);
// ����׼��
HI_U32 mrsNdmDcSdPrepare(HI_VOID);
// ���в���
HI_U32 mrsNdmDcSdCollect(HI_VOID);
// �����ָ�
HI_U32 mrsNdmDcSdRestore(HI_VOID);
// ����ֹͣ��������
HI_U32 mrsNdmDcSdStop(MRS_DC_SD_STOP_TYPE_E eStopType);
// ֹͣ�������̵�һ�׶�
HI_U32 mrsNdmDcSdStopPhase1(MRS_DC_SD_STOP_TYPE_E eStopType);
// ֹͣ�������̵ڶ��׶�
HI_U32 mrsNdmDcSdStopPhase2(MRS_DC_SD_STOP_TYPE_E eStopType);
// ����ֹͣ������Ϣ
HI_U32 mrsNdmDcSdStopMsgHandle(HI_U16 usId, HI_PVOID pMsgParam);

// ��ͨ���ڲ�: ���Ϳ�ʼ��Ϣ
HI_U32 mrsNdmDcSdSendPeriodCollectMsg(HI_VOID);
// ��ͨ���ڲ�: �ɼ�����
HI_U32 mrsNdmDcSdNromalPeriodCollect(HI_VOID);
// ��ͨ������: ��ѯ��ѯ��ʱ��������
HI_U32 mrsNdmDcSdNormalTrigerTimeout(HI_VOID);
// ��ͨ������: �����յ�PB��״̬
HI_U32 mrsNdmDcSdNormalTrigerPbMonitor(HI_VOID);
// ��ͨ������: �ɼ�����
HI_U32 mrsNdmDcSdTrigerCollect(HI_VOID);
// ��ͨ��: �ɼ�����
HI_U32 mrsNdmDcSdNormalCollect(DC_TRIG_TYPE_ENUM eCollectType, MRS_DC_SD_SAVE_FILE_TYPE_E eSaveFileType);

// �����: ��ѯ
HI_U32 mrsNdmDcSdFollowTimeout(HI_VOID);
// �����: �Ƿ񱣴��ļ�
HI_BOOL mrsNdmDcSdFollowIsSaveFile(HI_U8 ucSceneType);
// ���津����: ��ѯ: �����յ�PB��״̬
HI_U32 mrsNdmDcSdFollowTrigerPbMonitor(HI_VOID);
// ���津����: �ɼ�����
HI_U32 mrsNdmDcSdFollowTrigerCollect(DC_TRIG_TYPE_ENUM eCollectType);
// �������ڲ�: ��ѯ
HI_U32 mrsNdmDcSdFollowPeriodMonitor(HI_VOID);
// �������ڲ�: �ɼ�����
HI_U32 mrsNdmDcSdFollowPeriodCollect(DC_TRIG_TYPE_ENUM eCollectType);

// �����: ���й����
HI_U32 mrsNdmDcSdZcrossCollect(HI_VOID);
// �����: ֪ͨ������һ�βɼ�
HI_U32 mrsNdmDcSdNotifyDoNextZcrossCollect(HI_VOID);


/* ע��: ����ͨ�ɺ͸����֧�ֶϵ�����  */

// �ϵ�����: ����Ƿ���Ҫ�ϵ����ɣ�����Ҫ������ͨ����ѯ��ʱ��
HI_U32 mrsNdmDcSdCheckRebootStart(HI_VOID);
// �ϵ�����: ��ѯ��ʱ������
HI_U32 mrsNdmDcSdRebootCheckTimeout(HI_VOID);
// �ϵ�����: ���жϵ�����
HI_U32 mrsNdmDcSdRebootStart(HI_VOID);


// ���ɲ����������
HI_U32 mrsNdmDcGenerateInputParam(HI_IN DC_TRIG_TYPE_ENUM eCollectType, HI_OUT DC_INPUT_PARA_STRU * pstDcInParam);
// �洢�ļ�
HI_U32 mrsNdmDcSdSaveFile(HI_BUF_S *pstBuf, MRS_DC_SD_SAVE_FILE_TYPE_E eSaveFileType, HI_U32 ulErrCode);
// ��鴴���ļ���
HI_U32 mrsNdmDcSdCheckCreatFolder(HI_VOID);
// �����ļ���
HI_U32 mrsNdmDcSdGenerateFileName(HI_IN MRS_DC_SD_SAVE_FILE_TYPE_E eSaveFileType, HI_IN HI_U32 ulErrCode,
                                  HI_OUT HI_CHAR *scFileName, HI_OUT HI_U32 *pulLen);
// ���ļ��洢��SD��
HI_U32 mrsNdmDcSdSaveFileToSdCard(HI_BUF_S * pstBuf, HI_CHAR * scFileName, HI_U32 ulFileNameLen);


// ���SD��״̬
HI_U32 mrsNdmDcSdCheckSdCardStat(HI_OUT MRS_DC_SD_ALERT_TYPE_E * eSdAlertType);
// ��ʼ����������
HI_U32 mrsNdmDcSdInitConfig(MRS_DC_SD_START_TYPE_E eStartType);
// ���NV����
HI_U32 mrsNdmDcSdCheckConfig(HI_VOID);
// д���õ�NV
HI_U32 mrsNdmDcSdWriteConfigToNv(HI_BOOL bIsColleting, HI_U16 usNameId);


// ����������״̬
HI_U32 mrsNdmDcSdChangeMainPhase(MRS_DC_SD_CHANGE_PHASE_REASON_E eTrigerReason, HI_U32 ulKeyDownUpTime);
// ��ʾLED
HI_VOID mrsNdmDcSdLedDisplay(MRS_DC_SD_DISPLAY_E displayId);
// ����SD���������LED��ʾ
HI_VOID mrsNdmDcSdUpdateSdCardLedAlert(MRS_DC_SD_ALERT_TYPE_E eSdAlertType);
// 3518Уʱ (�뱣���ļ�ʱ���й�)
HI_U32 mrsNdmDcSdRtcSetTime(HI_VOID);


// ���ͨ���ļ���
HI_U32 mrsNdmDcSdCheckFolderName(HI_CHAR scFolderName[NDM_DC_FOLDER_NAME_SIZE]);
// ����������
HI_U32 mrsNdmDcSdCheckTrigerType(HI_U16 usTrigerType, HI_BOOL bIsFollowMode);
// ���RAM ADDR SIZE
HI_U32 mrsNdmDsSdCheckAddrAndLen(HI_U32 ulStartAddr, HI_U32 ulEndAddr, HI_U32 ulAfterPointlen);
// �������AGC
HI_U32 mrsNdmDcSdCheckFixAgcGain(HI_S32 slAgcGain);
// �������ʱ��
HI_U32 mrsNdmDcSdCheckPeriodTime(HI_U16 usMinute, HI_BOOL bFollowMode);
// ���ÿ��֧�ֲ�������
HI_U32 mrsNdmDcSdCheckTimesPerPeriod(HI_U8 ucTimes, HI_BOOL bFollowMode);
// ����������ʱ��
HI_U32 mrsNdmDcSdCheckTrigerTime(HI_U16 usMinute, HI_BOOL bFollowMode);

HI_END_HEADER
#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)

#endif // _MRS_SRV_NDM_COLLECT_DATA_H_

