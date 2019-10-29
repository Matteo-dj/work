################################################################################
# Makefile for HiBuilder 
#
# History
# Author                			Date          Version 
# chenghuanhuan/00132816   2016-05-19    V1.0  
################################################################################

################################################################################
# Environment Configuration
################################################################################
include $(LITEOSTOPDIR)/config.mk

SCRIPTS_DIR := $(LITEOSTOPDIR)/target/hibuilder/scripts/cts
PRJ_CFG_FILES_PATH := $(LITEOSTOPDIR)/app/cts/config
PRJ_PCFG_FILES            := $(wildcard $(PRJ_CFG_FILES_PATH)/*.pcfg)
PRJ_PCFGBIN_PATH           =$(PRJ_CFG_FILES_PATH)/pbin
PRJ_PCFGBIN_RSF_PATH       =$(PRJ_CFG_FILES_PATH)/pbin/rsf
PRJ_PCFGBIN_RSF_FILES     = $(wildcard $(PRJ_PCFGBIN_RSF_PATH)/*.rtcfg)
################################################################################

################################################################################
PRJ_OPTIONS := -fno-builtin  -gdwarf-2 -nostdinc -nostdlib -std=c99 -Wpointer-arith -Wstrict-prototypes -Wno-write-strings -mno-thumb-interwork  -ffunction-sections -fdata-sections -fno-exceptions -fno-short-enums  -D__FILE_ID__=0 -D__FILE_MOD_ID__=0 $(INCLUDE) -I$(PRJ_CFG_FILES_PATH)
################################################################################

PCFG_OBJS = $(patsubst %.pcfg,%,$(PRJ_PCFG_FILES))

PCFGBIN_OBJS = $(patsubst %.rtcfg,%,$(PRJ_PCFGBIN_RSF_FILES))

$(PCFG_OBJS): %:%.pcfg
	echo compile config file $< ...
	perl $(SCRIPTS_DIR)/readctscfg2pcfg.pl $< $(PRJ_PCFGBIN_RSF_PATH)
do_pcfg:$(PCFG_OBJS)
	#@echo compile pconfig end


################################################################################
$(PCFGBIN_OBJS): %:%.rtcfg
	@echo compile pbin $< ...
	@echo $(PRJ_OPTIONS) >$(TMP_LIBOUT)/tmp_opt.cfg 
	@st2bin -cfg $< -o $(PRJ_PCFGBIN_PATH)/$(basename $(notdir $<)) -opt $(TMP_LIBOUT)/tmp_opt.cfg
do_pbin:$(PCFGBIN_OBJS)
	#@echo compile pbin end
################################################################################




.PHONY: do_pcfg do_pbin
