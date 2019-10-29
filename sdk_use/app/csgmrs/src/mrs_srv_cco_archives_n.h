//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2014, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_cco_archives_n.h
//  �� �� ��   : V1.0 
//  ��    ��   : cuiate/c00233580
//  ��������   : 2014-10-30
//  ��������   : ��������ģ���ڲ��ӿ������ͺ궨��
//
//  �����б�   : 
//  �޸���ʷ   : 
//  1.��    �� : 2014-10-30
//    ��    �� : cuiate/c00233580
//    �޸����� : �����ļ� 
//
//*****************************************************************************

#ifndef _MRS_SRV_ARCHIVES_N_H_
#define _MRS_SRV_ARCHIVES_N_H_
HI_START_HEADER

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
//*****************************************************************************
//*****************************************************************************

#define MRS_ARCHIVES_HEAD_FLAG          (0x1A2B3C4D)
#define MRS_ARCHIVES_HEAD_VERSION       (0x1140)

#define MRS_ARCHIVES_NODE_SIZE          (12)
#define MRS_ARCHIVES_HEAD_SIZE          (MRS_ARCHIVES_NODE_SIZE)
#define MRS_METERS_PER_BLOCK            (PRODUCT_CFG_FLASH_BLOCK_SIZE / MRS_ARCHIVES_NODE_SIZE)
#define MRS_SPARE_PER_BLOCK             (PRODUCT_CFG_FLASH_BLOCK_SIZE % MRS_ARCHIVES_NODE_SIZE)

#define MRS_ARCHIVES_FLASH_SIZE         (PRODUCT_CFG_USERDATA_CCO_APP_MAX_SIZE - PRODUCT_CFG_FLASH_BLOCK_SIZE)
#define MRS_ARCHIVES_BLOCK_CNT          ((PRODUCT_CFG_MRS_MAX_NODE_NUM + 2 + MRS_METERS_PER_BLOCK - 1) / MRS_METERS_PER_BLOCK)

#define MRS_ARCHIVES_HEAD2_SEQ          (1024)
#define MRS_ARCHIVES_HEAD2_BLOCK        ((MRS_ARCHIVES_HEAD2_SEQ + 1) / MRS_METERS_PER_BLOCK)
#define MRS_ARCHIVES_HEAD2_INDEX        ((MRS_ARCHIVES_HEAD2_SEQ + 1) % MRS_METERS_PER_BLOCK)


//*****************************************************************************
//*****************************************************************************

typedef struct 
{
    HI_U16  usMeterNum; // ��ǰ�ӽڵ�(���)����
    HI_BOOL bChanged;   // ���������ʶ
    HI_BOOL bInited;    // ��ʼ����ʶ
    HI_PVOID pMeterInf; // �ڴ���������Ĵӽڵ�(���)ָ��ͷ��Ϣ
                        // �ڵ�����MRS_ARCHIVES_NODE_STRU
} MRS_METER_ARCHIVES_STRU;


typedef struct  
{
    HI_U32  ulFlag;         // �ļ���ʶ(MRS_ARCHIVES_HEAD_FLAG)
    HI_U16  usVersion;      // �汾��Ϣ(MRS_ARCHIVES_HEAD_VERSION)
    HI_U16  usNodeMax;      // ���֧�ֵĽڵ�����
} MRS_ARCHIVES_HEAD_S;


typedef enum
{
    MRS_ARCHIVES_FILE_VER_R0,
    MRS_ARCHIVES_FILE_VER_R1,
} MRS_ARCHIVES_FILE_VERSION_E;


typedef struct  
{
    HI_U8   ucVersion;      // �汾��Ϣ
    HI_U8   aucReserved[7];
    HI_U16  usNodeMax;      // ���֧�ֵĽڵ�����
    HI_U16  usNodeCur;      // ��ǰ�ڵ�����
} MRS_ARCHIVES_HEAD2_S;


// ÿ��FLASH�ϴ洢�ı��ַ�����Ϣ
typedef struct  
{
    HI_U16  usMin;          // �����Сֵ
    HI_U16  usMax;          // ������ֵ
} MRS_ARCHIVES_SEQ_NODE_S;


// FLASH�ϴ洢�ı��ַ�����Ϣ��
typedef struct
{
    HI_BOOL bInited;
    HI_U8   ucCount;
    HI_U8   ucReserved[2];
    MRS_ARCHIVES_SEQ_NODE_S stSeq[MRS_ARCHIVES_BLOCK_CNT];
} MRS_ARCHIVES_SEQ_TABLE_S;



//*****************************************************************************
// ��������: mrsGetArchivesCtx
// ��������: ���ر��������Ľṹ
// ����˵��: ��
// �� �� ֵ: ����������ָ��
// ��    ��: cuiate/c00233580 [2014-10-30]
//*****************************************************************************
MRS_METER_ARCHIVES_STRU *mrsArchivesCtx(HI_VOID);

//*****************************************************************************
// ��������: mrsArchivesCheckHead
// ��������: �������ļ�ͷ
// ����˵��: ��
// �� �� ֵ: 
//   �ɹ�: HI_ERR_SUCCESS
//   ʧ��: ����ֵ
// ��    ��: cuiate/c00233580 [2014-10-31]
//*****************************************************************************
HI_U32 mrsArchivesCheckHead(HI_VOID);

//*****************************************************************************
// ��������: mrsLoadMeterFromFlash
// ��������: ����FLASH���еı��ַ�������б�
// ����˵��: 
//   ulBlockId     [IN]    �洢������FLASH���,��0��ʼ
//   pBuffer       [IN]    Ԥ����õ�4KB���ڴ�
//   pusMeterRemain[INOUT] ��δ���صı�����
// �� �� ֵ:
//   �ɹ�: HI_ERR_SUCCESS
//   ʧ��: ����ֵ
// ��    ��: cuiate/c00233580 [2014-10-31]
//*****************************************************************************
HI_U32 mrsLoadMeterFromFlash(HI_U32 ulBlockId, HI_PBYTE pBuffer, HI_U16 *pusMeterTotal);

//*****************************************************************************
// ��������: mrsWriteMeterToFlash
// ��������: д�����б�FLASH����
// ����˵��: 
//   ulBlockId     [IN]    �洢������FLASH���,��0��ʼ
//   pBuffer       [IN]    Ԥ����õ�4KB���ڴ�
//
// �� �� ֵ:
//   �ɹ�: HI_ERR_SUCCESS
//   ʧ��: ����ֵ
// ��    ��: cuiate/c00233580 [2014-10-31]
//*****************************************************************************
HI_U32 mrsWriteMeterToFlash(HI_U32 ulBlockId, HI_PBYTE pBuffer);

//*****************************************************************************
// ��������: mrsMeterFileSave2ndHead
// ��������: �洢�ļ�ͷ2
// ����˵��: 
//   pstHead2      [IN] �ļ�ͷ2����Ϣ
//   pBuffer       [IN] Ԥ�ȷ���õ�4KB���ڴ�
//
// �� �� ֵ: ��
// ��    ��: cuiate/c00233580 [2014-10-31]
//*****************************************************************************
HI_VOID mrsArchivesSaveHead2(MRS_ARCHIVES_HEAD2_S *pstHead2, HI_U8 *pBuffer);

//*****************************************************************************
// ��������: mrsArchivesIsNullBlock
// ��������: �ж������Ƿ�ΪȫFF
// ����˵��: 
//   pBuffer       [IN] FLASH������ָ��
//
// �� �� ֵ: ��
// ��    ��: cuiate/c00233580 [2014-10-31]
//*****************************************************************************
HI_BOOL mrsArchivesIsNullBlock(HI_PBYTE pBuffer);


//*****************************************************************************
//*****************************************************************************
#endif //defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

HI_END_HEADER
#endif //_MRS_SRV_ARCHIVES_N_H_
