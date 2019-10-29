# A simple Makefile for lib(libxxx.a)
# By Late Lee(http://www.latelee.org)
ROOT_DIR= ../..
include $(LITEOSTOPDIR)/config.mk

MOD_INCLUDE = -I $(LITEOSTOPDIR)/target/include/plc/os \
		-I $(LITEOSTOPDIR)/target/include/plc/int \
		-I $(LITEOSTOPDIR)/target/include/layers/base \
		-I $(LITEOSTOPDIR)/target/config/product/cfg \
		-I $(LITEOSTOPDIR)/bsp/inc \
		-I $(LITEOSTOPDIR)/mdm/inc \
		-I $(LITEOSTOPDIR)/target/include/layers/driver \


MODDIR = bsp/init			
LIBOUT = $(BUILD)/bsp/init
LIB = $(OUT)/lib/libinit.a


SRCS = $(wildcard main/*.c)
OBJS = $(patsubst %.c,$(LIBOUT)/%.o,$(SRCS))
CFLAGS += $(MOD_INCLUDE)

all: $(LIBOUT) $(LIB)

$(OBJS): $(LIBOUT)/%.o : %.c
	@echo Compile  $<
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

$(LIBOUT):
	mkdir -p $(LIBOUT)/main


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
