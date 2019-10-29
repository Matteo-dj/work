# A simple Makefile for lib(libxxx.a)
# By Late Lee(http://www.latelee.org)
ROOT_DIR= ../..
include $(LITEOSTOPDIR)/config.mk

MOD_INCLUDE = -I $(LITEOSTOPDIR)/mdm \
			-I $(LITEOSTOPDIR)/mdm/inc \
			-I $(LITEOSTOPDIR)/bsp/sal/inc \
			-I $(LITEOSTOPDIR)/bsp/drivers/hiplcphy \
			-I $(LITEOSTOPDIR)/bsp/inc \
			-I $(LITEOSTOPDIR)/bsp/osa/inc \
			-I $(LITEOSTOPDIR)/target/include/plc/int \

MODDIR = bsp/osa			
LIBOUT = $(BUILD)/bsp/osa
LIB = $(OUT)/lib/libosa.a
CFLAGS += $(MOD_INCLUDE)

SRCS = $(wildcard src/*.c dfx/*.c )
OBJS = $(patsubst %.c,$(LIBOUT)/%.o,$(SRCS))

all: $(LIBOUT) $(LIB)

$(OBJS): $(LIBOUT)/%.o : %.c
	@echo Compile  $<
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

$(LIBOUT):
	mkdir -p $(OUT)/lib/
	mkdir -p $(LIBOUT)/src
	mkdir -p $(LIBOUT)/dfx
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
