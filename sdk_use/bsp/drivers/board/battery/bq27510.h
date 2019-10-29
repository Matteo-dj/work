

#ifndef __BQ27510_H__
#define __BQ27510_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <dfx_battery.h>//BQ27510_STATUS_S

#define BQ_UPG_SLEEP_AFTER_RESET 3000//ms BQ芯片重启后一段时间内读取到的BQ芯片状态是不准确的。需要Sleep一会再读取BQ状态

//BQ27510 寄存器地址
#define REG_TEMP_LOW            0x06
#define REG_VOLT_LOW            0x08
#define REG_FLAG_LOW            0x0a
#define REG_NAC_LOW             0x0C
#define REG_FAC_LOW             0x0E
#define REG_RM_LOW              0x10
#define REG_FFC_LOW             0x12
#define REG_CURRENT_LOW         0x14//s
#define REG_TTE_LOW             0x16//u
#define REG_SI_LOW              0x18//
#define REG_STTE_LOW            0x1a//
#define REG_SOC_LOW             0x20
#define REG_II_LOW              0x22//s

#define BQ27510_NORMAL_ADDR     (0xAA)
#define BQ27510_ROM_MOD_ADDR    (0x16)
#define BQ27510_GOOD_VERSION_A  0x0281

HI_EXTERN HI_U32 g_battert_sem;

#define BQ_I2C_MUTEX_LOCK()           HI_MDM_SEM_Wait(g_battert_sem,HI_SYS_WAIT_FOREVER)
#define BQ_I2C_MUTEX_UNLOCK()         HI_MDM_SEM_Signal(g_battert_sem)

/************上面bq27510和bqupg27510共享*************/


HI_EXTERN BQ27510_STATUS_S g_bq27510_status;

HI_U32 bq27510_write_bytes(HI_U8 clintAddress,HI_U8 *param,HI_U32 len);
HI_U32 bq27510_read_bytes(HI_U8 clintAddress,HI_U8 reg,HI_U8 *param,HI_U32 len);

HI_U32 bq27510_readVolt(HI_U16 *pVolt);
HI_U32 bq27510_readTemperature(HI_U16 *pTemp);
HI_U32 bq27510_readCurrent(HI_S16 *pCurrent);
HI_U32 bq27510_readBatPercent(HI_U8 *batpercent);
HI_U32 bq27510_readFirmWareMainVersion(HI_U16 *mainVer);
HI_U32 bq27510_readFirmWareSubVersion(HI_U8 *firmWareB);
HI_U32 bq27510_resetChip(HI_VOID);
HI_U32 bq27510_init(HI_VOID);



HI_U32 bq27510_CheckUpgFile(HI_U32 ulOption, HI_U32 ulId, HI_UCHAR *pFile, HI_U32 ulFileSize);
HI_U32 bq27510_Upgrade(HI_U32 ulOption, HI_U32 ulId, HI_PVOID pFile, HI_U32 ulFileSize);


#endif

