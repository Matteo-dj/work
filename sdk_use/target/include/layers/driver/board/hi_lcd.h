
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
    HI_LCD_FUNC_ID_BATTERY,         //��ص���
    HI_LCD_FUNC_ID_RTC_TIME,         //RTCʱ��
    HI_LCD_FUNC_ID_TF,              //̨��ʶ���ǹ���
    HI_LCD_FUNC_ID_DATACOLLECT,     //�������ܡ���������ʱ����澯(�翨��)
    HI_LCD_FUNC_ID_DFT_ALERT,       //�����澯(3518)
    HI_LCD_FUNC_ID_CD_ALERT,        //�����澯
    HI_LCD_FUNC_ID_VER_ALERT,       //3518�汾��һ�¸澯
    HI_LCD_FUNC_ID_SIGNAL_SEND,     //�źŷ���
    HI_LCD_FUNC_ID_MAX,
}HI_LCD_FUNC_ID_E;

typedef enum
{
    HI_LCD_DISPLAY_PRI_0 = 0x0, //�澯
    HI_LCD_DISPLAY_PRI_1,       //��������
    HI_LCD_DISPLAY_PRI_2,       //̨��ʶ���ǹ���
    HI_LCD_DISPLAY_PRI_3,       //RTCʱ��
    HI_LCD_DISPLAY_PRI_4,       //��ص���
}HI_LCD_DISPLAY_PRI_E;//���ȼ�����ֵԽ�����ȼ�Խ��


/* ���������Ϣ�ṹ�� */
typedef struct
{
    HI_CHAR data[HI_LCD_POSITION_MAX];   //���ݣ�Ҫ��ascii�ַ�������Ч�ַ������ʾ" "��HI_LCD_NOT_FLUSH��ʾ��ˢ���ַ�
    HI_BOOL blink;                      //HI_TRUE����ʾ��ʾ�ַ�������˸
    HI_LCD_DISPLAY_PRI_E pri;         //��ʾ���ȼ��������ɵ���
    HI_U32  tsec;                       //��ʾʱ�䣬��ʾ�������ڣ�����Ϊ��λ,HI_LCD_DISPLAY_FOREVER��ʾһֱ��ʾ
}HI_LCD_DISPLAY_INFO_S;

#ifdef PRODUCT_CFG_SUPPORT_LCD
//*****************************************************************************
// ��������: HI_DRV_LCD_DisplayPrint
// ��������: ������ʾ
//
// ����˵��:
//     func_id:����id
//     lcd_cfg:���ò���
// �� �� ֵ:
//     HI_ERR_SUCCESS:�ɹ�
//     ���������hi_errno.h
//
// ����Ҫ��:
//      1.��֧���������е��ã���֧�����ж��е���
// ���þ���: TODO: ...
//*****************************************************************************
HI_U32 HI_DRV_LCD_DisplayPrint(HI_IN HI_LCD_FUNC_ID_E func_id, HI_IN HI_LCD_DISPLAY_INFO_S *lcd_cfg);

//*****************************************************************************
// ��������: HI_DRV_LCD_DisplayDisable
// ��������: ��ֹ��ʾ
//
// ����˵��:
//     func_id:����id
// �� �� ֵ:
//     HI_ERR_SUCCESS:�ɹ�
//     ���������hi_errno.h
//
// ����Ҫ��:
//      1.��֧���������е��ã���֧�����ж��е���
// ���þ���: TODO: ...
//*****************************************************************************
HI_U32 HI_DRV_LCD_DisplayDisable(HI_IN HI_LCD_FUNC_ID_E func_id);

//*****************************************************************************
// ��������: HI_DRV_LCD_Init
// ��������: LCDģ���ʼ
//
// ����˵��:
//     
// �� �� ֵ:
//     
// ����Ҫ��:
// ���þ���: 
//*****************************************************************************
HI_U32 HI_DRV_LCD_Init(HI_VOID);
#else
HI_PRVL HI_U32 HI_DRV_LCD_DisplayPrint(HI_IN HI_LCD_FUNC_ID_E func_id, HI_IN HI_LCD_DISPLAY_INFO_S *lcd_cfg){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DRV_LCD_DisplayDisable(HI_IN HI_LCD_FUNC_ID_E func_id){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DRV_LCD_Init(HI_VOID){return HI_ERR_NOT_SUPPORT;}

#endif
HI_END_HEADER

#endif

