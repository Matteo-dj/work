//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_baudrate_manage.h
//  版 本 号   : V1.0 
//  作    者   : liuxipeng/KF54842
//  生成日期   : 2014-06-14
//  功能描述   : 波特率管理
//               函数声明及宏定义
//               
//  函数列表   : 
//  修改历史   : 
//  1.日    期 : 2014-06-14
//    作    者 : liuxipeng/KF54842
//    修改内容 : 创建文件 
//
//*****************************************************************************

#ifndef __MRS_BAUDRATE_MANAGE_H__
#define __MRS_BAUDRATE_MANAGE_H__
HI_START_HEADER

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

#define MRS_SRV_RATE_MAX                (5)      // 每一个波特率配置支持的最大波特率个数

#define MRS_SRV_BAUD_RATE_300           (300)
#define MRS_SRV_BAUD_RATE_600           (600)
#define MRS_SRV_BAUD_RATE_1200          (1200)
#define MRS_SRV_BAUD_RATE_2400          (2400)
#define MRS_SRV_BAUD_RATE_4800          (4800)
#define MRS_SRV_BAUD_RATE_7200          (7200)
#define MRS_SRV_BAUD_RATE_19200         (19200)
#define MRS_SRV_BAUD_RATE_38400         (38400)

#define MRS_SRV_BAUD_RATE_DEFAULT       MRS_SRV_BAUD_RATE_2400
#define MRS_SRV_BAUD_RATE_IDX_DEFAULT   (3)

#define MRS_UART_BYTE_BIT                       (1 + 8 + 1 + 1) // 1起始位 + 8数据位 + 1校验位 + 1停止位
#define MRS_GET_UART_SEND_TIME(BaudRate, Len)   ((MRS_UART_BYTE_BIT * 1000 * (Len) + (BaudRate) - 1) / (BaudRate))                 

typedef enum
{
    MRS_SRV_ID_BAUDRATE_SM_97 = 0,      // 波特率配置ID-搜表97规约
    MRS_SRV_ID_BAUDRATE_SM_07,          // 波特率配置ID-搜表07规约
    MRS_SRV_ID_BAUDRATE_SM_698,         // 波特率配置ID-搜表698.45规约
    MRS_SRV_ID_BAUDRATE_RM_97,          // 波特率配置ID-读表97规约
    MRS_SRV_ID_BAUDRATE_RM_07,          // 波特率配置ID-读表07规约
    MRS_SRV_ID_BAUDRATE_RM_698,        // 波特率配置ID-读表698.45规约
    MRS_SRV_ID_BAUDRATE_RM_RSV1,        // 波特率配置ID-读表保留1
    MRS_SRV_ID_BAUDRATE_ADJUST_TIME,    // 波特率配置ID-校时
    MRS_SRV_ID_BAUDRATE_DETECT,         // 波特率配置ID-上电波特率探测
    MRS_SRV_ID_BAUDRATE_MAX             // 波特率配置ID-最大个数
} MRS_SRV_BAUDRATE_ID_E;

typedef struct
{
    HI_U16 usBaudRateList[MRS_SRV_RATE_MAX];
    HI_U8  ucValidNum;
    HI_U8  padding;
} MRS_SRV_BAUDRATE_CFG_S, *P_MRS_SRV_BAUDRATE_CFG_S;


// 根据NV配置初始化波特率配置表
HI_PUBLIC HI_U32 mrsSrvInitBaudRateCfgTable(HI_U16);

// 获取指定ID的波特率配置
// 返回空指针不可使用
// 调用者通过返回值自行判断内容是否合法
// 输出内容中若波特率个数为0则说明对应该ID的波特率配置无效
HI_PUBLIC P_MRS_SRV_BAUDRATE_CFG_S mrsSrvGetBaudRateCfg(MRS_SRV_BAUDRATE_ID_E);

// 波特率和索引号互转函数
// 成功返回HI_ERR_SUCCESS 失败返回HI_ERR_FAILURE
// 索引和波特率对应关系:
//  1: MRS_SRV_BAUD_RATE_600
//  2: MRS_SRV_BAUD_RATE_1200
//  3: MRS_SRV_BAUD_RATE_2400
//  4: MRS_SRV_BAUD_RATE_4800
//  5: MRS_SRV_BAUD_RATE_9600
//  6: MRS_SRV_BAUD_RATE_19200
//  7: MRS_SRV_BAUD_RATE_38400
HI_PUBLIC HI_U32 mrsSrvBaudRate2Index(HI_IN HI_U16 baudrate, HI_OUT HI_U8 *index);
HI_PUBLIC HI_U32 mrsSrvIndex2BaudRate(HI_IN HI_U8 index, HI_OUT HI_U16 *baudrate);

#endif

HI_END_HEADER
#endif


