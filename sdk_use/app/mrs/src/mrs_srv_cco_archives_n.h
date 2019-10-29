//*****************************************************************************
//
//                  版权所有 (C), 2001-2014, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cco_archives_n.h
//  版 本 号   : V1.0 
//  作    者   : cuiate/c00233580
//  生成日期   : 2014-10-30
//  功能描述   : 表档案管理模块内部接口声明和宏定义
//
//  函数列表   : 
//  修改历史   : 
//  1.日    期 : 2014-10-30
//    作    者 : cuiate/c00233580
//    修改内容 : 创建文件 
//
//*****************************************************************************

#ifndef _MRS_SRV_ARCHIVES_N_H_
#define _MRS_SRV_ARCHIVES_N_H_
HI_START_HEADER

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
//*****************************************************************************
//*****************************************************************************

#define MRS_ARCHIVES_HEAD_FLAG          (0x1A2B3C4D)
#define MRS_ARCHIVES_HEAD_VERSION       (0x1140)

#define MRS_ARCHIVES_NODE_SIZE          (12)
#define MRS_ARCHIVES_HEAD_SIZE          (MRS_ARCHIVES_NODE_SIZE)
#define MRS_METERS_PER_BLOCK            (PRODUCT_CFG_FLASH_BLOCK_SIZE / MRS_ARCHIVES_NODE_SIZE)
#define MRS_SPARE_PER_BLOCK             (PRODUCT_CFG_FLASH_BLOCK_SIZE % MRS_ARCHIVES_NODE_SIZE)

#define MRS_ARCHIVES_FLASH_SIZE         (PRODUCT_CFG_USERDATA_CCO_APP_MAX_SIZE - PRODUCT_CFG_FLASH_BLOCK_SIZE)
#define MRS_ARCHIVES_BLOCK_CNT          ((PRODUCT_CFG_MRS_MAX_NODE_NUM + 2 + MRS_METERS_PER_BLOCK - 1) / MRS_METERS_PER_BLOCK)

#define MRS_ARCHIVES_HEAD2_SEQ          (1024)
#define MRS_ARCHIVES_HEAD2_BLOCK        ((MRS_ARCHIVES_HEAD2_SEQ + 1) / MRS_METERS_PER_BLOCK)
#define MRS_ARCHIVES_HEAD2_INDEX        ((MRS_ARCHIVES_HEAD2_SEQ + 1) % MRS_METERS_PER_BLOCK)


//*****************************************************************************
//*****************************************************************************

typedef struct 
{
    HI_U16  usMeterNum; // 当前从节点(电表)数量
    HI_BOOL bChanged;   // 表档案变更标识
    HI_BOOL bInited;    // 初始化标识
    HI_PVOID pMeterInf; // 内存增量分配的从节点(电表)指针头信息
                        // 节点类型MRS_ARCHIVES_NODE_STRU
} MRS_METER_ARCHIVES_STRU;


typedef struct  
{
    HI_U32  ulFlag;         // 文件标识(MRS_ARCHIVES_HEAD_FLAG)
    HI_U16  usVersion;      // 版本信息(MRS_ARCHIVES_HEAD_VERSION)
    HI_U16  usNodeMax;      // 最大支持的节点数量
} MRS_ARCHIVES_HEAD_S;


typedef enum
{
    MRS_ARCHIVES_FILE_VER_R0,
    MRS_ARCHIVES_FILE_VER_R1,
} MRS_ARCHIVES_FILE_VERSION_E;


typedef struct  
{
    HI_U8   ucVersion;      // 版本信息
    HI_U8   aucReserved[7];
    HI_U16  usNodeMax;      // 最大支持的节点数量
    HI_U16  usNodeCur;      // 当前节点数量
} MRS_ARCHIVES_HEAD2_S;


// 每块FLASH上存储的表地址序号信息
typedef struct  
{
    HI_U16  usMin;          // 序号最小值
    HI_U16  usMax;          // 序号最大值
} MRS_ARCHIVES_SEQ_NODE_S;


// FLASH上存储的表地址序号信息表
typedef struct
{
    HI_BOOL bInited;
    HI_U8   ucCount;
    HI_U8   ucReserved[2];
    MRS_ARCHIVES_SEQ_NODE_S stSeq[MRS_ARCHIVES_BLOCK_CNT];
} MRS_ARCHIVES_SEQ_TABLE_S;



//*****************************************************************************
// 函数名称: mrsGetArchivesCtx
// 功能描述: 返回表档案上下文结构
// 参数说明: 无
// 返 回 值: 表档案上下文指针
// 作    者: cuiate/c00233580 [2014-10-30]
//*****************************************************************************
MRS_METER_ARCHIVES_STRU *mrsArchivesCtx(HI_VOID);

//*****************************************************************************
// 函数名称: mrsArchivesCheckHead
// 功能描述: 检查表档案文件头
// 参数说明: 无
// 返 回 值: 
//   成功: HI_ERR_SUCCESS
//   失败: 其它值
// 作    者: cuiate/c00233580 [2014-10-31]
//*****************************************************************************
HI_U32 mrsArchivesCheckHead(HI_VOID);

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
HI_U32 mrsLoadMeterFromFlash(HI_U32 ulBlockId, HI_PBYTE pBuffer, HI_U16 *pusMeterTotal);

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
HI_U32 mrsWriteMeterToFlash(HI_U32 ulBlockId, HI_PBYTE pBuffer);

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
HI_VOID mrsArchivesSaveHead2(MRS_ARCHIVES_HEAD2_S *pstHead2, HI_U8 *pBuffer);

//*****************************************************************************
// 函数名称: mrsArchivesIsNullBlock
// 功能描述: 判断数据是否为全FF
// 参数说明: 
//   pBuffer       [IN] FLASH块数据指针
//
// 返 回 值: 无
// 作    者: cuiate/c00233580 [2014-10-31]
//*****************************************************************************
HI_BOOL mrsArchivesIsNullBlock(HI_PBYTE pBuffer);


//*****************************************************************************
//*****************************************************************************
#endif //defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

HI_END_HEADER
#endif //_MRS_SRV_ARCHIVES_N_H_
