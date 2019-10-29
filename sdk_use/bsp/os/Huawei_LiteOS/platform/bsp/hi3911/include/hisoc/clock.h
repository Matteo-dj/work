#ifndef	__HISOC_CLOCK_H__
#define	__HISOC_CLOCK_H__

#include "asm/platform.h"

#define	PERI_CRG12		(CRG_REG_BASE + 0x30)

/*lint -e160 */
#define get_bus_clk()  ({\
                  unsigned long tmp_reg, busclk;\
                  tmp_reg = GET_UINT32(PERI_CRG12);\
                  if (0x1 == (tmp_reg & 0x01))\
                          busclk = 198000000;\
                  else \
                          busclk = 24000000;\
                  busclk;\
                  })

#endif

