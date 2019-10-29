################################################################################
# Makefile for OneBuilder 
#
# History
# Author                Date          Version 
# shenhankun/00130424   2011-06-23    V1.0  
################################################################################

################################################################################
# Target
################################################################################
include $(OB_ENV_TARGET_SCRIPT_PATH)/makefile/mk/makefile_cfg.mk

PRJ_HNV_PARAM_AUTH_NV_ID := $(strip $(subst =,,$(subst ID,,$(rdfile $(PRJ_NV_AUTH_FILE_KEY_CFG).id))))
PRJ_HNV_NV_PARAM         := $(strip $(subst =,,$(subst PARAM_VALUE,,$(rdfile $(PRJ_NV_AUTH_FILE_KEY_CFG).param))))
PRJ_HNV_NV_PARAM_NAME    := $(strip $(subst =,,$(subst PARAM_NAME,,$(rdfile $(PRJ_NV_AUTH_FILE_KEY_CFG).name))))
PRJ_HNV_TMP_MK_FILE      := $(PRJ_HNV_TMP_PATH)/nv_user_auth.mk.t

auth:
ifdef OB_ENV_VERBOSE_COMPILE
	@print NV_ID         =$(PRJ_HNV_PARAM_AUTH_NV_ID)
	@print NV_PARAM      =$(PRJ_HNV_NV_PARAM)
	@print NV_PARAM_NAME =$(PRJ_HNV_NV_PARAM_NAME)
endif
	@$(PERL) $(PERL_SH_PATH)/chkword.pl $(PRJ_HNV_NV_PARAM_NAME)
	@$(PERL) $(PERL_SH_PATH)/chkword.pl $(PRJ_HNV_TMP_MK_FILE)

	@delx -q $(PRJ_HNV_TMP_MK_FILE)
	@filewt $(PRJ_HNV_TMP_MK_FILE) PRJ_HNV_TMP_PATH=$(PRJ_HNV_TMP_PATH)
	@fileat $(PRJ_HNV_TMP_MK_FILE) PRJ_HNV_FILE=$(PRJ_HNV_FILE)
	@filewt $(PRJ_HNV_TMP_MK_FILE) PRJ_NV_AUTH_FILE_KEY_CFG=$(PRJ_NV_AUTH_FILE_KEY_CFG)
	@fileat $(PRJ_HNV_TMP_MK_FILE) PRJ_HNV_PARAM_AUTH_NV_ID=$(PRJ_HNV_PARAM_AUTH_NV_ID)
	@fileat $(PRJ_HNV_TMP_MK_FILE) PRJ_HNV_AUTH_PARAM_CFG_FILE=$(PRJ_HNV_AUTH_PARAM_CFG_FILE)
	@fileat $(PRJ_HNV_TMP_MK_FILE) PRJ_HNV_NV_PARAM=$(PRJ_HNV_NV_PARAM)
	@fileat $(PRJ_HNV_TMP_MK_FILE) PRJ_HNV_NV_PARAM_NAME=$(PRJ_HNV_NV_PARAM_NAME)
	@fileat $(PRJ_HNV_TMP_MK_FILE) PRJ_HNV_COMP_OPT_FILE=$(PRJ_HNV_COMP_OPT_FILE)
ifdef OB_ENV_VERBOSE_COMPILE
	@print mk file is $(PRJ_HNV_TMP_MK_FILE)
endif
	@$(MAKE) $(MAKE_OPTION) -f hupg_make_user_cfg_auth2hnv.mk PRJ_HNV_TMP_MK_FILE=$(PRJ_HNV_TMP_MK_FILE)
ifndef OB_ENV_VERBOSE_COMPILE
	@delx -q $(PRJ_HNV_TMP_MK_FILE)
endif
