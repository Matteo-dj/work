//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_capture.h
//  版 本 号   : V1.0
//  作    者   : fengxiaomin/f00209182
//  生成日期   : 2016-09-26
//  功能描述   : TODO: ...
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2016-09-26
//    作    者 : fengxiaomin/f00209182
//    修改内容 : 创建文件
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   :
// SUBSYSTEM :
// MODULE    :
// OWNER     :
//*****************************************************************************

#ifndef __MRS_SRV_CAPTURE_H__
#define __MRS_SRV_CAPTURE_H__
HI_START_HEADER

typedef struct
{
    HI_U32 ver                  : 6;        // 协议版本号
    HI_U32 mac_head_len         : 6;        // 报文头长度
    HI_U32 test_mode            : 4;        // 测试模式
    HI_U32 ruel_mode            : 4;        // 规约类型
    HI_U32 test_para            : 12;       // 测试参数，转发数据长度或者模式持续时间
}mrs_app_test_fream_st;

#define HI_FC_SIZE                              16
#define HI_LONG_PB_SIZE                         520
#define MRS_PLC_APP_HEAD_LEN                    4

#define MRS_HEAD_PORT_OFFSET                    0       // app报文头中的端口号偏移
#define MRS_HEAD_PKT_ID_OFFSET                  1       // app报文头中的报文ID偏移
#define MRS_PBH_SIZE_PSG                        1       // 国网协议PB头大小
#define MRS_HEAD_MSDU_TYPE_OFFSET_PSG           7
#define MRS_MSDU_TYPE_APP_PSG                   48      // 应用层报文
#define MRS_SHORT_HEAD_SIZE_PSG                 16      // 国网协议短帧头长度
#define MRS_LONG_HEAD_SIZE_PSG                  28      // 国网协议长帧头长度

#define MRS_PKT_ID_PLC_TEST                     0x0006  // 通信测试报文ID
#define MRS_APP_PORT_NUM                        0x11    // 抄表业务的端口号
#define MRS_STD_PHYSICAL_TEST_MODE_LISTEN       (35)    // 可解析测试报文的时间，单位s


#define MRS_PLC_FRAME_COM_OFFSET  (0)
#define MRS_PLC_FRAME_ID_OFFSET   (1)
#define MRS_PLC_RESP_FLAG_OFFSET  (5)
#define MRS_PLC_WORK_FLAG_OFFSET  (6)
#define MRS_PLC_FRAME_SEQ_OFFSET  (8)
#define MRS_PLC_TEST_ID_OFFSET    (12)
#define MRS_PLC_TEST_LEN_OFFSET   (14)
#define MRS_PLC_TEST_FREQ_OFFSET  (16)

#define MRS_PLC_TEST_RESP_FLAG    (0x20)
#define MRS_PLC_FRAME_COM_VALUSE  (0x11)
#define MRS_PLC_FRAME_ID_VALUSE   (0x0101)

#define MAC_TYPE_LL_PK            (0)         // MAC HEAD(32byte) 和 ETH HEAD(18byte)为标准协议结构
#define MAC_TYPE_SS_PK            (1)         // MAC HEAD(12byte) 和 ETH HEAD(2byte)

#define MAC_HEAD_LONG_SIZE        (32)
#define MAC_HEAD_SHORT_SIZE       (12)

#define MSDU_HEAD_LONG_SIZE       (18)
#define MSDU_HEAD_SHORT_SIZE      (2)

#define MAC_HEAD_TYPE_MASK        (0x01)
#define MSDU_HEAD_APP_TYPE        (0x01)
#define MAC_HEAD_MSDU_LEN_OFFSET  (2)


HI_VOID mrsSrvCaptureInit(HI_VOID);
HI_U32 mrsSrvCaptureDeinit(HI_VOID);
HI_U32 mrsSrvParseAppData(HI_U8 *pstPbData, HI_U8 **ppstData, HI_U16 *usLen);

HI_BOOL mrs_get_test_mode_enable(HI_VOID);
HI_VOID mrs_set_test_mode_enable(HI_BOOL enable);

HI_END_HEADER
#endif


