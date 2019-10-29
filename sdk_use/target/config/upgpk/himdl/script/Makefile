################################################################################
# Makefile for OneBuilder 
#
# History
# Author                Date          Version 
# shenhankun/00130424   2011-06-23    V1.0  
################################################################################

################################################################################
# Environment Configuration
################################################################################
OBC_BUILDER_VER            :=$(OB_ENV_ONEBUILDER) V$(OB_ENV_ONEBUILDER_VER)
BUILD_NAME                 =UPGPK
include $(OB_ENV_TARGET_SCRIPT_PATH)/makefile/mk/makefile_cfg.mk
PRJ_UPGPK_PATH             =$(CURDIR)
PRJ_UPGPK_FILES_PATH       ?=$(basedir $(PRJ_UPGPK_PATH))
-include $(PRJ_UPGPK_FILES_PATH)/upgpk.cfg
PRJ_UPGPK_FILES_EXIST_PATH = $(OB_ENV_BUILD_TARGET_PATH)/libs/upgpk/$(strlower $(OB_ENV_PRODUCT_NAME))
################################################################################

################################################################################
OB_ENV_BUILD_CFG_UPGPK_PACKET_FILTER ?=*.upgpk
PRJ_UPGPK_FILES_V2         :=${foreach _name, ${OB_ENV_BUILD_CFG_UPGPK_PACKET_FILTER},$(wildcard $(PRJ_UPGPK_FILES_EXIST_PATH)/${_name})}
PRJ_PROT_VER_TYPE_V2           =$(lastword $(subst _, ,$(basename $(notdir $@))))
PRJ_PROT_VER_V2                =$(lastword $(subst ., ,$(PRJ_PROT_VER_TYPE_V2)))
PRJ_PROT_V2                    =$(word 1,$(subst ., ,$(PRJ_PROT_VER_TYPE_V2)))
PRJ_UPGPK_CFG_FILE_V2          =$@.cfg
include make_upgzpk_opt.cfg
PRJ_UPGPK_FILE_V2              =$@
PRJ_UPGPK_FILE_Z_TMP_PATH   =$(PRJ_DELIVERY_IMAGE_PATH)/tmp
PRJ_UPGPK_Z_FILE            =$(PRJ_UPGPK_FILE_Z_TMP_PATH)/$(notdir $(PRJ_UPGPK_FILE_V2)).z
PRJ_UPGPK_ZPK_FILE          =$(PRJ_UPG_PATH)/$(notdir $(PRJ_UPGPK_FILE_V2)).zpk
################################################################################
# Make Targets Configuration
################################################################################
all : do_build_hpgpk_upg

################################################################################
# Make bootrom target
################################################################################
do_build_hpgpk_upg: do_upg_file
ifdef OB_ENV_VERBOSE_COMPILE
	@echo -
	@echo Build $(BUILD_NAME) $(OB_ENV_PRJ_LIB_TYPE) code by $(OBC_BUILDER_VER)
	@echo -
endif
################################################################################

################################################################################
do_upg_file_pre:
	@mdx $(PRJ_UPG_PATH)/usr
	@delx $(PRJ_UPG_PATH)/usr/*.zpk
ifndef OB_ENV_BUILD_CFG_UPGPK_PACKET_FILTER
	@echoerr please define OB_ENV_BUILD_CFG_UPGPK_PACKET_FILTER to speicfy the package filter.
endif
	@mdx $(PRJ_UPGPK_FILE_Z_TMP_PATH)
	@print compress upgpk, log file: $(PRJ_UPGPK_FILE_Z_TMP_PATH)/upgpk_compress.log
	@filewt $(PRJ_UPGPK_FILE_Z_TMP_PATH)/upgpk_compress.log compress upgpk ....

do_upg_file_finish:
ifdef OB_ENV_VERBOSE_COMPILE
	@ls $(PRJ_UPG_PATH)/*.zpk
endif
################################################################################

################################################################################
do_upg_file : do_upg_file_pre do_upg_file_target do_upg_file_finish
MAKE_MULTI_TARGETS :=do_upg_file_target
MAKE_MODULE_LIST   := $(PRJ_UPGPK_FILES_V2)
.MAKE_MODULE_LIST_TARGETS
ifdef OB_ENV_VERBOSE_COMPILE
	@print build UPGPK z pack by UPGPK file: $(notdir $@), path:$(basedir $@)...
else
	@print build UPGPK z pack, UPGPK file: $(notdir $@)...
endif
	@delx -q $(PRJ_UPGPK_Z_FILE)
	@delx -q $(PRJ_UPGPK_ZPK_FILE)
	@findx $(PRJ_UPGPK_FILE_V2) ***error: fail to find $(PRJ_UPGPK_FILE_V2) file.

ifdef OB_ENV_VERBOSE_COMPILE
	@print ZIP config file: $(call PRJ_ZIP_TOOL_UPGPK_CFG_FILE,mcore)
endif

ifeq ($(strip $(PRJ_ZIP_TOOL_UPGPK_MCORE)),)
	@echoerr ***error: fail to find valid configuration in file: $(call PRJ_ZIP_TOOL_UPGPK_CFG_FILE,mcore)
endif
	@cmdexec $(PRJ_UPGPK_FILE_Z_TMP_PATH)/upgpk_compress.log $(PRJ_ZIP_TOOL_UPGPK_MCORE) $(PRJ_UPGPK_FILE_V2) $(PRJ_UPGPK_Z_FILE)
	@findx $@ ***error: fail to generate UPGPK Z file: $(PRJ_UPGPK_Z_FILE)
	@$(PRJ_MAKE_FILE) $(PRJ_UPGPK_PATH)/makefile_zpk.mk PRJ_UPG_PATH="$(PRJ_UPG_PATH)" PRJ_UPGPK_Z_FILE="$(PRJ_UPGPK_Z_FILE)" PRJ_UPGPK_ZPK_FILE="$(PRJ_UPGPK_ZPK_FILE)" PRJ_UPGPK_MAKE_PACKET_OPTION="$(PRJ_UPGPK_MAKE_PACKET_OPTION)" PRJ_UPGPK_CFG_FILE="$(PRJ_UPGPK_CFG_FILE_V2)" PRJ_CHIP_NAME="$(PRJ_CHIP_NAME)"
	@print build UPGPK z packet ok, file: $(notdir $(PRJ_UPGPK_ZPK_FILE)), product type: $(PRJ_PROT_V2), ver: $(PRJ_PROT_VER_V2), product ver: $(OB_ENV_PRODUCT_VER_V2)
	@findx $(PRJ_UPGPK_ZPK_FILE) ***error: fail to generate UPGPK ZPK file:$(PRJ_UPGPK_ZPK_FILE), maybe development environment is bad.
	@cpyx -q $(PRJ_UPGPK_ZPK_FILE) $(PRJ_UPG_PATH)/usr/$(notdir $(PRJ_UPGPK_ZPK_FILE))
ifdef OB_ENV_VERBOSE_COMPILE
	@print UPGPK ZPK file is $(PRJ_UPGPK_ZPK_FILE)
	@print -
endif
################################################################################
