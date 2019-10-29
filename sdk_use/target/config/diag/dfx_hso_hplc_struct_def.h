//*****************************************************************************
//
//                  版权所有 (C), 1998-2018, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : dfx_hso.h
//  版 本 号   : V1.0
//  作    者   : wangchao/00230251
//  生成日期   : 2018-12-13
//  功能描述   : HSO plc parser 配置头文件
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2018-12-13
//    作    者 : w00230251
//    修改内容 : 创建文件
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   :
// SUBSYSTEM :
// MODULE    :
// OWNER     :
//*****************************************************************************



#ifndef __DFX_HSO_H__
#define __DFX_HSO_H__
#include <hi_types.h>
#include <dfx_sys.h>


HI_START_HEADER


///////////////////////////////////////////beacon_pb//////////////////////////////////////////////////////////

#define BEACON_PB_FIXED_LEN  20
// 信标帧：帧载荷（固定部分）
typedef struct  
{
	HI_U8 beacon_type : 3;
	HI_U8 network_form : 1;
	HI_U8 pad1 : 2;
	HI_U8 associate : 1;
	HI_U8 beacon_use_flag : 1;

	HI_U8 serial_num;
	HI_U8 cco_mac[6];
	HI_U32 beacon_period_cnt;
	HI_U32 pad2[2];
}MPDU_TYPE_BEACON_PB_FIXED;



/////////////////beacon 站点能力条目 info///////////////////////////////
#define STATION_INFO_ENTRY_LEN 13
//站点能力条目
typedef struct
{
	HI_U32 tei:12;  	// 站点TEI
	HI_U32 proxy_tei:12;  	// 代理TEI
	HI_U32 route_lowest_succ_rate:8;  		// 路径最低通信成功率
    HI_U8 mac[6];  			// 发送信标站点MAC地址
	HI_U8 role:4;  			// 角色
	HI_U8 level:4	;  			// 层级
	HI_U8 proxy_channel_quality;  	// 代理站点信道质量
	HI_U8 phase:2;  				// 相线
	HI_U8  pad2:6;
	HI_U8  pad3[3];    //***********************不一样
}STATION_INFO_ENTRY;

//////////////////////////////路由参数通知条目/////////////////////////////////
#define ROUTE_PARAM_ENTRY_LEN 8
typedef struct
{
	//HI_U16 	station_entry_head;						// 信标条目头，南网未定义
	//HI_U16 	station_entry_len;						// 信标条目长度，南网未定义

	HI_U16  rt_evaluate_cycle_time_len;				// 路由周期
	HI_U16 rt_cco_evaluate_timeout;					// 路由评估剩余时间
	HI_U16 proxy_station_discovery_list_period;		// 代理站点发现列表周期
	HI_U16 discovery_station_discovery_list_period;	// 发现站点发现列表周期
}ROUTE_PARAM;


//////////////////////////////频段通知条目/////////////////////////////////
#define FREQ_NOTIFY_ENTRY_LEN 5
typedef struct
{
	HI_U32	dest_freq;						// 目标频段    ***********************不一样
	HI_U32	freq_chg_time;					// 频段切换剩余时间
}FREQ_NOTIFY;

#define BEACON_SLOT_FIXED_LEN  8
typedef struct
{
	HI_U8	non_center_beacon_slot_cnt;		// 非中央信标时隙总数
	HI_U8	center_beacon_slot_cnt:4;		// 中央信标时隙总数
	HI_U8	csma_slot_phase_cnt:2;//CSMA时隙支持的相线个数
	HI_U8   pad1_l:2;//保留
	HI_U8   pad1_h;//保留
	HI_U8   proxy_beacon_slot_cnt;//代理信标时隙总数

	HI_U8   beacon_slot_len;//信标时隙长度
	HI_U8   csma_slot_Slice_len;//CSMA时隙分片长度
	HI_U8   bind_csma_slot_count;// 绑定CSMA时隙相线个数
	HI_U8   bind_csma_slot_lid;// 绑定CSMA时隙链路标识符
}BEACON_SLOT_FIXED_INFO;

#define MAX_UNKNOW_BEACON_BUFFER_LEN    200
typedef struct
{
	BEACON_SLOT_FIXED_INFO beacon_slot_fixed;

	HI_U16   tdma_slot_len;// TDMA时隙长度                ***********************不一样
	HI_U16   tdma_slot_lid;// TDMA时隙链路标识符          ***********************不一样

	HI_U32  beacon_start_ntb;// 信标周期起始网络基准时
	HI_U32  beacon_period_len;// 信标周期长度

	HI_U8  data[MAX_UNKNOW_BEACON_BUFFER_LEN];
}BEACON_SLOT_INFO;


//字节对齐数(fixed)(8) + TDMA时隙长度(1) + TDMA时隙链路标识符(1) + 信标周期起始网络基准时(4) + 信标周期长度(4) + 保留(2)

#define SLOT_VARIETY_POS_IN_BEACON_SLOT    (20)
typedef struct  
{
	HI_U8  pad3;//协议中无      ***********************不一样

	HI_U8 station_entry_cnt;
	HI_U8 station_entry_head;	// 信标条目头
	HI_U8 station_entry_len;  	// 信标条目长度
	STATION_INFO_ENTRY station_entry;   //站点能力条目

	HI_U32 	route_para_notify_head:16;						// 信标条目头，南网未定义
	HI_U32 	route_para_notify_len:16;						// 信标条目长度，南网未定义
	ROUTE_PARAM  route_para_notify_entry;				//路由参数通知条目

	HI_U32 	freq_head:16;						// 信标条目头，南网未定义
	HI_U32 	freq_len:16;						// 信标条目长度，南网未定义
	FREQ_NOTIFY freq_entry;

	HI_U32 	beacon_slot_head:16;						// 信标条目头，南网未定义
	HI_U32 	beacon_slot_len:16;						// 信标条目长度，南网未定义
	BEACON_SLOT_INFO beacon_slot_entry;

	HI_U32 	unknow_head:16;						// 信标条目头，南网未定义
	HI_U32 	unknow_len:16;						// 信标条目长度，南网未定义
	HI_U8 unknow_buffer[MAX_UNKNOW_BEACON_BUFFER_LEN];

	//频段变更条目
}MPDU_TYPE_BEACON_STATION_ENTRY;


typedef struct  
{
	MPDU_TYPE_BEACON_PB_FIXED beacon_pb_fixed;
	MPDU_TYPE_BEACON_STATION_ENTRY beacon_station_entry;
}MPDU_TYPE_BEACON_PB;
///////////////////////////////////////////beacon_pb//////////////////////////////////////////////////////////







// ACK帧
typedef struct
{
	HI_U32 dt : 3;
	HI_U32 access : 5;
	HI_U32 snid : 24;

	HI_U32 recvResult : 4;
	HI_U32 state : 4;
	HI_U32 stei : 12;
	HI_U32 dtei : 12;

	HI_U32 pb_num : 3;
	HI_U32 pad1 : 5;
	HI_U32 snr : 8;
	HI_U32 load : 8;
	HI_U32 pad2 : 8;

	HI_U32 extend_frame_type : 4;
	HI_U32 ver : 4;
	HI_U32 fccs : 24;
}MPDU_TYPE_SACK_FC;

// 网间协调帧
typedef struct  
{
	HI_U32 dt : 3;
	HI_U32 access : 5;
	HI_U32 snid : 24;
	
	HI_U32 length_of_time:16;
	HI_U32 bandwidth_offset:16;
	
	HI_U32 neighbor_snid : 24;
	HI_U32 pad1_1 : 8;
	
	HI_U32 pad1_2 : 4;
	HI_U32 ver : 4;
	HI_U32 fccs : 24;
}MPDU_TYPE_COD_FC;

typedef struct
{
    HI_U32 dt:3;
	HI_U32 access:5;
	HI_U32 snid:24;
	
	HI_U32 bts;
	
	HI_U32 stei:12;
	HI_U32 tmi:4;
	HI_U32 symbol:9;
	HI_U32 phase:2;
	HI_U32 phy_lump_cnt:3;
	HI_U32 pad1:2;
	
	HI_U32 pad2:4;
	HI_U32 ver:4;
	HI_U32 fccs:24;
}MPDU_TYPE_BEACON_FC;

typedef struct
{
    HI_U32 dt:3;
	HI_U32 access:5;
	HI_U32 snid:24;
	
	HI_U32 stei:12;
	HI_U32 dtei:12;
	HI_U32 lid:8;
	
	HI_U32 frame_len:12;//帧长
	HI_U32 pbcnt:4;
	HI_U32 symbol_num:9;
	HI_U32 bcf:1;//广播标志位，取值为0;
	HI_U32 resend_flag:1;//重传标志位
	HI_U32 encrypt_sign:1;  //加密标志位
	HI_U32 tmi:4;   //分集拷贝基本模式;
	
	HI_U32 extend_tmi:4;	//分集拷贝扩展模式;	
	HI_U32 ver:4;
	HI_U32 fccs:24;
}MPDU_TYPE_SOF_FC;

typedef struct
{	
	HI_U32 ver : 4;
	HI_U32 ostei : 12;
	HI_U32 odtei : 12;
	HI_U32 send_type : 4;

	HI_U32 send_cnt_limit : 5;
	HI_U32 pad : 3;
	HI_U32 msdu_sn : 16;
	HI_U32 msdu_type : 8;

	HI_U32 msdu_len : 11;
	HI_U32 restart_cnt : 4;
	HI_U32 proxy_main_path_sn :1;
	HI_U32 route_total_hop : 4;
	HI_U32 route_res_hop : 4;
	HI_U32 broadcast_dir : 2;
	HI_U32 path_repair_flag : 1;
	HI_U32 mac_addr_flag : 1;
	HI_U32 pad2 : 4;

	HI_U32 pad3:8;
	HI_U32 net_format_serial_number:8;
	HI_U32 pad4:8;
	HI_U32 pad5:8;
}MAC_FRAME_HEAD_FIXED;  

typedef struct  
{
	MAC_FRAME_HEAD_FIXED head_fixed;
	HI_U8 src_mac[6];
	HI_U8 dst_mac[6];
}MAC_FRAME_HEAD;

typedef struct
{
	HI_U16 mmType;
	HI_U16 pad;
}MANAGE_NEWS_HEAD;

typedef struct
{
	MAC_FRAME_HEAD macFrameHead;
	MANAGE_NEWS_HEAD manageNewshead;
}MAC_MANAGE_NEWS_FRAME_HEAD;

typedef struct
{
	HI_U32 datagram_port : 8;
	HI_U32 datagram_ID : 16;
	HI_U32 datagram_ctrl : 8;
}APP_HEAD;

typedef struct
{
	MAC_FRAME_HEAD_FIXED macFrameHead;
	APP_HEAD appHead;
}MAC_APP_FRAME_HEAD;


//关联请求报文
typedef struct
{
	HI_U8  mac[6];
	HI_U8 candidate_proxy_tei0_l;
	HI_U8 candidate_proxy_tei0_h:4;
	HI_U8 pad0 :4;
	
	HI_U16 candidate_proxy_tei1 :12;
	HI_U16 pad1 :4;
	HI_U16 candidate_proxy_tei2 :12;
	HI_U16 pad2 :4;
	
	HI_U16 candidate_proxy_tei3 :12;
	HI_U16 pad3 :4;
	HI_U16 candidate_proxy_tei4 :12;
	HI_U16 pad4 :4;
	
	HI_U8  phase0 :2;
	HI_U8  phase1 :2;
	HI_U8  phase2 :2;
	HI_U8  pad5 :2;
	HI_U8  dev_type;//设备类型
	HI_U8  mac_type;//MAC地址类型
	HI_U8  pad6;
	
	HI_U32  associate_random;//站点关联随机数
	 
	//HI_U8 user_defined[18]; //--- 厂家自定义信息 ---HI_SYS_STATUS_INFO_S
   //--- 自定义部分 START ---
	HI_U8  system_mode;
	HI_U8  boot_ver_idx;
	HI_U8  system_abnormal_reason;
	HI_U8  main_ver[3];
	HI_U8  build_time[6];

	HI_U16 second_ver[2];

	HI_U16  chip_ver;
	//--- 自定义部分 END ---
	
	//
    //--- 站点版本信息 BEGIN ---
	HI_U16  system_start_reason:8;//系统启动原因
	HI_U16  boot_ver:8;//BOOT版本号
	HI_U16  sw_ver;//软件版本号
	HI_U16 usYear :7;//版本时间
	HI_U16 usMonth :4;
	HI_U16 usDay :5;
	HI_U8  manuCode[2];//厂商代码
	HI_U8  chipCode[2];//芯片代码
	//--- 站点版本信息 END ---
	
	HI_U16  Hard_reset_cnt;//硬复位累积次数
	HI_U16  soft_reset_cnt;//软复位累积次数
	HI_U32  proxy_type:8;//代理类型
	HI_U32  pad8:24;//保留
	HI_U32  End_to_end_serial_number;//端到端序列号
	HI_U8  manage_msg_id[24];//管理ID信息
}MME_ASSOC_REQ;

//关联确认报文
typedef struct
{
	HI_U8  mac[6];//站点MAC地址
	HI_U8  cco_mac[6];//CCO MAC地址
	
	HI_U16  result:8;//结果
	HI_U16  level:8;//站点层级
	HI_U16 tei :12;
	HI_U16 pad1 :4;
	
	HI_U16 proxy_tei :12;
	HI_U16 pad2 :4;
	HI_U16  total_subcontract_cnt:8;
	HI_U16  subcontract_number:8;
	
	HI_U32  associate_random;
	HI_U32  reassociate_time;
	HI_U32  End_to_end_serial_number;
	HI_U32  location_num;
	HI_U32  pad3;
	
	//路由表信息
	HI_U16  direct_station_num;//直连站点数
	HI_U16  direct_proxy_num;//直连代理数
	HI_U16  route_table_size;//路由表大小
	HI_U16  pad4;
	
	HI_U8  router_table[1]; // 路由表, N个8byte集合[站点TEI1(12b)、保留(4b)]
}MME_ASSOC_CNF;

//关联汇总指示报文
typedef struct
{
	HI_U8  result;
	HI_U8  level;
	HI_U8  cco_mac[6];
	
	HI_U16 proxy_tei :12;
	HI_U16 pad1 :4;
	HI_U16  pad2:8;
	HI_U16  sta_num:8;
	
	HI_U32  pad3;
	
	HI_U8  station_info[1]; // 站点信息， N个8byte集合[站点MAC地址1(6byte)、站点TEI1(12b)、保留(4b)]
}MME_ASSOC_GATHER_IND;

//代理变更请求报文
typedef struct
{
	HI_U16 tei :12;
	HI_U16 pad :4;
	HI_U16 new_proxy_tei0 :12;
	HI_U16 pad0 :4;
	
	HI_U16 new_proxy_tei1 :12;
	HI_U16 pad1 :4;
	HI_U16 new_proxy_tei2 :12;
	HI_U16 pad2 :4;
	
	HI_U16 new_proxy_tei3 :12;
	HI_U16 pad3 :4;
	HI_U16 new_proxy_tei4 :12;
	HI_U16 pad4 :4;
	
	HI_U16 old_proxy_tei :12;
	HI_U16 pad5 :4;
	HI_U16  proxy_type:8;
	HI_U16  reason:8;
	
	HI_U32  End_to_end_serial_number;
	
	HI_U8  sta_phase0 :2;
	HI_U8  sta_phase1 :2;
	HI_U8  sta_phase2 :2;
	HI_U8  pad6 :2;
	HI_U8  pad7[3];
}MME_CHANGE_PROXY_REQ;

//代理变更请求确认报文
typedef struct
{
	HI_U8  result;//结果
	HI_U8  total_packet_num;//总分包数
	HI_U8  pk_index;//分包序号
	HI_U8  pad;
	
	HI_U16 tei :12;//站点TEI
	HI_U16 pad1 :4;
	HI_U16 proxy_tei :12;//代理TEI
	HI_U16 pad2 :4;
	
	HI_U32  End_to_end_serial_number;
	HI_U32  path_sequence;
	
	HI_U16  child_sum;
	HI_U16  pad3;
	
	HI_U8  child_entry[1]; // 子站点条目, N个2byte集合[站点TEI1(12b)、保留(4b)]
}MME_CHANGE_PROXY_CNF;

//代理变更请求确认报文位图版
typedef struct
{
	HI_U8  result;
	HI_U8  pad;
	HI_U16  bitmapsize;
	
	HI_U16 tei :12;
	HI_U16 pad0 :4;
	HI_U16 proxy_tei :12;
	HI_U16 pad2 :4;
	
	HI_U32  End_to_end_serial_number;
	HI_U32  path_sequence;
	HI_U32  pad3;
	
	HI_U8  child_bitmap_entry[1]; // 子站点位图信息,可变长
}MME_CHANGE_PROXY_BITMAP_CNF;

//离线指示报文
typedef struct
{
	HI_U16  reason;
	HI_U16  sta_num;
	
	HI_U16  delay_time;//延迟时间
	HI_U16  pad1;
	
	HI_U8  pad2[8];
	
	HI_U8  sta_mac_entry[1]; // 站点MAC地址, N个6byte集合[站点MAC地址1(6byte)]
}MME_LEAVE_IND;

//心跳报文
typedef struct
{
	HI_U16 stei :12;
	HI_U16 pad :4;
	HI_U16 MaxDiscTei :12;
	HI_U16 pad1 :4;
	
	HI_U16  MaxDiscNum;
	HI_U16  bitMapSize;
	
	HI_U8  bitMap[1]; // 发现站点位图,可变长
}MME_HEART_BEAT_CHECK;

//发现列表报文
typedef struct
{
	HI_U32 tei : 12;
	HI_U32 proxy_tei : 12;
	HI_U32 role : 4;
	HI_U32 level : 4;
	
	HI_U8  mac[6];
	HI_U8  cco_mac[6];
	
	HI_U8  sta_phase1 : 2;
	HI_U8  sta_phase2 : 2;
	HI_U8  sta_phase3 : 2;
	HI_U8  pad1 : 2;
	HI_U8  proxy_channel_quality;
	HI_U8  proxy_route_succ_rate;
	HI_U8  proxy_route_down_succ_rate;
	
	HI_U16  sta_num;
	HI_U16  discover_node_list_frame_num:8;
	HI_U16  up_route_num:8;
	
	HI_U16  rt_cco_cycle_timeout;
	HI_U16  bitmapsize;
	
	HI_U8  lowest_succ_rate;
	HI_U8  pad2[3];
	
	HI_U8  up_route_info[1];//  上行路由条目信息, N个2byte集合[站点TEI(12b)、路由类型(4b)]
							//  发现站点列表位图,
							//  接收发现列表信息,三个可变长结构  N个1byte集合[接收发现列表数(1byte)]
}MME_DISCOVER_NODE_LIST;

//通信成功率上报报文
typedef struct
{
	HI_U16 tei :12;
	HI_U16 pad :4;
	HI_U16  StaNum;
	
	HI_U8  successRateInfo[1]; // 通信成功率信息, N个3byte集合[站点TEI(12b)、保留(4b)、下行通信成功率(1byte)、上行通信成功率(1byte)]
}MME_SUCCESSRATE_REPORT;

typedef struct
{
	HI_U16 statei :12;
	HI_U16 pad :4;
	HI_U16  DownCommRate:8;
	HI_U16  UpCommRate:8;
}STA_SUCCESSRATE_INFO;

//网络冲突上报报文
typedef struct
{
	HI_U8  mac[6];
	HI_U8 neighborNetCnt;
	HI_U8  neighborNetWidth;
	
	HI_U8  neighborNetContent[1]; // 邻居网络条目,N个3byte集合[邻居网络(3byte)]
}MME_NETWORK_CONFLICT_REP;

//过零NTB采集指示报文
typedef struct
{
	HI_U16 tei :12;
	HI_U16 pad :4;
	HI_U16  collect_mode:8;
	HI_U16  collect_cycle:8;
	
	HI_U8  collect_num;
	HI_U8  pad1[3];
}MME_ZEROCROSS_NTB_CLET_IND;

//过零NTB上报报文
typedef struct
{
	HI_U16 tei :12;
	HI_U16 pad :4;
	HI_U16  ntb_num:8;
	HI_U16  phase1_num:8;
	
	HI_U8  phase2_num;
	HI_U8  phase3_num;
	HI_U8  std_ntb[4];   //无法四字节对齐
	HI_U8  ntb_deviation[1]; // 相线1过零NTB差值 N个12b集合[过零NTB差值(12b)]
							 //	相线2过零NTB差值 N个12b集合[过零NTB差值(12b)]
							 //	相线3过零NTB差值 N个12b集合[过零NTB差值(12b)]
}MME_ZEROCROSS_NTB_REP;

//网络诊断报文
typedef struct
{
	HI_U8  manuId[2];
	HI_U8  manudata[1]; // 厂家自定义,可变长
}MME_DIAGNOSE;

//路由请求报文
typedef struct
{
	HI_U8  ver;
	HI_U8  route_request_serial_num[4];
	
	HI_U8  pad1 :3;
	HI_U8  route_preference_sign :1;
	HI_U8  load_data_type :4;
	HI_U8  load_data_lenth;
	HI_U8  load_data[1]; // 负载数据,N个4byte集合[站点TEI(12b)、保留(4b)、通信成功率(1byte)、信道质量(1byte)]
}MME_ROUTE_REQUEST;

//路由回复报文
typedef struct
{
	HI_U8  ver;
	HI_U8  route_request_serial_num[4];
	HI_U8  pad1 :4;
	HI_U8  load_data_type :4;
	HI_U8  load_data_lenth;
	HI_U8  load_data[1]; // 负载数据,N个4byte集合[站点TEI(12b)、保留(4b)、通信成功率(1byte)、信道质量(1byte)]
}MME_ROUTE_REPLY;

//路由错误报文
typedef struct
{
	HI_U8  ver;
	HI_U8  route_request_serial_num[4];
	HI_U8  pad1;
	HI_U8  cant_arrive_station_quantity;
	HI_U8  confirm_station_list[1]; // 不可达站点列表,N个2byte集合[站点TEI(12b)、保留(4b)]?
}MME_ROUTE_ERROR;

//路由应答报文
typedef struct
{
	HI_U8  ver;
	HI_U8  pad[3];
	HI_U32  route_request_serial_num;
}MME_ROUTE_ACK;

//链路确认请求报文
typedef struct
{
	HI_U8  ver;
	HI_U8  route_request_serial_num[4];
	HI_U8  pad1;
	HI_U8  confirm_station_num;
	HI_U8  confirm_station_list[1]; // 确认站点列表,N个2byte集合[站点TEI(12b)、保留(4b)]?
}MME_LINK_CONFIRM_REQUEST;

//链路确认回应报文
typedef struct
{
	HI_U8  ver;
	HI_U8  level;
	HI_U8  channel_quality;
	HI_U8  route_preference_sign :1;
	HI_U8  pad1 :7;
	HI_U32  route_request_serial_num;
}MME_LINK_CONFIRM_RESPONSE;

//通用报文
typedef struct
{
	HI_U32  datagram_port:8;
	HI_U32  datagram_ID:16;
	HI_U32  datagram_ctrl:8;
}APP_HEAD;

//抄表报文下行帧信息
typedef struct
{
	HI_U16 app_ver :6;
	HI_U16 frame_head_len :6;
	HI_U16 setmode :4;
	HI_U16 transdata_type :4;
	HI_U16 transdatalen :12;
	
	HI_U16  datagram_sequenc;
	HI_U16  dev_timeout:8;
	HI_U16  option:8;
	
	HI_U8  data[1]; // 数据，645_07、645_97、698.45
}APP_METERREAD_DOWN_INFO;

//抄表报文上行帧信息
typedef struct
{
	HI_U16 app_ver :6;
	HI_U16 frame_head_len :6;
	HI_U16 resp_state :4;
	HI_U16 transdata_type :4;
	HI_U16 transdatalen :12;
	
	HI_U16  datagram_sequenc;
	HI_U16  option;
	
	HI_U8  data[1]; // 数据，645_07、645_97、698.45
}APP_METERREAD_UP_INFO;

//启动节点注册下行报文
typedef struct
{
	HI_U16 app_ver :6;
	HI_U16 frame_head_len :6;
	HI_U16 force_resp_flag :1;
	HI_U16 nodereg_para :3;
	HI_U16  pad;
	
	HI_U32  datagram_sequenc;
}APP_START_NODE_REG_DOWN;

//查询节点注册下行报文
typedef struct
{
	HI_U16 app_ver :6;
	HI_U16 frame_head_len :6;
	HI_U16 force_resp_flag :1;
	HI_U16 nodereg_para :3;
	HI_U16  pad;
	
	HI_U32  datagram_sequenc;
	
	HI_U8  src_mac[6];
	HI_U8  dest_mac[6];
	
}APP_QUERY_NODE_REG_DOWN;

//查询节点注册上行报文
typedef struct
{
	HI_U16 app_ver :6;
	HI_U16 frame_head_len :6;
	HI_U16 force_resp_flag :1;
	HI_U16 nodereg_para :3;
	HI_U16  elecmetercnt:8;
	HI_U16  product_type:8;
	
	HI_U8  dev_add[6];
	HI_U8  dev_id[6];
	
	HI_U32  datagram_sequenc;
	HI_U32  pad0;
	
	HI_U8  src_mac[6];
	HI_U8  dest_mac[6];
	
	//数据
	HI_U8 data[1];
	//HI_U8  elecmeter_add[6];
	//HI_U8  rule_type;
	//HI_U8  module_type :4;
	//HI_U8  pad1 :4;
	
}APP_QUERY_NODE_REG_UP;

//停止节点注册下行报文
typedef struct
{
	HI_U16 app_ver :6;
	HI_U16 frame_head_len :6;
	HI_U16 pad0 :4;
	HI_U16  pad1;
	
	HI_U32  datagram_sequenc;
}APP_STOP_NODE_REG_DOWN;

//校时下行报文
typedef struct
{
	HI_U16 app_ver :6;
	HI_U16 frame_head_len :6;
	HI_U16 pad0 :4;
	HI_U16 pad1 :4;
	HI_U16 datalen :12;
	HI_U8  data[1]; // 数据，可变长	
}APP_CHECK_TIME_DOWN;

//事件报文
typedef struct
{
	HI_U32 app_ver :6;
	HI_U32 frame_head_len :6;
	HI_U32 direction :1;
	HI_U32 start :1;
	HI_U32 function :6;
	HI_U32 transdatalen :12;
	
	HI_U8  datagram_sequenc[2];
	HI_U8  elecmeter_add[6];
	HI_U8  data[1]; // 数据，可变长	
}APP_EVENT_INFO;

//通信测试命令报文
typedef struct
{
	HI_U16 app_ver :6;
	HI_U16 frame_head_len :6;
	HI_U16 pad_testmode :4;
	HI_U16 transdata_type :4;
	HI_U16 transdatalen_testmodetime :12;
	HI_U8  data[1]; // 数据，可变长	
}APP_COMMUNICATE_TEST_CMD;

//确认/否认报文====********
typedef struct
{
	HI_U16 app_ver :6;
	HI_U16 frame_head_len :6;
	HI_U16 direction :1;
	HI_U16 confirm :1;
	HI_U16 pad0 :2;
	HI_U16  datagram_sequenc;
	HI_U16  option;
	//HI_U8  pad1[2];
}APP_CONFIRM_OR_NOT;

//开始升级下行报文
typedef struct
{
	HI_U32 app_ver :6;
	HI_U32 frame_head_len :6;
	HI_U32 pad0 :20;
	
	HI_U32 upgrade_id;
	
	HI_U16 upgrade_timewin;
	HI_U16 block_size;
	
	HI_U32 file_size;
	HI_U32 crc;
}APP_START_UPGRADE_DOWN;

//停止升级下行报文
typedef struct
{
	HI_U32 app_ver :6;
	HI_U32 frame_head_len :6;
	HI_U32 pad0 :20;
	
	HI_U32 upgrade_id;
}APP_STOP_UPGRADE_DOWN;

//传输文件数据下行报文
typedef struct
{
	HI_U16 app_ver :6;
	HI_U16 frame_head_len :6;
	HI_U16 pad0 :4;
	HI_U16 block_size;
	
	HI_U32 upgrade_id;
	HI_U32 data_num;
	
	HI_U8  data[1]; // 数据，可变长	
}APP_TRANS_FILEDATA_DOWN;

//查询站点升级状态下行报文
typedef struct
{
	HI_U16 app_ver :6;
	HI_U16 frame_head_len :6;
	HI_U16 pad0 :4;
	HI_U16 block_cnt;
	
	HI_U32 start_block_num;
	HI_U32 upgrade_id;
}APP_QUERY_STATION_UPGRADESTATE_DOWN;

//执行升级下行报文
typedef struct
{
	HI_U16 app_ver :6;
	HI_U16 frame_head_len :6;
	HI_U16 pad0 :4;
	HI_U16 wait_reset_time;
	
	HI_U32 upgrade_id;
	HI_U32 test_runtime;
}APP_EXE_UPGRADE_DOWN;

//查询站点信息下行报文
typedef struct
{
	HI_U32 app_ver :6;
	HI_U32 frame_head_len :6;
	HI_U32 pad0 :12;
	HI_U32  list_elem_cnt:8;
	
	HI_U8  data[1]; // 数据，可变长	
}APP_QUERY_STATION_INFO_DOWN;

//开始升级上行报文
typedef struct
{
	HI_U32 app_ver :6;
	HI_U32 frame_head_len :6;
	HI_U32 pad0 :12;
	//HI_U8  pad1;
	HI_U32  result:8;
	HI_U32 upgrade_id;
}APP_START_UPGRADE_UP;

//查询站点升级状态上行报文
typedef struct
{
	HI_U16 app_ver :6;
	HI_U16 frame_head_len :6;
	HI_U16 upgrade_state :4;
	HI_U16 valid_block_cnt;
	
	HI_U32 start_block_num;
	HI_U32 upgrade_id;
	
	HI_U8  data[1]; // 数据，可变长	
}APP_QUERY_STATION_UPGRADESTATE_UP;

//查询站点信息上行报文
typedef struct
{
	HI_U32 app_ver :6;
	HI_U32 frame_head_len :6;
	HI_U32 pad0 :12;
	HI_U32  list_elem_cnt:8;
	
	HI_U32 upgrade_id;
	HI_U8  data[1]; // 数据，可变长	
}APP_QUERY_STATION_INFO_UP;

//台区户变关系识别报文  ****
typedef struct
{
	HI_U16 app_ver :6;
	HI_U16 frame_head_len :6;
	HI_U16 direction :1;
	HI_U16 start :1;
	HI_U16 collect_phase :2;
	HI_U16  datagram_sequenc;
	HI_U8  mac[6];
	HI_U8  feature_type;
	HI_U8  collect_type;
	HI_U8  data[1]; // 数据，可变长	
}APP_FAMILY_CHANGE_RELATION_ID;

//鉴权安全报文  ****
typedef struct
{
	HI_U8  data[1]; // 数据，可变长	
}APP_AUTHENTICATION_SECURITY;

HI_END_HEADER
#endif //__DRV_DFX_H__


