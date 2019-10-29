//*****************************************************************************
//
//                  版权所有 (C), 2001-2014, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cco_archives.c
//  版 本 号   : V1.0 
//  作    者   : cuiate/c00233580
//  生成日期   : 2014-10-30
//  功能描述   : 该模块提供表档案管理的功能接口定义，主要实现以下功能：
//               1)  表档案存储管理
//               2)  表档案相关操作函数，包括增删表项等
//
//               为了兼容旧版本，扩容后的新版本在第1025个表的位置增加了一个和
//               表信息大小相同的文件头2，存储以下信息：
//               1)  版本信息，从1开始，0预留给旧版本
//               2)  最大支持的节点数量，规格为2040
//               3)  当前存储的节点数量
//
//  函数列表   : 
//  修改历史   : 
//  1.日    期 : 2014-10-30
//    作    者 : cuiate/c00233580
//    修改内容 : 创建文件 
//
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_fw_proto376_2.h"
#include "mrs_fw_proto645.h" 
#include "mrs_fw_tools.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_proto376_2_frame.h"
#include "mrs_fw_log.h"
#include "mrs_fw_n.h"
#include "mrs_srv_common.h"
#include "mrs_fw_nv.h"
#include "mrs_srv_cco_broadcast.h"
#include "mrs_srv_list.h"
#include "mrs_srv_cco_event.h"
#include "mrs_srv_cco_remote_upg.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_cco_readmeter.h"
#include "mrs_srv_anytime_readmeter.h"
#include "mrs_srv_cco_searchmeter.h"
#include "mrs_srv_io.h"
#include "equip_dut_proc.h"
#include <app_nv.h>
#include "app_config.h"
#include "mrs_dfx.h"
#include "hi_mdm.h"
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
#include "mrs_srv_mrslog.h"
#endif
#include "mrs_srv_readmeter_manage.h"
#include "mrs_srv_cco_queue.h"
#include "mrs_dfx_cco.h"
#include "mrs_srv_cco_archives.h"
#include "mrs_srv_cco_archives_n.h"
#include "mrs_fixed_mem.h"



#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
//*****************************************************************************
//*****************************************************************************

HI_PRV MRS_METER_ARCHIVES_STRU g_stArchivesCtx = {0};
HI_PRV MRS_ARCHIVES_SEQ_TABLE_S g_stArchivesSeqTable = {0};


//*****************************************************************************
// 函数名称: mrsArchivesCtx
// 功能描述: 返回表档案上下文结构
// 参数说明: 无
// 返 回 值: 表档案上下文指针
// 作    者: cuiate/c00233580 [2014-10-30]
//*****************************************************************************
MRS_METER_ARCHIVES_STRU *mrsArchivesCtx(HI_VOID)
{
    return &g_stArchivesCtx;
}

//*****************************************************************************
// 函数名称: mrsSrvArchivesInit
// 功能描述: 表档案模块初始化
// 参数说明: 无
// 返 回 值: 
//   成功: HI_ERR_SUCCESS
//   失败: 其它值
// 作    者: cuiate/c00233580 [2014-10-30]
//*****************************************************************************
HI_U32 mrsSrvArchivesInit(HI_VOID)
{
    MRS_FIXED_MEM_PRAMS_STRU stParam;
    MRS_ARCHIVES_SEQ_NODE_S *pstSeq = HI_NULL;
    HI_S32 i = 0;
    HI_U16 usSeq = 0;

    if (HI_TRUE == g_stArchivesCtx.bInited)
    {
        return HI_ERR_SUCCESS;
    }

    (hi_void)memset_s(&g_stArchivesCtx, sizeof(g_stArchivesCtx), 0, sizeof(g_stArchivesCtx));

    stParam.ulInitItemCnt = MRS_SRV_FIXED_MEM_INIT_COUNT;
    stParam.ulIncItemCnt = MRS_SRV_FIXED_MEM_INC_COUNT;
    stParam.ulMaxItemCnt = PRODUCT_CFG_MRS_MAX_NODE_NUM;
    stParam.ulItemSize = sizeof(MRS_ARCHIVES_NODE_STRU);

    g_stArchivesCtx.pMeterInf = (HI_PVOID)mrsFixedMalloc(&stParam, MRS_SRV_ARCHIVES_DFX_INDEX);
    if (!g_stArchivesCtx.pMeterInf)
    {
        return HI_ERR_FAILURE;
    }

    g_stArchivesCtx.bInited = HI_TRUE;

    pstSeq = g_stArchivesSeqTable.stSeq;
    for (i = 0; i < MRS_ARCHIVES_BLOCK_CNT; i++,pstSeq++)
    {
        pstSeq->usMin = usSeq;

        usSeq += MRS_METERS_PER_BLOCK;
        if ((i == 0) || (i == MRS_ARCHIVES_HEAD2_BLOCK))
        {
            usSeq--;
        }

        pstSeq->usMax = usSeq - 1;
        if (pstSeq->usMax >= PRODUCT_CFG_MRS_MAX_NODE_NUM)
        {
            pstSeq->usMax = PRODUCT_CFG_MRS_MAX_NODE_NUM - 1;
            break;
        }
    }

    g_stArchivesSeqTable.bInited = HI_TRUE;
    g_stArchivesSeqTable.ucCount = (HI_U8)i + 1;

    mrsSetArchivesStatus(MRS_CCO_ARCHIVES_STATUS_EFFECT);

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: mrsSrvArchivesLoad
// 功能描述: 从FLASH中读取表档案
// 参数说明: 无
// 返 回 值: 
//   成功: HI_ERR_SUCCESS
//   失败: 其它值
// 作    者: cuiate/c00233580 [2014-10-30]
//*****************************************************************************
HI_U32 mrsSrvArchivesLoad(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_PBYTE pBuffer = HI_NULL;

    ulRet = mrsArchivesCheckHead();
    if (HI_ERR_SUCCESS != ulRet)
    {
        return HI_ERR_FAILURE;
    }

    pBuffer = (HI_PBYTE)mrsToolsMalloc(PRODUCT_CFG_FLASH_BLOCK_SIZE);
    if (!pBuffer)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memset_s(pBuffer, PRODUCT_CFG_FLASH_BLOCK_SIZE, 0, PRODUCT_CFG_FLASH_BLOCK_SIZE);

    do 
    {
        MRS_ARCHIVES_HEAD2_S stHead2;
        HI_U16 usMeterTotal = 0;
        HI_U32 i;

        // 读取文件头2所在块，判断数据有效性
        ulRet = HI_MDM_GetUserData(HI_USERDATA_ID_APP, MRS_ARCHIVES_HEAD2_BLOCK, 1, PRODUCT_CFG_FLASH_BLOCK_SIZE, pBuffer, HI_NULL);
        if (HI_ERR_SUCCESS != ulRet)
        {
            break;
        }

        usMeterTotal = PRODUCT_CFG_MRS_MAX_NODE_NUM;
        (hi_void)memset_s(&stHead2, sizeof(stHead2), 0, sizeof(stHead2));

        if (!mrsArchivesIsNullBlock(pBuffer))
        {
            (hi_void)memcpy_s(&stHead2, sizeof(stHead2), pBuffer + MRS_ARCHIVES_HEAD2_INDEX * MRS_ARCHIVES_NODE_SIZE, sizeof(stHead2));
            if (stHead2.ucVersion > MRS_ARCHIVES_FILE_VER_R1)
            {
                break;
            }
        }

        for (i = 0; i < MRS_ARCHIVES_BLOCK_CNT; i++)
        {
            (hi_void)memset_s(pBuffer, PRODUCT_CFG_FLASH_BLOCK_SIZE, 0, PRODUCT_CFG_FLASH_BLOCK_SIZE);
            ulRet = mrsLoadMeterFromFlash(i, pBuffer, &usMeterTotal);
            if (HI_ERR_SUCCESS != ulRet)
            {
                break;
            }
        }

        mrsArchivesSaveHead2(&stHead2, pBuffer);
    } while (0);

    mrsToolsFree(pBuffer);

    return ulRet;
}

//*****************************************************************************
// 函数名称: mrsSrvArchivesSave
// 功能描述: 存储表档案到FLASH
// 参数说明: 无
// 返 回 值: 
//   成功: HI_ERR_SUCCESS
//   失败: HI_ERR_MALLOC_FAILUE
// 作    者: cuiate/c00233580 [2014-10-30]
//*****************************************************************************
HI_U32 mrsSrvArchivesSave(HI_VOID)
{
    HI_PBYTE pBuffer = HI_NULL;
    HI_U32 i;

    pBuffer = (HI_PBYTE)mrsToolsMalloc(PRODUCT_CFG_FLASH_BLOCK_SIZE);
    if (!pBuffer)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    for (i = 0; i < MRS_ARCHIVES_BLOCK_CNT; i++)
    {
        (HI_VOID)mrsWriteMeterToFlash(i, pBuffer);
    }

    mrsArchivesCtx()->bChanged = HI_TRUE;
    mrsToolsFree(pBuffer);

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: mrsSrvArchivesInsert
// 功能描述: 添加节点(电表)信息到表档案列表末尾
// 参数说明: 
//   pstNode       [IN] 节点(电表)信息
//   bRepeatCheck  [IN] 重复检查标识
//
// 返 回 值: 
//   成功: HI_ERR_SUCCESS  (有重复表算添加成功)
//   失败: HI_ERR_FAILURE
//
// 作    者: cuiate/c00233580 [2014-11-11]
//*****************************************************************************
HI_U32 mrsSrvArchivesInsert(MRS_ARCHIVES_NODE_STRU *pstNode, HI_BOOL bRepeatCheck)
{
    MRS_METER_ARCHIVES_STRU *pstArchives = mrsArchivesCtx();
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if (bRepeatCheck)
    {
        HI_U16 i;
/* BEGIN: PN: DTS2015042907341 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/12 */
        HI_U8 aucMeterAddr[HI_METER_ADDR_LEN] = {0};
        HI_U8 aucTempAddr[HI_METER_ADDR_LEN] = {0};

        mrsMeterAddrPreProc(aucMeterAddr, pstNode->aucNodeAddr, pstNode->ucProtocol);

        for (i = 0; i < pstArchives->usMeterNum; i++)
        {
            MRS_ARCHIVES_NODE_STRU * pstTemp = mrsSrvArchivesQuery(i);
            if (pstTemp != HI_NULL)
            {
                mrsMeterAddrPreProc(aucTempAddr, pstTemp->aucNodeAddr, pstTemp->ucProtocol);
/* BEGIN: PN: DTS2015062304679 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/25 */
                if (mrsToolsMemEq(aucTempAddr, aucMeterAddr, HI_METER_ADDR_LEN))
/* END:   PN: DTS2015062304679 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/25 */
                {
                    return HI_ERR_SUCCESS;
                }
            }
        }
/* END:   PN: DTS2015042907341 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/12 */
    }

    ulRet = mrsFixedMemSetItem((MRS_FIXED_MEM_HEAD_PTR)pstArchives->pMeterInf, pstArchives->usMeterNum, pstNode, MRS_SRV_ARCHIVES_DFX_INDEX);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    pstArchives->usMeterNum++;

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: mrsSrvArchivesDelete
// 功能描述: 从表档案列表中删除节点(电表)信息
// 参数说明: 
//   pMeterAddr    [IN] 节点(电表)地址
//
// 返 回 值: 
//   成功: HI_ERR_SUCCESS
//   失败: HI_ERR_FAILURE
//
// 作    者: cuiate/c00233580 [2014-11-11]
//*****************************************************************************
HI_U32 mrsSrvArchivesDelete(HI_PBYTE pMeterAddr)
{
    MRS_METER_ARCHIVES_STRU *pstArchives = mrsArchivesCtx();
    MRS_ARCHIVES_NODE_STRU *pstNode = mrsSrvArchiversQueryByAddr(pMeterAddr);

    if (pstNode == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memset_s(pstNode, sizeof(MRS_ARCHIVES_NODE_STRU), 0, sizeof(MRS_ARCHIVES_NODE_STRU));
    pstNode->ucSave = HI_TRUE;
    pstArchives->usMeterNum--;

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: mrsSrvArchivesClear
// 功能描述: 清空表档案列表
// 参数说明: 无
// 返 回 值: 
//   成功: HI_ERR_SUCCESS
//   失败: HI_ERR_FAILURE
// 作    者: cuiate/c00233580 [2014-10-31]
//*****************************************************************************
HI_U32 mrsSrvArchivesClear(HI_VOID)
{
    MRS_METER_ARCHIVES_STRU *pstArchives = mrsArchivesCtx();
    MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;
    HI_U16 i;

    pstArchives->usMeterNum = 0;
    for (i = 0; i < PRODUCT_CFG_MRS_MAX_NODE_NUM; i++)
    {
        pstNode = mrsSrvArchivesQuery(i);
        if (pstNode == HI_NULL)
        {
            return HI_ERR_SUCCESS;
        }

        (hi_void)memset_s(pstNode, sizeof(MRS_ARCHIVES_NODE_STRU), 0, sizeof(MRS_ARCHIVES_NODE_STRU));
        pstNode->ucSave = HI_TRUE;
    }

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: mrsSrvArchivesRealign
// 功能描述: 表档案列表重新排列，用后面的节点(电表)信息补齐空位
// 参数说明: 无
// 返 回 值: 
//   成功: HI_ERR_SUCCESS
//   失败: HI_ERR_FAILURE
// 作    者: cuiate/c00233580 [2014-11-11]
//*****************************************************************************
HI_U32 mrsSrvArchivesRealign(HI_VOID)
{
    HI_U16 usIndex = 0;
    HI_U16 usSource = 0;

    while (usIndex < PRODUCT_CFG_MRS_MAX_NODE_NUM)
    {
        MRS_ARCHIVES_NODE_STRU *pstNode = mrsSrvArchivesQuery(usIndex);
        if ((HI_NULL != pstNode) && !pstNode->bValid)
        {
            usSource = MRS_MAX(usSource + 1, usIndex + 1);
            if (usSource >= PRODUCT_CFG_MRS_MAX_NODE_NUM)
            {
                break;
            }

            while (usSource < PRODUCT_CFG_MRS_MAX_NODE_NUM)
            {
                MRS_ARCHIVES_NODE_STRU *pstTemp = mrsSrvArchivesQuery(usSource);

                if (pstTemp == HI_NULL)
                {
                    break;
                }

                if (pstTemp->bValid)
                {
                    (hi_void)memcpy_s(pstNode, sizeof(MRS_ARCHIVES_NODE_STRU), pstTemp, sizeof(MRS_ARCHIVES_NODE_STRU));
                    pstNode->ucSave = HI_TRUE;

                    (hi_void)memset_s(pstTemp, sizeof(MRS_ARCHIVES_NODE_STRU), 0, sizeof(MRS_ARCHIVES_NODE_STRU));
                    pstTemp->ucSave = HI_TRUE;
                    pstTemp->bValid = HI_FALSE;
                    break;
                }

                usSource++;
            }
        }

        usIndex++;
    }

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: mrsSrvArchivesQuery
// 功能描述: 根据索引号查询节点(电表)信息
// 参数说明: 
//   usIndex       [IN]  节点(电表)索引号
//
// 返 回 值: 
//   成功: 节点(电表)信息指针
//   失败: HI_NULL
// 作    者: cuiate/c00233580 [2014-11-11]
//*****************************************************************************
MRS_ARCHIVES_NODE_STRU *mrsSrvArchivesQuery(HI_U16 usIndex)
{
    return (MRS_ARCHIVES_NODE_STRU *)mrsFixedMemGetItem((MRS_FIXED_MEM_HEAD_PTR)mrsArchivesCtx()->pMeterInf, usIndex);
}

//*****************************************************************************
// 函数名称: mrsSrvArchivesMeterNum
// 功能描述: 返回表档案中节点(电表)数量
// 参数说明: 无
// 返 回 值: 
//   节点(电表)数量
// 作    者: cuiate/c00233580 [2014-11-11]
//*****************************************************************************
HI_U16 mrsSrvArchivesMeterNum(HI_VOID)
{
    return mrsArchivesCtx()->usMeterNum;
}

//*****************************************************************************
// 函数名称: mrsSrvArchivesIsChanged
// 功能描述: 表档案变化标识
// 参数说明: 无
// 返 回 值: 
//   HI_TRUE:  表档案有变化
//   HI_FALSE: 表档案无变化
// 作    者: cuiate/c00233580 [2014-11-11]
//*****************************************************************************
HI_BOOL mrsSrvArchivesIsChanged(HI_VOID)
{
    return mrsArchivesCtx()->bChanged;
}

//*****************************************************************************
// 函数名称: mrsSrvArchivesChangedFlagClear
// 功能描述: 清除表档案变化标识
// 参数说明: 无
// 返 回 值: 无
// 作    者: cuiate/c00233580 [2014-11-11]
//*****************************************************************************
HI_VOID mrsSrvArchivesChangedFlagClear(HI_VOID)
{
    mrsArchivesCtx()->bChanged = HI_FALSE;
}

//*****************************************************************************
// 函数名称: mrsSrvArchivesReadFlagClear
// 功能描述: 清除表档案抄读标识
// 参数说明: 无
// 返 回 值: 
//   成功: HI_ERR_SUCCESS
//   失败: HI_ERR_FAILURE
// 作    者: cuiate/c00233580 [2014-11-07]
//*****************************************************************************
HI_U32 mrsSrvArchivesReadFlagClear(HI_VOID)
{
    MRS_METER_ARCHIVES_STRU *pstArchives = mrsArchivesCtx();
    MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;
    HI_U16 i;

    for (i = 0; i < pstArchives->usMeterNum; i++)
    {
        pstNode = mrsSrvArchivesQuery(i);
        if (pstNode != HI_NULL)
        {
            pstNode->ucRead = HI_FALSE;
        }
    }

    return HI_ERR_SUCCESS;
}



//*****************************************************************************
// 函数名称: mrsArchivesCheckHead
// 功能描述: 检查表档案文件头
// 参数说明: 无
// 返 回 值: 
//   成功: HI_ERR_SUCCESS
//   失败: 其它值
// 作    者: cuiate/c00233580 [2014-10-31]
//*****************************************************************************
HI_U32 mrsArchivesCheckHead(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U32 ulFlag = MRS_ARCHIVES_HEAD_FLAG;
    HI_U16 usVer = MRS_ARCHIVES_HEAD_VERSION;
    HI_PBYTE pBuffer = HI_NULL;

    pBuffer = (HI_PBYTE)mrsToolsMalloc(PRODUCT_CFG_FLASH_BLOCK_SIZE);
	if (pBuffer == HI_NULL)
	{
		return HI_ERR_MALLOC_FAILUE;
	}
	
    ulRet = HI_MDM_GetUserData(HI_USERDATA_ID_APP, 0, 1, PRODUCT_CFG_FLASH_BLOCK_SIZE, pBuffer, HI_NULL);
    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsToolsFree(pBuffer);
        return HI_ERR_FAILURE;
    }

    if (mrsToolsMemEq(&ulFlag, pBuffer, sizeof(ulFlag))
        && mrsToolsMemEq(&usVer, pBuffer + sizeof(ulFlag), sizeof(usVer)))
    {
        mrsToolsFree(pBuffer);
        return HI_ERR_SUCCESS;
    }

    mrsToolsFree(pBuffer);
    return HI_ERR_FAILURE;
}

//*****************************************************************************
// 函数名称: mrsLoadMeterFromFlash
// 功能描述: 加载FLASH块中的表地址到表档案列表
// 参数说明: 
//   ulBlockId     [IN]    存储表档案的FLASH块号,从0开始
//   pBuffer       [IN]    预分配好的4KB的内存
//   pusMeterRemain[INOUT] 尚未加载的表数量
// 返 回 值:
//   成功: HI_ERR_SUCCESS
//   失败: 其它值
// 作    者: cuiate/c00233580 [2014-10-31]
//*****************************************************************************
HI_U32 mrsLoadMeterFromFlash(HI_U32 ulBlockId, HI_PBYTE pBuffer, HI_U16 *pusMeterTotal)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U32 ulSize = PRODUCT_CFG_FLASH_BLOCK_SIZE;
    HI_U32 ulOffset = 0;
    HI_U16 usMeterCnt = g_stArchivesSeqTable.stSeq[ulBlockId].usMin;

    ulRet = HI_MDM_GetUserData(HI_USERDATA_ID_APP, ulBlockId, 1, (HI_U16)ulSize, pBuffer, HI_NULL);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    if (0 == ulBlockId)
    {
        ulOffset += MRS_ARCHIVES_HEAD_SIZE;
    }

    while ((ulOffset + MRS_ARCHIVES_NODE_SIZE) <= ulSize)
    {
        HI_U8 ucCS = 0;

        if ((MRS_ARCHIVES_HEAD2_BLOCK == ulBlockId) && (ulOffset == MRS_ARCHIVES_HEAD2_INDEX * MRS_ARCHIVES_NODE_SIZE))
        {
            MRS_ARCHIVES_HEAD2_S *pstHead2 = (MRS_ARCHIVES_HEAD2_S *)(pBuffer + ulOffset);

            if (!pstHead2->ucVersion)
            {
                return HI_ERR_NO_MORE_DATA;
            }

            *pusMeterTotal = MRS_MIN(pstHead2->usNodeCur, PRODUCT_CFG_MRS_MAX_NODE_NUM);

            // 文件头2，跳过
            ulOffset += MRS_ARCHIVES_NODE_SIZE;
        }

        if (*pusMeterTotal <= usMeterCnt)
        {
            return HI_ERR_NO_MORE_DATA;
        }

        if (0 == *(pBuffer + ulOffset))
        {
            // 标志位无效
            return HI_ERR_NO_MORE_DATA;
        }

        ucCS = mrsToolsCalcCheckSum(pBuffer + ulOffset, MRS_ARCHIVES_NODE_SIZE - 1);
        if (ucCS == *(pBuffer + ulOffset + MRS_ARCHIVES_NODE_SIZE - 1))
        {
            ulRet = mrsSrvArchivesInsert((MRS_ARCHIVES_NODE_STRU *)(pBuffer + ulOffset), HI_FALSE);
            if (HI_ERR_SUCCESS != ulRet)
            {
                return ulRet;
            }
        }

        usMeterCnt++;
        ulOffset += MRS_ARCHIVES_NODE_SIZE;
    }

    return ulRet;
}

//*****************************************************************************
// 函数名称: mrsWriteMeterToFlash
// 功能描述: 写表档案列表到FLASH块中
// 参数说明: 
//   ulBlockId     [IN]    存储表档案的FLASH块号,从0开始
//   pBuffer       [IN]    预分配好的4KB的内存
//
// 返 回 值:
//   成功: HI_ERR_SUCCESS
//   失败: 其它值
// 作    者: cuiate/c00233580 [2014-10-31]
//*****************************************************************************
HI_U32 mrsWriteMeterToFlash(HI_U32 ulBlockId, HI_PBYTE pBuffer)
{
    MRS_ARCHIVES_SEQ_NODE_S *pstSeq = &(g_stArchivesSeqTable.stSeq[ulBlockId]);
    MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;
    HI_U16 i;
    HI_U32 ulOffset = 0;
    HI_BOOL bSave = HI_FALSE;

    if (MRS_ARCHIVES_HEAD2_BLOCK == ulBlockId)
    {
        bSave = HI_TRUE;
    }
    else
    {
        for (i = pstSeq->usMin; i <= pstSeq->usMax; i++)
        {
            pstNode = mrsSrvArchivesQuery(i);
            if (HI_NULL == pstNode)
            {
                break;
            }

            if (pstNode->ucSave)
            {
                bSave = HI_TRUE;
                break;
            }
        }
    }

    if (HI_FALSE == bSave)
    {
        return HI_ERR_SUCCESS;
    }

    (hi_void)memset_s(pBuffer, PRODUCT_CFG_FLASH_BLOCK_SIZE, 0, PRODUCT_CFG_FLASH_BLOCK_SIZE);

    if (0 == ulBlockId)
    {
        MRS_ARCHIVES_HEAD_S stHead = {MRS_ARCHIVES_HEAD_FLAG, MRS_ARCHIVES_HEAD_VERSION, PRODUCT_CFG_MRS_MAX_NODE_NUM};

        (hi_void)memcpy_s(pBuffer + ulOffset, sizeof(stHead), &stHead, sizeof(stHead));
        ulOffset += MRS_ARCHIVES_HEAD_SIZE;
    }

    for (i = pstSeq->usMin; i <= pstSeq->usMax; i++)
    {
        if (MRS_ARCHIVES_HEAD2_SEQ == i)
        {
            MRS_ARCHIVES_HEAD2_S stHead2 = {0};
            (hi_void)memset_s(&stHead2, sizeof(stHead2), 0, sizeof(stHead2));
            stHead2.ucVersion = MRS_ARCHIVES_FILE_VER_R1;
            stHead2.usNodeMax = PRODUCT_CFG_MRS_MAX_NODE_NUM;
            stHead2.usNodeCur = g_stArchivesCtx.usMeterNum;

            (hi_void)memcpy_s(pBuffer + ulOffset, sizeof(stHead2), &stHead2, sizeof(stHead2));
            ulOffset += MRS_ARCHIVES_HEAD_SIZE;
        }

        pstNode = mrsSrvArchivesQuery(i);
        if (pstNode != HI_NULL)
        {
            HI_U8 ucRead = pstNode->ucRead;
			HI_U8 ucDetect = pstNode->ucDetect;
			HI_U8 ucRmTurn = pstNode->ucRmTurn;

            pstNode->ucSave = HI_FALSE;
            pstNode->ucRead = HI_FALSE;
			pstNode->ucDetect = HI_FALSE;
			pstNode->ucRmTurn = HI_FALSE;
            pstNode->ucCS = mrsToolsCalcCheckSum((HI_PBYTE)pstNode, MRS_ARCHIVES_NODE_SIZE - 1);
            (hi_void)memcpy_s(pBuffer + ulOffset, MRS_ARCHIVES_NODE_SIZE, pstNode, MRS_ARCHIVES_NODE_SIZE);

            pstNode->ucRead = ucRead;
			pstNode->ucDetect = ucDetect;
			pstNode->ucRmTurn = ucRmTurn;
        }

        ulOffset += MRS_ARCHIVES_NODE_SIZE;
    }

    return HI_MDM_SaveUserData(HI_USERDATA_ID_APP, ulBlockId, 1, PRODUCT_CFG_FLASH_BLOCK_SIZE, pBuffer);
}

//*****************************************************************************
// 函数名称: mrsMeterFileSave2ndHead
// 功能描述: 存储文件头2
// 参数说明: 
//   pstHead2      [IN] 文件头2的信息
//   pBuffer       [IN] 预先分配好的4KB的内存
//
// 返 回 值: 无
// 作    者: cuiate/c00233580 [2014-10-31]
//*****************************************************************************
HI_VOID mrsArchivesSaveHead2(MRS_ARCHIVES_HEAD2_S *pstHead2, HI_U8 *pBuffer)
{
    MRS_METER_ARCHIVES_STRU *pstArchives = mrsArchivesCtx();
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_ARCHIVES_HEAD2_S stHead = {0};

    mrsDfxLrMeterNum(pstArchives->usMeterNum);

    if ((PRODUCT_CFG_MRS_MAX_NODE_NUM == pstHead2->usNodeMax)
        && (pstArchives->usMeterNum == pstHead2->usNodeCur)
        && (MRS_ARCHIVES_FILE_VER_R1 == pstHead2->ucVersion))
    {
        return;
    }

    (hi_void)memset_s(pBuffer, PRODUCT_CFG_FLASH_BLOCK_SIZE, 0, PRODUCT_CFG_FLASH_BLOCK_SIZE);
    ulRet = HI_MDM_GetUserData(HI_USERDATA_ID_APP, MRS_ARCHIVES_HEAD2_BLOCK, 1, PRODUCT_CFG_FLASH_BLOCK_SIZE, pBuffer, HI_NULL);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return;
    }

    if (mrsArchivesIsNullBlock(pBuffer))
    {
        (hi_void)memset_s(pBuffer, PRODUCT_CFG_FLASH_BLOCK_SIZE, 0, PRODUCT_CFG_FLASH_BLOCK_SIZE);
    }

    (hi_void)memset_s(&stHead, sizeof(stHead), 0, sizeof(stHead));
    stHead.ucVersion = MRS_ARCHIVES_FILE_VER_R1;
    stHead.usNodeCur = pstArchives->usMeterNum;
    stHead.usNodeMax = PRODUCT_CFG_MRS_MAX_NODE_NUM;

    (hi_void)memcpy_s(pBuffer + MRS_ARCHIVES_NODE_SIZE * MRS_ARCHIVES_HEAD2_INDEX, MRS_ARCHIVES_NODE_SIZE, &stHead, MRS_ARCHIVES_NODE_SIZE);

    HI_MDM_SaveUserData(HI_USERDATA_ID_APP, MRS_ARCHIVES_HEAD2_BLOCK, 1, PRODUCT_CFG_FLASH_BLOCK_SIZE, pBuffer);
}

//*****************************************************************************
// 函数名称: mrsArchivesIsNullBlock
// 功能描述: 判断数据是否为全FF
// 参数说明: 
//   pBuffer       [IN] FLASH块数据指针
//
// 返 回 值: 无
// 作    者: cuiate/c00233580 [2014-10-31]
//*****************************************************************************
HI_BOOL mrsArchivesIsNullBlock(HI_PBYTE pBuffer)
{
    HI_U32 i = 0;

    for (i = 0; i < PRODUCT_CFG_FLASH_BLOCK_SIZE; i++)
    {
        if (*(pBuffer + i) != 0xFF)
        {
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}


/**
 @brief  Query meter information in archives according meter address.  CNcomment: 根据电表地址查询表档案中个电表信息。CNend
 @param[in] pMeterAddr  Meter address.  CNcomment: 电表地址。CNend
 @retval ::Meter node information pointer  success.   CNcomment: 成功。CNend
 @retval ::HI_NULL  failed.   CNcomment: 失败。CNend
 */
MRS_ARCHIVES_NODE_STRU * mrsSrvArchiversQueryByAddr(HI_PBYTE pMeterAddr)
{
    MRS_ARCHIVES_NODE_STRU *pstNode = HI_NULL;
    HI_U16 i;
    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN];
    HI_U8 aucTempAddr[HI_METER_ADDR_LEN];

    mrsMeterAddrPreProc(aucMeterAddr, pMeterAddr, MRS_PROTO645_VER_1997);

    for (i = 0; i < PRODUCT_CFG_MRS_MAX_NODE_NUM; i++)
    {
        pstNode = mrsSrvArchivesQuery(i);
        if (pstNode == HI_NULL)
        {
            break;
        }
        
        if (!pstNode->bValid)
        {
            continue;
        }

        mrsMeterAddrPreProc(aucTempAddr, pstNode->aucNodeAddr, pstNode->ucProtocol);
        if (mrsToolsMemEq(aucMeterAddr, aucTempAddr, HI_METER_ADDR_LEN))
        {
            return pstNode;
        }
    }

    return HI_NULL;
}




//*****************************************************************************
//*****************************************************************************
#endif //defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

