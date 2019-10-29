//****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//****************************************************************************
//  文 件 名   : hi_mac.h
//  版 本 号   : V1.0
//  作    者   : guopan/00178999
//  生成日期   : 2012年8月30日
//  功能描述   :提供MAC协议栈对外封装的接口
//  函数列表   :
//  修改历史   :
//    修改内容 :
//
//****************************************************************************
#ifndef _HI_MAC_
#define _HI_MAC_

#include "hi_mdm_types.h"
#include "hi_mdm.h"

#ifdef __cplusplus
extern "C" {
#endif

// BSP通知NDM的采数跟随命令结构体
typedef struct
{
    HI_U8 ccomac[HI_PLC_MAC_ADDR_LEN];    //跟随的CCO的MAC地址
    HI_U8 freq;                           //跟随网络的freq
    HI_U8 snid;                           //跟随的SNID

    HI_U32 mode;                          //启动跟随模式，固定跟随，切换频段跟随
    HI_U32 cmd;                           //启动跟随，停止跟随的命令
}DIAG_NDM_SET_FOLLOW_REQ_STRU;

//启动，停止跟随命令
typedef enum
{
    NOTIFY_START_NDM_FOLLOW = 1,     //通知启动采数跟随
    NOTIFY_STOP_NDM_FOLLOW,          //通知停止采数跟随
}NOTIFY_SET_NDM_FOLLOW_CMD_E;

//跟随模式
typedef enum
{
    NDM_FIXED_FREQ_FOLLOW = 1,    //固定频段跟随
    NDM_DYNAMIC_FREQ_FOLLOW,      //动态切换频段跟随
}NDM_FOLLOW_MODE_E;

//跟随状态
typedef enum
{
    DFX_NDM_FOLLOWING_STATUS = 0, //跟随中
    DFX_NDM_FOLLOWED_STATUS,      //跟随上
    DFX_NDM_FOLLOW_FAIL_STATUS,   //跟随失败
}DFX_NDM_FOLLOW_STATUS_ENUM;

//发送机模块通知BSP采数时隙回调函数的指针定义
typedef HI_U32 (*TX_NOTIFY_BSP_SLOT_FUNC)(HI_U32 slot_duration);
HI_VOID MAC_begin_upgrade_state(HI_VOID);
HI_VOID MAC_end_upgrade_state(HI_VOID);
HI_U8 MAC_get_phase_by_mac(HI_U8 mac[6]);
HI_BOOL MAC_get_my_proxy_mac(HI_U8 *proxy_mac);
HI_U32 HI_MAC_QueryNetParam(HI_OUT HI_MAC_NET_PARAM_QRY_S* pstNetParam);
HI_VOID MAC_SetValidMacList(HI_MAC_VALID_MAC_LIST_STRU* pList, HI_MAC_WHIST_LIST_CHANGE_REASON_ENUM reason);
HI_VOID MAC_AddValidMacList(HI_MAC_VALID_MAC_LIST_STRU* pList, HI_MAC_WHIST_LIST_CHANGE_REASON_ENUM reason);
HI_VOID MAC_DelValidMacList(HI_MAC_VALID_MAC_LIST_STRU* pList, HI_MAC_WHIST_LIST_CHANGE_REASON_ENUM reason);
HI_U32 MAC_GetMacAddr(HI_OUT HI_U8 *mac);
HI_U32 HI_MAC_SetTdmaPeriod(HI_U32 percent,HI_U8 lid,HI_U16 msdu_size);
HI_U32 HI_MAC_SetBindCsmaPeriod(HI_U32 percent,HI_U8 lid,HI_U16 msdu_size);
HI_U32 HI_MAC_GetRemainPeriodPercent (HI_VOID);

HI_U32 HI_MAC_get_current_bpc(HI_VOID);
HI_U32 HI_MAC_QueryMacAttr(HI_IN HI_U8 *pMac, HI_MAC_NETWORK_TOPO_ENTRY_S *pstTopoEntry);
HI_VOID HI_MAC_led_set_search_meter_status(HI_IN HI_U8 status);
HI_VOID HI_MAC_led_set_read_meter_addr_status(HI_IN HI_U8 status);
HI_U32  HI_MAC_led_set_485_led(HI_IN HI_U8 on_time);
HI_U32 HI_MDM_MacGetPktCntInTxQueue(HI_U8 Priority);

HI_U32 HI_MDM_GetCurrentBpc(HI_VOID);

//抄控器巡检采数提供给BSP的接口
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
HI_U32 HI_MAC_ndm_handle_set_follow_cmd(HI_IN DIAG_NDM_SET_FOLLOW_REQ_STRU *pset_cmd);
HI_U32 HI_MAC_ndm_get_cur_follow_status(HI_VOID);
HI_U32 HI_MAC_GetRxPbStatisticByFollow(HI_VOID);
HI_U32 HI_MAC_tx_reg_notify_bsp_slot(TX_NOTIFY_BSP_SLOT_FUNC func);
HI_U32 HI_MAC_tx_cancel_reg_notify_bsp_slot(HI_VOID);
HI_BOOL HI_MAC_check_follow_mac_is_valid(HI_IN HI_U8 mac[HI_PLC_MAC_ADDR_LEN]);
HI_BOOL HI_MAC_check_follow_freq_is_valid(HI_IN HI_U8 freq);
HI_BOOL HI_MAC_check_follow_snid_is_valid(HI_IN HI_U8 snid);
typedef enum
{
    HI_ZERO_CROSS_ISR_TYPE_FALL,
    HI_ZERO_CROSS_ISR_TYPE_RISE,
    HI_ZERO_CROSS_ISR_TYPE_UNKNOWN,
}HI_ZERO_CROSS_ISR_TYPE_E;

typedef HI_VOID (*zero_cross_isr_notify)(HI_ZERO_CROSS_ISR_TYPE_E isr_type);

#define HI_ZERO_CROSS_COLLECT_LEVEL_1M                                   1            // 1M采集档位
#define HI_ZERO_CROSS_COLLECT_LEVEL_240K                                 0            // 240K采集档位

#ifndef PRODUCT_CFG_FEATURE_ST_PHY
//使能NDM过零NTB并设置档位
HI_EXTERN HI_VOID HI_MAC_EnableNdmZeroCrossDetect(HI_U8 RcvLevel);
//去使能NDM过零NTB
HI_EXTERN HI_VOID HI_MAC_DisableNdmZeroCrossDetect(HI_VOID);
//NDM过零点采数函数注册
HI_EXTERN HI_U32 HI_MAC_RegistZeroCrossNotify(zero_cross_isr_notify pNotify);
#else
//使能NDM过零NTB并设置档位
HI_PRVL HI_VOID HI_MAC_EnableNdmZeroCrossDetect(HI_U8 RcvLevel){return;}
//去使能NDM过零NTB
HI_PRVL HI_VOID HI_MAC_DisableNdmZeroCrossDetect(HI_VOID){return;}
//NDM过零点采数函数注册
HI_PRVL HI_U32 HI_MAC_RegistZeroCrossNotify(zero_cross_isr_notify pNotify){return HI_ERR_NOT_SUPPORT;}
#endif
#endif

typedef struct
{
    HI_U8  dt;                  // 帧类型
    HI_U8  freq;                // 频段
    HI_U16 stei;                // 发送FC的站点TEI地址

    HI_U16 dtei;                // 目的设备地址
    HI_S8  snr;                 // 平均信噪比
    HI_U8  channel_info;        // 信道质量

    HI_U32 nid;                 // 网络号

    HI_U16 preamle_sat_num;     // 前导符号饱和率
    HI_U16 ptcl_ver;            // 协议号

    HI_U8  attenuation;         // 信道衰减
    HI_U8  snr_class;           // SNR分级
    HI_U8  phase;               // 相位，仅信标FC有效
    HI_U8  pad;
}HI_MDM_RCV_FC_INFO_ST;
typedef HI_VOID (*MAC_MONITOR_RCV_FC_INFO_FUN)(HI_IN HI_MDM_RCV_FC_INFO_ST *RcvFcInfo);
HI_U32 HI_MDM_RegisterMonitorRcvFcInfoInterface(HI_IN MAC_MONITOR_RCV_FC_INFO_FUN MonitorRcvFcInfoInterface);

typedef struct
{
    HI_U8  dt;                  // 帧类型
    HI_U8  freq;                // 频段
    HI_U16 ptcl_ver;            // 协议号

    HI_U32 nid;                 // 网络号

    HI_U16 stei;                // 发送PB块的源站点TEI
    HI_U16 pad;
}HI_MDM_RCV_PB_INFO_ST;
typedef HI_VOID (*MAC_RCV_PB_INFO_FUN)(HI_IN HI_MDM_RCV_PB_INFO_ST *RcvPbInfo);
HI_U32 HI_MDM_RegisterRcvPbInfoInterface(HI_IN MAC_RCV_PB_INFO_FUN RcvPbInfoInterface);

typedef HI_VOID (*MAC_BURST_RCV_END_FUN)(HI_VOID);
HI_U32 HI_MDM_RegisterBurstRcvEndInterface(HI_IN MAC_BURST_RCV_END_FUN MacBurstRcvEndInterface);

typedef struct
{
    HI_U16 stei;
    HI_U16 dtei;
    HI_U8  channel_info;
    HI_U8  attenuation;
}HI_MDM_SACK_INFO_ST;
typedef HI_VOID (*MAC_RCV_SACK_INFO_FUN)(HI_IN HI_MDM_SACK_INFO_ST *SackInfo);

HI_U32 HI_MDM_RegisterRcvSackInfoInterface(HI_IN MAC_RCV_SACK_INFO_FUN MacRcvSackInfoInterface);

typedef struct
{
    HI_U32 isp_counter;
    HI_U32 zero_cross_ntb;
}HI_MDM_RX_INTERRUPT_INFO_ST;
typedef HI_VOID (*MAC_HANDLE_RX_INTERRUPT_FUN)(HI_MDM_RX_INTERRUPT_INFO_ST *RxInterruptInfo);
HI_U32 HI_MDM_RegisterRxInterruptInterface(HI_U32 interrupt_type,MAC_HANDLE_RX_INTERRUPT_FUN MacRxInterruptInterface);

typedef struct
{
    HI_U32 nid;
    HI_U8  freq;
    HI_U8  synch_mac[HI_PLC_MAC_ADDR_LEN];
    HI_U8  pad[3];
    HI_U16 stei;

    HI_U32 ltmr_low;
    HI_U32 ltmr_high;

    HI_U32 bts;
}HI_MDM_NTB_INFO_ST;
typedef HI_VOID (*MAC_RCV_NTB_INFO_FUN)(HI_MDM_NTB_INFO_ST *NtbInfo);
HI_VOID HI_MDM_RegisterRcvNtbInfoInterface(MAC_RCV_NTB_INFO_FUN MacRcvNtbInfoInterface);

typedef struct
{
    HI_U16 protocol_ver;
    HI_U16 size;

    HI_U8  *capture_data;
}HI_MDM_CAPTURE_FRAME_INFO_ST;

typedef HI_VOID(*MAC_CAPTURE_FUN)(HI_MDM_CAPTURE_FRAME_INFO_ST *CaptureFrameInfo);
HI_U32 HI_MDM_RegisterCaptureFrameInterface(MAC_CAPTURE_FUN CaptureFc,MAC_CAPTURE_FUN CapturePb);
HI_VOID HI_MDM_UnregisterCaptureFrameInterface(MAC_CAPTURE_FUN CaptureFc,MAC_CAPTURE_FUN CapturePb);

typedef HI_U32 (*MAC_HANDLE_BEACON_FUN)(HI_U8 *buf);
HI_U32 HI_MDM_RegisterHandleBeaconFun(MAC_HANDLE_BEACON_FUN HandleBeaconInterface);

typedef HI_U32 (*MAC_HANDLE_COD_FUN)(HI_U8 *rts_frame, HI_U8 freq);
HI_U32 HI_MDM_RX_RegisterHandleCodFrame(MAC_HANDLE_COD_FUN HandleCodInterface);

typedef HI_VOID(*MAC_MONITOR_PKT_STEI_FUN)(HI_U16 stei,HI_U16 ostei);
HI_U32 HI_MDM_RegisterMonitorPktStei(MAC_MONITOR_PKT_STEI_FUN MonitorPktSteiInterface);

typedef struct
{
    HI_U8  freq;                                // 接收频段
    HI_S8  snr;                                 // 平均信噪比
    HI_U8  channel_info;                        // 信道质量
    HI_U8  attenuation;                         // 信道衰减

    HI_U32  ltmr_low;               // 接收FC时的本地时钟值
    HI_U32  ltmr_high;              // 接收FC时的本地时钟值
}rcv_channel_info_st;

typedef HI_VOID (*MAC_HANDLE_SACK_EXTEND_FC_FUN)(HI_U8 *fc_addr,rcv_channel_info_st *rcv_channel_info);

HI_U32 HI_MDM_RegisterSackExtendFrameInterfaceV100(HI_U8 extend_frame_type,
                                            MAC_HANDLE_SACK_EXTEND_FC_FUN handle_sack_extend_fc_interface);
HI_U32 HI_MDM_RegisterSackExtendFrameInterfaceSg(HI_U8 extend_frame_type,
                                            MAC_HANDLE_SACK_EXTEND_FC_FUN handle_sack_extend_fc_interface);

HI_VOID HI_MDM_SetNotFilterSofBySnid(HI_BOOL NotFilterSofBySnid);

#ifdef __cplusplus
}
#endif

#endif
