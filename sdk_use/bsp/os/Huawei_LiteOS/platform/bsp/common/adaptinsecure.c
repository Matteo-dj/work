/*----------------------------------------------------------------------------
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "adaptsecure.h"

int Stub_MemSet(void *pvDest,unsigned int ulDestMax, int Char, unsigned int ulCount)
{
    memset(pvDest, Char, ulCount);
    return 0;
}

int Stub_MemCpy(void *pvDest,unsigned int ulDestMax, const void *Src, unsigned int ulCount)
{
    memcpy(pvDest, Src, ulCount);
    return 0;
}

int Stub_StrnCpy( char *pcDest, unsigned int ulDestMax, const char * pcSrc, unsigned int ulCount )
{
    strncpy(pcDest, pcSrc, ulCount);
    return 0;
}
int Stub_StrnCat( char *pcDest, unsigned int ulDestMax,const char *pcSrc, unsigned int ulCount)
{
    strncat(pcDest, pcSrc, ulCount);
    return 0;
}
int Stub_StrCat( char *pcDest, unsigned int ulDestMax,const char *pcSrc)
{
    strcat(pcDest, pcSrc);
    return 0;
}
int Stub_MemMove(void *pcDest,unsigned int ulDestMax, const void *pcSrc, unsigned int ulCount)
{
    memmove(pcDest, pcSrc, ulCount) ;
    return 0;
}


int Stub_Snprintf(char* pcStrDest, unsigned int ulDestMax,
unsigned int ulCount, const char* pszFormat, ...)
{
    int ret = 0;
    va_list arglist;

    va_start(arglist, pszFormat);
    ret = vsnprintf(pcStrDest, ulCount, pszFormat, arglist);
    va_end(arglist);
    return ret;/*lint !e438*/
}

