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
#define MRS_CCO_TIMER_MAP                       (EN_APP_TASK_TIMER_MRS_1)   // ӳ���ά����ʱ��ID
#define MRS_CCO_TIMER_STA                       (EN_APP_TASK_TIMER_MRS_2)   // ��STAͨѶ��ʱ��ID
#define MRS_CCO_TIMER_MASTER                    (EN_APP_TASK_TIMER_MRS_3)   // �뼯����ͨѶ��ʱ��ID
#define MRS_CCO_TIMER_READMETER_ACTIVE          (EN_APP_TASK_TIMER_MRS_4)   // ����ģ�鼤�ʱ��
#define MRS_CCO_TIMER_SERVICE_END               (EN_APP_TASK_TIMER_MRS_5)   // ���������ʱ��(�����ѱ���泭�ȷ���Ľ���)
#define MRS_CCO_TIMER_FLASHWRITE_ACTIVE         (EN_APP_TASK_TIMER_MRS_6)   // �������̼��ʱ��ID
#define MRS_CCO_TIMER_GET_TIME                  (EN_APP_TASK_TIMER_MRS_7)   // ��ʱ��ȡ������ʱ�䶨ʱ��
#define MRS_CCO_TIMER_SYSTEM_RESET              (EN_APP_TASK_TIMER_MRS_8)   // AFN=01H-F1ִ��Ӳ��������ʱ��ID
#define MRS_CCO_TIMER_PARALLEL_CHECK            (EN_APP_TASK_TIMER_MRS_9)   // ��������PLC��ʱ��鶨ʱ��
/* BEGIN: PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
#define MRS_CCO_TIMER_UPG_WAIT_PLC_AVAILABLE    (EN_APP_TASK_TIMER_MRS_10)
/* END:   PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
#define MRS_CCO_TIMER_BROADCAST                 (EN_APP_TASK_TIMER_MRS_11)
#define MRS_CCO_TIMER_NODE_STATE_CHG_ACTIVE     (EN_APP_TASK_TIMER_MRS_12)
#define MRS_CCO_TIMER_EVENT_ACTIVE              (EN_APP_TASK_TIMER_MRS_13)
#define MRS_CCO_TIMER_MASTER_ACTIVE             (EN_APP_TASK_TIMER_MRS_14)
#define MRS_CCO_TIMER_MASTER_REPORT             (EN_APP_TASK_TIMER_MRS_15)
#define MRS_CCO_TIMER_RESET_PIN                 (EN_APP_TASK_TIMER_MRS_16)
#define MRS_CCO_TIMER_SIMU_CHL                  (EN_APP_TASK_TIMER_MRS_17)
#define MRS_CCO_TIMER_SEARCHMETER_SEND_FRAME    (EN_APP_TASK_TIMER_MRS_18)
#define MRS_CCO_TIMER_GET_METERLIST             (EN_APP_TASK_TIMER_MRS_19)
#define MRS_CCO_TIMER_TOPO_QUERY                (EN_APP_TASK_TIMER_MRS_20)
#define MRS_CCO_TIMER_CHECK_MAC_MAP_TTL         (EN_APP_TASK_TIMER_MRS_21)
#define MRS_CCO_TIMER_MAP_REFRESH               (EN_APP_TASK_TIMER_MRS_22)
#define MRS_CCO_TIMER_SEARCH_METER_END          (EN_APP_TASK_TIMER_MRS_23)
/* BEGIN: Added by fengxiaomin/00209182, 2014/1/13   ���ⵥ�ź�:DTS2014012607684 */
#define MRS_CCO_TIMER_UPG_FILE_RECEIVE          (EN_APP_TASK_TIMER_MRS_26)  //������װ�׶ν��յ��������ݳ�ʱ��ʱ��
#define MRS_CCO_TIMER_UPG_STOP_RETRY_ACTIVE     (EN_APP_TASK_TIMER_MRS_27)  //ֹͣ�����ط���ʱ��
#define MRS_CCO_TIMER_UPG_STATUS_QUERY_ACTIVE   (EN_APP_TASK_TIMER_MRS_28)  //����״̬��ѯ��ʱ��
#define MRS_CCO_TIMER_UPG_STATUS_QUERY_TIMEOUT  (EN_APP_TASK_TIMER_MRS_29)  //����״̬��ѯ��ʱ��ʱ��
/* END:   Added by fengxiaomin/00209182, 2014/1/13 */
#define MRS_CCO_TIMER_NODE_STATECHG_ENTRY_CACHE (EN_APP_TASK_TIMER_MRS_30)
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
#define MRS_CCO_TIMER_ACQUIRE_CLOCK             (EN_APP_TASK_TIMER_MRS_31)
#define MRS_CCO_TIMER_MRS_LOG                   (EN_APP_TASK_TIMER_MRS_32)
#endif
#define MRS_CCO_TIMER_SEARCH_METER_END_TF       (EN_APP_TASK_TIMER_MRS_33)
#define MRS_CCO_TIMER_SEARCH_METER_WHITELIST    (EN_APP_TASK_TIMER_MRS_34)
/* BEGIN: Added by fengxiaomin/00209182, 2015/9/7   ���ⵥ�ź�:DTS2015091503701 */
#define MRS_CCO_TIMER_SEARCH_REPORT             (EN_APP_TASK_TIMER_MRS_35)
/* END:   Added by fengxiaomin/00209182, 2015/9/7 */
#define MRS_CCO_TIMER_TEST_MODE_FRAME           (EN_APP_TASK_TIMER_MRS_36)
#define MRS_CCO_TIMER_ZERO_CROSS                (EN_APP_TASK_TIMER_MRS_37)
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
#define MRS_CCO_TIMER_RUN_LED_BBU               (EN_APP_TASK_TIMER_MRS_38)
#endif

#define MRS_CCO_TIMER_DETECT_WHITE_LIST_KEEP    (EN_APP_TASK_TIMER_MRS_39) //����mac��ַ�����Ĳɼ�����������������30������û�м���������Ĳɼ���վ������,ɾ����ӵİ�����
#define MRS_CCO_TIMER_DETECT_REJECT_CLT_CACHE   (EN_APP_TASK_TIMER_MRS_40) //̽�Ⳮ������л���ܾ�վ����Ϣ��ʱ��
#define MRS_CCO_TIMER_DETECT_WAIT_TOPO          (EN_APP_TASK_TIMER_MRS_41) //̽������еȴ���������,һ��ʱ�����������ֻ��CCO,�������ܾ���վ����������
#define MRS_CCO_TIMER_DETECT_JOIN_NET_CACHE     (EN_APP_TASK_TIMER_MRS_42) //̽������л�����׷�Ӱ������е�վ��������ʱ��,��ʱ������̽�Ⳮ��
#define MRS_CCO_TIMER_DETECT_BLACK_LIST         (EN_APP_TASK_TIMER_MRS_43) //��������ʱ����ʱ��
#define MRS_CCO_TIMER_DETECT_REFRESH_WHITE      (EN_APP_TASK_TIMER_MRS_44) //̽�Ⳮ�������֪ͨˢ�°�������ʱ��
#define MRS_CCO_TIMER_RESUME_DETECT_RM          (EN_APP_TASK_TIMER_MRS_45) //̽�Ⳮ�������֪ͨˢ�°�������ʱ��
#define MRS_CCO_TIMER_ENTER_TESTMODE            (EN_APP_TASK_TIMER_MRS_46) //�������ģʽ
#define MRS_CCO_TIMER_TESTMODE                  (EN_APP_TASK_TIMER_MRS_47) //����ģʽ
#define MRS_CCO_TIMER_PHYSICAL_TESTMODE         (EN_APP_TASK_TIMER_MRS_48) //ccy ҵ�񴮿ڲ���ģʽ ��ʱ��
#define MRS_CCO_TIMER_MSDU_TESTMODE             (EN_APP_TASK_TIMER_MRS_49) //cco msdu�ϱ�ҵ�񴮿ڲ��Զ�ʱ��
#define MRS_CCO_TIMER_POWERFAILURE              (EN_APP_TASK_TIMER_MRS_50)
#define MRS_CCO_TIMER_POWERFAILURE_ERASE_DATA   (EN_APP_TASK_TIMER_MRS_51) //cco�����ϱ����ݶ�ʱ��
#define MRS_CCO_TIMER_NOTIFY_STA_LOCK_NETWORK   (EN_APP_TASK_TIMER_MRS_52) //֪ͨվ�������
#define MRS_CCO_TIMER_CHECK_MAIN_NODE_ADDR      (EN_APP_TASK_TIMER_MRS_53) //������ڵ��ַ
#define MRS_CCO_TIMER_TF_RESULT_QUERY_NEXT      (EN_APP_TASK_TIMER_MRS_54) //̨��ʶ������ѯ: ��һ�β�ѯ��ʼ��ʱ��
#define MRS_CCO_TIMER_TF_RESULT_QUERY_ROUND     (EN_APP_TASK_TIMER_MRS_55) //̨��ʶ������ѯ: ��һ�ֲ�ѯ��ʼ��ʱ��
#define MRS_CCO_TIMER_TF_RESULT_QUERY_BEGIN     (EN_APP_TASK_TIMER_MRS_56) //̨��ʶ������ѯ: ��ʼ��ʱ��
#define MRS_CCO_TIMER_POWEROFF_DETECT           (EN_APP_TASK_TIMER_MRS_57) //ͣ��̽�ⶨʱ��
#define MRS_CCO_TIMER_POWEROFF_RECHECK          (EN_APP_TASK_TIMER_MRS_58) //ͣ�縴�˶�ʱ��
#define MRS_CCO_TIMER_POWEROFF_STABLE_CHECK     (EN_APP_TASK_TIMER_MRS_59) //ͣ��ģ��վ���ȶ��Լ��
#define MRS_CCO_TIMER_POWEROFF_MAC_CACHE        (EN_APP_TASK_TIMER_MRS_60) //ͣ��ģ��MAC���涨ʱ��
#define MRS_CCO_TIMER_POWEROFF_DEAL_SCALE_OFF   (EN_APP_TASK_TIMER_MRS_61) //ͣ��ģ�鴦����ģͣ��(̽��վ����Ŀ����10����ʱ�������ö�ʱ��)
#define MRS_CCO_TIMER_NDM_PLC_READ_METER        (EN_APP_TASK_TIMER_MRS_63) //�������ͼ�����ͬʱ����cco�㳭����ʱ����������������ʱ��
#define MRS_CCO_TIMER_STA_PLC_READ_DEV_ID       (EN_APP_TASK_TIMER_MRS_64) //ͨ��cco��ȡsta��I�ɣ�II���豸����ID��Ϣ���ȴ�plc֡���ض�ʱ��
#define MRS_CCO_TIMER_CLT_POWER_REPORT          (EN_APP_TASK_TIMER_MRS_65) //�ɼ���ͣ���ϵ��ϱ�
#define MRS_CCO_TIMER_CLT_POWEOFF_ERASE_DATA    (EN_APP_TASK_TIMER_MRS_66) //cco�����ɼ����½ӱ��ϱ����ݶ�ʱ��
#define MRS_CCO_TIMER_POWERON                   (EN_APP_TASK_TIMER_MRS_67) // cco�����ϱ���ʱ��
#define MRS_CCO_TIMER_POWERON_ERASE_DATA   		(EN_APP_TASK_TIMER_MRS_68) //cco�����ϱ����ݶ�ʱ��
#define MRS_CCO_TIMER_RF_UART_TIME      		(EN_APP_TASK_TIMER_MRS_69) //STA RF ��ʱ��
#define MRS_CCO_TIMER_RF_TIME      				(EN_APP_TASK_TIMER_MRS_70) //CCO RF ��ʱ��
#define MRS_CCO_TIMER_RF_READ_WATERMETER    	(EN_APP_TASK_TIMER_MRS_71) //��������ˮ��ʱ��
#define MRS_CCO_TIMER_GET_WATER_METERLIST       (EN_APP_TASK_TIMER_MRS_72) // ˮ���ѱ�
#define MRS_CCO_TIMER_SET_WATER_METERLIST_SHIP  (EN_APP_TASK_TIMER_MRS_73) // ����ˮ���ϵ
#define MRS_CCO_TIMER_SGD_RF_TIME               (EN_APP_TASK_TIMER_MRS_74) //CCO ���� RF ��ʱ��
//Matteo
#define MRS_CCO_TIMER_ARCHIVE_CHANGE_CHECK      (EN_APP_TASK_TIMER_MRS_81)

#define MRS_CCO_TIMER_MAX                       (EN_APP_TIMER_MAX)

#define EN_MRS_SRV_CCO_MAP_VIN_TIMER            (MRS_CCO_TIMER_MAP)
#define EN_MRS_SRV_CCO_STA_TIMER                (MRS_CCO_TIMER_STA)
#define EN_MRS_SRV_CCO_MUSTER_TIMER             (MRS_CCO_TIMER_MASTER)
#define EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER    (MRS_CCO_TIMER_READMETER_ACTIVE)
#define EN_MRS_SRV_CCO_SERVICE_END_TIMER        (MRS_CCO_TIMER_SERVICE_END)
#define EN_MRS_SRV_CCO_WRITE_FLASH_ACTIV_TIMER  (MRS_CCO_TIMER_FLASHWRITE_ACTIVE)
#define EN_MRS_SRV_CCO_GET_TIME_TIMER           (MRS_CCO_TIMER_GET_TIME)
#define EN_MRS_SRV_CCO_SYSTEM_RESET_TIMER       (MRS_CCO_TIMER_SYSTEM_RESET)
#define EN_MRS_SRV_CCO_RM_CHECK_TIMER           (MRS_CCO_TIMER_PARALLEL_CHECK)
/* BEGIN: PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
//#define EN_MRS_SRV_CCO_SEARCHMETER_TIMER        (MRS_CCO_TIMER_SEARCHMETER)
/* END:   PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
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

/* BEGIN: PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/11/11 */
#define MS_PER_SECOND                           (1000)      //��ʱ��������λ  1s
#define MRS_CCO_TIME_FLASHWRITE_ACTIVE          (20)     // �������̼���ʱ��  Ĭ��20s
/* END: PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/11/11 */

#define MRS_CCO_TIME_SYSTEM_RESET               (500)       // AFN=01H-F1ִ��Ӳ��������ʱʱ��
/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */
#define MRS_CCO_TIME_PARALLEL_CHECK             (50)       // ��������ʱ��鶨ʱ��Ĭ��ֵ(ÿ�����ʱ����һ�γ�ʱ���)
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */
#define MRS_CCO_TIME_SEARCHMETER                (2000)
#define MRS_CCO_TIME_RESET_PIN                  (150)

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
#define MRS_CCO_TIME_ACQUIRE_CLOCK              (1500)
#define MRS_CCO_TIME_CHECK_CLOCK                (1000*60)
#define MRS_CCO_TIME_STARTUP_CLOCK              (1000*60*20) //CCO�ϵ�20���Ӻ�ʼ��ѯ������ʱ�ӵ�ʱ��
#define MRS_CCO_TIME_ACTIVE_ACQUIRE_CLOCK       (200) //CCO�յ�·��������·�ɻָ�����󣬼�����ѯ������ʱ�ӵ�ʱ��
//#define MRS_CCO_TIME_GET_CLOCK                  (1000 * 8)//CCO�ϵ��׶���ѯʱ���뼯����֮��ĳ�ʱʱ��
#endif

#define MRS_SRV_CCO_TIMER_MUSTER_RETRY_NUM_DEFAULT      (2)             // ������ͨѶĬ��������Դ���
#define MRS_SRV_CCO_TIMER_MUSTER_TIMEOUT_DEFAULT        (1000 * 10)     // ������ͨѶĬ�ϳ�ʱʱ��(ģ��ͨ���ر�),��λ:ms

#define MRS_SRV_CCO_TIMER_MUSTER_SIMU_CHL_TIMEOUT_DEFAULT  (1000 * 60)   // ������ͨѶĬ�ϳ�ʱʱ��(ģ��ͨ����),��λ:ms

#define MRS_SRV_CCO_TIMER_MUSTER_VALUE                  (MRS_CCO_TIME_MASTER)
#define MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE         (MRS_CCO_TIME_READMETER_ACTIVE)
/* BEGIN: Added by fengxiaomin/00209182, 2015/9/7   ���ⵥ�ź�:DTS2015091503701 */
#define MRS_SRV_CCO_TIMER_SEARCH_ACTIV_VALUE            (MRS_CCO_TIME_READMETER_ACTIVE)
/* END:   Added by fengxiaomin/00209182, 2015/9/7 */
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
#define MRS_CCO_TIME_RESUME_DETECT_RM_DEFAULT           (3 * 1000) //������͸��һ֡������ָ�̽�Ⳮ���ʱ��
#define MRS_CCO_TIME_CLT_POWER_REPORT_VALUE             (5 *1000)  //CCO�յ��ɼ���ͣ�ϵ籨�ĺ�,��۵ȴ��ϱ���������ʱ��
//*****************************************************************************
// CCO����״̬
//*****************************************************************************
#define MRS_CCO_STATE_CREATE_MAP                (0)     // ��ʼ��״̬��δ̽�Ⲩ����
#define MRS_CCO_STATE_NORMAL                    (1)     // ����̬������̬��STA�������������κ�����


//*****************************************************************************
// STA�Ķ�ʱ��
//*****************************************************************************
#define MRS_STA_TIMER_TEST                      (EN_APP_TASK_TIMER_MRS_1)
#define MRS_STA_TIMER_INIT                      MRS_STA_TIMER_TEST
#define MRS_STA_TIMER_HEART                     (EN_APP_TASK_TIMER_MRS_2)   // ������ʱ��(���ַ��ȡʧ��LED��˸��ʱ������:MRS_STA_TIMER_TEST״̬��ΪLED��ʱ��)
#define MRS_STA_TIMER_QUEUE                     (EN_APP_TASK_TIMER_MRS_3)
#define MRS_STA_TIMER_TTL                       (EN_APP_TASK_TIMER_MRS_4)
#define MRS_STA_TIMER_IO_EVENT_OUT              (EN_APP_TASK_TIMER_MRS_5)   // IO����ģ����EVENT_OUT�ܽŲ鿴״̬��ʱ��ID
#define MRS_STA_TIMER_RUN_LED                   (EN_APP_TASK_TIMER_MRS_6)   // II�Ͳɼ��������е���/��ʱ��
#define MRS_STA_TIMER_UART_LED                  (EN_APP_TASK_TIMER_MRS_7)   // II�Ͳɼ�����UART�շ�����/��ʱ��
#define MRS_STA_TIMER_IRDA_RESET                (EN_APP_TASK_TIMER_MRS_8)   // II�Ͳɼ��������Ӧ����֡Ӳ����ʼ����ʱ��
#define MRS_STA_TIMER_EVENT_REPORT              (EN_APP_TASK_TIMER_MRS_9)
#define MRS_STA_TIMER_ZERO_CROSS_ADC            (EN_APP_TASK_TIMER_MRS_10)
#define MRS_STA_TIMER_SM_FRAME                  (EN_APP_TASK_TIMER_MRS_11)  // �ѱ�ȴ���Ӧ�����ֽڶ�ʱ��
#define MRS_STA_TIMER_SM_BYTE                   (EN_APP_TASK_TIMER_MRS_12)  // ��Ӧ�������ֽڼ����ʱ��
#define MRS_STA_TIMER_SM_VM                     (EN_APP_TASK_TIMER_MRS_13)  // ������ѹ��ܶ�ʱ��
#define MRS_STA_TIMER_SM_PERIOD                 (EN_APP_TASK_TIMER_MRS_14)  // �������ѱ�ʱ��
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
#define MRS_STA_TIMER_DAYFREEZE_REC             (EN_APP_TASK_TIMER_MRS_15)  // �����Լ�¼�ն���ͳ�ƶ�ʱ��
#endif
#define MRS_STA_TIMER_RESET_PIN                 (MRS_CCO_TIMER_RESET_PIN)   // EN_APP_TASK_TIMER_MRS_16
#define MRS_STA_TIMER_SIMU_CHL                  (MRS_CCO_TIMER_SIMU_CHL)    // EN_APP_TASK_TIMER_MRS_17
#define MRS_STA_TIMER_CLTI_POSITIVE_A           (EN_APP_TASK_TIMER_MRS_18)  // I��: STA�ϵ���ϴ��������(+A)��ʱ��
#define MRS_STA_TIMER_CLTI_SM_QUERY             (EN_APP_TASK_TIMER_MRS_19)  // I��: ��ѯ�ѱ�����ʱ��
#define MRS_STA_TIMER_CLTI_SM_START             (EN_APP_TASK_TIMER_MRS_20)  // I��: �ѱ�ʼ֡��ʱ��
#define MRS_STA_TIMER_CLTI_SM_STOP              (EN_APP_TASK_TIMER_MRS_21)  // I��: �ѱ�ֹͣ֡��ʱ��
#define MRS_STA_TIMER_RM_FRAME_INTERVAL         (EN_APP_TASK_TIMER_MRS_22)  // Frame interval timer
#define MRS_STA_TIMER_VM_EVT_ACTIVE             (EN_APP_TASK_TIMER_MRS_23)  // ģ����¼����ʱ��
#define MRS_STA_TIMER_ZERO_CROSS                (EN_APP_TASK_TIMER_MRS_24)  // ������
#define MRS_STA_TIMER_CLTII_EVENT               (EN_APP_TASK_TIMER_MRS_25)  // II���¼����
#define MRS_STA_TIMER_CLTII_LOCAL_RM_INTERVAL   (EN_APP_TASK_TIMER_MRS_26)  // II�ɱ��س���֡���
#define MRS_STA_TIMER_LOCAL_MONITOR             (EN_APP_TASK_TIMER_MRS_27)  // STA���ؼ�������ϱ�״̬��
#define MRS_STA_TIMER_SM_PERIOD_NO_METER        (EN_APP_TASK_TIMER_MRS_28)  // �ϵ�δ�ѵ������10�����ѱ�
#define MRS_STA_TIMER_RM_METER_JOIN_NET         (EN_APP_TASK_TIMER_MRS_29)  // ����ɹ���ȴ�5��������ʱ��
#define MRS_STA_TIMER_CLTI_UPG                  (EN_APP_TASK_TIMER_MRS_30)
#define MRS_STA_TIMER_ENTER_TESTMODE            (EN_APP_TASK_TIMER_MRS_31)  // �������ģʽ
#define MRS_STA_TIMER_TESTMODE                  (EN_APP_TASK_TIMER_MRS_32)  // ����ģʽ
#define MRS_STA_TIMER_PHYSICAL_TESTMODE         (EN_APP_TASK_TIMER_MRS_33)  // �����ת��
#define MRS_STA_TIMER_MSDU_TESTMODE             (EN_APP_TASK_TIMER_MRS_34)  // MSDUת��
#define MRS_STA_TIMER_POWERFAILURE              (EN_APP_TASK_TIMER_MRS_35)
#define MRS_STA_TIMER_NETWORK_LOCK_CHECK        (EN_APP_TASK_TIMER_MRS_36)  // �����󣬶�ʱ�������ʱ�������ʱ���Ƿ�ﵽ����
#define MRS_STA_TIMER_REJOIN_AFTER_UNLOCK       (EN_APP_TASK_TIMER_MRS_37)  // ��ʱ���վ���Ƿ�������磬��������������
#define MRS_STA_TIMER_TEST_MODE_FRAME           (EN_APP_TASK_TIMER_MRS_38)  // ���Ͳ���ģʽ����
#define MRS_STA_TIMER_RM_QUEUE_LOCK             (EN_APP_TASK_TIMER_MRS_39)  // STA������30�����������������ƶ�ʱ��
#define MRS_STA_TIMER_WAIT_POWERFAILURE_ACK     (EN_APP_TASK_TIMER_MRS_40)  // �ȴ�ͣ��ack��ʱ��
#define MRS_STA_TIMER_POWERON_PLC_SEND          (EN_APP_TASK_TIMER_MRS_41)
#define MRS_STA_TIMER_CLT_POWEROFF_WAIT_METER   (EN_APP_TASK_TIMER_MRS_42)
#define MRS_STA_TIMER_CLT_SEND_PLC              (EN_APP_TASK_TIMER_MRS_43)
#define MRS_STA_TIMER_COLLECT_VOLTAGE_BEGIN     (EN_APP_TASK_TIMER_MRS_44)  // ��Ƶ��ѹ�ɼ���ʼ��ʱ��
#define MRS_STA_TIMER_COLLECT_VOLTAGE           (EN_APP_TASK_TIMER_MRS_45)  // ��Ƶ��ѹ�ɼ���ʱ��
#define MRS_STA_TIMER_COLLECT_FREQENCY_BEGIN    (EN_APP_TASK_TIMER_MRS_46)  // ��ƵƵ�ʲɼ���ʼ��ʱ��
#define MRS_STA_TIMER_COLLECT_FREQENCY          (EN_APP_TASK_TIMER_MRS_47)  // ��ƵƵ�ʲɼ���ʱ��
#define MRS_STA_TIMER_RF_TIME      				(EN_APP_TASK_TIMER_MRS_57) //STA RF ��ʱ��
#define MRS_STA_TIMER_POWERON              		(EN_APP_TASK_TIMER_MRS_69)
#define MRS_STA_PULL_RST_DELAY_TIMER	 		 EN_APP_TASK_TIMER_MRS_70	// STA�ӳ���Ӧ����rst�ܽŶ�ʱ��
#define MRS_STA_DISABLE_POWER_FAILURE_TIMER		 EN_APP_TASK_TIMER_MRS_71	// TSTA �ر�ͣ���ⶨʱ��
//CJJ-HPLC:2018-9-7
#define MRS_STA_TIMER_RF_OVER_TIME               (EN_APP_TASK_TIMER_MRS_80)
//CJJ-HPLC:2018-9-7 
#define MRS_STA_TIMER_MAX                       (EN_APP_TIMER_MAX)

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
#define MRS_STA_TIME_UART_LED_TEST              (20)            // II�Ͳɼ�����UART�շ�����/��ʱ����ʱʱ��
#define MRS_STA_TIME_UART_LED_NORMAL            (100)           // II�Ͳɼ�����UART�շ�����/��ʱ����ʱʱ��
#define MRS_STA_TIME_GETMETER_ERR_LED           (1000)          // ���ַ��ȡʧ�ܺ�LED��˸��ʱ��ʱ��
#define MRS_STA_TIME_RESET_PIN                  (150)
#define MRS_COLLECTOR_TIME_IRDA_RESET           (500)           // II�Ͳɼ��������Ӧ����֡Ӳ����ʼ����ʱ����ʱʱ��
#define MRS_STA_TIME_SM_PERIOD_NO_METER         (1000*60*10)    // �������ѱ�ʱ��������10����

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
#define MRS_STA_TIME_DAYFREEZE_REC              (1000 * 60 * 5)  // �����Լ�¼�ն���ͳ�ƶ�ʱ����(ÿ5����ˢ��)
#endif

#define MRS_STA_TIME_ZERO_CROSS                 (800)
#define MRS_STA_TIME_RM_METER_JOIN_NET          (5 * 1000)          // ����ɹ���ȴ�������ʱ��
#define MRS_STA_TIME_RM_METER_JOIN_NET_DUT      (1 * 1000)          // ����I�ɳ�����װʱ����ɹ���ȴ�������ʱ��
#define MRS_STA_TIME_CLTI_UPG                   (1000)
#define MRS_STA_TIME_NETWORK_LOCK_CHECK         (3 * 60 * 1000)     // ��������ʱ�����޼�����ڣ�3����
#define MRS_STA_TIME_REJOIN_AFTER_UNLOCK        (1 * 1000)          // �����ɹ�����������

#define MRS_STA_TIME_RM_QUEUE_LOCK              (30 * 60 * 1000)    // STA������30������������������


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
#define MRS_STA_QUEUE_FROM_RF                   (6)     // ����

#define MRS_STA_TRANSFER_TIMING_NORMAL          (1)     // ͸��Уʱ����(08Уʱ)
#define MRS_STA_TRANSFER_TIMING_CLTII_EX        (2)     // ͸��Уʱ����(��չУʱ)
/* BEGIN: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */
#define MRS_IR_SUBID_SPECIAL_TRANSMIT           (3)     //II�ɺ�������DI͸������
#define MRS_IR_SUBID_TRANSMIT                   (4)     //II�ɺ�����ͨ͸������
/* END: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */
#define MRS_CLTI_EVT_QUERY_DISCARD              (5)     //������I���¼���ѯ����

#define MRS_RF_SUBID_TRANSMIT           (6)     // ����
#define MRS_RF_SUBID_TRANSMIT_TOOL           (7)     // ���� ��װ����

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
#define MRS_STA_ITEM_ID_DETECT                  (HI_U16)(-11)   // STA�ϵ���,��ȡ���ַ
/* BEGIN: Modified by cuichunyan/c00276962 2016/11/10  ���ⵥ��:cltii R485 debug */
#define MRS_STA_ITEM_ID_TRAN_DATA_CLT_II        (HI_U16)(-12)   // II��485������
#define MRS_STA_ITEM_ID_RM_CLT_II               (HI_U16)(-13)   // II��try�ѱ����Ƿ���Գ�ͨ

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


HI_END_HEADER
#endif //__MRS_SRV_RES_H__

