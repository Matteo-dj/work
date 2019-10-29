//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_baudrate_manage.c
//  版 本 号   : V1.0 
//  作    者   : liuxipeng/KF54842
//  生成日期   : 2014-06-14
//  功能描述   : 波特率管理
//               实现所有场景下对业务串口的波特率管理
//               
//  函数列表   : 
//  修改历史   : 
//  1.日    期 : 2014-06-14
//    作    者 : liuxipeng/KF54842
//    修改内容 : 创建文件 
//
//*****************************************************************************
#include "mrs_common.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_n.h"
#include "mrs_srv_common.h"
#include <app_nv.h>
#include "mrs_srv_baudrate_manage.h"


#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

typedef struct
{
    MRS_SRV_BAUDRATE_CFG_S stCfgItem[MRS_SRV_ID_BAUDRATE_MAX];
} MRS_SRV_BAUDRATE_CFG_TABLE_S, *P_MRS_SRV_BAUDRATE_CFG_TABLE_S;

// 根据NV项配置创建波特率配置列表
HI_PRV HI_VOID mrsCreateBaudRateCfgTable(P_MRS_SRV_BAUDRATE_CFG_TABLE_S, NV_APP_BAUDRATE_CFG_STRU *);

HI_PRV HI_U16 g_usBaudRateList[] = { MRS_SRV_BAUD_RATE_600,     // 对应NV项索引-1
                                     MRS_SRV_BAUD_RATE_1200,    // 对应NV项索引-2
                                     MRS_SRV_BAUD_RATE_2400,    // 对应NV项索引-3
                                     MRS_SRV_BAUD_RATE_4800,    // 对应NV项索引-4
                                     MRS_SRV_BAUD_RATE_9600,    // 对应NV项索引-5
                                     MRS_SRV_BAUD_RATE_19200,   // 对应NV项索引-6
                                     MRS_SRV_BAUD_RATE_38400    // 对应NV项索引-7
                                   };

HI_PRV MRS_SRV_BAUDRATE_CFG_TABLE_S g_MrsBaudRateCfgTable;

HI_PRV NV_APP_BAUDRATE_CFG_STRU g_usBaudRateCfgNvDefault = {0x0002,     // 97搜表     (1200)
                                                            0x0003,     // 07搜表     (2400) 
                                                            0x001D,     // 698搜表    (9600 2400) 
                                                            0x0002,     // 97抄表     (1200)
                                                            0x0003,     // 07抄表     (2400) 
                                                            0x001D,     // 698抄表    (9600 2400) 
                                                            0x0013,     // 抄表保留1
                                                            0x015A,     // 校时       (1200, 2400, 9600)
                                                            0x002B,     // 探测波特率 (2400, 9600)
                                                            {0}};

#define MRS_GET_BAUDRATE_LIST() ((HI_U16 *)g_usBaudRateList)
#define MRS_GET_BAUDRATE_CFG_TABLE() ((P_MRS_SRV_BAUDRATE_CFG_TABLE_S)&g_MrsBaudRateCfgTable)


// 根据NV项配置创建波特率配置列表
HI_PRV HI_VOID mrsCreateBaudRateCfgTable(P_MRS_SRV_BAUDRATE_CFG_TABLE_S pstTable,
                                                    NV_APP_BAUDRATE_CFG_STRU *pstNvCfg)
{
    HI_U16 *pusMapList = MRS_GET_BAUDRATE_LIST();
    HI_U32 i = 0, j = 0;
    HI_U16 usCfg = 0;
    
    if((!pstTable) || (!pstNvCfg))
    {
        return;
    }

    // 根据Nv项配置初始化配置表
    for(i = 0; i < MRS_SRV_ID_BAUDRATE_MAX; i++)
    {   
        P_MRS_SRV_BAUDRATE_CFG_S item = &pstTable->stCfgItem[i];
        
        usCfg = ((HI_U16 *)pstNvCfg)[i];

        if(!(usCfg & 0x7FFF))
        {
            usCfg = ((HI_U16 *)&g_usBaudRateCfgNvDefault)[i];
        }
        
        for(j = 0; j < MRS_SRV_RATE_MAX; j++)
        {
            HI_U16 usIndex = 0;
            
            usIndex = ((usCfg) & (0x07 << (j * 3))) >> (j * 3);
            if(usIndex)
            {
                usIndex--;
                item->usBaudRateList[item->ucValidNum++] = pusMapList[usIndex];
            }
        }
    }

    return;
}

// 获取指定ID的波特率配置
// 返回空指针不可使用
// 调用者通过返回值自行判断内容是否合法
// 输出内容中若波特率个数为0则说明对应该ID的波特率配置无效
P_MRS_SRV_BAUDRATE_CFG_S mrsSrvGetBaudRateCfg(MRS_SRV_BAUDRATE_ID_E enCfgId)
{
    P_MRS_SRV_BAUDRATE_CFG_TABLE_S pstCfgTable = MRS_GET_BAUDRATE_CFG_TABLE();
    P_MRS_SRV_BAUDRATE_CFG_S pstCfg = HI_NULL;
    
    if(enCfgId >= MRS_SRV_ID_BAUDRATE_MAX)
    {
        return HI_NULL;
    }

    pstCfg = &pstCfgTable->stCfgItem[enCfgId];

    // 至少有一个有效才返回内容
    if(pstCfg->ucValidNum)
    {
        return pstCfg;
    }
    else
    {
        return HI_NULL;
    }
}


// 根据NV配置初始化波特率配置表
HI_U32 mrsSrvInitBaudRateCfgTable(HI_U16 usId)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    NV_APP_BAUDRATE_CFG_STRU stBaudRateCfgNv = {0};
    P_MRS_SRV_BAUDRATE_CFG_TABLE_S pstCfgTable = MRS_GET_BAUDRATE_CFG_TABLE();

    MRS_NOT_USED(usId);
    
    ret = HI_MDM_NV_Read(ID_NV_APP_BAUDRATE_CFG,
                        &stBaudRateCfgNv, sizeof(NV_APP_BAUDRATE_CFG_STRU));
    if(ret != HI_ERR_SUCCESS)
    {
        (hi_void)memcpy_s(&stBaudRateCfgNv, sizeof(stBaudRateCfgNv),
                        &g_usBaudRateCfgNvDefault, 
                        sizeof(NV_APP_BAUDRATE_CFG_STRU));
    }
    
    (hi_void)memset_s(pstCfgTable, sizeof(MRS_SRV_BAUDRATE_CFG_TABLE_S), 0, sizeof(MRS_SRV_BAUDRATE_CFG_TABLE_S));
    
    mrsCreateBaudRateCfgTable(pstCfgTable, &stBaudRateCfgNv);
    
    return HI_ERR_SUCCESS;
}


HI_U32 mrsSrvBaudRate2Index(HI_IN HI_U16 baudrate, HI_OUT HI_U8 *index)
{
    HI_U16 *brList = g_usBaudRateList;
    HI_U32 size = MRS_TOOLS_ALEN(g_usBaudRateList);
    HI_U32 i;

    for (i = 0; i < size; i++)
    {
        if (brList[i] == baudrate)
        {
            *index = (HI_U8)(i + 1);
            return HI_ERR_SUCCESS;
        }
    }

    return HI_ERR_FAILURE;
}


HI_U32 mrsSrvIndex2BaudRate(HI_IN HI_U8 index, HI_OUT HI_U16 *baudrate)
{
    HI_U16 *brList = g_usBaudRateList;
    HI_U32 size = MRS_TOOLS_ALEN(g_usBaudRateList);

    if (index > 0 && index <= size)
    {
        *baudrate = brList[index - 1];
        return HI_ERR_SUCCESS;
    }

    *baudrate = MRS_SRV_BAUD_RATE_DEFAULT;
    return HI_ERR_SUCCESS;
}


#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_STA)


