
#ifndef __BATTERY_H__
#define __BATTERY_H__
#include <hi_types.h>
#include <hi_mdm_types.h>


typedef enum
{
    HI_BATTERY_MSG_ID_REFRESH_INFO,
    HI_BATTERY_MSG_ID_INIT,
}HI_BATTERY_MSG_ID_E;

HI_U32 battery_init(HI_VOID);
HI_U32 battery_updata_chip_val(HI_BOOL bFirstUpdate);

#endif

