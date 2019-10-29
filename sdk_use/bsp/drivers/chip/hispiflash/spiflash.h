
#ifndef __SPIFLASH_H__
#define __SPIFLASH_H__

HI_EXTERN HI_U32 s_stFlashSem;
#define FLASH_LOCK()  	     HI_MDM_SEM_Wait(s_stFlashSem, HI_SYS_WAIT_FOREVER)
#define FLASH_UNLOCK()  	 (HI_VOID)HI_MDM_SEM_Signal(s_stFlashSem)


#endif