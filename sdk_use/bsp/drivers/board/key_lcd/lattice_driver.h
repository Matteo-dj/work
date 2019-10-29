
#ifndef __LATTICE_DRIVER_H__
#define __LATTICE_DRIVER_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

HI_EXTERN HI_U32 g_lattice_event_handle;

#ifdef HI_LATTICE_DEBUG_SW
#define KEY_DBG_PRINT0(x)          HI_MDM_ReportPacket((HI_U32)HI_DCID_STR, (HI_U8)0, (HI_PBYTE)x, (HI_U16)(sizeof(x)-1), HI_TRUE)
#define KEY_DBG_PRINT1(x,v1)       do{ HI_CHAR buf[128] = {0,0, }; HI_MDM_Sprintf(buf, x, v1); HI_MDM_ReportPacket((HI_U32)HI_DCID_STR, (HI_U8)0, (HI_PBYTE)buf, (HI_U16)128, HI_TRUE);}while(0)
#define KEY_DBG_PRINT2(x,v1,v2)    do{ HI_CHAR buf[128] = {0,0, }; HI_MDM_Sprintf(buf, x, v1,v2); HI_MDM_ReportPacket((HI_U32)HI_DCID_STR, (HI_U8)0, (HI_PBYTE)buf, (HI_U16)128, HI_TRUE);}while(0)
#define KEY_DBG_PRINT3(x,v1,v2,v3) do{ HI_CHAR buf[128] = {0,0, }; HI_MDM_Sprintf(buf, x, v1,v2,v3); HI_MDM_ReportPacket((HI_U32)HI_DCID_STR, (HI_U8)0, (HI_PBYTE)buf, (HI_U16)128, HI_TRUE);}while(0)
#define KEY_DBG_PRINT4(x,v1,v2,v3,v4) do{ HI_CHAR buf[128] = {0,0, }; HI_MDM_Sprintf(buf, x, v1,v2,v3,v4); HI_MDM_ReportPacket((HI_U32)HI_DCID_STR, (HI_U8)0, (HI_PBYTE)buf, (HI_U16)128, HI_TRUE);}while(0)
#else
#define KEY_DBG_PRINT0(x) //edb_put_str_p0(x)
#define KEY_DBG_PRINT1(x,v1) //edb_put_str_p1(x,v1)
#define KEY_DBG_PRINT2(x,v1,v2) //edb_put_str_p2(x,v1,v2)
#define KEY_DBG_PRINT3(x,v1,v2,v3) //edb_put_str_p3(x,v1,v2,v3)
#define KEY_DBG_PRINT4(x,v1,v2,v3,v4) //edb_put_str_p4(x,v1,v2,v3,v4)
#endif

//define of events
#define I2C_EVT_TIMER_KEY0      (1<<0)
#define I2C_EVT_TIMER_KEY1      (1<<1)
#define I2C_EVT_TIMER_KEY2      (1<<2)
#define I2C_EVT_TIMER_KEY3      (1<<3)
#define I2C_EVT_TIMER_KEY4      (1<<4)
#define I2C_EVT_TIMER_KEY5      (1<<5)
#define I2C_EVT_TIMER_KEY6      (1<<6)
#define I2C_EVT_TIMER_KEY7      (1<<7)
#define I2C_EVT_TIMER_KEY8      (1<<8)
#define I2C_EVT_TIMER_KEY9      (1<<9)
#define I2C_EVT_TIMER_KEY10     (1<<10)
#define I2C_EVT_TIMER_KEY11     (1<<11)
#define I2C_EVT_LATTICE_INT     (1<<HI_DRV_KEY_NUM_MAX)
#define I2C_EVT_IOEXP_INT       (1<<13)
#define I2C_EVT_DISP_FLUSH_INT  (1<<14)
#define I2C_EVT_TIMER_KEY       (1<<15)
#define I2C_EVT_TIMER_KEYGROUP  ((1<<HI_DRV_KEY_NUM_MAX)-1)//(I2C_EVT_TIMER_KEY0||I2C_EVT_TIMER_KEY1||I2C_EVT_TIMER_KEY2||..||I2C_EVT_TIMER_KEY11)

//#define HI_LATTICE_DEBUG_HW
#ifdef HI_LATTICE_DEBUG_HW
#define HT16K33_I2C_ADDR1     (0xE0)//1110XXX 按键和点阵显示
#define HT16K33_I2C_ADDR2     (0xEE)//1110XXX 点阵显示2个
#else
//真实底板
#define HT16K33_I2C_ADDR1     (0xE4)//1110010 按键和点阵显示
#define HT16K33_I2C_ADDR2     (0xE6)//1110011 点阵显示2个
#endif
//#define HI_LATTICE_DEBUG_SW

//寄存器
#define REG_DISPLAY_DATA_ADDR   (0x00)//0x1-0xF
#define REG_SYSTEM_SETUP_ADDR   (0x20)
#define REG_SYSTEM_SETUP_TURNON     (1<<0)

#define REG_ROW_INT_SET_ADDR    (0xA0)
#define REG_ROW_INT_SET_INT         (1<<0)
#define REG_ROW_INT_SET_INT_HIGH    (1<<1)

#define REG_DISPLAY_SET_ADDR    (0x80)
#define REG_DISPLAY_SET_STATUSON    (1<<0)
#define REG_DISPLAY_SET_BLINK_FAST  (1<<1)
#define REG_DISPLAY_SET_BLINK_NORM  (2<<1)
#define REG_DISPLAY_SET_BLINK_SLOW  (3<<1)

#define REG_PULSE_WIDTH_ADDR    (0xE0)//亮度调节
#define REG_PULSE_WIDTH_01DUTY16    (0<<0)//1/16duty
#define REG_PULSE_WIDTH_05DUTY16    (4<<0)//5/16duty
#define REG_PULSE_WIDTH_14DUTY16    (13<<0)//14/16duty
#define REG_PULSE_WIDTH_16DUTY16    (15<<0)//16/16duty

#define REG_INTFLAG_ADDR        (0x60)
#define REG_KAYDATA_ADDR        (0x40)  //Key寄存器0x40-0x45，KS0对应0x40/0x41,KS1对应0x42/0x43,KS0对应0x44/0x45
#define REG_KAYDATA_NUM         (6)     //0x40-0x45共6个
#define REG_KAYSOURCE_NUM       (3)
#define REG_TESTMODE_ADDR       (0xD9)

HI_U32 i2c_write_bytes(HI_U8 clientAddress,HI_U8 *data,HI_U32 len);
HI_U32 i2c_read_bytes(HI_U8 clientAddress,HI_U8 reg,HI_U8 *data,HI_U32 len);
HI_U32 HI_LATTICE_Init(HI_VOID);
#endif

