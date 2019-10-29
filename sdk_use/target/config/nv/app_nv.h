//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : app_nv.h
//  �� �� ��   : V1.0
//  ��    ��   : shenhankun/00130424
//  ��������   : 2011-10-26
//  ��������   : TODO: ...
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2011-10-26
//    ��    �� : shenhankun/00130424
//    �޸����� : �����ļ�
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   :
// SUBSYSTEM :
// MODULE    :
// OWNER     :
//*****************************************************************************


#ifndef __APP_NV_H__
#define __APP_NV_H__


#define  ID_NV_APP_MR_CFG               0x0401  //
#define  ID_NV_MRS_CCO_ADDR             0x0402  // �ز����ڵ��ַ
#define  ID_NV_APP_SEARCHMETER_CFG      0x0403  // �ѱ��������
#define  ID_NV_APP_BAUDRATE_CFG         0x0404  // �������������
#define  ID_NV_APP_CTLII_CFG            0x0405  // II�Ͳɼ����������
#define  ID_NV_APP_EVENT_CFG            0x0406  // �¼��������
#define  ID_NV_APP_STATE_CHG_REPORT_CFG 0x0408  // �ϱ��ӽڵ�����״̬�仯��������
#define  ID_NV_APP_SM_RESULT1           0x040A  // �ѱ���ǰ16��
#define  ID_NV_APP_SM_RESULT2           0x040B  // �ѱ�����16��

#define  ID_NV_APP_NVM_DATA_0           0x040C  // ���ݴ洢0
#define  ID_NV_APP_NVM_DATA_1           0x040D  // ���ݴ洢1
#define  ID_NV_APP_NVM_DATA_2           0x040E  // ���ݴ洢2
#define  ID_NV_APP_NVM_DATA_3           0x040F  // ���ݴ洢3

#define  ID_NV_APP_CLT_EVT_INFO1        (ID_NV_APP_NVM_DATA_0)  // �ѱ����е�1--8�����ַ���¼���Ϣ
#define  ID_NV_APP_CLT_EVT_INFO2        (ID_NV_APP_NVM_DATA_1)  // �ѱ����е�9--16�����ַ���¼���Ϣ
#define  ID_NV_APP_CLT_EVT_INFO3        (ID_NV_APP_NVM_DATA_2)  // �ѱ����е�17--24�����ַ���¼���Ϣ
#define  ID_NV_APP_CLT_EVT_INFO4        (ID_NV_APP_NVM_DATA_3)  // �ѱ����е�25--32�����ַ���¼���Ϣ
#define  ID_NV_MRS_STA_RESET_TIME_INF_0 (ID_NV_APP_NVM_DATA_0)  // STAģ���ϵ�ʱ����Ϣ0
#define  ID_NV_MRS_STA_RESET_TIME_INF_1 (ID_NV_APP_NVM_DATA_1)  // STAģ���ϵ�ʱ����Ϣ1

#define  ID_NV_APP_PLC_OVERTIME_INFO    0x0410  // ����ͨѶ��ʱ����
#define  ID_MV_APP_SIMU_METER_INFO      0x0412  //
#define  ID_MV_APP_MAX_PLC_TIMEOUT_NUM  0x0413  // �ڵ�������PLC���ʱ����
#define  ID_NV_APP_MAC_FROM_METER       0x0414  // ʹ�ñ��ַ����Mac��ַ����ʹ�ܿ���
#define  ID_NV_APP_MAX_TURN_NUM         0x0415  // ����ִ����Դ���
#define  ID_NV_APP_BROADCAST_TX         0x0416  // ʹ�ù㲥��ַ��Ӧ�㲥��ַ����ʹ�ܿ���
#define  ID_NV_MRS_RM_CFG               0x0417  // �������������ز���
#define  ID_NV_MRS_DUTY_CYCLE_CFG_INFO  0x0418  // ռ�ձ���ֵ
#define  ID_NV_APP_MR_DUT               0x0421  // ��װִ�й㲥��־
#define  ID_NV_APP_MAC_MAP_INFO         0x0422  // CCO��ӳ���ά����Ϣ
#define  ID_NV_APP_REPORT_METER_INFO    0x0423  // STA�������ϱ����ַ��MAC��ַ��Ϣ
#define  ID_NV_MRS_UPG_PARAM_INFO       0x0426  //Զ��������MRSģ�������Ϣ
#define  ID_NV_APP_CLTI_CFG             0x0427  //I������
#define  ID_NV_MRS_STA_RM_CFG           0x0430  // STA�����������
#define  ID_NV_MRS_CCO_PARAM_ID         0x0431  // Ԥ��
#define  ID_NV_APP_CCO_SM_CFG_EXP       0x0432  // CCO�ѱ����������չ
#define  ID_NV_APP_STA_SM_CFG_EXP       0x0433  // STA�ѱ����������չ
#define  ID_NV_MRS_DC_SD_INFO           0X0434  // SD������������Ϣ
#define  MRS_RETRY_CFG_MAX_NUM          3       // �ش���������
#define  MRS_RM_POOL_MAX_NUM            2       // �����������
#define  MRS_BROADCAST_RM_97_DI_NUM     2       // ȫ���㲥�г���97���DI����

#define NDM_DC_FOLDER_NAME_SIZE 21
// SD��������������
typedef struct
{
    HI_U32  ulRamSize;          // ä�ɿ�ʼ��ַ
    HI_S32  slAgcGain;          // �����ä����Ч
    HI_U16  usTrigerType;       // ��������
    HI_U16  usPeriodTime;       // ä�����ڣ���λ����
    HI_U16  usTrigerTime;       // �೤ʱ���ղ���PB����д�����������λ����
    HI_U16  usNameId;           // ÿ�ΰ����������� +1

    HI_U8   ucTimesPerPeriod;   // ä��ÿ�������������ж��ٴβɼ���
    HI_U8   ucCPowerOnEable;    // �������ɹ���ʹ�ܿ���
    HI_U8   ucCollecting;       // �Ƿ����ڽ��в���
    HI_U8   ucIsFollowMode;     // �Ƿ����

    HI_CHAR scFolderName[NDM_DC_FOLDER_NAME_SIZE];
    HI_U8   reserved1[3];

    HI_U8   scCcoMac[HI_PLC_MAC_ADDR_LEN];  // �����CCO��MAC��ַ
    HI_U8   ucFreq;                         // ���������freq
    HI_U8   ucIsFixFreq;                    // �Ƿ�̶�Ƶ��

    HI_U8   ucSnid;                         // ����SNID
    HI_U8   reserved2[11];
}NV_APP_MRS_NDM_DC_SD_PARAM_STRU;

typedef struct
{
    HI_U8   ucBcmInterval;      // ��λ:��
    HI_U8   ucBcmRetryMax;      // ���Դ���

    HI_U8   ucProtoVer;         // �淶�汾 0-09�� 1-13�� ����-����

    // �ֳ������������ʱʱ�������Դ���
    HI_U8   ucAfn14F1UpRetryMax;//14F1���б��ĳ�ʱ���Դ���(Nv��)
    HI_U16  usAfn14F1UpTimeOut; //14F1���б��ĳ�ʱʱ��(Nv���λms)
    HI_U16  usAfn06F2UpTimeOut; //06F2���б��ĳ�ʱʱ��(Nv���λms)
    HI_U8   ucAfn06F2UpRetryMax;//06F2���б��ĳ�ʱ���Դ���(Nv��)

    HI_U8   ucTestModeSnId_ucCsgParallelEnable;

    // ����STAʹ�� begin
    HI_U8   ucFrameBufItemCount;// ����֡���������
    HI_U8   ucFrameBufTimeout;  // ����֡��ʱʱ�� ��λ:��
    // ����STAʹ�� end

    HI_U16  usTestFrameInterval_usCrcInitValue;// [����STA] ���Ͳ��Ա���ʱ��������λ����
    HI_U8   ucWhitelistDelay;   // CCO�ѱ�����򿪰�������ʱ��ʱ������λ:���ӣ�Ĭ��:0

    HI_U8   ucSendTimeCount;    // �ѱ�����з���ѯ�ʼ�����ʱ���ܴ����ļ���

    HI_U8   plc_rxtx_mode;      // PLCͨ���շ�ģʽ 00:�ȴ�����·���Ż���ɺ�Ž���PLCͨ�����շ�; 01:ǿ��PLCͨ���շ�(���ȴ�PLC״̬);
    HI_U8   get_addr_try_num;   // ��ȡ����ַ���Դ���
    HI_U16  usLowRatio;         // ��ͨ�ʵ͵��ͨ����ֵ�ٷֱȣ�9876��ʾ98.76%
    HI_U32  get_addr_Fail_wait_time;    // ��ȡ����ַʧ�ܺ�ȴ�ʱ�� ��λ:S
    HI_U32  ulSimuChlTimeout;   // ģ��ͨ�����г�ʱʱ�� ��λ:����
    HI_U8   ucCcoFreqEnable;    // CCO�����ȡ��ʱ�ӣ�ʹ�ܿ���Ƶ�ζ�ʱ�Ŀ��أ�0:������,1:����,Ĭ��ֵΪ1

    HI_U8   ucBindNetwork;      // �Ƿ��CCO Network��1��ʾ�󶨣�0��ʾ���󶨣�����ֵδ�á�
    HI_U16  usBindDuration;     // ��CCO Neteork�ĳ���ʱ�䣬��λ�����ӣ����45.5�죻ȡֵ0xFFFFʱ����ʾ����������ʵ������136�꣨U32��ʾ�����������

    HI_U8   ucTestModeEnable;   // [CCO] ����ģʽ����ʹ�ܿ���
    HI_U8   ucTestFrameTimeout; // [CCO] ����ģʽ����Ľ��ճ�ʱ����λ�����ӣ�Ĭ��ֵ��1����

    HI_U8   ucFlashSaveDelay;   // ��������洢flashʱ�� ģʽ 20s
    HI_U8   ucSoftTfMode;       // ����̨��ʶ����ģʽ: 0�رգ�1��(���������أ�վ�������л�����)��2��(���������أ�վ�㲻�����л�����)
}NV_APP_MR_CFG_STRU;

typedef struct
{
    HI_S16 PlcLayerDelayNv;     // �㼶��ʱʱ�����           ��λ:ms
    HI_S16 MeterOverTimeNv;     // ���ʱʱ�����           ��λ:ms
    HI_S16 PlcRevisoryNv;       // CCO��PLC��ʱ����ֵ;        ȡֵΪ0ʱ��ʾ������ ��λ:ms
    HI_S16 CtrlCmdDelayNv;      // ���˲��䳬ʱʱ�����     ��λ:ms
    HI_S16 CltIDelayNv;         // I�Ͳɼ������䳬ʱʱ�����  ��λ:ms
    HI_S16 MeterFrameInterval;  // STA��645֡���ʱ��
    HI_S8 PlcMaxRetryNvS;       // ������Դ�������(����)
    HI_S8 PlcMaxRetryNvB;       // ������Դ�������(�㲥)
    HI_U8 MeterMaxRetryNv;      // ���˳�ʱ���Դ���
    HI_U8 MeterRetryCfgNv;      // �����ش����� Bit0:��Ӧ���ش�; Bit1:����֡�ش�;
    HI_S8 PlcMaxRetryLoop;      // �����������
    HI_U8 MaxServiceTime;       // ���ʱʱ�� ��λ:s
    HI_U8 reserved[6];
}NV_MRS_RETRY_CFG_S;

typedef struct
{
    NV_MRS_RETRY_CFG_S stPlcOverTimeParam[MRS_RETRY_CFG_MAX_NUM];
}NV_APP_PLC_OVERTIME_INF_STRU;  // ID_NV_APP_PLC_OVERTIME_INFO

#define NV_SIMU_METER_DISABLE   0
#define NV_SIMU_METER_ENABLE    1
#define NV_SIMU_METER_AUTO      2

typedef struct
{
    HI_U8 ucMeterFlag;
    HI_U8 ucMeterNum;
    HI_U8 ucEvtOutFlg;
    HI_U8 ucDelay;
}MV_APP_SIMU_METER_INFO;

typedef struct
{
    HI_U32 ulMaxNum;     // �ڵ����������PLC��ʱ����
}MV_APP_MAX_PLC_TIMEOUT_NUM;    // ID_MV_APP_MAX_PLC_TIMEOUT_NUM

typedef struct
{
    HI_U8 ucEnable;  // ʹ�ñ��ַ����Mac��ַ���ܿ��� 0:������ 1:����
    HI_U8 reserved[3];
}NV_APP_MAC_FROM_METER_STRU;

typedef struct
{
    HI_U32 ulTurnNum;   // ����������������ִ�
}NV_APP_MAX_TURN_NUM;   // ID_NV_APP_MAX_TURN_NUM

typedef struct
{
    HI_U8 ucEnable;  // ʹ�ù㲥��ַ��Ӧ�㲥��ַ����ʹ�ܿ��� 0:������ 1:����
    HI_U8 reserved[3];
}NV_APP_BROADCAST_TX_STRU;

typedef struct
{
    HI_U8 ucDutFlag; //��װ�����е㳭ʹ�ù㲥�����־ 0:������  1:���� ��Ĭ��Ϊ0
    HI_U8 ucRePowerTime; // CCO�ָ����书������ʱ�䣬Ĭ��15����
    HI_S16 sNtbAjustBase;   // STA��II�ɹ�װNTBУ׼����
}NV_APP_MR_DUT_STRU;

typedef struct
{
    HI_U16 usSendMacPlcTime;     //CCO�˷��Ͷ����ַPLC֡ʱ����
    HI_U16 usSendLastMacPlcTime; //CCO�˷������һ��Mac�����ַPLC֡���ʱ����
    HI_U8 ucMapEnable;       //CCO��ӳ���ά��ʹ�ܿ��� 0:������  1:���� �� Ĭ��Ϊ1
    HI_S8 max_parallel_num;
    HI_S8 max_retry_num;
    HI_U8 ucCltMapEnable; //CCO�˲ɼ���ӳ���ά������  0:������ 1:���� Ŀǰ�ڽ��հ汾����
    HI_S16 max_ttl;   //Ĭ��Ϊ15
    HI_U16 check_ttl_time; //Ĭ��Ϊ120s����λΪs
    HI_U16 map_turn_time;
    HI_U8 max_turn_num;
    HI_U8 ucLevelFlag_ucMaxMrsTtl;   //����ʱ�м̼�������
}NV_APP_MAC_MAP_INFO_STRU;

// ��������NV������
typedef struct
{
    HI_U16 usMaxParallelNum;       // ��󲢷���
    HI_U16 usMaxFrame;             // ����֡��
    HI_U16 usMaxTotalItem;         // ���ҵ���������������
    HI_U8 ucCheckRepeatMode;       // ����ظ���������ģʽ(0:ʹ��CRC���; 1:ʹ�ñ��ַ���)
/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */
    HI_U8 ucMeterSwitchInterval;   // ����л����,���ڿ���ͬһ�ɼ����²�ͬ����л�ʱ�ļ��ʱ��,��λ10ms
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */
}NV_MRS_PARALLEL_CFG_S;

// �������ģ��NV������
typedef struct
{
    NV_MRS_RETRY_CFG_S stRetryCfg[MRS_RETRY_CFG_MAX_NUM];
    NV_MRS_PARALLEL_CFG_S stParallelCfg[MRS_RM_POOL_MAX_NUM];
}NV_MRS_RM_CFG_STRU;    // ID_NV_MRS_RM_CFG


typedef struct
{
    HI_U16 usFrameHead;         // ���ֽڳ�ʱʱ�䣬Ĭ��: STA/II��-800ms, I��-3600ms
    HI_U16 usFrameInterval;     // ֡���ʱ�䣬Ĭ��: 200ms
    HI_U16 usCtrlCmdDelay;      // �ѿ��������Ĭ��: 2000ms
    HI_U16 usMeterInterval;     // 645֡���
    HI_U8  ucQueueMax;          // ������ȣ�Ĭ��: 16
    HI_U8  ucRetryCfg;          // bit0-bit1:STA�ֳ���͸���ش�����  bit2-bit3:���Ӧ������ش�����
    HI_U8  pad[6];
} NV_MRS_STA_RM_CFG_INF;

#define MRS_STA_RM_CFG_MAX      (2)

typedef struct
{
    NV_MRS_STA_RM_CFG_INF astRmCfg[MRS_STA_RM_CFG_MAX];  // 0: STA/II��; 1: I��
} NV_MRS_STA_RM_CFG_STRU;


typedef struct
{
    HI_U8 ucStaRepFlag;         // STA�������ϱ����ַ��ά��ӳ����� 0:������  1:���� �� Ĭ��Ϊ0
    HI_U8 ucMeterChangeFlag;    // STA�˱��ַ�ı��,�ϱ�CCO����       0:������  1:����    Ĭ��Ϊ??????
    HI_U8 ucDutAdcFlag;         // ��װADC�������Կ��أ�Ĭ�ϴ�
    HI_U8 reserved;
}NV_APP_REPORT_METER_INFO_STRU;  //STA�������ϱ����ַ��MAC��ַ��Ϣ

typedef struct
{
    HI_U32  seq;                // �ѱ����к�:ÿ���ѱ����Ψһ��һ�����,�����ѱ���ż�1
    HI_U32  search_di_07;       // �ѱ����е�DIֵ(645-2007��Լ���ܱ�), Ĭ��ֵ0x04000401
    HI_U16  search_di_97;       // �ѱ����е�DIֵ(645-1997��Լ���ܱ�)����, Ĭ��ֵ0xC032
    HI_U16  frame_timeout;      // ���Ӧ��֡���ֽڳ�ʱʱ��(ms)
    HI_U8   byte_timeout;       // ���Ӧ��֡�ֽڼ����ʱʱ��(ms)
    HI_U8   meter_num_max;      // ������������������32
    HI_U8   search_bc_enable;   // �㲥����97���� 0:�ر� ����:��
    HI_U8   search_retry;       // �ѱ�ʱ�������Դ���
    HI_U8   bc_loop_max;        // ����/ֹͣ�ѱ�����㲥�ִ�
    HI_U8   bc_send_interval;   // ����/ֹͣ�ѱ���������ͼ��(100ms)
    HI_U8   topo_query_interval;// �ѱ�ʱTOPO��ѯʱ����(min)
    HI_U8   topo_stable_times;  // TOPO����δ�������
    HI_U8   query_max_way;      // ����������ѯ��վ���������
    HI_U8   query_interval;     // ����������ѯʱ����(s)
    HI_U8   query_force_times;  // ǿ��Ҫ��Ӧ���������
    HI_U8   query_except_times; // ��ѯ�쳣��������
    HI_U8   finish_reason;      // �ѱ����ԭ��

    HI_U8   detect_loop_max;            // STA�ϵ�����: ̽�Ⲩ�����ִ�
    HI_U16  detect_timeout;             // STA�ϵ�����: ÿ��̽�Ⲩ���ʳ�ʱʱ�� ��λ:����
    HI_U16  detect_period;              // STA�ϵ�����: һ��̽��ѭ���������ٴο�ʼѭ���ĵȴ�ʱ�� ��λ:��
    HI_U8   query_sm_interval;          // I���ѱ�: �����в�ѯ�ѱ���ʱ���� ��λ:��
    HI_U8   read_positive_A_times;      // I���ϵ�����: ����NV�е�������й����ܴ���
    HI_U8   read_positive_A_interval;   // I���ϵ�����: ��ѯI�Ͳɼ�����1���ѱ���ʱ���� ��λ:��
    HI_U8   explore_byte_count_max;     // II���ѵ�������֤�������ֽ������ޣ�Ĭ��ֵΪ0
    HI_U16  search_di_97c;              // �ѱ����е�DIֵ(645-1997��Լ���ܱ�)���, Ĭ��ֵ0x901f
    HI_U16  search_di_97b;              // �ѱ����е�DIֵ(645-1997��Լ���ܱ�)���, Ĭ��ֵ0x9010
    HI_U8   invalid_frm_len_07;         // 07��Լ�ѱ�����֡������Сֵ��Ĭ��ֵΪ1
    HI_U8   invalid_frm_len_97;         // 97��Լ�ѱ�����֡������Сֵ��Ĭ��ֵΪ2
    HI_U8   report_AA_2_master;         // �ѱ����ϱ�ʱ���ַ���ֽ�AAH�ϱ���ʶ: 0-���ϱ���1-�ϱ���Ĭ��ֵ0

    HI_U8   ucSearchEnableFlag_ucCltIPlcThreshold;        	// I��/II���ѱ�ʹ�ܣ�bit0��I���ѱ�ʹ�ܣ�bit1��ʾII���ѱ�ʹ��: 1��ʾ���ܿ�����0��ʾ���ܹر�
    HI_U16  search_period;              // �����ѱ�ʱʱ��

    HI_U16  usAfn06F4UpTimeOut;         // �ѱ�06F4�ϱ���ʱʱ��
    HI_U8   ucAfn06F4UpRetryMax;        // �ѱ�06F4�ϱ�������Դ���
    HI_U8   ucDetectTimeOut_ucCltIQueryThreshold;            // II���ѱ���̽���ѱ�����ʱʱ��,��λ:�ٺ���
} NV_APP_SEARCHMETER_CFG_STRU;

#define MRS_TF_REPORT_MODE_NUM      (4)

#define APP_AREA_CODE_DEFAULT       (0x00)      /**< ������: Ĭ��ֵ */
#define APP_AREA_CODE_BEIJING       (0x01)      /**< ������: ����   */

typedef struct
{
    HI_U16  usSMDurationDefault;        // �ѱ�Ĭ��ʱ������λmin
    HI_U8   ucRecoverWhiteSwitch;   // �ѱ����ʱ���Ƿ�ָ��ѱ�ǰ������״̬
    HI_U8   ucBindCmdSendInterVal;  // ��������ͼ����ÿ������һ�ּ����ô��ʱ�䣬��λs

    HI_U8   ucCheckMainNodeAddr;    // ������ڵ��ַ�ı�ʱ�����½���̨��ʶ��
    HI_U8   ucAllowSMInTf;          // ̨��ʶ�����Ƿ������ѱ�
    HI_U16  usCheckMainNodeTimeout; // ������ڵ��ַ��ʱʱ��

    HI_U8   ucSmReportWaitLockMsg;  // �ѱ��ϱ��Ƿ�ȴ���������
    HI_U8   ucCloseWhiteWhenSM;     // �ѱ�����Ƿ�رհ�����
    HI_U8   ucCcoRstPinEnable;      // CCO��RST�ܽ�ʹ�ܣ�1ʹ�ܣ�0��ʹ��
    HI_BOOL pad0;

    HI_U8   ucBindCmdSendMaxWay;    // ��������һ����󷢼�֡��Ŀǰ����ʹ��
    HI_U8   ucBindCmdSendMaxTimes;  // ������������ʹ�����Ŀǰ����ʹ��
    HI_U16  pad1;

    HI_U8   ucReportModeInit;       // ��ʼ���ϱ�ģʽ��ÿ��ģʽ��Ӧ���ϱ�������aucReportModeMask����
    HI_BOOL bUseNotifyMode;         // �Ƿ�ʹ�ü�����֪ͨ���ϱ�ģʽ
    HI_BOOL bCloseWhiteTfBegin;     // ����ʶ��ʱ�Ƿ�رհ�����
    HI_BOOL bRecoverWhiteTfEnd;     // ֹͣʶ��ʱ�Ƿ�ָ�������

    HI_BOOL bCareReportSwitch;      // ʹ��06-F5�ϱ�ʱ���Ƿ����¼��ϱ�����Ӱ��
    HI_BOOL bReportUnknownMainNode; // ����δ֪���ڵ��ַ��վ�㣬�Ƿ��ϱ�
    HI_U8   aucMainNodeAddrUnknown[HI_PLC_MAC_ADDR_LEN];    // δ֪���ڵ��ַ��վ���ϱ�ʱ����д�����ڵ��ַ

    HI_U8   aucReportModeMask[MRS_TF_REPORT_MODE_NUM];      // ����ģʽ�ϱ���Щ����

    HI_U8   area_code;              /**< ������: 0-Ĭ�ϣ�1-�����汾������-���� */
    HI_U8   ucQueryInterval;        // ��ѯ�������λs
    HI_U8   ucQueryMaxTry;          // ��ѯ����Դ���
    HI_U8   ucQueryMaxWay;          // һ�η���֡

    HI_U16  usQueryBeginDelayTIme;  // ��ѯ��ʼ�ӳ٣���λs
    HI_U16  usQueryRoundInterval;   // �ִμ������λs

    HI_U8   ucFlagInArcInBelong;    // ���Ͷ���: �ڵ��������ڱ�̨��
    HI_U8   ucFlagInArcNoBelong;    // ���Ͷ���: �ڵ����������ڱ�̨��
    HI_U8   ucFlagNoArcInBelong;    // ���Ͷ���: ���ڵ��������ڱ�̨��
    HI_U8   ucFlagNoArcNoBelong;    // ���Ͷ���: ���ڵ����������ڱ�̨��

    HI_U8   aucSetOffModeMask[MRS_TF_REPORT_MODE_NUM];
	HI_U16   usNvRfTimeSlot;				// ����˫ģʱ϶ʱ�䣬����
	HI_U16   usNvRfSGDTimeSlot;				// ���ù���˫ģʱ϶ʱ�䣬����;

    HI_U32  pad3[3];
} NV_APP_CCO_SM_CFG_EXP_STRU;

typedef struct
{
    HI_U16 usLeaveTimeLenTh;    // ����ʱ�����ޣ���λmin
    HI_U8  ucStaRstPinEnable;   // STA��RST�ܽ�ʹ�ܣ�1ʹ�ܣ�0��ʹ��
    HI_U8  pad;
    HI_U32 ulRstShielTime;      // ����RST�ܽ�ʱ������λs

    HI_U32 pad1[2];
} NV_APP_STA_SM_CFG_EXP_STRU;

// ���������ýṹ��
// �ṹ���Ա�����������λ��(�������������mrs_srv_baudrate_manage.h�е���ؽṹͬ���޸�)
// ÿ��������D0~D15ÿ3bit��ʾһ��ͨѶ����
// 0��ʾ��Ч
// 1��ʾ600bps
// 2��ʾ1200bps
// 3��ʾ2400bps
// 4��ʾ4800bps
// 5��ʾ9600bps
// 6��ʾ19200bps
// 7��ʾ38400bps
// ÿ���������֧��5��ͨѶ����
// ����07���������ò�����Ϊ2400,4800,1200,9600  ���ӦusRmOf07ȡֵΪ0 000 101 010 100 011 = 0x0AA3
typedef struct
{
    HI_U16 usSmOf97;        // 97���ѱ�����
    HI_U16 usSmOf07;        // 07���ѱ�����
    HI_U16 usSmOf698;       // 698���ѱ�����
    HI_U16 usRmOf97;        // 97��������
    HI_U16 usRmOf07;        // 07��������
    HI_U16 usRmOf698;       // 698���ѱ�����
    HI_U16 usReserved3;     // �����ֶ�3
    HI_U16 usAdjustTime;    // �㲥Уʱ����
    HI_U16 usDetect;        // �ϵ�̽�Ⲩ��������
    HI_U8  padding[2];
}NV_APP_BAUDRATE_CFG_STRU;


typedef struct
{
    HI_U16 usTimingSendInterval;    // �㲥Уʱ���ͼ��
    HI_U16 usEvtMonitorInterval;    // �����¼���س������
    HI_U8  ucEvtMonitorFlag;        // �����¼�����������ʶ
    HI_U8  ucIrTrasmit645;          // ͸��645��ʶ
    HI_U8  ucPowerSearchEnable;     // Ii�ɱ����ѱ�ʹ�ܿ���
    HI_U8  ucRunLedMode;            // II�����е�״̬ģʽ: 0��ʾ����ģʽ(����״̬485����100ms)��1��ʾ�����ͼ�ģʽ(485����20ms),Ĭ��Ϊ1
/* BEGIN: ADD/Modified by cuichunyan/c00276962 2016/11/10  ���ⵥ��:cltii R485 MultiConnect */
    HI_U8  ucR485Monitor;           //II��485�ڼ���Ƿ�����0���ر� 1������ ��Ĭ�ϴ�
/* END  : ADD/Modified by cuichunyan/c00276962 2016/11/10  ���ⵥ��:cltii R485 MultiConnect */
    HI_U8  area_version;            // ����汾  bit0:���հ汾��־,��1��ʾ���հ汾,Ĭ��Ϊ0
    HI_U8  padding[2];
} NV_APP_CLTII_CFG_STRU;


typedef struct
{
    HI_U8 ucFileCheckMsgMaxTimes; //�����ļ������Ϣ��ʱ����������Դ���
    HI_U8 ucUpgCmdMsgMaxTimes;   //MRSģ����UPGģ�鷢�͵���������,���ִ��ʧ�ܣ��ط���������
/* BEGIN: PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
    HI_U8 ucWaitUpgStartTimeMax;    // Զ�������ȴ���ʼ�������ʱ�䣬Ĭ��ֵ: 180, ��λ: ����
/* END:   PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
    HI_U8 ucPowerOnStatusMaxTimes; //MRSģ���ϵ���ѯ����״̬��ѯ��ʱ������
    HI_U16 usMrsUpgFileRxTimeOut; //��װ�����н����������ݰ���ʱʱ��,��ʼֵΪ600����λΪ��
    HI_U16 usMrsUpgStatusQueryTime; //MRSģ����UPGģ�鷢�Ͳ�ѯ����״̬����,��ʼֵΪ60����λΪ��
}NV_APP_MRS_UPG_PARAM_STRU;


typedef struct
{
    HI_U8   ucReportEnable;     // �¼��ϱ���ʶ
    HI_U8   ucCcoMaxTry;
    HI_U16  usCcoTimeOut;       // ��λ:ms

    HI_U16  usCcoFilterAgedTIme;//���������ϻ�ʱ��,��λ:����
    HI_U16  usCcoStatBakTime;   //CCO���¼�ͳ��������ʱ��,��λ:����

    HI_U16  usCcoEvtBufSize;    //CCO���¼���������С
    HI_U8   ucMeterEvtType;     //STAʶ�𵽵ĵ���¼����ͣ�0��δ֪��1��13�淶�����¼���Ĭ��ֵΪ0
	HI_U8   ucStaConsDenyTimes;

    HI_U8   ucStaEvtEnable;     // STA�Ƿ������¼��ϱ�
    HI_U8   ucStaMaxTry;        // �¼��ϱ�CCO������Դ���
    HI_U8   ucCltIIEnable;      // II���ϱ��¼�����
    HI_U8   ucEvtTimeOutMax;    // STA�����¼�֡��ʱʱ�䷶Χ���ֵ ��λ:��

    HI_U8   ucStaTimeOut;       // ��λ:ms
    HI_U8   ucStaMonitorInterval;//STA���ؼ�ؼ������λ������
    HI_U16  usPlcSeq;

    HI_U8   ucCltIEnable;
    HI_U8   ucWorkMode;         //����ģʽ
    HI_U16  usRandomMin;        // ͣ��վ������ϱ�ʱ����Сֵ����λ���룬Ĭ��0

    HI_U16  usRandomMax;        // ͣ��վ������ϱ�ʱ�����ֵ����λ���룬Ĭ��200
    HI_U16  usSendInterval; // ͣ��վ�㷢�ͼ��ʱ�䣬��λ���룬Ĭ��2000
    HI_U8   ucSendTimes;        // ͣ��վ�㷢��ͣ�籨�Ĵ�����Ĭ��10
    HI_U8   ucStaConvergeTime; // �㼯�ȴ�ʱ�䣬��λ�룬Ĭ��30
    HI_U8   ucPowerFailureReportEnable; //�Ƿ���ͣ��
    HI_U8   ucPowerOffMeterTimeDelay; //���ǵ��ɼ����ϵ�����еĻ��ӳٵ�ʱ�� ��λ:100ms Ĭ��15

    HI_U8   ucCcoConvergeTime; // �㼯�ȴ�ʱ�䣬��λ�룬Ĭ��30
    HI_U8   pad;
    HI_U16  pad1;
}NV_APP_MRS_EVT_CFG_STRU;


// ��С: 64�ֽ�
typedef struct
{
    HI_U8   ucReportEnable;              // �ϱ��ӽڵ�״̬�仯ʹ�ܿ���
    HI_U8   ucPowerOffReasonSwitch;      // ����ԭ���ϱ����أ��ر�ʱ����ԭ���Ϊδ֪
    HI_U8   ucPlcChlReasonSwitch;        // �ŵ�ԭ���ϱ����أ��ر�ʱ����ԭ���Ϊδ֪
    HI_U8   ucCacheNumMax;               // �ϱ�ǰ��໺�����Ϣ����

    HI_U16  usCacheTimeout;              // ���ȴ��೤ʱ�俪ʼ�ϱ�,��λ: 100����
    HI_U16  usReportTimeout;             // 06H-F10�����ϱ��ĳ�ʱʱ��, ��λ: ����

    HI_U8   ucReportRetryMax;            // 06H-F10�ϱ���������Դ���
    HI_U8   ucDetectEnable; //ȫ�������� 0:������ 1:����,Ĭ��Ϊ1
    HI_U16  usWhiteAddMaxTime; //����mac��ַ�����Ĳɼ�����������������ucWhilteAddMaxTimeʱ����û�м���������Ĳɼ���վ������,ɾ����ӵİ�����,��λ����

	HI_U16  aus97DI[MRS_BROADCAST_RM_97_DI_NUM];
	HI_U32  ul07DI;

    HI_U8   ucRmTurn;
	HI_U8   ucTopoWaitMaxTime; //���CCOһֱ������������ֻ��CCOһ��վ��,20���Ӻ��յ��ܾ�����֪ͨ�󽫾ܾ��ɼ���վ��������������,ǰ���Ǳ����ǿ�,��λ����
	HI_U8   ucJoinNetMaxTime;  //̽������л�����׷�Ӱ������е�վ������ʱ��,��ʱ������̽�Ⳮ��,��λ:����
    HI_U8   ucBlackValidTime;  //̽������к�������Чʱ�䣬��λ:����

    HI_U8   ucStaJoinWhiteTime; //���ܾ���վ����뵽��������ʱ�� ��λ:����
    HI_U8   ucSingleMeterTime;  //ÿ���Ԥ��̽��ʱ�� ��λ:��
    HI_U8   ucTurnTime; //���͵���ַ�б�Plc����ʱ���� ��λ:����
    HI_U8   ucPoweroffDetectMaxTries;   // ͣ��̽�������� 6

    HI_U8   ucPoweroffDetectInterval;   // ͣ��̽��㲥���ķ��ͼ�� 10
    HI_U8   ucPoweroffRecheckMaxTries;  // ͣ�縴�������� 30
    HI_U8   ucPoweroffRecheckInterval;  // ͣ�縴�˹㲥���ķ��ͼ�� 120
    HI_U8   ucReviewScoreFull;          // ͣ���⸴�����ַ��� 7

    HI_U8   ucWrongReportReduceScore;   // ��ʱ�����ַ��� 3
    HI_U8   ucMissReportPlusScore;      // ©��ʱ���ӷַ��� 1
    HI_U8   ucCorrectReportPlusScore;   // ��ȷ�ϱ�ʱ���ӷַ��� 1
    HI_U8   ucNoPassDetectPlusScore;    // ������վ��û��̽��ɹ��ļ�¼ʱ���ӷַ��� 1

    HI_U16  usNoPassDetectPeriod;       // վ���Ƿ���̽��ɹ���¼ʶ�����ڣ���λmin 1440
    HI_U16  usReportMaxDelayTime;       // ��ȷ�ϱ����ϱ�ʱ��-ͣ��ʱ��ܴ�ʱ�䣬��λmin 20

    HI_U16  usMacReportWrongPeriodTh;   // MAC���������ޣ���λmin  10080
    HI_U8   ucPoweroffDetectExpTries;   // ��MAC����������֮�ڣ�����MAC�󱨣�����Ⳣ����ô��� 6
    HI_U8   ucMacReportCacheMaxTime;    // MAC�ϱ����л������ʱ�� 5

    HI_U16  usMacReportCacheMaxNum;     // MAC�ϱ����л��������� 1024
    HI_U8   ucScalePowerOffDetectInterval; //10��վ��ͣ��ʱ��̽����  48
    HI_U8   pad;                            // 0

    HI_U8   aucPad2[16];
} NV_APP_MRS_STATECHG_REPORT_CFG_STRU;


#define NV_SM_RESULT_METER_NUM   (16)

typedef struct
{
    HI_U8 aucAddr[6];           // �ѱ���: ����ַ
    HI_U8 ucParam;              // �ѱ���: �õ�����Ϣ [0:3]: ���ܱ��Լ����(0͸����Լ 1-97��Լ 2-07��Լ ����-����)
                                // [4:7]: �ѱ�ʱ����δ�ѵ��Ĵ���������3��δ�ѵ��ı���б���ɾ��
    HI_U8 ucOption;             // �ѱ���: �õ����Ϊ97��Լ��: �ѱ��DIֵ 0x10-0x9010 0x1f-0x901f �������:�̶�Ϊ0
}NV_APP_MRS_SM_NODE;


typedef struct
{
    NV_APP_MRS_SM_NODE astMeter[NV_SM_RESULT_METER_NUM];     // �洢���ѱ���
}NV_APP_MRS_SM_RESULT;


// ID: 0x418
typedef struct
{
    HI_U8 ucValid;              // ��Ч�Ա�ʶ
    HI_U8 ucDutyCycle;          // ռ�ձȿ��Ʋ��Ե�Ŀ�����ްٷֱ�
    HI_U16 usMaxSendLength;     // ռ�ձȲ���������������ʱ������λ������
} NV_APP_STA_DUTY_CYCLE_CFG_ITEM;

typedef struct
{
    HI_U8 ucEnable;             // ռ�ձȵ������Կ�����ʶ��0���رգ�1��ʹ��
    HI_U8 aucReserved0[3];      // �����ֶΣ�3�ֽ�
    HI_U8 ucShortDuration;      // �����϶�ʱ�䣬��λ�����ӣ�Ĭ��ֵ30
    HI_U8 ucShortTimes;         // ��ʱ������������ֵ��Ĭ�ϣ�10�������ڴ洢�ռ䣬���ó���64��
    HI_U8 ucLongDuration;       // �����ϳ�ʱ�䣬��λ��Сʱ��Ĭ��ֵ72
    HI_U8 ucLongTimes;          // ��ʱ������������ֵ��Ĭ�ϣ�30�������ڴ洢�ռ䣬���ó���64��
    NV_APP_STA_DUTY_CYCLE_CFG_ITEM astCfgItem[6]; // ռ�ձ���Ϣ

    HI_U32 aulReserved1[4];
} NV_APP_STA_DUTY_CYCLE_CFG_INFO;

typedef struct
{
    HI_U32 ulUpgId;

    HI_U8  ucStatus;
    HI_U8  ucFileId;
    HI_U16 usReserved;
}NV_APP_CLTI_CSG_CFG_STRU;//I�����ò������NV 0x427 ID_NV_APP_CLTI_CFG

// ID: 0x40C ~ 0x40F
typedef struct
{
    HI_U8 aucData[128];
} NV_APP_NVM_DATA_STRU;

typedef struct
{
    HI_U8 aucData[64];
}NV_APP_RESERVED_STRU;


typedef struct
{
    HI_U32 reserved[4];//Ԥ��
}NV_APP_CCO_PARAM_S;//0x431


#endif //__APP_NV_H__


