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

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
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
    {0x00000000, mrs3762_AFN00_F1},
    {0x00000001, mrs3762_AFN00_F2},

    {0x00000100, mrs3762_AFN01_F1},
    {0x00000101, mrs3762_AFN01_F2},
    {0x00000102, mrs3762_AFN01_F3},

    {0x00000200, mrs3762_AFN02_F1},

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

    {0x00000400, mrs3762_AFN04_F1},
    {0x00000401, mrs3762_AFN04_F2},
    {0x00000402, mrs3762_AFN04_F3},

    {0x00000500, mrs3762_AFN05_F1},
    {0x00000501, mrs3762_AFN05_F2},
    {0x00000502, mrs3762_AFN05_F3},
    {0x00000503, mrs3762_AFN05_F4},

    {0x00001000, mrs3762_AFN10_F1},
    {0x00001001, mrs3762_AFN10_F2},
    {0x00001002, mrs3762_AFN10_F3},
    {0x00001003, mrs3762_AFN10_F4},
    {0x00001004, mrs3762_AFN10_F5},
    {0x00001005, mrs3762_AFN10_F6},
    {0x00001014, mrs3762_AFN10_F21},
    {0x00001063, mrs3762_AFN10_F100},
    {0x00001064, mrs3762_AFN10_F101},
    {0x000010E3, mrs3762_AFN10_F228},

    {0x00001100, mrs3762_AFN11_F1},
    {0x00001101, mrs3762_AFN11_F2},
    {0x00001102, mrs3762_AFN11_F3},
    {0x00001103, mrs3762_AFN11_F4},
    {0x00001104, mrs3762_AFN11_F5},
    {0x00001105, mrs3762_AFN11_F6},

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

    {0x0000F000, mrs3762_AFNF0_F1},

    {0x0000F001, mrs3762_AFNF0_F2},

    {0x0000F100, mrs3762_AFNF1_F1},
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

//��ǰ�Ƕ�̬���䣬���ڸ��ɾ�̬����
HI_PRV HI_U8 mrs3762AFN10F2Buffer[3+sizeof(AFN_10_F2_NODE_INFO)*MRS_3762_F10_NODE_MAX_NUM] = {0};
HI_PRV HI_U32 mrsCcoAddrWriteNv(HI_VOID);
HI_PRV HI_U16 mrsGetCurrTopoNum(HI_VOID);

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
/* BEGIN: Added by fengxiaomin/00209182, 2014/1/10   ���ⵥ�ź�:DTS2014012607684 */
HI_VOID mrsInitSustainAfn(HI_VOID)
{
    HI_U8 ucIndex = 0;
    HI_U8 ucAfnNum = sizeof(MRS_AFN_CAPABILITY)/sizeof(MRS_AFN_MAP);
    HI_U8 ucOffSet = 0;

    for (ucIndex = 0; ucIndex < ucAfnNum; ucIndex++)
    {
        (hi_void)memset_s(MRS_AFN_CAPABILITY[ucIndex].capability,sizeof(MRS_AFN_CAPABILITY[ucIndex].capability),
            0, sizeof(MRS_AFN_CAPABILITY[ucIndex].capability));
    }

    MRS_AFN_CAPABILITY[ucOffSet].capability[0] = 0x03;  //0x00  1,2    0000 0011
    ucOffSet++;
    MRS_AFN_CAPABILITY[ucOffSet].capability[0] = 0x07;  //0x01  1,2,3    0000 0111
    ucOffSet++;
    MRS_AFN_CAPABILITY[ucOffSet].capability[0] = 0x01;  //0x02  1       0000 0001
    ucOffSet++;
    MRS_AFN_CAPABILITY[ucOffSet].capability[0] = 0x7F;  //0x03  1,2,3,4,5,6,7     0111 1111
    MRS_AFN_CAPABILITY[ucOffSet].capability[1] = 0x07;  //0x03  9,10,11      0000 0111
    ucOffSet++;
    MRS_AFN_CAPABILITY[ucOffSet].capability[0] = 0x07;  //0x04  1,2,3        0000 0111
    ucOffSet++;
    MRS_AFN_CAPABILITY[ucOffSet].capability[0] = 0x0F;  //0x05  1,2,3,4        0000 1111
    ucOffSet++;
    MRS_AFN_CAPABILITY[ucOffSet].capability[0] = 0x1E;  //0x06  2,3,4,5        0001 1110
    MRS_AFN_CAPABILITY[ucOffSet].capability[1] = 0x00;  //0x06  10             0000 0000
    ucOffSet++;
    MRS_AFN_CAPABILITY[ucOffSet].capability[0] = 0x3F;  //0x10  1,2,3,4,5,6      0011 1111
    MRS_AFN_CAPABILITY[ucOffSet].capability[2] = 0x10;  //0x10  21        0001 0000 (24~17)
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014032708427 */
    MRS_AFN_CAPABILITY[ucOffSet].capability[12] = 0x18; //0x10  101,100      0001 1000 (104~97)
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
    MRS_AFN_CAPABILITY[ucOffSet].capability[28] = 0x08; //0x10  228      0000 1000 (232~225)
    ucOffSet++;
    MRS_AFN_CAPABILITY[ucOffSet].capability[0] = 0x3F;  //0x11  1,2,3,4,5,6      0011 1111
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/28   ���ⵥ��:DTS2014032708937 */
    MRS_AFN_CAPABILITY[ucOffSet].capability[12] = 0x00; //0x11  101      0000 0000
/* END:   Modified by fengxiaomin/00209182, 2014/3/28 */
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
/* END:   Added by fengxiaomin/00209182, 2014/1/10 */

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
    AFN_00_F1  stDefaultAck = {0};
    AFN_00_F1_NEW ack_new = {0};
    MRS_CCO_SRV_CTX_STRU * cco_ctx = mrsCcoGetContext();
    HI_U16 data_len = 0;
    HI_U8 *buf = HI_NULL;

    if ((ppucBuffer == HI_NULL) || (pusBufferLen == HI_NULL))
    {
        return HI_ERR_BAD_DATA;
    }

    if(cco_ctx->ucProtoVer != PROTO_VER_3762_2009)
    {
        buf = (HI_U8 *)&ack_new;
        data_len = sizeof(ack_new);
        ack_new.uSWitTime = waitTime;
    }
    else
    {
        buf = (HI_U8 *)&stDefaultAck;
        data_len = sizeof(stDefaultAck);
        stDefaultAck.uSWitTime = waitTime;
    }

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
    MRS_CCO_SRV_CTX_STRU * pstCtx = mrsCcoGetContext();

    if ((ppucBuffer == HI_NULL) || (pusBufferLen == HI_NULL) || (!pstDlFrameInf))
    {
        return HI_ERR_BAD_DATA;
    }

    if (pstDenyFrame == HI_NULL)
    {
        pstDenyFrame = &stDefaultAck;
    }

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
//
// �� �� ֵ:
//    HI_ERR_SUCCESS ��ʾ�ɹ�������Ϊ������
//
// ����Ҫ��:
// ���þ���:
// ��    ��: niesongsong/nKF62735 [2011-12-17]
//*****************************************************************************
HI_U32 mrs3762AfnDispatch(HI_IN  MRS_3762_FRAME_STRU * pstFrame,
                         HI_OUT HI_PBYTE * ppOutBuffer,
                         HI_OUT HI_U16 * pusBufferLen)
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

            //����
            ret = pAfnFnProc(&param);

            if (MRS_CCO_PROTOCOL_AFN_13 == param.ucAfn)
            {
                mrsRemoteUpgRxFilePro();
            }
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
    HI_U32 low, high, mid;

    low = 0;
    if (ulProcIndex == AFN_FUNC_MAP_ARRAY[low].afn_data)
    {
        return AFN_FUNC_MAP_ARRAY[low].func;
    }

    high = sizeof(AFN_FUNC_MAP_ARRAY)/sizeof(AFN_FUNC_MAP_ARRAY[0]) - 1;
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
        if ((msk[pos] & dt[0]) == msk[pos])
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
#endif

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


#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
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

//AFN00 F1:ȷ��
HI_U32 mrs3762_AFN00_F1(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    P_MRS_SRV_CCO_QUEUE pstCcoQueue = mrsGetCcoQueue();
    MRS_SRV_CCO_ITEM *pstCcoItem = HI_NULL;
    MRS_SRV_CCO_ITEM *pstCcoItemTemp = HI_NULL;

    MRS_NOT_USED(param); 
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

    mrsDfxCcoQueueRxAckCnt();
    
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

//AFN00 F2:����
HI_U32 mrs3762_AFN00_F2(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    P_MRS_SRV_CCO_QUEUE pstCcoQueue = mrsGetCcoQueue();
    MRS_SRV_CCO_ITEM *pstCcoItem = HI_NULL;
    MRS_SRV_CCO_ITEM *pstCcoItemTemp = HI_NULL;

    MRS_NOT_USED(param);
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

    mrsDfxCcoQueueRxDenyCnt();
    
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
        ret = mrsAnyTimeReadMeterProcess(pstUsrDataHrd->stAddrRealm.ucDestAdd, ucProtocolType, pucData, ucDataLen);
        if (HI_ERR_SUCCESS != ret)
        {
            stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;
        }
    } while (0);

    if(ret != HI_ERR_SUCCESS)
    {
        HI_U8 ucFrom = pstCcoCtx->stAnytimeRMModel.from;
        
        // �����ԴΪ "����ز��ӽڵ�"
        pstCcoCtx->stAnytimeRMModel.from = XR_FROM_AFN02_F1;
        ret = mrsAFN13F1CreateAckFrame(pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);  

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

    stStatusWord.Mode = PERIOD_METER_READ_MODE_CCO;

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

        if (protocol > MRS_STA_PROTOCOL_2007)
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
        
        buf[offset++] = 0xB2;   /* D0~D3 : 2  (����������ز�ͨѶ)
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

        ret = mrsCreate3762UpFrame(&stAfnData, &out_buf, &out_len, HI_NULL);
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
        // TODO: ����淶�ϱ��������������ʵ�ʲ��Ա��Ŀ��ܻ�Բ���
        HI_U16 head_len = 9; // ͨ������(1)+Ŀ���ַ(6)+ͨ��Э������(1)+���ĳ���(1)
        HI_U16 data_len = (HI_U16)pContent[head_len - 1 + 2];
/* BEGIN: PN: DTS2015040900760 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/7 */
        HI_U8 aucAddr[HI_METER_ADDR_LEN];
/* END:   PN: DTS2015040900760 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/7 */

        if (!mrsGetCcoReadyStatus(HI_TRUE))
        {
            ret = HI_ERR_BUSY;
            stDenyData = MRS_13762_DENY_STATUS_CCO_BUSY;
            break;
        }

        if (usContentLen != head_len + data_len + 2)
        {
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }

/* BEGIN: PN: DTS2015040900760 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/7 */
        mrsMeterAddrPreProc(aucAddr, pContent + 3, pContent[9]);

        // ����MAC��ַ
        ret = mrsMapGetMAC(aucAddr, stPlcData.ucMac);
        if (ret != HI_ERR_SUCCESS)
        {
            ret = HI_ERR_NOT_FOUND;
            stDenyData = MRS_13762_DENY_STATUS_INEXISTENT_METER;
            break;
        }
/* END:   PN: DTS2015040900760 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/7 */

        if ( (MRS_STA_PROTOCOL_1997 == pContent[9]) || (MRS_STA_PROTOCOL_2007 == pContent[9]))
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

//AFN05 F1�������ز����ڵ��ַ
HI_U32 mrs3762_AFN05_F1(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 * pContent;
    HI_U16 usContentLen = 0;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//����֡�еĴ���״̬��

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

        //�������ڵ��ַ
        (hi_void)memcpy_s(pstCcoCtx->ucMainNodeAddr, sizeof(pstCcoCtx->ucMainNodeAddr), pContent + 2, sizeof(pstCcoCtx->ucMainNodeAddr));
        mrsHexInvert(pstCcoCtx->ucMainNodeAddr, sizeof(pstCcoCtx->ucMainNodeAddr));
        HI_MDM_DevIdConfig(pstCcoCtx->ucMainNodeAddr);

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
            case MRS_STA_PROTOCOL_1997:
            case MRS_STA_PROTOCOL_2007:
                ret = mrsFind645Frame(pContent, (HI_S16)usContentLen, &pos, &frame_len);
                if (ret != HI_ERR_SUCCESS)
                {
                    ret = HI_ERR_BAD_DATA;
                    stDenyData = MRS_13762_DENY_STATUS_INVALID_DATA;
                    break;
                }

                break;

            case 0: // ͸��
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
            ret = mrs3762ProtoDenyFrame(&stDenyData,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
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
			if (pstMeterNode == HI_NULL)
			{
				continue;
			}
			
            if (pstMeterNode->bValid)
            {
                AFN_10_F2_NODE_INFO * pNode = (AFN_10_F2_NODE_INFO * )pCurrentNode;
                pCurrentNode += sizeof(AFN_10_F2_NODE_INFO);

                (hi_void)memset_s(pNode, sizeof(AFN_10_F2_NODE_INFO), 0, sizeof(AFN_10_F2_NODE_INFO));
                (hi_void)memcpy_s(pNode->ucNodeAddr, sizeof(pNode->ucNodeAddr), pstMeterNode->aucNodeAddr, HI_METER_ADDR_LEN);

                pNode->ucPhase = 1;
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
#endif

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

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    return (HI_BOOL)(mrsCcoIsTestMode() || pstCcoCtx->PlcChStatus);
#else
    return pstCcoCtx->PlcChStatus;
#endif
}

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
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

    (hi_void)memset_s(pBuffer, sizeof(pBuffer), 0, sizeof(pBuffer));
    ucWorkStatus = mrsGetCcoWorkFlag();
    if (MRS_CCO_NON_WORKING_STATUS != ucWorkStatus)
    {
        ucWorkFlag = MRS_CCO_WORKING_STATUS;
    }
    else
    {
        ucWorkStatus = 0;
    }

    route_flag = (HI_U8)mrsGetRouteFlag();

    pBuffer[0] = (HI_U8)(pstEvtCtx->bACDFlg << 2 | ucWorkFlag << 1 | route_flag); //����״̬��
    (hi_void)memcpy_s(&pBuffer[1], sizeof(pBuffer) - 1, &usMeterNum, 2);  //�ӽڵ�������
    (hi_void)memcpy_s(&pBuffer[3], sizeof(pBuffer) - 3, &pstCcoCtx->stReadMeterModel.usSuccNum, 2);  //�ѳ��ӽڵ�����
    pBuffer[7] = (HI_U8)(0x02 | (ucWorkStatus << 2));  //��������
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
        ret = mrsQueryTopoDownCheck(pstAfnParam, &usStartSeq, &ucQueryNum);
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

HI_U32 mrsQueryTopoDownCheck(AFN_FN_UL_PARAM *pstAfnParam, HI_U16 *pusStartSeq, HI_U8 *pucQueryNum)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 * pContent = HI_NULL;
    HI_U16 usContentLen = 0;
    HI_PRV HI_BOOL bQueryStartFlag = HI_FALSE;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_MAC_CONFIG_INFO_S mac_inf = {0};

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
            bQueryStartFlag = HI_TRUE;

            mac_inf.pTopo = pstCcoCtx->pTopoIntegerInfo;
            ret = HI_MDM_QueryInfo(&mac_inf);  // ��ȡmac���˱���Ϣ
        }

    } while (0);

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
    usTopoNum = (HI_U16)pstTopoInf->num;
    (hi_void)memcpy_s(pData, sizeof(HI_U16), &usTopoNum, sizeof(HI_U16));
    usOffset += sizeof(HI_U16) + sizeof(usStartSeq) + sizeof(ucReportNum);

    for (usIndex = usStartSeq; usIndex < usStartSeq + ucQueryNum; usIndex++)
    {
        if (pstTopoInf->num < usIndex)
        {
            break;
        }

        (hi_void)memcpy_s(pAddr, sizeof(pAddr), pstTopoInf->entry[usIndex-1].mac, HI_PLC_MAC_ADDR_LEN);
        mrsHexInvert(pAddr, sizeof(pAddr));
        (hi_void)memcpy_s(pData + usOffset, HI_PLC_MAC_ADDR_LEN, pAddr, HI_PLC_MAC_ADDR_LEN);
        usOffset += HI_PLC_MAC_ADDR_LEN;

        if (MRS_CCO_QUERY_TOPO_10_F228 == ucFrom)
        {
            HI_U8 pucDeviceAddr[HI_METER_ADDR_LEN] = {0};

            ulRet = mrsGetDeviceAddr(pstTopoInf->entry[usIndex-1].mac, 
                                     pstTopoInf->entry[usIndex-1].product_type, pucDeviceAddr);
            if (HI_ERR_SUCCESS == ulRet)
            {
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

    *(pData + MRS_CCO_TOPO_REPORT_NUM_OFFSET) = ucReportNum;
    if (ucReportNum)
    {
        (hi_void)memcpy_s(pData + MRS_CCO_TOPO_START_SEQ_OFFSET, sizeof(usStartSeq), &usStartSeq, sizeof(usStartSeq));
    }

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
        ret = mrsQueryTopoDownCheck(pstAfnParam, &usStartSeq, &ucQueryNum);
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
        if (mrsGetCcoPlcChlStatus())
        {
            pstCcoCtx->ucSearchWaitFlag = HI_FALSE;
            ulRet = mrsSearchMeterProc(pstCcoCtx);
        }
        else
        {
            pstCcoCtx->ucSearchWaitFlag = HI_TRUE;
        }

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
        if (!mrsGetCcoTimeFrist())
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
        if (!mrsGetCcoTimeFrist())
        {
            // ��ʱMRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUEʱ������¼������ģ��
            MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER, MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE * 3, HI_SYS_TIMER_ONESHOT);
        }
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
            if(!mrsIsDetectRm() && (pstCcoCtx->stAnytimeRMModel.enState == EN_MRS_ANYTIME_RM_WAIT_PLC))   // ֻ����PLC����ռ��״̬�²Ž�������
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
        // �����ԴΪ "����ز��ӽڵ�"
        pstCcoCtx->stAnytimeRMModel.from = XR_FROM_AFN13_F1;

        mrsDetectRmBreakProc();
        
        if (bDelayRelated)
        {
            ret = mrs3762_AFN14_F3_UP(pstAfnParam, MRS_AFN14_F3_DELAY_TIME, ucDataLen, pucData);
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
        ret = mrsAnyTimeReadMeterProcess(pstUsrDataHrd->stAddrRealm.ucDestAdd, ucProtocolType, pucData, ucDataLen);
        if (HI_ERR_SUCCESS != ret)
        {
            stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;
        }
    } while (0);

    if(ret != HI_ERR_SUCCESS)
    {      
        HI_U8 ucFrom = pstCcoCtx->stAnytimeRMModel.from;
        
        // �����ԴΪ "����ز��ӽڵ�"
        pstCcoCtx->stAnytimeRMModel.from = XR_FROM_AFN13_F1;
        //��װ����֡
        ret = mrsAFN13F1CreateAckFrame(pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);      
        pstCcoCtx->stAnytimeRMModel.from = ucFrom;
        
        mrsDfxRmUartDenyTx(pucContent, usContentLen);
        mrsDfxXrNakCnt();
        mrsDfxXrNak((HI_U8)stDenyData);
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
        if (pucContent[ucPos] > 2)
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

        if (ucDataLen != 0)
        {
            if (bDelayRelated)
            {
                // ����AFN=14H F3����
                ret = mrs3762_AFN14_F3_UP(pstAfnParam, MRS_STA_PROTOCOL_2007, ucDataLen, pucContent);
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

        ret = MRS_TimeIsValid(&pucContent[2]);
        if (ret != HI_ERR_SUCCESS)
        {
            ret = HI_ERR_BAD_DATA;
            break;
        }

        ret = mrsCcoSetClock(&pucContent[2]);
        if (ret != HI_ERR_SUCCESS)
        {
            break;
        }
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

    ret = mrsFind645Frame(pDataBuff, (HI_S16)usDataLen, &pos, &len);
    if (HI_ERR_SUCCESS != ret)
    {
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
    PROTO376_2_FRAME_PRE_STRU up_frame;

    // ���б�������: ���ַ(6) + ��ʱ(2) + ���ĳ���(1) + ��������
    payload_len = frame_len + 9;
    payload = mrsToolsMalloc(payload_len);
    if (HI_NULL == payload)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(payload, payload_len, 0, payload_len);

    if (mrsCheck645Proto(protocol))
    {
        HI_U16 pos;
        HI_U16 len;

        ret = mrsFind645Frame(frame_data, (HI_S16)frame_len, &pos, &len);
        if (ret != HI_ERR_SUCCESS)
        {
            // δ�ҵ�645֡�����ش���ֵ
            mrsToolsFree(payload);
            return ret;
        }

        (hi_void)memcpy_s(payload, payload_len, frame_data + pos + 1, HI_METER_ADDR_LEN);
    }
    else
    {
        MRS_3762_ADDR_REALM_STRU *realm = &(param->pstFrame->strUsrDataHrd.stAddrRealm);
        (hi_void)memcpy_s(payload, payload_len, realm->ucDestAdd, HI_METER_ADDR_LEN);
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

    if (param->pstFrame->strUsrDataHrd.stAppDataRealm.ucAfn == MRS_AFN(0x14))
    {
        up_frame.ucPrm = 1;
    }

    ret = mrsCreate3762Frame(&up_frame, param->ppucOutBuffer, param->pusBufferLen, payload);

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
    mrs3762_EasyEnc((HI_U8 *)&ulSeg, (HI_U16)sizeof(HI_U32), param);
/* END:   Added by fengxiaomin/00209182, 2014/1/10 */

    return ret;
}

#endif
#endif

/* BEGIN: Added by fengxiaomin/00209182, 2014/1/10   ���ⵥ�ź�:DTS2014012607684 */
//*****************************************************************************
// ��������: mrsGetSoftwareVer
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
HI_U16 mrsGetSoftwareVer(HI_CHAR *pVer)
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
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
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
    HI_U32 ret = HI_ERR_SUCCESS;

    if ((ucProto == 0x00) || (ucProto == 0x01) || (ucProto == 0x02))
    {
        return ret;
    }

    ret = HI_ERR_FAILURE;
    return ret;
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

        data = &app_data[pos];   // ��ȡת������

        frame_data = pstAfnParam->pstFrame; // ��ȡ����֡��Ϣ

        // ǿ���л�Ϊ·����ͣ״̬
        mrsForceSwitchPause();

        // �����泭����ģ����ж���
        ret = mrsParallelReadMeterProc(frame_data->strUsrDataHrd.uInfoRealm.stInfRealmDL.ucSeq, protocol, data, data_len);

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
    }

    return HI_ERR_SUCCESS;
}

#endif
#endif

