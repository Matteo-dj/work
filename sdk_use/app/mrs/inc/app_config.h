//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : app_config.h
//  �� �� ��   : V1.0
//  ��    ��   : ����/00130424
//  ��������   : 2011��09��06��
//  ��������   : APPȫ�����ö����ļ�
//  �����б�   :
//  �޸���ʷ   :
//  1.��    �� : 2011��09��06��
//    ��    �� : ����/00130424
//    �޸����� : �����ļ�
//
//*****************************************************************************

#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

//Matteo
//#define SIGNAL_MCU




//*****************************************************************************
// APP ģ�� �汾���� [ע��] �汾�����ֲ��ܼ����ţ�ʹ��ʱ��#x"."#y"."#z����ʽ
//*****************************************************************************
#define VERSION_MAIN        1 //���߰汾
#define VERSION_FEATURE     3 //���԰汾
#define VERSION_BUG         7 //�ش�BUG�޶���ż��Ϊ�ȶ��汾������Ϊ�����汾

//////////////////////////////////////////////////////////////////////////
// APP ģ�� ID ����
//////////////////////////////////////////////////////////////////////////

#define HI_MOD_ID_APP_MRS_CHL    HI_MOD_ID_APP_PRO_DEC
#define HI_MOD_ID_APP_MRS_PROTO  HI_MOD_ID_APP_PRO_DEC
#define HI_MOD_ID_APP_MRS_CCO    HI_MOD_ID_APP_COMMON
#define HI_MOD_ID_APP_MRS_STA    HI_MOD_ID_APP_COMMON
#define HI_MOD_ID_APP_DFX        HI_MOD_ID_APP_COMMON
#define HI_MOD_ID_APP_MRS_MAP    HI_MOD_ID_APP_MAC_MAP
#define HI_MOD_ID_APP_MRS_RM     HI_MOD_ID_APP_LOCAL_MR
#define HI_MOD_ID_APP_MRS_UPG    HI_MOD_ID_APP_UPG


//////////////////////////////////////////////////////////////////////////
// MRS ����
//////////////////////////////////////////////////////////////////////////
#define PRODUCT_CFG_MRS_MAX_TOPO_NUM                 (2048)          // ���ӽڵ�����

#define PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM         (32)            // ���һMAC��Ӧ�ĵ���ַ����
#define PRODUCT_CFG_MRS_MAX_MUSTER_COMM_RENUM        (3)             // ������ͨѶ������Դ���
#define PRODUCT_CFG_STA_INIT_TIME                    (2000)          // ��ʼ���ȴ�ʱ��500����
#define PRODUCT_CFG_STA_HEART_TIME                   (1000 * 60 * 5) // ��������Ϊ5����
#define PRODUCT_CFG_MRS_3762_BUFFER_BYTE_MAX         (2048)

#define PRODUCT_CFG_STA_FRAME_BUF_ITEM_COUNT_MAX     (20)           // STA����frame������(���)
#define PRODUCT_CFG_STA_FRAME_BUF_ITEM_COUNT_DEFAULT (10)           // STA����frame������(Ĭ��)
#define PRODUCT_CFG_STA_FRAME_BUF_ITEM_COUNT_MIN     (1)            // STA����frame������(��С)

#define PRODUCT_CFG_STA_FRAME_BUF_TIMEOUT_MAX        (255)          // STA����frame����Ч��(���) ��λ:��
#define PRODUCT_CFG_STA_FRAME_BUF_TIMEOUT_DEFAULT    (10)           // STA����frame����Ч��(Ĭ��) ��λ:��
#define PRODUCT_CFG_STA_FRAME_BUF_TIMEOUT_MIN        (1)            // STA����frame����Ч��(��С) ��λ:��

#define PRODUCT_CFG_MRS_PLC_TIMEOUT_NUM_MIN          (2)             // ��ǰ�ڵ�����PLC��ʱ����(�����ж�NV��)��С
#define PRODUCT_CFG_MRS_PLC_TIMEOUT_NUM_DEFAULT      (25)             // ��ǰ�ڵ�����PLC��ʱ����(������ת�ڵ�)Ĭ��

#define PRODUCT_CFG_MRS_TURN_NUM_DEFAULT             (100)             // ���������ִ�Ĭ��ֵ
#define PRODECT_CFG_MRS_CCO_FREQ_ENABLE_DEFAULT      (1)    //CCO��ȡ��ʱ�Ӻ�����Ƶ�ζ�ʱ��ʹ�ܿ���

// 645 Frame  receive buffer ��̬�ڴ�
#define PRODUCT_CFG_MRS_645_BUFFER_BYTE_MAX             (512)
#define PRODUCT_CFG_MRS_PROTO3762_FRAME_BUFFER_MAX      (2048)
#define PRODUCT_CFG_MRS_PACKET_MAX_SIZE                 (2048)  // 2KB, ���16KB�ֽ�

#define MRS_SRV_STA_BIND_NETWORK                        (0)
#define MRS_SRV_STA_BIND_DURATION                       (1680)
#define MRS_SRV_SOFT_TF_SWITCH_CLOSE                    (0)
#define MRS_SRV_SOFT_TF_SWITCH_OPEN_CLOSE_WHITE         (1)
#define MRS_SRV_SOFT_TF_SWITCH_OPEN_NOT_CARE_WHITE      (2)

//*****************************************************************************
//*****************************************************************************

#define MRS_STA_FRAME645_BUFSIZE_MAX    (256)
#define MRS_STA_FRAME_BUF_ITEM_BUFSIZE  (MRS_STA_FRAME645_BUFSIZE_MAX)

//*****************************************************************************
//*****************************************************************************

//////////////////////////////////////////////////////////////////////////
// MRS ����
//////////////////////////////////////////////////////////////////////////
#define PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT
#define MRS_SGD_INNER_VER
#define CSG_STG_VER 0		//����Ϊ0������Ϊ1
#define LOCAL_VER	"BJ"
#define BASE_VER  2
#ifdef SIGNAL_MCU
	#define VER_NO   19
#else
	#define VER_NO   03
#endif
#define EXT_VER  0x0245
#define MRS_SRV_IR_698_45 
#define MRS_SGD_INNER_VER
//#define PRODUCT_CFG_SUPPORT_MRS_UPG_DEMO
#define MRS_SRV_PROTOCOL_CJT188         // ֧��CJT188Э��

#define PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF

#define PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF_II //��չ�� �׶�2 ȡʱ϶ ����


#define MRS_SRV_MODULE_ID			//ģ��ID
#define MRS_SRV_MODULE_ID_LEN 11	//ģ��ID����
#define MRS_SRV_MODULE_ID_LEN_MAX 20	//���֧��ģ��ID����

#undef  PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_STA) && defined(PRODUCT_CFG_FEATURE_STA_CONCENTRATOR_CHECK))
#define PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// UPG ����
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// DFX ����
//////////////////////////////////////////////////////////////////////////
#define PRODUCT_CFG_SUPPORT_OSM
#define PRODUCT_CFG_SUPPORT_OSSM

//////////////////////////////////////////////////////////////////////////
// EQU ����
//////////////////////////////////////////////////////////////////////////



#endif // __APP_CONFIG_H__

