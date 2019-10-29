#include <boot.h>
#include <product.h>
#include <debug.h>
#include <types.h>
#include "securec.h"

extern unsigned _end;
volatile unsigned alloc_size = 0;
volatile unsigned free_size = 0;

/*malloc重新实现*/
typedef enum hi_boot_malloc_typee
{
    HI_NOTMALLOC    = 0x656D7074,//empt
    HI_HAVEMALLOC   = 0x66756C6C,//full
}HI_BOOT_MALLOC_TYPE_E;

//可用空间总大小由_end和701950000计算
u32 g_boot_malloc_head;
u32 g_boot_malloc_end;
//数据格式: 大小、状态(是否已用)、可用空间
typedef struct hi_boot_malloc_types
{
    u32 ulSize;
    HI_BOOT_MALLOC_TYPE_E ulIsused;
}HI_BOOT_MALLOC_TYPE_S;

/*
** malloc初始化,可用空间以4 字节对齐
** 注意:  本函数内不能增加打印，因其调用位置未进行串口初始化
*/
void boot_malloc_init(void)
{
    HI_BOOT_MALLOC_TYPE_S *stMemPos;

    if (g_boot_malloc_head != (((u32)&_end + (ALIGN4-1))&(~(ALIGN4-1))))
    {
        //创建第一个内存池
        g_boot_malloc_head = ((u32)&_end + (ALIGN4-1))&(~(ALIGN4-1));
        g_boot_malloc_end = (CFG_UPG_DATA_TEMP_ADDR - 2*ALIGN4)&(~(ALIGN4-1));

        stMemPos = (HI_BOOT_MALLOC_TYPE_S *)g_boot_malloc_head;
        stMemPos->ulSize = g_boot_malloc_end - g_boot_malloc_head -sizeof(HI_BOOT_MALLOC_TYPE_S);
        stMemPos->ulIsused = HI_NOTMALLOC;

#ifdef BOOT_MALLOC_MEMSET
        memset_s(g_boot_malloc_head + sizeof(HI_BOOT_MALLOC_TYPE_S), stMemPos->ulSize, 0xee, stMemPos->ulSize);
#endif
    }
}

/*
** 刷新malloc 可分配空间，合并相邻已free 链表
*/
void boot_mem_fresh(void)
{
    HI_BOOT_MALLOC_TYPE_S *stMemPos;
    HI_BOOT_MALLOC_TYPE_S *stNextPos;
    
    //头结点
    stMemPos = (HI_BOOT_MALLOC_TYPE_S *)g_boot_malloc_head;    
    while ((u32)((char*)stMemPos + sizeof(HI_BOOT_MALLOC_TYPE_S) + stMemPos->ulSize) < g_boot_malloc_end)
    {
        //指向下一个区域
        stNextPos = (HI_BOOT_MALLOC_TYPE_S *)((char*)stMemPos + sizeof(HI_BOOT_MALLOC_TYPE_S) + 
            stMemPos->ulSize);
        
        if ((stMemPos->ulIsused == HI_NOTMALLOC)&&(stNextPos->ulIsused == HI_NOTMALLOC))
        {
            stMemPos->ulSize += sizeof(HI_BOOT_MALLOC_TYPE_S) + stNextPos->ulSize;
        }
        else
        {
            stMemPos = stNextPos;
        }
    }
}

/*
** boot malloc 接口
*/
void* boot_malloc(u32 size)
{
    //首地址查询大于等于size且未用空间
    HI_BOOT_MALLOC_TYPE_S *stMemPos;
    HI_BOOT_MALLOC_TYPE_S *stNextPos;
    u32 ulSize;
    u32 ulCount = 0;

    if ((g_boot_malloc_head == 0)||(size == 0))
    {
        return HI_NULL;
    }
    //头结点
    stMemPos = (HI_BOOT_MALLOC_TYPE_S *)g_boot_malloc_head;
    if ((stMemPos->ulIsused != HI_NOTMALLOC)&&(stMemPos->ulIsused != HI_HAVEMALLOC))
    {
        return HI_NULL;
    }
    //分配空间4 字节对齐
    ulSize = (size+ (ALIGN4-1))&(~(ALIGN4-1));

    for (;;)
    {
        if (stMemPos->ulIsused == HI_NOTMALLOC)
        {
            //分配可用块的部分可用空间
            if ((sizeof(HI_BOOT_MALLOC_TYPE_S)+ulSize) < (stMemPos->ulSize))
            {
                //剩余空间自动成为未用块,须大于sizeof(HI_BOOT_MALLOC_TYPE_S)
                stNextPos = (HI_BOOT_MALLOC_TYPE_S*)((char*)stMemPos + ulSize + 
                    sizeof(HI_BOOT_MALLOC_TYPE_S));
                
                stNextPos->ulSize = stMemPos->ulSize - ulSize - sizeof(HI_BOOT_MALLOC_TYPE_S);
                stNextPos->ulIsused = HI_NOTMALLOC;          

                stMemPos->ulSize = ulSize;
                stMemPos->ulIsused = HI_HAVEMALLOC;            

#ifdef BOOT_MALLOC_MEMSET
                memset_s(stMemPos + sizeof(HI_BOOT_MALLOC_TYPE_S), stMemPos->ulSize, 0xee, stMemPos->ulSize);
#endif
                alloc_size += stMemPos->ulSize;
#ifdef BOOT_MEM_DEBUG_ON
                *(unsigned int*)BOOT_MALLOC_SIZE = alloc_size;
#endif
                return (void*)((char*)stMemPos + sizeof(HI_BOOT_MALLOC_TYPE_S));
            }
            //分配整块可用块
            else if ((ulSize <= stMemPos->ulSize)&&((sizeof(HI_BOOT_MALLOC_TYPE_S)+ulSize) >= (stMemPos->ulSize)))
            {
                stMemPos->ulIsused = HI_HAVEMALLOC;
                alloc_size += stMemPos->ulSize;
#ifdef BOOT_MEM_DEBUG_ON
                *(unsigned int*)BOOT_MALLOC_SIZE = alloc_size;
#endif
                return (void*)((char*)stMemPos + sizeof(HI_BOOT_MALLOC_TYPE_S));
            }
#ifdef CMD_TEST_MALLOC
            else
            {
                BOOT_MSG0("have none");
            }
#endif
        }
        
        //当前块不可用，查找下一块
        stNextPos = (HI_BOOT_MALLOC_TYPE_S*)((char*)stMemPos + sizeof(HI_BOOT_MALLOC_TYPE_S) + 
            stMemPos->ulSize);
        stMemPos = stNextPos;
        
        //查询至总空间结尾，失败则合并已free空间再次查询
        if ((u32)stMemPos >=  g_boot_malloc_end)
        {
            if (ulCount >= 1)
            {
                return HI_NULL;
            }

            //BOOT_MSG1("fresh:", (u32)stMemPos);
            boot_mem_fresh();
            stMemPos = (HI_BOOT_MALLOC_TYPE_S *)g_boot_malloc_head;
            ulCount ++;
        }
        if ((stMemPos->ulIsused != HI_HAVEMALLOC)&&(stMemPos->ulIsused != HI_NOTMALLOC))
        {
            //BOOT_MSG1("Wrong Isused:", stMemPos->ulIsused);
            return HI_NULL;
        }
    }
}

/*
** boot free 接口
*/
u32 boot_free(void *addr)
{
    //置链表状态为未用
    HI_BOOT_MALLOC_TYPE_S *stMemPos;

    if (addr == NULL)
        return (u32)BOOT_FALURE;
    
    stMemPos = (HI_BOOT_MALLOC_TYPE_S *)((char*)addr - sizeof(HI_BOOT_MALLOC_TYPE_S));
    if (stMemPos->ulIsused != HI_HAVEMALLOC)
    {
        return (u32)BOOT_FALURE;
    }
    stMemPos->ulIsused = HI_NOTMALLOC;
    
    free_size += stMemPos->ulSize;
#ifdef BOOT_MEM_DEBUG_ON
    *(unsigned int*)(BOOT_FREE_SIZE) = free_size;
#endif

#ifdef BOOT_MALLOC_MEMSET
    memset_s(addr, stMemPos->ulSize, 0xee, stMemPos->ulSize);
#endif
    return BOOT_SUCCESS;
}

#ifdef CMD_TEST_MALLOC
/*
** 测试代码
*/
void print_mem_info(void)
{
    HI_BOOT_MALLOC_TYPE_S *pos = (HI_BOOT_MALLOC_TYPE_S*)g_boot_malloc_head;

    if (g_boot_malloc_head == NULL)
        return ;
    BOOT_MSG0("info:");
    while((void*)pos < g_boot_malloc_end)
    {
        // 输出所有信息
        BOOT_MSG1("addr:",pos);
        BOOT_MSG2("size:", pos->ulSize, pos->ulIsused);
        pos = (HI_BOOT_MALLOC_TYPE_S*)((char*)pos+pos->ulSize + sizeof(HI_BOOT_MALLOC_TYPE_S));
    }
    return;
}
#endif


