//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2012, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_io.h
//  �� �� ��   : V2.0
//  ��    ��   : fengxiaomin/00209182
//  ��������   : 2012-07-17
//  ��������   : TODO: ...
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� :
//    ��    �� :
//    �޸����� : �����ļ�
//
//  2.��    �� : 2013��04��12��
//    ��    �� : cuiate/00233580
//    �޸����� : �ع�
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   :
// SUBSYSTEM :
// MODULE    :
// OWNER     :
//*****************************************************************************

#ifndef __MRS_SRV_IO_H__
#define __MRS_SRV_IO_H__

HI_START_HEADER


#include <hi_driver.h>


/* CCO RESET PIN : GPIO16 */
#define MRS_CCO_IO_RESET_INDEX      (HI_GPIO_2)
#define MRS_CCO_IO_RESET_BIT        (HI_GPIO_BIT0)

/* CCO SET PIN : GPIO8 */
#define MRS_CCO_IO_SET_INDEX        (HI_GPIO_1)
#define MRS_CCO_IO_SET_BIT          (HI_GPIO_BIT0)

/* STA EVENT OUT PIN : GPIO15 */ //GPIO10
#define MRS_STA_IO_EVENT_OUT_INDEX  (HI_GPIO_1)
#define MRS_STA_IO_EVENT_OUT_BIT    (HI_GPIO_BIT2)

/* STA SET PIN : GPI15 */
#define MRS_STA_IO_SET_INDEX        (HI_GPIO_1)
#define MRS_STA_IO_SET_BIT          (HI_GPIO_BIT7)

/* STA RESET PIN : GPIO9 */
#define MRS_STA_IO_RESET_INDEX      (HI_GPIO_1)
#define MRS_STA_IO_RESET_BIT        (HI_GPIO_BIT1)

/* STA ON SITE PIN : GPIO26 */
#define MRS_STA_IO_ON_SITE_INDEX      (HI_GPIO_3)
#define MRS_STA_IO_ON_SITE_BIT        (HI_GPIO_BIT2)


#define MRS_IO_INDEX_OFFSET_SIZE    (8)

typedef HI_U32 (*mrsCallBack)(HI_U32 ulParam, HI_BOOL bEvtOut);
typedef HI_U32 (*mrsResetCallBack)(HI_U32 ulParam);


// LEDָʾ�ƶ���
typedef enum
{
    EN_MRS_LED_PLCRX = 0,   // ���յ�
    EN_MRS_LED_PLCTX        // ���͵�
}MRS_LED_TYPE_ENUM;


typedef enum
{
    EN_MRS_IO_SET = 0,
    EN_MRS_IO_EVENT_OUT,
    EN_MRS_IO_LED,  //��װLED
    EN_MRS_IO_STA,
    EN_MRS_IO_RESET,
    EN_MRS_IO_ON_SITE,
} MRS_IO_TYPE_ENUM;


typedef struct
{
    MRS_IO_TYPE_ENUM       enIOType;       /* IO�ܽ����� */
    HI_GPIO_DATA_S         stData;         /* gpio���� */
    HI_GPIO_INT_CTRL_S     stInterrData;   /* gpio�ж����� */
} MRS_IO_PARAM_STRU;

typedef struct
{
    MRS_IO_PARAM_STRU       setIOInfo;
    MRS_IO_PARAM_STRU       resetIOInfo;
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    MRS_IO_PARAM_STRU       eventOutIOInfo;
#endif
} MRS_IO_CTX_STRU;


/* IO��ʼ�� */
HI_U32 mrsInitIOCtx(HI_VOID);

//ע��ص�����
HI_U32 mrsRegEventOutCallBack(mrsCallBack f, HI_U32 ulParam);

//ע��RESET pin�ص�����
HI_U32 mrsRegResetCallBack(mrsResetCallBack f, HI_U32 ulParam);

// RESET��ʱ����ʱ����
HI_VOID mrsResetTimeOutProc(HI_VOID);

// EVENT_OUT��ʱ����ʱ����
HI_VOID mrsEventTimeOutProc(HI_VOID);

//SET�ܽŴ����������������˺�����05-F1�����б����ã������˵�Ӧ�ñ��ĸ���������,��ʱ���ԣ�
HI_U8 mrsReadFootPinSet(HI_VOID);

HI_U32 mrsEvtOutResetInt(HI_VOID);

HI_END_HEADER

#endif //__SAL_RES_H__


