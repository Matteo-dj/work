
#ifndef __ICC_PROTOCAL_UPG_H__
#define __ICC_PROTOCAL_UPG_H__

#include <hi_types.h>
#include <hi_mdm_types.h>
#define HI_3518_UPG_MAGIC 0x33353138

typedef struct
{
    HI_U32 app;
    HI_CHAR kernel[64];
    HI_U8 pad[16];
}HI_3518_UPG_VER_S;


typedef struct
{
    HI_U32 magic;
    HI_U32 ver;
    HI_U32 crc;
    HI_U32 dataSize;
    HI_U8 data[0];
}HI_3518_UPG_HEAD_S;//�ļ�����ͷ|�����ļ�����REQ

typedef struct
{
    HI_U32 ret;
}HI_3518_UPG_IND_S;//�������IND

typedef struct
{
    HI_U32 reserver;
}HI_3518_GET_VER_CMD_S;

typedef struct
{
    HI_3518_UPG_VER_S ver;
}HI_3518_GET_VER_IND_S;
#endif//__ICC_PROTOCAL_UPG_H__

