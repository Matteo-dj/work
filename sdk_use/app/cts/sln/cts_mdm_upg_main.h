#ifndef _CTS_MDM_UPG_MAIN_H
#define _CTS_MDM_UPG_MAIN_H

#define UART_PORT_NUM0 0   //�ٿ������Կ�Ϊ����0������Ϊ����1

typedef struct
{
    HI_U32   ulUpgId;         //�����ȡ��������ʱ���û�ID
    HI_U32   ulObtainFileSize;//�û�����������ļ���С
    HI_U32   ulRealFileSize;  //�û���ǰʵ�ʻ����С
    HI_PVOID pUpgObtainBuf;   //��ȡ�������������ַ
} CTS_UPG_OBTAIN_MEM_CTX; //���������������������

typedef struct
{
    HI_U16 buf_len; //���ڻ���������
    HI_U8 reserved[2];
    HI_U8 *buffer; //���ڻ�����
}CTS_UART_DATA_STRU;

//��ʼ������
HI_PUBLIC HI_VOID cts_InitMdmUpgObj(HI_VOID);

//��ȡ�ļ�������ɱ�־
HI_PUBLIC HI_BOOL upgCtsGetLoadFileFinishFlag(HI_VOID);

//���������ļ�У���
HI_PUBLIC HI_U16 upgCtsCalcCheckSum(HI_PBYTE ptr, HI_U16 len);

//��ȡ����������Ϣ������
HI_PUBLIC CTS_UPG_OBTAIN_MEM_CTX * upgGetMemObtainCtx(HI_VOID);

HI_VOID upgCtsSetLdFileFinishFlag(HI_BOOL flag);

#endif
