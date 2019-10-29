################################################################################
# Makefile for OneBuilder 
#
# History
# Author                Date          Version 
# shenhankun/00130424   2011-06-23    V1.0  
################################################################################

-include $(PRJ_UPGPK_CFG_FILE)

ifneq ($(strip $(OBC_UPGPK_USER_VER)),)
#PRJ_DELIVERY_PRODUCT_VER_CMD=-ver $(OBC_UPGPK_USER_VER)
endif
ifneq ($(strip $(OBC_UPGPK_USER_PLAT)),)
PRJ_DELIVERY_PLAT_CMD= -plat $(OBC_UPGPK_USER_PLAT)
endif

ifeq ($(strip $(OBC_UPGPK_USER_TYPE)),HNV)
PRJ_UPGPK_MAKE_PACKET_OPTION_HNV = -type nv
endif

################################################################################
# Make Targets Configuration
################################################################################
all :
ifneq ($(strip $(OBC_UPGPK_USER_VER)),)
ifneq ($(strip $(OBC_UPGPK_USER_VER_TARGET)),)
	@filewt $(PRJ_UPG_PATH)/$(OBC_UPGPK_USER_VER_TARGET).z.ver.cfg $(OBC_UPGPK_USER_VER)
	@print user version: $(OBC_UPGPK_USER_VER)
endif
endif
	@packetfile -e $(PRJ_UPGPK_Z_FILE) -o $(PRJ_UPGPK_ZPK_FILE) $(PRJ_UPGPK_MAKE_PACKET_OPTION) $(PRJ_UPGPK_MAKE_PACKET_OPTION_HNV) $(PRJ_DELIVERY_PRODUCT_VER_CMD) $(PRJ_DELIVERY_PLAT_CMD)
################################################################################

