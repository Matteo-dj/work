//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2014, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_fixed_mem.h
//  �� �� ��   : V1.0 
//  ��    ��   : cuiate/c00233580
//  ��������   : 2014-10-22
//  ��������   : �ڴ���������
//               �����������궨��
//               
//      ��ģ�����Ҫ�������ṩһ��ӿڣ��Թ��ϴ���ڴ��ȷ���һ���ڴ棬������
//  ��ʵ��ʹ����������������䣬���ṩ���ڵ�����ڴ�Ľӿڡ�
//               
//  �����б�   : 
//  �޸���ʷ   : 
//  1.��    �� : 2014-10-22
//    ��    �� : cuiate/c00233580
//    �޸����� : �����ļ� 
//
//*****************************************************************************

#ifndef _MRS_MEM_INCREMENT_MANAGE_H_
#define _MRS_MEM_INCREMENT_MANAGE_H_
HI_START_HEADER

// �ò��ִ���Ŀǰֻ��CCO���ã�������CCO��ָ�
// ��������Ʒ��̬Ҫ���ã�ȥ��CCO��������Ӷ�Ӧ�Ĳ�Ʒ��̬�꼴��
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
//*****************************************************************************
//*****************************************************************************

#define MRS_SRV_FIXED_MEM_INIT_COUNT    (1024)
#define MRS_SRV_FIXED_MEM_INC_COUNT     (128)
#define MRS_SRV_ARCHIVES_DFX_INDEX      (0)


typedef struct
{
    HI_U32 ulInitItemCnt;   // ��ʼ�����ڴ�Ľڵ�����
    HI_U32 ulIncItemCnt;    // ���������ڴ�Ľڵ�����
    HI_U32 ulMaxItemCnt;    // �ڵ����������ֵ
    HI_U32 ulItemSize;      // �ڵ��С
} MRS_FIXED_MEM_PRAMS_STRU;

typedef struct
{
    MRS_SRV_LINK    stLink;
    HI_U32          ulItemCount;
    HI_U8           pucMemBuf[0];
} MRS_FIXED_MEM_ITEM_STRU;

typedef struct
{
    HI_U32 ulAllocItemCnt;
    MRS_FIXED_MEM_PRAMS_STRU stParam;
    MRS_SRV_LIST stMemList;
} MRS_FIXED_MEM_HEAD_STRU, *MRS_FIXED_MEM_HEAD_PTR;



//*****************************************************************************
// ��������: mrsFixedMalloc
// ��������: ���ճ�ʼ�ڵ����������ڴ棬�����������������ʵ���ڴ�ʹ������������
// ����˵��:
//   pstParam   : [IN]  �ڴ����Ĳ�����������ʼ�ڵ������ܽڵ����������ڵ�����
//                      �ڵ��С
//   ucDfxIndex : [IN]  ͳ������������(֧��0-6): 0->����������Ԥ��
// �� �� ֵ: 
//   ����ɹ�   : ���ط�����ڴ�ͷָ��ͷ
//   ����ʧ��   : HI_NULL
//
// ��    ��: cuiate/c00233580 [2014-10-30]
//*****************************************************************************
HI_PUBLIC MRS_FIXED_MEM_HEAD_PTR mrsFixedMalloc(MRS_FIXED_MEM_PRAMS_STRU * pstParam, HI_U8 ucDfxIndex);

//*****************************************************************************
// ��������: mrsFixedFree
// ��������: �ͷ��ڴ�
// ����˵��:
//   pstFixedHead [IN]  Ҫ�ͷŵ��ڴ�ָ��ͷ
//
// �� �� ֵ: ��
// ��    ��: cuiate/c00233580 [2014-10-30]
//*****************************************************************************
HI_PUBLIC HI_VOID mrsFixedFree(MRS_FIXED_MEM_HEAD_PTR pstFixedHead);

//*****************************************************************************
// ��������: mrsFixedMemClear
// ��������: �ڴ���0����
// ����˵��:
//   pstFixedHead [IN]  Ҫ��0���ڴ�ָ��ͷ
//
// �� �� ֵ: ��
// ��    ��: cuiate/c00233580 [2014-10-30]
//*****************************************************************************
HI_PUBLIC HI_VOID mrsFixedMemClear(MRS_FIXED_MEM_HEAD_PTR pstFixedHead);

//*****************************************************************************
// ��������: mrsFixedMemGetItem
// ��������: �����������ؽڵ���ڴ��ַ
// ����˵��:
//   pstFixedHead [IN]  �ڴ�ָ��ͷ
//   ulIndex      [IN]  �ڵ�����
//
// �� �� ֵ: 
//   �ɹ�: ���ؽڵ���ڴ��ַ
//   ʧ��: HI_NULL
//
// ��    ��: cuiate/c00233580 [2014-10-30]
//*****************************************************************************
HI_PUBLIC HI_VOID *mrsFixedMemGetItem(MRS_FIXED_MEM_HEAD_PTR pstFixedHead, HI_U32 ulIndex);

//*****************************************************************************
// ��������: mrsFixedMemSetItem
// ��������: �����������ýڵ�����
// ����˵��:
//   pstFixedHead [IN]  �ڴ�ָ��ͷ
//   ulIndex      [IN]  �ڵ�����
//   pstItem      [IN]  �ڵ�����
//   ucDfxIndex   [IN]  ͳ������������(֧��0-6): 0->����������Ԥ��
//
// �� �� ֵ: 
//   ���óɹ�: HI_ERR_SUCCESS
//   ����ʧ��: HI_ERR_FAILURE
//   ��������: HI_ERR_INVALID_PARAMETER
//
// ��    ��: cuiate/c00233580 [2014-10-30]
//*****************************************************************************
HI_PUBLIC HI_U32 mrsFixedMemSetItem(MRS_FIXED_MEM_HEAD_PTR pstFixedHead, HI_U32 ulIndex, HI_VOID *pstItem, HI_U8 ucDfxIndex);

#endif

HI_END_HEADER
#endif //_MRS_MEM_INCREMENT_MANAGE_H_


