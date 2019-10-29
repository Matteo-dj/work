#ifndef	__HISOC_RTC_H__
#define	__HISOC_RTC_H__

#include "asm/platform.h"

#define PERI_CRG57                      (CRG_REG_BASE + 0xE4)
#define BIT_TEMP_SRST_REQ		2

#define OSDRV_MODULE_VERSION_STRING "HISI_RTC @Hi3516A"

static int temp_crg_reset(void)
{
	uint32_t value = readl(PERI_CRG57);
	writel(value | (1<<BIT_TEMP_SRST_REQ),  PERI_CRG57);
	writel(value & ~(1<<BIT_TEMP_SRST_REQ), PERI_CRG57);

	return 0;
}



#endif

