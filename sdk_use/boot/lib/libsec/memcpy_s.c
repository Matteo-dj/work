/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.
* File name: memcpy_s.c
* History:
*     1. Date:
*         Author:
*         Modification:
********************************************************************************
*/

#include "securec.h"

/*******************************************************************************
* <NAME>
*    memcpy_s
*
* <SYNOPSIS>
*    errno_t memcpy_s(void *dest, size_t destMax, const void *src, size_t count)
*
* <FUNCTION DESCRIPTION>
*    memcpy_s copies count bytes from src to dest
*
* <INPUT PARAMETERS>
*    dest                       new buffer.
*    destMax                    Size of the destination buffer.
*    src                        Buffer to copy from.
*    count                      Number of characters to copy
*
* <OUTPUT PARAMETERS>
*    dest buffer                is updated.
*
* <RETURN VALUE>
*    EOK                        Success
*    EINVAL                     dest == NULL or strSrc == NULL
*    ERANGE                     count > destMax or destMax >
*                               SECUREC_MEM_MAX_LEN or destMax == 0
*    EOVERLAP_AND_RESET         dest buffer and source buffer are overlapped
*
*    if an error occured, dest will be filled with 0.
*    If the source and destination overlap, the behavior of memcpy_s is undefined.
*    Use memmove_s to handle overlapping regions.
*******************************************************************************
*/

errno_t memcpy_s(void* dest, size_t destMax, const void* src, size_t count)
{
    if (LIKELY( count <= destMax && dest && src   /*&& dest != src*/
                && destMax <= SECUREC_MEM_MAX_LEN
                && count > 0
                && ( (dest > src  &&  (void*)((UINT8T*)src  + count) <= dest) ||
                     (src  > dest &&  (void*)((UINT8T*)dest + count) <= src) )
              ) )
    {
        (void)memcpy(dest, src, count);
        return BOOT_SUCCESS;
    }
    else
    {
        /* meet some runtime violation, return error code */
        return BOOT_FAILURE;
    }
}



