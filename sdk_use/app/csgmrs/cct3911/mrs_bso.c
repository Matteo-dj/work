/**
 ******************************************************************************
 * @file       mrs_bso.c
 * @version   
 * @brief      ҵ��������б�Ĳ�������
 * @details    ����ҵ�����Ĳ�����Ҫ�������¼���
 *                1��ҵ�����Ĵ���
 *                2��ҵ�����Ĳ���
 *                3��ҵ������ɾ��
 *                4��ҵ�����Ļ�ȡ
 * @copyright  
 ******************************************************************************
 */
#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined (PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_CCT3911)

/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include "hi_mdm_time.h"
#include "memLib.h"
#include "mrs_chan.h"
#include "mrs_cfg.h"

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 Section: Constant Definitions
 ----------------------------------------------------------------------------*/
/* None */

/*-----------------------------------------------------------------------------
 Section: Global Variables
 ----------------------------------------------------------------------------*/
/* None */

/*-----------------------------------------------------------------------------
 Section: Local Variables
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 Section: Local Function Prototypes
 ----------------------------------------------------------------------------*/
extern mrs_bso_t* 
mrs_bsotfquery_getinstanc(mrs_bso_t* mrs_bso);

extern mrs_bso_t* 
mrs_bsocollect_getinstanc(mrs_bso_t* mrs_bso);

extern mrs_bso_t* 
mrs_bsosg_getinstanc(mrs_bso_t* mrs_bso);

extern mrs_bso_t* 
mrs_bsoparam_getinstanc(mrs_bso_t* mrs_bso);

extern mrs_bso_t* 
mrs_bsoreboot_getinstanc(mrs_bso_t* mrs_bso);

extern mrs_bso_t* 
mrs_bsoreport_getinstanc(mrs_bso_t* mrs_bso);

extern mrs_bso_t* 
mrs_bsomisc_getinstanc(mrs_bso_t* mrs_bso);

extern mrs_bso_t* 
mrs_bsobdcollect_getinstanc(mrs_bso_t* mrs_bso);

bso_instance the_bso_instance[] = {
    mrs_bsotfquery_getinstanc,
    mrs_bsocollect_getinstanc,
    mrs_bsosg_getinstanc,
    mrs_bsoparam_getinstanc,
    mrs_bsoreboot_getinstanc,
    mrs_bsoreport_getinstanc,
    mrs_bsomisc_getinstanc,
    mrs_bsobdcollect_getinstanc,
};

/*-----------------------------------------------------------------------------
 Section: Function Definitions
 ----------------------------------------------------------------------------*/
/**
 ******************************************************************************
 * @brief      �ط��ӿ�
 * @param[in]  fd �����
 * @param[in]  buf ���������ݻ�����
 * @param[in]  len ���������ݳ���
 * @param[in]  wcnt ������ʹ���
 * @param[in]  tiemout ����ʱʱ��(ms)
 * @param[in]  bso ��ҵ�����
 * @retval     STATUS  ��OK-�ɹ�, ERROR-ʧ��
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
HI_S32 
mrs_bso_wr(int fd, HI_U8 *buf, int len, 
    HI_U8 wcnt, 
    HI_U32 tiemout, 
    mrs_bso_t *bso)
{
    HI_U32 t;

    //�жϷ��ʹ����Ƿ����������
    if (bso->wcnt > wcnt)
    {
        bso->t = 0;
        bso->wcnt = 0;
        return ERROR;
    }

    t = HI_MDM_GetMilliSeconds();
    
    //��ʱ�ط�
    if ((bso->wcnt == 0) || (abs(t - bso->t) >= tiemout))
    {
        bso->t = t;
        bso->wcnt ++;

        if (bso->wcnt <= wcnt)
        {
            mrs_chan_write(bso->chantype, fd, buf, len);
        }
    }

    return OK;
}

/**
 ******************************************************************************
 * @brief     clear ҵ��״̬

 * @param[in]  bso ��ҵ�����
 * @retval    None
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
void mrs_bso_clr(mrs_bso_t *mrs_bso)
{
    if ((mrs_bso->run_state == 0) && (mrs_bso->proc_state == 0) 
        && (mrs_bso->pbuf == NULL))
    {
        return;
    }
    
    mrs_bso->run_state = 0;
    mrs_bso->proc_state = 0;
    if (mrs_bso->pbuf)
    {
        mlib_free(mrs_bso->pbuf);
        mrs_bso->pbuf = NULL;
    }

    if (mrs_bso->bsotype != BSO_TYPE_MISC)
    {
        printf("mrs_bso[%d][%d]: state clear\n", 
            mrs_bso->chantype, mrs_bso->bsotype);
    }
}

/**
 ******************************************************************************
 * @brief      ����ҵ�����
 * @param[in]  mrs_bso_type_e type  ��ҵ���������
 * @retval     mrs_bso_t*  �����ص� ҵ�����
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
mrs_bso_t*
mrs_bso_instance(mrs_bso_type_e type, 
	mrs_bso_t *mrs_bso)
{
    if (type < BSO_TYPE_MAX)
    {
        return the_bso_instance[type](mrs_bso);
    }

    return NULL;
}

/**
 ******************************************************************************
 * @brief      ��ҵ��������б������ҵ�����
 * @param[in]  mrs_bso_t *mrs_bso  ����Ҫ�����ҵ�����
 * @retval     STATUS  ���Ƿ����ɹ�(OK-�ɹ�, ERROR-ʧ��)
 *
 * @details    
 *
 * @note
 ******************************************************************************
 */
HI_S32
mrs_bsolist_ins(mrs_bso_t *mrs_bsolist[], 
	mrs_bso_t *mrs_bso)
{
    if (mrs_bso != NULL)
    {
        if (mrs_bso->bsotype < BSO_MAX_NUM)
        {
            mrs_bsolist[mrs_bso->bsotype] = mrs_bso;
            return OK;
        }
    }
    return ERROR;
}

/**
 ******************************************************************************
 * @brief      ��ҵ��������б���ɾ��ҵ�����
 * @param[in]  mrs_bso_t *mrs_bso  ����Ҫɾ����ҵ�����
 * @retval     STATUS  ���Ƿ�ɾ���ɹ�(OK-�ɹ�, ERROR-ʧ��)
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
void
mrs_bsolist_del(mrs_bso_t *mrs_bsolist[], 
	mrs_bso_t *mrs_bso)
{
    if (mrs_bso != NULL)
    {
        if (mrs_bso->bsotype < BSO_MAX_NUM)
        {
            mrs_bsolist[mrs_bso->bsotype] = NULL;
        }
    }
    return;
}

/**
 ******************************************************************************
 * @brief       ��ҵ��������б������ҵ������Ƿ����
 * @param[in]   HI_U8 i  ����Ҫ���ҵ�ҵ��������ȼ�
 * @param[out]  NONE
 * @retval      mrs_bso_t*  �����ҵ���ҵ�����
 *
 * @details     �����ص�ҵ�����Ϊ��,���ʾ������
 *
 * @note
 ******************************************************************************
 */
mrs_bso_t*
mrs_bsolist_get(mrs_bso_t *mrs_bsolist[], 
	HI_U8 i)
{
    if (i < BSO_MAX_NUM)
    {
        return mrs_bsolist[i];
    }
    return NULL;
}

/*-------------------------------End of mrs_bso.c--------------------------------*/
#endif