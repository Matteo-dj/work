
/**@defgroup hct_hrtimer �߾���ʱ��
 * @ingroup drivers
 */

#ifndef __HI_MDM_HRTIMER_H__
#define __HI_MDM_HRTIMER_H__

#define HI_FAST_TIMER_MAGIC 0x55667788

struct HI_hrtimer_list_node_TYPE_S
{
        struct HI_hrtimer_list_node_TYPE_S *prv;
        struct HI_hrtimer_list_node_TYPE_S *next;
        HI_VOID *parent;
};//�ڲ�ʹ�õ�����ṹ���ӿ�ʹ�����������

/**
 * @ingroup hct_hrtimer
 * �߾���ʱ�����ò������ռ���ʹ�������룬��ʱ���������������ڲ������޸Ļ��ͷš�
 * �û��ڵ�һ���ҽ���һ��������ʱ��֮ǰ��Ҫmemset�ÿռ�Ϊ0,֮�������޸ĸÿռ�����(�ر��Ƕ�ʱ�������ڼ�)�� 
 */
typedef struct hiHRTimer_TYPE_S
{
	HI_U32 expires;                             /**< ��ʱʱ�䣬��λ:΢��(us) */
	HI_VOID (*func)(HI_U32 arg);                /**< �ص����� */
	HI_U32 data;                                /**< �ص��������� */
	/********* DO NOT UPDATE THESE STAFF *****/
#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM
    HI_U32 attribute;                           /**< �ڲ�ʹ�ã��û���Ҫ���� */
    HI_VOID (*funcLisr)(HI_U32 arg);            /**< �ڲ�ʹ�ã��û���Ҫ���� */
#endif
	HI_U32 remaining_time;                      /**< �ڲ�ʹ�ã��û���Ҫ���� */
	struct HI_hrtimer_list_node_TYPE_S node;    /**< �ڲ�ʹ�ã��û���Ҫ���� */
}HI_HRTIMER_TYPE_S;


/**
* @ingroup  hct_hrtimer
* @brief  ����һ���߾��ȶ�ʱ����
*
* @par ����:
* ����һ���߾��ȶ�ʱ���������ʱ���Ѿ����������øýӿ�ʱ��ֹͣ��ǰ��ʱ������������
* @attention 
* li ��ʱ������������Ϊ100����
* li ���timer����Ϊȫ�ֱ�����
* li ����ʱ���ص�����ִ�����ж������ģ��ص����������򵥣�����ʹ��˯�ߡ��ȴ��ź�����������ȵĺ�����
*
* @param pTimer [IN] ����#HI_HRTIMER_TYPE_S*����ʱ��������
*
* @retval �ޡ�
*
* @par Dependency:
* @li hi_mdm_hrtimer.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
#define HI_MDM_HRTIMER_Start(pTimer)         HI_HRTIMER_Start(pTimer)

/**
* @ingroup  hct_hrtimer
* @brief ָֹͣ���ĸ߾��ȶ�ʱ����
*
* @par ����:
* ָֹͣ���ĸ߾��ȶ�ʱ����������øýӿ�ʱ��ʱ���Ѿ�ֹͣ�����������κ�Ч����
* @attention 
*
* @param pTimer [IN] ����#HI_HRTIMER_TYPE_S*����ʱ��������Ϊ�����ø߾���ʱ�������ȫ�ֱ�����
*
* @retval �ޡ�
*
* @par Dependency:
* @li hi_mdm_hrtimer.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
#define HI_MDM_HRTIMER_Stop(pTimer)         HI_HRTIMER_Stop(pTimer)

/**
* @ingroup  hct_hrtimer
* @brief  �޸ĸ߾��ȶ�ʱ������
*
* @par ����:
* �޸ĸ߾��ȶ�ʱ��������ʹ���û�����ĳ�ʱʱ��expires���ص�����func���ص�����dataֵ���滻timer�ĳ�ʼ����ֵ��
* @attention �ޡ�
*
* @param pTimer [IN] ����#HI_HRTIMER_TYPE_S *�� Ҫ�޸ĵĶ�ʱ��ָ�룬Ϊ�����ø߾���ʱ�������ȫ�ֱ���������Ϊ�ա�
* @param expires [IN] ����#HI_U32�� ��ʱʱ�䣬��λ:΢��(us)��
* @param func [IN] ����#HI_VOID (*func)(HI_U32 arg); �ص�����������Ϊ�ա�
* @param data [IN] ����#HI_U32���ص������Ĳ�����
*
* @retval �ޡ�
*
* @par Dependency:
* @li hi_stdlib.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
#define HI_MDM_HRTIMER_Modify(pTimer, expires, func, data)           HI_HRTIMER_Modify(pTimer, expires, func, data)


#endif