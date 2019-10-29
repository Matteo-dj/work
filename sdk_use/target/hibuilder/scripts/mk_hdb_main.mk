# A simple Makefile for lib(libxxx.a)
# By Late Lee(http://www.latelee.org)
include $(LITEOSTOPDIR)/config.mk

#目录路径 xml
HDB_DST_XML_ROOT_DIR := $(MAIN_OUT)/$(HB_PRODUCT_NAME_LIST)/hdb/hdbcfg/
#目标路径 hso资源
HDB_DST_HSO_RESOURCE_DIR :=  $(MAIN_OUT)/$(HB_PRODUCT_NAME_LIST)/hdb/hso/
#源路径: NV XML
HDB_NV_XML_SRC_DIR :=$(MAIN_OUT)/$(HB_PRODUCT_NAME_LIST)/nv
#源路径：其它xml
HDB_OTHER_XML_SRC_DIR := $(LITEOSTOPDIR)/target/config/hdbcfg

do_all: do_hdb_cp_xml do_hdb_cp_hso_resource do_hdb_creat_nv_txt do_hdb_creat_diag_txt do_hdb_create_prim_xml

do_hdb_cp_xml:
#@rm -rf $(HDB_DST_XML_ROOT_DIR)
	@mkdir -p $(HDB_DST_XML_ROOT_DIR)
ifeq ($(HDB_NV_XML_SRC_DIR)/mss_nvi_db.xml, $(wildcard $(HDB_NV_XML_SRC_DIR)/mss_nvi_db.xml))
	@cp -f $(HDB_NV_XML_SRC_DIR)/mss_nvi_db.xml $(HDB_DST_XML_ROOT_DIR)
endif
	@cp -f $(HDB_OTHER_XML_SRC_DIR)/*.xml $(HDB_DST_XML_ROOT_DIR)
	
do_hdb_cp_hso_resource:
	@mkdir -p $(HDB_DST_HSO_RESOURCE_DIR)
	cp -f $(LITEOSTOPDIR)/target/config/hso/base_datatype_def.txt $(HDB_DST_HSO_RESOURCE_DIR)
	
	
do_hdb_creat_nv_txt: 
	$(MAKE) -f $(LITEOSTOPDIR)/nv/scripts/make_nv_hdb.mk ROOT_DIR="../../../.."
	
do_hdb_creat_diag_txt:
	$(MAKE) -f $(LITEOSTOPDIR)/target/config/diag/script/make_diag_hdb.mk ROOT_DIR="../../../.."

include $(SCRIPT_PATH)/mk_prim_xml_step2.mk	

do_hdb_create_prim_xml:do_prim_xml_step2
	



.PHONY:do_hdb_main do_hdb_cp_xml do_hdb_cp_hso_resource do_hdb_creat_nv_txt do_hdb_creat_diag_txt do_hdb_create_prim_xml
	
