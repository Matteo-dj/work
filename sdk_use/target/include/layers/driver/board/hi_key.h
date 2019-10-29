
#ifndef __HI_KEY_H__
#define __HI_KEY_H__
HI_START_HEADER

#include <hi_types.h>
#include <hi_mdm_types.h>

//*****************************************************************************
//bKeyPressing:HI_TRUE �������� HI_FALSE:����̧��
//ms:��������̧��ʱ��
//usrData:�ص�����
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

#define HI_DRV_KEY_PRESS_TIME_PRECISION_MS 200//����ʱ�侫��

#ifdef PRODUCT_CFG_SUPPORT_KEY

//*****************************************************************************
// ��������: HI_DRV_KEY_UnRegister
// ��������: ����ȥע��
//
// ����˵��:
//     keyId[in]:����id
//
// �� �� ֵ:
//     HI_ERR_SUCCESS:ȥע��ɹ�
//     ����:ȥע��ʧ��
// ����Ҫ��:
// ���þ���:
//*****************************************************************************
HI_U32 HI_DRV_KEY_UnRegister(HI_DRV_KEY_NUM_E keyId);

//*****************************************************************************
// ��������: HI_DRV_KEY_Register
// ��������: ����ע��
//
// ����˵��:
//     keyId[in]:����id
//     keyFunc[in]:�����ص�����
//     usrData[in]:�ص��û�����
//
// �� �� ֵ:
//     HI_ERR_SUCCESS:ע��ɹ�
//     ����:ע��ʧ��
// ����Ҫ��:
// ���þ���:
//*****************************************************************************
HI_U32 HI_DRV_KEY_Register(HI_DRV_KEY_NUM_E keyId,key_up_down_func keyFunc,HI_U32 usrData);

//*****************************************************************************
// ��������: HI_DRV_KEY_Init
// ��������: ������ʼ��
//
// ����˵��:
//
// �� �� ֵ:
//     HI_ERR_SUCCESS:��ʼ���ɹ�
//     ����:��ʼ��ʧ��
// ����Ҫ��:
// ���þ���:
//*****************************************************************************
HI_U32 HI_DRV_KEY_Init(HI_VOID);
#else
HI_PRVL HI_U32 HI_DRV_KEY_UnRegister(HI_DRV_KEY_NUM_E keyId){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DRV_KEY_Register(HI_DRV_KEY_NUM_E keyId,key_up_down_func keyFunc,HI_U32 usrData){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_DRV_KEY_Init(HI_VOID){return HI_ERR_NOT_SUPPORT;}
#endif

HI_END_HEADER
#endif

