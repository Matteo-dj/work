#ifndef __I2C_H_
#define __I2C_H_

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */
#include <hi_driver_internal.h>
#ifndef __VX_PORTING_I2C__
#include <hi_platform.h>
#endif /* __VX_PORTING_I2C__ */
#define I2C1_BASE        (0x101E7000)
#define I2C2_BASE        (0x101EA800)

#define I2C_CLK              (g_cfg_apb_clock)
#define I2C_RATE_DEFAULT     (100000)
//#define I2C_RATE_DEFAULT     (400000)
#define I2C_MAP_SIZE         (0x2000)
#define I2C_SPACE_SIZE       (0x1000)

#define I2C_INIT_VALUE       0x1ff
#define READ_OPERATION       (1<<0)

#define I2C_CTRL             (0x000)
#define I2C_COM              (0x004)
#define I2C_ICR              (0x008)
#define I2C_SR               (0x00C)
#define I2C_SCL_H            (0x010)
#define I2C_SCL_L            (0x014)
#define I2C_TXR              (0x018)
#define I2C_RXR              (0x01C)

/* I2C_CTRL */
#define I2C_IP_ENABLE        (1<<8)
#define I2C_UNMASK_ALL       (1<<7)
#define I2C_UNMASK_START     (1<<6)
#define I2C_UNMASK_END       (1<<5)
#define I2C_UNMASK_SEND      (1<<4)
#define I2C_UNMASK_RECEIVE   (1<<3)
#define I2C_UNMASK_ACK       (1<<2)
#define I2C_UNMASK_ARBITRATE (1<<1)
#define I2C_UNMASK_OVER      (1<<0)

/* I2C_COM */
#define I2C_SEND_ACK         (~(1<<4))
#define I2C_START            (1<<3)
#define I2C_READ             (1<<2)
#define I2C_WRITE            (1<<1)
//#define I2C_STOP             (1<<0)

/* I2C_ICR */
#define I2C_CLEAR_START      (1<<6)
#define I2C_CLEAR_END        (1<<5)
#define I2C_CLEAR_SEND       (1<<4)
#define I2C_CLEAR_RECEIVE    (1<<3)
#define I2C_CLEAR_ACK        (1<<2)
#define I2C_CLEAR_ARBITRATE  (1<<1)
#define I2C_CLEAR_OVER       (1<<0)
#define I2C_CLEAR_ALL        (I2C_CLEAR_START | I2C_CLEAR_END | \
                              I2C_CLEAR_SEND | I2C_CLEAR_RECEIVE | \
                              I2C_CLEAR_ACK | I2C_CLEAR_ARBITRATE | \
                              I2C_CLEAR_OVER)

/* I2C_SR */
#define I2C_BUSY             (1<<7)
#define I2C_START_INTR       (1<<6)
#define I2C_END_INTR         (1<<5)
#define I2C_SEND_INTR        (1<<4)
#define I2C_RECEIVE_INTR     (1<<3)
#define I2C_ACK_INTR         (1<<2)
#define I2C_ARBITRATE_INTR   (1<<1)
#define I2C_OVER_INTR        (1<<0)

#define I2C_START_END        (I2C_START_INTR | I2C_OVER_INTR)
#define I2C_STOP_END         (I2C_END_INTR | I2C_OVER_INTR)
/* BEGIN: Modified by zhangna, 2012/5/8   PN:DTS2012050405939 */
#define TIMEOUT              3000
/* END:   Modified by zhangna, 2012/5/8 */
#define RECV_TIMEOUT         0x1000

#define I2C_SC_SCPERCTRL0    (0x1c)
#define I2C_SC_SCPERCTRL2    (0x34)

#ifdef PRODUCT_CFG_EQUIP_TEST_MODE    
#define TERMINAL_DISPLAY
#endif
#ifndef TERMINAL_DISPLAY
 #define i2c_error(_fmt, arg...)
#else
 #define i2c_error(_fmt, arg...) \
    HI_UART_Printf("%d: " _fmt, __LINE__, ## arg)
#endif

#ifndef DRV_DEBUG
 #define i2c_debug(_fmt, arg...)
#else
 #define i2c_debug(_fmt, arg...) \
    HI_UART_Printf("%d: " _fmt, __LINE__, ## arg)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif

