#ifndef __SPI_H
#define __SPI_H

#include "mrs_common.h"
#include "Hi_types.h"
#include "hi_mdm_io.h"
#include "hi_mdm_spi.h"
#include "Public.h"

#define __SPI_H

#define DUMMYBYTE			0xFF
						  	    													  
u8 SPI4438_Init(void);			
void SpiWriteOneByte(u8 byteToWrite);
u8 SpiReadOneByte(void);
void SpiWriteManyData(u8 byteCount, u8* pData);
void SpiReadManyData(u8 byteCount, u8* pData);

     
#endif

