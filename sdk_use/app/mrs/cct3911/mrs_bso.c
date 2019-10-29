/**
 ******************************************************************************
 * @file       mrs_bso.c
 * @version   
 * @brief      业务对象处理列表的操作集合
 * @details    各个业务对象的操作主要包含以下几个
 *                1、业务对象的创建
 *                2、业务对象的插入
 *                3、业务对象的删除
 *                4、业务对象的获取
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
 * @brief      重发接口
 * @param[in]  fd ：句柄
 * @param[in]  buf ：发送数据缓冲区
 * @param[in]  len ：发送数据长度
 * @param[in]  wcnt ：最大发送次数
 * @param[in]  tiemout ：超时时间(ms)
 * @param[in]  bso ：业务对象
 * @retval     STATUS  ：OK-成功, ERROR-失败
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

    //判断发送次数是否大于最大次数
    if (bso->wcnt > wcnt)
    {
        bso->t = 0;
        bso->wcnt = 0;
        return ERROR;
    }

    t = HI_MDM_GetMilliSeconds();
    
    //超时重发
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
 * @brief     clear 业务状态

 * @param[in]  bso ：业务对象
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
 * @brief      创建业务对象
 * @param[in]  mrs_bso_type_e type  ：业务对象类型
 * @retval     mrs_bso_t*  ：返回的 业务对象
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
 * @brief      在业务对象处理列表里插入业务对象
 * @param[in]  mrs_bso_t *mrs_bso  ：需要插入的业务对象
 * @retval     STATUS  ：是否插入成功(OK-成功, ERROR-失败)
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
 * @brief      在业务对象处理列表里删除业务对象
 * @param[in]  mrs_bso_t *mrs_bso  ：需要删除的业务对象
 * @retval     STATUS  ：是否删除成功(OK-成功, ERROR-失败)
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
 * @brief       在业务对象处理列表里查找业务对象是否存在
 * @param[in]   HI_U8 i  ：需要查找的业务对象优先级
 * @param[out]  NONE
 * @retval      mrs_bso_t*  ：查找到的业务对象
 *
 * @details     若返回的业务对象为空,则表示不存在
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