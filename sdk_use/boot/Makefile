# A simple Makefile for lib(libxxx.a)
# By Late Lee(http://www.latelee.org)
ROOT_DIR = ../
include $(LITEOSTOPDIR)/config.mk
MOD_INCLUDE = 	-I $(ROOT_DIR)/boot/include \
			-I $(ROOT_DIR)/boot/include/boot \

MODDIR = boot
LIBOUT = $(ALL_OUT)/boot/build
LIB = $(ALL_OUT)/boot/lib
BOOT_TARGET = $(ALL_OUT)/boot/bin/bootloader
BOOT_TARGET_POST = $(ALL_OUT)/boot/bin/bootloader.bin
REGBIN = $(LITEOSTOPDIR)/target/libs/boot_reg/reg_info.bin
FILESIZE = $(shell ls -l $(BOOT_TARGET)_pre.bin | awk '{print $$5}')
TEST_BEFORE = test
BT_TEXT_BASE = 0x70155000
FILL_ADDR_START = 64
FILL_SIZE = 1024
FILL_ADDR_END = 1088

SRCS = $(wildcard arch/arm/cpu/arm926ejs/*.c arch/arm/lib/*.c common/*.c  drivers/*/*.c lib/*.c lib/lzma/*.c lib/libsec/*.c)
OBJS = $(patsubst %.c,$(LIBOUT)/%.o,$(SRCS))

ASSRCS = $(wildcard arch/arm/cpu/arm926ejs/*.S common/*.S lowlevel_init/*.S arch/arm/lib/*.S)
ASOBJS = $(patsubst %.S,$(LIBOUT)/%.o,$(ASSRCS))

BTCFLAGS += $(MOD_INCLUDE)
ASFLAGS += $(MOD_INCLUDE) -DBOOT_FILLSIZE=$(FILL_SIZE)

BOOT_LDFLAGS  = -nostartfiles -static --gc-sections  -L$(ALL_OUT)/boot/lib -T$(LD_PATH)/bootloader.ld
BOOT_LDFLAGS += -Ttext $(BT_TEXT_BASE)

BOOT_LIBDEP:= --start-group \
		-lboot  -lgcc -lgcc_eh \
		--end-group

all: $(TEST_BEFORE) $(LIBOUT) $(BOOT_TARGET) $(BOOT_TARGET_POST)
#all: backupbin
$(TEST_BEFORE):
ifeq ($(BOOT_TARGET),$(wildcard $(BOOT_TARGET)))
	@echo -e -n "\033[42;37m We have built boot,exit ok !\033[0m" 
	exit 0
else
	@mkdir -p $(ALL_OUT)/boot/lib
	cp -rf $(LITEOSTOPDIR)/target/libs/liteos/libgcc.a $(ALL_OUT)/boot/lib
	cp -rf $(LITEOSTOPDIR)/target/libs/liteos/libgcc_eh.a $(ALL_OUT)/boot/lib
endif

$(IOBJS): $(LIBOUT)/%.i : %.c
	@echo Compile  $<
	$(CC) $(BTCFLAGS) -E $< -i $@

$(OBJS): $(LIBOUT)/%.o : %.c
	@echo Compile  $<
#$(CC) $(BTCFLAGS) -E $< -o $@
	$(CC) $(BTCFLAGS) -c $< -o $@
	
$(ASOBJS): $(LIBOUT)/%.o : %.S
	@$(CC) $(ASFLAGS) -c $< -o $@
	
$(BOOT_TARGET): $(IOBJS) $(OBJS) $(ASOBJS)
	$(AR) $(ARFLAGS) $(ALL_OUT)/boot/lib/libboot.a $(OBJS) $(ASOBJS)
	$(LD) $(BOOT_LDFLAGS) -Map=$@.map -o  $@ $(BOOT_LIBDEP)
	$(OBJCOPY) -O binary $@  $@_pre.bin
	$(OBJDUMP) -d  $@ > $@.asm
    
$(BOOT_TARGET_POST): $(BOOT_TARGET)
	dd if=$(BOOT_TARGET)_pre.bin of=$(ALL_OUT)/boot/bin/zz1 bs=1 count=$(FILL_ADDR_START) ;\
	dd if=$(BOOT_TARGET)_pre.bin of=$(ALL_OUT)/boot/bin/zz2 bs=1 count=$(FILESIZE) skip=$(FILL_ADDR_END);\
	dd if=$(ALL_OUT)/boot/bin/zz1 of=$(BOOT_TARGET).bin bs=1 count=$(FILL_ADDR_START); \
	dd if=$(REGBIN) of=$(BOOT_TARGET).bin bs=1 count=$(FILL_SIZE) seek=$(FILL_ADDR_START); \
	dd if=$(ALL_OUT)/boot/bin/zz2 of=$(BOOT_TARGET).bin bs=1 count=$(FILESIZE) seek=$(FILL_ADDR_END)
	rm -rf $(ALL_OUT)/boot/bin/zz1 $(ALL_OUT)/boot/bin/zz2 $(BOOT_TARGET)_pre.bin

backupbin:
	@echo -e -n "\033[42;37m We use backup boot.bin before v200 boot source code is ready!\033[0m"
	mkdir -p $(ALL_OUT)/boot/bin
	cp -rf backupbin/$(HB_PROJECT_NAME_LIST)/bootloader.bin $(ALL_OUT)/boot/bin
	cp -rf backupbin/$(HB_PROJECT_NAME_LIST)/bootloader.map $(ALL_OUT)/boot/bin
	cp -rf backupbin/$(HB_PROJECT_NAME_LIST)/bootloader $(ALL_OUT)/boot/bin
	$(OBJDUMP) -d  $(BOOT_TARGET) > $(BOOT_TARGET).asm

$(LIBOUT):
	@mkdir -p $(ALL_OUT)/boot/build
	@mkdir -p $(ALL_OUT)/boot/lib
	@mkdir -p $(ALL_OUT)/boot/bin
	@mkdir -p $(LIBOUT)/arch/arm/cpu/arm926ejs
	@mkdir -p $(LIBOUT)/arch/arm/lib
	@mkdir -p $(LIBOUT)/common
	@mkdir -p $(LIBOUT)/drivers/chip
	@mkdir -p $(LIBOUT)/drivers/iomux
	@mkdir -p $(LIBOUT)/drivers/hisfc350
	@mkdir -p $(LIBOUT)/drivers/led
	@mkdir -p $(LIBOUT)/drivers/serial
	@mkdir -p $(LIBOUT)/drivers/watchdog
	@mkdir -p $(LIBOUT)/lib
	@mkdir -p $(LIBOUT)/lib/lzma
	@mkdir -p $(LIBOUT)/lib/libsec
	@mkdir -p $(LIBOUT)/lowlevel_init

clean:
	@$(RM) $(OBJS) $(ASOBJS) $(LIB) $(LIBOUT) $(ALL_OUT)/boot/bin/* *.bak *~
    
#清空目标lib 用于在增量编译错误时，将对应的目标lib清空，避免后续还用旧的lib 2016/07/09 w00291248
clean_target_lib:
	@$(RM) $(LIB) *.bak *~	
	
check:
	@if [ -f $(BOOT_TARGET).bin ] && [ -f $(BOOT_TARGET).asm ] && [ -f $(BOOT_TARGET).map ]; \
	then \
	exit 0;\
	else \
	echo "$(TARGET)/$(target)_$$app.bin" ;\
	echo -n "HB1 BUILD $(MODDIR) $$dir [FAILURE]" ;\
	echo "`date "+%Y-%m-%d %H:%M:%S"`" ; \
	exit 1;\
	fi
	@echo -n "HB BUILD $(MODDIR) [SUCCESS]" 
	@echo "`date "+%Y-%m-%d %H:%M:%S"`"

.PHONY: all clean check	clean_target_lib
