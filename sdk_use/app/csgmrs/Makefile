# A simple Makefile for lib(libxxx.a)
# By Late Lee(http://www.latelee.org)
ROOT_DIR= ../..
include $(LITEOSTOPDIR)/config.mk
MOD_INCLUDE = 	-I $(LITEOSTOPDIR)/app/csgmrs/inc \
			-I $(LITEOSTOPDIR)/app/csgmrs/ndm_inc \
			-I $(LITEOSTOPDIR)/app/csgmrs/dfx \
			-I $(LITEOSTOPDIR)/app/csgmrs/equ \
			-I $(LITEOSTOPDIR)/app/csgmrs/src \
			-I $(LITEOSTOPDIR)/app/csgmrs/cct3911 \
			-I $(LITEOSTOPDIR)/app/csgmrs/ndm_device \

MODDIR = app/csgmrs
LIBOUT = $(BUILD)/$(MODDIR)
LIB = $(OUT)/lib/app/libcsgmrs.a

ifeq ($(HB_PRODUCT_NAME_LIST), NDM)
SRCS = $(wildcard ndm_common/*.c dfx/*.c equ/*.c init/*.c src/*.c ndm_device/*.c cct3911/*.c)
else
SRCS = $(wildcard dfx/*.c equ/*.c init/*.c src/*.c)
endif
OBJS = $(patsubst %.c,$(LIBOUT)/%.o,$(SRCS))
CFLAGS += $(MOD_INCLUDE)

include current_module.rules
include $(SCRIPT_PATH)/mk_prim_xml_step1.mk

all:  $(LIBOUT) $(LIB) do_prim_xml_step1

$(OBJS): $(LIBOUT)/%.o : %.c
	@echo Compile  $<
	@python $(SCRIPT_PATH)/cc_prim.py "_PYTHON_ARG_" "CC" $(CC) "_PYTHON_ARG_" "CFLAGS" $(CFLAGS) "_PYTHON_ARG_" "SRC_FILE_NAME" $< "_PYTHON_ARG_" "DST_FILE_NAME" $@ "_PYTHON_ARG_" "CURRENT_DIR" $(LITEOSTOPDIR)/$(MODDIR)

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

$(LIBOUT):
	@mkdir -p $(LIBOUT)/ndm_common
	@mkdir -p $(LIBOUT)/ndm_device
	@mkdir -p $(LIBOUT)/dfx
	@mkdir -p $(LIBOUT)/equ
	@mkdir -p $(LIBOUT)/init
	@mkdir -p $(LIBOUT)/src
	@mkdir -p $(OUT)/lib/app
	@mkdir -p $(LIBOUT)/cct3911
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

.PHONY: all clean check	clean_target_lib
