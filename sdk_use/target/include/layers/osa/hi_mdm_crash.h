/**@defgroup hct_crash  死机存储
 * @ingroup crash
 */

#ifndef __HI_MDM_CRASH_H__
#define __HI_MDM_CRASH_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
HI_START_HEADER

#define HI_MDM_RST_DATA_MAX_NUM             (60)
#define HI_MDM_BUILD_VER_PRODUCT_LEN_MAX    (52)
#define HI_MDM_BUILD_VER_BOOT_LEN_MAX       (HI_MDM_BUILD_VER_PRODUCT_LEN_MAX)
#define HI_MDM_ESP_DATA_MAX_NUM             (64)

/**
 * @ingroup hct_crash
 * 存储的死机信息
 */
typedef struct
{
    HI_U32  sig_s;
    HI_U32  ver;
    HI_U32  crash_num;
    HI_U32  reboot_num;
    HI_U32  sdm_num;
    HI_U32  rst_num;
    HI_U32  run_tsec;
    HI_S32  eid;
    HI_U32  rid[2];
    HI_S32  fid[2];
    HI_S32  evn;
    HI_U32  esp;
    HI_U32  sp;
    HI_U32  epc;
    HI_S32  vn;
    HI_S32  type;
    HI_U32  oerr[3];
    HI_U32  tid;
    HI_U32  tcsc;
    HI_U32  tcsp;
    HI_CHAR tname[16];
    HI_U32  tstatus;
    HI_U32  crash_tsec;
    HI_U32  rst_tsec;
    HI_U32  tstksize;
    HI_U32  tstk[2];
    HI_U32  dbg_data[14];
    HI_U8   usr_data0[64];
    HI_U8   rst_data[HI_MDM_RST_DATA_MAX_NUM];
    HI_U8   usr_data1[192];
    HI_U32  err_ln;
    HI_U32  val[4];
    HI_U32  ttfa[16];
    HI_U32  r[15];
    HI_U32  cpsr;
    HI_CHAR prod_ver[HI_MDM_BUILD_VER_PRODUCT_LEN_MAX];
    HI_CHAR cfg_ver[HI_MDM_BUILD_VER_PRODUCT_LEN_MAX];
    HI_CHAR boot_ver[HI_MDM_BUILD_VER_BOOT_LEN_MAX];
    HI_U32  esp_data[HI_MDM_ESP_DATA_MAX_NUM];
    HI_CHAR buildtype[2];
    HI_CHAR ostype[2];
    HI_CHAR wd[2];
    HI_U8   reserved[2];
    HI_U32  wdctrl;
    HI_U32  init_phase_no;
    HI_U32  check_sum;
    HI_U32  bn;
    HI_U32  size;
    HI_U32  sig_e;
} HI_MDM_SYS_CRASH_INFO_S;

/**
* @ingroup  hct_crash
* @brief  获取死机信息。
*
* @par 描述: 
* 获取死机信息。
* @attention 无。
* 
* @param pstCrashInfo [OUT] 存储的死机信息，请参见<<Hi3911V2R1 死机重启问题定位指南.doc>>。
* 
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_crash.h: 该接口声明所在的头文件。
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetSysCrashInfo(HI_MDM_SYS_CRASH_INFO_S *pstCrashInfo);

HI_END_HEADER
#endif

