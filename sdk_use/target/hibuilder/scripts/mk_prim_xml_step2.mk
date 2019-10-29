include $(SCRIPT_PATH)/mk_prim_xml.rules

PYTHON_ARGS_DO_PRIM_XML_STEP2 = "_PYTHON_ARG_" "PRIM_XML_TEMP_BASE_ROOT_DIR" $(PRIM_XML_TEMP_BASE_ROOT_DIR)\
	"_PYTHON_ARG_" "PRIM_XML_DST_XML_FILE" $(PRIM_XML_DST_XML_FILE)\
	"_PYTHON_ARG_" "PRIM_XML_SRC_XML_FILE" $(PRIM_XML_SRC_XML_FILE)\
	"_PYTHON_ARG_" "LOG_FILE_NAME" $(PRIM_XML_TEMP_ROOT_DIR)/LOG/BASE.log
		
do_prim_xml_step2:
#@echo do_prim_xml_step2 PYTHON_ARGS_DO_PRIM_XML_STEP2=$(PYTHON_ARGS_DO_PRIM_XML_STEP2)
	@python $(SCRIPT_PATH)/mk_prim_xml_step2.py $(PYTHON_ARGS_DO_PRIM_XML_STEP2)