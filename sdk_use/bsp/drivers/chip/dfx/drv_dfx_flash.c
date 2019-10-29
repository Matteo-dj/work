#include <hi_types.h>

#ifdef PRODUCT_CFG_FLASH_ERASE_COUNT
#include "drv_dfx_cmn_inc.h"

/* 内部全局变量，用于统计擦次数最多的20个块号(sector)和对应的次数 */
static HI_DFX_FLASH_ERASE_INFO_ST s_flash_erase_info;

/* 寻找数组中是否已经记录该sector,已经记录返回TRUE */
HI_PRV HI_BOOL dfx_flash_check_already_in_array(HI_U32 sector, HI_U32 current_num)
{
    HI_U32 i;

    for(i=0; i<current_num; i++)
    {
        //根据已存储个数,for循环判断是否sector已存储
        if(s_flash_erase_info.max_info[i].sector_num == sector)
        {
            return HI_TRUE;
        }
    }

    return HI_FALSE;
}
//查找并存储20个erase最多的块到g_flash_erase_info[](结构体:号和次数)
HI_VOID dfx_update_flash_erase_info(HI_VOID)
{
    HI_U32 i,j;

     (hi_void)memset_s(&s_flash_erase_info,sizeof( HI_DFX_FLASH_ERASE_INFO_ST), 0,sizeof( HI_DFX_FLASH_ERASE_INFO_ST));
    /* 存储FLASH_ERASE_STATE_NUM个擦次数最多的块 */
    for(i = 0; i < FLASH_ERASE_STATE_NUM; i++)
    {
        //轮询所有块，获取未被统计的成员中的最大值
        for(j=0; j< FLASH_TOTAL_SECTOR_NUM; j++)
        {
            //判断是否已存储在时s_flash_erase_info
            if (HI_TRUE == dfx_flash_check_already_in_array(j, i))
            {
                continue;
            }
            if(s_flash_erase_info.max_info[i].erase_times < g_aulFlashEraseCount[j])
            {
                s_flash_erase_info.max_info[i].sector_num = j;
                s_flash_erase_info.max_info[i].erase_times = g_aulFlashEraseCount[j];
            }
        }
    }
}

/* 获取Flash Sector Erase信息 */
HI_U32 dfx_get_flash_erase_stat(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    dfx_update_flash_erase_info();

    HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)(&s_flash_erase_info),
        sizeof(HI_DFX_FLASH_ERASE_INFO_ST), HI_TRUE);
    return HI_ERR_SUCCESS;
}
#endif
#ifdef PRODUCT_CFG_FLASH_PROTECT

#define INVALID_VALUE 0xFF

HI_U32 dfx_flash_protect_op(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_DIAG_FLASH_CMD_S *cmdInfo = (HI_DIAG_FLASH_CMD_S*)pCmdParam;
    HI_DIAG_FLASH_PROTECT_INFO_IND_S info;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    (hi_void)memset_s(&info, sizeof(HI_DIAG_FLASH_PROTECT_INFO_IND_S), 0x0, sizeof(HI_DIAG_FLASH_PROTECT_INFO_IND_S));
    info.ulRet = HI_ERR_SUCCESS;    
    
    ulRet = HI_FLASH_DfxGetProtectInfo(&(info.bSupportProtect), &(info.ucStatusVal));
    if(HI_ERR_SUCCESS != ulRet)
    {
        info.bSupportProtect = INVALID_VALUE;
        info.ucStatusVal = INVALID_VALUE;
    }
    
    if(info.bSupportProtect)
    {
        if(HI_DIAG_FLASH_PROTECT_CMD_UNLOCK == cmdInfo->ulcmdPara)
        {
            info.ulRet = HI_FLASH_RemoveProtect();//当且仅当:当前flash支持保护功能时，且输入解保护，ulRet才为有效值，否则为INVALID_VALUE
            ulRet = HI_FLASH_DfxGetProtectInfo(&(info.bSupportProtect), &(info.ucStatusVal));
            if(HI_ERR_SUCCESS != ulRet)
            {
                info.bSupportProtect = INVALID_VALUE;
                info.ucStatusVal = INVALID_VALUE;
            }
        }
        else
        {
            info.ulRet = INVALID_VALUE;
        }
    }
    else
    {
        info.ulRet = INVALID_VALUE;
    } 
    
    HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)(&info), sizeof(HI_DIAG_FLASH_PROTECT_INFO_IND_S), HI_TRUE);
    return HI_ERR_SUCCESS;

}
#endif

