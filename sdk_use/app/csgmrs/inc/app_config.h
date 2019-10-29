//*****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : app_config.h
//  版 本 号   : V1.0
//  作    者   : 沈汉坤/00130424
//  生成日期   : 2011年09月06日
//  功能描述   : APP全局配置定义文件
//  函数列表   :
//  修改历史   :
//  1.日    期 : 2011年09月06日
//    作    者 : 沈汉坤/00130424
//    修改内容 : 创建文件
//
//*****************************************************************************

#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

//*****************************************************************************
// APP 模块 版本配置 [注意] 版本号数字不能加括号，使用时是#x"."#y"."#z的形式
//*****************************************************************************
#define VERSION_MAIN        1 //主线版本
#define VERSION_FEATURE     3 //特性版本
#define VERSION_BUG         7 //重大BUG修订，偶数为稳定版本，奇数为开发版本

//////////////////////////////////////////////////////////////////////////
// APP 模块 ID 配置
//////////////////////////////////////////////////////////////////////////

#define HI_MOD_ID_APP_MRS_CHL    HI_MOD_ID_APP_PRO_DEC
#define HI_MOD_ID_APP_MRS_PROTO  HI_MOD_ID_APP_PRO_DEC
#define HI_MOD_ID_APP_MRS_CCO    HI_MOD_ID_APP_COMMON
#define HI_MOD_ID_APP_MRS_STA    HI_MOD_ID_APP_COMMON
#define HI_MOD_ID_APP_DFX        HI_MOD_ID_APP_COMMON
#define HI_MOD_ID_APP_MRS_MAP    HI_MOD_ID_APP_MAC_MAP
#define HI_MOD_ID_APP_MRS_RM     HI_MOD_ID_APP_LOCAL_MR
#define HI_MOD_ID_APP_MRS_UPG    HI_MOD_ID_APP_UPG


//////////////////////////////////////////////////////////////////////////
// MRS 配置
//////////////////////////////////////////////////////////////////////////
#define PRODUCT_CFG_MRS_MAX_TOPO_NUM                 (2048)          // 最大从节点数量

#define PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM         (32)            // 最大单一MAC对应的电表地址个数
#define PRODUCT_CFG_MRS_MAX_MUSTER_COMM_RENUM        (3)             // 集中器通讯最大重试次数
#define PRODUCT_CFG_STA_INIT_TIME                    (2000)          // 初始化等待时间500毫秒
#define PRODUCT_CFG_STA_HEART_TIME                   (1000 * 60 * 5) // 心跳周期为5分钟
#define PRODUCT_CFG_MRS_3762_BUFFER_BYTE_MAX         (2048)

#define PRODUCT_CFG_STA_FRAME_BUF_ITEM_COUNT_MAX     (20)           // STA缓存frame的数量(最大)
#define PRODUCT_CFG_STA_FRAME_BUF_ITEM_COUNT_DEFAULT (10)           // STA缓存frame的数量(默认)
#define PRODUCT_CFG_STA_FRAME_BUF_ITEM_COUNT_MIN     (1)            // STA缓存frame的数量(最小)

#define PRODUCT_CFG_STA_FRAME_BUF_TIMEOUT_MAX        (255)          // STA缓存frame的有效期(最大) 单位:秒
#define PRODUCT_CFG_STA_FRAME_BUF_TIMEOUT_DEFAULT    (10)           // STA缓存frame的有效期(默认) 单位:秒
#define PRODUCT_CFG_STA_FRAME_BUF_TIMEOUT_MIN        (1)            // STA缓存frame的有效期(最小) 单位:秒

#define PRODUCT_CFG_MRS_PLC_TIMEOUT_NUM_MIN          (2)             // 当前节点连续PLC超时次数(用于判断NV项)最小
#define PRODUCT_CFG_MRS_PLC_TIMEOUT_NUM_DEFAULT      (25)             // 当前节点连续PLC超时次数(用于跳转节点)默认

#define PRODUCT_CFG_MRS_TURN_NUM_DEFAULT             (100)             // 定抄读表轮次默认值
#define PRODECT_CFG_MRS_CCO_FREQ_ENABLE_DEFAULT      (1)    //CCO获取到时钟后启动频段定时的使能开关

// 645 Frame  receive buffer 静态内存
#define PRODUCT_CFG_MRS_645_BUFFER_BYTE_MAX             (512)
#define PRODUCT_CFG_MRS_PROTO3762_FRAME_BUFFER_MAX      (2048)
#define PRODUCT_CFG_MRS_PACKET_MAX_SIZE                 (2048)  // 2KB, 最大16KB字节

#define MRS_SRV_STA_BIND_DURATION                       (0xFFFF)
#define MRS_SRV_SOFT_TF_SWITCH_CLOSE                    (0)
#define MRS_SRV_SOFT_TF_SWITCH_OPEN_CLOSE_WHITE         (1)
#define MRS_SRV_SOFT_TF_SWITCH_OPEN_NOT_CARE_WHITE      (2)

//*****************************************************************************
//*****************************************************************************

#define MRS_STA_FRAME645_BUFSIZE_MAX    (256)
#define MRS_STA_FRAME_BUF_ITEM_BUFSIZE  (MRS_STA_FRAME645_BUFSIZE_MAX)

//*****************************************************************************
//*****************************************************************************

//////////////////////////////////////////////////////////////////////////
// MRS 配置
//////////////////////////////////////////////////////////////////////////
//#define PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT
//#define PRODUCT_CFG_PRODUCT_TYPE_CSG_GD

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT) && defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))
#error "PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT/PRODUCT_CFG_PRODUCT_TYPE_CSG_GD can't be defined both!"
#endif

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// UPG 配置
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// DFX 配置
//////////////////////////////////////////////////////////////////////////
#define PRODUCT_CFG_SUPPORT_OSM
#define PRODUCT_CFG_SUPPORT_OSSM

//////////////////////////////////////////////////////////////////////////
// EQU 配置
//////////////////////////////////////////////////////////////////////////



#endif // __APP_CONFIG_H__

