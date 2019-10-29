//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2012, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_res.h
//  �� �� ��   : V1.0
//  ��    ��   : niesongsong/kf62735
//  ��������   : 2012-07-25
//  ��������   : SRV��ʱ���빫��ID����
//  �����б�   :
//  �޸���ʷ   :
//  1.��    �� : 2012-07-25
//    ��    �� : niesongsong/kf62735
//    �޸����� : �����ļ�
//
//  2.��    �� : 2013-07-24
//    �� �� �� : cuiate/00233580
//    �޸����� : ��������timer����
//
//  ��ע��
//      �����ڲ���Դ����ʹ�ã��ⲿ���ɼ�
//*****************************************************************************
#ifndef __MRS_SRV_RES_H__
#define __MRS_SRV_RES_H__
HI_START_HEADER


//������ʽ
//#define MRS_CCO_TIMER_XXX
//#define MRS_CCO_TIME_XXX
//#define MRS_CCO_STATE_XXX
//#define MRS_STA_TIMER_XXX
//#define MRS_STA_TIME_XXX
//#define MRS_STA_STATE_XXX

//*****************************************************************************
// CCO�Ķ�ʱ��
//*****************************************************************************

typedef enum
{
    MRS_CCO_TIMER_MAP,                      // ӳ���ά����ʱ��ID
    MRS_CCO_TIMER_STA,                      // ��STAͨѶ��ʱ��ID
    MRS_CCO_TIMER_MASTER,                   // �뼯����ͨѶ��ʱ��ID
    MRS_CCO_TIMER_READMETER_ACTIVE,         // ����ģ�鼤�ʱ��
    MRS_CCO_TIMER_SERVICE_END,              // ���������ʱ��(�����ѱ���泭�ȷ���Ľ���)

    MRS_CCO_TIMER_FLASHWRITE_ACTIVE,        // �������̼��ʱ��ID
    MRS_CCO_TIMER_GET_TIME,                 // ��ʱ��ȡ������ʱ�䶨ʱ��
    MRS_CCO_TIMER_SYSTEM_RESET,             // AFN=01H-F1ִ��Ӳ��������ʱ��ID
    MRS_CCO_TIMER_PARALLEL_CHECK,           // ��������PLC��ʱ��鶨ʱ��
    MRS_CCO_TIMER_UPG_WAIT_PLC_AVAILABLE,

    MRS_CCO_TIMER_BROADCAST,
    MRS_CCO_TIMER_NODE_STATE_CHG_ACTIVE,
    MRS_CCO_TIMER_EVENT_ACTIVE,
    MRS_CCO_TIMER_MASTER_ACTIVE,
    MRS_CCO_TIMER_MASTER_REPORT,

    MRS_CCO_TIMER_RESET_PIN,
    MRS_CCO_TIMER_SIMU_CHL,
    MRS_CCO_TIMER_SEARCHMETER_SEND_FRAME,
    MRS_CCO_TIMER_GET_METERLIST,
    MRS_CCO_TIMER_TOPO_QUERY,
    MRS_CCO_TIMER_SEND_BIND_CMD,            // ������������

    MRS_CCO_TIMER_CHECK_MAC_MAP_TTL,
    MRS_CCO_TIMER_MAP_REFRESH,
    MRS_CCO_TIMER_SEARCH_METER_END,
/* BEGIN: Added by fengxiaomin/00209182, 2014/1/13   ���ⵥ�ź�:DTS2014012607684 */
    MRS_CCO_TIMER_UPG_FILE_RECEIVE,         // ������װ�׶ν��յ��������ݳ�ʱ��ʱ��
    MRS_CCO_TIMER_UPG_STOP_RETRY_ACTIVE,    // ֹͣ�����ط���ʱ��
    MRS_CCO_TIMER_UPG_STATUS_QUERY_ACTIVE,  // ����״̬��ѯ��ʱ��
    MRS_CCO_TIMER_UPG_STATUS_QUERY_TIMEOUT, // ����״̬��ѯ��ʱ��ʱ��
    MRS_CCO_TIMER_NODE_STATECHG_ENTRY_CACHE,

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    MRS_CCO_TIMER_ACQUIRE_CLOCK,
    MRS_CCO_TIMER_MRS_LOG,
#endif
    MRS_CCO_TIMER_SEARCH_METER_END_TF,
    MRS_CCO_TIMER_SEARCH_METER_WHITELIST,
    MRS_CCO_TIMER_SEARCH_REPORT,
    MRS_CCO_TIMER_TEST_MODE_FRAME,

    MRS_CCO_TIMER_ZERO_CROSS,

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
    MRS_CCO_TIMER_RUN_LED_BBU,
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    MRS_CCO_TIMER_EVT_FLAG_NOTICE,
    MRS_CCO_TIMER_UPG_QUERY_INFO_TIMEOUT,   // ��ѯ����ȫ����Ϣ
    MRS_CCO_TIMER_MR_TASK_ACTIVE,           // �������񼤻��������ͣ״̬��δ����ʱʹ��
    MRS_CCO_TIMER_MODE_PHYSICAL_TEST,       //��������ģʽ
    MRS_CCO_TIMER_ENTER_TESTMODE,           //�������ģʽ
#endif

    MRS_CCO_TIMER_DETECT_WHITE_LIST_KEEP,   // ����mac��ַ�����Ĳɼ�����������������30������û�м���������Ĳɼ���վ������,ɾ����ӵİ�����
    MRS_CCO_TIMER_DETECT_REJECT_CLT_CACHE,  // ̽�Ⳮ������л���ܾ�վ����Ϣ��ʱ��
    MRS_CCO_TIMER_DETECT_WAIT_TOPO,         // ̽������еȴ���������,һ��ʱ�����������ֻ��CCO,�������ܾ���վ����������
    MRS_CCO_TIMER_DETECT_JOIN_NET_CACHE,    // ̽������л�����׷�Ӱ������е�վ��������ʱ��,��ʱ������̽�Ⳮ��
    MRS_CCO_TIMER_DETECT_BLACK_LIST,        // ��������ʱ����ʱ��
    MRS_CCO_TIMER_DETECT_REFRESH_WHITE,     // ̽�Ⳮ�������֪ͨˢ�°�������ʱ��
    MRS_CCO_TIMER_RESUME_DETECT_RM,         // ̽�Ⳮ������лָ�̽�Ⳮ��ʱ��
    MRS_CCO_TIMER_SEND_METER_LIST_PLC,      // ̽�Ⳮ������з��ͱ��ַ�б�PLC֡��ʱ��

    MRS_CCO_TIMER_RECOVER_POWER_AFE,        // �ָ����书�ʶ�ʱ��
    MRS_CCO_TIMER_POWERFAILURE,             //CCO�ϱ�ͣ���¼�
    MRS_CCO_TIMER_POWERFAILURE_ERASE_DATA,  //CCO�������ݶ�ʱ��

    MRS_CCO_TIMER_MAX
} MRS_CCO_LOGIC_TIMER_ID_E;


#define EN_MRS_SRV_CCO_MAP_VIN_TIMER            (MRS_CCO_TIMER_MAP)
#define EN_MRS_SRV_CCO_STA_TIMER                (MRS_CCO_TIMER_STA)
#define EN_MRS_SRV_CCO_MUSTER_TIMER             (MRS_CCO_TIMER_MASTER)
#define EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER    (MRS_CCO_TIMER_READMETER_ACTIVE)
#define EN_MRS_SRV_CCO_SERVICE_END_TIMER        (MRS_CCO_TIMER_SERVICE_END)
#define EN_MRS_SRV_CCO_WRITE_FLASH_ACTIV_TIMER  (MRS_CCO_TIMER_FLASHWRITE_ACTIVE)
#define EN_MRS_SRV_CCO_GET_TIME_TIMER           (MRS_CCO_TIMER_GET_TIME)
#define EN_MRS_SRV_CCO_SYSTEM_RESET_TIMER       (MRS_CCO_TIMER_SYSTEM_RESET)
#define EN_MRS_SRV_CCO_RM_CHECK_TIMER           (MRS_CCO_TIMER_PARALLEL_CHECK)
#define EN_MRS_SRV_CCO_BROADCAST_TIMER          (MRS_CCO_TIMER_BROADCAST)
#define EN_MRS_SRV_CCO_NODE_STATE_CHG_TIMER     (MRS_CCO_TIMER_NODE_STATE_CHG_ACTIVE)
#define EN_MRS_SRV_CCO_EVENT_ACTIVE_TIMER       (MRS_CCO_TIMER_EVENT_ACTIVE)
#define EN_MRS_SRV_CCO_RESET_PIN_TIMER          (MRS_CCO_TIMER_RESET_PIN)
#define EN_MRS_SRV_CCO_SIMU_CHL_NOTIFY_MONITOR  (MRS_CCO_TIMER_SIMU_CHL)
#define EN_MRS_SRV_CCO_CHECK_MAC_MAP_TTL_TIMER  (MRS_CCO_TIMER_CHECK_MAC_MAP_TTL)
#define EN_MRS_SRV_CCO_MAP_REFRESH_TIMER        (MRS_CCO_TIMER_MAP_REFRESH)

//*****************************************************************************
// CCO��ʱ���Ķ�ʱ����
//*****************************************************************************
#define MRS_CCO_TIME_MASTER                     (1000 * 60) // ���䳭����ipͨ�����Ϊ1min
#define MRS_CCO_TIME_READMETER_ACTIVE           (200)       // ����ģ�鼤��ʱ��
#define MRS_CCO_TIME_SERVICE_END                (300)     // ���������ʱ��ʱ��(�����泭�ȷ���Ľ���)

#define MS_PER_SECOND                           (1000)      //��ʱ��������λ  1s
#define MRS_CCO_TIME_FLASHWRITE_ACTIVE          (20)     // �������̼���ʱ��  Ĭ��20s

#define MRS_CCO_TIME_SYSTEM_RESET               (5000)       // AFN=01H-F1ִ��Ӳ��������ʱʱ��
#define MRS_CCO_TIME_PARALLEL_CHECK             (50)       // ��������ʱ��鶨ʱ��Ĭ��ֵ(ÿ�����ʱ����һ�γ�ʱ���)
#define MRS_CCO_TIME_SEARCHMETER                (2000)
#define MRS_CCO_TIME_RESET_PIN                  (150)

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
#define MRS_CCO_TIME_ACQUIRE_CLOCK              (1500)
#define MRS_CCO_TIME_CHECK_CLOCK                (1000*60)
#define MRS_CCO_TIME_STARTUP_CLOCK              (1000*60*20) //CCO�ϵ�20���Ӻ�ʼ��ѯ������ʱ�ӵ�ʱ��
#define MRS_CCO_TIME_ACTIVE_ACQUIRE_CLOCK       (200) //CCO�յ�·��������·�ɻָ�����󣬼�����ѯ������ʱ�ӵ�ʱ��
#endif

#define MRS_SRV_CCO_TIMER_MUSTER_RETRY_NUM_DEFAULT      (2)             // ������ͨѶĬ��������Դ���
#define MRS_SRV_CCO_TIMER_MUSTER_TIMEOUT_DEFAULT        (1000 * 10)     // ������ͨѶĬ�ϳ�ʱʱ��(ģ��ͨ���ر�),��λ:ms

#define MRS_SRV_CCO_TIMER_MUSTER_SIMU_CHL_TIMEOUT_DEFAULT  (1000 * 60)   // ������ͨѶĬ�ϳ�ʱʱ��(ģ��ͨ����),��λ:ms

#define MRS_SRV_CCO_TIMER_MUSTER_VALUE                  (MRS_CCO_TIME_MASTER)
#define MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE         (MRS_CCO_TIME_READMETER_ACTIVE)
#define MRS_SRV_CCO_TIMER_SEARCH_ACTIV_VALUE            (MRS_CCO_TIME_READMETER_ACTIVE)
#define MRS_SRV_CCO_TIMER_READMETER_REACTIV_VALUE       (1000 * 30)
#define MRS_SRV_CCO_TIMER_SERVICE_END_VALUE             (10)     // ���������ʱ��ʱ��(�����泭������ָ��ֳ��ȷ���Ľ���)
#define MRS_SRV_CCO_TIMER_SAVEMETERS_FLASH_ACTIVE_VALUE (MRS_CCO_TIME_FLASHWRITE_ACTIVE)
#define MRS_SRV_CCO_SYSTEM_RESET_TIMER_VALUE            (MRS_CCO_TIME_SYSTEM_RESET)
#define MRS_SRV_CCO_SEARCHMETER_TIME_VALUE              (MRS_CCO_TIME_SEARCHMETER)
#define MRS_SRV_CCO_TIMER_READMETER_INIT_ACTIV_VALUE    (10000)     // ����ģ���ʼ����ʱ��(�ϵ����ʱ5min)
#define MRS_SRV_CCO_TIMER_DEFAULT_TIMEOUT_VALUE         (90000)     // �泭�ȷ���Ĭ�ϳ�ʱʱ��
#define MRS_SRV_CCO_TIMER_SEARCH_REPORT_VALUE           (8000)      // �ѱ����ϱ����������ĳ�ʱʱ��,��λ:ms

#define MRS_CCO_UPG_CLEAR_FILE_TIMEOUT_VALUE            (800)  //�յ������װ�����ʱʱ��,��λ:ms
#define MRS_CCO_DETECT_MAC_CACHE_TIME_VALUE             (30 * 1000)  //APP�յ�MAC�ܾ�������Ϣ����󻺴��ʱ��
#define MRS_CCO_DETECT_REFRESH_WHITE_TIME_VALUE         (10 * 1000)  //̽�������֪ͨˢ�°��������ʱ��

#define MRS_CCO_TIME_SEARCH_METER_WHITELIST_DEFAULT     (0)
#define MRS_CCO_TIME_RESUME_DETECT_RM_DEFAULT           (3 * 1000)  //������͸��һ֡������ָ�̽�Ⳮ���ʱ��

#define MRS_CCO_TIME_MR_TASK_ACTIVE                     (1000)      // �������񼤻��������ͣ״̬��δ����ʱʹ��

#define MRS_SRV_STA_SYSTEM_RESET_TIMER_VALUE            (MRS_CCO_TIME_SYSTEM_RESET) // �����������ʱ��  ��λ:ms

#define MRS_CCO_RECOVER_POWER_AFE_TIME_MIN              (15) //15����
#define MRS_CCO_RECOVER_POWER_AFE_TIME_DEFAULT          (15) //15����
#define MRS_CCO_RECOVER_POWER_AFE_TIME_MAX              (30) //30����

#define MRS_CCO_LOW_POWER_RUN_TIME                      (5 * 60) //5���� CCO���г���5���Ӳ������ٽ��ͷ��书��

//*****************************************************************************
// CCO����״̬
//*****************************************************************************
#define MRS_CCO_STATE_CREATE_MAP                (0)     // ��ʼ��״̬��δ̽�Ⲩ����
#define MRS_CCO_STATE_NORMAL                    (1)     // ����̬������̬��STA�������������κ�����


//*****************************************************************************
// STA�Ķ�ʱ��
//*****************************************************************************

typedef enum
{
    MRS_STA_TIMER_TEST,
    MRS_STA_TIMER_HEART,                    // ������ʱ��(���ַ��ȡʧ��LED��˸��ʱ������:MRS_STA_TIMER_TEST״̬��ΪLED��ʱ��)
    MRS_STA_TIMER_QUEUE,
    MRS_STA_TIMER_TTL,
    MRS_STA_TIMER_IO_EVENT_OUT,             // IO����ģ����EVENT_OUT�ܽŲ鿴״̬��ʱ��ID
    MRS_STA_TIMER_RUN_LED,                  // II�Ͳɼ��������е���/��ʱ��
    MRS_STA_TIMER_UART_LED,                 // II�Ͳɼ�����UART�շ�����/��ʱ��
    MRS_STA_TIMER_IRDA_RESET,               // II�Ͳɼ��������Ӧ����֡Ӳ����ʼ����ʱ��
    MRS_STA_TIMER_EVENT_REPORT,
    MRS_STA_TIMER_SM_FRAME,                 // �ѱ�ȴ���Ӧ�����ֽڶ�ʱ��
    MRS_STA_TIMER_SM_BYTE,                  // ��Ӧ�������ֽڼ����ʱ��
    MRS_STA_TIMER_SM_VM,                    // ������ѹ��ܶ�ʱ��
    MRS_STA_TIMER_SM_PERIOD,                // �������ѱ�ʱ��
    MRS_STA_TIMER_SM_PERIOD_NO_METER,       // �ϵ�δ�ѵ������10�����ѱ�
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    MRS_STA_TIMER_DAYFREEZE_REC,            // �����Լ�¼�ն���ͳ�ƶ�ʱ��
#endif
    MRS_STA_TIMER_RESET_PIN,
    MRS_STA_TIMER_SIMU_CHL,
    MRS_STA_TIMER_CLTI_POSITIVE_A,          // I��: STA�ϵ���ϴ��������(+A)��ʱ��
    MRS_STA_TIMER_CLTI_SM_QUERY,            // I��: ��ѯ�ѱ�����ʱ��
    MRS_STA_TIMER_CLTI_SM_START,            // I��: �ѱ�ʼ֡��ʱ��
    MRS_STA_TIMER_CLTI_SM_STOP,             // I��: �ѱ�ֹͣ֡��ʱ��
    MRS_STA_TIMER_VM_EVT_ACTIVE,            // ģ����¼����ʱ��
    MRS_STA_TIMER_ZERO_CROSS,               // ������
    MRS_STA_TIMER_CLTII_EVENT,              // II���¼����
    MRS_STA_TIMER_CLTII_LOCAL_RM_INTERVAL,  // II�ɱ��س���֡���
    MRS_STA_TIMER_LOCAL_MONITOR,            // STA���ؼ�������ϱ�״̬��
    MRS_STA_TIMER_RM_METER_JOIN_NET,        // ����ɹ���ȴ�5��������ʱ��
    MRS_STA_TIMER_CSG_CLTI_SM_QUERY,        // �ѱ��ѯ
    MRS_STA_TIMER_CSG_CLTI_QUEUE,           // ���ж�ʱ��

    MRS_STA_TIMER_CSG_QRY_DETECT_STATUS,    // ��ѯ̽���ѱ�״̬

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    MRS_STA_TIMER_SET_CLT_MAC,
    MRS_STA_TIMER_CSG_CLTI_UPG,
    MRS_STA_TIMER_MODE_PHYSICAL_TEST,       //��������ģʽ
    MRS_STA_TIMER_ENTER_TESTMODE,           //�������ģʽ
    //MRS_STA_TIMER_MODE_FREQ_FELLOW,         //Ƶ�θ�������
    MRS_STA_TIMER_CSG_REBOOT,               //������ʱ��
#endif

    MRS_STA_TIMER_RM_FRAME_INTERVAL,        /* Frame interval timer. CNcomment: ֡�����ʱ����CNend */
    MRS_STA_TIMER_RM_QUEUE_LOCK,            // STA������5�����������������ƶ�ʱ��
    MRS_STA_TIMER_POWERFAILURE,             //ͣ���ϱ�sta��ʱ��

    MRS_STA_TIMER_NETWORK_LOCK_CHECK,       // �����󣬶�ʱ�������ʱ�������ʱ���Ƿ�ﵽ����
    MRS_STA_TIMER_REJOIN_AFTER_UNLOCK,      // ��ʱ���վ���Ƿ�������磬��������������

    MRS_STA_TIMER_MAX
} MRS_STA_LOGIC_TIMER_ID_E;

#define MRS_STA_TIMER_INIT                      (MRS_STA_TIMER_TEST)
#define MRS_COLLECTOR_TIMER_IRDA_RESET          (MRS_STA_TIMER_IRDA_RESET)

//*****************************************************************************
// STA��ʱ���Ķ�ʱ����
//*****************************************************************************
#define MRS_STA_TIME_TEST                       (1500)          // 1.5��
#define MRS_STA_TIME_INIT                       (1000*60*30)    // 30����
#define MRS_STA_TIME_HEART                      (1000*60*5)     // ���Ϊ5���� (liuxipeng KF54842)
#define MRS_STA_TIME_IO_EVENTOUT_VALUE          (50)
#define MRS_STA_TIME_RUN_LED_0_25HZ             (2000)          // II�Ͳɼ��������е���/��ʱ����ʱʱ��(0.25Hz)
#define MRS_STA_TIME_RUN_LED_0_50HZ             (1000)          // II�Ͳɼ��������е���/��ʱ����ʱʱ��(0.50Hz)
#define MRS_STA_TIME_RUN_LED_5_00HZ             (100)           // II�Ͳɼ��������е���/��ʱ����ʱʱ��(5.00Hz)
#define MRS_STA_TIME_UART_LED                   (100)           // II�Ͳɼ�����UART�շ�����/��ʱ����ʱʱ��
#define MRS_STA_TIME_GETMETER_ERR_LED           (1000)          // ���ַ��ȡʧ�ܺ�LED��˸��ʱ��ʱ��
#define MRS_STA_TIME_RESET_PIN                  (150)
#define MRS_COLLECTOR_TIME_IRDA_RESET           (500)           // II�Ͳɼ��������Ӧ����֡Ӳ����ʼ����ʱ����ʱʱ��
#define MRS_STA_TIME_SM_PERIOD_NO_METER         (1000*60*10)    // �������ѱ�ʱ��������10����

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
#define MRS_STA_TIME_DAYFREEZE_REC              (1000 * 60 * 5)  // �����Լ�¼�ն���ͳ�ƶ�ʱ����(ÿ5����ˢ��)
#endif

#define MRS_STA_TIME_ZERO_CROSS                 (800)
#define MRS_STA_TIME_RM_METER_JOIN_NET          (5 * 1000)  // ����ɹ���ȴ�������ʱ��
#define MRS_STA_TIME_RM_METER_JOIN_NET_DUT      (1 * 1000)  // ����I�ɳ�����װʱ����ɹ���ȴ�������ʱ��

#define MRS_STA_TIME_CSG_QRY_DETECT_STATUS      (15 * 1000)
#define MRS_STA_TIME_CSG_CLTI_UPG               (3000)

#define MRS_STA_TIME_RM_QUEUE_LOCK              (30 * 60 * 1000) //STA������30������������������


#define MRS_STA_TIME_NETWORK_LOCK_CHECK         (3 * 60 * 1000)     // ��������ʱ�����޼�����ڣ�3����
#define MRS_STA_TIME_REJOIN_AFTER_UNLOCK        (1 * 1000)          // �����ɹ�����������

//*****************************************************************************
// STA����״̬
//*****************************************************************************
#define MRS_STA_STATE_INIT                      (0)     // ��ʼ��״̬��δ̽�Ⲩ����
#define MRS_STA_STATE_MR                        (1)     // ����̬������̬��STA�������������κ�����

//*****************************************************************************
// STA������Դ����
//*****************************************************************************

#define MRS_STA_QUEUE_FROM_STA                  (0)     // ����
#define MRS_STA_QUEUE_FROM_PLC                  (1)     // PLC
#define MRS_STA_QUEUE_FROM_LOCAL                (2)     // HSO
#define MRS_STA_QUEUE_FROM_REMOTE               (3)     // ������(Զ�˴���)
#define MRS_STA_QUEUE_FROM_REMOTE_HSO           (4)     // ������(Զ��HSO)
#define MRS_STA_QUEUE_FROM_IR                   (5)     // ����

#define MRS_STA_TRANSFER_TIMING_NORMAL          (1)     // ͸��Уʱ����(08Уʱ)
#define MRS_STA_TRANSFER_TIMING_CLTII_EX        (2)     // ͸��Уʱ����(��չУʱ)
/* BEGIN: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */
#define MRS_IR_SUBID_SPECIAL_TRANSMIT           (3)     //II�ɺ�������DI͸������
#define MRS_IR_SUBID_TRANSMIT                   (4)     //II�ɺ�����ͨ͸������
/* END: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */
#define MRS_CLTI_EVT_QUERY_DISCARD              (5)     //������I���¼���ѯ����

//*****************************************************************************
// STA��������
//*****************************************************************************
#define MRS_STA_ITEM_ID_HEART                   (HI_U16)(-1)    // ����ID
#define MRS_STA_ITEM_ID_TRANSMIT                (HI_U16)(-2)    // ת������
#define MRS_STA_ITEM_ID_SEARCH_METER_CLT_II     (HI_U16)(-3)    // �ѱ�����
#define MRS_STA_ITEM_ID_SEARCH_METER_CLT_I      (HI_U16)(-4)    // I���ѱ�����
#define MRS_STA_ITEM_ID_SEND_WORK_MODE_CLT_II   (HI_U16)(-5)    // II�ɷ��͹���ģʽ(����ģʽ)
#define MRS_STA_ITEM_ID_EVENT                   (HI_U16)(-6)    // �¼���ѯ
#define MRS_STA_ITEM_ID_CLTII_EVENT             (HI_U16)(-7)    // II���¼����
#define MRS_STA_ITEM_ID_DUTY_CYCLE              (HI_U16)(-8)    // STA�͹��Ķ�ȡ���ʱ��
#define MRS_STA_ITEM_ID_SET_VER_CLT_I           (HI_U16)(-9)    // I��ģ�����ð汾�Ÿ�I��
#define MRS_STA_ITEM_ID_CLTI_EVENT              (HI_U16)(-10)   // I���¼���ѯ

#define MRS_STA_ITEM_ID_CSG_CLTI_SEARCH_METER   (HI_U16)(-11)
#define MRS_STA_ITEM_ID_CSG_TRANS_XR            (HI_U16)(-12)
#define MRS_STA_ITEM_ID_CSG_CLTI_UPG_DEVICE     (HI_U16)(-13)

#define MRS_STA_SEND_ADDR_FRAME_TIMES_MAX       (150)   // 150��
#define MRS_STA_SEND_ADDR_FRAME_TIMES_DEFAULT   (7)     // 7��
#define MRS_STA_SEND_ADDR_FRAME_TIMES_MIN       (0)     // 0��

#define MRS_STA_WAIT_SEND_TIME_MAX              (24 * 60 * 60)  // 24Сʱ
#define MRS_STA_WAIT_SEND_TIME_DEFAULT          (60)    // 60��
#define MRS_STA_WAIT_SEND_TIME_MIN              (1)     // 1��


//*****************************************************************************
// ��Ʒ����
//*****************************************************************************
#define MRS_SRV_PRODUCT_STA                     (0)
#define MRS_SRV_PRODUCT_II_COLLECTOR            (1)


//*****************************************************************************
// CCO����״̬��غ���
//*****************************************************************************
HI_PUBLIC HI_U32 mrsCcoSetState(HI_U32 state);
HI_PUBLIC HI_U32 mrsCcoGetState(HI_VOID);
HI_PUBLIC HI_VOID mrsCcoForcePlcConnect(HI_VOID);


//*****************************************************************************
// STA����״̬��غ���
//*****************************************************************************
HI_PUBLIC HI_U8 mrsStaSetState(HI_U8 state);
HI_PUBLIC HI_U8 mrsStaGetState(HI_VOID);


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#define MRS_SRV_TIMER_MAX   MRS_CCO_TIMER_MAX
#else
#define MRS_SRV_TIMER_MAX   MRS_STA_TIMER_MAX
#endif

HI_END_HEADER
#endif //__MRS_SRV_RES_H__

