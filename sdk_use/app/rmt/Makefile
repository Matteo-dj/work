# A simple Makefile for lib(libxxx.a)
# By Late Lee(http://www.latelee.org)
ROOT_DIR= ../..
include $(LITEOSTOPDIR)/config.mk
MOD_INCLUDE = 	-I $(LITEOSTOPDIR)/app/rmt/inc \

MODDIR = app/rmt
LIBOUT = $(BUILD)/$(MODDIR)
LIB = $(OUT)/lib/app/librmt.a

SRCS = $(wildcard init/*.c src/*.c)
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
	@mkdir -p $(LIBOUT)/init
	@mkdir -p $(OUT)/lib/app

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
