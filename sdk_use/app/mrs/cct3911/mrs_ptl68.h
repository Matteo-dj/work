/**
 ******************************************************************************
 * @file       mrs_ptl68.h
 * @brief      协议头文件
 * @details    This file including all API functions's declare of mrs_ptl68.h.
 * @copyright
 ******************************************************************************
 */

#ifndef _MRS_PTL68_H_
#define _MRS_PTL68_H_ 

#include "hi_types.h"
#include "mrs_ptl.h"

/*-----------------------------------------------------------------------------
 Section: Macro Definitions
 ----------------------------------------------------------------------------*/
#define PTL_HEAD                0x68
#define PTL_END                 0x16

#define PTL_HEAD_SIZE           4
#define PTL_DATA_SIZE           2

#define PTL_CS_SIZE             2
#define PTL_END_SIZE            1
#define PTL_CMDID_SIZE          2       //报文命令长度

#define PTL_DLEN_OFT_HEAD       2
#define PTL_DATA_OFT_HEAD       4
#define PTL_TXT_OFT_HEAD        (PTL_DATA_OFT_HEAD + PTL_CMDID_SIZE) //从报文头开始偏移

#define PTL_LEN(x)              (PTL_HEAD_SIZE + x + PTL_CS_SIZE + PTL_END_SIZE)
#define PTL_MIN_LEN             PTL_LEN(0)

#define PTL_BSOTYPE_GET(x)      (x & 0x3F)

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 Section: Function Definitions
 ----------------------------------------------------------------------------*/
extern HI_U8 
mrs_ptl68_ctrlcode_set(HI_U8 dir,
    HI_U8 flag,
    HI_U8 bsotype);

extern HI_U16   
mrs_ptl68_pack(HI_U8 ctrlcode, 
    HI_U16 cmdId,
    HI_U8* ctxt, 
    HI_U16 clen,
    HI_U8* frm,
    HI_U16 frmlen);

extern int 
mrs_ptl_chkfrm_68(mrs_ptl_chk_t *pchk);

#endif /* _MRS_PTL68_H_ */

/*----------------------------End of mrs_ptl68.h-------------------------------*/
