# A simple Makefile for lib(libxxx.a)
# By Late Lee(http://www.latelee.org)

#########################################################################################################
#1.cfg≈‰÷√≤ø∑÷
#########################################################################################################
include $(LITEOSTOPDIR)/config.mk
#temp file dir target\delivery\HiMDL_release\TEMP_DIR\cco\hdb\diag
HDB_TEMP_FILE_ROOT_DIR = $(MAIN_OUT)/TEMP_DIR/$(HB_PRODUCT_NAME_LIST)/hdb/diag/
#dst file dir  target\delivery\HiMDL_release\cco\hdb\diag
HDB_DST_FILE_ROOT_DIR =  $(MAIN_OUT)/$(HB_PRODUCT_NAME_LIST)/hdb/diag/

#src file dir target\config\diag
HDB_TXT_SRC_DIR = $(LITEOSTOPDIR)/target/config/diag

#########################################################################################################
#1.do hdb
#########################################################################################################
include $(SCRIPT_PATH)/make_hdb.mk

