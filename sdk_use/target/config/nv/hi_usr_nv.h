//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : hi_usr_nv.h
//  �� �� ��   : V1.0
//  ��    ��   : shenhankun/00130424
//  ��������   : 2011-10-26
//  ��������   : �������û��Զ���NV��, �������Ϊ64�ֽ�, ID��Χ [0x8000, 0x9000)
//
//  �����б�   :  NA
//  �޸���ʷ   :
//  1.��    �� : 2011-10-26
//    ��    �� : shenhankun/00130424
//    �޸����� : �����ļ�
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
// NV �� ID����, ID��Χ [0x8000, 0x9000)
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
    HI_CHAR aucUserVersion[HI_USER_VER_LENGTH]; // �û��汾�ţ�ASCII��ʽ
    HI_U16  usSwVer;        // ����汾�ţ�BCD��ʽ
    HI_CHAR szChipCode[3];  // оƬ���룬ASCII��ʽ
    HI_U8   aucDate[3];     // �汾���ڣ�BIN��ʽ��YYMMDD
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


