
#ifndef __HI_MDM_DATACOLLECT__
#define __HI_MDM_DATACOLLECT__
#include <hi_types.h>
#include <hi_mdm_types.h>

/**@defgroup hct_data_collect ����
 * @ingroup mdm
*/

/**
 * @ingroup hct_data_collect
 * �ɼ����ݴ洢������
 */
typedef struct
{
    HI_U32 bufAddr[2]; /**< ��Ϊ��ת���п������������� �����������׵�ַ*/
    HI_U32 bufSize[2]; /**< ���������С*/
}HI_BUF_S;

/**
 * @ingroup hct_data_collect
 * ����������
 */
typedef enum
{
    EN_TRIG_TYPE_FST_PB = 0,            /**< 0 ����һ��PB��������*/
    EN_TRIG_TYPE_SND_PB,                /**< 1 ���ڶ���PB��������*/
    EN_TRIG_TYPE_TRD_PB,                /**< 2 ��������PB��������*/
    EN_TRIG_TYPE_FTH_PB,                /**< 3 �����ĸ�PB��������*/
    EN_TRIG_TYPE_FIV_PB,                /**< 4 �������PB��������*/
    EN_TRIG_TYPE_FC_START,              /**< 5 ��FC��ʼ*/
    EN_TRIG_TYPE_FC_ERR,                /**< 6 ��ʱ��֡β������FC�����*/
    EN_TRIG_TYPE_NOISE,                 /**< 7 : ä��*/
    EN_TRIG_TYPE_STAT_FOUR,             /**< 8 : �ز�����״̬4*/
    EN_TRIG_TYPE_TX_ENABLE_FFT,         /**< 9 ��tx��FFTʹ��*/
    EN_TRIG_TYPE_TX_START_FFT,          /**< 10��tx��FFT��ʼ*/
    EN_TRIG_TYPE_ENABLE_IFFT,           /**< 11�����Ŷ����IFFTʹ��*/
    EN_TRIG_TYPE_START_IFFT,            /**< 12�����Ŷ����IFFT��ʼ*/
    EN_TRIG_TYPE_ENABLE_FFT,            /**< 13��Ƶ������FFTʹ��*/
    EN_TRIG_TYPE_START_FFT,             /**< 14��Ƶ������FFT��ʼ*/
    EN_TRIG_TYPE_PREM_FFT_OUTPUT_START, /**< 15��Preamble��FFT�����ʼ*/
    EN_TRIG_TYPE_RREM_FFT_OUTPUT_END,   /**< 16��Preamble��FFT�������*/
    EN_TRIG_TYPE_PAYLOAD_START,         /**< 17��PayLoad���뿪ʼ*/
    EN_TRIG_TYPE_CTR12FFT_DIN_EN,       /**< 18��ctrl2fft_din_en*/
    EN_TRIG_TYPE_CTR12FFT_DOUT_EN,      /**< 19��ctrl2fft_dout_en*/
    EN_TRIG_TYPE_NBI_FFT_ENABLE,        /**< 20��NBI��FFTʹ��*/
    EN_TRIG_TYPE_NBI_FFT_START,         /**< 21��NBI��FFT��ʼ*/
    EN_TRIG_TYPE_PL_DECODE_FAIL,        /**< 22��pl����ʧ��*/
    EN_TRIG_TYPE_FC_DECODE_FAIL,        /**< 23��fc����ʧ��*/
    EN_TRIG_TYPE_FC_0PPM_DECODE_FAIL,   /**< 24��fc����ʧ�ܣ�0ppm��*/
    EN_TRIG_TYPE_PARA_DETECT_FAIL,      /**<25�����м��ʧ��*/
    EN_TRIG_TYPE_FRAME_END,             /**< 26��frame_end*/
    EN_TRIG_TYPE_FC_START_COUNT,        /**< 27��fc_start�������*/
    EN_TRIG_TYPE_MAX,
    EN_TRIG_TYPE_MASK_DATA=256,         /**< 256������mask����*/
}DC_TRIG_TYPE_ENUM;

/**
 * @ingroup hct_data_collect
 * �������ݣ���ʱ��Ƶ����ǰ����
 */
typedef enum
{
    EN_TRIG_AD_12_BIT=0,               /**< 0 ��12bit AD���*/
    EN_TRIG_ASYNC_PRO_10_BIT=11,       /**< 11��10bit Async_Pro���*/
    EN_TRIG_CUBIC_10_BIT=1,            /**< 1 ��10bit Cubic���*/
    EN_TRIG_IMP_10_BIT=2,              /**< 2 ��10bit Imp���*/
    EN_TRIG_DCOC_10_BIT=3,             /**< 3 ��10bit DCOC���*/
    EN_TRIG_NF_10_BIT=4,               /**< 4 :     10bit NF�˲������*/
    EN_TRIG_DAGC_10_BIT=5,             /**< 5 ��10bit Dagc���*/
    EN_TRIG_CSMA_4_BIT=6,              /**< 6 �� 4bit �ز�����״̬*/
    EN_TRIG_FFT_ORDER_32_BIT=7,        /**< 7 ��32bit FFT˳�����(q��i)*/
    EN_TRIG_FFT_DISORDER_32_BIT=8,     /**< 8 ��32bit FFT�������(q��i)*/
    EN_TRIG_FC_12_BIT=9,               /**< 9 ��12bit FC��������q��i��*/
    EN_TRIG_PAYLOAD_12_BIT=10,         /**< 10��12bit PayLoad��������q��i��*/
    EN_TRIG_TF_10_BIT_AD_DATA=12,      /**< 12��10bit ̨��ʶ��AD����*/
    EN_TRIG_DA_12_BIT=13,              /**< 13��12bit DA���� */
    EN_TRIG_IMP_18_BIT=14,             /**< 14��18bit IMP��� */
    EN_TRIG_FFT_16_BIT=15,             /**< 15��16bit FFT���� */
    EN_TRIG_CS_16_BIT=16,              /**< 16��16bit CS����*/
    EN_TRIG_LOGIC_16_LBIT_LOGIC=17,    /**< 17����16bit �߷�����*/

    // оƬDC_CAPTURE_CTRL enTrigerTypeλ��Ϊ8bit��������Ŀ�ʹ��λ��Ϊ16bit�����Դ�256��ʼ��ʾ��������ģʽ
    EN_TRIG_INCREMENTAL_SEQUENCE=256,  /**< 256���������� ƫ�Ƶ�ַ0x80��Ӧ��Աself_test_mode����Ϊ1*/
    EN_TRIG_DESCENDING_SEQUENCE=257,   /**< 257���ݼ����� ƫ�Ƶ�ַ0x80��Ӧ��Աself_test_mode����Ϊ2*/
    EN_TRIG_LOGIC_32_BIT_LOGIC=258,    /**< 258:      32bit�߷�����*/
    EN_TRIG_LOGIC_16_HBIT_LOGIC=259,   /**< 259:��16bit�߷�����*/

    EN_TRIG_DATA_MAX
}DC_TRIG_DATA_ENUM;

/**
 * @ingroup hct_data_collect
 * �������ò���
 */
typedef struct
{
    HI_U32 ulWaitEnvOkTime;          /**< �ȴ����������ȶ�ʱ��(ms)*/
    HI_U32 ulWaitResultTimeOut;      /**<�ȴ����������ʱʱ��(ms)*/
    HI_U32  ulStartAddr;             /**< �����ڴ���ʼ��ַ,��Ҫ64Byte����*/
    HI_U32  ulEndAddr;               /**< �����ڴ������ַ,��Ҫ64Byte����*/
    HI_S32  lAgcGain;                /**< ��������*/
    HI_U32  ulAfterPointlen;         /**< �����ɼ�ʱ����������ٲɼ������ݳ���*/
    DC_TRIG_TYPE_ENUM  enTrigerType; /**< ����������*/
    DC_TRIG_DATA_ENUM  enDataSrc;    /**< �������ݣ���ʱ��Ƶ����ǰ����*/
}DC_INPUT_PARA_STRU;


/**
* @ingroup  hct_data_collect
* @brief  �������ܳ�ʼ����
*
* @par ����:
* �������ܳ�ʼ����
* @attention �ޡ�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_datacollect.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_DC_Init(HI_VOID);


/**
* @ingroup  hct_data_collect
* @brief  ��ͨ������
*
* @par ����:
* ��ͨ������
* @attention �����ڴ�ռ���ʼ�ͽ�����ַ��Ҫ64Byte���롣
*
* @param  pDcInput [IN] �������õĲ�����
* @param  sBuf [OUT] �洢��������
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_datacollect.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_DC_StartNormalDataCollect(HI_IN DC_INPUT_PARA_STRU *pDcInput,HI_OUT HI_BUF_S *sBuf);

#endif


