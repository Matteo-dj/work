/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.
* File name: strcpy_s.c
* History:
*     1. Date:
*         Author:
*         Modification:
********************************************************************************
*/

#include "securec.h"

/*******************************************************************************
 * <NAME>
 *    strcpy_s
 *
 * <SYNOPSIS>
 *    errno_t strcpy_s(char* strDest, size_t destMax, const char* strSrc)
 *
 * <FUNCTION DESCRIPTION>
 *    The strcpy_s function copies the contents in the address of strSrc,
 *    including the terminating null character, to the location specified by strDest.
 *    The destination string must be large enough to hold the source string,
 *    including the terminating null character. strcpy_s will return EOVERLAP_AND_RESET
 *    if the source and destination strings overlap.
 *
 * <INPUT PARAMETERS>
 *    strDest                  Location of destination string buffer
 *    destMax                  Size of the destination string buffer.
 *    strSrc                   Null-terminated source string buffer.
 *
 * <OUTPUT PARAMETERS>
 *    strDest                  is updated.
 *
 * <RETURN VALUE>
 *    0                        success
 *    EINVAL                   strDest == NULL or strSrc == NULL
 *    ERANGE                   destination buffer is NOT enough,  or size of
 *                             buffer is zero or greater than SECUREC_STRING_MAX_LEN
 *    EOVERLAP_AND_RESET       dest buffer and source buffer are overlapped
 *
 *    If there is a runtime-constraint violation, then if strDest is not a null
 *    pointer and destMax is greater than zero and not greater than
 *    SECUREC_STRING_MAX_LEN, then strcpy_s sets strDest[0] to the null character.
 *******************************************************************************
*/

errno_t strcpy_error(char* strDest, size_t destMax, const char* strSrc)
{
    if (destMax == 0 || destMax > SECUREC_STRING_MAX_LEN)
    {
        return BOOT_FAILURE;
    }
    else if (strDest == NULL || strSrc == NULL)
    {
        if (strDest != NULL)
        {
            strDest[0] = '\0';
            return BOOT_FAILURE;
        }

        return BOOT_FAILURE;
    }
    else if (strlen(strSrc) + 1 > destMax)
    {
        strDest[0] = '\0';
        return BOOT_FAILURE;
    }
    else
    {
        return BOOT_SUCCESS;
    }
}

errno_t strcpy_s(char* strDest, size_t destMax, const char* strSrc)
{
    if ((destMax > 0 && destMax <= SECUREC_STRING_MAX_LEN && strDest != NULL && strSrc != NULL && strDest != strSrc))
    {
        const char* endPos = strSrc;
        size_t srcStrLen = destMax;  /* use it to store the maxi length limit */
		/*lint -e722*/

        while ( *(endPos++) && srcStrLen-- > 0);  /* use srcStrLen as boundary checker */
		/*lint +e722*/
        srcStrLen = (unsigned int)(endPos - strSrc) ; /*with ending terminator*/

        if (srcStrLen <= destMax)
        {
            if (strDest < strSrc)
            {
                if (strDest + srcStrLen <= strSrc )
                {
                    (void)memcpy_s(strDest, srcStrLen, strSrc, srcStrLen);
                    return BOOT_SUCCESS;
                }
                else
                {
                    strDest[0] = '\0';
                    return BOOT_FAILURE;
                }
            }
            else
            {
                if (strSrc + srcStrLen <= strDest )
                {
                    (void)memcpy_s(strDest, srcStrLen, strSrc, srcStrLen);
                    return BOOT_SUCCESS;
                }
                else
                {
                    strDest[0] = '\0';
                    return BOOT_FAILURE;
                }
            }
        }
    }

    return strcpy_error(strDest, destMax, strSrc);
}

