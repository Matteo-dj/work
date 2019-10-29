/**
 ******************************************************************************
 * @file       typedo.h
 * @brief 
 * @details
 * @copyright
 *
 ******************************************************************************
 */

#ifndef TYPEDO_H_
#define TYPEDO_H_

/*------------------------------------------------------------------------------
Section: Includes
------------------------------------------------------------------------------*/
#include "hi_types.h"

/*------------------------------------------------------------------------------
Section: Macro Definitions
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
Section: Function Prototypes
------------------------------------------------------------------------------*/
HI_U16 take_uint8(const HI_U8* take_ptr, HI_U16* offset, HI_U8* value);
HI_U16 load_uint8(HI_U8* load_ptr, HI_U16* offset, HI_U8 value);

HI_U16 take_uint16(const HI_U8* take_ptr, HI_U16* offset, HI_U16* value);
HI_U16 load_uint16(HI_U8* load_ptr, HI_U16* offset, HI_U16 value);

HI_U16 take_uint32(const HI_U8* take_ptr, HI_U16* offset, HI_U32* value);
HI_U16 load_uint32(HI_U8* load_ptr, HI_U16* offset, HI_U32 value);

HI_U16 take_buf(const HI_U8* take_ptr, HI_U16* offset, void* buf, HI_U16 len);
HI_U16 load_buf(HI_U8* load_ptr, HI_U16* offset, const void* buf, HI_U16 len);

#endif /* TYPEDO_H_ */
