/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : phy_stub_interface.h
  版 本 号   : 初稿
  作    者   : likunhe/00194117
  生成日期   : 2011年11月29日
  最近修改   :
  功能描述   : 驱动层的测试桩接口
  函数列表   :
  修改历史   :
  1.日    期   : 2011年11月29日
    作    者   : likunhe/00194117
    修改内容   : 创建文件

******************************************************************************/
#ifndef DRIVER_STUB_INTERFACE_H
#define DRIVER_STUB_INTERFACE_H

/************************************************************
                     包含其它模块的头文件
************************************************************/
#include "sal_inf.h"
#include "phy_driver_interface.h"

#ifdef __cplusplus
    extern "C"{
#endif

/************************************************************
                               宏定义
************************************************************/
//GTR下发的消息ID
#define STUB_REQ_BASE_ID             0x0000
#define STUB_BEACON_SG_ID            (STUB_REQ_BASE_ID + 0x0)
#define STUB_SOF_SG_ID            (STUB_REQ_BASE_ID + 0x1)
#define STUB_SACK_SG_ID            (STUB_REQ_BASE_ID + 0x2)
#define STUB_COORDINATE_SG_ID            (STUB_REQ_BASE_ID + 0x3)
#define STUB_SET_TONE_MASK_TABLE_ID   (STUB_REQ_BASE_ID + 0x4)
#define STUB_SET_POWER_LOADING_TABLE_ID   (STUB_REQ_BASE_ID + 0x5)


#define STUB_FC_BEACON_ID           (STUB_REQ_BASE_ID + 0x50)  // Beacon的FC的消息
#define STUB_FC_SOF_ID              (STUB_REQ_BASE_ID + 0x51)  // Sof的FC的消息
#define STUB_FC_SOUND_ID            (STUB_REQ_BASE_ID + 0x52)  // 未用
#define STUB_FC_SACK_ID             (STUB_REQ_BASE_ID + 0x53)  // Sack的FC的消息
#define STUB_NTB_CONFIG_ID         (STUB_REQ_BASE_ID + 0x54)  // 本站点的短网标识配置
#define STUB_TEI_CONFIG_ID          (STUB_REQ_BASE_ID + 0x55)  // 本站点的设备标识配置
#define STUB_ROLE_CONFIG_ID         (STUB_REQ_BASE_ID + 0x56)  // 本站点的角色配置
#define STUB_PROXY_CONFIG_ID        (STUB_REQ_BASE_ID + 0x57)  // 本站点的父代理配置
#define STUB_TX_TONE_MAP_ID         (STUB_REQ_BASE_ID + 0x58)  // 发送时用的Tone Map
#define STUB_RX_TONE_MAP_ID         (STUB_REQ_BASE_ID + 0x59)  // 接收时用的Tone Map
#define STUB_CRC_SWITCH_ID          (STUB_REQ_BASE_ID + 0x5a)  // CRC结果上报的开关配置
#define STUB_BLER_REQUEST_ID        (STUB_REQ_BASE_ID + 0x5b)  // Bler上报的开关配置
#define STUB_PCS_SWITCH_ID          (STUB_REQ_BASE_ID + 0x5c)  // 载波侦听上报的开关配置
#define STUB_TX_DONE_SWITCH_ID      (STUB_REQ_BASE_ID + 0x5d)  // 发送结果上报的开关
#define STUB_CHANNEL_INFO_SWITCH_ID (STUB_REQ_BASE_ID + 0x5e)  // 信道质量上报的开关
#define STUB_FC_RTS_CTS_ID          (STUB_REQ_BASE_ID + 0x5f)  // RTS/CTS的FC的消息
/* BEGIN: Added by likunhe/00194117, 2012/2/24   PN:Build3 Require*/
#define STUB_STATISTIC_INFO_REQUEST_ID (STUB_REQ_BASE_ID + 0x60)  // Preamble和FC的消息查询
/* BEGIN: Added by likunhe/00194117, 2012/4/28   问题单号:DTS2012050801863*/
//#define STUB_REQ_MAX_ID             STUB_STATISTIC_INFO_REQUEST_ID
#define STUB_ISP_SEND_ID            (STUB_REQ_BASE_ID + 0x61)   //ISP发送使能
#define STUB_ISP_RCV_ID             (STUB_REQ_BASE_ID + 0x62)   //ISP接收使能
#define STUB_STATISTIC_INTERRUTP_INFO_REQUEST_ID    (STUB_REQ_BASE_ID + 0x63)   //查询异常中断
//#define STUB_REQ_MAX_ID             STUB_STATISTIC_INTERRUTP_INFO_REQUEST_ID
#define STUB_COORDINATE_ID         (STUB_REQ_BASE_ID + 0x64)
#define STUB_UNIVERSE_ID           (STUB_REQ_BASE_ID + 0x65)
#define STUB_RX_FILTER_ID          (STUB_REQ_BASE_ID + 0x66)
#define STUB_RD_REG_ID             (STUB_REQ_BASE_ID + 0x67)
#define STUB_WR_REG_ID             (STUB_REQ_BASE_ID + 0x68)
#define STUB_ACCESS_CONFIG_ID      (STUB_REQ_BASE_ID + 0x69)
#define STUB_CYC_SEND_ID           (STUB_REQ_BASE_ID + 0x6a)
#define STUB_RX_FILTER1_ID         (STUB_REQ_BASE_ID + 0x6b)
#define STUB_TX_PL_ID              (STUB_REQ_BASE_ID + 0x6c)
#define STUB_REQ_PL_ID             (STUB_REQ_BASE_ID + 0x6d)   //查询异常中断

/* BEGIN: Added by liujian 00280874, 2016/4/26   问题单号:MAC_PHY_TEST*/
#define STUB_RX_FREQERR_ID          (STUB_REQ_BASE_ID + 0x6e)
#define STUB_QUERY_RCV_INFO_ID      (STUB_REQ_BASE_ID + 0x6F)
#define STUB_SET_TEST_INFO_ID       (STUB_REQ_BASE_ID + 0x70)
#define STUB_DEFC_FORM_INFO_ID      (STUB_REQ_BASE_ID + 0x71)
#define STUB_BUFF_FILTER_INFO_ID    (STUB_REQ_BASE_ID + 0x72)
#define STUB_SEND_TIMESTAMP_ID      (STUB_REQ_BASE_ID + 0x73)
#define STUB_TX_FREQERR_ID          (STUB_REQ_BASE_ID + 0x74)
#define STUB_BEACON_V200_ID         (STUB_REQ_BASE_ID + 0x75)
#define STUB_SOF_V200_ID            (STUB_REQ_BASE_ID + 0x76)
#define STUB_SACK_V200_ID           (STUB_REQ_BASE_ID + 0x77)
#define STUB_COORDINATE_V200_ID     (STUB_REQ_BASE_ID + 0x78)
#define STUB_SNID_FREQERR_FORM_INFO_ID          (STUB_REQ_BASE_ID + 0x79)
#define STUB_MAC_PHY_TEST_INFO_REQUEST_ID       (STUB_REQ_BASE_ID + 0x7b)
#define STUB_SC_FRAME_TX_ID         (STUB_REQ_BASE_ID + 0x7c)   //单载波(窄带)报文发送
#define STUB_SC_RX_STATIC_REQ_ID    (STUB_REQ_BASE_ID + 0x7d)   //单载波(窄带)报文接收统计查询
#define STUB_SET_TMI_TABLE_ID       (STUB_REQ_BASE_ID + 0x7e)
#define STUB_REQ_MAX_ID             (STUB_SET_TMI_TABLE_ID)


//异常测试GTR下发的命令ID
#define STUB_ABNORMITY_REQ_BASE_ID          0x00a0
#define STUB_ISP_RX_TX_SWITCH_REQ_ID        (STUB_ABNORMITY_REQ_BASE_ID + 0x00)//ISP的收、发切换异常测试
#define STUB_SOFT_RESET_REQ_ID              (STUB_ABNORMITY_REQ_BASE_ID + 0x01)//软件对PHY复位
/* BEGIN: Added by likunhe/00194117, 2012/5/11   问题单号:DTS2012052206629*/
#define STUB_BACK_TONE_MASK_REQ_ID          (STUB_ABNORMITY_REQ_BASE_ID + 0x02)//使用备用ToneMask
#define STUB_NTB_ROLE_CONFIG_REQ_ID         (STUB_ABNORMITY_REQ_BASE_ID + 0x03)//NTB的角色配置
#define STUB_NTB_RESULT_REQ_ID              (STUB_ABNORMITY_REQ_BASE_ID + 0x04)//NTB的结果查询
/* BEGIN: Modified by likunhe/00194117, 2012/5/22   问题单号:DTS2012052508462*/
#define STUB_POWER_SAVE_REQ_ID              (STUB_ABNORMITY_REQ_BASE_ID + 0x05)//启动低功耗测试
#define STUB_RX_TX_SWITCH_REQ_ID            (STUB_ABNORMITY_REQ_BASE_ID + 0x06)//启动rx/tx切换测试
/* BEGIN: Modified by likunhe/00194117, 2012/6/18   PN:DTS2012061806451*/
//#define STUB_ABNORMITY_REQ_MAX_ID           STUB_RX_TX_SWITCH_REQ_ID
#define STUB_GTR_CUT_LOG_REQ_ID             (STUB_ABNORMITY_REQ_BASE_ID + 0x07)//打印测试例结束的log分割符
/* BEGIN: Modified by likunhe/00194117, 2012/6/20   问题单号:AddChannelConfigPara*/
#define STUB_CHANNEL_CONFIG_REQ_ID          (STUB_ABNORMITY_REQ_BASE_ID + 0x08)//信道参数配置
#define STUB_SPEED_CONFIG_REQ_ID            (STUB_ABNORMITY_REQ_BASE_ID + 0x09)//速率测试
#define STUB_NOISE_CONFIG_REQ_ID            (STUB_ABNORMITY_REQ_BASE_ID + 0x0a)//噪声参数配置
#define STUB_WR_REG_REQ_ID                  (STUB_ABNORMITY_REQ_BASE_ID + 0x0b)//写寄存器
#define STUB_RD_REG_REQ_ID                  (STUB_ABNORMITY_REQ_BASE_ID + 0x0c)//读寄存器
#define STUB_TFPT_DOWNLOAD_REG_ID           (STUB_ABNORMITY_REQ_BASE_ID + 0x0d )//tftp下载
#define STUB_ABNORMITY_REQ_MAX_ID           STUB_TFPT_DOWNLOAD_REG_ID


//GTR下发的NV项变更的ID编号基值
#define STUB_CMD_BASE_NV_ID                 0x0720
#define STUB_CMD_AGC_PARA_NV_ID             (STUB_CMD_BASE_NV_ID + 0x0000)
#define STUB_CMD_HREST_NV_ID                (STUB_CMD_BASE_NV_ID + 0x0001)
#define STUB_CMD_FEQ_NV_ID                  (STUB_CMD_BASE_NV_ID + 0x0002)
#define STUB_CMD_CARRIER_SENSE_NV_ID        (STUB_CMD_BASE_NV_ID + 0x0003)
#define STUB_CMD_TX_NV_ID                   (STUB_CMD_BASE_NV_ID + 0x0004)
#define STUB_CMD_TD_PROCESS_NV_ID           (STUB_CMD_BASE_NV_ID + 0x0006)
#define STUB_CMD_FEC_NV_ID                  (STUB_CMD_BASE_NV_ID + 0x0007)
#define STUB_CMD_SYMALIGN_NV_ID             (STUB_CMD_BASE_NV_ID + 0x000b)
#define STUB_CMD_NBI_PARA_NV_ID             (STUB_CMD_BASE_NV_ID + 0x000c)
#define STUB_CMD_IMPN_PARA_NV_ID            (STUB_CMD_BASE_NV_ID + 0x000d)
#define STUB_CMD_PLCSC_PARA_NV_ID           (STUB_CMD_BASE_NV_ID + 0x000e)
#define STUB_CMD_CUBIC_PARA_NV_ID           (STUB_CMD_BASE_NV_ID + 0x000f)
#define STUB_CMD_FREQ_NV_ID                 (STUB_CMD_BASE_NV_ID + 0x0010)
#define STUB_CMD_ISP_NV_ID                  (STUB_CMD_BASE_NV_ID + 0x0011)
#define STUB_CMD_CLOCK_NV_ID                (STUB_CMD_BASE_NV_ID + 0x0012)
//#define STUB_CMD_NTB_NV_ID                  (STUB_CMD_BASE_NV_ID + 0x0013)
#define STUB_CMD_PCS_NV_ID                  (STUB_CMD_BASE_NV_ID + 0x0014)
#define STUB_CMD_PHY_STUB_NV_ID             (STUB_CMD_BASE_NV_ID + 0x0015)
#define STUB_CMD_OVER_CURRENT_NV_ID         (STUB_CMD_BASE_NV_ID + 0x0016)
#define STUB_CMD_FREQ_PARA_NV_1_ID          (STUB_CMD_BASE_NV_ID + 0x0017)
#define STUB_CMD_CARRIER_SENSE_NV_1_ID      (STUB_CMD_BASE_NV_ID + 0x0018)
#define STUB_CMD_CARRIER_SENSE_NV_2_ID      (STUB_CMD_BASE_NV_ID + 0x0019)
#define STUB_CMD_MAX_NV_ID                  (STUB_CMD_CARRIER_SENSE_NV_2_ID)


//上报GTR的ID
#define STUB_RX_BASE_ID             0x0080
#define STUB_TX_DONE_ID             (STUB_RX_BASE_ID + 0x00)  // TX完成消息
#define STUB_FC_CRC_RESULT_ID       (STUB_RX_BASE_ID + 0x01)  // FC的CRC结果的消息
#define STUB_PAYLOAD_CRC_RESULT_ID  (STUB_RX_BASE_ID + 0x02)  // Payload的CRC结果的消息
#define STUB_PCS_RESULT_ID          (STUB_RX_BASE_ID + 0x03)  // PCS的结果的消息
#define STUB_CHANNEL_INFO_ID        (STUB_RX_BASE_ID + 0x04)  // 信道质量上报
#define STUB_STATISTIC_INFO_ID      (STUB_RX_BASE_ID + 0x05)  // 信道质量上报
#define STUB_ISP_RESULT_INFO_ID     (STUB_RX_BASE_ID + 0x06)  // 查询ISP的结果
#define STUB_STATISTIC_INTERRUPT_INFO_ID    (STUB_RX_BASE_ID + 0x07)  // 查询ISP的结果
#define STUB_NTB_RESULT_INFO_ID     (STUB_RX_BASE_ID + 0x08)  //上报的NTB查询结果
#define STUB_PCS_IND                (STUB_RX_BASE_ID + 0x09)
#define STUB_FC_IND                 (STUB_RX_BASE_ID + 0x0a)
#define STUB_PB_IND                 (STUB_RX_BASE_ID + 0x0b)
#define STUB_WAKE_SLEEP_IND         (STUB_RX_BASE_ID + 0x0c)
#define STUB_GTR_CUT_LOG_IND        (STUB_RX_BASE_ID + 0x0d)  //GTR区分Log的分割符
#define STUB_CHIP_SELECT_INFO_IND   (STUB_RX_BASE_ID + 0x0e)  //芯片筛选的结果
#define STUB_RD_REG_INFO_ID         (STUB_RX_BASE_ID + 0x0f)  //读取的寄存器结果
#define STUB_RCV_INFO_ID            (STUB_RX_BASE_ID + 0X10)    //接收信息结果上报
#define STUB_MAC_PHY_TEST_INFO_ID   (STUB_RX_BASE_ID + 0X11)    //mac_phy接口测试信息上报
#define STUB_SC_RX_STATIC_RSP_ID    (STUB_RX_BASE_ID + 0x12)   //单载波(窄带)报文接收统计查询应答
#define STUB_SC_TX_FRAME_RPT_ID     (STUB_RX_BASE_ID + 0x13)   //单载波发送报文维测上报
#define STUB_SC_RX_FRAME_RPT_ID     (STUB_RX_BASE_ID + 0x14)   //单载波接收报文维测上报
#define STUB_SC_FREAM_TIME_RPT_ID     (STUB_RX_BASE_ID + 0x15)   //单载波接收报文维测上
#define STUB_SC_PCS_RPT_ID     (STUB_RX_BASE_ID + 0x16)   //单载波接收报文维测上报


#define FC_BYTE_SIZE        16          //FC的字节数
#define TONE_MAP_SIZE       512         //Tone Map的大小
/* BEGIN: Added by likunhe/00194117, 2012/6/20   PN:AddChannelConfigPara*/
#define CHANNEL_COEFF_NUM           1024        //信道系数的个数
/* END:   Added by likunhe/00194117, 2012/6/20 */
/* BEGIN: Added by likunhe/00194117, 2012/7/19   PN:AddNoiseCtrl*/
#define NOISE_PACKET_DATA_NUM       512         //噪声数据包的大小
/* END:   Added by likunhe/00194117, 2012/7/19 */
/* BEGIN: Added by likunhe/00194117, 2012/8/31   PN:AddNoiseCtrl*/
#define TFTP_DOWNLOAD_FILE_MAX_BYTE_SIZE (8 * 1024 * 1024)   //tftp文件的最大字节数
/* END:   Added by likunhe/00194117, 2012/8/31 */

//单载波的工作模式
#define STUB_SC_WORK_MODE_OFF                   0           //关闭
#define STUB_SC_WORK_MODE_TX                    1           //发送模式,带内容填充
#define STUB_SC_WORK_MODE_RX                    2           //接收模式
#define STUB_SC_WORK_MODE_TX_PKT_CHECK          3           //发送模式，报文自动校验
#define STUB_SC_WORK_MODE_RX_PKT_CHECK          4           //接收模式，报文自动校验
#define STUB_SC_WORK_MODE_TX_NTB                5           //发送模式，带NTB
#define STUB_SC_WORK_MODE_RX_NTB                6           //接收模式，带NTB
#define STUB_SC_WORK_MODE_PHY_TEST              7           //PHY的测试模式

//报文类型
#define RTS_CTS_DT                  (3)
#define SOUND_DT                    (4)
#define SYNCH_DT                     (5)
#define UNIVERSE_DT                 (6)

/************************************************************
                             数据结构定义
************************************************************/

//HSO下发的TX的Tone Map结构
typedef struct _tx_tone_map_table_st_
{
    //HI_U32 dtei;
    bit_loading_st tone_map;
}tx_tone_map_table_st;

//HSO下发的RX的Tone Map结构
typedef struct _rx_tone_map_table_st_
{
    /* BEGIN: Modified by likunhe/00194117, 2012/5/29   问题单号:SoundingBitloadingTest*/
    HI_U16 stei;
    HI_U16 tmi;
    /* END:   Modified by likunhe/00194117, 2012/5/29 */
    bit_loading_st tone_map_table_pb136;
    bit_loading_st tone_map_table_pb520;
}rx_tone_map_table_st;


//Bitloading的Tone Map的上报结构
typedef struct _bitloading_result_st_
{
    bit_loading_st tone_map_table_pb136;
    bit_loading_st tone_map_table_pb520;
}bitloading_result_st;


//stub结果上报控制开关
typedef struct _stub_reprot_switch_st_
{
    HI_U8  crc_switch;
    HI_U8  tx_done_switch;
    HI_U8  pcs_switch;
    HI_U8  channel_info_switch;
}stub_reprot_switch_st;

//生成Beacon帧的FC参数
typedef struct _beacon_fc_para_st_
{
    HI_U8  dt;         //定界符
    HI_U8 access;
    HI_U8  snid;       //短网络标识符
    HI_U8 insertCP_flag;
    HI_U32 bts;        //信标时间戳（绝对时间）
    HI_U32 bpc;        //信标周期计数
    HI_U16 stei;       //源设备标识符
    HI_U8  tmi;
	HI_U8 bts_ext;
	HI_U32 rsd;
}beacon_fc_para_st;


//生成Sof帧的FC参数
typedef struct _sof_fc_para_st_
{
    HI_U8  dt;        //定界符
    HI_U8 access;
    HI_U8  snid;      //短网络标识符
    HI_U8 insertCP_flag;

    HI_U16 stei;      //源设备标识符
    HI_U16 dtei;      //目标设备标识符

    HI_U8  pb_num;    //PB块数目
    HI_U8  tmi;       //ToneMap索引号
    HI_U8  bcf;       //广播标志位
    HI_U8 next_tmi;

	HI_U8 ext_tmi;
	HI_U8 rsd1;
	HI_U16 reserve2;     //保留
    /* BEGIN: Added by likunhe/00194117, 2012/3/3   问题单号:V130 Stub Edit*/
    //HI_U8  pb_size;   //PB块的大小，0：520，1：136
    // HI_U8  pilot;      //pilot标识
    /* END:   Added by likunhe/00194117, 2012/3/3 */
}sof_fc_para_st;


//生成Sound帧的FC参数
typedef struct _sound_fc_para_st_
{
    HI_U8  dt;         //定界符
    HI_U8  snid;       //短网络标识符
    HI_U16 stei;       //源设备标识符
    HI_U16 dtei;       //目标设备标识符
    HI_U8  saf;        //侦听应答标志
    /* BEGIN: Added by likunhe/00194117, 2012/3/3   问题单号:V130 Stub Edit*/
    //HI_U8  tmi;        //
    HI_U8  ver;        //
    /* END:   Added by likunhe/00194117, 2012/3/3 */
}sound_fc_para_st;


//生成Sack帧的FC的参数
typedef struct _sack_fc_para_st_
{
    HI_U8  dt;
    HI_U8  snid;
    HI_U8 access;//j00206949 2015.2.7
    HI_U8 reserve;
}sack_fc_para_st;

//生成Rts/Cts帧的FC的参数
typedef struct _rts_cts_fc_para_st_
{
    HI_U8  dt;
    HI_U8  snid;
    HI_U8 access;//j00206949 2015.2.7
    HI_U8 ver;
}rts_cts_fc_para_st;

//j00206949 2015.2.7 start
//生成coordinate帧的FC的参数
typedef struct _coordinate_fc_para_st_
{
    HI_U8  dt;
    HI_U8  snid;
    HI_U8 access;
    HI_U8 reserve;
}coordinate_fc_para_st;

//生成通用帧的FC的参数
typedef struct _universe_fc_para_st_
{
    HI_U8  dt;
    HI_U8  snid;
    HI_U8 access;//j00206949 2015.2.7
    /* BEGIN: Modified by liujian 00280874, 2016/5/30   问题单号:MAC_PHY_TEST*/
    //HI_U8 reserve;
    HI_U8 stei;
    /* END:   Modified by liujian 00280874, 2016/5/30 */

    HI_U8 mac[6];
    HI_U16 ver;
}universe_fc_para_st;

//生成保留帧的FC的参数
typedef struct _reserve_fc_para_st_
{
    HI_U8  dt;
    HI_U8  snid;
    HI_U8 access;//j00206949 2015.2.7
    HI_U8 reserve;
}reserve_fc_para_st;
//j00206949 2015.2.7 end

// 本网SNID
typedef struct _stub_my_snid_st_
{
    HI_U8  my_snid;
    HI_U8  resreve1;
    HI_U16 reserve2;
}stub_my_snid_st;

// 本站点TEI
typedef struct _stub_my_tei_st_
{
    HI_U16 my_tei;
    HI_U16 reserve;
}stub_my_tei_st;

typedef struct _stub_ntb_para_st_
{
    HI_U16 syn_freq;
    HI_U16 syn_snid;

    HI_U16 syn_tei;
    HI_U16 pad;
}stub_ntb_para_st;

typedef struct _stub_my_access_st_
{
    HI_U16 access;
    HI_U16 reserve;
}stub_my_access_st;

// 本站点角色
typedef struct _stub_my_role_st_
{
    //HI_U8  my_role;
    //HI_U8  reserve1;
    //HI_U16 reserve2;
    HI_U32 my_role;//实际上是中断向量
}stub_my_role_st;

// 本站点父代理TEI
typedef struct _stub_my_proxy_st_
{
    /* BEGIN: Modified by likunhe/00194117, 2012/5/17   问题单号:DTS2012052206629*/
    //HI_U8  my_proxy;
    //HI_U8  reserve1;
    //HI_U16 reserve2;
    HI_U16 my_proxy;
    HI_U16 reserve1;
    /* END:   Modified by likunhe/00194117, 2012/5/17 */
}stub_my_proxy_st;

/* BEGIN: Added by likunhe/00194117, 2012/5/17   PN:DTS2012052206629*/
//NTB的角色配置
typedef struct _stub_ntb_role_st_
{
    HI_U8  synch_snid;
    HI_U8  ntb_stei_parse_en;   //同步父代理判断使能
    HI_U16 rsd2;
}stub_ntb_role_st;
/* END:   Added by likunhe/00194117, 2012/5/17 */

// 前导检出上报的
typedef struct _stub_pcs_report_st_
{
    HI_U8  pcs_report;
    HI_U8  type;
    HI_U16 reserve;
}stub_pcs_report_st;

// CRC结果上报的开关
typedef struct _stub_crc_switch_st_
{
    HI_U8  crc_switch;
    HI_U8  reserve1;
    HI_U16 reserve2;
}stub_crc_switch_st;

// PCS结果上报的开关
typedef struct _stub_pcs_switch_st_
{
    HI_U8  pcs_switch;
    HI_U8  reserve1;
    HI_U16 reserve2;
}stub_pcs_switch_st;

// BLER结果上报的开关
typedef struct _stub_bler_switch_st_
{
    HI_U8  bler_switch;
    HI_U8  reserve1;
    HI_U16 reserve2;
}stub_bler_switch_st;

// 发送结果上报的开关
typedef struct _stub_tx_done_switch_st_
{
    HI_U8  dt;
    HI_U8  reserve1;
    HI_U16 reserve2;
}stub_tx_done_switch_st;

// 信道质量上报的开关
typedef struct _stub_channel_info_switch_st_
{
    HI_U8  channel_info_switch;
    HI_U8  reserve1;
    HI_U16 reserve2;
}stub_channel_info_switch_st;

/* BEGIN: Added by likunhe/00194117, 2012/3/4   问题单号:V130 Stub Edit*/
typedef struct
{
    HI_U8  statistic_info_switch;
    HI_U8  rsd1;
    HI_U16 period;           //定时上报的周期，单位为ms
}stub_statistic_info_switch_st;
/* END:   Added by likunhe/00194117, 2012/3/4 */

/* BEGIN: Added by likunhe/00194117, 2012/4/28   问题单号:DTS2012050801863*/
//ISP发送开关
typedef struct
{
    HI_U8  tx_en;
    HI_U8  tx_once_en;
    HI_U16 rsd2;
}stub_isp_tx_para_st;

//ISP接收开关
typedef struct
{
    HI_U8  isp_det_en;
    HI_U8  isp_det_mode;
    HI_U16 rsd2;
}stub_isp_rx_para_st;
/* END:   Added by likunhe/00194117, 2012/4/28 */

/* BEGIN: Added by likunhe/00194117, 2012/5/3   问题单号:DTS2012050801863*/
//ISP收、发切换定时器配置参数
typedef struct
{
    HI_U16 timer_switch;
    HI_U16 rsd;
    HI_U32 timer_period;
}stub_isp_rx_tx_switch_para_st;
/* END:   Added by likunhe/00194117, 2012/5/3 */

/* BEGIN: Added by likunhe/00194117, 2012/5/5   问题单号:DTS2012050801863*/
//软件对PHY复位的定时器配置参数
typedef struct
{
    HI_U16 timer_switch;
    HI_U16 rsd;
    HI_U32 timer_period;
}stub_soft_reset_para_st;
/* END:   Added by likunhe/00194117, 2012/5/5 */

//OM统计量
typedef struct _stub_tx_om_st_
{
    HI_U32 stub_receive_cnt;
    HI_U32 nv_id_error;
    /* BEGIN: Added by likunhe/00194117, 2012/5/5   问题单号:DTS2012050801863*/
    HI_U32 abnormity_cmd_error;
    /* END:   Added by likunhe/00194117, 2012/5/5 */
}stub_tx_om_st;


// 发送结果上报的结构
typedef struct _tx_done_result_st_
{
    HI_U8  frame_type;     // 帧类型
    HI_U8  result;         // ?
    HI_U16 rsd;
} tx_done_result_st;

// FC的CRC结果上报的结构
typedef struct _fc_crc_result_st_
{
    HI_U8  frame_type;
    HI_U8  crc_result;
    HI_U16 rsd;
}fc_crc_result_st;

// Payload的CRC结果上报的结构
typedef struct _payload_crc_result_st_
{
    HI_U8  frame_type;
    HI_U8  crc_result;
    HI_U8  pb_index;
    HI_U8  reserve;
}payload_crc_result_st;

//PCS的结果上报的结构
typedef struct _pcs_resut_st_
{
    HI_U8  type;
    HI_U8  reserve1;
    HI_U16 reserve2;
}pcs_resut_st;

//信道质量上报的结构
typedef struct _channel_info_st_
{
    HI_U8  channel_info;
    HI_U8  reserve1;
    HI_U16 reserve2;
}channel_info_st;
/* BEGIN: Added by likunhe/00194117, 2012/3/4   问题单号:V130 Stub Edit*/
//PCS和FC的CRC统计
typedef struct _statistic_info_st_
{
    HI_U32 pcs_interrupt_cnt;      //Preamble中断
    HI_U32 fc_interrupt_cnt;       //FC的中断
    HI_U32 pb_interrupt_cnt;       //PB的中断
    HI_U32 rx_end_interrupt_cnt;   //接收完成中断
    HI_U32 tx_end_interrupt_cnt;   //发送完成中断

    HI_U32 fc_right_expect_cnt;
    HI_U32 fc_right_cnt;
    HI_U32 fc_total_cnt;
    HI_U32 stub_tx_cnt;            //Stub的发送次数
}statistic_info_st;
/* END:   Added by likunhe/00194117, 2012/2/24 */

/* BEGIN: Added by likunhe/00194117, 2012/4/28   问题单号:DTS2012050801863*/
//异常中断统计
typedef struct _statistic_interrupt_info_st_
{
    //HI_U32 isp_series_detect    ;
    HI_U32 nbi_pow_int_cnt;
    HI_U32 isp_win_detect       ;
    HI_U32 isp_send_end         ;
    HI_U32 zero_cross_detect    ;
    HI_U32 only_preamble_rcv    ;
    HI_U32 onlypreamble_send_end;
    HI_U32 tone_map_move_err    ;
    HI_U32 tei_query_err        ;
    HI_U32 beacon_tmi_err       ;
    HI_U32 sof_tmi_err          ;
 	HI_U32 burst_send_finish_cnt;
	HI_U32 burst_rcv_finish_cnt;
	HI_U32 snr_rpt_cnt;
	HI_U32 fc_err_cnt;
	HI_U32 fc_ok_cnt;
	HI_U32 pb4_cnt;
	HI_U32 pb3_cnt;
	HI_U32 pb2_cnt;
	HI_U32 pb1_cnt;
	HI_U32 rcv_overflow_cnt;
	HI_U32 st4_cnt;
	HI_U32 st3_cnt;
	HI_U32 st2_cnt;
	HI_U32 para_ok_cnt;
	HI_U32 frame_send_ok_cnt;
	HI_U32 frame_send_fail_cnt;
	HI_U32 one_frame_send_rlt_cnt;
	HI_U32 one_frame_send_start_cnt;
	HI_U32 one_frame_send_finish_cnt;
	HI_U32 tx_rx_collision_cnt;
	HI_U32 tx_tx_collision_cnt;
	HI_U32 wirte_collision_cnt;
	HI_U32 cancel_send_fail_cnt;
	//HI_U32 rsv;
	HI_U32 nbi_table_int_cnt;
}statistic_interrupt_info_st;

//ISP结果信息统计
typedef struct _isp_result_info_st_
{
    HI_U32 f1_ph1_cnt;
    HI_U32 f1_ph2_cnt;
    HI_U32 f1_ph3_cnt;
    HI_U32 f1_ph4_cnt;
    HI_U32 f1_ph5_cnt;
    HI_U32 f2_ph1_cnt;
    HI_U32 f2_ph2_cnt;
    HI_U32 f2_ph3_cnt;
    HI_U32 f2_ph4_cnt;
    HI_U32 f2_ph5_cnt;
}isp_result_info_st;
/* END:   Added by likunhe/00194117, 2012/4/28 */

/* BEGIN: Added by likunhe/00194117, 2012/5/17   PN:DTS2012052206629*/
//NTB的结果
typedef struct _ntb_result_info_
{
    HI_U32 curr_bts;    //当前解出的bts值
}ntb_result_info;
/* END:   Added by likunhe/00194117, 2012/5/17 */
/* BEGIN: Added by likunhe/00194117, 2012/5/22   PN:DTS2012052508462*/
//低功耗测试的开关
typedef struct _power_save_switch_st_
{
    HI_U16 timer_switch;
    HI_U16 rsd;
    HI_U32 timer_period;
}power_save_switch_st;

//低功耗测试结果上报
typedef struct
{
    HI_U32 TimerCount;
    HI_U32 CrcRsult;
}OM_POWER_SAVE_TRACE_IND_STRU;
/* END:   Added by likunhe/00194117, 2012/5/22 */

/* BEGIN: Added by likunhe/00194117, 2012/5/23   PN:DTS2012052508462*/
//rx/tx切换测试开关
typedef struct _rx_tx_switch_st_
{
    HI_U16 timer_switch;
    HI_U16 rsd;
    HI_U32 timer_period;
}rx_tx_switch_st;
/* END:   Added by likunhe/00194117, 2012/5/23 */

#define PL_LEN_MAX (520)

//GTR打测试例结果分割符
typedef struct _gtr_mpdu_st_
{
    HI_U16 mpdu_num;
    HI_U16 mpdu_len;
    HI_U8 mpdu[PL_LEN_MAX];
}gtr_mpdu_st;
typedef struct _mpdu_st_
{
    HI_U16 mpdu_num;
    HI_U16 mpdu_len;
    HI_U8 mpdu[PL_LEN_MAX * PB_NUM_MAX];
}mpdu_st;
/* BEGIN: Added by likunhe/00194117, 2012/6/20   PN:AddChannelConfigPara*/
//信道参数的配置
typedef struct _channel_config_st_
{
    HI_U32 index;       //数据包的索引
    HI_S16 data[512];
}channel_config_st;
/* END:   Added by likunhe/00194117, 2012/6/20 */
/* BEGIN: Added by likunhe/00194117, 2012/6/30   PN:DTS2012062705423*/
//芯片筛选的结果
typedef struct _chip_select_info_st_
{
   HI_U32 reslut;
}chip_select_info_st;
/* END:   Added by likunhe/00194117, 2012/6/30 */
/* BEGIN: Added by likunhe/00194117, 2012/7/19   PN:AddNoiseCtrl*/
//噪声数据
typedef struct _noise_config_st_
{
    HI_U16 index;                       //数据包的索引号
    HI_U16 length;                      //数据包的有效长度:字节
    HI_U32 addr;                        //要写入的地址
    HI_U8  data[NOISE_PACKET_DATA_NUM]; //噪声数据
}noise_config_st;

//写寄存器的参数
typedef struct _wr_reg_para_st_
{
    HI_U32 addr;            //地址
    HI_U32 data;            //要写入的数据
}wr_reg_para_st;

//读寄存器的参数
typedef struct _rd_reg_para_st_
{
    HI_U32 addr;            //地址
}rd_reg_para_st;

//读取的寄存器值
typedef struct _rd_reg_info_st_
{
    HI_U32 data;            //数据
}rd_reg_info_st;
/* END:   Added by likunhe/00194117, 2012/7/19 */
/* BEGIN: Added by likunhe/00194117, 2012/8/31   PN:AddNoiseCtrl*/
//tftp下载的参数
typedef struct _tftp_download_para_st_
{
    HI_CHAR remote_ip[4];       //PC的IP地址
    HI_CHAR path[128];           //文件路径
    HI_PBYTE loc_addr;          //数据要写入的内在地址
    HI_U32  file_size;          //数据文件的大小，单位:字节
}tftp_download_para_st;
/* END:   Added by likunhe/00194117, 2012/8/31 */

// 此参数用于旧逻辑版本(0728)
typedef struct _filter_cfg_para_st_
{
	HI_U8 filter_id;
	HI_U8 filter_dt;
	HI_U8 filter_access;
	HI_U8 filter_snid;
	HI_U16 filter_dtei;
	HI_U16 filter_action;
	HI_U8 filter_dt_vld;
	HI_U8 filter_access_vld;
	HI_U8 filter_snid_vld;
	HI_U8 filter_dtei_vld;
}filter_cfg_para_st;

// 此参数用于新逻辑版本，目前b版本使用
typedef struct _filter_cfg_para_1_st_
{
	filter_cfg_para_st stFilterCfgPara;
	HI_U16 filter_stei;
	HI_U8 filter_stei_vld;
	HI_U8 rsv;
}filter_cfg_para_1_st;

typedef struct _stub_cyc_send_info_st_
{
    HI_U32 cyc_send_gap;    //循环发送的帧间隔，单位us
    HI_U16 cyc_send_time;   //循环发送的次数
    HI_U16 cyc_send_mode;   //循环发送使能标识，1:多载波循环发送使能,2:单载波循环发送使能
}stub_cyc_send_info_st;

/* BEGIN: Added by liujian 00280874, 2016/4/29   问题单号:MAC_PHY_TEST*/
typedef struct
{
    HI_U8 is_5bit_tmi;
    HI_U8 is_not_clear_buff_status;
    HI_U8 wr_collision_fc_pb;
    HI_U8 read_buff_when_phy_read;

    HI_U8 pad;
    HI_U8 is_fill_payload_data_from_file;
    HI_U8 is_v200_mode;
    HI_U8 is_niverse_frame_head_stamp;
}stub_test_info_st;

typedef struct
{
    HI_U8 is_fill_time_stamp;
    HI_U8 is_fill_local_clock;
    HI_U16 time_stamp_offset;
}stub_send_time_stamp_st;

typedef struct
{
    HI_BOOL use_local_clock;
    HI_U8 pad;
    HI_S16 send_freqerr;
}stub_tx_freqerr_st;


typedef struct
{
    HI_BOOL is_fix_rx_freqerr;
    HI_U8  pad;
    HI_S16 rcv_freqerr;
}stub_rx_freqerr_st;

typedef struct
{
    HI_U8 mac[6];
    HI_U8 rsvd[2];
}stub_mac_addr_st;

typedef struct
{
    HI_U8 verid_enable[4];
    HI_U8 verid_value[4];
    HI_U8 verid_policy[4];

    HI_U8 dt_enable[9];
    HI_U8 dt_value[9];
    HI_U8 dt_policy[9];
    HI_U8 snid_enable[6];
    HI_U8 snid_value[6];
    HI_U8 snid_policy[6];
    HI_U8 odtei_enable[3];
    HI_U8 odtei_value[3];
    HI_U8 odtei_policy[3];
    HI_U8 dmac_enable[3];
    stub_mac_addr_st dmac_value[3];
    HI_U8 dmac_policy[3];
    HI_U8 bcf_enable[2];
    HI_U8 bcf_value[2];
    HI_U8 bcf_policy[2];
    HI_U8 access_enable[3];
    HI_U8 access_value[3];
    HI_U8 access_policy[3];

    HI_U8 Pad;
}stub_buff_filter_st;

typedef struct
{
    HI_U8 report_switch;
    HI_U8 rsd1;
    HI_U16 rsd2;
}stub_statistic_interrupt_info_st;


typedef struct
{
    HI_U8 ver_index;
    HI_BOOL ver_enable_flag;
    HI_U8 dt_index;
    HI_BOOL dt_enable_flag;

    HI_U8 data_tart_addr;
    HI_U8 data_end_addr;
    HI_BOOL data_enable_flag;
    HI_U8 pad;
}defc_form_set_info_st;

typedef struct
{
    HI_U8 data;
    HI_U8 filter_policy;
    HI_BOOL enable_flag;
    HI_U8 pad;
}buff_filter_set_info_st;

typedef struct
{
    HI_U8  dt;         //定界符
    HI_U8 access;
    HI_U8  snid;       //短网络标识符
    HI_U8 insertCP_flag;
    HI_U32 bts;        //信标时间戳（绝对时间）
    HI_U32 bpc;        //信标周期计数
    HI_U16 stei;       //源设备标识符
    HI_U8  tmi;
    HI_U8 pb_cnt;
    HI_U8 ver;
    HI_U8 rsd[3];
}v200_bcn_fc_para_st;

typedef struct
{
    HI_U8  dt;        //定界符
    HI_U8 access;
    HI_U8  snid;      //短网络标识符
    HI_U8 insertCP_flag;

    HI_U16 stei;      //源设备标识符
    HI_U16 dtei;      //目标设备标识符

    HI_U8  pb_num;    //PB块数目
    HI_U8  tmi;       //ToneMap索引号
    HI_U8  bcf;       //广播标志位
    HI_U8 ext_tmi;

    HI_S16 freq_err;
    HI_U8 tx_local_clock;
    HI_U8 ver;
}v200_sof_fc_para_st;

typedef struct
{
    HI_U8  dt;
    HI_U8  snid;
    HI_U8 access;
    HI_U8 ver;
}v200_sack_fc_para_st;

typedef struct
{
    HI_U8  dt;
    HI_U8  snid;
    HI_U8 access;
    HI_U8 ver;
}v200_coordinate_fc_para_st;

typedef struct
{
    HI_U8  dt;        //定界符
    HI_U8 access;
    HI_U8  pad;
    HI_U8  bcf;       //广播标志位

    HI_U32  snid;      //短网络标识符

    HI_U16 stei;      //源设备标识符
    HI_U16 dtei;      //目标设备标识符

    HI_U8  pb_num;    //PB块数目
    HI_U8  tmi;       //ToneMap索引号
    HI_U8 ext_tmi;
    HI_U8 ver;
}sg_sof_fc_para_st;

typedef struct
{
    HI_U8  dt;         //定界符
    HI_U8 access;
    HI_U8 rsd[2];

    HI_U32  snid;      //短网络标识符
    HI_U32 bpc;        //信标周期计数

    HI_U16 stei;       //源设备标识符
    HI_U8  tmi;
    HI_U8 ver;
}sg_bcn_fc_para_st;

typedef struct
{
    HI_U8  dt;
    HI_U8  rsvd;
    HI_U8 access;
    HI_U8 ver;

    HI_U32  snid;      //短网络标识符
}sg_sack_fc_para_st;

typedef struct
{
    HI_U8  dt;
    HI_U8  rsvd;
    HI_U8 access;
    HI_U8 ver;

    HI_U32  snid;      //短网络标识符
}sg_coordinate_fc_para_st;

typedef struct
{
    HI_U32 snid;
    HI_S16 freq_err;
    HI_U16 index;
}snid_freqerr_form_set_info_st;

typedef struct
{
    HI_U32 send_enable_time_diff;
    HI_U32 buff_ret_err_cnt;
    HI_U32 read_rcv_info_time;
    HI_U32 rsvd[11];
}mac_phy_test_info_st;

typedef struct
{
    HI_U8 nCopys;
    HI_U8 roboMap;
    HI_U8 pbSize;
    HI_U8 gi;

    HI_U8 codeRate;
    HI_U8 is_tmi_mode;
    HI_U8 tmi_index;
    HI_U8 rsvd;
}tmi_table_info_st;

typedef struct
{
    HI_U32 data[16];
    HI_U8 rsvd[4];
}tone_mask_table_info_st;

typedef struct
{
    HI_U8 data[512];
    HI_U8 rsvd[4];
}power_loading_table_info_st;


//窄带报文发送命令
typedef struct
{
    HI_U8  sc_mode;                         //窄带工作模式
    HI_U8  modulation_mode;            //调制方式
    HI_U16 frame_len;                       //帧长

    HI_U8 code_rate;                        //码率
    HI_U8 bts_offset;
    HI_U8 pad1[2];
}stub_sc_frame_tx_req_st;

//窄带报文接收统计查询请求
typedef struct
{
    HI_U8 is_query;                         //是否查询，1:查询。0，清除
    HI_U8 pad[3];
}stub_sc_rx_static_req_st;

//窄带报文接收统计查询应答
typedef struct
{
    HI_U32 tx_end_cnt;                      //发送完成中断统计
    HI_U32 pcs_cnt;                         //载波侦听中断统计
    HI_U32 rx_end_cnt;                      //接收完成中断统
    HI_U32 crc_err_cnt;             //译码错误中断统计

    HI_U32 rx_state_err_cnt;        //接收状态错误
    HI_U32 rx_len_err_cnt;          //接收报文长度错误
    HI_U32 rx_data_err_cnt;         //接收报文内容错误

    HI_U32 pad[3];
}stub_sc_rx_static_rsp_st;

//单载波NTB 报文
typedef struct
{
    HI_U8 is_ntb ;
    HI_U8 bts_offset;
    HI_U16 pkt_cnt ;

    HI_U8 data[60];
}stub_sc_ntb_pkt_st;

//单载波发送报文维测上报
typedef struct
{
    HI_U32 send_cnt;                        //发送报文的计数
    HI_U32 cur_ntb_high;                         //发送时的NTB
    HI_U32 cur_ntb_low;                         //发送时的NTB
    HI_U32 pkt_ntb;                         //报文中填充的NTB
    HI_U32 cur_ltmr_high;                        //发送时的ltmr
    HI_U32 cur_ltmr_low;                        //发送时的ltmr

    HI_U32 pad[4];
}stub_sc_tx_frame_rpt_st;


//单载波接收报文维测上报
typedef struct
{
    HI_U32 send_cnt_in_pkt;                 //接收报文中的发送计数
    HI_U32 rcv_cnt;                         //接收报文计数
    HI_U32 cur_ntb_high;                         //发送时的NTB的高位
    HI_U32 cur_ntb_low;                         //发送时的NTB的低位

    HI_U32 cur_ltmr_high;                        //发送时的ltmr的高位
    HI_U32 cur_ltmr_low;                        //发送时的ltmr的低位
    HI_U32 lock_ntb_high;                        //锁定的NTB的高位
    HI_U32 lock_ntb_low;                        //锁定的NTB的低位

    HI_U32 lock_ltmr_high;                       //锁定的ltmr的高位
    HI_U32 lock_ltmr_low;                       //锁定的ltmr的低位
    HI_U32 pkt_ntb;                         //报文中填充的NTB
    HI_U32 fec_clk_delay;                   //译码模块的延时

    HI_U16 phy_rcv_cnt;
    HI_S16 freq_err;
    HI_U32 pad[2];
}stub_sc_rx_frame_rpt_st;

typedef struct
{
       HI_U32 fream_time;
       HI_U32 send_time;
}stub_sc_frame_time_rpt_st;
typedef struct
{
       HI_U32 pcs_report_time;
       HI_U32 sys_ok_int_tim;
}stub_sc_pcs_rpt_st;
typedef struct
{
    HI_U8 is_rcv_filter_en;
    HI_U8 pad;
    HI_U16 pad2;

    HI_U16 expect_snid;
    HI_U16 expect_stei;
}stub_rx_rcv_filter_st;

/************************************************************
                             接口函数声明
 ************************************************************/

extern HI_VOID stub_init(HI_VOID);
/* BEGIN: Added by likunhe/00194117, 2012/4/1   问题单号:DTS2012040100709*/
/*****************************************************************************
 函 数 名  : GetToneMapFixed
 功能描述  : 配置固定ToneMap时的测试参数
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2012年3月31日
    作    者   : likunhe/00194117
    修改内容   : 新生成函数

*****************************************************************************/
extern HI_VOID GetToneMapFixed(HI_VOID);

/*****************************************************************************
 函 数 名  : GetToneMapBitloading
 功能描述  : 配置自动ToneMap时的测试参数
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2012年3月31日
    作    者   : likunhe/00194117
    修改内容   : 新生成函数

*****************************************************************************/
HI_EXTERN HI_VOID GetToneMapBitloading(HI_VOID);
/* END:   Added by likunhe/00194117, 2012/4/1 */

/* BEGIN: Added by likunhe/00194117, 2012/6/30   PN:DTS2012062705423*/
/*****************************************************************************
 函 数 名  : stub_chip_select_init
 功能描述  : 芯片筛选功能初始化
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2012年6月30日
    作    者   : likunhe/00194117
    修改内容   : 新生成函数

*****************************************************************************/
HI_EXTERN HI_VOID stub_chip_select_init(HI_VOID);
/* END:   Added by likunhe/00194117, 2012/6/30 */
/* BEGIN: Added by likunhe/00194117, 2012/7/10   PN:DTS2012062705407*/

/* BEGIN: Modified by liujian 00280874, 2016/5/3   问题单号:MAC_PHY_TEST*/
HI_EXTERN HI_VOID stub_statistic_interrupt_info_report(stub_statistic_interrupt_info_st* stub_statistic_interrupt_info);
/* END:   Modified by liujian 00280874, 2016/5/3 */

HI_EXTERN void stub_statistic_info_report(stub_statistic_info_switch_st *p_static_info_switch);
/* END:   Added by likunhe/00194117, 2012/7/10 */

/* BEGIN: Added by sunhaiyang/00238140, 2015/6/23   PN:B4 NTB*/
HI_EXTERN HI_VOID ntb_b4_clear_ntb_table(HI_VOID);
/* END:   Added by sunhaiyang/00238140, 2015/6/23 */
HI_VOID stub_rx_increase_isp_send_cnt(HI_VOID);

HI_VOID stub_rx_disable_isp_tx_en(HI_VOID);

#ifdef __cplusplus
}
#endif

#endif
