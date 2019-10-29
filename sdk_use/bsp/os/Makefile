
################################################################################
# Main Makefile for HiBuilder 
#
# History
# Author                			Date          Version 
# chenghuanhuan/00132816   2016-05-19    V1.0  
################################################################################

#A simple Makefile for lib(libxxx.a)
# By Late Lee(http://www.latelee.org)

include $(LITEOSTOPDIR)/config.mk

LIB_LITEOS_BAK_DIR := $(LITEOSTOPDIR)/target/libs/liteos

LIB_SUBDIRS := Huawei_LiteOS

LITEOS_LIBS := libarm926.a libcppsupport.a  libgcc_eh.a liblitekernel.a  libsupc++.a libbspcommon.a libcpup.a libhi3911.a  libm.a libz.a libc.a  libcsysdeps.a  libhidmac.a libposix.a libcmsis.a libgcc.a liblinuxadp.a libstdc++.a liblwip.a  libhieth-sf.a

MODDIR = liteos/Huawei_LiteOS
exist = notexist

ifeq ($(HB_PRODUCT_NAME_LIST), CCO)
PRODUCT = PRODUCT_CFG_PRODUCT_TYPE_CCO
endif

ifeq ($(HB_PRODUCT_NAME_LIST), STA)
PRODUCT = PRODUCT_CFG_PRODUCT_TYPE_STA
endif

ifeq ($(HB_PRODUCT_NAME_LIST), NDM)
PRODUCT = PRODUCT_CFG_PRODUCT_TYPE_NDM
endif

prepare:
	@mkdir -p $(ALL_LIBS)
	@mkdir -p $(LIB_LITEOS_BAK_DIR)
#编译之前需要先clean，
#因为如果要编译其他产品形态的SDK，Os中的代码因为没有修改而不会重新编译。

all: prepare 
ifeq ($(HB_LITEOS_COMPILE_ENABLE), YES)  
	for dir in $(LIB_SUBDIRS); \
	do $(MAKE) -C $$dir lib || exit 1; \
	done
	cp $(LIB_SUBDIRS)/out/hi3911/lib/*.a  $(ALL_LIBS)
#	rm -rf $(LIB_LITEOS_BAK_DIR)/*
	cp $(LIB_SUBDIRS)/out/hi3911/lib/*.a  $(LIB_LITEOS_BAK_DIR)/
else
	cp -f $(LIB_LITEOS_BAK_DIR)/*.a  $(ALL_LIBS)
endif
clean:
ifeq ($(HB_LITEOS_COMPILE_ENABLE), YES)
	@for dir in $(LIB_SUBDIRS); \
	do $(MAKE) -C $$dir clean|| exit 1; \
	done
endif
	for dir in $(LITEOS_LIBS); \
	do if [ -f $(ALL_LIBS)/$$dir ]; \
	then \
	rm $(ALL_LIBS)/$$dir;\
	fi \
	done
	
check:
	@for dir in $(LITEOS_LIBS);\
	do if [ -f $(ALL_LIBS)/$$dir ]; \
	then \
	exit 0;\
	else \
	echo -n "HB BUILD $(MODDIR) $$dir [FAILURE]" ;\
	echo "`date "+%Y-%m-%d %H:%M:%S"`" ; \
	exit 1;\
	fi;\
	done
	@echo -n "HB BUILD $(MODDIR) [SUCCESS]" 
	@echo "`date "+%Y-%m-%d %H:%M:%S"`" 
.PHONY: all clean  check

