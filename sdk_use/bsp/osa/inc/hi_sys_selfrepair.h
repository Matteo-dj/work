
#ifndef __HI_SYS_SELF_REPAIRE_H__
#define __HI_SYS_SELF_REPAIRE_H__
#include <hi_types.h>

/*���ӿ�����ϵͳ�����ͳ��*/
typedef struct
{
    HI_BOOL is_jobadd_exhaust; /*�жϺ�����Դ�ľ�*/
    HI_BOOL is_timer_exhaust; /*TIMER��Դ�ľ�*/
    HI_BOOL is_mem_notEnough; /*�ڴ治��*/
    HI_BOOL is_mempoll_check_err; /*��̬�ڴ�ؼ�����,��δ��*/
}HI_SELF_REPAIRE_STAT_S;

HI_EXTERN HI_SELF_REPAIRE_STAT_S g_stSelfRepaire;

#endif

