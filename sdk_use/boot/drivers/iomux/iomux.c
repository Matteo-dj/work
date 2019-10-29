#include <iomux.h>
#include <reg.h>

#define GPIO_SEL_BASE           0x101E6000
u32 iomux_config(HI_HW_IO_E enIoIdx, u8 ulIoVal)
{
	u32 ulReg = 0;

	if (enIoIdx >= HI_HW_IO_MAX)
	{
		return (u32)(-1);
	}

	if (enIoIdx <= HI_HW_IO25)
	{
		ulReg = GPIO_SEL_BASE + (u32)((unsigned int)enIoIdx << 2);
	}

	// HI_HW_ETXD0_IO26管脚与前一个管脚复用寄存器空了1个地址
	else if ((enIoIdx > HI_HW_IO25) && (enIoIdx < HI_HW_IO16_BACKUP))
	{
		ulReg = GPIO_SEL_BASE + (u32)(((unsigned int)enIoIdx + 1) << 2);
	}
	// HI_HW_ECRS_IO16_BACKUP管脚与前一个管脚复用寄存器空了25个地址
	else //if (enIoIdx >= HI_HW_IO16_BACKUP)
	{
		ulReg = GPIO_SEL_BASE + (u32)((u32)(enIoIdx + 24 + 1) << 2);
	}

	HI_REG_WRITE(ulReg, (u32)ulIoVal);
	return 0;
}