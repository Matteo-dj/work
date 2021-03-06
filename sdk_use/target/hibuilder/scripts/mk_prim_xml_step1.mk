# A simple Makefile for lib(libxxx.a)
# By Late Lee(http://www.latelee.org)
include $(SCRIPT_PATH)/mk_prim_xml.rules

PYTHON_ARGS_DO_PRIM_XML_STEP1 = "_PYTHON_ARG_" "PRIM_XML_SRC_ROOT_DIR" $(PRIM_XML_SRC_ROOT_DIR)\
	"_PYTHON_ARG_" "PRIM_XML_SRC_SUB_DIR" $(PRIM_XML_SRC_SUB_DIR)\
	"_PYTHON_ARG_" "PRIM_XML_TEMP_ROOT_DIR" $(PRIM_XML_TEMP_ROOT_DIR)\
	"_PYTHON_ARG_" "CFLAGS" $(CFLAGS) -DMAKE_PRIM_XML_PROCESS_IN \
	"_PYTHON_ARG_" "CC" $(CC)\
	"_PYTHON_ARG_" "PRIM_XML_TEMP_BASE_ROOT_DIR" $(PRIM_XML_TEMP_BASE_ROOT_DIR)\
	"_PYTHON_ARG_" "PRIM_XML_KEY_WORD" $(PRIM_XML_KEY_WORD)\
	"_PYTHON_ARG_" "PRIM_XML_FILE_ID_FILE" $(PRIM_XML_FILE_ID_FILE)\
	"_PYTHON_ARG_" "PRIM_XML_MOD_ID_FILE" $(PRIM_XML_MOD_ID_FILE)\
	"_PYTHON_ARG_" "LOG_FILE_NAME" $(PRIM_XML_TEMP_ROOT_DIR)/LOG/$(PRIM_XML_KEY_WORD).log
		
do_prim_xml_step1:
#@echo do_prim_xml_step1 PYTHON_ARGS_DO_PRIM_XML_STEP1 $(PYTHON_ARGS_DO_PRIM_XML_STEP1)
	@python $(SCRIPT_PATH)/mk_prim_xml_step1.py $(PYTHON_ARGS_DO_PRIM_XML_STEP1)
#@echo %ERRORLEVEL%  $(haiwen) $?
	
