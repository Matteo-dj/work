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

include $(PRJ_HNV_TMP_MK_FILE)

PRJ_HNV_AUTH_PARAM_CFG_FILE_T     = $(PRJ_HNV_TMP_PATH)/$(PRJ_HNV_PARAM_AUTH_NV_ID)_nv_auth
PRJ_HNV_AUTH_PARAM_CFG_FILE_BIN   = $(PRJ_HNV_TMP_PATH)/$(PRJ_HNV_PARAM_AUTH_NV_ID)_nv_auth.bin

auth2hnv:
	@filewt $(PRJ_HNV_AUTH_PARAM_CFG_FILE_T).tmp #include "nv_interface.h"
	@fileatl $(PRJ_HNV_AUTH_PARAM_CFG_FILE_T).tmp $(PRJ_HNV_NV_PARAM_NAME)
	@fileatl $(PRJ_HNV_AUTH_PARAM_CFG_FILE_T).tmp $(PRJ_HNV_NV_PARAM)
	@findx $(PRJ_HNV_AUTH_PARAM_CFG_FILE_T).tmp ***error: fail to find $(PRJ_HNV_AUTH_PARAM_CFG_FILE_T).tmp
	@findx $(PRJ_HNV_FILE) ***error: fail to find $(PRJ_HNV_FILE)
	@delx -q $(PRJ_HNV_AUTH_PARAM_CFG_FILE_BIN)
	@st2bin -cfg $(PRJ_HNV_AUTH_PARAM_CFG_FILE_T).tmp -o $(PRJ_HNV_AUTH_PARAM_CFG_FILE_BIN) -opt $(PRJ_HNV_COMP_OPT_FILE)
	@findx $(PRJ_HNV_AUTH_PARAM_CFG_FILE_BIN) ***error: fail to create tmp file, maybe don't find nv_interface.h or its sub-include file have some errors.
	$(PRJ_CMDL) upghnv -file $(PRJ_HNV_FILE) -id $(PRJ_HNV_PARAM_AUTH_NV_ID) -bvalue $(PRJ_HNV_AUTH_PARAM_CFG_FILE_BIN)
