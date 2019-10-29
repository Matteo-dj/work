# A simple Makefile for lib(libxxx.a)
# By Late Lee(http://www.latelee.org)

#########################################################################################################
#1.cfg≈‰÷√≤ø∑÷
#########################################################################################################
include $(LITEOSTOPDIR)/config.mk
#temp file dir target\delivery\HiMDL_release\TEMP_DIR\cco\hdb\nv
HDB_TEMP_FILE_ROOT_DIR = $(MAIN_OUT)/TEMP_DIR/$(HB_PRODUCT_NAME_LIST)/hdb/nv/
#dst file dir  target\delivery\HiMDL_release\cco\hdb\nv
HDB_DST_FILE_ROOT_DIR =  $(MAIN_OUT)/$(HB_PRODUCT_NAME_LIST)/hdb/nv/
#src file dir target\config\nv
HDB_TXT_SRC_DIR = $(LITEOSTOPDIR)/nv/src/tmp_txt_header

#########################################################################################################
#1.do hdb
#########################################################################################################
include $(SCRIPT_PATH)/make_hdb.mk

