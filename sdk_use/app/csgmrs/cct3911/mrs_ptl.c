/**
 ******************************************************************************
 * @file      mrs_ptl.c
 * @brief     协议处理
 * @details   This file including all API functions's implement of mrs_ptl.c.
 * @copyright  
 ******************************************************************************
 */
#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined (PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_CCT3911)

/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "securec.h"

#include "maths.h"
#include "comLib.h"
#include "mrs_ptl.h"

/*-----------------------------------------------------------------------------
 Section: Macro Definitions
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 Section: Global Variables
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Local Variables
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 Section: Local Function Prototypes
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Global Function Prototypes
 ----------------------------------------------------------------------------*/
extern int 
mrs_ptl_chkfrm_68(mrs_ptl_chk_t *pchk);


/*-----------------------------------------------------------------------------
 Section: Function Definitions
 ----------------------------------------------------------------------------*/
/**
 ******************************************************************************
 * @brief      协议解析,检测多种协议
 * @param[in] pchk ：报文对象

 * @retval   根据具体协议处理
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
HI_S32 mrs_ptl_chkfrm(mrs_ptl_chk_t *pchk)
{
    switch(pchk->ptype)
    {
        case PTL_68:
            return mrs_ptl_chkfrm_68(pchk);
            break;
        default:
            break;
    }
    
    return ERROR;
}


/**
 ******************************************************************************
 * @brief      协议解析,检测多种协议
 * @param[in] ptype:   协议类型
 * @param[in] ptl_chk：报文对象
 * @param[in] CHKFRMFUNCPTR cb: 回调函数

 * @retval   报文对象
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
void*
mrs_ptl_chkfrm_init(HI_U8 ptype, void *ptl_chk, 
    CHKFRMFUNCPTR cb)
{
    mrs_ptl_chk_t *pchk = (mrs_ptl_chk_t *)ptl_chk;
    
    (hi_void)memset_s(pchk, sizeof(mrs_ptl_chk_t), 0x00, sizeof(mrs_ptl_chk_t));
    pchk->ptype = ptype;
    pchk->cb = cb;
    pchk->pin = pchk->in_buf;

    return (void *)pchk;
}

/*---------------------------------mrs_ptl.c-------------------------------------*/
#endif