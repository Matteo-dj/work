//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : hi_ft_nv.h
//  �� �� ��   : V1.0
//  ��    ��   : shenhankun/00130424
//  ��������   : 2011-10-26
//  ��������   : ����ģʽ�µ�NV������
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2011-10-26
//    ��    �� : shenhankun/00130424
//    �޸����� : �����ļ�
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   :
// SUBSYSTEM :
// MODULE    :
// OWNER     :
//*****************************************************************************

#ifndef __FT_NV_H__
#define __FT_NV_H__
#include <hi_types.h>
//*****************************************************************************


//*****************************************************************************
// NV ID ���� [0xF00, 0xFFF]
//*****************************************************************************
#define  HI_NV_FTM_ID_START            0xF00
#define  HI_NV_FTM_ID_END              0xFFF

#define  HI_NV_FTM_MANUFACOTR_ID       0xF01 // HI_NV_FTM_MANUFACOTR_ID_S
#define  HI_NV_FTM_RESERVED1_ID        0xF02 // HI_NV_FTM_RESERVED1_ID_S
#define  HI_NV_FTM_PRODUCT_ID          0xF03 // HI_NV_FTM_PRODUCT_ID_S
#define  HI_NV_FTM_RESERVED2_ID        0xF04 // HI_NV_FTM_RESERVED2_ID_S
#define  HI_NV_FTM_STARTUP_CFG_ID      0xF05 // HI_NV_FTM_STARTUP_CFG_S
#define  HI_NV_FTM_UPG_START_CFG_ID    0xF06 // HI_NV_FTM_UPG_START_CFG_S
#define  HI_NV_FTM_UPG_CFG_ID          0xF07 // HI_NV_FTM_UPG_CFG_S

#define  ID_DIAG_CMD_CONFIG_NM_SERIAL_NUMBER        (0xF08)

#define HI_NV_FTM_RESERVED3_ID         0xF09 // HI_NV_FTM_RESERVED3_ID_S


///////////////////////////////////////////////////////////////////////////////
// Reserved for used.
#define HI_NV_FTM_RESERVED4_ID         0xFA0 // HI_NV_FTM_RESERVED4_ID_S
#define  HI_NV_FTM_NM_INIT_PARA         0xFA1 // HI_NV_FTM_RESERVED1_S

#define  HI_NV_FTM_UPG_SCHEDL_CFG_ID            0xFA2 // ����ȫ�����Ե�����
#define  HI_NV_FTM_UPG_SCHEDL_ADVANCE_CFG_ID    0xFA3 // ����ȫ�����Ե�����

#define  HI_NV_FTM_COLLECT_PARAM_FILE   0xFA4 // �ɼ��������洢
/* BEGIN: Modified by b00208046, 2014/1/10   ���ⵥ��:DTS2014010400315*/
#define  HI_NV_FTM_BQ_UPG_ID            0xFA5 // HI_NV_FTM_CMN_CFG_ID_S
/* END:   Modified by b00208046, 2014/1/10 */
#define  HI_NV_FTM_CCO_PARAM_ID         0xFA6 // CCO
#define  HI_NV_FTM_PASSWORD_ID          0xFA7 // �洢HSO����
#define  HI_NV_FTM_NDM_AUTO_CHECK_ID    0xFA7 // HI_NV_FTM_RESERVED7_S
#define  HI_NV_FTM_MANAGEMENT_ID        0xFA8 // �洢оƬ����ID ���豸ID
#define  HI_NV_FTM_RESERVED9_ID         0xFA9 // HI_NV_FTM_RESERVED9_S
//*****************************************************************************


//*****************************************************************************
#define HI_IS_NV_FTM(x)               ((x)>=HI_NV_FTM_ID_START) && ((x)<HI_NV_FTM_ID_END)
//*****************************************************************************


//*****************************************************************************
typedef struct
{
    HI_CHAR  szId[3];     // ���̴��룬Ĭ��Ϊ"HS"
    HI_CHAR  szChipId[5]; // оƬ���룬Ĭ��Ϊ"3911"
}HI_NV_FTM_MANUFACOTR_ID_S;

typedef enum
{
    HI_FTM_HW_PLATFORM_TYPE_UNKOWN = 0, // Ĭ��Ϊ ASIC
    HI_FTM_HW_PLATFORM_TYPE_FPGA, // FPGA
    HI_FTM_HW_PLATFORM_TYPE_ASIC  // ASIC
} HI_FTM_HW_PLATFORM_TYPE_E;


/**
 * @ingroup hct_mdm
 * оƬ����
 */
typedef enum
{
    HI_FTM_CHIP_TYPE_UNKOWN = 0,/**< δ֪оƬ����*/
    HI_FTM_CHIP_TYPE_3911C,     /**< CоƬ����*/
    HI_FTM_CHIP_TYPE_3911T,     /**< TоƬ����*/
    HI_FTM_CHIP_TYPE_MAX
} HI_FTM_CHIP_TYPE_E;

/* BEGIN: Added by b00208046, 2014/1/10   ���ⵥ��:DTS2014010400315*/
typedef struct
{
    HI_U32 ulBatChipUpgVer;                   //BQоƬ�汾��
    HI_U32 ulBatChipUpgCheckSum;              //֮ǰ�����ļ�CRC
    HI_U16 usBatChipUpgStatus;                //֮ǰ������״̬
    HI_U8 ucReserved1[6];                     //BQ����Ԥ��
    HI_U8 ucDefaultGateway[HI_IPV4_ADDR_LEN]; //��̫��Ĭ������
    HI_U8 ucReserved2[44];                    //�����ֽ�
}HI_NV_FTM_CMN_CFG_ID_S;


//���ڴ洢������սֵ�������
typedef struct
{
    HI_U32 verMagic:24;//0X50 0X57 0X44  'PWD'
    HI_U32 verNum:8;//Ĭ��д0x1
    HI_U8  aucSTK[32];//�����洢PWD��STK
    HI_U32 aulSTKCrc;//STK����У��ֵ
    HI_U8  aucSalt[16];		// �������洢��Ϣ����ֵ���ɰ汾��λ��Ϊ0
    HI_U16 usCnt;		// �������洢��Ϣ��PBKDF2�����������ɰ汾��λ��Ϊ0
    HI_U16 reserved[3];
} HI_NV_FTM_PASS_WORD_S;

typedef HSO_ENUM
{
    HI_FTM_STA_TYPE_UNKOWN = 0, // Ĭ��Ϊ ASIC
    HI_FTM_STA_TYPE_USTA, // USTA
    HI_FTM_STA_TYPE_MAX
} HI_FTM_STA_TYPE_E;

/* END:   Added by b00208046, 2014/1/10 */
#define HI_FTM_CHIP_SDRAM_2M   2
#define HI_FTM_CHIP_SDRAM_8M   8
#define HI_FTM_CHIP_SDRAM_16M  16

// STA��Ʒ�����е��/I�ɵĲ�Ʒ�����Ͷ���
#define  HI_FTM_PRODUCT_TYPE_STATYPE_M 0 // ���
#define  HI_FTM_PRODUCT_TYPE_STATYPE_I 1 // I��

#define HI_FTM_PRODUCT_APPID_MRS    (0)//����ϵͳ
#define HI_FTM_PRODUCT_APPID_RMT    (1)//���Զ���ز��ն�
#define HI_FTM_PRODUCT_APPID_RSV    (4)//ע��:4096��ID����ID������boot���ǲ�Ʒʹ��
#define HI_FTM_PRODUCT_ID_RANGE_MRS  (500)
#define HI_FTM_PRODUCT_ID_RANGE_RMT  (200)
#ifdef PRODUCT_CFG_FEATURE_REMOTE_COMMUNICATION_TERMINAL
#define HI_FTM_PRODUCT_TYPE_BASE  (HI_FTM_PRODUCT_ID_RANGE_MRS) //����ز�����ն�ϵͳ
#define HI_FTM_PRODUCT_TYPE_RANGE (HI_FTM_PRODUCT_ID_RANGE_RMT)
#else
#define HI_FTM_PRODUCT_TYPE_BASE 0    //����ϵͳ
#define HI_FTM_PRODUCT_TYPE_RANGE (HI_FTM_PRODUCT_ID_RANGE_MRS)
#endif
// ���ֵΪ16bit
#ifndef MAC_V200_LLT
typedef HSO_ENUM
{
    HI_FTM_PRODUCT_TYPE_UNKOWN = 0,

    HI_FTM_PRODUCT_TYPE_C01 = (HI_FTM_PRODUCT_TYPE_BASE+1),// �������ز�ģ�飻�汾1���������
    HI_FTM_PRODUCT_TYPE_C02,
    HI_FTM_PRODUCT_TYPE_C03,
    HI_FTM_PRODUCT_TYPE_C04,
    HI_FTM_PRODUCT_TYPE_C05,
    HI_FTM_PRODUCT_TYPE_C06,
    HI_FTM_PRODUCT_TYPE_C07,
    HI_FTM_PRODUCT_TYPE_C08,

    HI_FTM_PRODUCT_TYPE_C11 = (HI_FTM_PRODUCT_TYPE_BASE+11),// �������ز�ģ�飻�汾1���������
    HI_FTM_PRODUCT_TYPE_C12,
    HI_FTM_PRODUCT_TYPE_C13,
    HI_FTM_PRODUCT_TYPE_C14,
    HI_FTM_PRODUCT_TYPE_C15,
    HI_FTM_PRODUCT_TYPE_C16,
    HI_FTM_PRODUCT_TYPE_C17,
    HI_FTM_PRODUCT_TYPE_C18,

    HI_FTM_PRODUCT_TYPE_R21 = (HI_FTM_PRODUCT_TYPE_BASE+21),// �м������汾1���������
    HI_FTM_PRODUCT_TYPE_R22,
    HI_FTM_PRODUCT_TYPE_R23,
    HI_FTM_PRODUCT_TYPE_R24,
    HI_FTM_PRODUCT_TYPE_R25,
    HI_FTM_PRODUCT_TYPE_R26,
    HI_FTM_PRODUCT_TYPE_R27,
    HI_FTM_PRODUCT_TYPE_R28,

    HI_FTM_PRODUCT_TYPE_T31 = (HI_FTM_PRODUCT_TYPE_BASE+31),// ������汾1���������
    HI_FTM_PRODUCT_TYPE_T32,
    HI_FTM_PRODUCT_TYPE_T33,
    HI_FTM_PRODUCT_TYPE_T34,

    HI_FTM_PRODUCT_TYPE_R35,                    // �м������汾1���������
    HI_FTM_PRODUCT_TYPE_R36,
    HI_FTM_PRODUCT_TYPE_R37,
    HI_FTM_PRODUCT_TYPE_R38,

    HI_FTM_PRODUCT_TYPE_D41 = (HI_FTM_PRODUCT_TYPE_BASE+41),// ���������汾1���������
    HI_FTM_PRODUCT_TYPE_D42,
    HI_FTM_PRODUCT_TYPE_D43,
    HI_FTM_PRODUCT_TYPE_D44,
    HI_FTM_PRODUCT_TYPE_D45,
    HI_FTM_PRODUCT_TYPE_D46,
    HI_FTM_PRODUCT_TYPE_D47,
    HI_FTM_PRODUCT_TYPE_D48,

    HI_FTM_PRODUCT_TYPE_D51 = (HI_FTM_PRODUCT_TYPE_BASE+51),// ���������汾8���������
    HI_FTM_PRODUCT_TYPE_D52,
    HI_FTM_PRODUCT_TYPE_D53,
    HI_FTM_PRODUCT_TYPE_D54,
    HI_FTM_PRODUCT_TYPE_D55,
    HI_FTM_PRODUCT_TYPE_D56,
    HI_FTM_PRODUCT_TYPE_D57,
    HI_FTM_PRODUCT_TYPE_D58,

    HI_FTM_PRODUCT_TYPE_D61 = (HI_FTM_PRODUCT_TYPE_BASE+61),// ����������
    HI_FTM_PRODUCT_TYPE_D62,
    HI_FTM_PRODUCT_TYPE_D63,
    HI_FTM_PRODUCT_TYPE_D64,

    HI_FTM_PRODUCT_TYPE_M101 = (HI_FTM_PRODUCT_TYPE_BASE+101),// ���/I�Ͳɼ��� �ز�ģ�飻�汾1
    HI_FTM_PRODUCT_TYPE_M102,
    HI_FTM_PRODUCT_TYPE_M103,
    HI_FTM_PRODUCT_TYPE_M104,
    HI_FTM_PRODUCT_TYPE_M105,
    HI_FTM_PRODUCT_TYPE_M106,
    HI_FTM_PRODUCT_TYPE_M107,
    HI_FTM_PRODUCT_TYPE_M108,

    HI_FTM_PRODUCT_TYPE_M111 = (HI_FTM_PRODUCT_TYPE_BASE+111),// II�Ͳɼ����汾1
    HI_FTM_PRODUCT_TYPE_M112,
    HI_FTM_PRODUCT_TYPE_M113,
    HI_FTM_PRODUCT_TYPE_M114,
    HI_FTM_PRODUCT_TYPE_M115,
    HI_FTM_PRODUCT_TYPE_M116,
    HI_FTM_PRODUCT_TYPE_M117,
    HI_FTM_PRODUCT_TYPE_M118,
    /* start- ����ID�����ڼ��������⣬������Ӳ��ID�������������ID -start */
    HI_FTM_PRODUCT_TYPE_M119,
    HI_FTM_PRODUCT_TYPE_M120,
    HI_FTM_PRODUCT_TYPE_M121,
    HI_FTM_PRODUCT_TYPE_M122,
    HI_FTM_PRODUCT_TYPE_M123,
    HI_FTM_PRODUCT_TYPE_M124,
    HI_FTM_PRODUCT_TYPE_M125,
    HI_FTM_PRODUCT_TYPE_M126,

    HI_FTM_PRODUCT_TYPE_M131 = (HI_FTM_PRODUCT_TYPE_BASE+131),// ���/I�Ͳɼ��� �ز�ģ�飻�汾9
    HI_FTM_PRODUCT_TYPE_M132,
    HI_FTM_PRODUCT_TYPE_M133,
    HI_FTM_PRODUCT_TYPE_M134,
    HI_FTM_PRODUCT_TYPE_M135,
    HI_FTM_PRODUCT_TYPE_M136,
    HI_FTM_PRODUCT_TYPE_M137,
    HI_FTM_PRODUCT_TYPE_M138,
    /* end --end */
    HI_FTM_PRODUCT_TYPE_ANY = (HI_FTM_PRODUCT_TYPE_BASE + HI_FTM_PRODUCT_TYPE_RANGE),

    HI_FTM_PRODUCT_TYPE_MAX
} HI_FTM_PRODUCT_TYPE_E;
#else

/**
* @ingroup hct_sys
* @brief ��Ʒ��̬
*/
typedef enum
{
	HI_FTM_PRODUCT_TYPE_UNKOWN = 0,

		HI_FTM_PRODUCT_TYPE_C01 = (HI_FTM_PRODUCT_TYPE_BASE+1),/**< �������ز�ģ�飻�汾1��������� */
		HI_FTM_PRODUCT_TYPE_C02,
		HI_FTM_PRODUCT_TYPE_C03,
		HI_FTM_PRODUCT_TYPE_C04,
		HI_FTM_PRODUCT_TYPE_C05,
		HI_FTM_PRODUCT_TYPE_C06,
		HI_FTM_PRODUCT_TYPE_C07,
		HI_FTM_PRODUCT_TYPE_C08,

		HI_FTM_PRODUCT_TYPE_C11 = (HI_FTM_PRODUCT_TYPE_BASE+11),/**< �������ز�ģ�飻�汾1���������*/
		HI_FTM_PRODUCT_TYPE_C12,
		HI_FTM_PRODUCT_TYPE_C13,
		HI_FTM_PRODUCT_TYPE_C14,
		HI_FTM_PRODUCT_TYPE_C15,
		HI_FTM_PRODUCT_TYPE_C16,
		HI_FTM_PRODUCT_TYPE_C17,
		HI_FTM_PRODUCT_TYPE_C18,

		HI_FTM_PRODUCT_TYPE_R21 = (HI_FTM_PRODUCT_TYPE_BASE+21),/**< �м������汾1���������*/
		HI_FTM_PRODUCT_TYPE_R22,
		HI_FTM_PRODUCT_TYPE_R23,
		HI_FTM_PRODUCT_TYPE_R24,
		HI_FTM_PRODUCT_TYPE_R25,
		HI_FTM_PRODUCT_TYPE_R26,
		HI_FTM_PRODUCT_TYPE_R27,
		HI_FTM_PRODUCT_TYPE_R28,

		HI_FTM_PRODUCT_TYPE_T31 = (HI_FTM_PRODUCT_TYPE_BASE+31),/**< ������汾1���������*/
		HI_FTM_PRODUCT_TYPE_T32,
		HI_FTM_PRODUCT_TYPE_T33,
		HI_FTM_PRODUCT_TYPE_T34,

		HI_FTM_PRODUCT_TYPE_R35,                                /**< �м������汾1���������*/
		HI_FTM_PRODUCT_TYPE_R36,
		HI_FTM_PRODUCT_TYPE_R37,
		HI_FTM_PRODUCT_TYPE_R38,

		HI_FTM_PRODUCT_TYPE_D41 = (HI_FTM_PRODUCT_TYPE_BASE+41),/**< ���������汾1���������*/
		HI_FTM_PRODUCT_TYPE_D42,
		HI_FTM_PRODUCT_TYPE_D43,
		HI_FTM_PRODUCT_TYPE_D44,
		HI_FTM_PRODUCT_TYPE_D45,
		HI_FTM_PRODUCT_TYPE_D46,
		HI_FTM_PRODUCT_TYPE_D47,
		HI_FTM_PRODUCT_TYPE_D48,

		HI_FTM_PRODUCT_TYPE_D51 = (HI_FTM_PRODUCT_TYPE_BASE+51),/**< ���������汾8���������*/
		HI_FTM_PRODUCT_TYPE_D52,
		HI_FTM_PRODUCT_TYPE_D53,
		HI_FTM_PRODUCT_TYPE_D54,
		HI_FTM_PRODUCT_TYPE_D55,
		HI_FTM_PRODUCT_TYPE_D56,
		HI_FTM_PRODUCT_TYPE_D57,
		HI_FTM_PRODUCT_TYPE_D58,

		HI_FTM_PRODUCT_TYPE_D61 = (HI_FTM_PRODUCT_TYPE_BASE+61),/**< ����������*/
		HI_FTM_PRODUCT_TYPE_D62,
		HI_FTM_PRODUCT_TYPE_D63,
		HI_FTM_PRODUCT_TYPE_D64,

		HI_FTM_PRODUCT_TYPE_M101 = (HI_FTM_PRODUCT_TYPE_BASE+101),/**< ���/I�Ͳɼ��� �ز�ģ�飻�汾1*/
		HI_FTM_PRODUCT_TYPE_M102,
		HI_FTM_PRODUCT_TYPE_M103,
		HI_FTM_PRODUCT_TYPE_M104,
		HI_FTM_PRODUCT_TYPE_M105,
		HI_FTM_PRODUCT_TYPE_M106,
		HI_FTM_PRODUCT_TYPE_M107,
		HI_FTM_PRODUCT_TYPE_M108,

		HI_FTM_PRODUCT_TYPE_M111 = (HI_FTM_PRODUCT_TYPE_BASE+111),/**< II�Ͳɼ����汾1*/
		HI_FTM_PRODUCT_TYPE_M112,
		HI_FTM_PRODUCT_TYPE_M113,
		HI_FTM_PRODUCT_TYPE_M114,
		HI_FTM_PRODUCT_TYPE_M115,
		HI_FTM_PRODUCT_TYPE_M116,
		HI_FTM_PRODUCT_TYPE_M117,
		HI_FTM_PRODUCT_TYPE_M118,
		/* start- ����ID�����ڼ��������⣬������Ӳ��ID�������������ID -start */
		HI_FTM_PRODUCT_TYPE_M119,
		HI_FTM_PRODUCT_TYPE_M120,
		HI_FTM_PRODUCT_TYPE_M121,
		HI_FTM_PRODUCT_TYPE_M122,
		HI_FTM_PRODUCT_TYPE_M123,
		HI_FTM_PRODUCT_TYPE_M124,
		HI_FTM_PRODUCT_TYPE_M125,
		HI_FTM_PRODUCT_TYPE_M126,

		HI_FTM_PRODUCT_TYPE_M131 = (HI_FTM_PRODUCT_TYPE_BASE+131),/**< ���/I�Ͳɼ��� �ز�ģ�飻�汾9*/
		HI_FTM_PRODUCT_TYPE_M132,
		HI_FTM_PRODUCT_TYPE_M133,
		HI_FTM_PRODUCT_TYPE_M134,
		HI_FTM_PRODUCT_TYPE_M135,
		HI_FTM_PRODUCT_TYPE_M136,
		HI_FTM_PRODUCT_TYPE_M137,
		HI_FTM_PRODUCT_TYPE_M138,
		/* end --end */
		HI_FTM_PRODUCT_TYPE_ANY = (HI_FTM_PRODUCT_TYPE_BASE + HI_FTM_PRODUCT_TYPE_RANGE),

		HI_FTM_PRODUCT_TYPE_MAX
} HI_FTM_PRODUCT_TYPE_E;

#endif

#define HI_FTM_PRODUCT_TYPE_C21   HI_FTM_PRODUCT_TYPE_R21
#define HI_FTM_PRODUCT_TYPE_C28   HI_FTM_PRODUCT_TYPE_R28
#define HI_FTM_PRODUCT_TYPE_C31   HI_FTM_PRODUCT_TYPE_T31
#define HI_FTM_PRODUCT_TYPE_C34   HI_FTM_PRODUCT_TYPE_T34
#define HI_FTM_PRODUCT_TYPE_C38   HI_FTM_PRODUCT_TYPE_R38
#define HI_FTM_PRODUCT_TYPE_C41   HI_FTM_PRODUCT_TYPE_D41
#define HI_FTM_PRODUCT_TYPE_C48   HI_FTM_PRODUCT_TYPE_D48
#define HI_FTM_PRODUCT_TYPE_C58   HI_FTM_PRODUCT_TYPE_D58
#define HI_FTM_PRODUCT_TYPE_S101  HI_FTM_PRODUCT_TYPE_M101
#define HI_FTM_PRODUCT_TYPE_S107  HI_FTM_PRODUCT_TYPE_M107
#define HI_FTM_PRODUCT_TYPE_S108  HI_FTM_PRODUCT_TYPE_M108
#define HI_FTM_PRODUCT_TYPE_S111  HI_FTM_PRODUCT_TYPE_M111
#define HI_FTM_PRODUCT_TYPE_S118  HI_FTM_PRODUCT_TYPE_M118
//*****************************************************************************

typedef struct
{
    HI_FTM_HW_PLATFORM_TYPE_E enHwPlatform; // ƽ̨����
    HI_FTM_CHIP_TYPE_E enChipType;          // оƬ����
    HI_FTM_PRODUCT_TYPE_E enProductType;    // ��Ʒ��̬
    HI_U8A ucPlcMac[HI_PLC_MAC_ADDR_LEN]; // ͨ��ģ��MAC��ַ
    HI_U8 ucEthMac[HI_PLC_MAC_ADDR_LEN]; // ���ڵ�MAC��ַ
    HI_U8 ucEthIp[HI_IPV4_ADDR_LEN]; // ���ڵ�IP��ַ, Ĭ��V4  192.168.1.100
    HI_U8 ucEthMask[HI_IPV4_ADDR_LEN];  //���ڵ���������
} HI_NV_FTM_PRODUCT_ID_S;



#define HI_SYS_TEST_MODE_EQM_NONE      0
#define HI_SYS_TEST_MODE_EQM_STA_M     1
#define HI_SYS_TEST_MODE_EQM_STA_IIC   2
#define HI_SYS_TEST_MODE_EQM_NDM       4
#define HI_SYS_TEST_MODE_EQM_NSTA      8

#ifndef PRODUCT_CFG_ALONE_NV_FILE_UPG
typedef struct
{
    HI_U32 ulStatus;         // HI_ND_SYS_STATUS_NORMAL HI_ND_SYS_STATUS_UPGRADE HI_ND_SYS_STATUS_TEST  HI_ND_SYS_STATUS_*
    HI_U32 ulLoadFlashAddr;  // ��������ַ
    HI_U32 ulLastBlkIdx;     // ���һ��������Ч�����ļ����������
    HI_U32 ulRcvFileSize;    // ���յ������ļ�����(ָ��������Ч����ɵĴ�С)
    HI_U32 ulUpgId;
    HI_U32 ulUpgWindowTime;
    HI_U32 ulBackTimes;   //��¼���˵Ĵ���
    HI_U32 ulRuntimes;    //boot��������¼����OS��Ӧ�õĴ�����
    HI_U32 reserved[2];
} HI_NV_FTM_STARTUP_CFG_S;
#else
//HI_NV_FTM_STARTUP_CFG_S��ucUpgTarget������ֵ
#define DRV_UPG_CLEAR    0
#define DRV_UPG_NV_ALONE 1
#define DRV_UPG_CODE_NV  2
#define DRV_UPG_BOOT     3

typedef struct
{
    HI_U32 ulStatus;         // HI_ND_SYS_STATUS_NORMAL HI_ND_SYS_STATUS_UPGRADE HI_ND_SYS_STATUS_TEST  HI_ND_SYS_STATUS_*
    HI_U32 ulLoadFlashAddr;  // ��������ַ
    HI_U32 ulLastBlkIdx;     // ���һ��������Ч�����ļ����������
    HI_U32 ulRcvFileSize;    // ���յ������ļ�����(ָ��������Ч����ɵĴ�С)
    HI_U32 ulUpgId;
    HI_U32 ulUpgWindowTime;
    HI_U32 ulBackTimes;   //��¼���˵Ĵ���
    HI_U32 ulRuntimes; //boot��������¼����OS��Ӧ�õĴ�����
    HI_U8  ucUpgTarget;     //����NV������CODE+NV
    HI_U8  ucTestMode; // HI_SYS_TEST_MODE_EQM_STA_M, HI_SYS_TEST_MODE_EQM_*
    HI_U8  ucDecompressNv;//����ȡ���ı�־λ0x55��ʱ���ѹnv
    HI_U8  reserved1[1];
    HI_U32 reserved2;
} HI_NV_FTM_STARTUP_CFG_S;
#endif

//������ʼ��Ϣ,������Start���������
typedef struct
{
    HI_U32 ulOption;    //����ѡ��
    HI_U32 ulUpgId;     //����ID
    HI_U32 ulFileLen;   //�����ļ��ĳ���
    HI_U32 ulBlkNum;    //���512�ֽ�Ϊһ����λ
    HI_U32 ulUpgTimeWindow; //����ʱ�䴰
    HI_U32 ulLastBlockId;   //��һ��BLOCK��ID
    HI_U32 ulCrc;
}HI_NV_FTM_UPG_START_CFG_S;

/**
 *@ingroup hct_upg
 * ��ʼ����ʱ�û�����UPG�Ĳ������á�Ĭ����0���ɡ�
*/
typedef struct
{
    HI_U16 total_number_when_start;    /**<��ʼ����ʱ��������վ���������ɹ���������ʱ�������ֵ��*/
    HI_U16 extra_timer_max_count; /**<��δʹ��*/
    HI_U16 extra_timer_period;    /**<��δʹ�á�*/
    HI_U16 upg_verify_new_policy; /**<��δʹ�á�*/
    HI_U16 total_number_threshold; /**<��δʹ�á�*/
    HI_U16 rcv_timeout_timer_period; /**<HSO�뵥�彻����ʱʱ�䡣*/
    HI_U16 blk_send_period; /**<��δʹ�á�*/
    HI_U8  reserved2;
    HI_U8  upg_type; /**<��������Ϊbootʱ,��ֵΪ0xAB����������Ϊcodeʱ����ֵΪ1��*/
    HI_U8  device_upg_status;   /**<�豸����״̬,�������������CCO/STA����,���������ʶ���豸����״̬��ȡֵ�������£�\n
								0��ʾ����״̬��\n
								1��ʾ��������״̬��\n
								2��ʾ�����ɹ�״̬��\n
								3��ʾ����ʧ��״̬��*/
    HI_U8  transmit_blk_num;    /**<��δʹ�á�*/
    HI_U16 mac_tx_arq_period;   /**<��δʹ�á�*/
    HI_U16 start_tx_period_num; /**<��δʹ�á�*/
    HI_U16 transmit_blk_resend_num; /**<��δʹ�á�*/
    HI_U16 broadcast_stop_num;      /**<��δʹ�á�*/
    HI_U16 broadcast_stop_period;   /**<��δʹ�á�*/
    HI_U16 sta_dfx_report;          /**<��δʹ�á�*/
    HI_U16 cco_dfx_report;          /**<��δʹ�á�*/
    HI_U16 broadcast_finish_num;    /**<��δʹ�á�*/
    HI_U16 broadcast_finish_period; /**<��δʹ�á�*/
    HI_U16 reserved;                /**<��δʹ�á�*/
    HI_U16 whether_upg_test_mode;   /**<��δʹ�á�*/
}HI_NV_FTM_UPG_CFG_S;


typedef struct _DIAG_CMD_CONFIG_NM_SERIAL_NUMBER_
{
    HI_U32 form_serial_number;
    HI_U32 assoc_random_number;
}DIAG_CMD_CONFIG_NM_SERIAL_NUMBER_STRU;



// ���ܵĹ�����NV����
typedef struct
{
    HI_U8   main_network_snid;      // �����������
    HI_U8   init_freq;              // ��ʼ��Ƶ��
    HI_U8   sub_network_snid;       // �����������
    HI_U8   network_mode;           // ����ģʽ

    HI_U8   network_mode_policy;    // ����ģʽ����
    HI_U8   pa_switch;              // PA����,0��PA,1��PA
    HI_U8   rf_channel;             // RF�ŵ�
    HI_U8   rf_rate;                // RF����

    HI_U8   is_dual_module;         // �Ƿ�Ϊ˫ģģ�飬�����ڿ�ά�ɲ�
    HI_U8   tx_power;               // RF���书��
    HI_U8   modem_type;             // RF��������2FSK:0,2GFSK:1
    HI_U8   white_list_mode;        // ��������ģʽ

    HI_U8   sta_type;               // sta����
    HI_U8   recover_sg_freq;        // �ָ�Ϊ����Э��ʱ��Ƶ��
    HI_S16  correction_voltage;     // �����У����ѹ

    HI_U32  main_network_nid;
    HI_U32  sub_network_nid;

    HI_U8   lock_cco_mac[HI_PLC_MAC_ADDR_LEN];      // ����CCO��MAC��ַ
    HI_U8   belong_cco_addr[HI_PLC_MAC_ADDR_LEN];   // ��������CCO�ĵ�ַ
    HI_U8   reserved[6];
    HI_BOOL is_belong_main_node_addr;               // �Ƿ�������ڵ��ַ
    HI_BOOL not_recover_sg_from_v100    :1;          // ����V100Э��ָ�����Э��
    HI_U8   reserved1                  :7;

    HI_U32  reserved2[5];
}HI_NV_FTM_NM_INIT_PARA_S;

typedef struct
{
    HI_U8   succ_ratio;          // �������վ���ֱ�
    HI_U8   blk_per_pkt;          //���͵�һ�����еĿ����� ÿ���鰴 UPG_BLOCK_SIZE ��
    HI_U8   query_switch;         //�״��Ƿ��ѯ����
    HI_U8   query_count;        // ����ѯ����

    HI_U16  data_loop_max;       //���ݷ�������ִΣ�
    HI_U16  data_send_interval;  //���Ͱ�֮�䷢�ͼ�� ms
    HI_U16  query_send_interval;  //��ѯ֡���ͼ�� ms
    HI_U16  query_max_ways;       //��󲢷���ѯ��

    HI_U8   exception_max_count; // ���̵����EXCEPTION����
/* BEGIN:PN:DTS2014092002573 ȫ������״̬��ѯͳ�����Ż� MODIFY\ADD  by chenli/00238131 at 2014/9/14*/
    //����STA����FLASH�е�ͳ���� 19���ֽ�
    HI_U8   dfx_sta_reply_status_report_cnt;//STA�����ϱ�״̬��ѯ����
    HI_U8   dfx_sta_rcv_status_cnt;         //STA���յ�״̬��ѯ�Ĵ���
    HI_U8   dfx_sta_reply_status_suc_cnt;   //STAӦ��״̬��ѯ�ɹ�����

    HI_U8   dfx_sta_reply_status_fail_cnt;  //STAӦ��״̬��ѯʧ�ܴ���
    HI_U8   dfx_sta_reply_status_full_cnt;  //STAӦ��״̬��ѯ���Ĵ���
    HI_U16  dfx_sta_reply_status_LRC;       //STA���һ��Ӧ��״̬��ѯ�Ľ��
    HI_U32  dfx_sta_reply_rcv_file_size_LT; //STA���һ���ϱ�״̬��ѯʱ�����ļ���С

    HI_U16  dfx_sta_rcv_start_suc_LTS;  //STA���һ�γɹ�����START֡��ʱ���
    HI_U16  dfx_sta_rcv_blk_LTS;          //STA���һ�ν��յ����ݰ���ʱ���
    HI_U16  dfx_sta_reply_status_suc_LTS;//STA���һ�γɹ�Ӧ��״̬��ѯ��ʱ���
    HI_U16  dfx_sta_rcv_affirm_LTS;  //STA���һ�ν��յ�ȫ�����֪ͨ��ʱ���
/* END:PN:DTS2014092002573 ȫ������״̬��ѯͳ�����Ż�  MODIFY\ADD  by chenli/00238131 at 2014/9/14*/
}UPG_BROADCAST_CFG_STRU; // 32

typedef struct
{
    HI_U8  succ_ratio;              // �������վ���ֱ�
    HI_U8  blk_per_pkt;             //���͵�һ�����еĿ����� ÿ���鰴 UPG_BLOCK_SIZE ��
    HI_U8  query_switch;            //�״��Ƿ��ѯ����
    HI_U8  query_count;            // ����ѯ����

    HI_U16 data_loop_max;          //���ݷ�������ִΣ�
    HI_U16 data_max_ways;           //ͬʱ���������·��
    HI_U16 data_send_internal;      //���Ͱ�֮�䷢�ͼ�� ms
    HI_U16 query_send_interval;     //��ѯ֡���ͼ�� ms
    HI_U16 query_max_ways;          //��󲢷���ѯ��
    HI_U16 dfx_cco_snd_affirm_cnt;  //CCO����ȫ�����֪ͨ�Ĵ���

    HI_U8   exception_max_count; // ���̵����EXCEPTION����
    HI_U8  dfx_cco_set_query_exception_sta_mac[3];  //���һ������Ϊ����״̬�쳣վ���MAC��ַĩ3λ

    HI_U16 dfx_cco_set_query_exception_sta_cnt;     //����Ϊ����״̬�쳣վ���ܸ���
    HI_U16 dfx_cco_first_rcv_start_time_stamp;     //CCO��һ�γɹ�����START������ʱ���

    HI_U16  dfx_cco_entry_bc_time_stamp;            //CCO����㲥���Ե�ʱ���
    HI_U16  dfx_cco_entry_u2b_time_stamp;           //CCO����U2B���Ե�ʱ���
    HI_U16  dfx_cco_entry_uc_time_stamp;            //CCO���뵥�����Ե�ʱ���
    HI_U16  dfx_cco_snd_affim_time_stamp;           //CCO��ʼ����ȫ�����֪ͨ��ʱ���
}UPG_U2B_CFG_STRU; // 32 ���ܳ���32�ֽ�

typedef struct
{
    HI_U8   succ_ratio;          // �������վ���ֱ�
    HI_U8   blk_per_pkt;          //���͵�һ�����еĿ����� ÿ���鰴 UPG_BLOCK_SIZE ��
    HI_U8   query_switch;         //�״��Ƿ��ѯ����
    HI_U8   query_count;         // ����ѯ������ ��� exception_max_countʹ��

    HI_U16  data_loop_max;     // ���ݷ�������ִΣ����ﵽ����ʱ,exceptio��1��
                                // ��exception�ﵽexception_max_count
                                // �󽫸�վ����Ϊexception.
    HI_U16  data_max_ways;       //ͬʱ���������·��
    HI_U16  data_send_interval;  //���Ͱ�֮�䷢�ͼ�� ms
    HI_U16  query_send_interval;  //��ѯ֡���ͼ�� ms
    HI_U16  query_max_ways;       //��󲢷���ѯ��
    HI_U16  dfx_cco_rcv_status_report_cnt;//CCO���յ�STA�����ϱ���״̬��ѯ�ܴ���

    HI_U8   exception_max_count; // ���̵����EXCEPTION����
/* BEGIN:PN:DTS2014092002573 ȫ������״̬��ѯͳ�����Ż� MODIFY\ADD  by chenli/00238131 at 2014/9/14*/
    HI_U8   dfx_cco_snd_query_suc_sta_mac[3];   //CCO���һ�η���״̬��ѯվ���MAC��ַĩ3λ

    HI_U16  dfx_cco_snd_status_suc_cnt;         //CCO����״̬��ѯ�ɹ�����
    HI_U16  dfx_cco_snd_status_fail_cnt;        //CCO����״̬��ѯʧ�ܴ���

    HI_U16  dfx_cco_rcv_status_reply_cnt;        //CCO���յ�״̬��ѯӦ���ܴ���
    HI_U16  dfx_cco_rcv_status_reply_discard_cnt;//CCO���յ�״̬��ѯ�����ܴ���
    HI_U32  dfx_cco_upg_id;                      //��ǰ������ID
/* END:PN:DTS2014092002573 ȫ������״̬��ѯͳ�����Ż�  MODIFY\ADD  by chenli/00238131 at 2014/9/14*/
}UPG_UNICAST_CFG_STRU; // 32  ���ܳ���32�ֽ�

typedef struct
{
    HI_U8 delayed_for_send_restart; // ����ִ����ɺ�ȴ���÷�����������. ��λ(m)
    HI_U8 delayed_for_recv_to_restart; // sta �յ���������� ��ʱ������� . ��λ(s)
    HI_U8 send_count;          // ���ͼ�������
    HI_U8 dfx_sta_rcv_affirm_cnt; //STA���յ�ȫ�����֪ͨ�Ĵ���
    HI_U16 send_interval;      //���Ͱ�֮�䷢�ͼ�� ms
    HI_U16 dfx_sta_rcv_start_FRC; //��һ�ε��ÿ�ʼ���������ķ���ֵ
}UPG_AFFIRM_CFG_STRU; // 8

typedef struct
{
    HI_U8 delayed; //����start ֡�ȴ���� ��������. ��λs
    HI_U8 send_count; // ���ͼ�������
    HI_U16 send_interval; //���Ͱ�֮�䷢�ͼ�� ms
    HI_U32 send_ways; // ��������ʱ ����·���� //
}UPG_NOTIFY2START_CFG_STRU;// 8

typedef struct
{
    HI_U8 broadcast_switch; //�Ƿ����ȫ���㲥�Ŀ���
    HI_U8 u2b_switch;       //�Ƿ���õ���ת���ع㲥�Ŀ���
    HI_U8 unicast_switch;   //�Ƿ���ò��������Ŀ���
    HI_U8 slot_type;   // 0: �� B-CSMA�� 1: �� TDMA, 2: B-CSMA �� TDMA ���

    UPG_BROADCAST_CFG_STRU bc_cfg; //ȫ���㲥����
    UPG_AFFIRM_CFG_STRU affirm_cfg; // ��������
    UPG_NOTIFY2START_CFG_STRU notify2start_cfg; // start ������

    HI_U8 query_loop_interval; // ����һ���ѯ���ĺ�ȴ�ʱ�� ��λs.
    HI_U8 data_loop_interval; // ����һ�� ������ɺ�ȴ���ʱ�� s
    HI_U8 switch_schedule_delay; // ���Ե����л�ʱ�� ��λ 100ms
    HI_U8 upg_mode;
/* BEGIN:PN:DTS2014092002573 ȫ������״̬��ѯͳ�����Ż� MODIFY\ADD  by chenli/00238131 at 2014/9/15*/
    HI_U32 dfx_sta_upg_id;       //��ǰ������ID
    HI_U32 dfx_sta_upg_file_size;//��ǰ�����ļ���С
/* END:PN:DTS2014092002573 ȫ������״̬��ѯͳ�����Ż�  MODIFY\ADD  by chenli/00238131 at 2014/9/15*/
}HI_NV_FTM_UPG_SCHEDL_DEFAULT_CFG_S;

typedef struct
{
    UPG_U2B_CFG_STRU u2b_cfg;      //����ת���ع㲥����
    UPG_UNICAST_CFG_STRU uc_cfg;   //������������
}HI_NV_FTM_UPG_SCHEDL_ADVANCE_CFG_S;

typedef struct
{
    HI_U8   CCTAddress[2];  // ע�Ἧ�����߼���ַ��Ĭ��ֵ0xFFFF
    HI_U8   CLTAddress[6];  // �ɼ����ʲ���(��ַ)��Ĭ��ֵ0xBBBBBBBBBBBB
    HI_U8   meter[6];       // �ѵ����׿���ܱ��ַ���õ�ַ����ΪII�Ͳɼ�����MAC��ַ����
    HI_U8   protocol;       // ����Լ����
    HI_U8   CCT_register;   // �Ƿ�ע���ʶ��Ĭ��ֵ0
    HI_U16  reset_times;    // �ն˸�λ����
    HI_U8   reserved1;      // v1r1�汾����չ��ռ�ã������ԣ���װfactory_mode����
    HI_U8   eqt_result;     // ��װ����������0��1λ��ʾ���߹�װ����2��3λ��ʾ������װ����λ��ֵ:
                            // 00:Ĭ��ֵ
                            // 01:��װ���Ժϸ�
                            // 10:��װ���Բ��ϸ�
                            // 11:��Чֵ
    HI_U16  DI_v97;         // �׿���ַ��Ӧ���ѱ�DIֵ(97����Ч)
    HI_S8   duty_cycle_index;//ռ�ձ���ֵ����������NV���õ���Ч����ʱ��ΪĬ��ռ�ձȲ��ԡ�Ĭ��ֵ-1
    HI_U8   baudrate_index; // �׿����������
    HI_U8  isTsta;
    HI_U8   reserved[7];    // �����ֶΣ�Ĭ��Ϊ0
} HI_NV_FTM_COLLECT_PARAM_FILE_S;


typedef struct
{
    HI_U32 reserved[16];
} HI_NV_FTM_RESERVED1_ID_S;

typedef struct
{
    HI_U32 reserved[16];
} HI_NV_FTM_RESERVED2_ID_S;

typedef struct
{
    HI_U32 reserved[16];
} HI_NV_FTM_RESERVED3_ID_S;

typedef struct
{
    HI_U32 reserved[16];
} HI_NV_FTM_RESERVED4_ID_S;

typedef struct
{
    HI_U8 ucDevAddr[6];     // CCO�豸��ַ
    HI_U8 ucLrMode;         // �ֳ�ģʽ: 0-����ģʽ(Ĭ��) 1-����ģʽ
    HI_U8 ucSeqCtrl;        // �ӽڵ����к�
    HI_U8 ucFuncCfg;        //�ط��汾��Լ��0:ͨ�ð汾;1:���ղ���������Ӧ�汾;����Ԥ��
    HI_U8 ucMaxByteIntervalTimeBbu;     // ʱ��= ��ֵ * 10ms
    HI_U8 ucFileFlg;        //�ļ���ʶ �㶫�汾ʹ��
    HI_U8 ucUpgId;         // �����ļ�ID ��㶫�汾����
    HI_U8 ucFileId;     // �����ļ�ID �㶫�汾�������·�������ID
    HI_U8 pad1;
    HI_U16 pad2;
    HI_U32 reserved[12];
} HI_NV_FTM_CCO_PARAM_ID_S;

typedef struct
{
    HI_U8 management_id[24];      // оƬid
    HI_U8 dev_id[11];             // �豸id  MRS_DEV_ID_SIZE
    HI_U8 pad;
    HI_U32 reserved[7];
}HI_NV_FTM_MANAGEMENT_ID_S;

typedef struct
{
    HI_U32 reserved[16];
}HI_NV_FTM_RESERVED9_S;
//*****************************************************************************


//*****************************************************************************
#endif //__FT_NV_H__


