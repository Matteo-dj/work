#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_share_task.h>
HI_U32 usr_share_task_init(HI_VOID)
{
    return HI_SHARE_TASKQ_Init();
}   
