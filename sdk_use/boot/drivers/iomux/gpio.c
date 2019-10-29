#include <iomux.h>
#include <gpio.h>
#include <reg.h>

int  io_set_dir(HI_HW_IO_E enId, HI_GPIO_DIRECTION_E enDir)
{
    unsigned int ulDirVal = 0;

    if(enId >= HI_HW_IO_MAX)
    {
        return -1;
    }

    HI_REG_READ((GPIO_PHY_BASE + GPIO_SWPORT_DDR), ulDirVal);
    
    if(enDir)
    {
        ulDirVal |= (unsigned int)(1 << enId);
    }
    else
    {
        ulDirVal &= ~(unsigned int)(1 << enId);
    }
    
    HI_REG_WRITE((GPIO_PHY_BASE + GPIO_SWPORT_DDR), ulDirVal);
    
    return 0;
}

int  io_set_output_val(HI_HW_IO_E enId, HI_GPIO_VALUE_E enVal)
{
    unsigned int ulRegVal = 0;
//    unsigned lock_stat = 0;

    if(enId >= HI_HW_IO_MAX)
    {
        return -1;
    }
    

    HI_REG_READ((GPIO_PHY_BASE + GPIO_SWPORT_DR), ulRegVal);
    if(enVal)
    {
        ulRegVal |= (unsigned int)(1 << enId);
    }
    else
    {
        ulRegVal &= ~(unsigned int)(1 << enId);
    }
    HI_REG_WRITE((GPIO_PHY_BASE + GPIO_SWPORT_DR), ulRegVal);
    
    return 0;
}
void gpio_init(void)
{
	io_set_dir(HI_HW_IO7,HI_GPIO_DIRECTION_OUT);
	io_set_output_val(HI_HW_IO7,HI_GPIO_VALUE0);
}
void uart_set_485tx_mode(void)
{
	io_set_output_val(HI_HW_IO7,HI_GPIO_VALUE1);
}

void uart_set_485rx_mode(void)
{
	io_set_output_val(HI_HW_IO7,HI_GPIO_VALUE0);
}
