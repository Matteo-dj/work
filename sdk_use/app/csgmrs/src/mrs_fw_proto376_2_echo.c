//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_fw_proto376_2_echo.c
//  版 本 号   : V1.0
//  作    者   : niesongsong/kf62735
//  生成日期   : 2011-12-14
//  功能描述   : TODO: ...
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2011-12-14
//    作    者 : niesongsong/kf62735
//    修改内容 : 创建文件
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
#define MRS_MAX_FRAME_LEN                   (1024)  // 最大支持的报文长度

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

// 该数组添加元素时需严格根据afn_data升序排列
// 在查找时按照二分法查找，如排序有误，查找会出错，影响后续操作
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


/* 旧规约: 376.2报文中(AFN=11-F1)从节点数据单元(字节数):表地址(6字节)+从节点序号(2字节)+规约类型(1字节) */
/* 新规约: 376.2报文中(AFN=11-F1)从节点数据单元(字节数):表地址(6字节)+规约类型(1字节) */
#define GetSecondaryNodeItemSize(ctx)   (((ctx)->ucProtoVer == PROTO_VER_3762_2009) ? MRS_SECONDARY_NODE_ITEM_SIZE_OLD : MRS_SECONDARY_NODE_ITEM_SIZE_NEW)

//以前是动态分配，现在改由静态存在
HI_PRV HI_U8 mrs3762AFN10F2Buffer[3+sizeof(AFN_10_F2_NODE_INFO)*MRS_3762_F10_NODE_MAX_NUM] = {0};
HI_PRV HI_U32 mrsCcoAddrWriteNv(HI_VOID);
HI_PRV HI_U16 mrsGetCurrTopoNum(HI_VOID);

//*****************************************************************************
// 函数名称: mrsInitSustainAfn
// 功能描述: 初始化支持的AFN-FN功能码
//
// 参数说明:
//
// 返 回 值:
//
// 调用要求:
// 调用举例:
// 作    者: fengxiaomin/00209182 [2014-1-10]
//*****************************************************************************
/* BEGIN: Added by fengxiaomin/00209182, 2014/1/10   问题单号号:DTS2014012607684 */
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
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032708427 */
    MRS_AFN_CAPABILITY[ucOffSet].capability[12] = 0x18; //0x10  101,100      0001 1000 (104~97)
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
    MRS_AFN_CAPABILITY[ucOffSet].capability[28] = 0x08; //0x10  228      0000 1000 (232~225)
    ucOffSet++;
    MRS_AFN_CAPABILITY[ucOffSet].capability[0] = 0x3F;  //0x11  1,2,3,4,5,6      0011 1111
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/28   问题单号:DTS2014032708937 */
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
// 函数名称: mrs3762ProtoAckFrame
// 功能描述: 构造确认帧
//
// 参数说明:
//   AFN_00_F1 * pstAckFrame [in/out] 自定义响应头，如果传空值会构造一个默认的
//                                    全零响应头,很多情况是可以直接传空值的。
//
//   HI_PBYTE * ppucBuffer [in/out] 输出3762帧的缓存起始地址的指针
//
//   HI_U16 * pusBufferLen [in/out] 记录长度
//
//
// 返 回 值:
//
//
// 调用要求:
// 调用举例:
// 作    者: niesongsong/kf62735 [2011-12-13]
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
// 函数名称: mrs3762ProtoDenyFrame
// 功能描述: TODO: ...
//
//   AFN_00_F1 * pstDenyFrame [in/out] 自定义响应头，如果传空值会构造一个默认的
//                                    全零响应头,很多情况是可以直接传空值的。
//
//   HI_PBYTE * ppucBuffer [in/out] 输出3762帧的缓存起始地址的指针
//
//   HI_U16 * pusBufferLen [in/out] 记录长度
//
// 返 回 值:
//    TODO: ...
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: niesongsong/kf62735 [2011-12-13]
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
// 函数名称: mrs3762AfnDispatch
// 功能描述: 上行报文统一处理函数
//
// 参数说明:
//   MRS_3762_FRAME_STRU * pstFrame [in/out] 帧结构
//
//   HI_PBYTE * ppOutBuffer [in/out] 输出缓存（如果有的话）
//
//   HI_U16 * pusBufferLen [in/out] 输出缓存长度（如果有的话）
//
//
// 返 回 值:
//    HI_ERR_SUCCESS 表示成功，其它为错误码
//
// 调用要求:
// 调用举例:
// 作    者: niesongsong/nKF62735 [2011-12-17]
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
        //获取FN
        MRS_3762_AFN_FUN pAfnFnProc = 0;
        HI_U8 ucFnIndex = mrs3762GetDtFnIndex(pucContent);

        pAfnFnProc = mrs3762FindAfnProc(ucAfn,ucFnIndex);

        if (pAfnFnProc != 0)
        {
            //组织参数
            AFN_FN_UL_PARAM param = {0};
            param.pstFrame = pstFrame;
            param.pContent = pstFrame->strUsrDataHrd.stAppDataRealm.pucAppDataContent;
            param.usContentLen = pstFrame->strUsrDataHrd.stAppDataRealm.usAppDataLen;
            param.ppucOutBuffer = ppOutBuffer;
            param.pusBufferLen = pusBufferLen;
            param.ucAfn = ucAfn;
            param.ucFnIndex = ucFnIndex;  //FN的索引值

            //处理
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
// 函数名称: mrs3762FindAfnProc
// 功能描述: 根据AFN和FN找到相应的FN处理函数
//
// 参数说明:
//   afn [in] AFN，应用程序功能码
//
//   fnIndex [in] fn索引
//
//
// 返 回 值:
//    成功返回该处理的函数指针
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: niesongsong/kf62735 [2011-12-23]
// 修    改: cuiate/00233580     [2013-04-22]
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
// 函数名称: mrs3762GetDtFnIndex
// 功能描述: 根据DT类得到FN的索引值
//
// 参数说明:
//   dt[2] [in] DT类
//
// 返 回 值:
//    0xFF为无效值
//
// 调用要求:
// 调用举例:
// 作    者: niesongsong/nKF62735 [2011-12-17]
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
// 函数名称: mrs3762_EasyEnc
// 功能描述: 一个简易的丑陋的组帧函数
//
// 参数说明:
//   pucContent [in/out] 数据单元的内容
//
//   usLen [in] 数据单元长度
//
//   param [in/out] 上行传过来的参数
//
//
// 返 回 值:
//
// 调用要求:
// 调用举例:
// 作    者: niesongsong/nKF62735 [2011-12-17]
// 修    改: cuiate/00233580      [2013-03-12]
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
// 函数名称: mrs3762_AFNXX_XX
// 功能描述: 没有实现或不支持的FN处理函数
//
// 参数说明:
//
//
// 返 回 值:
//    HI_ERR_SUCCESS
//
// 调用要求: 不支持的FN或者暂未支持的FN处理
// 调用举例:
// 作    者: niesongsong/nKF62735 [2011-12-17]
//*****************************************************************************
HI_U32 mrs3762_AFNXX_XX(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_INEXISTENT_INFO;//否认帧中的错误状态字
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;

    mrs3762ProtoDenyFrame(&stDenyData,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);

    return ret;
}

//AFN00 F1:确认
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

//AFN00 F2:否认
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

//AFN01 F1:硬件初始化
HI_U32 mrs3762_AFN01_F1(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    static HI_U8 first = 0;
/* BEGIN: Added by fengxiaomin/00209182, 2014/3/4   问题单号:DTS2014022806293 */
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//否认帧中的错误状态字

/* BEGIN: Added by fengxiaomin/00209182, 2014/1/10   问题单号号:DTS2014012607684 */
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

    //组织确认帧
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

//AFN01 F2:参数区初始化
HI_U32 mrs3762_AFN01_F2(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;

    mrsSrvArchivesClear();

    //组织确认帧
    mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);

    (hi_void)memset_s(&pstCcoCtx->stReadMeterModel.stCurrNode, sizeof(MRS_ARCHIVES_NODE_STRU), 0, sizeof(MRS_ARCHIVES_NODE_STRU));
    pstCcoCtx->stReadMeterModel.usCurrNodeSeq = 0;  //(改为这样)
    pstCcoCtx->stReadMeterModel.usSuccNum = 0;

    mrsCcoReadFailReset();

    MRS_StartTimer(EN_MRS_SRV_CCO_WRITE_FLASH_ACTIV_TIMER,
                    pstCcoCtx->ucFlashSaveDelay *MS_PER_SECOND,
                    HI_SYS_TIMER_ONESHOT);
    mrsCcoSmWhitelistSwitchTimerReset();

    if (pstCcoCtx->status & MRS_CCO_STATUS_NORMAL)
    {
        // 延时MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE时间后重新激活读表模块
        MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER,
                            MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE * 3,
                            HI_SYS_TIMER_ONESHOT);
    }

    mrsCcoDetectRmReset();
    mrsDfxCcoArchivesClearTime(HI_MDM_GetMilliSeconds());
    mrsDfxLrMeterNum(0);

    return ret;
}

//AFN01 F3:数据区初始化
HI_U32 mrs3762_AFN01_F3(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    MRS_CCO_SRV_CTX_STRU * cco_ctx = mrsCcoGetContext();

    // 数据区初始化代码
    cco_ctx->stReadMeterModel.usSuccNum = 0;
    if (cco_ctx->stAutoSearchModel.pReportedNodeInf)
    {
        (hi_void)memset_s(cco_ctx->stAutoSearchModel.pReportedNodeInf,
            sizeof(MR_NODE_METER_ADD_STRU) * PRODUCT_CFG_MRS_MAX_NODE_NUM,0,
            sizeof(MR_NODE_METER_ADD_STRU) * PRODUCT_CFG_MRS_MAX_NODE_NUM);
    }

    mrsCcoReadFailReset();
    //组织确认帧
    mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);

    return ret;
}


// AFN02 F1: 转发通讯协议数据帧
HI_U32 mrs3762_AFN02_F1(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    HI_U8 * pucContent = pstAfnParam->pContent;
    HI_U16 usContentLen = pstAfnParam->usContentLen;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;  // 否认帧中的错误状态字

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
        HI_U8 ucMinLen = 4; // Fn(2) + 通信协议类型(1) + 报文长度(1)
        
        // 获取地址域
        pstUsrDataHrd = &pstAfnParam->pstFrame->strUsrDataHrd;

        // 获取帧序号
        pstCcoCtx->stAnytimeRMModel.seq_1376_2 = pstUsrDataHrd->uInfoRealm.stInfRealmDL.ucSeq;

        // 只有在PLC处于占用状态下才进行阻拦
        if(!mrsIsDetectRm() && (pstCcoCtx->stAnytimeRMModel.enState == EN_MRS_ANYTIME_RM_WAIT_PLC))
        {
            ret = HI_ERR_BUSY;  // 当前正在执行随抄命令
            stDenyData = MRS_13762_DENY_STATUS_CCO_BUSY;
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_023, HI_DIAG_MT("XR PLC BUSY"),pstCcoCtx->stAnytimeRMModel.enState);
            break;
        }

        // Fn(2) + 通信协议类型(1) + 报文长度(1)
        if (usContentLen < ucMinLen)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_024, HI_DIAG_MT("Len Too Short"));
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }

        // 偏移Fn(2Byte)
        ucPos += 2;

        // 获取规约类型
        ucProtocolType = pucContent[ucPos++];

        // 检查规约类型是否有效
        ret = mrsCheckProtocolValid(ucProtocolType);
        if (ret != HI_ERR_SUCCESS)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_025, HI_DIAG_MT("Protocol Err!"));
            stDenyData = MRS_13762_DENY_STATUS_ERROR_FORMAT;
            ret = HI_ERR_BAD_DATA;
            break;
        }

        // 获取报文长度
        ucDataLen = pucContent[ucPos++];

        // 剩余长度检查
        if((HI_S16)(usContentLen - ucPos) < ucDataLen)
        {
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_027, HI_DIAG_MT("Farmer Len Too Short"));
            break;
        }

        // 获取转发报文
        pucData = &pucContent[ucPos];

        // 获取规约类型
        pstCcoCtx->stAnytimeRMModel.ucProtocol = ucProtocolType;

        // 标记来源为 "转发通讯协议数据帧"
        pstCcoCtx->stAnytimeRMModel.from = XR_FROM_AFN02_F1;

        // 初始化随抄读表模块
        mrsAnyTimeReadMeterInit();

        mrsDetectRmBreakProc();
        
        // 调用随抄读表模块进行读表
        ret = mrsAnyTimeReadMeterProcess(pstUsrDataHrd->stAddrRealm.ucDestAdd, ucProtocolType, pucData, ucDataLen);
        if (HI_ERR_SUCCESS != ret)
        {
            stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;
        }
    } while (0);

    if(ret != HI_ERR_SUCCESS)
    {
        HI_U8 ucFrom = pstCcoCtx->stAnytimeRMModel.from;
        
        // 标记来源为 "监控载波从节点"
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
    HI_U16 frame_len = 9;  // 帧数据大小为9字节
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

//AFN03 F1 厂商代码和版本信息
HI_U32 mrs3762_AFN03_F1(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 buf[12] = {0};
    HI_U16 buf_len = sizeof(buf);

    ret = makeAfn0301Buf(buf, &buf_len);
    if (ret == HI_ERR_SUCCESS)
    {
        //组装成帧
        ret = mrs3762_EasyEnc(buf, buf_len, param);
    }

    return ret;
}

//AFN03 F2 噪声值
HI_U32 mrs3762_AFN03_F2(HI_IN HI_OUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 noise = 0;

    ret = mrs3762_EasyEnc(&noise,sizeof(noise),param);

    //返回
    return ret;
}

/* 问题单号: DTS2014010608635 BEGIN: Added by liuxipeng/lkf54842, 2014/01/07 */
//AFN03 F3 查询从节点侦听信息
HI_U32 mrs3762_AFN03_F3(HI_IN HI_OUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 resp[2] = {0};    // 【0】侦听到的从节点数量; 【1】本次传输的从节点数量

    ret = mrs3762_EasyEnc(resp,sizeof(resp),param);

    //返回
    return ret;
}
/* 问题单号: DTS2014010608635 END:   Added by liuxipeng/lkf54842, 2014/01/07 */

//AFN03 F4：载波主节点地址
HI_U32 mrs3762_AFN03_F4(HI_INOUT HI_VOID * param)
{
    HI_U8 ucNodeAddress[6] = {0};
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();

    //查询载波主节点地址
    (hi_void)memcpy_s(ucNodeAddress, sizeof(ucNodeAddress), pstCcoCtx->ucMainNodeAddr, sizeof(pstCcoCtx->ucMainNodeAddr));
    mrsHexInvert(ucNodeAddress, sizeof(ucNodeAddress));

    //组装成帧
    return mrs3762_EasyEnc(ucNodeAddress,sizeof(ucNodeAddress),param);
}


//AFN03 F5：载波主节点状态字及载波速率
HI_U32 mrs3762_AFN03_F5(HI_INOUT HI_VOID * param)
{
    HI_U8 buf[4] = {0};

    struct
    {
        HI_U16 SpeedNum : 4;     // 载波速率数量
        HI_U16 CHFeature : 2;    // 信道特征
        HI_U16 Mode : 2;         // 抄表模式
        HI_U16 CHNum : 4;        // 载波信道数量
        HI_U16 Back : 4;         // 保留
    }stStatusWord;

    struct
    {
        HI_U16 SpeedRate : 15;     // 载波速率
        HI_U16 Unit : 1;           // 速率单位
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


//AFN03 F6 查询主节点干扰状态
HI_U32 mrs3762_AFN03_F6(HI_IN HI_OUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 resp = 1;    // 主节点存在干扰

    ret = mrs3762_EasyEnc(&resp,sizeof(resp),param);

    //返回
    return ret;
}


//AFN03 F7: 读取从节点监控最大超时时间
HI_U32 mrs3762_AFN03_F7(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    PLC_OVERTIME_INF_STRU *pstAfn13 = HI_NULL;

    HI_U8 maxTimeout = 0;

    /* 获取超时时间 */
    pstAfn13 = mrsGetPlcOverTimeInfAfn13();
    maxTimeout = pstAfn13->MaxTimeout;

    //组装成帧
    ret = mrs3762_EasyEnc(&maxTimeout,sizeof(maxTimeout),param);

    //返回
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
        
        buf[offset++] = 0xB2;   /* D0~D3 : 2  (宽带电力线载波通讯)
                                   D4    : 1  (带有路由管理功能)
                                   D5    : 1  (需要下发从节点信息)
                                   D6~D7 : 10 (载波模块主导的周期抄表模式)     */
                                   
        buf[offset++] = 0x37;   /* D0    : 1  (广播支持传输延时参数)
                                   D1    : 1  (监控载波从节点支持传输延时参数)
                                   D2    : 1  (路由主动抄表支持传输延时参数)
                                   D3~D4 : 10 (失败节点由集中器主动切换)
                                   D5    : 1  (广播命令在执行前返回确认帧)
                                   D6~D7 : 0  (执行广播命令不需要信道标识)     */

        buf[offset++] = 0x01;   // 高三位:掉电信息  低五位:载波信道数量
        buf[offset++] = 0x01;   // 高四位:保留      低四位:通讯速率数量

        pstAfn13 = mrsGetPlcOverTimeInfAfn13();
        offset = MRS_CCO_MON_MAX_TIMEOUT_OFFSET;
        buf[offset] = pstAfn13->MaxTimeout;
        // 广播命令最大超时时间：执行05H-F3"启动广播命令"的最大超时时长。
        tmp = MRS_AFN03_F10_BC_TIMEOUT;
        offset = MRS_CCO_BC_MAX_TIMEOUT_OFFSET;
        (hi_void)memcpy_s(buf + offset, buf_len - offset, &tmp, sizeof(tmp));

        // 最大支持的376.2报文长度：可以正确接收的376.2报文最大长度
        tmp = MRS_MAX_FRAME_LEN;
        offset = MRS_3762FRAME_MAX_LENGTH_OFFSET;
        (hi_void)memcpy_s(buf + offset, buf_len - offset, &tmp, sizeof(tmp));

        // 文件传输支持的最大单个数据包长度：在15H-F1"文件传输协议"中支持的最大分包大小
        tmp = MRS_MAX_FRAME_LEN;
        offset = MRS_MAX_PACKET_LENGTH_OFFSET;
        (hi_void)memcpy_s(buf + offset, buf_len - offset, &tmp, sizeof(tmp));

        // 升级操作等待时间：终端发送完最后一个升级数据包后，需要等待模块完成升级的时间长度。
        offset = MRS_UPG_WAIT_TIME_OFFSET;
        buf[offset] = MRS_UPG_WAIT_TIME;

        // 主节点地址：本地通信模块的主节点地址。
        offset = MRS_CCO_ADDR_OFFSET;
        (hi_void)memcpy_s(buf + offset, buf_len - offset, pstCcoCtx->ucMainNodeAddr, sizeof(pstCcoCtx->ucMainNodeAddr));
        mrsHexInvert(buf + offset, sizeof(pstCcoCtx->ucMainNodeAddr));

        // 支持的最大从节点数量：本模块（带路由管理）支持的最大从节点下装数量
        tmp = PRODUCT_CFG_MRS_MAX_NODE_NUM;
        offset = MRS_STATION_MAX_NUM_OFFSET;
        (hi_void)memcpy_s(buf + offset, buf_len - offset, &tmp, sizeof(tmp));

        // 当前从节点数量：本模块（带路由管理）当前下装的从节点数量
        tmp = mrsSrvArchivesMeterNum(); // 表档案节点数
        offset = MRS_STATION_NUM_OFFSET;
        (hi_void)memcpy_s(buf + offset, buf_len - offset, &tmp, sizeof(tmp));

        // 通信模块使用的376.2协议发布日期： BCD编码，YYMMDD日期格式
        offset = MRS_PROTO3762_RELEASE_OFFSET;
        (hi_void)memcpy_s(buf + offset, buf_len - offset, MRS_AFN03F10_RELEASE_DATE, 3);

        // 通信模块使用的376.2协议最后备案日期： BCD编码，YYMMDD日期格式。
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
    // 江苏版本，不回复，清buffer
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
        pstCcoItem->usTimeOut = 1500;//单位为ms
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

    /* 数据单元标识(2字节)+数据单元(1字节) */
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
    HI_MDM_QueryInfo(&stMacInfo);  // 读取mac拓扑表信息
    usTopoNum = (HI_U16)((HI_MAC_NETWORK_TOPO_S*)pstCcoCtx->pTopoIntegerInfo)->num;

    return usTopoNum;
}

//*****************************************************************************
// 函数名称: mrs3762_AFN10_F101
// 功能描述: AFN=10H-F101处理函数
//
// 参数说明:
//    param 数据域内容
//
// 返 回 值:
//    HI_ERR_SUCCESS   正确响应
//
// 调用要求:
// 调用举例:
// 作    者: fengxiaomin/00209182 [2014-1-10]
//*****************************************************************************
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032708427 */
HI_U32 mrs3762_AFN10_F101(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 * pContent = HI_NULL;
    HI_U16 usContentLen = 0;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//否认帧中的错误状态字
    HI_U16 usStartSeq = 0;
    HI_U8 ucQueryNum = 0;
    HI_U8 *pBuffer = HI_NULL;
    HI_U16 usBufLen = 0;
    HI_U8 ucReportNum = 0;

    pContent = pstAfnParam->pContent;
    usContentLen = pstAfnParam->usContentLen;

    do{
        /* 数据单元标识(2字节)+数据单元(3字节) */
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
// 函数名称: mrsAfn10101Buf
// 功能描述: 获取站点版本信息
//
// 参数说明:
//    pBuffer 数据域内容
//    pBufLen 数据域内容长度
//    ucQueryNum 询问数量
//    usStartSeq 起始序号
//
// 返 回 值:
//    上报数量
//
// 调用要求:
// 调用举例:
// 作    者: fengxiaomin/00209182 [2014-1-10]
//*****************************************************************************
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032708427 */
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

    //CCO的版本信息
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

/* 问题单号: DTS2014010608635 BEGIN: Added by liuxipeng/lkf54842, 2014/01/07 */
//AFN04 F1 发送测试
HI_U32 mrs3762_AFN04_F1(HI_IN HI_OUT HI_VOID * param)
{
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;

    return mrs3762ProtoAckFrame(0, pstAfnParam, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
}

//AFN04 F2 载波从节点点名
HI_U32 mrs3762_AFN04_F2(HI_IN HI_OUT HI_VOID * param)
{
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;

    return mrs3762ProtoAckFrame(0, pstAfnParam, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
}
/* 问题单号: DTS2014010608635 END:   Added by liuxipeng/lkf54842, 2014/01/07 */

HI_U32 mrs3762_AFN04_F3(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 * pContent = HI_NULL;
    HI_U16 usContentLen = 0;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//否认帧中的错误状态字

    pContent = pstAfnParam->pContent;
    usContentLen = pstAfnParam->usContentLen;

    do
    {
        MRS_PLC_FRAME_DATA_S stPlcData = {0};
        MRS_TEST_TRANSMIT_STRU * test_frame = HI_NULL;
        HI_U8 * payload = HI_NULL;
        HI_U16 payload_len = 0;
        // TODO: 这里规范上表格跟描述不相符，实际测试报文可能会对不上
        HI_U16 head_len = 9; // 通信速率(1)+目标地址(6)+通信协议类型(1)+报文长度(1)
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

        // 查找MAC地址
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

//AFN05 F1：设置载波主节点地址
HI_U32 mrs3762_AFN05_F1(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 * pContent;
    HI_U16 usContentLen = 0;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//否认帧中的错误状态字

    if (param == HI_NULL)
    {
        ret = HI_ERR_BAD_DATA;
        return ret;
    }

    //上行数据为6个字节的主节点地址
    pContent = pstAfnParam->pContent;
    usContentLen = pstAfnParam->usContentLen;

    do
    {
        if (usContentLen != (sizeof(pstCcoCtx->ucMainNodeAddr) + 2)) //设置载波主节点的应用长度为
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_012, HI_DIAG_MT("Length err=%d"),usContentLen);
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }

        //设置主节点地址
        (hi_void)memcpy_s(pstCcoCtx->ucMainNodeAddr, sizeof(pstCcoCtx->ucMainNodeAddr), pContent + 2, sizeof(pstCcoCtx->ucMainNodeAddr));
        mrsHexInvert(pstCcoCtx->ucMainNodeAddr, sizeof(pstCcoCtx->ucMainNodeAddr));
        HI_MDM_DevIdConfig(pstCcoCtx->ucMainNodeAddr);

        //根据项ID写NV项内容
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

        //组织确认帧
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

//AFN05 F2：允许从节点上报
HI_U32 mrs3762_AFN05_F2(HI_INOUT HI_VOID * param)
{
    HI_U16 usContentLen = 0;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//否认帧中的错误状态字
    HI_U32 ret = HI_ERR_SUCCESS;

    if (param == HI_NULL)
    {
        ret = HI_ERR_BAD_DATA;
        return ret;
    }

    usContentLen = pstAfnParam->usContentLen;
    /* 数据单元标识(2字节)+数据单元(1字节) */
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

//AFN05 F3：启动广播
HI_U32 mrs3762_AFN05_F3(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 * pContent = HI_NULL;
    HI_U16 usContentLen = 0;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//否认帧中的错误状态字

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

        // 长度: Fn(2) + protocol(1) + len(1) + data(len)
        if (usContentLen != 4 + pContent[3])
        {
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }

        // 规约
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

            case 0: // 透传
            case 3: // 相位
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
        // 如果是江苏版本(NV开启)且用业务串口，不拦截;否则拦截
        if (!(mrsToolIsBbu()
            && (!mrsIsSimuChlOpened())))
#endif
        {
            //组织确认帧
            mrs3762ProtoAckFrame(6, pstAfnParam, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
        }
    } while(0);

    if (ret != HI_ERR_SUCCESS)
    {
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
        // 如果是江苏版本(NV开启)且用业务串口，不拦截;否则拦截
        if (!(mrsToolIsBbu()
            && (!mrsIsSimuChlOpened())))
#endif
        {
            ret = mrs3762ProtoDenyFrame(&stDenyData,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
        }
    }

    return ret;
}

//AFN05 F4：设置从节点监控最大超时时间
HI_U32 mrs3762_AFN05_F4(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 * pContent = HI_NULL;
    HI_U16 usContentLen = 0;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//否认帧中的错误状态字

    pContent = pstAfnParam->pContent;
    usContentLen = pstAfnParam->usContentLen;

    do
    {
        PLC_OVERTIME_INF_STRU *pstAfn13 = HI_NULL;
/* BEGIN: PN: DTS2015042205399 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
        NV_MRS_RM_CFG_STRU stRmCfgInf;
/* END:   PN: DTS2015042205399 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */

        /* 数据单元标识(2字节)+数据单元(1字节) */
        if (usContentLen != 3)
        {
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }

        /* 设置参数 */
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
        /* 写NV */
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

        //组织确认帧
        mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
    } while(0);

    if (ret != HI_ERR_SUCCESS)
    {
        ret = mrs3762ProtoDenyFrame(&stDenyData,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
    }

    return ret;
}

//AFN10 F1：载波从节点数量
HI_U32 mrs3762_AFN10_F1(HI_INOUT HI_VOID * param)
{
    HI_U16 usSlaveNum[2] = {0};

    usSlaveNum[0] = mrsSrvArchivesMeterNum();
    usSlaveNum[1] = PRODUCT_CFG_MRS_MAX_NODE_NUM;

    return mrs3762_EasyEnc((HI_U8 *)usSlaveNum, sizeof(usSlaveNum), param);
}

//AFN10 F2：载波从节点信息
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

    //下行数据为3个字节的主节点地址:从节点起始序号 2个字节,从节点数量1个字节
    pContent = pstAfnParam->pContent;
    usContentLen = pstAfnParam->usContentLen;

    do
    {
        HI_U16 usMeterOrder = 0;
        HI_U8 * pCurrentNode = 0;

        //载波从节点信息
        if (usContentLen != 5)
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_013, HI_DIAG_MT("length err=%d"),usContentLen);
            ret = HI_ERR_BAD_DATA;
            return ret;
        }

        //获取到了从节点信息
        (hi_void)memcpy_s(&usStartNo, sizeof(usStartNo), pContent + 2, sizeof(usStartNo));
        ucNodeNum = pContent[4];
        if(ucNodeNum > MRS_3762_F10_NODE_MAX_NUM)
        {
            // 从节点数量超过最大限制，按照32处理
            ucNodeNum = MRS_3762_F10_NODE_MAX_NUM;
        }

        //分配大小(总数量2 + 当前数量1 + 节点信息8*N),全局变量
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
        
        //总数和数量
        (hi_void)memcpy_s(pBuffer, 2, &usMeterNum, 2);
        pBuffer[2] = ucFind;
        ret = mrs3762_EasyEnc(pBuffer,usReplyLen,param);
    }

    mrsCcoSmWhitelistSwitchTimerReset();

    return ret;
}

/* 问题单号: DTS2014010608635 BEGIN: Added by liuxipeng/lkf54842, 2014/01/07 */

//AFN10 F3 查询指定从节点上一级中继路由信息
HI_U32 mrs3762_AFN10_F3(HI_IN HI_OUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 resp = 0;    // 提供路由的从节点总数量

    ret = mrs3762_EasyEnc(&resp,sizeof(resp),param);

    //返回
    return ret;
}
/* 问题单号: DTS2014010608635 END:   Added by liuxipeng/lkf54842, 2014/01/07 */
#endif

//*****************************************************************************
// 函数名称: mrsGetRouteFlag
// 功能描述: 获取路由完成标志
//
// 参数说明:
//   HI_VOID
//
// 返 回 值:
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2013-02-21]
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
// 函数名称: mrs3762_AFN10_F4
// 功能描述: AFN10 F4: 查询路由运行状态
//
// 参数说明:
//   param [in/out] 数据单元
//
// 返 回 值:
//   HI_ERR_SUCCESS 返回确认帧
//   HI_ERR_FAILURE 地址映射表错误或读取失败
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2012-06-01]
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

    pBuffer[0] = (HI_U8)(pstEvtCtx->bACDFlg << 2 | ucWorkFlag << 1 | route_flag); //运行状态字
    (hi_void)memcpy_s(&pBuffer[1], sizeof(pBuffer) - 1, &usMeterNum, 2);  //从节点总数量
    (hi_void)memcpy_s(&pBuffer[3], sizeof(pBuffer) - 3, &pstCcoCtx->stReadMeterModel.usSuccNum, 2);  //已抄从节点数量
    pBuffer[7] = (HI_U8)(0x02 | (ucWorkStatus << 2));  //工作开关
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
        // 从节点数量超过最大限制，按照32处理
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

//查询网络拓扑信息
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
        /* 数据单元标识(2字节)+数据单元(3字节) */
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
            ret = HI_MDM_QueryInfo(&mac_inf);  // 读取mac拓扑表信息
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

//查询网络拓扑信息
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

//AFN11 F1：添加载波从节点
HI_U32 mrs3762_AFN11_F1(HI_INOUT HI_VOID * param)
{
    HI_U8 ucMeterNum = 0;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    HI_U8 ucUnitConstLen = GetSecondaryNodeItemSize(pstCcoCtx);
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//否认帧中的错误状态字

    //下行的应用数据
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

        //长度判断
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
            stDenyData = MRS_13762_DENY_STATUS_INVALID_DATA;//无效的数据单元
            ret = HI_ERR_BAD_DATA;
            break;
        }

        pucContent += 3;

        ret = mrsWhetherAddNode(pucContent, ucMeterNum, &stDenyData); //判断是否应添加从节点
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
        //组织确认帧
        mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);

        //新添加了一个定时器(激活写入flash定时器)（分配ID）：超时后将添加的表信息写入flash
        MRS_StartTimer(EN_MRS_SRV_CCO_WRITE_FLASH_ACTIV_TIMER, pstCcoCtx->ucFlashSaveDelay * MS_PER_SECOND, HI_SYS_TIMER_ONESHOT);
        mrsCcoSmWhitelistSwitchTimerReset();
    }
    else  //这里还没有区分什么原因不能添加，区分表号重复的否认帧数据单元
    {
        //组织否认帧
        ret = mrs3762ProtoDenyFrame(&stDenyData, pstAfnParam, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
    }

    mrsDfxLrMeterNum(mrsSrvArchivesMeterNum());

    return ret;
}

//AFN11 F2：删除载波从节点
HI_U32 mrs3762_AFN11_F2(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 ucUnitConstLen = 6;//376.2报文中数据单元内容内容最小单元(字节数):表地址(6个字节)
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//否认帧中的错误状态字

    //下行的应用数据
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

        //长度判断
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
            stDenyData = MRS_13762_DENY_STATUS_INVALID_DATA;//无效的数据单元
            ret = HI_ERR_BAD_DATA;
            break;
        }

        pucContent += 3;
        while (ucMeterIndex < ucMeterNum)
        {
            //检查是否是广播地址
            bIsBCD = !(mrsToolsBroadCastAddr(pucContent + ucMeterIndex * ucUnitConstLen));
            if (bIsBCD != HI_TRUE)
            {
                ret = HI_ERR_BAD_DATA;
                stDenyData = MRS_13762_DENY_STATUS_ERROR_FORMAT; //格式错误
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
            stDenyData = MRS_13762_DENY_STATUS_INEXISTENT_METER;//表号不存在
            ret = HI_ERR_BAD_DATA;
            break;
        }
    } while (0);

    if (ret == HI_ERR_SUCCESS)
    {
        //组织确认帧
        ret = mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);

        mrsSrvArchivesRealign();

        //(激活写入flash定时器)（与添加从节点时的定时器相同）：超时后将删除且重新排列的表信息写入flash
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
        //组织否认帧
        ret = mrs3762ProtoDenyFrame(&stDenyData,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
    }

    mrsDfxLrMeterNum(mrsSrvArchivesMeterNum());

    return ret;
}

/* 问题单号: DTS2014010608635 BEGIN: Added by liuxipeng/lkf54842, 2014/01/07 */

//AFN11 F3 设置从节点固定中继路径
HI_U32 mrs3762_AFN11_F3(HI_IN HI_OUT HI_VOID * param)
{
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;

    //返回
    return mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
}

//AFN11 F4 设置路由工作模式
HI_U32 mrs3762_AFN11_F4(HI_IN HI_OUT HI_VOID * param)
{
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;

    //返回
    return mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
}
/* 问题单号: DTS2014010608635 END:   Added by liuxipeng/lkf54842, 2014/01/07 */

//*****************************************************************************
// 函数名称: mrs3762_AFN11_F5
// 功能描述: AFN11 F5: 激活载波从节点主动注册
//
// 参数说明:
//   param [in/out] 数据单元
//
// 返 回 值:
//   HI_ERR_SUCCESS 返回确认帧
//   HI_ERR_FAILURE 地址映射表错误或读取失败
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2012-05-18]
// 修    改: cuiate/00233580       [2013-04-08]
//*****************************************************************************
HI_U32 mrs3762_AFN11_F5(HI_INOUT HI_VOID * param)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    AFN_FN_UL_PARAM *pstAfnParam = (AFN_FN_UL_PARAM *)param;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;

    // 下行的应用数据
    HI_U8 * pucContent = pstAfnParam->pContent;
    HI_U16 usContentLen = pstAfnParam->usContentLen;

    do
    {
        // 数据域长度判断: 12 = fn(2) + time(6) + duration(4)
        if (usContentLen != 12)
        {
            ulRet = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }

        // 检查开始时间是否合法
        ulRet = MRS_TimeIsValid(pucContent + 2);
        if (ulRet != HI_ERR_SUCCESS)
        {
            ulRet = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_FORMAT;
            break;
        }

        // 检查单板状态
        // 1. 远程升级时，应答否认帧
        // 2. 打开白名单开关:
        //    2.1 组网未完成，等待组网完成开始搜表
        //    2.2 组网完成，开始搜表流程

        // 正在远程升级，应答否认帧
        if (mrsIsCcoUpgStatus())
        {
            ulRet = HI_ERR_BUSY;
            stDenyData = MRS_13762_DENY_STATUS_CCO_BUSY;
            break;
        }

        if (pstCcoCtx->status & MRS_CCO_STATUS_SEARCH_METER)
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_059, HI_DIAG_MT("搜表中...[%d]"), pstCcoCtx->status);
            ulRet = HI_ERR_SUCCESS;
            break;
        }
        else if (pstCcoCtx->status & MRS_CCO_STATUS_SEARCH_METER_TRANSFORMER)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_007, HI_DIAG_MT("台区识别搜表中..."));
            ulRet = HI_ERR_BUSY;
            stDenyData = MRS_13762_DENY_STATUS_CCO_BUSY;
            break;
        }

        if (HI_FALSE == pstCcoCtx->bPauseFlag)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_058, HI_DIAG_MT("非路由暂停"));
            ulRet = HI_ERR_BUSY;
            stDenyData = MRS_3762_DENY_STATUS_COMMUNICATION_OVERTIME;
            break;
        }

        // 记录搜表参数: 持续时间
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
        HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("开始搜表"));
    } while (0);

    if (ulRet == HI_ERR_SUCCESS)
    {
        mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
    }
    else
    {
        //组织否认帧
       ulRet = mrs3762ProtoDenyFrame(&stDenyData, pstAfnParam, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
    }

    return ulRet;
}


//AFN11-F6: 终止从节点主动注册
HI_U32 mrs3762_AFN11_F6(HI_INOUT HI_VOID * param)
{
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;

    HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("【停止搜表】"));

    mrsCcoStopSearchMeterNormal((HI_U8)MRS_CCO_SEARCH_FINISH_STOP);

    mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);

    return HI_ERR_SUCCESS;
}

//AFN12 F1：重启
HI_U32 mrs3762_AFN12_F1(HI_INOUT HI_VOID * param)
{
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//否认帧中的错误状态字

    do
    {
        if (mrsIsCcoUpgStatus())
        {
            ret = HI_ERR_BUSY;
            stDenyData = MRS_13762_DENY_STATUS_CCO_BUSY;
            break;
        }

        mrsCcoStopSearchMeterNormal((HI_U8)MRS_CCO_SEARCH_FINISH_ROUTE_RESET);

        // 切换到普通状态并初始化
        mrsRecoverNormalState(pstCcoCtx);

        // 复位读表管理模块(停止所有读表请求)
        mrsRmModuleReset();

        (hi_void)memset_s(&pstCcoCtx->stReadMeterModel.stCurrNode, sizeof(MRS_ARCHIVES_NODE_STRU), 0, sizeof(MRS_ARCHIVES_NODE_STRU));
        pstCcoCtx->stReadMeterModel.usCurrNodeSeq = 0;  //(改为这样)
        pstCcoCtx->stReadMeterModel.usSuccNum = 0;

        mrsSrvArchivesReadFlagClear();

        pstCcoCtx->bPauseFlag = HI_FALSE;
        mrsDfxLrRoute((HI_U8)pstCcoCtx->bPauseFlag);

        // 当前轮次清零
        mrsSetCurrTurnNum(0);
        mrsDetectRmBreakProc();
    } while (0);

    if (HI_ERR_SUCCESS == ret)
    {        
        mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
        if (!mrsGetCcoTimeFrist())
        {
            // 延时MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE时间后重新激活读表模块
            MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER, MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE*3, HI_SYS_TIMER_ONESHOT);
        }
    }
    else
    {
        //组织否认帧
       ret = mrs3762ProtoDenyFrame(&stDenyData, pstAfnParam, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
    }

    return ret;
}


//AFN12 F2：暂停
HI_U32 mrs3762_AFN12_F2(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_U32 status = mrsCcoGetState();

    //在这里添加暂停代码
    if (pstCcoCtx->status & MRS_CCO_STATUS_NORMAL)
    {
        if(pstCcoCtx->stReadMeterModel.enCurrState != EN_MRS_SRV_CCO_READMETER_IDLE)
        {
            // 如果当前不是空闲状态则清除所有状态(当前节点不变化)
            mrsReadMeterStateInit();  // 读表模块状态初始化
        }
        
        status &= ~MRS_CCO_STATUS_NORMAL;
        mrsCcoSetState(status);
        
        //恢复探测抄表
        mrsResumeDetectRmSendMsg();
    }
    
    
    pstCcoCtx->bPauseFlag = HI_TRUE;
    mrsDfxLrRoute((HI_U8)pstCcoCtx->bPauseFlag);

    //组织确认帧
    mrs3762ProtoAckFrame(0,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);

    return ret;
}


//AFN12 F3：恢复
HI_U32 mrs3762_AFN12_F3(HI_INOUT HI_VOID * param)
{
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//否认帧中的错误状态字

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

        // 切换到普通状态并初始化
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
            // 延时MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE时间后重新激活读表模块
            MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER, MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE * 3, HI_SYS_TIMER_ONESHOT);
        }
    }
    else
    {
        //组织否认帧
       ret = mrs3762ProtoDenyFrame(&stDenyData, pstAfnParam, pstAfnParam->ppucOutBuffer, pstAfnParam->pusBufferLen);
    }

    return ret;
}


// AFN13 F1: 监控载波从节点
HI_U32 mrs3762_AFN13_F1(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    HI_U8 * pucContent = pstAfnParam->pContent;
    HI_U16 usContentLen = pstAfnParam->usContentLen;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//否认帧中的错误状态字
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
        
        pstUsrDataHrd = &pstAfnParam->pstFrame->strUsrDataHrd;     // 获取地址域
        pstCcoCtx->stAnytimeRMModel.seq_1376_2 = pstUsrDataHrd->uInfoRealm.stInfRealmDL.ucSeq;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
        // 如果是江苏版本(NV开启)且用业务串口，不拦截;否则拦截
        if (!(mrsToolIsBbu()
            && (!mrsIsSimuChlOpened())))
#endif
        {
            if(!mrsIsDetectRm() && (pstCcoCtx->stAnytimeRMModel.enState == EN_MRS_ANYTIME_RM_WAIT_PLC))   // 只有在PLC处于占用状态下才进行阻拦
            {
                ret = HI_ERR_BUSY;  // 当前正在执行随抄命令
                stDenyData = MRS_13762_DENY_STATUS_CCO_BUSY;
                HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_023, HI_DIAG_MT("enState= %d"),pstCcoCtx->stAnytimeRMModel.enState);
                break;
            }
        }

        // Fn(2)+通信协议类型(1)+通信延时相关性标识(1)+从节点附属节点数量(1)+报文长度(1)
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

        //检查规约类型是否有效
        ret = mrsCheckProtocolValid(pucContent[ucPos]);
        if (ret != HI_ERR_SUCCESS)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_025, HI_DIAG_MT("protocol err!"));
            stDenyData = MRS_13762_DENY_STATUS_ERROR_FORMAT;
            ret = HI_ERR_BAD_DATA;
            break;
        }

        ucProtocolType = pucContent[ucPos++]; // 获取规约类型

        if (pstCcoCtx->ucProtoVer != PROTO_VER_3762_2009)
        {
            if (pucContent[ucPos] == MRS_AFN_FRAME_DELAY_RELATED)
            {
                bDelayRelated = HI_TRUE;
            }
            ucPos++;
        }

        // 剩余长度 < 从节点附属节点信息 + 报文长度
        if((HI_S16)(usContentLen - ucPos) < (pucContent[ucPos] * 6 + 1))
        {
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_026, HI_DIAG_MT("extra_len<node_len+data_len"));
            break;
        }

        ucPos = (HI_U8)(ucPos + pucContent[ucPos] * 6);   // 偏移从节点附属节点信息(当前不处理附属节点信息)
        ucPos++;

        ucDataLen = pucContent[ucPos];    // 获取报文长度
        ucPos++;

        if((HI_S16)(usContentLen - ucPos) < ucDataLen)
        {
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_027, HI_DIAG_MT("(usContentLen - ucPos) < ucDataLen"));
            break;
        }

        pucData = &pucContent[ucPos];   // 获取转发报文

        pstCcoCtx->stAnytimeRMModel.ucProtocol = ucProtocolType;

        mrsAnyTimeReadMeterInit();  // 初始化随抄读表模块
        // 标记来源为 "监控载波从节点"
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
                
                // 启动服务结束定时器(到时间后会将随抄状态置为预恢复状态)
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

        // 调用随抄读表模块进行读表
        ret = mrsAnyTimeReadMeterProcess(pstUsrDataHrd->stAddrRealm.ucDestAdd, ucProtocolType, pucData, ucDataLen);
        if (HI_ERR_SUCCESS != ret)
        {
            stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;
        }
    } while (0);

    if(ret != HI_ERR_SUCCESS)
    {      
        HI_U8 ucFrom = pstCcoCtx->stAnytimeRMModel.from;
        
        // 标记来源为 "监控载波从节点"
        pstCcoCtx->stAnytimeRMModel.from = XR_FROM_AFN13_F1;
        //组装上行帧
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
    AFN_00_F2 stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//否认帧中的错误状态字

    //下行的应用数据
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

        if (pstCcoCtx->stReadMeterModel.enCurrState != EN_MRS_SRV_CCO_READMETER_WAIT_ASK_REPLY) // 如果抄表模块当前状态不是"等待询问应答状态"
        {
            // 暂时丢弃此帧,不进行处理,后续待优化
            ret = HI_ERR_SUCCESS;
            stDenyData = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;
            break;
        }

        if (pstCcoCtx->stReadMeterModel.pstAfn14F1 != HI_NULL) // 如果AFN14参数数据区还没被释放
        {
            // 释放掉此内存区域,准备留给新数据使用
            pstAfn14F1Param = pstCcoCtx->stReadMeterModel.pstAfn14F1;
            pstAfn14F1Param->MRS_AFN14_FREE_FUN(pstAfn14F1Param);

            pstAfn14F1Param = HI_NULL;
            pstCcoCtx->stReadMeterModel.pstAfn14F1 = HI_NULL;
        }

        // Fn(2)+抄读标志(1)+通信延时相关性标识(1)+数据长度(1) 容错:不计算从节点附属节点长度
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

        //抄读标志判断
        ucPos = 2;
        if (pucContent[ucPos] > 2)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_032, HI_DIAG_MT("14-F1 flag err"));
            ret = HI_ERR_BAD_DATA;
            stDenyData = MRS_13762_DENY_STATUS_ERROR_FORMAT;
            break;
        }

        pstAfn14Dl->ucReadFlg = pucContent[ucPos];

        // 判断是否时延相关
        if (pstCcoCtx->ucProtoVer != PROTO_VER_3762_2009)
        {
            ucPos++;
            if (pucContent[ucPos] == MRS_AFN_FRAME_DELAY_RELATED)
            {
                bDelayRelated = HI_TRUE;
            }
        }

        //数据长度L
        ucPos++;
        pucContent += ucPos;
        ucDataLen = *pucContent++;

        //Fn(2)+抄读标志(1)+通信延时相关性标识(1)+数据长度(1)+数据(ucDataLen) 容错:不判断从节点附属节点长度
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
                // 发送AFN=14H F3请求
                ret = mrs3762_AFN14_F3_UP(pstAfnParam, MRS_STA_PROTOCOL_2007, ucDataLen, pucContent);
                if (ret == HI_ERR_SUCCESS)
                {
                    mrsSetReadMeterState(EN_MRS_SRV_CCO_READMETER_WAIT_REVISE_RESP);
                    // 重新启动定时器，等待一个新周期
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

        pstCcoCtx->stReadMeterModel.pstAfn14F1 = pstAfn14F1Param;  //保存参数，由CCO处理
        mrsReadterModelCco(HI_NULL);
    } while (0);

    if (ret != HI_ERR_SUCCESS)
    {
        // 组织否认帧
        ret = mrs3762ProtoDenyFrame(&stDenyData,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);

        if (pstAfn14F1Param != HI_NULL)
        {
            pstAfn14F1Param->MRS_AFN14_FREE_FUN(pstAfn14F1Param);
        }
    }
    return ret;
}

//*****************************************************************************
// 函数名称: mrs3762AFN14DLFree
// 功能描述: 释放AFN014F1的下行帧
//
// 参数说明:
//   pafn14 [in/out]
//
// 返 回 值:
//    TODO: ...
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: niesongsong/nKF62735 [2011-12-17]
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
// 函数名称: mrs3762_AFN14_F2
// 功能描述: 路由请求集中器时钟
//
// 参数说明:
//   pafn14 [in/out]
//
// 返 回 值:
//    TODO: ...
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: liuxipeng/lkf54842 [2012-08-07]
// 修    改: cuiate/00233580    [2013-03-30]
//*****************************************************************************
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
HI_U32 mrs3762_AFN14_F2(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    //下行的应用数据
    HI_U8 * pucContent = pstAfnParam->pContent;
    HI_U16 usContentLen = pstAfnParam->usContentLen;

    do
    {
        // Fn(2Byte) + 时间(年月日时分秒)
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

// 该函数没有处理附属结点
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

    if (pstCcoCtx->stReadMeterModel.pstAfn14F1 != HI_NULL) // 如果AFN14参数数据区还没被释放
    {
        // 释放掉此内存区域,准备留给新数据使用
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
        pstAfn14Dl->ucReadFlg = 0x02;   // 可以抄读

        pstAfn14Dl->pucData = mrsToolsMalloc(usDataLen);
        if (pstAfn14Dl->pucData == HI_NULL)
        {
            ret = HI_ERR_MALLOC_FAILUE;
            break;
        }

        (hi_void)memcpy_s(pstAfn14Dl->pucData, usDataLen, pDataBuff, usDataLen);
        pstAfn14Dl->ucDataLen = (HI_U8)usDataLen;

        pstCcoCtx->stReadMeterModel.pstAfn14F1 = pstAfn14F1Param;  //保存参数，由CCO处理
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
    HI_U16 usMinLen = 3;    // Fn(2) + 长度(1)
    HI_U16 usDataLen = 0;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();

/* BEGIN: Modified by fengxiaomin/00209182, 2015/9/7   问题单号号:DTS2015091503701 */
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
            // 标记来源为 "监控载波从节点"
            pstCcoCtx->stAnytimeRMModel.from = XR_FROM_AFN13_F1;
            mrsSetXRStatus(EN_MRS_ANYTIME_RM_REVISE_RESP_PROC);
            
            if (HI_ERR_SUCCESS != ret)
            {
                // 透抄结束
                mrsAnyTimeReadMeterFailEndProc(pstCcoCtx);

                return ret;
            }
/* END:   Modified by fengxiaomin/00209182, 2015/9/7 */

            ret = mrsAnyTimeReadMeterProcessDelay(pucContent + 3, usDataLen);
            if (ret == HI_ERR_SUCCESS)
            {
                // 启动服务结束定时器(到时间后会将随抄状态置为预恢复状态)
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

    // 上行报文内容: 表地址(6) + 延时(2) + 报文长度(1) + 报文内容
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
            // 未找到645帧，返回错误值
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
/* BEGIN: Added by fengxiaomin/00209182, 2014/1/10   问题单号号:DTS2014012607684 */
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
            HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("【ERR】NOT Pemit UPG"));
            break;
        }

        (hi_void)memcpy_s(&usDataLen, sizeof(usDataLen), pContent + MRS_CCO_UPG_DATA_LEN_OFFSET, sizeof(HI_U16));
        if (usContentLen != (MRS_CCO_UPG_FIX_DATA_OFFSET + usDataLen))
        {
            ulSeg = MRS_CCO_UPG_ERROR_SEG;
            HI_DIAG_LOG_MSG_E2(__LINE__, HI_DIAG_MT("【ERR】Content-datalen"),usContentLen,usDataLen);
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
                HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("【ERR】clear ret"),ret);
                break;
            }

            pstMrsUpgCtx->pstFrame = mrsToolsMalloc(sizeof(MRS_3762_FRAME_STRU));
            if (HI_NULL == pstMrsUpgCtx->pstFrame)
            {
                ulSeg = MRS_CCO_UPG_ERROR_SEG;
                HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("【ERR】melloc"));
                break;
            }

            (hi_void)memset_s(pstMrsUpgCtx->pstFrame, sizeof(MRS_3762_FRAME_STRU), 0, sizeof(MRS_3762_FRAME_STRU));
            (hi_void)memcpy_s(pstMrsUpgCtx->pstFrame, sizeof(MRS_3762_FRAME_STRU), pstAfnParam->pstFrame, sizeof(MRS_3762_FRAME_STRU));
		    (hi_void)memcpy_s(&pstMrsUpgCtx->ulClearFileSeg, sizeof(pstMrsUpgCtx->ulClearFileSeg), pContent + MRS_CCO_UPG_CURRENT_SEG_OFFSET, sizeof(HI_U32));
            return ret;
        }

        mrsRemoteUpgRxFilePro();

        //判断文件标识和文件属性
        ucFileProperty = *(pContent + MRS_CCO_UPG_FILE_PROPERTY_OFFSET);
        if (!mrsIsUpgFileFlagValid(ucFileFlag, ucFileProperty))
        {
            ulSeg = MRS_CCO_UPG_ERROR_SEG;
            HI_DIAG_LOG_MSG_E2(__LINE__, HI_DIAG_MT("【ERR】property-flag"),ucFileProperty,ucFileFlag);
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

/* BEGIN: Added by fengxiaomin/00209182, 2014/1/10   问题单号号:DTS2014012607684 */
//*****************************************************************************
// 函数名称: mrsGetSoftwareVer
// 功能描述: 获取CCO版本信息
//
// 参数说明:
//    pVer 版本号
//
// 返 回 值:
//    版本号
//
// 调用要求:
// 调用举例:
// 作    者: fengxiaomin/00209182 [2014-1-10]
//*****************************************************************************
HI_U16 mrsGetSoftwareVer(HI_CHAR *pVer)
{
    HI_U16 usVVer = 0;
    HI_U16 usRVer = 0;
    HI_U16 usSpcVer = 0;
    HI_U16 usVer = 0;  //D15~D12:V版本; D11~D8:R版本; D7~D0:SPC版本

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
// 函数名称: mrsGetVRSpcVer
// 功能描述: 获取V/R/SPC版本号
//
// 参数说明:
//    pVer 版本号
//    pStr 要查找的版本号标志
//
// 返 回 值:
//    版本号
//
// 调用要求:
// 调用举例:
// 作    者: fengxiaomin/00209182 [2014-1-10]
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
// 函数名称: mrsWhetherAddNode
// 功能描述: 判断是否应添加从节点
//
// 参数说明:
//   pucContent [in] AFN11-F1的数据单元
//   ucMeterNum [in] 要添加的电表数量
//   stDenyData [in/out] 否认帧中的错误状态字
//
// 返 回 值:
//   HI_ERR_SUCCESS 应添加从节点
//   HI_ERR_BAD_DATA 地址映射表错误或读取失败
//   HI_ERR_FULL 表档案空间不足，不添加
//   HI_ERR_INITILIZATION_ALREADY 表号重复，不添加
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2012-06-05]
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

    if (HI_NULL == pucContent) //入参判断
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

        //检查是否是BCD码
        usIsBCD = mrsToolsCheckBCD(aucAddr, HI_METER_ADDR_LEN);
        if (usIsBCD != HI_TRUE)
        {
            ret = HI_ERR_BAD_DATA;
            *stDenyData = MRS_13762_DENY_STATUS_ERROR_FORMAT; //格式错误
            return ret;
        }

        //检查是否是广播地址
        usIsBCD = mrsToolsBroadCastAddr(aucAddr);
        if (usIsBCD == HI_TRUE)
        {
            ret = HI_ERR_BAD_DATA;
            *stDenyData = MRS_13762_DENY_STATUS_ERROR_FORMAT; //格式错误
            return ret;
        }

        ret = mrsCheckProtocolValid(ucProtocol);
        if (ret != HI_ERR_SUCCESS)
        {
            *stDenyData = MRS_13762_DENY_STATUS_ERROR_FORMAT; //格式错误
            ret = HI_ERR_BAD_DATA;//规约类型格式错误
            return ret;
        }

        ucMeterIndex++;
    }

    if (mrsSrvArchivesMeterNum() + ucMeterNum > PRODUCT_CFG_MRS_MAX_NODE_NUM) //检查表档案是否已满
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
            *stDenyData = MRS_13762_DENY_STATUS_REPETITION_METER;//要添加的表中存在重复的表号
            ret = HI_ERR_INITILIZATION_ALREADY;
            break;
        }

        mrsMeterAddrPreProc(aucMeterAddr, pucContent + ucMeterIndex * ucUnitConstLen, 
            *(pucContent + ucMeterIndex * ucUnitConstLen + ucUnitConstLen - 1));

        //只查询表档案中的有效表
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
                    *stDenyData = MRS_13762_DENY_STATUS_REPETITION_METER;//表号重复
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
// 函数名称: mrsIsExistRepeatMeter
// 功能描述: 判断要添加的表中是否存在重复表号
//
// 参数说明:
//   pucContent [in] AFN11-F1的数据单元
//   ucMeterIndex [in] 序号
//   ucMeterNum [in] 要添加的电表数量
//
// 返 回 值:
//   HI_ERR_SUCCESS 不存在重复表号
//   HI_ERR_FAILURE 存在重复表号
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2012-11-02]
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
            ret = HI_ERR_FAILURE; //要添加的表中存在重复表号
            break;
        }
    }

    return ret;
}

//*****************************************************************************
// 函数名称: mrsCheckProtocolValid
// 功能描述: 检查规约类型是否有效
//
// 参数说明:
//   ucProto [in] 规约类型
//
// 返 回 值:
//   HI_ERR_FAILURE  规约类型无效
//   HI_ERR_SUCCESS  规约类型有效
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2012-07-03]
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

// AFNF0 F1: PLC测试帧
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

// AFNF0 F2: 过零检测测试帧
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
// AFNF1 F1: 并发读表下行帧处理
HI_U32 mrs3762_AFNF1_F1(HI_INOUT HI_VOID * param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    AFN_FN_UL_PARAM * pstAfnParam = (AFN_FN_UL_PARAM *)param;
    HI_U8 * app_data = pstAfnParam->pContent;
    HI_U16 app_data_len = pstAfnParam->usContentLen;
    AFN_00_F2 deny = MRS_13762_DENY_STATUS_COMMUNICATION_OVERTIME;//否认帧中的错误状态字

    mrsDfxPrUartRx();
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032906596 */
    mrsCcoDfxRmUartRx(app_data, app_data_len, HI_FALSE);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
    
    do
    {
        HI_U8 *data = HI_NULL;
        HI_U16 data_len = 0;
        HI_U8 pos = 0;
        HI_U8 protocol = 0;
        MRS_3762_FRAME_STRU * frame_data = 0;

        // 获取CCO就绪状态(HI_TRUE : 已就绪可以读表; HI_FALSE : 未就绪)
/* BEGIN: Added by fengxiaomin/00209182, 2014/1/10   问题单号号:DTS2014012607684 */
        if(mrsGetCcoReadyStatus(HI_TRUE) == HI_FALSE)
/* END:   Added by fengxiaomin/00209182, 2014/1/10 */
        {
            ret = HI_ERR_BUSY;
            deny = MRS_13762_DENY_STATUS_CCO_BUSY;

            break;
        }

        // 规约类型1Byte + 从节点附属节点数量1Byte + 报文长度2Byte + Fn
        if (app_data_len < 6)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_045, HI_DIAG_MT("len<5!"));
            ret = HI_ERR_BAD_DATA;
            deny = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            break;
        }

        pos += 2;     // Fn

        //检查规约类型是否有效
        ret = mrsCheckProtocolValid(app_data[pos]);
        if (ret != HI_ERR_SUCCESS)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_046, HI_DIAG_MT("protocol err"));
            deny = MRS_13762_DENY_STATUS_ERROR_FORMAT;
            ret = HI_ERR_BAD_DATA;
            break;
        }

        protocol = app_data[pos++]; // 获取规约类型

        // 剩余长度 < 从节点附属节点信息 + 报文长度
        if((app_data_len - pos) < (app_data[pos] * 6 + 2))
        {
            ret = HI_ERR_BAD_DATA;
            deny = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_047, HI_DIAG_MT("extra_len<node_len+data_len"));
            break;
        }

        // 偏移从节点附属节点信息(当前不处理附属节点信息)
        pos = (HI_U8)(pos + app_data[pos] * 6);
        pos++;

        // 获取645报文区的报文长度
        data_len = (HI_U16)(app_data[pos]+ (HI_U16)app_data[pos+1] * 256);
        pos += 2;

        if((app_data_len - pos) < data_len)
        {
            ret = HI_ERR_BAD_DATA;
            deny = MRS_13762_DENY_STATUS_ERROR_LENGTH;
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_048, HI_DIAG_MT("645 frame len err"));
            break;
        }

        data = &app_data[pos];   // 获取转发报文

        frame_data = pstAfnParam->pstFrame; // 获取下行帧信息

        // 强制切换为路由暂停状态
        mrsForceSwitchPause();

        // 调用随抄读表模块进行读表
        ret = mrsParallelReadMeterProc(frame_data->strUsrDataHrd.uInfoRealm.stInfRealmDL.ucSeq, protocol, data, data_len);

        switch(ret)
        {
        case HI_ERR_FULL:   // 并发池满了
        case HI_ERR_NO_MORE_MEMORY: // 系统没有内存了
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
        // 组织否认帧
        mrs3762ProtoDenyFrame(&deny,pstAfnParam,pstAfnParam->ppucOutBuffer,pstAfnParam->pusBufferLen);
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032906596 */
        mrsDfxRmUartDenyTx(app_data, app_data_len);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
        mrsDfxPrNakCnt((HI_U8)deny);
    }

    return HI_ERR_SUCCESS;
}

#endif
#endif

