################################################################################
# Toplevel Makefile for OneBuilder 
#
# History
# Author                Date          Version 
# shenhankun/00130424   2011-06-23    V1.0  
################################################################################


################################################################################
# Environment Configuration
################################################################################
OBC_BUILDER_VER    :=$(OB_ENV_ONEBUILDER) V$(OB_ENV_ONEBUILDER_VER)
BUILD_NAME         :=$(strupper $(OB_ENV_SUBSYS_NAME))

.PHONY: all
.PHONY: do_all
.NOTPARALLEL:

ifeq ($(strip $(OB_ENV_PRODUCT_CFG_BUILD_HDB_AUTO_DELIVERY)),YES)
include $(OB_ENV_TARGET_SCRIPT_PATH)/makefile/mk/makefile_path_cfg.mk
include $(OB_ENV_TARGET_SCRIPT_PATH)/makefile/mk/makefile_cfg.mk

ifeq ($(OB_ENV_PRJ_IMAGE_CORE),OBC_IMAGE_MODEM)
all : do_init do_make_pre do_ld_post
else
ifeq ($(OB_ENV_PRJ_IMAGE_CORE),OBC_IMAGE_APP)
all : do_ld_post
else
ifeq ($(OB_ENV_PRJ_IMAGE_CORE),OBC_IMAGE_BOOTROM)
all : do_ld_post
else
ifeq ($(OB_ENV_PRJ_IMAGE_CORE),OBC_IMAGE_BOOTLOADER)
all : do_all
	@filewtl $(OB_ENV_PRJ_LIB_FILE).txt ok $(OB_ENV_PRODUCT_CFG_COMP_CHIP_PROD_TYPE_NAME)
else
all :
	@echo skip Link $(BUILD_NAME) for Configuration $(OB_ENV_PRJ_IMAGE_CORE) by $(OBC_BUILDER_VER).
endif
endif
endif
endif
################################################################################

include $(OB_ENV_TARGET_SCRIPT_PATH)/hso/makefile_cfg.mk

################################################################################
do_ld_post:
ifdef OB_ENV_VERBOSE_COMPILE
	@print ==== $(OB_ENV_PRODUCT_CFG_COMP_CHIP_PROD_TYPE_NAME) ====
endif
	@delx -q $(OB_ENV_PRJ_LIB_FILE).txt
	@$(OB_PRJ_MAKE) -C $(OB_ENV_BUILD_TARGET_PATH)/config/diag/script
	@$(OB_PRJ_MAKE) -C $(OB_ENV_BUILD_TARGET_PATH)/config/nv/script
	@filewtl $(OB_ENV_PRJ_LIB_FILE).txt ok $(OB_ENV_PRODUCT_CFG_COMP_CHIP_PROD_TYPE_NAME)
#	@filewt $(OB_ENV_PRJ_TARGET_FILE).txt dummy
else
.PHONY: all
.PHONY: do_all
.NOTPARALLEL:
ifeq ($(OB_ENV_PRJ_IMAGE_CORE),OBC_IMAGE_MODEM)
all : do_all
	@filewtl $(OB_ENV_PRJ_LIB_FILE).txt ok $(OB_ENV_PRODUCT_CFG_COMP_CHIP_PROD_TYPE_NAME)
else
ifeq ($(OB_ENV_PRJ_IMAGE_CORE),OBC_IMAGE_APP)
all :  
	@filewtl $(OB_ENV_PRJ_LIB_FILE).txt ok $(OB_ENV_PRODUCT_CFG_COMP_CHIP_PROD_TYPE_NAME)
else
ifeq ($(OB_ENV_PRJ_IMAGE_CORE),OBC_IMAGE_BOOTROM)
all :  
	@filewtl $(OB_ENV_PRJ_LIB_FILE).txt ok $(OB_ENV_PRODUCT_CFG_COMP_CHIP_PROD_TYPE_NAME)
else
ifeq ($(OB_ENV_PRJ_IMAGE_CORE),OBC_IMAGE_BOOTLOADER)
all : 
	@filewtl $(OB_ENV_PRJ_LIB_FILE).txt ok $(OB_ENV_PRODUCT_CFG_COMP_CHIP_PROD_TYPE_NAME)
else
all : 
	@echo skip Link $(BUILD_NAME) for Configuration $(OB_ENV_PRJ_IMAGE_CORE) by $(OBC_BUILDER_VER).
endif 
endif
endif
endif
endif # OB_ENV_PRODUCT_CFG_BUILD_HDB_AUTO_DELIVERY

################################################################################
# Environment Configuration
################################################################################
export PRJ_MK_SCRIPTS_DIR  :=$(basedir $(CURDIR))
_PATH_FILE_LIST_CONFIG     :=$(addprefix $(CURDIR)/,$(rdfile make_modules.cfg))
define PRJ_SUBDIR
$(subst _FN_,,$@)
endef
define PRJ_MAKEFILE_FILE
$(PRJ_SUBDIR)/makefile
endef

ifdef OB_ENV_PRODUCT_CFG_BUILD_HSO_PACKAGE
OB_ENV_PRODUCT_CFG_CUSTOM_BUILD_HSO_PACKAGE=$(OB_ENV_PRODUCT_CFG_BUILD_HSO_PACKAGE)
endif
################################################################################

################################################################################
# Make modem target
################################################################################
#do_all : do_prepare_MAKIE_FILES_LIST_TARGET do_final
do_all : do_prepare do_final
ifneq ($(strip $(_PATH_FILE_LIST_CONFIG)), )
.MAKE_FILE_LIST_MAKE_FEATURES
	@delx $(OB_ENV_BUILD_CHECK_OK_FILE)
	@print Make HSO image package, configuration path: $(PRJ_SUBDIR)
	@findx $(PRJ_MAKEFILE_FILE) ***error: fail to makefile in $(PRJ_SUBDIR), please check configuration.
	@findx $(PRJ_SUBDIR) ***error: fail to find folder: $(PRJ_SUBDIR), please check configuration.
	@chdirx $(PRJ_SUBDIR)
	@delx -q $(OB_ENV_PRJ_LIB_FILE).txt
	@$(MAKE) $(MAKE_OPTION) --no-print $(OB_ENV_MAKE_OPTION_VERBOSE) MAKE_SETUP_VER_CFG_PATH=$(PRJ_SUBDIR)
	@delx -q $(OB_ENV_PRJ_LIB_FILE).txt
	@print Make HSO image package for $(PRJ_SUBDIR) complete.
endif

do_prepare:
	@print ====================Component: $(BUILD_NAME) - HSO $(OB_ENV_PRJ_LIB_TYPE)====================
	@echodt
	@delx -q $(OB_ENV_PRJ_LIB_FILE).txt
	@print Build Prim LOG message.
	@$(MAKE) --no-print -C $(OB_ENV_BUILD_TARGET_PATH)/config/prim
	@delx -q $(OB_ENV_PRJ_LIB_FILE).txt
	@print Build Prim LOG message complete.
	@print -
################################################################################

################################################################################
do_init: obc_do_init
include $(OB_ENV_TARGET_SCRIPT_PATH)/makefile/mk/makefile_cfg_init.mk
################################################################################

################################################################################
do_final:
	@filewt $(OB_ENV_BUILD_CHECK_OK_FILE) =======================
	@echodt
ifeq ($(strip $(OB_ENV_PRODUCT_CFG_CUSTOM_BUILD_HSO_PACKAGE)),NO)	
	@print -------------- Build HSO complete(skip by user) --------------
else
	@print -------------- Build HSO complete --------------
endif
	@echodt
	@print ====================Component: $(BUILD_NAME) - HSO $(OB_ENV_PRJ_LIB_TYPE) OK====================
	@print -
################################################################################
