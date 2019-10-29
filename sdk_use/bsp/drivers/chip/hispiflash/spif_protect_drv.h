
#ifndef __SPIF_PROTECT_DRV_H__
#define __SPIF_PROTECT_DRV_H__

#include <spifdrv.h>

#ifdef PRODUCT_CFG_FLASH_PROTECT

#define SPI_PROTECT_512KB                           ((1<<4)|(1<<5))                     /* protect top 512KB */
#define SPI_PROTECT_32KB                            ((1<<4)|(1<<5)|(1<<6))              /* protect top 32KB */
#define SPI_PROTECT_SET_SRP0                        (1<<7)                      /* SRP0 */
#define SPI_PROTECT_NONE                            (0)                                 /* protect none */
#define SPI_PROTECT_MASK_512KB                      ((1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6))/* mask for 512KB */
#define SPI_PROTECT_MASK_32KB                       ((1<<3)|(1<<4)|(1<<5)|(1<<6))       /* mask for 32KB */
#define SPI_PROTECT_MASK_NONE                       ((1<<2)|(1<<3)|(1<<4))              /* mask for none */
//-----------------------------------------------------------------------------
#define SPI_PROTECT_ENABLE_TIME                     (360*60)/*360*60s,after 360min,protect 512KB*/
#define SPI_PROTECT_MAX_TIMES_WR_STATUS_REG         (3)  
//-----------------------------------------------------------------------------
HI_BOOL spif_protect_mechanism_is_enable(HI_VOID);
HI_U32 spif_shrink_protect_area(HI_U32 ulOffset);
HI_U32 spif_protect_all(HI_VOID);

#endif

#endif