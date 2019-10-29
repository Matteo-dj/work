
#ifndef __HI_FLASH_H__
#define __HI_FLASH_H__

/*function: erase sectors and write byte_size */
HI_VOID flash_init_crash(HI_VOID);
HI_U32 flash_write_crash(HI_IN HI_U32 ulByteOffAddr, HI_IN HI_VOID *pBuf, HI_IN HI_U32 ulByteSize);
HI_U32 flash_write_1_to_0(HI_IN HI_U32 ulByteOffAddr, HI_IN HI_VOID *pBuf, HI_IN HI_U32 ulByteSize);
HI_U32 flash_read_crash(HI_IN HI_U32 ulByteOffAddr, HI_IN HI_VOID *pBuf, HI_IN HI_U32 ulByteSize);

#endif/*__HI_FLASH_H__*/
