/******************************************************************************
  File Name     : hal_key_ctrl.c
  Version       : Initial Draft
  Author        : HCT team
  Created       : 
  Last Modified :
  Description   : 
  Function List :
  History       :
******************************************************************************/
#include "hal_key_ctrl.h"
#include "drv_cipher_define.h"
#include <os_vx_sal_api.h>
#include <hi_drv_com.h>
#include <hi_low_power.h>


#define EFUSE_TIMEOUT_DEFAULT 5000//5秒, 随着后期回片性能结果，适当调整
#define EFUSE_TIMECNT_TICK    10//必须可以被EFUSE_TIMEOUT_DEFAULT整除

HI_BOOL hal_efuse_isbusy(HI_VOID)
{
    HI_U32 ulCnt = 0;
    HI_U32 ulVal;
    
    while (ulCnt < EFUSE_TIMEOUT_DEFAULT)
    {
        HI_REG_READ32(KL_STA, ulVal);  
        if((ulVal&KL_STA_READY_MASK) == KL_STA_READY)
        {
            break;
        }
        HI_MDM_Sleep(EFUSE_TIMECNT_TICK);
        ulCnt += EFUSE_TIMECNT_TICK;
    }
    if (ulCnt >= EFUSE_TIMEOUT_DEFAULT)
    {
        HI_ERR_CIPHER("State val:0x%x mask:0x%x time:%d\r\n",ulVal, ulVal&KL_STA_READY_MASK, ulCnt);
        return HI_TRUE;
    }
    return HI_FALSE;
}

/*烧写efuse 128bit*/
HI_U32 HAL_EFUSE_BurnKey(HI_EFUSE_KEY_IDX_E enArea, HI_U8 ucSrcData[EFUSE_AREA_SIZE], HI_U8 ucErrState[EFUSE_AREA_SIZE])
{
    HI_U32 ulVal;
    HI_U32 ulCnt = 0;

    //参数判断
    if ((enArea >= EFUSE_AERA_MAX)||(ucSrcData == HI_NULL)||(ucErrState == HI_NULL))
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    HI_REG_WRITE32(EFUSE_AVDD_EN, EFUSE_AVDD_EN_EN); 
    HI_REG_WRITE32(KL_WKEY0, *(HI_U32*)ucSrcData); 
    HI_REG_WRITE32(KL_WKEY1, *(HI_U32*)(ucSrcData+0x4)); 
    HI_REG_WRITE32(KL_WKEY2, *(HI_U32*)(ucSrcData+0x8)); 
    HI_REG_WRITE32(KL_WKEY3, *(HI_U32*)(ucSrcData+0xc));     
    //等空闲
    if (HI_TRUE == hal_efuse_isbusy())
    {
        HI_ERR_CIPHER("Error Time out! \r\n");
        return HI_ERR_TIMEOUT;
    }
   
    //启动烧写(按手册操作，避免同时写入错误)    
    HI_REG_READ32(KL_CTRL, ulVal);  
    ulVal &= ~(KL_CTRL_KEY_ADDR_MASK|KL_CTRL_MODE_MASK);
    ulVal |= KL_CTRL_MODE_EFUSE|(HI_U32)((HI_U32)enArea<<4)|KL_CTRL_MODE_START;
    HI_REG_WRITE32(KL_CTRL, ulVal); 
    
    //查询状态
    while (ulCnt < EFUSE_TIMEOUT_DEFAULT)
    {
        HI_REG_READ32(KL_STA1, ulVal); 
        HI_ERR_CIPHER("KL_STA1 = %x\r\n", ulVal);
        if (ulVal&KL_STA1_WT_FINISH)
        {
            break;
        }
        HI_MDM_Sleep(EFUSE_TIMECNT_TICK);
        ulCnt += EFUSE_TIMECNT_TICK;
    }  
    if (ulCnt >= EFUSE_TIMEOUT_DEFAULT)
    {
        HI_ERR_CIPHER("timeout\r\n");
        return HI_ERR_TIMEOUT;
    }
    if (ulVal&KL_STA1_WT_ERROR)
    {
        HI_ERR_CIPHER("Have Burned:%x\r\n", ulVal);
        return HI_ERR_EFUSE_ALREADY_BURN;
    }

    //回读比较 错误 
    if (KL_STA1_RB_CHECK_ERROR&ulVal)
    {     
        //错误状态内部保存或上报错误码
        HI_REG_READ32(RBC_WKEY0_STA, *(HI_U32*)ucErrState);  
        HI_REG_READ32(RBC_WKEY1_STA, *(HI_U32*)(ucErrState+0x4)); 
        HI_REG_READ32(RBC_WKEY2_STA, *(HI_U32*)(ucErrState+0x8)); 
        HI_REG_READ32(RBC_WKEY3_STA, *(HI_U32*)(ucErrState+0xc)); 
        return HI_ERR_EFUSE_READBACK;
    }
    else
    {
        (hi_void)memset_s(ucErrState, EFUSE_AREA_SIZE, 0, EFUSE_AREA_SIZE);
    }
    
    return HI_ERR_SUCCESS;
}

/*查询状态*/
HI_U32 HAL_EFUSE_CheckState(HI_U32 *pulState)
{
    HI_U32 ulVal;
    HI_U32 ulCnt = 0;

    if (HI_NULL == pulState)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    //等空闲
    if (HI_TRUE == hal_efuse_isbusy())
    {
        HI_ERR_CIPHER("Error Time out! \r\n");
        return HI_ERR_TIMEOUT;
    }
   
    HI_LOW_POWER_Forbid(HI_LOW_POWER_FORBID_ID_EFUSE);
     //锁定回读
    HI_REG_READ32(KL_CTRL, ulVal);  
    ulVal &= ~(KL_CTRL_MODE_MASK);
    ulVal |= KL_CTRL_MODE_START;
    HI_REG_WRITE32(KL_CTRL, ulVal); 
          
    while (ulCnt < EFUSE_TIMEOUT_DEFAULT)
    {
        HI_REG_READ32(KL_STA1, ulVal);  
        if (ulVal&KL_STA1_WT_FINISH)
        {
            break;
        }
        HI_MDM_Sleep(EFUSE_TIMECNT_TICK);
        ulCnt += EFUSE_TIMECNT_TICK;
    }
    HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_EFUSE, 0);
    if (ulCnt >= EFUSE_TIMEOUT_DEFAULT)
    {
        HI_ERR_CIPHER("timeout\r\n");
        return HI_ERR_TIMEOUT;
    }
    //查询4个aera状态
    *pulState = KL_STA1_BURN_STATE_MASK&ulVal;
    return HI_ERR_SUCCESS;

}

HI_U32 HAL_EFUSE_LoadCipher(HI_EFUSE_KEY_IDX_E enArea, HI_U32 ulKeyChannel)
{
    HI_U32 ulVal;
    HI_U32 ulCnt = 0;

    if ((ulKeyChannel >= CIPHER_KEY_CHANNEL_NUM)||(enArea >= EFUSE_AERA_MAX))
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    //等空闲
    if (HI_TRUE == hal_efuse_isbusy())
    {
        HI_ERR_CIPHER("Error Time out! \r\n");
        return HI_ERR_TIMEOUT;
    }
    
    HI_LOW_POWER_Forbid(HI_LOW_POWER_FORBID_ID_EFUSE);
    //设置地址和模式
    HI_REG_READ32(KL_CTRL, ulVal);  
    ulVal &= ~(KL_CTRL_CIPHER_ADDR_MASK|KL_CTRL_MODE_MASK|KL_CTRL_KEY_ADDR_MASK);
    ulVal |= KL_CTRL_MODE_CIPHER|(ulKeyChannel<<8)|KL_CTRL_MODE_START|((HI_U32)enArea<<4);
    HI_REG_WRITE32(KL_CTRL, ulVal); 
       
    //查询状态
    while (ulCnt < EFUSE_TIMEOUT_DEFAULT)
    {
        HI_REG_READ32(KL_STA, ulVal);  
        if (ulVal & KL_STA_CIPHER_FINISH)
        {
            break;
        }
        HI_MDM_Sleep(EFUSE_TIMECNT_TICK);
        ulCnt += EFUSE_TIMECNT_TICK;
    }
    HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_EFUSE, 0);
    if (ulCnt >= EFUSE_TIMEOUT_DEFAULT)
    {
        HI_ERR_CIPHER("timeout\r\n");
        return HI_ERR_TIMEOUT;
    }

    return HI_ERR_SUCCESS;
}

HI_U32 HAL_EFUSE_LoadHash(HI_EFUSE_KEY_IDX_E enArea)
{
    HI_U32 ulVal;
    HI_U32 ulCnt = 0;

    if (enArea >= EFUSE_AERA_MAX)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
   
    //等空闲
    if (HI_TRUE == hal_efuse_isbusy())
    {
        HI_ERR_CIPHER("Error Time out! \n");
        return HI_ERR_TIMEOUT;
    }

    HI_LOW_POWER_Forbid(HI_LOW_POWER_FORBID_ID_EFUSE);
    //设置地址和模式
    HI_REG_READ32(KL_CTRL, ulVal);  
    ulVal &= ~(KL_CTRL_CIPHER_ADDR_MASK|KL_CTRL_MODE_MASK|KL_CTRL_KEY_ADDR_MASK);
    ulVal |= KL_CTRL_MODE_HASH|KL_CTRL_MODE_START|((HI_U32)enArea<<4);
    HI_REG_WRITE32(KL_CTRL, ulVal); 
    
    //查询状态
    while (ulCnt < EFUSE_TIMEOUT_DEFAULT)
    {
        HI_REG_READ32(KL_STA, ulVal);  
        if (0 == (ulVal & KL_STA_HASH_FINISH))
        {
            break;
        }
        HI_MDM_Sleep(EFUSE_TIMECNT_TICK);
        ulCnt += EFUSE_TIMECNT_TICK;
    }
    HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_EFUSE, 0);
    if (ulCnt >= EFUSE_TIMEOUT_DEFAULT)
    {
        HI_ERR_CIPHER("timeout\r\n");
        return HI_ERR_TIMEOUT;
    }

    return HI_ERR_SUCCESS;
}

// HI_EFUSE_FREE_IDX_0-HI_EFUSE_FREE_IDX_4区域大小均为128bit; HI_EFUSE_FREE_IDX_5区域大小为120bit,高8bit的0-5bit表示free区的锁定状态，可读可写, 6-7bit为key的烧写状态，只读
HI_U32 HAL_EFUSE_Read(HI_EFUSE_IDX_E enEfuseFreeId, HI_U8 ucKeyData[EFUSE_AREA_SIZE])
{
    HI_U32 i;
    HI_U32 ulVal;
    HI_U32 ulFreeAreaAddStart = EFUSE_AREA_SIZE * enEfuseFreeId;
    HI_U32 ulFreeAreaAddEnd = ulFreeAreaAddStart + EFUSE_AREA_SIZE;
    HI_U32 ulCnt = 0;
    if ((enEfuseFreeId > HI_EFUSE_FREE_RW_IDX_5) || (!ucKeyData))
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    HI_REG_READ32(EFUSE_STATUS, ulVal); 
    if ((ulVal&EFUSE_STATUS_MASK) != EFUSE_STATUS_READY)
    {
        return HI_ERR_BUSY;
    }
    HI_LOW_POWER_Forbid(HI_LOW_POWER_FORBID_ID_EFUSE);
    for (i = ulFreeAreaAddStart; i < ulFreeAreaAddEnd; i++)
    {
        HI_REG_WRITE32(EFUSE_RD_ADDR, i);
        ulVal = HI_TRUE;
        HI_REG_WRITE32(EFUSE_RD_EN, ulVal);
        //查询状态
		ulCnt = 0;
        while (ulCnt < (EFUSE_TIMEOUT_DEFAULT*1000))
        {
            HI_REG_READ32(EFUSE_RD_EN, ulVal); 
            if (EFUSE_EN_OK == (ulVal&EFUSE_EN_SWITCH))
            {
				ulCnt = 0;
                break;
            }
            HI_MDM_Udelay(1);
            ulCnt ++;
        }  
        if (ulCnt >= (EFUSE_TIMEOUT_DEFAULT*1000))
        {
            HI_ERR_CIPHER("timeout\r\n");
            HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_EFUSE, 0);
            return HI_ERR_TIMEOUT;
        }
        HI_REG_READ32(EFUSE_RDATA, ulVal);     
        ucKeyData[i - ulFreeAreaAddStart] = (HI_U8)ulVal;//仅8位有效   
    }
    HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_EFUSE, 0);
    return HI_ERR_SUCCESS;
}

HI_U32 hal_efuse_writebit(HI_U16 usAddr)
{
    HI_U32 ulVal;
    HI_U32 ulCnt = 0;

    if (usAddr > EFUSE_PGM_ADDR_SIZE)
    {
    	return HI_ERR_INVALID_PARAMETER;
    }
    
    HI_REG_WRITE32(EFUSE_WT_ADDR, usAddr);
    HI_REG_WRITE32(EFUSE_WT_EN, HI_TRUE);
    
    //查询状态
    while (ulCnt < EFUSE_TIMEOUT_DEFAULT)
    {
        HI_REG_READ32(EFUSE_WT_EN, ulVal); 
        if (EFUSE_EN_OK == (ulVal&EFUSE_EN_SWITCH))
        {
            break;
        }
        HI_MDM_Sleep(EFUSE_TIMECNT_TICK);
        ulCnt += EFUSE_TIMECNT_TICK;
    }
    if (ulCnt >= EFUSE_TIMEOUT_DEFAULT)
    {
        HI_ERR_CIPHER("timeout\r\n");
        return HI_ERR_TIMEOUT;
    }
    return HI_ERR_SUCCESS;
}

#define TEST_SOC_EFUSE_ERR_REG
#ifdef TEST_SOC_EFUSE_ERR_REG
//仅显示最后一个比特烧写时的状态，不会记录历史的错误,所以在驱动中增加测试桩函数
HI_VOID HAL_EFUSE_GetErrReg(HI_U32 ucByteId, HI_U32 ucBitId, HI_U8 ucErrState[EFUSE_AREA_SIZE])
{
    HI_U32 ulVal = 0;

    for (;;)
    {
        HI_REG_READ32(EFUSE_STATUS, ulVal);
        if ((ulVal & EFUSE_STATUS_MASK) == EFUSE_STATUS_READY)
        {
            HI_REG_READ32(EFUSE_STATUS, ulVal);
            if (ulVal & EFUSE_CTRL_ST)
            {
                ucErrState[ucByteId] |= (HI_U8)(1 << ucBitId);
            }
            else
            {
                ucErrState[ucByteId] &= (~(HI_U8)(1 << ucBitId)); 
            }
            break;
        }

    }

}
#endif

// HI_EFUSE_FREE_IDX_0-HI_EFUSE_FREE_IDX_4区域大小均为128bit; HI_EFUSE_FREE_IDX_5区域大小为120bit
HI_U32 HAL_EFUSE_Write(HI_EFUSE_IDX_E enEfuseFreeId, HI_U8 ucKeyData[EFUSE_AREA_SIZE],HI_U8 ucErrState[EFUSE_AREA_SIZE])
{
        HI_U16 i = 0;
        HI_U16 j = 0;
        HI_U32 ulVal = 0;
        HI_U32 ulFreeAreaAddEnd;
        HI_U32 ulFreeAreaAddStart = EFUSE_AREA_SIZE * enEfuseFreeId;

        if (enEfuseFreeId > HI_EFUSE_FREE_RW_IDX_5)
        {
            return HI_ERR_INVALID_PARAMETER;
        }
    
        HI_REG_READ32(EFUSE_STATUS, ulVal); 
        if ((ulVal&EFUSE_STATUS_MASK) != EFUSE_STATUS_READY)
        {
            return HI_ERR_BUSY;
        }

        if (enEfuseFreeId == HI_EFUSE_FREE_RW_IDX_5)
        {
            ulFreeAreaAddEnd = FREE_AREA_LOCK_ADDR;
        }
        else
        {
            ulFreeAreaAddEnd = ulFreeAreaAddStart + EFUSE_AREA_SIZE;
        }
    
        HI_REG_WRITE32(EFUSE_AVDD_EN, EFUSE_AVDD_EN_EN); 

        for (i = (HI_U16)ulFreeAreaAddStart; i < ulFreeAreaAddEnd; i++)
        {
            for (j = 0; j < EFUSE_8_BIT; j++)
            {
                if (ucKeyData[i - ulFreeAreaAddStart] & (HI_U8)(1 << j))
                {
                    hal_efuse_writebit((HI_U16)(i * EFUSE_8_BIT+ j));
                    HAL_EFUSE_GetErrReg((i - ulFreeAreaAddStart),j,ucErrState);
                }
            }
        }

        // lock
        hal_efuse_writebit((HI_U16)(FREE_AREA_LOCK_ADDR * EFUSE_8_BIT + enEfuseFreeId));  
        return HI_ERR_SUCCESS;
}

HI_U32 HI_MDM_EFUSE_Read(HI_EFUSE_IDX_E enEfuseId, HI_U8 aucData[EFUSE_AREA_SIZE])
{
    return HAL_EFUSE_Read(enEfuseId, aucData);
}

HI_U32 HI_MDM_EFUSE_Write(HI_EFUSE_IDX_E enEfuseId, HI_U8 aucData[EFUSE_AREA_SIZE])
{
    HI_U32 ulRet = HI_ERR_FAILURE;
    HI_U32 i = 0;
    HI_U8 aucErrState[EFUSE_AREA_SIZE] = {0};
    
    if ((enEfuseId >= HI_EFUSE_IDX_MAX) || (!aucData))
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    HI_LOW_POWER_Forbid(HI_LOW_POWER_FORBID_ID_EFUSE);
    if (enEfuseId <= HI_EFUSE_FREE_RW_IDX_5)
    {
         ulRet = HAL_EFUSE_Write(enEfuseId, aucData, aucErrState);
    }
    else
    {
        enEfuseId -= HI_EFUSE_KEY_WO_IDX_0;/*lint !e656*/
        ulRet = HAL_EFUSE_BurnKey((HI_EFUSE_KEY_IDX_E)enEfuseId, aucData, aucErrState);
    }
    HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_EFUSE, 0);
    
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }
    for (i = 0; i < sizeof(aucErrState); i++)
    {        
        if (aucErrState[i])
        {
            return HI_ERR_CIPHER_EFUSE_WRT_BIT_FAIL;
        }
    }

    return ulRet;
}

