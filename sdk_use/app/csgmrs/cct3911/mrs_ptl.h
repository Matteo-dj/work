/**
 ******************************************************************************
 * @file       mrs_ptl.h
 * @brief      Э��ͷ�ļ�
 * @details    This file including all API functions's declare of mrs_ptl.h.
 * @copyright 
 ******************************************************************************
 */

#ifndef _MRS_PTL_H_
#define _MRS_PTL_H_ 

#include "hi_types.h"

/*-----------------------------------------------------------------------------
 Section: Macro Definitions
 ----------------------------------------------------------------------------*/
#define PTL_INBUF_LEN         (1*512 + 20)
#define FRM_CHK_LEN           512

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/

/* Э������ */
typedef enum
{
    PTL_68 = 0,
    PTL_UNKNOW = 0xFF,        /**< δ֪ */
} mrs_ptl_type_e;

typedef void (*CHKFRMFUNCPTR)(void *);

/* Э�������ṹ�� */
typedef struct
{
    HI_U8 ptype;                /**<Э������*/
    
    HI_U8 in_buf[PTL_INBUF_LEN];  /**< ���Ļ��� */
    
    HI_U8 *pin;      /**< ָ�� in_buf �׵�ַ*/
    HI_S32 in_len;       /**< �յ����ĵĳ��� */
    
    HI_U8 *pout;     /**< ָ���⵽�ı����׵�ַ*/
    HI_S32 out_len;      /**< ��⵽�ı��ĳ��� */

    CHKFRMFUNCPTR cb;   /**< �ص����� */
} mrs_ptl_chk_t;

/*-----------------------------------------------------------------------------
 Section: Function Definitions
 ----------------------------------------------------------------------------*/
HI_S32 mrs_ptl_chkfrm(mrs_ptl_chk_t *pchk);

void*
mrs_ptl_chkfrm_init(HI_U8 ptype, void *ptl_chk, 
    CHKFRMFUNCPTR cb);

#endif /* _MRS_PTL_H_ */

/*----------------------------End of mrs_ptl.h-------------------------------*/
