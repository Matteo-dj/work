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
OBC_BUILDER_VER     :=$(OB_ENV_ONEBUILDER) V$(OB_ENV_ONEBUILDER_VER)
ifeq ($(strip $(OB_ENV_PRJ_IMAGE_CORE)),)
OB_ENV_PRJ_IMAGE_CORE=OBC_IMAGE_MODEM
endif
PRJ_HAVE_OBC_INC_CFG=YES
include $(OB_ENV_TARGET_SCRIPT_PATH)/makefile/mk/makefile_cfg.mk
include $(OB_ENV_TARGET_SCRIPT_PATH)/makefile/mk/makefile_compiler.mk

PRJ_USR_PRODUCT_NAME             :=$(strlower $(OB_ENV_PRODUCT_NAME))
PRJ_HNV_TMP_PATH                 :=$(PRJ_HNV_FILE_Z_TMP_PATH)/$(PRJ_USR_PRODUCT_NAME)
PRJ_TARGET_CFG_PATH              :=$(OB_ENV_BUILD_TARGET_PATH)/config
PRJ_PRODUCT_NV_FILE_NAME         :=$(notdir $(subst .hnv,,$(subst nv_,,$(subst product_,,$(PRJ_HNV_FILE)))))
################################################################################

################################################################################
PRJ_HNV_PARAM_FOLDER_NAME       = hnv_upg_usr_param
PRJ_HNV_USRE_PARAM_PATH         = $(PRJ_TARGET_CFG_NV_PATH)/$(PRJ_HNV_PARAM_FOLDER_NAME)/$(PRJ_USR_PRODUCT_NAME)
PRJ_HNV_PARAM_CFG_FILE_LIST     := $(wildcard $(PRJ_HNV_USRE_PARAM_PATH)/0x*.npcfg)
ifeq ($(strip $(PRJ_HNV_PARAM_CFG_FILE_LIST)),)
PRJ_USR_PRODUCT_NAME            =custom
PRJ_HNV_USRE_PARAM_PATH         = $(PRJ_TARGET_CFG_NV_PATH)/$(PRJ_HNV_PARAM_FOLDER_NAME)/$(PRJ_USR_PRODUCT_NAME)
PRJ_HNV_PARAM_CFG_FILE_LIST     := $(wildcard $(PRJ_HNV_USRE_PARAM_PATH)/0x*.npcfg)
endif

ifdef OB_ENV_PRODUCT_CFG_MAKE_V2
## $(rdfile $(PRJ_TARGET_CFG_NV_PATH)/$(PRJ_HNV_PARAM_FOLDER_NAME)/nv_compile_opt_inc.cfg)
PRJ_HNV_COMP_OPT                := $(OBC_CC_OPT_S_CMP) -D__FILE_ID__=0 -D__FILE_MOD_ID__=0
PRJ_HNV_COMP_OPT_FILE           :=$(PRJ_HNV_FILE_Z_TMP_PATH)/nv_comp_opt.cfg

ifeq ($(strip $(OB_ENV_PRODUCT_CFG_HNV_CUSTOM_ONLY)),YES)
PRJ_USR_PRODUCT_NAME            =custom
PRJ_HNV_USRE_PARAM_PATH         = $(PRJ_TARGET_CFG_NV_PATH)/$(PRJ_HNV_PARAM_FOLDER_NAME)/$(PRJ_USR_PRODUCT_NAME)
PRJ_HNV_PARAM_CFG_FILE_LIST     := $(wildcard $(PRJ_HNV_USRE_PARAM_PATH)/0x*.npcfg)
endif

endif

PRJ_HNV_PARAM_DEF_FILE          =nv_usr_struct_def.txt
PRJ_HNV_PARAM_CFG_FILE_BIN      =$(PRJ_HNV_TMP_PATH)/$(notdir $@).bin
PRJ_HNV_PARAM_NV_ID             =$(subst nv_,,$(subst .usr.npcfg,,$(notdir $@)))
PRJ_MDM_HNV_PARAM_NV_ID         =$(subst $(PRJ_PRODUCT_NV_FILE_NAME),,$(subst nv_,,$(subst .usr.npcfg,,$(notdir $@))))
PRJ_HNV_USER_PARAM_CFG_FILE     =$(PRJ_TARGET_CFG_NV_PATH)/$(PRJ_HNV_PARAM_FOLDER_NAME)/$(PRJ_USR_PRODUCT_NAME)/$(PRJ_HNV_PARAM_NV_ID).usr.npcfg
PRJ_HNV_MDM_PARAM_CFG_FILE      =$(PRJ_TARGET_CFG_NV_PATH)/$(PRJ_HNV_PARAM_FOLDER_NAME)/$(PRJ_USR_PRODUCT_NAME)/mdm/$(PRJ_HNV_PARAM_NV_ID).mdm.npcfg
PRJ_HNV_PARAM_CFG_FILE          =$(PRJ_HNV_TMP_PATH)/$(PRJ_HNV_PARAM_NV_ID).npcfg
PRJ_HNV_PARAM_CFG_DEF_FILE_PATH =$(PRJ_TARGET_CFG_NV_PATH)
NV_FILE_CFG_PATH                =$(PRJ_HNV_TMP_PATH)
PRJ_NV_AUTH_FILE_KEY_CFG       := $(NV_FILE_CFG_PATH)/nv_auth.cfg

PRJ_NV_AUTH_CFG_FILE     :=$(PRJ_TARGET_CFG_PATH)/nv/nv_auth.cfg
ifeq ($(OB_ENV_PRODUCT_CFG_DIAG_AUTH_UI_INPUT),YES)
PRJ_NV_AUTH_PSW_FILE      :=$(PRJ_TARGET_CFG_PATH)/product/version.txt
else
PRJ_NV_AUTH_PSW_FILE      :=$(PRJ_TARGET_CFG_PATH)/nv/nv_auth.psw
endif
PRJ_NV_AUTH_FILE_USR_CFG :=$(PRJ_HNV_TMP_PATH)/nv_auth_usr.cfg
ifeq ($(OB_ENV_PRODUCT_CFG_DIAG_AUTH_UI_INPUT),YES)
PRJ_AUTH_PSW_FILE        :=$(PRJ_NV_AUTH_FILE_USR_CFG)
else
PRJ_AUTH_PSW_FILE        :=$(PRJ_NV_AUTH_PSW_FILE)
endif

PRJ_HNV_PROD_PARAM_CFG_FILE_LIST :=$(wildcard $(PRJ_HNV_USRE_PARAM_PATH)/$(PRJ_PRODUCT_NV_FILE_NAME).*.usr.npcfg)
ifneq ($(strip $(PRJ_HNV_PROD_PARAM_CFG_FILE_LIST)),)
PRJ_HNV_PROD_PARAM_CFG_FILE_LIST_NAME :=$(notdir $(PRJ_HNV_PROD_PARAM_CFG_FILE_LIST))
PRJ_HNV_PARAM_CFG_FILE_LIST_NAME      :=$(notdir $(PRJ_HNV_PARAM_CFG_FILE_LIST))
PRJ_HNV_PARAM_CFG_FILE_LIST_NAME      :=$(filter-out $(subst $(PRJ_PRODUCT_NV_FILE_NAME).,,$(PRJ_HNV_PROD_PARAM_CFG_FILE_LIST_NAME)), $(notdir $(PRJ_HNV_PARAM_CFG_FILE_LIST)))
PRJ_HNV_PARAM_CFG_FILE_LIST           :=$(PRJ_HNV_PROD_PARAM_CFG_FILE_LIST) $(foreach name,$(PRJ_HNV_PARAM_CFG_FILE_LIST_NAME),$(PRJ_HNV_USRE_PARAM_PATH)/$(name))
endif

ifndef OB_ENV_VERBOSE_COMPILE
PRJ_CMDL=cmdexeca $(PRJ_DELIVERY_TMP_PATH)/nv_usr.log
export PRJ_CMDL
endif
PRJ_USR_HNV_PARAM_NV_ID =$(subst $(PRJ_PRODUCT_NV_FILE_NAME).,,$(PRJ_HNV_PARAM_NV_ID))
################################################################################


################################################################################
ifdef OB_ENV_PRODUCT_CFG_MAKE_V2
all: do_prepare do_nv_auth_usr do_upg_file do_upg_file_finish
else
all: do_prepare do_upg_file do_upg_file_finish
endif

ifdef OB_ENV_PRODUCT_CFG_MAKE_V2

do_prepare :
	@print ---------upgrade nv config name: $(PRJ_PRODUCT_NV_FILE_NAME) ... --------
	@chkexe -q repparm ***error: fail to find tool(repparm.exe)	
	@chkexe -q upghnv ***error: fail to find tool(upghnv.exe)	
ifeq ($(strip $(PRJ_USR_PRODUCT_NAME)),)
	@echoerr ***error: fail to find valid product name is empty, maybe OneBuilder is bad or lost.
endif
	@print hnv user-defined param folder: $(PRJ_USR_PRODUCT_NAME), path: $(PRJ_HNV_USRE_PARAM_PATH)
	@mdx $(PRJ_HNV_FILE_Z_TMP_PATH)
	@mdx $(PRJ_HNV_TMP_PATH)
	@delx -q $(PRJ_HNV_TMP_PATH)/*.c
	@delx -q $(PRJ_HNV_TMP_PATH)/*.npcfg
	@delx -q $(PRJ_HNV_TMP_PATH)/*.bin
	@delx -q $(PRJ_HNV_TMP_PATH)/*.hnv
	@filewt $(PRJ_HNV_COMP_OPT_FILE) $(PRJ_HNV_COMP_OPT)
################################


################################
do_upg_file : do_upg_file_target
MAKE_MULTI_TARGETS :=do_upg_file_target
MAKE_MODULE_LIST   := $(PRJ_HNV_PARAM_CFG_FILE_LIST)
.MAKE_MODULE_LIST_TARGETS
	@print NV ID is $(PRJ_HNV_PARAM_NV_ID), $(PRJ_USR_HNV_PARAM_NV_ID), product nv config name $(PRJ_PRODUCT_NV_FILE_NAME)
	@mdx $(PRJ_HNV_TMP_PATH)
	@cpyx -q $(PRJ_HNV_FILE) $(PRJ_HNV_TMP_PATH)/$(notdir $(PRJ_HNV_FILE))
	@delx -q $(PRJ_HNV_PARAM_CFG_FILE)
	@delx -q $(PRJ_HNV_PARAM_CFG_FILE).c
ifdef OB_ENV_VERBOSE_COMPILE
	@print HNV_USER_PARAM_CFG_FILE: $(PRJ_HNV_USER_PARAM_CFG_FILE)
	@print HNV_MDM_PARAM_CFG_FILE: $(PRJ_HNV_MDM_PARAM_CFG_FILE)
endif

	@repparm -cfg $(PRJ_HNV_MDM_PARAM_CFG_FILE) -paramfile $(PRJ_HNV_USER_PARAM_CFG_FILE) -o $(PRJ_HNV_PARAM_CFG_FILE) -b $(PRJ_HNV_PARAM_CFG_FILE_BIN)
	@filewt $(PRJ_HNV_PARAM_CFG_FILE).tmp #include "nv_interface.h"
	@cmdexeca $(PRJ_HNV_PARAM_CFG_FILE).tmp cat $(PRJ_HNV_PARAM_CFG_FILE)
	@findx $(PRJ_HNV_PARAM_CFG_FILE) ***error: fail to find $(PRJ_HNV_PARAM_CFG_FILE)
	@delx -q $(PRJ_HNV_PARAM_CFG_FILE_BIN)
ifeq ($(strip $(OB_ENV_PRODUCT_CFG_HNV_CUSTOM_HAT_BNT_VER)),YES)
	$(PRJ_CMDL) hat_bnt_stp -deffile $(PRJ_HNV_PARAM_DEF_FILE) -defpath $(PRJ_HNV_PARAM_CFG_DEF_FILE_PATH) -paramcfgfile $(PRJ_HNV_PARAM_CFG_FILE) -o $(PRJ_HNV_PARAM_CFG_FILE_BIN)
else
	@sed -i -e "s/\[/\{/g" -e "s/\]/\}/g" $(PRJ_HNV_PARAM_CFG_FILE).tmp
	st2bin -cfg $(PRJ_HNV_PARAM_CFG_FILE).tmp -o $(PRJ_HNV_PARAM_CFG_FILE_BIN) -opt $(PRJ_HNV_COMP_OPT_FILE)
endif
	@findx $(PRJ_HNV_PARAM_CFG_FILE_BIN) ***error: fail to create tmp file, maybe don't find nv_interface.h or its sub-include file have some errors.
	$(PRJ_CMDL) upghnv -file $(PRJ_HNV_FILE) -id $(PRJ_USR_HNV_PARAM_NV_ID) -bvalue $(PRJ_HNV_PARAM_CFG_FILE_BIN)
################################

################################
do_nv_auth_usr:
ifeq ($(OB_ENV_PRODUCT_CFG_DIAG_AUTH_UI_INPUT),YES)
ifdef OB_ENV_VERBOSE_COMPILE
	@print AUTH PSW File: $(PRJ_NV_AUTH_PSW_FILE)
	@print AUTH CFG File: $(PRJ_NV_AUTH_FILE_USR_CFG)
endif
	@cmdexec $(PRJ_NV_AUTH_FILE_USR_CFG) sed -n "/NV_VER/p" $(PRJ_NV_AUTH_PSW_FILE)
	@findx $(PRJ_NV_AUTH_FILE_USR_CFG) ***error: fail to generate temporary file, maybe auth config file $(PRJ_NV_AUTH_PSW_FILE) is invalid.
	@sed -i -e "s/NV_VER//g" -e "s/ //g" -e "s/=//g" -e "s/^M//g" -e "s/\n//" $(PRJ_NV_AUTH_FILE_USR_CFG)
	@delx -q $(PRJ_NV_AUTH_FILE_USR_CFG).tt
	@cmdexec $(PRJ_NV_AUTH_FILE_USR_CFG).tt $(PERL) -p -e "s/\n//" $(PRJ_NV_AUTH_FILE_USR_CFG)
	@cpyx -q $(PRJ_NV_AUTH_FILE_USR_CFG).tt $(PRJ_NV_AUTH_FILE_USR_CFG)
	@findx $(PRJ_NV_AUTH_FILE_USR_CFG) ***error: fail to copy file $(PRJ_NV_AUTH_FILE_USR_CFG).tt
	@delx -q $(PRJ_NV_AUTH_FILE_USR_CFG).tt
ifdef OB_ENV_VERBOSE_COMPILE
	@cat $(PRJ_NV_AUTH_FILE_USR_CFG)
endif
endif

ifeq ($(wildcard $(strip $(PRJ_NV_AUTH_CFG_FILE))), $(strip $(PRJ_NV_AUTH_CFG_FILE)))
	@print build nv auth to hnv file from config file $(notdir $(PRJ_NV_AUTH_CFG_FILE))
	@$(MAKE) $(MAKE_OPTION) --makefile=$(PERL_SH_PATH)/makefile_nv_auth_cfg.mk PRJ_NV_BAT_BUILD_CFG_PATH=$(NV_FILE_CFG_PATH) PRJ_NV_AUTH_CFG_FILE=$(PRJ_NV_AUTH_CFG_FILE) PRJ_NV_AUTH_PSW_FILE=$(PRJ_NV_AUTH_PSW_FILE) PRJ_NV_DO_AUTH_CFG=YES
	@sed -i -e "s/\[/\{/g" -e "s/\]/\}/g" $(PRJ_NV_AUTH_FILE_KEY_CFG)
	@cmdexec $(PRJ_NV_AUTH_FILE_KEY_CFG).id sed -n "/ID /"p $(PRJ_NV_AUTH_FILE_KEY_CFG)	
	@cmdexec $(PRJ_NV_AUTH_FILE_KEY_CFG).param sed -n "/PARAM_VALUE /"p $(PRJ_NV_AUTH_FILE_KEY_CFG)
	@cmdexec $(PRJ_NV_AUTH_FILE_KEY_CFG).name sed -n "/PARAM_NAME /"p $(PRJ_NV_AUTH_FILE_KEY_CFG)	
	@$(MAKE) $(MAKE_OPTION) -f hupg_make_user_cfg_auth.mk PRJ_NV_AUTH_FILE_KEY_CFG=$(PRJ_NV_AUTH_FILE_KEY_CFG) PRJ_HNV_FILE=$(PRJ_HNV_FILE) PRJ_HNV_TMP_PATH=$(PRJ_HNV_TMP_PATH) PRJ_HNV_COMP_OPT_FILE=$(PRJ_HNV_COMP_OPT_FILE)
ifndef OB_ENV_VERBOSE_COMPILE	
	@delx -q $(PRJ_NV_AUTH_FILE_KEY_CFG).id
	@delx -q $(PRJ_NV_AUTH_FILE_KEY_CFG).param
	@delx -q $(PRJ_NV_AUTH_FILE_KEY_CFG).name
	@delx -q $(PRJ_NV_AUTH_FILE_KEY_CFG)
endif
endif

do_upg_file_finish:
	@delx -q $(PRJ_HNV_TMP_PATH)/*.tmp
	@delx -q $(PRJ_HNV_TMP_PATH)/*.hnv
	@print --------- upgrade nv config name: $(PRJ_PRODUCT_NV_FILE_NAME) complete -------
	@print 
################################################################################

else

do_prepare :
ifeq ($(strip $(PRJ_USR_PRODUCT_NAME)),)
	@echoerr ***error: fail to find valid product name is empty, maybe OneBuilder is bad or lost.
endif
	@print hnv user-defined param folder: $(PRJ_USR_PRODUCT_NAME), path: $(PRJ_HNV_USRE_PARAM_PATH)
	@mdx $(PRJ_HNV_TMP_PATH)
################################

do_upg_file :do_upg_file_target
MAKE_MULTI_TARGETS :=do_upg_file_target
MAKE_MODULE_LIST   := $(PRJ_HNV_PARAM_CFG_FILE_LIST)
.MAKE_MODULE_LIST_TARGETS
	@print NV ID is $(PRJ_HNV_PARAM_NV_ID)
	@cpyx -q $(PRJ_HNV_FILE) $(PRJ_HNV_TMP_PATH)/$(notdir $(PRJ_HNV_FILE))
	@delx -q $(PRJ_HNV_PARAM_CFG_FILE)
	repparm -cfg $(PRJ_HNV_MDM_PARAM_CFG_FILE) -paramfile $(PRJ_HNV_USER_PARAM_CFG_FILE) -o $(PRJ_HNV_PARAM_CFG_FILE)
	@findx $(PRJ_HNV_PARAM_CFG_FILE) ***error: fail to find $(PRJ_HNV_PARAM_CFG_FILE)
	hat_bnt_stp -deffile $(PRJ_HNV_PARAM_DEF_FILE) -defpath $(PRJ_HNV_PARAM_CFG_DEF_FILE_PATH) -paramcfgfile $(PRJ_HNV_PARAM_CFG_FILE) -o $(PRJ_HNV_PARAM_CFG_FILE_BIN)
	@findx $(PRJ_HNV_PARAM_CFG_FILE_BIN) ***error: fail to create temp file $(PRJ_HNV_PARAM_CFG_FILE_BIN)
	upghnv -file $(PRJ_HNV_FILE) -id $(PRJ_HNV_PARAM_NV_ID) -bvalue $(PRJ_HNV_PARAM_CFG_FILE_BIN)
################################

do_upg_file_finish:

################################################################################
endif 

