
#ifndef __APP_NDM_TOOLS_H__
#define __APP_NDM_TOOLS_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_stdlib.h>

typedef struct
{
    HI_U8 ucTimerId; // Timer ID
    HI_U8 reserved[3];
} MRS_MSG_MR_TIMER_STRU;

#define EN_APP_QUEUE_NDM_SUB 1


#define mrsToolsMalloc(sz)              HI_MDM_Malloc(HI_MOD_ID_APP_COMMON,sz)
#define mrsToolsFree(sz)                do{if(sz)HI_MDM_Free(HI_MOD_ID_APP_COMMON,(HI_PVOID)sz);sz=0;}while(0)

#define MRS_TOOLS_ARRAY_COUNT(arr)  (HI_ARRAY_COUNT(arr))
#define MRS_TOOLS_ALEN(x)           (HI_ARRAY_COUNT(x))
#define MRS_NOT_USED(p)             ((p) = (p))

#define MRS_CHAR_ISUPPER(c)         (((c) >= 'A') && ((c) <= 'Z'))  // ¡ä¨®D¡ä¡Á???
#define MRS_CHAR_ISLOWER(c)         (((c) >= 'a') && ((c) <= 'z'))  // D?D¡ä¡Á???
#define MRS_CHAR_ISDIGIT(c)         (((c) >= '0') && ((c) <= '9'))  // ¨ºy¡Á?


#endif
