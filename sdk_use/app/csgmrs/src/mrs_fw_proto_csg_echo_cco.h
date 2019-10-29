//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2014, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_fw_proto_csg_echo_cco.h
//  �� �� ��   : V1.0
//  ��    ��   : cuiate/c00233580
//  ��������   : 2015-12-25
//  ��������   : TODO: ...
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2015-12-25
//    ��    �� : cuiate/c00233580
//    �޸����� : �����ļ�
//
//*****************************************************************************

#ifndef _MRS_FW_PROTO_CSG_ECHO_CCO_H_
#define _MRS_FW_PROTO_CSG_ECHO_CCO_H_
HI_START_HEADER
#include "hi_equip.h"
#include "equip_dut_proc.h"
#include "mrs_fw_proto_csg_echo.h"

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))

#define MRS_BC_DELAY_TIME                   (1)     // 1s
#define MRS_UPG_WAIT_TIME                   (60)    // 10s
#define MRS_MAX_FRAME_LEN                   (1024)  // ���֧�ֵı��ĳ���
#define MRS_CSG_PROTO_RELEASE_DATE          "\x01\x02\x16"

#define MRS_CSG_FRAME_MAX_LENGTH_OFFSET     (1)     // ���֧��Э�鱨�ĳ���
#define MRS_CSG_MAX_PACKET_LENGTH_OFFSET    (3)     // �ļ�������󵥰�����
#define MRS_CSG_UPG_WAIT_TIME_OFFSET        (5)     // �����ȴ�ʱ��
#define MRS_CSG_CCO_ADDR_OFFSET             (6)     // ���ڵ��ַ
#define MRS_CSG_STATION_MAX_NUM_OFFSET      (12)    // ֧�ֵ����ӽڵ�����
#define MRS_CSG_STATION_NUM_OFFSET          (14)    // ��ǰ�ӽڵ�����
#define MRS_CSG_NODE_MAX_NUM_OFFSET         (16)    // ֧�ֵ��ζ�д�ӽڵ���Ϣ���������
#define MRS_CSG_PROTO_RELEASE_OFFSET        (18)    // ����ʱ��
#define MRS_CSG_DEVICE_INFO_OFFSET          (21)    // ���̴���Ͱ汾��Ϣ
#define MRS_CSG_COMMUNICATION_UP_DATA_LEN   (30)

#define MRS_CSG_FILES_INFOS_FIX_LEN         (18)
#define MRS_CSG_FILES_FAILED_FIX_LEN        (3)



HI_PUBLIC HI_U32 mrsCcoProtoCsgEchoInit(HI_VOID);

HI_PUBLIC HI_U32 mrsCcoProtoCsgEchoDeinit(HI_VOID);

HI_U32 makeBuf_AFN03_E8000301(HI_OUT HI_U8 * pucBuf, HI_INOUT HI_U16 * pusBufLen);
HI_U32 makeBuf_AFN03_E8000302(HI_OUT HI_U8 ** ppaucOutBuf, HI_OUT HI_U16 * pusOutLen);

HI_VOID mrsCsgQueueFree(HI_VOID *p);
HI_VOID mrsCsgListFree(HI_VOID *p, HI_VOID *pParam);

HI_U32 mrsCsgIsMeterRepeat(HI_U8 *pucContent, HI_U8 ucMeterIndex, HI_U8 ucMeterNum);
HI_U32 mrsCsgNeedInsertNode(HI_U8 *pucContent, HI_U8 ucMeterNum, HI_U8 *ucDeny);

HI_U32 mrsCsgGetFileBuffer(HI_U8 **ppBuffer, HI_U16 *pusLength);
HI_U32 mrsCsgGetFileProcess(HI_U8 **ppucBuffer, HI_U16 *pusLength);
HI_U32 mrsCsgGetFileFailedInfo(HI_U16 usStartNo, HI_U8 ucNodesCnt, HI_U8 **ppucBuffer, HI_U16 *pusLength);
HI_U32 mrsCsgPlcTestProc(HI_PVOID pParam, HI_U16 usSize);

// AFN00 F1 E8010001: ȷ��
HI_U32 mrsCsg_AFN00_E8010001(HI_INOUT HI_VOID *pParam);

// AFN00 F2: ����
HI_U32 mrsCsg_AFN00_E8010002(HI_INOUT HI_VOID *pParam);


// AFN01 F1: Ӳ����ʼ��
HI_U32 mrsCsg_AFN01_E8020101(HI_INOUT HI_VOID *pParam);

// AFN01 F2: ��ʼ������
HI_U32 mrsCsg_AFN01_E8020102(HI_INOUT HI_VOID *pParam);

// AFN01 F3: ��ʼ������
HI_U32 mrsCsg_AFN01_E8020103(HI_INOUT HI_VOID *pParam);


// �������
HI_U32 mrsCsg_AFN02_E8020201(HI_INOUT HI_VOID *pParam);

// ɾ������
HI_U32 mrsCsg_AFN02_E8020202(HI_INOUT HI_VOID *pParam);

// ��ѯδ�������
HI_U32 mrsCsg_AFN02_E8000203(HI_INOUT HI_VOID *pParam);

// ��ѯδ��������б�
HI_U32 mrsCsg_AFN02_E8030204(HI_INOUT HI_VOID *pParam);

// ��ѯδ���������ϸ��Ϣ
HI_U32 mrsCsg_AFN02_E8030205(HI_INOUT HI_VOID *pParam);

// ��ѯʣ��ɷ���������
HI_U32 mrsCsg_AFN02_E8000206(HI_INOUT HI_VOID *pParam);

// ��Ӷಥ����
HI_U32 mrsCsg_AFN02_E8020207(HI_INOUT HI_VOID *pParam);

// ��������
HI_U32 mrsCsg_AFN02_E8020208(HI_INOUT HI_VOID *pParam);

// ��ͣ����
HI_U32 mrsCsg_AFN02_E8020209(HI_INOUT HI_VOID *pParam);


// AFN03 F1:��ѯ���Ҵ���Ͱ汾��
HI_U32 mrsCsg_AFN03_E8000301(HI_INOUT HI_VOID *pParam);

// AFN03 DIE8000302:��ѯ����ͨ��ģ������ģʽ��Ϣ
HI_U32 mrsCsg_AFN03_E8000302(HI_INOUT HI_VOID *pParam);

// �����ϱ�������ͨ��ģ������ģʽ��Ϣ��
HI_U32 mrsCsg_AFN03_E8000302_UP(HI_VOID);

// E8 00 03 03����ѯ���ڵ��ַ
HI_U32 mrsCsg_AFN03_E8000303(HI_INOUT HI_VOID *pParam);

// E8 03 03 04����ѯͨѶ�ӳ�ʱ��
HI_U32 mrsCsg_AFN03_E8030304(HI_INOUT HI_VOID *pParam);

// E8 00 03 05����ѯ�ӽڵ�����
HI_U32 mrsCsg_AFN03_E8000305(HI_INOUT HI_VOID *pParam);

// AFN03 F3���ӽڵ���Ϣ
HI_U32 mrsCsg_AFN03_E8030306(HI_INOUT HI_VOID *pParam);

// AFN03 F7���ӽڵ�����ע�����
HI_U32 mrsCsg_AFN03_E8000307(HI_INOUT HI_VOID *pParam);

// AFN03 F8���ӽڵ�ĸ��ڵ�
HI_U32 mrsCsg_AFN03_E8030308(HI_INOUT HI_VOID *pParam);


// �������ڵ��ַ E8020401
HI_U32 mrsCsg_AFN04_E8020401(HI_INOUT HI_VOID *pParam);

// ��Ӵӽڵ��ַ E8020402
HI_U32 mrsCsg_AFN04_E8020402(HI_INOUT HI_VOID *pParam);

// ɾ���ӽڵ��ַ E8020403
HI_U32 mrsCsg_AFN04_E8020403(HI_INOUT HI_VOID *pParam);

// ����/��ֹ�ӽڵ��¼� E8020404   �ϵ�� Ĭ��Ϊ����
HI_U32 mrsCsg_AFN04_E8020404(HI_INOUT HI_VOID *pParam);

// ����ӽڵ�����ע�� E8020405
HI_U32 mrsCsg_AFN04_E8020405(HI_INOUT HI_VOID *pParam);

// ��ֹ�ӽڵ�����ע�� E8020406
HI_U32 mrsCsg_AFN04_E8020406(HI_INOUT HI_VOID *pParam);


//��������ʱ�� E8060601
HI_U32 mrsCsg_AFN06_E8060601(HI_INOUT HI_VOID *pParam);


// �����ļ����� E8020701
HI_U32 mrsCsg_AFN07_E8020701(HI_INOUT HI_VOID *pParam);

// �����ļ����� E8020702
HI_U32 mrsCsg_AFN07_E8020702(HI_INOUT HI_VOID *pParam);

// ��ѯ�ļ���Ϣ E8030703
HI_U32 mrsCsg_AFN07_E8000703(HI_INOUT HI_VOID *pParam);

// ��ѯ�ļ�������� E8000704
HI_U32 mrsCsg_AFN07_E8000704(HI_INOUT HI_VOID *pParam);

//��ѯ�ļ�����ʧ�ܽڵ�
HI_U32 mrsCsg_AFN07_E8030705(HI_INOUT HI_VOID * pParam);

//PLC�����Ȳ���
HI_U32 mrsCsg_AFNF0_E803F001(HI_INOUT HI_VOID * pParam);

//������
HI_U32 mrsCsg_AFNF0_E803F002(HI_INOUT HI_VOID * pParam);


#endif

HI_END_HEADER
#endif

