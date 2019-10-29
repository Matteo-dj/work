
#ifndef __DFX_BATTERY_H__
#define __DFX_BATTERY_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

typedef enum
{
    BQ_UPG_FAIL_REASON_NO,
    BQ_UPG_FAIL_REASON_ID_FILE_SIZE,            //�����ļ���С���ʧ��
    BQ_UPG_FAIL_REASON_ID_FILE_CONTENT,         //�����ļ����ݼ��ʧ��
    BQ_UPG_FAIL_REASON_ID_CRC,                  //CRCУ�����
    BQ_UPG_FAIL_REASON_ID_OVER_TIME,            //��ʱ
    BQ_UPG_FAIL_REASON_ID_POWER_OFF,            //�쳣�ϵ�
    BQ_UPG_FAIL_REASON_ID_CHANGE_MOD_FAIL,      //�л�������ģʽʧ��
    BQ_UPG_FAIL_REASON_ID_I2C,                  //��BQоƬ��������
    BQ_UPG_FAIL_REASON_WORK_QUEUE_BUSY,
}BQ_UPG_FAIL_REASON_ID_E;//����ʧ�ܾ���ԭ��ͳ����


typedef struct
{
    //������ز���
    HI_U16 mainVer;     //BQоƬ���汾��
    HI_U16 subVer;      //���оƬfirmwareоƬ�����İ汾���
    HI_U32 ulCheckSum;  //���оƬfirmware������CRC32
    HI_U16 usUpgStatus; //���оƬfirmware����״̬  HI_NDM_BAT_UPG_STATUS_E
    HI_U16 usErrCode;   //���оƬ��ǰ�Ƿ���������  HI_NDM_BAT_UPG_ERR_CODE_E

    //���ʵʱ״̬
    HI_U16 voltValue;//��ص�ѹ(mv)
    HI_U16 temperature;//����¶�(k)
    HI_S16 current;//��ص���(mA)�ŵ�Ϊ���������Ϊ����
    HI_BOOL isBatExist;
    HI_U8 ucBatteryPercent;//�ȼ��ڵ����λ&&��ذ汾��ȷ;���ص���<=100%ͬʱ����ʱ�ϱ���HSO��ص���

    //��س�ʼ��״̬
    HI_U16 initVoltValue;
    HI_U16 initTemperature;
    HI_S16 initCurrent;
    HI_BOOL initIsBatExist;
    HI_U8 initBatPercent;

    //����������Ϣ
    HI_BOOL isInit;//�Ƿ��ʼ����� HI_TRUE��ʼ�����
    HI_BOOL isResetting;// HI_TURE:����������
    HI_U8 pad_1[2];

    //���Բ���ͳ����
    HI_U32 currentExeLine;
    HI_U8 bqUpgFailReason;//BQ_UPG_FAIL_REASON_ID_E
    HI_U8 bqUpgNotAllowReason;//BQ_UPG_NOT_ALLOW_ID_E
    HI_U8 writeNvFailTimes;
    HI_U8 readNvFailTimes;
    HI_U8 readFirmWareVersionFail;
    HI_U8 readTemperatureFailTimes;
    HI_U8 readVoltValFailTimes;
    HI_U8 readCurrentFailTimes;

    HI_U8 readBatPercentFailTimes;
    HI_U8 isSupportBq;
    HI_U8 pad_2[2];
}BQ27510_STATUS_S;

#endif

