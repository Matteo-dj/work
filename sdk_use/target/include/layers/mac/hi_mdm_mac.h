/******************************************************************************
*             Copyright (C) 2018 - 2018, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_mdm_mac.h
* Description��������·����ο����ӿڡ�
*
* History:
* Version              Date                      DefectNum           Description
* V200R001C00SPC010    2018-03-31                NA                  ��һ�ΰ汾������
* V200R001C00SPC011    2018-05-23                NA                  ����V200��Դ���ο����ӿڡ�
* V200R001C00SPC012    2018-05-24--2018-07-20    DTS2018071704905    ����:��ѹ������ؽӿڡ��ж�ģ���Ƿ�Ϊ��׼�����·����ȡ/���ù���ID�ӿڡ�ץ����غ궨�弰�ӿڡ���վ���Ƿ������ӿڡ����ο����ӿ�ע�͡�
                                                 DTS2018070406515         ����Э��궨�塢�ɼ�����ַ�����ڵ��ַ�궨�塢��������ģʽ���塢����Э�����ù���Ƶ�νӿڡ����ع�����V100Э�顢����������V100Э�顣
                                                 DTS2018062205983    �޸�:ע��ץ���ص������ӿڵĲ�����PLCͨ������ʱ���˱��״̬4����Ӧ�����ṹ��������mac��ַ��Ϣ��
                                                 DTS2018061501525
                       2018-08-02                DTS2018080209741    ����:����nv�е�snidΪ0�����õ�ǰ����Ƶ�εķ��书�ʡ�������λģʽ�ӿ�
                                                 DTS2018072807719    �޸�:PLCͨ������ʱ���˱��״̬�еı��״̬��Ϊ���ߺ�����״̬���������ù���Ƶ��˵��ע��
* V200R001C00SPC020    2018-08-04                DTS2018080213757    �����������ڵ��ַ״̬���޸İ������仯ԭ���룬�޸���������ʶ�𿪹ؽӿ�Ϊ��������ʶ��ģʽ�ӿڣ�������ѯSNR̨��ʶ�����ӿڣ������������ڵ��ַ�ӿڣ�
* V200R001C00SPC030    2018-08-22                DTS2018082011809    ������ѯ��Դģʽ�ӿڣ�������Դ�û�����ֹͣЭ��̽��ӿڣ����������ű귢�ͽӿڣ�������ȡ̽��оƬ�汾��Ϣ�ӿڣ��������̽��оƬ��Ϣ�ӿڣ�
                                                                     ��������STAվ���ж�����ʱ��ӿڣ���������CCO������Ϊ�����ӿڣ�������Դ�û��ж�����δ�������߽ӿڣ�����̨��ʶ��Ĵ������ؽӿڣ������ϵ��Լ칦��ע��ӿڣ�
                                                                     ����������λʶ����ע��ӿڣ���������NTBʶ����ע��ӿڣ����������ͣ���ϱ�����ע��ӿڣ��޸�CCO��������Ȩע��˵��ע�ͣ��޸�����������ʱʱ��ӿڡ�
                                                 DTS2018081401152    �޸���������Ƶ�νӿ��е�����жϼ��ӿ�˵��ע��
                       2018-09-10         DTS2018090702209    �޸����ûָ�V200Э�鿪�ؽӿ�ע�ͣ��޸Ļ�ȡV200Э�鿪�ؽӿ�ע�ͣ��޸Ĳ��ֽӿ�ע�͡�
                       2018-09-12         DTS2018091210477    ɾ��HI_MDM_SetAppLedSwitch�ӿڣ��޸��շ�ָʾ������ģʽ�ӿ�ע�ͣ��޸�ָʾ��״̬�ӿ�ע�͡�
                       2018-09-18         DTS2018091810261    �������ù���ģʽ������Ч�ӿڡ�
                       2018-09-21         DTS2018091710031    ����MAC��ͨ��ע��ͷ��ͽӿڡ�Э��ջ��ά�ɲ�ͳ������ȡ�ӿڡ��������߼�����ڸ����ӿڡ������������ýӿڡ��ͻ��Զ����ű���Ŀ�ӿڡ�������������Ƚӿڡ�

* V200R001C00SPC050    2018-11-10         DTS2018110703483    ������������̨��ʶ��SNR����ӿڡ���������̨��ʶ��NTB�ֲ�ʽ����ӿڡ��������̨��ʶ��NTBʶ�����ӿڡ�����վ���������ӿڡ���������̨��ʶ��NTB�������̿��ƽӿڡ�
                                                              ������ȡ��λʶ�����ӿڡ�����վ�����λʶ�����ӿڡ�

* V200R001C00SPC050B011    2018-12-03   DTS2018112903669    ���ӽӿ�֧���źŷ��͡�
* V200R001C00SPC050B012    2018-12-20   DTS2018122005101    ����Զ��ͨѶ�ն˽���ģ��ע��ӿڡ��Ƿ��������վ��
* V200R001C00SPC050B012    2018-12-26   DTS2018122107640    �޸Ĳɼ�����NTB�ӿڡ�����ָʾ����˸���ƽӿ�
* V200R001C00SPC050B012    2018-12-26   DTS2018121707454    ����оƬID̽�⹦�ܡ�
* V200R001C00SPC050B013    2019-01-09   DTS2019010904749    ������ȡ�״�������ɵ�ʱ���ӿ�
                                                            ������ȡ����������ʱ���ӿ�
                                                            ������ȡ���˱�������ӿ�
                                                            ������ȡ·�����ڡ�����վ�㷢���б������ڡ�����վ�㷢���б������ڽӿڡ�
                                                            ��������MAC��ַ��ȡվ�����ID��Ϣ�ӿ�
                                                            �޸Ļ�ȡ���˽ṹ���ֶ�
* V200R001C00SPC050B014    2019-01-23   DTS2019012104349    �������ù�����ʧ��Ƶ���ڽӿ�
                                                            ������ȡ������ʧ��Ƶ���ڽӿ�
                                                            ����ģ��γ��ص�����
                                                            ����CCO �����ϱ�������ָ��¼��ص�
                                        DTS2019012410663    ������Ƶ������ظ����궨��
                                                            �޸�����̨��ʶ��������ṹ�ֶ�
                                                            ����̨��ʶ��ģ��ص����������ͼ��ص����ݽṹ
                                                            ����ע��̨��ʶ��ص������ӿ�
                                                            ����֪ͨSTA�ɼ�NTB���ݽӿ�
                                                            ����֪ͨ����NTB���ݽӿ�
                                                            �������ý�����̨���㲥APP���Ŀ��ƽӿ�
                                                            �޸�PLCͨ�����Ͳ����ֶ�
                                         2019-02-01  DTS2019012104349
                                                            �������õ�������ģ��ӿ�
                                                            �������õ�������ģ���ѹ���������ӿ�
                                                            ����������ʧ��ѹδ����ص��¼��ϱ�
                                         2019-02-01  DTS2019010900227  ����������������λB,Cʶ����
                                                            �����ж������ģ��֧�ֹ������ͽӿ�
                                                            ����������λʶ�����֪ͨ�ӿ�
                                                            �޸Ĺ�װ�ɼ�NTB�ӿ�
* V200R001C00SPC050B019    2019-02-21   DTS2019010504405    �������ر��Ƿ����ǿ��ӿ�
                                                            ����CCO��������·������ӿ�
* V200R001C00SPC051B012    2019-03-11   DTS2019031104604    ����CCO�����ϱ�վ����������
                                                            ����CCO��ȡ�ϱ�վ����������
                                                            
******************************************************************************/

/**@defgroup hct_mac ������·��
 * @ingroup mdm
 */

#ifndef __HI_MDM_MAC_H__
#define __HI_MDM_MAC_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
HI_START_HEADER

/**
 *@ingroup hct_mac
 * IO�ܽŶ���
 */
typedef enum
{
    HI_LED_ID0 = 0,     /**< CCO��B��λ���͵�,�����⴮�ڽ���ָʾ��             */
    HI_LED_ID1,           /**< CCO��C��λ���͵�,�����⴮�ڷ���ָʾ��             */
    HI_LED_ID2,          /**< CCO��A��λ���͵�             */
    HI_LED_ID3,          /**< ����ָʾ��                          */
    HI_LED_ID4,          /**< ���͵ƣ�(II��Ϊ485��)      */
    HI_LED_ID5,          /**<������澯ָʾ��           */
    HI_LED_ID6,          /**<�����A��λ������ʾ    */
    HI_LED_ID7,         /**<�����B��λ������ʾ     */
    HI_LED_ID8,         /**<�����C��λ������ʾ     */
    HI_LED_MAX_ID,  /**<��ʾ�Ƿ�ID              */
}HI_LED_ID_E;

/**
 *@ingroup hct_mac
 * TONEMASK��size
 */
#define HI_MAX_TONEMASK_NUM                    16
/**
 *@ingroup hct_mac
 * TONEMASK��ģʽ����Ӧ������ȫƵ��ʹ��
 */
#define TONE_MASK_MODE_1                          0x00
/**
 *@ingroup hct_mac
 * TONEMASK��ģʽ����Ӧ������2.5M~5.7Mʹ��
 */
#define TONE_MASK_MODE_2                          0x01
/**
 *@ingroup hct_mac
 * TONEMASK��ģʽ����Ӧ������0.7M~3M��ʹ��
 */
#define TONE_MASK_MODE_3                          0x02
/**
 *@ingroup hct_mac
 * TONEMASK��ģʽ����Ӧ������1.7M~3Mʹ��
 */
#define TONE_MASK_MODE_4                          0x03
/**
 *@ingroup hct_mac
 * ��ʾ��ǰģ���ɫδ֪
 */
#define HI_MDM_PLC_ROLE_UNKNOWN        (0)
/**
 *@ingroup hct_mac
 * ��ʾ��ǰվ��ΪSTAģ��
 */
#define HI_MDM_PLC_ROLE_STA                  (1)
/**
 *@ingroup hct_mac
 * ��ʾ��ǰվ��Ϊ����ģ��
 */
#define HI_MDM_PLC_ROLE_PROXY              (2)
/**
 *@ingroup hct_mac
 * ��ʾ��ǰվ��ΪCCOģ��
 */
#define HI_MDM_PLC_ROLE_CCO                  (4)

/**
 *@ingroup hct_mac
 * ��ʾ��ǰ����ģʽ0
 */
#define HI_MDM_PLC_FREQ_0                       (0)
/**
 *@ingroup hct_mac
 * ��ʾ��ǰ����ģʽ1
 */
#define HI_MDM_PLC_FREQ_1                       (1)
/**
 *@ingroup hct_mac
 * ��ʾ��ǰ����ģʽ2
 */
#define HI_MDM_PLC_FREQ_2                       (2)
/**
 *@ingroup hct_mac
 * ��ʾ��ǰ����ģʽ3
 */
#define HI_MDM_PLC_FREQ_3                       (3)
/**
 *@ingroup hct_mac
 * ��ʾ��ǰ����ģʽ4
 */
#define HI_MDM_PLC_FREQ_4                       (4)

/**
 *@ingroup hct_mac
 * ��ʾA��λ
 */
#define HI_MDM_PLC_PHASE_A                      (1)
/**
 *@ingroup hct_mac
 * ��ʾB��λ
 */
#define HI_MDM_PLC_PHASE_B                      (2)
/**
 *@ingroup hct_mac
 * ��ʾC��λ
 */
#define HI_MDM_PLC_PHASE_C                      (3)

/**
 *@ingroup hct_mac
 * ��ʾδ֪״̬�������ʼ�����Ĭ��״̬
 */
#define HI_MDM_CHL_STATUS_UNKNOWN         (0)
/**
 *@ingroup hct_mac
 * ��ʾPLCͨ���Ѿ� ���ӣ�APP����ʹ��
 */
#define HI_MDM_CHL_STATUS_CONNECT          (1)
/**
 *@ingroup hct_mac
 * ��ʾPLCͨ���Ѿ��Ͽ���APP������ʹ�ã����ʹ�ÿ��ܻ�ʧ�ܣ�
 */
#define HI_MDM_CHL_STATUS_DISCONNECT     (2)
/**
 *@ingroup hct_mac
 * ��ʾPLCͨ�����ڽ������ӣ�APP������ʹ�ã����ʹ�ÿ��ܻ�ʧ�ܣ�
 */
#define HI_MDM_CHL_STATUS_CONNECTING     (3)

/**
 *@ingroup hct_mac
 * ��ʾ����״̬
 */
#define HI_MDM_CHL_STATUS_SEARCHING        (4)

/**
 *@ingroup hct_mac
 * ���ð�����ѡ��: MDM�㲻��Ҫ�ظ���������ϢastItem���������ý�����ظ����������õĳɹ���ʧ���ܸ���
 */
#define HI_MDM_NM_WHITE_LIST_SET_OPTION_NO_RSP          (0)
/**
 *@ingroup hct_mac
 * ���þ�̬����ѡ��: ���洢FLASH,����ʧЧ
 */
#define HI_MDM_NM_STATIC_PROXY_OPT_NOT_SAVE               (1)
#if 0
/**
 *@ingroup hct_mac
 * ���þ�̬����ѡ��: �洢FLASH,������Ч
 */
//#define HI_MDM_NM_STATIC_PROXY_OPT_SAVE                        (2)
#endif
/**
 *@ingroup hct_mac
 * �û�NA��ID����
 */
#define HI_MDM_OBJ_ID_NA        0x0
/**
 *@ingroup hct_mac
 * �û�һ��ID����
 */
#define HI_MDM_OBJ_ID_USR1      0x1
/**
 *@ingroup hct_mac
 * �û�����ID����
 */
#define HI_MDM_OBJ_ID_USR2      0x2
/**
 *@ingroup hct_mac
 * �û�����ID����
 */
#define HI_MDM_OBJ_ID_USR3      0x3
/**
 *@ingroup hct_mac
 * �����û���ID����
 */
#define HI_MDM_OBJ_ID_MRS       0x4
/**
 *@ingroup hct_mac
 * �û�����
 */
#define HI_MDM_OBJ_ID_NUM       4
/**
 *@ingroup hct_mac
 * �û����󻺴�����ֵ
 */
#define HI_MDM_OBJ_BUFF_NUM_MAX (HI_MDM_OBJ_ID_NUM + 1)

/**
 *@ingroup hct_mac
 * ̨������ˢ�µ�״̬��δ֪
 */
#define HI_MDM_TRANSFORMER_LIST_REFRESH_STATE_UNKNOWN    0
/**
 *@ingroup hct_mac
 * ��λ����ˢ�µ�״̬��δ֪
 */
#define HI_MDM_PHASE_LIST_REFRESH_STATE_UNKNOWN                 0

/**
 *@ingroup hct_mac
 * ̨��ʶ������δʶ��
 */
#define HI_MDM_TRANSFORMER_RESULT_UNKNOWN                       0x00
/**
 *@ingroup hct_mac
 * ̨��ʶ���������ڱ�̨��
 */
#define HI_MDM_TRANSFORMER_RESULT_BELONG                           0x01
/**
 *@ingroup hct_mac
 * ̨��ʶ�����������ڱ�̨��
 */
#define HI_MDM_TRANSFORMER_RESULT_NOT_BELONG                   0x02
/**
 *@ingroup hct_mac
 * ̨��ʶ��������֧��ʶ��
 */
#define HI_MDM_TRANSFORMER_RESULT_NOT_SUPPORT                 0x03
/**
 *@ingroup hct_mac
 * ̨��ʶ������ʶ��ʧ��
 */
#define HI_MDM_TRANSFORMER_RESULT_FAIL                                 0x04

/**
 *@ingroup hct_mac
 * ��λʶ��״̬��δʶ��״̬
 */
#define HI_MAC_PHASE_IDENTIFY_STATE_UNKNOWN                         0
/**
 *@ingroup hct_mac
 * ��λʶ��״̬����֧����λʶ��
 */
#define HI_MAC_PHASE_IDENTIFY_STATE_NOT_SUPPORT                  1
/**
 *@ingroup hct_mac
 * ��λʶ��״̬������ʶ��
 */
#define HI_MAC_PHASE_IDENTIFY_STATE_DOING                               2
/**
 *@ingroup hct_mac
 * ��λʶ��״̬��ʶ�����
 */
#define HI_MAC_PHASE_IDENTIFY_STATE_OVER                                 3
/**
 *@ingroup hct_mac
 * �����ʶ��״̬��δʶ��״̬
 */
#define HI_MAC_PN_IDENTIFY_STATE_UNKNOWN      HI_MAC_PHASE_IDENTIFY_STATE_UNKNOWN
/**
 *@ingroup hct_mac
 * �����ʶ��״̬����֧��ʶ��
 */
#define HI_MAC_PN_IDENTIFY_STATE_NOT_SUPPORT  HI_MAC_PHASE_IDENTIFY_STATE_NOT_SUPPORT
/**
 *@ingroup hct_mac
 * �����ʶ��״̬������ʶ��
 */
#define HI_MAC_PN_IDENTIFY_STATE_DOING        HI_MAC_PHASE_IDENTIFY_STATE_DOING
/**
 *@ingroup hct_mac
 * �����ʶ��״̬��ʶ�����
 */
#define HI_MAC_PN_IDENTIFY_STATE_OVER         HI_MAC_PHASE_IDENTIFY_STATE_OVER
/**
 *@ingroup hct_mac
 * ��������վ��������ֵ
 */
#define HI_MDM_BLACK_LIST_NUM_MAX                                           2048
/**
 *@ingroup hct_mac
 * ̨��ʶ��������վ��������ֵ
 */
#define HI_MDM_TRANSFORMER_IDENTIFY_LIST_NUM_MAX              2048

 /**
 * @ingroup hct_mac
 * Э��汾�Ŷ���: V100Э��汾��
 */
#define HI_MDM_PTCL_VER_100         0x0001
 /**
 * @ingroup hct_mac
 * Э��汾�Ŷ���: ����Э��汾��
 * 0x0040 - 0x007f:�����汾
 */
#define HI_MDM_PTCL_VER_SG           0x0040
 /**
 * @ingroup hct_mac
 * Э��汾�Ŷ���: V200Э��汾��
 * 0x0080 - 0x00bf:V200�汾
 */
#define HI_MDM_PTCL_VER_200        0x0080

 /**
 * @ingroup hct_mac
 * Э��汾�Ŷ���: ����Э��汾��
 * 0x00c0 - 0x00ff:CSG�汾
 */
#define HI_MDM_PTCL_VER_CSG        0x0002

/**
 *@ingroup hct_mac
 * V100δ����״̬
 */
#define HI_MDM_UNLOCK_NETWORK_STATUS    (0)
/**
 *@ingroup hct_mac
 * ����MAC״̬
 */
#define HI_MDM_LOCK_CCOMAC_STATUS           (1)
/**
 *@ingroup hct_mac
 * ����Ƶ��SNID״̬
 */
#define HI_MDM_LOCK_FREQSNID_STATUS         (2)
/**
 *@ingroup hct_mac
 * �������ڵ��ַ״̬
 */
#define HI_MDM_LOCK_MAIN_NODE_ADDR_TATUS    (3)

/**
 *@ingroup hct_mac
 * DIAG ����֪ͨĬ��ID��
 */
#define HI_MDM_DIAG_CMD_NOTIFY_DEFAULT_ID 0

/**
 *@ingroup hct_mac
 * �豸���Ʊ�ʾ�ֽڴ�С
 */
#define HI_MDM_DEV_ID_LEN       6

/**
* @ingroup  hct_mac
* @brief  ̨��ʶ��װ���ע�ắ��
*
* @par  ����:
* ̨��ʶ��װ���ע�ắ��
* @attention �ޡ�
* @param  ret [IN] ���� #HI_U32��#HI_ERR_SUCCESS:��ѯ�ɹ���#HI_ERR_FAILURE:��ѯʧ�ܡ�
* @param  avg_ntb [IN] ���� #HI_U32��������ƽ�����ڣ���λus��
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see EQUIP_ZERO_NTB_NUM ���塣
* @since HiMDL_V200R001C00
*/
typedef HI_VOID (*QueryZeroCrossCallback)(HI_U32 ret, HI_U32 avg_ntb);

/**
 * @ingroup hct_mac
 *  ��װ������õĽ��յ�λ
 */
#define EQUIP_ZERO_CROSS_LEVEL_240K                   0
/**
 * @ingroup hct_mac
 *  ��װ������õĽ��յ�λ
 */
#define EQUIP_ZERO_CROSS_LEVEL_1M                      1
/**
 * @ingroup hct_mac
 *  ��װ���ɼ����ݸ���
 */
#define EQUIP_ZERO_NTB_NUM                                  20
/**
 * @ingroup hct_mac
 *  ��װ���������ģʽ
 */
#define EQUIP_ZERO_CROSS_CRICUIT_RISE                  0
/**
 * @ingroup hct_mac
 *  ��װ����½���ģʽ
 */
#define EQUIP_ZERO_CROSS_CRICUIT_FALL                  1
/**
 * @ingroup hct_mac
 *  ���ʼ���������ֵ
 */
#define MAX_POWER_DETECT_THRESHOLD                    127

/**
* @ingroup  hct_mac
* @brief  ��װ����ռ�CCO,STA��Ʒ�Ĺ�������ݵĻص���������
*
* @par  ����:
* ��װ����ռ�CCO,STA��Ʒ�Ĺ�������ݵĻص���������
* @attention �ޡ�
* @param  ulRet [IN] ���� #HI_U32��#HI_ERR_SUCCESS:��ѯ�ɹ���#HI_ERR_FAILURE:��ѯʧ�ܡ�
* @param  ucLevel [IN] ���� #HI_U8��STA�Ľ��յ�λ������0��ʾ240K��1��ʾ1M��
* @param  ulNtb [EQUIP_ZERO_NTB_NUM] [IN] ���� #HI_U32����������ݣ������С�μ�#EQUIP_ZERO_NTB_NUM���塣
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see EQUIP_ZERO_NTB_NUM ���塣
* @since HiMDL_V200R001C00
*/
typedef HI_VOID (*CollectNtbCallback)(HI_U32 ulRet, HI_U8 ucLevel, HI_U32 ulNtb[EQUIP_ZERO_NTB_NUM]);

/**
* @ingroup  hct_mac
* @brief  �����Ե�ͨ�Ž������ݺ���������
*
* @par  ����:
* �����Ե�ͨ�Ž������ݺ���������
* @attention ���ò�Ʒ����ΪSTA��
* @param  data [IN] ���� #HI_PVOID*����������ָ�롣
* @param  data_size [IN] ���� #HI_U16���������ݴ�С��
* @param  mac_frame_ctrl [IN] ����  #DMS_MAC_FRAME_CTRL_S*�����Ŀ�����Ϣ��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS �ɹ������෵��ֵ�μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see hi_mdm_type.h�ļ���#DMS_MAC_FRAME_CTRL_S ���塣
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_PTPCHL_RX_F)(HI_IN HI_PVOID *, HI_U16 , HI_IN DMS_MAC_FRAME_CTRL_S* );
/**
* @ingroup  hct_mac
 *  ���緽ʽö�ٶ���
 */
typedef enum
{
    HI_MAC_STRONG_POOL_POWER_SUPPLY = 0,        /**<ǿ�繩��  */
    HI_MAC_BATTERY_POWER_SUPPLY                        /**<��ع���  */
}HI_MAC_POWER_SUPPLY_MODE_ENUM;

/**
* @ingroup  hct_mac
* @brief ͨ����Ե�ͨ�Żص�����
*
* @par  ����:
* @li ͨ����Ե�ͨ�Żص�����
* @attention �ޡ�
* @param  pCmdParam [IN] ���� #HI_PVOID�����뷢��buff �������ݡ�
* @param  usCmdParamSize [IN] ���� #HI_U16���������ȡ�
* @param  ulOption [IN] ���� #HI_U32��ѡ���֣���ǰ������д0��
* @retval   ret ���� #HI_U32��#HI_ERR_INVALID_PARAMETER ��β��Ϸ���#HI_ERR_NOT_ENOUGH_MEMORY    �ڴ�����ʧ�ܣ�\n
                   #HI_ERR_SUCCESS ���ķ��ͳɹ������෵��ֵ�μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
typedef HI_U32(*HI_MDM_NDM_PING_F)(HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

/**
 * @ingroup  hct_mac
 *  ������ģʽ���ã���������Ϣ����Ϊ������
 */
#define HI_MDM_WHITE_LIST_MODE_BASE_LIB               0
/**
 * @ingroup  hct_mac
 *  ������ģʽ���ã������ͻ�����Ϣ����Ϊ������
 */
#define HI_MDM_WHITE_LIST_MODE_METER_BASE_LIB   1
/**
 * @ingroup  hct_mac
 *  ������ģʽ���ã���������Ϊ������
 */
#define HI_MDM_WHITE_LIST_MODE_METER                    2
/**
 * @ingroup  hct_mac
 *  ������ģʽ���ã���Чģʽ
 */
#define HI_MDM_WHITE_LIST_MODE_INVALID              0xff

/**
 * @ingroup  hct_mac
 *  DIAG�����ӳ�����С����ID
 */
#define HI_MDM_NM_REQ_ID_MIN                        0x3610

/**
* @ingroup  hct_mac
* @brief ����ϢIDʵ�ֻ�ȡ�������˽ṹ����
*
* @par   ����:
* ͨ������ HI_MDM_MsgSend���͸���Ϣ��MDM�㣬��ȡ�������˽ṹ
* @attention ���ò�Ʒ����ΪCCO��
* @param  topo_req [IN] ���� #HI_MDM_NM_TOPO_REQ_S�����˲�ѯ����
* @param  topo_ind [OUT] ���� #HI_MDM_NM_TOPO_IND_S�����˲�ѯӦ�� ��
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see ��hi_mdm_mac.h�ļ���#HI_MDM_NM_TOPO_REQ_S��#HI_MDM_NM_TOPO_IND_S�Ķ��塣
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_TOPO_ID                   0x3610

/**
* @ingroup  hct_mac
* @brief ����ϢIDʵ�ֻ�ȡվ�㵱ǰƵ�ι���
*
* @par   ����:
* ͨ������ HI_MDM_MsgSend���͸���Ϣ��MDM�㣬��ȡվ�㵱ǰƵ��
* @attention �ޡ�
* @param  freq_qry_ind [OUT] ���� #HI_MDM_NM_FREQ_QRY_IND_S��Ƶ�β�ѯӦ�� ��
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�li></ul>
* @see ��hi_mdm_mac.h�ļ���#HI_MDM_NM_FREQ_QRY_IND_S�Ķ��塣
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_FREQ_QRY_ID               0x3611

/**
* @ingroup  hct_mac
* @brief ����ϢIDʵ��֪ͨȫ���л�Ƶ����
*
* @par   ����:
* ͨ������ HI_MDM_MsgSend���͸���Ϣ��MDM�㣬֪ͨȫ���л�Ƶ��(ֻ������CCO)
* @attention ���ò�Ʒ����ΪCCO��
* @param  freq_set_req [IN] ���� #HI_MDM_NM_FREQ_SET_REQ_S��Ƶ���������� ��
* @param  freq_set_ind [OUT] ���� #HI_MDM_NM_FREQ_SET_IND_S��Ƶ������Ӧ�� ��
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see ��hi_mdm_mac.h�ļ���#HI_MDM_NM_FREQ_SET_REQ_S��#HI_MDM_NM_FREQ_SET_IND_S�Ķ��塣
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_FREQ_SET_ID               0x3612

/**
* @ingroup  hct_mac
* @brief ����ϢIDʵ�ֻ�ȡվ��汾��Ϣ����
*
* @par   ����:
* ͨ������ HI_MDM_MsgSend���͸���Ϣ��MDM�㣬��ȡվ��汾��Ϣ
* @attention �ޡ�
* @param  device_ver_ind [OUT] ���� #HI_MDM_NM_DEVICE_VER_IND_S��վ��汾��ϢӦ�� ��
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see ��hi_mdm_mac.h�ļ���#HI_MDM_NM_DEVICE_VER_IND_S�Ķ��塣
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_DEVICE_VER_ID             0x3613

/**
* @ingroup  hct_mac
* @brief ����ϢIDʵ�ֻ�ȡվ��·�ɱ���
*
* @par   ����:
* ͨ������ HI_MDM_MsgSend���͸���Ϣ��MDM�㣬��ȡվ��·�ɱ�
* @attention �ޡ�
* @param  route_info_req [IN] ���� #HI_MDM_NM_ROUTER_INFO_REQ_S����ѯ·����Ϣ���� ��
* @param  route_info_ind [OUT] ���� #HI_MDM_NM_ROUTER_INFO_IND_S����ѯ·����ϢӦ�� ��
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see ��hi_mdm_mac.h�ļ���#HI_MDM_NM_ROUTER_INFO_REQ_S��#HI_MDM_NM_ROUTER_INFO_IND_S�Ķ��塣
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_ROUTER_INFO_ID            0x3614

/**
* @ingroup  hct_mac
* @brief ����ϢIDʵ�ֻ�ȡ��ȡվ����Ϣ����
*
* @par   ����:
* ͨ������ HI_MDM_MsgSend���͸���Ϣ��MDM�㣬��ȡվ����Ϣ
* @attention �ޡ�
* @param  node_qry_ind [OUT] ���� #HI_MDM_NM_NODE_IND_S����ѯվ����ϢӦ�� ��
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see ��hi_mdm_mac.h�ļ���#HI_MDM_NM_NODE_IND_S�Ķ��塣
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_NODE_ID                   0x3615

/**
* @ingroup  hct_mac
* @brief ����ϢIDʵ�ֻ�ȡ��ȡվ�㷢���б���
*
* @par   ����:
* ͨ������ HI_MDM_MsgSend���͸���Ϣ��MDM�㣬��ȡվ�㷢���б�
* @attention �ޡ�
* @param  discover_list_req [IN] ���� #HI_MDM_NM_DISCOVER_LIST_REQ_S����ѯ�����б����� ��
* @param  discover_list_ind [OUT] ���� #HI_MDM_NM_DISCOVER_LIST_IND_S����ѯ�����б�Ӧ�� ��
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see ��hi_mdm_mac.h�ļ���#HI_MDM_NM_DISCOVER_LIST_REQ_S��#HI_MDM_NM_DISCOVER_LIST_IND_S�Ķ��塣
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_DISCOVER_LIST_ID          0x3616

/**
* @ingroup  hct_mac
* @brief ����ϢIDʵ�ֻ�ȡ�������������
*
* @par   ����:
* ͨ������ HI_MDM_MsgSend���͸���Ϣ��MDM�㣬��ȡ���������(ֻ��CCOʹ��)
* @attention  ���ò�Ʒ����ΪCCO��
* @param  white_list_qry_req [IN] ���� #HI_MDM_NM_WHITE_LIST_QRY_REQ_S����ѯ���������� ��
* @param  white_list_qry_ind [OUT] ���� #HI_MDM_NM_WHITE_LIST_QRY_IND_S����ѯ������Ӧ�� ��
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see ��hi_mdm_mac.h�ļ���#HI_MDM_NM_WHITE_LIST_QRY_REQ_S��#HI_MDM_NM_WHITE_LIST_QRY_IND_S�Ķ��塣
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_WHITE_LIST_QRY_ID         0x3617

/**
* @ingroup  hct_mac
* @brief ����ϢIDʵ�������������������
*
* @par   ����:
* ͨ������ HI_MDM_MsgSend���͸���Ϣ��MDM�㣬�������������(ֻ��CCOʹ��,������Ϊ������Ϣ��)
* @attention  ���ò�Ʒ����ΪCCO��
* @param  whitelist_set_req [IN] ���� #HI_MDM_NM_WHITE_LIST_SET_REQ_S�����ð��������� ��
* @param  whitelist_set_ind [OUT] ���� #HI_MDM_NM_WHITE_LIST_SET_IND_S�����ð�����Ӧ�� ��
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see ��hi_mdm_mac.h�ļ���#HI_MDM_NM_WHITE_LIST_SET_REQ_S��#HI_MDM_NM_WHITE_LIST_SET_IND_S�Ķ��塣
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_WHITE_LIST_SET_ID         0x3618

/**
* @ingroup  hct_mac
* @brief ����ϢIDʵ�ֻ�ȡվ��SNID����
*
* @par   ����:
* ͨ������ HI_MDM_MsgSend���͸���Ϣ��MDM�㣬��ȡվ��SNID
* @attention �ޡ�
* @param  snid_qry_ind [OUT] ���� #HI_MDM_NM_SNID_QRY_IND_S����ѯվ��SNIDӦ�� ��
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see ��hi_mdm_mac.h�ļ���#HI_MDM_NM_SNID_QRY_IND_S�Ķ��塣
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_SNID_QRY_ID               0x3619

/**
* @ingroup  hct_mac
* @brief ����ϢIDʵ������վ��SNID����
*
* @par   ����:
* ͨ������ HI_MDM_MsgSend���͸���Ϣ��MDM�㣬����վ��SNID
* @attention ���ò�Ʒ����ΪCCO��
* @param  snid_set_req [IN] ���� #HI_MDM_NM_SNID_SET_REQ_S������վ��SNID���� ��
* @param  snid_set_ind [OUT] ���� #HI_MDM_NM_SNID_SET_IND_S������վ��SNIDӦ�� ��
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see ��hi_mdm_mac.h�ļ���#HI_MDM_NM_SNID_SET_REQ_S��#HI_MDM_NM_SNID_SET_IND_S�Ķ��塣
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_SNID_SET_ID               0x361A

/**
* @ingroup  hct_mac
* @brief ����ϢIDʵ�ֻ�ȡվ�㾲̬������
*
* @par   ����:
* ͨ������ HI_MDM_MsgSend���͸���Ϣ��MDM�㣬��ȡվ�㾲̬����
* @attention ���ò�Ʒ����ΪSTA��NDM����CCO����
* @param  static_proxy_qry_ind [OUT] ���� #HI_MDM_NM_STATIC_PROXY_QRY_IND_S����ȡվ�㾲̬�������� ��
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see ��hi_mdm_mac.h�ļ���#HI_MDM_NM_STATIC_PROXY_QRY_IND_S�Ķ��塣
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_STATIC_PROXY_QRY_ID       0x361B

/**
* @ingroup  hct_mac
* @brief ����ϢIDʵ������վ�㾲̬������
*
* @par   ����:
* ͨ������ HI_MDM_MsgSend���͸���Ϣ��MDM�㣬����վ�㾲̬����
* @attention ���ò�Ʒ����ΪSTA��NDM����CCO����
* @param  static_proxy_set_req [IN] ���� #HI_MDM_NM_STATIC_PROXY_SET_REQ_S������վ�㾲̬�������� ��
* @param  static_proxy_set_ind [OUT] ���� #HI_MDM_NM_STATIC_PROXY_SET_IND_S������վ�㾲̬����Ӧ�� ��
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see ��hi_mdm_mac.h�ļ���#HI_MDM_NM_STATIC_PROXY_SET_REQ_S��#HI_MDM_NM_STATIC_PROXY_SET_IND_S�Ķ��塣
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_STATIC_PROXY_SET_ID       0x361C

/**
* @ingroup  hct_mac
* @brief  ����ϢIDʵ�ֻ�ȡ����������
*
* @par   ����:
* ͨ��HI_MDM_MsgSend�������͸���Ϣ����������MDM�㣬��ȡ����������
* @attention ���ò�Ʒ����ΪCCO��
* @param  whitelist_switch_qry_ind [OUT] ���� #HI_MDM_NM_WHITE_LIST_SWITCH_QRY_IND_S����ȡ����������Ӧ�� ��
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see ��hi_mdm_mac.h�ļ���#HI_MDM_NM_WHITE_LIST_SWITCH_QRY_IND_S�Ķ��塣
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_QRY_WHITE_LIST_SWITCH_ID  0x361F

/**
* @ingroup  hct_mac
* @brief  ����ϢIDʵ�����ð���������
*
* @par   ����:
* ͨ��HI_MDM_MsgSend�������͸���Ϣ����������MDM�㣬���ð���������
* @attention ���ò�Ʒ����ΪCCO��
* @param  set_req [IN] ���� #HI_MDM_NM_WHITE_LIST_SWITCH_SET_REQ_S�����ð������������� ��
* @param  set_ind [OUT] ���� #HI_MDM_NM_WHITE_LIST_SWITCH_SET_IND_S�����ð���������Ӧ�� ��
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see ��hi_mdm_mac.h�ļ���#HI_MDM_NM_WHITE_LIST_SWITCH_SET_REQ_S��#HI_MDM_NM_WHITE_LIST_SWITCH_SET_IND_S�Ķ��塣
* @since HiMDL_V200R001C00
*/
#define HI_MDM_NM_SET_WHITE_LIST_SWITCH_ID  0x3620


/**
 * @ingroup  hct_mac
 *  DIAG�����ӳ�����С����ID
 */
#define HI_MDM_NM_REQ_ID_MAX                0x3620

/**
 * @ingroup  hct_mac
 *  �ű�����ʹ��������ֵ
 */
#define HI_MDM_NM_MAX_BEACON_MAX_SEND_CNT       3
/**
 *@ingroup hct_mac
 * ��ȡ���˽ṹ����ṹ
 */
typedef struct
{
    HI_U16 usNodeNum;       /**<��ʾ�����������˲�ѯ��MDM��ÿ���ϱ���APP������˽ڵ��������\n
                                                    ����ò���Ϊ0����MDM��ÿ���ϱ���APP������˽ڵ����Ϊ1 ��     */
    HI_U16 usOffsetNum;     /**<ָ����ȡ�ڼ����ڵ㣬ȡֵΪ0��ʾ��ָ����Ŀǰ�汾��֧�֡�  */
}HI_MDM_NM_TOPO_REQ_S;

/**
 *@ingroup hct_mac
 * ��ȡ�����е�վ����Ϣ
 */
typedef struct
{
    HI_U32 uTei:12;                  /**< ��վ��TEI                 */
    HI_U32 uProxyTei:12;         /**<  վ�㸸����TEI      */
    HI_U32 uLevel:4;                /**<  ��վ��㼶(0-15)     */
    HI_U32 uRole:3;                 /**<  ��վ���ɫ(�ο� #HI_NDM_PLC_ROLE_STA��)   */
    HI_U32 ulOnLine:1;            /**<  ��ǰվ���Ƿ�����      */
    HI_U8  ucUlCommRate;      /**<  ��վ������ͨ�ųɹ��� */
    HI_U8  ucDlCommRate;      /**<  ��վ������ͨ�ųɹ���   */
    HI_U8  aucMacAddr[HI_PLC_MAC_ADDR_LEN];     /**<  ��վ��MAC��ַ    */
    HI_U32 ulJoinTime;             /**<  ��վ���������ʱ��    */
    HI_U8  ucBootVer;              /**<  boot�汾��           */
    HI_U8  aucVer[3];               /**<  VRC�汾�ţ�ȡֵ˵�����¡�\n
                                                     aucVer[0]��ʾV�汾�ţ���ȡֵ100���ʾV100\n
                                                      aucVer[1]��ʾR�汾�ţ���ȡֵ1���ʾR001\n
                                                       aucVer[2]��ʾC�汾�ţ���ȡֵ0���ʾC00       */
    HI_U16 ausVer[2];              /**< B��SPC�汾�ţ�ȡֵ˵�����¡�  */
                                            /**<  ausVer[0]��ʾB�汾�ţ����Ϊ256����ȡֵ102���ʾB102\n
                                                     ausVer[1]��ʾSPC�汾�ţ����Ϊ256����ȡֵ1���ʾSPC001��*/
    HI_U32 uLeaveTime;         /**<  վ������ʱ������λs��*/
    HI_U32 uLeaveCnt;           /**<  վ������ߴ�����     */
    HI_U32 uProxyCnt;           /**<  վ�������������   */
    HI_U8  ucProxyType;        /**<  վ��Ĵ������͡� */
    HI_U8  reserved[3];
}HI_MDM_NM_TOPO_NODE_S;

/**
 *@ingroup hct_mac
 * ��ȡ������Ϣ�ṹ
 */
typedef struct
{
    HI_U16 usTotalNodeNum;      /**< Topo������վ�����(����CCO)       */
    HI_U16 usNodeNum;             /**<ÿ�����ݰ�����Чվ�����       */
    HI_MDM_NM_TOPO_NODE_S astNode[0];   /**<վ����Ϣ                               */
}HI_MDM_NM_TOPO_IND_S;

/**
 *@ingroup hct_mac
 * ����ȫ����Ƶ������ṹ
 */
typedef struct
{
    HI_U16 usFreqSn;        /**<���õ�Ƶ��(�ο�#HI_MDM_PLC_FREQ_1��)        */
    HI_U16 usStartTime;   /**< �л�Ƶ�ε�ʱ�䣻��λs               */
}HI_MDM_NM_FREQ_SET_REQ_S;

/**
 *@ingroup hct_mac
 * Ƶ�ο�����Ϣ�ṹ
 */
typedef struct
{
    HI_U8 aucMacAddr[HI_PLC_MAC_ADDR_LEN];  /**< ��վ���MAC��ַ    */
    HI_U16 usFreqSn;    /**< Ҫ�л�Ƶ��    */
    HI_U32 ulErr;          /**< �Ƿ����óɹ�    */
}HI_MDM_NM_FREQ_SET_IND_S;

/**
 *@ingroup hct_mac
 * ��ȡƵ����Ϣ�ṹ
 */
typedef struct
{
    HI_U8 aucMacAddr[HI_PLC_MAC_ADDR_LEN];      /**< ��վ���MAC��ַ    */
    HI_U16 usFreqSn;                                            /**< ��վ�㵱ǰƵ��    */
}HI_MDM_NM_FREQ_QRY_IND_S;

/**
 *@ingroup hct_mac
 * ��ȡ�汾��Ϣ�ṹ
 */
typedef struct
{
    HI_U8  ucBootVer;      /**< boot�汾��           */
    HI_U8  aucVer[3];       /**<VRC�汾�ţ�ȡֵ˵�����¡�\n
                                        aucVer[0]��ʾV�汾�ţ���ȡֵ100���ʾV100\n
                                        aucVer[1]��ʾR�汾�ţ���ȡֵ1���ʾR001\n
                                        aucVer[2]��ʾC�汾�ţ���ȡֵ0���ʾC00           */
    HI_U16 ausVer[2];       /**< B��SPC�汾�ţ�ȡֵ˵�����¡�\n
                                        ausVer[0]��ʾB�汾�ţ����Ϊ256����ȡֵ102���ʾB102\n
                                        ausVer[1]��ʾSPC�汾�ţ����Ϊ256����ȡֵ1���ʾSPC001      */
    HI_U8  ucNvCVer;        /**< NV��C�汾��,NV��VΪaucVer[0],R�汾ΪaucVer[1]          */
    HI_U8  reserved;
    HI_U16 ausNvVer[2];     /**< NV��B��SPC�汾�ţ�ȡֵ˵�����¡�\n
                                            ausVer[0]��ʾB�汾�ţ����Ϊ256����ȡֵ102���ʾB102\n
                                            ausVer[1]��ʾSPC�汾�ţ����Ϊ256����ȡֵ1���ʾSPC001      */
    HI_U16 pad;                   /**< ����    */
}HI_MDM_NM_DEVICE_VER_IND_S;

/**
 *@ingroup hct_mac
 * ��ȡ·�ɱ�����ṹ
 */
 typedef struct
{
    HI_U16 usNodeNum;       /**< ��ʾ��������·����Ϣ��ѯ��MDM��ÿ���ϱ���APP��Ľڵ��������\n
                                                 ����ò���Ϊ0����MDM��ÿ���ϱ���APP��Ľڵ����Ϊ1   */
    HI_U16 usOffsetNum;     /**< ָ����ȡ�ڼ����ڵ㣬ȡֵΪ0��ʾ��ָ����Ŀǰ�汾��֧�� */
}HI_MDM_NM_ROUTER_INFO_REQ_S;

/**
 *@ingroup hct_mac
 * վ��·�ɱ���Ϣ
 */
typedef struct
{
    HI_U16 usOdTei;     /**<Ŀ��վ��*/
    HI_U16 reserved;   /**<����*/
    HI_U16 ausNextHopTei[4];    /**<��һ������*/
}HI_MDM_NM_ROUTER_INFO_NODE_S;

/**
 *@ingroup hct_mac
 * ��ȡ·�ɱ���Ϣ�ṹ
 */
typedef struct
{
    HI_U16 usTotalNodeNum;      /**<��������վ�����(����CCO)       */
    HI_U16 usNodeNum;           /**<ÿ�����ݰ�����Чվ�����        */
    HI_MDM_NM_ROUTER_INFO_NODE_S astNode[0];    /** <վ��·�ɱ���Ϣ  */
}HI_MDM_NM_ROUTER_INFO_IND_S;

/**
 *@ingroup hct_mac
 * ��ȡվ����Ϣ�ṹ
 */
typedef struct
{
    HI_U32 uTei:12;             /**<��վ��TEI          */
    HI_U32 uProxyTei:12;    /**<��վ�㸸����TEI            */
    HI_U32 uLevel:4;            /**< ��վ��㼶(0-15)          */
    HI_U32 uRole:3;             /**<��վ���ɫ(�ο� #HI_NDM_PLC_ROLE_STA��)         */
    HI_U32 bOnline:1;           /**< ��ǰվ���Ƿ�����       */
    HI_U8  ucUlCommRate;    /**< ��վ������ͨ�ųɹ���      */
    HI_U8  ucDlCommRate;    /**< ��վ������ͨ�ųɹ���      */
    HI_U8  aucMacAddr[HI_PLC_MAC_ADDR_LEN];     /**< ��վ��MAC��ַ     */
    HI_U8  aucIpAddr[HI_IPV4_ADDR_LEN];             /**< ��վ��IP��ַ      */
    HI_U32 ulRunTime;            /**< ��վ������ʱ�䣬��λms        */
}HI_MDM_NM_NODE_IND_S;

/**
 *@ingroup hct_mac
 * �����б���������ֶ�
 */
typedef struct
{
    HI_U16 usItemNum;       /**<MDM��ÿ���ϱ���APP��ķ����б����������\n
                                                ����ò���Ϊ0����MDM��ÿ���ϱ���APP��ķ���\n
                                                �б������Ϊ1        */
    HI_U16 reserved;   /**<����*/
}HI_MDM_NM_DISCOVER_LIST_REQ_S;

/**
 *@ingroup hct_mac
 * ��ȡ�����б���Ϣ�ṹ��
 */
typedef struct
{
    HI_U32 uTei:12;             /**<��վ��TEI         */
    HI_U32 uProxyTei:12;     /**< ��վ�㸸����TEI      */
    HI_U32 uLevel:4;            /**< ��վ��㼶(0-15)         */
    HI_U32 uRole:3;             /**< ��վ���ɫ(�ο� #HI_NDM_PLC_ROLE_STA��)       */
    HI_U8 aucMacAddr[HI_PLC_MAC_ADDR_LEN];  /**< ��վ��MAC��ַ     */
    HI_U8 ucUlCommRate;      /**< ��վ������ͨ�ųɹ���     */
    HI_U8 ucDlCommRate;     /**< ��վ������ͨ�ųɹ���      */
}HI_MDM_NM_DISCOVER_LIST_ITEM_S;
/**
 *@ingroup hct_mac
 * ��ȡ�����б���Ϣ�ṹ��
 */
typedef struct
{
    HI_U16 usTotalItemNum;      /**<   �����б��ܸ���       */
    HI_U16 usItemNum;             /**<  astItem����Ч����           */
    HI_MDM_NM_DISCOVER_LIST_ITEM_S astItem[0];   /**< �����б��б��� */
}HI_MDM_NM_DISCOVER_LIST_IND_S;

/**
 *@ingroup hct_mac
 *��ȡ ������������Ϣ�ṹ��
 */
typedef struct
{
    HI_U16 usItemNum;   /**< ���������ֲ�ѯ��MDM��ÿ���ϱ���APP��İ�������������\n
                                 ����ò���Ϊ0����MDM��ÿ���ϱ���APP��İ���������Ϊ1       */
    HI_U16 reserved;    /**<����*/
}HI_MDM_NM_WHITE_LIST_QRY_REQ_S;

/**
 *@ingroup hct_mac
 * ��������
 */
typedef struct
{
    HI_U8  aucMacAddr[HI_PLC_MAC_ADDR_LEN];  /**<  վ��MAC��ַ   */
    HI_U16 reserved;    /**<����*/
}HI_MDM_NM_WHITE_LIST_ITEM_S;

/**
 *@ingroup hct_mac
 * ���ð�������Ϣ�ṹ��
 */
typedef struct
{
    HI_U16 usTotalItemNum;      /**<  ���������ܸ���   */
    HI_U16 usItemNum;               /**<stItem����Ч����     */
    HI_MDM_NM_WHITE_LIST_ITEM_S astItem[0];   /**<  ��������    */
}HI_MDM_NM_WHITE_LIST_QRY_IND_S;

/**
 *@ingroup hct_mac
 * ���ð�����������Ϣ�ṹ��
 */
typedef struct
{
    HI_U16 usOption;        /**< �������ظ�ѡ�ȡֵ���£�\n
                                            #HI_MDM_NM_BLACK_WHITE_LIST_SET_OPTION_NO_RSP��ʾMDM�㲻��Ҫ�ظ���������ϢastItem���������ý�����ظ����õĳɹ���ʧ���ܸ���\n
                                            ����ѡ�������һ�������ѡ��Ϊ׼ */
    HI_U16 usId;                    /**<��������ID�š����MDM����յ������������Ϣ��ID��һ�£�MDM������������һ��ID�İ�������*/
    HI_U16 usTotalItemNum;  /**< ���ð��������ܸ�����Ϊ0ʱ��ջ�����Ϣ�� */
    HI_U16 usItemNum;          /**< astItem����Ч����    */
    HI_MDM_NM_WHITE_LIST_ITEM_S astItem[0];  /**<  ��������   */
}HI_MDM_NM_WHITE_LIST_SET_REQ_S;

/**
 *@ingroup hct_mac
 * ���ð������ӿ���������ý����Ϣ
 */
typedef struct
{
    HI_U32 ulErr;                             /**< �����Ƿ�ɹ�; #HI_ERR_SUCCESS��ʾ�ɹ�*/
    HI_U16 usTotalItemNum;            /**< ���ð��������ܸ���*/
    HI_U16 usTotalItemNumOk;        /**<���ð������ɹ����ܸ���*/
    HI_U16 usTotalItemNumErr;       /**<���ð�����ʧ�ܵ��ܸ���*/
    HI_U16 usItemNum;                   /**<�������������astItem�ĸ�����*/
    HI_MDM_NM_WHITE_LIST_ITEM_S astItem[0];   /**<�ڰ�������*/
}HI_MDM_NM_WHITE_LIST_SET_IND_S;

/**
 *@ingroup hct_mac
 * ��ȡ��վ��SNID�ṹ��
 */
typedef struct
{
    HI_U32 ulSnid;      /**<��վ�㵱ǰSNID      */
}HI_MDM_NM_SNID_QRY_IND_S;

/**
 *@ingroup hct_mac
 * ����SNID����ṹ��
 */
typedef struct
{
    HI_U32 ulSnid;      /**< Ҫ���õ�SNID  */
}HI_MDM_NM_SNID_SET_REQ_S;

/**
 *@ingroup hct_mac
 * ����SNID��Ϣ�ṹ��
 */
typedef struct
{
    HI_U32 ulSnid;      /**< �����SNID       */
    HI_U32 usRc:1;      /**<  ����Ϣ�ķ����룬ȡֵ���£�\n
                                 #HI_FALSE��ʾָ����SNID�Ѿ����ڣ�����Ҫ��������\n
                                 #HI_TRUE��ʾ���óɹ�         */
    HI_U32 reserved:31;    /**<����*/
}HI_MDM_NM_SNID_SET_IND_S;

/**
 *@ingroup hct_mac
 * ��ȡվ�㾲̬����
 */
typedef struct
{
    HI_U8 aucProxyMac[HI_PLC_MAC_ADDR_LEN];     /**<    ָ����̬�м�MAC��ַ\n
                                                                                        ȫ��Ϊ0��ʾû��ָ����̬�м̵�ַ   */
    HI_U8 reserved[2];          /**<����*/
}HI_MDM_NM_STATIC_PROXY_QRY_IND_S;

/**
 *@ingroup hct_mac
 * ����վ�㾲̬������ƽṹ��
 */
typedef struct
{
    HI_U8 aucProxyMac[HI_PLC_MAC_ADDR_LEN];     /**< ָ����̬�м�MAC��ַ\n
                                                                                 ȫ��Ϊ0��ʾû��ָ����̬�м̵�ַ */
    HI_U8 ucOption;     /**< ѡ�����ã�ȡֵ���£�\n
                                         #HI_MDM_NM_STATIC_PROXY_OPT_NOT_SAVE(1)��ʾ��̬�м̲��洢��FLASH���ϵ�����ʧЧ��Ĭ��ֵ��\n
                                         ע�⣺�ϵ���������Ȼ��Ч�����м�ģ��λ�ñ��ƶ����滻�ȣ�������ɸ�վ���޷����� */
    HI_U8 reserved;    /**<����*/
}HI_MDM_NM_STATIC_PROXY_SET_REQ_S;

/**
 *@ingroup hct_mac
 * ����վ�㾲̬������Ϣ�ṹ��
 */
typedef struct
{
    HI_U8 aucProxyMac[HI_PLC_MAC_ADDR_LEN];     /**< ָ����̬�м�MAC��ַ\n
                                                 ȫ��Ϊ0��ʾ������̬���� */
    HI_U8 ucOption;     /**<  ѡ�����ã�ȡֵ���£�\n
                         #HI_MDM_NM_STATIC_PROXY_OPT_NOT_SAVE(1)��ʾ��̬�м̲��洢��FLASH���ϵ�����ʧЧ��Ĭ��ֵ��\n
                         ע�⣺�ϵ���������Ȼ��Ч�����м�ģ��λ�ñ��ƶ����滻�ȣ�������ɸ�վ���޷����� */
    HI_U8 reserved;
    HI_U32 ulErr;       /**<  �����Ƿ�ɹ���HI_ERR_SUCCESS��ʾ�ɹ��� */
}HI_MDM_NM_STATIC_PROXY_SET_IND_S;

/**
 *@ingroup hct_mac
 * ��ȡ���������ؽṹ��
 */
typedef struct
{
    HI_BOOL aucWhiteListSwitch;     /**< ��ǰ����������״̬ */
    HI_U8 mode;                      /**< ��������ģʽ */
    HI_U8 reserved[2];              /**<����*/
}HI_MDM_NM_WHITE_LIST_SWITCH_QRY_IND_S;

/**
 *@ingroup hct_mac
 * ���ð���������ṹ��
 */
typedef struct
{
    HI_BOOL aucWhiteListSwitch;     /**<���õİ���������״̬    */
    HI_U8 mode;                             /**<��������ģʽ        */
    HI_U8 reserved[2];             /**<����*/
}HI_MDM_NM_WHITE_LIST_SWITCH_SET_REQ_S;

/**
 *@ingroup hct_mac
 * ���ð�������Ϣ�ṹ��
 */
typedef struct
{
    HI_BOOL aucWhiteListSwitch; /**<��ǰ����������״̬ */
    HI_U8 mode;                 /**<��������ģʽ */
    HI_U8 reserved[2];          /**<����*/

    HI_U32 ulErr;               /**<�����Ƿ�ɹ���HI_ERR_SUCCESS��ʾ�ɹ��� */
}HI_MDM_NM_WHITE_LIST_SWITCH_SET_IND_S;

/**
 *@ingroup hct_mac
 * ����ģʽ����
 */
typedef enum
{
    HI_NDC_CHL_ACCESS_MODE_CCO = 0,             /**<��ʾCCO��������ģʽ����������������CCO���ϵ�󳭿���Ĭ�����������ģʽ��*/
    HI_NDC_CHL_ACCESS_MODE_STA,                   /**<��ʾSTA��������ģʽ��������������STA*/
    HI_NDC_CHL_ACCESS_MODE_AUTO                 /**<��ʾ��ָ��ģʽ*/
}HI_NDC_CHL_ACCESS_MODE_E;

/**
 *@ingroup hct_mac
 * �����������
 */
typedef enum
{
    HI_NDC_CHL_ACTION_SEARCH = 0,           /**< ��ʾNDC���������磬�����������磨CCOģʽ���ϵ��NDC���Զ�������*/
    HI_NDC_CHL_ACTION_SEARCH_QRY,         /**< ��ʾ���������в�ѯ������Ϣ����������ģʽ��MAC��ַ��IP��ַ���豸���Ƶȣ�*/
    HI_NDC_CHL_ACTION_CONNECT,               /**< ��ʾѡ������*/
    HI_NDC_CHL_ACTION_DISCONNECT,          /**< ��ʾ�Ͽ�����*/
    HI_NDC_CHL_ACTION_CLOSE                     /**< ��ʾ�ر�����*/
}HI_NDC_CHL_CONNECT_ACTION_E;

/**
 *@ingroup hct_mac
 * �����������
 */
typedef struct
{
    HI_NDC_CHL_ACCESS_MODE_E enAccessMode;    /**< ����ģʽ*/

    HI_U8       ucFreq;                                            /**< ����Ƶ��*/
    HI_U8       ucAction;                                          /**<�μ�HI_NDC_CHL_CONNECT_ACTION_E����*/
    HI_U8       ucMac[HI_PLC_MAC_ADDR_LEN];         /**< MAC��ַ*/

    HI_U32     ulSnid;                                               /**< �����ʶ��*/
} HI_ND_NETWORK_SEARCH_CONN_REQ_S;

/**
 *@ingroup hct_mac
 * �����������
 */
typedef struct
{
    HI_ND_NETWORK_SEARCH_CONN_REQ_S stReq;  /**<�μ�#HI_ND_NETWORK_SEARCH_CONN_REQ_S����*/
} HI_MDM_NET_SEARCH_CONN_REQ_S;

/**
 *@ingroup hct_mac
 * ��������ϱ��еĵ���վ����Ϣ
 */
typedef struct
{
    HI_U8       bConnect:1;
    HI_U8       ucFreq:5;                           /**< Ƶ��*/
    HI_U8       ucProdTypeStaType:1;        /**< #HI_FTM_PRODUCT_TYPE_STA_MI_M     #HI_FTM_PRODUCT_TYPE_STA_MI_I  */
    HI_U8       pad:1;                                       /**<����*/
    HI_U8       rsvd;                      /**<����*/
    HI_U8       aucMac[HI_PLC_MAC_ADDR_LEN];        /**<վ���MAC��ַ*/

    HI_U32     ulSnid;                             /**<�����ʾ�� */

    HI_U32      ulDiscoveryCnt;                    /**<�Ѳ鵽��ͨ��ģ��Ĵ���*/

    HI_U8       aucIpv4[HI_IPV4_ADDR_LEN];          /**<����ڵ��PLC��IP��ַ*/
    HI_U8       aucDevId[HI_MDM_DEV_ID_LEN];        /**<����ڵ���豸��ʶ*/
    HI_U8       ucChannelInfo;                      /**<�ŵ�������Ϣ*/
    HI_U8       ucNdcVer;                           /**<NDCͨ���汾��*/
    HI_U8       ucProdType;                         /**<��Ʒ�汾 -- #HI_FTM_PRODUCT_TYPE_E*/
    HI_U8       reserved[3];                          /**<����*/
} HI_ND_MULTINETWORK_INFO_S;

/**
 *@ingroup hct_mac
 * ��������ϱ�
 */
typedef struct
{
    HI_NDC_CHL_ACCESS_MODE_E enAccessMode;      /**<��ʾ������ͨ��ģ�����PLC�����ģʽ*/

    HI_U8  ucStatus :3;                             /**<��ʾNDC��ǰ״̬*/
    HI_U8  ucIndType:3;                             /**<�ϱ����ͣ������ϱ�����ȫ���ϱ���������״̬����*/
    HI_U8  reserved0:2;                             /**<����*/
    HI_U8  ucCurrentFreq;                           /**<��ʾ��ǰ����������Ƶ��*/
    HI_U8  ucNodeNum;                              /**<��ʾ�������Ķ��������*/
    HI_U8  pad;                                    /**<����*/

    HI_U32  ulCurrentSnid;                          /**<��ʾ��ǰ�����������*/

    HI_U32 ulErrCode;                               /**<��ʾ����/�����Ĵ�����*/
    HI_U32 ulDuration;                              /**<�����������ʱ������λ�롣*/

    HI_ND_MULTINETWORK_INFO_S stNode[0];            /**<�������������ucNodeNum��Ϊ0��Ч*/
} HI_ND_NETWORK_SEARCH_CONN_IND_S;

/**
 *@ingroup hct_mac
 * ��������ϱ�����
 */
typedef struct
{
   HI_ND_NETWORK_SEARCH_CONN_IND_S stInd;   /**<�μ�#HI_ND_NETWORK_SEARCH_CONN_IND_S����*/
} HI_MDM_NET_SEARCH_CONN_IND_S;

/**
 *@ingroup hct_mac
 * ָʾ��������˸�����ָʾ�Ƹ���
 */
#define LED_MAX_FLASH_NUM             3

/**
 *@ingroup hct_mac
 * ָʾ����˸���
 */
typedef struct
{
    HI_U8 light_on_cnt;                 /**<������˸�ڶ��ٸ�duration�󣬵���ָʾ��*/
    HI_U8 light_off_cnt;                /**< ������˸�ڶ��ٸ�duration�󣬵���ָʾ��*/
    HI_U16 duration;                    /**< ������˸��С��λʱ�䣬ms*/

    HI_U8 led_num;                      /**< ������˸��ָʾ�Ƹ��������Ϊ#LED_MAX_FLASH_NUM��*/
    HI_U8 led_id[LED_MAX_FLASH_NUM];    /**< ������˸��ָʾ��ID,�μ�#HI_LED_ID_E*/
}HI_MDM_LED_FLASH_PARA_S;


/**
* @ingroup  hct_mac
* @brief  ͨ�����PLC���ٷ�����ɺ�֪ͨAPP�Ļص���������
*
* @par  ����:
* ͨ�����PLC���ٷ�����ɺ�֪ͨAPP�Ļص���������
* @attention ���ò�Ʒ����ΪCCO��STA����������V100Ƶ�Ρ�
* @param  ulResult [OUT] ���� #HI_U32�����ٷ��͵Ľ����#HI_ERR_SUCCESS �ɹ������෵��ֵ�μ�hi_errno.h��
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
typedef HI_VOID(*HI_MAC_BBPLCSENDRESULTFUN)(HI_U32 ulResult);

/**
 *@ingroup hct_mac
 * ���ٷ���ģʽ����
 */

typedef enum
{
    HI_MAC_FAST_SEND_BACKOFF,
    HI_MAC_FAST_SEND_PREEMPT,
    HI_MAC_FAST_SEND_MAX
}HI_MDM_FAST_SEND_MODE_E;

/**
 *@ingroup hct_mac
 * ���ڿ��PLC���ٷ��͵����
 */
typedef struct
{
    HI_U8   SendPhase;              /**<������λ*/
    HI_U8   SendFreq;               /**<����Ƶ��*/
    HI_U8   Tmi;                    /**<�ּ�����ģʽ*/
    HI_U8   ExtTmi;                 /**<��չ�ּ�����ģʽ*/

    HI_U8   ResendCnt;              /**<�ش�����*/
    HI_BOOL AllPhaseSendFlag;       /**<����ͬʱ���ͱ�־,���ΪTRUE��ʾͬʱ�����෢���з��ͣ�FLASE��ֻ�ڵ��෢��*/
    HI_U8   rsvd[2];                /**<����*/

    HI_U32  Timeout;                /**<��ʱʱ��,��λms*/

    HI_U16  DataLength;             /**<�������ݶ�*/
    HI_U8   Odmac[6];               /**<ָ��ԴʼĿ��mac*/

    HI_BOOL IsMacEncode;            /**<�����Ƿ��Ѿ���װ��mac���ʽ�����ΪTRUE�������а���FC��pb*/
    HI_U8   PbNum;                  /**<PB�����*/
    HI_U16  PbLen;                  /**<PB�鳤��*/

    HI_U8   SendMode;               /**<���ٷ���ģʽ��1��ʾ��ռ���ͣ�0��ʾ�˱ܷ���*/
    HI_U8   BroadcastFlag;          /**<�㲥��־��1��ʾ�㲥��0��ʾ�ǹ㲥*/
    HI_U16  DataIndex;              /**<���ͱ��ĵ�����*/


    HI_MAC_BBPLCSENDRESULTFUN BbPlcSendResultFun;  /**<���ͽ���ϱ��ص�����*/

    HI_U8  Data[0];              /**<��������*/
}HI_MDM_BBPLC_SEND_PARA_S;


/**
* @ingroup  hct_mac
* @brief  ͨ�����ز����ٷ�����ɺ�֪ͨAPP�Ļص���������
*
* @par  ����:
* ͨ�����ز����ٷ�����ɺ�֪ͨAPP�Ļص���������
* @attention ���ò�Ʒ����ΪCCO��STA����������V100Ƶ�Ρ�
* @param  ulResult [OUT] ���� #HI_U32�����ٷ��͵Ľ����#HI_ERR_SUCCESS �ɹ������෵��ֵ�μ�hi_errno.h��
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
typedef HI_VOID(*HI_MAC_SCSENDRESULTFUN)(HI_U32 ulResult);

/**
 *@ingroup hct_mac
 * ���ڵ��ز����ٷ��͵����
 */
typedef struct
{
    HI_U8 SendPhase;                               /**<������λ*/
    HI_U8 DataLength;                              /**<���ĳ���*/
    HI_U8 SendMode;                                /**<���ٷ���ģʽ��1��ʾ��ռ���ͣ�0��ʾ�˱ܷ���*/
    HI_U8 ResendNum;                               /**<�ظ����ʹ���*/

    HI_U16 DataIndex;                              /**<���ͱ�������*/
    HI_U8 Odmac[6];                                /**<ָ��ԴʼĿ��mac*/

    HI_U32 Timeout;                                /**<��ʱʱ��,��λms*/

    HI_BOOL AllPhaseSendFlag;                      /**<����ͬʱ���ͱ�־,���ΪTRUE��ʾͬʱ�����෢���з��ͣ�FLASE��ֻ�ڵ��෢��*/
    HI_U8   rsvd[3];

    HI_MAC_SCSENDRESULTFUN ScSendResultFun;        /**<���ͽ���ص�����*/
    HI_U8 Data[0];                                 /**<���Ļ����ַ*/
}HI_MAC_SC_SEND_PARA_S;

/**
 *@ingroup hct_mac
 * �ϱ����յ��Ŀ��ٷ����������
 */
typedef struct
{
    HI_U8 *DataAddr;                /**<���յ�����ָ��*/

    HI_U16 DataIndex;               /**<���յ���������*/
    HI_U16 DataLength;              /**<���յ����ĳ���*/

    HI_U8 Odmac[6];                 /**<Ŀ��mac ��ַ*/
    HI_BOOL IsMacDecode;            /**<���������Ƿ��Ѿ���ȥmac���װֻ�ϱ����ݣ�TRUE��ʾ��ȥ*/
    HI_U8 Rsvd;                     /**<����*/
}HI_MAC_REPORT_RCV_FAST_SEND_PKT_S;

/**
 *@ingroup hct_mac
 * ���õ��ز��������ʵ����
 */
typedef struct
{
    HI_U8  FecRate;             /**<����*/
    HI_U8  ModulationMode;      /**<����ģʽ*/
    HI_U16 FsFreq;              /**<������*/
    HI_U16 MaxSendLength;       /**<���������ı��ĳ���*/
    HI_BOOL ScEnable;            /**<���ز�ʹ�ܱ�־��HI_TRUE��ʾʹ�ܣ�HI_FALSE��ʾȥʹ��*/
    HI_U8  rsvd;
}HI_MAC_SC_PARA_S;

/**
* @ingroup  hct_mac
* @brief  ���տ��ٷ��͵����ݺ�֪ͨAPP�Ļص���������
*
* @par  ����:
* ͨ�����ز����ٷ�����ɺ�֪ͨAPP�Ļص���������
* @attention ���ò�Ʒ����ΪCCO��STA����������V100Ƶ�Ρ�
* @param  pReportData [OUT] ���� #HI_MAC_REPORT_RCV_FAST_SEND_PKT_S������������Ϣ��ָ�롣
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
typedef HI_VOID (*HI_MDM_pfnFastSendCallback)(HI_MAC_REPORT_RCV_FAST_SEND_PKT_S * pReportData);


 /**
 * @ingroup hct_mac
 * վ��������Ϣ
 */
 typedef struct
 {
     HI_U8    MyLevel;                       /**<��վ��㼶*/
     HI_U8    MyRole;                        /**<��վ���ɫ*/
     HI_U8    ProxyCommRate;                 /**<�����վ���ͨ�ųɹ���*/
     HI_U8    rsvd;                          /**<����*/

     HI_U16   MyTei;                         /**<��վ��tei*/
     HI_U16   MyProxyTei;                    /**<��վ�㸸����*/

     HI_U32   MySnid;                        /**<��վ��SNID*/

     HI_U8    CcoMac[HI_PLC_MAC_ADDR_LEN];   /**<CCO MAC ��ַ*/
     HI_U8    MyMac[HI_PLC_MAC_ADDR_LEN];    /**<��վ��MAC��ַ*/

     HI_U8    ProductType;                   /**<APP���õĲ�Ʒ����*/
     HI_U8    FinalSelectFreq;               /**<��������Ƶ��*/
     HI_U16   ProtocolVer;                   /**<��ǰִ�е�Э��汾*/

 } HI_MDM_NETWORK_ATTRIBUTE_S;


 /**
 * @ingroup hct_mac
 * ���ٹر�֪ͨ��ǰ״̬
 */
typedef enum
{
    HI_MDM_FAST_NOTICE_FINISHED,    /**<�������/δ����*/
    HI_MDM_FAST_NOTICE_PROCESSING,  /**<���ڴ���*/
} HI_MDM_FAST_NOTICE_STATUS_E;

typedef struct
{
    HI_U16 usTotal;     /**<STAվ������*/
    HI_U16 usSuccess;   /**<�յ�Ӧ���STAվ������*/
    HI_U16 usFailed;    /**<��β�ѯδӦ���ж�Ϊʧ�ܵ�STAվ������*/
    HI_U16 usProcess;   /**<���ڴ����վ������*/
    HI_U8  ucStatus;    /**<��ǰ״̬#HI_MDM_FAST_NOTICE_STATUS_E*/
    HI_U8  pad[3];      /**<Ԥ��*/
} HI_MDM_FAST_NOTICE_INFO_S;

/**
* @ingroup  hct_mac
* @brief  �ѿ��ٹر�֪ͨ�Ľ��֪ͨAPP�Ļص���������
*
* @par  ����:
* �ѿ��ٹر�֪ͨ�Ľ��֪ͨAPP�Ļص���������
* @attention ���ò�Ʒ����ΪCCO����������V100Ƶ�Ρ�
* @param  pstInf [OUT] ���� #HI_MDM_FAST_NOTICE_INFO_S������������Ϣ��ָ�롣
* @param  pParam [IN] ���� #HI_PVOID���û��Զ��������ע��ʱ���룬�ص�ʱ�ش���
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
typedef HI_VOID (*HI_MDM_pfnCcoFastNoticeCallback)(HI_MDM_FAST_NOTICE_INFO_S *pstInf, HI_PVOID pParam);


/**
* @ingroup  hct_mac
* @brief  ���տ��ٹر�֪ͨ�����ݺ�֪ͨAPP�Ļص���������
*
* @par  ����:
* ���տ��ٹر�֪ͨ�����ݺ�֪ͨAPP�Ļص���������
* @attention ���ò�Ʒ����ΪSTA����������V100Ƶ�Ρ�
* @param  ucData [OUT] ���� #HI_U8���ϱ����ٹر�ָ��
* @param  pParam [IN] ���� #HI_PVOID���û��Զ��������ע��ʱ���룬�ص�ʱ�ش���
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
typedef HI_VOID (*HI_MDM_pfnStaFastNoticeCallback)(HI_U8 ucData, HI_PVOID pParam);

/**
 *@ingroup hct_mac
 * ȫ��λ
 */
#define PHASE_ALL     (0)
/**
 *@ingroup hct_mac
 * A��λ
 */
#define PHASE_A       (1)
/**
 *@ingroup hct_mac
 * B��λ
 */
#define PHASE_B       (2)
/**
 *@ingroup hct_mac
 * C��λ
 */
#define PHASE_C       (3)
/**
 *@ingroup hct_mac
 * ����λ
 */
#define INVALID_PHASE   (4)
/**
 *@ingroup hct_mac
 * ��λ����
 */
#define PHASE_CNT      3
/**
 *@ingroup hct_mac
 * ����߽�����ȷ
 */
#define PN_CONNECTION_RIGHT     (0)
/**
 *@ingroup hct_mac
 * ����߽��߷���
 */
#define PN_CONNECTION_ERROR     (1)
/**
 *@ingroup hct_mac
 * ����߽������δ֪
 */
#define INVALID_PN_CONNECTION   (2)
/**
 *@ingroup hct_mac
 * ��Ƶ�������: ˫��
 */
#define POWER_EDGE_DOUBLE       (0)
 /**
  *@ingroup hct_mac
  * ��Ƶ�������: ������
  */
#define POWER_EDGE_RISE         (1)
 /**
  *@ingroup hct_mac
  * ��Ƶ�������: �½���
  */
#define POWER_EDGE_FALL         (2)
 /**
  *@ingroup hct_mac
  * ��Ƶ�������: ����δ֪
  */
#define POWER_EDGE_UNKNOWN      (3)
 /**
  *@ingroup hct_mac
  * ��Ƶ������ظ���
  */
#define POWER_EDGE_CNT          (2)

/**
* @ingroup hct_mac
 * STA��Ʒ����: �Ƿ�
 */
#define STA_TYPE_INVALID (0)
/**
* @ingroup hct_mac
 * STA����Ϊ���
 */
#define STA_TYPE_METER   (1)
/**
* @ingroup hct_mac
 * STA����ΪI�Ͳɼ���
 */
#define STA_TYPE_CLTI    (2)

/**
* @ingroup hct_mac
 * ��ʾ�ѱ����
 */
#define LED_SEARCH_METER_END        1
/**
* @ingroup hct_mac
 * ��ʾ�ѱ�ʼ
 */
#define LED_SEARCH_METER_BEGIN      0
/**
* @ingroup hct_mac
 * ��ʾ�����ַʧ��
 */
#define LED_READ_METER_FAIL         1
/**
* @ingroup hct_mac
 * ��ʾ�����ַ�ɹ�
 */
#define LED_READ_METER_SUCCESS      0

/**
* @ingroup hct_mac
 * ���ջ���չLED��IONum �澯��
 */
#define NDM_LED_RECVIVER_ALARM          (IO_EXP_IO_NUM_05)
/**
* @ingroup hct_mac
 * �������չLED��IONum A��ʶ���
 */
#define NDM_LED_TRANSMITTER_A           (IO_EXP_IO_NUM_00)
/**
* @ingroup hct_mac
 * �������չLED��IONum B��ʶ���
 */
#define NDM_LED_TRANSMITTER_B           (IO_EXP_IO_NUM_01)
/**
* @ingroup hct_mac
 * �������չLED��IONum C��ʶ���
 */
#define NDM_LED_TRANSMITTER_C           (IO_EXP_IO_NUM_05)
/**
* @ingroup hct_mac
 * �������չLED��IONum �澯��
 */
#define NDM_LED_TRANSMITTER_ALARM       (IO_EXP_IO_NUM_12)
/**
* @ingroup hct_mac
 * ���ջ�/����� ������չIO Wifi Enable IO
 */
#define NDM_IO_WIFI_ENABLE              (IO_EXP_IO_NUM_11)
/**
* @ingroup hct_mac
 * ���ջ�/����� ������չIO Beep IO
 */
#define NDM_IO_BEEP_ENABLE              (IO_EXP_IO_NUM_14)

/**
* @ingroup hct_mac
 * ���ջ�/����� �ڲ�IO �����շ���
 */
#define NDM_LED_DATA_RECV_SEND          (HI_LED_IDX_3)

/**
* @ingroup hct_mac
 * �������
 */
typedef struct
{
    HI_U8 bProxy:1;       /**<��ǰվ���Ƿ�Ϊ����*/
    HI_U8 bCco:1;         /**<��ǰվ���Ƿ�Ϊͷ��*/
    HI_U8 ucLayer;        /**< ��ǰվ�����ڲ㼶��*/
    HI_U8 ucChildLayer;   /**< ��ǰվ����ӽڵ�Ĳ㼶��*/
    HI_U16 reserved;        /**< ����*/
    HI_U16 usNodeNum;    /**< ��ǰվ���һ���ӽڵ������0��ʾû���ӽڵ�*/
    HI_U16 usTei;        /**< ��ǰģ���TEI*/
} HI_MAC_NET_PARAM_QRY_S;

/**
* @ingroup hct_mac
* ����MAC��ַ��С�����Ŀ��
*/
#define HI_MAC_VALID_MAC_LIST_MAX 2048

/**
* @ingroup hct_mac
* MAC��ַ��Ŀ�ṹ
*/
typedef struct
{
    HI_U8  mac[6];                  /**<MAC��ַ*/
    HI_U8  pad[2];                   /**<����*/
} HI_MAC_ENTRY_STRU;

/**
*@ingroup hct_mac
* ����MAC����
*/
typedef struct
{
    HI_U8 list_num;          /**<mac_list������ȡֵ��Χ1-32*/
    HI_U8 clear_flag;        /**<ȡֵ: #HI_TRUE ��ʾ�������MAC��ַ�б�*/
    HI_U8 type;               /**<mac���ͣ����Ա��ַ:#HI_MDM_METER_MAC������NV:#HI_MDM_NV_MAC,���Բɼ�����ַ:#HI_MDM_CLT_MAC*/
    HI_U8 reserved;          /**<����*/
    HI_MAC_ENTRY_STRU mac_list[0];      /**< MAC��ַ��Ŀ�ṹ��*/
}HI_MAC_ATTR_STRU;

/**
* @ingroup hct_mac
* MAC��ַ��Ŀ
*/
typedef struct
{
   HI_U16 list_num;              /**<MAC��ַ��Ŀ*/
   HI_U16 pad;                   /**<����*/
   HI_MAC_ENTRY_STRU  mac_list[HI_MAC_VALID_MAC_LIST_MAX];  /**<MAC��ַ��Ŀ�ṹ*/
}HI_MAC_VALID_MAC_LIST_STRU;

/**
* @ingroup hct_mac
* ������˥��ģʽ
*/
typedef enum
{
    NORMAL_STATUS = 0,     /**<����ģʽ*/
    ATTENUATE_STATUS,      /**<˥��ģʽ*/
}en_ndm_status;

 /**
 * @ingroup hct_mac
 * �������仯��ԭ����(HSOҲ��ʹ��)
 */
typedef enum
{
    HI_MAC_WHIST_LIST_CHANGE_REASON_UNKNOWN = 0x00,                   /**< ԭ��δ֪       */
    HI_MAC_WHIST_LIST_CHANGE_REASON_HSO_NV = 0x5,                                 /**< �޸�NV���NV       */
    HI_MAC_WHIST_LIST_CHANGE_REASON_HSO_BASE_LIB,                       /**< ������Ϣ�ⷢ���仯     */
    HI_MAC_WHIST_LIST_TF_IDENTIFY,                                                      /**< ̨��ʶ��       */
    HI_MAC_WHIST_LIST_CHANGE_REASON_HSO_SWITCH_MODE,              /**< HSO�����޸İ�������ģʽ     */

    HI_MAC_WHIST_LIST_CHANGE_REASON_MAC_REQ = 0x1f,                    /**< MAC��ַ����       */
    HI_MAC_WHIST_LIST_CHANGE_REASON_APP_INIT,                               /**< App��ʼ������       */
    HI_MAC_WHIST_LIST_CHANGE_REASON_APP_START_SEARCH,              /**<�����ѱ� */
    HI_MAC_WHIST_LIST_CHANGE_REASON_APP_STOP_SEARCH,                /**<ֹͣ�ѱ� */
    HI_MAC_WHIST_LIST_CHANGE_REASON_APP_CON,                                /**<������������� */
    HI_MAC_WHIST_LIST_CHANGE_REASON_CLT_JOIN_NET,                       /**<δ�Ա��ַ�����Ĳɼ���վ�㱻�ܾ����������ӵ������� */
    HI_MAC_WHIST_LIST_CHANGE_REASON_TEST_MODE,                          /**<�ͼ�汾���رհ�����*/
    HI_MAC_WHIST_LIST_CHANGE_REASON_TF_BEGIN,                   /**< ����̨��ʶ��*/
    HI_MAC_WHIST_LIST_CHANGE_REASON_TF_END,                     /**< ֹ̨ͣ��ʶ��*/
    HI_MAC_WHIST_LIST_CHANGE_REASON_MAX = 0x3f,                              /**<ԭ�������ֵ*/
}HI_MAC_WHIST_LIST_CHANGE_REASON_ENUM;

/**
* @ingroup hct_mac
* APPҵ��״̬����
*/
typedef enum
{
    HI_MAC_BUSINESS_FREE = 0,                       /**<ҵ�����״̬*/
    HI_MAC_BUSINESS_BUSY,                           /**<ҵ��æ״̬*/
}HI_MAC_BUSINESS_STATE_ENUM;

/**
* @ingroup  hct_mac
* @brief  ̨��ʶ��������ˢ��ʱ֪ͨAPP�Ļص���������
*
* @par  ����:
* ̨��ʶ��������ˢ��ʱ֪ͨAPP�Ļص���������
* @attention ���ò�Ʒ����ΪCCO��
* @param  state [IN] ���� #HI_U32��̨������ˢ�µ�״̬��Ĭ��Ϊ#HI_MDM_TRANSFORMER_LIST_REFRESH_STATE_UNKNOWN��
* @retval   ret ����HI_U32��#HI_ERR_SUCCESS �ɹ������෵��ֵ�μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_REPORT_TRANSFORMER_LIST_REFRESH_FUN)(HI_U32 state);

/**
* @ingroup  hct_mac
* @brief  ��λʶ��������ˢ��ʱ֪ͨAPP�Ļص���������
*
* @par  ����:
* ��λʶ��������ˢ��ʱ֪ͨAPP�Ļص���������
* @attention ���ò�Ʒ����ΪCCO��
* @param  state [IN] ���� HI_U32����λʶ������ˢ�µ�״̬��Ĭ��Ϊ#HI_MDM_PHASE_LIST_REFRESH_STATE_UNKNOWN��
* @retval   ret ����HI_U32��#HI_ERR_SUCCESS �ɹ������෵��ֵ�μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_REPORT_PHASE_LIST_REFRESH_FUN)(HI_U32 state);

/**
* @ingroup hct_mac
 * ̨��ʶ������վ����Ϣ
 */
typedef struct
{
    HI_U16 tei;                         /**< վ���TEI, �����ߵ�վ���ֵΪ0        */
    HI_U8  is_online    : 1;         /**< վ���Ƿ����ߣ�0: �����ߣ�1: ����       */
    HI_U8  pad1         : 3;         /**< ����       */
    HI_U8  result       : 4;          /**< ̨��ʶ������0: δ֪��1: ��̨��վ�㣬2: �Ǳ�̨��վ�㣬3: ��վ�㲻֧��̨��ʶ��     */
    HI_U8  pad[3];                    /**< ����       */

    HI_U8  mac[HI_PLC_MAC_ADDR_LEN];     /**< վ���MAC��ַ       */
}HI_MDM_TRANSFORMER_LIST_ENTRY_S;

/**
* @ingroup hct_mac
 * ̨��ʶ����
 */
typedef struct
{
    HI_U16 num;                                      /**< �ϱ�����а�����վ����        */
    HI_U16 pad;                                      /**< ����         */
    HI_MDM_TRANSFORMER_LIST_ENTRY_S *pEntry;        /**< �ϱ�վ����Ϣ��Ŀ     */
}HI_MDM_TRANSFORMER_LIST_S;

/**
* @ingroup hct_mac
 * ��������ĳ��վ�����Ϣ
 */
typedef struct
{
    HI_U8  mac[HI_PLC_MAC_ADDR_LEN];               /**< վ���mac ��ַ       */
    HI_U8  mode;                                                /**< �洢���ͣ�����     */
    HI_U8  pad;                                                  /**< ����     */
}HI_MDM_BLACK_LIST_ENTRY_S;

/**
* @ingroup hct_mac
 * ���ڶ�����������
 */
typedef struct
{
    HI_U16 num;                                     /**< �ϱ�վ���mac ��ַ��Ŀ ��        */
    HI_U16 pad;                                    /**< ����         */
    HI_MDM_BLACK_LIST_ENTRY_S *pEntry;         /**< �ϱ�վ���mac ��ַ��Ŀ         */
}HI_MDM_BLACK_LIST_S;

/**
* @ingroup hct_mac
 * �����ѱ�Ľӿ�
 */
typedef struct
{
    HI_U32 time;            /**<̨��ʶ��ĳ���ʱ�䣬��λ:����*/
}HI_MDM_TF_START_SEARCH_METER_S;

/**
* @ingroup hct_mac
 * ���屨�ĵĽ���״̬����ǰ���ڲ���ϵͳ
 */
typedef enum
{
    PBB_CORRECT_TYPE,          /**<���յ���PB����ȷ*/
    PBB_ERROR_TYPE,             /**<����PB�����*/
    PBB_NOTHING_TYPE,         /**<����ֻ��FCû��PB��*/
    PBB_MAX_TYPE              /**<���ֵ*/
}HI_MDM_CAPTURE_PKT_PBB_TYPE_EN;

/**
* @ingroup hct_mac
 * ���ڶ���ʹ��
 */
#define HI_MDM_ENABLE  1
/**
* @ingroup hct_mac
 * ���ڶ���ȥʹ��
 */
#define HI_MDM_DISABLE 0
/**
* @ingroup hct_mac
 * ���ڶ�������״̬
 */
typedef enum
{
    FORMING_STATUS = 1,  /**< ������        */
    EVALUATE_STAUS,         /**< �Ż���        */
    EVALUATE_END,             /**< �������Ż����        */
}HI_MDM_NETWORK_STATUS_E;

/**
* @ingroup  hct_mac
* @brief  ץ�������ص�����
* @par  ����:
* ���ڲ�׽ģ����յ��ı���
* @attention �ޡ�
* @param  ucPayloadType [IN] ���� #HI_MDM_CAPTURE_PKT_PBB_TYPE_EN ,�ϱ�payload�����ͣ�����μ�#HI_MDM_CAPTURE_PKT_PBB_TYPE_EN���塣
* @param  pucFcData [IN] ���� #HI_U8 *�����ڴ���ϱ�FC��Ҫ��ָ��ָ��Ŀռ䲻С��16�ֽڡ�
* @param  pucPayload [IN] ���� #HI_U8 *�����ڴ���ϱ���payload��Ҫ��ָ��ָ��Ŀռ䲻С520�ֽڡ�
*
* @retval ret [OUT] ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ����app���гɹ�, #HI_ERR_QUEUE_FULL��ʾ����������
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see #HI_MDM_POWER_FAILURE_REPORT_ST ���塣
* @since HiMDL_V200R001C00
*/
typedef  HI_U32 (*HI_MDM_CAPTURE_FUNCTION)(HI_MDM_CAPTURE_PKT_PBB_TYPE_EN ePayloadType, HI_U8 *pucFcData, HI_U8 *pucPayloadeData);

/**
* @ingroup hct_mac
* ��������STAģ���μ��ܽ�
*/
#define POWER_FAILURE_PLUG_MODULE_GPIO  HI_HW_GPIO26_BACKUP
/**
* @ingroup hct_mac
* ��������STAģ����ѹ��·���ƹܽ�
*/
#define POWER_FAILURE_ELEVATE_POWER_GPIO  HI_HW_GPIO28_BACKUP

/**
* @ingroup hct_mac
* ��������STAģ���ѹ�����жϼ��ܽ�
*/
#define POWER_FAILURE_POWER_DOWN_GPIO   HI_HW_GPIO29_BACKUP

/**
 *@ingroup hct_mac
 * ͣ���ϱ��¼�mac�ϱ���app������
 */
typedef struct
{
    HI_U16 my_tei;                          /**< ��վ��TEI                 */
    HI_U8 device_power_failure  : 1;           /**< �豸ͣ����, �豸ͣ��ΪHI_TRUE,δͣ��ΪHI_FALSE */
    HI_U8 device_not_pull_reset : 1;          /**< �豸�����ܽű��,�豸ͣ��ΪHI_TRUE,δͣ��ΪHI_FALSE */
    HI_U8 is_plug_module : 1;          /**< ģ��γ���ǰγ�ΪHI_TRUE,δ�γ�ΪHI_FALSE */
	HI_U8 pad			 : 5;
}HI_MDM_POWER_FAILURE_REPORT_ST;
/**
 *@ingroup hct_mac
 * ��λ������ʧ�ϱ��¼�mac�ϱ���app������
 */
typedef struct
{
    HI_U8 phase[PHASE_CNT];             /**< �����źű仯����λ, ֵΪHI_TRUE˵��״̬�����仯     */
    HI_U8 pad;

    HI_U32 time_stamp;                      /**< �����źű仯��ʱ��                 */
}HI_MDM_PHASE_POWER_FAILURE_REPORT_ST;

/**
 *@ingroup hct_mac
 * ͣ���ϱ����ܿ���
 */
#define IS_DETECT_POWER_FAILURE         1

/**
 *@ingroup hct_mac
 * STA��ͣ���ϱ��¼�����
 */
typedef enum
{
	HI_MDM_POWER_FAILURE_EVENT_LOCAL = 0,          /**< STA����ͣ���ϱ��¼�             */
	HI_MDM_POWER_FAILURE_EVNET_POWERON,            /**< STAͣ��ָ������¼�             */
    HI_MDM_POWER_ON_EVNET_REPORT,                  /**< STA���ظ����ϱ��¼�             */
	HI_MDM_POWER_FAILURE_EVNET_PLUG_MODULE,        /**< STAģ��γ��¼�             */
	HI_MDM_POWER_FAILURE_EVNET_NOT_POWER_DOWN,     /**< STAģ������ź���ʧ��ѹδ�����¼�             */
} HI_MDM_POWER_FAILURE_EVENT_TYPE_E;


/**
 *@ingroup hct_mac
 * CCO��ͣ���ϱ��¼�����
 */
typedef enum
{
	HI_MDM_CCO_POWER_FAILURE_EVENT_LOCAL = 0,               /**< CCO����ͣ���ϱ��¼�             */
	HI_MDM_CCO_POWER_FAILURE_EVNET_POWERON,                 /**< CCOͣ��ָ������¼�             */
	HI_MDM_CCO_PHASE_POWER_FAILURE_EVNET,                   /**< CCO��λ�����ź���ʧ�¼�     */
    HI_MDM_CCO_PHASE_POWER_ON_EVNET,                        /**< CCO��λ�����źŻָ��¼�     */
} HI_MDM_CCO_POWER_FAILURE_EVENT_TYPE_E;

/**
 *@ingroup hct_mac
 * ������ģ���ѹ�����ѯ���
 */
typedef enum
{
	HI_MDM_TSTA_POWER_DOWN_CHECK_INIT = 0 ,						/**< ������ģ���ѹ�����ѯ��ʼ״̬ */
	HI_MDM_TSTA_POWER_DOWN_CHECK_TIMEOUT ,                  /**< ������ģ���ѹ�����ѯ��ʱ            */
	HI_MDM_TSTA_POWER_DOWN_CHECK_POWER_DOWN,                /**<������ģ���ѹ�����ѯ��ѹ����  */
	HI_MDM_TSTA_POWER_DOWN_CHECK_NORMAL,                   /**< ������ģ���ѹ�����ѯ��ѹ����   */
} HI_MDM_TSTA_POWER_DOWN_EVENT_TYPE_E;

/**
* @ingroup  hct_mac
* @brief  ͣ���ϱ����ܵĻص�����
* @par  ����:
* ��API�ӿ�ʹ�ó���������������Ƶ����û�м�⵽�����ź�(ǿ�绷����)�ҵ�ѹ���䵽9.5V���£��ж�Ϊ�ǲ��ģ��ʱ���ϱ�ͣ���¼���APP
* @attention �ޡ�
* @param  ucType [IN] ���� #HI_U8���¼����͡�STAģ�� �ο� #HI_MDM_POWER_FAILURE_EVENT_TYPE_E��\n
*													CCOģ��ο� #HI_MDM_CCO_POWER_FAILURE_EVENT_TYPE_E
* @param  pCmdData [IN] ���� #HI_MDM_POWER_FAILURE_REPORT_ST *������ָ�롣
* @param  ulDataSize [IN] ���� #HI_U32�����ݴ�С��
* @retval ret [OUT] ���� #HI_U32��#HI_ERR_SUCCESS �ɹ������෵��ֵ�μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see HI_MDM_POWER_FAILURE_REPORT_ST ���塣
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_PowerFailureEventCallback)(HI_U8 ucType,HI_MDM_POWER_FAILURE_REPORT_ST *pCmdData,HI_U32 ulDataSize);
/**
* @ingroup  hct_mac
* @brief  ��λ������ʧ�Ļص�����
* @par  ����:
* ��API�ӿ�ʹ�ó���������n��Ƶ����û�м�⵽�����źţ��ж�Ϊ��λ������ʧ���ϱ���app��n�������ã�Ĭ��������
* @attention �ޡ�
* @param  ucType [IN] ���� #HI_U8���¼����͡��ο� #HI_MDM_POWER_FAILURE_EVENT_TYPE_E��
* @param  pCmdData [IN] ���� #HI_MDM_PHASE_POWER_FAILURE_REPORT_ST *������ָ�롣
* @param  ulDataSize [IN] ���� #HI_U32�����ݴ�С��
* @retval ret [OUT] ���� #HI_U32��#HI_ERR_SUCCESS �ɹ������෵��ֵ�μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see HI_MDM_PHASE_POWER_FAILURE_REPORT_ST ���塣
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_PhasePowerFailureEventCallback)(HI_U8 ucType,HI_MDM_PHASE_POWER_FAILURE_REPORT_ST *pCmdData,HI_U32 ulDataSize);

/**
 *@ingroup hct_mac
 * ͣ���ϱ��¼��ص�������װ�ṹ��
 */
typedef struct
{
    HI_MDM_PowerFailureEventCallback powerfailure_callback;              /**< ͣ���ϱ��¼��ص�����             */
    HI_MDM_PhasePowerFailureEventCallback phasePowerFailureCallback;     /**< ��λͣ���ϱ��¼��ص�����         */
}HI_MDM_MAC_POWER_FAILURE_ST;

/**
 *@ingroup hct_mac
 * ����λͼ��С
 */
#define POWER_FAILURE_BITMAP_LEN    256

/**
* @ingroup hct_mac
 * ͣ���ϱ��¼�ά����Ϣ
 */
typedef struct
{
    HI_U32 rcv_pwf_frame_time;                                           /**< ���յ�ͣ���ϱ�֡��ʱ��       */

    HI_U8 cco_statistic_pwf_bitmap[POWER_FAILURE_BITMAP_LEN];           /**< ccoͳ�Ƶ�bitmap      */

    HI_U16 pwf_tei_cnt;                                                  /**< ͣ��tei��Ŀ      */
    HI_U16 pad;
}CCO_POWER_FAILURE_STATISTIC_S;

/**
* @ingroup hct_mac
 * ����������Ϣ
 */
typedef struct
{
    HI_U8  lock_status;                             /**< ����״̬ \n
                                                        V100δ������״̬: #HI_MDM_UNLOCK_NETWORK_STATUS \n
                                                        ����MAC״̬:  #HI_MDM_UNLOCK_NETWORK_STATUS \n
                                                        ����Ƶ��snid״̬: #HI_MDM_LOCK_FREQSNID_STATUS \n
                                                        */
    HI_U8  pad1;
    HI_U8  cco_mac[HI_PLC_MAC_ADDR_LEN];    /**< ���������CCO MAC��ַ        */

    HI_U32 lock_freq : 8;                           /**< ����Ƶ��        */
    HI_U32 lock_snid : 24;                          /**< ����SNID        */

    HI_U32 lock_begin_time;                      /**< ������ʼʱ�䣬��λs        */
    HI_U32 lock_duration;                          /**< ����ʱ������λs        */
    HI_U32 leave_time_len;                       /**< ��������ʼ������ʱ������λs        */
} HI_MDM_LOCK_INFO_STRU;

/**
 *@ingroup hct_mac
 * �ھ�������Ϣ�ṹ
 */
typedef struct
{
    HI_U32   ulSnid;                /**< �ھ����������             */

    HI_U8   ucFreq;                 /**< �ھ�����Ƶ��              */
    HI_BOOL bIsSingleComm;          /**< �Ƿ�Ϊ����ͨ��            */
    HI_U8  pad;                     /**< ����λ                    */

    HI_U32  ulBandWidth;            /**< �ھ��������              */
} HI_MDM_NEIGHBOR_NET_ITEM_S;

/**
 *@ingroup hct_mac
 * �ھ��������������
 */
#define HI_MAX_NEIGHBOR_NET_TOTAL_NUM    30


/**
 *@ingroup hct_mac
 * �ھ������ѯ�ṹ
 */
typedef struct
{
    HI_U16 usNum;                                                   /**< �ھ�������Ŀ             */
    HI_U16 pad;                                                     /**< ����λ                   */
    HI_MDM_NEIGHBOR_NET_ITEM_S astItem[HI_MAX_NEIGHBOR_NET_TOTAL_NUM];    /**< �ھ�������Ϣ����         */
} HI_MDM_NEIGHBOR_NET_INFO_S;

/**
* @ingroup  hct_mac
* @brief  ��ѯSTA������ԭ��Ļص���������
*
* @par  ����:
* ��ѯSTA������ԭ��Ļص���������
* @attention ���ò�Ʒ����ΪSTA��
* @param  staNotJoinReason [IN] ���� HI_U8��STA������ԭ�򣬲μ�#HI_MDM_STA_JOIN_NET_STATUS_E��
* @retval   ret ����HI_U32��#HI_ERR_SUCCESS �ɹ������෵��ֵ�μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_StaNotJoinReasonCallback)(HI_U8 staNotJoinReason);

/**
 *@ingroup hct_mac
 * ��������STA�ṹ
 */
typedef struct
{
    HI_U16 usCount;                 /**< ��������STA��Ŀ������С�ڵ���1016             */
    HI_U8 reserved[2];              /**< ����                    */
    HI_U16 ausTeiList[0];           /**< ��������STA��Ϣ             */
}HI_MDM_TEI_LIST_S;

/**
 *@ingroup hct_mac
 * STA״̬ö��
 */
typedef enum
{
    HI_MDM_STA_JOIN_NET_STATUS_NOT_FIND_CCO = 0, /**< STAδ��⵽CCO������,��״̬�������ϱ�*/
    HI_MDM_STA_JOIN_NET_STATUS_NOT_JOIN_NET,     /**< STAδ��������                */
    HI_MDM_STA_JOIN_NET_STATUS_REFUSED,          /**< STA���ܾ���������            */
    HI_MDM_STA_JOIN_NET_STATUS_ONLINE = 3,       /**< STA��������                  */
}HI_MDM_STA_JOIN_NET_STATUS_E;

/**
*@ingroup hct_mac
* STA����ID��Ϣ
*/
typedef struct
{
    HI_U8  aucMacAddr[HI_PLC_MAC_ADDR_LEN];   /**< STA��MAC��ַ                  */
    HI_U8  pad[2];                            /**< ����                  */
    HI_U8  aucStaId[24];                      /**< STA�Ĺ���ID                  */
}HI_MDM_STA_ID_S;

/**
*@ingroup hct_mac
* ��ѯSTA����ID��¼
*/
typedef struct
{
    HI_U32 num;                                                /**< STA�ĸ���                  */
    HI_MDM_STA_ID_S entry[HI_MAC_NETWORK_TOPO_ENTRY_MAX];      /**< STA�Ĺ���ID�б�                  */
}HI_MDM_NETWORK_STA_ID_S;

/**
*@ingroup hct_mac
* �շ�ָʾ������ģʽ
*/

typedef enum
{
    HI_MDM_SET_LED_DISABLE_MODE = 0,                /**< �շ�����ʱ���������շ�ָʾ��*/
    HI_MDM_SET_MAC_LED_MODE,                        /**< �շ�MAC����ʱ�������շ�ָʾ��*/
    HI_MDM_SET_APP_LED_MODE,                        /**< �շ�APP����ʱ�������շ�ָʾ��*/
    HI_MDM_SET_IP_LED_MODE,                         /**< �շ�IP����ʱ�������շ�ָʾ��*/
    HI_MDM_SET_APP_IP_LED_MODE,                     /**< �շ�APP��IP����ʱ�������շ�ָʾ��*/
}HI_MDM_SET_TX_RX_LED_MODE_E;

/**
* @ingroup  hct_mac
* @brief  ͣ��ʱ��ȡ���ܽ��������λ
* @par  ����:
* ͣ��ʱ��ȡ���ܽ��������λ��ģ��ͣ�������reset�ܽţ�����ģ������
* @attention �ޡ�
* @param  �ޡ�
* @retval   ret ���� #HI_U32��\n
                                      @li  #HI_FALSE   �������reset�ܽ�
                                      @li  #HI_TRUE    �������reset�ܽ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_BOOL HI_MDM_GetDevicePullFlag(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ��������n����Ƶ�����޹����ź��ж�ͣ�磬n��ֵ��
* @par  ����:
* ͣ��ʱn����Ƶ�����޹����ź��ж�ͣ��
* @attention �ޡ�
* @param  no_zero_cross_cnt [IN] ���� #HI_U8
* @retval   ret ���� #HI_U32 #HI_ERR_SUCCESS ��ʾע��ɹ���
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetPowerFailureNoZeroCrossCnt(HI_U8 no_zero_cross_cnt);

/**
* @ingroup  hct_mac
* @brief  ��ȡ����n����Ƶ�����޹����ź��ж�ͣ�磬n��ֵ��
* @par  ����:
* ͣ��ʱn����Ƶ�����޹����ź��ж�ͣ��
* @attention �ޡ�
* @param  �ޡ�
* @retval   ret ���� #HI_U8
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetPowerFailureNoZeroCrossCnt(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ����ͣ���⹦���Ƿ�Ϊ������ģ�顣ϵͳĬ��Ϊ�ǵ�����ģ��
* @par  ����:
*  ����ͣ���⹦���Ƿ�Ϊ������ģ��
* @attention  ��
* @param  is_single_tsta [IN] ���� #HI_BOOL�������Ƿ�Ϊ������ģ��\n
* #HI_TRUE �ǵ�����ģ��, # HI_FALSE �ǵ�����ģ�� ��
* @retval  ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetIsSingleTsta(hi_bool is_single_tsta);

/**
* @ingroup  hct_mac
* @brief  APP ����ͣ���⹦��ѹ�����������ڵ�����ģ���е��á�
* @par  ����:
* APP ����ͣ���⹦��ѹ����������
* @attention  ��֧�ַǵ������������ ģ��ʹ��
* @param  power_down_check_result [IN] ���� #power_down_check_result��APP �ж���ѹ������\n
* �ο�#HI_MDM_TSTA_POWER_DOWN_EVENT_TYPE_E��
* @retval  ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetSingleTstaPowerDownCheckResult(hi_u8 power_down_check_result);

/**
* @ingroup  hct_mac
* @brief  ͣ���ϱ��¼�appע��ӿ�
* @par  ����:
* ͣ���ϱ��¼�appע��ӿ�
* @attention �ޡ�
* @param    pfnCallback [IN] ���� #HI_MDM_PowerFailureEventCallback�� ͣ���ϱ����ܵĻص�������
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾע��ɹ���
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see #HI_MDM_pfnPowerFailureEventCallback �ص�������
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_RegisterPowerFailureEvent(HI_MDM_PowerFailureEventCallback pfnCallback);
/**
* @ingroup  hct_mac
* @brief  ��λ�����źű仯�¼�appע��ӿ�
* @par  ����:
* ��λ�����źű仯�¼�appע��ӿ�
* @attention �ޡ�
* @param    phasePfCallback [IN] ���� #HI_MDM_PhasePowerFailureEventCallback��  ��λ�����źű仯�Ļص�������
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾע��ɹ���
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see #HI_MDM_PhasePowerFailureEventCallback �ص�������
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterPhasePowerFailureEvent(HI_MDM_PhasePowerFailureEventCallback phasePfCallback);

typedef struct
{
    HI_U8 ucStaNum;         /**< ��̽��վ�����*/
    HI_U8 pad[3];

    HI_U8 aucMacList[0];    /**< MAC��ַ�б�*/
} HI_MDM_POWEROFF_DETECT_MAC_LIST_S;
/**
* @ingroup  hct_mac
* @brief  �����ھӽڵ�Ӧ��ͣ��̽��
* @par  ����:
* �����ھӽڵ�Ӧ��ͣ��̽��
* @attention �ޡ�
* @param    pstMacList [IN] ���� #HI_MDM_POWEROFF_DETECT_MAC_LIST_S�� ��̽���վ��MAC��ַ�б�
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾִ�гɹ���
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see #HI_MDM_POWEROFF_DETECT_MAC_LIST_S ��̽��վ��MAC��ַ�б�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_HelpNeighborAnswerDetect(HI_IN HI_MDM_POWEROFF_DETECT_MAC_LIST_S * pstMacList);
/**
* @ingroup  hct_mac
* @brief ��ȡ��ѹ�����жϼ����ӿ�
* @par  ����:
* ��ȡ��ѹ�����жϼ����ӿ�
* @attention �������ݵ�δ��ȫ�ͷ���ʱ����ʱ�ϵ磬�����һ�ε����жϡ�
* @param   ��
* @retval  HI_U8
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U8 HI_MDM_VolDeclineCnt(HI_VOID);


/**
* @ingroup  hct_mac
* @brief  ͣ���¼��ϱ������Ƿ���
* @par  ����:
* ͣ���¼��ϱ������Ƿ���
* @attention �ޡ�
* @param    PowerFailureReportEnable [IN] ���� #HI_U8 *��ͣ���¼��ϱ����ܿ��أ�#IS_DETECT_POWER_FAILURE ���ܴ򿪣�����ֵ���ܹرա�
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾͣ���ϱ����ܿ�����#HI_ERR_INVALID_PARAMETER ��ʾ��δ���
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_NM_PowerFailurReportEnable(HI_U8 *PowerFailureReportEnable);

/**
* @ingroup  hct_mac
* @brief  ͣ���¼��ϱ���ȡCCO���͵�����������Ϣ
*
* @par  ����:
* ͣ���¼��ϱ���ȡCCO���͵�����������Ϣ
* @attention ���ò�Ʒ����ΪCCO��
* @param    PowerFailureDfxInfo [IN] ���� #CCO_POWER_FAILURE_STATISTIC_S *��ͣ���ϱ��¼�ά����Ϣ��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ��ȡ��Ϣ�ɹ���#HI_ERR_INVALID_PARAMETER ��ʾ��δ���
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see CCO_POWER_FAILURE_STATISTIC_S
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_NM_PowerFailurCcoDfxInfo(CCO_POWER_FAILURE_STATISTIC_S *PowerFailureDfxInfo);

/**
* @ingroup  hct_mac
* @brief  ��ѯͨ��ģ���PLC��MAC��ַ��IP��ַ
 * @par  ����:
* �ú��������ѯͨ��ģ���PLC��MAC��ַ��IP��ַ
* @attention �ޡ�
* @param    aucMacAddr[HI_PLC_MAC_ADDR_LEN] [OUT] ���� #HI_U8 ͨ��ģ���PLC MAC��ַ ����Ϊ #HI_PLC_MAC_ADDR_LEN��
* @param    aucIpAddr[HI_IPV4_ADDR_LEN] [OUT] ���� #HI_U8 ͨ��ģ���PLC IP��ַ ����Ϊ #HI_IPV4_ADDR_LEN��
* @retval   ret ���� #HI_U32��HI_ERR_SUCCESS ��ʾ�ӿ�ִ�гɹ�������Ϊ�����롣
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_NM_GetAddr(HI_OUT HI_U8 aucMacAddr[HI_PLC_MAC_ADDR_LEN], HI_OUT HI_U8 aucIpAddr[HI_IPV4_ADDR_LEN]);

/**
* @ingroup  hct_mac
* @brief  ��ѯͨ��ģ���PLC��MAC��ַ��IP��ַ����������
 * @par  ����:
* �ú��������ѯͨ��ģ���PLC��MAC��ַ��IP��ַ����������
* @attention �ޡ�
* @param    aucIpAddr[HI_IPV4_ADDR_LEN] [OUT] ���� #HI_U8��ͨ��ģ���վ��IP��ַ ����Ϊ #HI_IPV4_ADDR_LEN��
* @param    aucNetMask[HI_IPV4_ADDR_LEN] [OUT] ���� #HI_U8��ͨ��ģ���վ���������� ����Ϊ #HI_IPV4_ADDR_LEN��
* @param    aucMacAddr[HI_PLC_MAC_ADDR_LEN] [OUT] ���� #HI_U8��ͨ��ģ���վ��MAC��ַ ����Ϊ #HI_PLC_MAC_ADDR_LEN��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ�ӿ�ִ�гɹ�������Ϊ�����롣
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_NM_GetPlcEth(HI_U8 aucIpAddr[HI_IPV4_ADDR_LEN], HI_U8 aucNetMask[HI_IPV4_ADDR_LEN], HI_U8 aucMacAddr[HI_PLC_MAC_ADDR_LEN]);

/**
* @ingroup  hct_mac
* @brief  ���ⲿ��ѯ��������ͼ��Ϊͬ�����ýӿڣ��ڴ����ⲿ���롣
 * @par  ����:
* ���ⲿ��ѯ��������ͼ��Ϊͬ�����ýӿڣ��ڴ����ⲿ���롣
* @attention ���ò�Ʒ����ΪCCO��
* @param  pstQueryInfo [IN]/[OUT] ���� #HI_MAC_CONFIG_INFO_S * �����MAC��ַΪpMac��վ��������Ϣ��\n
                                                        ��ϸ���ݲμ�#HI_MAC_CONFIG_INFO_S���塣
* @retval   ret ���� #HI_U32��\n
                                      @li  #HI_ERR_SUCCESS ��ȡ�ɹ���\n
                                      @li  #HI_ERR_INVALID_PARAMETER ����ָ�����Ϊ�գ�\n
                                      ���෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see HI_MAC_CONFIG_INFO_S���塣
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_QueryInfo(HI_INOUT HI_MAC_CONFIG_INFO_S* pstQueryInfo);
/**
* @ingroup  hct_mac
* @brief  ��ѯ������ĳվ�������
 * @par  ����:
* �ù���ֻ��CCO��Ʒ��̬��Ч
* @attention  ���ò�Ʒ����ΪCCO��
* @param  pMac [IN] ���� #HI_U8 * ����ѯĿ��վ���MAC��ַ��
* @param  pstTopoEntry [OUT] ���� #HI_MAC_NETWORK_TOPO_ENTRY_S * �����MAC��ַΪpMac��վ��������Ϣ��\n
                                                        ��ϸ���ݲμ�#HI_MAC_NETWORK_TOPO_ENTRY_S���塣
* @retval   ret ���� #HI_U32��\n
                                    @li    #HI_ERR_SUCCESS ��ȡ�ɹ���\n
                                    @li    #HI_ERR_INVALID_PARAMETER ����ָ�����Ϊ�գ�\n
                                    @li    #HI_ERR_NOT_EXIST ��ǰ��Ʒ��̬��֧�֣�\n
                                        ���෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see HI_MAC_NETWORK_TOPO_ENTRY_S���塣
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_QueryMacAttr(HI_IN HI_U8 *pMac, HI_MAC_NETWORK_TOPO_ENTRY_S *pstTopoEntry);
/**
* @ingroup  hct_mac
* @brief  �������������
 * @par  ����:
* �������������
* @attention  ���ò�Ʒ����ΪCCO��
* @param  pList [IN] ���� #HI_MAC_VALID_MAC_LIST_STRU * �����õİ�����MAC��ַ�б�
* @param  reason [IN] ���� HI_MAC_WHIST_LIST_CHANGE_REASON_ENUM ���������ı�ԭ��
* @retval  �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see HI_MAC_NETWORK_TOPO_ENTRY_S���塣
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetValidMacList(HI_MAC_VALID_MAC_LIST_STRU* pList, HI_MAC_WHIST_LIST_CHANGE_REASON_ENUM reason);

/**
* @ingroup  hct_mac
* @brief  ������������
 * @par  ����:
* ������������
* @attention  ���ò�Ʒ����ΪCCO��
* @param  pList [IN] ���� #HI_MAC_VALID_MAC_LIST_STRU * ����ӵİ�����MAC��ַ�б�
* @param  reason [IN] ���� HI_MAC_WHIST_LIST_CHANGE_REASON_ENUM �����������ԭ��
* @retval  �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see HI_MAC_VALID_MAC_LIST_STRU���塣
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_AddValidMacList(HI_MAC_VALID_MAC_LIST_STRU* pList, HI_MAC_WHIST_LIST_CHANGE_REASON_ENUM reason);

/**
* @ingroup  hct_mac
* @brief  ɾ�����������
 * @par  ����:
* ɾ�����������
* @attention  ���ò�Ʒ����ΪCCO��
* @param  pList [IN] ���� #HI_MAC_VALID_MAC_LIST_STRU * ��ɾ���İ�����MAC��ַ�б�
* @param  reason [IN] ���� HI_MAC_WHIST_LIST_CHANGE_REASON_ENUM ��������ɾ��ԭ��
* @retval  �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see HI_MAC_VALID_MAC_LIST_STRU���塣
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_DelValidMacList(HI_MAC_VALID_MAC_LIST_STRU* pList, HI_MAC_WHIST_LIST_CHANGE_REASON_ENUM reason);

/**
* @ingroup  hct_mac
* @brief   ��ѯ��ǰ�Ƿ���Է��ͱ��ġ�
 * @par  ����:
* ��ѯ��ǰ�Ƿ���Է��ͱ��ġ�
* @attention �ޡ�
* @param  macbuffer_lid [IN] ���� #HI_U8 ��mac_buffer���Ķ��е����кš�
* @retval   ret ���� #HI_U32��#HI_TRUE ���Է��ͣ�#HI_FALSE �����Է��͡�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/

HI_BOOL HI_MDM_QureySendStatus(HI_IN HI_U8 macbuffer_lid);

/**
* @ingroup  hct_mac
* @brief   �����ѱ�״̬��
*
* @par  ����:
* �����ѱ�״̬����������ָʾ�ư����ѱ�״̬��˸������ָʾ����˸�μ�#HI_MDM_LED_Set485Led
* @attention ���ò�Ʒ����ΪSTA(II��)��
* @param  status [IN] ���� #HI_U8 ���ѱ�״̬��#HI_FALSE:δ�ѱ�#HI_TRUE:�ѱ��С�
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_LED_SetSearchMeterStatus(HI_IN HI_U8 status);

/**
* @ingroup  hct_mac
* @brief   ���ö�ȡ���״̬��
*
* @par  ����:
* ���ö�ȡ���״̬����������ָʾ����˸
* @attention ���ò�Ʒ����ΪSTA(II��)��
* @param  status [IN] ���� #HI_U8 ��ȡ���״̬��#HI_FALSE:��ȡʧ�ܣ�����ʱ����̵���500ms����500ms������ѭ����ʾ��δ����ʱ����̵���9.5s����0.5s��\n
                                              #HI_TRUE:��ȡ�ɹ�������ʱ����ҵ���շ����ĵ�ƣ�δ����ʱ�����3s����1s���̵���
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_LED_SetReadMeterAddrStatus(HI_IN HI_U8 status);

/**
* @ingroup  hct_mac
* @brief   ����485״̬�ơ�
*
* @par  ����:
* ��485״̬���ṩ��ƽӿڣ���485ͨ��ʱ������485��
* @attention ���ò�Ʒ����ΪSTA��
* @param  on_time [IN] ���� #HI_U8 ����ʱ�䳤�̣���λms��
* @retval   ret ���� #HI_U32��#HI_ERR_FAILURE ����ʧ�� #HI_ERR_SUCCESS �����ɹ���
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_LED_Set485Led(HI_IN HI_U8 on_time);

/**
* @ingroup  hct_mac
* @brief   ����ָʾ��
*
* @par  ����:
* ����ָʾ��
* @attention �ޡ�
* @param  LedId [IN] ���� #HI_U8 LED ID ȡֵ��Χ�ο�HI_LED_ID_E��
* @retval   ret ���� #HI_U32��#HI_ERR_FAILURE ����ʧ�� #HI_ERR_SUCCESS �����ɹ���
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_LightOn(HI_U8 LedId);

/**
* @ingroup  hct_mac
* @brief   ����ָʾ��
*
* @par  ����:
* ����ָʾ��
* @attention �ޡ�
* @param  LedId [IN] ���� #HI_U8 LED ID ȡֵ��Χ�ο�#HI_LED_ID_E��
* @retval   ret ���� #HI_U32��#HI_ERR_FAILURE ����ʧ�� #HI_ERR_SUCCESS ����ɹ���
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_LightOff(HI_U8 LedId);
/**
* @ingroup  hct_mac
* @brief   ����LED��ʹ�ü�����
*
* @par  ����:
* ����LED��ʹ�ü�����
* @attention �ޡ�
* @param  LedId [IN] ���� #HI_U8 LED ID ȡֵ��Χ�ο�HI_LED_ID_E��
* @retval   ret ���� #HI_U32��#HI_ERR_FAILURE ����ʧ�� #HI_ERR_SUCCESS ���óɹ���
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_UnLockLed(HI_U8 LedId);
/**
* @ingroup  hct_mac
* @brief   ����LEDʹ�ü�ռ��
*
* @par  ����:
* ����LEDʹ�ü�ռ��
* @attention �ޡ�
* @param  LedId [IN] ���� #HI_U8 LED ID ȡֵ��Χ�ο�HI_LED_ID_E��
* @retval   ret ���� #HI_U32��#HI_ERR_FAILURE ����ʧ�� #HI_ERR_SUCCESS ���óɹ���
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_LockLed(HI_U8 LedId);

/**
* @ingroup  hct_mac
* @brief   ��ȡLED�Ƿ�ռ��
*
* @par  ����:
* ��ȡLED�Ƿ�ռ��
* @attention �ޡ�
* @param  LedId [IN] ���� #HI_U8 LED ID ȡֵ��Χ�ο�HI_LED_ID_E��
* @param  IsLock [OUT] ���� #HI_BOOL,#HI_TRUE ��ʾLED��ռ�ã�#HI_FALSE ��ʾLEDû��ռ�á�
* @retval   ret ���� #HI_U32��#HI_ERR_INVALID_PARAMETER ������Ч #HI_ERR_SUCCESS ��ȡ�ɹ���
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetLedIsLock(HI_U8 LedId,HI_BOOL *IsLock);

/**
* @ingroup  hct_mac
* @brief   ����ָʾ����˸
*
* @par  ����:
* ����ָʾ����˸
* @attention �ޡ�
* @param  led_flash_para [IN] ���� #HI_MDM_LED_FLASH_PARA_S* LED��˸������
* @retval   ret ���� #HI_U32��#HI_ERR_FAILURE ����ʧ�� #HI_ERR_SUCCESS ���óɹ���
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_StartLedFlash(HI_MDM_LED_FLASH_PARA_S *led_flash_para);

/**
* @ingroup  hct_mac
* @brief   ָֹͣʾ����˸
*
* @par  ����:
* ָֹͣʾ����˸
* @attention �ޡ�
* @param  ��
* @retval   ret ���� #HI_U32��#HI_ERR_FAILURE ����ʧ�� #HI_ERR_SUCCESS ���óɹ���
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_StopLedFlash(HI_VOID);

/**
* @ingroup  hct_mac
* @brief   ���÷��ͱ���ָʾ��
*
* @par  ����:
* ���÷��ͱ���ָʾ��
* @attention �ޡ�
* @param  LightDuration [IN] ���� #HI_U32 LED����ʱ����
* @retval   ret ���� #HI_U32��#HI_ERR_FAILURE ����ʧ�� #HI_ERR_SUCCESS ���óɹ���
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_LightOnTx(HI_U32 LightDuration);

/**
* @ingroup  hct_mac
* @brief   ���ý��ձ���ָʾ��
*
* @par  ����:
* ���ý��ձ���ָʾ��
* @attention �ޡ�
* @param  LightDuration [IN] ���� #HI_U32 LED����ʱ����
* @retval   ret ���� #HI_U32��#HI_ERR_FAILURE ����ʧ�� #HI_ERR_SUCCESS ���óɹ���
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_LightOnRx(HI_U32 LightDuration);

/**
* @ingroup  hct_mac
* @brief   ���÷��ͱ��ĵ���λָʾ��
*
* @par  ����:
* ���÷��ͱ��ĵ���λָʾ��
* @attention ���ò�Ʒ����ΪCCO��
* @param  phase [IN] ���� #HI_U8 ��λ��#PHASE_A��#PHASE_B��#PHASE_C��#PHASE_ALL����
* @param  LightDuration [IN] ���� #HI_U32 LED����ʱ����
* @retval   ret ���� #HI_U32��#HI_ERR_FAILURE ����ʧ�� #HI_ERR_SUCCESS ���óɹ���
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_LightPhaseOnTx(HI_U8 phase,HI_U32 LightDuration);

/**
* @ingroup  hct_mac
* @brief   ����LEDģ����빤װģʽ
*
* @par  ����:
* APP�ṩ�ӿڣ�����LEDģ����빤װģʽ
* @attention ���ò�Ʒ����ΪSTA��NDM��
* @param  �ޡ�
* @retval  �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetDutMode(HI_VOID);
/**
* @ingroup  hct_mac
* @brief  �ú���ʵ�������շ�ָʾ�ơ�
*
* @par   ����: �������ֵ���ö�Ӧָʾ��״̬��
* @attention
* @param  mode [IN] ���� #HI_U8��ֵ�μ�#HI_MDM_SET_TX_RX_LED_MODE_E��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ���óɹ���#HI_ERR_INVALID_PARAMETER�·���������#HI_ERR_NOT_SUPPORT����δ���ò�֧�֣�
   #HI_ERR_FAILURE ����ʧ�ܣ����෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetTxRxLedMode(HI_U8 mode);

/**
* @ingroup  hct_mac
* @brief  �ú���ʵ������ָʾ��״̬��
*
* @par   ����:
* HI_TRUE: STA����״ָ̬ʾ�ƣ�HI_FALSE: STA������״ָ̬ʾ��
* @attention
* @param  setflag [IN] ���� #HI_BOOL HI_TRUE:STA����״̬����ָʾ�ƣ�HI_FALSE: STA������״̬����ָʾ�ơ�
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ���óɹ���#HI_ERR_NOT_SUPPORT����δ���ò�֧�֣�
   #HI_ERR_FAILURE ����ʧ�ܣ����෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see setflag���Ƶ�ģ������״̬, �μ�<Hi3911V200 API ����ָ��>"ָʾ�ƿ���"С��
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetLedStateSwitch(HI_BOOL setflag);

/**
* @ingroup  hct_mac
* @brief   �ú���ʵ�����ö�ȡ����ַʧ�ܲ���δ����״̬��ָʾ����˸����ʼ���׶ε��ã�STA��Ʒ��Ч��
*
* @par  ����:
*��˸����:��ָʾ�ƽ��е���light_on_cnt��durtion_ms��Ȼ�����light_off_cnt��durtion_ms����Ϊһ��
*��˸���ڣ�Ȼ���ոò���ѭ����ʾ��
* @attention �ޡ�
* @param  enable_led [IN] ���� #HI_BOOL ��ʾ�Ƿ�ʹ�ñ������ò���������˸��HI_TRUE��ʾ���ã�HI_FALSE��ʾ�����á�
* @param  durtion_ms [IN] ���� #HI_U16 ��ʾָʾ��������˸�����ڣ���λms����СΪ10ms�����򷵻ز�����Ч��
* @param  light_on_cnt [IN] ���� #HI_U8 ��ʾָʾ�Ƶ��������ڸ�������Ч��Χ(0,100]�����򷵻ز�����Ч��
* @param  light_off_cnt [IN] ���� #HI_U8 ��ʾָʾ�Ƶ�������ڸ�������Ч��Χ(0,100]�����򷵻ز�����Ч��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ���óɹ���#HI_ERR_INVALID_PARAMETER �·�������Ч����·���ʧ�ܣ�
*#HI_ERR_BUSY�Ѿ����ýӿ����������ò������ã��������ٴ��·����ò������á�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see LedStateSwitch���Ƶ�ģ������״̬���μ�<Hi3911V200 API ����ָ��>"ָʾ�ƿ���"С��
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetLedNotJoinNetAndReadMeterFail(HI_BOOL enable_led, HI_U16 durtion_ms, HI_U8 light_on_cnt, HI_U8 light_off_cnt);

/**
* @ingroup  hct_mac
* @brief   �ú���ʵ�����ö�ȡ����ַʧ��״̬��ָʾ����˸����ʼ���׶ε��ã�STA��Ʒ��Ч��
*
* @par  ����:
*��˸����:��ָʾ�ƽ��е���light_on_cnt��durtion_ms��Ȼ�����light_off_cnt��durtion_ms����Ϊһ��
*��˸���ڣ�Ȼ���ոò���ѭ����ʾ��
* @attention �ޡ�
* @param  enable_led [IN] ���� #HI_BOOL ��ʾ�Ƿ�ʹ�ñ������ò���������˸��HI_TRUE��ʾ���ã�HI_FALSE��ʾ�����á�
* @param  durtion_ms [IN] ���� #HI_U16 ��ʾָʾ��������˸�����ڣ���λms����СΪ10ms�����򷵻ز�����Ч��
* @param  light_on_cnt [IN] ���� #HI_U8 ��ʾָʾ�Ƶ��������ڸ�������Ч��Χ(0,100]�����򷵻ز�����Ч��
* @param  light_off_cnt [IN] ���� #HI_U8 ��ʾָʾ�Ƶ�������ڸ�������Ч��Χ(0,100]�����򷵻ز�����Ч��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ���óɹ���#HI_ERR_INVALID_PARAMETER �·�������Ч����·���ʧ�ܣ�
*#HI_ERR_BUSY�Ѿ����ýӿ����������ò������ã��������ٴ��·����ò������á�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see LedStateSwitch���Ƶ�ģ������״̬���μ�<Hi3911V200 API ����ָ��>"ָʾ�ƿ���"С��
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetLedReadMeterFail(HI_BOOL enable_led, HI_U16 durtion_ms, HI_U8 light_on_cnt, HI_U8 light_off_cnt);

/**
* @ingroup  hct_mac
* @brief   �ú���ʵ�����ö������ͻ״̬��ָʾ����˸����ʼ���׶ε��ã�STA��Ʒ��Ч��
*
* @par  ����:
*��˸����:��ָʾ�ƽ��е���light_on_cnt��durtion_ms��Ȼ�����light_off_cnt��durtion_ms����Ϊһ��
*��˸���ڣ�Ȼ���ոò���ѭ����ʾ��
* @attention �ޡ�
* @param  enable_led [IN] ���� #HI_BOOL ��ʾ�Ƿ�ʹ�ñ������ò���������˸��HI_TRUE��ʾ���ã�HI_FALSE��ʾ�����á�
* @param  durtion_ms [IN] ���� #HI_U16 ��ʾָʾ��������˸�����ڣ���λms����СΪ10ms�����򷵻ز�����Ч��
* @param  light_on_cnt [IN] ���� #HI_U8 ��ʾָʾ�Ƶ��������ڸ�������Ч��Χ(0,100]�����򷵻ز�����Ч��
* @param  light_off_cnt [IN] ���� #HI_U8 ��ʾָʾ�Ƶ�������ڸ�������Ч��Χ(0,100]�����򷵻ز�����Ч��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ���óɹ���#HI_ERR_INVALID_PARAMETER �·�������Ч����·���ʧ�ܣ�
*#HI_ERR_BUSY�Ѿ����ýӿ����������ò������ã��������ٴ��·����ò������á�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see LedStateSwitch���Ƶ�ģ������״̬���μ�<Hi3911V200 API ����ָ��>"ָʾ�ƿ���"С��
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetLedMultiExpFail(HI_BOOL enable_led, HI_U16 durtion_ms, HI_U8 light_on_cnt, HI_U8 light_off_cnt);

/**
* @ingroup  hct_mac
* @brief   �ú���ʵ������δ����״̬��ָʾ����˸����ʼ���׶ε��ã�STA��Ʒ��Ч��
*
* @par  ����:
*��˸����:��ָʾ�ƽ��е���light_on_cnt��durtion_ms��Ȼ�����light_off_cnt��durtion_ms����Ϊһ��
*��˸���ڣ�Ȼ���ոò���ѭ����ʾ��
* @attention �ޡ�
* @param  enable_led [IN] ���� #HI_BOOL ��ʾ�Ƿ�ʹ�ñ������ò���������˸��HI_TRUE��ʾ���ã�HI_FALSE��ʾ�����á�
* @param  durtion_ms [IN] ���� #HI_U16 ��ʾָʾ��������˸�����ڣ���λms����СΪ10ms�����򷵻ز�����Ч��
* @param  light_on_cnt [IN] ���� #HI_U8 ��ʾָʾ�Ƶ��������ڸ�������Ч��Χ(0,100]�����򷵻ز�����Ч��
* @param  light_off_cnt [IN] ���� #HI_U8 ��ʾָʾ�Ƶ�������ڸ�������Ч��Χ(0,100]�����򷵻ز�����Ч��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ���óɹ���#HI_ERR_INVALID_PARAMETER �·�������Ч����·���ʧ�ܣ�
*#HI_ERR_BUSY�Ѿ����ýӿ����������ò������ã��������ٴ��·����ò������á�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see LedStateSwitch���Ƶ�ģ������״̬���μ�<Hi3911V200 API ����ָ��>"ָʾ�ƿ���"С��
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetLedNotJoinNet(HI_BOOL enable_led, HI_U16 durtion_ms, HI_U8 light_on_cnt, HI_U8 light_off_cnt);

/**
* @ingroup  hct_mac
* @brief  �ú���ʵ��ͨ�������������û�֡
*
* @par  ����:
* ��
* @attention �ޡ�
* @param  usPacketId [IN] ���� #HI_U16������ID��
* @param  pucPacket [IN] ���� #HI_PBYTE����������ָ�롣
* @param  usPacketSize [IN] ���� #HI_U16�����Ĵ�С��
* @param  aucMacAddr[HI_PLC_RELAY_NUM_MAX][HI_PLC_MAC_ADDR_LEN] [IN] ���� #HI_U8��MAC��ַ��
* @param  ulSn [IN] ���� #HI_U32��������š�
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS �ɹ���#HI_ERR_FAILURE ʧ�ܣ����෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see ���Ľ��մ���APPע��ӿڲμ�#mrsStaTransmitRemoteCmd��
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SendNdmReqFrame(HI_U16 usPacketId, HI_PBYTE pucPacket, HI_U16 usPacketSize,
                HI_U8 aucMacAddr[HI_PLC_RELAY_NUM_MAX][HI_PLC_MAC_ADDR_LEN], HI_U32 ulSn);
#if 0
/**
* @ingroup  hct_mac
* @brief  ���÷��͹���ģ�⹦�ʡ�
*
* @par  ����:
* �ú������õ���ģ�⹦�ʵ�ʵ��ֵ��һ����λ˥��2dB�����ֵΪ10����Ӧ˥��20dB ��
* @attention
* <ul><li></li>��ֵ��д��FLASH�У����м��书�ܣ�������Ա���</ul>
* @param  TxAfePgaGainStep [IN] ���� #HI_U8�����ֵΪ10����СֵΪ0��
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_WriteAfePgaGain(HI_U8 TxAfePgaGain);
#endif
/**
* @ingroup  hct_mac
* @brief  ���÷��͹���ģ�����档
*
* @par  ����:
* �ú������õ���ģ�⹦�ʵ����棬����ֱ������ģ�⹦�ʣ�\n
  ʵ�ʵ�ģ��������Ϊ���ģ�⹦����TxAfePgaGainStep�Ĳ\n
  һ����λ˥��2dB�����ֵΪ10����Ӧ˥��20dB ��
* @attention
* <ul><li></li>ʵʱ��Ч�������ָ�ģ�⹦��Ĭ��ֵ</ul>
* @param  TxAfePgaGainStep [IN] ���� #HI_U8�����ֵΪ10����СֵΪ0��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ���óɹ���#HI_ERR_FAILURE ����ʧ�ܣ����෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_ReduceAfePower(HI_IN HI_U8 TxAfePgaGainStep);
/**
* @ingroup  hct_mac
* @brief  ���õ�ǰģ��ģ�ⷢ�͹��ʡ�
*
* @par  ����:
* ���õ�ǰģ��ģ�ⷢ�͹��ʡ�
* @attention
* <ul><li></li> ʵʱ��Ч������ָ�Ĭ��ֵ</ul>
* @param TxAfePgaStep [IN] ���� #HI_U8�����ֵΪ10����СֵΪ0����λΪ2dB��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ���óɹ���#HI_ERR_FAILURE ����ʧ�ܣ����෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see HI_MDM_SetAfePgaGain
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetAfePgaGain(HI_IN HI_U8 TxAfePgaStep);
/**
* @ingroup  hct_mac
* @brief  ��ȡ��ǰģ��ģ�ⷢ�͹��ʡ�
*
* @par  ����:
* ��ȡ��ǰģ��ģ�ⷢ�͹��ʡ�
* @attention
* <ul><li></li> ��#HI_MDM_SetAfePgaGain���</ul>
* @param  �ޡ�
* @retval   ret ���� #HI_U8�����ֵΪ10����СֵΪ0��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see HI_MDM_SetAfePgaGain
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetAfePgaGain(HI_VOID);
/**
* @ingroup  hct_mac
* @brief  ���÷��͹����������档
*
* @par  ����:
* �ĸ���ηֱ�Ϊ�������ز�����Ӧ�����ֹ���ֵ��һ��ʱ��ֻ��һ����Ч��\n
  һ����λ��Ӧ˥��1dB���ú������õ������ֹ��ʵ�����������ֱ���������ֹ��ʣ�\n
  ��ǰ����Ƶ�ε����ֹ�����PowerStep�Ĳ
* @attention
* <ul><li></li>ʵʱ��Ч������ָ�NV 0x67�е�Ĭ��ֵ</ul>
* @param  PowerStep1 [IN] ���� #HI_U8��Ƶ���ز�Ϊ411���ز�ʱ��Ĺ���ֵ��ȡֵ��Χ[0,20]��
* @param  PowerStep2 [IN] ���� #HI_U8��Ƶ���ز�Ϊ131���ز�ʱ��Ĺ���ֵ��ȡֵ��Χ[0,20]��
* @param  PowerStep3 [IN] ���� #HI_U8��Ƶ���ز�Ϊ64���ز�ʱ��Ĺ���ֵ��ȡֵ��Χ[0,20]��
* @param  PowerStep4 [IN] ���� #HI_U8��Ƶ���ز�Ϊ32���ز�ʱ��Ĺ���ֵ��ȡֵ��Χ[0,20]��
* @param  PowerStep5 [IN] ���� #HI_U8��������
* @retval   ret ���� #HI_U32��HI_ERR_INVALID_PARAMETER��ʾ��С��ķ��书�ʳ������书���½�; HI_ERR_SUCCESS�ɹ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_ReducePowerStep(HI_IN HI_U8 PowerStep1,HI_IN HI_U8 PowerStep2,HI_IN HI_U8 PowerStep3,HI_IN HI_U8 PowerStep4,HI_IN HI_U8 PowerStep5);

/**
* @ingroup  hct_mac
* @brief  �������ַ��͹���
*
* @par  ����:
* �ĸ���ηֱ�Ϊ�������ز�����Ӧ�����ֹ���ֵ����ξ��Ϸ�ʱ��ͬʱ�޸����ز��Ĺ���ֵ��һ����λ��Ӧ˥��1dB��\n
  �ú������õ������ֹ���
* @attention
* <ul><li></li>ʵʱ��Ч������ָ����ֹ��ʵ�Ĭ��ֵ</ul>
* @param  PowerStep1 [IN] ���� #HI_S8��Ƶ���ز�Ϊ411���ز�ʱ��Ĺ���ֵ��ȡֵ��Χ[-11,9]��
* @param  PowerStep2 [IN] ���� #HI_S8��Ƶ���ز�Ϊ131���ز�ʱ��Ĺ���ֵ��ȡֵ��Χ-6,14]��
* @param  PowerStep3 [IN] ���� #HI_S8��Ƶ���ز�Ϊ64���ز�ʱ��Ĺ���ֵ��ȡֵ��Χ[-3,17]��
* @param  PowerStep4 [IN] ���� #HI_S8��Ƶ���ز�Ϊ32���ز�ʱ��Ĺ���ֵ��ȡֵ��Χ[0,20]��
* @retval   ret ���� #HI_32��HI_ERR_INVALID_PARAMETER��ʾ��η��书�ʲ��ڹ涨��Χ��;HI_ERR_SUCCESS��ʾ�ɹ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetPowerStep(HI_IN HI_S8 PowerStep1,HI_IN HI_S8 PowerStep2,HI_IN HI_S8 PowerStep3,HI_IN HI_S8 PowerStep4);

/**
* @ingroup  hct_mac
* @brief  ��ȡģ������ֹ��ʡ�
*
* @par  ����:
* ��ȡģ������ֹ��ʡ�
* @attention �ޡ�
* @param  �ޡ�
* @retval   ret ���� #HI_S8����ֵΪ��ǰ���ز����¶�Ӧ�����ֹ��ʡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see HI_MDM_SetPowerStep
* @since HiMDL_V200R001C00
*/
HI_S8 HI_MDM_GetPowerStep(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ���õ�ǰ����Ƶ�ζ�Ӧ�����ֺ�ģ�ⷢ�书�ʣ����ڴ���Ч
*
* @par  ����:
* ���õ�ǰ����Ƶ�ζ�Ӧ�����ֺ�ģ�ⷢ�书��
* @attention
* <ul><li></li> ʵʱ��Ч������ָ�Ĭ��ֵ</ul>
* @param  PowerStep [IN] ���� #HI_S8����ǰ����Ƶ�ζ�Ӧ�Ĺ���ֵ��ȡֵ��ΧӦ���㲻ͬ���ز���Ӧ�Ĺ��ʷ�Χ��
          Ƶ���ز�Ϊ411���ز�ʱ��Ĺ���ֵ��ȡֵ��Χ[-11,9]��
          Ƶ���ز�Ϊ131���ز�ʱ��Ĺ���ֵ��ȡֵ��Χ[-6,14]��
          Ƶ���ز�Ϊ64���ز�ʱ��Ĺ���ֵ��ȡֵ��Χ[-3,17]��
          Ƶ���ز�Ϊ32���ز�ʱ��Ĺ���ֵ��ȡֵ��Χ[0,20]��
          Ƶ���ز�Ϊ49���ز�ʱ��Ĺ���ֵ��ȡֵ��Χ[-2,18]��
          Ƶ���ز�Ϊ89���ز�ʱ��Ĺ���ֵ��ȡֵ��Χ[-4,16]��
* @param TxAfePgaStep [IN] ���� #HI_U8�����ֵΪ10����СֵΪ0����λΪ2dB��
* @retval   ret ���� #HI_32��HI_ERR_INVALID_PARAMETER��ʾ��η��书�ʲ��ڹ涨��Χ��;HI_ERR_SUCCESS��ʾ�ɹ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetCurrWorkAfePower(HI_IN HI_S8 PowerStep, HI_IN HI_U8 TxAfePgaStep);

/**
* @ingroup  hct_mac
* @brief  �Ƿ����V100оƬ��STAվ��
*
* @par  ����:
* �Ƿ����V100оƬ��STAվ��
* @attention ���ò�Ʒ����ΪCCO��
* @param  �ޡ�
* @retval   ret ���� #HI_BOOL #HI_TRUE ���ڣ� #HI_FALSE �����ڡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_IsV100ChipExist(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ���ñ���mac��ַ
*
* @par  ����:
* II�����ñ���mac��ַ
* @attention  ��� #HI_MAC_ATTR_STRU* �е�#HI_MAC_ENTRY_STRU �����ĵ�ַ��Ҫ����Ϊȫ0x00��ȫ0xff��
* @param  mac_attr [IN] ���� #HI_MAC_ATTR_STRU* mac��ַ��Ϣ��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ���óɹ���#HI_ERR_FAILURE ����ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_SetBackupMacAddr (HI_IN HI_MAC_ATTR_STRU *mac_attr);

/**
* @ingroup  hct_mac
* @brief   ����STA��Ʒ���͡�
*
* @par  ����:
* ����STA��Ʒ���͡�
* @attention ���ò�Ʒ����ΪSTA��
* @param  sta_type [IN] ���� #HI_U8 ��STA_TYPE_METER(1)��ʾ������ͣ�STA_TYPE_CLTI(2)��ʾI�Ͳɼ�����
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ���óɹ���#HI_ERR_FAILURE ����ʧ�ܣ����෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_SetSTAProductType(HI_U8 sta_type);
/**
* @ingroup  hct_mac
* @brief   ���ð��������ؽӿ�
*
* @par  ����:
* ���ð��������ؽӿ�
* @attention ���ò�Ʒ����ΪCCO��
* @param  isOn [IN] ���� #HI_BOOL ���������Ƿ�����1��ʾ������0��ʾ�رա�
* @param  isPermanent [IN] ���� #HI_BOOL���´������Ƿ���Ч��1��Ч��0����Ч��
* @param  reason [IN] ���� #HI_MAC_WHIST_LIST_CHANGE_REASON_ENUM���������仯ԭ����,������塣
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ���óɹ���#HI_ERR_FAILURE ����ʧ�ܣ����෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetWhiteListSwitch(HI_BOOL isOn, HI_BOOL isPermanent, HI_MAC_WHIST_LIST_CHANGE_REASON_ENUM reason);
/**
* @ingroup  hct_mac
* @brief   ��ȡ���������ؽӿ�
*
* @par  ����:
* ��ȡ���������ؽӿ�
* @attention ���ò�Ʒ����ΪCCO
* @param  isOn [OUT] ���� #HI_BOOL*
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ��ȡ�ɹ���#HI_ERR_FAILURE ����ʧ�ܣ����෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetWhiteListSwitch(HI_BOOL* isOn);

/**
* @ingroup  hct_mac
* @brief   ������λģʽ
*
* @par  ����:
* ������λģʽ
* @attention �ޡ�
* @param  phase_cnt [IN] ���� #HI_U8 ��λ������CCO����Ч��1 <= phase_cnt <= 3�����øò�������Ҫ�����豸��Ч��
* @param  default_phase [IN] ���� #HI_U8 Ĭ����λ��CCO���ڵ���λģʽʱ����ֵ��Ч\n
          default_phase��ЧֵΪ:#HI_MDM_PLC_PHASE_A:A��λ #HI_MDM_PLC_PHASE_B:B��λ #HI_MDM_PLC_PHASE_C:C��λ��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ���óɹ���#HI_ERR_FAILURE ����ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_WritePhaseMode(HI_IN HI_U8 phase_cnt,HI_IN HI_U8 default_phase);

/**
* @ingroup  hct_mac
* @brief   ������λģʽ
*
* @par  ����:
* ������λģʽ�����޸��ڴ�
* @attention ����CCO��װ����ʱ�ı���CCOʹ�á�
* @param  phase_cnt [IN] ���� #HI_U8 ��λ������CCO����Ч��1 <= phase_cnt <= 3�����øò����󣬲���Ҫ������
* @param  default_phase [IN] ���� #HI_U8 Ĭ����λ��CCO���ڵ���λģʽʱ����ֵ��Ч\n
          default_phase��ЧֵΪ:#HI_MDM_PLC_PHASE_A:A��λ #HI_MDM_PLC_PHASE_B:B��λ #HI_MDM_PLC_PHASE_C:C��λ��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ���óɹ���#���� ����ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_SetPhaseMode(HI_IN HI_U8 phase_cnt,HI_IN HI_U8 default_phase);

/**
* @ingroup  hct_mac
* @brief   �������ģʽȷ��
*
* @par  ����:
* �ڳ�������STA����CCO�ϵ��øýӿڣ�ȷ�ϵ�ǰ�Ƿ�Ϊ���������������ģʽ
* @attention �ޡ�
* @param �ޡ�
* @retval   ret ���� #HI_BOOL ȡֵ #HI_FALSE �����������ģʽ��#HI_TRUE  ���������ģʽ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_BOOL HI_MDM_IsNdmMeterAccess(HI_VOID);
/**
* @ingroup  hct_mac
* @brief  ��ѯSTA��̨��ʶ�������Ƶ�ʵ�ƽ��ֵ
*
* @par  ����:
* ��ѯSTA��̨��ʶ�������Ƶ�ʵ�ƽ��ֵ
* @attention ���ò�Ʒ����ΪSTA��NDM��
* @param  pfnCallback [IN] ���� QueryZeroCrossCallback���μ�#QueryZeroCrossCallback���塣
* @retval   ret ���� #HI_U32 ȡֵ @li #HI_ERR_SUCCESS ��ȡ�ɹ�\n
                                       @li #HI_ERR_BUSY    ���ڲ�ѯ��\n
                                       @li #HI_ERR_FAILURE ����ʧ��\n
                                       @li #HI_ERR_NOT_SUPPORT δ������
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_QueryZeroCross(QueryZeroCrossCallback pfnCallback);
/**
* @ingroup  hct_mac
* @brief  ��װ����ռ�CCO,STA��Ʒ�Ĺ��������
*
* @par  ����:
* ��װ����ռ�CCO,STA��Ʒ�Ĺ��������
* @attention �ޡ�
* @param  enable [IN] ���� #HI_BOOL��ȡֵ: #HI_TRUE ��ʾ�����ɼ���#HI_FALSE  ��ʾֹͣ�ɼ���
* @param  ucLevel [IN] ���� #HI_U8��ȡֵ: #EQUIP_ZERO_CROSS_LEVEL_240K ��ʾ240k��#EQUIP_ZERO_CROSS_LEVEL_1M  ��ʾ1M��
* @param  ucPhaseMode [IN] ���� #HI_U8, ȡֵ: #PHASE_A ��ʾA�࣬#PHASE_B ��ʾB�࣬#PHASE_C ��ʾC��
* @param  edge_mdoe[IN]���� #HI_U8��ȡֵ: #EQUIP_ZERO_CROSS_CRICUIT_RISE��ʾ�����أ�#EQUIP_ZERO_CROSS_CRICUIT_FALL��ʾ�½��أ�����������Ч�����CCO��Ʒ��Ч��������Ʒ��Ч��
* @param  pfnCallback [IN] ���� CollectNtbCallback��ע��ص�������
* @retval   ret ���� #HI_U32��\n
                                       @li #HI_ERR_SUCCESS ��ȡ�ɹ�\n
                                       @li #HI_ERR_BUSY    �������ݲɼ���\n
                                       @li #HI_ERR_FAILURE ����ʧ��\n
                                       @li #HI_ERR_NOT_SUPPORT STA��Ʒδ���������·��ظô���CCO��Ʒ��֧���½�������·��ظô���\n
                                       @li #HI_ERR_INVALID_PARAMETER ��������
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_CollectNtb(HI_BOOL enable, HI_U8 ucLevel, HI_U8 ucPhaseMode, HI_U8 edge_mode, CollectNtbCallback pfnCallback);
/**
* @ingroup  hct_mac
* @brief  �·�����STA,NDM��Ʒ�Ĺ��㵵λ
*
* @par  ����:
* �·�����STA,NDM��Ʒ�Ĺ��㵵λ
* @attention ���ò�Ʒ����ΪSTA��NDM��
* @param  ucLevel [IN] ���� #HI_U8��ȡֵ:\n @li #EQUIP_ZERO_CROSS_LEVEL_240K ��ʾ240k\n
                                            @li #EQUIP_ZERO_CROSS_LEVEL_1M  ��ʾ1M��
* @retval   ret ���� #HI_U32��\n
                                      @li  #HI_ERR_SUCCESS ���óɹ�\n
                                      @li  #HI_ERR_FAILURE ����ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetZeroCrossLevel(HI_U8 ucLevel);
/**
* @ingroup  hct_mac
* @brief  �·����ò�Ʒ�Ĺ���У����ѹ
*
* @par  ����:
* �·����ò�Ʒ�Ĺ���У����ѹ
* @attention �ޡ�
* @param  vlotage [IN] ���� #HI_S16��ȡֵ: vlotage ��Чֵ��Χ[-511, 511]
* @retval   ret ���� #HI_U32��\n
                                       @li #HI_ERR_SUCCESS ���óɹ�\n
                                       @li #HI_ERR_FAILURE ����ʧ��\n
                                       @li #HI_ERR_INVALID_PARAMETER �����·�����
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_WriteCorrectionVoltage(HI_S16 vlotage);

/**
* @ingroup  hct_mac
* @brief  ��ȡ��Ʒ�Ĺ���У����ѹ
*
* @par   ����:
* ��ȡ��Ʒ�Ĺ���У����ѹ
* @attention
* @param  vlotage [IN] ���� #HI_S16 *
* @retval   ret [IN] ���� #HI_U32��#HI_ERR_SUCCESS ��ȡ�ɹ���#HI_ERR_FAILURE ��ȡʧ��
* @par Dependency:
* <ul><li>hi_mdm.h: �ļ�����������·�����ӿ�.</li></ul>
* @see
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetCorrectionVoltage(HI_S16 *vlotage);

/**
* @ingroup  hct_mac
* @brief  ����̨��ʶ���ܼĴ���ʹ�ܿ���
*
* @par   ����:
* ����̨��ʶ���ܼĴ���ʹ�ܿ���
* @attention
* @param  vlotage [IN] ���� #HI_BOOL,#HI_TRUE��ʾ���ù��ܣ�#HI_FALSE��ʾ�رչ���
* @retval   ret [IN] ���� #HI_U32��#HI_ERR_SUCCESS ���óɹ���#HI_ERR_FAILURE����ʧ��
* @par Dependency:
* <ul><li>hi_mdm.h: �ļ�����������·�����ӿ�.</li></ul>
* @see
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetTfRegEnSwitch(HI_BOOL setflag);

/**
* @ingroup  hct_mac
* @brief  ���ó��ر�ͬ������ֻ�ڳ��ر���װ��ʹ�á�
*
* @par  ����:
* @li ���ó��ر�ͬ������ֻ�ڳ��ر���װ��ʹ�á�
* @attention ���ò�Ʒ����ΪNDM��
* @param  freq_index [IN] ���� #HI_U8��ͬ������Ĺ���ģʽ0~5��ע�⹤��ģ������ֵȡ���ڰ汾���õ�ä��Ƶ�θ�����
* @param  snid [IN] ���� #HI_U32�� ͬ�������snid��ȡֵ1~15��ӦV100��Э�飬16~16777215��ӦV200Э�顣
* @param  cco_mac [IN] ���� #HI_U8��CCO��Mac��ַ��
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ͬ������������óɹ���#HI_ERR_FAILURE ͬ�������������ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetNdmNtbSynchStation(HI_U8 freq_index,HI_U32 snid,HI_U8 cco_mac[HI_PLC_MAC_ADDR_LEN]);

/**
* @ingroup  hct_mac
* @brief  �ж�NTBͬ���Ƿ�ɹ�
*
* @par  ����:
* �ж�NTBͬ���Ƿ�ɹ�
* @attention �ޡ�
* @param  �ޡ�
* @retval   ret ���� #HI_BOOL ȡֵHI_ETRUE ͬ���ɹ��� HI_EFALSE ͬ��ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_CheckNtbIsSynchSuccess(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ע��̨��ʶ����Ϣ��ˢ��ʱ���ϱ��ص�����
*
* @par  ����:
* ע��̨��ʶ����Ϣ��ˢ��ʱ���ϱ��ص�����
* @attention
* <ul><li> ���ò�Ʒ����ΪCCO����֧�������ԵĻص�����ע�ᵽ���ӿ��С�</li></ul>
* @param  func [IN] ���� #HI_MDM_REPORT_TRANSFORMER_LIST_REFRESH_FUN ��
* @retval   ret ����HI_U32��ȡֵ:\n @li #HI_ERR_INVALID_PARAMETER ��ʾ��δ���\n
                                    @li #HI_ERR_SUCCESS ��ʾ�ɹ�\n
                                    @li #HI_ERR_NOT_SUPPORT ��ʾ�ò�Ʒ��̬��֧��̨��ʶ���������ѯ\n
                                    @li ����ֵ����hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterReportTransformerListRefresh(HI_MDM_REPORT_TRANSFORMER_LIST_REFRESH_FUN func);
/**
* @ingroup  hct_mac
* @brief  ע����λʶ����Ϣ��ˢ��ʱ���ϱ��ص�����
*
* @par  ����:
* ע����λʶ����Ϣ��ˢ��ʱ���ϱ��ص�����
* @attention ���ò�Ʒ����ΪCCO��
* @param  func [IN] ���� #HI_MDM_REPORT_TRANSFORMER_LIST_REFRESH_FUN ��
* @retval   ret ����HI_U32��ȡֵ:\n @li #HI_ERR_INVALID_PARAMETER ��ʾ��δ���\n
                                    @li #HI_ERR_SUCCESS ��ʾ�ɹ�\n
                                    @li #HI_ERR_NOT_SUPPORT ��ʾ�ò�Ʒ��̬��֧��\n
                                    @li ����ֵ����hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterReportPhaseListRefresh(HI_MDM_REPORT_PHASE_LIST_REFRESH_FUN func);
/**
* @ingroup  hct_mac
* @brief  ��ѯ̨��ʶ������
*
* @par  ����:
* ��ѯ̨��ʶ������
* @attention ���ò�Ʒ����ΪCCO��
* @param  pstList [IN] ���� #HI_MDM_TRANSFORMER_LIST_S *����ϸ�μ�#HI_MDM_TRANSFORMER_LIST_S �Ķ��塣
* @param  entry_num [IN] ���� HI_U16���û������pstList->pEntryָ��Ŀռ��ܴ洢��վ������
* @retval   ret ����HI_U32��ȡֵ: \n @li #HI_ERR_FAILURE ��ʾ���ݴ���\n
                                     @li #HI_ERR_INVALID_PARAMETER ��ʾ��δ���\n
                                     @li #HI_ERR_SUCCESS ��ʾ�ɹ�\n
                                     @li #HI_ERR_NOT_SUPPORT ��ʾ�ò�Ʒ��̬��֧��̨��ʶ���������ѯ\n
                                     @li ����ֵ����hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see  HI_MDM_TRANSFORMER_LIST_S
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_QueryTransformerList(HI_MDM_TRANSFORMER_LIST_S *pstList, HI_U16 entry_num);
/**
* @ingroup  hct_mac
* @brief  ��ѯ���������Ǳ�̨����վ�㣩
*
* @par  ����:
* ��ѯ���������Ǳ�̨����վ�㣩
* @attention ���ò�Ʒ����ΪCCO��
* @param  pstList [IN] ���� #HI_MDM_BLACK_LIST_S *����ϸ�μ�#HI_MDM_BLACK_LIST_S �Ķ��塣
* @param  entry_num [IN] ���� #HI_U16���û������pstList->pEntryָ��Ŀռ��ܴ洢��վ������
* @retval   ret ����HI_U32��ȡֵ: \n @li #HI_ERR_FAILURE ��ʾ���ݴ���\n
                                     @li #HI_ERR_INVALID_PARAMETER ��ʾ��δ���\n
                                     @li #HI_ERR_SUCCESS ��ʾ�ɹ�\n
                                     @li #HI_ERR_NOT_SUPPORT ��ʾ�ò�Ʒ��̬��֧��̨��ʶ���������ѯ\n
                                     @li ����ֵ����hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_QueryBlackList(HI_MDM_BLACK_LIST_S *pstList, HI_U16 entry_num);
/**
* @ingroup  hct_mac
* @brief   ��Ե�ͨ�Ž��ձ��Ļص�����ע��
*
* @par  ����:
* ��Ե�ͨ�Ž��ձ��Ļص�����ע��
* @attention �ޡ�
* @param  id [IN] ���� #HI_U8����Ե�ͨ������ID��ȡֵ��Χ1~5��
* @param  func [IN] ���� #HI_MDM_NDM_PING_F��
* @retval   ret ���� #HI_U32��#HI_ERR_INVALID_PARAMETER ��β��Ϸ���#HI_ERR_SUCCESS ���ķ��ͳɹ���\n
                           ���෵��ֵ�μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterNdmPingFunc(HI_U8 id, HI_MDM_NDM_PING_F func);

/**
* @ingroup  hct_mac
* @brief  ��Ե�ͨ�ű��ķ���
*
* @par  ����:
* ��Ե�ͨ�ű��ķ���
* @attention �ޡ�
* @param  usId [IN] ���� #HI_U8��NDM��Ե�ͨ������ID��ȡֵ��Χ #HI_MDM_NDM_PING_ID_0 �� #HI_MDM_NDM_PING_ID_5��
* @param  pucMsgPaload [IN] ���� #HI_PBYTE�����ĵ����ݻ����ַ��
* @param  uspucMsgPaloadSize [IN] ���� #HI_U32�����ĵĳ��Ȼ��Զ����ֽڶ��룬���348��λ�ֽڡ�
* @param  pstCtrl [IN] ���� #HI_MDM_MSG_CTRL_S* �����Ŀ����ֽṹ����ϸ��#HI_MDM_MSG_CTRL_S���塣
* @retval   ret ���� #HI_U32��\n
                                     @li  #HI_ERR_INVALID_PARAMETER ��β��Ϸ�\n
                                     @li  #HI_ERR_NOT_ENOUGH_MEMORY    �ڴ�����ʧ��\n
                                     @li  #HI_ERR_SUCCESS ���ķ��ͳɹ�\n
                                     @li  ���෵��ֵ�μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SendNdmPing(HI_U8 usId, HI_PBYTE pucMsgPaload, HI_U32 uspucMsgPaloadSize,
                                    HI_MDM_MSG_CTRL_S* pstCtrl);

/**
* @ingroup  hct_mac
* @brief  ����CCO/STAģ���豸��ʶ����
*
* @par ����:
* ����CCO/STAģ���豸��ʶ����
* @attention �ޡ�
*
* @param  aucDevIdBcd[HI_MDM_DEV_ID_LEN] [IN] 6���ֽڵ�BCD�룬�����֡�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_mac.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_DevIdConfig(HI_U8 aucDevIdBcd[HI_MDM_DEV_ID_LEN]);

/**
* @ingroup  hct_mac
* @brief ����STA��ռ�ձȲ���
*
* @par  ����:
* ����STA��ռ�ձȲ���
* @attention ���ò�Ʒ����ΪSTA��
* @param  duty_enable [IN] ���� #HI_BOOL���Ƿ�����ռ�ղ��ԡ�
* @param  duty_percent [IN] ���� #HI_U8��ռ�ձȡ�
* @param  max_sendlength [IN] ���� #HI_U32�������������� ʱ������λms��
* @retval   ret ���� #HI_U32��\n
                                     @li #HI_ERR_SUCCESS ���óɹ�\n
                                     @li #HI_ERR_FAILURE  ����ʧ��\n
                                     @li ���෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetMacDutyCycleCtrlParam(HI_IN HI_BOOL duty_enable,HI_IN HI_U8 duty_percent,HI_IN HI_U32 max_sendlength);

/**
* @ingroup  hct_mac
* @brief �����ű�����
*
* @par  ����:
* �����ű�����
* @attention ���ò�Ʒ����ΪCCO��
* @param  MinPeriod [IN] ���� #HI_U32����С�ű����ڣ���λms��ȡֵ��Χ:1000ms-10000ms��
* @param  MaxPeriod [IN] ���� #HI_U32������ű����ڣ���λms��ȡֵ��Χ:1000ms-10000ms��
* @retval   ret ���� #HI_U32��\n
                              @li #HI_ERR_SUCCESS ���óɹ�\n
                              @li #HI_ERR_FAILURE ����ʧ��\n
                              @li ���෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetTdmaPeriod(HI_IN HI_U32 MinPeriod,HI_IN HI_U32 MaxPeriod);

/**
* @ingroup  hct_mac
* @brief ��ȡ�ű�����
*
* @par  ����:
* �����ű�����
* @attention ���ò�Ʒ����ΪCCO��
* @param  MinPeriod [OUT] ���� #HI_U32����С�ű����ڣ���λms��ȡֵ��Χ:1000ms-10000ms��
* @param  MaxPeriod [OUT] ���� #HI_U32������ű����ڣ���λms��ȡֵ��Χ:1000ms-10000ms��
* @retval   ret ���� #HI_U32��\n
                              @li #HI_ERR_SUCCESS ��ȡ�ɹ�\n
                              @li #HI_ERR_FAILURE ����ʧ��\n
                              @li ���෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetTdmaPeriod(HI_OUT HI_U32 *MinPeriod, HI_OUT HI_U32 *MaxPeriod);

/**
* @ingroup  hct_mac
* @brief ��ȡ��ǰ�ű����ڵĳ���
*
* @par  ����:
* ��ȡ��ǰ�ű�����
* @attention ���ò�Ʒ����ΪCCO��NDM��
* @param  ��
* @retval   ret ���� #HI_U32����ǰ�ű����ڵĳ���,��λ��100us��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetTdmaPeriodLen(HI_VOID);
/**
* @ingroup  hct_mac
* @brief  �����ű�ʱ϶ʱ��
*
* @par  ����:
* �����ű�ʱ϶ʱ��
* @attention ���ò�Ʒ����ΪCCO��
* @param  BeaDuration [IN] ���� #HI_U8��һ���ű�ʱ������λms�� ȡֵ: [0,25]��
* @retval   ret ���� #HI_U32��\n
                              @li #HI_ERR_SUCCESS ���óɹ�\n
                              @li #HI_ERR_FAILURE ����ʧ��\n
                              @li ���෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetBeaconDuration(HI_IN HI_U8 BeaDuration);
/**
* @ingroup  hct_mac
* @brief  ���ö�����Э������
*
* @par  ����:
* ���ö�����Э������
* @attention ���ò�Ʒ����ΪCCO��
* @param  MultiNetworkCoordinateSwitch [IN] ���� #HI_BOOL��ȡֵ#HI_TRUE, ��ʾ�򿪣�#HI_FALSE ��ʾ�رա�
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ��ȡ�ɹ���#HI_ERR_FAILURE ����ʧ�ܣ����෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetMultiNetworkCoordinateSwitch(HI_IN HI_BOOL MultiNetworkCoordinateSwitch);

/**
* @ingroup  hct_mac
* @brief  ��ȡ������Э������
*
* @par  ����:
* ��ȡ������Э������
* @attention ���ò�Ʒ����ΪCCO��
* @param  �ޡ�
* @retval   ret ���� #HI_BOOL��ȡֵ#HI_TRUE, ��ʾ�򿪣�#HI_FALSE ��ʾ�رա�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_GetMultiNetworkCoordinateSwitch(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ���ö��������ʱ��
*
* @par  ����:
* ���ö��������ʱ��
* @attention ���ò�Ʒ����ΪCCO��
* @param  MultiNetworkListenTime [IN] ���� #HI_U32, ���������ʱ�䵥λms��ȡֵ: [100,10000]��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ���óɹ���#HI_ERR_INVALID_PARAMETER ��Ч����Σ����෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetMultiNetWorkListenTime(HI_IN HI_U32 MultiNetworkListenTime);

/**
* @ingroup  hct_mac
* @brief  �����ͼ�ģʽ��Ƶ��ΪȫƵ����֧�ֹ���Э��ʹ�á�
*
* @par  ����:
* �����ͼ�ģʽ��Ƶ��ΪȫƵ
* @attention �ޡ�
* @param  �ޡ�
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetTestFreq(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ���������ڷ���ʱ����led���ƣ�
*
* @par  ����:
* ���������ڷ���ʱ������led 20ms
* @attention ���ò�Ʒ����ΪNDM��
* @param  �ޡ�
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ���óɹ���#HI_ERR_FAILURE ����ʧ�ܣ����෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_NdmSerialInterfaceTxLedProc(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ���������ڽ���ʱ����led���ƣ�
*
* @par  ����:
* ���������ڽ���ʱ������led 20ms
* @attention ���ò�Ʒ����ΪNDM��
* @param  �ޡ�
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ���óɹ���#HI_ERR_FAILURE ����ʧ�ܣ����෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_NdmSerialInterfaceRxLedProc(HI_VOID);

/**
 * @ingroup hct_mac
* @brief  ����̨��ʶ��ģʽ
*/
typedef enum
{
    SOFT_TF_OFF = 0,                        /**< �ر�����̨��ʶ��*/
    SOFT_TF_ON_WITH_STA_JOIN_BY_TF_RESULT,  /**< ������̨��ʶ��վ�����ʶ������������*/
    SOFT_TF_ON_WITH_STA_JOIN_BY_WHITE,      /**< ������̨��ʶ��վ����ݰ�������������*/
    SOFT_TF_MODE_INVALID,                   /**< �Ƿ�ֵ*/
}HI_MDM_SOFT_TF_MODE_EN;

/**
* @ingroup  hct_mac
* @brief  ��������̨��ʶ��ģʽ��
*
* @par  ����:
* ��������̨��ʶ��ģʽ��
* @attention ���ò�Ʒ����ΪCCO��STA��
* @param  eSoftTfMode [IN] ���� #HI_MDM_SOFT_TF_MODE_EN������̨��ʶ��ģʽ��
* @retval   ret ���� #HI_U32��ȡֵHI_ERR_SUCCESS :����������Ϣ�ɹ������෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetSoftTfMode(HI_MDM_SOFT_TF_MODE_EN eSoftTfMode);

/**
 *@ingroup hct_mac
 * ����̨��ʶ��������ṹ
 */
typedef struct
{
    HI_BOOL bIsTfComplete;                          /**< �Ƿ�ʶ����� */
    HI_BOOL bIsBelongJoinNet;                       /**< �Ƿ����ڼ���̨�� */
    HI_U16  usRequeryTime;                          /**< ����ȴ���ú��ٲ�ѯ����λs */

    HI_U8  aucMainNodeAddr[HI_PLC_MAC_ADDR_LEN];    /**< վ����������ڵ��ַ   */
    HI_U8  bIsMainNodeAddr;                         /**< �Ƿ��ȡ�����ڵ��ַ��δ��ȡ�����ڵ��ַʱ�����MAC��ַ */
    HI_U8  ucPad;
} HI_MDM_SNR_TF_RESULT_S;

/**
* @ingroup  hct_mac
* @brief  ��ѯ����̨��ʶ����
*
* @par  ����:
* ��ѯ����̨��ʶ����
* @attention ���ò�Ʒ����ΪSTA����
* @param  pstResult [OUT] ���� #HI_MDM_SNR_TF_RESULT_S *�����̨��ʶ������
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS :��ѯ�ɹ������෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �μ�hi_mdm.h��#HI_MDM_SNR_TF_RESULT_S���塣
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetSnrTfResult(HI_MDM_SNR_TF_RESULT_S *pstResult);

/**
* @ingroup  hct_mac
* @brief  �������̨��ʶ��NTBʶ����
*
* @par  ����:
* �������̨��ʶ��NTBʶ����
* @attention ���ò�Ʒ����ΪSTA����
* @param is_clear_history [IN] ���� #HI_BOOL���Ƿ����ʶ����ʷ��¼��
* @param is_restart_identify [IN] ���� #HI_BOOL���Ƿ����¿�ʼʶ��
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS :���������Ϣ�ɹ������෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_ClearSoftTfNtbResult(HI_BOOL is_clear_history, HI_BOOL is_restart_identify);

/**
* @ingroup  hct_mac
* @brief  �������繦��
*
* @par  ����:
* �������繦��
* @attention ���ò�Ʒ����ΪSTA��
* @param  lock_duration [HI_IN] ���� #HI_U32������ʱ������λ:S��
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS :���óɹ���#HI_ERR_INVALID_PARAMETER ��ε�ָ��Ϊ�ա�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_LockNetwork(HI_IN HI_U32 lock_duration);
/**
* @ingroup  hct_mac
* @brief  ��ѯ��ǰ������Ϣ��
*
* @par  ����:
* ��ѯ��ǰ������Ϣ��
* @attention ���ò�Ʒ����ΪSTA��
* @param  pstLockInfo [HI_OUT] ���� #HI_MDM_LOCK_INFO_STRU *�����������Ϣ��
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ��ѯ�ɹ���#HI_ERR_INVALID_PARAMETER ��ε�ָ��Ϊ�ա�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �μ�hi_mdm.h��#HI_MDM_LOCK_INFO_STRU���塣
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetLockInfo(HI_OUT HI_MDM_LOCK_INFO_STRU * pstLockInfo);
/**
* @ingroup  hct_mac
* @brief  �������硣
*
* @par  ����:
* �������硣
* @attention ���ò�Ʒ����ΪSTA��
* @param  �ޡ�
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ���ͽ�����Ϣ�ɹ������෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_UnlockNetwork(HI_VOID);
/**
* @ingroup  hct_mac
* @brief  ����������
*
* @par  ����:
* ����������
* @attention ���ò�Ʒ����ΪSTA��
* @param  �ޡ�
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ֪ͨ���������ɹ�(2s����������������������\n
                  ����: ֪ͨʧ�ܣ����ܵ�ǰδ���������෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RejoinNetwork(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ����վ���������
*
* @par  ����:
* ����վ��������硣
* @attention ���ò�Ʒ����ΪSTA�����ù����������վ���ܽ��յ������磬����̨��ʶ���������õ�����Ϊ׼��
* @param cco_addr[HI_PLC_MAC_ADDR_LEN] [IN] ���� #HI_U8 ���������MAC��ַ�������ڵ��ַ(Ĭ��ΪMAC��ַ)��
* @param is_main_node_addr [IN] ���� #HI_BOOL cco_addr�Ƿ�Ϊ���ڵ��ַ��
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS �������ù���������Ϣ�ɹ������෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetBelongNetwork(HI_U8 cco_addr[HI_PLC_MAC_ADDR_LEN], HI_BOOL is_main_node_addr);

/**
* @ingroup  hct_mac
* @brief  �������������Ϣ
*
* @par  ����:
* �������������Ϣ��
* @attention ���ò�Ʒ����ΪSTA��
* @param  �ޡ�
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ���������Ϣ�ɹ������෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_ClearBelongNetwork(HI_VOID);

/**
 * @ingroup hct_mac
* @brief  ����̨��ʶ��NTB�������̿�������
*/
typedef enum
{
    HI_MDM_NTB_SEND_PROCESS_RESUME = 0,         /**< �ָ���������*/
    HI_MDM_NTB_SEND_FREQUENCY_REDUCE,           /**< ���ͷ���Ƶ��*/
    HI_MDM_NTB_SEND_PROCESS_PAUSE,              /**< ��ͣ��������*/
    HI_MDM_NTB_SEND_CTRL_MAX,
}HI_MDM_NTB_SEND_PROCESS_CTRL_EN;

/**
 * @ingroup hct_mac
 * @brief ����̨��ʶ��NTB�������̵�ԭ��: ����
 */
#define HI_MDM_NTB_SEND_CTRL_REASON_READ_METER      (1 << 0)
/**
 * @ingroup hct_mac
 * @brief ����̨��ʶ��NTB�������̵�ԭ��: ����
 */
#define HI_MDM_NTB_SEND_CTRL_REASON_UPGADE          (1 << 1)

/**
* @ingroup  hct_mac
* @brief  ��������̨��ʶ��NTB�������̿���
*
* @par  ����:
* ��������̨��ʶ��NTB�������̿���
* @attention ���ò�Ʒ����ΪCCO����Ҫ��֤ҵ��ͨ������ʱ��ʹ�ô˽ӿڽ�������̨��ʶ���ŵ�ռ�á�
* @param  ctrl_type [IN] ���� #HI_MDM_NTB_SEND_PROCESS_CTRL_EN ����̨��ʶ��NTB���Ϳ������͡�
* @param  reason [IN] ����̨��ʶ��NTB���Ϳ���ԭ�򣬲μ�HI_MDM_NTB_SEND_CTRL_REASON_XX��
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ��Ϣ���ͳɹ������෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetTfNtbSendProcessCtrl(HI_MDM_NTB_SEND_PROCESS_CTRL_EN ctrl_type, HI_U32 reason);

/**
*@ingroup hct_mac
* ̨��ʶ��ص�������������ö��
*/
typedef enum
{
    TF_CALLBACK_OPTION_NOTIFY_START_COLLECT_NTB,    /**< ֪ͨ����NTB���ݲɼ������ݽṹΪTF_NOTIFY_START_COLLECT_NTB_INFO_S */
    TF_CALLBACK_OPTION_NOTIFY_NTB_DATA,             /**< ��֪�ɼ���NTB���ݣ����ݽṹΪTF_NOTIFY_NTB_DATA_INFO_S */
    TF_CALLBACK_OPTION_QUERY_NTB_DATA,              /**< ��ѯ�ɼ���NTB���ݣ����ݽṹΪTF_QUERY_NTB_DATA_INFO_S */
    TF_CALLBACK_OPTION_STA_REPORT_NTB_DATA,         /**< STA�ϱ�NTB���ݸ�APP�����ݽṹΪTF_STA_REPORT_NTB_DATA_INFO_S */
} TF_CALLBACK_OPTION_E;

/**
*@ingroup hct_mac
* ֪ͨ����NTB���ݲɼ���Ϣ����Ӧ�ص�����TF_CALLBACK_OPTION_NOTIFY_START_COLLECT_NTB
*/
typedef struct
{
    HI_U32  start_ntb;                      /**< ��ʼNTB */

    HI_U8   collect_num;                    /**< �ɼ����� */
    HI_BOOL is_incarease_collect_seq;       /**< �Ƿ����Ӳɼ����к� */
    HI_U8   dest_mac[HI_PLC_MAC_ADDR_LEN];  /**< Ŀ��վ��MAC��ַ��ȫFF��ʾ�㲥�� */
} TF_NOTIFY_START_COLLECT_NTB_INFO_S;

/**
*@ingroup hct_mac
* ��֪NTB������Ϣ����Ӧ�ص�����TF_CALLBACK_OPTION_NOTIFY_NTB_DATA
*/
typedef struct
{
    HI_U32  freq : 8;                                /**< ������Դ��Ƶ�κ� */
    HI_U32  nid  : 24;                               /**< ������Դ������� */

    HI_U8   addr[HI_PLC_MAC_ADDR_LEN];               /**< ������Դ�ĵ�ַ��CCOΪ���ڵ��ַ��STAΪ���ַ */
    HI_U16  tei;                                     /**< ������Դ��TEI */

    HI_U8   is_increase_collect_seq;                 /**< �Ƿ����Ӳɼ����кţ�����ʱ��Ч */
    HI_U8   collect_seq;                             /**< �ɼ����кţ�����ʱ��Ч */
    HI_BOOL is_collect_seq_valid;                    /**< �ɼ����к��Ƿ�Ϸ�(��CCO�˲ɼ����к�һ��Ϊ�Ϸ�)������ʱ��Ч */
    HI_U8   collect_edge;                            /**< �ɼ����أ�����μ�POWER_EDGE_XX */

    HI_U16 total_ntb_num;                           /**< �ܵ�NTB���� */
    HI_U8  phase_ntb_num[PHASE_CNT*POWER_EDGE_CNT]; /**< ����λNTB������˳��: ����ʱΪABCXXX��˫��ʱ�½���ABC������ABC */

    HI_U32 ntb_buf[0];                              /**< NTBֵ����phase_diff_num˳��һ�� */
} TF_NOTIFY_NTB_DATA_INFO_S;

/**
*@ingroup hct_mac
* ��ѯNTB������Ϣ����Ӧ�ص�����TF_CALLBACK_OPTION_QUERY_NTB_DATA
*/
typedef struct
{
    HI_U8  dest_mac[HI_PLC_MAC_ADDR_LEN];           /**< ��ѯĿ��MAC��ַ��ȫFF��ʾ�㲥�� */
    HI_U16 pad;
} TF_QUERY_NTB_DATA_INFO_S;

/**
*@ingroup hct_mac
* STA�ϱ���APP���NTB������Ϣ����Ӧ�ص�����TF_CALLBACK_OPTION_STA_REPORT_NTB_DATA
*/
typedef struct
{
    HI_U8  collect_num;             /**< �ɼ�����(��NTB���еĳ���) */
    HI_U8  pad[3];

    HI_U32 ntb_buf[0];             /**< NTB���� */
} TF_STA_REPORT_NTB_DATA_INFO_S;

/**
* @ingroup  hct_mac
* @brief   ����̨��ʶ��ģ��ص�����������
*
* @par  ����:
* ����̨��ʶ��ģ��ص�����������
* @attention ���ò�Ʒ����ΪCCO��STA���ص��������غ󣬳�����ͷ������ڴ棬����Ҫ����ʹ�����ݣ�����ڻص�����ǰ���п�����
* @param  option [IN] ���� #TF_CALLBACK_OPTION_E�������ص������͡�
* @param  data [IN] ���� #HI_U8*�����ûص����ݵ����ݡ�
* @param  data_size [IN] ���� #HI_U16�����ûص����ݵ����ݴ�С��
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see #TF_NOTIFY_STA_COLLECT_NTB_INFO_S��#TF_NOTIFY_NTB_DATA_INFO_S��
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*TF_CALLBACK_FUN)(TF_CALLBACK_OPTION_E option, HI_U8 *data, HI_U16 data_size);

/**
* @ingroup  hct_mac
* @brief   ע��̨��ʶ��ص�����
*
* @par  ����:
* ע��̨��ʶ��ص�����
* @attention ���ò�Ʒ����ΪCCO��STA��
* @param  fun [IN] ���� #TF_CALLBACK_FUN��̨��ʶ��ص��������͡�
* @retval ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS��ʾ��ѯ�ɹ������෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_registerTfCallbackFun(TF_CALLBACK_FUN fun);

/**
* @ingroup  hct_mac
* @brief   ֪ͨSTA�ɼ�NTB����
*
* @par  ����:
* ֪ͨSTA�ɼ�NTB���ݣ��ɼ���ɺ󣬻�ͨ��HI_MDM_registerTfCallbackFunע��Ļص��������ϱ���APP�㣬
* �ϱ��ص�����ΪTF_CALLBACK_OPTION_STA_REPORT_NTB_DATA��
* @attention ���ò�Ʒ����ΪSTA��
* @param  start_ntb [IN] ���� #HI_U32����ʼNTB��
* @param  collect_num [IN] ���� #HI_U8���ɼ�NTB������
* @retval ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS��ʾ������Ϣ�ɹ������෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_notifyStaCollectNtbData(HI_U8 collect_seq, HI_U32 start_ntb, HI_U8 collect_num);

/**
* @ingroup  hct_mac
* @brief   ֪ͨ����NTB����
*
* @par  ����:
* ֪ͨ����NTB����
* @attention ���ò�Ʒ����ΪCCO��STA��
* @param  ntb_data [IN] ���� #TF_NOTIFY_NTB_DATA_INFO_S�����յ���NTB���ݡ�
* @param  data_size [IN] ���� #HI_U16��NTB������ռ�ڴ�ռ䡣
* @retval ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS��ʾ������Ϣ�ɹ������෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_notifyHandleRcvNtbData(TF_NOTIFY_NTB_DATA_INFO_S *ntb_data, HI_U16 data_size);

/**
*@ingroup hct_mac
* ��ȡ��λʶ����Ϣ
*/
typedef struct
{
    HI_U32 is_identify_complete : 1;       /**< �Ƿ�ʶ����� */
    HI_U32 phase_result : 3;               /**< ����1��λʶ���� */
    HI_U32 phase_result_b : 3;             /**< ����2��λʶ���� */
    HI_U32 phase_result_c : 3;             /**< ����3��λʶ���� */
    HI_U32 pn_result : 2;                  /**< ����1��𷴽�ʶ���� */
    HI_U32 pn_result_b : 2;                /**< ����2��𷴽�ʶ���� */
    HI_U32 pn_result_c : 2;                /**< ����3��𷴽�ʶ���� */
    HI_U32 pad : 16;
} HI_MDM_PHASE_IDENTIFY_RESULT_STRU;

/**
* @ingroup  hct_mac
* @brief  ��ȡ��λʶ����
*
* @par  ����:
* ��ȡ��λʶ����(������𷴽ӽ��)
* @attention ��������STA�ˡ�
* @param  phase_identify_info [OUT] ���� #HI_MDM_PHASE_IDENTIFY_RESULT_STRU����λʶ����Ϣ��
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS��ʾ��ѯ�ɹ������෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetPhaseIdentifyResult(HI_OUT HI_MDM_PHASE_IDENTIFY_RESULT_STRU *phase_identify_info);

/**
* @ingroup  hct_mac
* @brief  ����վ�����λʶ����
*
* @par  ����:
* ����վ�����λʶ����(������𷴽ӽ��)
* @attention ��������CCO�ˡ�
* @param  tei [IN] ���� #HI_U16��������վ���TEI��
* @param  phase_identify_info [IN] ���� #HI_MDM_PHASE_IDENTIFY_RESULT_STRU����λʶ������
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS��ʾ������Ϣ���ͳɹ������෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetStaPhaseIdentifyResut(HI_U16 tei, HI_MDM_PHASE_IDENTIFY_RESULT_STRU *phase_identify_info);

/**
* @ingroup  hct_mac
* @brief  ��ȡCCO Mac��ַ����
*
* @par  ����:
* ��ȡCCO Mac��ַ����
* @attention ���ò�Ʒ����ΪSTA��
* @param  pucMac [IN]/[OUT] ���� #HI_U8 * ��ȡcco mac��ַ��
* @retval   ret ���� #HI_U32��ȡֵ��\n
                                @li #HI_ERR_SUCCESS ��ȡcco mac��ַ�ɹ�\n
                                @li #HI_ERR_FAILURE ��ȡcco mac��ַʧ��\n
                                @li #HI_ERR_INVALID_PARAMETER ��β����á�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetCCOMacAddr(HI_U8 *pucMac);

/**
* @ingroup  hct_mac
* @brief  ��ȡ����ڵ�� Mac��ַ����
*
* @par  ����:
* ��ȡ����ڵ�� Mac��ַ����
* @attention ���ò�Ʒ����ΪSTA��
* @param  pucMac [IN]/[OUT] ���� #HI_U8 * ��ȡproxy mac��ַ��
* @retval   ret ���� #HI_U32��ȡֵ��\n
                                @li #HI_ERR_SUCCESS ��ȡproxy mac��ַ�ɹ�\n
                                @li #HI_ERR_FAILURE ��ȡproxy mac��ַʧ��\n
                                @li #HI_ERR_INVALID_PARAMETER ��β����á�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetProxyMacAddr(HI_U8 *pucMac);

/**
* @ingroup  hct_mac
* @brief  ��ȡSTA������ԭ��ע�ắ��
*
* @par  ����:
* ��ȡSTA������ԭ��ע�ắ��
* @attention ���ò�Ʒ����ΪSTA��
* @param  staNotJoinCallback [IN] ���� #HI_MDM_StaNotJoinReasonCallback ������ԭ��ص�������
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ע��ɹ� #HI_ERR_INVALID_PARAMETER ��β����á�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_Register_NotJoinReason(HI_MDM_StaNotJoinReasonCallback staNotJoinCallback);

/**
* @ingroup  hct_mac
* @brief  ʹ��MAC������������ģʽ
*
* @par  ����:
* ʹ��MAC������������ģʽ
* @attention ���ò�Ʒ����ΪCCO��
* @param  snid [IN] ���� #HI_U32 ����ţ�V100Э��ȡֵ��Χ 1-15������Э��ȡֵ��ΧΪ 1- 16777215
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ʹ�ܳɹ���#HI_ERR_FAILURE ʹ��ʧ�ܣ����෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_EnterTestMode(HI_IN HI_U32 snid);
/**
* @ingroup  hct_mac
* @brief  ʹ��MAC���˳��������ģʽ
*
* @par  ����:
* CCO�˳��������ģʽʱ��ģ��Ӳ��λ
* @attention ���ò�Ʒ����ΪCCO��
* @param  �ޡ�
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ��ȡ�ɹ���#HI_ERR_FAILURE ����ʧ�ܣ����෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_ExitTestMode(HI_VOID);

/**
* @ingroup hct_mac
* @brief ����ҵ��״̬
*
* @par ����:
* ��APP��ǰ��ҵ������MAC�ж�ʱ�����øýӿ�֪ͨMAC����ֹMAC������Ӱ��ҵ��Ĳ���(����̨��ʶ����)��\n
��APP������ҵ��ʱ�����øýӿ�֪ͨMAC��ȡ����MAC������Ӱ��ҵ��Ĳ��������ơ�\n
Ŀǰ��Ԥ֪��APPҵ��ֻ������ҵ��
* @attention ���ò�Ʒ����ΪCCO��
* @param businessState [IN] ���� #HI_MAC_BUSINESS_STATE_ENUM businessState��
* @retval ret ���� #HI_U32��#HI_ERR_SUCCESS ��ȡ�ɹ���#HI_ERR_FAILURE ����ʧ�ܣ����෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MAC_InformBusinessState(HI_MAC_BUSINESS_STATE_ENUM businessState);
/**
* @ingroup  hct_mac
* @brief  ��ȡЭ��汾��
*
* @par  ����:
* ��ȡЭ��汾��
* @attention ���ò�Ʒ����ΪCCO��
* @param  �ޡ�
* @retval   protocol_ver [IN] ���� #HI_U16��V100Э��汾��:#HI_MDM_PTCL_VER_100������Э��汾��: #HI_MDM_PTCL_VER_SG��V200Э��汾��: #HI_MDM_PTCL_VER_200��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see hi_mdm.h�ļ���#HI_MDM_PTCL_VER_100�ȶ��塣
* @since HiMDL_V200R001C00
*/
HI_U16 HI_MDM_GetProtocolVer(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ֪ͨ��������STAվ����������
*
* @par  ����:
* ֪ͨ��������STAվ����������
* @attention ���ò�Ʒ����ΪCCO��
* @param  aucStaMac[HI_PLC_MAC_ADDR_LEN] [IN] ���� #HI_U8��
* @param  usStaTei [IN] ���� #HI_U16��
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_INVALID_PARAMETER �����Ч #HI_ERR_SUCCESS ����STA���߳ɹ���
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetStaOffline(HI_U8 aucStaMac[HI_PLC_MAC_ADDR_LEN], HI_U16 usStaTei);

/**
* @ingroup  hct_mac
* @brief  ����CCO��������ɳ�ʱ����
*
* @par  ����:
* ��ʱʱ�䵽�ڣ���վ�����ߣ�ǿ�ƽ�����������Ϊ�������״̬
* @attention ���ò�Ʒ����ΪCCO��
* @param  NetStateFormEnable[IN] ���� #HI_BOOL���·�����ֹͣ���أ�#HI_TRUE����ʾ�����ù��ܣ�#HI_FASLE����ʾֹͣ�ù��ܡ�
* @param  NetStateFormTimeOut [IN] ���� #HI_U32������������ʱʱ�䣬��С�������2���ӣ���λms��
* @retval   ret ���� #HI_U32��ȡֵ��\n
                               @li #HI_ERR_SUCCESS �·����ù��ܳɹ�\n
                               @li #HI_ERR_BUSY ��ʾ�Ѿ��������������ظ�����\n
                               @li #HI_ERR_FAILURE �·�����ʧ��\n
                               @li #HI_ERR_INVALID_PARAMETER ��β����á�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetNetWorkStateFormSwitch(HI_BOOL NetStateFormEnable,HI_U32 NetStateFormTimeOut);

/**
* @ingroup  hct_mac
* @brief  ����STA���߽ӿ�
*
* @par  ����:
* ����STA���߽ӿ�
* @attention ���ò�Ʒ����ΪCCO��
* @param  pstList [IN] ���� #HI_MDM_TEI_LIST_S *��
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_INVALID_PARAMETER �����Ч #HI_ERR_SUCCESS �߳�STA�ɹ���
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see HI_MDM_TEI_LIST_S �ṹ�塣
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetStaOfflineByTeiList(HI_MDM_TEI_LIST_S *pstList);

/**
* @ingroup  hct_mac
* @brief  ��ȡ�ھ�������Ϣ����
*
* @par  ����:
* ��ȡ�ھ�������Ϣ������
* @attention �����ڹ����汾�����ò�Ʒ����ΪCCO��
* @param  pstInfo [IN]/[OUT] ���� #HI_MDM_NEIGHBOR_NET_INFO_S *���洢�ھ�������Ϣ��
* @retval   ret ���� #HI_U32��ȡֵ��\n
                               @li #HI_ERR_SUCCESS ��ȡ�ھ�������Ϣ�ɹ�\n
                               @li #HI_ERR_FAILURE ��ȡ�ھ�������Ϣʧ��\n
                               @li #HI_ERR_INVALID_PARAMETER ��β����á�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see hi_mdm_mac.h �ļ��й��� #HI_MDM_NEIGHBOR_NET_INFO_S�ṹ �Ķ��塣
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetNeighborNetInfo(HI_MDM_NEIGHBOR_NET_INFO_S *pstInfo);
/**
* @ingroup  hct_mac
* @brief  ��ȡ�ھ��������
*
* @par  ����:
* ��ȡ�ھ�������Ϣ������
* @attention ���ò�Ʒ����ΪCCO,STA��
* @param  pstInfo [IN]/[OUT] ���� #HI_U32 *����ȡ�ھ����������
* @retval   ret ���� #HI_U32��ȡֵ��\n
                               @li #HI_ERR_SUCCESS ��ȡ�ھ���������ɹ�\n
                               @li #HI_ERR_INVALID_PARAMETER ��β����á�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetNeighBorNetNum(HI_U32 *pneighborNetNum);
/**
* @ingroup  hct_mac
* @brief  ��ѯ������վ���оƬ����ID
*
* @par  ����:
* ��ѯ������վ���оƬ����ID
* @attention ���ò�Ʒ����ΪCCO��
* @param  pstQueryIdInfo [OUT] ���� #HI_MDM_NETWORK_STA_ID_S *��վ���оƬ����ID��Ϣ��
* @retval   ret ���� #HI_U32��ȡֵ��\n
                               @li #HI_ERR_SUCCESS ��ȡվ���оƬ����ID��Ϣ�ɹ�\n
                               @li #HI_ERR_FAILURE ��ȡվ���оƬ����ID��Ϣʧ��\n
                               @li #HI_ERR_INVALID_PARAMETER ��β����á�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_QueryStaID(HI_OUT HI_MDM_NETWORK_STA_ID_S *pstQueryIdInfo);

/**
 * @ingroup hct_mac
* @brief  ģ�����ID��Ϣ�ĳ���
*/
#define HI_MDM_MANAGEMENT_ID_SIZE            (24)

/**
* @ingroup  hct_mac
* @brief  ����վ��MAC��ַ��ѯվ��Ĺ���ID��Ϣ
*
* @par  ����:
* ��ѯ������վ���оƬ����ID
* @attention ���ò�Ʒ����ΪCCO��
* @param  mac [IN] ���� #HI_U8 *��վ���MAC��ַ��
* @param  management_id [OUT] ���� #HI_U8 *��վ���оƬ����ID��Ϣ��
* @retval   ret ���� #HI_U32��ȡֵ��\n
                               @li #HI_ERR_SUCCESS ��ȡվ���оƬ����ID��Ϣ�ɹ�\n
                               @li #HI_ERR_FAILURE ��ȡվ���оƬ����ID��Ϣʧ��\n
                               @li #HI_ERR_INVALID_PARAMETER ��β����á�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_QueryStaIDByMac(HI_U8 mac[HI_PLC_MAC_ADDR_LEN],HI_U8 management_id[HI_MDM_MANAGEMENT_ID_SIZE]);


/**
* @ingroup  hct_mac
* @brief   ע����֤MAC��ַ�ص�����
*
* @par  ����:
* ע����֤MAC��ַ�ص�����
* @attention ���ò�Ʒ����ΪCCO��
* @param  pfnAuthCallback [IN] ���� #HI_BOOL����֤MAC��ַ�ص�������ΪHI_NULLʱ��ʾȡ��ע��\n
          @li pucMacAddr: MAC��ַ\n
          @li ucProductType: ��Ʒ���� #HI_ND_TYPE_CCO/#HI_ND_TYPE_STA��\n
          @li ucMacType: MAC��ַ���� #HI_MDM_METER_MAC/#HI_MDM_NV_MAC\n
          @li pParam: �û����� �ص���������#HI_TRUE��ʾ��֤�ɹ�������#HI_FALSE��ʾ��֤ʧ�ܡ�
* @param  pParam [IN] ���� #HI_VOID��������֤������#HI_NULL��ʾ�����Ƿ���
* @retval   ret ���� #HI_U32��ȡֵ:\n
                                @li #HI_ERR_SUCCESS ��ʾע��ɹ�\n
                                @li #HI_ERR_INVALID_PARAMETER ��β����á���
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see pfnAuthCallback�ص�����ָ��Ķ��塣
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_RegisterStaAuthCallback(HI_IN HI_BOOL (*pfnAuthCallback)(HI_U8 *pucMacAddr, HI_U8 ucProductType, HI_U8 ucMacType, HI_VOID *pParam), HI_IN HI_VOID *pParam);

/**
* @ingroup  hct_mac
* @brief   �������ڵ��ַ
*
* @par  ����:
* ���ü������·������ڵ��ַ��MAC��
* @attention ���ò�Ʒ����ΪCCO��
* @param  main_node_addr [IN] ���� #HI_U8�����ڵ��ַ����
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ִ�гɹ������෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see pfnAuthCallback�ص�����ָ��Ķ��塣
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetMainNodeAddr(HI_IN HI_U8 main_node_addr[HI_PLC_MAC_ADDR_LEN]);

/**
* @ingroup  hct_mac
* @brief   ����ʹ���ϴε�̨��ʶ��״̬
*
* @par  ����:
* ����ʹ���ϴε�̨��ʶ��״̬
* @attention ���ò�Ʒ����ΪCCO��
* @param  bIsUseLastTfStatus [IN] ���� #HI_BOOL���Ƿ�ʹ���ϴε�̨��ʶ��״̬��#HI_TRUE��ʹ�õ���ǰ�����̨��ʶ��״̬��#HI_FALSE����Ϊʶ��δ��ɣ����¿�ʼʶ��
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ִ�гɹ���#HI_ERR_FAILURE ����ʧ�ܣ����෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see pfnAuthCallback�ص�����ָ��Ķ��塣
* @since HiMDL_V200R001C00
*/
HI_U32  HI_MDM_SetUseLastTfStatus(HI_IN HI_BOOL bIsUseLastTfStatus);

/**
* @ingroup  hct_mac
* @brief   ���SNR̨��ʶ���Ƿ����
*
* @par  ����:
* ���SNR̨��ʶ���Ƿ����
* @attention ���ò�Ʒ����ΪCCO��
* @param  �ޡ�
* @retval   ret ���� #HI_BOOL #HI_TRUE ̨��ʶ������ɣ�#HI_FALSE ̨��ʶ��û����ɡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_IsTfComplete(HI_VOID);

/**
 * @ingroup hct_mac
* @brief  �ز��ڵ�����ԭ�� δ֪
*/
#define HI_ND_OFFLINE_REASON_UNKNOWN            (0x00)
/**
 * @ingroup hct_mac
* @brief  �ز��ڵ�����ԭ�� ������
*/
#define HI_ND_OFFLINE_REASON_METER_POWEROFF     (0x01)
/**
 * @ingroup hct_mac
* @brief  �ز��ڵ�����ԭ�� �ŵ��仯
*/
#define HI_ND_OFFLINE_REASON_PLC_CHANNEL        (0x02)
/**
 * @ingroup hct_mac
* @brief  �ز��ڵ�����״̬�仯 ���߱�Ϊ����
*/
#define HI_ND_STATE_CHANGE_OFFLINE_TO_ONLINE    (0x00)
/**
 * @ingroup hct_mac
* @brief  �ز��ڵ�����״̬�仯 ���߱�Ϊ����
*/
#define HI_ND_STATE_CHANGE_ONLINE_TO_OFFLINE    (0x01)

/**
 * @ingroup hct_mac
* @brief  �����仯�ڵ�����ṹ��
*/
typedef struct
{
    HI_U8  aucMac[6];               /**<  ����״̬�ı�Ĵӽڵ�MAC��ַ         */
    HI_U8  ucStateChange;        /**<  �ýڵ��״̬�仯,ȡֵ�ο�HI_ND_STATE_CHANGE_XXX       */
    HI_U8  ucOfflineReason;      /**<  ��Ϊ���߱�Ϊ���ߣ�����ԭ��δ֪: #HI_ND_OFFLINE_REASON_UNKNOWN��\n
                                                             ������: HI_ND_OFFLINE_REASON_METER_POWEROFF���ŵ��仯: HI_ND_OFFLINE_REASON_PLC_CHANNEL  */
    HI_U32 ulOfflineDuration;    /**<  ��Ϊ���߱�Ϊ���ߣ�����ʱ������λ:��  */

    HI_BOOL is_init_poweroff;   /**<  ͣ���ϱ������Ƿ��ʼ����HI_TRUE��ʾ��ʼ���ù��ܣ�HI_FALSE��ʾδ��ʼ���ù���   */
    HI_U8   pad;
    HI_U16  pad1;
} HI_MDM_NETWORK_TOPOCHG_ENTRY_S;
/**
* @ingroup  hct_mac
* @brief   �������������нڵ�״̬�仯����������
*
* @par  ����:
* �������������нڵ�״̬�仯����������
* @attention ���ò�Ʒ����ΪCCO��
* @param  entry [IN]/[OUT] ���� #HI_MDM_NETWORK_TOPOCHG_ENTRY_S*�������仯�ڵ���������#HI_MDM_NETWORK_TOPOCHG_ENTRY_S���塣
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see hi_mdm_mac.h �ļ���#HI_MDM_Report_Topo_Change_Node_Fun ���塣
* @since HiMDL_V200R001C00
*/
typedef HI_VOID (*HI_MDM_Report_Topo_Change_Node_Fun)(HI_IN HI_MDM_NETWORK_TOPOCHG_ENTRY_S *);
/**
* @ingroup  hct_mac
* @brief   ע�����������нڵ�״̬�仯�ص�����
*
* @par  ����:
* ע�����������нڵ�״̬�仯�ص�����
* @attention ���ò�Ʒ����ΪCCO��
* @param  fun [IN] ���� #HI_MDM_Report_Topo_Change_Node_Fun��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ��ȡ�ɹ���#HI_ERR_FAILURE ����ʧ�ܣ����෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see hi_mdm_mac.h �ļ���#HI_MDM_Report_Topo_Change_Node_Fun ���塣
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_RegisterReportTopoChangeNode(HI_IN HI_MDM_Report_Topo_Change_Node_Fun fun);

/**
* @ingroup hct_mac
* @brief  MAC��ַ��Դ�����Ա��ַ
*/
#define HI_MDM_METER_MAC    (0)
/**
* @ingroup hct_mac
* @brief  MAC��ַ��Դ������NV
*/
#define HI_MDM_NV_MAC       (1)
/**
* @ingroup hct_mac
* @brief  MAC��ַ��Դ�����Բɼ�����ַ
*/
#define HI_MDM_CLT_MAC      (2) //���Բɼ�����ַ
/**
* @ingroup hct_mac
* @brief  MAC��ַ��Դ���������ڵ��ַ
*/
#define HI_MDM_CCO_MAIN_MAC (3)

/**
 * @ingroup hct_mac
* @brief  �������ܾ��ڵ�Ĳ����ṹ��
*/
typedef struct
{
    HI_U8  aucMac[6];       /**<  �������ܽڵ��MAC��ַ         */
    HI_U8  ucMacType;     /**<  �������ܽڵ��MAC��ַ ���ͣ����Ա��ַ: #HI_MDM_METER_MAC������NV: #HI_MDM_NV_MAC�����Բɼ�����ַ: HI_MDM_CLT_MAC */
    HI_U8  ucProduct;       /**<  �������ܽڵ�Ĳ�Ʒ��̬���μ�CCO: #HI_ND_TYPE_CCO��         */
} HI_MDM_NETWORK_REJECT_ENTRY_S;

/**
* @ingroup  hct_mac
* @brief   �������������нڵ��������ܾ�����������
*
* @par  ����:
* �������������нڵ��������ܾ�����������
* @attention ���ò�Ʒ����ΪCCO��
* @param  fun [IN] ���� #HI_MDM_NETWORK_REJECT_ENTRY_S*���������ܾ��ڵ�Ĳ��������#HI_MDM_NETWORK_REJECT_ENTRY_S���塣�r
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see hi_mdm_mac.h �ļ���#HI_MDM_NETWORK_REJECT_ENTRY_S ���塣
* @since HiMDL_V200R001C00
*/
typedef HI_VOID (*HI_MDM_Report_Reject_Node_Fun)(HI_IN HI_MDM_NETWORK_REJECT_ENTRY_S *);
/**
* @ingroup  hct_mac
* @brief   ע�����������нڵ��������ܾ��ص�����
*
* @par  ����:
* ע�����������нڵ��������ܾ��ص�����
* @attention ���ò�Ʒ����ΪCCO��
* @param  fun [IN] ���� #HI_MDM_Report_Reject_Node_Fun��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ��ȡ�ɹ���#HI_ERR_FAILURE ����ʧ�ܣ����෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see hi_mdm_mac.h �ļ���#HI_MDM_Report_Reject_Node_Fun ���塣
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_RegisterReportRejectNode(HI_IN HI_MDM_Report_Reject_Node_Fun fun);

/**
 * @ingroup hct_mac
* @brief  PLCͨ������ʱ���˱��״̬ �������
*/
#define HI_MDM_MAC_NETWORK_FORMED         (1)
/**
 * @ingroup hct_mac
* @brief  PLCͨ������ʱ���˱��״̬ �Ż����
*/
#define HI_MDM_MAC_NETWORK_EVALUATE_END   (2)
/**
 * @ingroup hct_mac
* @brief  PLCͨ������ʱ���˱��״̬ CCO�Ż��к�,վ������
*/
#define HI_MDM_MAC_STA_LEAVE              (3)
/**
 * @ingroup hct_mac
* @brief  PLCͨ������ʱ���˱��״̬ CCO�Ż��к�,վ���������
*/
#define HI_MDM_MAC_STA_JOIN_NET           (4)
/**
 * @ingroup hct_mac
* @brief  PLCͨ������ʱ���˱��״̬�ṹ��
*/
typedef struct
{
    HI_U8 ucStatus;            /**<  PLCͨ������ʱ���˱��״̬���������: #HI_MDM_MAC_NETWORK_FORMED��\n
                                                     �Ż����: #HI_MDM_MAC_NETWORK_EVALUATE_END��\n
                                                     CCO�Ż��к�վ������: #*HI_MDM_MAC_STA_LEAVE��CCO�Ż��к�վ�����: #HI_MDM_MAC_STA_JOIN_NET*/
    HI_U8 pad;
    HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN]; //����ucStatus=HI_MDM_MAC_STA_JOIN_NET�� HI_MDM_MAC_STA_LEAVEʱ��aucMac����Ч����������վ���mac��ַ

}HI_MDM_TOPO_STATUS_CHG_ENTRY_S;

/**
* @ingroup  hct_mac
* @brief   ����ͨ������ʱ�������˱仯֪ͨ����������
*
* @par  ����:
* ����ͨ������ʱ�������˱仯֪ͨ����������
* @attention ���ò�Ʒ����ΪCCO��
* @param  entry [IN]/[OUT] ���� #HI_MDM_TOPO_STATUS_CHG_ENTRY_S*��ͨ�����ò��������#HI_MDM_TOPO_STATUS_CHG_ENTRY_S���塣�r
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see hi_mdm_mac.h �ļ���#HI_MDM_TOPO_STATUS_CHG_ENTRY_S ���塣
* @since HiMDL_V200R001C00
*/
typedef HI_VOID (*HI_MDM_Report_Topo_Status_Chg_Fun)(HI_IN HI_MDM_TOPO_STATUS_CHG_ENTRY_S *);
/**
* @ingroup  hct_mac
* @brief ע��ͨ������ʱ�������˱仯֪ͨ
*
* @par  ����:
* ע��ͨ������ʱ�������˱仯֪ͨ
* @attention ���ò�Ʒ����ΪCCO��
* @param  fun [IN] ���� #HI_MDM_Report_Topo_Status_Chg_Fun��
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ͬ������������óɹ���#HI_ERR_FAILURE ͬ�������������ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see hi_mdm_mac.h �ļ���#HI_MDM_Report_Topo_Status_Chg_Fun�Ķ��塣
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_RegisterReportTopoStatusChg(HI_IN HI_MDM_Report_Topo_Status_Chg_Fun fun);

/**
* @ingroup  hct_mac
* @brief �����������Ϳ�����ʹ�ܱ��
*
* @par  ����:
* �����������Ϳ�����ʹ�ܱ��
* @attention �ޡ�
* @param  fun [IN] ���� #HI_BOOL��#HI_TRUE��ʾ���������������ͣ�#HI_FALSE��ʾ�����������������͡�
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ���óɹ���#HI_ERR_FAILURE ����ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetTopoTypeFlag(HI_BOOL flag);

/**
* @ingroup  hct_mac
* @brief ��ȡ�������Ϳ�����ʹ�ܱ��
*
* @par  ����:
* ��ȡ�������Ϳ�����ʹ�ܱ��
* @attention �ޡ�
* @param  fun [IN] ���� #HI_VOID��
* @retval   ret ���� #HI_BOOL��ȡֵ#HI_TRUE��ʾ���������������ͣ�#HI_FALSE��ʾ�����������������͡�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_GetTopoTypeFlag(HI_VOID);

/**
 * @ingroup hct_mac
* @brief  ������������ö��
*/
typedef enum
{
    HI_MDM_TOPO_TREE = 0,
    HI_MDM_TOPO_STAR,
    HI_MDM_TOPO_MAX
} HI_MDM_TOPO_TYPE_E;

/**
* @ingroup  hct_mac
* @brief ������������
*
* @par  ����:
* ������������
* @attention ���ò�Ʒ����ΪCCO��
* @param  fun [IN] ���� #HI_MDM_TOPO_TYPE_E���������͡�
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ���óɹ���#HI_ERR_FAILURE ����ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see hi_mdm_mac.h �ļ���#HI_MDM_TOPO_TYPE_E�Ķ��塣
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetTopoType(HI_MDM_TOPO_TYPE_E enTopoType);

/**
* @ingroup  hct_mac
* @brief ��ȡ��������
*
* @par  ����:
* ��ȡ��������
* @attention �ޡ�
* @param  fun [IN] ���� #HI_VOID��
* @retval   ret ���� #HI_MDM_TOPO_TYPE_E���������͡�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see hi_mdm_mac.h �ļ���#HI_MDM_TOPO_TYPE_E�Ķ��塣
* @since HiMDL_V200R001C00
*/
HI_MDM_TOPO_TYPE_E HI_MDM_GetTopoType(HI_VOID);

/**
* @ingroup  hct_mac
* @brief   �����ű�֡�û��Զ�����Ŀ���뺯��������
*
* @par  ����:
* �����ű�֡�û��Զ�����Ŀ���뺯��������
* @attention �ޡ�
* @param  entry [IN] ���� #HI_U8����������ַ��
* @param  entry [IN] ���� #HI_U16�����������ȡ�
* @param  entry [OUT] ���� #HI_U16��ʵ�ʱ��볤�ȡ��r
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ���óɹ���#HI_ERR_FAILURE ����ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_Encode_Beacon_User_Fun)(HI_U8 *buf, HI_U16 buf_len, HI_U16 *offset);

/**
* @ingroup  hct_mac
* @brief   �����ű�֡�û��Զ�����Ŀ���뺯��������
*
* @par  ����:
* �����ű�֡�û��Զ�����Ŀ���뺯��������
* @attention �ޡ�
* @param  entry [IN] ���� #HI_U8����������ַ��
* @param  entry [IN] ���� #HI_U16�����������ȡ�
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ���óɹ���#HI_ERR_FAILURE ����ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_Decode_Beacon_User_Fun)(HI_U8 *addr, HI_U16 len);

/**
 * @ingroup hct_mac
* @brief  �ű�֡�û��Զ�����Ŀ����뺯��ע��ṹ��
*/
typedef struct
{
    HI_MDM_Encode_Beacon_User_Fun pfEncode;
    HI_MDM_Decode_Beacon_User_Fun pfDecode;
}HI_MDM_BEACON_USER_FUN_S;

/**
* @ingroup  hct_mac
* @brief ע���ű�֡�û��Զ�����Ŀ����뺯��
*
* @par  ����:
* ע���ű�֡�û��Զ�����Ŀ����뺯��
* @attention �ޡ�
* @param  fun [IN] ���� #HI_MDM_BEACON_USER_FUN_S��
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ���óɹ���#HI_ERR_FAILURE ����ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see hi_mdm_mac.h �ļ���#HI_MDM_BEACON_USER_FUN_S�Ķ��塣
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterBeaconUserDef(HI_MDM_BEACON_USER_FUN_S *fun);

/**
* @ingroup  hct_mac
* @brief  ���÷��;�Ĭ״̬��ȡ�����;�Ĭ״̬��
*
* @par  ����:
* ���÷��;�Ĭ״̬��ȡ�����;�Ĭ״̬��
* @attention �ޡ�
* @param  ucSilenceStatus [IN] ���� #HI_U8��#HI_MDM_ENABLE(1)��ʾ��Ĭ״̬��#HI_MDM_DISABLE(0)��ʾȡ����Ĭ״̬��
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ���óɹ���#HI_ERR_INVALID_PARAMETER ��β��Ϸ���
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see #HI_MDM_ENABLE ��#HI_MDM_DISABLE�Ķ��塣
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetSendSilenceStatus(HI_U8 ucSilenceStatus);

/**
* @ingroup  hct_mac
* @brief  ��ȡ���;�Ĭ״̬��
*
* @par  ����:
* ��ȡ���;�Ĭ״̬��
* @attention �ޡ�
* @param  pu8SilenceStatus [IN] ���� #HI_U8��#HI_MDM_ENABLE(1)��ʾ��Ĭ״̬��#HI_MDM_DISABLE(0)��ʾȡ����Ĭ״̬��
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ��ȡ�ɹ���#HI_ERR_INVALID_PARAMETER ��β��Ϸ���
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see #HI_MDM_ENABLE ��#HI_MDM_DISABLE�Ķ��塣
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetSendSilenceStatus(HI_U8 *pu8SilenceStatus);

/**
* @ingroup  hct_mac
* @brief ��ȡ��ǰ����״̬��
*
* @par  ����:
* ��ȡ��ǰ����״̬��
* @attention ���ò�Ʒ����ΪCCO��
* @param  eNetStatus [IN] ���� #HI_MDM_NETWORK_STATUS_E*��#FORMING_STATUS(1)��ʾ�����С�#EVALUATE_STAUS(2)�Ż��С�#EVALUATE_END(3)�������Ż���ɡ�
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ��ȡ��ǰ����״̬�ɹ���#HI_ERR_INVALID_PARAMETER ָ�����Ϊ�ա�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see HI_MDM_NETWORK_STATUS_E���塣
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetCurrentNetworkStatus(HI_MDM_NETWORK_STATUS_E *eNetStatus);

/**
* @ingroup  hct_mac
* @brief ע��ץ���ص�������Ҫ��ע���ץ��������ִ��ʱ�価���̣ܶ�����Ӱ���ж���Ӧ��ʵʱ�ԡ�
* @par  ����:
* ע�᳭����ץ���ص�������Ҫ��ע���ץ��������ִ��ʱ�価���̣ܶ���������жϴ���
* @attention �ޡ�
* @param  Function [IN] ���� #HI_MDM_CAPTURE_FUNCTION ע��Ļص��������ص�������3������,�������˵���ο����Ͷ��塣
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ͬ������������óɹ���#HI_ERR_INVALID_PARAMETER ָ�����Ϊ�ա�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterCapturePktFunction(HI_MDM_CAPTURE_FUNCTION Function);


/**
* @ingroup  hct_mac
* @brief ע��ץ���ص�����
* @par  ����:
* ע��ץ���ص�����
* @attention �ޡ�
* @param  �ޡ�
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ע���ɹ�������ֵΪ�����롣
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_CancelCapturePktFunction(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ���Ƴ������������ܿ��أ����ر���������ʱ�����������ó�������Ƶ�Ρ�
*
* @par  ����:
* ���Ƴ������������ܿ��أ����ر���������ʱ�����������ó�������Ƶ�Ρ�
* @attention ���ò�Ʒ����ΪNDM��
* @param  ucCtrlFlag [IN] ���� #HI_U8�����ڿ��Ƴ������������ܵĿ��أ�0��ʾ�ر��������ܣ�1��ʾ�򿪡�
* @param  ucSetFreq [IN] ���� #HI_U8�� �������ù���ģʽ0-5��
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ͬ������������óɹ���#HI_ERR_FAILURE ͬ�������������ʧ�ܣ�#HI_ERR_INVALID_PARAMETER ��β����á�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SearchNetworkCtrl(HI_U8 ucCtrlFlag, HI_U8 ucSetFreq);
/**
 * @ingroup hct_mac
* @brief  ��������ģʽ
*/
typedef enum
{
    PHYSICAL_TEST_MODE_NONE = 0,
    PHYSICAL_TEST_MODE_APP_TO_COM = 1,  /**<ת����ǰ֡Ӧ�ò㱨����Ӧ�ò㴮��ͨ��*/
    PHYSICAL_TEST_MODE_APP_TO_PLC = 2,  /**<ת����ǰ֡Ӧ�ò㱨�ĵ�PLCͨ��*/
    PHYSICAL_TEST_MODE_APP_TO_COM_LOOP = 3, /**<�����ز�͸��Ӧ�ò㴮���Զ�����ģʽ�����ֲ���ģʽ��"����ģʽ����ʱ��"���˳�*/
    PHYSICAL_TEST_MODE_APP_TO_PLC_LOOP = 4, /**<�����ز������Զ��ظ��Զ�����ģʽ�����ֲ���ģʽ��"����ģʽ����ʱ��"���˳�*/
    PHYSICAL_TEST_MODE_EXIT = 5,            /**<����MAC��͸��ģʽ��͸�����յ��ı��ĵ�MSDU�������ŵ������ֲ���ģʽ����ʱ��"���˳�*/
    PHYSICAL_TEST_MODE_SET_FREQ = 6,            /**<����Ƶ��,��ǰֻ֧�����ù���Ƶ��*/
    PHYSICAL_TEST_MODE_SET_TONE_MASK_MODE = 7,            /**<����TONEMAP����ǰֻ֧�����ù���Ƶ��*/
    PHYSICAL_TEST_MODE_LOOPBACK = 8,    /**<�����ػ�����ģʽ*/
    PHYSICAL_TEST_MODE_TRANSIMIT = 9,   /**<����͸��ת������ģʽ*/
}HI_MDM_PHYSICAL_TEST_MODE_EN;

/**
* @ingroup  hct_mac
* @brief  ��ȡNTB�Լ�ͬ��״̬
*
* @par  ����:
* ��ȡNTB��ͬ��״̬
* @attention �ޡ�
* @param  isSync [IN]/[OUT] ���� #HI_BOOL *���Ƿ���ͬ������
* @retval   ret ���� #HI_U32��վ����ͬ������ʱ������NTBֵ;û��ͬ������ʱ������0��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetNtb(HI_BOOL *isSync);
/**
* @ingroup  hct_mac
* @brief  ���ù���Ƶ��
*
* @par  ����:
* ���ù���Ƶ��,��ǰ֧�ֹ���4��Ƶ��
* @attention ��֧�ֲ���ϵͳ��ʹ�ã���Ҫ���ܺ�#PRODUCT_CFG_TTCN�򿪡�\n
                ����Ƶ�β��޸�nvֵ��������ʧЧ��
* @param  freq_para [IN] ���� #HI_U8��Ƶ�β��� 0ʱΪ����Ϊ����ȫƵ��1ʱΪ����Ϊ����2.5-5.7mƵ�Σ�\n
                                                2ʱΪ����Ϊ����0.7-3mƵ�Σ�3ʱΪ����Ϊ����1.7-3mƵ�Ρ�
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ���óɹ���#HI_ERR_FAILURE ����ʧ�ܣ����෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_SetWorkFreq(HI_U8 freq_para);

/**
* @ingroup  hct_mac
* @brief  ���ù���Ƶ��(����Э��)
*
* @par  ����:
* ���ù���Ƶ��,��ǰ֧������Ƶ��:2-12m��2.5-5.7m��500k-3.7m
* @attention ��֧�ֲ���ϵͳ��ʹ�ã���Ҫ���ܺ�#PRODUCT_CFG_TTCN�򿪡�\n
                ����Ƶ�β��޸�nvֵ��������ʧЧ��
* @param  freq_para [IN] ���� #HI_U8��Ƶ�β��� 0ʱΪ����Ϊ����ȫƵ��1ʱΪ����Ϊ����2.5-5.7mƵ�Σ�3ʱΪ����Ϊ����500k-3.7mƵ�Ρ�
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ���óɹ���#HI_ERR_FAILURE ����ʧ�ܣ����෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_SetWorkFreqCsg(HI_U8 Freq);


/**
* @ingroup  hct_mac
* @brief  CCO����ȫ������Ƶ��
*
* @par  ����:
* CCO����ȫ������Ƶ��
* @attention ���ò�Ʒ����ΪCCO��
* @param  FreqMode [IN] ���� #HI_U8��Ƶ�β��� 0��ʾ����Ϊ����ȫƵ��1��ʾ����Ϊ����2.5M~5.7MƵ�Σ�2��ʾ����Ϊ����0.7M~3MƵ�Σ�3��ʾ����Ϊ����1.7M~3MƵ��
* @param  ChangeTime [IN] ���� #HI_U32��ȫ���л�Ƶ����ʱʱ�䣬��λs��С��10sʱ�������0x22aNV��whole_network_change_freq_time����Ĭ��ֵ��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ���óɹ���#HI_ERR_FAILURE ����ʧ�ܣ����෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetWholeNetWorkFreq(HI_IN HI_U8 FreqMode, HI_IN HI_U32 ChangeTime);

/**
* @ingroup  hct_mac
* @brief  CCO��ѯȫ������Ƶ��
*
* @par  ����:
*  CCO��ѯȫ������Ƶ��
* @attention ���ò�Ʒ����ΪCCO��
* @param  �ޡ�
* @retval   ret ���� #HI_U8��Ƶ�β��� 0��ʾ����Ϊ����ȫƵ��1��ʾ����Ϊ����2.5M~5.7MƵ�Σ�2��ʾ����Ϊ����0.7M~3MƵ�Σ�3��ʾ����Ϊ����1.7M~3MƵ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetWholeNetWorkFreq(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  CCO��ѯȫ��NID
*
* @par  ����:
*  CCO��ѯȫ��NID
* @attention ���ò�Ʒ����ΪCCO��
* @param  �ޡ�
* @retval   ret ���� #HI_U32������Э����NID��ȡֵ��Χ16~16777215
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetWholeNetNid(HI_VOID);

/**
* @ingroup hct_mac
* @brief  ����ģʽ�趨
*/
typedef enum
{
    HI_SEARCH_NET_ACCESS_MODE_CCO = 0,             /**<��ʾCCO��������ģʽ����������������CCO*/
    HI_SEARCH_NET_ACCESS_MODE_STA,                   /**<��ʾSTA��������ģʽ��������������STA*/
}HI_SEARCH_NET_ACCESS_MODE_E;

/**
* @ingroup hct_mac
* @brief  ��������Ƶ�������С
*/

#define RCV_SEARCH_FREQ_NUM         (6)
/**
 * @ingroup hct_mac
* @brief  ������ѯ��Ŀ�ṹ��
*/
typedef struct
{
    HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN];          /**<��ѯվ���mac ��ַ */
    HI_U8 ucFreq;                               /**<��ѯվ���Ƶ��  */
    HI_U8 pad;                                  /**<����λ  */

    HI_U32 ucSnid;                              /**<��ѯվ�������  */
}MAC_NDC_QUERY_INFO_S;

/**
 * @ingroup hct_mac
* @brief  ��������ṹ��
*/

typedef struct
{

    HI_U32   ulSnid;                                        /**<����ID*/

    HI_U32   ulDiscoveryCnt;                               /**<�Ѳ鵽��ͨ��ģ��Ĵ���*/

    HI_U8    ucIp[HI_IPV4_ADDR_LEN];                       /**<վ��IP��ַ*/

    HI_U8    ucMac[HI_PLC_MAC_ADDR_LEN];                   /**<վ��MAC��ַ*/
    HI_U8    ucDevId[HI_MDM_DEV_ID_LEN];                   /**<����ڵ���豸��ʶ*/

    HI_U8    ucChannelInfo;                                /**<�ŵ�������ֻ��¼�ű�֡���ŵ�����*/
    HI_U8    ucConnect;                                    /**<��ʾ�Ƿ�������*/
    HI_U8    ucProdType;                                   /**<��ʶ��Ʒ����: \n
                                                               CCO: #HI_FTM_PRODUCT_TYPE_C01       \n
                                                               STA: #HI_FTM_PRODUCT_TYPE_M101  */
    HI_U8    ucCurrentFreq;                                  /**<��ʾ��ǰ����������Ƶ��*/

}MAC_NDC_MDL_RESULT_INFO_S;


/**
 * @ingroup hct_mac
* @brief  ����״̬���ת��
*/

typedef enum
{
    SEARCH_NET_STATUS_IDLE,             /**<δ��������ǰ��״̬*/
    SEARCH_NET_STATUS_SEARCH,           /**<����״̬*/
    SEARCH_NET_STATUS_READY,            /**<���Ӻ��״̬*/
} SEARCH_NET_STATUS_E;


/**
* @ingroup  hct_mac
* @brief  ��������
*
* @par  ����:
* ��������
* @attention ��
* @param  ucSetFreq [IN] ���� #HI_U8����Ҫ�����Ĺ���ģʽ���飬����ֵ���255
* @param  access_mode [IN] ���� #HI_SEARCH_NET_ACCESS_MODE_E������ģʽ
* @retval   ret [OUT] ���� #HI_U32��#HI_ERR_SUCCESS �ɹ������෵��ֵ�μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see HI_SEARCH_NET_ACCESS_MODE_E ����
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_StartNetSearch(HI_U8 ucSetFreq[RCV_SEARCH_FREQ_NUM],HI_SEARCH_NET_ACCESS_MODE_E access_mode);

/**
* @ingroup  hct_mac
* @brief  ֹͣ����
*
* @par  ����:
* ֹͣ����,ֹͣ���й���ģʽ�������贫����Ч�Ĺ���ģʽ
* @attention ��
* @param  ucSetFreq [IN] ���� #HI_U8������ģʽ��
* @retval   ret [OUT] ���� #HI_U32��#HI_ERR_SUCCESS �ɹ������෵��ֵ�μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_StopNetSearch(HI_U8 ucSetFreq);

/**
* @ingroup  hct_mac
* @brief  ��ȡ�������
*
* @par  ����:
* ��ȡ�������
* @attention ��
* @param  pst_query_info [IN] ���� #MAC_NDC_QUERY_INFO_S *��������ѯ��Ŀ��Ϣ��
* @param  pst_query_info [OUT] ���� #MAC_NDC_MDL_RESULT_INFO_S *��������ѯ��Ŀ��Ϣ��
* @retval  ret [OUT] ���� #HI_U32��#HI_ERR_SUCCESS �ɹ������෵��ֵ�μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ο�MAC_NDC_QUERY_INFO_S��MAC_NDC_MDL_RESULT_INFO_S�ṹ�嶨�塣
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetNetSearchResult(HI_IN MAC_NDC_QUERY_INFO_S *pst_query_info,
                                            HI_OUT MAC_NDC_MDL_RESULT_INFO_S *pst_query_result);

/**
* @ingroup  hct_mac
* @brief  ����״̬�ϱ��ص�����
* @par  ����:
* ��API�ӿ�ʹ�ó���������״̬�����仯ʱ�ϱ�
* @attention �ޡ�
* @param  search_net_state [IN] ���� #HI_U8������״̬
* @retval ret [OUT] ���� #HI_U32��#HI_ERR_SUCCESS �ɹ������෵��ֵ�μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_RegisterNetSearchStateCallBack)(HI_U8 search_net_state);
/**
* @ingroup  hct_mac
* @brief  ����״̬�ϱ�ע��ӿ�
* @par  ����:
* ����״̬�ϱ�ע��ӿ�
* @attention �ޡ�
* @param    pfnCallback [IN] ���� #HI_MDM_RegisterNetSearchStateCallBack�� ����״̬�ϱ��Ļص�������
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾע��ɹ���
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see #HI_MDM_RegisterNetSearchStateCallBack �ص�������
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterNetSearchStateInterface(HI_MDM_RegisterNetSearchStateCallBack);

/**
* @ingroup  hct_mac
* @brief  ��ȡ��������ģʽ
*
* @par  ����:
*  ��ȡ��������ģʽ
* @attention �ޡ�
* @param  �ޡ�
* @retval   ret ���� #HI_U8����������ģʽ������ο�#HI_MDM_PHYSICAL_TEST_MODE_EN��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetPhysicalTestMode(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ������������ģʽ
*
* @par  ����:
* ������������ģʽ
* @attention �ޡ�
* @param   mode [IN] ���� #HI_MDM_PHYSICAL_TEST_MODE_EN�� ��������ģʽ��
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_EnterPhyTestMode(HI_MDM_PHYSICAL_TEST_MODE_EN mode);
/**
* @ingroup  hct_mac
* @brief  �˳���������ģʽ
*
* @par  ����:
* �˳���������ģʽ
* @attention �ޡ�
* @param   �ޡ�
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_ExitPlcTest(HI_VOID);
/**
* @ingroup  hct_mac
* @brief  ��ȡ֡Payload����
*
* @par  ����:
*  ��ȡ֡Payload����
* @attention �ޡ�
* @param   ucType [IN] ���� #HI_U8�� ֡���Ͷ��壬�ű�֡: #BEACON_DT��SOF֡: #SOF_DT��
* @retval   ret ���� #HI_U16��֡Payload���ȡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U16 HI_MDM_Get_PayloadLen(HI_U8 ucType);
/**
* @ingroup  hct_mac
* @brief  ����ToneMaskģʽ
*
* @par  ����:
* ����ToneMaskģʽ
* @attention �ޡ�
* @param  mode [IN] ���� #HI_U8�� ToneMaskģʽ������ȫƵģʽ: #TONE_MASK_MODE_1������2.5M~5.7Mģʽ: #TONE_MASK_MODE_2������0.7M~3.0Mģʽ: #TONE_MASK_MODE_3������1.7M~3.0Mģʽ: #TONE_MASK_MODE_4��
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetToneMaskMode(HI_U8 mode);

/**
* @ingroup  hct_mac
* @brief  �����Ƿ�00��β��mac��ַת��ΪFE
*
* @par  ����:
* STA������MAC��ַʱ�Ƿ�00��β��mac��ַת��ΪFE��CCO��MAC��֤ʱ�Ƿ���������վ��00��β��mac��ַת��ΪFE
* @attention ���ò�Ʒ����ΪCCO��STA��ֻ������APP��ʼ��ʱ���á�
* @param  IsConvertMacAddr [IN] ���� #HI_BOOL�� �Ƿ�00��β��mac��ַת��ΪFE��#HI_TRUEΪ��00��β��mac��ַת��ΪFE�� #HI_FALSE Ϊ��ת��
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetIsConvertMacAddr(HI_BOOL IsConvertMacAddr);

/**
* @ingroup  hct_mac
* @brief  �����Ƿ����ð�����Ϊ�ձ�־λ
*
* @par  ����:
* �����Ƿ����ð�����Ϊ�ձ�־λ
* @attention ���ò�Ʒ����ΪCCO��
* @param  WhiteListAssocSwitch [IN] ���� #HI_BOOL�� �Ƿ����ð�����Ϊ�ձ�־λ��#HI_TRUEΪ������ #HI_FALSE Ϊ�ر�
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetWhiteListAssocSwitch(HI_BOOL WhiteListAssocSwitch);
/**
* @ingroup  hct_mac
* @brief  ��ѹ���䴥���жϻص�����
* @par  ����:
* ��API�ӿ�ʹ�ó�������ѹ����ʱ�����ж�
* @attention ���ò�Ʒ����ΪSTA��
* @param   #HI_U32����ѹ����
* @retval ret [OUT] ���� #HI_S32��#HI_ERR_SUCCESS �ɹ������෵��ֵ�μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
typedef HI_S32 (*HI_MDM_VoltageDecFunc)(HI_U32);
/**
* @ingroup  hct_mac
* @brief  ��ѹ���䴥���ж�ע��ӿ�
* @par  ����:
* ��ѹ���䴥���ж�ע��ӿ�
* @attention  ���ò�Ʒ����ΪSTA��
* @param    pfnCallback [IN] ���� #HI_MDM_VoltageDecFunc�� ��ѹ���䴥���жϻص�������
* @retval   ret ���� #HI_U32���Ƿ�ע��ɹ���#HI_ERR_SUCCESS:ע��ɹ���#HI_ERR_NOT_SUPPORT:��Ʒ��֧�֣�#HI_ERR_INVALID_PARAMETER:ע���������
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see #HI_MDM_VoltageDecFunc �ص�������
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_VoltageDecFuncReg(HI_MDM_VoltageDecFunc GpioCallBackFunc);

/**
* @ingroup  hct_mac
* @brief  �����Ƿ������������ر�־λ
*
* @par  ����:
* �����Ƿ������������ر�־λ
* @attention ���ò�Ʒ����ΪCCO��
* @param  FullLoadAssocSwitch [IN] ���� #HI_BOOL���Ƿ������������ر�־λ��#HI_TRUEΪ������ #HI_FALSE Ϊ�ر�
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetFullLoadAssocSwitch(HI_BOOL FullLoadAssocSwitch);

/**
* @ingroup  hct_mac
* @brief  ����Ӧ�ò���֤����
*
* @par  ����:
* ����Ӧ�ò���֤����
* @attention ���ò�Ʒ����ΪCCO��
* @param  IsAppAuth [IN] ���� #HI_BOOL���Ƿ�����Ӧ�ò���֤���ܣ�#HI_TRUEΪ������ #HI_FALSE Ϊ�ر�
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetAppAuth(HI_BOOL IsAppAuth);

/**
* @ingroup  hct_mac
* @brief  �Ƿ��������
*
* @par  ����:
* �Ƿ��������
* @attention ���ò�Ʒ����ΪCCO��
* @param  IsAllowAssoc [IN] ���� #HI_BOOL���Ƿ����������#HI_TRUEΪ���� #HI_FALSE Ϊ������
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetAllowAssoc(HI_BOOL IsAllowAssoc);

/**
* @ingroup  hct_mac
* @brief  ��ȡ�Ƿ������������
*
* @par  ����:
* ��ȡ�Ƿ������������
* @attention ���ò�Ʒ����ΪCCO��
* @param  ��
* @retval   ret ���� #HI_BOOL���Ƿ�����������磬#HI_TRUEΪ���� #HI_FALSE Ϊ������
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_GetAllowAssoc(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ���ð���������
*
* @par  ����:
* ���ð���������
* @attention ���ò�Ʒ����ΪCCO��
* @param  WhiteListNum [IN] ���� #HI_U32��������������ȡֵ��Χ0x0 ~ 0xFFFFFFFF��
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetAppWhiteListNum(HI_U32 WhiteListNum);

/**
* @ingroup  hct_mac
* @brief  ��ȡCCO�˸�������Э��֡���������ھ��������
*
* @par  ����:
* ��ȡCCO�˸�������Э��֡���������ھ��������
* @attention ���ò�Ʒ����ΪCCO��
* @param  ��
* @retval   ret ���� #HI_U8��CCO�˸�������Э��֡���������ھ������������Χ0 ~ 15��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetNeighborNetworkNum(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ��ȡCCO����STA�ϱ������ͻЭ��ʱ���ռ������ھ��������
*
* @par  ����:
*  ��ȡCCO����STA�ϱ������ͻЭ��ʱ���ռ������ھ��������
* @attention ���ò�Ʒ����ΪCCO������CCO����STA�ϱ���ͻЭ��ʱ�������һ���ھ����������
* @param  ��
* @retval   ret ���� #HI_U8��CCO����STA�ϱ������ͻЭ��ʱ���ռ������ھ������������Χ0 ~ 15��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetNeighborNetworkNumInConflict(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  վ�����FC�ռ������ھ��������,CCO,STA��������������
*
* @par  ����:
*  վ�����FC�ռ������ھ��������,CCO,STA��������������
* @attention ���ò�Ʒ����ΪCCO��STA��
* @param  ��
* @retval   ret ���� #HI_U8��վ�����FC�ռ������ھ������������Χ0 ~ 15��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetNeighborNetworkNumByFc(HI_VOID);

/**
* @ingroup  hct_mac
* @brief    STA��ע�����Ƿ�����Ը�CCO_MAC���й�������ĺ���
*
* @par  ����:
*  STA��ע�����Ƿ�����Ը�CCO_MAC���й�������ĺ���
* @attention ���ò�Ʒ����ΪSTA��
* @param  cco_mac [IN] ���� #HI_U8 cco_mac[HI_PLC_MAC_ADDR_LEN]��CCO��MAC��ַ���r
* @retval   ret ���� #HI_BOOL���Ƿ�����Ը�CCO_MAC���й�������#HI_TRUEΪ���� #HI_FALSE Ϊ������
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
typedef HI_BOOL (*HI_MDM_HANDLE_LOCK_ASSOC_CCO_MAC_FUN)(HI_U8 cco_mac[HI_PLC_MAC_ADDR_LEN]);

/**
* @ingroup  hct_mac
* @brief  STA��ע�����Ƿ������CCO_MAC���й���
*
* @par  ����:
*  STA��ע�����Ƿ������CCO_MAC���й���
* @attention ���ò�Ʒ����ΪSTA��
* @param   func [IN] ���� #HI_MDM_HANDLE_LOCK_ASSOC_CCO_MAC_FUN��STA��ע�����Ƿ�����Ը�CCO_MAC���й�������ĺ������r
* @retval   ret ���� #HI_U32���Ƿ�ע��ɹ���#HI_ERR_SUCCESS:ע��ɹ���#HI_ERR_NOT_SUPPORT:��Ʒ��֧�֣�#HI_ERR_INVALID_PARAMETER:ע���������
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterLockAssocCcoMacFunc(HI_MDM_HANDLE_LOCK_ASSOC_CCO_MAC_FUN func);

/**
* @ingroup  hct_mac
* @brief  �Ƿ�����STA���ж�CCO_MAC�Ƿ���й������ƹ���
*
* @par  ����:
*  �Ƿ�����STA���ж�CCO_MAC�Ƿ���й������ƹ���
* @attention ���ò�Ʒ����ΪSTA��
* @param   SetSwitch [IN] ���� #HI_BOOL���Ƿ�����STA���ж�CCO_MAC�Ƿ���й������ƹ��ܣ�#HI_TRUE��ʾ���ã�#HI_FALSE��ʾ������
* @retval   ret ���� #HI_U32���Ƿ����óɹ���#HI_ERR_SUCCESS��ʾ���óɹ���������ʾ����ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetLockAssocCcoMacSwitch(HI_BOOL SetSwitch);

/**
* @ingroup  hct_mac
* @brief  ֪ͨ��ʼ���ٹر�֪ͨ������
*
* @par  ����:
* ֪ͨ��ʼ���ٹر�֪ͨ������
* @attention ֻ��CCO�����С�
* @param  usMaxLoop [IN] ���� #HI_U16�����ѭ��������
* @param  ucNotifyData [OUT] ���� #HI_U8��֪ͨʱЯ���������ݣ���Χ0~127��
* @param  pfnCallback [OUT] ���� #HI_MDM_pfnCcoFastNoticeCallback��֪ͨ����ص��ϱ�������
* @param  pParam [IN] ���� #HI_PVOID���û��Զ��������ע��ʱ���룬�ص�ʱ�ش�������Ҫʱ����Ϊ�ա�
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ��ʼ�ر�֪ͨ�ɹ���#HI_ERR_INVALID_PARAMETER �����Ч��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_StartWholeNetFastNotice(HI_U16 usMaxLoop, HI_U8 ucNotifyData, HI_MDM_pfnCcoFastNoticeCallback pfnCallback, HI_PVOID pParam);


/**
* @ingroup  hct_mac
* @brief  ע�ᴦ����ٹر�֪ͨ���ݻص�����
*
* @par  ����:
* ע�ᴦ����ٹر�֪ͨ���ݻص�����
* @attention ֻ��STA������
* @param  pfnCallback [IN] ���� #HI_MDM_pfnStaFastNoticeCallback��ע��Ļص�������
* @param  pParam [IN] ���� #HI_PVOID���û��Զ��������ע��ʱ���룬�ص�ʱ�ش�������Ҫʱ����Ϊ�ա�
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ע��ɹ���#HI_ERR_INVALID_PARAMETER ָ�����Ϊ�ա�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterStaFastNoticeCallback(HI_MDM_pfnStaFastNoticeCallback pfnCallback, HI_PVOID pParam);


/**
* @ingroup  hct_mac
* @brief  ���ڿ�����ٷ������ݵĴ�����
*
* @par  ����:
* ���ڿ�����ٷ������ݵĴ�����,һ�����ڷ��ͳ��ȳ���11�ֽڵ����ݡ�
* @attention
* ���Ͳ�����ʹ��˵������:
* 1���ڷ��͹㲥����ʱָ����λ������Ч�ģ����͹㲥����ʱBroadcastFlag��־��Ϊ1��
* 2����������������Ѿ����MPDU��ʽ���͵ı��ģ�IsMacEncode��־Ҫ�ó�TRUE,���ҷ���ʱҪָ����Ƶ�Ρ�TMI��PB������PB�鳤����Щ��Ϣ�������ó�FLASE������д��Щ�ֶΡ�
* 3������������յ����ĺ���ռ�����ͣ�SendMode���ó�1���������ó�0����ռ������Ҫ�û��Լ�ȷ����·���У��������Ϊ��ͻ���·���ʧ�ܡ�
* 4��������������ָ��Ҳ���Բ�ָ������ָ��ʱʹ��Ĭ�ϵ�ֵ
* 5���Զ˽��ձ��ĺ���Ҫͨ��HI_MDM_ChlSetupע���MDMͨ����PLCҵ��˿����ͣ����ʹ�ô˽ӿڿ��ٷ���ǰ��Ҫ��ע���ϱ�ͨ����������
* @param  MacBbPlcSendPara [IN] ���� #HI_MDM_BBPLC_SEND_PARA_S *�����ٷ��ʹ�������ָ�롣
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ������ͳɹ���#HI_ERR_INVALID_PARAMETER �����Ч��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_FastSendOnBbPlc(HI_MDM_BBPLC_SEND_PARA_S *MacBbPlcSendPara);

/**
* @ingroup  hct_mac
* @brief  ���ڵ��ز����ٷ��ʹ�����
*
* @par  ����:
* ���ڵ��ز����ٷ��ʹ�����
* @attention
* 1������������յ����ĺ���ռ�����ͣ�SendMode���ó�1���������ó�0����ռ������Ҫ�û��Լ�ȷ����·���У��������Ϊ��ͻ���·���ʧ�ܡ�
* 2�����ز�һ������ͱ��ĳ��������Ʋ��ܳ���64�ֽڡ�
* 3�����ú���ǰȷ�����ز�����������ȷ�����õ��ز������ο�HI_MDM_ConfigScPara��
* @param  MacBbPlcSendPara [IN] ���� #HI_MDM_BBPLC_SEND_PARA_S *�����ٷ��ʹ�������ָ�롣
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ���ز����ٷ��ͳɹ���#HI_ERR_INVALID_PARAMETER �����Ч��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_FastSendOnScPlc(HI_MAC_SC_SEND_PARA_S *MacScPlcSendPara);


/**
* @ingroup  hct_mac
* @brief  ���ڿ�����ٷ��Ͳ���PB�����չsack���Ĵ�����
*
* @par  ����:
* ���ڿ�����ٷ��Ͳ���PB�鴦������
* @attention
* 1���ڷ��͹㲥����ʱָ����λ������Ч�ģ����͹㲥����ʱBroadcastFlag��־��Ϊ1��
* 2����������������Ѿ����MPDU��ʽ���͵ı��ģ�IsMacEncode��־Ҫ�ó�TRUE,ֻ���ͳ���Ϊ16�ֽڵı��ģ������ó�FLASE��Ҫָ���������ݳ��ȣ������ܳ���11�ֽڡ�
* 3������������յ����ĺ���ռ�����ͣ�SendMode���ó�1���������ó�0����ռ������Ҫ�û��Լ�ȷ����·���У��������Ϊ��ͻ���·���ʧ�ܡ�
* 4���˺����������Ҫָ��DataLength��Timeout��SendPhase��IsMacEncode��������������ע��
* @param  MacBbPlcSendPara [IN] ���� #HI_MDM_BBPLC_SEND_PARA_S *�����ٷ��ʹ�������ָ�롣
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ���ͳɹ���#HI_ERR_INVALID_PARAMETER �����Ч��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_FastSendSackOnBbPlc(HI_MDM_BBPLC_SEND_PARA_S *MacBbPlcSendPara);

/**
* @ingroup  hct_mac
* @brief  ע�ᵥ�ز����յ����ٷ������ݺ��ϱ�APP�Ļص�����
*
* @par  ����:
* ע����յ����ٷ������ݺ��ϱ�APP�Ļص�����
* @attention �ޡ�
* @param  pfnCallback [IN] ���� #HI_MDM_pfnFastSendCallback��ע��Ļص�������
* @param  pData [IN] ���� #HI_U8 *�����ڴ���ϱ����ݵĵ�ַָ�롣
* @param  ucMaxDataLength [IN] ���� #HI_U8���ɴ�����ݵ���󳤶ȡ�
* @param  ucRportMode [IN] ���� #HI_U8,�����ϱ�����ģʽ��0��ʾ��ȥmac��װֻ�ϱ�Ӧ�ò����ݣ�1��ʾ��mac���װ�ϱ�
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ע��ɹ���#HI_ERR_INVALID_PARAMETER �����Ч��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterFastCallbackSc(HI_MDM_pfnFastSendCallback pfnCallback, HI_U8 *pData, HI_U8 ucMaxDataLength, HI_U8 ucRportMode);

/**
* @ingroup  hct_mac
* @brief  ע����յ����ٷ�����չsack���ݺ��ϱ�APP�Ļص�����
*
* @par  ����:
* ע����յ����ٷ������ݺ��ϱ�APP�Ļص�����
* @attention
* 1���ϱ����ݳ��ȸ���ucRportMode�в������ϱ����ݴ�MAC���װ��������Ϊ16������ϱ����ݲ���MAC���װ�����ݳ��ȹ̶�Ϊ11�������Ҫ�������ж����ݵ���Ч�ԡ�
* 2���ϱ�������IsMacDecode��DataAddr����Ч�ģ������������ù�ע��
* 3��ucMaxDataLength����Ҫ�ܱ�֤������ϱ������������ϱ���ʧ�ܡ�
* @param  pfnCallback [IN] ���� #HI_MDM_pfnFastSendCallback��ע��Ļص�������
* @param  pData [IN] ���� #HI_U8 *�����ڴ���ϱ����ݵĵ�ַָ�롣
* @param  ucMaxDataLength [IN] ���� #HI_U8���ɴ�����ݵ���󳤶ȡ�
* @param  ucRportMode [IN] ���� #HI_U8,�����ϱ�����ģʽ��0��ʾ��ȥmac��װֻ�ϱ�Ӧ�ò����ݣ�1��ʾ��mac���װ�ϱ�
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ע��ɹ���#HI_ERR_INVALID_PARAMETER �����Ч��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterFastCallbackSack(HI_MDM_pfnFastSendCallback pfnCallback, HI_U8 *pData, HI_U8 ucMaxDataLength, HI_U8 ucRportMode);

/**
* @ingroup  hct_mac
* @brief  ����MAC��ֹͣ���ݷ���ָ��ʱ�䡣
*
* @par  ����:
* ����MAC��ֹͣ���ݷ���ָ��ʱ�䡣
* @attention
* 1��Ҫ��ָ��ֹͣ��ʱ�䲻�ܳ���1���ӣ�������ܵ���վ�����ߡ�
* 2������1���ӣ������1���ӽ������á�
* @param  DisableTime [IN] ���� #HI_U32��ָ��MAC��ֹͣ���͵�ʱ�䵥λms��ȡֵ��Χ0-60000ms��
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ����ֹͣ���ͳɹ���#HI_ERR_INVALID_PARAMETER �����Ч��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_DisableSending(HI_IN HI_U32 DisableTime);

/**
* @ingroup  hct_mac
* @brief  �ָ�MAC�����ݷ��͹��ܡ�
*
* @par  ����:
* �ָ�MAC�����ݷ��͹��ܡ�
* @attention �ޡ�
* @param  ��
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_EnableSending(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ���õ��ز�ʹ�ܺͷ������ʡ�
*
* @par  ����:
* ���õ��ز���������,�������ò�����ҪͬоƬ������Աȷ�ϡ�
* @attention �ޡ�
* @param  ScPara [IN] ���� #HI_MAC_SC_PARA_S *�����õ��ز����ʲνṹָ�롣
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ���õ��ز������ɹ���#HI_ERR_INVALID_PARAMETER �����Ч��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_ConfigScPara(HI_IN HI_MAC_SC_PARA_S *ScPara);

/**
* @ingroup  hct_mac
* @brief  ��ȡ��վ������������Ϣ
*
* @par  ����:
* ��ȡ��վ������������Ϣ
* @attention
* @param  AttributeInfo [IN] ���� #HI_MDM_NETWORK_ATTRIBUTE_S *����Ż�ȡ����������Ϣ��ָ�롣
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ��ȡ�������Գɹ���#HI_ERR_INVALID_PARAMETER �����Ч��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetNetworkAttribute(HI_MDM_NETWORK_ATTRIBUTE_S *AttributeInfo);


/**
* @ingroup  hct_mac
* @brief  ��ѯTOPO����վ�����Ĵ���վ���tei��mac
*
* @par  ����:
* ��ѯTOPO����վ�����Ĵ���վ��tei��mac,�˺���ֻ������CCO.
* @attention
* @param  Tei [OUT] ���� #HI_U16 *����ѯ���Ĵ���TEI��
* @param  Mac [OUT] ���� #HI_U8����ѯ���Ĵ���MAC��
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ��ѯ��վ��������ɹ���#HI_ERR_INVALID_PARAMETER �����Ч��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_QueryMostChildProxy(HI_U16 *Tei, HI_U8 Mac[6]);

/**
* @ingroup  hct_mac
* @brief  �����Ƿ�ָ�V200Э���ʶ������Դ�汾ʹ�á�
*
* @par  ����:
* �����Ƿ�ָ�V200Э���ʶ
* @attention ���ò�Ʒ����ΪCCO��
* @param  IsRecoverPtcl [IN] ���� #HI_BOOL�� �Ƿ�ָ�V200Э���ʶ��#HI_TRUE��ʾ�ָ���#HI_FALSE��ʾ���ָ���
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetIsRecoverPtcl(HI_BOOL IsRecoverPtcl);

/**
* @ingroup  hct_mac
* @brief  ��ȡ�Ƿ�ָ�V200Э���ʶ������Դ�汾ʹ�á�
*
* @par  ����:
*  ��ȡ�Ƿ�ָ�V200Э���ʶ
* @attention ���ò�Ʒ����ΪCCO��
* @param  ��
* @retval   ret ���� #HI_BOOL���Ƿ�ָ�V200Э���ʶ��#HI_TRUE��ʾ�ָ���#HI_FALSE��ʾ���ָ���
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_GetIsRecoverPtcl(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ����PHY�͹��Ŀ���
*
* @par  ����:
* ����PHY�͹��Ŀ���
* @attention ���ò�Ʒ����ΪSTA��վ�����δ����״̬�µ͹�����Ч����֧�ֶ�̬�򿪽�֧�ֶ�̬�رա�
* @param  PhyLowPowerSwtich [IN] ���� #HI_BOOL�� PHY�͹��Ŀ��أ�#HI_FALSE��ʾ�رա�
* @retval   ret ���� #HI_U32������Ϣ����PHY�͹��Ŀ��صķ���ֵ��#HI_ERR_SUCCESS��ʾ���óɹ������෵��ֵ�μ�hi_errno.h����
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetPhyLowPowerSwtich(HI_BOOL PhyLowPowerSwtich);

/**
* @ingroup  hct_mac
* @brief  ��ȡPHY�͹��Ŀ���
*
* @par  ����:
*  ��ȡPHY�͹��Ŀ���
* @attention ���ò�Ʒ����ΪSTA��
* @param  ��
* @retval   ret ���� #HI_BOOL��PHY�͹��Ŀ��أ�#HI_TRUE��ʾ�򿪣�#HI_FALSE��ʾ�رա�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_GetPhyLowPowerSwitch(HI_VOID);

/**
 * @ingroup hct_mac
* @brief  ��ץ��ģʽ
*/
#define HI_MAC_NO_CAPTURE_MODE              (0)
/**
 * @ingroup hct_mac
* @brief  ��PB��ץ��ģʽ
*/
#define HI_MAC_CAPTURE_SINGLE_PB_MODE       (1)
/**
 * @ingroup hct_mac
* @brief  ��PB��ץ��ģʽ
*/
#define HI_MAC_CAPTURE_MULTI_PB_MODE        (2)
/**
 * @ingroup hct_mac
* @brief  ��PB��ذ�ģʽ
*/
#define HI_MAC_INTERCEPT_MULTI_PB_MODE      (3)
/**
 * @ingroup hct_mac
* @brief  ��PB��ػ�ģʽ
*/
#define HI_MAC_INTERCEPT_LOOP_MODE          (4)
/**
 * @ingroup hct_mac
* @brief  ��ת���ز���ģʽ������Э�����ģʽʹ�á�
*/
#define HI_MAC_INTERCEPT_REVERSE_LOOP_MODE  (5)

/**
* @ingroup  hct_mac
* @brief  ����ץ��ģʽ
*
* @par  ����:
* ����ץ��ģʽ
* @attention �ޡ�
* @param  RxCaptureMode [IN] ���� #HI_U8��ȡֵ:\n @li #HI_MAC_NO_CAPTURE_MODE ��ʾ��ץ��ģʽ\n
                                                  @li #HI_MAC_CAPTURE_SINGLE_PB_MODE ��ʾ��PB��ץ��ģʽ\n
                                                  @li #HI_MAC_CAPTURE_MULTI_PB_MODE ��ʾ��PB��ץ��ģʽ\n
                                                  @li #HI_MAC_INTERCEPT_MULTI_PB_MODE ��ʾ��PB��ץ��������ץ�������ñ���ģʽ\n
                                                  @li #HI_MAC_INTERCEPT_LOOP_MODE ��ʾ��PB�黷�ز���ģʽ\n
                                                  @li #HI_MAC_INTERCEPT_REVERSE_LOOP_MODE  ��ʾ��ת���ز���ģʽ��
* @retval   ret ���� #HI_U32��\n
                                      @li  #HI_ERR_SUCCESS ���óɹ�\n
                                      @li  #HI_ERR_FAILURE ����ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetCaptureMode(HI_U8 RxCaptureMode);

/**
* @ingroup  hct_mac
* @brief  ��ȡץ��ģʽ
*
* @par  ����:
* ��ȡץ��ģʽ
* @attention �ޡ�
* @param  �ޡ�
* @retval   RxCaptureMode [IN] ���� #HI_U8��ȡֵ:\n @li #HI_MAC_NO_CAPTURE_MODE ��ʾ��ץ��ģʽ\n
                                                  @li #HI_MAC_CAPTURE_SINGLE_PB_MODE ��ʾ��PB��ץ��ģʽ\n
                                                  @li #HI_MAC_CAPTURE_MULTI_PB_MODE ��ʾ��PB��ץ��ģʽ\n
                                                  @li #HI_MAC_INTERCEPT_MULTI_PB_MODE ��ʾ��PB��ץ��������ץ�������ñ���ģʽ\n
                                                  @li #HI_MAC_INTERCEPT_LOOP_MODE ��ʾ��PB�黷�ز���ģʽ\n
                                                  @li #HI_MAC_INTERCEPT_REVERSE_LOOP_MODE  ��ʾ��ת���ز���ģʽ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see hi_mdm.h�ļ���#HI_MDM_PTCL_VER_100�ȶ��塣
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetCaptureMode(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ��������MSDU���Ŀ���
*
* @par  ����:
* ��������MSDU���Ŀ���
* @attention �ޡ�
* @param  InterceptMsduSwitch [IN] ���� #HI_BOOL��
* @retval   ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetInterceptMsdu(HI_BOOL InterceptMsduSwitch);

/**
 *@ingroup hct_mac
 * ץȡMSDU������Ϣ
 */
typedef struct
{
    HI_U16 protocol_ver;        /**< MSDUЭ������                 */
    HI_U16 msdu_size;           /**< MSDU����                 */

    HI_U8  *msdu_addr;          /**< MSDU���ݻ����ַ                 */
}HI_MDM_CAPTURE_MSDU_INFO_ST;

/**
* @ingroup  hct_mac
* @brief  ץȡMSDU����ע�ắ��
*
* @par  ����:
* ץȡMSDU����ע�ắ��
* @attention �ޡ�
* @param  CaptureMsduInfo [IN] ���� #HI_MDM_CAPTURE_MSDU_INFO_ST��MSDU��Ϣ��Ϣ
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see EQUIP_ZERO_NTB_NUM ���塣
* @since HiMDL_V200R001C00
*/
typedef HI_VOID (*MAC_CAPTURE_MSDU_FUN)(HI_MDM_CAPTURE_MSDU_INFO_ST *CaptureMsduInfo);

/**
* @ingroup  hct_mac
* @brief  ע��ץȡMSDU���ݵĽӿ�
*
* @par  ����:
* ע��ץȡMSDU���ݵĽӿ�
* @attention �ޡ�
* @param  CaptureMsudInterface [IN] ���� #MAC_CAPTURE_MSDU_FUN��MSDUץ���ӿڡ�
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ע��ɹ���#HI_ERR_FAILURE ע��ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterCaptureMsduInterface(MAC_CAPTURE_MSDU_FUN CaptureMsudInterface);
/**
* @ingroup  hct_mac
* @brief  ȥע��ץȡMSDU���ݵĽӿ�
*
* @par  ����:
* ȥע��ץȡMSDU���ݵĽӿ�
* @attention �ޡ�
* @param  CaptureMsudInterface [IN] ���� #MAC_CAPTURE_MSDU_FUN��MSDUץ���ӿڡ�
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ȥע��ɹ���#HI_ERR_FAILURE ȥע��ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_UnregisterCaptureMsduInterface(MAC_CAPTURE_MSDU_FUN CaptureMsudInterface);

/**
* @ingroup  hct_mac
* @brief  ��ȡվ���Ƿ��������
*
* @par  ����:
* ��ȡվ���Ƿ��������
* @attention ���ò�Ʒ����ΪSTA��
* @param  ��
* @retval   ret ���� #HI_BOOL���Ƿ�������磬#HI_TRUE��ʾ�������磬#HI_FALSE��ʾδ�������硣
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_IsStaJoinNet(HI_VOID);
/**
* @ingroup  hct_mac
* @brief  �ж�ģ���Ƿ�Ϊ��׼�����·
*
* @par  ����:
*  �ж�ģ���Ƿ�Ϊ��׼�����·
* @attention ���ò�Ʒ����ΪCCO,STA��
* @param  ��
* @retval   ret ���� #HI_BOOL���Ƿ�Ϊ��׼�����·��#HI_TRUE��ʾΪ��׼�����·��#HI_FALSE��ʾΪ�Ǿ�׼�����·��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_IsPreciseZeroCross(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ��ȡ����ID
*
* @par  ����:
* ��ȡ����ID
* @attention
* @param  my_management_id [OUT] ���� #HI_U8�� ����ID
* @retval  ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ��ȡ�ɹ���#������ʾ��ȡʧ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetMyManagementId(HI_U8 *my_management_id);
/**
* @ingroup  hct_mac
* @brief  ���ù���ID
*
* @par  ����:
* ���ù���ID
* @attention ��װʹ�ã����ù���ID
* @param  my_management_id [IN] ���� #HI_U8�� ����ID
* @retval  ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ���óɹ���#������ʾ����ʧ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetMyManagementId(HI_U8 *my_management_id);

/**
* @ingroup  hct_mac
* @brief  ���ع�����V100Э��
*
* @par  ����:
* ���ع�����V100Э��
* @attention ���ع�����V100Э��
* @param  ��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ���سɹ���#HI_ERR_FAILURE ����ʧ�ܣ����෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_LoadSgAndV100Protocol(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ����������V100Э��
*
* @par  ����:
* ���ع�����V100Э��
* @attention ������������V100Э��
* @param  ��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ���سɹ���#HI_ERR_FAILURE ����ʧ�ܣ����෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_LoadCsgAndV100Protocol(HI_VOID);


/**
* @ingroup  hct_mac
* @brief  ���ؿ��ٹ㲥����
*
* @par  ����:
* ���ؿ��ٹ㲥���ܣ��Կ��ٹ㲥�еĵ��ز�����չsack�����շ����ܽ��г�ʼ����
* @attention
* ʹ�ÿ��ٹ㲥���ܣ������ڳ�ʼ���е��ô˽ӿڣ�������ٹ㲥���ܲ����á�
* * @param  ��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ���سɹ���#HI_ERR_FAILURE ����ʧ�ܣ����෵��ֵ����μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_FastSendInit(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ��nv�е�snid��Ϊ0
*
* @par  ����:
* ��nv�е�snid��Ϊ0
* @attention CCO��װʹ�ã���nv�е�snid������nid����Ϊ0
* @param   ��
* @retval  ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ���óɹ���#������ʾ����ʧ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_ResetSnidNv(HI_VOID);
/**
* @ingroup  hct_mac
* @brief  ע��ͣ���ϱ�����(�ǵ���ģ��)��غ���
*
* @par  ����:
*       ע��ͣ���ϱ�����(�ǵ���ģ��)��غ���
* @attention ͣ���ϱ�����(�ǵ���ģ��)ע��ӿڣ���Ҫ������Ӧ��nv����\n
*            ��CCO 0x408 nv��ucReportEnable ��ֵ����Ϊ1\n
*              STA 0x418 nv��ucEnable ��ֵ����Ϊ1.
* @param   ��
* @retval  ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ���óɹ���#������ʾ����ʧ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_PowerOffRegister(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ����վ��״̬Ϊ����״̬
*
* @par  ����:
*       ����վ��״̬Ϊ����״̬
* @attention �������������վ��ʱ��̽��ɹ���������50%ʱ ����վ����뱣��״̬
* @param   mac_addr [IN] ���� #HI_U8�� վ��mac��ַ
* @retval  ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ���óɹ���#������ʾ����ʧ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_NM_PowerOffSetNodeProtectingState(HI_U8 *mac_addr);

/**
* @ingroup  hct_mac
* @brief  ģ������ϵ������
*
* @par  ����:
* ģ������ϵ������
* @attention CCO,STA,NDM�����ϵ�׶ν��й�����
* @param   ��
* @retval  ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ���øü�⹦�ܳɹ���#HI_ERR_BUSY��ʾ�Ѿ��������������ٴ��·���
*#������ʾ����ʧ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_ZeroCrossAutoCheck(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ������V100Э����������λʶ��������
*
* @par  ����:
* ������V100Э����������λʶ��������
* @attention CCO,STA��Ʒ������λʶ��������
* @param   ��
* @retval  ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ���óɹ�,#������ʾ����ʧ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_PhaseIdentifyRegSgAndV100Protocol(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ������V100Э����������λʶ��������
*
* @par  ����:
* ������V100Э����������λʶ��������
* @attention CCO,STA��Ʒ������λʶ��������
* @param   ��
* @retval  ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ���óɹ�,#������ʾ����ʧ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_PhaseIdentifyRegCsgAndV100Protocol(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ������V100Э��������NTBʶ��������
*
* @par  ����:
* ������V100Э��������NTBʶ��������
* @attention CCO,STA��Ʒ����NTBʶ��������
* @param   ��
* @retval  ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ���óɹ�,#������ʾ����ʧ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_TfpRegSgAndV100Protocol(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ������V100Э��������NTBʶ��������
*
* @par  ����:
* ������V100Э��������NTBʶ��������
* @attention CCO,STA��Ʒ����NTBʶ��������
* @param   ��
* @retval  ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ���óɹ�,#������ʾ����ʧ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_TfpRegCsgAndV100Protocol(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ��������̨��ʶ��SNR���
*
* @par  ����:
* ��������̨��ʶ��SNR���
* @attention CCO,STA��Ʒ����̨��ʶ��SNR�����������
* @param   ��
* @retval  ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ���سɹ�,#������ʾ����ʧ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_LoadSoftTfSnrComponent(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ��������̨��ʶ��NTB�ֲ�ʽ���
*
* @par  ����:
* ��������̨��ʶ��NTB�ֲ�ʽ���
* @attention CCO,STA��Ʒ����̨��ʶ��NTB�ֲ�ʽ�������
* @param   ��
* @retval  ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ���سɹ�,#������ʾ����ʧ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_LoadSoftTfNtbDistComponent(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ��������̨��ʶ�𱱾�NTBЭ��
*
* @par  ����:
* ��������̨��ʶ�𱱾�NTBЭ��
* @attention CCO,STA��Ʒ����̨��ʶ�𱱾�NTBЭ�����
* @param   ��
* @retval  ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ���سɹ�,#������ʾ����ʧ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_LoadSoftTfNtbProtocolBeijing(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  �����ͣ���ϱ���������
*
* @par  ����:
* �����ͣ���ϱ���������
* @attention STA��Ʒ��ͣ���ϱ���������
* @param   ��
* @retval  ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ���óɹ�,#������ʾ����ʧ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_PowerFailureCheckRegister(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ָʾ���շ����쳣״̬��������
*
* @par  ����:
* ָʾ���շ����쳣״̬��������
* @attentionָʾ���շ����쳣״̬��������
* @param   ��
* @retval ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_LedCtrlRegister(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ��ȡ�Ƿ�Ϊ��Դģʽ
*
* @par  ����:
* ��ȡ�Ƿ�Ϊ��Դģʽ
* @attention �ж��Ƿ�Ϊ��Դģʽ
* @param   ��
* @retval  ret ���� #HI_BOOL��#HI_TRUE ��ʾΪ��Դģʽ,#������ʾ����Դģʽ
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_GetIsEnergyMode(HI_VOID);

/**
* @ingroup  hct_mac
* @brief ��ԴӦ�ó����£�����CCO������ֹͣоƬ̽�⿪��
*
* @par  ����:
* ��ԴӦ�ó����£�����CCO������ֹͣоƬ̽�⿪��
* @attention �ޡ�
* @param  setflag[IN] ���� #HI_TRUE����ʾ����̽��,#HI_FALSE����ʾֹͣ̽�⡣
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ���óɹ���#HI_ERR_NOT_SUPPORT ����Դ�����²�֧�֣�#HI_ERR_BUSY�Ѿ��������������ٴ�����
* #��������ֵ��ʾ����ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetChipDetectSwitch(HI_BOOL setflag);

/**
 *@ingroup hct_mac
 * оƬ̽����
 */
typedef enum
{
    HI_MDM_CHIP_VER_UNKOWN = 0,         /**<��ʾû��̽�⵽վ��*/
    HI_MDM_HI3911V100_CHIP_VER = 1,     /**<bit0��ֵ����ʾ̽�⵽Hi3911V100оƬ*/
    HI_MDM_HI3911V200_CHIP_VER = 2,     /**<bit1��ֵ����ʾ̽�⵽Hi3911V200оƬ*/
    HI_MDM_HI3921V100_CHIP_VER = 4,     /**<bit2��ֵ����ʾ̽�⵽Hi3921V100оƬ*/
}HI_MDM_CHIP_VER_BIT_OFFSET_E;

/**
* @ingroup  hct_mac
* @brief ��ԴӦ�ó����£�����CCO�˻�ȡоƬ̽����
*
* @par  ����:
* ��ԴӦ�ó����£�����CCO�˻�ȡоƬ̽����
* @attention �ޡ�
* @param  �ޡ�
* @retval   ret ���� #HI_U32���μ�#HI_MDM_CHIP_VER_BIT_OFFSET_Eö��ֵ�ж��塣
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetChipDetectVer(HI_VOID);

/**
* @ingroup  hct_mac
* @brief ��ԴӦ�ó����£�CCO�����̽�⵽оƬ�汾��Ϣ
*
* @par  ����:
* ��ԴӦ�ó����£�CCO�����̽�⵽оƬ�汾��Ϣ
* @attention �ޡ�
* @param  �ޡ�
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS��ʾ����ɹ�,#������ʾ���ʧ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_ClearChipDetectVer(HI_VOID);

/**
* @ingroup  hct_mac
* @brief ��ԴӦ�ó����£����������Ƿ�����CCO�ű귢�Ϳ���
*
* @par  ����:
* ��ԴӦ�ó����£�����CCO��Ʒ��̬�������ű귢�Ϳ���
* @attention �ޡ�
* @param  setflag[IN] ���� #HI_TRUE����ʾ���������ű�,#HI_FALSE����ʾ�������űꡣ
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS��ʾ���óɹ�,#������ʾ����ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetSendBeaconSwitch(HI_BOOL setflag);

/**
* @ingroup  hct_mac
* @brief ��ԴӦ�ó����£���������STAվ�������ж�ʱ�䣬�������ý����ű꣬�Լ����մ�����Ϣ���������ж�
*
* @par  ����:
* ��ԴӦ�ó����£�����STAվ�������ж�վ������ʱ�䣬����check_bcn_time,check_proxy_time��Сֵ�ֱ�Ϊ60s,120s,
* ����NV���ò���ֵ�󣬰���NV��ֵ�������á�
* @attention �ޡ�
* @param  setflag[IN] ���� #HI_TRUE����ʾ������Ч,#HI_FALSE,��ʾ���ò���Ч��
*@param  check_bcn_time[IN] ����#HI_U32, ��ʾ���ݽ����ű���վ������ʱ�䣬��λms����СΪ60s��
*@param  check_proxy_time[IN]���� #HI_U32, ��ʾ���ݽ��մ�����Ϣ���վ������ʱ�䣬��λms����СΪ120s��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS��ʾ���óɹ�,#HI_ERR_INVALID_PARAMETER��ʾ��������,#HI_ERR_NOT_SUPPORT��ʾ��֧��,������ʾ����ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetCheckOfflineTime(HI_BOOL setflag,HI_U32 check_bcn_time,HI_U32 check_proxy_time);

/**
* @ingroup  hct_mac
* @brief ��ȡ�ռ�������CCO�ı��ĸ���
*
* @par  ����:
* ��STA�˲鿴�ռ�������CCO�ı��ĸ���
* @attention �ޡ�
* @param  �ޡ�
* @retval   ret ���� #HI_U32����ȡ��CCO�ı��ĸ�����
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetCCOPacketCnt(HI_VOID);

/**
* @ingroup  hct_mac
* @brief ������Դ�汾�Ƿ����������ز���û�б���������£�վ���������߹��ܡ�
*
* @par  ����:
* ��STA��������Դ�汾�Ƿ����������ز���û�б���������£�վ���������߹��ܡ�
* @attention �ޡ�
* @param  setflag[IN] ���� #HI_TRUE����ʾ������Ч,#HI_FALSE,��ʾ���ò���Ч��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS��ʾ���óɹ���#HI_ERR_NOT_SUPPORT��ʾ��Ʒ ��֧��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetOfflineByUnlockInEnergyMode(HI_BOOL setflag);

/**
* @ingroup  hct_mac
* @brief  �����ű�����ʹ���
*
* @par  ����:
* �����ű�����ʹ���
* @attention �ޡ�
* @param  count [IN] ���� #HI_U8���ű�����ʹ�����ȡֵ��Χ[1,3]��
* @retval   ret ���� #HI_VOID��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetBeaconMaxSendCnt(HI_U8 count);

/**
* @ingroup  hct_mac
* @brief  �����ű�ʱ϶����Ӧ���
*
* @par  ����:
* �����ű�ʱ϶����Ӧ���
* @attention ���ò�Ʒ����ΪCCO���ýӿڽ��ڹ���Ƶ������Ч��
* @param  count [IN] ���� #HI_BOOL��ȡֵHI_TRUE��ʾʹ���ű�ʱ϶����Ӧ��ȡֵHI_FALSE��ʾȥʹ���ű�ʱ϶����Ӧ��
* @retval   ret ���� #HI_VOID��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetAutoBeaconDurationFlag(HI_BOOL flag);

/**
* @ingroup  hct_mac
* @brief  ��ȡ�ű�ʱ϶����Ӧ���
*
* @par  ����:
* ��ȡ�ű�ʱ϶����Ӧ���
* @attention ���ò�Ʒ����ΪCCO���ýӿڽ��ڹ���Ƶ���¿��Ի�ȡ����Ӧֵ��
* @param  count [IN] ���� #HI_VOID��
* @retval   ret ���� #HI_BOOL��ȡֵHI_TRUE��ʾʹ���ű�ʱ϶����Ӧ��ȡֵHI_FALSE��ʾȥʹ���ű�ʱ϶����Ӧ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_GetAutoBeaconDurationFlag(HI_VOID);

/**
 * @ingroup hct_mac
* @brief  ·�ɲ������ĸ���
*/
#define HI_MDM_ROUTE_PARAM_GROUP_NUM (6)

/**
 * @ingroup hct_mac
* @brief  ·�ɲ�������ṹ��
*/
typedef struct
{
    HI_U16 station_cnt;                    /**<վ�����(����CCO)           */
    HI_U16 evaluate_cycle_len;             /**<�������ڳ��� ����λ:��           */
    HI_U8 proxy_send_dk_cycle_len;         /**<����վ�㷢�ͷ����б������ڣ� ��λ:��           */
    HI_U8 sta_send_dk_cycle_len;           /**<����վ�㷢�ͷ����б������ڣ� ��λ:��           */
    HI_U8 pad[2];
}HI_MDM_ROUTE_PARAM_S;

/**
* @ingroup  hct_mac
* @brief  ����·�ɲ���
*
* @par  ����:
* ����·�ɲ���
* @attention ���ò�Ʒ����ΪCCO��
* @param  count [IN] ���� #HI_MDM_ROUTE_PARAM_S*��·�ɲ������顣
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ���óɹ���#HI_ERR_FAILURE ����ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see hi_mdm_mac.h �ļ���#HI_MDM_ROUTE_PARAM_S�Ķ��塣
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetRouteParam(HI_MDM_ROUTE_PARAM_S param[HI_MDM_ROUTE_PARAM_GROUP_NUM]);

/**
* @ingroup  hct_mac
* @brief  ��ȡ·�ɲ���
*
* @par  ����:
* ��ȡ·�ɲ���
* @attention ���ò�Ʒ����ΪCCO��
* @param  count [OUT] ���� #HI_MDM_ROUTE_PARAM_S*��·�ɲ������顣
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ��ȡ�ɹ���#HI_ERR_FAILURE ��ȡʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see hi_mdm_mac.h �ļ���#HI_MDM_ROUTE_PARAM_S�Ķ��塣
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetRouteParam(HI_MDM_ROUTE_PARAM_S param[HI_MDM_ROUTE_PARAM_GROUP_NUM]);

/**
* @ingroup  hct_mac
* @brief  ����ÿ��·�����ڷ��͵���������
*
* @par  ����:
* ����ÿ��·�����ڷ��͵���������
* @attention �ޡ�
* @param  count [IN] ���� #HI_U8��ÿ��·�����ڷ��͵�����������ȡֵ��Χ[4,8]��
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ���óɹ�������ֵ��ʾ����ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetHeartbeatCnt(HI_U8 count);

/**
* @ingroup  hct_mac
* @brief  ��ȡÿ��·�����ڷ��͵���������
*
* @par  ����:
* ��ȡÿ��·�����ڷ��͵���������
* @attention �ޡ�
* @param  count [IN] ���� #HI_VOID��
* @retval   ret ���� #HI_U8��ÿ��·�����ڷ��͵�����������
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetHeartbeatCnt(HI_VOID);

/**
 * @ingroup hct_mac
* @brief  ���ܱ������ȼ�����ṹ��
*/
typedef struct
{
    HI_U8 assoc_priority;           /**<�������������ȼ�           */
    HI_U8 discovery_priority;       /**<�����б������ȼ�           */
}HI_MDM_NM_PKT_PRIORITY_S;

/**
* @ingroup  hct_mac
* @brief  �������ܱ������ȼ�
*
* @par  ����:
* �������ܱ������ȼ�
* @attention �ޡ�
* @param  count [IN] ���� #HI_MDM_NM_PKT_PRIORITY_S*�����ܱ������ȼ���
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ���óɹ���#HI_ERR_FAILURE ����ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see hi_mdm_mac.h �ļ���#HI_MDM_NM_PKT_PRIORITY_S�Ķ��塣
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetNmPktPriority(HI_MDM_NM_PKT_PRIORITY_S *param);

/**
* @ingroup  hct_mac
* @brief  ��ȡ���ܱ������ȼ�
*
* @par  ����:
* ��ȡ���ܱ������ȼ�
* @attention �ޡ�
* @param  count [OUT] ���� #HI_MDM_NM_PKT_PRIORITY_S*��·�ɲ������顣
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ��ȡ�ɹ���#HI_ERR_FAILURE ��ȡʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see hi_mdm_mac.h �ļ���#HI_MDM_NM_PKT_PRIORITY_S�Ķ��塣
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetNmPktPriority(HI_MDM_NM_PKT_PRIORITY_S *param);

/**
* @ingroup  hct_mac
* @brief  ��������Э������
*
* @par  ����:
* ��������Э������
* @attention ���ò�Ʒ����ΪCCO��
* @param  count [IN] ���� #HI_BOOL��ȡֵHI_TRUE��ʾ������Э����ȡֵHI_FALSE��ʾ�ر�����Э����
* @retval   ret ���� #HI_VOID��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetNetCoordinateSwitch(HI_BOOL on_off);

/**
* @ingroup  hct_mac
* @brief  ��ȡ����Э������
*
* @par  ����:
* ��ȡ����Э������
* @attention ���ò�Ʒ����ΪCCO��
* @param  count [IN] ���� #HI_VOID��
* @retval   ret ���� #HI_BOOL��ȡֵHI_TRUE��ʾ������Э����ȡֵHI_FALSE��ʾ�ر�����Э����
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_GetNetCoordinateSwitch(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ����Э��̽�⿪��
*
* @par  ����:
* ����Э��̽�⿪��
* @attention ���ò�Ʒ����ΪCCO��
* @param  count [IN] ���� #HI_BOOL��ȡֵHI_TRUE��ʾ��Э��̽�⣬ȡֵHI_FALSE��ʾ�ر�Э��̽�⡣
* @retval   ret ���� #HI_VOID��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetPtclDetectSwtich(HI_BOOL on_off);

/**
* @ingroup  hct_mac
* @brief  ��ȡЭ��̽�⿪��
*
* @par  ����:
* ��ȡЭ��̽�⿪��
* @attention ���ò�Ʒ����ΪCCO��
* @param  count [IN] ���� #HI_VOID��
* @retval   ret ���� #HI_BOOL��ȡֵHI_TRUE��ʾ��Э��̽�⣬ȡֵHI_FALSE��ʾ�ر�Э��̽�⡣
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_GetPtclDetectSwtich(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ������չ֡���ʱ��
*
* @par  ����:
* ������չ֡���ʱ��
* @attention �ޡ�
* @param  count [IN] ���� #HI_U16����չ֡���ʱ�䣬��λ��us��
* @retval   ret ���� #HI_VOID��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetEifsUS(HI_U16 eifs_us);

/**
* @ingroup  hct_mac
* @brief  ��ȡ��չ֡���ʱ��
*
* @par  ����:
* ��ȡ��չ֡���ʱ��
* @attention �ޡ�
* @param  count [IN] ���� #HI_VOID��
* @retval   ret ���� #HI_U16����չ֡���ʱ�䣬��λ��us��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U16 HI_MDM_GetEifsUS(HI_VOID);

/**
 * @ingroup hct_mac
* @brief  ���Ʋ�������ṹ��
*/
typedef struct
{
    HI_U8 PB_72_TMI;        /**<PB72           */
    HI_U8 PB_136_TMI;       /**<PB136           */
    HI_U8 PB_264_TMI;       /**<PB264           */
    HI_U8 PB_520_TMI;       /**<PB520           */
}HI_MDM_BEACON_TMI_S;

/**
* @ingroup  hct_mac
* @brief  ���õ��Ʋ���
*
* @par  ����:
* ���õ��Ʋ���
* @attention ���ò�Ʒ����ΪCCO��
* @param  count [IN] ���� #HI_MDM_BEACON_TMI_S*�����Ʋ�����
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ���óɹ���#HI_ERR_FAILURE ����ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see hi_mdm_mac.h �ļ���#HI_MDM_BEACON_TMI_S�Ķ��塣
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetBeaconTMI(HI_MDM_BEACON_TMI_S *tmi);

/**
* @ingroup  hct_mac
* @brief  ��ȡ���Ʋ���
*
* @par  ����:
* ��ȡ���Ʋ���
* @attention ���ò�Ʒ����ΪCCO��
* @param  count [OUT] ���� #HI_MDM_BEACON_TMI_S*�����Ʋ�����
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ��ȡ�ɹ���#HI_ERR_FAILURE ��ȡʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see hi_mdm_mac.h �ļ���#HI_MDM_BEACON_TMI_S�Ķ��塣
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetBeaconTMI(HI_MDM_BEACON_TMI_S *tmi);

/**
 * @ingroup hct_mac
* @brief  ���ͷ����б�ʹ�õ�TMIģʽ
*/
typedef enum
{
    HI_MDM_DISCOVERY_TMI_CHANGE = 0,            /**<��̬TMI����           */
    HI_MDM_DISCOVERY_TMI_LOW_LATENCY,           /**<��ʱ��TMI����           */
    HI_MDM_DISCOVERY_TMI_HIGH_DEPENDENCE,       /**<�߿ɿ���TMI����           */
    HI_MDM_DISCOVERY_TMI_MAX
}HI_MDM_DISCOVERY_TMI_MODE;

/**
* @ingroup  hct_mac
* @brief  ���÷��ͷ����б�ʹ�õ�TMIģʽ
*
* @par  ����:
* ���÷��ͷ����б�ʹ�õ�TMIģʽ
* @attention �ޡ�
* @param  count [IN] ���� #HI_MDM_DISCOVERY_TMI_MODE*�����ͷ����б�ʹ�õ�TMIģʽ��
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ���óɹ���#HI_ERR_FAILURE ����ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see hi_mdm_mac.h �ļ���#HI_MDM_DISCOVERY_TMI_MODE�Ķ��塣
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetNmDiscoveryTMIMode(HI_MDM_DISCOVERY_TMI_MODE mode);

/**
* @ingroup  hct_mac
* @brief  ��ȡ���ͷ����б�ʹ�õ�TMIģʽ
*
* @par  ����:
* ��ȡ���ͷ����б�ʹ�õ�TMIģʽ
* @attention �ޡ�
* @param  count [IN] ���� #HI_VOID��
* @retval   ret ���� #HI_MDM_DISCOVERY_TMI_MODE�����ͷ����б�ʹ�õ�TMIģʽ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see hi_mdm_mac.h �ļ���#HI_MDM_DISCOVERY_TMI_MODE�Ķ��塣
* @since HiMDL_V200R001C00
*/
HI_MDM_DISCOVERY_TMI_MODE HI_MDM_GetNmDiscoveryTMIMode(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ������λ��
*
* @par  ����:
* ������λ��
* @attention ���ò�Ʒ����ΪCCO��
* @param  count [IN] ���� #HI_U8����λ����ȡֵ��ΧΪ1��3
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ���óɹ�������ֵ��ʾ����ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetPhaseCnt(HI_U8 count);

/**
* @ingroup  hct_mac
* @brief  ��ȡ��λ��
*
* @par  ����:
* ��ȡ��λ��
* @attention �ޡ�
* @param  count [IN] ���� #HI_VOID��
* @retval   ret ���� #HI_U8����λ����
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetPhaseCnt(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ���ù��ʼ������
*
* @par  ����:
* ���ù��ʼ������
* @attention �ޡ�
* @param  count [IN] ���� #HI_U8�����ʼ�����ޡ�
* @retval    ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ���óɹ���#HI_ERR_FAILURE ����ʧ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetPowerDetectThreshold(HI_U8 threshold);

/**
* @ingroup  hct_mac
* @brief  ��ȡ���ʼ������
*
* @par  ����:
* ��ȡ���ʼ������
* @attention �ޡ�
* @param  count [IN] ���� #HI_VOID��
* @retval   ret ���� #HI_U8�����ʼ�����ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetPowerDetectThreshold(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ���ò��м�⿪��
*
* @par  ����:
* ���ò��м�⿪��
* @attention �ޡ�
* @param  count [IN] ���� #HI_BOOL��ȡֵHI_TRUE��ʾ�򿪲��м�⣬ȡֵHI_FALSE��ʾ�رղ��м�⡣
* @retval   ret ���� #HI_VOID��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetParallelDetectSwitch(HI_BOOL on_off);

/**
* @ingroup  hct_mac
* @brief  ��ȡ���м�⿪��
*
* @par  ����: ��ȡ���м�⿪��
* @attention �ޡ�
* @param  on_off [OUT] ���� #HI_BOOL*�����м�⿪�ء�
* @retval   ret ���� #HI_U32��ȡֵ#HI_ERR_SUCCESS ��ȡ�ɹ���#HI_ERR_FAILURE ��ȡʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetParallelDetectSwitch(HI_BOOL *on_off);


/**
*@ingroup hct_mac
* ��ȡ�շ������ͱ���ͳ��
*/
typedef struct
{
    HI_U32 unicast_count;                       /**<�������ĸ���           */
    HI_U32 broadcast_count;                     /**<�㲥���ĸ���           */

    HI_U32 short_packet_count;                  /**<���Ͷ̰�����           */
    HI_U32 long_packet_count;                   /**<���ͳ�������           */
    HI_U32 pb72_packet_count;                   /**<����pb72����           */
    HI_U32 pb264_packet_count;                  /**<����pb264����           */

    HI_U32 tx_send_msdu_success;                /**<���ͻ��ɹ�����msdu������           */
    HI_U32 tx_send_msdu_timeout;                /**<���ͻ����ͳ�ʱmsdu������           */
    HI_U32 tx_send_msdu_fail;                   /**<���ͻ�����ʧ��msdu������           */
    HI_U32 tx_send_msdu_resend;                 /**<���ͻ��ط�msdu�Ĵ���           */

    HI_U32 tx_send_app_success;                 /**<���ͻ��ɹ�����app���ĵĸ���           */
    HI_U32 tx_send_app_timeout;                 /**<���ͻ����ͳ�ʱ��app���ĵĸ���           */
    HI_U32 tx_send_app_fail;                    /**<���ͻ�����ʧ�ܵ�app���ĵĸ���           */
    HI_U32 tx_send_app_resend;                  /**<���ͻ��ط�app���ĵĸ���           */
}HI_MDM_TX_STATICS_S;

/**
*@ingroup hct_mac
* ��ȡ�շ������ձ���ͳ��
*/
typedef struct
{
    HI_U32 fc_count;                        /**<FC�жϴ���             */
    HI_U32 fc_crc_success_count;            /**<FCУ����ȷ����            */
    HI_U32 fc_crc_err_count;                /**<FCУ��������           */
    HI_U32 fc_local_count;                  /**<����FC����           */

    HI_U32 sof_count;                       /**<����sof֡����           */
    HI_U32 beacon_count;                    /**<����beacon֡����           */

    HI_U32 beacon_pb_crc_err_count;         /**<beacon pbУ��ʧ������           */
    HI_U32 beacon_pb_crc_success_count;     /**<beacon pbУ��ɹ�����           */

    HI_U32 broadcast_count;                 /**<���ع㲥���ĸ���            */
    HI_U32 unicast_count;                   /**<���ص������ĸ���             */

    HI_U32 resend_count;                    /**<�����ش����ĸ���           */

    HI_U32 short_pb_crc_err_count;          /**<136pb��crcУ�������           */
    HI_U32 long_pb_crc_err_count;           /**<520pb��crcУ�������           */
    HI_U32 pb72_crc_err_count;              /**<72pb��crcУ�������           */
    HI_U32 pb264_crc_err_count;             /**<264pb��crcУ�������           */

    HI_U32 short_pb_count;                  /**<136pb�����           */
    HI_U32 long_pb_count;                   /**<520pb�����           */
    HI_U32 pb72_count;                      /**<72pb�����           */
    HI_U32 pb264_count;                     /**<264pb�����            */

    HI_U32 discard_packet_count;            /**<��������           */
}HI_MDM_RX_STATICS_S;

/**
*@ingroup hct_mac
* ��ȡ���������ͳ��
*/
typedef struct
{
    HI_U32 cvg_send_app_success;            /**<���������app���ĵ����ͻ��ɹ��ĸ���           */
    HI_U32 cvg_send_app_fail;               /**<���������app���ĵ����ͻ�ʧ�ܵĸ���           */
    HI_U32 cvg_recv_app_success;            /**<������ӷ��ͻ�����app���ĳɹ��ĸ���           */
    HI_U32 cvg_recv_app_fail;               /**<������ӷ��ͻ�����app����ʧ�ܵĸ���           */
}HI_MDM_CVG_STATICS_S;

/**
* @ingroup  hct_mac
* @brief ��ȡ���ͱ���ͳ��
*
* @par  ����:
* ��STA/CCO ��ȡ���ͱ���ͳ��
* @attention �ޡ�
* @param  �ޡ�
* @retval   ret ����#HI_U32��#HI_ERR_SUCCESS��ʾ��ѯ�ɹ�,#HI_ERR_INVALID_PARAMETER��ʾ��������,#HI_ERR_FAILURE ��ʾʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetTxStatistics(HI_MDM_TX_STATICS_S  *tx_stats);

/**
* @ingroup  hct_mac
* @brief ��ȡ���ձ���ͳ��
*
* @par  ����:
* ��STA/CCO ��ȡ���ձ���ͳ��
* @attention �ޡ�
* @param  �ޡ�
* @retval   ret ����#HI_U32��#HI_ERR_SUCCESS��ʾ��ѯ�ɹ�,#HI_ERR_INVALID_PARAMETER��ʾ��������,#HI_ERR_FAILURE ��ʾʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetRxStatistics(HI_MDM_RX_STATICS_S *rx_stats);

/**
* @ingroup  hct_mac
* @brief ��ȡ���������ͳ��
*
* @par  ����:
* ��STA/CCO ��ȡ���������ͳ��
* @attention �ޡ�
* @param  �ޡ�
* @retval   ret ����#HI_U32��#HI_ERR_SUCCESS��ʾ��ѯ�ɹ�,#HI_ERR_INVALID_PARAMETER��ʾ��������,#HI_ERR_FAILURE ��ʾʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetCvgStatistics(HI_MDM_CVG_STATICS_S  *cvg_stats);

/**
* @ingroup  hct_mac
* @brief ������ͱ���ͳ��
*
* @par  ����:
* ��STA/CCO ������ͱ���ͳ��
* @attention �ޡ�
* @param  �ޡ�
* @retval  �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_ClearTxStatistics(HI_VOID);

/**
* @ingroup  hct_mac
* @brief ������ձ���ͳ��
*
* @par  ����:
* ��STA/CCO ������ձ���ͳ��
* @attention �ޡ�
* @param  �ޡ�
* @retval  �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_ClearRxStatistics(HI_VOID);

/**
* @ingroup  hct_mac
* @brief ������������ͳ��
*
* @par  ����:
* ��STA/CCO ������������ͳ��
* @attention �ޡ�
* @param  �ޡ�
* @retval  �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_ClearCvgStatistics(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ��������������ڵĸ���
*
* @par  ����:
* ��������������ڵĸ���
* @attention ���ò�Ʒ����ΪCCO��
* @param  count [IN] ���� #HI_U8������������ڵĸ�����
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS��ʾ���óɹ�,#HI_ERR_INVALID_PARAMETER��ʾ��������
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetLeaveCycle(HI_U8 leave_cycle);

/**
* @ingroup  hct_mac
* @brief  ��ȡ�����ϱ����ڵĸ���
*
* @par  ����:
* ��ȡ�����ϱ����ڵĸ���
* @attention ���ò�Ʒ����ΪCCO��
* @param  count [IN] ���� #HI_VOID��
* @retval   ret ���� #HI_U8��վ�������ϱ����ڵĸ�����
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetOfflineReportCycle(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ���������ϱ����ڵĸ���
*
* @par  ����:
* ���������ϱ����ڵĸ���
* @attention ���ò�Ʒ����ΪCCO��
* @param  offline_cycle [IN] ���� #HI_U8�������ϱ����ڵĸ�����
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS��ʾ���óɹ�,#HI_ERR_INVALID_PARAMETER��ʾ��������
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetOfflineReportCycle(HI_U8 offline_cycle);

/**
* @ingroup  hct_mac
* @brief  ��ȡ����������ڵĸ���
*
* @par  ����:
* ��ȡ����������ڵĸ���
* @attention ���ò�Ʒ����ΪCCO��
* @param  count [IN] ���� #HI_VOID��
* @retval   ret ���� #HI_U8������������ڵĸ�����
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetLeaveCycle(HI_VOID);


/**
* @ingroup  hct_mac
* @brief ���ù���ģʽ������Ч�ӿڡ�
*
* @par  ����:
* ���ù���ģʽ������Ч��
* @attention ��CCO/STA��ʹ�á�
* @param  freq_mode[IN] ���� #HI_U8��ʾ���ù���ģʽ��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS��ʾ���óɹ���#HI_ERR_INVALID_PARAMETER��ʾ����ģʽ��Ч��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetFreqWithoutDelay(HI_U8 freq_mode);

/**
 * @ingroup hct_mac
 * @brief ������̨��APP�㲥���Ĺ��˺���
 */
typedef HI_BOOL (*MAC_NEIGHBOR_BROADCAST_FRAME_FILTER_FUN)(HI_CONST HI_PVOID frame, HI_U16 size);

/**
 * @ingroup hct_mac
 * @brief ������̨��APP�㲥���Ŀ��ƽṹ
 */
typedef struct
{
    HI_BOOL rcv_switch;     /**< ���տ���: HI_TRUE��ʾ����������㲥APP���ģ�HI_FALSE��ʾ������ >*/
    HI_U8   pad[3];

    MAC_NEIGHBOR_BROADCAST_FRAME_FILTER_FUN filter_fun; /**< ����֮��Ĺ��˺��� >*/
} HI_PLC_NEIGHBOR_BROADCAST_FRAME_CTRL_STRU;

/**
* @ingroup  hct_mac
* @brief ���ý�����̨���㲥APP���Ŀ���
*
* @par  ����:
* ���ý�����̨���㲥APP���Ŀ��ơ�
* @attention ��CCO/STA��ʹ�á�
* @param  rcv_ctrl [IN] ���� #HI_PLC_NEIGHBOR_BROADCAST_FRAME_CTRL_STRU ���õĿ���ģʽ��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS��ʾ���óɹ���������ʾʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetRcvNeighborBrodcastFrameCtrl(HI_PLC_NEIGHBOR_BROADCAST_FRAME_CTRL_STRU *rcv_ctrl);

/**
 * @ingroup hct_mac
* @brief  ͨ�����ͣ�IPV6ͨ�����͡�
*/
#define HI_MDM_IPV6_TYPE                  (7)

/**
 * @ingroup hct_mac
* @brief  MAC֡ͷ���ͣ���֡ͷ��
*/
#define HI_MDM_MAC_TYPE_SHORT_PK          (0)

/**
 * @ingroup hct_mac
 * @brief  MAC֡ͷ���ͣ���֡ͷ��
*/
#define HI_MDM_MAC_TYPE_LONG_PK           (1)


/**
 * @ingroup hct_mac
 * @brief  PLCͨ�����Ͳ���
*/
typedef struct
{
    HI_U8   ucSrcMac[HI_PLC_MAC_ADDR_LEN];      /**<��������Դ��ַ           */
    HI_U8   ucDestMac[HI_PLC_MAC_ADDR_LEN];     /**<��������Ŀ�ĵ�ַ           */

    HI_U8   Mtype;                              /**<��������           */
    HI_U8   MacHeadType;                        /**<֡ͷ����           */
    HI_BOOL IsDefaultLid;                       /**<�Ƿ�ʹ��Ĭ������           */
    HI_U8   Lid;                                /**<�������ȼ�           */

    HI_U32  Timeout;                            /**<���Ĺ���ʱ�䣬0��ʾ����Ĭ�ϵĳ�ʱʱ��           */

    HI_U32  IsMyNetFrame    : 1;                /**<�Ƿ����籨�� */
    HI_U32  RcvFreq         : 7;                /**<���ձ��ĵ�Ƶ�κ� */
    HI_U32  RcvNid          : 24;               /**<���ձ��ĵ������ */
}HI_PLC_FRAME_CTRL_STRU;

/**
* @ingroup  hct_mac
* @brief   Ӧ�ò�ע�ᵽЭ��ջ�Ľ��սӿ�
*
* @par  ����:
* Ӧ�ò�ע�ᵽЭ��ջ�Ľ��սӿ�
* @attention �ޡ�
* @param  Frame [IN] ���� #HI_PVOID��Ӧ�ò����ݻ��档
* @param  FrameSize [IN] ���� #HI_U16��Ӧ�ò����ݴ�С��
* @param  pFrameCtrl [IN] ���� #HI_PLC_FRAME_CTRL_STRU��Ӧ�ò����ݲ�����
* @param  pBuf [IN] ���� #HI_PVOID��Э��ջ�������ݵĻ��棬�㿽����ʹ�ã����㿽��Ϊ�ա�
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see hi_mdm_mac.h �ļ���#HI_MDM_Report_Topo_Change_Node_Fun ���塣
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*MAC_PACKET_INPUT_FUN)(HI_PVOID Frame, HI_U16 FrameSize,HI_PLC_FRAME_CTRL_STRU* pFrameCtrl,HI_PVOID pBuf);


/**
 * @ingroup hct_mac
 * @brief  Ӧ�ò�ͨ��ע�����
*/
typedef struct
{
    HI_U8   DefalutLid;         /**<Ӧ�ò㱨��Ĭ�����ȼ�           */
    HI_U8   Pad[3];

    MAC_PACKET_INPUT_FUN InputFun;  /**<Ӧ�ò���սӿ�           */
}HI_MDM_PLC_CHANNEL_INFO_STRU;

/**
* @ingroup  hct_mac
* @brief Ӧ�ò���Э��ջע�����ͨ��
*
* @par  ����:
* Ӧ�ò���Э��ջע�����ͨ��
* @attention IPV6���ͽ�֧���ڹ���Э����ʹ�á�
* @param  pMacChannelInfo[IN] ���� #HI_MDM_PLC_CHANNEL_INFO_STRU��ͨ��������
* @param  ChannelType[IN] ���� #HI_U8��ͨ�����ͣ�#HI_MDM_IPV6_TYPE��IPV6ͨ��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS��ʾע��ɹ���#HI_ERR_INVALID_PARAMETER��ʾע��ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterPlcChannel(HI_IN HI_MDM_PLC_CHANNEL_INFO_STRU *pMacChannelInfo,HI_IN HI_U8 ChannelType);

/**
* @ingroup  hct_mac
* @brief PLC���ݷ��ͽӿ�
*
* @par  ����:
* Ӧ�ò���øýӿڷ�������
* @attention ��CCO/STA��ʹ�á�
* @param  Frame[IN] ���� #HI_PVOID��Ӧ�ò����ݻ���
* @param  FrameSize[IN] ���� #HI_U16��Ӧ�ò����ݴ�С
* @param  pFrameCtrl[IN] ���� #HI_PLC_FRAME_CTRL_STRU�����ķ��Ͳ���
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS��ʾ���ķ��͵�Э��ջ�ɹ���#HI_ERR_INVALID_PARAMETER��ʾ���͵�Э��ջʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_PlcSendFrame(HI_PVOID Frame, HI_U16 FrameSize,HI_PLC_FRAME_CTRL_STRU *pFrameCtrl);
/**
* @ingroup  hct_mac
* @brief   Ӧ�ò�ע�ᵽЭ��ջ�Ķ�������֤�ӿ����Ͷ���
* @par  ����:
* Ӧ�ò�ע�ᵽЭ��ջ�Ķ�������֤�ӿ�
* @attention �ޡ�
* @param  network_type [IN] ���� #HI_U32 CCO������Ϣ
* @param  is_open_auth_cco [IN] ���� #HI_BOOL  CCO�Ƿ���������֤
* @retval   HI_U8 ��֤���:
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @since HiMDL_V200R001C00
*/
typedef HI_BOOL(*STA_AUTH_CCO_BY_NETWORK_TYPE)(HI_BOOL is_open_auth_cco,HI_U32 network_type);
/**
* @ingroup  hct_mac
* @brief ����Э���¶�������֤��ʼ���ӿ�
* @param  sta_auth_coo_network_func [IN] ���� #STA_AUTH_CCO_BY_NETWORK_TYPE ��������֤�ص�����
* @par  ����:
* Ӧ�ò���øýӿڳ�ʼ��������֤��Դ
* @attention ��STA��ʹ�á�
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS��ʾ��ʼ���ɹ���#HI_ERR_INVALID_PARAMETER��ʾ��ʼ��ʧ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterStaAuthCco(STA_AUTH_CCO_BY_NETWORK_TYPE sta_auth_coo_network_func);
/**
* @ingroup  hct_mac
* @brief ������֤����
* @par  ����:
* Ӧ�ò���øýӿ����������߹ر�������֤
* @attention ��STA��ʹ�á�
* @param  Switch [IN] ���� #HI_BOOL ������֤����
* @retval   ret ���� #HI_VOID
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_SetNetworkSwitch(HI_BOOL Switch);
/**
* @ingroup  hct_mac
* @brief ���ñ�������Ϣ
* @par  ����:
* Ӧ�ò���øýӿڳ�ʼ��������Ϣ
* @attention ��CCO/STA��ʹ�á������һ����Ϊ���������
* @param  field_type [IN] ���� #HI_U32���������͡�
* @retval   ret ���� HI_U32#HI_ERR_SUCCESS :���óɹ���#HI_ERR_INVALID_PARAMETER:����ʧ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetProductFieldType(HI_U32 field_type);
/**
* @ingroup  hct_mac
* @brief ��ȡ����Ʒ��������Ϣ
* @par  ����:
* Ӧ�ò���øýӿڻ�ȡ����Ʒ��������Ϣ
* @attention ��CCO/STA��ʹ�á�
* @retval   ret ���� HI_U32#��Ʒ��������
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetProductFieldType(HI_VOID);
/**
 * @ingroup hct_mac
 * @brief  ����ID����
*/
#define MANAGEMENT_ID_SIZE  (24)
/**
 * @ingroup hct_mac
 * @brief  PLCͨ�����Ͳ���
*/
typedef struct
{
     HI_U8  sta_mac[HI_PLC_MAC_ADDR_LEN]; /**<STA mac��ַ          */
     HI_U8  product_type;                 /**<��Ʒ����             */
     HI_U8  mac_type;
     HI_U8  id[MANAGEMENT_ID_SIZE];       /**<����ID��Ϣ           */
}HI_MDM_CCO_AHTU_STA_INFO_STRU;

/**
 * @ingroup hct_mac
 * @brief  �ϱ����������ӹ����������
*/
#define NDM_CONNECT_REPORT_SEARCH_RESULT                0

/**
 * @ingroup hct_mac
 * @brief  �ϱ����������ӹ��ܽ����mac��ַ
*/
#define NDM_CONNECT_REPORT_ACCESS_MAC                   1

/**
 *@ingroup hct_mac
 * ���������ӹ����ϱ���Ϣ����
 */
typedef struct
{
    HI_BOOL is_search_success;      /**<�Ƿ������ɹ�           */
    HI_U8 pad;
    HI_U16 stei;                    /**<����tei                */

    HI_U32 nid;                     /**<����nid                */

    HI_U8 mac[HI_PLC_MAC_ADDR_LEN]; /**<����վ���mac��ַ      */
    HI_U16 pad1;
}HI_MDM_NDM_CONNECT_SEARCH_REPORT_INFO_ST;

/**
 *@ingroup hct_mac
 * ���������ӹ���CCO/STAģ��״̬ö��
 */
typedef enum
{
    NDM_CONNECT_PASSIVE_SEARCH_STATE = 0,           /**< ��������״̬ */
    NDM_CONNECT_FAST_SYNC_STATE,                    /**<  ����ͬ��״̬ */
    NDM_CONNECT_LISTEN_STATE,                       /**<  ����״̬ */
    NDM_CONNECT_DATA_TRANSMISSION_STATE,            /**<  ���ݴ���״̬ */
}ndm_connect_state_enum;

/**
 *@ingroup hct_mac
 * ���������ӹ��ܳ�����ģ��״̬ö��
 */
typedef enum
{
    NDM_CONNECT_NDM_INIT_STATE = 0,                     /**< ��ʼ��״̬ */
    NDM_CONNECT_NDM_SEND_SEARCH_FRAME_STATE,            /**< ��������֡״̬ */
    NDM_CONNECT_NDM_CONNECTED_STATE                      /**<  ����������״̬ */
}ndm_connect_ndm_state_enum;

/**
 * @ingroup hct_mac
 * @brief  ��������ʹ�ð汾����
*/
typedef enum
{
    NDM_BJ_VERSION = 1,                                 /**< �����������汾��� */
    NDM_CQ_VERSION,                                     /**< ���쳭�����汾��� */
    NDM_MAX_VERSION                                     /**< ��󳭿����汾��� */
}ndm_special_version_enum;

/**
* @ingroup  hct_mac
* @brief  ���������ӹ��ܵĻص�����
* @par  ����:
* �������ڽ��յ�ͬ��֡�����ntb ͬ�� �ϱ��¼���app
* @attention �ޡ�
* @param  ucType [IN] ���� #HI_U8���¼����͡��ο� #NDM_CONNECT_REPORT_SEARCH_RESULT��\n
                                                  #NDM_CONNECT_REPORT_ACCESS_MAC��
* @param  pCmdData [IN] ���� #HI_MDM_NDM_CONNECT_SEARCH_REPORT_INFO_ST *������ָ�롣
* @param  ulDataSize [IN] ���� #HI_U32�����ݴ�С��
* @retval ret [OUT] ���� #HI_U32��#HI_ERR_SUCCESS �ɹ������෵��ֵ�μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see HI_MDM_NDM_CONNECT_SEARCH_REPORT_INFO_ST ���塣
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_NdmConnectSearchReportCallback)(HI_U8 ucType,HI_MDM_NDM_CONNECT_SEARCH_REPORT_INFO_ST *pCmdData,HI_U32 ulDataSize);

/**
 *@ingroup hct_mac
 * ���������ӹ��ܻص�������װ�ṹ��
 */
typedef struct
{
    HI_MDM_NdmConnectSearchReportCallback ndm_connect_search_report_callback;  /**<  ������������������ص����� */
}HI_MDM_MAC_NDM_CONNECT_SEARCH_REPORT_ST;

/**
* @ingroup  hct_mac
* @brief  ע�᳭�������ӹ�����غ���
*
* @par  ����:
*       ע�᳭�������ӹ�����غ���
* @attention ���������ӹ���ע��ӿ�\n
*
* @param   ��
* @retval  ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ���óɹ���#������ʾ����ʧ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_NdmSgConnectRegister(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ����������״̬�ϱ�appע��ӿ�
* @par  ����:
* ����������״̬�ϱ�appע��ӿ�
* @attention �ޡ�
* @param    SearchCallback [IN] ���� #HI_MDM_NdmConnectSearchReportCallback�� ���������ӹ��ܵĻص�������
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾע��ɹ���
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see #HI_MDM_NdmConnectSearchReportCallback �ص�������
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_NdmConnectSearchResultReport(HI_MDM_NdmConnectSearchReportCallback SearchCallback);

/**
* @ingroup  hct_mac
* @brief ��ȡʹ�ó��������ӹ����µ�ģ��״̬
* @par  ����:
* ��ȡʹ�ó��������ӹ����µ�ģ��״̬
* @attention �������ο�ndm_connect_ndm_state_enum�ṹ�壬CCO/STA �ο�ndm_connect_state_enum�ṹ��
* @retval   ret ���� #HI_U8 ������ģ��״̬ #ndm_connect_ndm_state_enum��\n
                            CCO/STA ģ��״̬ #ndm_connect_state_enum
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_NdmConnectState(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  �������������ӽӿ�
* @par  ����:
* ����������ָ��mac��ַվ��
* @attention �ޡ�
* @param    oda [IN] ���� #HI_U8�� ��������Ҫ����վ���mac��ַ��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ�����ɹ���������ʾʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_NdmConnectStartSearchNet(HI_U8 oda[HI_PLC_MAC_ADDR_LEN]);

/**
* @ingroup  hct_mac
* @brief   Ӧ�ò�ע�ᵽЭ��ջ�Ĺ���ID��֤�ӿ�
* @par  ����:
* Ӧ�ò�ע�ᵽЭ��ջ�Ĺ���ID��֤�ӿ�
* @attention �ޡ�
* @param  ManagementId [IN] ���� #HI_U8 *������ID��Ϣ��
* @retval   HI_BOOL ��֤���:
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @since HiMDL_V200R001C00
*/
typedef HI_BOOL (*CCO_AUTH_MANAGEMENTID_FUN)(HI_MDM_CCO_AHTU_STA_INFO_STRU *auth_info);
/**
* @ingroup  hct_mac
* @brief ����Э����CCO��֤sta����ID�ӿ�
* @param  cco_auth_id_func [IN] ���� #CCO_AUTH_MANAGEMENTID_FUN ��֤����ID�ӿڻص�����
* @par  ����:
* ����Э����CCO��֤sta����ID�ӿ�
* @attention ��STA��ʹ�á�
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS��ʾע��ɹ���#HI_ERR_FAILURE��ʾע��ʧ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterCcoAuthManagementId(CCO_AUTH_MANAGEMENTID_FUN cco_auth_id_func);
/**
* @ingroup  hct_mac
* @brief ��վ����������ʱ���ù���ID����������ֻ�ڱ�������ʱ��Ч,����Ӱ��оƬ�б����ID��Ϣ���������Ҫ�������á�
* @param  runtime_management_id [IN] ���� #HI_U8* ����ID��Ϣ,is_use_default_id[IN] #HI_BOOL
* @param  is_use_default_id [IN] ���� #HI_BOOL �Ƿ�ʹ��Ĭ�ϵĹ���ID
* @par  ����:
* ��վ����������ʱ���ù���ID
* @attention ��CCO/STA��ʹ�á�
* @attention is_use_default_id:HI_TRUEʱ������ID��ʹ��оƬ�Դ�ID��Ϣ��������ʹ��runtime_management_id��ֵ
                              :HI_FALSEʱ����runtime_management_id��Ϊ�豸�Ĺ���ID��Ϣ
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ���óɹ���#HI_ERR_INVALID_PARAMETER:��������
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetRunTimeManagementId(HI_U8 runtime_management_id[MANAGEMENT_ID_SIZE],HI_BOOL is_use_default_id);
/**
* @ingroup  hct_mac
* @brief ��ȡվ����������ʱ���õĹ���ID�����δ���ù���ID��Ϣ���򷵻ص���оƬ����Դ�����ID��Ϣ��
* @param  runtime_management_id [OUT] ���� #HI_U8* ����ID��Ϣ
* @par  ����:
* ��ȡվ����������ʱ���ù���ID
* @attention ��CCO/STA��ʹ�á�
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ��ȡ�ɹ���#HI_ERR_INVALID_PARAMETER:��������
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetRunTimeManagementId(HI_U8 runtime_management_id[MANAGEMENT_ID_SIZE]);

/**
* @ingroup  hct_mac
* @brief ����STA�˰�������ʶ�������ر�ʶ��(��������ʶ�����ر�ʶĬ���ǹر�״̬)
* @param  is_set_disable [IN] ���� #HI_BOOL HI_TRUE :����  HI_FALSE:ͣ��
* @par  ����:
* ���ð�������ʶ�������ر�ʶ��
* @attention �ýӿڽ�����������ʹ��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ���óɹ���#HI_ERR_INVALID_PARAMETER:��������
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetFullLoadAndWhiteListEnable(HI_BOOL is_set_enable);
/**
* @ingroup  hct_mac
* @brief ��ȡSTA�˵�ǰ��������ʶ�������ر�ʶ��ʹ��״̬
* @par  ����:
* ��ȡ��ǰ��������ʶ�������ر�ʶ��ʹ��״̬
* @attention �ýӿڽ�����������ʹ��
* @retval   ret ���� #HI_BOOL��#HI_TRUE :��ʾ����ʹ�ã�#HI_FALSE :δʹ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_GetFullLoadAndWhiteListState(HI_VOID);
/**
* @ingroup  hct_mac
* @brief  ��ȡ������ȷ��PB����ۼ�ֵ
* @par  ����:
*  ��ȡ������ȷ��PB����ۼ�ֵ
* @retval   ret ���� #HI_U32��������ȷ��PB�������
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MAC_GetRxPbStatistic(HI_VOID);

/**
* @ingroup hct_mac
* @brief  ID̽�⹦�� cco/sta ״̬�ṹ��
*/
typedef enum
{
    NDM_DETECTID_INIT_STATE = 5,                      /**< ��ʼ��״̬ */
    NDM_DETECTID_ACCESS_NDM_STATE,                    /**<  ���볭����״̬ */
    NDM_DETECTID_DATA_TRANSMISSION_STATE,            /**<  ���ݴ���״̬ */
}ndm_detectid_state_enum;

/**
* @ingroup hct_mac
* @brief  ID̽�⹦�� ������״̬�ṹ��
*/
typedef enum
{
    NDM_DETECTID_NDM_INIT_STATE = 5,                    /**< ��ʼ��״̬ */
    NDM_DETECTID_NDM_IDLE_STATE,                        /**< ����״̬ */
    NDM_DETECTID_CREATE_BCN_STATE,                      /**<  �����ű�״̬ */
    NDM_DETECTID_NDM_LISTEN_DATA_STATE,                 /**<  ��������״̬ */
    NDM_DETECTID_NDM_DATA_TRANSMISSION_STATE,           /**<  ���ݴ���״̬ */
    NDM_DETECTID_NDM_FINISH_DETECT_STATE,               /**<  ̽�����״̬ */
}ndm_detectid_ndm_state_enum;

/**
* @ingroup hct_mac
* @brief  ������̽��id����״̬�ϱ�
*/
#define NDM_DETECTID_STATE_REPORT       1

/**
* @ingroup hct_mac
* @brief  ������̽��id�����ϱ���Ϣ
*/

typedef struct
{
    HI_U8 ndm_state;
    HI_U8 freq;
    HI_U16 pad1;
}HI_MDM_NDM_DETECTID_REPORT_INFO_ST;

/**
* @ingroup  hct_mac
* @brief  ������̽��id����״̬�仯�ص�����
* @par  ����:
* ����������״̬�仯ʱ�����øú������ϱ�״̬�仯��app
* @attention �ޡ�
* @param  ucType [IN] ���� #HI_U8���¼����͡��ο� #NDM_DETECTID_STATE_REPORT��
* @param  pCmdData [IN] ���� #HI_MDM_NDM_DETECTID_REPORT_INFO_ST *������ָ�롣
* @param  ulDataSize [IN] ���� #HI_U32�����ݴ�С��
* @retval ret [OUT] ���� #HI_U32��#HI_ERR_SUCCESS �ɹ������෵��ֵ�μ�hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see HI_MDM_NDM_DETECTID_REPORT_INFO_ST ���塣
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_NdmDetectIdStateReportCallback)(HI_U8 ucType,HI_MDM_NDM_DETECTID_REPORT_INFO_ST *pCmdData,HI_U32 ulDataSize);

/**
* @ingroup hct_mac
* ������̽��id���ܻص�������װ�ṹ��
*/
typedef struct
{
    HI_MDM_NdmDetectIdStateReportCallback ndm_detectid_state_report_callback;
}HI_MDM_MAC_NDM_DETECTID_STATE_REPORT_ST;

/**
* @ingroup  hct_mac
* @brief  ������̽��id��������̽��ӿڣ���Ҫ��ɹ���ע��
* @param  oda [IN] ���� #HI_U8 ̽��վ���ַ�� freq [IN] ���� #HI_U8 ̽��Ƶ��
* @par  ����:
*  ��ɹ���ע��󣬵��øýӿڽ���ģ��id̽��
* @attention �ýӿ��ڹ���ע����ɺ������Ч
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS :����̽��ɹ���#HI_ERR_FAILURE :����̽��ʧ��(δ����ע��)
*                              ���෵��ֵ�μ�hi_errno.h��
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_NdmStartDetectId(HI_U8 oda[HI_PLC_MAC_ADDR_LEN], HI_U8 freq);

/**
* @ingroup  hct_mac
* @brief  ������̽��id����ע��ӿ�
* @param  ��
* @par  ����:
*  ��ɳ�����ע��
* @attention ��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS :ע��ɹ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_NdmDetectIdRegister(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ������̽��id����ʱ����Ƶ�νӿڣ�Ĭ��Э��Ϊ����Э�飬 snidΪ0
* @param  freq [IN] ���� #HI_U8 Ƶ��
* @par  ����:
*  ������̽��id����ʱ����Ƶ�νӿڣ�Ĭ��Э��Ϊ����Э�飬 snidΪ0
* @attention �ýӿ�ֻ�ڳ�����ģ��ʹ��
* @retval  ret ���� #HI_U32��#HI_ERR_SUCCESS :���óɹ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_NdmDetectIdSetFreq(HI_U8 freq);
/**
* @ingroup  hct_mac
* @brief  ������̽��id��������ģ��״̬
* @param  state [IN] ���� #HI_U8 ȡֵ�ο� #ndm_detectid_ndm_state_enum
* @par  ����:
*  app����յ� cco/sta ��Ӧ��̽��id����ʱ���ɵ��øýӿ����ó�����ģ���״̬
* @attention �ýӿ�ֻ�ڳ�����ģ��ʹ��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS :���óɹ���#HI_ERR_FAILURE :����ʧ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_NdmDetectIdSetState(HI_U8 state);
/**
* @ingroup  hct_mac
* @brief  ������̽��id����״̬�ϱ�appע��ӿ�
* @par  ����:
* ������̽��id����״̬�ϱ�appע��ӿ�
* @attention �ޡ�
* @param    DetectidStateCallback [IN] ���� #HI_MDM_NdmDetectIdStateReportCallback�� ������̽��id���ܵĻص�������
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾע��ɹ���
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see #HI_MDM_NdmDetectIdStateReportCallback �ص�������
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_NdmDetectidStateReport(HI_MDM_NdmDetectIdStateReportCallback DetectidStateCallback);

/**
* @ingroup  hct_mac
* @brief  ������̽��id�������ý������ݱ�־λ
* @param  is_rcv_data [IN] ���� #HI_BOOL HI_TRUE :���յ�̽����Ӧ������  HI_FALSE:δ���յ�̽����Ӧ������
* @par  ����: ��
* @attention ��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS :���óɹ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_NdmDetectIdSetRcvData(HI_BOOL is_rcv_data);

/**
* @ingroup hct_mac
* @brief  �ر�˥��ģʽ
*/
#define HI_MDM_NDM_ATTENUATE_DISABLE    0

/**
* @ingroup hct_mac
* @brief  ����˥��ģʽ
*/
#define HI_MDM_NDM_ATTENUATE_ENABLE    1

/**
* @ingroup hct_mac
* @brief  ���ر�̨��ʶ����
*/
#define HI_MDM_NDM2NDM_TF_IDENTIFY_STATUS_DOING      1

/**
* @ingroup hct_mac
* @brief  ���ر�̨��ʶ�����
*/
#define HI_MDM_NDM2NDM_TF_IDENTIFY_STATUS_OVER       0

/**
* @ingroup hct_mac
* @brief  ���ر�̨��ʶ��û��ʶ����������ʶ����
*/
#define HI_MDM_NDM2NDM_TF_IDENTIFY_DOING       0


/**
* @ingroup hct_mac
* @brief  ���ر�̨��ʶ��û��ʶ��ɹ�
*/
#define HI_MDM_NDM2NDM_TF_IDENTIFY_SUCCESS     1

/**
* @ingroup hct_mac
* @brief  ���ر�̨��ʶ��û��ʶ��ʧ��
*/
#define HI_MDM_NDM2NDM_TF_IDENTIFY_FAIL         2

/**
* @ingroup hct_mac
* @brief  ���ջ���λΪ240K
*/
#define NDM_TF_REPORT_LEVEL_240K                0


/**
* @ingroup hct_mac
* @brief  ���ջ���λΪ1M
*/

#define NDM_TF_REPORT_LEVEL_1M                1
/**
 * @ingroup hct_mac
* @brief  ̨��ʶ�����ϱ��ṹ��
*/
typedef struct
{
    HI_U8  ucStatus;                   /**<ʶ��״̬         */
    HI_U8  ucTfResult;                 /**<ʶ����         */
    HI_U8  ucSerialNum;                /**<ʶ�𵽵�̨����         */
    HI_U8  ucPhase;                    /**<ʶ�𵽵���λ         */

    HI_U8  ucLevel;                    /**<���ջ���λ  \n
                                        ȡֵ: #NDM_TF_REPORT_LEVEL_240K ��ʾ240k��#NDM_TF_REPORT_LEVEL_1M  ��ʾ1M��
                                        */
    HI_U8  ulPad[3];                   /**<����λ         */

    HI_U16 wobble_scope;               /**<�Ŷ���ֵ����λus         */
    HI_U16 noise_scope;                /**<ʵʱ������ֵ����λus         */
} HI_MDM_QUERY_NDM2NDM_TF_STATUS_IND_STRU;

/**
* @ingroup  hct_mac
* @brief   ������ճ������ϱ�̨��ʶ�����ݺ������͡�
*
* @par  ����:
*������ճ������ϱ�̨��ʶ�����ݺ������͡�
* @attention �ޡ�
* @param  ndm2ndmtfstatuscallback [IN] ���� #HI_MDM_QUERY_NDM2NDM_TF_STATUS_IND_STRU*�����ر��ϱ���̨��ʶ�����ݡ��r
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see hi_mdm_mac.h �ļ���#HI_MDM_QUERY_NDM2NDM_TF_STATUS_IND_STRU ���塣
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_RegisterQueryNdm2NdmTfStatusCallBack)(HI_MDM_QUERY_NDM2NDM_TF_STATUS_IND_STRU *ndm2ndmtfstatuscallback);

/**
* @ingroup  hct_mac
* @brief   ���崦�����������ϱ��������͡�
*
* @par  ����:
* ���崦�����������ϱ��������͡�
* @attention �ޡ�
* @param  HI_MDM_NET_SEARCH_CONN_IND_S [IN] ���� #HI_MDM_NET_SEARCH_CONN_IND_S*����������ϱ����ݣ����#HI_MDM_NET_SEARCH_CONN_IND_S���塣�r
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see hi_mdm_mac.h �ļ���#HI_MDM_NET_SEARCH_CONN_IND_S ���塣
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_RegisterNdmNetSearchStateCallBack)(HI_MDM_NET_SEARCH_CONN_IND_S *search_data);

/**
* @ingroup  hct_mac
* @brief  ���ó��ر�tei
* @par  ����:
* ���ó��ر�tei�ӿ�
* @attention �ޡ�
* @param    tei [IN] ���� #HI_U32�� ���õĳ��ر�teiֵ��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ�趨�ɹ���\n
                              #HI_ERR_INVALID_PARAMETER ��ʾ�·��������� \n
                              #HI_ERR_NOT_SUPPORT��ʾ���ǳ��ر���Ʒ�� \n
                              #HI_ERR_FAILURE��ʾ��������
* @par Dependency:
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetNdmTei(HI_IN HI_U32 tei);

/**
* @ingroup  hct_mac
* @brief  ��ȡ���ر�tei
* @par  ����:
* ��ȡ���ر�tei�ӿ�
* @attention �ޡ�
* @param    tei [OUT] ���� #HI_U32 *�� ���ر���teiֵ��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ�趨�ɹ���\n
                              #HI_ERR_INVALID_PARAMETER ��ʾ�·��������� \n
                              #HI_ERR_NOT_SUPPORT��ʾ���ǳ��ر���Ʒ�� \n
                              #HI_ERR_FAILURE��ʾ��������
* @par Dependency:
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetNdmTei(HI_IN HI_U32 *tei);
/**
* @ingroup  hct_mac
* @brief  ���ó��ر�˥��ģʽ
* @par  ����:
* ���ó��ر�˥��ģʽ
* @attention �ޡ�
* @param    attenuate_mode [IN] ���� #HI_U8������˥��ģʽֵ
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ�趨�ɹ���\n
                              #HI_ERR_INVALID_PARAMETER ��ʾ�·��������� \n
                              #HI_ERR_NOT_SUPPORT��ʾ���ǳ��ر���Ʒ \n
                              #HI_ERR_FAILURE��ʾ��������
* @par Dependency:
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetNdmAttenuate (HI_IN HI_U8 attenuate_mode);
/**
* @ingroup  hct_mac
* @brief  ��ȡ���ر�˥��ģʽ
* @par  ����:
* ��ȡ���ر�˥��ģʽ
* @attention �ޡ�
* @param    attenuate_mode [OUT] ���� #HI_U8 *����ȡ��˥��ģʽ
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ��ȡ�ɹ���\n
                              #HI_ERR_INVALID_PARAMETER ��ʾ�·��������� \n
                              #HI_ERR_NOT_SUPPORT��ʾ���ǳ��ر���Ʒ \n
                              #HI_ERR_FAILURE��ʾ��������
* @par Dependency:
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetNdmAttenuate (HI_IN HI_U8 *attenuate_mode);
/**
* @ingroup  hct_mac
* @brief  �ϱ�̨��ʶ������ע�ắ���ӿ�
* @par  ����:
* �ϱ�̨��ʶ������ע�ắ���ӿ�
* @attention �ޡ�
* @param    func [IN] ���� #HI_MDM_RegisterQueryNdm2NdmTfStatusCallBack��ע��Ļص�����
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾע��ɹ���\n
                              #HI_ERR_INVALID_PARAMETER ��ʾ�·��������� \n
* @par Dependency:
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterQueryNdm2NdmTfStatus(HI_MDM_RegisterQueryNdm2NdmTfStatusCallBack func);
/**
* @ingroup  hct_mac
* @brief  �ϱ�����վ��ע��ص������ӿ�
* @par  ����:
* �ϱ�����վ��ע��ص������ӿ�
* @attention �ޡ�
* @param    func [IN] ���� #HI_MDM_RegisterNdmNetSearchStateCallBack��ע��ص�����
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ��ȡ�ɹ���\n
                              #HI_ERR_INVALID_PARAMETER ��ʾ�·��������� \n
* @par Dependency:
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterQueryNdmSearchData(HI_MDM_RegisterNdmNetSearchStateCallBack func);

/**
* @ingroup  hct_mac
* @brief  ���ó����������Ƶ�Σ�����ţ�Э��
* @par  ����:
* ���ó����������Ƶ�Σ�����ţ�Э��
* @attention �ޡ�
* @param    freq_index [IN] ���� #HI_U8��Ƶ������
* @param    nid [IN] ���� #HI_U32������Э����NID��ȡֵ��Χ16~16777215�� ����Э����ȡֵ��Χ1-15
* @param    ptcl_ver [IN] ���� #HI_U16��Э��汾��
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ���óɹ���\n
                              #HI_ERR_INVALID_PARAMETER ��ʾ����ʧ�� \n
* @par Dependency:
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetNdmAccessInfo(HI_U8 freq_index, HI_U32 nid, HI_U16 ptcl_ver);

/**
* @ingroup  hct_mac
* @brief  �Ƿ��������վ��
*
* @par  ����:
* �Ƿ��������վ��
* @attention �Ƿ��������վ��
* @param   ��
* @retval  ret ���� #HI_BOOL��#HI_TRUE ��ʾΪ,#������ʾ
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetMaxTei(HI_U16 max_tei);
/**
* @ingroup  hct_mac
* @brief  ע��Զ��ͨѶ�ն˽���ģ��ӿ�
*
* @par  ����:
* ע��Զ��ͨѶ�ն˽���ģ��ӿ�
* @attention ���ò�Ʒ����ΪCCO,STA��
* @param   ��
* @retval  ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾ���óɹ���#������ʾ����ʧ��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_LoadSwitchRegister(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ��ȡ�״�������ɵ�ʱ��
*
* @par  ����:
* ��ȡ��һ��CCO�ж�������ɵ�ʱ��
* @attention ���ò�Ʒ����ΪCCO
* @param   ��
* @retval  ret ���� #HI_U32��#������ɵ�ʱ����0xFFFFFFFF��ʾ������δ��ɡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetNetworkFormTime(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ��ȡ����������ʱ��
*
* @par  ����:
* ��ȡ����������ʱ��
* @attention ���ò�Ʒ����ΪCCO
* @param   ��
* @retval  ret ���� #HI_U32��#����������ʱ����0xFFFFFFFF��ʾδ����������
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetStartNetworkFormTime(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ��ȡ���˱���Ĵ���
*
* @par  ����:
* ��ȡ���˱���Ĵ���
* @attention ���ò�Ʒ����ΪCCO
* @param   ��
* @retval  ret ���� #HI_U32��#���˱���Ĵ�����
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetTopoChangeCnt(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ��ȡ·�ɵ�����
*
* @par  ����:
* ��ȡ·�����������ڡ�
* @attention ���ò�Ʒ����ΪCCO
* @param   ��
* @retval  ret ���� #HI_U32��#·���������ڵĳ��ȣ���λ�롣
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U16 HI_MDM_GetRouteCycle(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ��ȡ����վ�㷢�ͷ����б��ĵ�����
*
* @par  ����:
* ��ȡ����վ�㷢�ͷ����б��ĵ�����
* @attention ���ò�Ʒ����ΪCCO
* @param   ��
* @retval  ret ���� #HI_U32��#����վ�㷢�ͷ����б��ĵ����ڣ���λ�롣
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U16 HI_MDM_GetPcoDiscoverPkCycle(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  ��ȡ����վ�㷢�ͷ����б��ĵ�����
*
* @par  ����:
* ��ȡ����վ�㷢�ͷ����б��ĵ�����
* @attention ���ò�Ʒ����ΪCCO
* @param   ��
* @retval  ret ���� #HI_U32��#����վ�㷢�ͷ����б��ĵ����ڣ���λ�롣
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U16 HI_MDM_GetStaDiscoverPkCycle(HI_VOID);

/**
* @ingroup  hct_mac
* @brief   ���������������λʶ������ϱ��������͡�
*
* @par  ����:
*���������������λʶ������ϱ��������͡�
* @attention �ޡ�
* @param  �ޡ�
* @retval   �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @since HiMDL_V200R001C00
*/
typedef HI_VOID (*HI_MDM_RegisterThreeMeterPhaseCheckCallBack)(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  �ϱ��������λ������ע�ắ��
* @par  ����:
* �ϱ������������λ������ע�ắ����
* @attention  ���ò�Ʒ����ΪCCO��
* @param    func [IN] ���� #HI_MDM_RegisterThreeMeterPhaseCheckCallBack��ע��Ļص�����
* @retval   ret ���� #HI_U32��#HI_ERR_SUCCESS ��ʾע��ɹ���\n
                              #HI_ERR_INVALID_PARAMETER ��ʾ�·��������� \n
                              #HI_ERR_NOT_SUPPORT ��ʾ��Ʒ��֧��\n
* @par Dependency:
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_RegisterThreeMeterPhaseCheckResultReportFunc(HI_MDM_RegisterThreeMeterPhaseCheckCallBack func);

/**
* @ingroup hct_mac
* @brief  �����������ģʽ
*/
typedef enum
{
    HI_MDM_CHECK_UNKOWN_THREE_METER_TYPE = 0,         /**< δ��⵽������������ */
    HI_MDM_CHECK_NO_THREE_METER_TYPE,                 /**< ����������Ʒ���� */
    HI_MDM_CHECK_SUPPORT_ONE_LINE_ZERO_CROSS,         /**<֧��һ·�����������Ʒ���� */
    HI_MDM_CHECK_SUPPORT_THREE_LINE_ZERO_CROSS,       /**<֧����·�����������Ʒ ����*/
}HI_MDM_CHECK_THREE_METER_TYPE_MODE;

/**
* @ingroup  hct_mac
* @brief  ��ȡ������Ʒ֧�ֹ�������͡�
*
* @par  ����:
* ��ȡ������Ʒ֧�ֹ�������͡�
* @attention ���ò�Ʒ����Ϊ������Ʒ
* @param   ��
* @retval  ret ���� #HI_U8��#HI_MDM_CHECK_THREE_METER_TYPE_MODE��ȡ��֧�ֹ�������
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetThreeMeterSupportZeroCrossType(HI_VOID);

/**
* @ingroup  hct_mac
* @brief  �жϳ��ر���Ʒ�Ĺ����·�Ƿ�������
*
* @par  ����:
* �жϳ��ر���Ʒ�Ĺ����·�Ƿ�������
* @attention �����ڳ��ر���Ʒ��̬��
* @param   ��
* @retval  ret ���� #HI_BOOL��#HI_TRUE�����·������#HI_FALSE�����·�쳣��
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_GetZeroCrossCirCuitIsNormal(HI_VOID);

/**
* @ingroup hct_mac
* @brief  CCO�����������
*/
typedef enum
{
    HI_MDM_PHASE_CHECK_UNKOWN = 0,                /**< δ��⵽��λ��� */
    HI_MDM_PHASE_CHECK_ABC,                       /**< ��⵽��������ΪABC */
    HI_MDM_PHASE_CHECK_ACB,                       /**< ��⵽��������ΪACB */
    HI_MDM_PHASE_CHECK_LOSS,                      /**< ��⵽���ڶ��� */
    HI_MDM_PHASE_CHECK_SAME,                      /**< ��⵽������ͬ��λ */
    HI_MDM_PHASE_CHECK_ERR,                       /**< ��⵽����������� */
}HI_MDM_CCO_ZERO_CROSS_PHASE_CHECK_RESULT_MODE;

/**
* @ingroup  hct_mac
* @brief  ��ȡCCO��Ʒ��̬����������·�������
*
* @par  ����:
* ��ȡCCO��Ʒ��̬����������·�������
* @attention ������CCO��Ʒ��̬��
* @param   ��
* @retval  ret ���� #HI_U8���μ�HI_MDM_CCO_ZERO_CROSS_PHASE_CHECK_RESULT_MODE���塣
* @par Dependency:
* <ul><li>hi_mdm_mac.h: �ļ�����������·�����ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U8 HI_MDM_GetCcoZeroCrossCirCuitCheckResult(HI_VOID);

HI_END_HEADER
#endif // __HI_MDM_MAC_H__


