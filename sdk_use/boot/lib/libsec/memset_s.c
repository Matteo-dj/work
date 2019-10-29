/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.
* File name: memset_s.c
* History:
*     1. Date:
*         Author:
*         Modification:
********************************************************************************
*/

#include "securec.h"

/*******************************************************************************
* <NAME>
*    memset_s
*
* <SYNOPSIS>
*    errno_t memset_s(void* dest, size_t destMax, int c, size_t count)
*
* <FUNCTION DESCRIPTION>
*    Sets buffers to a specified character.
*
* <INPUT PARAMETERS>
*    dest                       Pointer to destination.
*    destMax                    The size of the buffer.
*    c                          Character to set.
*    count                      Number of characters.
*
* <OUTPUT PARAMETERS>
*    dest buffer                is uptdated.
*
* <RETURN VALUE>
*    EOK                        Success
*    EINVAL                     dest == NULL
*    ERANGE                     count > destMax or destMax > SECUREC_MEM_MAX_LEN
*                               or destMax == 0
*******************************************************************************
*/


errno_t memset_s(void* dest, size_t destMax, int c, size_t count)
{
    if (LIKELY(count <= destMax  && dest  && destMax <= SECUREC_MEM_MAX_LEN ))
    {
        (void)memset(dest, c, count);
        return BOOT_SUCCESS;
    }
    else
    {
        return BOOT_FAILURE;
    }
}


