//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2018, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : dfx_hso.h
//  �� �� ��   : V1.0
//  ��    ��   : wangchao/00230251
//  ��������   : 2018-12-13
//  ��������   : HSO plc parser ����ͷ�ļ�
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2018-12-13
//    ��    �� : w00230251
//    �޸����� : �����ļ�
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
// �ű�֡��֡�غɣ��̶����֣�
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



/////////////////beacon վ��������Ŀ info///////////////////////////////
#define STATION_INFO_ENTRY_LEN 13
//վ��������Ŀ
typedef struct
{
	HI_U32 tei:12;  	// վ��TEI
	HI_U32 proxy_tei:12;  	// ����TEI
	HI_U32 route_lowest_succ_rate:8;  		// ·�����ͨ�ųɹ���
    HI_U8 mac[6];  			// �����ű�վ��MAC��ַ
	HI_U8 role:4;  			// ��ɫ
	HI_U8 level:4	;  			// �㼶
	HI_U8 proxy_channel_quality;  	// ����վ���ŵ�����
	HI_U8 phase:2;  				// ����
	HI_U8  pad2:6;
	HI_U8  pad3[3];    //***********************��һ��
}STATION_INFO_ENTRY;

//////////////////////////////·�ɲ���֪ͨ��Ŀ/////////////////////////////////
#define ROUTE_PARAM_ENTRY_LEN 8
typedef struct
{
	//HI_U16 	station_entry_head;						// �ű���Ŀͷ������δ����
	//HI_U16 	station_entry_len;						// �ű���Ŀ���ȣ�����δ����

	HI_U16  rt_evaluate_cycle_time_len;				// ·������
	HI_U16 rt_cco_evaluate_timeout;					// ·������ʣ��ʱ��
	HI_U16 proxy_station_discovery_list_period;		// ����վ�㷢���б�����
	HI_U16 discovery_station_discovery_list_period;	// ����վ�㷢���б�����
}ROUTE_PARAM;


//////////////////////////////Ƶ��֪ͨ��Ŀ/////////////////////////////////
#define FREQ_NOTIFY_ENTRY_LEN 5
typedef struct
{
	HI_U32	dest_freq;						// Ŀ��Ƶ��    ***********************��һ��
	HI_U32	freq_chg_time;					// Ƶ���л�ʣ��ʱ��
}FREQ_NOTIFY;

#define BEACON_SLOT_FIXED_LEN  8
typedef struct
{
	HI_U8	non_center_beacon_slot_cnt;		// �������ű�ʱ϶����
	HI_U8	center_beacon_slot_cnt:4;		// �����ű�ʱ϶����
	HI_U8	csma_slot_phase_cnt:2;//CSMAʱ϶֧�ֵ����߸���
	HI_U8   pad1_l:2;//����
	HI_U8   pad1_h;//����
	HI_U8   proxy_beacon_slot_cnt;//�����ű�ʱ϶����

	HI_U8   beacon_slot_len;//�ű�ʱ϶����
	HI_U8   csma_slot_Slice_len;//CSMAʱ϶��Ƭ����
	HI_U8   bind_csma_slot_count;// ��CSMAʱ϶���߸���
	HI_U8   bind_csma_slot_lid;// ��CSMAʱ϶��·��ʶ��
}BEACON_SLOT_FIXED_INFO;

#define MAX_UNKNOW_BEACON_BUFFER_LEN    200
typedef struct
{
	BEACON_SLOT_FIXED_INFO beacon_slot_fixed;

	HI_U16   tdma_slot_len;// TDMAʱ϶����                ***********************��һ��
	HI_U16   tdma_slot_lid;// TDMAʱ϶��·��ʶ��          ***********************��һ��

	HI_U32  beacon_start_ntb;// �ű�������ʼ�����׼ʱ
	HI_U32  beacon_period_len;// �ű����ڳ���

	HI_U8  data[MAX_UNKNOW_BEACON_BUFFER_LEN];
}BEACON_SLOT_INFO;


//�ֽڶ�����(fixed)(8) + TDMAʱ϶����(1) + TDMAʱ϶��·��ʶ��(1) + �ű�������ʼ�����׼ʱ(4) + �ű����ڳ���(4) + ����(2)

#define SLOT_VARIETY_POS_IN_BEACON_SLOT    (20)
typedef struct  
{
	HI_U8  pad3;//Э������      ***********************��һ��

	HI_U8 station_entry_cnt;
	HI_U8 station_entry_head;	// �ű���Ŀͷ
	HI_U8 station_entry_len;  	// �ű���Ŀ����
	STATION_INFO_ENTRY station_entry;   //վ��������Ŀ

	HI_U32 	route_para_notify_head:16;						// �ű���Ŀͷ������δ����
	HI_U32 	route_para_notify_len:16;						// �ű���Ŀ���ȣ�����δ����
	ROUTE_PARAM  route_para_notify_entry;				//·�ɲ���֪ͨ��Ŀ

	HI_U32 	freq_head:16;						// �ű���Ŀͷ������δ����
	HI_U32 	freq_len:16;						// �ű���Ŀ���ȣ�����δ����
	FREQ_NOTIFY freq_entry;

	HI_U32 	beacon_slot_head:16;						// �ű���Ŀͷ������δ����
	HI_U32 	beacon_slot_len:16;						// �ű���Ŀ���ȣ�����δ����
	BEACON_SLOT_INFO beacon_slot_entry;

	HI_U32 	unknow_head:16;						// �ű���Ŀͷ������δ����
	HI_U32 	unknow_len:16;						// �ű���Ŀ���ȣ�����δ����
	HI_U8 unknow_buffer[MAX_UNKNOW_BEACON_BUFFER_LEN];

	//Ƶ�α����Ŀ
}MPDU_TYPE_BEACON_STATION_ENTRY;


typedef struct  
{
	MPDU_TYPE_BEACON_PB_FIXED beacon_pb_fixed;
	MPDU_TYPE_BEACON_STATION_ENTRY beacon_station_entry;
}MPDU_TYPE_BEACON_PB;
///////////////////////////////////////////beacon_pb//////////////////////////////////////////////////////////







// ACK֡
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

// ����Э��֡
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
	
	HI_U32 frame_len:12;//֡��
	HI_U32 pbcnt:4;
	HI_U32 symbol_num:9;
	HI_U32 bcf:1;//�㲥��־λ��ȡֵΪ0;
	HI_U32 resend_flag:1;//�ش���־λ
	HI_U32 encrypt_sign:1;  //���ܱ�־λ
	HI_U32 tmi:4;   //�ּ���������ģʽ;
	
	HI_U32 extend_tmi:4;	//�ּ�������չģʽ;	
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


//����������
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
	HI_U8  dev_type;//�豸����
	HI_U8  mac_type;//MAC��ַ����
	HI_U8  pad6;
	
	HI_U32  associate_random;//վ����������
	 
	//HI_U8 user_defined[18]; //--- �����Զ�����Ϣ ---HI_SYS_STATUS_INFO_S
   //--- �Զ��岿�� START ---
	HI_U8  system_mode;
	HI_U8  boot_ver_idx;
	HI_U8  system_abnormal_reason;
	HI_U8  main_ver[3];
	HI_U8  build_time[6];

	HI_U16 second_ver[2];

	HI_U16  chip_ver;
	//--- �Զ��岿�� END ---
	
	//
    //--- վ��汾��Ϣ BEGIN ---
	HI_U16  system_start_reason:8;//ϵͳ����ԭ��
	HI_U16  boot_ver:8;//BOOT�汾��
	HI_U16  sw_ver;//����汾��
	HI_U16 usYear :7;//�汾ʱ��
	HI_U16 usMonth :4;
	HI_U16 usDay :5;
	HI_U8  manuCode[2];//���̴���
	HI_U8  chipCode[2];//оƬ����
	//--- վ��汾��Ϣ END ---
	
	HI_U16  Hard_reset_cnt;//Ӳ��λ�ۻ�����
	HI_U16  soft_reset_cnt;//��λ�ۻ�����
	HI_U32  proxy_type:8;//��������
	HI_U32  pad8:24;//����
	HI_U32  End_to_end_serial_number;//�˵������к�
	HI_U8  manage_msg_id[24];//����ID��Ϣ
}MME_ASSOC_REQ;

//����ȷ�ϱ���
typedef struct
{
	HI_U8  mac[6];//վ��MAC��ַ
	HI_U8  cco_mac[6];//CCO MAC��ַ
	
	HI_U16  result:8;//���
	HI_U16  level:8;//վ��㼶
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
	
	//·�ɱ���Ϣ
	HI_U16  direct_station_num;//ֱ��վ����
	HI_U16  direct_proxy_num;//ֱ��������
	HI_U16  route_table_size;//·�ɱ��С
	HI_U16  pad4;
	
	HI_U8  router_table[1]; // ·�ɱ�, N��8byte����[վ��TEI1(12b)������(4b)]
}MME_ASSOC_CNF;

//��������ָʾ����
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
	
	HI_U8  station_info[1]; // վ����Ϣ�� N��8byte����[վ��MAC��ַ1(6byte)��վ��TEI1(12b)������(4b)]
}MME_ASSOC_GATHER_IND;

//������������
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

//����������ȷ�ϱ���
typedef struct
{
	HI_U8  result;//���
	HI_U8  total_packet_num;//�ְܷ���
	HI_U8  pk_index;//�ְ����
	HI_U8  pad;
	
	HI_U16 tei :12;//վ��TEI
	HI_U16 pad1 :4;
	HI_U16 proxy_tei :12;//����TEI
	HI_U16 pad2 :4;
	
	HI_U32  End_to_end_serial_number;
	HI_U32  path_sequence;
	
	HI_U16  child_sum;
	HI_U16  pad3;
	
	HI_U8  child_entry[1]; // ��վ����Ŀ, N��2byte����[վ��TEI1(12b)������(4b)]
}MME_CHANGE_PROXY_CNF;

//����������ȷ�ϱ���λͼ��
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
	
	HI_U8  child_bitmap_entry[1]; // ��վ��λͼ��Ϣ,�ɱ䳤
}MME_CHANGE_PROXY_BITMAP_CNF;

//����ָʾ����
typedef struct
{
	HI_U16  reason;
	HI_U16  sta_num;
	
	HI_U16  delay_time;//�ӳ�ʱ��
	HI_U16  pad1;
	
	HI_U8  pad2[8];
	
	HI_U8  sta_mac_entry[1]; // վ��MAC��ַ, N��6byte����[վ��MAC��ַ1(6byte)]
}MME_LEAVE_IND;

//��������
typedef struct
{
	HI_U16 stei :12;
	HI_U16 pad :4;
	HI_U16 MaxDiscTei :12;
	HI_U16 pad1 :4;
	
	HI_U16  MaxDiscNum;
	HI_U16  bitMapSize;
	
	HI_U8  bitMap[1]; // ����վ��λͼ,�ɱ䳤
}MME_HEART_BEAT_CHECK;

//�����б���
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
	
	HI_U8  up_route_info[1];//  ����·����Ŀ��Ϣ, N��2byte����[վ��TEI(12b)��·������(4b)]
							//  ����վ���б�λͼ,
							//  ���շ����б���Ϣ,�����ɱ䳤�ṹ  N��1byte����[���շ����б���(1byte)]
}MME_DISCOVER_NODE_LIST;

//ͨ�ųɹ����ϱ�����
typedef struct
{
	HI_U16 tei :12;
	HI_U16 pad :4;
	HI_U16  StaNum;
	
	HI_U8  successRateInfo[1]; // ͨ�ųɹ�����Ϣ, N��3byte����[վ��TEI(12b)������(4b)������ͨ�ųɹ���(1byte)������ͨ�ųɹ���(1byte)]
}MME_SUCCESSRATE_REPORT;

typedef struct
{
	HI_U16 statei :12;
	HI_U16 pad :4;
	HI_U16  DownCommRate:8;
	HI_U16  UpCommRate:8;
}STA_SUCCESSRATE_INFO;

//�����ͻ�ϱ�����
typedef struct
{
	HI_U8  mac[6];
	HI_U8 neighborNetCnt;
	HI_U8  neighborNetWidth;
	
	HI_U8  neighborNetContent[1]; // �ھ�������Ŀ,N��3byte����[�ھ�����(3byte)]
}MME_NETWORK_CONFLICT_REP;

//����NTB�ɼ�ָʾ����
typedef struct
{
	HI_U16 tei :12;
	HI_U16 pad :4;
	HI_U16  collect_mode:8;
	HI_U16  collect_cycle:8;
	
	HI_U8  collect_num;
	HI_U8  pad1[3];
}MME_ZEROCROSS_NTB_CLET_IND;

//����NTB�ϱ�����
typedef struct
{
	HI_U16 tei :12;
	HI_U16 pad :4;
	HI_U16  ntb_num:8;
	HI_U16  phase1_num:8;
	
	HI_U8  phase2_num;
	HI_U8  phase3_num;
	HI_U8  std_ntb[4];   //�޷����ֽڶ���
	HI_U8  ntb_deviation[1]; // ����1����NTB��ֵ N��12b����[����NTB��ֵ(12b)]
							 //	����2����NTB��ֵ N��12b����[����NTB��ֵ(12b)]
							 //	����3����NTB��ֵ N��12b����[����NTB��ֵ(12b)]
}MME_ZEROCROSS_NTB_REP;

//������ϱ���
typedef struct
{
	HI_U8  manuId[2];
	HI_U8  manudata[1]; // �����Զ���,�ɱ䳤
}MME_DIAGNOSE;

//·��������
typedef struct
{
	HI_U8  ver;
	HI_U8  route_request_serial_num[4];
	
	HI_U8  pad1 :3;
	HI_U8  route_preference_sign :1;
	HI_U8  load_data_type :4;
	HI_U8  load_data_lenth;
	HI_U8  load_data[1]; // ��������,N��4byte����[վ��TEI(12b)������(4b)��ͨ�ųɹ���(1byte)���ŵ�����(1byte)]
}MME_ROUTE_REQUEST;

//·�ɻظ�����
typedef struct
{
	HI_U8  ver;
	HI_U8  route_request_serial_num[4];
	HI_U8  pad1 :4;
	HI_U8  load_data_type :4;
	HI_U8  load_data_lenth;
	HI_U8  load_data[1]; // ��������,N��4byte����[վ��TEI(12b)������(4b)��ͨ�ųɹ���(1byte)���ŵ�����(1byte)]
}MME_ROUTE_REPLY;

//·�ɴ�����
typedef struct
{
	HI_U8  ver;
	HI_U8  route_request_serial_num[4];
	HI_U8  pad1;
	HI_U8  cant_arrive_station_quantity;
	HI_U8  confirm_station_list[1]; // ���ɴ�վ���б�,N��2byte����[վ��TEI(12b)������(4b)]?
}MME_ROUTE_ERROR;

//·��Ӧ����
typedef struct
{
	HI_U8  ver;
	HI_U8  pad[3];
	HI_U32  route_request_serial_num;
}MME_ROUTE_ACK;

//��·ȷ��������
typedef struct
{
	HI_U8  ver;
	HI_U8  route_request_serial_num[4];
	HI_U8  pad1;
	HI_U8  confirm_station_num;
	HI_U8  confirm_station_list[1]; // ȷ��վ���б�,N��2byte����[վ��TEI(12b)������(4b)]?
}MME_LINK_CONFIRM_REQUEST;

//��·ȷ�ϻ�Ӧ����
typedef struct
{
	HI_U8  ver;
	HI_U8  level;
	HI_U8  channel_quality;
	HI_U8  route_preference_sign :1;
	HI_U8  pad1 :7;
	HI_U32  route_request_serial_num;
}MME_LINK_CONFIRM_RESPONSE;

//ͨ�ñ���
typedef struct
{
	HI_U32  datagram_port:8;
	HI_U32  datagram_ID:16;
	HI_U32  datagram_ctrl:8;
}APP_HEAD;

//����������֡��Ϣ
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
	
	HI_U8  data[1]; // ���ݣ�645_07��645_97��698.45
}APP_METERREAD_DOWN_INFO;

//����������֡��Ϣ
typedef struct
{
	HI_U16 app_ver :6;
	HI_U16 frame_head_len :6;
	HI_U16 resp_state :4;
	HI_U16 transdata_type :4;
	HI_U16 transdatalen :12;
	
	HI_U16  datagram_sequenc;
	HI_U16  option;
	
	HI_U8  data[1]; // ���ݣ�645_07��645_97��698.45
}APP_METERREAD_UP_INFO;

//�����ڵ�ע�����б���
typedef struct
{
	HI_U16 app_ver :6;
	HI_U16 frame_head_len :6;
	HI_U16 force_resp_flag :1;
	HI_U16 nodereg_para :3;
	HI_U16  pad;
	
	HI_U32  datagram_sequenc;
}APP_START_NODE_REG_DOWN;

//��ѯ�ڵ�ע�����б���
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

//��ѯ�ڵ�ע�����б���
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
	
	//����
	HI_U8 data[1];
	//HI_U8  elecmeter_add[6];
	//HI_U8  rule_type;
	//HI_U8  module_type :4;
	//HI_U8  pad1 :4;
	
}APP_QUERY_NODE_REG_UP;

//ֹͣ�ڵ�ע�����б���
typedef struct
{
	HI_U16 app_ver :6;
	HI_U16 frame_head_len :6;
	HI_U16 pad0 :4;
	HI_U16  pad1;
	
	HI_U32  datagram_sequenc;
}APP_STOP_NODE_REG_DOWN;

//Уʱ���б���
typedef struct
{
	HI_U16 app_ver :6;
	HI_U16 frame_head_len :6;
	HI_U16 pad0 :4;
	HI_U16 pad1 :4;
	HI_U16 datalen :12;
	HI_U8  data[1]; // ���ݣ��ɱ䳤	
}APP_CHECK_TIME_DOWN;

//�¼�����
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
	HI_U8  data[1]; // ���ݣ��ɱ䳤	
}APP_EVENT_INFO;

//ͨ�Ų��������
typedef struct
{
	HI_U16 app_ver :6;
	HI_U16 frame_head_len :6;
	HI_U16 pad_testmode :4;
	HI_U16 transdata_type :4;
	HI_U16 transdatalen_testmodetime :12;
	HI_U8  data[1]; // ���ݣ��ɱ䳤	
}APP_COMMUNICATE_TEST_CMD;

//ȷ��/���ϱ���====********
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

//��ʼ�������б���
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

//ֹͣ�������б���
typedef struct
{
	HI_U32 app_ver :6;
	HI_U32 frame_head_len :6;
	HI_U32 pad0 :20;
	
	HI_U32 upgrade_id;
}APP_STOP_UPGRADE_DOWN;

//�����ļ��������б���
typedef struct
{
	HI_U16 app_ver :6;
	HI_U16 frame_head_len :6;
	HI_U16 pad0 :4;
	HI_U16 block_size;
	
	HI_U32 upgrade_id;
	HI_U32 data_num;
	
	HI_U8  data[1]; // ���ݣ��ɱ䳤	
}APP_TRANS_FILEDATA_DOWN;

//��ѯվ������״̬���б���
typedef struct
{
	HI_U16 app_ver :6;
	HI_U16 frame_head_len :6;
	HI_U16 pad0 :4;
	HI_U16 block_cnt;
	
	HI_U32 start_block_num;
	HI_U32 upgrade_id;
}APP_QUERY_STATION_UPGRADESTATE_DOWN;

//ִ���������б���
typedef struct
{
	HI_U16 app_ver :6;
	HI_U16 frame_head_len :6;
	HI_U16 pad0 :4;
	HI_U16 wait_reset_time;
	
	HI_U32 upgrade_id;
	HI_U32 test_runtime;
}APP_EXE_UPGRADE_DOWN;

//��ѯվ����Ϣ���б���
typedef struct
{
	HI_U32 app_ver :6;
	HI_U32 frame_head_len :6;
	HI_U32 pad0 :12;
	HI_U32  list_elem_cnt:8;
	
	HI_U8  data[1]; // ���ݣ��ɱ䳤	
}APP_QUERY_STATION_INFO_DOWN;

//��ʼ�������б���
typedef struct
{
	HI_U32 app_ver :6;
	HI_U32 frame_head_len :6;
	HI_U32 pad0 :12;
	//HI_U8  pad1;
	HI_U32  result:8;
	HI_U32 upgrade_id;
}APP_START_UPGRADE_UP;

//��ѯվ������״̬���б���
typedef struct
{
	HI_U16 app_ver :6;
	HI_U16 frame_head_len :6;
	HI_U16 upgrade_state :4;
	HI_U16 valid_block_cnt;
	
	HI_U32 start_block_num;
	HI_U32 upgrade_id;
	
	HI_U8  data[1]; // ���ݣ��ɱ䳤	
}APP_QUERY_STATION_UPGRADESTATE_UP;

//��ѯվ����Ϣ���б���
typedef struct
{
	HI_U32 app_ver :6;
	HI_U32 frame_head_len :6;
	HI_U32 pad0 :12;
	HI_U32  list_elem_cnt:8;
	
	HI_U32 upgrade_id;
	HI_U8  data[1]; // ���ݣ��ɱ䳤	
}APP_QUERY_STATION_INFO_UP;

//̨�������ϵʶ����  ****
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
	HI_U8  data[1]; // ���ݣ��ɱ䳤	
}APP_FAMILY_CHANGE_RELATION_ID;

//��Ȩ��ȫ����  ****
typedef struct
{
	HI_U8  data[1]; // ���ݣ��ɱ䳤	
}APP_AUTHENTICATION_SECURITY;

HI_END_HEADER
#endif //__DRV_DFX_H__


