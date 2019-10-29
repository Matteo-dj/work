/**
 ******************************************************************************
 * @file       mbuf.h
 * @brief      业务内存块管理
 * @details    This file including all API functions's declare of mbuf.h.
 * @copyright
 ******************************************************************************
 */

#ifndef _MBUF_H_
#define _MBUF_H_

/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include "hi_types.h"

/*-----------------------------------------------------------------------------
 Section: Macro Definitions
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
typedef enum
{
    MBUF_IN_2048 = 0,
    MBUF_IN_1024,
    MBUF_IN_512,
    MBUF_IN_256,
    MBUF_IN_128,

    MBUF_OUT_2048,
    MBUF_OUT_1024,
    MBUF_OUT_512,
    MBUF_OUT_256,
    MBUF_OUT_128,
    MBUF_MAX
} mbuf_e;

typedef HI_U32 mbuf_id_t;

/*-----------------------------------------------------------------------------
 Section: Globals
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Function Prototypes
 ----------------------------------------------------------------------------*/
extern mbuf_id_t
mbuf_create(const char *pname,
        void *pmem,
        HI_S32 blksize,
        HI_S32 nblks);

extern void
mbuf_release(mbuf_id_t mid);

extern void *
mbuf_malloc(mbuf_id_t mid);

extern void
mbuf_free(mbuf_id_t mid,
        void *mem);

extern void mbuf_info(void);

#endif /* _MBUF_H_ */

/*------------------------------End of mbuf.h--------------------------------*/
