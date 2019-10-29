
#ifndef __HI_PRODUCT_TYPES_H__
#define __HI_PRODUCT_TYPES_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_ft_nv.h>

HI_START_HEADER

#if !defined(PRODUCT_CFG_BUILD_HSO_CODE)
//*****************************************************************************
// ��������: HI_MDM_GetProductType
// ��������: ��ȡģ��Ĳ�Ʒ��̬
//
// ����˵��:
//
//
// �� �� ֵ:
//         ���� hi_ft_nv.h�ж���
//
// ����Ҫ��: ��Ʒ��̬��Դ��0xF03NV���enProductType��Ա��
//*****************************************************************************
HI_EAPI HI_FTM_PRODUCT_TYPE_E HI_MDM_GetProductType(HI_VOID);

//*****************************************************************************
// ��������: HI_MDM_GetHwType
// ��������: ��ȡӲ��ƽ̨����
//
// ����˵��:
//
// �� �� ֵ:
//          ���� hi_ft_nv.h�ж���
//
// ����Ҫ��:
//
//*****************************************************************************
HI_EAPI HI_FTM_HW_PLATFORM_TYPE_E HI_MDM_GetHwType(HI_VOID);

//*****************************************************************************
// ��������: HI_MDM_IsProductXXX
// ��������: �жϲ�Ʒ��̬�Ƿ���XXX
//                      HI_MDM_IsProductCon     �Ƿ�Ϊ�������ز�ģ��
//                      HI_MDM_IsProductRelay       �Ƿ�Ϊ�м���
//                      HI_MDM_IsProductMeter       �Ƿ�Ϊ���
//                      HI_MDM_IsProductMeterI      �Ƿ�ΪI�Ͳɼ���
//                      HI_MDM_IsProductTSta    �Ƿ�Ϊ�����ģ��
//                      HI_MDM_IsProductMeterII          II�Ͳɼ���
//                      HI_MDM_IsProductNdm     �Ƿ�Ϊ������
//
// ����˵��:
//      enProductType[out]��Ʒ��̬
//
// �� �� ֵ:
//          HI_TRUE ��
//          HI_FALSE ����
//
// ����Ҫ��:
//
//*****************************************************************************
HI_EAPI HI_BOOL HI_MDM_IsMeterCheckProduct(HI_VOID);
HI_EAPI HI_BOOL HI_MDM_IsProductCon(HI_FTM_PRODUCT_TYPE_E enProductType);       // �Ƿ�Ϊ�������ز�ģ��
HI_EAPI HI_BOOL HI_MDM_IsProductRelay(HI_FTM_PRODUCT_TYPE_E enProductType);     // �Ƿ�Ϊ�м���
HI_EAPI HI_BOOL HI_MDM_IsProductMeter(HI_FTM_PRODUCT_TYPE_E enProductType);     // �Ƿ�Ϊ���
HI_EAPI HI_BOOL HI_MDM_IsProductMeterI(HI_FTM_PRODUCT_TYPE_E enProductType);        // �Ƿ�ΪI�Ͳɼ���
HI_EAPI HI_BOOL HI_MDM_IsProductTSta(HI_FTM_PRODUCT_TYPE_E enProductType);      // �ж��Ƿ�Ϊ�����ģ��
HI_EAPI HI_BOOL HI_MDM_IsProductMeterII(HI_FTM_PRODUCT_TYPE_E enProductType);   // �Ƿ�Ϊ II�Ͳɼ���
HI_EAPI HI_BOOL HI_MDM_IsProductNdm(HI_FTM_PRODUCT_TYPE_E enProductType);       // �Ƿ�Ϊ������

//*****************************************************************************
// ��������: HI_MDM_IsInvalidProductType
// ��������: �жϲ�Ʒ��̬�Ƿ񲻺Ϸ���
//
// ����˵��:
//      enProductType[out]��Ʒ��̬
//
// �� �� ֵ:
//      HI_TRUE ��
//      HI_FALSE ����
//
// ����Ҫ��:
//
//*****************************************************************************
HI_EAPI HI_BOOL HI_MDM_IsInvalidProductType(HI_FTM_PRODUCT_TYPE_E enProductType);

HI_BOOL HI_MDM_IsUstaProduct(HI_VOID);
#endif

HI_END_HEADER
#endif
