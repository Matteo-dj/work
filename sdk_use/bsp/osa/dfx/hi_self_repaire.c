//*****************************************************************************
//
//                  ��Ȩ���� (C), 2017-2027, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : hi_self_repaire.c
//  �� �� ��   : V1.0
//  ��    ��   : chenghuanhuan/375526
//  ��������   : 2017-11-15
//  ��������   : ʵ��ϵͳ�Իָ�����
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2017-11-15
//    ��    �� : chenghuanhuan/c00375526
//    �޸����� : �����ļ�
//
//*****************************************************************************
#include <hi_sys_selfrepair.h>
#include <hi_mdm_types.h>
#include <hi_stdlib.h>
#include <hi_sal.h >//HI_SYS_RstRegisterNotify
HI_SELF_REPAIRE_STAT_S g_stSelfRepaire= {0,};

HI_U32 timer_exhaust_reset_callback(HI_VOID)
{
    static HI_U32 ret = 0;
    if (g_stSelfRepaire.is_timer_exhaust == HI_TRUE)
        ret = 1;
    else
        ret++;
    return ret;
}
HI_U32 jobadd_exhaust_reset_callback(HI_VOID)
{
    static HI_U32 ret = 0;
    if (g_stSelfRepaire.is_jobadd_exhaust == HI_TRUE)
        ret = 1;
    else
        ret++;
    return ret;
}
HI_U32 mem_not_enough_reset_callback(HI_VOID)
{
    static HI_U32 ret = 0;
    if (g_stSelfRepaire.is_mem_notEnough == HI_TRUE)
        ret = 1;
    else
        ret++;
    return ret;
}

HI_U32 mempool_err_reset_callback(HI_VOID)
{
    static HI_U32 ret = 0;
    if (g_stSelfRepaire.is_mempoll_check_err == HI_TRUE)
        ret = 1;
    else
        ret++;
    return ret;
}

HI_U32 HI_MDM_SelfRepair_Init(HI_VOID)
{
    HI_U32 ret = 0;
    memset_s(&g_stSelfRepaire,sizeof(HI_SELF_REPAIRE_STAT_S), 0,sizeof(HI_SELF_REPAIRE_STAT_S));
#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
    ret += HI_SYS_RstRegisterNotify(timer_exhaust_reset_callback, HI_SYS_RST_CON_TIMER_EXHAUST);
    ret += HI_SYS_RstRegisterNotify(jobadd_exhaust_reset_callback, HI_SYS_RST_CON_JOBADD_EXHAUST);
    ret += HI_SYS_RstRegisterNotify(mem_not_enough_reset_callback, HI_SYS_RST_CON_MEM_NOT_ENOUGH);
    ret += HI_SYS_RstRegisterNotify(mempool_err_reset_callback, HI_SYS_RST_CON_MEMPOOL_ERR);
#endif
    return ret;
}

