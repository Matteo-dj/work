//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2012, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_io_dev.h
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

#ifndef __MRS_SRV_IO_DEV_H__
#define __MRS_SRV_IO_DEV_H__


#include <hi_driver.h>

/* Ϊ�˼��ݣ���ģ�鲻ʹ�� */
#define FOOTPRINT_GPIO_AREA_ZERO           HI_GPIO_0 //����SET�ܽ�����GPIO��
#define FOOTPRINT_GPIO_AREA_ONE            HI_GPIO_1 //CCO��SET�ܽ�����GPIO��
#define FOOTPRINT_GPIO_AREA_TWO            HI_GPIO_2
#define FOOTPRINT_GPIO_AREA_THREE          HI_GPIO_3
#define FOOTPRINT_GPIO_BIT_ZERO            HI_GPIO_BIT0 //EVENT_OUT�ܽ�����GPIO�ڵ�λ
#define FOOTPRINT_GPIO_BIT_ONE             HI_GPIO_BIT1 //EVENT_OUT�ܽ�����GPIO�ڵ�λ
#define FOOTPRINT_GPIO_BIT_TWO             HI_GPIO_BIT2 //EVENT_OUT�ܽ�����GPIO�ڵ�λ
#define FOOTPRINT_GPIO_BIT_THREE           HI_GPIO_BIT3 //EVENT_OUT�ܽ�����GPIO�ڵ�λ
#define FOOTPRINT_GPIO_BIT_SEVEN           HI_GPIO_BIT7 //����SET�ܽ�����GPIO��
#define FOOTPRINT_GPIO_VALUE_HIGH          HI_GPIO_VALUE1 //�ܽ�Ϊ�ߵ�ƽ
#define FOOTPRINT_GPIO_VALUE_LOW           HI_GPIO_VALUE0 //�ܽ�Ϊ�͵�ƽ
#define FOOTPRINT_SENSE_LEVEL_TYPE         HI_SENSE_LEVEL //��ƽ����
#define FOOTPRINT_SENSE_EDGE_TYPE          HI_SENSE_EDGE  //���ش���
#define FOOTPRINT_RISE_FALL_VALUE_HIGH     HI_EDGE_RISE_LEVEL_HIGH //������Ե��ߵ�ƽ
#define FOOTPRINT_RISE_FALL_VALUE_LOW      HI_EDGE_FALL_LEVEL_LOW //�½���Ե

#endif //__MRS_SRV_IO_DEV_H__


