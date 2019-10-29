
#ifndef __HI_KEY_H__
#define __HI_KEY_H__
HI_START_HEADER

#include <hi_types.h>
#include <hi_mdm_types.h>

//*****************************************************************************
//bKeyPressing:HI_TRUE 按键按下 HI_FALSE:按键抬起
//ms:按键按下抬起时间
//usrData:回调数据
//*****************************************************************************
typedef HI_VOID (*key_up_down_func)(HI_BOOL bKeyPressing,HI_U32 ms,HI_U32 usrData);

typedef enum
{
    HI_DRV_KEY_NUM_0 = 0,
    HI_DRV_KEY_NUM_1,
    HI_DRV_KEY_NUM_2,
    HI_DRV_KEY_NUM_3,
    HI_DRV_KEY_NUM_4,
    HI_DRV_KEY_NUM_5,
    HI_DRV_KEY_NUM_6,
    HI_DRV_KEY_NUM_7,
    HI_DRV_KEY_NUM_8,
    HI_DRV_KEY_NUM_9,
    HI_DRV_KEY_NUM_10,
    HI_DRV_KEY_NUM_11,
    HI_DRV_KEY_NUM_MAX,
} HI_DRV_KEY_NUM_E;

#define HI_DRV_KEY_PRESS_TIME_PRECISION_MS 200//按键时间精度

#ifdef PRODUCT_CFG_SUPPORT_KEY

//*****************************************************************************
// 函数名称: HI_DRV_KEY_UnRegister
// 功能描述: 按键去注册
//
// 参数说明:
//     keyId[in]:按键id
//
// 返 回 值:
//     HI_ERR_SUCCESS:去注册成功
//     其它:去注册失败
// 调用要求:
// 调用举例:
//*****************************************************************************
HI_U32 HI_DRV_KEY_UnRegister(HI_DRV_KEY_NUM_E keyId);

//*****************************************************************************
// 函数名称: HI_DRV_KEY_Register
// 功能描述: 按键注册
//
// 参数说明:
//     keyId[in]:按键id
//     keyFunc[in]:按键回调函数
//     usrData[in]:回调用户数据
//
// 返 回 值:
//     HI_ERR_SUCCESS:注册成功
//     其它:注册失败
// 调用要求:
// 调用举例:
//*****************************************************************************
HI_U32 HI_DRV_KEY_Register(HI_DRV_KEY_NUM_E keyId,key_up_down_func keyFunc,HI_U32 usrData);

//*****************************************************************************
// 函数名称: HI_DRV_KEY_Init
// 功能描述: 按键初始化
//
// 参数说明:
//
// 返 回 值:
//     HI_ERR_SUCCESS:初始化成功
//     其它:初始化失败
// 调用要求:
// 调用举例:
//*****************************************************************************
HI_U32 HI_DRV_KEY_Init(HI_VOID);
#else
HI_PRVL HI_U32 HI_DRV_KEY_UnRegister(HI_DRV_KEY_NUM_E keyId){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DRV_KEY_Register(HI_DRV_KEY_NUM_E keyId,key_up_down_func keyFunc,HI_U32 usrData){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DRV_KEY_Init(HI_VOID){return HI_ERR_NOT_SUPPORT;}
#endif

HI_END_HEADER
#endif

