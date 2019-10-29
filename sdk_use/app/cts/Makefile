# A simple Makefile for lib(libxxx.a)
# By Late Lee(http://www.latelee.org)
ROOT_DIR= ../..
include $(LITEOSTOPDIR)/config.mk
MOD_INCLUDE = 	-I $(LITEOSTOPDIR)/app/cts/inc \
			-I $(LITEOSTOPDIR)/app/cts/config \
			-I $(LITEOSTOPDIR)/app/cts/sln \

MODDIR = app/cts
LIBOUT = $(BUILD)/$(MODDIR)
LIB = $(OUT)/lib/app/libcts.a

SRCS = $(wildcard  init/*.c  main/*.c api/*.c )
OBJS = $(patsubst %.c,$(LIBOUT)/%.o,$(SRCS))
CFLAGS += $(MOD_INCLUDE)


include current_module.rules
include $(SCRIPT_PATH)/mk_prim_xml_step1.mk

all:  $(LIBOUT) make_test_config $(LIB) do_prim_xml_step1  
$(OBJS): $(LIBOUT)/%.o : %.c
	@echo Compile  $<
	@python $(SCRIPT_PATH)/cc_prim.py "_PYTHON_ARG_" "CC" $(CC) "_PYTHON_ARG_" "CFLAGS" $(CFLAGS) "_PYTHON_ARG_" "SRC_FILE_NAME" $< "_PYTHON_ARG_" "DST_FILE_NAME" $@ "_PYTHON_ARG_" "CURRENT_DIR" $(LITEOSTOPDIR)/$(MODDIR)

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)


	
$(LIBOUT):
	@mkdir -p $(LIBOUT)/init
	@mkdir -p $(LIBOUT)/main
	@mkdir -p $(LIBOUT)/tmp
	@mkdir -p $(LIBOUT)/api
	@mkdir -p $(OUT)/lib/app
	@mkdir -p $(LITEOSTOPDIR)/$(MODDIR)/config/pbin/rsf

make_test_config:
#根据配置文件生成C语言文件
	make -C $(LITEOSTOPDIR)/$(MODDIR)/config  do_pcfg
#根据C语言文件生成二进制文件
	make -C $(LITEOSTOPDIR)/$(MODDIR)/config TMP_LIBOUT=$(LIBOUT)/tmp  do_pbin

clean:
	@$(RM) $(OBJS) $(LIB) $(LIBOUT) $(CFG_FILES_PATH)/config/pbin *.bak *~  $(LITEOSTOPDIR)/app/cts/config/pbin

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

.PHONY: all clean check	clean_target_lib make_test_config
