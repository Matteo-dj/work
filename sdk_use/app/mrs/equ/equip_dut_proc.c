//*****************************************************************************
//
//                  版权所有 (C), 2001-2012, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_product_dut.c
//  版 本 号   : V2.0
//  作    者   : fengxiaomin/00209182
//  生成日期   : 2012年07月20日
//  功能描述   : IO控制实现.
//  函数列表   :
//  修改历史   :
//  1.日    期 :
//    作    者 :
//    修改内容 : 创建文件
//
//*****************************************************************************
#include <mrs_common.h>
#include <mrs_cmd_msg.h>
#include <mrs_fw_tools.h>
#include <mrs_fw_log.h>
#include <mrs_srv_res.h>
#include <mrs_srv_io_dev.h>
#include <mrs_srv_io.h>
#include <mrs_fw_proto376_2_frame.h>
#include <mrs_fw_proto376_2_echo.h>
#include <hi_equip.h>
#include <mrs_fw_proto645.h>
#include <mrs_fw_tools.h>
#include <mrs_fw_n.h>
#include "mrs_fw_nv.h"
#include <hi_phy_nv.h>
#include <hi_mdm.h>
#include <mrs_srv_sta_queue.h>
#include <mrs_srv_sta_baudrate.h>
#include <mrs_srv_parallel_sta.h>
#include <mrs_srv_sta_event.h>
#include <mrs_srv_sta.h>
#include <mrs_srv_sta_searchmeter.h>
#include "mrs_srv_common.h"
#include "equip_dut_proc.h"
#include "mrs_fw_sgd_flash.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

#define MRS_DUT_DI_LEN  (4)

#define EQUIP_DUT_UNNORMAL_ACK(pst645)  \
    do { \
        pst645->stCtrl = MRS_DUT_645_FRAME_CTRL_UP_DENY; \
        pst645->ucDataRealmLen = 1; \
        pst645->ucData[0] = MRS_DUT_645_FRAME_ERROR_DATA; \
    } while (0)

typedef struct
{
    HI_U32 DI;
    MRS_DUT_645_DI_FUN func;
} MRS_DUT_DI_MAP_STRU;


HI_PRV MRS_DUT_645_FRAME_STRU * g_pstZeroCrossFrame = HI_NULL;

HI_PRV HI_VOID mrsDut645Reply_DI7EFF1207(HI_U32 result, EQUIP_ZERO_CROSS_REQ_ACK_STRU *pstResult);

// 查询工作/测试模式（测试本地串口通信）
HI_U32 mrsDut645_DI7EFF1000(HI_VOID *param);

// 控制被测板STA的输出状态
HI_U32 mrsDut645_DIFEFF1100(HI_VOID *param);

// 查询被测板SET和EventOut输入状态
HI_U32 mrsDut645_DI7EFF1101(HI_VOID *param);

//电压跌落中断查询
HI_U32 mrsDut645_DI7EFF1102(HI_VOID * param);

//读取芯片ID
HI_U32 mrsDut645_DI7EFF1103(HI_VOID * param);

//芯片ID写入
HI_U32 mrsDut645_DIFEFF1103(HI_VOID *param);

// MAC地址写入
HI_U32 mrsDut645_DIFEFF1202(HI_VOID *param);

// STA/II采产线工装测试模式/发射功率修改
HI_U32 mrsDut645_DIFEFF1203(HI_VOID *param);

// 查询DUT工作模式
HI_U32 mrsDut645_DI7EFF1204(HI_VOID *param);

// 查询MAC地址和版本号
HI_U32 mrsDut645_DI7EFF1206(HI_VOID *param);

//7EFE1000,支持I采查询模块版本信息
HI_U32 mrsDut645_DI7EFE1000(HI_VOID *param);

// 组网状态查询
HI_U32 mrsDut645_DI7EFF1208(HI_VOID *param);

// II采写MAC和工位ID
HI_U32 mrsDut645_DIFEFF1209(HI_VOID *param);

// 设置客户信息
HI_U32 mrsDut645_DIFEFF1210(HI_VOID *param);
// 设置&查询二级客户信息
HI_U32 mrsDut_FEFF1E10_7EFF1E11(HI_VOID *param);
// 出厂设置客户信息(二级客户)
HI_U32 mrsDut645_DIFEFF1E10(HI_VOID *param);
// 读取二级客户版本信息
HI_U32 mrsDut645_DI7EFF1E11(HI_VOID *param);
// 写入模块ID
HI_U32 mrsDut645_DIFEFF1E16(HI_VOID *param);
HI_U32 mrsDut645_DI7EFF1E16(HI_VOID *param);
//设置&查询三相表标识
HI_U32 mrsDut645_DIFEFF1E12_7EFF1E13(HI_VOID *param);

// II采 写资产编号
HI_U32 mrsDut645_DIFEFF120B(HI_VOID *param);

// II采出厂检验工装发射功率修改
HI_U32 mrsDut645_DIFEFF120C(HI_VOID *param);

// 过零检测
HI_U32 mrsDut645_DI7EFF1207(HI_VOID *param);

// 硬件版本获取
HI_U32 mrsDut645_DI7EFF120E(HI_VOID *param);
// 读取二级客户版本信息
//写三相表模块标识
HI_U32 mrsDut645_DIFEFF1E12(HI_VOID *param);
//读三相表模块标识
HI_U32 mrsDut645_DI7EFF1E13(HI_VOID *param);

#if defined(MRS_SGD_INNER_VER)
// 软件版本获取
HI_U32 mrsDut645_DI7EFF1E18(HI_VOID *param);
#endif
// STA管脚处理函数 置为高/低电平（0-低电平 1-高电平）
HI_U32 mrsDutStaPro(HI_U8 ucStaValue);

// 查询SET和EVENT_OUT管脚状态
HI_U32 mrsDutReadSetEventOut(HI_GPIO_VALUE_E *penBit0, HI_GPIO_VALUE_E *penBit1, HI_GPIO_VALUE_E *penBit2);

// DUT模块中发送645帧数据
HI_U32 mrsDutSendData(HI_U8 *pData, HI_U8 ucDataLen);

// 增加初始化上报
HI_VOID mrsDutStaSendStatus(HI_VOID);
HI_PRV HI_VOID doSendDutAck(MRS_DUT_645_FRAME_STRU *pst645Frame);

// 解析645帧
HI_U32 mrsDutProto645Dec(HI_U8 *pucData, HI_U16 usDataSize, MRS_DUT_645_FRAME_STRU *pst645Frame);


MRS_DUT_DI_MAP_STRU DUT_DI_MAP[] =
{
    {0x7EFF1000, mrsDut645_DI7EFF1000}, // 查询工作/测试模式（测试本地串口通信）
    {0xFEFF1100, mrsDut645_DIFEFF1100}, // 控制被测板STA的输出状态
    {0x7EFF1101, mrsDut645_DI7EFF1101}, // 查询被测板SET和EventOut输入状态
    {0x7EFF1102, mrsDut645_DI7EFF1102},	//电压跌落中断状态查询
    {0x7EFF1103, mrsDut645_DI7EFF1103}, //读取芯片ID
    {0xFEFF1103, mrsDut645_DIFEFF1103}, //芯片ID写入
    {0xFEFF1202, mrsDut645_DIFEFF1202}, // MAC地址写入
    {0xFEFF1203, mrsDut645_DIFEFF1203}, // DUT工作模式修改
    {0x7EFF1204, mrsDut645_DI7EFF1204}, // 查询DUT工作模式
    {0x7EFF1206, mrsDut645_DI7EFF1206}, // 查询MAC地址和版本号
    {0x7EFF1208, mrsDut645_DI7EFF1208}, // 组网状态查询
    {0xFEFF1209, mrsDut645_DIFEFF1209}, // 写MAC和工位ID
    {0xFEFF1210, mrsDut645_DIFEFF1210}, // 写客户信息
    {0xFEFF120B, mrsDut645_DIFEFF120B}, // 写资产编号
    {0xFEFF120C, mrsDut645_DIFEFF120C}, // II采出厂检验工装测试模式/发射功率修改
    {0x7EFF1207, mrsDut645_DI7EFF1207}, // 过零检测
    {0x7EFF120E, mrsDut645_DI7EFF120E}, // 查询硬件版本
	{0x7EFE1000, mrsDut645_DI7EFE1000}, // 支持I采查询模块版本信息
    {0xFEFF1E10, mrsDut645_DIFEFF1E10}, // 写客户信息(二级客户厂商代码)
	{0x7EFF1E11, mrsDut645_DI7EFF1E11}, // 查询客户信息(二级客户厂商代码)
	{0xFEFF1E12, mrsDut645_DIFEFF1E12},// 写三相表模块标识
	{0x7EFF1E13, mrsDut645_DI7EFF1E13},// 读三相表模块标识
	{0xFEFF1E16, mrsDut645_DIFEFF1E16}, // 写入模块ID
	{0x7EFF1E16, mrsDut645_DI7EFF1E16},// 读模块ID
#if defined(MRS_SGD_INNER_VER)	
	{0x7EFF1E18, mrsDut645_DI7EFF1E18}, // 查询软件版本
#endif	
};

#define DUT_DI_MAP_CNT  (sizeof(DUT_DI_MAP) / sizeof(MRS_DUT_DI_MAP_STRU))


HI_U32 DutReadParse(MRS_DUT_645_FRAME_STRU *pst645Frame);
HI_U32 Dut_NVM_Write(HI_U16 usId, HI_CONST HI_PVOID pItemData, HI_U16 ulItemDataLen);

EQUIP_DUT_MAIN_INFO_STRU g_st_equip_main_info = {0};

EQUIP_DUT_MAIN_INFO_STRU * do_GetDutInfo(HI_VOID)
{
    return &g_st_equip_main_info;
}


HI_VOID mrsDutCtxInit(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_NV_FTM_STARTUP_CFG_S stStartUp = {0};

    ret = HI_MDM_NV_Read(HI_NV_FTM_STARTUP_CFG_ID, (HI_PVOID)&stStartUp, sizeof(HI_NV_FTM_STARTUP_CFG_S));
    if (HI_ERR_SUCCESS != ret)
    {
        return;
    }

    g_st_equip_main_info.ulIsEquipMode = stStartUp.ulStatus;
    g_st_equip_main_info.ulInitDutMode = stStartUp.ulStatus;
    g_st_equip_main_info.ulRxDutMode = stStartUp.ulStatus;
    g_st_equip_main_info.zero_cross_phase = 0;

    if (!mrsToolsIsIICollector())
    {
        if (HI_ND_SYS_STATUS_TEST != stStartUp.ulStatus)
        {
            return;
        }

        mrsDutStaSendStatus();

        return;
    }

    if (HI_ND_SYS_STATUS_TEST == stStartUp.ulStatus)
    {
        mrsDutCltIISendStatus();
        mrsDutRunLedOn();
        return;
    }

    return;
}


HI_U32 mrsJudgeEqiupMode(HI_U8 *pucData, HI_U16 usDataLen)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U16 usDataSize = usDataLen;
    HI_U16 pos = 0;
    HI_U16 frmLen = 0;
    HI_U8 dut_addr[HI_METER_ADDR_LEN] = DUT_NORMAL_MODE_ADDR;
    HI_U8 addr[HI_METER_ADDR_LEN] = {0};
    HI_U8 pModeDI[] = MRS_DUT_645_JUDGE_MODE_DI;
    HI_U32 ulDI = 0;
    HI_U32 ulMode = 0;

    if (!pucData || g_st_equip_main_info.ucGetModeFlag)
    {
        return HI_ERR_FAILURE;
    }

    ret = mrsFind645Frame(pucData, (HI_S16)usDataSize, &pos, &frmLen);
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }

    (hi_void)memcpy_s(addr, HI_METER_ADDR_LEN, pucData + pos + MRS_645_FRAME_METERADD_OFFSET, HI_METER_ADDR_LEN);
    (hi_void)memcpy_s(&ulDI, sizeof(HI_U32), pucData + pos + MRS_645_FRAME_DATA_OFFSET, sizeof(ulDI));
    (hi_void)memcpy_s(&ulMode, sizeof(HI_U32), pucData + pos + MRS_645_FRAME_DATA_OFFSET + sizeof(ulDI), sizeof(ulMode));

    mrsHexInvert(dut_addr, sizeof(dut_addr));
    if (!mrsToolsMemEq(addr, dut_addr, HI_METER_ADDR_LEN))
    {
        return HI_ERR_BAD_DATA;
    }

    mrs645DataDecode((HI_U8*)&ulDI, sizeof(ulDI));
    if (memcmp(&ulDI, pModeDI, sizeof(ulDI)) != 0)
    {
        return HI_ERR_BAD_DATA;
    }

    g_st_equip_main_info.ucGetModeFlag = 1;
    mrsDutFrmDeQueue();
    if (HI_ND_SYS_STATUS_NORMAL == g_st_equip_main_info.ulInitDutMode)
    {
        //停止搜表
        mrsStopCltIISearchMeter();
    }

    mrs645DataDecode((HI_U8*)&ulMode, sizeof(ulMode));
    mrsDutSetRxMode(ulMode);

    return ret;
}

HI_VOID mrsDutSetRxMode(HI_U32 ulMode)
{
    if (MRS_DUT_PRODUCT_LINE_MODE == ulMode)
    {
        g_st_equip_main_info.ulRxDutMode = HI_ND_SYS_STATUS_TEST;
    }
    else if (MRS_DUT_PRODUCT_CHECK_MODE == ulMode)
    {
        g_st_equip_main_info.ulRxDutMode = HI_ND_SYS_STATUS_NORMAL;
    }

    return;
}
HI_VOID mrsDutFrmDeQueue(HI_VOID)
{
    MRS_STA_SRV_CTX_STRU * sta = mrsStaGetContext();
    MRS_STA_ITEM  *item;

    if (HI_ND_SYS_STATUS_TEST != g_st_equip_main_info.ulInitDutMode)
    {
        return;
    }

    item = mrsSrvQueueTop(&sta->stQueModule.stMrQueue);
    if ((item != HI_NULL) && (MRS_STA_ITEM_ID_SEND_WORK_MODE_CLT_II == item->id))
    {
        mrsStaTryDeQueue(&sta->stQueModule, mrsStaQueueFree);
    }

    return;
}


HI_U32 mrsSetDutTestMode(HI_U32 ulCurrWorkMode)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    if (ulCurrWorkMode != g_st_equip_main_info.ulInitDutMode)
    {
        ret = HI_MDM_SetWorkStatus(ulCurrWorkMode);
    }

    return ret;
}

#define MRS_DUT_DOWN_AFE_VALUE  0
//原值 9 14 17 20
//衰减 34DB
#define MRS_DUT_DOWM_POWER_CLTII_VALUE_1  -5
#define MRS_DUT_DOWM_POWER_CLTII_VALUE_2  0
#define MRS_DUT_DOWM_POWER_CLTII_VALUE_3  3
#define MRS_DUT_DOWM_POWER_CLTII_VALUE_4  6

//衰减 36DB
#define MRS_DUT_DOWM_POWER_STA_LINE_VALUE_1  -7
#define MRS_DUT_DOWM_POWER_STA_LINE_VALUE_2  -2
#define MRS_DUT_DOWM_POWER_STA_LINE_VALUE_3  1
#define MRS_DUT_DOWM_POWER_STA_LINE_VALUE_4  4

//衰减 40DB
#define MRS_DUT_DOWM_POWER_STA_CHECK_VALUE_1  -11
#define MRS_DUT_DOWM_POWER_STA_CHECK_VALUE_2  -6
#define MRS_DUT_DOWM_POWER_STA_CHECK_VALUE_3  -3
#define MRS_DUT_DOWM_POWER_STA_CHECK_VALUE_4  0

//STA 出厂降40DB
HI_VOID mrsDutDownStaCheckPower(HI_VOID)
{
    //数字发射功率
    HI_MDM_SetPowerStep(MRS_DUT_DOWM_POWER_STA_CHECK_VALUE_1,MRS_DUT_DOWM_POWER_STA_CHECK_VALUE_2,MRS_DUT_DOWM_POWER_STA_CHECK_VALUE_3,MRS_DUT_DOWM_POWER_STA_CHECK_VALUE_4);

    //模拟发射功率
    HI_MDM_SetAfePgaGain(MRS_DUT_DOWN_AFE_VALUE);

}

//STA 产线衰减36DB
HI_VOID mrsDutDownStaLinePower(HI_VOID)
{
    //数字发射功率
    HI_MDM_SetPowerStep(MRS_DUT_DOWM_POWER_STA_LINE_VALUE_1,MRS_DUT_DOWM_POWER_STA_LINE_VALUE_2,MRS_DUT_DOWM_POWER_STA_LINE_VALUE_3,MRS_DUT_DOWM_POWER_STA_LINE_VALUE_4);

    //模拟发射功率
    HI_MDM_SetAfePgaGain(MRS_DUT_DOWN_AFE_VALUE);

}

//II采衰减34DB
HI_VOID mrsDutDownCLTIIPower(HI_VOID)
{
    //数字发射功率
    HI_MDM_SetPowerStep(MRS_DUT_DOWM_POWER_CLTII_VALUE_1,MRS_DUT_DOWM_POWER_CLTII_VALUE_2,MRS_DUT_DOWM_POWER_CLTII_VALUE_3,MRS_DUT_DOWM_POWER_CLTII_VALUE_4);

    //模拟发射功率
    HI_MDM_SetAfePgaGain(MRS_DUT_DOWN_AFE_VALUE);

}


HI_U32 mrsWriteCltAddress(HI_U8* CltAddress)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_NV_FTM_COLLECT_PARAM_FILE_S nv;
    HI_U8 assert_addr[MRS_DUT_ASSERT_ADDRSS_LEN] = MRS_DUT_ASSERT_ADDRSS_DEFAULT;

    if (mrsToolsMemEq(assert_addr, CltAddress, MRS_DUT_ASSERT_ADDRSS_LEN))
    {
        return HI_ERR_SUCCESS;
    }

    (hi_void)memset_s(&nv, sizeof(nv),0, sizeof(nv));
    ret = HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &nv, sizeof(nv));
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }

    (hi_void)memcpy_s(nv.CLTAddress, sizeof(nv.CLTAddress), CltAddress, MRS_DUT_ASSERT_ADDRSS_LEN);
    if (!mrsToolsCheckBCD(nv.CLTAddress, MRS_DUT_ASSERT_ADDRSS_LEN))
    {
        return HI_ERR_FAILURE;
    }

    ret = HI_MDM_NV_Write(HI_NV_FTM_COLLECT_PARAM_FILE, &nv, sizeof(nv));

    return ret;
}

HI_U32 mrsDutSetResultOk(HI_U32 ulEquipMode)
{
    HI_NV_FTM_COLLECT_PARAM_FILE_S cltNv;
    HI_U32 ret = HI_ERR_SUCCESS;

    (hi_void)memset_s(&cltNv, sizeof(cltNv),0, sizeof(cltNv));
    ret = HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &cltNv, sizeof(cltNv));
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }

   switch (ulEquipMode)
    {
        case HI_ND_SYS_STATUS_TEST:
            cltNv.eqt_result = (cltNv.eqt_result & 0xFC) | 0x01;
            break;
        case HI_ND_SYS_STATUS_NORMAL:
            cltNv.eqt_result = (cltNv.eqt_result & 0xF3) | 0x04;
            break;
        default:
            break;
    }

    ret = HI_MDM_NV_Write(HI_NV_FTM_COLLECT_PARAM_FILE, &cltNv, sizeof(cltNv));

    return ret;
}

HI_U32 mrsDutResultReset(HI_U32 ulEquipMode)
{
    HI_NV_FTM_COLLECT_PARAM_FILE_S cltNv;
    HI_U32 ret = HI_ERR_SUCCESS;

    (hi_void)memset_s(&cltNv, sizeof(cltNv),0, sizeof(cltNv));
    ret = HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &cltNv, sizeof(cltNv));
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }

    switch (ulEquipMode)
    {
        case HI_ND_SYS_STATUS_TEST:
            cltNv.eqt_result = (cltNv.eqt_result & 0xFC) | 0x02;
            break;
        case HI_ND_SYS_STATUS_NORMAL:
            cltNv.eqt_result = (cltNv.eqt_result & 0xF3) | 0x08;
            break;
        default:
            break;
    }

    ret = HI_MDM_NV_Write(HI_NV_FTM_COLLECT_PARAM_FILE, &cltNv, sizeof(cltNv));

    return ret;
}


//DUT接收帧处理
HI_U32 mrsDutRxProcess(MRS_ONE_RAW_FRAME_STRU *pstFrame)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_DUT_645_FRAME_STRU *pst645Frame = HI_NULL;

    pst645Frame = (MRS_DUT_645_FRAME_STRU *)mrsToolsMalloc(sizeof(MRS_DUT_645_FRAME_STRU));
    if(!pst645Frame)
    {
        return HI_ERR_NO_MORE_MEMORY;
    }

    (hi_void)memset_s(pst645Frame, sizeof(MRS_DUT_645_FRAME_STRU),0, sizeof(MRS_DUT_645_FRAME_STRU));

    do
    {
        MRS_DUT_645_DI_FUN pDutDIPro = HI_NULL;

        ret = mrsDutProto645Dec(pstFrame->pucDatagram, pstFrame->usDatagramSize, pst645Frame);
        if (HI_ERR_SUCCESS != ret)
        {
            break;
        }

        mrs645DataDecode(pst645Frame->ucData, pst645Frame->ucDataRealmLen);

        pDutDIPro = mrsDutFramePro(pst645Frame);
        if (pDutDIPro)
        {
            ret = pDutDIPro(pst645Frame);
        }
    } while (0);

    mrsToolsFree(pst645Frame);

    return ret;
}


// 解析645帧
HI_U32 mrsDutProto645Dec(HI_U8 *pucData, HI_U16 usDataSize, MRS_DUT_645_FRAME_STRU *pst645Frame)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U16 pos = 0;
    HI_U16 frmLen = 0;
    HI_U8 addr[HI_METER_ADDR_LEN] = MRS_DUT_STA_645_FRAME_ADDR;
    HI_U8 CltAddr[HI_METER_ADDR_LEN] = DUT_NORMAL_MODE_ADDR;

    do
    {
        if (!pucData || !pst645Frame)
        {
            ret = HI_ERR_FAILURE;
            break;
        }

        ret = mrsFind645Frame(pucData, (HI_S16)usDataSize, &pos, &frmLen);
        if (HI_ERR_SUCCESS != ret)
        {
            ret = HI_ERR_BAD_DATA;
            break;
        }

        (hi_void)memcpy_s(pst645Frame, sizeof(MRS_DUT_645_FRAME_STRU), pucData + pos, frmLen);

        if (mrsToolsIsIICollector())
        {
            (hi_void)memcpy_s(addr, HI_METER_ADDR_LEN, CltAddr, HI_METER_ADDR_LEN);
            mrsHexInvert(addr, sizeof(addr));
        }

        if (!mrsToolsMemEq(pst645Frame->ucAddr, addr, HI_METER_ADDR_LEN))
        {
            ret = HI_ERR_FAILURE;
            break;
        }

        if (MRS_DUT_645_FRAME_CTRL != pst645Frame->stCtrl)
        {
            ret = HI_ERR_FAILURE;
            break;
        }
    } while (0);

    return ret;
}


MRS_DUT_645_DI_FUN mrsDutFramePro(MRS_DUT_645_FRAME_STRU *pst645Frame)
{
    MRS_DUT_645_DI_FUN pDutDiPro = HI_NULL;

    HI_U8 * data = pst645Frame->ucData;
    HI_U16 dataLen = pst645Frame->ucDataRealmLen;
    HI_U32 DI = 0;
    HI_U32 idx;

    if (dataLen < sizeof(DI))
    {
        return HI_NULL;
    }

    (hi_void)memcpy_s(&DI, sizeof(HI_U32), data, sizeof(DI));

    for (idx = 0; idx < DUT_DI_MAP_CNT; idx++)
    {
        if (DI == DUT_DI_MAP[idx].DI)
        {
            pDutDiPro = DUT_DI_MAP[idx].func;
            break;
        }
    }

    return pDutDiPro;
}

// 工装模式下，点亮II采运行灯
HI_VOID mrsDutRunLedOn(HI_VOID)
{
    HI_MDM_LED_On(HI_LED_IDX_2);
}


HI_PRV HI_VOID doSendDutAck(MRS_DUT_645_FRAME_STRU *pst645Frame)
{
    mrs645DataEncode(pst645Frame->ucData, pst645Frame->ucDataRealmLen);

    pst645Frame->ucData[pst645Frame->ucDataRealmLen] =
        mrsToolsCalcCheckSum((HI_PBYTE)pst645Frame, pst645Frame->ucDataRealmLen + EQUIP_DUT_645_MIN_LEN - 2);

    mrsDutSendData((HI_PBYTE)pst645Frame, pst645Frame->ucDataRealmLen + EQUIP_DUT_645_MIN_LEN);
}


HI_U32 DutReadParse(MRS_DUT_645_FRAME_STRU *pst645Frame)
{
    return (pst645Frame->ucDataRealmLen == MRS_DUT_DI_LEN) ? HI_ERR_SUCCESS : HI_ERR_FAILURE;
}


HI_U32 Dut_NVM_Write(HI_U16 usId, HI_CONST HI_PVOID pItemData, HI_U16 ulItemDataLen)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_U32 i = 0;

    for (i = 0; i < EQUIP_WRT_NV_MAX_TIMES; i++)
    {
        ret = HI_MDM_NV_Write(usId, pItemData, ulItemDataLen);
        if (HI_ERR_SUCCESS == ret)
        {
            break;
        }
    }

    return ret;
}


// 查询工作/测试模式，目前为测试本地通信串口/PLC
HI_U32 mrsDut645_DI7EFF1000(HI_VOID *param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_DUT_645_FRAME_STRU *frame = (MRS_DUT_645_FRAME_STRU *)param;

    do
    {
        ret = DutReadParse(frame);
        if (HI_ERR_SUCCESS != ret)
        {
            EQUIP_DUT_UNNORMAL_ACK(frame);
            break;
        }

        frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
        frame->ucData[frame->ucDataRealmLen] = EQUIP_DUT_645_EQUIPMODE;
        frame->ucDataRealmLen++;
    } while (0);

    // 帧结束符
    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;

    doSendDutAck(frame);

    return ret;
}


//控制被测板STA的输出状态
HI_U32 mrsDut645_DIFEFF1100(HI_VOID *param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 ucBit0 = 0;
    MRS_DUT_645_FRAME_STRU *frame = (MRS_DUT_645_FRAME_STRU *)param;

    do
    {
        if (frame->ucDataRealmLen != MRS_DUT_DI_LEN + 1)
        {
            ret = HI_ERR_FAILURE;
            break;
        }

        ucBit0 = (frame->ucData[4]) & 0x01;

        ret = mrsDutStaPro(ucBit0);
        if (HI_ERR_SUCCESS != ret)
        {
            ret = HI_ERR_FAILURE;
            break;
        }
    }
    while (0);

    if (HI_ERR_SUCCESS != ret)
    {
        EQUIP_DUT_UNNORMAL_ACK(frame);
    }
    else
    {
        frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
    }

    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;

    doSendDutAck(frame);

    return ret;
}


// 查询被测板SET和EventOut输入状态
HI_U32 mrsDut645_DI7EFF1101(HI_VOID *param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_GPIO_VALUE_E ucBit0 = HI_GPIO_VALUE0; //Bit0-SET
    HI_GPIO_VALUE_E ucBit1 = HI_GPIO_VALUE0; //Bit1-EVENT_OUT
    HI_GPIO_VALUE_E ucBit2 = HI_GPIO_VALUE0; //Bit2-ON_SITE
    MRS_DUT_645_FRAME_STRU *frame = (MRS_DUT_645_FRAME_STRU *)param;

    do
    {
        ret = DutReadParse(frame);
        if (HI_ERR_SUCCESS != ret)
        {
            break;
        }

        //查询SET管脚状态
        ret = mrsDutReadSetEventOut(&ucBit0, &ucBit1, &ucBit2);
        if (HI_ERR_SUCCESS != ret)
        {
            ret = HI_ERR_FAILURE;
            break;
        }
    } while (0);

    if (HI_ERR_SUCCESS != ret)
    {
        EQUIP_DUT_UNNORMAL_ACK(frame);
    }
    else
    {
        frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
        frame->ucData[frame->ucDataRealmLen] = (hi_u8)((hi_u8)ucBit0 | ((hi_u8)ucBit1 << 1) | ((hi_u8)ucBit2 << 2));
        frame->ucDataRealmLen++;
    }

    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;

    doSendDutAck(frame);

    return ret;
}

//电压跌落中断查询
HI_U32 mrsDut645_DI7EFF1102(HI_VOID * param)
{
	HI_U32 ret = HI_ERR_SUCCESS;
	MRS_DUT_645_FRAME_STRU *frame = (MRS_DUT_645_FRAME_STRU *)param;
	HI_U8 cnt = 0;

	ret = DutReadParse(frame);
	if (HI_ERR_SUCCESS != ret)
	{
		EQUIP_DUT_UNNORMAL_ACK(frame);
	}
	else
	{
		cnt = HI_MDM_VolDeclineCnt();

	    frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
	    frame->ucData[frame->ucDataRealmLen] = (HI_U8)cnt;
	    frame->ucDataRealmLen++;
	}
	
    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;

    doSendDutAck(frame);

	return ret;

}

//读取芯片ID
HI_U32 mrsDut645_DI7EFF1103(HI_VOID * param)
{
	HI_U32 ret = HI_ERR_SUCCESS;
	EQUIP_CHIP_ID_R_STRU idInfo = {0};
	MRS_DUT_645_FRAME_STRU *frame = (MRS_DUT_645_FRAME_STRU *)param;
	
    (hi_void)memset_s(&idInfo, sizeof(idInfo),0, sizeof(idInfo));

	do
	{
		ret = DutReadParse(frame);
		if (HI_ERR_SUCCESS != ret)
		{
			break;
		}

		ret = mrsReadIDProc(&idInfo);
		if(HI_ERR_SUCCESS != ret)
		{
			break;
		}
		
	}while(HI_FALSE);

	if(HI_ERR_SUCCESS != ret)
	{
	    (hi_void)memset_s(idInfo.chipId, MANAGEMENT_ID_SIZE,0, MANAGEMENT_ID_SIZE);
	}

	ret = mrsReadMac(idInfo.mac);
	if(HI_ERR_SUCCESS != ret)
	{
		idInfo.ret |= HI_ERR_READ_CHIP_ID_MAC_FAIL;
	}

	frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
	frame->ucDataRealmLen = MRS_DUT_DI_LEN + sizeof(EQUIP_CHIP_ID_R_STRU);
    (hi_void)memcpy_s(&frame->ucData[MRS_DUT_DI_LEN], sizeof(EQUIP_CHIP_ID_R_STRU), &idInfo, sizeof(EQUIP_CHIP_ID_R_STRU));	
	frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;

	doSendDutAck(frame);
	return ret;
}

//芯片ID写入
HI_U32 mrsDut645_DIFEFF1103(HI_VOID *param)
{
	HI_U32 ulRet = HI_ERR_SUCCESS;
	HI_U8 chipId[24] = {0};
	EQUIP_CHIP_ID_W_STRU stInfo = {0};
	MRS_DUT_645_FRAME_STRU *frame = (MRS_DUT_645_FRAME_STRU *)param;
	
    (hi_void)memset_s(&stInfo, sizeof(EQUIP_CHIP_ID_W_STRU),0, sizeof(EQUIP_CHIP_ID_W_STRU));

	do
	{
		ulRet = ((frame->ucDataRealmLen == (MRS_DUT_DI_LEN + MANAGEMENT_ID_SIZE)) ? HI_ERR_SUCCESS : HI_ERR_FAILURE);
		if (HI_ERR_SUCCESS != ulRet)
		{
			break;
		}

	    (hi_void)memcpy_s(chipId, MANAGEMENT_ID_SIZE, &frame->ucData[MRS_DUT_DI_LEN], MANAGEMENT_ID_SIZE);
		//写入芯片ID
		ulRet = HI_MDM_SetMyManagementId(chipId);
		if(HI_ERR_SUCCESS != ulRet)
		{
			stInfo.ret = HI_ERR_WRITE_CHIP_ID_NV_FAIL;
			break;
		}
		//读取芯片ID
		ulRet = HI_MDM_GetMyManagementId(stInfo.chipId);
		if(HI_ERR_SUCCESS != ulRet)
		{
			stInfo.ret = HI_ERR_READ_CHIP_ID_NV_FAIL;
			break;
		}
	}while(HI_FALSE);

	if(HI_ERR_SUCCESS != ulRet)
	{
	    (hi_void)memset_s(stInfo.chipId, MANAGEMENT_ID_SIZE,0, MANAGEMENT_ID_SIZE);
	}
	//发送芯片ID
	frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
	frame->ucDataRealmLen = MRS_DUT_DI_LEN + sizeof(EQUIP_CHIP_ID_W_STRU);
    (hi_void)memcpy_s(&frame->ucData[MRS_DUT_DI_LEN], sizeof(EQUIP_CHIP_ID_W_STRU), &stInfo, sizeof(EQUIP_CHIP_ID_W_STRU));
		
	frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;

	doSendDutAck(frame);
	
	return ulRet;
}
// MAC地址写入
HI_U32 mrsDut645_DIFEFF1202(HI_VOID *param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_DUT_645_FRAME_STRU *frame = (MRS_DUT_645_FRAME_STRU *)param;

    do
    {
        HI_U8 ucPlcMac[HI_PLC_MAC_ADDR_LEN] = {0};

        (hi_void)memcpy_s(ucPlcMac, HI_PLC_MAC_ADDR_LEN, frame->ucData + MRS_DUT_DI_LEN, HI_PLC_MAC_ADDR_LEN);
        if (mrsToolsAbnormalMac(ucPlcMac))
        {
            ret = HI_ERR_FAILURE;
            break;
        }

        
        ret = HI_MDM_SetPlcMac(ucPlcMac);
        if (HI_ERR_SUCCESS != ret)
        {
            break;
        }

        mrsStaSetMeterToMac(ucPlcMac, STA_TYPE_METER, HI_MDM_METER_MAC);
		
        if (HI_ND_SYS_STATUS_TEST == g_st_equip_main_info.ulInitDutMode)
        {
            mrsDutDownStaLinePower();
        }

    } while (0);

    (hi_void)memcpy_s(frame->ucData + MRS_DUT_DI_LEN, sizeof(HI_U32), &ret, sizeof(ret));
    frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
    frame->ucDataRealmLen = MRS_DUT_DI_LEN + sizeof(ret);
    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;

    doSendDutAck(frame);

    return ret;
}


// STA/II采产线工装测试模式
HI_U32 mrsDut645_DIFEFF1203(HI_VOID *param)
{
    HI_U32 ret = HI_ERR_FAILURE;
    MRS_DUT_645_FRAME_STRU * frame = (MRS_DUT_645_FRAME_STRU *)param;

    do
    {
        HI_U32 ulStatus = 0;

        // 1. 修改被测板工作模式
        (hi_void)memcpy_s(&ulStatus, sizeof(HI_U32), frame->ucData + frame->ucDataRealmLen - sizeof(HI_U32), sizeof(HI_U32));
        if (ulStatus != EQUIP_UE_NORMAL_MODE)
        {
            ret = HI_ERR_FAILURE;
            break;
        }

        ret = HI_MDM_SetWorkStatus(ulStatus);
        if (HI_ERR_SUCCESS != ret)
        {
            break;
        }

        if (mrsToolsIsIICollector())
        {
            mrsDutSetResultOk(HI_ND_SYS_STATUS_TEST);
        }

        ret = HI_MDM_NV_RefreshDataToNvFile();

    } while (0);

    (hi_void)memcpy_s(frame->ucData + MRS_DUT_DI_LEN, sizeof(HI_U32), &ret, sizeof(ret));
    frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
    frame->ucDataRealmLen = MRS_DUT_DI_LEN + sizeof(ret);
    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;

    doSendDutAck(frame);

    return ret;
}


//工作模式查询
HI_U32 mrsDut645_DI7EFF1204(HI_VOID *param)
{
    HI_U32 ret = HI_ERR_FAILURE;
    MRS_DUT_645_FRAME_STRU * frame = (MRS_DUT_645_FRAME_STRU *)param;
    EQUIP_UE_MODE_READ_ACK_STRU result = {0};

    do
    {
        HI_NV_FTM_STARTUP_CFG_S startupNV;

        ret = DutReadParse(frame);
        if (HI_ERR_SUCCESS != ret)
        {
            break;
        }

        (hi_void)memset_s(&startupNV, sizeof(startupNV),0, sizeof(startupNV));
        ret = HI_MDM_NV_Read(HI_NV_FTM_STARTUP_CFG_ID, &startupNV, sizeof(startupNV));
        if (HI_ERR_SUCCESS != ret)
        {
            break;
        }

        result.ucMode = (HI_U8)startupNV.ulStatus;
        result.ucCapability = EQUIP_MODE_READ_CAPABILITY_ZERO_CROSS | EQUIP_MODE_READ_CAPABILITY_HW_VER;
    } while (0);

    result.ulModeRet = ret;

    (hi_void)memcpy_s(frame->ucData + MRS_DUT_DI_LEN, sizeof(EQUIP_UE_MODE_READ_ACK_STRU), &result, sizeof(result));
    frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
    frame->ucDataRealmLen = MRS_DUT_DI_LEN + sizeof(result);
    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;

    doSendDutAck(frame);

    return ret;
}

// 读取STA端mac地址和版本信息
HI_U32 mrsDut645_DI7EFF1206(HI_VOID *param)
{
    HI_U32 ret = HI_ERR_FAILURE;
    MRS_DUT_645_FRAME_STRU * frame = (MRS_DUT_645_FRAME_STRU *)param;
    EQUIP_TEST_MAC_CHK_RLT_STRU stData;
    NV_MRS_USER_VER_S stInfo;
    HI_NV_FTM_MANUFACOTR_ID_S stManuInfo;
	
    (hi_void)memset_s(&stData, sizeof(stData),0, sizeof(stData));

    do
    {
        ret = DutReadParse(frame);
        if (HI_ERR_SUCCESS != ret)
        {
            stData.RltCode= 1;
            break;
        }

        ret = HI_MDM_GetLocalMacAddr(stData.DutMac);
        if (HI_ERR_SUCCESS != ret)
        {
            stData.RltCode= 2;
            (hi_void)memset_s(stData.DutMac, HI_PLC_MAC_ADDR_LEN,0, HI_PLC_MAC_ADDR_LEN);
            break;
        }
        
        mrsHexInvert(stData.DutMac, sizeof(stData.DutMac));

        (hi_void)memset_s(&stInfo, sizeof(stInfo),0, sizeof(stInfo));
        ret = mrsGetUserVerNvCfg(&stInfo);
        if (HI_ERR_SUCCESS != ret)
        {
            stData.RltCode= 3;
            break;
        }
        (hi_void)memset_s(&stManuInfo, sizeof(HI_NV_FTM_MANUFACOTR_ID_S),0, sizeof(HI_NV_FTM_MANUFACOTR_ID_S));
        ret = mrsNVGetszId(&stManuInfo);
        if (HI_ERR_SUCCESS != ret)
        {
            stData.RltCode= 4;
            break;
        }
                
        (hi_void)memcpy_s(stData.usVender, sizeof(HI_U16), stManuInfo.szId, sizeof(stData.usVender));
        (hi_void)memcpy_s(stData.usChip, sizeof(HI_U16), stInfo.szChipCode, sizeof(stData.usChip));
        mrsIntToBcd((HI_U32)stInfo.aucDate[2], &stData.ucVerDate, sizeof(HI_U8));
        mrsIntToBcd((HI_U32)stInfo.aucDate[1], &stData.ucVerMonth, sizeof(HI_U8));
        mrsIntToBcd((HI_U32)stInfo.aucDate[0], &stData.ucVerYear, sizeof(HI_U8));
        (hi_void)memcpy_s(stData.ucVersion, sizeof(stData.ucVersion), &stInfo.usSwVer, sizeof(stData.ucVersion));

        if ((HI_ND_SYS_STATUS_NORMAL == g_st_equip_main_info.ulInitDutMode) && (mrsToolsIsSTA() || mrsToolsIsTSTA()))
        {
            mrsDutDownStaCheckPower();
        }
    } while (0);

    (hi_void)memcpy_s(frame->ucData + MRS_DUT_DI_LEN, sizeof(EQUIP_TEST_MAC_CHK_RLT_STRU), &stData, sizeof(stData));
    frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
    frame->ucDataRealmLen = MRS_DUT_DI_LEN + sizeof(stData);
    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;

    doSendDutAck(frame);

    return ret;
}


//7EFE1000,支持I采查询模块版本信息
HI_U32 mrsDut645_DI7EFE1000(HI_VOID *param)
{
    HI_U32 ulRet = HI_ERR_FAILURE;
    MRS_DUT_645_FRAME_STRU * frame = (MRS_DUT_645_FRAME_STRU *)param;
	HI_U16 usVer = 0;
	HI_U8 ucBootVer = 0;
	HI_U8 ucOffset = 0;

	ulRet = mrsSrvGetVerInfo(&usVer, &ucBootVer);
	if (HI_ERR_SUCCESS != ulRet)
	{
		return ulRet;
	}

	ucOffset += MRS_DUT_DI_LEN;
    (hi_void)memcpy_s(frame->ucData + MRS_DUT_DI_LEN, sizeof(HI_U16), &usVer, sizeof(usVer));
	ucOffset += sizeof(usVer);
	frame->ucData[ucOffset] = ucBootVer;
    frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
    frame->ucDataRealmLen = MRS_DUT_DI_LEN + MRS_SRV_VER_LEN;
    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;

    doSendDutAck(frame);

    return ulRet;
}


// 组网状态查询
HI_U32 mrsDut645_DI7EFF1208(HI_VOID *param)
{
    MRS_DUT_645_FRAME_STRU * frame = (MRS_DUT_645_FRAME_STRU *)param;
    MRS_STA_SRV_CTX_STRU * sta = mrsStaGetContext();
    HI_U32 status = 0;
    HI_U32 ret = HI_ERR_SUCCESS;

    ret = DutReadParse(frame);
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }

    status = (sta->plcState) ? HI_ERR_SUCCESS : HI_ERR_FAILURE;
    (hi_void)memcpy_s(frame->ucData + MRS_DUT_DI_LEN, sizeof(HI_U32), &status, sizeof(status));
    frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
    frame->ucDataRealmLen = MRS_DUT_DI_LEN + sizeof(status);
    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;

    doSendDutAck(frame);

    return HI_ERR_SUCCESS;
}

//II采产线工装写MAC和工位ID
HI_U32 mrsDut645_DIFEFF1209(HI_VOID *param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U32 ulRet = 0;
    MRS_DUT_645_FRAME_STRU *frame = (MRS_DUT_645_FRAME_STRU *)param;

    if (HI_ND_SYS_STATUS_NORMAL == g_st_equip_main_info.ulRxDutMode)
    {
        return HI_ERR_FAILURE;
    }

    mrsDutResultReset(HI_ND_SYS_STATUS_TEST);

    do
    {
        HI_U8 ucPlcMac[HI_PLC_MAC_ADDR_LEN] = {0};
        HI_U8 mac[HI_PLC_MAC_ADDR_LEN] = {0};

        if (HI_ND_SYS_STATUS_TEST != g_st_equip_main_info.ulInitDutMode)
        {
            ret = mrsSetDutTestMode(HI_ND_SYS_STATUS_TEST);
            if (HI_ERR_SUCCESS != ret)
            {
                ret = HI_ERR_FAILURE;
                ulRet |= MRS_DUT_645_MAC_WRITE_NV_ERROR;
                break;
            }

            g_st_equip_main_info.ulIsEquipMode = HI_ND_SYS_STATUS_TEST;
            //通知灯控模块
            HI_MDM_SetDutMode();
            MRS_StopTimer(MRS_STA_TIMER_RUN_LED);
            mrsDutRunLedOn();

        }

        //降低发射功率
        mrsDutDownCLTIIPower();

        (hi_void)memcpy_s(ucPlcMac, HI_PLC_MAC_ADDR_LEN, frame->ucData + MRS_DUT_DI_LEN, HI_PLC_MAC_ADDR_LEN);
        if (mrsToolsAbnormalMac(ucPlcMac))
        {
            ret = HI_ERR_FAILURE;
            ulRet |= MRS_DUT_645_MAC_WRITE_NV_ERROR;
            break;
        }

        ret = HI_MDM_SetPlcMac(ucPlcMac);
        if (HI_ERR_SUCCESS != ret)
        {
            ulRet |= MRS_DUT_645_MAC_WRITE_NV_ERROR;
            break;
        }

        (hi_void)memcpy_s(mac, HI_PLC_MAC_ADDR_LEN, frame->ucData + MRS_DUT_DI_LEN + HI_PLC_MAC_ADDR_LEN, HI_PLC_MAC_ADDR_LEN);
        mrsStaSetMeterToMac(mac, STA_TYPE_INVALID, HI_MDM_METER_MAC);//II型采集器形态,故使用STA_TYPE_INVALID
    } while (0);

    (hi_void)memcpy_s(frame->ucData + MRS_DUT_DI_LEN, sizeof(HI_U32), &ulRet, sizeof(ulRet));
    frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
    frame->ucDataRealmLen = MRS_DUT_DI_LEN + sizeof(ulRet);
    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;

    doSendDutAck(frame);

    return ret;

}

// 出厂设置客户信息
HI_U32 mrsDut645_DIFEFF1210(HI_VOID *param)
{
    HI_U32 ulRet = 0;
    MRS_DUT_645_FRAME_STRU *frame = (MRS_DUT_645_FRAME_STRU *)param;
    
    EQUIP_SET_USER_INF_REQ_STRU stReq;
    EQUIP_SET_USER_INF_IND_STRU stInd;
    NV_MRS_USER_VER_S stVer;

    do
    {
        (hi_void)memset_s(&stReq, sizeof(stReq),0, sizeof(stReq));
        (hi_void)memset_s(&stInd, sizeof(stInd),0, sizeof(stInd));
        (hi_void)memset_s(&stVer, sizeof(stVer),0, sizeof(stVer));
                  
        if (HI_ND_SYS_STATUS_TEST == g_st_equip_main_info.ulRxDutMode)
        {        
            ulRet = HI_ERR_FAILURE;
            break;
        }

        if ((param == HI_NULL) || (frame->ucDataRealmLen == 0))
        {
            return HI_ERR_INVALID_PARAMETER;
        }

        if (frame->ucDataRealmLen != MRS_DUT_DI_LEN + sizeof(stReq))
        {
            ulRet = HI_ERR_BAD_FRAME_SIZE;
            break;
        }

        (hi_void)memcpy_s(&stReq, sizeof(stReq), frame->ucData + MRS_DUT_DI_LEN, sizeof(stReq));
        ulRet = MRS_TimeBinIsValid(stReq.aucDate);
        if (HI_ERR_SUCCESS != ulRet)
        {
            break;
        }

        ulRet = mrsNVSetszId(stReq.aucManuCode);
        if (HI_ERR_SUCCESS != ulRet)
        {
            HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("【ERR】Set szId =%d"),ulRet);
            break;
        }

        (hi_void)memcpy_s(stVer.aucDate, HI_USER_DATE_LENGTH, stReq.aucDate, HI_USER_DATE_LENGTH);
        (hi_void)memcpy_s(stVer.aucUserVersion, HI_USER_VER_LENGTH, stReq.szUserVer, HI_USER_VER_LENGTH);
        (hi_void)memcpy_s(stVer.szChipCode, HI_CHIP_CODE_LENGTH, stReq.szChipCode, HI_CHIP_CODE_LENGTH);
        stVer.usSwVer = stReq.usSwVer;

        ulRet = mrsNVSetUserVerInfo(&stVer);
        if (HI_ERR_SUCCESS != ulRet)
        {
            HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("【ERR】Set UserInfo =%d"),ulRet);
            break;
        }
    }while(HI_FALSE);

    stInd.ulRet = ulRet;
    (hi_void)memcpy_s(frame->ucData + MRS_DUT_DI_LEN, MRS_PROTO645_DATA_CS_END_LEN_MAX - MRS_DUT_DI_LEN, &stInd, sizeof(stInd));
    frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
    frame->ucDataRealmLen = MRS_DUT_DI_LEN + sizeof(stInd);
    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;

    doSendDutAck(frame);
    return ulRet;
}

// II采出厂检验写资产编号
HI_U32 mrsDut645_DIFEFF120B(HI_VOID *param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U32 ulRet = 0;
    MRS_DUT_645_FRAME_STRU *frame = (MRS_DUT_645_FRAME_STRU *)param;

    if (HI_ND_SYS_STATUS_TEST == g_st_equip_main_info.ulRxDutMode)
    {
        return HI_ERR_FAILURE;
    }

    mrsDutResultReset(HI_ND_SYS_STATUS_NORMAL);

    do
    {
        HI_U8 CltAddress[MRS_DUT_ASSERT_ADDRSS_LEN] = {0};
        HI_U8 mac[HI_PLC_MAC_ADDR_LEN] = {0};

        g_st_equip_main_info.ulIsEquipMode = HI_ND_SYS_STATUS_NORMAL;

        if (HI_ND_SYS_STATUS_NORMAL == g_st_equip_main_info.ulInitDutMode)
        {
            //降低发射功率  36DB
            mrsDutDownCLTIIPower();
        }

        (hi_void)memcpy_s(mac, HI_PLC_MAC_ADDR_LEN, frame->ucData + MRS_DUT_DI_LEN + MRS_DUT_ASSERT_ADDRSS_LEN, HI_PLC_MAC_ADDR_LEN);
        //设置工位号给mac
        mrsStaSetMeterToMac(mac, STA_TYPE_INVALID, HI_MDM_METER_MAC);
        //写资产编号
        (hi_void)memcpy_s(CltAddress, MRS_DUT_ASSERT_ADDRSS_LEN, frame->ucData + MRS_DUT_DI_LEN, MRS_DUT_ASSERT_ADDRSS_LEN);
        ret = mrsWriteCltAddress(CltAddress);
        if (HI_ERR_SUCCESS != ret)
        {
            ulRet |= MRS_DUT_645_ASSERT_WRITE_NV_ERROR;
			break;
        }
    }while (0);

    (hi_void)memcpy_s(frame->ucData + MRS_DUT_DI_LEN, sizeof(HI_U32), &ulRet, sizeof(ulRet));
    frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
    frame->ucDataRealmLen = MRS_DUT_DI_LEN + sizeof(ulRet);
    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;

    doSendDutAck(frame);

    return ret;
}

// II采出厂检验工装写结果
HI_U32 mrsDut645_DIFEFF120C(HI_VOID *param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_DUT_645_FRAME_STRU * frame = (MRS_DUT_645_FRAME_STRU *)param;

    mrsDutSetResultOk(HI_ND_SYS_STATUS_NORMAL);

    (hi_void)memcpy_s(frame->ucData + MRS_DUT_DI_LEN, sizeof(HI_U32), &ret, sizeof(ret));
    frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
    frame->ucDataRealmLen = MRS_DUT_DI_LEN + sizeof(ret);
    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;

    doSendDutAck(frame);

    return ret;
}

// 硬件版本获取
HI_U32 mrsDut645_DI7EFF120E(HI_VOID *param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_DUT_645_FRAME_STRU * frame = (MRS_DUT_645_FRAME_STRU *)param;
    EQUIP_HW_VER_ID_REQ_ACK_STRU stHWVerInfo;

    (hi_void)memset_s(&stHWVerInfo, sizeof(stHWVerInfo),0, sizeof(stHWVerInfo));

    do
    {
	    HI_FTM_CHIP_TYPE_E enChipType = HI_FTM_CHIP_TYPE_UNKOWN;
		HI_FTM_PRODUCT_TYPE_E enProductType = HI_FTM_PRODUCT_TYPE_UNKOWN;
		
        ret = DutReadParse(frame);
        if (HI_ERR_SUCCESS != ret)
        {
            stHWVerInfo.ulRet = ret;
            break;
        }

        ret = HI_MDM_GetHwProductId(&enChipType, &enProductType);
        if (HI_ERR_SUCCESS != ret)
        {
            stHWVerInfo.ulRet = HI_ERR_FAILURE;
            break;
        }

        stHWVerInfo.ucHwVerId = (HI_U8)enProductType;
    } while (0);

    (hi_void)memcpy_s(frame->ucData + MRS_DUT_DI_LEN, sizeof(stHWVerInfo), &stHWVerInfo, sizeof(stHWVerInfo));
    frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
    frame->ucDataRealmLen = MRS_DUT_DI_LEN + sizeof(stHWVerInfo);
    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;

    doSendDutAck(frame);

    return ret;
}

#if 0
// 设置&查询二级客户信息
HI_U32 mrsDut_FEFF1E10_7EFF1E11(HI_VOID *param)
{
    MRS_DUT_645_FRAME_STRU * frame = (MRS_DUT_645_FRAME_STRU *)param;
	hi_u8 data[MRS_DUT_DI_LEN] = {0x10,0x1E,0xFF,0xFE};
	HI_PRV EQUIP_SET_USER_INF_REQ_STRU userInfo = {0};
	
	if(mrsToolsMemcmp(frame->ucData,data,MRS_DUT_DI_LEN) == 0){
		mrsToolsMemcpy_s(&userInfo, sizeof(EQUIP_SET_USER_INF_REQ_STRU),
						&frame->ucData[MRS_DUT_DI_LEN], sizeof(EQUIP_SET_USER_INF_REQ_STRU));
		frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
		frame->ucDataRealmLen = MRS_DUT_DI_LEN + sizeof(HI_U32);
		mrsToolsMemZero_s(&frame->ucData[MRS_DUT_DI_LEN], sizeof(HI_U32), sizeof(HI_U32));
	}else{
		frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
		frame->ucDataRealmLen = MRS_DUT_DI_LEN + sizeof(EQUIP_SET_USER_INF_REQ_STRU);
		mrsToolsMemcpy_s(&frame->ucData[MRS_DUT_DI_LEN], sizeof(EQUIP_SET_USER_INF_REQ_STRU),
						&userInfo, sizeof(EQUIP_SET_USER_INF_REQ_STRU));
	}
	
    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;
    doSendDutAck(frame);

    return HI_ERR_SUCCESS;
}

// 写入模块ID
HI_U32 mrsWriteModuleId(HI_U8* moduleId)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_NV_FTM_MANAGEMENT_ID_S nv;

    mrsToolsMemZero_s(&nv, sizeof(nv), sizeof(nv));
    ret = HI_MDM_NV_Read(HI_NV_FTM_MANAGEMENT_ID, &nv, sizeof(nv));
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }
	mrsToolsMemcpy_s(nv.dev_id, sizeof(nv.dev_id),moduleId,MRS_SRV_MODULE_ID_LEN);

    ret = HI_MDM_NV_Write(HI_NV_FTM_MANAGEMENT_ID, &nv, sizeof(nv));

    return ret;
}

HI_U32 mrsDut645_DIFEFF1E16(HI_VOID *param)
{
    HI_U32 ulRet = 0;
    MRS_DUT_645_FRAME_STRU *frame = (MRS_DUT_645_FRAME_STRU *)param;
	HI_U8 module_id_ret[2 + MRS_SRV_MODULE_ID_LEN] = {0};
    do
    {
		if (frame->ucDataRealmLen != MRS_DUT_DI_LEN + frame->ucData[MRS_DUT_DI_LEN] + 1
			|| MRS_SRV_MODULE_ID_LEN != frame->ucData[MRS_DUT_DI_LEN])
        {
            ulRet = HI_ERR_BAD_FRAME_SIZE;
			module_id_ret[0] = 1;
            break;
        }
		mrsToolsMemcpy_s(&module_id_ret[2], MRS_SRV_MODULE_ID_LEN,&frame->ucData[MRS_DUT_DI_LEN + 1],MRS_SRV_MODULE_ID_LEN);
	
        ulRet = mrsWriteModuleId(&module_id_ret[2]);
        if (HI_ERR_SUCCESS != ulRet)
        {
            ulRet |= MRS_DUT_645_ASSERT_WRITE_NV_ERROR;
			module_id_ret[0] = 1;
			break;
        }
		module_id_ret[1] = MRS_SRV_MODULE_ID_LEN;
    }while (0);

    mrsToolsMemcpy_s(frame->ucData + MRS_DUT_DI_LEN, 2 + MRS_SRV_MODULE_ID_LEN, module_id_ret, 2 + MRS_SRV_MODULE_ID_LEN);
    frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
    frame->ucDataRealmLen = MRS_DUT_DI_LEN + 2 + MRS_SRV_MODULE_ID_LEN;
    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;

    doSendDutAck(frame);

    return ulRet;
}

#if 0
HI_U32 mrsDut645_DIFEFF1E16(HI_VOID *param)
{
    MRS_DUT_645_FRAME_STRU *frame = (MRS_DUT_645_FRAME_STRU *)param;
	hi_u8 id[20];
	hi_u8 length = 0;
	
	length = frame->ucData[MRS_DUT_DI_LEN];
    mrsToolsMemcpy_s(id, length, &frame->ucData[MRS_DUT_DI_LEN + 1], length);	
    frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
    frame->ucDataRealmLen++;
	frame->ucData[MRS_DUT_DI_LEN] = HI_ERR_SUCCESS;
	frame->ucData[MRS_DUT_DI_LEN + 1] = length;
    mrsToolsMemcpy_s(&frame->ucData[MRS_DUT_DI_LEN + 2], length, id, length);
    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;
    doSendDutAck(frame);
	
    return HI_ERR_SUCCESS;
}
#endif

//设置、查询三相表标识
HI_U32 mrsDut645_DIFEFF1E12_7EFF1E13(HI_VOID *param)
{
    MRS_DUT_645_FRAME_STRU * frame = (MRS_DUT_645_FRAME_STRU *)param;
	Equip_ReadPhaseFlag_Up_S stReadPhaseFlag = {0};
	hi_u8 data[MRS_DUT_DI_LEN] = {0x12,0x1E,0xFF,0xFE};
	HI_PRV hi_u8 type = 0;

	if(mrsToolsMemcmp(frame->ucData,data,MRS_DUT_DI_LEN) == 0){
		type = frame->ucData[MRS_DUT_DI_LEN];
		frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
		frame->ucDataRealmLen = MRS_DUT_DI_LEN + sizeof(HI_U32);
		mrsToolsMemZero_s(&frame->ucData[MRS_DUT_DI_LEN], sizeof(HI_U32), sizeof(HI_U32));	
	}else{		
		frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
		frame->ucDataRealmLen = MRS_DUT_DI_LEN + sizeof(Equip_ReadPhaseFlag_Up_S);
		stReadPhaseFlag.type = type;
		stReadPhaseFlag.ret = HI_ERR_SUCCESS;
		mrsToolsMemcpy_s(&frame->ucData[MRS_DUT_DI_LEN], sizeof(Equip_ReadPhaseFlag_Up_S), 
						&stReadPhaseFlag, sizeof(Equip_ReadPhaseFlag_Up_S));	
	}
    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;
    doSendDutAck(frame);
    return HI_ERR_SUCCESS;
}
#endif
#if defined(MRS_SGD_INNER_VER)
// 软件版本获取
HI_U32 mrsDut645_DI7EFF1E18(HI_VOID *param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_DUT_645_FRAME_STRU * frame = (MRS_DUT_645_FRAME_STRU *)param;
    EQUIP_SW_VER_ID_REQ_ACK_STRU stSWVerInfo;
//	HI_U8 ucVer1= 0;
//	HI_U8 ucVer2 = 0;
    (hi_void)memset_s(&stSWVerInfo, sizeof(stSWVerInfo),0, sizeof(stSWVerInfo));

    do
    {
       
        ret = DutReadParse(frame);
        if (HI_ERR_SUCCESS != ret)
        {
            stSWVerInfo.rltCode = 1;
            break;
        }

        ret = mrsSrvGetInnerVer(stSWVerInfo.aucSwInnerVer);
        if (HI_ERR_SUCCESS != ret)
        {
            stSWVerInfo.rltCode = 2;
            break;
        }
	//ucVer1 = stSWVerInfo.aucSwInnerVer[0];
	//ucVer2 = stSWVerInfo.aucSwInnerVer[1];
        //stSWVerInfo.aucSwExtVer =  ((HI_U16)(( (ucVer1 >> 1) & 0xf0) | ((ucVer1 & 0x1f)/10)) << 8) | ((((ucVer1 & 0x1f)%10) << 4) |( ucVer2 & 0xf));

        stSWVerInfo.aucSwExtVer = (HI_U16)EXT_VER;
    } while (0);

    (hi_void)memcpy_s(frame->ucData + MRS_DUT_DI_LEN, sizeof(stSWVerInfo), &stSWVerInfo, sizeof(stSWVerInfo));
    frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
    frame->ucDataRealmLen = MRS_DUT_DI_LEN + sizeof(stSWVerInfo);
    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;

    doSendDutAck(frame);

    return ret;
}
#endif
// 设置&查询二级客户信息
HI_U32 mrsDut_FEFF1E10_7EFF1E11(HI_VOID *param)
{
    MRS_DUT_645_FRAME_STRU * frame = (MRS_DUT_645_FRAME_STRU *)param;
	hi_u8 data[MRS_DUT_DI_LEN] = {0x10,0x1E,0xFF,0xFE};
	HI_PRV EQUIP_SET_USER_INF_REQ_STRU userInfo = {{0}, {0}, {0}, 0, {0}, {0}};
	
	if(memcmp(frame->ucData,data,MRS_DUT_DI_LEN) == 0){
	    (hi_void)memcpy_s(&userInfo, sizeof(EQUIP_SET_USER_INF_REQ_STRU),
						&frame->ucData[MRS_DUT_DI_LEN], sizeof(EQUIP_SET_USER_INF_REQ_STRU));
		frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
		frame->ucDataRealmLen = MRS_DUT_DI_LEN + sizeof(HI_U32);
	    (hi_void)memset_s(&frame->ucData[MRS_DUT_DI_LEN], sizeof(HI_U32),0, sizeof(HI_U32));
	}else{
		frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
		frame->ucDataRealmLen = MRS_DUT_DI_LEN + sizeof(EQUIP_SET_USER_INF_REQ_STRU);
	    (hi_void)memcpy_s(&frame->ucData[MRS_DUT_DI_LEN], sizeof(EQUIP_SET_USER_INF_REQ_STRU),
						&userInfo, sizeof(EQUIP_SET_USER_INF_REQ_STRU));
	}
	
    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;
    doSendDutAck(frame);

    return HI_ERR_SUCCESS;
}

// 写入模块ID
#if 0//未实现
HI_U32 mrsDut645_DIFEFF1E16(HI_VOID *param)
{
    MRS_DUT_645_FRAME_STRU *frame = (MRS_DUT_645_FRAME_STRU *)param;
	hi_u8 id[20];
	hi_u8 length = 0;

    /* 注意: 这里需要考虑存储顺序和HPLC报文上报的顺序，HPLC需要逆序上报 */
    /* 		 存储功能现在没有实现，需要根据各地需求存储。 */
	/*       对应读的接口为mrsGetDevIdDev，也需相应修改。 */
    /* 建议: 这里按照报文顺序保存，在上报HPLC报文的地方mrsGetDevIdDev实现逆序。 */
    /*       目前mrsGetDevIdDev中已经逆序。 */
   
	length = frame->ucData[MRS_DUT_DI_LEN];
    (hi_void)memcpy_s(id, length, &frame->ucData[MRS_DUT_DI_LEN + 1], length);	
    frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
    frame->ucDataRealmLen++;
	frame->ucData[MRS_DUT_DI_LEN] = HI_ERR_SUCCESS;
	frame->ucData[MRS_DUT_DI_LEN + 1] = length;
    (hi_void)memcpy_s(&frame->ucData[MRS_DUT_DI_LEN + 2], length, id, length);
    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;
    doSendDutAck(frame);
	
    return HI_ERR_SUCCESS;
}
#endif
#if 1	//weichao-20190320
//设置、查询三相表标识
HI_U32 mrsDut645_DIFEFF1E12_7EFF1E13(HI_VOID *param)
{
    MRS_DUT_645_FRAME_STRU * frame = (MRS_DUT_645_FRAME_STRU *)param;
	Equip_ReadPhaseFlag_Up_S stReadPhaseFlag = {0};
	hi_u8 data[MRS_DUT_DI_LEN] = {0x12,0x1E,0xFF,0xFE};
	HI_PRV hi_u8 type = 0;

	if(memcmp(frame->ucData,data,MRS_DUT_DI_LEN) == 0){
		type = frame->ucData[MRS_DUT_DI_LEN];
		frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
		frame->ucDataRealmLen = MRS_DUT_DI_LEN + sizeof(HI_U32);
	    (hi_void)memset_s(&frame->ucData[MRS_DUT_DI_LEN], sizeof(HI_U32),0, sizeof(HI_U32));	
	}else{		
		frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
		frame->ucDataRealmLen = MRS_DUT_DI_LEN + sizeof(Equip_ReadPhaseFlag_Up_S);
		stReadPhaseFlag.type = type;
		stReadPhaseFlag.ret = HI_ERR_SUCCESS;
	    (hi_void)memcpy_s(&frame->ucData[MRS_DUT_DI_LEN], sizeof(Equip_ReadPhaseFlag_Up_S), 
						&stReadPhaseFlag, sizeof(Equip_ReadPhaseFlag_Up_S));	
	}
    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;
    doSendDutAck(frame);
    return HI_ERR_SUCCESS;
}
#endif

// STA管脚处理函数 置为高/低电平（0-低电平 1-高电平）
HI_U32 mrsDutStaPro(HI_U8 ucStaValue)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_FTM_PRODUCT_TYPE_E product_type = HI_FTM_PRODUCT_TYPE_UNKOWN;

    // 将STA管脚设置为高/低电平
    (HI_VOID)HI_MDM_GetHwProductId(HI_NULL, &product_type);
    if (((product_type >= HI_FTM_PRODUCT_TYPE_M102)&&(product_type <= HI_FTM_PRODUCT_TYPE_M108))
        || ((product_type >= HI_FTM_PRODUCT_TYPE_M131)&&(product_type <= HI_FTM_PRODUCT_TYPE_M138))
        || ((product_type >= HI_FTM_PRODUCT_TYPE_T31)&&(product_type <= HI_FTM_PRODUCT_TYPE_T34)))
    {
        if(ucStaValue)
        {
            ucStaValue = HI_GPIO_VALUE0;
        }
        else
        {
            ucStaValue = HI_GPIO_VALUE1;
        }
    }

    //SET 管脚  V200 GPIO 16
    ret = HI_MDM_IO_SetOutputVal((HI_GPIO_IDX_E)(FOOTPRINT_GPIO_AREA_TWO * MRS_IO_INDEX_OFFSET_SIZE + FOOTPRINT_GPIO_BIT_ZERO), (HI_GPIO_VALUE_E)ucStaValue);
    if (HI_ERR_SUCCESS != ret)
    {
        ret = HI_ERR_FAILURE;
    }

    return ret;
}


//查询SET和EVENT_OUT管脚状态
HI_U32 mrsDutReadSetEventOut(HI_GPIO_VALUE_E *penBit0, HI_GPIO_VALUE_E *penBit1, HI_GPIO_VALUE_E *penBit2)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    // 开始读取SET管脚状态 8-->gpio 15
    ret = HI_MDM_IO_GetInputVal((HI_GPIO_IDX_E)(FOOTPRINT_GPIO_AREA_ONE * MRS_IO_INDEX_OFFSET_SIZE + FOOTPRINT_GPIO_BIT_SEVEN),
                            penBit0);
    if (HI_ERR_SUCCESS != ret)
    {
        ret = HI_ERR_FAILURE;
        return ret;
    }

    // 开始读取EVENT_OUT管脚状态15-->gpio 10
    ret = HI_MDM_IO_GetInputVal((HI_GPIO_IDX_E)(FOOTPRINT_GPIO_AREA_ONE * MRS_IO_INDEX_OFFSET_SIZE + FOOTPRINT_GPIO_BIT_TWO),
                            penBit1);
    if (HI_ERR_SUCCESS != ret)
    {
        ret = HI_ERR_FAILURE;
        return ret;
    }

	// 开始读取在位管脚状态gpio 26
    ret = HI_MDM_IO_GetInputVal((HI_GPIO_IDX_E)(FOOTPRINT_GPIO_AREA_THREE * MRS_IO_INDEX_OFFSET_SIZE + FOOTPRINT_GPIO_BIT_TWO),
                            penBit2);
    if (HI_ERR_SUCCESS != ret)
    {
        ret = HI_ERR_FAILURE;
        return ret;
    }

    return ret;
}


//DUT模块中发送645帧数据
HI_U32 mrsDutSendData(HI_U8 *pData, HI_U8 ucDataLen)
{
    return MRS_SendMrData(pData, ucDataLen, HI_DMS_CHL_UART_PORT_APP);
}


// STA组包645数据上报就绪状态
HI_VOID mrsDutStaSendStatus(HI_VOID)
{
    HI_U8 frame[] = {0x68,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0x68,0x9E,0x08,0x35,0x35,0x32,0x91,0x35,0x33,0x34,0x33,0x3A,0x16};

    MRS_SendMrData(frame, sizeof(frame), HI_DMS_CHL_UART_PORT_APP);
}


// II采组包645数据上报就绪状态
HI_VOID mrsDutCltIISendStatus(HI_VOID)
{
    MRS_STA_ITEM * item = HI_NULL;
    HI_U8 frame[] = MRS_DUT_645_WORK_MODE_FRAME;
    MRS_STA_SRV_CTX_STRU *sta = mrsStaGetContext();

    //将数据发送到队列以后处理
    item = mrsToolsMalloc(sizeof(MRS_STA_ITEM) + sizeof(frame));
    if (item == HI_NULL)
    {
        return;
    }

    (hi_void)memset_s(item, sizeof(MRS_STA_ITEM) + sizeof(frame),0, sizeof(MRS_STA_ITEM) + sizeof(frame));

    item->buf = (HI_U8 *)item + sizeof(MRS_STA_ITEM);   // 偏移数据缓冲区指针
    item->len = sizeof(frame); // 获取数据长度
    item->from = MRS_STA_QUEUE_FROM_STA;
    item->id = MRS_STA_ITEM_ID_SEND_WORK_MODE_CLT_II;
    item->time_out = MRS_DUT_645_TIMEOUT; //单位:百毫秒
    item->bAutoStrategy = HI_TRUE;
    item->ucFrameTimeout = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameInterval(MRS_STA_RM_CFG_LOCAL));
    item->ucProtocol = METER_PROTO_645_2007;

    (hi_void)memcpy_s(item->buf, sizeof(frame), frame, sizeof(frame));     // 报文内容

    //尝试加入队列
    if(!mrsStaTryEnQueue(&sta->stQueModule,item))
    {
        mrsToolsFree(item);
    }

    //通知队列
    mrsStaNotifyQueue();

    return;
}


HI_VOID mrsDut645Reply_DI7EFF1207(HI_U32 result, EQUIP_ZERO_CROSS_REQ_ACK_STRU *pstResult)
{
    MRS_DUT_645_FRAME_STRU * pFrame = g_pstZeroCrossFrame;
    EQUIP_ZERO_CROSS_REQ_ACK_STRU stResult;
    (hi_void)memset_s(&stResult, sizeof(stResult),0, sizeof(stResult));

    if (pstResult)
    {
        (hi_void)memcpy_s(pFrame->ucData + MRS_DUT_DI_LEN, sizeof(EQUIP_ZERO_CROSS_REQ_ACK_STRU), pstResult, sizeof(EQUIP_ZERO_CROSS_REQ_ACK_STRU));
    }
    else
    {
        stResult.ret = result;
        (hi_void)memcpy_s(pFrame->ucData + MRS_DUT_DI_LEN, sizeof(EQUIP_ZERO_CROSS_REQ_ACK_STRU), &stResult, sizeof(stResult));

        if (result == HI_ERR_TIMEOUT) {
            HI_MDM_CollectNtb(HI_FALSE, 0, 0, 0, HI_NULL);
        }
    }
    
    pFrame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
    pFrame->ucDataRealmLen = MRS_DUT_DI_LEN + sizeof(stResult);
    pFrame->ucData[pFrame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;

    doSendDutAck(pFrame);

    mrsToolsFree(g_pstZeroCrossFrame);
}


// 过零检测
HI_U32 mrsDut645_DI7EFF1207(HI_VOID *param)
{
    HI_U32 ret = HI_ERR_FAILURE;
    MRS_DUT_645_FRAME_STRU * pFrame = (MRS_DUT_645_FRAME_STRU *)param;

    do
    {
        EQUIP_ZERO_CROSS_REQ_STRU req;

        (hi_void)memset_s(&req, sizeof(req), 0, sizeof(req));
        mrsToolsFree(g_pstZeroCrossFrame);

        g_pstZeroCrossFrame = (MRS_DUT_645_FRAME_STRU *)mrsToolsMalloc(sizeof(MRS_DUT_645_FRAME_STRU));
        if (HI_NULL == g_pstZeroCrossFrame)
        {
            break;
        }
            
        (hi_void)memset_s(g_pstZeroCrossFrame, sizeof(MRS_DUT_645_FRAME_STRU), 0, sizeof(MRS_DUT_645_FRAME_STRU));
        (hi_void)memcpy_s(g_pstZeroCrossFrame, sizeof(MRS_DUT_645_FRAME_STRU), pFrame, sizeof(MRS_DUT_645_FRAME_STRU));

        if (pFrame->ucDataRealmLen != MRS_DUT_DI_LEN + sizeof(EQUIP_ZERO_CROSS_REQ_STRU))
        {
            ret = HI_ERR_INVALID_PARAMETER;
            break;
        }

        (hi_void)memcpy_s(&req, sizeof(req), g_pstZeroCrossFrame->ucData + MRS_DUT_DI_LEN, sizeof(EQUIP_ZERO_CROSS_REQ_STRU));
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

        MRS_StartTimer(MRS_STA_TIMER_ZERO_CROSS, MRS_STA_TIME_ZERO_CROSS, HI_SYS_TIMER_ONESHOT);

        return HI_ERR_SUCCESS;
    } while (0);

    mrsDut645Reply_DI7EFF1207(ret, HI_NULL);

    return ret;
}


// 出厂设置客户信息(二级客户)
HI_U32 mrsDut645_DIFEFF1E10(HI_VOID *param)
{
    HI_U32 ulRet = 0;
    MRS_DUT_645_FRAME_STRU *frame = (MRS_DUT_645_FRAME_STRU *)param;
    
    EQUIP_SET_USER_INF_REQ_STRU stReq;
    EQUIP_SET_USER_INF_IND_STRU stInd;
    NV_MRS_USER_VER_S stVer;

    do
    {
        (hi_void)memset_s(&stReq, sizeof(stReq),0, sizeof(stReq));
        (hi_void)memset_s(&stInd, sizeof(stInd), 0,sizeof(stInd));
        (hi_void)memset_s(&stVer, sizeof(stVer), 0,sizeof(stVer));
        

        if ((param == HI_NULL) || (frame->ucDataRealmLen == 0))
        {
            HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("【ERR】INVALID")); 
            return HI_ERR_INVALID_PARAMETER;
        }

        if (frame->ucDataRealmLen != MRS_DUT_DI_LEN + sizeof(stReq))
        {
            ulRet = HI_ERR_BAD_FRAME_SIZE;
            HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("【ERR】DATA LEN  =%d"),sizeof(stReq)); 
            break;
        }

        (hi_void)memcpy_s(&stReq, sizeof(stReq), frame->ucData + MRS_DUT_DI_LEN, sizeof(stReq));
        ulRet = MRS_TimeBinIsValid(stReq.aucDate);
        if (HI_ERR_SUCCESS != ulRet)
        {
            HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("【ERR】date =%d"),ulRet);
            break;
        }

        ulRet = mrsNVSetClientId(stReq.aucManuCode);//mrsNVSetszId(stReq.aucManuCode);
        if (HI_ERR_SUCCESS != ulRet)
        {
            HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("【ERR】Set szId =%d"),ulRet);
            break;
        }

        (hi_void)memcpy_s(stVer.aucDate, HI_USER_DATE_LENGTH, stReq.aucDate, HI_USER_DATE_LENGTH);
        (hi_void)memcpy_s(stVer.aucUserVersion, HI_USER_VER_LENGTH, stReq.szUserVer, HI_USER_VER_LENGTH);
        (hi_void)memcpy_s(stVer.szChipCode, HI_CHIP_CODE_LENGTH, stReq.szChipCode, HI_CHIP_CODE_LENGTH);
        stVer.usSwVer = stReq.usSwVer;
	(hi_void)memcpy_s(stVer.reserved+21, 2,stReq.aucManuCode,2);
	
        ulRet = mrsNVSetUserVerInfo(&stVer);
        if (HI_ERR_SUCCESS != ulRet)
        {
            HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("【ERR】Set UserInfo =%d"),ulRet);
            break;
        }
    }while(HI_FALSE);

    stInd.ulRet = ulRet;
    (hi_void)memcpy_s(frame->ucData + MRS_DUT_DI_LEN, MRS_PROTO645_DATA_CS_END_LEN_MAX - MRS_DUT_DI_LEN, &stInd, sizeof(stInd));
    frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
    frame->ucDataRealmLen = MRS_DUT_DI_LEN + sizeof(stInd);
    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;

    doSendDutAck(frame);
    return ulRet;
}


// 读取二级客户版本信息
HI_U32 mrsDut645_DI7EFF1E11(HI_VOID *param)
{
    HI_U32 ret = HI_ERR_FAILURE;
    MRS_DUT_645_FRAME_STRU * frame = (MRS_DUT_645_FRAME_STRU *)param;
    //EQUIP_TEST_MAC_CHK_RLT_STRU stData;
	EQUIP_SET_USER_INF_REQ_STRU stData;
    NV_MRS_USER_VER_S stInfo;
    FLASH_USR_CFG_INFO_S stCfg;
    //HI_U8 aucCode[4] = {0};
	HI_U8 rltCode = 0;
	
    (hi_void)memset_s(&stData, sizeof(stData), 0,sizeof(stData));

    do
    {
        ret = DutReadParse(frame);
        if (HI_ERR_SUCCESS != ret)
        {
            rltCode= 1;
            break;
        }

        (hi_void)memset_s(&stInfo, sizeof(stInfo), 0,sizeof(stInfo));
        ret = mrsGetUserVerNvCfg(&stInfo);
        if (HI_ERR_SUCCESS != ret)
        {
            rltCode= 3;
            break;
        }
        (hi_void)memset_s(&stCfg, sizeof(stCfg), 0,sizeof(stCfg));
        ret = mrsNVGetClientId(&stCfg);
        if (HI_ERR_SUCCESS != ret)
        {
            rltCode= 4;
            break;
        }
                
        (hi_void)memcpy_s(stData.szUserVer, HI_USER_VER_LENGTH, stInfo.aucUserVersion, HI_USER_VER_LENGTH);
        (hi_void)memcpy_s(stData.szChipCode, HI_CHIP_CODE_LENGTH, stInfo.szChipCode, HI_CHIP_CODE_LENGTH);
       	(hi_void)memcpy_s(stData.aucDate, HI_USER_DATE_LENGTH, stInfo.aucDate, HI_USER_DATE_LENGTH);
        stData.usSwVer = stInfo.usSwVer;
	(hi_void)memcpy_s(stData.aucManuCode,HI_MANU_CODE_LENGTH , stCfg.aucCode, HI_MANU_CODE_LENGTH);
	stData.reserved[0] = rltCode;
       
    } while (0);

    (hi_void)memcpy_s(frame->ucData + MRS_DUT_DI_LEN, sizeof(EQUIP_SET_USER_INF_REQ_STRU), &stData, sizeof(stData));
    frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
    frame->ucDataRealmLen = MRS_DUT_DI_LEN + sizeof(stData);
    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;

    doSendDutAck(frame);

    return ret;
}
// 写模块ID  存NV时逆序--工装命令保持原来的正序
HI_U32 mrsDut645_DIFEFF1E16(HI_VOID *param)
{
    HI_U32 ulRet = 0;
    MRS_DUT_645_FRAME_STRU *frame = (MRS_DUT_645_FRAME_STRU *)param;
	HI_U8 module_id_ret[2 + MRS_DEV_ID_SIZE] = {0};
    do
    {
		if (frame->ucDataRealmLen != MRS_DUT_DI_LEN + frame->ucData[MRS_DUT_DI_LEN] + 1
			|| MRS_DEV_ID_SIZE != frame->ucData[MRS_DUT_DI_LEN])
        {
            ulRet = HI_ERR_BAD_FRAME_SIZE;
			module_id_ret[0] = 1;
            break;
        }
		(hi_void)memcpy_s(&module_id_ret[2], MRS_DEV_ID_SIZE,&frame->ucData[MRS_DUT_DI_LEN + 1],MRS_DEV_ID_SIZE);
		
        ulRet = mrsWriteModuleId(&module_id_ret[2]);
        if (HI_ERR_SUCCESS != ulRet)
        {
            ulRet |= MRS_DUT_645_ASSERT_WRITE_NV_ERROR;
			module_id_ret[0] = 1;
			break;
        }
		module_id_ret[1] = MRS_DEV_ID_SIZE;
    }while (0);

    (hi_void)memcpy_s(frame->ucData + MRS_DUT_DI_LEN, 2 + MRS_DEV_ID_SIZE, module_id_ret, 2 + MRS_DEV_ID_SIZE);
    frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
    frame->ucDataRealmLen = MRS_DUT_DI_LEN + 2 + MRS_DEV_ID_SIZE;
    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;

    doSendDutAck(frame);

    return ulRet;
}
//读模块ID
HI_U32 mrsDut645_DI7EFF1E16(HI_VOID *param)
{
    HI_U32 ret = HI_ERR_FAILURE;
    MRS_DUT_645_FRAME_STRU * frame = (MRS_DUT_645_FRAME_STRU *)param;
	HI_U8 module_id_ret[2 + MRS_DEV_ID_SIZE] = {0};
    do
    {
        ret = DutReadParse(frame);
        if (HI_ERR_SUCCESS != ret)
        {
            module_id_ret[0] = 2;
            break;
        }
        ret = mrsGetModule(&module_id_ret[2]);
        if (HI_ERR_SUCCESS != ret)
        {
            module_id_ret[0] = 2;
            break;
        }
        module_id_ret[1] = MRS_DEV_ID_SIZE; 
    } while (0);

	(hi_void)memcpy_s(&frame->ucData[MRS_DUT_DI_LEN],2 + MRS_DEV_ID_SIZE,module_id_ret,2+ MRS_DEV_ID_SIZE);
    frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
    frame->ucDataRealmLen = MRS_DUT_DI_LEN + 2 + MRS_DEV_ID_SIZE;
    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;

    doSendDutAck(frame);

    return ret;
}
HI_U32 mrsWriteTstaFlag(HI_U8 flag)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_NV_FTM_COLLECT_PARAM_FILE_S nv;

    (hi_void)memset_s(&nv, sizeof(nv),0, sizeof(nv));
    ret = HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &nv, sizeof(nv));
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }
	if(nv.isTsta == flag)
	{
		return ret;
	}
  	nv.isTsta = flag;

    ret = HI_MDM_NV_Write(HI_NV_FTM_COLLECT_PARAM_FILE, &nv, sizeof(nv));

    return ret;
}

HI_U32 mrsGetTstaFlag(HI_U8 *flag)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_NV_FTM_COLLECT_PARAM_FILE_S nv;

    (hi_void)memset_s(&nv, sizeof(nv), 0,sizeof(nv));
    ret = HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &nv, sizeof(nv));
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }
	*flag = nv.isTsta;
    return ret;
}
// 写三相表模块标识
HI_U32 mrsDut645_DIFEFF1E12(HI_VOID *param)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U32 ulRet = 0;
    MRS_DUT_645_FRAME_STRU *frame = (MRS_DUT_645_FRAME_STRU *)param;
	HI_U8 type = 0;
    do
    {
	if (frame->ucDataRealmLen != MRS_DUT_DI_LEN + sizeof(type))
        {
            ulRet = HI_ERR_BAD_FRAME_SIZE;
            HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("【ERR】DATA LEN  =%d"),sizeof(type)); 
            break;
        }
	type = frame->ucData[MRS_DUT_DI_LEN];
	if(type != 0 && type != 1)
	{
		 ulRet = HI_ERR_BAD_FRAME_SIZE;
            	break;
	}
        ret = mrsWriteTstaFlag(type);
        if (HI_ERR_SUCCESS != ret)
        {
            ulRet |= MRS_DUT_645_ASSERT_WRITE_NV_ERROR;
			break;
        }
    }while (0);

    (hi_void)memcpy_s(frame->ucData + MRS_DUT_DI_LEN, sizeof(HI_U32), &ulRet, sizeof(ulRet));
    frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
    frame->ucDataRealmLen = MRS_DUT_DI_LEN + sizeof(ulRet);
    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;

    doSendDutAck(frame);

    return ret;
}

HI_U32 mrsDut645_DI7EFF1E13(HI_VOID *param)
{
    HI_U32 ret = HI_ERR_FAILURE;
    MRS_DUT_645_FRAME_STRU * frame = (MRS_DUT_645_FRAME_STRU *)param;
	HI_U8 rltCode = 0;
	HI_U8 type = 0;
    do
    {
        ret = DutReadParse(frame);
        if (HI_ERR_SUCCESS != ret)
        {
            rltCode= 1;
            break;
        }
        ret = mrsGetTstaFlag(&type);
        if (HI_ERR_SUCCESS != ret)
        {
            rltCode= 3;
            break;
        }
         
    } while (0);

	frame->ucData[MRS_DUT_DI_LEN] = type;
	frame->ucData[MRS_DUT_DI_LEN+1] = rltCode;

    frame->stCtrl = MRS_DUT_645_FRAME_CTRL_UP;
    frame->ucDataRealmLen = MRS_DUT_DI_LEN + 2;
    frame->ucData[frame->ucDataRealmLen + 1] = EQUIP_DUT_645_FRAME_END_CODE;

    doSendDutAck(frame);

    return ret;
}
#endif

#if defined (PRODUCT_CFG_PRODUCT_TYPE_CCO)

HI_PRV HI_U8 g_zero_cross_info = 0;

HI_VOID mrsDut3762Reply_AFNF002(HI_U32 result, EQUIP_ZERO_CROSS_REQ_ACK_STRU *pstResult)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    PROTO376_2_FRAME_PRE_STRU frame;
    EQUIP_ZERO_CROSS_REQ_ACK_STRU ack;
    HI_PBYTE buf = HI_NULL;
    HI_U16 bufLen = 0;
    
    (hi_void)memset_s(&ack, sizeof(ack), 0, sizeof(ack));
    (hi_void)memset_s(&frame, sizeof(frame), 0, sizeof(frame));

    if (pstResult)
    {
        frame.stAfnData.pData = (HI_U8*)pstResult;
    }
    else
    {
        ack.ret = result;
        frame.stAfnData.pData = (HI_U8*)&ack;

        if (result == HI_ERR_TIMEOUT) {
            HI_MDM_CollectNtb(HI_FALSE, 0, 0, 0, HI_NULL);
        }
    }

    frame.stAfnData.ucAFN = MRS_AFN(0xF0);
    frame.stAfnData.ucDT[0] = MRS_AFN_FN_LO(2);
    frame.stAfnData.ucDT[1] = MRS_AFN_FN_HI(2);
    frame.stAfnData.usDataLen = sizeof(EQUIP_ZERO_CROSS_REQ_ACK_STRU);

    ret = mrsCreate3762Frame(&frame, &buf, &bufLen, HI_NULL);
    if (HI_ERR_SUCCESS == ret)
    {
        ret = MRS_SendMrData(buf, bufLen, HI_DMS_CHL_UART_PORT_APP);
        if(ret == HI_ERR_SUCCESS)
        {
            mrsDfxXrUartTx();
            mrsDfxRmUartTx(buf, bufLen, HI_FALSE);
        }
    }

    mrsToolsFree(buf);
}

#endif

HI_VOID mrsZeroCrossCheckProc(HI_U32 result, EQUIP_ZERO_CROSS_REQ_ACK_STRU *pstResult)
{
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    HI_UNREF_PARAM(result);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    mrsDut645Reply_DI7EFF1207(result, pstResult);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    mrsDut3762Reply_AFNF002(result, pstResult);
#endif

    mrsToolsFree(pstResult);
}

HI_VOID mrsZeroCrossCheckNotify(HI_U32 ulRet, HI_U8 ucLevel, HI_U32 ntb[EQUIP_ZERO_NTB_NUM])
{
    HI_SYS_QUEUE_MSG_S stMsg = {0};
    EQUIP_ZERO_CROSS_REQ_ACK_STRU* pstResult = mrsToolsMalloc(sizeof(EQUIP_ZERO_CROSS_REQ_ACK_STRU));

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    MRS_StopTimer(MRS_STA_TIMER_ZERO_CROSS);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    MRS_StopTimer(MRS_CCO_TIMER_ZERO_CROSS);
#endif    

    (hi_void)memset_s(&stMsg, sizeof(stMsg), 0, sizeof(stMsg));
    
    if (pstResult)
    {
        (hi_void)memset_s(pstResult, sizeof(EQUIP_ZERO_CROSS_REQ_ACK_STRU), 0, sizeof(EQUIP_ZERO_CROSS_REQ_ACK_STRU));
        pstResult->ret = ulRet;
        pstResult->level = ucLevel;
		pstResult->preciseFlag = HI_MDM_IsPreciseZeroCross();
        pstResult->phase = mrs_zero_cross_get_phase();
        pstResult->edge = mrs_zero_cross_get_edge();
        (hi_void)memcpy_s(pstResult->ntb, sizeof(pstResult->ntb), ntb, EQUIP_ZERO_NTB_NUM * sizeof(HI_U32));   
    }
    
    stMsg.ulMsgId = EN_MRS_FW_MSG_ZERO_CROSS_NOTIFY;
    stMsg.ulParam[0] = (HI_U32) pstResult;
    (HI_VOID)mrsSendMessage2Queue(&stMsg);
}


//从efuse读取
HI_U32 mrsEfuseReadID(HI_U8 efuse_data[EFUSE_AREA_SIZE])
{		
	if(HI_ERR_SUCCESS != HI_MDM_EFUSE_Read(HI_EFUSE_FREE_RW_IDX_2,efuse_data))
	{
		return HI_ERR_FAILURE;
	}
	if(HI_ERR_SUCCESS != HI_MDM_EFUSE_Read(HI_EFUSE_FREE_RW_IDX_3,&efuse_data[EFUSE_AREA_SIZE]))
	{
		return HI_ERR_FAILURE;
	}

	return HI_ERR_SUCCESS;
}

//读取ID处理函数
HI_U32 mrsReadIDProc(EQUIP_CHIP_ID_R_STRU* idInfo)
{
	HI_U32 ulRet = HI_ERR_SUCCESS;
	HI_U8 efuse_data[EFUSE_AREA_SIZE *2] = {0};
	
	do
	{
		idInfo->flag = EQUIP_READ_CHIP_ID_FROM_EFUSE;
		//从efuse读取
		//efuse读取失败不从NV读取,以免efuse有合规的ID,NV中没有合规的ID,导致PC重新写入到NV
		ulRet = mrsEfuseReadID(efuse_data);
		if(HI_ERR_SUCCESS != ulRet)
		{
			idInfo->ret = HI_ERR_READ_CHIP_ID_EFUSE_FAIL;
			break;
		}

		if(mrsToolsMemIsZero(efuse_data, MANAGEMENT_ID_SIZE))
		{
			//efuse中没有再从nv中读取
			ulRet = HI_MDM_GetMyManagementId(efuse_data);
			idInfo->flag = EQUIP_READ_CHIP_ID_FROM_NV;
			if(HI_ERR_SUCCESS != ulRet)
			{
				idInfo->ret = HI_ERR_READ_CHIP_ID_NV_FAIL;
				break;
			}
		}
	    (hi_void)memcpy_s(idInfo->chipId, MANAGEMENT_ID_SIZE, efuse_data, MANAGEMENT_ID_SIZE);
	}while(HI_FALSE);	

	return ulRet;
}

HI_U32 mrsReadMac(HI_U8 *mac)
{
	HI_U32 ulRet = HI_ERR_FAILURE;
	HI_NV_FTM_PRODUCT_ID_S stNvCfg;
    (hi_void)memset_s(&stNvCfg,sizeof( HI_NV_FTM_PRODUCT_ID_S),0,sizeof( HI_NV_FTM_PRODUCT_ID_S));
	
	do
	{
		if(HI_NULL == mac)
		{
			break;
		}
		
		ulRet = HI_MDM_NV_Read(HI_NV_FTM_PRODUCT_ID, &stNvCfg, sizeof(HI_NV_FTM_PRODUCT_ID_S));
		if(HI_ERR_SUCCESS != ulRet)
		{
			break;
		}
	    (hi_void)memcpy_s(mac, HI_PLC_MAC_ADDR_LEN, stNvCfg.ucPlcMac, HI_PLC_MAC_ADDR_LEN);		
		ulRet = HI_ERR_SUCCESS;
	}while(0);
	
	return ulRet;
}

//供获取设备各类ID的功能函数调用，获取芯片ID
HI_U32 mrsGetDevIdChip(HI_PBYTE pid_info, HI_U16 length)
{
	HI_U32 ret = HI_ERR_SUCCESS;
	HI_U8 runtime_management_id[MANAGEMENT_ID_SIZE] = {0};

    /* 芯片ID的字节序获取到的字节需就是HPLC要求的字节序，所以报文中无需再逆序。 */

    ret = HI_MDM_GetRunTimeManagementId(runtime_management_id);
    if (ret == HI_ERR_SUCCESS) {
        (hi_void)memcpy_s(pid_info, length, runtime_management_id, sizeof(runtime_management_id));
    }

    return ret;
}

//供获取设备各类ID的功能函数调用，获取设备ID
HI_U32 mrsGetDevIdDev(HI_PBYTE pid_info, HI_U16 length)
{
	HI_U32 ret = HI_ERR_SUCCESS;
	HI_NV_FTM_MANAGEMENT_ID_S nv;

    /* 注意: 这里模块ID处理仅为样例，非正式交付功能，需要从存储的相应位置读取。*/
	/*       本接口做了逆序处理，存NV需按正序存储。 */

    (hi_void)memset_s(&nv, sizeof(nv), 0, sizeof(nv));
	ret = HI_NVM_Read(HI_NV_FTM_MANAGEMENT_ID, &nv, sizeof(nv));
	if(ret != HI_ERR_SUCCESS)
	{
		return ret;
	}

    ret = (hi_u32)memcpy_s(pid_info, length, nv.dev_id, MRS_DEV_ID_SIZE);
	if(ret != HI_ERR_SUCCESS)
	{
		return ret;
	}
    mrsHexInvert(pid_info, MRS_DEV_ID_SIZE);
	HI_DIAG_LOG_BUF(0, HI_DIAG_MT("[mrsGetDevIdDev] dev id info"), pid_info, MRS_DEV_ID_SIZE);

	return ret;
}

hi_void mrs_zero_cross_set_info(hi_u8 info)
{
    /*
     * [1:0] phase: 00-Phase A, 01-Phase B, 02-Phase C
     * [2:2] edge: 0-Rising, 1-Falling
     */
    if ((info & 0x3) > 2) {
        return;
    }

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    {
        EQUIP_DUT_MAIN_INFO_STRU * dut = do_GetDutInfo();
        dut->zero_cross_phase = (info & 0x3);
        dut->zero_cross_edge = (info >> 2) & 0x1;
    }
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    g_zero_cross_info = info;
#endif
}

hi_u8 mrs_zero_cross_get_phase(hi_void)
{
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    {
        EQUIP_DUT_MAIN_INFO_STRU * dut = do_GetDutInfo();
        return dut->zero_cross_phase;
    }
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    return (g_zero_cross_info & 0x3);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    return 0;
#endif
}

hi_u8 mrs_zero_cross_get_edge(hi_void)
{
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    {
        EQUIP_DUT_MAIN_INFO_STRU * dut = do_GetDutInfo();
        return dut->zero_cross_edge;
    }
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    return ((g_zero_cross_info >> 2) & 0x1);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    return 0;
#endif
}
HI_U32 mrsWriteModuleId(HI_U8* moduleId)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_NV_FTM_MANAGEMENT_ID_S nv;

    (hi_void)memset_s(&nv, sizeof(nv), 0,sizeof(nv));
    ret = HI_MDM_NV_Read(HI_NV_FTM_MANAGEMENT_ID, &nv, sizeof(nv));
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }
	(hi_void)memcpy_s(nv.dev_id, sizeof(nv.dev_id),moduleId,MRS_DEV_ID_SIZE);
	//mrsHexInvert(nv.dev_id, MRS_DEV_ID_SIZE);
    ret = HI_MDM_NV_Write(HI_NV_FTM_MANAGEMENT_ID, &nv, sizeof(nv));

    return ret;
}
HI_U32 mrsGetModule(HI_U8* moduleId)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_NV_FTM_MANAGEMENT_ID_S nv;

    (hi_void)memset_s(&nv, sizeof(nv), 0,sizeof(nv));
    ret = HI_MDM_NV_Read(HI_NV_FTM_MANAGEMENT_ID, &nv, sizeof(nv));
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }
	(hi_void)memcpy_s(moduleId, MRS_DEV_ID_SIZE,nv.dev_id,MRS_DEV_ID_SIZE);
	//mrsHexInvert(moduleId, MRS_DEV_ID_SIZE);
    return ret;
}
