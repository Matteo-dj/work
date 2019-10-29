/***********************************************************************************
*              Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName:   hi_e2prom.h
* Description:
*
* History:
* Version   Date                Author          DefectNum     Description
* 1.0        2007-10-16       w54542        NULL             Create this file.
***********************************************************************************/

#ifndef __HI_E2PROM_H__
#define __HI_E2PROM_H__

#include <hi_types.h>

typedef struct hiE2PROM_Data_S
{
    HI_U32 u32Addr;
    HI_U8* pu8Buf;
    HI_U32 u32Length;
} E2PROM_Data_S, *PTR_E2PROM_Data_S;

typedef struct hiE2PROM_PARA_S
{
    HI_U32 u32EpromSize;
    HI_U32 u32PageSize;
    HI_U32 u32AddrLen;
} E2PROM_PARA_S;

typedef enum hiE2PROM_ChipType_E
{
    AT24C01,
    AT24C02,
    AT24C04,
    AT24C08,
    AT24C16,
    AT24C32,
    AT24C64,
    AT24C128,
    AT24C256,
    M24LC01,
    M24LC02,
    M24LC04,
    M24LC08,
    M24LC16,
    M24LC32,
    M24LC64,
    M24LC128,
    M24LC256
} E2PROM_ChipType_E;

#define E2PROM_I2C_NUM 1 //todo
#define E2PROMAddr 0xA0 //todo
#define E2PromWrMAX 32 //todo

/*---- E2PROM ----*/
void HI_E2PROM_Init(void);
HI_U32 HI_E2PROM_Read (HI_U32 u32Addr, HI_U8* pu8Buf, HI_U32 u32Length);
HI_U32 HI_E2PROM_Write (HI_U32 u32Addr, HI_U8* pu8Buf, HI_U32 u32Length);
HI_U32 HI_E2PROM_SetChipType(HI_U32 u32ChipType);




#endif

