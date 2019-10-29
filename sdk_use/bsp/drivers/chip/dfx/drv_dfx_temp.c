#include <hi_types.h>
#include "drv_dfx_cmn_inc.h"
#include <mdm_inf.h>

#include "drv_dfx_com.h"
//#define DRV_DFX_SYS_VAL_DECLARE
#include <drv_dfx_sys_val.h>
#include <hi_platform.h>
#include <hi_drv_com.h>
/* BEGIN: Added by wuxianfeng 190206, 2012/8/10   PN:DTS2012081001116*/
/* BEGIN: Added by b00208046, 2013/6/24   PN:DTS2013060804531 */

#define BVER_MAX_SIZE    12     //XXX.XXX.XXX 预留12个字节空间
#define PVER_MAX_SIZE   (sizeof(PRODUCT_CFG_VERSION_STR)+6)
/* END:   Added by b00208046, 2013/6/24 */

HI_PRV HI_VOID BOOT_VERSION_V100(HI_VOID)
{
#ifndef HAVE_PCLINT_CHECK
    HI_U32 size;
    HI_U32 addr = PRODUCT_CFG_BOOT_VERSION_ADDR;
    /* BEGIN: Modified by b00208046, 2013/6/24   PN:DTS2013060804531 */
    // 前提条件 size"V100R001C00B020SPC001" > size"V100R001C00B020" 读取时只是大小依赖于PRODUCT_CFG_VERSION_STR
    HI_CHAR pver[PVER_MAX_SIZE] = {0};
    HI_CHAR bver[BVER_MAX_SIZE] = {0};//// 000.000.000\n
    /* END:   Modified by b00208046, 2013/6/24 */
    HI_CHAR btime[sizeof(PRODUCT_CFG_BUILD_DATE)+sizeof(PRODUCT_CFG_BUILD_TIME)] = {0};
    HI_U32 len;

    (HI_VOID)HI_MDM_FLASH_Read(addr,
        PVER_MAX_SIZE, &size, (HI_U8 *)pver);
    /* BEGIN: Modified by b00208046, 2013/6/24   PN:DTS2013060804531 */
    len = strlen(pver);
    addr += len+1;
    (hi_void)memset_s((HI_VOID *)(pver+len), PVER_MAX_SIZE-len, 0x00, PVER_MAX_SIZE-len);

    (HI_VOID)HI_MDM_FLASH_Read(addr,
        BVER_MAX_SIZE, &size, (HI_U8 *)bver);
    len = strlen(bver);
    addr += len+1;
    (hi_void)memset_s((HI_VOID *)(bver+len),BVER_MAX_SIZE-len, 0x00,BVER_MAX_SIZE-len);
    /* END:   Modified by b00208046, 2013/6/24 */
    (HI_VOID)HI_MDM_FLASH_Read(addr, sizeof(PRODUCT_CFG_BUILD_DATE)+
        sizeof(PRODUCT_CFG_BUILD_TIME), &size, (HI_U8 *)btime);

    HI_SYS_CONFIG_BOOT_VER(pver, bver, btime);
#endif/*HAVE_PCLINT_CHECK*/
}

HI_PRV HI_VOID BOOT_VERSION_200(HI_VOID)
{
    HI_U32 ulBootVer;
    HI_U8 aucBootVer[3]; 
    HI_CHAR aucStrBootVer[BVER_MAX_SIZE] = {0};//// 000.000.000\n
    
    HI_REG_READ32(GENERAL_REG_BOOT_VER,ulBootVer);
    aucBootVer[0]=(HI_U8)((ulBootVer>>16)&0xFF);
    aucBootVer[1]=(HI_U8)((ulBootVer>>8)&0xFF);
    aucBootVer[2]=(HI_U8)((ulBootVer)&0xFF);
    
    (hi_void)snprintf_s(aucStrBootVer,BVER_MAX_SIZE,BVER_MAX_SIZE-1,"%u.%u.%u",aucBootVer[0],aucBootVer[1],aucBootVer[2]);
    HI_SYS_CONFIG_BOOT_VER(HI_NULL,aucStrBootVer,HI_NULL);
    
}
void BOOT_VERSION(void)
{
    if (IsV200Boot())
    {
        BOOT_VERSION_200();
    }
    else
    {
        BOOT_VERSION_V100();
    }
    return;

}
/* END:   Added by wuxianfeng 190206, 2012/8/10 */

HI_U32 DRV_Init(HI_VOID)
{
#if 1
	#if (!defined(HAVE_PCLINT_CHECK) )
	DRV_VERSION();
	OS_VERSION();
	BOOT_VERSION();
	#endif

	DRV_INIT_STAT_TBL();
	return HI_DRV_DFX_Init();

#endif
}
// BEGIN: Added by cwf/chh, 2013/2/23 仅仅为了编译通过
#define TIMER_NUMBER 40
typedef struct {
    HI_CHAR  tm_name[16];  /* Timer name            */
    HI_BOOL  tm_enabled;            /* Timer enabled flag    */
    HI_CHAR  reserved[3];
    HI_U32   tm_expirations;        /* Number of expirations */
    HI_U32   tm_remain_time;       /* Remain time*/
}NU_TIMERLISTINFO_S;

typedef struct {
    HI_U32  timerTotalNum;
    HI_U32  timerListNum;
    NU_TIMERLISTINFO_S g_stDfxTimerinfo[TIMER_NUMBER];
} NU_TIMERINFO_DEBUG_S;

NU_TIMERINFO_DEBUG_S g_stOsTimerDfxInfo;

HI_U32 DRV_GetOSTimerListSize(HI_VOID)
{
    return sizeof(NU_TIMERINFO_DEBUG_S);
}

HI_PVOID DRV_GetOSTimerList(HI_VOID)
{
    (hi_void)memset_s((HI_VOID *)&g_stOsTimerDfxInfo, sizeof(NU_TIMERINFO_DEBUG_S), 0, sizeof(NU_TIMERINFO_DEBUG_S));
    g_stOsTimerDfxInfo.timerTotalNum = 0xdeaddead;

    return (HI_PVOID)(&g_stOsTimerDfxInfo);
}

