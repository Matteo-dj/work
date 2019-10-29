
#ifndef __KEY_DRIVER_H_
#define __KEY_DRIVER_H_
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_mdm.h>
#include <hi_key.h>


/*
 * KS0 0x40寄存器对应位从高到低分别为K8/K7/K6/K5 /K4 /K3 /K2 /K1
 *     0x41寄存器对应位从高到低分别为 0/ 0/ 0/K13/K12/K11/K10/K9
 * KS1/KS2与KS0位格式相同
 */
#define LATTICE_KEY_MASK (0x1E0)//按键9、8、7、6


typedef struct
{
    key_up_down_func pKeyFunc;
    HI_U32 ulUsrData;
    HI_U32 ulKeyPressMs;
}HI_KEY_DRIVER_ITEM_S;

typedef struct
{
    HI_BOOL bInit;
    HI_U8 pad[3];
    HI_U32 key_press_bit;
    HI_KEY_DRIVER_ITEM_S key_items[HI_DRV_KEY_NUM_MAX];    
    HI_U32 key_press_ms[HI_DRV_KEY_NUM_MAX];
    HI_TIMER_HANDLE_S timerHandle;
}HI_KEY_DRIVER_CTRL_S;

HI_U32 HI_DRV_KEY_ProcessEvt(HI_U32 ulEventBit);
#endif

