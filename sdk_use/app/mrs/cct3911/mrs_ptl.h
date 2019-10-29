/**
 ******************************************************************************
 * @file       mrs_ptl.h
 * @brief      协议头文件
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

/* 协议类型 */
typedef enum
{
    PTL_68 = 0,
    PTL_UNKNOW = 0xFF,        /**< 未知 */
} mrs_ptl_type_e;

typedef void (*CHKFRMFUNCPTR)(void *);

/* 协议检测对象结构体 */
typedef struct
{
    HI_U8 ptype;                /**<协议类型*/
    
    HI_U8 in_buf[PTL_INBUF_LEN];  /**< 报文缓存 */
    
    HI_U8 *pin;      /**< 指向 in_buf 首地址*/
    HI_S32 in_len;       /**< 收到报文的长度 */
    
    HI_U8 *pout;     /**< 指向检测到的报文首地址*/
    HI_S32 out_len;      /**< 检测到的报文长度 */

    CHKFRMFUNCPTR cb;   /**< 回调函数 */
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
