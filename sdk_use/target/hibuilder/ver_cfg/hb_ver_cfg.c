//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : hb_ver_cfg.c
//  �� �� ��   : V1.0
//  ��    ��   :  chenghuanhuan/00132816
//  ��������   : 2016-06-07
//  ��������   :  �汾���û�ͨ��HB�������룬����ͷ�ļ� hb_ver_cfg.h��
//				���ļ���������ʱ���ã�����Ϊ�������ʹ�õ�ȫ�ֱ�����ֵ��
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2016-06-07
//    ��    �� : chenghuanhuan/00132816
//    �޸����� : �����ļ�
//             
//
//*****************************************************************************

#include <hi_types.h>
#include <hb_ver_cfg.h>
#include <hi_mdm_types.h>

#define  PRODUCT_CFG_FILE_VER_MAJOR_MINOR   HI_MAKEU32(PRODUCT_CFG_FILE_VER_MINOR,PRODUCT_CFG_FILE_VER_MAJOR)

HI_CONST HI_PRODUCT_INFO_S g_stPlcModuleProductInfo =
{
    PRODUCT_CFG_VERSION_STR,
    PRODUCT_CFG_FULL_VERSION_STR,
    PRODUCT_CFG_FILE_VERSION_STR,
    PRODUCT_CFG_CHIP_SOLUTION_NAME,
    PRODUCT_CFG_BUILD_DATE,
    PRODUCT_CFG_BUILD_TIME
};
HI_CONST HI_UE_SOFT_VERSION_S g_stSoftVer =
{
    PRODUCT_CFG_FILE_VER_MAJOR_MINOR, // ���汾��.�ΰ汾��
    PRODUCT_CFG_FILE_VER_REVISION,   // �����汾��
    0      // �ڲ��汾��
};
HI_CONST HI_UE_PRODUCT_VER_S g_stProductVer =
{
    PRODUCT_CFG_VERSION_V,           // V����
    PRODUCT_CFG_VERSION_R,           // R����
    PRODUCT_CFG_VERSION_C,           // C����
    PRODUCT_CFG_VERSION_B,           // B����
    PRODUCT_CFG_VERSION_SPC,            // SPC����
    0,    // Ӳ��PCB�ź�ӡ�ư�汾��
    0        // ��Ʒ���ͱ�ţ�����ͬ������ϵ�Ӳ��ƽ̨
};
 HI_CHAR* creationDate = PRODUCT_CFG_BUILD_DATE;

