
#include "types.h"
#include "spi_flash.h"
#include "spi_flash_protect_drv.h"
#include "platform.h"

int spi_flash_read(struct spi_flash *flash, u32 offset,
		size_t len, void *buf)
{   
	return flash->read(flash, offset, len, buf);
}

int spi_flash_write(struct spi_flash *flash, u32 offset,
		size_t len, const void *buf)
{
    int ret = 0;
#ifdef PRODUCT_CFG_FLASH_PROTECT   
    ret = hisfc350_shrink_protect_area(offset);
    if(ret)
    {
        return ret;
    }
#endif
    return flash->write(flash, offset, len, buf);
}

int spi_flash_erase(struct spi_flash *flash, u32 offset,
		size_t len)
{
    int ret = 0;
#ifdef PRODUCT_CFG_FLASH_PROTECT   
    ret = hisfc350_shrink_protect_area(offset);
    if(ret)
    {
        return ret;
    }
#endif
    return flash->erase(flash, offset, len);
}

