/**
 ******************************************************************************
 * @file      mbuf.c
 * @brief     内存块管理
 * @details   This file including all API functions's implement of mbuf.c.
 * @copyright
 ******************************************************************************
 */
 
/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined (PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_CCT3911)

#include <stdio.h>
#include <stdlib.h>

#include "hi_mdm_task.h"
#include "securec.h"
#include "mrs_misc.h"
#include "mbuf.h"

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
struct stats_mem
{
    HI_U16 avail;
    HI_U16 used;
    HI_U16 max;
    HI_U16 err;
};

struct memp
{
    struct memp *next;
};

/** 外部内存块管理 */
typedef struct _exmem_
{
    struct _exmem_ *next;
    struct stats_mem stats;
    struct memp *memp_tab;
    const char *pname;
    void *pmem;
    HI_S32 blksize;
    HI_S32 nblks;
} exmem_t;

/*-----------------------------------------------------------------------------
 Section: Constant Definitions
 ----------------------------------------------------------------------------*/
#define STATS_INC(x) ++x
#define STATS_DEC(x) --x
#define STATS_INC_USED(x, y)        do { x.used += y; \
                                        if (x.max < x.used) { \
                                            x.max = x.used; \
                                        } \
                                    } while(0)

#ifndef MEM_ALIGNMENT
#define MEM_ALIGNMENT               4
#define MEM_ALIGN_SIZE(size)    (((size) + MEM_ALIGNMENT - 1) \
                                    & ~(MEM_ALIGNMENT-1))
#define MEM_ALIGN(addr)         ((void *)(((HI_U32)(addr) + MEM_ALIGNMENT - 1) \
                                    & ~(HI_U32)(MEM_ALIGNMENT-1)))
#endif

#ifndef OS_ENTER_CRITICAL
# define OS_ENTER_CRITICAL()	Mrs_Misc_Sem_Wait(MISC_SEM_MBUF, HI_SYS_WAIT_FOREVER)
#endif

#ifndef OS_EXIT_CRITICAL
# define OS_EXIT_CRITICAL()		Mrs_Misc_Sem_Signal(MISC_SEM_MBUF);
#endif

/*-----------------------------------------------------------------------------
 Section: Global Variables
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Local Variables
 ----------------------------------------------------------------------------*/
/** stats */
static HI_U32 mbuf_buf_used = 0;

static exmem_t *the_exmems = NULL;

/*-----------------------------------------------------------------------------
 Section: Local Function Prototypes
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Global Function Prototypes
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Function Definitions
 ----------------------------------------------------------------------------*/
/**
 ******************************************************************************
 * @brief   获取外部内存块对象
 * @param[in]  mid : 内存块句柄
 *
 * @retval  内存块对象
 ******************************************************************************
 */
exmem_t *
exmem_get(mbuf_id_t mid)
{
    exmem_t *pexmem;

    OS_ENTER_CRITICAL();
    pexmem = the_exmems;
    while (pexmem)
    {
        if ((mbuf_id_t)pexmem == mid)
        {
            OS_EXIT_CRITICAL();
            return pexmem;
        }
        pexmem = pexmem->next;
    }
    OS_EXIT_CRITICAL();

    return NULL;
}

/**
 ******************************************************************************
 * @brief   创建内存块
 * @param[in]  *pname: 名称
 * @param[in]  *pmem : 内存地址
 * @param[in]  size  : 块大小
 * @param[in]  blocks: 块数量
 *
 * @retval  内存块句柄
 ******************************************************************************
 */
mbuf_id_t
mbuf_create(const char *pname,
        void *pmem,
        HI_S32 blksize,
        HI_S32 nblks)
{
    HI_S32 i;
    struct memp *memp;
    exmem_t *pexmem = malloc(sizeof(exmem_t));

    do
    {
        if (!pexmem)
        {
            break;
        }
        (hi_void)memset_s(pexmem, sizeof(exmem_t), 0x00, sizeof(exmem_t));
        pexmem->pname = pname;
        pexmem->pmem = pmem;
        pexmem->blksize = blksize;
        pexmem->nblks = nblks;
        pexmem->stats.avail = nblks;
        memp = (struct memp *)pmem;
        pexmem->memp_tab = NULL;
        for (i = 0; i < nblks; i++)
        {
            memp->next = pexmem->memp_tab;
            pexmem->memp_tab = memp;
            memp = (struct memp *)(void *)((HI_U8 *)memp + blksize);
        }
        OS_ENTER_CRITICAL();
        pexmem->next = the_exmems;
        the_exmems = pexmem;
        OS_EXIT_CRITICAL();
    } while (0);

    return (mbuf_id_t)pexmem;
}

/**
 ******************************************************************************
 * @brief   释放用户内存块管理
 * @param[in]  mid : 块类型
 *
 * @return  None
 ******************************************************************************
 */
void
mbuf_release(mbuf_id_t mid)
{
	(void)mid;
    //todo:
}

/**
 ******************************************************************************
 * @brief   申请外部内存块
 * @param[in]  mid : 块类型
 *
 * @retval  NULL : 申请失败
 * @retval !NULL : 内存块地址
 ******************************************************************************
 */
static void *
mbuf_malloc_ex(mbuf_id_t mid)
{
    exmem_t *pexmem;
    struct memp *memp = NULL;

    if ((pexmem = exmem_get(mid)))
    {
        OS_ENTER_CRITICAL();
        memp = pexmem->memp_tab;

        if (((HI_U32)memp - (HI_U32)pexmem->pmem)
                < (HI_U32)(pexmem->blksize * pexmem->nblks))
        {
            pexmem->memp_tab = memp->next;

            pexmem->stats.used += 1;
            if (pexmem->stats.max < pexmem->stats.used)
            {
                pexmem->stats.max = pexmem->stats.used;
            }

            memp = (struct memp*)(void *)((HI_U8*)memp);
            mbuf_buf_used ++;
        }
        else
        {
            memp = NULL;
            pexmem->stats.err++;
        }

        OS_EXIT_CRITICAL();
    }

    return memp;
}

/**
 ******************************************************************************
 * @brief   释放外部内存块
 * @param[in]  mid : 块类型
 * @param[in]  *p  : 待释放内存块地址
 *
 * @return  OK    : 成功
 * @return  ERROR : 失败
 ******************************************************************************
 */
int 
mbuf_free_ex(mbuf_id_t mid,
        void *p)
{
    exmem_t *pexmem;
    struct memp *memp;

    if ((pexmem = exmem_get(mid)))
    {
        memp = (struct memp *)(void *)((HI_U8*)p);

        if (((HI_U32)memp - (HI_U32)pexmem->pmem)
                < (HI_U32)(pexmem->blksize * pexmem->nblks))
        {
            OS_ENTER_CRITICAL();

            pexmem->stats.used--;

            memp->next = pexmem->memp_tab;
            pexmem->memp_tab = memp;

            mbuf_buf_used--;
            OS_EXIT_CRITICAL();
            return OK;
        }
    }

    return ERROR;
}

/**
 ******************************************************************************
 * @brief   申请内存块
 * @param[in]  mid : 块类型
 *
 * @retval  NULL : 申请失败
 * @retval !NULL : 内存块地址
 ******************************************************************************
 */
void *
mbuf_malloc(mbuf_id_t mid)
{
    if (mid >= MBUF_MAX)
    {
        return mbuf_malloc_ex(mid);
    }

    return NULL;
}

/**
 ******************************************************************************
 * @brief   释放内存块
 * @param[in]  mid : 块类型
 * @param[in]  *p  : 待释放内存块地址
 *
 * @return  None
 ******************************************************************************
 */
void
mbuf_free(mbuf_id_t mid,
        void *p)
{
    if (OK == mbuf_free_ex(mid, p))
    {
        return;
    }

    printf("mbuf_free_num = %d,%d\n", mbuf_buf_used, mid);
    return;
}

/**
 ******************************************************************************
 * @brief   显示指定内存块信息
 * @param[in]  *mem   : 统计信息
 * @param[in]  *pname : 名称
 * @param[in]  sizes  : 块大小
 *
 * @return  None
 ******************************************************************************
 */
static void
stats_display_memp(struct stats_mem *mem,
        const char *pname,
        HI_U16 sizes)
{
    printf("%-14s %5u   %3u  %3u  %3u  %3u\n", pname, sizes,
        mem->avail, mem->used, mem->max, mem->err);
}

/**
 ******************************************************************************
 * @brief   输出memp信息
 * @return  None
 ******************************************************************************
 */
void mbuf_info(void)
{
    exmem_t *pexmem;

    printf("  %-12s totle avail used  max  err\n"
           "-----------------------------------------\n", "name");

    pexmem = the_exmems;
    while (pexmem)
    {
        stats_display_memp(&pexmem->stats, pexmem->pname, pexmem->blksize);
        pexmem = pexmem->next;
    }
}
#endif
/*-------------------------------mbuf.c-----------------------------------*/
