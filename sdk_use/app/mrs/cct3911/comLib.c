/**
 ******************************************************************************
 * @file      comlib.c
 * @brief     公用库接口实现.
 * @details   本文件实现了一些基于应用调用的公共函数.
 * @copyright
 ******************************************************************************
 */
 #include <hi_types.h>
#include <hi_mdm_types.h>
#if defined (PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_CCT3911)
/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include "mrs_cfg.h"
#include "hi_socket.h"

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Constant Definitions
 ----------------------------------------------------------------------------*/
/* NONE */ 

/*-----------------------------------------------------------------------------
 Section: Global Variables
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 Section: Local Variables
 ----------------------------------------------------------------------------*/
// CRC 高位字节值表
static const HI_U8 CRCHi[] = 
{
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

// CRC 低位字节值表
const HI_U8 CRCLo[] = 
{
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

/*-----------------------------------------------------------------------------
 Section: Local Function Prototypes
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Function Definitions
 ----------------------------------------------------------------------------*/
/**
 ******************************************************************************
 * @brief      反转缓冲区中的数据
 * @param[in]  pfbuf   缓冲区
 * @param[in]  bytes 字节数
 *
 * @details    例如:
 *             {1, 2, 3, 4, 5} -> {5, 4, 3, 2, 1}.
 *
 * @note
 ******************************************************************************
 */
void
binvert(void *pin, HI_S32 bytes)
{
    HI_U8 * pfbuf = pin;
    HI_U8* pfbuf_end = pfbuf + (bytes - 1);
    HI_U8 temp;

    while (pfbuf < pfbuf_end)
    {
        temp = *pfbuf;
        *pfbuf = *pfbuf_end;
        *pfbuf_end = temp;

        pfbuf_end--;
        pfbuf++;
    }
}

/**
 ******************************************************************************
 * @brief      16位的CRC校验计算
 * @param[in]  pdata    校验内容起始地址
 * @param[in]  count  校验内容长度
 * @param[in]  crc    校验初始值
 * @retval     返回计算得出的CRC校验结果
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
HI_U16 get_crc16(const void* pbuf, HI_U16 usLen, HI_U16 crc)
{
    HI_U8 ucCRCHi = (HI_U8)(crc >> 8);   /* 高CRC字节初始化 */
    HI_U8 ucCRCLo = (HI_U8)crc;   /* 低CRC 字节初始化 */
    HI_U8 usIndex = 0;      /* CRC循环中的索引 */
    const HI_U8* p = (const HI_U8 *)pbuf;

    while (usLen > 0)
    {
        usIndex = ucCRCHi ^ *p++; /* 计算CRC */
        ucCRCHi = ucCRCLo ^ CRCHi[usIndex];
        ucCRCLo = CRCLo[usIndex];

        usLen --;
    }
    
    return ((HI_U16)(ucCRCHi << 8) | ucCRCLo);
}

/**
 ******************************************************************************
 * @brief      判断给定的缓冲区数据是否全等于指定的字符
 * @param[in]  pfbuf 需要判断的缓冲区数据
 * @param[in]  c     用于比较的字符
 * @param[in]  len   缓冲区长度
 * @retval     返回判断结果 0-相等 其它-不相等
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
HI_S32
mem_equal(const void * pfbuf, HI_U8 c, HI_S32 len)
{
    HI_S32 i;
    const HI_U8* p;

    p = (const HI_U8 *) pfbuf;
    for (i = 0; i < len; ++i)
    {
        if (*(p + i) != c)
        {
            return (i + 1);
        }
    }
    return 0;
}

HI_S32
mem_bit_equal(HI_U8* p, HI_U8 c, HI_S32 len)
{
    HI_S32 i;

    for (i = 0; i < len; i ++)
    {
        if (BITS(p[i/8], (i % 8)) != c)
        {
            return (i + 1);
        }
    }

    return 0;
}

HI_S32 collect_data_pack(HI_BUF_S* HiBuf, 
    HI_U16 frm_sn, 
    HI_U8 **frm_buf, 
    HI_U16 *frm_len)
{
    HI_U8 *pbuf0, *pbuf1;
    HI_U16 frm_num0 = 0, frm_num1 = 0;
    HI_U16 frm_num;
    HI_U32 frm_Pos;

    frm_num = collect_frm_num(HiBuf->bufSize, &frm_num0, &frm_num1);
    
    pbuf0 = (HI_U8* )HiBuf->bufAddr[0];
    pbuf1 = (HI_U8* )HiBuf->bufAddr[1];

    if (frm_num == 0)
    {
        return -1;
    }

    if (frm_sn < (frm_num0 - 1))
    {
        frm_Pos = frm_sn * FRM_TRANS_LEN;
        *frm_len = FRM_TRANS_LEN;
        *frm_buf = pbuf0 + frm_Pos;
        return 0;
    }
    else if (frm_sn < frm_num0)
    {
        frm_Pos = frm_sn * FRM_TRANS_LEN;
        *frm_len = HiBuf->bufSize[0] - frm_Pos;
        *frm_buf = pbuf0 + frm_Pos;
        return 0;
    }
    else if (frm_sn < (frm_num - 1))
    {
        frm_Pos = (frm_sn - frm_num0) * FRM_TRANS_LEN;
        *frm_len = FRM_TRANS_LEN;
        *frm_buf = pbuf1 + frm_Pos;
        return 0;
    }
    else if (frm_sn < frm_num)
    {
        frm_Pos = (frm_sn - frm_num0) * FRM_TRANS_LEN;
        *frm_len = HiBuf->bufSize[1] - frm_Pos;
        *frm_buf = pbuf1 + frm_Pos;
        return 0;
    }

    return -1;
}

HI_U16 collect_frm_num(HI_U32 bufSize[], HI_U16 *fnum0, HI_U16 *fnum1)
{
    HI_U16 frm_num0, frm_num1;
    HI_U16 frm_num;

    frm_num0 = bufSize[0]/FRM_TRANS_LEN;
    frm_num1 = bufSize[1]/FRM_TRANS_LEN;
    
    if ((frm_num0 * FRM_TRANS_LEN) != bufSize[0])
    {
        frm_num0 += 1;
    }

    if ((frm_num1 * FRM_TRANS_LEN) != bufSize[1])
    {
        frm_num1 += 1;
    }

    if (fnum0)
    {
        *fnum0 = frm_num0;
    }

    if (fnum1)
    {
        *fnum1 = frm_num1;
    }

    frm_num = frm_num0 + frm_num1;
    return frm_num;
}

/**
 ******************************************************************************
 * @brief       写通讯通道数据
 * @param[in]   int fd  : fd句柄
 * @param[in]   void *buf   : 待发送数据缓冲区
 * @param[in]   int len    : 最大发送数据长度
 * @param[out]  None
 * @retval      -1 - 出错, >=0 - 实际发送的数据长度
 *
 * @details     void
 *
 * @note
 ******************************************************************************
 */
HI_S32 comm_wr(int fd, void *buf, int len)
{
    int nleft;
    int nwritten;
    HI_U8* ptr;

    ptr = (HI_U8* )buf;
    nleft = len;

    if (fd < 0)
    {
        return -1;
    }
    
    while(nleft > 0)
    {
        nwritten = send(fd, ptr, nleft, 0);
        if ((nwritten < 0) || (nwritten > nleft))
        {
            return ERROR;
        }
   
        nleft -= nwritten;
        ptr += nwritten;
    }
    
    return len;
}

/*--------------------------------comLib.c------------------------------------*/
#endif