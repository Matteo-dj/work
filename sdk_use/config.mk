################################################################################
# Main configure for compile 
#
# History
# Author                			Date          Version 
# chenghuanhuan/00132816   2016-05-19    V1.0  
################################################################################

#包含界面配置参数文件
include $(LITEOSTOPDIR)/hb.cfg
CUR_DATE = \"$(shell date "+%Y-%m-%d")\"
CUR_TIME = \"$(shell date "+%H:%M:%S")\"
CROSS_COMPILE = arm-hisiv500-linux-uclibcgnueabi-
ifeq (${IF_FORTIFY_BUILD}, 1)
CC = sourceanalyzer -b ${FORTIFY_BUILDID} $(CROSS_COMPILE)gcc
else
CC = $(CROSS_COMPILE)gcc
endif
AS = $(CROSS_COMPILE)as
AR = $(CROSS_COMPILE)ar
LD = $(CROSS_COMPILE)ld
GPP = $(CROSS_COMPILE)g++
OBJCOPY= $(CROSS_COMPILE)objcopy
OBJDUMP= $(CROSS_COMPILE)objdump

################################################################################################
#										 target
#										   |
#										   |
#										delivery
#										   |
#										   |
#					|----------------|----------------|----------------|	
#			 himdl_release		himdl_debug（e.g.）	rmt_release		  ...
#									 |
#									 |
#	|---------------|----------------|----------------|----------------|
#	ALL				CCO				STA(e.g.)         NDM			HUPG&HBIN&log
#									 |
#									 |
#					|----------------|----------------|----------------|					  
#					bin				build			lib				 log
################################################################################################


#区分产品形态目录
ifeq ($(HB_DEBUG_COMPILE_ENABLE), YES)
MAIN_OUT_DIR_NAME = $(HB_PROJECT_NAME_LIST)_debug
else
MAIN_OUT_DIR_NAME = $(HB_PROJECT_NAME_LIST)_release
endif
MAIN_OUT =	$(LITEOSTOPDIR)/target/delivery/$(MAIN_OUT_DIR_NAME)
OUT = $(MAIN_OUT)/$(HB_PRODUCT_NAME_LIST)
BUILD = $(OUT)/build
LIBS = $(OUT)/lib
TARGET = $(OUT)/bin

#检测APP类型
comma:= ,
ifeq ($(findstring 0MRS0,$(subst $(comma),0,$(HB_APP_COMPILE_LIST))),0MRS0)
APP_LIST += mrs
endif

ifeq ($(findstring 0RMT0,$(subst $(comma),0,$(HB_APP_COMPILE_LIST))),0RMT0)
APP_LIST += rmt
endif

ifeq ($(findstring 0CSGMRS0,$(subst $(comma),0,$(HB_APP_COMPILE_LIST))),0CSGMRS0)
APP_LIST += csgmrs
endif

ifeq ($(findstring 0CTS0,$(subst $(comma),0,$(HB_APP_COMPILE_LIST))),0CTS0)
APP_LIST += cts
endif

ifeq ($(findstring 0DEMO0,$(subst $(comma),0,$(HB_APP_COMPILE_LIST))),0DEMO0)
APP_LIST += demo
endif


#检测产品形态类型
ifeq ($(findstring 0CCO0,$(subst $(comma),0,$(HB_SELECT_PRODUCT_TYPE_LIST))),0CCO0)
PRODUCT_LIST += cco
endif

ifeq ($(findstring 0STA0,$(subst $(comma),0,$(HB_SELECT_PRODUCT_TYPE_LIST))),0STA0)
PRODUCT_LIST += sta
endif

ifeq ($(findstring 0NDM0,$(subst $(comma),0,$(HB_SELECT_PRODUCT_TYPE_LIST))),0NDM0)
PRODUCT_LIST += ndm
endif

#ALL 目录下主要存放不区分产品形态的编译信息
ifeq ($(HB_DEBUG_COMPILE_ENABLE), YES)  
ALL_OUT = $(LITEOSTOPDIR)/target/delivery/$(HB_PROJECT_NAME_LIST)_debug/ALL
else
ALL_OUT = $(LITEOSTOPDIR)/target/delivery/$(HB_PROJECT_NAME_LIST)_release/ALL
endif
ALL_BUILD := $(ALL_OUT)/build
ALL_LIBS := $(ALL_OUT)/lib
ALL_TARGET := $(ALL_OUT)/bin
ALL_NV := $(ALL_OUT)/nv



INCLUDE_LIB_DIR_MDM = $(LITEOSTOPDIR)/mdm/inc
INCLUDE_LIB_DIR_BSP = $(LITEOSTOPDIR)/bsp/inc
INCLUDE_LIB_DIR_ROOT= $(LITEOSTOPDIR)/target/include

INCLUDE_LIB_DIR_STD_LIB = $(LITEOSTOPDIR)/target/include/layers/base
INCLUDE_LIB_DIR_BOARDCFG = $(LITEOSTOPDIR)/target/include/layers/boardcfg
INCLUDE_LIB_DIR_DFX = $(LITEOSTOPDIR)/target/include/layers/dfx
INCLUDE_LIB_DIR_DRIVER_CHIP = $(LITEOSTOPDIR)/target/include/layers/driver/chip
INCLUDE_LIB_DIR_DRIVER_BOARD = $(LITEOSTOPDIR)/target/include/layers/driver/board
INCLUDE_LIB_DIR_OSA = $(LITEOSTOPDIR)/target/include/layers/osa
INCLUDE_LIB_DIR_SAL = $(LITEOSTOPDIR)/target/include/layers/sal
INCLUDE_LIB_DIR_BOOT = $(LITEOSTOPDIR)/target/include/plc/boot
INCLUDE_LIB_DIR_MAC = $(LITEOSTOPDIR)/target/include/layers/mac
INCLUDE_LIB_DIR_UPG = $(LITEOSTOPDIR)/target/include/layers/upg
INCLUDE_LIB_DIR_ICCP = $(LITEOSTOPDIR)/target/include/iccp


LD_PATH = $(LITEOSTOPDIR)/target/hibuilder/ld_scripts
SCRIPT_PATH = $(LITEOSTOPDIR)/target/hibuilder/scripts
INCLUDE = 	-I $(LITEOSTOPDIR)/target/hibuilder/ver_cfg \
			-I $(LITEOSTOPDIR)/bsp/os/Huawei_LiteOS/kernel/include \
			-I $(LITEOSTOPDIR)/bsp/os/Huawei_LiteOS/kernel/base/include \
			-I $(LITEOSTOPDIR)/bsp/os/Huawei_LiteOS/lib/libc/include \
			-I $(LITEOSTOPDIR)/bsp/os/Huawei_LiteOS/lib/libc/stdio \
			-I $(LITEOSTOPDIR)/bsp/os/Huawei_LiteOS/lib/libc/time \
			-I $(LITEOSTOPDIR)/bsp/os/Huawei_LiteOS/lib/libc/include/sys \
			-I $(LITEOSTOPDIR)/bsp/os/Huawei_LiteOS/lib/libsec/include \
			-I $(LITEOSTOPDIR)/bsp/os/Huawei_LiteOS/lib/libm/include \
			-I $(LITEOSTOPDIR)/bsp/os/Huawei_LiteOS/lib/libc/include/arch-arm/include \
			-I $(LITEOSTOPDIR)/bsp/os/Huawei_LiteOS/lib/libc/include/kernel \
			-I $(LITEOSTOPDIR)/bsp/os/Huawei_LiteOS/lib/libc/include/kernel/arch \
			-I $(LITEOSTOPDIR)/bsp/os/Huawei_LiteOS/compat/posix/include \
			-I $(LITEOSTOPDIR)/bsp/os/Huawei_LiteOS/compat/linux/include \
			-I $(LITEOSTOPDIR)/bsp/os/Huawei_LiteOS/compat/posix/src \
			-I $(LITEOSTOPDIR)/bsp/os/Huawei_LiteOS/compat/linux/include/linux \
			-I $(LITEOSTOPDIR)/bsp/os/Huawei_LiteOS/platform/bsp/hi3911 \
			-I $(LITEOSTOPDIR)/bsp/os/Huawei_LiteOS/platform/bsp/hi3911/include \
			-I $(LITEOSTOPDIR)/bsp/os/Huawei_LiteOS/platform/bsp/hi3911/config  \
			-I $(LITEOSTOPDIR)/bsp/os/Huawei_LiteOS/platform/bsp/hi3911/mmu \
			-I $(LITEOSTOPDIR)/bsp/os/Huawei_LiteOS/platform/cpu/arm/include \
			-I $(LITEOSTOPDIR)/bsp/os/Huawei_LiteOS/platform/cpu/arm/arm926 \
			-I $(LITEOSTOPDIR)/bsp/os/Huawei_LiteOS/net/lwip_sack/include/lwip \
			-I $(LITEOSTOPDIR)/bsp/os/Huawei_LiteOS/net/lwip_sack/include \
			-I $(LITEOSTOPDIR)/bsp/os/Huawei_LiteOS/net/lwip_sack/include/ipv4 \
			-I $(LITEOSTOPDIR)/bsp/os/Huawei_LiteOS/net/lwip_sack/include/ipv4/lwip \
			-I $(LITEOSTOPDIR)/mac/inc \
			-I $(LITEOSTOPDIR)/target/include/plc \
			-I $(LITEOSTOPDIR)/target/include/plc/boot \
			-I $(LITEOSTOPDIR)/target/config/diag \
			-I $(LITEOSTOPDIR)/target/config/app \
			-I $(LITEOSTOPDIR)/target/config/nv \
			-I $(LITEOSTOPDIR)/target/config/product/cfg \
			-I $(LITEOSTOPDIR)/target/config/newbee \
			-I $(INCLUDE_LIB_DIR_ROOT) \
			-I $(INCLUDE_LIB_DIR_STD_LIB)\
			-I $(INCLUDE_LIB_DIR_DRIVER_CHIP)\
			-I $(INCLUDE_LIB_DIR_DRIVER_BOARD)\
			-I $(INCLUDE_LIB_DIR_OSA)\
			-I $(INCLUDE_LIB_DIR_SAL)\
			-I $(INCLUDE_LIB_DIR_DFX)\
			-I $(INCLUDE_LIB_DIR_BSP)\
			-I $(INCLUDE_LIB_DIR_BOARDCFG)\
			-I $(INCLUDE_LIB_DIR_MAC)\
			-I $(INCLUDE_LIB_DIR_UPG)\
			-I $(INCLUDE_LIB_DIR_ICCP)
			
BTINCLUDE = 	-I $(LITEOSTOPDIR)/target/hibuilder/ver_cfg \
			-I $(LITEOSTOPDIR)/boot/include \
			-I $(LITEOSTOPDIR)/boot/arch/arm/include/asm \
			-I $(LITEOSTOPDIR)/bsp/inc \
			-I $(LITEOSTOPDIR)/mac/inc \
			-I $(LITEOSTOPDIR)/target/include \
			-I $(LITEOSTOPDIR)/target/include/plc \
			-I $(LITEOSTOPDIR)/target/include/plc/boot \
			-I $(LITEOSTOPDIR)/target/config/diag \
			-I $(LITEOSTOPDIR)/target/config/app \
			-I $(LITEOSTOPDIR)/target/config/nv \
			-I $(LITEOSTOPDIR)/target/config/product/cfg \
			-I $(LITEOSTOPDIR)/target/config/newbee \
			-I $(LITEOSTOPDIR)/target/include/layers/osa  \
			-I $(LITEOSTOPDIR)/target/include/layers/base  \
			-I $(LITEOSTOPDIR)/target/include/layers/driver  \
			-I $(LITEOSTOPDIR)/target/include/layers/dfx  \
			
#C 语言编译选项	
FP = -fno-omit-frame-pointer
OPTS = -fno-aggressive-loop-optimizations -fno-builtin  -gdwarf-2 -nostdinc -nostdlib -std=c99 -Wpointer-arith -Wstrict-prototypes -Wno-write-strings -mno-thumb-interwork  -ffunction-sections -fdata-sections -fno-exceptions -fno-short-enums 
BTOPTS = -fno-aggressive-loop-optimizations -fno-builtin  -gdwarf-2 -nostdinc -nostdlib -std=c99 -Wpointer-arith -Wstrict-prototypes -Wno-write-strings -mno-thumb-interwork -ffunction-sections -fdata-sections -fno-exceptions  -fno-short-enums 
#-march=armv5te
OPTS += -mcpu=arm926ej-s -Wundef -DPRODUCT_CFG_BUILD_DATE=$(CUR_DATE) -DPRODUCT_CFG_BUILD_TIME=$(CUR_TIME)
BTOPTS += -DPRODUCT_CFG_BUILD_DATE=$(CUR_DATE) -DPRODUCT_CFG_BUILD_TIME=$(CUR_TIME)
CFLAGS  = $(OPTS) $(INCLUDE) -DCYGPKG_POSIX_SIGNALS  -D__ECOS__   -D__RTOS_ -DLOSCFG_ARM926
BTCFLAGS  = $(BTOPTS) $(BTINCLUDE) -Wall -fno-strict-aliasing -Werror
CFLAGS += -Wall -fno-strict-aliasing #-Werror
CFLAGS += -fstack-protector


# (CCO) (STA) (NDM)  (BOOT)

ifeq ($(HB_PRODUCT_NAME_LIST), CCO)
CFLAGS += -DOBC_IMAGE_MODEM
else ifeq ($(HB_PRODUCT_NAME_LIST), STA)
CFLAGS += -DOBC_IMAGE_BOOTROM
else ifeq ($(HB_PRODUCT_NAME_LIST), NDM)
CFLAGS += -DOBC_IMAGE_APP
else ifeq ($(HB_PRODUCT_NAME_LIST), BOOT)
CFLAGS += -DOBC_IMAGE_BOOTLOADER
endif
BTCFLAGS += -DOBC_IMAGE_BOOTLOADER

ifeq ($(HB_DEBUG_COMPILE_ENABLE), YES)  
CFLAGS += -g -O2 -DPRODUCT_CFG_HAVE_FEATURE_SYS_ERR_INFO 
BTCFLAGS += -O2
else
CFLAGS += -O2  -DPRODUCT_CFG_HAVE_FEATURE_SYS_ERR_INFO
BTCFLAGS += -O2
endif

#汇编语言编译选项
#ASOPTS = -march=armv5te 
ASOPTS += -mcpu=arm926ej-s
ASFLAGS = $(ASOPTS) $(INCLUDE)
AS_OBJS_LIBC_FLAGS=-D__ASSEMBLY__ 

#库文件生成选项
ARFLAGS = cr

#常用命令
RM = -rm -rf
MAKE = make
