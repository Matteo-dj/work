//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : drv_dfx.c
//  版 本 号   : V1.0
//  作    者   :  chenghuanhuan/00132816
//  生成日期   : 2012-05-09
//  功能描述   : 可维可测相关应用
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2012-05-09
//    作    者 : chenghuanhuan/00132816
//    修改内容 : 创建文件
//               2012-06-19 upload_data 增加噪声采集
//
//*****************************************************************************
#ifndef __DRV_COLLECT_DATA_H__
#define __DRV_COLLECT_DATA_H__
#include <hi_types.h>
#include <hi_mdm.h>
#include <hi_mdm_datacollect.h>
#define DRV_DC_UN_FIX_AGCGAIN ((HI_S32)0xFFFFFFFF)

typedef enum
{
    HI_DRV_DATA_COLLECT_STEP_PREPARE        =0x1,
    HI_DRV_DATA_COLLECT_STEP_START          =0x2,
    HI_DRV_DATA_COLLECT_STEP_GET_RESULT     =0x4,

    //下面特殊处理
    HI_DRV_DATA_COLLECT_STEP_FORCE_RESTORE  =0x8,
}HI_DRV_DATA_COLLECT_STEP_E;

// Define the struct C_fix_aagc_gain
struct C_fix_aagc_gain
{
    unsigned int fix_aagc_gain : 11;
    unsigned int fix_aagc_gain_v200 : 7;
    unsigned int Reserved_0 : 14;
};
#define HI_DRV_DATA_COLLECT_STEP_ALL            (HI_DRV_DATA_COLLECT_STEP_PREPARE|HI_DRV_DATA_COLLECT_STEP_START|HI_DRV_DATA_COLLECT_STEP_GET_RESULT)
#define HI_DRV_DATA_COLLECT_OVERFLOW            (1 << 1)
#define HI_DRV_DATA_COLLECT_ADDR_CYCLE_IND      (1 << 17)
#define HI_DRV_DATA_COLLECT_TEST_MODE_OFFSET    (2)
#define HI_DRV_DATA_COLLECT_TRIG_TEST_MODE_BASE (255)
#if defined (PRODUCT_CFG_COLLECT_DATA)
HI_U32 DC_Check_TrigerType(DC_TRIG_TYPE_ENUM enTrigerType);
HI_U32 DC_Check_DataSrc(DC_TRIG_DATA_ENUM  enDataSrc);
HI_U32 DC_Check_AgcGain(HI_S32 lAgcGain);
HI_U32 DC_Check_FixAgcGain(HI_S32 lAgcGain);
HI_U32 DC_Check_Addr_And_Len(HI_U32 ulStartAddr,HI_U32 ulEndAddr,HI_U32 ulAfterPointlen);
HI_U32 DC_Data_Collect(HI_IN DC_INPUT_PARA_STRU *pDcInput,HI_OUT HI_BUF_S *sBuf,HI_DRV_DATA_COLLECT_STEP_E step);
HI_VOID DC_Data_CollectForceRestore(HI_VOID);//恢复
#else
HI_PRVL HI_U32 DC_Check_TrigerType(DC_TRIG_TYPE_ENUM enTrigerType){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 DC_Check_DataSrc(DC_TRIG_DATA_ENUM  enDataSrc){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 DC_Check_AgcGain(HI_S32 lAgcGain){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 DC_Check_FixAgcGain(HI_S32 lAgcGain){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 DC_Check_Addr_And_Len(HI_U32 ulStartAddr,HI_U32 ulEndAddr,HI_U32 ulAfterPointlen){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 DC_Data_Collect(HI_IN DC_INPUT_PARA_STRU *pDcInput,HI_OUT HI_BUF_S *sBuf,HI_DRV_DATA_COLLECT_STEP_E step){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_VOID DC_Data_CollectForceRestore(HI_VOID){return;}
#endif
#endif//__DRV_COLLECT_DATA_H__