/**
 ******************************************************************************
 * @file      typedo.c
 * @brief    
 * @details
 * @copy 
 ******************************************************************************
 */
#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined (PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_CCT3911)

/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

#include "securec.h"
#include "typedo.h"

/*-----------------------------------------------------------------------------
 Section: Define
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 Section: static function
 ----------------------------------------------------------------------------*/
/**
 ******************************************************************************
 * @brief      ��ȡHI_U8
 * @param[in]  const HI_U8* take_ptr          ��ȡָ��
 * @param[io]  HI_U16* offset           ��ȡλ��ƫ��
 * @param[out] HI_U8* value        ֵ
 * @retval HI_U16 ������
 ******************************************************************************
 */
HI_U16 take_uint8(const HI_U8* take_ptr, HI_U16* offset, HI_U8* value)
{
    HI_U16 ptr_offset = offset == NULL ? 0 : *offset;
    HI_U16 pre_ptr_offset = ptr_offset;
    HI_U8 value_swap = take_ptr[ptr_offset++];
    if (value != NULL){*value = value_swap;}
    if (offset != NULL){*offset = ptr_offset;}
    return ptr_offset - pre_ptr_offset;
}

/**
 ******************************************************************************
 * @brief      װ��HI_U8
 * @param[in]  HI_U8* load_ptr          װ��ָ��
 * @param[io]  HI_U16* offset           װ��λ��ƫ��
 * @param[in]  HI_U8 value         ֵ
 * @retval HI_U16 ������
 ******************************************************************************
 */
HI_U16 load_uint8(HI_U8* load_ptr, HI_U16* offset, HI_U8 value)
{
    HI_U16 ptr_offset = offset == NULL ? 0 : *offset;
    HI_U16 pre_ptr_offset = ptr_offset;
    load_ptr[ptr_offset ++]= value;
    if (offset != NULL){*offset = ptr_offset;}
    return ptr_offset - pre_ptr_offset;
}

/**
 ******************************************************************************
 * @brief      ��ȡHI_U16
 * @param[in]  const HI_U8* take_ptr          ��ȡָ��
 * @param[io]  HI_U16* offset           ��ȡλ��ƫ��
 * @param[out] HI_U16* value         ֵ
 * @retval HI_U16 ������
 ******************************************************************************
 */
HI_U16 take_uint16(const HI_U8* take_ptr, HI_U16* offset, HI_U16* value)
{
    HI_U16 ptr_offset = offset == NULL ? 0 : *offset;
    HI_U16 pre_ptr_offset = ptr_offset;
    HI_U16 value_swap;
    (hi_void)memcpy_s(&value_swap, sizeof(HI_U16), take_ptr + ptr_offset, sizeof(HI_U16));
    ptr_offset += sizeof(HI_U16);
    if (value != NULL){*value = value_swap;}
    if (offset != NULL){*offset = ptr_offset;}
    return ptr_offset - pre_ptr_offset;
}

/**
 ******************************************************************************
 * @brief      װ��HI_U16
 * @param[in]  HI_U8* load_ptr          װ��ָ��
 * @param[io]  HI_U16* offset           װ��λ��ƫ��
 * @param[in]  HI_U16 value          ֵ
 * @retval HI_U16 ������
 ******************************************************************************
 */
HI_U16 load_uint16(HI_U8* load_ptr, HI_U16* offset, HI_U16 value)
{
    HI_U16 ptr_offset = offset == NULL ? 0 : *offset;
    HI_U16 pre_ptr_offset = ptr_offset;
    (hi_void)memcpy_s(load_ptr + ptr_offset, sizeof(HI_U16), &value, sizeof(HI_U16));
    ptr_offset += sizeof(HI_U16);
    if (offset != NULL){*offset = ptr_offset;}
    return ptr_offset - pre_ptr_offset;
}

/**
 ******************************************************************************
 * @brief      ��ȡHI_U32
 * @param[in]  const HI_U8* take_ptr          ��ȡָ��
 * @param[io]  HI_U16* offset           ��ȡλ��ƫ��
 * @param[out] HI_U32* value        ֵ
 * @retval HI_U16 ������
 ******************************************************************************
 */
HI_U16 take_uint32(const HI_U8* take_ptr, HI_U16* offset, HI_U32* value)
{
    HI_U16 ptr_offset = offset == NULL ? 0 : *offset;
    HI_U16 pre_ptr_offset = ptr_offset;
    HI_U32 value_swap;
    (hi_void)memcpy_s(&value_swap, sizeof(HI_U32), take_ptr + ptr_offset, sizeof(HI_U32));
    ptr_offset += sizeof(HI_U32);
    if (value != NULL){*value = value_swap;}
    if (offset != NULL){*offset = ptr_offset;}
    return ptr_offset - pre_ptr_offset;
}

/**
 ******************************************************************************
 * @brief      װ��HI_U32
 * @param[in]  HI_U8* load_ptr      װ��ָ�룬����װ��λ��
 * @param[io]  HI_U16* offset           ��ȡλ��ƫ��
 * @param[out] HI_U32 value        ֵ
 * @retval HI_U16 ������
 ******************************************************************************
 */
HI_U16 load_uint32(HI_U8* load_ptr, HI_U16* offset, HI_U32 value)
{
    HI_U16 ptr_offset = offset == NULL ? 0 : *offset;
    HI_U16 pre_ptr_offset = ptr_offset;
    (hi_void)memcpy_s(load_ptr + ptr_offset, sizeof(HI_U32), &value, sizeof(HI_U32));
    ptr_offset += sizeof(HI_U32);
    if (offset != NULL){*offset = ptr_offset;}
    return ptr_offset - pre_ptr_offset;
}

/**
 ******************************************************************************
 * @brief      ��ȡbuf
 * @param[in]  const HI_U8_t* take_ptr      ��ȡָ��
 * @param[io]  HI_U16_t* offset       ��ȡλ��ƫ��
 * @param[out] void* buf
 * @param[in]  HI_U16 len       buf����
 * @retval HI_U16 ������
 ******************************************************************************
 */
HI_U16 take_buf(const HI_U8* take_ptr, HI_U16* offset, void* buf, HI_U16 len)
{
    HI_U16 ptr_offset = offset == NULL ? 0 : *offset;
    HI_U16 pre_ptr_offset = ptr_offset;
    if (buf != NULL){(hi_void)memmove_s((HI_U8*)buf, len, take_ptr + ptr_offset, len);}
    ptr_offset += len;
    if (offset != NULL){*offset = ptr_offset;}
    return ptr_offset - pre_ptr_offset;
}

/**
 ******************************************************************************
 * @brief      װ��buf
 * @param[in]  HI_U8* load_ptr      װ��ָ��
 * @param[io]  HI_U16* offset       װ��λ��ƫ��
 * @param[in]  const void* buf
 * @param[in]  HI_U16 len       buf����
 * @retval HI_U16_t ������
 ******************************************************************************
 */
HI_U16 load_buf(HI_U8* load_ptr, HI_U16* offset, const void* buf, HI_U16 len)
{
    HI_U16 ptr_offset = offset == NULL ? 0 : *offset;
    HI_U16 pre_ptr_offset = ptr_offset;
    if (buf != NULL){(hi_void)memmove_s(load_ptr + ptr_offset, len, (HI_U8*)buf, len);}
    ptr_offset += len;
    if (offset != NULL){*offset = ptr_offset;}
    return ptr_offset - pre_ptr_offset;
}

/*---------------------------End of typedo.c--------------------------------*/
#endif