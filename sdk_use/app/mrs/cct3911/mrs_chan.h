/**
 ******************************************************************************
 * @file       mrs_chan.h
 * @brief      通道头文件
 * @details    This file including all API functions's declare of mrs_chan.h.
 * @copyright 
 ******************************************************************************
 */

#ifndef _MRS_CHAN_H_
#define _MRS_CHAN_H_ 

#include "hi_types.h"
#include "mrs_ptl.h"

/*-----------------------------------------------------------------------------
 Section: Macro Definitions
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
typedef enum
{
    CHAN_ETH_9G25 = 0, 
    CHAN_LSP_9G25,    
    CHAN_MAX,
} mrs_chan_e;

typedef struct
{
    HI_S32 fd;
    HI_U8 ptype;
    mrs_ptl_chk_t ptl_chk;
    HI_S32 (*ptl_chkfrm)(mrs_ptl_chk_t *pchk);
} mrs_chan_t;

/*-----------------------------------------------------------------------------
 Section: Global Variables
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 Section: Function Prototypes
 ----------------------------------------------------------------------------*/
void mrs_chan_static_clr(HI_U8 chantype);
HI_U32 mrs_chan_static_get(HI_U8 chantype);

extern mrs_chan_t* mrs_chan_ctx(HI_U8 n);

extern void mrs_chan_read_cb(void *ptl_chk);

void mrs_chan_ptl_unpack(HI_U8 chantype);

extern int  
mrs_chan_write(HI_U8 n, int fd, 
    HI_U8* buf, int len);

extern int 
mrs_chan_writes(HI_U8 n, HI_U8 wrnum, int fd, 
    HI_U8* buf, int len);

extern HI_S32 
mrs_lsp_init(void);

extern HI_S32 
mrs_eth_init(void);

#endif /* _MRS_CHAN_H_ */

/*----------------------------End of mrs_chan.h-------------------------------*/
