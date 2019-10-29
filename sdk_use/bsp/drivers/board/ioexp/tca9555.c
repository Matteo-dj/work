
#include <hi_types.h>
#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM

#include <hi_mdm_types.h>
#include <hi_driver.h>
#include <hi_mdm.h>
#include "tca9555.h"

//from ioexp
#define TCA9555_I2C_ADDR    0x42
#define TCA9555_IO_NUM      0x08
#define TCA9555_IO_PAIR     0x02

#define TCA9555_INPUT		0
#define TCA9555_OUTPUT		2
#define TCA9555_INVERT		4
#define TCA9555_DIRECTION	6





/*******************************************************************************
*
* tca9555_write_reg - write reg in io expander through i2c bus
*
* input: unsigned char clintAddress ---i2c address , could be 0x40~0x47.
*        unsigned char reg          ---io expander register, there are 8 registers.
*                                      we can write 2 in one time.
*                                      could be 0,2,4,6.
*        unsigned char *pBuff       ---value to be writen in register. 2 bytes.
*        int len                    ---should be 2.
* RETURNS: N/A.
*
* ERRNO: N/A.
*/
static int tca9555_write_reg(unsigned char clintAddress, unsigned char reg, unsigned char* pBuff, int len)
{
    HI_I2C_DATA_S dataStruct = {0};
    unsigned char data[3] = {0};

    data[0] = reg;
    data[1] = *pBuff;
    data[2] = *(pBuff + 1);   //add 1 because of the first byte reg
    (hi_void)memset_s((HI_VOID *)&dataStruct, sizeof(dataStruct), 0x00, sizeof(dataStruct));
    dataStruct.pSendBuf = data;
    dataStruct.ulSendLength = (HI_U32)len + 1;
    dataStruct.pReceiveBuf = HI_NULL;
    dataStruct.ulReceiveLength = 0;
    return (int)HI_I2C_SendData(HI_I2C_IDX_0, clintAddress, &dataStruct);
}

/*******************************************************************************
*
* tca9555_read_reg - read reg in io expander through i2c bus
*
* input: unsigned char clintAddress ---i2c address , could be 0x40~0x47.
*        unsigned char reg          ---io expander register, there are 8 registers.
*                                      we can write 2 in one time.
*                                      could be 0,2,4,6.
*        unsigned char *pBuff       ---value to be writen in register. 2 bytes.
*        int len                    ---should be 2.
* RETURNS: N/A.
*
* ERRNO: N/A.
*/
static int tca9555_read_reg(unsigned char clintAddress, unsigned char reg, unsigned char* pBuff, int len)
{
    HI_I2C_DATA_S dataStruct = {0};

    (hi_void)memset_s((HI_VOID *)&dataStruct, sizeof(dataStruct), 0x00, sizeof(dataStruct));
    dataStruct.pSendBuf = &reg;
    dataStruct.ulSendLength = sizeof(reg);
    dataStruct.pReceiveBuf = pBuff;
    dataStruct.ulReceiveLength = (unsigned int)len;
    return (int)HI_I2C_SendRecvData(HI_I2C_IDX_0, clintAddress, &dataStruct);
}


/*******************************************************************************
*
* tca9555_gpio_direction_get ---get io direction
*
* input: unsigned char ionum ---io number, could be 0x00~0x0f.
*
* RETURNS: 0/1               ---1 as input, 0 as output.
*
* ERRNO: HI_ERR_BAD_DATA     ---input invalid
*/
static int tca9555_gpio_direction_get(unsigned char ionum)
{
    int ret, reg_num = 0;
    unsigned char io_reg[2] = {0};

    if (ionum > 15)
    {
        return ERROR;
    }

    ret = tca9555_read_reg(TCA9555_I2C_ADDR, TCA9555_DIRECTION, io_reg, sizeof(io_reg));

    if (HI_ERR_SUCCESS != ret)
    {
        return ERROR;
    }

    reg_num = ionum / TCA9555_IO_NUM;
    return (io_reg[reg_num] & (1 << (ionum % TCA9555_IO_NUM)) ? 1 : 0);
}

/*******************************************************************************
*
* tca9555_gpio_direction_set     - set io direction
*
* input: unsigned char ionum     ---io number, could be 0x00~0x0f.
*        unsigned char direction ---io direction, 1 as input, 0 as output.
*
* RETURNS: N/A.
*
* ERRNO: HI_ERR_BAD_DATA         --input invalid
*/
static int tca9555_gpio_direction_set(unsigned char ionum, unsigned char direction)
{
    int ret, reg_num = 0;
    unsigned char io_reg[2] = {0};

    if (ionum > 15)
    {
        return HI_ERR_BAD_DATA;
    }

    ret = tca9555_read_reg(TCA9555_I2C_ADDR, TCA9555_DIRECTION, io_reg, sizeof(io_reg));

    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }

    reg_num = ionum / TCA9555_IO_NUM;

    if (direction)
    {
        io_reg[reg_num] |= (1 << (ionum % TCA9555_IO_NUM));
    }
    else
    {
        io_reg[reg_num] &= ~(HI_U32)(1 << (ionum % TCA9555_IO_NUM));
    }

    return tca9555_write_reg(TCA9555_I2C_ADDR, TCA9555_DIRECTION, io_reg, sizeof(io_reg));
}

/*******************************************************************************
*
* tca9555_gpio_get_single_value --- get io value
* input: unsigned char ionum    ---io number, could be 0x00~0x0f.
*
* RETURNS: 0/1                  ---1 as high, 0 as low.
*
* ERRNO: HI_ERR_BAD_DATA        --input invalid
*/
static int tca9555_gpio_get_single_value(unsigned char ionum)
{
    int ret = 0, reg_num = 0;
    unsigned char input_reg[2] = {0};

    if (ionum > 15)
    {
        return ERROR;
    }

    ret = tca9555_read_reg(TCA9555_I2C_ADDR, TCA9555_INPUT, input_reg, sizeof(input_reg));

    if (HI_ERR_SUCCESS != ret)
    {
        return ERROR;
    }

    reg_num = ionum / TCA9555_IO_NUM;
    return (input_reg[reg_num] & (1 << (ionum % TCA9555_IO_NUM)) ? 1 : 0);
}

static int tca9555_gpio_get_out_value(unsigned char ionum)
{
    int ret = 0, reg_num = 0;
    unsigned char input_reg[2] = {0};

    if (ionum > 15)
    {
        return ERROR;
    }

    ret = tca9555_read_reg(TCA9555_I2C_ADDR, TCA9555_OUTPUT, input_reg, sizeof(input_reg));

    if (HI_ERR_SUCCESS != ret)
    {
        return ERROR;
    }

    reg_num = ionum / TCA9555_IO_NUM;
    return (input_reg[reg_num] & (1 << (ionum % TCA9555_IO_NUM)) ? 1 : 0);
}


/*******************************************************************************
*
* tca9555_gpio_set_single_value     - set io value
*
* input: unsigned char ionum     ---io number, could be 0x00~0x0f.
*        unsigned char direction ---io value, 1 as high, 0 as low.
*
* RETURNS: N/A.
*
* ERRNO: HI_ERR_BAD_DATA         --input invalid
*/
static int tca9555_gpio_set_single_value(unsigned char ionum, unsigned char val)
{
    int ret, reg_num = 0;
    unsigned char out_reg[2] = {0};

    if (ionum > 15)
    {
        return HI_ERR_BAD_DATA;
    }

    ret = tca9555_read_reg(TCA9555_I2C_ADDR, TCA9555_OUTPUT, out_reg, sizeof(out_reg));

    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }

    reg_num = ionum / TCA9555_IO_NUM;

    if (val)
    { out_reg[reg_num] |= (1 << (ionum % TCA9555_IO_NUM)); }
    else
    { out_reg[reg_num] &= ~(HI_U32)(1 << (ionum % TCA9555_IO_NUM)); }

    return tca9555_write_reg(TCA9555_I2C_ADDR, TCA9555_OUTPUT, out_reg, sizeof(out_reg));
}


/*******************************************************************************
*
* ioexpWrite - write to a io expander file
*
* Perform a write operation on an open io expander file.
*
* RETURNS: ERROR if the file open mode is O_WRONLY; otherwise, the number of
* bytes read (may be less than the amount requested), or 0 if the
* file is at EOF.
*
* ERRNO
* \is
* \i ENOENT
* \i ERROR
* \ie
*/
HI_SSiZE_T ioexpWrite
(
    int ioexpDevId,	/* file descriptor of file to read */
    const char* pBuffer,	/* buffer to receive data */
    HI_SiZE_T nBytes	/* max bytes to read in to buffer */
)
{
    int ReadLength = 0;
    char pBits[8] = {0};
    int ret = (int)HI_ERR_FAILURE; 
    unsigned int i = 0;

    if (nBytes > sizeof(pBits))
    {
        return (HI_S32)HI_ERR_FAILURE;
    }

    (hi_void)memcpy_s(pBits, sizeof(pBits), pBuffer, nBytes);

/*
【屏蔽人】z00193780
【审核人】w00190206
【代码分析】pBits读写均不存在越界问题，误报。
【功能影响】误报，屏蔽告警
*/
    if (nBytes % 2)
    {
        // 为了去掉codex误报，增加判断
        if (nBytes < sizeof(pBits))
        {
            pBits[nBytes] = 0;         
        }
    }

    for (i = 0; i < ((nBytes + 1) / 2); i++)
    {
        ret = tca9555_write_reg((unsigned char)TCA9555_I2C_ADDR, (unsigned char)(2 * (unsigned char)i), (unsigned char*)(pBits + 2 * i), 2);
        if (HI_ERR_SUCCESS != ret)
        { 
            return (HI_S32)(HI_ERR_FAILURE);
        }
        
        ReadLength += 2;
    }

    return ((ReadLength > (HI_S32)nBytes) ? (HI_S32)nBytes : ReadLength);
}

/*******************************************************************************
*
* ioexpIoctl - do device specific control function
*
* Only the FIO_GET_DIR, FIO_SET_DIR_INPUT, FIO_SET_DIR_OUTPUT, FIO_GET_INPUT_VAL, FIO_SET_OUT_VAL_HIGH and FIO_SET_OUT_VAL_LOW options are supported.
*
* RETURNS: OK, or ERROR .
*
* ERRNO
* \is
* \i EINVAL
* \i S_ioLib_UNKNOWN_REQUEST
* \ie
*/
HI_S32 ioexpIoctl
(
    int ioexpDevId,	/* descriptor to control */
    int cmd,	/* function code */
    int	ionum	/* some argument */
)
{
    HI_S32 status = 0;

    switch (cmd)
    {
        case FIO_GET_DIR:
            status = tca9555_gpio_direction_get((unsigned char)ionum);
            break;

        case FIO_SET_DIR_INPUT:
            status = tca9555_gpio_direction_set((unsigned char)ionum, 1);
            break;

        case FIO_SET_DIR_OUTPUT:
            status = tca9555_gpio_direction_set((unsigned char)ionum, 0);
            break;

        case FIO_GET_INPUT_VAL:
            status = tca9555_gpio_get_single_value((unsigned char)ionum);
            break;

        case FIO_SET_OUT_VAL_HIGH:
            status = tca9555_gpio_set_single_value((unsigned char)ionum, 1);
            break;

        case FIO_SET_OUT_VAL_LOW:
            status = tca9555_gpio_set_single_value((unsigned char)ionum, 0);
            break;

        case FIO_GET_OUT_VAL:
            status = tca9555_gpio_get_out_value((unsigned char)ionum);
            break;

        default:
            break;
    }

    return (status);
}
#endif
