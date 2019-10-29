// SPDX-License-Identifier: GPL-2.0+
/*
 * Usefuls routines based on the LzmaTest.c file from LZMA SDK 4.65
 *
 * Copyright (C) 2007-2009 Industrie Dial Face S.p.A.
 * Luigi 'Comio' Mantellini (luigi.mantellini@idf-hit.com)
 *
 * Copyright (C) 1999-2005 Igor Pavlov
 */

/*
 * LZMA_Alone stream format:
 *
 * uchar   Properties[5]
 * uint64  Uncompressed size
 * uchar   data[*]
 *
 */

#include <watchdog.h>
#include <debug.h>

#define LZMA_PROPERTIES_OFFSET 0
#define LZMA_SIZE_OFFSET       LZMA_PROPS_SIZE
#define LZMA_DATA_OFFSET       (LZMA_SIZE_OFFSET+sizeof(uint64_t))

#include <LzmaTools.h>
#include "LzmaDec.h"

#include <boot.h>
#include "securec.h"
	
static void *SzAlloc(void *p, size_t size)
{ 
    HI_UNREF_PARAM(p);
    return xmalloc(size);
}
static void SzFree(void *p, void *address)
{ 
    HI_UNREF_PARAM(p);
    xfree(address);
}

int lzmaBuffToBuffDecompress (unsigned char *outStream, SizeT *uncompressedSize,
                  unsigned char *inStream,  SizeT  length)
{
    int res = SZ_ERROR_DATA;
    int i;
    ISzAlloc g_Alloc;

    SizeT outSizeFull = 0xFFFFFFFF; /* 4GBytes limit */
    SizeT outProcessed;
    SizeT outSize;
    SizeT outSizeHigh;
    ELzmaStatus state;
    SizeT compressedSize = (SizeT)(length - LZMA_PROPS_SIZE);

    memset_s(&state, sizeof(state), 0, sizeof(state));

    outSize = 0;
    outSizeHigh = 0;
    /* Read the uncompressed size */
    for (i = 0; i < 8; i++) {
        unsigned char b = inStream[LZMA_SIZE_OFFSET + i];
        if (i < 4) {
                outSize     += (UInt32)(b) << (UInt32)(i * 8);
        } else {
                outSizeHigh += (UInt32)(b) << (UInt32)((i - 4) * 8);
        }
    }

    outSizeFull = (SizeT)outSize;
    /*lint -e506*/
    /*lint -e774*/
    if (sizeof(SizeT) >= 8) {
    /*lint +e774*/
    /*lint +e506*/
        /*
         * SizeT is a 64 bit uint => We can manage files larger than 4GB!
         *
         */
/*
                【屏蔽人】z00193780
                【审核人】w00190206 b00208046
                【代码分析】开源及第三方代码，不做CodeDEX整改要求，暂不处理
                【功能影响】不影响正常业务，屏蔽告警
*/

            outSizeFull |= (SizeT)((SizeT)((SizeT)outSizeHigh << 16) << 16);
    } else if (outSizeHigh != 0 || (UInt32)(SizeT)outSize != outSize) {
        /*
         * SizeT is a 32 bit uint => We cannot manage files larger than
         * 4GB!  Assume however that all 0xf values is "unknown size" and
         * not actually a file of 2^64 bits.
         *
         */
        if (outSizeHigh != (SizeT)-1 || outSize != (SizeT)-1) {
            print_msg0 ("LZMA: 64bit support not enabled.\n");
            return SZ_ERROR_DATA;
        }
    }

    print_msg1("Uncompresed size: ", outSizeFull);
    print_msg1("Compresed size: ", compressedSize);

    g_Alloc.Alloc = SzAlloc;
    g_Alloc.Free = SzFree;

    /* Short-circuit early if we know the buffer can't hold the results. */
    if (outSizeFull != (SizeT)-1 && *uncompressedSize < outSizeFull)
        return SZ_ERROR_OUTPUT_EOF;

    /* Decompress */
    outProcessed = (outSizeFull > *uncompressedSize)?(*uncompressedSize):outSizeFull;

    watchdog_feed();

    res = LzmaDecode(
        outStream, &outProcessed,
        inStream + LZMA_DATA_OFFSET, &compressedSize,
        inStream, LZMA_PROPS_SIZE, LZMA_FINISH_END, &state, &g_Alloc);
    *uncompressedSize = outProcessed;

    //print_msg1("LZMA: Uncompressed ............... 0x%zx\n", outProcessed);

    if (res != SZ_OK)  {
        return res;
    }

    return res;
}

