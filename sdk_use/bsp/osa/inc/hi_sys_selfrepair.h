
#ifndef __HI_SYS_SELF_REPAIRE_H__
#define __HI_SYS_SELF_REPAIRE_H__
#include <hi_types.h>

/*本接口用于系统自诊断统计*/
typedef struct
{
    HI_BOOL is_jobadd_exhaust; /*中断后半段资源耗尽*/
    HI_BOOL is_timer_exhaust; /*TIMER资源耗尽*/
    HI_BOOL is_mem_notEnough; /*内存不足*/
    HI_BOOL is_mempoll_check_err; /*动态内存池检测错误,暂未打开*/
}HI_SELF_REPAIRE_STAT_S;

HI_EXTERN HI_SELF_REPAIRE_STAT_S g_stSelfRepaire;

#endif

