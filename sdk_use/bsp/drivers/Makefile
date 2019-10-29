# A simple Makefile for lib(libxxx.a)
# By Late Lee(http://www.latelee.org)
ROOT_DIR= ../..
include $(LITEOSTOPDIR)/config.mk

MOD_INCLUDE = -I $(LITEOSTOPDIR)/bsp/drivers/chip/dfx \
		-I $(LITEOSTOPDIR)/bsp/drivers/chip/hispiflash \
		-I $(LITEOSTOPDIR)/bsp/drivers/chip/spi \
		-I $(LITEOSTOPDIR)/target/include/plc/os \
		-I $(LITEOSTOPDIR)/target/include/plc/int \
		-I $(LITEOSTOPDIR)/target/include/layers/base \
		-I $(LITEOSTOPDIR)/target/config/product/cfg \
		-I $(LITEOSTOPDIR)/bsp/sal/inc \
		-I $(LITEOSTOPDIR)/target/include/layers/driver \
		-I $(LITEOSTOPDIR)/target/include/iccp \
		-I $(INCLUDE_LIB_DIR_MDM)\

MODDIR = bsp/drivers			
LIBOUT = $(BUILD)/bsp/drivers
LIB = $(OUT)/lib/libdrv.a


SRCS = $(wildcard chip/sharemem/*.c chip/sharetask/*.c chip/eth/*.c chip/hrtimer/*.c chip/watchdog/*.c chip/gpio/*.c chip/common/*.c  chip/rf/*.c chip/spi/*.c chip/dfx/*.c chip/hispiflash/*.c chip/led/*.c chip/uart/*.c chip/switch/*.c chip/lattice/*.c chip/i2c/*.c chip/ssi/*.c chip/cipher/*.c chip/time_cnt/*.c chip/datacollect/*.c chip/printf/*.c)
SRCS += $(wildcard board/hwid/*.c  board/rtc/*.c board/ioexp/*.c board/3518/*.c board/battery/*.c board/filemanager/*.c board/key_lcd/*.c board/icc/*.c board/wifi/*.c)
OBJS = $(patsubst %.c,$(LIBOUT)/%.o,$(SRCS))
CFLAGS += $(MOD_INCLUDE)

all: $(LIBOUT) $(LIB)

$(OBJS): $(LIBOUT)/%.o : %.c
	@echo Compile  $<
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

$(LIBOUT):
	mkdir -p $(LIBOUT)/chip/hrtimer
	mkdir -p $(LIBOUT)/chip/watchdog
	mkdir -p $(LIBOUT)/chip/gpio
	mkdir -p $(LIBOUT)/chip/common
	mkdir -p $(LIBOUT)/chip/rf
	mkdir -p $(LIBOUT)/chip/spi
	mkdir -p $(LIBOUT)/chip/dfx
	mkdir -p $(LIBOUT)/chip/hispiflash
	mkdir -p $(LIBOUT)/chip/led
	mkdir -p $(LIBOUT)/chip/uart
	mkdir -p $(LIBOUT)/chip/switch
	mkdir -p $(LIBOUT)/chip/lattice
	mkdir -p $(LIBOUT)/chip/i2c
	mkdir -p $(LIBOUT)/chip/ssi
	mkdir -p $(LIBOUT)/chip/cipher
	mkdir -p $(LIBOUT)/chip/time_cnt
	mkdir -p $(LIBOUT)/chip/datacollect
	mkdir -p $(LIBOUT)/chip/eth
	mkdir -p $(LIBOUT)/board/hwid
	mkdir -p $(LIBOUT)/board/rtc
	mkdir -p $(LIBOUT)/board/ioexp
	mkdir -p $(LIBOUT)/board/3518
	mkdir -p $(LIBOUT)/board/battery
	mkdir -p $(LIBOUT)/board/filemanager
	mkdir -p $(LIBOUT)/board/key_lcd
	mkdir -p $(LIBOUT)/board/icc
	mkdir -p $(LIBOUT)/board/wifi
	mkdir -p $(LIBOUT)/chip/printf
	mkdir -p $(LIBOUT)/chip/sharemem
	mkdir -p $(LIBOUT)/chip/sharetask
	@mkdir -p $(OUT)/lib

clean:
	@$(RM) $(OBJS) $(LIB) $(LIBOUT) *.bak *~

#清空目标lib 用于在增量编译错误时，将对应的目标lib清空，避免后续还用旧的lib 2016/07/09 w00291248
clean_target_lib:
	@$(RM) $(LIB) *.bak *~
	
check:
ifeq ($(LIB),$(wildcard $(LIB)))
	@echo -n "HB BUILD $(MODDIR) [SUCCESS]" 
	@echo "`date "+%Y-%m-%d %H:%M:%S"`"
else
	@echo -n "HB BUILD $(MODDIR) [FAILURE]" 
	@echo "`date "+%Y-%m-%d %H:%M:%S"`"
endif

.PHONY: all clean check clean_target_lib
