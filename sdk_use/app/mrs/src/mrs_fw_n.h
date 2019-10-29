//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_fw_n.h
//  �� �� ��   : V1.0
//  ��    ��   : ����/00130424
//  ��������   : 2011-10-25
//  ��������   : MRS��������ڲ��ӿ�, ����mrs_fw.c�ļ�ʹ�ã������ļ�����ʹ��
//  �����б�   : NA
//  �޸���ʷ   :
//  1.��    �� : 2011-10-25
//    ��    �� : ����/00130424
//    �޸����� : �����ļ�
//*****************************************************************************

#ifndef __MRS_FW_N_H__
#define __MRS_FW_N_H__
HI_START_HEADER


#include "mrs_fw_proto645.h"
#include "mrs_fw_proto376_2.h"


// 0 -- 0x100 Ϊ�ڲ���Ϣ
#define MRS_MSG_BASE  0x1   // ע����ndm_eqt_common.h�е�NDM_EQT_MSG_BASE�Ŷβ����ظ�


#define MRS_MSG_BASE_MAX_NUM    (PRODUCT_CFG_Q_APP_MRS_MSG_BASE_NUM)

enum
{
    EN_MRS_FW_MSG_376_2_FRAME_INPUT = MRS_MSG_BASE,
    EN_MRS_FW_MSG_645_FRAME_INPUT,
    EN_MRS_FW_MSG_698_FRAME_INPUT,
    EN_MRS_FW_MSG_TRANSPARENT_FRAME_INPUT,
    EN_MRS_FW_MSG_MAC_FRAME_INPUT,
    EN_MRS_FW_MSG_MAC_FRAME_OUTPUT,
    EN_MRS_FW_MSG_MR_FRAME_OUTPUT,
    EN_MRS_FW_MSG_MDM_INPUT,
    EN_MRS_FW_MSG_RM_PLC_SEND_REQ,
    EN_MRS_FW_MSG_NV_CHANGE_NOTIFY,
    EN_MRS_FW_MSG_ZERO_CROSS_NOTIFY,
    EN_MRS_FW_MSG_MR_RETRY_DIAG_CMD,
    EN_MRS_FW_MSG_MAC_ND_STATE_CHG,
    EN_MRS_FW_MSG_MAC_MSG,
    EN_MRS_FW_MSG_CLTII_EVENT_MON,
    EN_MRS_FW_MSG_MAC_JOIN_WHITE_LIST,
	EN_MRS_FW_MSG_DIAG_CMD_MSG,
    EN_MRS_FW_MSG_PAUSE_RESUME_DETECT,
    EN_MRS_FW_MSG_POWER_FAILURE,
    EN_MRS_FW_MSG_TOPO_CHANGE_NOTIFY,
    EN_MRS_FW_MSG_TF,
    EN_MRS_FW_MSG_DETECTID,
};


typedef struct
{
    HI_U16 usCmdId;
    HI_U8 reserved[2];
    MRS_FW_CMD_REQ_PFN pfnReq;  // ��ǰ������ĺ���ָ�룬��֧��һ�����Ϊ�˼򻯴�����֧�ֶ�������֧��
} MRS_CMD_NODE_STRU;

typedef struct
{
    HI_U16 usMsgId;
    HI_U8 reserved[2];
    MRS_FW_MSG_PROC_PFN pfnMsg;
} MRS_MSG_NODE_STRU;


typedef struct
{
    // Task
    HI_U32 aulTaskId[EN_APP_TASK_MAX];

    // Queue
    HI_U32 aulQueueId[EN_APP_Q_MAX];

    // Timer
    HI_TIMER_HANDLE_S astTimerHandle[EN_APP_TIMER_MAX];

    // Semaphore
    HI_U32 aulSemId[EN_APP_SEM_MAX];
} MRS_OS_RES_CFG_STRU;


// MRS �����Ľṹ����
typedef struct
{
    HI_CHAR aucUserVersion[HI_USER_VER_LENGTH];

    HI_CONST MRS_FW_CMD_PROC_TBL_STRU* pCmdList; // Ĭ�ϵ�������б�
    HI_CONST MRS_FW_MSG_PROC_TBL_STRU* pMsgList; // Ĭ�ϵ���Ϣ�����б�
    HI_U16 usCmdNum; // ͨ������������
    HI_U16 usMsgNum; // ��Ϣ����

    MRS_CMD_NODE_STRU stCmdNode; // ��ǰ���������ڵ�
    MRS_MSG_NODE_STRU stMsgNode; // ��ǰ�������Ϣ�ڵ�

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_NDM))
    MRS_3762_PROTO_CTX_STRU st3762Proto;
#endif

    MRS_645_PROTO_CTX_STRU st645Proto[MRS_645BUF_MAX];

    HI_U32 ulSimuChlTimeout;    // ģ��ͨ�����г�ʱʱ�� ��λ:����
    HI_BOOL bSimuChlConnect;    // HI_TRUE: ���� HI_FALSE: �Ͽ�
    HI_U8 ucPlcChlStatus;
	HI_U8 ucPlcTopoStatus;  //��������״̬ 0-������ 1-�������  2-�Ż����
    HI_U8 padding;

    MRS_OS_RES_CFG_STRU stResCfg;
} MRS_FW_CTX_STRU;


// ������Ϣ�� MRS ����
HI_U32 mrsSendMessage2Queue(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg);

//��ȡͨ���˿�
HI_DMS_CHL_PORT_OBJ_S mrsGetChlPort(HI_VOID);

MRS_FW_CTX_STRU * mrsGetFwCtx(HI_VOID);


//*****************************************************************************
// ��������: mrsFwChlInit
// ��������: MRS���ͨ����ʼ��
//           ��ɸ���ͨ���ĳ�ʼ����װ���ص�����ע�Ṧ��
//           ��ʶ��ΪUART�˿ڲ���ΪII�Ͳɼ���ʱ��UARTģʽ����Ϊ485
//           ��ʶ��ΪIR�˿ں�ģʽ����ΪIR
// ����˵��:
//          pstChlPort : ͨ���ص�����ʵ��
//          usPortNum  : ͨ���˿ں�
// �� �� ֵ:
//    HI_ERR_SUCCESS : �ɹ�
//    ����ֵΪ������
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liuxipeng/lkf54842 [2014-01-15]
//*****************************************************************************
HI_U32 mrsFwChlInit(HI_CONST HI_DMS_CHL_PORT_OBJ_S * pstChlPort, HI_U16 usPortNum);


HI_VOID mrsSetPlcForceMode(HI_VOID);
HI_VOID mrsPowerOnPro(HI_VOID);

HI_VOID mrsFwGetUserVer(HI_CHAR* pucVer);
HI_VOID mrsFwSetUserVer(NV_MRS_USER_VER_S* pstVer);
#if defined(MRS_SGD_INNER_VER)
HI_U32 mrsSrvGetInnerVer(HI_U8 aucVer[8]);
#endif
HI_END_HEADER
#endif // __MRS_FW_N_H__




