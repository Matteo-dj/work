/*
 * for time_cnt driver .
 *
 * History: 
 *      2017-03-06 create by z00193780
 */ 
 
#ifndef __TIME_CNT_H__
#define __TIME_CNT_H__

#include <hi_types.h>
#include <hi_mdm_types.h>

#define HI_TIME_CNT_REG_BASE     (0x101EA000)
#define TIME_S                   (0x0)
#define TIME_MS                  (0x4)
#define TIME_US                  (0x8)
#define TIME_CNT_CTRL            (0xC)
#define TIME_RD_EN               (1 << 1)
#define TIME_CNT_EN              (1 << 0)
#define TIME_RD_EN_WAIT_MAX      (10000)

#define HI_TIME_CNT_REG_WRITE(reg_off,value)\
    HI_REG_WRITE32((HI_TIME_CNT_REG_BASE +(reg_off)), value) 
    
#define HI_TIME_CNT_REG_READ(reg_off,value)\
    HI_REG_READ32((HI_TIME_CNT_REG_BASE +(reg_off)),value) 
#endif

