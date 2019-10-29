/**
 ******************************************************************************
 * @file      mrs_ptl68.c
 * @brief     Э�鴦��
 * @details   This file including all API functions's implement of mrs_ptl68.c.
 * @copyright 
 ******************************************************************************
 */
#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined (PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_CCT3911)

/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "maths.h"
#include "typedo.h"
#include "comLib.h"
#include "mrs_ptl68.h"

/*-----------------------------------------------------------------------------
 Section: Macro Definitions
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 Section: Global Variables
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Local Variables
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 Section: Local Function Prototypes
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Global Function Prototypes
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Function Definitions
 ----------------------------------------------------------------------------*/
HI_U8 
mrs_ptl68_ctrlcode_set(HI_U8 dir,
    HI_U8 flag,
    HI_U8 bsotype)
{
    return (((dir & 0x01) << 7) 
            | ((flag & 0x01) << 6) 
            | PTL_BSOTYPE_GET(bsotype)); 
}

HI_U16   
mrs_ptl68_pack(HI_U8 ctrlcode, 
    HI_U16 cmdId,
    HI_U8* ctxt, 
    HI_U16 clen,
    HI_U8* frm,
    HI_U16 frmlen)
{
    HI_U16 i = 0;
    HI_U16 dlen = clen + PTL_CMDID_SIZE; //PTL_CMDID_SIZE ��cmdId����

    if (PTL_LEN(dlen) > frmlen)
    {
        return 0;
    }
    
    load_uint8(frm, &i, PTL_HEAD);      //����ͷ
    load_uint8(frm, &i, ctrlcode);      //������
    load_uint16(frm, &i, dlen);         //���ݳ���
    load_uint16(frm, &i, cmdId);        //��������

    if ((clen > 0) && (ctxt != NULL))
    {
        if (ctxt == (frm + i))             
        { //��ַһ������������
            i += clen;
        }
        else   
        {
            load_buf(frm, &i, ctxt, clen);  //��������
        }
    }

    HI_U16 crc16 = get_crc16(frm, i, 0xFFFF);
    load_uint16(frm, &i, crc16);        //cs
    load_uint8(frm, &i, PTL_END);       //����β

    return i;
}

/**
 ******************************************************************************
 * @brief      Э�����
 * @param[in] pchk �����Ķ���

 * @retval   Э�鷵��ҵ������
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
int mrs_ptl_chkfrm_68(mrs_ptl_chk_t *pchk)
{
    int i;
    int pre_len;
    HI_U16 data_len;
    HI_U16 ptl_len;

    for (i = 0;i < pchk->in_len;i ++)
    {
        if (pchk->pin[i] == 0x68)
        {
            break;
        }
    }

    if (i >= pchk->in_len)
    {
        pchk->in_len = 0;
        return ERROR;
    }

    pre_len = i;
    if (pchk->in_len < (pre_len + PTL_MIN_LEN)) //���ĳ���̫��
    {
        return ERROR;
    }

    pchk->pout = pchk->pin + pre_len;
    (hi_void)memcpy_s(&data_len, PTL_DATA_SIZE, pchk->pout + PTL_DLEN_OFT_HEAD, PTL_DATA_SIZE);

    if (data_len > FRM_CHK_LEN)
    {
        //ɾ��pre_len + 1�ֽ�, ���Ż�
        int del_len = pre_len + 1;
        (hi_void)memmove_s(pchk->pin, PTL_INBUF_LEN, pchk->pin + del_len, pchk->in_len - del_len);
        pchk->in_len -= del_len;
        printf("ptl_chkfrm_68: ptl_len long\n");
        return ERROR;
    }

    ptl_len = PTL_LEN(data_len);
    if (pchk->in_len < (pre_len + ptl_len)) //����û��ȫ
    {
        return ERROR;
    }

    HI_U16 crc16 = get_crc16(pchk->pout, ptl_len - 3, 0xFFFF);
    if ((crc16 != (pchk->pout[ptl_len - 3] | (pchk->pout[ptl_len - 2] << 8))) 
        || (pchk->pout[ptl_len - 1] != PTL_END))
    {
        //ɾ��pre_len + 1�ֽ�, ���Ż�
        int del_len = pre_len + 1;
        (hi_void)memmove_s(pchk->pin, PTL_INBUF_LEN, pchk->pin + del_len, pchk->in_len - del_len);
        pchk->in_len -= del_len;
        return ERROR;
    }
  
    pchk->out_len = ptl_len;

    return PTL_BSOTYPE_GET(pchk->pout[1]);
}

/*---------------------------------mrs_ptl68.c-------------------------------------*/
#endif