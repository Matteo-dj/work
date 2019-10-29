//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_fw_proto376_2_echo.c
//  �� �� ��   : V1.0
//  ��    ��   : niesongsong/kf62735
//  ��������   : 2011-12-14
//  ��������   : TODO: ...
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2011-12-14
//    ��    �� : niesongsong/kf62735
//    �޸����� : �����ļ�
//
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_fw_proto376_2.h"
#include "mrs_fw_proto645.h"
#include "mrs_fw_proto698_45.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_log.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_parallel_cco.h"
#include "mrs_srv_common.h"
#include "mrs_srv_list.h"
#include "mrs_srv_cco_event.h"
#include "mrs_srv_cco_remote_upg.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_cco_readmeter.h"
#include "mrs_srv_anytime_readmeter.h"
#include "mrs_srv_cco_searchmeter.h"
#include "mrs_srv_cco_broadcast.h"
#include "mrs_fw_proto376_2_frame.h"
#include "mrs_time.h"
#include "mrs_fw_n.h"
#include "mrs_fw_nv.h"
#include "mrs_srv_io.h"
#include <app_nv.h>
#include "hi_mdm.h"
#include "mrs_dfx.h"
#include "mrs_srv_mrslog.h"
#include "mrs_dfx_cco_remote_upg.h"
#include "mrs_srv_readmeter_manage.h"
#include "mrs_srv_cco_queue.h"
#include "mrs_srv_cco_archives.h"
#include "mrs_dfx_cco.h"
#include "mrs_srv_cco_testmode.h"
#include <hi_equip.h>
#include <equip_dut_proc.h>
#include "mrs_srv_cco_detect_rm.h"
#include "mrs_fw_log.h"
#include "mrs_transfomer_identify.h"
#include "mrs_srv_ndm_detectid.h"
#include "mrs_fw_sgd_flash.h"
#include "mrs_srv_rf.h"

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_NDM))
#define MRS_3762_AFN_NOT_IMPLEMENT(x)   MRS_NOT_USED(x)

#define MRS_AFN03_FN_UP_DATA_LEN        (41)
#define MRS_CCO_MON_MAX_TIMEOUT_OFFSET  (6)
#define MRS_CCO_BC_MAX_TIMEOUT_OFFSET   (7)
#define MRS_3762FRAME_MAX_LENGTH_OFFSET (9)
#define MRS_MAX_PACKET_LENGTH_OFFSET    (11)
#define MRS_UPG_WAIT_TIME_OFFSET        (13)
#define MRS_CCO_ADDR_OFFSET             (14)
#define MRS_STATION_MAX_NUM_OFFSET      (20)
#define MRS_STATION_NUM_OFFSET          (22)
#define MRS_PROTO3762_RELEASE_OFFSET    (24)
#define MRS_PROTO3762_BACKUP_OFFSET     (27)
#define MRS_DEVICE_INFO_OFFSET          (30)
#define MRS_COMMUNICATION_OFFSET        (39)

#define MRS_AFN03F10_RELEASE_DATE       "\x18\x09\x13"
#define MRS_AFN03F10_BACKUP_DATE        "\x18\x09\x13"

#define MRS_SECONDARY_NODE_ITEM_SIZE_OLD    (9)
#define MRS_SECONDARY_NODE_ITEM_SIZE_NEW    (7)

#define MRS_BC_DELAY_TIME                   (1)     // 1s
#define MRS_AFN14_F3_DELAY_TIME             (1)     // 1s
#define MRS_AFN03_F10_BC_TIMEOUT            (10)    // 10s
#define MRS_UPG_WAIT_TIME                   (60)    // 10s
#define MRS_MAX_FRAME_LEN                   (1024)  // ���֧�ֵı��ĳ���

typedef struct
{
    HI_U32 afn_data;
    MRS_3762_AFN_FUN func;
} MRS_AFN_FUNC_MAP;

typedef struct
{
    HI_U8 afn;
    HI_U8 capability[31];
} MRS_AFN_MAP;

// ���������Ԫ��ʱ���ϸ����afn_data��������
// �ڲ���ʱ���ն��ַ����ң����������󣬲��һ����Ӱ���������
static MRS_AFN_FUNC_MAP AFN_FUNC_MAP_ARRAY[] =
{
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    {0x00000000, mrs3762_AFN00_F1},
    {0x00000001, mrs3762_AFN00_F2},

    {0x00000100, mrs3762_AFN01_F1},
    {0x00000101, mrs3762_AFN01_F2},
    {0x00000102, mrs3762_AFN01_F3},

    {0x00000200, mrs3762_AFN02_F1},
	{0x00000201, mrs3762_AFN02_F1},
	
    {0x00000300, mrs3762_AFN03_F1},
    {0x00000301, mrs3762_AFN03_F2},
    {0x00000302, mrs3762_AFN03_F3},
    {0x00000303, mrs3762_AFN03_F4},
    {0x00000304, mrs3762_AFN03_F5},
    {0x00000305, mrs3762_AFN03_F6},
    {0x00000306, mrs3762_AFN03_F7},
    {0x00000308, mrs3762_AFN03_F9},
    {0x00000309, mrs3762_AFN03_F10},
    {0x0000030A, mrs3762_AFN03_F11},
    {0x0000030F, mrs3762_AFN03_F16},

    {0x00000400, mrs3762_AFN04_F1},
    {0x00000401, mrs3762_AFN04_F2},
    {0x00000402, mrs3762_AFN04_F3},

	{0x00000405, mrs3762_AFN04_F4},
    {0x00000406, mrs3762_AFN04_F5},

    {0x00000500, mrs3762_AFN05_F1},
    {0x00000501, mrs3762_AFN05_F2},
    {0x00000502, mrs3762_AFN05_F3},
    {0x00000503, mrs3762_AFN05_F4},
 	{0x00000504, mrs3762_AFN05_F5},
    {0x00000505, mrs3762_AFN05_F6},
    {0x0000050F, mrs3762_AFN05_F16},
	{0x00000564, mrs3762_AFN05_F101},
    {0x00000581, mrs3762_AFN05_F130},

    {0x00001000, mrs3762_AFN10_F1},
    {0x00001001, mrs3762_AFN10_F2},
    {0x00001002, mrs3762_AFN10_F3},
    {0x00001003, mrs3762_AFN10_F4},
    {0x00001004, mrs3762_AFN10_F5},
    {0x00001005, mrs3762_AFN10_F6},
    {0x00001014, mrs3762_AFN10_F21},
    {0x0000101E, mrs3762_AFN10_F31},
    {0x0000101F, mrs3762_AFN10_F32},
    {0x00001020, mrs3762_AFN10_F33},
#endif
    {0x00001027, mrs3762_AFN10_F40},
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    {0x00001063, mrs3762_AFN10_F100},
    {0x00001064, mrs3762_AFN10_F101},
    {0x00001067, mrs3762_afn10_f104},
    {0x0000106E, mrs3762_AFN10_F111},
    {0x0000106F, mrs3762_AFN10_F112},
    {0x000010E3, mrs3762_AFN10_F228},

    {0x00001100, mrs3762_AFN11_F1},
    {0x00001101, mrs3762_AFN11_F2},
    {0x00001102, mrs3762_AFN11_F3},
    {0x00001103, mrs3762_AFN11_F4},
    {0x00001104, mrs3762_AFN11_F5},
    {0x00001105, mrs3762_AFN11_F6},
    {0x00001164, mrs3762_AFN11_F101},

    {0x00001200, mrs3762_AFN12_F1},
    {0x00001201, mrs3762_AFN12_F2},
    {0x00001202, mrs3762_AFN12_F3},

    {0x00001300, mrs3762_AFN13_F1},

    {0x00001400, mrs3762_AFN14_F1},
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    {0x00001401, mrs3762_AFN14_F2},
#endif
    {0x00001402, mrs3762_AFN14_F3},

    {0x00001500, mrs3762_AFN15_F1},
    
    {0x00002000, mrs3762_AFN20_F1},
    {0x00002001, mrs3762_AFN20_F2},

    {0x0000F000, mrs3762_AFNF0_F1},

    {0x0000F001, mrs3762_AFNF0_F2},

#if defined(MRS_SGD_INNER_VER)
    {0x0000F0E4, mrs3762_AFNF0_F229},
    {0x0000F0E5, mrs3762_AFNF0_F230},
	{0x0000F0E6, mrs3762_AFNF0_F231},
#endif	
    {0x0000F100, mrs3762_AFNF1_F1},
#endif
};

MRS_AFN_MAP MRS_AFN_CAPABILITY[] =
{
    {0x00, {0x00}},
    {0x01, {0x00}},
    {0x02, {0x00}},
    {0x03, {0x00}},
    {0x04, {0x00}},
    {0x05, {0x00}},
    {0x06, {0x00}},
    {0x10, {0x00}},
    {0x11, {0x00}},
    {0x12, {0x00}},
    {0x13, {0x00}},
    {0x14, {0x00}},
    {0x15, {0x00}},
    {0xF1, {0x00}}
};


/* �ɹ�Լ: 376.2������(AFN=11-F1)�ӽڵ����ݵ�Ԫ(�ֽ���):���ַ(6�ֽ�)+�ӽڵ����(2�ֽ�)+��Լ����(1�ֽ�) */
/* �¹�Լ: 376.2������(AFN=11-F1)�ӽڵ����ݵ�Ԫ(�ֽ���):���ַ(6�ֽ�)+��Լ����(1�ֽ�) */
#define GetSecondaryNodeItemSize(ctx)   (((ctx)->ucProtoVer == PROTO_VER_3762_2009) ? MRS_SECONDARY_NODE_ITEM_SIZE_OLD : MRS_SECONDARY_NODE_ITEM_SIZE_NEW)

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
//��ǰ�Ƕ�̬���䣬���ڸ��ɾ�̬����
HI_PRV HI_U8 mrs3762AFN10F2Buffer[3+sizeof(AFN_10_F2_NODE_INFO)*MRS_3762_F10_NODE_MAX_NUM] = {0};
HI_PRV HI_U32 mrsCcoAddrWriteNv(HI_VOID);
HI_PRV HI_U16 mrsGetCurrTopoNum(HI_VOID);
HI_PRV HI_U32 mrsCcoQueueRx(HI_VOID);
#endif
HI_PRV HI_U32 CCO_APP_MsgRcv(HI_U32 ulMdmMsgId, HI_PVOID pMdmMsg, HI_PVOID pData, HI_U32 ulOption);

HI_PRV HI_U32 g_TestReqTimesCnt = 0;
PROTO376_2_FRAME_PRE_STRU *g_read_id_frame = HI_NULL;
HI_PRV HI_CONST HI_MDM_APPOBJ_S g_CCO_stMdmAppObj =  
{
    HI_MDM_OBJ_ID_USR1,         //APP ����ID 
    CCO_APP_MsgRcv  //APP ����MDM ��Ϣ�Ĵ������  
};

HI_U32 mrsAppObjInit(HI_VOID) 
{ 
    //APP �����ʼ�� 
    HI_MDM_AppInit(&g_CCO_stMdmAppObj); 
	return HI_ERR_SUCCESS;
}

HI_PRV HI_U32 CCO_APP_MsgRcv(HI_U32 ulMdmMsgId, HI_PVOID pMdmMsg, HI_PVOID pData, HI_U32 ulOption)
{
	HI_MDM_MSG_APPRCV_S* pstMsgRcv = (HI_MDM_MSG_APPRCV_S*)pMdmMsg;
	g_TestReqTimesCnt++;
	if(ulMdmMsgId == HI_MDM_NM_SNID_SET_ID)
	{
		HI_MDM_NM_SNID_SET_IND_S *pstInd = (HI_MDM_NM_SNID_SET_IND_S *)pstMsgRcv->pucPacket;
        if(0== pstInd->usRc)
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_060, HI_DIAG_MT("SNID�Ѿ����ڣ�����Ҫ��������"),pstInd->usRc);
        }
		else
		{
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_061, HI_DIAG_MT("SNID �Ѿ����óɹ�"),pstInd->usRc);
		}
	}
	return HI_ERR_SUCCESS;
}
HI_U32 mrsSetSnid(HI_U32 ulSnidSn)
{
	HI_MDM_MSG_CTRL_S stCtrl;
	HI_MDM_NM_SNID_SET_REQ_S stReq;
	//��stCtrl.aucMacAddr��ֵ,����Ŀ��MAC��ַ,���ز�ѯʱֱ������Ϊȫ0
	//HI_MDM_Memzero(&stCtrl, sizeof(HI_MDM_MSG_CTRL_S));
	(hi_void)memset_s(&stCtrl, sizeof(HI_MDM_MSG_CTRL_S),0,sizeof(HI_MDM_MSG_CTRL_S));
	stCtrl.ulSn = g_TestReqTimesCnt;// g_TestReqTimesCntΪȫ�ֱ�����ÿ�η��͸ñ���Ӧ�ü�1�������ֲ�ͬ�εĲ�ѯ
	stCtrl.usDULink = HI_FALSE;
	stCtrl.usTransId = HI_MDM_OBJ_ID_USR1;
	//HI_MDM_Memzero(&stReq, sizeof(HI_MDM_NM_SNID_SET_REQ_S));
	(hi_void)memset_s(&stReq, sizeof(HI_MDM_NM_SNID_SET_REQ_S),0,sizeof(HI_MDM_NM_SNID_SET_REQ_S));
	stReq.ulSnid = ulSnidSn;
	//����ָ����ͨ��ģ���SNID ,��Ҫ����HI_MDM_MsgSend()�ж�MAC��ַ�Ķ��壬��������stCtrl.aucMacAddr
	
	//���ñ���ģ���SNID
	HI_MDM_MsgSend(HI_MDM_NM_SNID_SET_ID,(HI_PBYTE)&stReq,sizeof(HI_MDM_NM_SNID_SET_REQ_S), &stCtrl);
	return HI_ERR_SUCCESS;
}
//*****************************************************************************
// ��������: mrsInitSustainAfn
// ��������: ��ʼ��֧�ֵ�AFN-FN������
//
// ����˵��:
//
// �� �� ֵ:
//
// ����Ҫ��:
// ���þ���:
// ��    ��: fengxiaomin/00209182 [2014-1-10]
//*****************************************************************************
HI_VOID mrsInitSustainAfn(HI_VOID)
{
    HI_U8 ucIndex = 0;
    HI_U8 ucAfnNum = sizeof(MRS_AFN_CAPABILITY)/sizeof(MRS_AFN_MAP);
    HI_U8 ucOffSet = 0;

    for (ucIndex = 0; ucIndex < ucAfnNum; ucIndex++)
    {
        (hi_void)memset_s(MRS_AFN_CAPABILITY[ucIndex].capability,
            sizeof(MRS_AFN_CAPABILITY[ucIndex].capability),0, sizeof(MRS_AFN_CAPABILITY[ucIndex].capability));
    }

    MRS_AFN_CAPABILITY[ucOffSet].capability[0] = 0x03;  //0x00  1,2    0000 0011
    ucOffSet++;
    MRS_AFN_CAPABILITY[ucOffSet].capability[0] = 0x07;  //0x01  1,2,3    0000 0111
    ucOffSet++;
    MRS_AFN_CAPABILITY[ucOffSet].capability[0] = 0x01;  //0x02  1       0000 0001
    ucOffSet++;
    MRS_AFN_CAPABILITY[ucOffSet].capability[0] = 0x7F;  //0x03  1,2,3,4,5,6,7     0111 1111
    MRS_AFN_CAPABILITY[ucOffSet].capability[1] = 0x87;  //0x03  9,10,11,16        1000 0111
    ucOffSet++;
    MRS_AFN_CAPABILITY[ucOffSet].capability[0] = 0x07;  //0x04  1,2,3        0000 0111
    ucOffSet++;
    MRS_AFN_CAPABILITY[ucOffSet].capability[0] = 0x0F;  //0x05  1,2,3,4        0000 1111
    MRS_AFN_CAPABILITY[ucOffSet].capability[1] = 0x80;  //0x05  16             1000 0000
    MRS_AFN_CAPABILITY[ucOffSet].capability[16] = 0x02; //0x05  130            0000 0010
    ucOffSet++;
    MRS_AFN_CAPABILITY[ucOffSet].capability[0] = 0x1E;  //0x06  2,3,4,5        0001 1110
    MRS_AFN_CAPABILITY[ucOffSet].capability[1] = 0x00;  //0x06  10             0000 0000
    ucOffSet++;
    MRS_AFN_CAPABILITY[ucOffSet].capability[0] = 0x3F;  //0x10  1,2,3,4,5,6      0011 1111
    MRS_AFN_CAPABILITY[ucOffSet].capability[2] = 0x10;  //0x10  21               0001 0000 (24~17)
    MRS_AFN_CAPABILITY[ucOffSet].capability[3] = 0xC0;  //0x10  32,31            1100 0000 (32~25)
    MRS_AFN_CAPABILITY[ucOffSet].capability[4] = 0x01;  //0x10  33               0000 0001 (33)
    MRS_AFN_CAPABILITY[ucOffSet].capability[12] = 0x98; //0x10  104,101,100      1001 1000 (104~97)
    MRS_AFN_CAPABILITY[ucOffSet].capability[13] = 0xC0; //0x10  111,112      1100 0000 (105~112)
    MRS_AFN_CAPABILITY[ucOffSet].capability[28] = 0x08; //0x10  228      0000 1000 (232~225)
    ucOffSet++;
    MRS_AFN_CAPABILITY[ucOffSet].capability[0] = 0x3F;  //0x11  1,2,3,4,5,6      0011 1111
    MRS_AFN_CAPABILITY[ucOffSet].capability[12] = 0x00; //0x11  101      0000 0000
    ucOffSet++;
    MRS_AFN_CAPABILITY[ucOffSet].capability[0] = 0x07;  //0x12  1,2,3            0000 0111
    ucOffSet++;
    MRS_AFN_CAPABILITY[ucOffSet].capability[0] = 0x01; //0x13  1            0000 0001
    ucOffSet++;
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    MRS_AFN_CAPABILITY[ucOffSet].capability[0] = 0x07; //0x14  1,2,3            0000 0111
#else
    MRS_AFN_CAPABILITY[ucOffSet].capability[0] = 0x05; //0x14  1,3            0000 0101
#endif
    ucOffSet++;
    MRS_AFN_CAPABILITY[ucOffSet].capability[0] = 0x01; //0x15  1            0000 0001
    ucOffSet++;
    MRS_AFN_CAPABILITY[ucOffSet].capability[0] = 0x01; //0xF1  1            0000 0001

    return;
}

//*****************************************************************************
// ��������: mrs3762ProtoAckFrame
// ��������: ����ȷ��֡
//
// ����˵��:
//   AFN_00_F1 * pstAckFrame [in/out] �Զ�����Ӧͷ���������ֵ�ṹ��һ��Ĭ�ϵ�
//                                    ȫ����Ӧͷ,�ܶ�����ǿ���ֱ�Ӵ���ֵ�ġ�
//
//   HI_PBYTE * ppucBuffer [in/out] ���3762֡�Ļ�����ʼ��ַ��ָ��
//
//   HI_U16 * pusBufferLen [in/out] ��¼����
//
//
// �� �� ֵ:
//
//
// ����Ҫ��:
// ���þ���:
// ��    ��: niesongsong/kf62735 [2011-12-13]
//*****************************************************************************
HI_U32 mrs3762ProtoAckFrame(HI_IN HI_U16 waitTime,
                           HI_IN AFN_FN_UL_PARAM * pstDlFrameInf,
                           HI_OUT HI_PBYTE * ppucBuffer,
                           HI_OUT HI_U16   * pusBufferLen)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM stAfnParam = {0};
    AFN_00_F1_NEW ack_new = {0};
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    AFN_00_F1  stDefaultAck = {0};
    MRS_CCO_SRV_CTX_STRU * cco_ctx = mrsCcoGetContext();
#endif
    HI_U16 data_len = 0;
    HI_U8 *buf = HI_NULL;

    if ((ppucBuffer == HI_NULL) || (pusBufferLen == HI_NULL))
    {
        return HI_ERR_BAD_DATA;
    }

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    if(cco_ctx->ucProtoVer != PROTO_VER_3762_2009)
#endif
    {
        buf = (HI_U8 *)&ack_new;
        data_len = sizeof(ack_new);
        ack_new.uSWitTime = waitTime;
    }
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    else
    {
        buf = (HI_U8 *)&stDefaultAck;
        data_len = sizeof(stDefaultAck);
        stDefaultAck.uSWitTime = waitTime;
    }
#endif

    stAfnParam.ppucOutBuffer = ppucBuffer;
    stAfnParam.pusBufferLen = pusBufferLen;
    stAfnParam.ucAfn = 0;
    stAfnParam.ucFnIndex = 0;
    stAfnParam.pstFrame = pstDlFrameInf->pstFrame;

    ret = mrs3762_EasyEnc(buf, data_len,&stAfnParam);

    return ret;
}

//*****************************************************************************
// ��������: mrs3762ProtoDenyFrame
// ��������: TODO: ...
//
//   AFN_00_F1 * pstDenyFrame [in/out] �Զ�����Ӧͷ���������ֵ�ṹ��һ��Ĭ�ϵ�
//                                    ȫ����Ӧͷ,�ܶ�����ǿ���ֱ�Ӵ���ֵ�ġ�
//
//   HI_PBYTE * ppucBuffer [in/out] ���3762֡�Ļ�����ʼ��ַ��ָ��
//
//   HI_U16 * pusBufferLen [in/out] ��¼����
//
// �� �� ֵ:
//    TODO: ...
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: niesongsong/kf62735 [2011-12-13]
//*****************************************************************************
HI_U32 mrs3762ProtoDenyFrame(HI_IN AFN_00_F2 * pstDenyFrame,
                           HI_IN AFN_FN_UL_PARAM * pstDlFrameInf,
                           HI_OUT HI_PBYTE * ppucBuffer,
                           HI_OUT HI_U16   * pusBufferLen)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM stAfnParam = {0};
    AFN_00_F2  stDefaultAck = {0};
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    MRS_CCO_SRV_CTX_STRU * pstCtx = mrsCcoGetContext();
#endif

    if ((ppucBuffer == HI_NULL) || (pusBufferLen == HI_NULL) || (!pstDlFrameInf))
    {
        return HI_ERR_BAD_DATA;
    }

    if (pstDenyFrame == HI_NULL)
    {
        pstDenyFrame = &stDefaultAck;
    }

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    if (pstCtx->ucProtoVer == PROTO_VER_3762_2009)
    {
        switch (*pstDenyFrame)
        {
            case MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME:
            case MRS_13762_DENY_STATUS_INVALID_DATA:
            case MRS_13762_DENY_STATUS_ERROR_LENGTH:
            case MRS_13762_DENY_STATUS_ERROR_CHECK_SUM:
            case MRS_13762_DENY_STATUS_INEXISTENT_INFO:
            case MRS_13762_DENY_STATUS_ERROR_FORMAT:
            case MRS_13762_DENY_STATUS_REPETITION_METER:
            case MRS_13762_DENY_STATUS_INEXISTENT_METER:
                break;

            case MRS_13762_DENY_RM_BUFPOOL_FULL:
                *pstDenyFrame = MRS_3762_DENY_RM_BUFPOOL_FULL;
                break;

            case MRS_13762_DENY_RM_645ID_OVERRUN:
                *pstDenyFrame = MRS_3762_DENY_RM_645ID_OVERRUN;
                break;

            case MRS_13762_DENY_RM_METER_BUSY:
                *pstDenyFrame = MRS_3762_DENY_RM_METER_BUSY;
                break;

            case MRS_13762_DENY_STATUS_CCO_BUSY:
                *pstDenyFrame = MRS_3762_DENY_CCO_BUSY;
                break;

            default:
                *pstDenyFrame = MRS_3762_DENY_STATUS_COMMUNICATION_OVERTIME;
                break;
        }
    }
#endif

    stAfnParam.ppucOutBuffer = ppucBuffer;
    stAfnParam.pusBufferLen = pusBufferLen;
    stAfnParam.ucAfn = 0;
    stAfnParam.ucFnIndex = 1;
    stAfnParam.pstFrame = pstDlFrameInf->pstFrame;

    ret = mrs3762_EasyEnc((HI_U8 *)pstDenyFrame,sizeof(AFN_00_F2),&stAfnParam);

    return ret;
}

//*****************************************************************************
// ��������: mrs3762AfnDispatch
// ��������: ���б���ͳһ������
//
// ����˵��:
//   MRS_3762_FRAME_STRU * pstFrame [in/out] ֡�ṹ
//
//   HI_PBYTE * ppOutBuffer [in/out] ������棨����еĻ���
//
//   HI_U16 * pusBufferLen [in/out] ������泤�ȣ�����еĻ���
//
//	HI_BOOL is_ndm_plc ������ndm��plc֡ʱΪHI_TRUE,����ΪHI_FALSE
//
// �� �� ֵ:
//    HI_ERR_SUCCESS ��ʾ�ɹ�������Ϊ������
//
// ����Ҫ��:
// ���þ���:
// ��    ��: niesongsong/nKF62735 [2011-12-17]
//*****************************************************************************
HI_U32 mrs3762AfnDispatch(HI_IN  MRS_3762_FRAME_STRU * pstFrame, HI_OUT HI_PBYTE * ppOutBuffer,
                         HI_OUT HI_U16 * pusBufferLen, HI_BOOL ucNdmPlc)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 ucAfn = 0;
    HI_U8  *pucContent = HI_NULL;
    HI_U16 usContentLen = 0;

    if ((pstFrame == HI_NULL) || (ppOutBuffer == HI_NULL) || (pusBufferLen == HI_NULL))
    {
        return HI_ERR_BAD_DATA;
    }

    *pusBufferLen = 0;
    *ppOutBuffer = 0;

    ucAfn = pstFrame->strUsrDataHrd.stAppDataRealm.ucAfn;
    pucContent = pstFrame->strUsrDataHrd.stAppDataRealm.pucAppDataContent;
    usContentLen = pstFrame->strUsrDataHrd.stAppDataRealm.usAppDataLen;

    if (usContentLen >= 2)
    {
        //��ȡFN
        MRS_3762_AFN_FUN pAfnFnProc = 0;
        HI_U8 ucFnIndex = mrs3762GetDtFnIndex(pucContent);

        pAfnFnProc = mrs3762FindAfnProc(ucAfn,ucFnIndex);

        if (pAfnFnProc != 0)
        {
            //��֯����
            AFN_FN_UL_PARAM param = {0};
            param.pstFrame = pstFrame;
            param.pContent = pstFrame->strUsrDataHrd.stAppDataRealm.pucAppDataContent;
            param.usContentLen = pstFrame->strUsrDataHrd.stAppDataRealm.usAppDataLen;
            param.ppucOutBuffer = ppOutBuffer;
            param.pusBufferLen = pusBufferLen;
            param.ucAfn = ucAfn;
            param.ucFnIndex = ucFnIndex;  //FN������ֵ
			param.ucNdmPlc = ucNdmPlc;

            //����
            ret = pAfnFnProc(&param);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
            if (MRS_CCO_PROTOCOL_AFN_13 == param.ucAfn && ret != HI_ERR_BUSY)
            {
                mrsRemoteUpgRxFilePro();
            }
#endif
        }
    }
    else
    {
        ret = HI_ERR_BAD_DATA;
    }

    return ret;
}

//*****************************************************************************
// ��������: mrs3762FindAfnProc
// ��������: ����AFN��FN�ҵ���Ӧ��FN������
//
// ����˵��:
//   afn [in] AFN��Ӧ�ó�������
//
//   fnIndex [in] fn����
//
//
// �� �� ֵ:
//    �ɹ����ظô���ĺ���ָ��
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: niesongsong/kf62735 [2011-12-23]
// ��    ��: cuiate/00233580     [2013-04-22]
//*****************************************************************************
MRS_3762_AFN_FUN mrs3762FindAfnProc(HI_U8 afn,HI_U8 fnIndex)
{
    HI_U32 ulProcIndex = (HI_U16)((afn << 8) + fnIndex);
    int low, high, mid;

    low = 0;
    if (ulProcIndex == AFN_FUNC_MAP_ARRAY[low].afn_data)
    {
        return AFN_FUNC_MAP_ARRAY[low].func;
    }
    
/*lint -e778*/
    high = sizeof(AFN_FUNC_MAP_ARRAY)/sizeof(AFN_FUNC_MAP_ARRAY[0]) - 1;
/*lint +e778*/
    if (ulProcIndex == AFN_FUNC_MAP_ARRAY[high].afn_data)
    {
        return AFN_FUNC_MAP_ARRAY[high].func;
    }

    while (low <= high)
    {
        mid = low + (high - low)/2;
        if (ulProcIndex == AFN_FUNC_MAP_ARRAY[mid].afn_data)
        {
            return AFN_FUNC_MAP_ARRAY[mid].func;
        }

        if (AFN_FUNC_MAP_ARRAY[mid].afn_data > ulProcIndex)
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }

    return mrs3762_AFNXX_XX;
}

//*****************************************************************************
// ��������: mrs3762GetDtFnIndex
// ��������: ����DT��õ�FN������ֵ
//
// ����˵��:
//   dt[2] [in] DT��
//
// �� �� ֵ:
//    0xFFΪ��Чֵ
//
// ����Ҫ��:
// ���þ���:
// ��    ��: niesongsong/nKF62735 [2011-12-17]
//*****************************************************************************
HI_U8 mrs3762GetDtFnIndex(HI_U8 dt[2])
{
    HI_U8 msk[8] = {1,2,4,8,16,32,64,128};
    HI_U8 pos = 0;

    if (dt[1] > 30)
    {
        return 0xFF;
    }

    for (pos = 0;pos < sizeof(msk);pos++)
    {
        if (dt[0] == msk[pos])
        {
            break;
        }
    }

    if (pos == sizeof(msk))
    {
        return 0xFF;
    }
    return (HI_U8)((dt[1] * 8) + pos);
}

//*****************************************************************************
// ��������: mrs3762_EasyEnc
// ��������: һ�����׵ĳ�ª����֡����
//
// ����˵��:
//   pucContent [in/out] ���ݵ�Ԫ������
//
//   usLen [in] ���ݵ�Ԫ����
//
//   param [in/out] ���д������Ĳ���
//
//
// �� �� ֵ:
//
// ����Ҫ��:
// ���þ���:
// ��    ��: niesongsong/nKF62735 [2011-12-17]
// ��    ��: cuiate/00233580      [2013-03-12]
//*****************************************************************************
HI_U32 mrs3762_EasyEnc(HI_U8 * pucContent,HI_U16 usLen,HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = param;
    PROTO376_2_FRAME_PRE_STRU stNewFrame;

    if ((pstAfnParam == HI_NULL) || (pucContent == HI_NULL))
    {
        return HI_ERR_BAD_DATA;
    }

    (hi_void)memset_s(&stNewFrame, sizeof(stNewFrame), 0, sizeof(stNewFrame));
    stNewFrame.ucSeq = pstAfnParam->pstFrame->strUsrDataHrd.uInfoRealm.stInfRealmDL.ucSeq;
    stNewFrame.stAfnData.ucAFN = pstAfnParam->ucAfn;
    stNewFrame.stAfnData.ucDT[0] = (1 << (pstAfnParam->ucFnIndex % 8));
    stNewFrame.stAfnData.ucDT[1] = (pstAfnParam->ucFnIndex / 8);
    stNewFrame.stAfnData.pData = pucContent;
    stNewFrame.stAfnData.usDataLen = usLen;

    ret = mrsCreate3762Frame(&stNewFrame, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen, HI_NULL);

    return ret;
}

//*****************************************************************************
// ��������: mrs3762_AFNXX_XX
// ��������: û��ʵ�ֻ�֧�ֵ�FN������
//
// ����˵��:
//
//
// �� �� ֵ:
//    HI_ERR_SUCCESS
//
// ����Ҫ��: ��֧�ֵ�FN������δ֧�ֵ�FN����
// ���þ���:
// ��    ��: niesongsong/nKF62735 [2011-12-17]
//*****************************************************************************
HI_U32 mrs3762_AFNXX_XX(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_INEXISTENT_INFO;//����֡�еĴ���״̬��
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;

    mrs3762ProtoDenyFrame(&stDenyData,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);

    return ret;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_U32 mrsCcoQueueRx(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    P_MRS_SRV_CCO_QUEUE pstCcoQueue = mrsGetCcoQueue();
    MRS_SRV_CCO_ITEM *pstCcoItem = HI_NULL;
    MRS_SRV_CCO_ITEM *pstCcoItemTemp = HI_NULL;

    MRS_StopTimer(MRS_CCO_TIMER_MASTER_REPORT);

    if (MRS_CCO_QUEUE_WAIT_ACK_STATUS != mrsCcoGetQueueStatus())
    {
        return ret;
    }

    pstCcoItem = mrsSrvQueueTop(&pstCcoQueue->stCcoQueue);
    if (!pstCcoItem)
    {
        return HI_ERR_FAILURE;
    }

    if (pstCcoItem->MrsCcoRxRespProc)
    {
        pstCcoItem->MrsCcoRxRespProc(pstCcoItem->pParam);
    }

    pstCcoItemTemp = mrsSrvQueueTop(&pstCcoQueue->stCcoQueue);
    if (pstCcoItem == pstCcoItemTemp)
    {
        mrsCcoQueueEnd(pstCcoQueue);
    }

    return ret;
}

//AFN00 F1:ȷ��
HI_U32 mrs3762_AFN00_F1(HI_INOUT HI_VOID * param)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

    MRS_NOT_USED(param);

    ulRet = mrsCcoQueueRx();
    if (HI_ERR_SUCCESS == ulRet)
    {
        mrsDfxCcoQueueRxAckCnt();
    }

    return ulRet;
}

//AFN00 F2:����
HI_U32 mrs3762_AFN00_F2(HI_INOUT HI_VOID * param)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

    MRS_NOT_USED(param);

    ulRet = mrsCcoQueueRx();
    if (HI_ERR_SUCCESS == ulRet)
    {
        mrsDfxCcoQueueRxDenyCnt();
    }

    return ulRet;
}

//AFN01 F1:Ӳ����ʼ��
HI_U32 mrs3762_AFN01_F1(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    static HI_U8 first = 0;
/* BEGIN: Added by fengxiaomin/00209182, 2014/3/4   ���ⵥ��:DTS2014022806293 */
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//����֡�еĴ���״̬��

/* BEGIN: Added by fengxiaomin/00209182, 2014/1/10   ���ⵥ�ź�:DTS2014012607684 */
    if (!mrsIsAllowCcoReset())
    {
        first = 0;
        stDenyData = MRS_13762_DENY_STATUS_CCO_BUSY;
        ret = mrs3762ProtoDenyFrame(&stDenyData, pstAfnParam, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
        return ret;
    }
/* END:   Added by fengxiaomin/00209182, 2014/1/10 */

    if (first)
    {
        ret = mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
        return ret;
    }

    first++;

    //��֯ȷ��֡
    ret = mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);

    if (ret == HI_ERR_SUCCESS)
    {
        MRS_StartTimer(EN_MRS_SRV_CCO_SYSTEM_RESET_TIMER, MRS_SRV_CCO_SYSTEM_RESET_TIMER_VALUE, HI_SYS_TIMER_ONESHOT);
    }
    else
    {
        first = 0;
    }
/* END:   Added by fengxiaomin/00209182, 2014/3/4 */

    return ret;
}

//AFN01 F2:��������ʼ��
HI_U32 mrs3762_AFN01_F2(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;

    mrsSrvArchivesClear();

    //��֯ȷ��֡
    mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);

    (hi_void)memset_s(&pstCcoCtx->stReadMeterModel.stCurrNode, sizeof(MRS_ARCHIVES_NODE_STRU), 0, sizeof(MRS_ARCHIVES_NODE_STRU));
    pstCcoCtx->stReadMeterModel.usCurrNodeSeq = 0;  //(��Ϊ����)
    pstCcoCtx->stReadMeterModel.usSuccNum = 0;

    mrsCcoReadFailReset();

    MRS_StartTimer(EN_MRS_SRV_CCO_WRITE_FLASH_ACTIV_TIMER,
                    pstCcoCtx->ucFlashSaveDelay *MS_PER_SECOND,
                    HI_SYS_TIMER_ONESHOT);
    mrsCcoSmWhitelistSwitchTimerReset();

    if (pstCcoCtx->status & MRS_CCO_STATUS_NORMAL)
    {
        // ��ʱMRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUEʱ������¼������ģ��
        MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER,
                            MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE * 3,
                            HI_SYS_TIMER_ONESHOT);
    }

    mrsCcoDetectRmReset();
    mrsDfxCcoArchivesClearTime(HI_MDM_GetMilliSeconds());
    mrsDfxLrMeterNum(0);

    return ret;
}

//AFN01 F3:��������ʼ��
HI_U32 mrs3762_AFN01_F3(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    MRS_CCO_SRV_CTX_STRU * cco_ctx = mrsCcoGetContext();

    // ��������ʼ������
    cco_ctx->stReadMeterModel.usSuccNum = 0;
    if (cco_ctx->stAutoSearchModel.pReportedNodeInf)
    {
        (hi_void)memset_s(cco_ctx->stAutoSearchModel.pReportedNodeInf,
            sizeof(MR_NODE_METER_ADD_STRU) * PRODUCT_CFG_MRS_MAX_NODE_NUM,0,
            sizeof(MR_NODE_METER_ADD_STRU) * PRODUCT_CFG_MRS_MAX_NODE_NUM);
    }

    mrsCcoReadFailReset();
    //��֯ȷ��֡
    mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);

    return ret;
}


// AFN02 F1: ת��ͨѶЭ������֡
HI_U32 mrs3762_AFN02_F1(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    HI_U8 * pucContent = pstAfnParam->pContent;
    HI_U16 usContentLen = pstAfnParam->usContentLen;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;  // ����֡�еĴ���״̬��

    mrsDfxXrUartRx();
    mrsRemoteUpgRxFilePro();
    mrsCcoDfxRmUartRx(pucContent, usContentLen, HI_FALSE);

    do
    {
        HI_U8 ucProtocolType = 0;
        HI_U8 ucDataLen = 0;
        HI_U8 ucPos = 0;
        HI_U8 *pucData = HI_NULL;
        MRS_3762_USER_DATA_HRD_STRU * pstUsrDataHrd = HI_NULL;
        HI_U8 ucMinLen = 4; // Fn(2) + ͨ��Э������(1) + ���ĳ���(1)

        // ��ȡ��ַ��
        pstUsrDataHrd = &pstAfnParam->pstFrame->strUsrDataHrd;

        // ��ȡ֡���
        pstCcoCtx->stAnytimeRMModel.seq_1376_2 = pstUsrDataHrd->uInfoRealm.stInfRealmDL.ucSeq;

        // ֻ����PLC����ռ��״̬�²Ž�������
        if(!mrsIsDetectRm() && (pstCcoCtx->stAnytimeRMModel.enState == EN_MRS_ANYTIME_RM_WAIT_PLC))
        {
            ret = HI_ERR_BUSY;  // ��ǰ����ִ���泭����
            stDenyData = MRS_13762_DENY_STATUS_CCO_BUSY;
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_023, HI_DIAG_MT("XR PLC BUSY"),pstCcoCtx->stAnytimeRMModel.enState);
            break;
        }

        // Fn(2) + ͨ��Э������(1) + ���ĳ���(1)
        if (usContentLen < ucMinLen)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_024, HI_DIAG_MT("Len Too Short"));
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }

        // ƫ��Fn(2Byte)
        ucPos += 2;

        // ��ȡ��Լ����
        ucProtocolType = pucContent[ucPos++];

        // ����Լ�����Ƿ���Ч
        ret = mrsCheckProtocolValid(ucProtocolType);
        if (ret != HI_ERR_SUCCESS)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_025, HI_DIAG_MT("Protocol Err!"));
            stDenyData = MRS_13762_DENY_STATUS_ERROR_FORMAT;
            ret = HI_ERR_BAD_DATA;
            break;
        }

        // ��ȡ���ĳ���
        ucDataLen = pucContent[ucPos++];

        // ʣ�೤�ȼ��
        if((HI_S16)(usContentLen - ucPos) < ucDataLen)
        {
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_027, HI_DIAG_MT("Farmer Len Too Short"));
            break;
        }

        // ��ȡת������
        pucData = &pucContent[ucPos];

        // ��ȡ��Լ����
        pstCcoCtx->stAnytimeRMModel.ucProtocol = ucProtocolType;

        // �����ԴΪ "ת��ͨѶЭ������֡"
        pstCcoCtx->stAnytimeRMModel.from = XR_FROM_AFN02_F1;

        // ��ʼ���泭����ģ��
        mrsAnyTimeReadMeterInit();

        mrsDetectRmBreakProc();

        // �����泭����ģ����ж���
        ret = mrsAnyTimeReadMeterProcess(pstUsrDataHrd, ucProtocolType, pucData, ucDataLen);
        if (HI_ERR_SUCCESS != ret)
        {
            stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;
			break;
        }
		if(pucContent[0]==0x02)
		{
			pstCcoCtx->stAnytimeRMModel.ucFn=2;
		}
    } while (0);

    if(ret != HI_ERR_SUCCESS)
    {
    	//ndm��plc֡�㳭��ɵ�æ���ش���
    	if((ret == HI_ERR_BUSY) && (mrsCcoNdmPlcBusyAdd(pstAfnParam, XR_FROM_AFN02_F1) == HI_ERR_BUSY))
    	{
    		return ret;
    	}

        HI_U8 ucFrom = pstCcoCtx->stAnytimeRMModel.from;

        // �����ԴΪ "����ز��ӽڵ�"
        pstCcoCtx->stAnytimeRMModel.from = XR_FROM_AFN02_F1;
        ret = mrsAFN13F1CreateAckFrame(pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen, HI_NULL);

        pstCcoCtx->stAnytimeRMModel.from = ucFrom;

        mrsDfxRmUartDenyTx(pucContent, usContentLen);
        mrsDfxXrNakCnt();
        mrsDfxXrNak((HI_U8)stDenyData);
    }

    return ret;
}


static HI_U32 makeAfn0301Buf(HI_OUT HI_U8 * buf, HI_INOUT HI_U16 * buf_len)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U16 frame_len = 9;  // ֡���ݴ�СΪ9�ֽ�
    HI_U16 offset = 0;
    HI_MDM_SYS_INFO_S *pstVerInfo = HI_NULL;

    if ((!buf) || (!buf_len) || (*buf_len < frame_len))
    {
        return HI_ERR_FAILURE;
    }

    pstVerInfo = (HI_MDM_SYS_INFO_S *)HI_MDM_GetSysInfo();
    if (!pstVerInfo)
    {
        return ret;
    }

    (hi_void)memcpy_s(buf + offset, frame_len - offset, pstVerInfo->szManuCode, MRS_MANU_CODE_LEN);
    offset += MRS_MANU_CODE_LEN;
    (hi_void)memcpy_s(buf + offset, frame_len - offset, pstVerInfo->szChipCode, MRS_CHIP_CODE_LEN);
    offset += MRS_CHIP_CODE_LEN;
    mrsIntToBcd((HI_U32)pstVerInfo->usDay, &buf[offset], sizeof(HI_U8));
    offset++;
    mrsIntToBcd((HI_U32)pstVerInfo->usMonth, &buf[offset], sizeof(HI_U8));
    offset++;
    mrsIntToBcd((HI_U32)pstVerInfo->usYear, &buf[offset], sizeof(HI_U8));
    offset++;
    (hi_void)memcpy_s(buf + offset, frame_len - offset, &pstVerInfo->usSwVer, sizeof(pstVerInfo->usSwVer));
    offset += sizeof(pstVerInfo->usSwVer);

    *buf_len = offset;

    return ret;
}

//AFN03 F1 ���̴���Ͱ汾��Ϣ
HI_U32 mrs3762_AFN03_F1(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 buf[12] = {0};
    HI_U16 buf_len = sizeof(buf);

    ret = makeAfn0301Buf(buf, &buf_len);
    if (ret == HI_ERR_SUCCESS)
    {
        //��װ��֡
        ret = mrs3762_EasyEnc(buf, buf_len, param);
    }

    return ret;
}

//AFN03 F2 ����ֵ
HI_U32 mrs3762_AFN03_F2(HI_IN HI_OUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 noise = 0;

    ret = mrs3762_EasyEnc(&noise,sizeof(noise),param);

    //����
    return ret;
}

/* ���ⵥ��: DTS2014010608635 BEGIN: Added by liuxipeng/lkf54842, 2014/01/07 */
//AFN03 F3 ��ѯ�ӽڵ�������Ϣ
HI_U32 mrs3762_AFN03_F3(HI_IN HI_OUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 resp[2] = {0};    // ��0���������Ĵӽڵ�����; ��1�����δ���Ĵӽڵ�����

    ret = mrs3762_EasyEnc(resp,sizeof(resp),param);

    //����
    return ret;
}
/* ���ⵥ��: DTS2014010608635 END:   Added by liuxipeng/lkf54842, 2014/01/07 */

//AFN03 F4���ز����ڵ��ַ
HI_U32 mrs3762_AFN03_F4(HI_INOUT HI_VOID * param)
{
    HI_U8 ucNodeAddress[6] = {0};
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();

    //��ѯ�ز����ڵ��ַ
    (hi_void)memcpy_s(ucNodeAddress, sizeof(ucNodeAddress), pstCcoCtx->ucMainNodeAddr, sizeof(pstCcoCtx->ucMainNodeAddr));
    mrsHexInvert(ucNodeAddress, sizeof(ucNodeAddress));

    //��װ��֡
    return mrs3762_EasyEnc(ucNodeAddress,sizeof(ucNodeAddress),param);
}


//AFN03 F5���ز����ڵ�״̬�ּ��ز�����
HI_U32 mrs3762_AFN03_F5(HI_INOUT HI_VOID * param)
{
    HI_U8 buf[4] = {0};

    struct
    {
        HI_U16 SpeedNum : 4;     // �ز���������
        HI_U16 CHFeature : 2;    // �ŵ�����
        HI_U16 Mode : 2;         // ����ģʽ
        HI_U16 CHNum : 4;        // �ز��ŵ�����
        HI_U16 Back : 4;         // ����
    }stStatusWord;

    struct
    {
        HI_U16 SpeedRate : 15;     // �ز�����
        HI_U16 Unit : 1;           // ���ʵ�λ
    }stSpeedRate;

    (hi_void)memset_s(&stStatusWord, sizeof(stStatusWord), 0, sizeof(stStatusWord));
    (hi_void)memset_s(&stSpeedRate, sizeof(stSpeedRate), 0, sizeof(stSpeedRate));

    stStatusWord.SpeedNum = 1;
    stStatusWord.CHFeature = 1;

    stStatusWord.CHNum = 1;

    stStatusWord.Mode = PERIOD_METER_READ_MODE_BOTH;

    stSpeedRate.Unit = 1;

    (hi_void)memcpy_s(buf, sizeof(buf), &stStatusWord, sizeof(stStatusWord));
    (hi_void)memcpy_s(buf + 2, sizeof(buf) - 2, &stSpeedRate,sizeof(stSpeedRate));

    return mrs3762_EasyEnc(buf,sizeof(buf),param);
}


//AFN03 F6 ��ѯ���ڵ����״̬
HI_U32 mrs3762_AFN03_F6(HI_IN HI_OUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 resp = 1;    // ���ڵ���ڸ���

    ret = mrs3762_EasyEnc(&resp,sizeof(resp),param);

    //����
    return ret;
}


//AFN03 F7: ��ȡ�ӽڵ������ʱʱ��
HI_U32 mrs3762_AFN03_F7(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    PLC_OVERTIME_INF_STRU *pstAfn13 = HI_NULL;

    HI_U8 maxTimeout = 0;

    /* ��ȡ��ʱʱ�� */
    pstAfn13 = mrsGetPlcOverTimeInfAfn13();
    maxTimeout = pstAfn13->MaxTimeout;

    //��װ��֡
    ret = mrs3762_EasyEnc(&maxTimeout,sizeof(maxTimeout),param);

    //����
    return ret;
}

HI_U32 mrs3762_AFN03_F9(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 * pContent = HI_NULL;
    HI_U16 usContentLen = 0;
    HI_U16 length = 0;
    HI_U8 protocol = 0;
    HI_U8 * buf = HI_NULL;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;

    pContent = pstAfnParam->pContent;
    usContentLen = pstAfnParam->usContentLen;

/* BEGIN: PN: DTS2015061101642 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/12 */
    do
    {
        // usContentLen: Fn(2) + protocol(1) + len(1) + data(len)
        length = 2 + (MRS_MAX(2, usContentLen) - 2);

        if (length >= 4)
        {
            protocol = pContent[2];
        }
        else
        {
            ret = HI_ERR_BAD_DATA;
            break;
        }

        if (!mrsCheckValidProto(protocol))
        {
            ret = HI_ERR_BAD_DATA;
            break;
        }
    } while (0);

    do
    {
        buf = mrsToolsMalloc(length);
        if (!buf)
        {
            ret = HI_ERR_NOT_ENOUGH_MEMORY;
            break;
        }

        (hi_void)memset_s(buf, length, 0, length);
        buf[0] = (MRS_BC_DELAY_TIME % 256);
        buf[1] = (MRS_BC_DELAY_TIME / 256);

        if (length > 2)
        {
            (hi_void)memcpy_s(buf + 2, length - 2, pContent + 2, length - 2);
        }

        ret = mrs3762_EasyEnc(buf, length, param);

        mrsToolsFree(buf);
    } while (0);
/* END:   PN: DTS2015061101642 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/12 */

    return ret;
}

static HI_U32 makeAfn0310Buf(HI_OUT HI_U8 ** out_buf, HI_OUT HI_U16 * out_len)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    do
    {
        HI_U8 * buf = HI_NULL;
        HI_U16 buf_len = MRS_AFN03_FN_UP_DATA_LEN;
        HI_U16 tmp = 0;
        HI_U16 offset = 0;
        HI_U16 tmp_len = 0;

        PLC_OVERTIME_INF_STRU *pstAfn13 = HI_NULL;
        MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();

        buf = mrsToolsMalloc(buf_len);
        if (buf == HI_NULL)
        {
            ret = HI_ERR_MALLOC_FAILUE;
            break;
        }

        (hi_void)memset_s(buf, buf_len, 0, buf_len);

        buf[offset++] = 0xF2;   /* D0~D3 : 2  (����������ز�ͨѶ)
                                   D4    : 1  (����·�ɹ�����)
                                   D5    : 1  (��Ҫ�·��ӽڵ���Ϣ)
                                   D6~D7 : 10 (�ز�ģ�����������ڳ���ģʽ)     */

        buf[offset++] = 0x37;   /* D0    : 1  (�㲥֧�ִ�����ʱ����)
                                   D1    : 1  (����ز��ӽڵ�֧�ִ�����ʱ����)
                                   D2    : 1  (·����������֧�ִ�����ʱ����)
                                   D3~D4 : 10 (ʧ�ܽڵ��ɼ����������л�)
                                   D5    : 1  (�㲥������ִ��ǰ����ȷ��֡)
                                   D6~D7 : 0  (ִ�й㲥�����Ҫ�ŵ���ʶ)     */

        buf[offset++] = 0x01;   // ����λ:������Ϣ  ����λ:�ز��ŵ�����
        buf[offset++] = 0x01;   // ����λ:����      ����λ:ͨѶ��������

        pstAfn13 = mrsGetPlcOverTimeInfAfn13();
        offset = MRS_CCO_MON_MAX_TIMEOUT_OFFSET;
        buf[offset] = pstAfn13->MaxTimeout;
        // �㲥�������ʱʱ�䣺ִ��05H-F3"�����㲥����"�����ʱʱ����
        tmp = MRS_AFN03_F10_BC_TIMEOUT;
        offset = MRS_CCO_BC_MAX_TIMEOUT_OFFSET;
        (hi_void)memcpy_s(buf + offset, buf_len - offset, &tmp, sizeof(tmp));

        // ���֧�ֵ�376.2���ĳ��ȣ�������ȷ���յ�376.2������󳤶�
        tmp = MRS_MAX_FRAME_LEN;
        offset = MRS_3762FRAME_MAX_LENGTH_OFFSET;
        (hi_void)memcpy_s(buf + offset, buf_len - offset, &tmp, sizeof(tmp));

        // �ļ�����֧�ֵ���󵥸����ݰ����ȣ���15H-F1"�ļ�����Э��"��֧�ֵ����ְ���С
        tmp = MRS_MAX_FRAME_LEN;
        offset = MRS_MAX_PACKET_LENGTH_OFFSET;
        (hi_void)memcpy_s(buf + offset, buf_len - offset, &tmp, sizeof(tmp));

        // ���������ȴ�ʱ�䣺�ն˷��������һ���������ݰ�����Ҫ�ȴ�ģ�����������ʱ�䳤�ȡ�
        offset = MRS_UPG_WAIT_TIME_OFFSET;
        buf[offset] = MRS_UPG_WAIT_TIME;

        // ���ڵ��ַ������ͨ��ģ������ڵ��ַ��
        offset = MRS_CCO_ADDR_OFFSET;
        (hi_void)memcpy_s(buf + offset, buf_len - offset, pstCcoCtx->ucMainNodeAddr, sizeof(pstCcoCtx->ucMainNodeAddr));
        mrsHexInvert(buf + offset, sizeof(pstCcoCtx->ucMainNodeAddr));

        // ֧�ֵ����ӽڵ���������ģ�飨��·�ɹ���֧�ֵ����ӽڵ���װ����
        tmp = PRODUCT_CFG_MRS_MAX_NODE_NUM;
        offset = MRS_STATION_MAX_NUM_OFFSET;
        (hi_void)memcpy_s(buf + offset, buf_len - offset, &tmp, sizeof(tmp));

        // ��ǰ�ӽڵ���������ģ�飨��·�ɹ�����ǰ��װ�Ĵӽڵ�����
        tmp = mrsSrvArchivesMeterNum(); // �����ڵ���
        offset = MRS_STATION_NUM_OFFSET;
        (hi_void)memcpy_s(buf + offset, buf_len - offset, &tmp, sizeof(tmp));

        // ͨ��ģ��ʹ�õ�376.2Э�鷢�����ڣ� BCD���룬YYMMDD���ڸ�ʽ
        offset = MRS_PROTO3762_RELEASE_OFFSET;
        (hi_void)memcpy_s(buf + offset, buf_len - offset, MRS_AFN03F10_RELEASE_DATE, 3);

        // ͨ��ģ��ʹ�õ�376.2Э����󱸰����ڣ� BCD���룬YYMMDD���ڸ�ʽ��
        offset = MRS_PROTO3762_BACKUP_OFFSET;
        (hi_void)memcpy_s(buf + offset, buf_len - offset, MRS_AFN03F10_BACKUP_DATE, 3);

        buf[MRS_COMMUNICATION_OFFSET] = 0x00;
        buf[MRS_COMMUNICATION_OFFSET + 1] = 0x80;

        offset = MRS_DEVICE_INFO_OFFSET;
        tmp_len = buf_len - offset;
        ret = makeAfn0301Buf(buf + offset, &tmp_len);
        if (ret == HI_ERR_SUCCESS)
        {
            *out_buf = buf;
            *out_len = buf_len;
        }
        else
        {
            mrsToolsFree(buf);
        }
    } while (0);

    return ret;
}

HI_U32 mrs3762_AFN03_F10(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 *buf = HI_NULL;
    HI_U16 buf_len = 0;

    ret = makeAfn0310Buf(&buf, &buf_len);
    if (ret == HI_ERR_SUCCESS)
    {
        ret = mrs3762_EasyEnc(buf, buf_len, param);
        mrsToolsFree(buf);
    };

    return ret;
}

HI_U32 mrs3762_AFN03_F10_UP(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 *buf = HI_NULL;
    HI_U16 buf_len = 0;
    HI_U16 out_len = 0;
    HI_U8* out_buf = HI_NULL;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
    // ���հ汾�����ظ�����buffer
    if (mrsToolIsBbu()
        && (!mrsIsSimuChlOpened()))
    {
        return HI_ERR_SKIP;
    }
#endif

    ret = makeAfn0310Buf(&buf, &buf_len);
    if (ret != HI_ERR_SUCCESS)
    {
        return ret;
    }

    do
    {
        PROTO376_2_FRAME_AFN_AND_DATA_STRU stAfnData;
        MRS_SRV_CCO_ITEM *pstCcoItem = HI_NULL;

        (hi_void)memset_s(&stAfnData, sizeof(stAfnData), 0, sizeof(stAfnData));
        stAfnData.ucAFN = MRS_AFN(0x03);
        stAfnData.ucDT[0] = MRS_AFN_FN_LO(10);
        stAfnData.ucDT[1] = MRS_AFN_FN_HI(10);
        stAfnData.pData = buf;
        stAfnData.usDataLen = buf_len;

        ret = mrsCreate3762UpFrame(&stAfnData, &out_buf, &out_len, HI_NULL, HI_NULL);
        if (ret != HI_ERR_SUCCESS)
        {
            break;
        }

        pstCcoItem = (MRS_SRV_CCO_ITEM *)mrsToolsMalloc(sizeof(MRS_SRV_CCO_ITEM) + out_len);
        if (!pstCcoItem)
        {
            mrsDfxCcoQueueMallocFailCnt();
            break;
        }

        (hi_void)memset_s(pstCcoItem, sizeof(MRS_SRV_CCO_ITEM) + out_len, 0, sizeof(MRS_SRV_CCO_ITEM) + out_len);
        pstCcoItem->bValid = HI_TRUE;
        pstCcoItem->ucAfn = 0x03;
        pstCcoItem->usFn = 0x10;
        pstCcoItem->usTimeOut = 1500;//��λΪms
        pstCcoItem->ucMaxTry = 0;
        pstCcoItem->usDataLen = out_len;
        (hi_void)memcpy_s(pstCcoItem->pData, pstCcoItem->usDataLen, out_buf, out_len);

        mrsCcoJoinQueue(pstCcoItem);

        mrsActiveCcoQueue();
    } while (0);

    mrsToolsFree(out_buf);
    mrsToolsFree(buf);

    return ret;
}

HI_U32 mrs3762_AFN03_F11(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 * pContent = HI_NULL;
    HI_U16 usContentLen = 0;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;

    pContent = pstAfnParam->pContent;
    usContentLen = pstAfnParam->usContentLen;

    /* ���ݵ�Ԫ��ʶ(2�ֽ�)+���ݵ�Ԫ(1�ֽ�) */
    if (usContentLen == 3)
    {
        HI_U32 cap_count = sizeof(MRS_AFN_CAPABILITY)/sizeof(MRS_AFN_MAP);
        HI_U32 i;
        HI_U8 buf[33];

        (hi_void)memset_s(buf, sizeof(buf), 0, sizeof(buf));
        buf[0] = pContent[2];

        for (i = 0; i < cap_count; ++i)
        {
            if (pContent[2] == MRS_AFN_CAPABILITY[i].afn)
            {
                (hi_void)memcpy_s(buf + 1, sizeof(buf) - 1, MRS_AFN_CAPABILITY[i].capability, sizeof(MRS_AFN_CAPABILITY[i].capability));
                break;
            }
        }

        ret = mrs3762_EasyEnc(buf, sizeof(buf), param);
        return ret;
    }
    else
    {
        ret = HI_ERR_BAD_DATA;
    }

    return ret;
}

HI_U32 mrs3762_AFN03_F16(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 ucFreq = 0;
    HI_U8 ucLen = sizeof(ucFreq);
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();

    if (pstCcoCtx->ucSetFreqFlag)
    {
        ucFreq = pstCcoCtx->ucFreq;
    }
    else
    {
        ucFreq = HI_MDM_GetWholeNetWorkFreq();
    }
    ret = mrs3762_EasyEnc(&ucFreq, (HI_U16)ucLen, param);

    return ret;
}

HI_U32 mrs3762_AFN10_F100(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U16 usTopoNum = 0;

    usTopoNum = mrsGetCurrTopoNum();
    ret = mrs3762_EasyEnc((HI_U8*)&usTopoNum,sizeof(usTopoNum),param);

    return ret;
}

HI_U16 mrsGetCurrTopoNum(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_MAC_CONFIG_INFO_S stMacInfo = {0};
    HI_U16 usTopoNum = 0;

    stMacInfo.pTopo = pstCcoCtx->pTopoIntegerInfo;
    HI_MDM_QueryInfo(&stMacInfo);  // ��ȡmac���˱���Ϣ
    usTopoNum = (HI_U16)((HI_MAC_NETWORK_TOPO_S*)pstCcoCtx->pTopoIntegerInfo)->num;

    return usTopoNum;
}

//*****************************************************************************
// ��������: mrs3762_AFN10_F101
// ��������: AFN=10H-F101������
//
// ����˵��:
//    param ����������
//
// �� �� ֵ:
//    HI_ERR_SUCCESS   ��ȷ��Ӧ
//
// ����Ҫ��:
// ���þ���:
// ��    ��: fengxiaomin/00209182 [2014-1-10]
//*****************************************************************************
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014032708427 */
HI_U32 mrs3762_AFN10_F101(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 * pContent = HI_NULL;
    HI_U16 usContentLen = 0;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//����֡�еĴ���״̬��
    HI_U16 usStartSeq = 0;
    HI_U8 ucQueryNum = 0;
    HI_U8 *pBuffer = HI_NULL;
    HI_U16 usBufLen = 0;
    HI_U8 ucReportNum = 0;

    pContent = pstAfnParam->pContent;
    usContentLen = pstAfnParam->usContentLen;

    do{
        /* ���ݵ�Ԫ��ʶ(2�ֽ�)+���ݵ�Ԫ(3�ֽ�) */
        if (usContentLen != 5)
        {
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }

        if (MRS_CCO_UPGRADE_LOCAL_UPG_PERIOD == mrsGetRemoteUpgStatus())
        {
            ret = HI_ERR_FAILURE;
            stDenyData = MRS_13762_DENY_STATUS_CCO_BUSY;
            break;
        }

        ucQueryNum = *(pContent + 4);
        if (ucQueryNum > MRS_CCO_QUERY_NODE_NUM_MAX)
        {
            ret = HI_ERR_BAD_DATA;
            break;
        }

        usBufLen = MRS_CCO_10_F101_DATA_FIX_LEN + ucQueryNum * MRS_CCO_10_F101_VER_INFO_LEN;
        pBuffer = (HI_U8*)mrsToolsMalloc(usBufLen);
        if (!pBuffer)
        {
            ret = HI_ERR_FAILURE;
            break;
        }

        (hi_void)memcpy_s(&usStartSeq, sizeof(usStartSeq), pContent + 2, sizeof(usStartSeq));
        (hi_void)memset_s(pBuffer, usBufLen, 0, usBufLen);
        ret = mrsAfn10101Buf(pBuffer, &ucReportNum, ucQueryNum, usStartSeq);
    } while (0);

    if (HI_ERR_SUCCESS == ret)
    {
        HI_U16 usReplyLen = (HI_U16)(MRS_CCO_10_F101_DATA_FIX_LEN + ucReportNum * MRS_CCO_10_F101_VER_INFO_LEN);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */

        mrs3762_EasyEnc(pBuffer,usReplyLen,param);
    }
    else
    {
        ret = mrs3762ProtoDenyFrame(&stDenyData,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
    }

    mrsToolsFree(pBuffer);

    return ret;
}

//*****************************************************************************
// ��������: mrsAfn10101Buf
// ��������: ��ȡվ��汾��Ϣ
//
// ����˵��:
//    pBuffer ����������
//    pBufLen ���������ݳ���
//    ucQueryNum ѯ������
//    usStartSeq ��ʼ���
//
// �� �� ֵ:
//    �ϱ�����
//
// ����Ҫ��:
// ���þ���:
// ��    ��: fengxiaomin/00209182 [2014-1-10]
//*****************************************************************************
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014032708427 */
HI_U32 mrsAfn10101Buf(HI_U8* pBuffer, HI_U8* pucReportNum, HI_U8 ucQueryNum, HI_U16 usStartSeq)
{
    HI_U16 usIndex = 0;
    HI_U8 pAddr[6] = {0};
    HI_U16 usNodeInf = 0;
    HI_U8 ucBootVer = 0;
    HI_U16 ucOffsetNum = 0;
    HI_U8 ucReportNum = 0;
    HI_MAC_NETWORK_TOPO_S * pstTopoInf = HI_NULL;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();

    pstTopoInf = &pstCcoCtx->stTopoInf;
    (hi_void)memset_s(pAddr, sizeof (pAddr), 0, sizeof (pAddr));

    (hi_void)memcpy_s(pBuffer + ucOffsetNum, sizeof(HI_U16), &pstTopoInf->num, sizeof(HI_U16));
    ucOffsetNum += sizeof(HI_U16);
    ucOffsetNum += sizeof(ucReportNum);

    for (usIndex = usStartSeq; usIndex < usStartSeq + ucQueryNum; usIndex++)
    {
        if (pstTopoInf->num <= usIndex)
        {
            break;
        }

        if (!usIndex)
        {
            (hi_void)memcpy_s(pAddr, sizeof(pAddr), pstCcoCtx->ucMainNodeAddr, sizeof(pAddr));
            mrsHexInvert(pAddr, sizeof(pAddr));
            usNodeInf = 0;
        }
        else
        {
            (hi_void)memcpy_s(pAddr, sizeof(pAddr), pstTopoInf->entry[usIndex].mac, sizeof(pAddr));
            mrsHexInvert(pAddr, sizeof(pAddr));
            usNodeInf = MRS_CCO_STA_NODE_INFORMATION;
            MRS_TOOLS_FE_TO_00(pAddr[0]);
        }

        ucBootVer = pstTopoInf->entry[usIndex].sta_ver.ucBootVer;
        (hi_void)memcpy_s(pBuffer + ucOffsetNum, sizeof(pAddr), pAddr, sizeof(pAddr));
        ucOffsetNum += sizeof(pAddr);
        (hi_void)memcpy_s(pBuffer + ucOffsetNum, sizeof(usNodeInf), &usNodeInf, sizeof(usNodeInf));
        ucOffsetNum += sizeof(usNodeInf);
        (hi_void)memcpy_s(pBuffer + ucOffsetNum, MRS_SW_VER_LEN, &pstTopoInf->entry[usIndex].sta_ver.usSwVer, MRS_SW_VER_LEN);
        ucOffsetNum += MRS_SW_VER_LEN;
        *(pBuffer + ucOffsetNum) = ucBootVer;
        ucOffsetNum += sizeof(ucBootVer);
        ucReportNum++;
    }

    if ((!usStartSeq) && (!ucReportNum) && ucQueryNum)
    {
        ucReportNum++;
        mrsGetCco10F101Data(pBuffer, ucReportNum);
    }

    *(pBuffer + MRS_CCO_10_F101_REPORT_NUM_OFFSET) = ucReportNum;
    *pucReportNum = ucReportNum;

    return HI_ERR_SUCCESS;
}

HI_VOID mrsGetCco10F101Data(HI_U8 *pBuffer, HI_U8 ucReportNum)
{
    HI_U16 usTotalNum = 1;
    HI_U16 usNodeInfo = 0;
    HI_MDM_SYS_INFO_S *pstVerInfo = HI_NULL;
    HI_U8 ucBootVer = 0;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();

    pstVerInfo = (HI_MDM_SYS_INFO_S *)HI_MDM_GetSysInfo();
    if (pstVerInfo == HI_NULL)
    {
        return;
    }

    ucBootVer = pstVerInfo->ucBootVer;

    //CCO�İ汾��Ϣ
    (hi_void)memcpy_s(pBuffer, sizeof(usTotalNum), &usTotalNum, sizeof(usTotalNum));
    *(pBuffer + MRS_CCO_10_F101_REPORT_NUM_OFFSET) = ucReportNum;
    (hi_void)memcpy_s(pBuffer + MRS_CCO_10_F101_ADDR_OFFSET, sizeof(pstCcoCtx->ucMainNodeAddr), pstCcoCtx->ucMainNodeAddr, sizeof(pstCcoCtx->ucMainNodeAddr));
    mrsHexInvert(pBuffer + MRS_CCO_10_F101_ADDR_OFFSET, sizeof(pstCcoCtx->ucMainNodeAddr));
    (hi_void)memcpy_s(pBuffer + MRS_CCO_10_F101_NODER_INFO_OFFSET, sizeof(usNodeInfo), &usNodeInfo, sizeof(usNodeInfo));
    (hi_void)memcpy_s(pBuffer + MRS_CCO_10_F101_SOFTWARE_VER_OFFSET, sizeof(pstVerInfo->usSwVer), &pstVerInfo->usSwVer, sizeof(pstVerInfo->usSwVer));
    (hi_void)memcpy_s(pBuffer + MRS_CCO_10_F101_BOOT_VER_OFFSET, sizeof(ucBootVer), &ucBootVer, sizeof(ucBootVer));

    return;
}

/* END:   Added by fengxiaomin/00209182, 2014/1/10 */

/* ���ⵥ��: DTS2014010608635 BEGIN: Added by liuxipeng/lkf54842, 2014/01/07 */
//AFN04 F1 ���Ͳ���
HI_U32 mrs3762_AFN04_F1(HI_IN HI_OUT HI_VOID * param)
{
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;

    return mrs3762ProtoAckFrame(0, pstAfnParam, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
}

//AFN04 F2 �ز��ӽڵ����
HI_U32 mrs3762_AFN04_F2(HI_IN HI_OUT HI_VOID * param)
{
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;

    return mrs3762ProtoAckFrame(0, pstAfnParam, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
}
/* ���ⵥ��: DTS2014010608635 END:   Added by liuxipeng/lkf54842, 2014/01/07 */

HI_U32 mrs3762_AFN04_F3(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 * pContent = HI_NULL;
    HI_U16 usContentLen = 0;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//����֡�еĴ���״̬��

    pContent = pstAfnParam->pContent;
    usContentLen = pstAfnParam->usContentLen;

    do
    {
        MRS_PLC_FRAME_DATA_S stPlcData = {0};
        MRS_TEST_TRANSMIT_STRU * test_frame = HI_NULL;
        HI_U8 * payload = HI_NULL;
        HI_U16 payload_len = 0;
        HI_U16 head_len = 9; // ͨ������(1)+Ŀ���ַ(6)+ͨ��Э������(1)+���ĳ���(1)
        HI_U16 data_len = (HI_U16)pContent[head_len - 1 + 2];
        HI_U8 aucAddr[HI_METER_ADDR_LEN] = {0};

        if (usContentLen != head_len + data_len + 2)
        {
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }

        mrsMeterAddrPreProc(aucAddr, pContent + 3, pContent[9]);

        //�Ƿ��ڱ�����
        if (mrsSrvArchivesIsExsit(aucAddr) == HI_FALSE)
        {
            ret = HI_ERR_NOT_FOUND;
            stDenyData = MRS_13762_DENY_STATUS_INEXISTENT_METER;
            break;
        }

        // ����MAC��ַ
        ret = mrsMapGetMAC(aucAddr, stPlcData.ucMac);
        if (ret != HI_ERR_SUCCESS)
        {
            mrsToolsConvertMeter2Mac(aucAddr, METER_PROTO_645_2007, stPlcData.ucMac);
        }

        if (mrsCheck645Proto(pContent[9]))
        {
            HI_U16 pos = 0;
            HI_U16 frame_len = 0;

            ret = mrsFind645Frame(pContent + 10, (HI_S16)(usContentLen - 10), &pos, &frame_len);
            if (ret != HI_ERR_SUCCESS)
            {
                ret = HI_ERR_BAD_DATA;
                stDenyData = MRS_13762_DENY_STATUS_INVALID_DATA;
                break;
            }
        }

        payload_len = sizeof(MRS_TEST_TRANSMIT_STRU) + data_len;
        payload = mrsToolsMalloc(payload_len);
        if (!payload)
        {
            ret = HI_ERR_MALLOC_FAILUE;
            stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;
            break;
        }

        (hi_void)memset_s(payload, payload_len, 0, payload_len);

        test_frame = (MRS_TEST_TRANSMIT_STRU *)payload;
        test_frame->interface_ver = MRS_TEST_TRANSMIT_DL_VER;
        test_frame->stru_len = sizeof(MRS_TEST_TRANSMIT_STRU);
        test_frame->data_len = data_len;
        test_frame->protocol = pContent[9];
        test_frame->timeout = 0;
        (hi_void)memcpy_s(test_frame->data, test_frame->data_len, pContent + head_len + 2, data_len);

        stPlcData.usId = ID_MRS_CMD_TEST_TRANSMIT_PLC;
        stPlcData.pucPayload = payload;
        stPlcData.usPayloadLen = payload_len;

        ret = MRS_SendPlcFrame(&stPlcData);

        mrsToolsFree(payload);
    } while (0);

    if (ret == HI_ERR_SUCCESS)
    {
        ret = mrs3762ProtoAckFrame(0, pstAfnParam, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
    }
    else
    {
        ret = mrs3762ProtoDenyFrame(&stDenyData,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
    }

    return ret;
}

HI_U32 mrs3762_AFN04_F4(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 * pContent = HI_NULL;
    HI_U16 usContentLen = 0;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//����֡�еĴ���״̬��
   	HI_U32 ulSnid=0;

    pContent = pstAfnParam->pContent;
    usContentLen = pstAfnParam->usContentLen;

    do
    {
        
        /* ���ݵ�Ԫ��ʶ(2�ֽ�)+���ݵ�Ԫ(3�ֽ�) */
        if (usContentLen != 5)
        {
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }
		ulSnid = pContent[2]|(pContent[3]<<8)|(pContent[4]<<16);
        mrsSetSnid(ulSnid);

        //��֯ȷ��֡
        mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
    } while(0);

    if (ret != HI_ERR_SUCCESS)
    {
        ret = mrs3762ProtoDenyFrame(&stDenyData,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
    }

    return ret;
}


//AFN05 F4������SNID��Ϣ
HI_U32 mrs3762_AFN04_F5(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 * pContent = HI_NULL;
    HI_U16 usContentLen = 0;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//����֡�еĴ���״̬��
   	HI_U32 ulSnid=0;

    pContent = pstAfnParam->pContent;
    usContentLen = pstAfnParam->usContentLen;

    do
    {
        
        /* ���ݵ�Ԫ��ʶ(2�ֽ�)+���ݵ�Ԫ(3�ֽ�) */
        if (usContentLen != 5)
        {
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }
		ulSnid = pContent[2]|(pContent[3]<<8)|(pContent[4]<<16);
        mrsSetSnid(ulSnid);

        //��֯ȷ��֡
        mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
    } while(0);

    if (ret != HI_ERR_SUCCESS)
    {
        ret = mrs3762ProtoDenyFrame(&stDenyData,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
    }

    return ret;
}


//AFN05 F1�������ز����ڵ��ַ
HI_U32 mrs3762_AFN05_F1(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 * pContent;
    HI_U16 usContentLen = 0;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//����֡�еĴ���״̬��
    HI_U8 aucLastMainNodeAddr[6] = {0};

    if (param == HI_NULL)
    {
        ret = HI_ERR_BAD_DATA;
        return ret;
    }

    //��������Ϊ6���ֽڵ����ڵ��ַ
    pContent = pstAfnParam->pContent;
    usContentLen = pstAfnParam->usContentLen;

    do
    {
        if (usContentLen != (sizeof(pstCcoCtx->ucMainNodeAddr) + 2)) //�����ز����ڵ��Ӧ�ó���Ϊ
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_012, HI_DIAG_MT("Length err=%d"),usContentLen);
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }

        (hi_void)memcpy_s(aucLastMainNodeAddr, sizeof(pstCcoCtx->ucMainNodeAddr), pstCcoCtx->ucMainNodeAddr, sizeof(pstCcoCtx->ucMainNodeAddr));
        //�������ڵ��ַ
        (hi_void)memcpy_s(pstCcoCtx->ucMainNodeAddr, sizeof(pstCcoCtx->ucMainNodeAddr), pContent + 2, sizeof(pstCcoCtx->ucMainNodeAddr));
        mrsHexInvert(pstCcoCtx->ucMainNodeAddr, sizeof(pstCcoCtx->ucMainNodeAddr));
        HI_MDM_DevIdConfig(pstCcoCtx->ucMainNodeAddr);

        HI_MDM_SetMainNodeAddr(pstCcoCtx->ucMainNodeAddr);

        if (pstCcoCtx->stAutoSearchModel.bCheckingMainAddr
            && !mrsToolsMemEq(aucLastMainNodeAddr, pstCcoCtx->ucMainNodeAddr, sizeof(pstCcoCtx->ucMainNodeAddr)))
        {
            mrsCcoHandleMainAddrChange(pstCcoCtx);
        }

        //��������IDдNV������
#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
        HI_MDM_RstDelayEnable(HI_SYS_RST_DELAY_CON_WR_NV, HI_SYS_RST_DELAY_CON_WR_NV_TO);
#endif
        ret = mrsCcoAddrWriteNv();
#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
        HI_MDM_RstDelayDisable(HI_SYS_RST_DELAY_CON_WR_NV);
#endif

        if (ret != HI_ERR_SUCCESS)
        {
            stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;
            break;
        }

        //��֯ȷ��֡
        mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);


    }
    while (0);

    if (ret != HI_ERR_SUCCESS)
    {
        ret = mrs3762ProtoDenyFrame(&stDenyData,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
    }

    return ret;
}

HI_U32 mrsCcoAddrWriteNv(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_NV_FTM_CCO_PARAM_ID_S stCcoParam;

    (hi_void)memset_s(&stCcoParam, sizeof(stCcoParam), 0, sizeof(stCcoParam));
    ret = HI_MDM_NV_Read(HI_NV_FTM_CCO_PARAM_ID, &stCcoParam, sizeof(HI_NV_FTM_CCO_PARAM_ID_S));
    if (ret != HI_ERR_SUCCESS)
    {
        return ret;
    }

    (hi_void)memcpy_s(stCcoParam.ucDevAddr, sizeof(stCcoParam.ucDevAddr), pstCcoCtx->ucMainNodeAddr, sizeof(pstCcoCtx->ucMainNodeAddr));

    ret = HI_MDM_NV_Write(HI_NV_FTM_CCO_PARAM_ID, &stCcoParam, sizeof(HI_NV_FTM_CCO_PARAM_ID_S));

    return ret;
}

//AFN05 F2������ӽڵ��ϱ�
HI_U32 mrs3762_AFN05_F2(HI_INOUT HI_VOID * param)
{
    HI_U16 usContentLen = 0;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//����֡�еĴ���״̬��
    HI_U32 ret = HI_ERR_SUCCESS;
    P_MRS_CCO_EVT_CTX_STRU pstEvtCtx = mrsGetCcoEvtCtx();
    HI_U8 ucEvtEnable = 0;

    if (param == HI_NULL)
    {
        ret = HI_ERR_BAD_DATA;
        return ret;
    }

    usContentLen = pstAfnParam->usContentLen;
    /* ���ݵ�Ԫ��ʶ(2�ֽ�)+���ݵ�Ԫ(1�ֽ�) */
    if (usContentLen != 3)
    {
        ret = HI_ERR_BAD_DATA;
        stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
    }

    ucEvtEnable = pstAfnParam->pContent[2];
    if (ucEvtEnable >= 2)
    {
        ret = HI_ERR_BAD_DATA;
        stDenyData = MRS_13762_DENY_STATUS_ERROR_FORMAT;
    }
    else
    {
        pstEvtCtx->ucReportEnable = ucEvtEnable;
    }

    if (HI_ERR_SUCCESS == ret)
    {
        ret = mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
    }
    else
    {
        ret = mrs3762ProtoDenyFrame(&stDenyData,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
    }

    return ret;
}

//AFN05 F3�������㲥
HI_U32 mrs3762_AFN05_F3(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 * pContent = HI_NULL;
    HI_U16 usContentLen = 0;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//����֡�еĴ���״̬��

    if (param == HI_NULL)
    {
        ret = HI_ERR_BAD_DATA;
        return ret;
    }


    pContent = pstAfnParam->pContent;
    usContentLen = pstAfnParam->usContentLen;

    do
    {
        HI_U16 pos = 0;
        HI_U16 frame_len = 0;
        MRS_CCO_BROADCAST_STRU * pstBroadcast = mrsGetBroadcastCtx();

        if ((HI_U8)MRS_CCO_BROADCAST_BUSY == pstBroadcast->stParam.ucState)
        {
            ret = HI_ERR_BUSY;
            stDenyData = MRS_13762_DENY_STATUS_CCO_BUSY;
            break;
        }

        if (!mrsGetCcoReadyStatus(HI_TRUE))
        {
            ret = HI_ERR_BUSY;
            stDenyData = MRS_13762_DENY_STATUS_CCO_BUSY;
            break;
        }

        // ����: Fn(2) + protocol(1) + len(1) + data(len)
        if (usContentLen != 4 + pContent[3])
        {
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }

        // ��Լ
        switch (pContent[2])
        {
            case METER_PROTO_645_1997:
            case METER_PROTO_645_2007:
                ret = mrsFind645Frame(pContent, (HI_S16)usContentLen, &pos, &frame_len);
                if (ret != HI_ERR_SUCCESS)
                {
                    ret = HI_ERR_BAD_DATA;
                    stDenyData = MRS_13762_DENY_STATUS_INVALID_DATA;
                    break;
                }

                break;

            case METER_PROTO_TRANSPARENT: // ͸��
            case 3: // ��λ
                break;

            default:
                ret = HI_ERR_BAD_DATA;
                stDenyData = MRS_13762_DENY_STATUS_INVALID_DATA;
                break;
        }

        if (HI_ERR_SUCCESS != ret)
        {
            break;
        }

        ret = mrsBroadcastProcess(pContent + 4, pContent[3]);
        if (HI_ERR_SUCCESS != ret)
        {
            mrsBroadcastReset();
            stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;
            break;
        }
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
        // ����ǽ��հ汾(NV����)����ҵ�񴮿ڣ�������;��������
        if (!(mrsToolIsBbu()
            && (!mrsIsSimuChlOpened())))
#endif
        {
            //��֯ȷ��֡
            mrs3762ProtoAckFrame(6, pstAfnParam, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
        }
    } while(0);

    if (ret != HI_ERR_SUCCESS)
    {
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
        // ����ǽ��հ汾(NV����)����ҵ�񴮿ڣ�������;��������
        if (!(mrsToolIsBbu()
            && (!mrsIsSimuChlOpened())))
#endif
        {
            ret = mrs3762ProtoDenyFrame(&stDenyData, pstAfnParam, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
        }
    }

    return ret;
}

//AFN05 F4�����ôӽڵ������ʱʱ��
HI_U32 mrs3762_AFN05_F4(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 * pContent = HI_NULL;
    HI_U16 usContentLen = 0;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//����֡�еĴ���״̬��

    pContent = pstAfnParam->pContent;
    usContentLen = pstAfnParam->usContentLen;

    do
    {
        PLC_OVERTIME_INF_STRU *pstAfn13 = HI_NULL;
/* BEGIN: PN: DTS2015042205399 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
        NV_MRS_RM_CFG_STRU stRmCfgInf;
/* END:   PN: DTS2015042205399 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */

        /* ���ݵ�Ԫ��ʶ(2�ֽ�)+���ݵ�Ԫ(1�ֽ�) */
        if (usContentLen != 3)
        {
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }

        /* ���ò��� */
        pstAfn13 = mrsGetPlcOverTimeInfAfn13();
        if (pContent[2] < MRS_RM_MAX_SERVICE_TIME_MIN)
        {
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_INVALID_DATA;
            break;
        }
        else
        {
            pstAfn13->MaxTimeout = pContent[2];
        }

/* BEGIN: PN: DTS2015042205399 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
        /* дNV */
        (hi_void)memset_s(&stRmCfgInf, sizeof(stRmCfgInf), 0, sizeof(stRmCfgInf));

        ret = HI_MDM_NV_Read(ID_NV_MRS_RM_CFG, &stRmCfgInf, sizeof(stRmCfgInf));
        if (ret != HI_ERR_SUCCESS)
        {
            break;
        }

        if (stRmCfgInf.stRetryCfg[0].MaxServiceTime != pstAfn13->MaxTimeout)
        {
            stRmCfgInf.stRetryCfg[0].MaxServiceTime = pstAfn13->MaxTimeout;

            ret = HI_MDM_NV_Write(ID_NV_MRS_RM_CFG, &stRmCfgInf, sizeof(stRmCfgInf));
            if (ret != HI_ERR_SUCCESS)
            {
                break;
            }
        }
/* END:   PN: DTS2015042205399 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */

        //��֯ȷ��֡
        mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
    } while(0);

    if (ret != HI_ERR_SUCCESS)
    {
        ret = mrs3762ProtoDenyFrame(&stDenyData,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
    }

    return ret;
}
//AFN05 F5����������ͨ��
HI_U32 mrs3762_AFN05_F5(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    //HI_U8 * pContent = HI_NULL; //weichao
    HI_U16 usContentLen = 0;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//����֡�еĴ���״̬��

    //pContent = pstAfnParam->pContent; //weichao
    usContentLen = pstAfnParam->usContentLen;

    do
    {
        /* ���ݵ�Ԫ��ʶ(2�ֽ�)+���ݵ�Ԫ(1�ֽ�) */
        if (usContentLen != 4)
        {
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }

        //��֯ȷ��֡
        mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
    } while(0);

    if (ret != HI_ERR_SUCCESS)
    {
        ret = mrs3762ProtoDenyFrame(&stDenyData,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
    }

    return ret;
}

HI_U32 mrs3762_AFN05_F101(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    //���е�Ӧ������
    HI_U8 * pucContent = pstAfnParam->pContent;
    HI_U16 usContentLen = pstAfnParam->usContentLen;
	AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//����֡�еĴ���״̬��

    do
    {
        // Fn(2Byte) + ʱ��(������ʱ����)
        if(usContentLen != 2 + 6)
        {
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }

        ret = MRS_TimeIsValid(&pucContent[2]);
        if (ret != HI_ERR_SUCCESS)
        {
            ret = HI_ERR_BAD_DATA;
			stDenyData =MRS_13762_DENY_STATUS_INVALID_DATA;
            break;
        }
#if 0//optimized by weichao
        ret = mrsCcoSetClock(&pucContent[2]);
        if (ret != HI_ERR_SUCCESS)
        {
            break;
        }
#endif
		//��֯ȷ��֡
        mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
  
    } while (0);
	if (ret != HI_ERR_SUCCESS)
	{
		ret = mrs3762ProtoDenyFrame(&stDenyData,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
	}
    return ret;
}

//AFN05 F06: ʹ��/��ֹ̨��ʶ��
HI_U32 mrs3762_AFN05_F6(HI_INOUT HI_VOID * param)
{
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;

    do
    {
        if (pstAfnParam->usContentLen != 3)
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_069, HI_DIAG_MT("Length err=%d"), pstAfnParam->usContentLen);
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }

        if (pstAfnParam->pContent[2] >= MRS_TF_REPORT_MODE_NUM)
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_068, HI_DIAG_MT("Content err=%d"), pstAfnParam->pContent[2]);
            stDenyData = MRS_13762_DENY_STATUS_INVALID_DATA;
            break;
        }

        mrsTfCcoHandleRcvNotify(pstAfnParam->pContent[2]);

        return mrs3762ProtoAckFrame(0, pstAfnParam, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
    } while(0);

    return mrs3762ProtoDenyFrame(&stDenyData, pstAfnParam, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
}

//AFN05 F130������/ͣ��̨��ʶ������
HI_U32 mrs3762_AFN05_F130(HI_INOUT HI_VOID * param)
{
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;

    do
    {
        if (pstAfnParam->usContentLen != 3)
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("05 f130 Length err=%d"), pstAfnParam->usContentLen);
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }

        if (pstAfnParam->pContent[2] > 1)
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_002, HI_DIAG_MT("05 f130 Content err=%d"), pstAfnParam->pContent[2]);
            stDenyData = MRS_13762_DENY_STATUS_INVALID_DATA;
            break;
        }

        mrsTfCcoHandleRcv05F130(pstAfnParam->pContent[2]);

        //��֯ȷ��֡
        return mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
    }while(0);

    return mrs3762ProtoDenyFrame(&stDenyData, pstAfnParam, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
}

HI_U32 mrs3762_AFN05_F16(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 * pContent;
    HI_U16 usContentLen = 0;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//����֡�еĴ���״̬��
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();

    pContent = pstAfnParam->pContent;
    usContentLen = pstAfnParam->usContentLen;

    do
    {
        if (usContentLen != 3)
        {
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }

        ret = HI_MDM_SetWholeNetWorkFreq(pContent[2], 300);
        if (HI_ERR_SUCCESS != ret)
        {
            stDenyData = MRS_13762_DENY_STATUS_INVALID_DATA;
            break;
        }

        pstCcoCtx->ucFreq = pContent[2];
        pstCcoCtx->ucSetFreqFlag = HI_TRUE;
        //��֯ȷ��֡
        mrs3762ProtoAckFrame(300, pstAfnParam,pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
    } while(0);

    if (ret != HI_ERR_SUCCESS)
    {
        ret = mrs3762ProtoDenyFrame(&stDenyData,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
    }

    return ret;
}

//AFN10 F1���ز��ӽڵ�����
HI_U32 mrs3762_AFN10_F1(HI_INOUT HI_VOID * param)
{
    HI_U16 usSlaveNum[2] = {0};

    usSlaveNum[0] = mrsSrvArchivesMeterNum();
    usSlaveNum[1] = PRODUCT_CFG_MRS_MAX_NODE_NUM;

    return mrs3762_EasyEnc((HI_U8 *)usSlaveNum, sizeof(usSlaveNum), param);
}

//AFN10 F2���ز��ӽڵ���Ϣ
HI_U32 mrs3762_AFN10_F2(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 * pContent;
    HI_U16 usContentLen = 0;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    HI_U16 usStartNo;
    HI_U16 usMeterNum = mrsSrvArchivesMeterNum();
    HI_U8  ucFind = 0;
    HI_U8 ucNodeNum = 0;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_U8 *pBuffer = HI_NULL;

    //��������Ϊ3���ֽڵ����ڵ��ַ:�ӽڵ���ʼ��� 2���ֽ�,�ӽڵ�����1���ֽ�
    pContent = pstAfnParam->pContent;
    usContentLen = pstAfnParam->usContentLen;

    do
    {
        HI_U16 usMeterOrder = 0;
        HI_U8 * pCurrentNode = 0;

        //�ز��ӽڵ���Ϣ
        if (usContentLen != 5)
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_013, HI_DIAG_MT("length err=%d"),usContentLen);
            ret = HI_ERR_BAD_DATA;
            return ret;
        }

        //��ȡ���˴ӽڵ���Ϣ
        (hi_void)memcpy_s(&usStartNo, sizeof(usStartNo), pContent + 2, sizeof(usStartNo));
        ucNodeNum = pContent[4];
        if(ucNodeNum > MRS_3762_F10_NODE_MAX_NUM)
        {
            // �ӽڵ���������������ƣ�����32����
            ucNodeNum = MRS_3762_F10_NODE_MAX_NUM;
        }

        //�����С(������2 + ��ǰ����1 + �ڵ���Ϣ8*N),ȫ�ֱ���
        pBuffer = (HI_U8 *)mrs3762AFN10F2Buffer;
        (hi_void)memset_s(pBuffer, sizeof(mrs3762AFN10F2Buffer), 0, sizeof(mrs3762AFN10F2Buffer));

        pCurrentNode = pBuffer + 3;

        if (MRS_CCO_SEQ_START_FROM_1 == pstCcoCtx->ucSeqControl)
        {
            usStartNo--;
        }

        for (usMeterOrder = usStartNo; (usMeterOrder < PRODUCT_CFG_MRS_MAX_NODE_NUM)
            && (ucFind < ucNodeNum); usMeterOrder++)
        {
            MRS_ARCHIVES_NODE_STRU *pstMeterNode = mrsSrvArchivesQuery(usMeterOrder);

            if ((pstMeterNode != HI_NULL)&& (pstMeterNode->bValid))
            {
                AFN_10_F2_NODE_INFO * pNode = (AFN_10_F2_NODE_INFO * )pCurrentNode;
                pCurrentNode += sizeof(AFN_10_F2_NODE_INFO);

                (hi_void)memset_s(pNode, sizeof(AFN_10_F2_NODE_INFO), 0, sizeof(AFN_10_F2_NODE_INFO));
                (hi_void)memcpy_s(pNode->ucNodeAddr, sizeof(pNode->ucNodeAddr), pstMeterNode->aucNodeAddr, HI_METER_ADDR_LEN);

                pNode->ucPhase = mrsGetPhaseFromMap(pstMeterNode->aucNodeAddr);
                pNode->ucProtocol = (pstMeterNode->ucProtocol & 0x7);
                ucFind ++;
            }
        }
    }
    while(0);

    {
        HI_U16 usReplyLen = (HI_U16)(3 + ucFind * sizeof(AFN_10_F2_NODE_INFO));

        //����������
        (hi_void)memcpy_s(pBuffer, 2, &usMeterNum, 2);
        pBuffer[2] = ucFind;
        ret = mrs3762_EasyEnc(pBuffer,usReplyLen,param);
    }

    mrsCcoSmWhitelistSwitchTimerReset();

    return ret;
}

/* ���ⵥ��: DTS2014010608635 BEGIN: Added by liuxipeng/lkf54842, 2014/01/07 */

//AFN10 F3 ��ѯָ���ӽڵ���һ���м�·����Ϣ
HI_U32 mrs3762_AFN10_F3(HI_IN HI_OUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 resp = 0;    // �ṩ·�ɵĴӽڵ�������

    ret = mrs3762_EasyEnc(&resp,sizeof(resp),param);

    //����
    return ret;
}
/* ���ⵥ��: DTS2014010608635 END:   Added by liuxipeng/lkf54842, 2014/01/07 */

//*****************************************************************************
// ��������: mrsGetRouteFlag
// ��������: ��ȡ·����ɱ�־
//
// ����˵��:
//   HI_VOID
//
// �� �� ֵ:
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/f00209182 [2013-02-21]
//*****************************************************************************
HI_BOOL mrsGetRouteFlag(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();

    return (HI_BOOL)(mrsCcoIsTestMode() || pstCcoCtx->PlcChStatus);
}

//*****************************************************************************
// ��������: mrs3762_AFN10_F4
// ��������: AFN10 F4: ��ѯ·������״̬
//
// ����˵��:
//   param [in/out] ���ݵ�Ԫ
//
// �� �� ֵ:
//   HI_ERR_SUCCESS ����ȷ��֡
//   HI_ERR_FAILURE ��ַӳ��������ȡʧ��
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/f00209182 [2012-06-01]
//*****************************************************************************
HI_U32 mrs3762_AFN10_F4(HI_INOUT HI_VOID * param)
{
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    P_MRS_CCO_EVT_CTX_STRU pstEvtCtx = mrsGetCcoEvtCtx();
    HI_U8 pBuffer[16] = {0};
    HI_U8 ucWorkFlag = 0;
    HI_U8 route_flag = 0;
    HI_U8 ucWorkStatus = 0;
    HI_U16 usMeterNum = mrsSrvArchivesMeterNum();
    HI_U8 ucTfFlag = 0;

    (hi_void)memset_s(pBuffer, sizeof(pBuffer), 0, sizeof(pBuffer));
    ucWorkStatus = mrsGetCcoWorkFlag();
    if (MRS_CCO_NON_WORKING_STATUS != ucWorkStatus) {
        ucWorkFlag = MRS_CCO_WORKING_STATUS;
    } else {
        ucWorkStatus = 0;
    }

    if (mrs_check_area_code(APP_AREA_CODE_BEIJING) == HI_TRUE) {
        ucTfFlag = (HI_MDM_IsTfComplete() == HI_TRUE ? 0 : 1);
        if (1 == ucTfFlag) {
            ucWorkFlag = MRS_CCO_WORKING_STATUS;
        }
    } else {
        ucTfFlag = mrsTfCcoIsEnable();
    }

    route_flag = (HI_U8)mrsGetRouteFlag();

    /*
     * ����״̬��
     * [D0:D0] ·����ɱ�־  1:·��ѧϰ��� 0:δ���
     * [D1:D1] ������־      1:���ڹ��� 0:ֹͣ����  [�������ⶨ��: ̨��ʶ��Ҳ�������ڹ���]
     * [D2:D2] �ϱ��¼���־  1:�дӽڵ��ϱ��¼� 0:�޴ӽڵ��ϱ��¼�
     * [D3:D3] ����
     * [D7:D4] �������
     */
	//CJJ-HPLC-20181203:��������д���ˮ����ȴ�ˮ����������ˣ�����λ·��ѧϰ״̬
		#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)
		do
		{
			HI_U16 usMeterAllNum = mrsSrvArchivesMeterNum();
			for(HI_U16 i = 0; i < usMeterAllNum; i++)
	    {
        MRS_ARCHIVES_NODE_STRU *pstNode = mrsSrvArchivesQuery(i);
        if (pstNode == HI_NULL)
        {
            break;
        }

        if(pstNode->ucProtocol > METER_PROTO_SINGLEWAY_WATERMETER)
        {
					if(WATER_METER_COMPLETE_NET == getWaterMeterNetStatue())
					{
						route_flag = 0x01;
					}
					else
					{
						route_flag = 0x00;
					}
					break;
				}
			}
		}
		while(0);
		#endif
		//CJJ-HPLC-20181203
    pBuffer[0] = (HI_U8)(pstEvtCtx->bACDFlg << 2 | ucWorkFlag << 1 | route_flag);

    (hi_void)memcpy_s(&pBuffer[1], sizeof(pBuffer) - 1, &usMeterNum, 2);  //�ӽڵ�������
    (hi_void)memcpy_s(&pBuffer[3], sizeof(pBuffer) - 3, &pstCcoCtx->stReadMeterModel.usSuccNum, 2);  //�ѳ��ӽڵ�����

    /*
     * ��������
     * [D0:D0] ����״̬      1:ѧϰ 0:����
     * [D1:D1] ע������״̬  1:���� 0:������
     * [D3:D2] ����״̬      00:�ֳ� 01:�ѱ� 10:���� 11:���� �����ֶ���
     * [D4:D4] ̨��ʶ��״̬  1:ʶ���� 0:ʶ����� �����ֶ���
     * [D7:D4] ���� �����ֶ���
     * [D2:D2] �¼��ϱ���־  1:���� 0:������ ��������
     * [D3:D3] ̨��ʶ��״̬  1:���� 0:������ ��������
     * [D5:D4] ���� ��������
     * [D7:D6] ��ǰ״̬      00:�ֳ� 01:�ѱ� 10:���� 11:���� ��������
     */
    pBuffer[7] = (HI_U8)(0x02);
    if (mrs_check_area_code(APP_AREA_CODE_BEIJING) == HI_TRUE) {
        pBuffer[7] = (hi_u8)(pBuffer[7] | (ucWorkStatus << 2) | (ucTfFlag << 4));
    } else {
        pBuffer[7] = (hi_u8)(pBuffer[7] | (pstEvtCtx->ucReportEnable << 2) | (ucTfFlag << 3) | (ucWorkStatus << 6));
    }

    pBuffer[13] = 0x08;
    pBuffer[14] = 0x08;
    pBuffer[15] = 0x08;

    return mrs3762_EasyEnc((HI_U8 *)pBuffer, sizeof(pBuffer), param);
}

HI_U32 mrs3762_AFN10_F5(HI_INOUT HI_VOID * param)
{
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_U32 ret = HI_ERR_SUCCESS;

    HI_U16 usContentLen = pstAfnParam->usContentLen;
    HI_U8 * pucContent = pstAfnParam->pContent;

    do
    {
        HI_U8 ucCount;
        HI_U8* pstBuf = HI_NULL;
        HI_U8 cnt= 0;
        HI_U8 ucSize = sizeof(MR_NODE_METER_ADD_STRU);
        HI_U16 i;
        HI_U16 usBufLen = 0;
        HI_U16 usIndex;

        if (usContentLen != 5)
        {
            ret = HI_ERR_BAD_DATA;
            break;
        }

        ucCount = *(pucContent + 4);
        // �ӽڵ���������������ƣ�����32����
        if (ucCount > MRS_3762_F10_NODE_MAX_NUM)
        {
            ucCount = MRS_3762_F10_NODE_MAX_NUM;
        }

        (hi_void)memcpy_s(&usIndex, sizeof(usIndex), pucContent + 2, sizeof(usIndex));

        usBufLen = 3 + ucCount * ucSize; // sizeof(MR_NODE_METER_ADD_STRU)
        pstBuf = mrsToolsMalloc(usBufLen);
        if (HI_NULL == pstBuf)
        {
            ret = HI_ERR_NOT_ENOUGH_MEMORY;
            break;
        }

        (hi_void)memset_s(pstBuf, usBufLen, 0, usBufLen);

        if (MRS_CCO_SEQ_START_FROM_1 == pstCcoCtx->ucSeqControl)
        {
            usIndex--;
        }

        for (i = usIndex; i < pstCcoCtx->stReadFail.usReadFailNum && cnt < ucCount; i++, cnt++)
        {
            (hi_void)memcpy_s(pstBuf + 3 + cnt * ucSize, usBufLen - 3 - cnt * ucSize, &pstCcoCtx->stReadFail.pReportedNodeInf[i], ucSize);
        }

        (hi_void)memcpy_s(pstBuf, usBufLen, &pstCcoCtx->stReadFail.usReadFailNum, 2);
        *(pstBuf + 2) = cnt;

        usBufLen = 3 + cnt * ucSize;
        ret = mrs3762_EasyEnc((HI_U8 *)pstBuf, usBufLen, param);

        mrsToolsFree(pstBuf);
    } while (0);

    return ret;
}

HI_U32 mrs3762_AFN10_F6(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();

    HI_U8 * pucContent = pstAfnParam->pContent;
    HI_U16 usContentLen = pstAfnParam->usContentLen;

    do
    {
        HI_U16 usIndex;
        HI_U8 ucCount;
        HI_U8* pstBuf = HI_NULL;
        HI_U16 usBufLen = 0;
        HI_U16 i;
        HI_U8 cnt= 0;
        HI_U8 ucSize = sizeof(MR_NODE_METER_ADD_STRU);

        if (usContentLen != 5)
        {
            ret = HI_ERR_BAD_DATA;
            break;
        }

        ucCount = *(pucContent + 4);
        if (ucCount > MRS_3762_F10_NODE_MAX_NUM)
        {
            ucCount = MRS_3762_F10_NODE_MAX_NUM;
        }

        (hi_void)memcpy_s(&usIndex, sizeof(usIndex), pucContent + 2, sizeof(usIndex));

        usBufLen = 3 + ucCount * ucSize; // sizeof(MR_NODE_METER_ADD_STRU)
        pstBuf = mrsToolsMalloc(usBufLen);
        if (HI_NULL == pstBuf)
        {
            ret = HI_ERR_NOT_ENOUGH_MEMORY;
            break;
        }

        (hi_void)memset_s(pstBuf, usBufLen, 0, usBufLen);

        if (MRS_CCO_SEQ_START_FROM_1 == pstCcoCtx->ucSeqControl)
        {
            usIndex--;
        }

        for (i = usIndex; i < pstCcoCtx->stAutoSearchModel.usReportedNum && cnt < ucCount; i++, cnt++)
        {
            (hi_void)memcpy_s(pstBuf + 3 + cnt * ucSize, usBufLen - 3 - cnt * ucSize, &(pstCcoCtx->stAutoSearchModel.pReportedNodeInf[i]), ucSize);
        }

        (hi_void)memcpy_s(pstBuf, usBufLen, &pstCcoCtx->stAutoSearchModel.usReportedNum, 2);
        *(pstBuf + 2) = cnt;

        usBufLen = 3 + cnt * ucSize;
        ret = mrs3762_EasyEnc((HI_U8 *)pstBuf, usBufLen, param);

        mrsToolsFree(pstBuf);
    } while (0);

    return ret;
}

//��ѯ����������Ϣ
HI_U32 mrs3762_AFN10_F21(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    HI_U16 usStartSeq = 0;
    HI_U8 ucQueryNum = 0;
    HI_U8 ucReportNum = 0;
    HI_U16 usDataLen = 0;
    HI_U8 *pData = HI_NULL;

    do
    {
        ret = mrsQueryTopoDownCheck(pstAfnParam, &usStartSeq, &ucQueryNum, MRS_CCO_QUERY_TOPO_INFO);
        if (HI_ERR_SUCCESS != ret)
        {
            return ret;
        }

        usDataLen = MRS_CCO_10_F21_DATA_FIX_LEN + ucQueryNum * MRS_CCO_10_F21_TOPO_INFO_LEN;
        pData = (HI_U8*)mrsToolsMalloc(usDataLen);
        if (!pData)
        {
            return HI_ERR_NOT_ENOUGH_MEMORY;
        }

        (hi_void)memset_s(pData, usDataLen, 0, usDataLen);
        ret = mrsSetTopoQueryBuf(pData, ucQueryNum, usStartSeq, &ucReportNum, MRS_CCO_QUERY_TOPO_10_F21);
    } while (0);

    if (HI_ERR_SUCCESS == ret)
    {
        HI_U16 usReplyLen = (HI_U16)(MRS_CCO_10_F21_DATA_FIX_LEN +
                            ucReportNum * MRS_CCO_10_F21_TOPO_INFO_LEN);

        mrs3762_EasyEnc(pData,usReplyLen,param);
    }

    mrsToolsFree(pData);

    return ret;
}

HI_U32 mrsQueryTopoDownCheck(AFN_FN_UL_PARAM *pstAfnParam, HI_U16 *pusStartSeq, HI_U8 *pucQueryNum, HI_U8 ucFrom)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 * pContent = HI_NULL;
    HI_U16 usContentLen = 0;
    HI_PRV HI_BOOL bQueryStartFlag = HI_FALSE;

    pContent = pstAfnParam->pContent;
    usContentLen = pstAfnParam->usContentLen;

    do
    {
        /* ���ݵ�Ԫ��ʶ(2�ֽ�)+���ݵ�Ԫ(3�ֽ�) */
        if (usContentLen != 5)
        {
            ret = HI_ERR_BAD_DATA;
            break;
        }

        (hi_void)memcpy_s(pusStartSeq, sizeof(HI_U16), pContent + 2, sizeof(HI_U16));
        if (!(*pusStartSeq))
        {
            ret = HI_ERR_BAD_DATA;
            break;
        }

        *pucQueryNum = *(pContent + 4);
        if (*pucQueryNum > MRS_CCO_QUERY_NODE_NUM_MAX)
        {
            *pucQueryNum = MRS_CCO_QUERY_NODE_NUM_MAX;
        }

        if ((HI_FALSE == bQueryStartFlag) && (MRS_CCO_START_SEQ_ONE != *pusStartSeq))
        {
            ret = HI_ERR_BAD_DATA;
            break;
        }

        if (MRS_CCO_START_SEQ_ONE == *pusStartSeq)
        {
            HI_MAC_CONFIG_INFO_S mac_inf = {0};
            MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();

            bQueryStartFlag = HI_TRUE;

            if (ucFrom & MRS_CCO_QUERY_STA_ID_INFO)
            {
                ret = HI_MDM_QueryStaID(pstCcoCtx->pstStaIdInfo);
            }

            if (ucFrom & MRS_CCO_QUERY_TOPO_INFO)
            {
                mac_inf.pTopo = pstCcoCtx->pTopoIntegerInfo;
                ret = HI_MDM_QueryInfo(&mac_inf);  // ��ȡmac���˱���Ϣ
            }
        }

    } while (0);

    return ret;
}

//��ѯ������Ϣ
HI_U32 mrs3762_AFN10_F31(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    HI_U16 usStartSeq = 0;
    HI_U8 ucQueryNum = 0;
    HI_U8 ucReportNum = 0;
    HI_U16 usDataLen = 0;
    HI_U8 *pData = HI_NULL;

    do
    {
        ret = mrsQueryTopoDownCheck(pstAfnParam, &usStartSeq, &ucQueryNum, MRS_CCO_QUERY_TOPO_INFO);
        if (HI_ERR_SUCCESS != ret)
        {
            return ret;
        }

        usDataLen = MRS_CCO_10_F31_DATA_FIX_LEN + ucQueryNum * MRS_CCO_10_F31_PHASE_INFO_LEN;
        pData = (HI_U8*)mrsToolsMalloc(usDataLen);
        if (!pData)
        {
            return HI_ERR_NOT_ENOUGH_MEMORY;
        }

        (hi_void)memset_s(pData, usDataLen, 0, usDataLen);
        ret = mrsSetQueryInfoBuf(pData, ucQueryNum, usStartSeq, &ucReportNum, MRS_CCO_QUERY_TOPO_10_F31);
    } while (0);

    if (HI_ERR_SUCCESS == ret)
    {
        HI_U16 usReplyLen = (HI_U16)(MRS_CCO_10_F31_DATA_FIX_LEN +
                            ucReportNum * MRS_CCO_10_F31_PHASE_INFO_LEN);

        mrs3762_EasyEnc(pData,usReplyLen,param);
    }

    mrsToolsFree(pData);

    return ret;
}

HI_U32 mrsSetTopoQueryBuf(HI_U8* pData, HI_U8 ucQueryNum, HI_U16 usStartSeq, HI_U8* pucReportNum, HI_U8 ucFrom)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_MAC_NETWORK_TOPO_S *pstTopoInf = HI_NULL;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_U16 usOffset = 0;
    HI_U8 ucReportNum = 0;
    HI_U16 usIndex = 0;
    HI_U16 usTopoNum = 0;
    HI_U8 pAddr[HI_PLC_MAC_ADDR_LEN] = {0};
    HI_U8 ucRole = 0;

    pstTopoInf = (HI_MAC_NETWORK_TOPO_S*)pstCcoCtx->pTopoIntegerInfo;
		//CJJ-HPLC-20182107:���˽ڵ���������˫��ˮ��
		#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)
		HI_U16 usWaterMeterResultNum = 0;

		if (MRS_CCO_QUERY_TOPO_10_F21 == ucFrom)
		{
			usWaterMeterResultNum = pstCcoCtx->stWaterMeterSearchModel.usWaterMeterResultNum;
		}
		usTopoNum = (HI_U16)pstTopoInf->num + usWaterMeterResultNum;
		#else
		usTopoNum = (HI_U16)pstTopoInf->num;
		#endif
		//CJJ-HPLC-20182107
    (hi_void)memcpy_s(pData, sizeof(HI_U16), &usTopoNum, sizeof(HI_U16));
    usOffset += sizeof(HI_U16) + sizeof(usStartSeq) + sizeof(ucReportNum);

    for (usIndex = usStartSeq; usIndex < usStartSeq + ucQueryNum; usIndex++)
    {
        if (pstTopoInf->num < usIndex)
        {
            break;
        }
//#if defined(PRODUCT_CFG_JIANGXI) || defined(PRODUCT_CFG_NINGXIA) || defined(PRODUCT_CFG_JIBEI)
		if (1 == usIndex)
        {
            (hi_void)memcpy_s(pAddr, sizeof(pAddr), pstCcoCtx->ucMainNodeAddr, sizeof(pAddr));
			mrsHexInvert(pAddr, sizeof(pAddr));
        }
		else
//#endif
		{
	        (hi_void)memcpy_s(pAddr, sizeof(pAddr), pstTopoInf->entry[usIndex-1].mac, HI_PLC_MAC_ADDR_LEN);
	        mrsHexInvert(pAddr, sizeof(pAddr));
	        MRS_TOOLS_FE_TO_00(pAddr[0]);
		}
        (hi_void)memcpy_s(pData + usOffset, HI_PLC_MAC_ADDR_LEN, pAddr, HI_PLC_MAC_ADDR_LEN);
        usOffset += HI_PLC_MAC_ADDR_LEN;

        if (MRS_CCO_QUERY_TOPO_10_F228 == ucFrom)
        {
            HI_U8 pucDeviceAddr[HI_METER_ADDR_LEN] = {0};

            ulRet = mrsGetDeviceAddr(pstTopoInf->entry[usIndex-1].mac,
                                     pstTopoInf->entry[usIndex-1].product_type, pucDeviceAddr);
            if (HI_ERR_SUCCESS == ulRet)
            {
                MRS_TOOLS_FE_TO_00(pucDeviceAddr[0]);
                (hi_void)memcpy_s(pData + usOffset, HI_METER_ADDR_LEN, pucDeviceAddr, HI_METER_ADDR_LEN);
            }
            else
            {
                (hi_void)memset_s(pData + usOffset, HI_METER_ADDR_LEN, 0xFF, HI_METER_ADDR_LEN);
            }

            usOffset += HI_PLC_MAC_ADDR_LEN;
        }

        (hi_void)memcpy_s(pData + usOffset, sizeof(HI_U16), &pstTopoInf->entry[usIndex-1].tei, sizeof(HI_U16));
        usOffset += sizeof(HI_U16);
        (hi_void)memcpy_s(pData + usOffset, sizeof(HI_U16), &pstTopoInf->entry[usIndex-1].proxy_tei, sizeof(HI_U16));
        usOffset += sizeof(HI_U16);
        ucRole = mrsGetNodeRole(pstTopoInf->entry[usIndex-1].role);
        *(pData + usOffset) = (HI_U8)((ucRole << MRS_CCO_TOPO_ROLE_OFFSET) + pstTopoInf->entry[usIndex-1].level);
        usOffset += sizeof(HI_U8);

        if (MRS_CCO_QUERY_TOPO_10_F228 == ucFrom)
        {
            *(pData + usOffset) = mrsGetNodeType(pstTopoInf->entry[usIndex-1].product_type);
            usOffset += sizeof(HI_U8);
        }

        ucReportNum++;
    }

		//CJJ-HPLC-20181207:���������ˮ�������ͼ
		#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)
		do
		{
			HI_U16 usWaterIndex = 0,usWaterStartSeq = 0,ucWaterQueryNum = 0;
			HI_PBYTE ucWaterMeter = NULL,ucaStaMac = NULL;
			//HI_U16 usMeterAllNum = mrsSrvArchivesMeterNum();//weichao
		
			if((MRS_CCO_QUERY_TOPO_10_F21 != ucFrom) || (0 == usWaterMeterResultNum))
			{
				break;
			}
			if((ucReportNum >= ucQueryNum) || ((usStartSeq + ucReportNum) <= pstTopoInf->num))
			{
				break;
			}
			ucWaterQueryNum = ucQueryNum - ucReportNum;
			usWaterStartSeq = (usStartSeq + ucReportNum) - pstTopoInf->num;

			for(usWaterIndex = usWaterStartSeq;usWaterIndex < usWaterStartSeq + ucWaterQueryNum;usWaterIndex++)
			{
				if(usWaterIndex > usWaterMeterResultNum )
				{
					break;
				} 	

				if(ucReportNum >= ucQueryNum)
				{
					break;
				}
				
				ucaStaMac = pstCcoCtx->stWaterMeterSearchModel.stWaterMeterResult[usWaterIndex-1].ucaStaMac[0]; 		
				ucWaterMeter = pstCcoCtx->stWaterMeterSearchModel.stWaterMeterResult[usWaterIndex-1].ucaWaterMac;
				for (usIndex = 1; usIndex  <= pstTopoInf->num; usIndex++)
				{
					mrsToolsMemcpy_s(pAddr, sizeof(pAddr), pstTopoInf->entry[usIndex-1].mac, HI_PLC_MAC_ADDR_LEN);
					if(0 == mrsToolsMemcmp(ucaStaMac,pAddr, HI_PLC_MAC_ADDR_LEN))
					{
						break;
					}
				}

				if(usIndex <= pstTopoInf->num)
				{
					mrsToolsMemcpy_s(pData + usOffset, HI_PLC_MAC_ADDR_LEN, ucWaterMeter, HI_PLC_MAC_ADDR_LEN);
					usOffset += HI_PLC_MAC_ADDR_LEN;
					mrsToolsMemset_s(pData + usOffset, sizeof(HI_U16), 0x00, sizeof(HI_U16));
					usOffset += sizeof(HI_U16);
					mrsToolsMemcpy_s(pData + usOffset, sizeof(HI_U16), &pstTopoInf->entry[usIndex-1].tei, sizeof(HI_U16));
					usOffset += sizeof(HI_U16);
					*(pData + usOffset) = (HI_U8)((0x01 << MRS_CCO_TOPO_ROLE_OFFSET) + (pstTopoInf->entry[usIndex-1].level + 1));
					usOffset += sizeof(HI_U8);

					ucReportNum++;
				}
			}
		}
		while(0);
		#endif
		//CJJ-HPLC-20181207
    *(pData + MRS_CCO_TOPO_REPORT_NUM_OFFSET) = ucReportNum;

    (hi_void)memcpy_s(pData + MRS_CCO_TOPO_START_SEQ_OFFSET, sizeof(usStartSeq), &usStartSeq, sizeof(usStartSeq));

    *pucReportNum = ucReportNum;

    return HI_ERR_SUCCESS;
}

HI_U8 mrsGetPhaseFromTopo(HI_U8 ucPhaseState, HI_U8 ucPhaseResult)
{
    HI_U8 ucPhase = MRS_PHASE_INVALID;

    if (HI_MAC_PHASE_IDENTIFY_STATE_OVER != ucPhaseState)
    {
        return ucPhase;
    }

    switch (ucPhaseResult)
    {
    case PHASE_ALL:
        ucPhase = MRS_PHASE_ALL;
        break;

    case PHASE_A:
        ucPhase = MRS_PHASE_A;
        break;

    case PHASE_B:
        ucPhase = MRS_PHASE_B;
        break;

    case PHASE_C:
        ucPhase = MRS_PHASE_C;
        break;

    case INVALID_PHASE:
    default:
        ucPhase = MRS_PHASE_INVALID;
        break;
    }

    return ucPhase;
}

HI_U8 mrsGetPhaseResultFromPhase(HI_U8 phase)
{
    switch(phase)
    {
        case MRS_PHASE_ALL:
            return PHASE_ALL;

        case MRS_PHASE_A:
            return PHASE_A;

        case MRS_PHASE_B:
            return PHASE_B;

        case MRS_PHASE_C:
            return PHASE_C;

        default:
            return INVALID_PHASE;
    }
}


HI_U8 mrsGetPhaseState(HI_U8 ucPhaseResult, HI_U8 ucPhaseResult_b, HI_U8 ucPhaseResult_c)
{
   HI_U8 ucPhase = 0;
 
   if (ucPhaseResult != PHASE_ALL && ucPhaseResult != INVALID_PHASE)
   {
       ucPhase = 1;
   }
   if (ucPhaseResult_b != PHASE_ALL && ucPhaseResult_b != INVALID_PHASE)
   {
       ucPhase |= 1<<1;
   }
   if (ucPhaseResult_c != PHASE_ALL && ucPhaseResult_c != INVALID_PHASE)
   {
       ucPhase |= 1<<2;
   }
   return ucPhase & 0x7;
}

//pucType �Ƿ�����������𷴽�
HI_U8 mrsGetThreePhaseType(HI_MAC_NETWORK_TOPO_ENTRY_S *pstEntry, HI_U8 *pucType)
{
   HI_U8 ucPhaseType = 0;
   HI_U8 ucPhase = 0;

   if (pstEntry->pn_state == HI_MAC_PN_IDENTIFY_STATE_OVER && 
       (pstEntry->pn_result == PN_CONNECTION_ERROR || pstEntry->pn_result_b == PN_CONNECTION_ERROR || pstEntry->pn_result_c == PN_CONNECTION_ERROR)) 
   {
      ucPhaseType = 0x6;
      *pucType = 1;//����
      return ucPhaseType;
   }

   ucPhase = mrsGetPhaseState(pstEntry->phase_result, pstEntry->phase_result_b, pstEntry->phase_result_c);

   if ((ucPhase & 0x7) != 0x7)
   {
      ucPhaseType = 0;
      *pucType = 1; //����
      return ucPhaseType;
   }

    *pucType = 1; //������

   {
      if (pstEntry->phase_result == PHASE_A && pstEntry->phase_result_b == PHASE_B && pstEntry->phase_result_c == PHASE_C) 
      {
         ucPhaseType = 0;
         *pucType = 0;//��������
      }
      else if (pstEntry->phase_result == PHASE_A && pstEntry->phase_result_b == PHASE_C && pstEntry->phase_result_c == PHASE_B)
      {
         ucPhaseType = 1;        
      }
      else if (pstEntry->phase_result == PHASE_B && pstEntry->phase_result_b == PHASE_C && pstEntry->phase_result_c == PHASE_A)
      {
         ucPhaseType = 3;
      }
      else if (pstEntry->phase_result == PHASE_B && pstEntry->phase_result_b == PHASE_A && pstEntry->phase_result_c == PHASE_C)
      {
         ucPhaseType = 2;
      }
      else if (pstEntry->phase_result == PHASE_C && pstEntry->phase_result_b == PHASE_A && pstEntry->phase_result_c == PHASE_B)
      {
         ucPhaseType = 4;
      }
      else if (pstEntry->phase_result == PHASE_C && pstEntry->phase_result_b == PHASE_B && pstEntry->phase_result_c == PHASE_A)
      {
         ucPhaseType = 5;
      }
      else ucPhaseType = 0;//������δ֪
   }

   return ucPhaseType;
}




HI_U32 mrsSetQueryInfoBuf(HI_U8* pData, HI_U8 ucQueryNum, HI_U16 usStartSeq, HI_U8* pucReportNum, HI_U8 ucFrom)
{
    HI_MAC_NETWORK_TOPO_S *pstTopoInf = HI_NULL;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_U8 ucReportNum = 0;
    HI_U16 usTopoNum = 0;
    HI_U8 pAddr[HI_PLC_MAC_ADDR_LEN] = {0};
    HI_U16 usIndex = 0;
    HI_U16 usOffset = 0;
    HI_U8 Phase = 0;
    HI_U8 ucError = 0;


    pstTopoInf = (HI_MAC_NETWORK_TOPO_S*)pstCcoCtx->pTopoIntegerInfo;
    usTopoNum = (HI_U16)pstTopoInf->num;
    (hi_void)memcpy_s(pData, sizeof(HI_U16), &usTopoNum, sizeof(HI_U16));
    usOffset += sizeof(HI_U16);
    (hi_void)memcpy_s(pData + usOffset, sizeof(usStartSeq), &usStartSeq, sizeof(usStartSeq));
    usOffset += sizeof(usStartSeq) + sizeof(ucReportNum);

    for (usIndex = usStartSeq; usIndex < usStartSeq + ucQueryNum; usIndex++)
    {
        if (pstTopoInf->num < usIndex)
        {
            break;
        }
//	HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1003, HI_DIAG_MT("topo entry i"), &(pstTopoInf->entry[usIndex-1]), sizeof(HI_MAC_NETWORK_TOPO_ENTRY_S));
		if (1 == usIndex)
        {
            (hi_void)memcpy_s(pAddr, sizeof(pAddr), pstCcoCtx->ucMainNodeAddr, sizeof(pAddr));
			mrsHexInvert(pAddr, sizeof(pAddr));
        }
		else
		{
        	(hi_void)memcpy_s(pAddr, sizeof(pAddr), pstTopoInf->entry[usIndex-1].mac, HI_PLC_MAC_ADDR_LEN);
        	mrsHexInvert(pAddr, sizeof(pAddr));
        	MRS_TOOLS_FE_TO_00(pAddr[0]);
		}
        (hi_void)memcpy_s(pData + usOffset, HI_PLC_MAC_ADDR_LEN, pAddr, HI_PLC_MAC_ADDR_LEN);
        usOffset += HI_PLC_MAC_ADDR_LEN;

        if (MRS_CCO_QUERY_TOPO_10_F31 == ucFrom)
        {
            if (HI_ND_TYPE_THREE_STA == pstTopoInf->entry[usIndex - 1].product_type)
            {
               if (HI_MAC_PHASE_IDENTIFY_STATE_OVER != pstTopoInf->entry[usIndex-1].phase_state)
               {
                    *(pData + usOffset) = MRS_PHASE_INVALID;
               }
               else
               {
                  //���A\B\C
                  Phase = mrsGetPhaseState(pstTopoInf->entry[usIndex - 1].phase_result,
                                           pstTopoInf->entry[usIndex - 1].phase_result_b,
                                           pstTopoInf->entry[usIndex - 1].phase_result_c);
                  *(pData + usOffset) = Phase;
               }
            }
            //CCO ���ȫ1
            else if(HI_ND_TYPE_CCO == pstTopoInf->entry[usIndex - 1].product_type) 
               *(pData + usOffset) = mrsGetPhaseFromTopo(pstTopoInf->entry[usIndex-1].phase_state, PHASE_ALL);

			else
               *(pData + usOffset) = mrsGetPhaseFromTopo(pstTopoInf->entry[usIndex-1].phase_state, pstTopoInf->entry[usIndex-1].phase_result & MRS_PHASE_MASK);
            
            if (HI_ND_TYPE_THREE_STA == pstTopoInf->entry[usIndex - 1].product_type)
            {
               Phase = mrsGetThreePhaseType(&pstTopoInf->entry[usIndex - 1], &ucError); 
               //�������ࡢ����������߷���
               if (ucError == 1)
               {
                  *(pData + usOffset) |= (1 << MRS_CCO_10_F31_PN_RESULT_OFFSET);
               }
               *(pData + usOffset) |= (1 << MRS_CCO_10_F31_METER_TYPE_OFFSET);
                //��ʱֻ���ǻ���HPLCӦ���ֲ�V2.0,�����ȫ���ǲ�Ʒ��̬Ϊ7
               *(pData + usOffset) |= ((Phase & 0x7) << MRS_CCO_10_F31_T_PHASE_TYPE_OFFSET);
            }					
            else
            {
              if (HI_MAC_PN_IDENTIFY_STATE_OVER == pstTopoInf->entry[usIndex - 1].pn_state && PN_CONNECTION_ERROR == pstTopoInf->entry[usIndex - 1].pn_result)
              {
                *(pData + usOffset) |= (1 << MRS_CCO_10_F31_PN_RESULT_OFFSET);
              }
            }
						
            usOffset += sizeof(HI_U16);
        }
        else if (MRS_CCO_QUERY_TOPO_10_F32 == ucFrom)
        {
            if ((usTopoNum > 1) && (1 == usIndex))
            {
                *(pData + usOffset) = MRS_CCO_10_F32_CCO_SUCC_RATE;
                *(pData + usOffset + 1) = MRS_CCO_10_F32_CCO_SUCC_RATE;
                usOffset += 2;
            }
            else
            {
                *(pData + usOffset) = pstTopoInf->entry[usIndex-1].up_comm_rate;
                *(pData + usOffset + 1) = pstTopoInf->entry[usIndex-1].down_comm_rate;
                usOffset += 2;
            }
        }

        ucReportNum++;
    }

    *(pData + MRS_CCO_TOPO_REPORT_NUM_OFFSET) = ucReportNum;
    *pucReportNum = ucReportNum;

    return HI_ERR_SUCCESS;
}


HI_U8 mrsGetNodeRole(HI_U8 ucTopoRole)
{
    switch (ucTopoRole)
    {
        case HI_ND_TYPE_CCO:
            return MRS_ND_TOPO_ROLE_CCO;
        case HI_ND_TYPE_STA:
            return MRS_ND_TOPO_ROLE_STA;
        case HI_ND_TYPE_RELAY:
            return MRS_ND_TOPO_ROLE_PROXY;
        default:
            break;
    }

    return MRS_ND_TOPO_ROLE_INVALID;
}

HI_U8 mrsGetNodeType(HI_U8 ucTopoType)
{
    switch (ucTopoType)
    {
        case HI_ND_TYPE_CCO:
            return MRS_ND_TOPO_NODE_TYPE_CCO;
        case HI_ND_TYPE_STA:
        case HI_ND_TYPE_THREE_STA:
            return MRS_ND_TOPO_NODE_TYPE_STA;
        case HI_ND_TYPE_CLTI:
            return MRS_ND_TOPO_NODE_TYPE_CLTI;
        case HI_ND_TYPE_CLTII:
            return MRS_ND_TOPO_NODE_TYPE_CLTII;
        default:
            break;
    }

    return MRS_ND_TOPO_NODE_TYPE_INVALID;
}

//��ѯ�ŵ�Ʒ����Ϣ
HI_U32 mrs3762_AFN10_F32(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    HI_U16 usStartSeq = 0;
    HI_U8 ucQueryNum = 0;
    HI_U8 ucReportNum = 0;
    HI_U16 usDataLen = 0;
    HI_U8 *pData = HI_NULL;

    do
    {
        ret = mrsQueryTopoDownCheck(pstAfnParam, &usStartSeq, &ucQueryNum, MRS_CCO_QUERY_TOPO_STA_ID_INFO);
        if (HI_ERR_SUCCESS != ret)
        {
            return ret;
        }

        usDataLen = MRS_CCO_10_F32_DATA_FIX_LEN + ucQueryNum * MRS_CCO_10_F32_CHANNEL_INFO_LEN;
        pData = (HI_U8*)mrsToolsMalloc(usDataLen);
        if (!pData)
        {
            return HI_ERR_NOT_ENOUGH_MEMORY;
        }

        (hi_void)memset_s(pData, usDataLen, 0, usDataLen);
        ret = mrsSetQueryInfoBuf(pData, ucQueryNum, usStartSeq, &ucReportNum, MRS_CCO_QUERY_TOPO_10_F32);
    } while (0);

    if (HI_ERR_SUCCESS == ret)
    {
        HI_U16 usReplyLen = (HI_U16)(MRS_CCO_10_F32_DATA_FIX_LEN +
                            ucReportNum * MRS_CCO_10_F32_CHANNEL_INFO_LEN);

        mrs3762_EasyEnc(pData,usReplyLen,param);
    }

    mrsToolsFree(pData);

    return ret;
}

//��ѯ�ڵ�ID��Ϣ
HI_U32 mrs3762_AFN10_F33(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    HI_U16 usStartSeq = 0;
    HI_U8 ucQueryNum = 0;
    HI_U8 ucReportNum = 0;
    HI_U16 usDataLen = 0;
    HI_U8 *pData = HI_NULL;

    do
    {
        ret = mrsQueryTopoDownCheck(pstAfnParam, &usStartSeq, &ucQueryNum, MRS_CCO_QUERY_TOPO_STA_ID_INFO);
        if (HI_ERR_SUCCESS != ret)
        {
            return ret;
        }

        usDataLen = MRS_CCO_10_F33_DATA_FIX_LEN + ucQueryNum * MRS_CCO_10_F33_ID_INFO_LEN;
        pData = (HI_U8*)mrsToolsMalloc(usDataLen);
        if (!pData)
        {
            return HI_ERR_NOT_ENOUGH_MEMORY;
        }

        (hi_void)memset_s(pData, usDataLen, 0, usDataLen);
        ret = mrsSetQueryStaIdBuf(pData, ucQueryNum, usStartSeq, &ucReportNum);
    } while (0);

    if (HI_ERR_SUCCESS == ret)
    {
        HI_U16 usReplyLen = (HI_U16)(MRS_CCO_10_F33_DATA_FIX_LEN +
                            ucReportNum * MRS_CCO_10_F33_ID_INFO_LEN);

        mrs3762_EasyEnc(pData,usReplyLen,param);
    }

    mrsToolsFree(pData);

    return ret;
}

HI_U32 mrsSetQueryStaIdBuf(HI_U8* pData, HI_U8 ucQueryNum, HI_U16 usStartSeq, HI_U8* pucReportNum)
{
    HI_MDM_NETWORK_STA_ID_S *pstStaIdInfo;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_U8 ucReportNum = 0;
    HI_U16 usStaNum = 0;
    HI_U8 pAddr[HI_PLC_MAC_ADDR_LEN] = {0};
    HI_U16 usIndex = 0;
    HI_U16 usOffset = 0;

    pstStaIdInfo = pstCcoCtx->pstStaIdInfo;
    usStaNum = (HI_U16)pstStaIdInfo->num;
    (hi_void)memcpy_s(pData, sizeof(HI_U16), &usStaNum, sizeof(HI_U16));
    usOffset += sizeof(HI_U16);
    (hi_void)memcpy_s(pData + usOffset, sizeof(usStartSeq), &usStartSeq, sizeof(usStartSeq));
    usOffset += sizeof(usStartSeq) + sizeof(ucReportNum);

    for (usIndex = usStartSeq; usIndex < usStartSeq + ucQueryNum; usIndex++)
    {
        if (usStaNum < usIndex)
        {
            break;
        }
		if (1 == usIndex)
        {
            (hi_void)memcpy_s(pAddr, sizeof(pAddr), pstCcoCtx->ucMainNodeAddr, sizeof(pAddr));
			mrsHexInvert(pAddr, sizeof(pAddr));
        }
		else
		{
        (hi_void)memcpy_s(pAddr, sizeof(pAddr), pstStaIdInfo->entry[usIndex-1].aucMacAddr, HI_PLC_MAC_ADDR_LEN);
        mrsHexInvert(pAddr, sizeof(pAddr));
        MRS_TOOLS_FE_TO_00(pAddr[0]);
		}
        (hi_void)memcpy_s(pData + usOffset, HI_PLC_MAC_ADDR_LEN, pAddr, HI_PLC_MAC_ADDR_LEN);
        usOffset += HI_PLC_MAC_ADDR_LEN;
        (hi_void)memcpy_s(pData + usOffset, MRS_CCO_STA_CHIP_ID_LEN, pstCcoCtx->pstStaIdInfo->entry[usIndex-1].aucStaId, MRS_CCO_STA_CHIP_ID_LEN);
        usOffset += MRS_CCO_STA_CHIP_ID_LEN;

        ucReportNum++;
    }

    *(pData + MRS_CCO_TOPO_REPORT_NUM_OFFSET) = ucReportNum;
    *pucReportNum = ucReportNum;

    return HI_ERR_SUCCESS;
}
#endif

//������1376.2֡����cco��sta��ȡ�豸����ID���ȴ�staӦ��plc֡��ʱ
hi_void mrs_plc_detect_id_timeout(hi_void)
{
    if (g_read_id_frame != HI_NULL) {
        hi_u32 ret = HI_ERR_SUCCESS;
        PROTO376_2_FRAME_PRE_STRU frame;
        hi_pbyte buffer = HI_NULL;
        hi_u16 buf_len = 0;
        AFN_00_F2 deny = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;


        (hi_void)memset_s(&frame, sizeof(frame), 0, sizeof(frame));
        frame.ucSeq = g_read_id_frame->ucSeq;
        frame.stAfnData.ucAFN = 0x00;
        frame.stAfnData.ucDT[0] = 0x02;
        frame.stAfnData.ucDT[1] = 0x00;
        frame.stAfnData.pData = &deny;
        frame.stAfnData.usDataLen = 1;
        ret = mrsCreate3762Frame(&frame, &buffer, &buf_len, HI_NULL);
        if (ret == HI_ERR_SUCCESS) {
            MRS_SendMrData(buffer, buf_len, HI_DMS_CHL_UART_PORT_APP);
        }

        mrsToolsFree(g_read_id_frame);
    }
}

uintptr_t mrs_get_read_id_frame(hi_void)
{
    return (uintptr_t)g_read_id_frame;
}

hi_void mrs_free_read_id_frame(hi_void)
{
    mrsToolsFree(g_read_id_frame);
}

// ��ȡ�豸����ID��ϢʱccoID��Ϣ����1376.2����֡�����豸 sta��II�ɡ�I�ɴ洢֡��Ϣ
HI_U32 mrsGetDevIdCreateFrame (AFN_FN_UL_PARAM * pst_afn_param, HI_U8* data_buf, HI_U8 buf_len,
	afn_10_f40_dl_data_param *pdl_data)
{
	HI_U32 ret = HI_ERR_SUCCESS;
	PROTO376_2_FRAME_PRE_STRU *pst_frame = HI_NULL;
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    hi_u8 my_type = MRS_DEV_TYPE_CCO;
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    hi_u8 my_type = MRS_DEV_TYPE_NDM;
#endif

	pst_frame = (PROTO376_2_FRAME_PRE_STRU *)mrsToolsMalloc(sizeof(PROTO376_2_FRAME_PRE_STRU));
	if (pst_frame == HI_NULL) {
		return HI_ERR_NOT_ENOUGH_MEMORY;
	}

    (hi_void)memset_s(pst_frame, sizeof(PROTO376_2_FRAME_PRE_STRU), 0, sizeof(PROTO376_2_FRAME_PRE_STRU));
	pst_frame->ucSeq = pst_afn_param->pstFrame->strUsrDataHrd.uInfoRealm.stInfRealmDL.ucSeq;
	pst_frame->ucModuleFlag = 0;

	// ���޵�ַ����ж�
	if (pst_afn_param->pstFrame->strUsrDataHrd.uInfoRealm.stInfRealmDL.ucModuleFlag == 1) {
		pst_frame->ucModuleFlag = 1;

        (hi_void)memcpy_s(pst_frame->ucSrcAdd, HI_METER_ADDR_LEN,
            pst_afn_param->pstFrame->strUsrDataHrd.stAddrRealm.ucDestAdd, HI_METER_ADDR_LEN);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
		if (pdl_data->dev_type == MRS_DEV_TYPE_CCO) {
		    (hi_void)memcpy_s(pst_frame->ucSrcAdd, HI_METER_ADDR_LEN, mrsCcoGetContext()->ucMainNodeAddr, HI_METER_ADDR_LEN);
			mrsHexInvert(pst_frame->ucSrcAdd, sizeof(pst_frame->ucSrcAdd));
		}
#endif
	    (hi_void)memcpy_s(pst_frame->ucDstAdd, HI_METER_ADDR_LEN,
			pst_afn_param->pstFrame->strUsrDataHrd.stAddrRealm.ucSrcAdd, HI_METER_ADDR_LEN);
	}

	pst_frame->stAfnData.ucAFN   = pst_afn_param->ucAfn;
    pst_frame->stAfnData.ucDT[0] = pst_afn_param->pContent[0];
    pst_frame->stAfnData.ucDT[1] = pst_afn_param->pContent[1];

	if (pdl_data->dev_type == my_type) {
	    pst_frame->stAfnData.usDataLen = buf_len;
	    pst_frame->stAfnData.pData = data_buf;

	    ret = mrsCreate3762Frame(pst_frame, pst_afn_param->ppucOutBuffer, pst_afn_param->pusBufferLen, HI_NULL);
		mrsToolsFree(pst_frame);
	} else {
		// ��ȡ���豸 sta��II�ɡ�I�ɵ�ID��Ϣ����plc֡����֯1376.2֡���õ�֡��Ϣ
		g_read_id_frame = pst_frame;
	}

	return ret;
}

HI_U32 mrs3762_AFN10_F40(HI_INOUT HI_VOID * param)
{
	HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM *afn_param = (AFN_FN_UL_PARAM *)param;
	afn_10_f40_dl_data_param *dl_data = HI_NULL;
	afn_10_f40_ul_data_param ul_data;
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    hi_u8 my_type = MRS_DEV_TYPE_CCO;
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    hi_u8 my_type = MRS_DEV_TYPE_NDM;
#endif

    (hi_void)memset_s(&ul_data, sizeof(ul_data), 0, sizeof(ul_data));

	/* ����֡����, 2�ֽ�fn */
	if (afn_param->usContentLen != (sizeof(afn_10_f40_dl_data_param) + 2)) {
		return HI_ERR_BAD_DATA;
	}

	dl_data = (afn_10_f40_dl_data_param *)(afn_param->pContent + 2);

	/*
	 * id���Ͳ��ڻ�ȡ��Ϣ�ķ�Χ
	 * �豸���Ͳ��ڻ�ȡ��Ϣ�ķ�Χ�������ǳ�����
	 */
	if ((dl_data->id_type >= MRS_DEV_ID_MAX) ||
        (dl_data->dev_type >= MRS_DEV_TYPE_MAX) ||
        (dl_data->dev_type <= MRS_DEV_TYPE_NDM)) {
		return HI_ERR_INVALID_PARAMETER;
	}

    if (dl_data->dev_type == my_type) {
    	hi_u8 ul_data_len = 0;
        hi_u8 ip[HI_IPV4_ADDR_LEN] = {0};

        /* ��ȡоƬID */
        if (dl_data->id_type == MRS_DEV_ID_CHIP) {
            ret = mrsGetDevIdChip(ul_data.id_info, sizeof(ul_data.id_info));
				//CJJ-HPLC-20190115:оƬID����
				mrsHexInvert(ul_data.id_info, MANAGEMENT_ID_SIZE);
            ul_data.id_len = MANAGEMENT_ID_SIZE;
        } else {
            ret = mrsGetDevIdDev(ul_data.id_info, sizeof(ul_data.id_info));
				//CJJ-HPLC-20190115:оƬID����
				mrsHexInvert(ul_data.id_info, MRS_DEV_ID_SIZE);
            ul_data.id_len = MRS_DEV_ID_SIZE;
        }

        if (ret != HI_ERR_SUCCESS) {
            return ret;
        }

        ul_data.ul_param.dev_type = dl_data->dev_type;
        ul_data.ul_param.id_type = dl_data->id_type;
        (hi_void)HI_MDM_NM_GetAddr(ul_data.ul_param.node_addr, ip);
        mrsHexInvert(ul_data.ul_param.node_addr, sizeof(ul_data.ul_param.node_addr));
        ul_data_len = sizeof(ul_data.ul_param) + sizeof(ul_data.id_len) + ul_data.id_len;

        return mrsGetDevIdCreateFrame(afn_param, (hi_u8 *)&ul_data, ul_data_len, dl_data);
    } else {
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
        /* ��֯PLC֡�·� */
        MRS_PLC_FRAME_DATA_S plc;
        mrs_plc_dev_id_info_dl id_info;
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
        ndm_read_id_param read_id_param;
#endif

        /* ��һ֡����δ�꣬��������һ֡ */
        if (g_read_id_frame != HI_NULL) {
            return HI_ERR_BUSY;
        }

        ret = mrsGetDevIdCreateFrame(afn_param, HI_NULL, 0, dl_data);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
        id_info.stru_ver = MRS_CMD_STRU_VER_R1;
        id_info.stru_len = sizeof(id_info);
        id_info.dir_flag = MRS_PLC_DL_FLG;
        id_info.id_type = dl_data->id_type;
        id_info.seq = afn_param->pstFrame->strUsrDataHrd.uInfoRealm.stInfRealmDL.ucSeq;

        (hi_void)memset_s(&plc, sizeof(plc), 0, sizeof(plc));

        (hi_void)memcpy_s(plc.ucMac, sizeof(plc.ucMac), dl_data->node_addr, sizeof(dl_data->node_addr));
        mrsHexInvert(plc.ucMac, sizeof(plc.ucMac));
        plc.usId = ID_MRS_CMD_GET_DEVICE_ID_PLC;
        plc.usPayloadLen = sizeof(id_info);
        plc.pucPayload = (hi_pbyte)&id_info;

        ret = MRS_SendPlcFrame(&plc);
        if (ret == HI_ERR_SUCCESS) {
            MRS_StartTimer(MRS_CCO_TIMER_STA_PLC_READ_DEV_ID, MRS_CCO_STA_PLC_READ_DEV_ID_TIME, HI_TIMER_TYPE_ONCE);
        }
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
        (hi_void)memset_s(&read_id_param, sizeof(read_id_param), 0, sizeof(read_id_param));
/*lint -e545*/
        (hi_void)memcpy_s(read_id_param.node, sizeof(read_id_param.node), dl_data->node_addr, sizeof(dl_data->node_addr));
/*lint +e545*/
        mrsHexInvert(read_id_param.node, sizeof(read_id_param.node));
        read_id_param.id_type = dl_data->id_type;
        read_id_param.dev_type = dl_data->dev_type;
        read_id_param.seq = afn_param->pstFrame->strUsrDataHrd.uInfoRealm.stInfRealmDL.ucSeq;
        ret = mrs_ndm_detect_id_tx(&read_id_param);
#endif

        return ret;
    }
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
/* ��ѯHPLC�ز�ģ����Ϣ */
hi_u32 mrs3762_afn10_f104(HI_INOUT hi_void *param)
{
    AFN_FN_UL_PARAM *fn_param = (AFN_FN_UL_PARAM *)param;
    MRS_CCO_SRV_CTX_STRU *cco_ctx = mrsCcoGetContext();
    HI_MAC_NETWORK_TOPO_S *topo = &cco_ctx->stTopoInf;
    HI_MAC_NETWORK_TOPO_ENTRY_S *entry = HI_NULL;
    hi_pbyte payload = HI_NULL;
    hi_u16 payload_len = 0;
    hi_u16 topo_num = (HI_U16)topo->num;
    hi_u16 offset = 0;
    hi_u16 start_sn = 0;
    hi_u16 i = 0;
    hi_u8 query_cnt = 0;

    /*
     * Ӧ�ñ��Ľṹ: Dt(2 bytes) + Start(2 bytes) + Cnt(1 bytes)
     */
    if (fn_param->usContentLen != 5) {
        return HI_ERR_BAD_DATA;
    }

    start_sn = HI_MAKEU16(fn_param->pContent[2], fn_param->pContent[3]);
    query_cnt = fn_param->pContent[4];

    if (query_cnt > MRS_CCO_10_F104_NODE_QUERY_MAX) {
        query_cnt = MRS_CCO_10_F104_NODE_QUERY_MAX;
    }

    if (start_sn >= topo_num) {
        query_cnt = 0;
    } else {
        query_cnt = (hi_u8)(HI_MIN(topo_num - start_sn, query_cnt));
    }

    payload_len = MRS_CCO_10_F104_HEAD_LEN + MRS_CCO_10_F104_NODE_SIZE * query_cnt;
    payload = (hi_pbyte)mrsToolsMalloc(payload_len);
    if (payload == HI_NULL) {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(payload, payload_len, 0, payload_len);
    payload[offset++] = (hi_u8)(topo_num & 0xff);
    payload[offset++] = (hi_u8)((topo_num >> 8) & 0xff);
    payload[offset++] = query_cnt;

    for (i = start_sn; i < start_sn + query_cnt; i++) {
        entry = topo->entry + i;

        /* mac addr: ���� */
        payload[offset++] = entry->mac[5];
        payload[offset++] = entry->mac[4];
        payload[offset++] = entry->mac[3];
        payload[offset++] = entry->mac[2];
        payload[offset++] = entry->mac[1];
        payload[offset++] = entry->mac[0];

        /* ����汾�� */
        payload[offset++] = (hi_u8)(entry->sta_ver.usSwVer & 0xff);
        payload[offset++] = (hi_u8)((entry->sta_ver.usSwVer >> 8) & 0xff);

        /* ����汾����: ��-��-�� */
        mrsIntToBcd((HI_U32)entry->sta_ver.usDay, payload + offset, sizeof(hi_u8));
        offset++;
        mrsIntToBcd((HI_U32)entry->sta_ver.usMonth, payload + offset, sizeof(hi_u8));
        offset++;
        mrsIntToBcd((HI_U32)entry->sta_ver.usYear, payload + offset, sizeof(hi_u8));
        offset++;

        /* ģ�鳧�̴��� */
        payload[offset++] = (hi_byte)entry->sta_ver.szManuCode[0];
        payload[offset++] = (hi_byte)entry->sta_ver.szManuCode[1];

        /* оƬ���� */
        payload[offset++] = (hi_byte)entry->sta_ver.szChipCode[0];
        payload[offset++] = (hi_byte)entry->sta_ver.szChipCode[1];
    }

    mrs3762_EasyEnc(payload, payload_len, param);
    mrsToolsFree(payload);

    return HI_ERR_SUCCESS;
}

HI_U32 mrs3762_AFN10_F111(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//����֡�еĴ���״̬��
    HI_U8 *pBuffer = HI_NULL;
    HI_U16 usBufLen = 0;
    HI_MDM_NEIGHBOR_NET_INFO_S *pstNeighborInfo = HI_NULL;


    do{
        pstNeighborInfo = (HI_MDM_NEIGHBOR_NET_INFO_S*)mrsToolsMalloc(sizeof(HI_MDM_NEIGHBOR_NET_INFO_S));
        if (!pstNeighborInfo)
        {
            ret = HI_ERR_FAILURE;
            break;
        }

        (hi_void)memset_s(pstNeighborInfo, sizeof(HI_MDM_NEIGHBOR_NET_INFO_S), 0, sizeof(HI_MDM_NEIGHBOR_NET_INFO_S));
        ret = HI_MDM_GetNeighborNetInfo(pstNeighborInfo);
        if (HI_ERR_SUCCESS != ret)
        {
            ret = HI_ERR_BAD_DATA;
            break;
        }

        usBufLen = (HI_U16)(MRS_CCO_10_F111_DATA_FIX_LEN + pstNeighborInfo->usNum * MRS_CCO_10_F111_NID_INFO_LEN);
        pBuffer = (HI_U8*)mrsToolsMalloc(usBufLen);
        if (!pBuffer)
        {
            ret = HI_ERR_FAILURE;
            break;
        }

        (hi_void)memset_s(pBuffer, usBufLen, 0, usBufLen);
        ret = mrsSetNetInfoBuf(pBuffer, usBufLen, pstNeighborInfo);
    } while (0);

    if (HI_ERR_SUCCESS == ret)
    {
        mrs3762_EasyEnc(pBuffer, usBufLen, param);
    }
    else
    {
        ret = mrs3762ProtoDenyFrame(&stDenyData,pstAfnParam, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
    }

    mrsToolsFree(pstNeighborInfo);
    mrsToolsFree(pBuffer);

    return ret;
}

HI_U32 mrsSetNetInfoBuf(HI_U8 *pBuffer, HI_U16 usBufLen, HI_MDM_NEIGHBOR_NET_INFO_S *pstNeighborInfo)
{
    HI_U16 usOffset = 0;
    HI_U8 aucNodeAddress[MRS_MAIN_NODE_ADDR_LEN] = {0};
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_U8 ucIndex = 0;
    HI_U32 ulNid = 0;

    *(pBuffer + usOffset) = (HI_U8)(pstNeighborInfo->usNum);
    usOffset++;

    ulNid = HI_MDM_GetWholeNetNid();
    (hi_void)memcpy_s(pBuffer + usOffset, MRS_CCO_NETWORK_NID_LEN, &ulNid, MRS_CCO_NETWORK_NID_LEN);
    usOffset += MRS_CCO_NETWORK_NID_LEN;

    (hi_void)memcpy_s(aucNodeAddress, MRS_MAIN_NODE_ADDR_LEN, pstCcoCtx->ucMainNodeAddr, MRS_MAIN_NODE_ADDR_LEN);
    mrsHexInvert(aucNodeAddress, MRS_MAIN_NODE_ADDR_LEN);
    (hi_void)memcpy_s(pBuffer + usOffset, usBufLen - usOffset, aucNodeAddress, MRS_MAIN_NODE_ADDR_LEN);
    usOffset += MRS_MAIN_NODE_ADDR_LEN;

    for (ucIndex = 0; ucIndex < pstNeighborInfo->usNum; ucIndex++)
    {
        (hi_void)memcpy_s(pBuffer + usOffset, MRS_CCO_NETWORK_NID_LEN, &pstNeighborInfo->astItem[ucIndex].ulSnid, MRS_CCO_NETWORK_NID_LEN);
        usOffset += MRS_CCO_NETWORK_NID_LEN;
    }

    return HI_ERR_SUCCESS;
}

HI_U32 mrs3762_AFN10_F112(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    HI_U16 usStartSeq = 0;
    HI_U8 ucQueryNum = 0;
    HI_U8 ucReportNum = 0;
    HI_U16 usDataLen = 0;
    HI_U8 *pData = HI_NULL;

    do
    {
        ret = mrsQueryTopoDownCheck(pstAfnParam, &usStartSeq, &ucQueryNum, MRS_CCO_QUERY_TOPO_STA_ID_INFO);
        if (HI_ERR_SUCCESS != ret)
        {
            return ret;
        }

        usDataLen = MRS_CCO_10_F112_DATA_FIX_LEN + ucQueryNum * MRS_CCO_10_F112_CHIP_INFO_LEN;
        pData = (HI_U8*)mrsToolsMalloc(usDataLen);
        if (!pData)
        {
            return HI_ERR_NOT_ENOUGH_MEMORY;
        }

        (hi_void)memset_s(pData, usDataLen, 0, usDataLen);
        ret = mrsSetChipQueryBuf(pData, ucQueryNum, usStartSeq, &ucReportNum);
    } while (0);

    if (HI_ERR_SUCCESS == ret)
    {
        HI_U16 usReplyLen = (HI_U16)(MRS_CCO_10_F112_DATA_FIX_LEN +
                            ucReportNum * MRS_CCO_10_F112_CHIP_INFO_LEN);

        mrs3762_EasyEnc(pData, usReplyLen, param);
    }

    mrsToolsFree(pData);

    return ret;
}

HI_U32 mrsSetChipQueryBuf(HI_U8* pData, HI_U8 ucQueryNum, HI_U16 usStartSeq, HI_U8* pucReportNum)
{
    HI_MAC_NETWORK_TOPO_S *pstTopoInf = HI_NULL;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_U16 usOffset = 0;
    HI_U8 ucReportNum = 0;
    HI_U16 usIndex = 0;
    HI_U16 usTopoNum = 0;
    HI_U8 pAddr[HI_PLC_MAC_ADDR_LEN] = {0};

    pstTopoInf = (HI_MAC_NETWORK_TOPO_S*)pstCcoCtx->pTopoIntegerInfo;
    usTopoNum = (HI_U16)pstTopoInf->num;
    (hi_void)memcpy_s(pData, sizeof(HI_U16), &usTopoNum, sizeof(HI_U16));
    usOffset += sizeof(HI_U16) + sizeof(usStartSeq) + sizeof(ucReportNum);

    for (usIndex = usStartSeq; usIndex < usStartSeq + ucQueryNum; usIndex++)
    {
        if (pstTopoInf->num < usIndex)
        {
            break;
        }
		if (1 == usIndex)
        {
            (hi_void)memcpy_s(pAddr, sizeof(pAddr), pstCcoCtx->ucMainNodeAddr, sizeof(pAddr));
			mrsHexInvert(pAddr, sizeof(pAddr));
        }
		else
		{
        (hi_void)memcpy_s(pAddr, sizeof(pAddr), pstTopoInf->entry[usIndex-1].mac, HI_PLC_MAC_ADDR_LEN);
        mrsHexInvert(pAddr, sizeof(pAddr));
        MRS_TOOLS_FE_TO_00(pAddr[0]);
		}
        (hi_void)memcpy_s(pData + usOffset, HI_PLC_MAC_ADDR_LEN, pAddr, HI_PLC_MAC_ADDR_LEN);
        usOffset += HI_PLC_MAC_ADDR_LEN;

        *(pData + usOffset) = pstTopoInf->entry[usIndex-1].product_type;
        usOffset += sizeof(HI_U8);

        (hi_void)memcpy_s(pData + usOffset, MRS_CCO_STA_CHIP_ID_LEN, pstCcoCtx->pstStaIdInfo->entry[usIndex-1].aucStaId, MRS_CCO_STA_CHIP_ID_LEN);
        usOffset += MRS_CCO_STA_CHIP_ID_LEN;

        (hi_void)memcpy_s(pData + usOffset, sizeof(HI_U16), &pstTopoInf->entry[usIndex-1].sta_ver.usSwVer, sizeof(HI_U16));
        usOffset += sizeof(HI_U16);

        ucReportNum++;
    }

    *(pData + MRS_CCO_TOPO_REPORT_NUM_OFFSET) = ucReportNum;

    (hi_void)memcpy_s(pData + MRS_CCO_TOPO_START_SEQ_OFFSET, sizeof(usStartSeq), &usStartSeq, sizeof(usStartSeq));

    *pucReportNum = ucReportNum;

    return HI_ERR_SUCCESS;
}

//��ѯ����������Ϣ
HI_U32 mrs3762_AFN10_F228(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    HI_U16 usStartSeq = 0;
    HI_U8 ucQueryNum = 0;
    HI_U8 ucReportNum = 0;
    HI_U16 usDataLen = 0;
    HI_U8 *pData = HI_NULL;

    do
    {
        ret = mrsQueryTopoDownCheck(pstAfnParam, &usStartSeq, &ucQueryNum, MRS_CCO_QUERY_TOPO_STA_ID_INFO);
        if (HI_ERR_SUCCESS != ret)
        {
            return ret;
        }

        usDataLen = MRS_CCO_10_F228_DATA_FIX_LEN + ucQueryNum * MRS_CCO_10_F228_TOPO_INFO_LEN;
        pData = (HI_U8*)mrsToolsMalloc(usDataLen);
        if (!pData)
        {
            return HI_ERR_NOT_ENOUGH_MEMORY;
        }

        (hi_void)memset_s(pData, usDataLen, 0, usDataLen);
        ret = mrsSetTopoQueryBuf(pData, ucQueryNum, usStartSeq, &ucReportNum, MRS_CCO_QUERY_TOPO_10_F228);
    } while (0);

    if (HI_ERR_SUCCESS == ret)
    {
        HI_U16 usReplyLen = (HI_U16)(MRS_CCO_10_F228_DATA_FIX_LEN +
                            ucReportNum * MRS_CCO_10_F228_TOPO_INFO_LEN);

        mrs3762_EasyEnc(pData,usReplyLen,param);
    }

    mrsToolsFree(pData);

    return ret;
}

//AFN11 F1������ز��ӽڵ�
HI_U32 mrs3762_AFN11_F1(HI_INOUT HI_VOID * param)
{
    HI_U8 ucMeterNum = 0;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    HI_U8 ucUnitConstLen = GetSecondaryNodeItemSize(pstCcoCtx);
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//����֡�еĴ���״̬��

    //���е�Ӧ������
    HI_U8 * pucContent = pstAfnParam->pContent;
    HI_U16 usContentLen = pstAfnParam->usContentLen;

    do
    {
        HI_U8 ucItemSize = GetSecondaryNodeItemSize(mrsCcoGetContext());
        HI_U8 i = 0;
        if (usContentLen < 3)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_015, HI_DIAG_MT("len<3"));
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }

        //�����ж�
        ucMeterNum = pucContent[2];
        if (usContentLen != ((ucUnitConstLen * ucMeterNum + 1) + 2))
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_016, HI_DIAG_MT("len error"));
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }

        if (ucMeterNum == 0)
        {
            stDenyData = MRS_13762_DENY_STATUS_INVALID_DATA;//��Ч�����ݵ�Ԫ
            ret = HI_ERR_BAD_DATA;
            break;
        }

        pucContent += 3;

        ret = mrsWhetherAddNode(pucContent, ucMeterNum, &stDenyData); //�ж��Ƿ�Ӧ��Ӵӽڵ�
        if (HI_ERR_SUCCESS != ret)
        {
            break;
        }

        for (i = 0; i < ucMeterNum; i++)
        {
            HI_U16 usTempNum = mrsSrvArchivesMeterNum();

            MRS_ARCHIVES_NODE_STRU stNode;
            (hi_void)memset_s(&stNode, sizeof(stNode), 0, sizeof(stNode));
            (hi_void)memcpy_s(stNode.aucNodeAddr, sizeof(stNode.aucNodeAddr), pucContent, HI_METER_ADDR_LEN);
            stNode.bValid = HI_TRUE;
            stNode.ucSave = HI_TRUE;
            stNode.ucProtocol = *(pucContent + ucItemSize - 1);
            if (HI_ERR_SUCCESS != mrsSrvArchivesInsert(&stNode, HI_TRUE))
            {
                HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_016, HI_DIAG_MT("mrsSrvArchivesInsert failed"));
                break;
            }

            if (!usTempNum)
            {
                mrsCcoDetectEmptyArchivesAddProc();
            }

            pucContent += ucItemSize;
        }
    } while (0);

    if (ret == HI_ERR_SUCCESS)
    {
        //��֯ȷ��֡
        mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);

        //�������һ����ʱ��(����д��flash��ʱ��)������ID������ʱ����ӵı���Ϣд��flash
        MRS_StartTimer(EN_MRS_SRV_CCO_WRITE_FLASH_ACTIV_TIMER, pstCcoCtx->ucFlashSaveDelay * MS_PER_SECOND, HI_SYS_TIMER_ONESHOT);
        mrsCcoSmWhitelistSwitchTimerReset();
    }
    else  //���ﻹû������ʲôԭ������ӣ����ֱ���ظ��ķ���֡���ݵ�Ԫ
    {
        //��֯����֡
        ret = mrs3762ProtoDenyFrame(&stDenyData, pstAfnParam, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
    }

    mrsDfxLrMeterNum(mrsSrvArchivesMeterNum());

    return ret;
}

//AFN11 F2��ɾ���ز��ӽڵ�
HI_U32 mrs3762_AFN11_F2(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 ucUnitConstLen = 6;//376.2���������ݵ�Ԫ����������С��Ԫ(�ֽ���):���ַ(6���ֽ�)
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//����֡�еĴ���״̬��

    //���е�Ӧ������
    HI_U8 * pucContent = pstAfnParam->pContent;
    HI_U16 usContentLen = pstAfnParam->usContentLen;
    HI_U16 usTempNum = mrsSrvArchivesMeterNum();

    do
    {
        HI_U8 ucMeterNum = 0;
        HI_U8 ucMeterIndex = 0;
        HI_BOOL bIsBCD = HI_TRUE;
        HI_U8 ucDelFlag = 0;

        if (usContentLen < 3)
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_017, HI_DIAG_MT("len<3:%d"),usContentLen);
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }

        //�����ж�
        ucMeterNum = pucContent[2];
        if (usContentLen != ((ucUnitConstLen * ucMeterNum + 1) + 2))
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_018, HI_DIAG_MT("len err:%d"),usContentLen);
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }

        if (ucMeterNum == 0)
        {
            stDenyData = MRS_13762_DENY_STATUS_INVALID_DATA;//��Ч�����ݵ�Ԫ
            ret = HI_ERR_BAD_DATA;
            break;
        }

        pucContent += 3;
        while (ucMeterIndex < ucMeterNum)
        {
            //����Ƿ��ǹ㲥��ַ
            bIsBCD = !(mrsToolsBroadCastAddr(pucContent + ucMeterIndex * ucUnitConstLen));
            if (bIsBCD != HI_TRUE)
            {
                ret = HI_ERR_BAD_DATA;
                stDenyData = MRS_13762_DENY_STATUS_ERROR_FORMAT; //��ʽ����
                break;
            }

            ucMeterIndex++;
        }

        if (bIsBCD == HI_FALSE)
        {
            break;
        }

        while(ucMeterNum-- > 0)
        {
            if (HI_ERR_SUCCESS == mrsSrvArchivesDelete(pucContent))
            {
                ucDelFlag++;
            }

            pucContent += HI_METER_ADDR_LEN;
        }

        if (ucDelFlag== 0)
        {
            stDenyData = MRS_13762_DENY_STATUS_INEXISTENT_METER;//��Ų�����
            ret = HI_ERR_BAD_DATA;
            break;
        }
    } while (0);

    if (ret == HI_ERR_SUCCESS)
    {
        //��֯ȷ��֡
        ret = mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);

        mrsSrvArchivesRealign();

        //(����д��flash��ʱ��)������Ӵӽڵ�ʱ�Ķ�ʱ����ͬ������ʱ��ɾ�����������еı���Ϣд��flash
        MRS_StartTimer(EN_MRS_SRV_CCO_WRITE_FLASH_ACTIV_TIMER, mrsCcoGetContext()->ucFlashSaveDelay * MS_PER_SECOND, HI_SYS_TIMER_ONESHOT);
        mrsCcoSmWhitelistSwitchTimerReset();

        if (usTempNum && !mrsSrvArchivesMeterNum())
        {
            mrsCcoDetectRmReset();
            mrsDfxCcoArchivesClearTime(HI_MDM_GetMilliSeconds());
        }
    }
    else
    {
        //��֯����֡
        ret = mrs3762ProtoDenyFrame(&stDenyData,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
    }

    mrsDfxLrMeterNum(mrsSrvArchivesMeterNum());

    return ret;
}

/* ���ⵥ��: DTS2014010608635 BEGIN: Added by liuxipeng/lkf54842, 2014/01/07 */

//AFN11 F3 ���ôӽڵ�̶��м�·��
HI_U32 mrs3762_AFN11_F3(HI_IN HI_OUT HI_VOID * param)
{
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;

    //����
    return mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
}

//AFN11 F4 ����·�ɹ���ģʽ
HI_U32 mrs3762_AFN11_F4(HI_IN HI_OUT HI_VOID * param)
{
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;

    //����
    return mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
}
/* ���ⵥ��: DTS2014010608635 END:   Added by liuxipeng/lkf54842, 2014/01/07 */

//*****************************************************************************
// ��������: mrs3762_AFN11_F5
// ��������: AFN11 F5: �����ز��ӽڵ�����ע��
//
// ����˵��:
//   param [in/out] ���ݵ�Ԫ
//
// �� �� ֵ:
//   HI_ERR_SUCCESS ����ȷ��֡
//   HI_ERR_FAILURE ��ַӳ��������ȡʧ��
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/f00209182 [2012-05-18]
// ��    ��: cuiate/00233580       [2013-04-08]
//*****************************************************************************
HI_U32 mrs3762_AFN11_F5(HI_INOUT HI_VOID * param)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    AFN_FN_UL_PARAM *pstAfnParam = (AFN_FN_UL_PARAM *)param;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;

    // ���е�Ӧ������
    HI_U8 * pucContent = pstAfnParam->pContent;
    HI_U16 usContentLen = pstAfnParam->usContentLen;

    do
    {
        // �����򳤶��ж�: 12 = fn(2) + time(6) + duration(4)
        if (usContentLen != 12)
        {
            ulRet = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }

        // ��鿪ʼʱ���Ƿ�Ϸ�
        ulRet = MRS_TimeIsValid(pucContent + 2);
        if (ulRet != HI_ERR_SUCCESS)
        {
            ulRet = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_FORMAT;
            break;
        }

        // ��鵥��״̬
        // 1. Զ������ʱ��Ӧ�����֡
        // 2. �򿪰���������:
        //    2.1 ����δ��ɣ��ȴ�������ɿ�ʼ�ѱ�
        //    2.2 ������ɣ���ʼ�ѱ�����

        // ����Զ��������Ӧ�����֡
        if (mrsIsCcoUpgStatus())
        {
            ulRet = HI_ERR_BUSY;
            stDenyData = MRS_13762_DENY_STATUS_CCO_BUSY;
            break;
        }

        // ����������ģʽ��: �ж�̨��ʶ���У��Ƿ������ѱ�
        if (mrsCcoGetSoftTfMode() == SOFT_TF_ON_WITH_STA_JOIN_BY_TF_RESULT
            && HI_FALSE == pstCcoCtx->stAutoSearchModel.stSearchCfg.ucAllowSMInTf
            && HI_MDM_IsTfComplete() == HI_FALSE)
        {
            ulRet = HI_ERR_BUSY;
            stDenyData = MRS_13762_DENY_STATUS_CCO_BUSY;
            break;
        }

        if (pstCcoCtx->status & MRS_CCO_STATUS_SEARCH_METER)
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_059, HI_DIAG_MT("�ѱ���...[%d]"), pstCcoCtx->status);
            ulRet = HI_ERR_SUCCESS;
            break;
        }
        else if (pstCcoCtx->status & MRS_CCO_STATUS_SEARCH_METER_TRANSFORMER)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_007, HI_DIAG_MT("̨��ʶ���ѱ���..."));
            ulRet = HI_ERR_BUSY;
            stDenyData = MRS_13762_DENY_STATUS_CCO_BUSY;
            break;
        }

        if (HI_FALSE == pstCcoCtx->bPauseFlag)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_058, HI_DIAG_MT("��·����ͣ"));
            ulRet = HI_ERR_BUSY;
            stDenyData = MRS_3762_DENY_STATUS_COMMUNICATION_OVERTIME;
            break;
        }

        // ��¼�ѱ����: ����ʱ��
        (hi_void)memcpy_s(&pstCcoCtx->usSearchDuration, sizeof(pstCcoCtx->usSearchDuration), pucContent + 8, 2);
        mrsCcoDfxSearchSetDuration(&(pstCcoCtx->stAutoSearchModel.stSearchDfx), pstCcoCtx->usSearchDuration);

        mrsSearchMeterPrepare(pstCcoCtx);
        pstCcoCtx->ucSearchWaitFlag = HI_FALSE;
        ulRet = mrsSearchMeterProc(pstCcoCtx);

        mrsCcoDfxSearchStartTime(&pstCcoCtx->stAutoSearchModel.stSearchDfx);
        HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("��ʼ�ѱ�"));
    } while (0);

    if (ulRet == HI_ERR_SUCCESS)
    {
        mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
    }
    else
    {
        //��֯����֡
       ulRet = mrs3762ProtoDenyFrame(&stDenyData, pstAfnParam, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
    }

    return ulRet;
}


//AFN11-F6: ��ֹ�ӽڵ�����ע��
HI_U32 mrs3762_AFN11_F6(HI_INOUT HI_VOID * param)
{
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;

    HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("��ֹͣ�ѱ�"));

    mrsCcoStopSearchMeterNormal((HI_U8)MRS_CCO_SEARCH_FINISH_STOP);

    mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);

    return HI_ERR_SUCCESS;
}

/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/28   ���ⵥ��:DTS2014032708937 */
HI_U32 mrs3762_AFN11_F101(HI_INOUT HI_VOID * param)
{
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;

    return mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
}
/* END:   Modified by fengxiaomin/00209182, 2014/3/28 */


//AFN12 F1������
HI_U32 mrs3762_AFN12_F1(HI_INOUT HI_VOID * param)
{
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//����֡�еĴ���״̬��

    do
    {
        if (mrsIsCcoUpgStatus())
        {
            ret = HI_ERR_BUSY;
            stDenyData = MRS_13762_DENY_STATUS_CCO_BUSY;
            break;
        }

        mrsCcoStopSearchMeterNormal((HI_U8)MRS_CCO_SEARCH_FINISH_ROUTE_RESET);

        // �л�����ͨ״̬����ʼ��
        mrsRecoverNormalState(pstCcoCtx);

        // ��λ�������ģ��(ֹͣ���ж�������)
        mrsRmModuleReset();

        (hi_void)memset_s(&pstCcoCtx->stReadMeterModel.stCurrNode, sizeof(MRS_ARCHIVES_NODE_STRU), 0, sizeof(MRS_ARCHIVES_NODE_STRU));
        pstCcoCtx->stReadMeterModel.usCurrNodeSeq = 0;  //(��Ϊ����)
        pstCcoCtx->stReadMeterModel.usSuccNum = 0;

        mrsSrvArchivesReadFlagClear();

        pstCcoCtx->bPauseFlag = HI_FALSE;
        mrsDfxLrRoute((HI_U8)pstCcoCtx->bPauseFlag);

        // ��ǰ�ִ�����
        mrsSetCurrTurnNum(0);
        mrsDetectRmBreakProc();
    } while (0);

    if (HI_ERR_SUCCESS == ret)
    {
        mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
		//if (!mrsGetCcoTimeFrist())//optimized by weichao
        {
            // ��ʱMRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUEʱ������¼������ģ��
            MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER, MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE*3, HI_SYS_TIMER_ONESHOT);
        }

    }
    else
    {
        //��֯����֡
       ret = mrs3762ProtoDenyFrame(&stDenyData, pstAfnParam, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
    }

    return ret;
}


//AFN12 F2����ͣ
HI_U32 mrs3762_AFN12_F2(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_U32 status = mrsCcoGetState();

    //�����������ͣ����
    if (pstCcoCtx->status & MRS_CCO_STATUS_NORMAL)
    {
        if(pstCcoCtx->stReadMeterModel.enCurrState != EN_MRS_SRV_CCO_READMETER_IDLE)
        {
            // �����ǰ���ǿ���״̬���������״̬(��ǰ�ڵ㲻�仯)
            mrsReadMeterStateInit();  // ����ģ��״̬��ʼ��
        }

        status &= ~MRS_CCO_STATUS_NORMAL;
        mrsCcoSetState(status);

        //�ָ�̽�Ⳮ��
        mrsResumeDetectRmSendMsg();

        HI_MDM_SetTfNtbSendProcessCtrl(HI_MDM_NTB_SEND_PROCESS_RESUME, HI_MDM_NTB_SEND_CTRL_REASON_READ_METER);
    }


    pstCcoCtx->bPauseFlag = HI_TRUE;
    mrsDfxLrRoute((HI_U8)pstCcoCtx->bPauseFlag);

    //��֯ȷ��֡
    mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);

    return ret;
}


//AFN12 F3���ָ�
HI_U32 mrs3762_AFN12_F3(HI_INOUT HI_VOID * param)
{
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//����֡�еĴ���״̬��

    do
    {
        if (mrsIsCcoUpgStatus())
        {
            ret = HI_ERR_BUSY;
            stDenyData = MRS_13762_DENY_STATUS_CCO_BUSY;
            break;
        }

        mrsCcoStopSearchMeterNormal((HI_U8)MRS_CCO_SEARCH_FINISH_ROUTE_RESUME);

        pstCcoCtx->bPauseFlag = HI_FALSE;
        mrsDfxLrRoute((HI_U8)pstCcoCtx->bPauseFlag);

        // �л�����ͨ״̬����ʼ��
        mrsRecoverNormalState(pstCcoCtx);
        mrsDetectRmBreakProc();
    } while (0);

    if ((pstCcoCtx->stReadMeterModel.usCurrNodeSeq == 0) && (pstCcoCtx->stReadMeterModel.ulCurrTurn == 0))
    {
        mrsSrvArchivesReadFlagClear();
        pstCcoCtx->stReadMeterModel.usSuccNum = 0;
    }

    if (HI_ERR_SUCCESS == ret)
    {
        mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
#if 0
        if (!mrsGetCcoTimeFrist())
        {
            // ��ʱMRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUEʱ������¼������ģ��
            MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER, MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE * 3, HI_SYS_TIMER_ONESHOT);
        }
#endif
    }
    else
    {
        //��֯����֡
       ret = mrs3762ProtoDenyFrame(&stDenyData, pstAfnParam, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
    }

    return ret;
}


// AFN13 F1: ����ز��ӽڵ�
HI_U32 mrs3762_AFN13_F1(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    HI_U8 * pucContent = pstAfnParam->pContent;
    HI_U16 usContentLen = pstAfnParam->usContentLen;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//����֡�еĴ���״̬��
    HI_U32 status = mrsCcoGetState();

    mrsDfxXrUartRx();
    mrsCcoDfxRmUartRx(pucContent, usContentLen, HI_FALSE);

    do
    {
        HI_U8 ucProtocolType = 0;
        HI_U8 ucDataLen = 0;
        HI_U8 ucPos = 0;
        HI_U8 *pucData = HI_NULL;
        MRS_3762_USER_DATA_HRD_STRU * pstUsrDataHrd = HI_NULL;
        HI_U8 ucMinLen = 5;
        HI_BOOL bDelayRelated = HI_FALSE;

        pstUsrDataHrd = &pstAfnParam->pstFrame->strUsrDataHrd;     // ��ȡ��ַ��
        pstCcoCtx->stAnytimeRMModel.seq_1376_2 = pstUsrDataHrd->uInfoRealm.stInfRealmDL.ucSeq;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
        // ����ǽ��հ汾(NV����)����ҵ�񴮿ڣ�������;��������
        if (!(mrsToolIsBbu()
            && (!mrsIsSimuChlOpened())))
#endif
        {
            if(!mrsIsDetectRm()
                && ((EN_MRS_ANYTIME_RM_WAIT_PLC == pstCcoCtx->stAnytimeRMModel.enState)
                || (EN_MRS_ANYTIME_RM_WAIT_REVISE_RESP == pstCcoCtx->stAnytimeRMModel.enState)))   // ֻ����PLC����ռ��״̬�²Ž�������
            {
                ret = HI_ERR_BUSY;  // ��ǰ����ִ���泭����
                stDenyData = MRS_13762_DENY_STATUS_CCO_BUSY;
                HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_023, HI_DIAG_MT("enState= %d"),pstCcoCtx->stAnytimeRMModel.enState);
                break;
            }
        }

        // Fn(2)+ͨ��Э������(1)+ͨ����ʱ����Ա�ʶ(1)+�ӽڵ㸽���ڵ�����(1)+���ĳ���(1)
        if (pstCcoCtx->ucProtoVer != PROTO_VER_3762_2009)
        {
            ucMinLen = 6;
        }

        if (usContentLen < ucMinLen)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_024, HI_DIAG_MT("len<5!"));
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }

        ucPos += 2;     // Fn

        //����Լ�����Ƿ���Ч
        ret = mrsCheckProtocolValid(pucContent[ucPos]);
        if (ret != HI_ERR_SUCCESS)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_025, HI_DIAG_MT("protocol err!"));
            stDenyData = MRS_13762_DENY_STATUS_ERROR_FORMAT;
            ret = HI_ERR_BAD_DATA;
            break;
        }

        ucProtocolType = pucContent[ucPos++]; // ��ȡ��Լ����

        if (pstCcoCtx->ucProtoVer != PROTO_VER_3762_2009)
        {
            if (pucContent[ucPos] == MRS_AFN_FRAME_DELAY_RELATED)
            {
                bDelayRelated = HI_TRUE;
            }
            ucPos++;
        }

        // ʣ�೤�� < �ӽڵ㸽���ڵ���Ϣ + ���ĳ���
        if((HI_S16)(usContentLen - ucPos) < (pucContent[ucPos] * 6 + 1))
        {
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_026, HI_DIAG_MT("extra_len<node_len+data_len"));
            break;
        }

        ucPos = (HI_U8)(ucPos + pucContent[ucPos] * 6);   // ƫ�ƴӽڵ㸽���ڵ���Ϣ(��ǰ���������ڵ���Ϣ)
        ucPos++;

        ucDataLen = pucContent[ucPos];    // ��ȡ���ĳ���
        ucPos++;

        if((HI_S16)(usContentLen - ucPos) < ucDataLen)
        {
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_027, HI_DIAG_MT("(usContentLen - ucPos) < ucDataLen"));
            break;
        }

        pucData = &pucContent[ucPos];   // ��ȡת������

        pstCcoCtx->stAnytimeRMModel.ucProtocol = ucProtocolType;

        mrsAnyTimeReadMeterInit();  // ��ʼ���泭����ģ��

		if(pstAfnParam->ucNdmPlc)
		{
			pstCcoCtx->stAnytimeRMModel.from = XR_FROM_NDM_AFN13_F1;
		} else {
	        // �����ԴΪ "����ز��ӽڵ�"
	        pstCcoCtx->stAnytimeRMModel.from = XR_FROM_AFN13_F1;
		}

        mrsDetectRmBreakProc();

        HI_MDM_SetTfNtbSendProcessCtrl(HI_MDM_NTB_SEND_FREQUENCY_REDUCE, HI_MDM_NTB_SEND_CTRL_REASON_READ_METER);

        if (bDelayRelated)
        {
            ret = mrs3762_AFN14_F3_UP(pstAfnParam, ucProtocolType, ucDataLen, pucData);
            if (ret == HI_ERR_SUCCESS)
            {
                status &= ~MRS_CCO_STATUS_NORMAL;
                status |= MRS_CCO_STATUS_ANYTIME_READMETER;
                mrsCcoSetState(status);
                mrsSetXRStatus(EN_MRS_ANYTIME_RM_WAIT_REVISE_RESP);

                // �������������ʱ��(��ʱ���Ὣ�泭״̬��ΪԤ�ָ�״̬)
                MRS_StartTimer(EN_MRS_SRV_CCO_SERVICE_END_TIMER,
                               (pstCcoCtx->stAnytimeRMModel.stOverTimeInf.MaxTimeout - MRS_RM_MAX_SERVICE_TIME_SUB) * 1000,
                               HI_SYS_TIMER_ONESHOT);
                return ret;
            }
            else
            {
                mrsAnyTimeReadMeterFinishProc();
                HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_051, HI_DIAG_MT("any read 14F3 fail"));
                break;
            }
        }

        // �����泭����ģ����ж���
        ret = mrsAnyTimeReadMeterProcess(pstUsrDataHrd, ucProtocolType, pucData, ucDataLen);
        if (HI_ERR_SUCCESS != ret)
        {
            stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;
        }
    } while (0);

    if(ret != HI_ERR_SUCCESS)
    {
    	HI_U8 ucFrom;

		if(pstAfnParam->ucNdmPlc)
		{
			ucFrom = XR_FROM_NDM_AFN13_F1;
		} else {
	        // �����ԴΪ "����ز��ӽڵ�"
	        ucFrom = XR_FROM_AFN13_F1;
		}

		//ndm��plc֡�㳭��ɵ�æ���ش���
    	if((ret == HI_ERR_BUSY) && (mrsCcoNdmPlcBusyAdd(pstAfnParam,ucFrom) == HI_ERR_BUSY))
    	{
    		return ret;
    	}

        ucFrom = pstCcoCtx->stAnytimeRMModel.from;

		if(pstAfnParam->ucNdmPlc)
		{
			pstCcoCtx->stAnytimeRMModel.from = XR_FROM_NDM_AFN13_F1;
		} else {
	        // �����ԴΪ "����ز��ӽڵ�"
	        pstCcoCtx->stAnytimeRMModel.from = XR_FROM_AFN13_F1;
		}

        //��װ����֡
        ret = mrsAFN13F1CreateAckFrame(pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen, HI_NULL);
        pstCcoCtx->stAnytimeRMModel.from = ucFrom;

        mrsDfxRmUartDenyTx(pucContent, usContentLen);
        mrsDfxXrNakCnt();
        mrsDfxXrNak((HI_U8)stDenyData);

        HI_MDM_SetTfNtbSendProcessCtrl(HI_MDM_NTB_SEND_PROCESS_RESUME, HI_MDM_NTB_SEND_CTRL_REASON_READ_METER);
    }

    return ret;
}

HI_U32 mrs3762_AFN14_F1(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    AFN_14_F1_DL * pstAfn14Dl = HI_NULL;
    AFN_14_F1_DL_PARAM *pstAfn14F1Param = HI_NULL;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//����֡�еĴ���״̬��

    //���е�Ӧ������
    HI_U8 * pucContent = pstAfnParam->pContent;
    HI_U16 usContentLen = pstAfnParam->usContentLen;

    mrsDfxLrAckCnt();
    mrsCcoDfxRmUartRx(pucContent, usContentLen, HI_TRUE);

    do
    {
        HI_U8 ucDataLen = 0;
        MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
        HI_U8 ucMinLen = 4;
        HI_U8 ucPos = 0;
        HI_BOOL bDelayRelated = HI_FALSE;
        HI_U8 ucRmFlag = 0;

        if (pstCcoCtx->stReadMeterModel.enCurrState != EN_MRS_SRV_CCO_READMETER_WAIT_ASK_REPLY) // �������ģ�鵱ǰ״̬����"�ȴ�ѯ��Ӧ��״̬"
        {
            // ��ʱ������֡,�����д���,�������Ż�
            ret = HI_ERR_SUCCESS;
            stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;
            break;
        }

        if (pstCcoCtx->stReadMeterModel.pstAfn14F1 != HI_NULL) // ���AFN14������������û���ͷ�
        {
            // �ͷŵ����ڴ�����,׼������������ʹ��
            pstAfn14F1Param = pstCcoCtx->stReadMeterModel.pstAfn14F1;
            pstAfn14F1Param->MRS_AFN14_FREE_FUN(pstAfn14F1Param);

            pstAfn14F1Param = HI_NULL;
            pstCcoCtx->stReadMeterModel.pstAfn14F1 = HI_NULL;
        }

        // Fn(2)+������־(1)+ͨ����ʱ����Ա�ʶ(1)+���ݳ���(1) �ݴ�:������ӽڵ㸽���ڵ㳤��
        if (pstCcoCtx->ucProtoVer != PROTO_VER_3762_2009)
        {
            ucMinLen = 5;
        }

        if (usContentLen < ucMinLen)
        {
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }

        pstAfn14F1Param = mrsToolsMalloc(sizeof(AFN_14_F1_DL_PARAM));
        if (pstAfn14F1Param == HI_NULL)
        {
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;
            break;
        }

        (hi_void)memset_s(pstAfn14F1Param, sizeof(AFN_14_F1_DL_PARAM), 0, sizeof(AFN_14_F1_DL_PARAM));
        pstAfn14F1Param->MRS_AFN14_FREE_FUN  = mrs3762AFN14DLFree;

        pstAfn14Dl = mrsToolsMalloc(sizeof(AFN_14_F1_DL));
        if (pstAfn14Dl == HI_NULL)
        {
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;
            break;
        }

        (hi_void)memset_s(pstAfn14Dl, sizeof(AFN_14_F1_DL), 0, sizeof(AFN_14_F1_DL));
        pstAfn14F1Param->pstAFN_14_F1_DL = pstAfn14Dl;

        //������־�ж�
        ucPos = 2;
        ucRmFlag = pucContent[ucPos];
        if (ucRmFlag > 2)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_032, HI_DIAG_MT("14-F1 flag err"));
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_FORMAT;
            break;
        }

        pstAfn14Dl->ucReadFlg = pucContent[ucPos];

        // �ж��Ƿ�ʱ�����
        if (pstCcoCtx->ucProtoVer != PROTO_VER_3762_2009)
        {
            ucPos++;
            if (pucContent[ucPos] == MRS_AFN_FRAME_DELAY_RELATED)
            {
                bDelayRelated = HI_TRUE;
            }
        }

        //���ݳ���L
        ucPos++;
        pucContent += ucPos;
        ucDataLen = *pucContent++;

        //Fn(2)+������־(1)+ͨ����ʱ����Ա�ʶ(1)+���ݳ���(1)+����(ucDataLen) �ݴ�:���жϴӽڵ㸽���ڵ㳤��
        if (usContentLen < (ucMinLen + ucDataLen))
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_033, HI_DIAG_MT("len<4:%d"),usContentLen);
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }

        if (2 == ucRmFlag)
        {
            ret = mrsCheckRmMeter(pstCcoCtx->stReadMeterModel.stCurrNode.ucProtocol, pstCcoCtx->stReadMeterModel.stCurrNode.aucNodeAddr, ucDataLen, pucContent);
            if (HI_ERR_SUCCESS != ret)
            {
                pstAfn14F1Param->MRS_AFN14_FREE_FUN(pstAfn14F1Param);
                return ret;
            }
        }

        if (ucDataLen != 0)
        {
            if (bDelayRelated)
            {
                // ����AFN=14H F3����
                ret = mrs3762_AFN14_F3_UP(pstAfnParam, METER_PROTO_645_2007, ucDataLen, pucContent);
                if (ret == HI_ERR_SUCCESS)
                {
                    mrsSetReadMeterState(EN_MRS_SRV_CCO_READMETER_WAIT_REVISE_RESP);
                    // ����������ʱ�����ȴ�һ��������
                    MRS_StartTimer(EN_MRS_SRV_CCO_MUSTER_TIMER, MRS_SRV_CCO_TIMER_MUSTER_VALUE, HI_SYS_TIMER_ONESHOT);
                }
                return ret;
            }

            pstAfn14Dl->pucData = mrsToolsMalloc(ucDataLen);
            if (pstAfn14Dl->pucData == HI_NULL)
            {
                ret = HI_ERR_BAD_DATA;
                stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;
                break;
            }
            (hi_void)memcpy_s(pstAfn14Dl->pucData, ucDataLen, pucContent, ucDataLen);
            pucContent += ucDataLen;
        }

        pstAfn14Dl->ucDataLen = ucDataLen;

        pstCcoCtx->stReadMeterModel.pstAfn14F1 = pstAfn14F1Param;  //�����������CCO����
        mrsReadterModelCco(HI_NULL);
    } while (0);

    if (ret != HI_ERR_SUCCESS)
    {
        // ��֯����֡
        ret = mrs3762ProtoDenyFrame(&stDenyData,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);

        if (pstAfn14F1Param != HI_NULL)
        {
            pstAfn14F1Param->MRS_AFN14_FREE_FUN(pstAfn14F1Param);
        }
    }
    return ret;
}

HI_U32 mrsGetFrameMeter(HI_U8 ucProtocol, HI_U16 usDataLen, HI_U8 *pData, HI_U8* pMeter)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    if (mrsCheck645Proto(ucProtocol))
    {
        HI_U16 pos = 0;
        HI_U16 len = 0;

        ret = mrsFind645Frame(pData, (HI_S16)usDataLen, &pos, &len);
        if (ret != HI_ERR_SUCCESS)
        {
            return HI_ERR_BAD_FRAME;
        }

        (hi_void)memcpy_s(pMeter, HI_METER_ADDR_LEN, pData + pos + 1, HI_METER_ADDR_LEN);
    }
    else if (mrsCheck69845Proto(ucProtocol))
    {
        ret = mrsFind698MeterAddr(pData, usDataLen, pMeter);
        if (ret != HI_ERR_SUCCESS)
        {
            return HI_ERR_BAD_FRAME;
        }
    }
    else
    {
        return HI_ERR_BAD_DATA;
    }

    return ret;
}

HI_U32 mrsCheckRmMeter(HI_U8 ucProtocol, HI_U8 *pMeter, HI_U16 usDatalen, HI_U8 *pData)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 aucAddr[HI_METER_ADDR_LEN] = {0};

    if ((HI_NULL == pMeter) || (0 == usDatalen) || (HI_NULL == pData))
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    ret = mrsGetFrameMeter(ucProtocol, usDatalen, pData, aucAddr);
    if ((HI_ERR_SUCCESS == ret) && mrsToolsMemEq(pMeter, aucAddr, HI_METER_ADDR_LEN))
    {
        return HI_ERR_SUCCESS;
    }

    if (mrsCheckTransProto(ucProtocol))
    {
        return HI_ERR_SUCCESS;
    }

    return HI_ERR_BAD_DATA;
}

//*****************************************************************************
// ��������: mrs3762AFN14DLFree
// ��������: �ͷ�AFN014F1������֡
//
// ����˵��:
//   pafn14 [in/out]
//
// �� �� ֵ:
//    TODO: ...
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: niesongsong/nKF62735 [2011-12-17]
//*****************************************************************************
HI_VOID mrs3762AFN14DLFree(HI_VOID * pafn14)
{
    AFN_14_F1_DL_PARAM * param = pafn14;
    if (param != HI_NULL)
    {
        if (param->pstAFN_14_F1_DL != HI_NULL)
        {
            if (param->pstAFN_14_F1_DL->pucData)
            {
                mrsToolsFree(param->pstAFN_14_F1_DL->pucData);
            }
            if (param->pstAFN_14_F1_DL->pucAttNodeAdd)
            {
                mrsToolsFree(param->pstAFN_14_F1_DL->pucAttNodeAdd);
            }
            mrsToolsFree(param->pstAFN_14_F1_DL);
        }
        mrsToolsFree(pafn14);
    }
}

//*****************************************************************************
// ��������: mrs3762_AFN14_F2
// ��������: ·����������ʱ��
//
// ����˵��:
//   pafn14 [in/out]
//
// �� �� ֵ:
//    TODO: ...
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liuxipeng/lkf54842 [2012-08-07]
// ��    ��: cuiate/00233580    [2013-03-30]
//*****************************************************************************
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
HI_U32 mrs3762_AFN14_F2(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    //���е�Ӧ������
    HI_U8 * pucContent = pstAfnParam->pContent;
    HI_U16 usContentLen = pstAfnParam->usContentLen;

    do
    {
        // Fn(2Byte) + ʱ��(������ʱ����)
        if(usContentLen != 2 + 6)
        {
            ret = HI_ERR_BAD_DATA;
            break;
        }

        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1001, HI_DIAG_MT("rcv afn time"),&pucContent[2],6);
        ret = MRS_TimeIsValid(&pucContent[2]);
        if (ret != HI_ERR_SUCCESS)
        {
            ret = HI_ERR_BAD_DATA;
            break;
        }
#if 0//optimized by weichao
        ret = mrsCcoSetClock(&pucContent[2]);
        if (ret != HI_ERR_SUCCESS)
        {
            break;
        }
#endif
    } while (0);

    return ret;
}
#endif

// �ú���û�д��������
static HI_U32 mrs3762_AFN14_F3_DelayProc(HI_IN HI_U8 * pDataBuff, HI_IN HI_U16 usDataLen)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    AFN_14_F1_DL_PARAM * pstAfn14F1Param = HI_NULL;
    AFN_14_F1_DL * pstAfn14Dl = HI_NULL;
    HI_U16 pos = 0;
    HI_U16 len = 0;

    if(EN_MRS_SRV_CCO_READMETER_WAIT_REVISE_RESP != pstCcoCtx->stReadMeterModel.enCurrState)
    {
        return HI_ERR_BAD_DATA;
    }

    switch (pstCcoCtx->stReadMeterModel.stCurrNode.ucProtocol)
    {
    case METER_PROTO_645_2007:
    case METER_PROTO_645_1997:
        ret = mrsFind645Frame(pDataBuff, (HI_S16)usDataLen, &pos, &len);
        if (HI_ERR_SUCCESS != ret)
        {
            return ret;
        }
        break;

    case METER_PROTO_698_45:
        ret = mrsFind698Frame(pDataBuff, usDataLen, &pos, &len);
        if (ret != HI_ERR_SUCCESS)
        {
            return ret;
        }
        break;

    case METER_PROTO_TRANSPARENT:
        break;

    default:
        return HI_ERR_BAD_DATA;
    }

    if (pstCcoCtx->stReadMeterModel.pstAfn14F1 != HI_NULL) // ���AFN14������������û���ͷ�
    {
        // �ͷŵ����ڴ�����,׼������������ʹ��
        pstAfn14F1Param = pstCcoCtx->stReadMeterModel.pstAfn14F1;
        pstAfn14F1Param->MRS_AFN14_FREE_FUN(pstAfn14F1Param);

        pstAfn14F1Param = HI_NULL;
        pstCcoCtx->stReadMeterModel.pstAfn14F1 = HI_NULL;
    }

    do
    {
        pstAfn14F1Param = mrsToolsMalloc(sizeof(AFN_14_F1_DL_PARAM));
        if (pstAfn14F1Param == HI_NULL)
        {
            ret = HI_ERR_MALLOC_FAILUE;
            break;
        }

        (hi_void)memset_s(pstAfn14F1Param, sizeof(AFN_14_F1_DL_PARAM), 0, sizeof(AFN_14_F1_DL_PARAM));
        pstAfn14F1Param->MRS_AFN14_FREE_FUN = mrs3762AFN14DLFree;

        pstAfn14Dl = mrsToolsMalloc(sizeof(AFN_14_F1_DL));
        if (pstAfn14Dl == HI_NULL)
        {
            ret = HI_ERR_MALLOC_FAILUE;
            break;
        }

        (hi_void)memset_s(pstAfn14Dl, sizeof(AFN_14_F1_DL), 0, sizeof(AFN_14_F1_DL));
        pstAfn14F1Param->pstAFN_14_F1_DL = pstAfn14Dl;
        pstAfn14Dl->ucReadFlg = 0x02;   // ���Գ���

        pstAfn14Dl->pucData = mrsToolsMalloc(usDataLen);
        if (pstAfn14Dl->pucData == HI_NULL)
        {
            ret = HI_ERR_MALLOC_FAILUE;
            break;
        }

        (hi_void)memcpy_s(pstAfn14Dl->pucData, usDataLen, pDataBuff, usDataLen);
        pstAfn14Dl->ucDataLen = (HI_U8)usDataLen;

        pstCcoCtx->stReadMeterModel.pstAfn14F1 = pstAfn14F1Param;  //�����������CCO����
        mrsReadterModelCco(HI_NULL);
    } while (0);

    if (ret != HI_ERR_SUCCESS)
    {
        if (pstAfn14F1Param != HI_NULL)
        {
            pstAfn14F1Param->MRS_AFN14_FREE_FUN(pstAfn14F1Param);
        }
    }

    return ret;
}

HI_U32 mrs3762_AFN14_F3(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = param;
    HI_U8 *pucContent = pstAfnParam->pContent;
    HI_U16 usContentLen = pstAfnParam->usContentLen;
    HI_U16 usMinLen = 3;    // Fn(2) + ����(1)
    HI_U16 usDataLen = 0;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_U8 ucProtocol = 0;
    HI_U8 aucMeter[HI_METER_ADDR_LEN] = {0};

/* BEGIN: Modified by fengxiaomin/00209182, 2015/9/7   ���ⵥ�ź�:DTS2015091503701 */
    do
    {
        if (usContentLen < usMinLen)
        {
            ret = HI_ERR_BAD_DATA;
            break;
        }

        usDataLen = pucContent[2];
        if ((pucContent[2] == 0) || (usContentLen < usMinLen + usDataLen))
        {
            ret = HI_ERR_BAD_DATA;
            break;
        }

        if (pstCcoCtx->status & MRS_CCO_STATUS_NORMAL)
        {
            ucProtocol = pstCcoCtx->stReadMeterModel.stCurrNode.ucProtocol;
            (hi_void)memcpy_s(aucMeter, HI_METER_ADDR_LEN, pstCcoCtx->stReadMeterModel.stCurrNode.aucNodeAddr, HI_METER_ADDR_LEN);
        }
        else if (pstCcoCtx->status & MRS_CCO_STATUS_ANYTIME_READMETER)
        {
            ucProtocol = pstCcoCtx->stAnytimeRMModel.ucProtocol;
            (hi_void)memcpy_s(aucMeter, HI_METER_ADDR_LEN, pstCcoCtx->stAnytimeRMModel.aucMeterAddr, HI_METER_ADDR_LEN);
        }
        else
        {
            ret = HI_ERR_BAD_DATA;
            break;
        }

        ret = mrsCheckRmMeter(ucProtocol, aucMeter, usDataLen, pucContent + 3);
        if (HI_ERR_SUCCESS != ret)
        {
            return ret;
        }
    } while (0);

    if (pstCcoCtx->status & MRS_CCO_STATUS_ANYTIME_READMETER)
    {
        if(pstCcoCtx->stAnytimeRMModel.enState == EN_MRS_ANYTIME_RM_WAIT_REVISE_RESP)
        {
            // �����ԴΪ "����ز��ӽڵ�"
            pstCcoCtx->stAnytimeRMModel.from = XR_FROM_AFN13_F1;
            mrsSetXRStatus(EN_MRS_ANYTIME_RM_REVISE_RESP_PROC);

            if (HI_ERR_SUCCESS != ret)
            {
                // ͸������
                mrsAnyTimeReadMeterFailEndProc(pstCcoCtx);

                return ret;
            }
/* END:   Modified by fengxiaomin/00209182, 2015/9/7 */

            ret = mrsAnyTimeReadMeterProcessDelay(pucContent + 3, usDataLen);
            if (ret == HI_ERR_SUCCESS)
            {
                // �������������ʱ��(��ʱ���Ὣ�泭״̬��ΪԤ�ָ�״̬)
                MRS_StartTimer(EN_MRS_SRV_CCO_SERVICE_END_TIMER, (pstCcoCtx->stAnytimeRMModel.stOverTimeInf.MaxTimeout - MRS_RM_MAX_SERVICE_TIME_SUB) * 1000, HI_SYS_TIMER_ONESHOT);
            }
        }

        return ret;
    }

    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }

    if (pstCcoCtx->status & MRS_CCO_STATUS_NORMAL)
    {
        return mrs3762_AFN14_F3_DelayProc(pucContent + 3, usDataLen);
    }

    return HI_ERR_FAILURE;
}

HI_U32 mrs3762_AFN14_F3_UP(HI_INOUT AFN_FN_UL_PARAM *param, HI_IN HI_U8 protocol, HI_IN HI_U8 frame_len, HI_IN HI_U8 *frame_data)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 * payload = HI_NULL;
    HI_U16 payload_len = 0;
    HI_U16 offset = 0;
    HI_U8 ucAfn = 0;
    PROTO376_2_FRAME_PRE_STRU up_frame;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();

    if ((HI_NULL == param) || (HI_NULL == frame_data) || (0 == frame_len))
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    // ���б�������: ���ַ(6) + ��ʱ(2) + ���ĳ���(1) + ��������
    payload_len = frame_len + 9;
    payload = mrsToolsMalloc(payload_len);
    if (HI_NULL == payload)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(payload, payload_len, 0, payload_len);
    ucAfn = param->pstFrame->strUsrDataHrd.stAppDataRealm.ucAfn;

    if (MRS_AFN(0x14) == ucAfn)
    {
        (hi_void)memcpy_s(payload, payload_len, pstCcoCtx->stReadMeterModel.stCurrNode.aucNodeAddr, HI_METER_ADDR_LEN);
    }
    else
    {
        switch (protocol)
        {
        case METER_PROTO_645_2007:
        case METER_PROTO_645_1997:
            {
                HI_U16 pos = 0;
                HI_U16 len = 0;

                ret = mrsFind645Frame(frame_data, (HI_S16)frame_len, &pos, &len);
                if (ret != HI_ERR_SUCCESS)
                {
                    // δ�ҵ�645֡�����ش���ֵ
                    mrsToolsFree(payload);
                    return ret;
                }

                (hi_void)memcpy_s(payload, payload_len, frame_data + pos + 1, HI_METER_ADDR_LEN);
                break;
            }

        case METER_PROTO_698_45:
            ret = mrsFind698MeterAddr(frame_data, frame_len, payload);
            if (ret != HI_ERR_SUCCESS)
            {
                // δ�ҵ�69845֡�����ش���ֵ
                mrsToolsFree(payload);
                return ret;
            }
            break;

        case METER_PROTO_TRANSPARENT:
            {
                MRS_3762_ADDR_REALM_STRU *realm = &(param->pstFrame->strUsrDataHrd.stAddrRealm);
                (hi_void)memcpy_s(payload, payload_len, realm->ucDestAdd, HI_METER_ADDR_LEN);
                break;
            }

        default:
            mrsToolsFree(payload);
            return HI_ERR_BAD_DATA;
        }
    }

    offset += HI_METER_ADDR_LEN;
    payload[offset++] = (MRS_AFN14_F3_DELAY_TIME % 256);
    payload[offset++] = (MRS_AFN14_F3_DELAY_TIME / 256);
    payload[offset++] = frame_len;
    (hi_void)memcpy_s(payload + offset, payload_len - offset, frame_data, frame_len);

    (hi_void)memset_s(&up_frame, sizeof(up_frame), 0, sizeof(up_frame));
    up_frame.stAfnData.ucAFN = MRS_AFN(0x14);
    up_frame.stAfnData.ucDT[0] = MRS_AFN_FN_LO(3);
    up_frame.stAfnData.ucDT[1] = MRS_AFN_FN_HI(3);
    up_frame.stAfnData.pData = payload;
    up_frame.stAfnData.usDataLen = payload_len;
    up_frame.ucModuleFlag = 0;
    up_frame.ucSeq = param->pstFrame->strUsrDataHrd.uInfoRealm.stInfRealmDL.ucSeq;

    up_frame.ucPrm = 1;

    ret = mrsCreate3762Frame(&up_frame, param->ppucOutBuffer, param->pusBufferLen, payload);
    if ((HI_ERR_SUCCESS == ret) && (MRS_AFN(0x13) == ucAfn))
    {
        (hi_void)memcpy_s(pstCcoCtx->stAnytimeRMModel.aucMeterAddr, HI_METER_ADDR_LEN, payload, HI_METER_ADDR_LEN);
    }

    mrsToolsFree(payload);

    return ret;
}

//AFN 15H
HI_U32 mrs3762_AFN15_F1(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
/* BEGIN: Added by fengxiaomin/00209182, 2014/1/10   ���ⵥ�ź�:DTS2014012607684 */
    HI_U32 ulSeg = 0;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    HI_U8 * pContent = HI_NULL;
    HI_U16 usContentLen = 0;
    MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx = mrsGetRemoteUpgCtx();
    HI_U8 ucFileFlag = 0;
    HI_U16 usTotalSegNum = 0;

    pContent = pstAfnParam->pContent;
    usContentLen = pstAfnParam->usContentLen;

    do
    {
        HI_U16 usDataLen = 0;
        HI_U8 ucFileProperty = 0;

        ucFileFlag = *(pContent + MRS_CCO_UPG_FILE_FLAG_OFFSET);
        mrsDfxRefreshClearUpg(ucFileFlag);
        mrsDfxRxUpgFrameNum();

        if (!mrsIsCcoStatePermitUpg())
        {
            ulSeg = MRS_CCO_UPG_ERROR_SEG;
            HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("��ERR��NOT Pemit UPG"));
            break;
        }

        (hi_void)memcpy_s(&usDataLen, sizeof(usDataLen), pContent + MRS_CCO_UPG_DATA_LEN_OFFSET, sizeof(HI_U16));
        if (usContentLen != (MRS_CCO_UPG_FIX_DATA_OFFSET + usDataLen))
        {
            ulSeg = MRS_CCO_UPG_ERROR_SEG;
            HI_DIAG_LOG_MSG_E2(__LINE__, HI_DIAG_MT("��ERR��Content-datalen"),usContentLen,usDataLen);
            break;
        }

        if (MRS_CCO_CLEAR_UPG_LOAD_FILE == ucFileFlag)
        {
            pstMrsUpgCtx->ucUpgFileFlag = ucFileFlag;
            ret = mrsRemoteUpgClear(pstMrsUpgCtx);
            if (HI_ERR_SUCCESS == ret)
            {
                (hi_void)memcpy_s(&ulSeg, sizeof(ulSeg), pContent + MRS_CCO_UPG_CURRENT_SEG_OFFSET, sizeof(HI_U32));
                break;
            }
            else if (HI_ERR_CONTINUE != ret)
            {
                ulSeg = MRS_CCO_UPG_ERROR_SEG;
                HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("��ERR��clear ret"),ret);
                break;
            }

            pstMrsUpgCtx->pstFrame = mrsToolsMalloc(sizeof(MRS_3762_FRAME_STRU));
            if (HI_NULL == pstMrsUpgCtx->pstFrame)
            {
                ulSeg = MRS_CCO_UPG_ERROR_SEG;
                HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("��ERR��melloc"));
                break;
            }

            (hi_void)memset_s(pstMrsUpgCtx->pstFrame, sizeof(MRS_3762_FRAME_STRU), 0, sizeof(MRS_3762_FRAME_STRU));
            (hi_void)memcpy_s(pstMrsUpgCtx->pstFrame, sizeof(MRS_3762_FRAME_STRU), pstAfnParam->pstFrame, sizeof(MRS_3762_FRAME_STRU));
            (hi_void)memcpy_s(&pstMrsUpgCtx->ulClearFileSeg, sizeof(pstMrsUpgCtx->ulClearFileSeg), pContent + MRS_CCO_UPG_CURRENT_SEG_OFFSET, sizeof(HI_U32));
            return ret;
        }

        mrsRemoteUpgRxFilePro();

        //�ж��ļ���ʶ���ļ�����
        ucFileProperty = *(pContent + MRS_CCO_UPG_FILE_PROPERTY_OFFSET);
        if (!mrsIsUpgFileFlagValid(ucFileFlag, ucFileProperty))
        {
            ulSeg = MRS_CCO_UPG_ERROR_SEG;
            HI_DIAG_LOG_MSG_E2(__LINE__, HI_DIAG_MT("��ERR��property-flag"),ucFileProperty,ucFileFlag);
            break;
        }

        (hi_void)memcpy_s(&usTotalSegNum, sizeof(usTotalSegNum), pContent + MRS_CCO_UPG_TOTAL_SEG_OFFSET, sizeof(HI_U16));
        (hi_void)memcpy_s(&ulSeg, sizeof(ulSeg), pContent + MRS_CCO_UPG_CURRENT_SEG_OFFSET, sizeof(HI_U32));

        if (ulSeg >= usTotalSegNum)
        {
            mrsDfxRefreshFrameErrFlag(MRS_DFX_CCO_UPG_CURR_SEG_OVER_TOTAL_SEG_ERR);
            ulSeg = MRS_CCO_UPG_ERROR_SEG;
            break;
        }

        HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_057, HI_DIAG_MT("curr seg-total seg"),ulSeg,usTotalSegNum);

        ret = mrsRemoteUpgFrameRxPro(pContent, ulSeg, usTotalSegNum);

        pstMrsUpgCtx->ucFrameSeq = pstAfnParam->pstFrame->strUsrDataHrd.uInfoRealm.stInfRealmDL.ucSeq;

        if (HI_ERR_SUCCESS != ret)
        {
            ulSeg = MRS_CCO_UPG_ERROR_SEG;
            break;
        }

        ulSeg = pstMrsUpgCtx->ulCurrSegNum;
    } while (0);

    mrsDfxTxUpgFrameNum();
    ret = mrs3762_EasyEnc((HI_U8 *)&ulSeg, (HI_U16)sizeof(HI_U32), param);
/* END:   Added by fengxiaomin/00209182, 2014/1/10 */

    return ret;
}
#endif
#endif


/* BEGIN: Added by fengxiaomin/00209182, 2014/1/10   ���ⵥ�ź�:DTS2014012607684 */
//*****************************************************************************
// ��������: mrsGetCcoCodeVer
// ��������: ��ȡCCO�汾��Ϣ
//
// ����˵��:
//    pVer �汾��
//
// �� �� ֵ:
//    �汾��
//
// ����Ҫ��:
// ���þ���:
// ��    ��: fengxiaomin/00209182 [2014-1-10]
//*****************************************************************************
HI_U16 mrsGetCcoCodeVer(HI_CHAR *pVer)
{
    HI_U16 usVVer = 0;
    HI_U16 usRVer = 0;
    HI_U16 usSpcVer = 0;
    HI_U16 usVer = 0;  //D15~D12:V�汾; D11~D8:R�汾; D7~D0:SPC�汾

    if (!pVer)
    {
        return 0;
    }

    usVVer = mrsGetVRSpcVer(pVer, MRS_V_VERSION_STRING);
    usRVer = mrsGetVRSpcVer(pVer, MRS_R_VERSION_STRING);
    usSpcVer = mrsGetVRSpcVer(pVer, MRS_SPC_VERSION_STRING);

    usVer = mrsMappedCodeVer(usVVer, usRVer, usSpcVer);

    return usVer;
}

//*****************************************************************************
// ��������: mrsGetVRSpcVer
// ��������: ��ȡV/R/SPC�汾��
//
// ����˵��:
//    pVer �汾��
//    pStr Ҫ���ҵİ汾�ű�־
//
// �� �� ֵ:
//    �汾��
//
// ����Ҫ��:
// ���þ���:
// ��    ��: fengxiaomin/00209182 [2014-1-10]
//*****************************************************************************
HI_U16 mrsGetVRSpcVer(HI_CHAR *pVer, HI_CHAR* pStr)
{
    HI_CHAR *pData = HI_NULL;
    HI_U16 usVer = 0;

    pData = mrsFindStrEnd(pVer, pStr);
    if (pData)
    {
        usVer = (HI_U16)mrsStrToInt(pData, MRS_CCO_ALGORISM_TYPE);
    }

    return usVer;
}

HI_U16 mrsMappedCodeVer(HI_U16 usVVer, HI_U16 usRVer, HI_U16 usSpcVer)
{
    HI_U16 usCodeVer = 0;
    HI_U8 ucVersion = 0;

    ucVersion = (HI_U8)((usVVer/100 - 1) % 9 + 1);
    usCodeVer = (HI_U16)(ucVersion << 12);

    ucVersion = (HI_U8)((usRVer - 1) % 15 + 1);
    usCodeVer |= (HI_U16)(ucVersion << 8);

    ucVersion = usSpcVer % 256;
    usCodeVer |= (HI_U16)ucVersion;

    return usCodeVer;
}
/* END:   Added by fengxiaomin/00209182, 2014/1/10 */

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
//*****************************************************************************
// ��������: mrsWhetherAddNode
// ��������: �ж��Ƿ�Ӧ��Ӵӽڵ�
//
// ����˵��:
//   pucContent [in] AFN11-F1�����ݵ�Ԫ
//   ucMeterNum [in] Ҫ��ӵĵ������
//   stDenyData [in/out] ����֡�еĴ���״̬��
//
// �� �� ֵ:
//   HI_ERR_SUCCESS Ӧ��Ӵӽڵ�
//   HI_ERR_BAD_DATA ��ַӳ��������ȡʧ��
//   HI_ERR_FULL �����ռ䲻�㣬�����
//   HI_ERR_INITILIZATION_ALREADY ����ظ��������
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/f00209182 [2012-06-05]
//*****************************************************************************
HI_U32 mrsWhetherAddNode(HI_U8 * pucContent, HI_U8 ucMeterNum, AFN_00_F2 *stDenyData)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U32 usIsBCD = HI_FALSE;
    HI_U8 ucMeterIndex = 0;
    HI_U8 ucUnitConstLen = 0;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_U8 ucMaxAddNum = MRS_3762_ADD_METERS_MAX_NUM;

    *stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;

    ucUnitConstLen = GetSecondaryNodeItemSize(pstCcoCtx);

    if (HI_NULL == pucContent) //����ж�
    {
        ret = HI_ERR_BAD_DATA;

        return ret;
    }

    if (ucMeterNum > ucMaxAddNum)
    {
        ret = HI_ERR_FAILURE;

        return ret;
    }

    while (ucMeterIndex < ucMeterNum)
    {
        HI_U8 aucAddr[HI_METER_ADDR_LEN] = {0};
        HI_U8 ucProtocol = *(pucContent + ucMeterIndex * ucUnitConstLen + ucUnitConstLen - 1);

        mrsMeterAddrPreProc(aucAddr, pucContent + ucMeterIndex * ucUnitConstLen, ucProtocol);

        //����Ƿ���BCD��
        usIsBCD = mrsToolsCheckBCD(aucAddr, HI_METER_ADDR_LEN);
        if (usIsBCD != HI_TRUE)
        {
            ret = HI_ERR_BAD_DATA;
            *stDenyData = MRS_13762_DENY_STATUS_ERROR_FORMAT; //��ʽ����
            return ret;
        }

        //����Ƿ��ǹ㲥��ַ
        usIsBCD = mrsToolsBroadCastAddr(aucAddr);
        if (usIsBCD == HI_TRUE)
        {
            ret = HI_ERR_BAD_DATA;
            *stDenyData = MRS_13762_DENY_STATUS_ERROR_FORMAT; //��ʽ����
            return ret;
        }

        ret = mrsCheckProtocolValid(ucProtocol);
        if (ret != HI_ERR_SUCCESS)
        {
            *stDenyData = MRS_13762_DENY_STATUS_ERROR_FORMAT; //��ʽ����
            ret = HI_ERR_BAD_DATA;//��Լ���͸�ʽ����
            return ret;
        }

        ucMeterIndex++;
    }

    if (mrsSrvArchivesMeterNum() + ucMeterNum > PRODUCT_CFG_MRS_MAX_NODE_NUM) //�������Ƿ�����
    {
        ret = HI_ERR_FULL;
        *stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;

        return ret;
    }

    for (ucMeterIndex = 0; ucMeterIndex < ucMeterNum; ucMeterIndex++)
    {
        HI_U16 usMeterTotalNum = 0;
        HI_U8 aucMeterAddr[HI_METER_ADDR_LEN];
        HI_U8 aucTempAddr[HI_METER_ADDR_LEN];

        ret = mrsIsExistRepeatMeter(pucContent, ucMeterIndex, ucMeterNum);
        if (ret != HI_ERR_SUCCESS)
        {
            *stDenyData = MRS_13762_DENY_STATUS_REPETITION_METER;//Ҫ��ӵı��д����ظ��ı��
            ret = HI_ERR_INITILIZATION_ALREADY;
            break;
        }

        mrsMeterAddrPreProc(aucMeterAddr, pucContent + ucMeterIndex * ucUnitConstLen,
            *(pucContent + ucMeterIndex * ucUnitConstLen + ucUnitConstLen - 1));

        //ֻ��ѯ�����е���Ч��
        while (usMeterTotalNum < mrsSrvArchivesMeterNum())
        {
            MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;

            pstNode = mrsSrvArchivesQuery(usMeterTotalNum);
            if (pstNode == HI_NULL)
            {
                return HI_ERR_FAILURE;
            }

            if(pstNode->bValid)
            {
                mrsMeterAddrPreProc(aucTempAddr, pstNode->aucNodeAddr, pstNode->ucProtocol);
                if (memcmp(aucMeterAddr, aucTempAddr, HI_METER_ADDR_LEN) == 0)
                {
                    *stDenyData = MRS_13762_DENY_STATUS_REPETITION_METER;//����ظ�
                    ret = HI_ERR_INITILIZATION_ALREADY;

                    return ret;
                }
            }

            usMeterTotalNum++;
        }
    }

    return ret;
}

//*****************************************************************************
// ��������: mrsIsExistRepeatMeter
// ��������: �ж�Ҫ��ӵı����Ƿ�����ظ����
//
// ����˵��:
//   pucContent [in] AFN11-F1�����ݵ�Ԫ
//   ucMeterIndex [in] ���
//   ucMeterNum [in] Ҫ��ӵĵ������
//
// �� �� ֵ:
//   HI_ERR_SUCCESS �������ظ����
//   HI_ERR_FAILURE �����ظ����
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/f00209182 [2012-11-02]
//*****************************************************************************
HI_U32 mrsIsExistRepeatMeter(HI_U8 * pucContent, HI_U8 ucMeterIndex, HI_U8 ucMeterNum)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U16 usIndex = 0;
    HI_U8 ucUnitConstLen = GetSecondaryNodeItemSize(mrsCcoGetContext());
    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN]= {0};
    HI_U8 aucTempAddr[HI_METER_ADDR_LEN]= {0};

    if (pucContent == HI_NULL)
    {
        ret = HI_ERR_BAD_DATA;

        return ret;
    }

    mrsMeterAddrPreProc(aucMeterAddr, pucContent + ucMeterIndex * ucUnitConstLen,
        *(pucContent + ucMeterIndex * ucUnitConstLen + ucUnitConstLen - 1));

    for (usIndex = ucMeterIndex + 1; usIndex < ucMeterNum; usIndex++)
    {
        mrsMeterAddrPreProc(aucTempAddr, pucContent + usIndex * ucUnitConstLen,
            *(pucContent + usIndex * ucUnitConstLen + ucUnitConstLen - 1));

        if (memcmp(aucMeterAddr, aucTempAddr, HI_METER_ADDR_LEN) == 0)
        {
            ret = HI_ERR_FAILURE; //Ҫ��ӵı��д����ظ����
            break;
        }
    }

    return ret;
}

//*****************************************************************************
// ��������: mrsCheckProtocolValid
// ��������: ����Լ�����Ƿ���Ч
//
// ����˵��:
//   ucProto [in] ��Լ����
//
// �� �� ֵ:
//   HI_ERR_FAILURE  ��Լ������Ч
//   HI_ERR_SUCCESS  ��Լ������Ч
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/f00209182 [2012-07-03]
//*****************************************************************************
HI_U32 mrsCheckProtocolValid(HI_U8 ucProto)
{
    return (mrsCheckValidProto(ucProto)) ? HI_ERR_SUCCESS : HI_ERR_FAILURE;
}

// AFNF0 F1: PLC����֡
HI_U32 mrs3762_AFNF0_F1(HI_INOUT HI_VOID * param)
{
    AFN_FN_UL_PARAM *afnParam = (AFN_FN_UL_PARAM *)param;
    MRS_3762_FRAME_STRU *frame = (MRS_3762_FRAME_STRU *)afnParam->pstFrame;
    AFN_00_F2 deny = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;
    HI_PBYTE content = afnParam->pContent;
    HI_U16 contentLen = afnParam->usContentLen;
    HI_U32 ret = HI_ERR_SUCCESS;

    do
    {
        HI_PBYTE payload = HI_NULL;
        HI_U16 payloadLen = 0;
        HI_U16 headLen = 4; // down_len(2) + up_len(2)
        HI_U16 dataLen = 0;
        HI_U16 expectLen = 0;

        MRS_PLC_FRAME_DATA_S plc;
        MRS_TEST_TRANSMIT_AFNF0_STRU *trans = HI_NULL;

        (hi_void)memcpy_s(&dataLen, sizeof(dataLen), content + 2, sizeof(HI_U16));
        (hi_void)memcpy_s(&expectLen, sizeof(expectLen), content + 4, sizeof(HI_U16));
        if (contentLen != headLen + 2 + dataLen)
        {
            ret = HI_ERR_BAD_DATA;
            deny = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }

        payloadLen = sizeof(MRS_TEST_TRANSMIT_AFNF0_STRU) + dataLen;
        payload = mrsToolsMalloc(payloadLen);
        if (!payload)
        {
            ret = HI_ERR_MALLOC_FAILUE;
            break;
        }

        (hi_void)memset_s(payload, payloadLen, 0, payloadLen);

        trans = (MRS_TEST_TRANSMIT_AFNF0_STRU *)payload;
        trans->interface_ver = MRS_TEST_AFNF0_DL_VER;
        trans->stru_len = sizeof(MRS_TEST_TRANSMIT_AFNF0_STRU);
        trans->data_len = dataLen;
        trans->expect_len = expectLen;
        (hi_void)memcpy_s(trans->data, trans->data_len, content + headLen + 2, dataLen);

        (hi_void)memset_s(&plc, sizeof(plc), 0, sizeof(plc));
        plc.usId = ID_MRS_CMD_TEST_TRANSMIT_AFNF0_PLC;
        plc.pucPayload = payload;
        plc.usPayloadLen = payloadLen;
        (hi_void)memcpy_s(plc.ucMac, sizeof(plc.ucMac), frame->strUsrDataHrd.stAddrRealm.ucDestAdd, HI_PLC_MAC_ADDR_LEN);
        mrsHexInvert(plc.ucMac, HI_PLC_MAC_ADDR_LEN);

        ret = MRS_SendPlcFrame(&plc);

        mrsToolsFree(payload);
    } while (0);

    if (HI_ERR_SUCCESS != ret)
    {
        ret = mrs3762ProtoDenyFrame(&deny, afnParam, afnParam->ppucOutBuffer, afnParam->pusBufferLen);
    }

    return ret;
}

// AFNF0 F2: ���������֡
HI_U32 mrs3762_AFNF0_F2(HI_INOUT HI_VOID * param)
{
    AFN_FN_UL_PARAM *afnParam = (AFN_FN_UL_PARAM *)param;
    HI_PBYTE pContent = afnParam->pContent;
    HI_U32 ret = HI_ERR_FAILURE;

    do
    {
        EQUIP_ZERO_CROSS_REQ_STRU req;

        if (afnParam->usContentLen != sizeof(EQUIP_ZERO_CROSS_REQ_STRU) + 2)
        {
            ret = HI_ERR_INVALID_PARAMETER;
            break;
        }

        (hi_void)memcpy_s(&req, sizeof(req), pContent + 2, sizeof(req));
        if ((req.level > 1) || (req.phase > 2)) {
            ret = HI_ERR_INVALID_PARAMETER;
            break;
        }

        mrs_zero_cross_set_info(req.phase | (req.edge << 2));

        ret = HI_MDM_CollectNtb(HI_TRUE, req.level, req.phase + PHASE_A, req.edge, mrsZeroCrossCheckNotify);
        if (HI_ERR_SUCCESS != ret)
        {
            break;
        }

        MRS_StartTimer(MRS_CCO_TIMER_ZERO_CROSS, MRS_STA_TIME_ZERO_CROSS, HI_SYS_TIMER_ONESHOT);

        return HI_ERR_SUCCESS;
    } while (0);

    mrsDut3762Reply_AFNF002(ret, HI_NULL);
    return ret;
}
#if defined(MRS_SGD_INNER_VER)
HI_U32 mrs3762_AFNF0_F229(HI_INOUT HI_VOID * param)
{
   	HI_U32 ret = HI_ERR_SUCCESS;
//	MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
	HI_U8 buf[64] = {0}; 
	FLASH_USR_CFG_INFO_S stCfg;

	ret = mrsGetUserVerNvCfg((NV_MRS_USER_VER_S*)buf);
    	ret = mrsSgdFlashRead(ID_FLASH_USR_CFG,&stCfg,sizeof(stCfg));
	if(ret == HI_ERR_SUCCESS){	
		(hi_void)memcpy_s(buf+40, 2, &stCfg.aucCode, 2);
	}
	ret = mrs3762_EasyEnc(buf,42,param);
	return ret;
}

HI_U32 mrs3762_AFNF0_F230(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 * pContent;
    HI_U16 usContentLen = 0;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//����֡�еĴ���״̬��
    //MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
	NV_MRS_USER_VER_S stVer;
	HI_CHAR aucManuCode[3] = {0};

    pContent = pstAfnParam->pContent;
    usContentLen = pstAfnParam->usContentLen;

	do
	{
		if (usContentLen != 44)
      		{
            		ret = HI_ERR_BAD_DATA;
            		stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            		break;
        	}
		(hi_void)memset_s(&stVer, sizeof(stVer),0,  sizeof(stVer));
		(hi_void)memcpy_s(&stVer, sizeof(stVer), pContent+2, 40);
		(hi_void)memcpy_s(aucManuCode,sizeof(aucManuCode), pContent+42, 2);
		(hi_void)memcpy_s(stVer.reserved+21, 2,aucManuCode,2);
		ret = MRS_TimeBinIsValid(stVer.aucDate);
    		if (ret != HI_ERR_SUCCESS)
    		{
    			HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_049, HI_DIAG_MT("date err="),ret);
        		break;
    		}
		ret = mrsNVSetClientId(aucManuCode);
		if (ret != HI_ERR_SUCCESS)
    		{
    			HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_053, HI_DIAG_MT("set client err="),ret);
        		break;
    		}

		ret = mrsNVSetUserVerInfo(&stVer);
        	if (HI_ERR_SUCCESS != ret)
        	{
            		HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_054, HI_DIAG_MT("��ERR��Set UserInfo =%d"),ret);
            		break;
        	}
		 //��֯ȷ��֡
        mrs3762ProtoAckFrame(0, pstAfnParam,pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
	}while(0);
	 if (ret != HI_ERR_SUCCESS)
    	{
        	ret = mrs3762ProtoDenyFrame(&stDenyData,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
    	}
    return ret;
}
HI_U32 mrs3762_AFNF0_F231(HI_INOUT HI_VOID * param)
{
	HI_U32 ulRet = HI_ERR_SUCCESS;
    EQUIP_SW_VER_ID_REQ_ACK_STRU stSWVerInfo;
	//HI_U8 ucVer1= 0;		
	//HI_U8 ucVer2 = 0;
    (hi_void)memset_s(&stSWVerInfo, sizeof(stSWVerInfo),0, sizeof(stSWVerInfo));

    do
    {
        if (HI_ERR_SUCCESS != mrsSrvGetInnerVer(stSWVerInfo.aucSwInnerVer))
        {
            stSWVerInfo.rltCode = 2;
            break;
        }
		//ucVer1 = stSWVerInfo.aucSwInnerVer[0];
		//ucVer2 = stSWVerInfo.aucSwInnerVer[1];
        //stSWVerInfo.aucSwExtVer =  ((HI_U16)(((ucVer1 >> 1) & 0xf0) | ((ucVer1 & 0x1f)/10)) << 8) | ((((ucVer1 & 0x1f)%10) << 4) |( ucVer2 & 0xf));
        stSWVerInfo.aucSwExtVer = (HI_U16)EXT_VER;
    } while (0);

	ulRet = mrs3762_EasyEnc((HI_U8 *)&stSWVerInfo,sizeof(stSWVerInfo),param);
	return ulRet;
}
#endif
// AFNF1 F1: ������������֡����
HI_U32 mrs3762_AFNF1_F1(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    HI_U8 * app_data = pstAfnParam->pContent;
    HI_U16 app_data_len = pstAfnParam->usContentLen;
    AFN_00_F2 deny = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//����֡�еĴ���״̬��

    mrsDfxPrUartRx();
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014032906596 */
    mrsCcoDfxRmUartRx(app_data, app_data_len, HI_FALSE);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */

    do
    {
        HI_U8 *data = HI_NULL;
        HI_U16 data_len = 0;
        HI_U8 pos = 0;
        HI_U8 protocol = 0;
        MRS_3762_FRAME_STRU * frame_data = 0;

        // ��ȡCCO����״̬(HI_TRUE : �Ѿ������Զ���; HI_FALSE : δ����)
/* BEGIN: Added by fengxiaomin/00209182, 2014/1/10   ���ⵥ�ź�:DTS2014012607684 */
        if(mrsGetCcoReadyStatus(HI_TRUE) == HI_FALSE)
/* END:   Added by fengxiaomin/00209182, 2014/1/10 */
        {
            ret = HI_ERR_BUSY;
            deny = MRS_13762_DENY_STATUS_CCO_BUSY;

            break;
        }

        // ��Լ����1Byte + �ӽڵ㸽���ڵ�����1Byte + ���ĳ���2Byte + Fn
        if (app_data_len < 6)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_045, HI_DIAG_MT("len<5!"));
            ret = HI_ERR_BAD_DATA;
            deny = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }

        pos += 2;     // Fn

        //����Լ�����Ƿ���Ч
        ret = mrsCheckProtocolValid(app_data[pos]);
        if (ret != HI_ERR_SUCCESS)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_046, HI_DIAG_MT("protocol err"));
            deny = MRS_13762_DENY_STATUS_ERROR_FORMAT;
            ret = HI_ERR_BAD_DATA;
            break;
        }

        protocol = app_data[pos++]; // ��ȡ��Լ����

        // ʣ�೤�� < �ӽڵ㸽���ڵ���Ϣ + ���ĳ���
        if((app_data_len - pos) < (app_data[pos] * 6 + 2))
        {
            ret = HI_ERR_BAD_DATA;
            deny = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_047, HI_DIAG_MT("extra_len<node_len+data_len"));
            break;
        }

        // ƫ�ƴӽڵ㸽���ڵ���Ϣ(��ǰ���������ڵ���Ϣ)
        pos = (HI_U8)(pos + app_data[pos] * 6);
        pos++;

        // ��ȡ645�������ı��ĳ���
        data_len = (HI_U16)(app_data[pos]+ (HI_U16)app_data[pos+1] * 256);
        pos += 2;

        if((app_data_len - pos) < data_len)
        {
            ret = HI_ERR_BAD_DATA;
            deny = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_048, HI_DIAG_MT("645 frame len err"));
            break;
        }

        //�ж�645�����������Ƿ񳬹�����2000�ֽ�
        if (data_len > MRS_AFNF1_F1_645_MAX_LEN)
        {
            ret = HI_ERR_BAD_DATA;
            deny = MRS_13762_DENY_RM_645ID_OVERRUN;
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_049, HI_DIAG_MT("645 frame len too large"));
            break;
        }

        data = &app_data[pos];   // ��ȡת������

        frame_data = pstAfnParam->pstFrame; // ��ȡ����֡��Ϣ

        // ǿ���л�Ϊ·����ͣ״̬
        mrsForceSwitchPause();

        HI_MDM_SetTfNtbSendProcessCtrl(HI_MDM_NTB_SEND_FREQUENCY_REDUCE, HI_MDM_NTB_SEND_CTRL_REASON_READ_METER);

        // �����泭����ģ����ж���
        ret = mrsParallelReadMeterProc(frame_data, protocol, data, data_len);

        switch(ret)
        {
        case HI_ERR_FULL:   // ����������
        case HI_ERR_NO_MORE_MEMORY: // ϵͳû���ڴ���
            deny = MRS_13762_DENY_RM_BUFPOOL_FULL;
            break;
        case HI_ERR_NOT_SUPPORT:
            deny = MRS_13762_DENY_RM_645ID_OVERRUN;
            break;
        case HI_ERR_BUSY:
            deny = MRS_13762_DENY_RM_METER_BUSY;
            break;

        case HI_ERR_SUCCESS:
            mrsDetectRmBreakProc(); // Pause detect rm.
            break;

        default:
            deny = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;
            break;
        }
    } while (0);

    if(ret != HI_ERR_SUCCESS)
    {
        // ��֯����֡
        mrs3762ProtoDenyFrame(&deny,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014032906596 */
        mrsDfxRmUartDenyTx(app_data, app_data_len);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
        mrsDfxPrNakCnt((HI_U8)deny);

        HI_MDM_SetTfNtbSendProcessCtrl(HI_MDM_NTB_SEND_PROCESS_RESUME, HI_MDM_NTB_SEND_CTRL_REASON_READ_METER);
    }

    return HI_ERR_SUCCESS;
}
//AFN20 F1����������ˮ����������
HI_U32 mrs3762_AFN20_F1(HI_INOUT HI_VOID * param)
{
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//����֡�еĴ���״̬��

    do
    {
        if (mrsIsCcoUpgStatus())
        {
            ret = HI_ERR_BUSY;
            stDenyData = MRS_13762_DENY_STATUS_CCO_BUSY;
            break;
        }
		if (pstCcoCtx->status & MRS_CCO_STATUS_SEARCH_METER)
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_090, HI_DIAG_MT("[AFN20_F1]�ѱ���...[%d]"), pstCcoCtx->status);
            ret = HI_ERR_BUSY;
            stDenyData = MRS_13762_DENY_STATUS_CCO_BUSY;
            break;
        }
        else if (pstCcoCtx->status & MRS_CCO_STATUS_SEARCH_METER_TRANSFORMER)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_091, HI_DIAG_MT("[AFN20_F1]̨��ʶ���ѱ���..."));
            ret = HI_ERR_BUSY;
            stDenyData = MRS_13762_DENY_STATUS_CCO_BUSY;
            break;
        }

        if (HI_FALSE == pstCcoCtx->bPauseFlag)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_092, HI_DIAG_MT("[AFN20_F1]��·����ͣ"));
            ret = HI_ERR_BUSY;
            stDenyData = MRS_3762_DENY_STATUS_COMMUNICATION_OVERTIME;
            break;
        }
		/*
		if (0 == (pstCcoCtx->status & MRS_CCO_STATUS_NORMAL))
		{
			HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_093, HI_DIAG_MT("[AFN20_F1] NOT MRS_CCO_STATUS_NORMAL"));
			ret = HI_ERR_BUSY;
            stDenyData = MRS_13762_DENY_STATUS_CCO_BUSY;
		}
		*/

    } while (0);
    if (HI_ERR_SUCCESS == ret)
    {
        mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
		
		HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_068, HI_DIAG_MT("mrs3762_AFN20_F1"),pstCcoCtx->stReadWaterMeterModel.enCurrRfState);
		if(pstCcoCtx->stReadWaterMeterModel.enCurrRfState==0)
		{
			
			pstCcoCtx->stReadWaterMeterModel.usCurrRfNodeSeq=0;
			pstCcoCtx->stReadWaterMeterModel.enCurrRfState =1;
			MRS_StartTimer(MRS_CCO_TIMER_RF_READ_WATERMETER, 10*1000, HI_SYS_TIMER_ONESHOT);
			mrsReadWaterMeterStateInit();
			mrsReportCcoUartRecvWaterMeterData();
		}
	}
    else
    {
        //��֯����֡
       ret = mrs3762ProtoDenyFrame(&stDenyData, pstAfnParam, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
    }

    return ret;
}


//AFN20 F2��ֹͣ����ˮ����������
HI_U32 mrs3762_AFN20_F2(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    //HI_U32 status = mrsCcoGetState();

    // ֹͣ�·�����
	//�����������ͣ����
	
	if(pstCcoCtx->stReadWaterMeterModel.enCurrRfState != EN_MRS_SRV_CCO_READMETER_IDLE)
	{
		// �����ǰ���ǿ���״̬���������״̬(��ǰ�ڵ㲻�仯)
		MRS_StopTimer(MRS_CCO_TIMER_RF_READ_WATERMETER);
		pstCcoCtx->stReadWaterMeterModel.enCurrRfState =0;

		if(pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl != HI_NULL)
		{
		    if(pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl->pucPayload != HI_NULL)
		    {
		        mrsToolsFree(pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl->pucPayload);   // �ͷ�ת�����ݿ�   
		        pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl->pucPayload = HI_NULL;
		    }
		    
		    mrsToolsFree(pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl);   // �ͷ�PLC֡����    
		    pstCcoCtx->stReadWaterMeterModel.pstRfPlcDataDl = HI_NULL;
		}
		if (pstCcoCtx->stReadWaterMeterModel.pstRfUpReadMeterData != HI_NULL)
		{
		    mrsToolsFree(pstCcoCtx->stReadWaterMeterModel.pstRfUpReadMeterData);   // �ͷ��ڴ�
		}
	}

    //��֯ȷ��֡
    mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);

    return ret;
}
HI_U32 mrs3762_ReportAFN20_F4(HI_VOID)
{
    PROTO376_2_FRAME_AFN_AND_DATA_STRU stAfnData;
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_PBYTE buf = HI_NULL;
    HI_U16 len = 0;
	MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext(); // ����ָ��ȫ�������Ľṹ��ָ��
	pstCcoCtx->stReadWaterMeterModel.enCurrRfState=0;

    do
    {
        MRS_SRV_CCO_ITEM *pstCcoItem = HI_NULL;
        mrsToolsMemZero_s(&stAfnData, sizeof(stAfnData), sizeof(stAfnData));
        stAfnData.ucAFN = MRS_AFN(0x20); // AFN=20H
        stAfnData.ucDT[0] = MRS_AFN_FN_LO(4);
        stAfnData.ucDT[1] = MRS_AFN_FN_HI(4);
        stAfnData.pData = 0;
        stAfnData.usDataLen = 0;

        ret = mrsCreate3762UpFrame(&stAfnData, &buf, &len, HI_NULL, HI_NULL);
        if (HI_ERR_SUCCESS != ret)
        {
            break;
        }

        pstCcoItem = (MRS_SRV_CCO_ITEM *)mrsToolsMalloc(sizeof(MRS_SRV_CCO_ITEM) + len);
        if (!pstCcoItem)
        {
            mrsDfxCcoQueueMallocFailCnt();
            break;
        }

        mrsToolsMemZero_s(pstCcoItem, sizeof(MRS_SRV_CCO_ITEM) + len, sizeof(MRS_SRV_CCO_ITEM) + len);
        pstCcoItem->bValid = HI_TRUE;
        pstCcoItem->ucAfn = 0x20;
        pstCcoItem->usFn = 0x04;
        pstCcoItem->usTimeOut = 1000;//��λΪms
        pstCcoItem->ucMaxTry = 2;
        pstCcoItem->usDataLen = len;
        mrsToolsMemcpy_s(pstCcoItem->pData, pstCcoItem->usDataLen, buf, len);
        pstCcoItem->MrsCcoRxRespProc = HI_NULL;
        pstCcoItem->MrsCcoRxTotalTimeOutProc = HI_NULL;
        pstCcoItem->MrsCcoRxEveryTimeOutProc = HI_NULL;

        mrsCcoJoinQueue(pstCcoItem);

        mrsActiveCcoQueue();
    } while (0);

    mrsToolsFree(buf);

    return ret;
}
#endif

