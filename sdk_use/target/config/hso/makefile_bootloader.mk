################################################################################
# Copyright (C) 2011 Huawei Tech. Co., Ltd.
# Makefile Rules for OneBuilder
# 
# Author : shenhankun/00130424, 2011-06-22
################################################################################


################################################################################
# Build  Configuration
# File and Directory Configuration
################################################################################
OBC_LOCAL_SRC_DIR_CFG             := cfg_makefile_src_dir.def
OBC_LOCAL_INC_DIR_CFG             := cfg_makefile_inc.def
OBC_CC_USER_MACRO_FILE            := cfg_makefile_cc_macro.def
OBC_AS_USER_MACRO_FILE            := 
OBC_LOCAL_SRC_FILE_CFG            := cfg_makefile_src_file.def
OBC_LOCAL_USER_OBJ_DIR_CFG        := 
OBC_LOCAL_USER_OBJ_FILE_CFG       := 
OBC_LOCAL_SEPERATE_OBJ_FILES_CFG  := 
CC_USER_DEFINES_CFG               := cfg_makefile_cc_macro.def
AS_USER_DEFINES_CFG               := 
OBC_ENV_MK_LIB_O                  := 
OB_MAKE_USER_MACRO_NAME           := PRODUCT_CFG_FEATURE_HAVE_APPS 
OB_MAKE_FEATURE_APPS              = YES 
################################################################################


################################################################################
# Compiler Flags to cumstom.
################################################################################
CC_USER_FLAGS                     := 
AS_USER_FLAGS                     := 

################################################################################


################################################################################
# Common Rules from OneBuilder
################################################################################
include ${OB_ENV_MAKE_CC_FILE}
################################################################################

