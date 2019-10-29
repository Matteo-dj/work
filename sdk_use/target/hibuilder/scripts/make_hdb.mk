
#########################################################################################################
#hdbÁ÷³Ì
#########################################################################################################
HDB_TEMP := $(wildcard $(HDB_TXT_SRC_DIR)/*_struct_def.txt)
HDB_TXT_SRC_FILE_LIST = $(notdir $(HDB_TEMP))
HSO_BASE_DATATYPE_TXT:=$(LITEOSTOPDIR)/target/config/hso/base_datatype_def.txt
do_hdb:
	@rm -rf $(HDB_TEMP_FILE_ROOT_DIR)
	@mkdir -p $(HDB_TEMP_FILE_ROOT_DIR)
	@rm -rf $(HDB_DST_FILE_ROOT_DIR)
	@mkdir -p $(HDB_DST_FILE_ROOT_DIR)

#@echo HDB_TEMP ============$(HDB_TEMP)
#@echo HDB_TEMP_FILE_ROOT_DIR ============$(HDB_TEMP_FILE_ROOT_DIR)
#@echo HDB_TXT_SRC_FILE_LIST ============$(HDB_TXT_SRC_FILE_LIST)
	
	@$(foreach x,$(HDB_TXT_SRC_FILE_LIST),\
		cp -f $(HDB_TXT_SRC_DIR)/$(x) $(HDB_TEMP_FILE_ROOT_DIR)/$(x).c;\
		$(CC) -E $(HDB_TEMP_FILE_ROOT_DIR)/$(x).c $(CFLAGS) -I $(LITEOSTOPDIR)/target/config/hso  -I $(LITEOSTOPDIR)/mdm/sys_drv/drivers/hiplcphy -I $(LITEOSTOPDIR)/mdm/inc  -I $(LITEOSTOPDIR)/mdm/mac/inc > $(HDB_TEMP_FILE_ROOT_DIR)/$(x);\
		rm -f $(HDB_TEMP_FILE_ROOT_DIR)/$(x).c;\
		)
	python $(SCRIPT_PATH)/hdb_txt_simplyfy.py $(HDB_TEMP_FILE_ROOT_DIR) $(HDB_DST_FILE_ROOT_DIR) $(HSO_BASE_DATATYPE_TXT) 
		
.PHONY:do_hdb