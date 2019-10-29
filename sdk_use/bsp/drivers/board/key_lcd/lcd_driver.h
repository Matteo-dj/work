
#ifndef __LCD_DRIVER_H__
#define __LCD_DRIVER_H__
#include <hi_lcd.h>

#define LATTICE_LED_ROWS 8 //��������
#define LATTICE_CHIP_ROWS 16
#define LATTICE_LED_FLUSH_TIME 1000//��ʾˢ��ʱ��

typedef enum
{
	HI_LATTICE_BLINK_OFF = 0x0,
	HI_LATTICE_BLINK_FAST,  //��˸����0.5S
    HI_LATTICE_BLINK_NORMAL,//��˸����1S
    HI_LATTICE_BLINK_SLOW,  //��˸����2S
}HI_LATTICE_BLINK_EN;


/* �������ȫ����Ϣ�ṹ�� */
typedef struct _hi_lattice_lcd_cfg_st
{
    HI_LCD_FUNC_ID_E current_id;
    HI_LCD_DISPLAY_INFO_S disp_info[HI_LCD_FUNC_ID_MAX];
    HI_BOOL enable[HI_LCD_FUNC_ID_MAX];
    HI_U8 pad[4];
}HI_LATTICE_LCD_CFG_ST;

/*
 * Key0:����
 * Key1:����
 * Key2-4:̨��
*/
#define HI_KEY_DATA_COLLECT HI_DRV_KEY_NUM_0
#define HI_KEY_BATTERY      HI_DRV_KEY_NUM_1
#define HI_KEY_RESERVE      HI_DRV_KEY_NUM_2
#define HI_KEY_TQ_FUNC1     HI_DRV_KEY_NUM_3
#define HI_KEY_TQ_FUNC2     HI_DRV_KEY_NUM_4
#define HI_KEY_TQ_FUNC3     HI_DRV_KEY_NUM_5
#define HI_KEY_SIGNAL_SEND  HI_DRV_KEY_NUM_8

#define HI_LATTICE_FUNC_ID_MIN HI_LCD_FUNC_ID_BATTERY//��С��ЧID
#define HI_LATTICE_LED_PRI_MIN HI_LCD_DISPLAY_PRI_0
#define HI_LATTICE_SAME_PRI_FLUSH_TIME 3//LATTICE_LED_FLUSH_TIME�ı���,��ʾ��ͬ���ȼ����л�ʱ�䣬��λ��
HI_U32 HI_LCD_ProcessEvt(HI_U32 ulEventBit);
#endif

