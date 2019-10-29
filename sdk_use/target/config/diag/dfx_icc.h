
#ifndef __DFX_ICC_H__
#define __DFX_ICC_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

//内部用统计量
typedef struct
{
    HI_U16 s_create_fail;
    HI_U16 s_connect_fail;
    HI_U16 s_send_fail;
    HI_U16 s_rcv_fail;

    HI_U16 s_set_config_fail;
    HI_U16 s_creat_times;
    HI_U16 s_close_times;
    HI_U16 cmd_rpt_times;

    HI_U32 suc_rcv_bytes;
    HI_U32 suc_snd_bytes;
    HI_U16 last_snd_cmd_id;
    HI_U16 last_snd_ind_id;
}HI_ICC_STAT_S;

typedef struct
{
    HI_BOOL bRemote;
    HI_U8 pad[3];
}HI_DIAG_GET_ICC_STAT_CMD_S;
typedef struct
{
    HI_ICC_STAT_S iccStat;
}HI_DIAG_GET_ICC_STAT_IND_S;

HI_U32 HI_ICC_ReportStat(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
HI_U32 HI_ICC_GetDfxInfo(HI_OUT HI_ICC_STAT_S * pIccStat);

#endif//__DFX_ICC_H__
