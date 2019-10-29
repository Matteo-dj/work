//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : hi_usr_nv.h
//  版 本 号   : V1.0
//  作    者   : shenhankun/00130424
//  生成日期   : 2011-10-26
//  功能描述   : 第三方用户自定义NV项, 最大限制为64字节, ID范围 [0x8000, 0x9000)
//
//  函数列表   :  NA
//  修改历史   :
//  1.日    期 : 2011-10-26
//    作    者 : shenhankun/00130424
//    修改内容 : 创建文件
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   :
// SUBSYSTEM :
// MODULE    :
// OWNER     :
//*****************************************************************************

#ifndef __HI_USR_NV_H__
#define __HI_USR_NV_H__
//*****************************************************************************


//*****************************************************************************
// NV 项 ID定义, ID范围 [0x8000, 0x9000)
//*****************************************************************************

#define  ID_NV_USR_0x8000                0x8000
#define  ID_NV_USR_0x8001                0x8001
#define  ID_NV_USR_0x8002                0x8002
#define  ID_NV_USR_0x8003                0x8003
#define  ID_NV_USR_0x8004                0x8004
#define  ID_NV_USR_0x8005                0x8005
#define  ID_NV_USR_0x8006                0x8006
#define  ID_NV_USR_0x8007                0x8007
#define  ID_NV_USR_0x8008                0x8008
#define  ID_NV_MRS_USER_VER              0x8009
//*****************************************************************************

typedef struct
{
    HI_CHAR aucUserVersion[HI_USER_VER_LENGTH]; // 用户版本号，ASCII格式
    HI_U16  usSwVer;        // 软件版本号，BCD格式
    HI_CHAR szChipCode[3];  // 芯片代码，ASCII格式
    HI_U8   aucDate[3];     // 版本日期，BIN格式，YYMMDD
    HI_U8   reserved[24];
}NV_MRS_USER_VER_S;

typedef struct
{
    HI_U8 aucData[64];
}NV_USR_0x8000_S;

typedef struct
{
    HI_U8 aucData[64];
}NV_USR_0x8001_S;

typedef struct
{
    HI_U8 aucData[64];
}NV_USR_0x8002_S;

typedef struct
{
    HI_U8 aucData[64];
}NV_USR_0x8003_S;

typedef struct
{
    HI_U8 aucData[64];
}NV_USR_0x8004_S;

typedef struct
{
    HI_U8 aucData[64];
}NV_USR_0x8005_S;

typedef struct
{
    HI_U8 aucData[64];
}NV_USR_0x8006_S;

typedef struct  
{
    HI_U8 aucData[64]; 
}NV_USR_0x8007_S;

typedef struct  
{
    HI_U8 aucData[64]; 
}NV_USR_0x8008_S;
//*****************************************************************************


//*****************************************************************************
#endif //__HI_USR_NV_H__


