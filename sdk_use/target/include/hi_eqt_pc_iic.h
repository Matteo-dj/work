#ifndef _HI_EQT_PC_IIC_H_
#define _HI_EQT_PC_IIC_H_

#include "hi_types.h"

// ���㼰Ӳ���汾�Ƿ����
#define EQUIP_PC_STATUS_CAPABILITY_DEFAULT  (0x00)
#define EQUIP_PC_STATUS_CAPABILITY_ZERO_CROSS (0x01)       
#define EQUIP_PC_STATUS_CAPABILITY_HW_VER_ID     (0x02)

#define IIC_ASSET_NUM_COMPST_LENGTH_MAX     (12)

// ̽�ⱨ������

typedef struct _EQUIP_II_STATUS_STRU_
{ 
    HI_U8 Id;                  // ��λID
    HI_U8 ProductKind;         // ��Ʒ��̬
    HI_U8 EquipMode;           // ��װ����
    HI_U8 ucRsv;
    HI_CHAR Version[48]; // "V100R001C00B020SPC001 2011-08-01 14:30:26"
    HI_U8 ucRsv1[12];
} EQUIP_II_STATUS_STRU;

// �����������ս������
typedef struct _EQUIP_II_PRODCHECK_TEST_REPORT_STRU_
{ 
    HI_U8 assetNumWrt;        // �ʲ����д��
    HI_U8 IrTest;             // ����
    volatile HI_U8 LedTest;   // LED
    HI_U8 PlcTestRet;         // �ز�����
    HI_U8 modePowerModify;    // NV�޸ģ���ģʽ�����书�ʺ��ѱ���NV�޸��Ƿ�ɹ�
    HI_U8 usVersionResult;    // 0�������ԣ�1���ɹ���2��ʧ��
    HI_U8 usIIMACResult;      // 0�������ԣ�1���ɹ���2��ʧ��
    HI_U8 ucHwVerRet;
    HI_U32 powerUA;
    HI_U8 cZeroCrossRet;
    HI_S8 cZeroCrossValue;
    HI_U8 ucRsv[18];
}EQUIP_II_PRODCHECK_TEST_REPORT_STRU;


#define EQUIP_IR_VERSION_LEN_MAX (32)

// ���߹�װ���ս������
typedef struct _EQUIP_II_TEST_REPORT_STRU_
{ 
    HI_U8 IrTest;            // ����
    HI_U8 LedTest;          // LED
    HI_U8 PlcTestRet;         // �ز�����
    HI_U8 modePowerModify;    // NV�޸ģ���ģʽ�����书�ʺ��ѱ���NV�޸��Ƿ�ɹ�
    HI_U8 ucTestNum;            // ����ִ�и���
    HI_U8 ucRsv[3];
    HI_U8 irVersion[EQUIP_IR_VERSION_LEN_MAX];       //�����ϱ��汾��Ϣ    
    HI_U8 ucRsv1[8];
}EQUIP_II_TEST_REPORT_STRU; 

// ɨ���������

typedef struct _EQUIP_II_STATUS_ACK_STRU_
{ 
    HI_U8 CompensateHead[IIC_ASSET_NUM_COMPST_LENGTH_MAX];           //ǰ׺
    HI_U8 CompensateTail[IIC_ASSET_NUM_COMPST_LENGTH_MAX];           //��׺
    HI_U8 UsedInHead;                                 //ǰ׺����λ��Ч
    HI_U8 UsedInTail;                                 //��׺����λ��Ч
    HI_U8 ScanOrNot;                                  //�Ƿ�ɨ������ 1 ɨ 0 ��ɨ
    HI_U8 StartPosition;
    HI_U8 EndPosition;
    HI_U8 totalLen;
    HI_U8 ret;
    HI_U8 ucCapability;
    HI_U8 ucHwVerId;
    HI_U8 ucRsv1;
    HI_U16 usMinThreshold240K;  
	HI_U16 usMaxThreshold240K;
	HI_U16 usMinThreshold1M; 
	HI_U16 usMaxThreshold1M; 
	HI_U16 usLevelThreshold;
    HI_U8  ucRsv2[20]; 

} EQUIP_II_STATUS_ACK_STRU;

#endif