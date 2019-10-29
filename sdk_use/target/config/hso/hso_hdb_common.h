#define  SAL_ALIGNED4  

#include "hi_config.h"

typedef struct
{

} NULL_STRU;

typedef struct
{

}HSO_NULL_STRU;

// Don't need to show ACK 
typedef struct
{

}HSO_NACK_NULL_STRU;

typedef struct
{
  char str[1];
} HSO_STR_TYPE_STRU;

typedef struct
{
    long data[2];
}HSO_S32ARRAY_2_STRU;

typedef struct
{
    long data[3];
}HSO_S32ARRAY_3_STRU;

typedef struct
{
    long data[4];
}HSO_S32ARRAY_4_STRU;

typedef struct
{
    long data[5];
}HSO_S32ARRAY_5_STRU;

typedef struct
{
    long data[6];
}HSO_S32ARRAY_6_STRU;

typedef struct
{
    HI_U32 data[2];
}HSO_U32ARRAY_2_STRU;

typedef struct
{
    unsigned long data[3];
}HSO_U32ARRAY_3_STRU;

typedef struct
{
    HI_U32 data[4];
}HSO_U32ARRAY_4_STRU;

typedef struct
{
  char str[128];
} HSO_FILE_TYPE_S;

typedef struct
{
  char str[1];
} HSO_DUMP_DATA_STRU;

typedef struct
{
  char str[1];
} HSO_DUMP_HEX_STRU;

typedef struct
{

}HSO_FILE_TYPE_STRU;

typedef struct
{
  HI_U32 data;
}HSO_U32_TYPE_STRU;


typedef struct
{
  HI_U32 opt;
  HI_U32 val;
}HSO_U32_2_TYPE_STRU;

typedef struct  
{
    HI_U8 data[19];
}HSO_AU8_19_STRU;


typedef struct  
{
    HI_U32 data[20];
}HSO_ARRAY20_STRU;

typedef struct
{
    HI_U32 data[23];
}HSO_ARRAY23_STRU;

typedef struct
{
    HI_U8 mac[6];
    HI_U8 data[90];
}HSO_MD90B_STRU;

typedef struct  
{
    HI_U8 mac[8];
}HSO_PLC_SDM_MAC_STRU;

typedef struct  
{
    HI_U32 id;
    HI_U32 size;
    HI_U8 data[128];
}HSO_NV_REQ_STRU;

typedef struct 
{
    HI_U32 ulRc;
    HI_U32 ulNVId;
    HI_U32 ulDataSize;
    HI_U8  aucData[128];
} HSO_NV_ACK_STRU;

typedef struct
{
    HI_U32 ulAddr;
    HI_U32 ulTotalSize;
    HI_U32 ulSize;
    HI_U32 ulSn;
} HSO_DUMP_BIN_REQ_STRU;
