include $(LITEOSTOPDIR)/config.mk
MODDIR  = nv

#生成文件(区分产品形态)存放目录
PRODUCT_LIBOUT=$(MAIN_OUT)/$(HB_PRODUCT_NAME_LIST)/nv

#生成文件(不区分产品形态)存放目录
#LIBOUT

#解析用生成头文件存放目录
XML_INCLUDE_FILE_DIR =  $(MAIN_OUT)/$(HB_PRODUCT_NAME_LIST)/hdb/nv/

comma =,
empty = 
APP_NAME = $(strip $(subst $(comma), $(empty), $(HB_APP_COMPILE_LIST)))
CFG_FILES = $(wildcard $(LITEOSTOPDIR)/app/$(APP_NAME)/nv/cfg/$(HB_PRODUCT_NAME_LIST)/*.cfg)

HNV_FILES = $(wildcard $(MAIN_OUT)/$(HB_PRODUCT_NAME_LIST)/nv/hnv/*.hnv)
.PHONY:all clean

all: clean
ifeq ($(APP_NAME) , $(empty))
	@echo -e -n "not select app, can not compile nv\n" 
else
	$(MAKE) -f $(LITEOSTOPDIR)/nv/scripts/make_nv_hdb.mk ROOT_DIR="../.." 
	python $(SCRIPT_PATH)/build_nv.py  $(LITEOSTOPDIR)  $(PRODUCT_LIBOUT) $(XML_INCLUDE_FILE_DIR)  $(HB_PRODUCT_NAME_LIST) $(HB_APP_COMPILE_LIST) $(HI_DIAG_AUTH_PASSWORD) $(MAIN_OUT)/$(HB_PRODUCT_NAME_LIST)/nv/hnv    
endif

check:
ifeq ($(APP_NAME) , $(empty))
	@echo -e -n "\033[41;37m HB BUILD $(MODDIR) [FAILURE] \033[0m" 
	@echo -e "\033[41;37m `date "+%Y-%m-%d %H:%M:%S"` \033[0m"
	exit 1
endif
ifeq ($(words $(CFG_FILES)) , $(words $(wildcard $(MAIN_OUT)/$(HB_PRODUCT_NAME_LIST)/nv/hnv/*.hnv)))
	@echo -e -n "\033[42;37m HB BUILD $(MODDIR) [SUCCESS] \033[0m" 
	@echo -e "\033[42;37m `date "+%Y-%m-%d %H:%M:%S"` \033[0m"
else
	@echo -e -n "\033[41;37m HB BUILD $(MODDIR) [FAILURE] \033[0m" 
	@echo -e "\033[41;37m `date "+%Y-%m-%d %H:%M:%S"` \033[0m"
endif
#@echo CFG_FILES=$(CFG_FILES)
#@echo CFG_FILES=$(HNV_FILES)
#@echo -e -n "\033[42;37m HB BUILD $(MODDIR) [SUCCESS] \033[0m" 
#@echo -e "\033[42;37m `date "+%Y-%m-%d %H:%M:%S"` \033[0m"

clean:
	-rm -rf $(PRODUCT_LIBOUT)
