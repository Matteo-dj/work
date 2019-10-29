
#ifndef __HI_LCD_H__
#define __HI_LCD_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

HI_START_HEADER
#define HI_LCD_DISPLAY_FOREVER (0xFFFFFFFF)
#define HI_LCD_DISP_NONE (0x0)
#define HI_LCD_NOT_FLUSH ((HI_CHAR)0xFF)

typedef enum
{
    HI_LCD_POSITION_1 = 0x0, //
    HI_LCD_POSITION_2, //
    HI_LCD_POSITION_3, //
    HI_LCD_POSITION_MAX,
} HI_LCD_POSITION_E;

typedef enum
{
    HI_LCD_FUNC_ID_BATTERY,         //电池电量
    HI_LCD_FUNC_ID_RTC_TIME,         //RTC时间
    HI_LCD_FUNC_ID_TF,              //台区识别仪功能
    HI_LCD_FUNC_ID_DATACOLLECT,     //采数功能、采数进行时错误告警(如卡满)
    HI_LCD_FUNC_ID_DFT_ALERT,       //升级告警(3518)
    HI_LCD_FUNC_ID_CD_ALERT,        //采数告警
    HI_LCD_FUNC_ID_VER_ALERT,       //3518版本不一致告警
    HI_LCD_FUNC_ID_SIGNAL_SEND,     //信号发送
    HI_LCD_FUNC_ID_MAX,
}HI_LCD_FUNC_ID_E;

typedef enum
{
    HI_LCD_DISPLAY_PRI_0 = 0x0, //告警
    HI_LCD_DISPLAY_PRI_1,       //采数功能
    HI_LCD_DISPLAY_PRI_2,       //台区识别仪功能
    HI_LCD_DISPLAY_PRI_3,       //RTC时间
    HI_LCD_DISPLAY_PRI_4,       //电池电量
}HI_LCD_DISPLAY_PRI_E;//优先级，数值越大优先级越高


/* 点阵控制信息结构体 */
typedef struct
{
    HI_CHAR data[HI_LCD_POSITION_MAX];   //数据，要求ascii字符，非有效字符清空显示" "，HI_LCD_NOT_FLUSH表示不刷新字符
    HI_BOOL blink;                      //HI_TRUE：表示显示字符进行闪烁
    HI_LCD_DISPLAY_PRI_E pri;         //显示优先级，后续可调整
    HI_U32  tsec;                       //显示时间，显示生命周期，以秒为单位,HI_LCD_DISPLAY_FOREVER表示一直显示
}HI_LCD_DISPLAY_INFO_S;

#ifdef PRODUCT_CFG_SUPPORT_LCD
//*****************************************************************************
// 函数名称: HI_DRV_LCD_DisplayPrint
// 功能描述: 点阵显示
//
// 参数说明:
//     func_id:功能id
//     lcd_cfg:配置参数
// 返 回 值:
//     HI_ERR_SUCCESS:成功
//     其它错误见hi_errno.h
//
// 调用要求:
//      1.仅支持在任务中调用，不支持在中断中调用
// 调用举例: TODO: ...
//*****************************************************************************
HI_U32 HI_DRV_LCD_DisplayPrint(HI_IN HI_LCD_FUNC_ID_E func_id, HI_IN HI_LCD_DISPLAY_INFO_S *lcd_cfg);

//*****************************************************************************
// 函数名称: HI_DRV_LCD_DisplayDisable
// 功能描述: 禁止显示
//
// 参数说明:
//     func_id:功能id
// 返 回 值:
//     HI_ERR_SUCCESS:成功
//     其它错误见hi_errno.h
//
// 调用要求:
//      1.仅支持在任务中调用，不支持在中断中调用
// 调用举例: TODO: ...
//*****************************************************************************
HI_U32 HI_DRV_LCD_DisplayDisable(HI_IN HI_LCD_FUNC_ID_E func_id);

//*****************************************************************************
// 函数名称: HI_DRV_LCD_Init
// 功能描述: LCD模块初始
//
// 参数说明:
//     
// 返 回 值:
//     
// 调用要求:
// 调用举例: 
//*****************************************************************************
HI_U32 HI_DRV_LCD_Init(HI_VOID);
#else
HI_PRVL HI_U32 HI_DRV_LCD_DisplayPrint(HI_IN HI_LCD_FUNC_ID_E func_id, HI_IN HI_LCD_DISPLAY_INFO_S *lcd_cfg){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DRV_LCD_DisplayDisable(HI_IN HI_LCD_FUNC_ID_E func_id){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DRV_LCD_Init(HI_VOID){return HI_ERR_NOT_SUPPORT;}

#endif
HI_END_HEADER

#endif

