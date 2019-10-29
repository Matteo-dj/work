/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2014-2018. All rights reserved.
 * Description: strncpy_s  function
 * Author: lishunda
 * Create: 2014-02-25
 */
/* [Standardize-exceptions] Use unsafe function: Performance-sensitive
 * [reason] Always used in the performance critical path,
 *          and sufficient input validation is performed before calling
 */

#include "securec.h"

/*
 * <FUNCTION DESCRIPTION>
 *   The strncpy_s function copies not more than n successive characters (not including the terminating null character)
 *                     from the array pointed to by strSrc to the array pointed to by strDest.
 *
 * <INPUT PARAMETERS>
 *    strDest                          Destination string.
 *    destMax                          The size of the destination string, in characters.
 *    strSrc                            Source string.
 *    count                              Number of characters to be copied.
 *
 * <OUTPUT PARAMETERS>
 *    strDest                           is updated
 *
 * <RETURN VALUE>
 *    BOOT_SUCCESS                      Success
 *    BOOT_FAILURE                      FAILURE                          
 *    If there is a runtime-constraint violation, strDest[0] will be set to the '\0' when strDest and destMax valid
 */

 errno_t strncpy_s(void* dest, size_t destMax, const void* src, size_t count)
 {
	 if (LIKELY( count <= destMax && dest && src   /*&& dest != src*/
				 && destMax <= SECUREC_MEM_MAX_LEN
				 && count > 0
				 && ( (dest > src  &&  (void*)((UINT8T*)src  + count) <= dest) ||
					  (src	> dest &&  (void*)((UINT8T*)dest + count) <= src) )
			   ) )
	 {
		 (void)strncpy(dest, src, count);
		 return BOOT_SUCCESS;
	 }
	 else
	 {
		 /* meet some runtime violation, return error code */
		 return BOOT_FAILURE;
	 }
 }

	

