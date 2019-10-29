//*****************************************************************************
//
//                  版权所有 (C), 1998-2010, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : diag_cpab.h
//  版 本 号   : V1.0 
//  作    者   : 陈文峰/00149739
//  生成日期   : 2010-04-21
//  功能描述   : DIAG和CPAB之间所使用的头文件
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2010-04-21
//    作    者 : 陈文峰/00149739
//    修改内容 : 创建文件 
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   : 
// SUBSYSTEM :
// MODULE    :  
// OWNER     :  
//*****************************************************************************




#ifndef __DIAG_CPAB_H__
#define __DIAG_CPAB_H__

//*****************************************************************************
// 
// DIAG 和 CPAB的命令包的接口定义说明. 
// 不包含DIAG命令定义接口，DIAG命令在
// 
// CPAB和DIAG之间的数据结构定义如下:
// 请求(REG)的数据结构DIAG_HEAD_REQ_STRU
// 回复(CNF)的数据结构为DIAG_HEAD_CNF_STRU.
// 上报(IND)的数据结构为DIAG_HEAD_IND_STRU.
//*****************************************************************************

//
// HiStudio对DIAG的请求(REQ)的数据结构定义.
//
typedef struct
{
    HI_U32 ulReqIdx;         // 请求索引号，用于甄别是否丢帧.
    HI_U16 usSn;             // 命令序列号
    HI_U16 usCtrl;           // 命令控制字，用于控制命令的优先级
    HI_U32 ulCmdID;          // 命令字
    HI_U32 ulParamSize;      // 命令参数的长度
    HI_U8  ucParam[0];       // 命令参数的数据 
} MSP_DIAG_HEAD_REQ_STRU;


//
// DIAG对HiStudio的回复(CNF)或是上报(IND)的数据结构定义.
//
typedef struct
{
    HI_U32 ulCmdID;      // 命令字
    HI_U32 ulReqIdx;     // 请求索引号，用于甄别是否丢帧.
    HI_U16 usSn;         // 命令序列号
    HI_U16 usCtrl;       // 命令控制字，用于标识命令是否传输完毕
    HI_U32 ulTimeStamp;  // UE的时间戳
    HI_U32 ulParamSize;  // 命令参数的长度
    HI_U8  ucParam[0];   // 命令参数的数据
} MSP_DIAG_HEAD_CNF_STRU;

typedef struct
{
    HI_U32 ulCmdID;      // 命令字
    HI_U32 ulNo;         // IND标号
    HI_U16 ulNo4GCnt;    // 4G的倍数
    HI_U16 usCtrl;       // 命令控制字，用于标识命令是否传输完毕
    HI_U32 ulTimeStamp;  // UE的时间戳
    HI_U32 ulParamSize;  // 命令参数的长度
    HI_U8  ucParam[0];   // 命令参数的数据
}  MSP_DIAG_HEAD_IND_STRU;



#endif //__DIAG_CPAB_H__


