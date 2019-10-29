
################################################################################
# Main Makefile for HiBuilder 
#
# History
# Author                			Date          Version 
# chenghuanhuan/00132816   2016-05-19    V1.0  
################################################################################
include $(LITEOSTOPDIR)/config.mk

LDFLAGS  = -nostartfiles -static --gc-sections -L$(LIBS) -L$(ALL_LIBS) -L$(LIBS)/app -T$(LD_PATH)/hi_server.ld



target =  hi3911_$(HB_PRODUCT_NAME_LIST)
libs_target = libs_target
MODDIR = $(LITEOSTOPDIR)/target/hibuilder/scripts
HUPGDIR = $(MAIN_OUT)/mk_img/image
BINSRCDIR = $(MAIN_OUT)/mk_img/src/Xcore_bin
FILE=$(MAIN_OUT)/mk_img/pub/imagelog.txt
#IMAGELIST=`cat $(FILE)`
IMAGELIST = ` awk '{print $1}' $(FILE) `
prepare:
	@echo "****************************create __LIBS directory!********************************"
	@mkdir -p $(LIBS)
	@rm -rf $(TARGET)
	@mkdir -p $(TARGET) 

make_libs:
	$(MAKE) -C $(LITEOSTOPDIR)/target/hibuilder/ver_cfg all
	-@mkdir -p $(ALL_LIBS)
	if [ -d "$(LITEOSTOPDIR)/target/libs/sdk_bak/ALL" ]; \
	then \
	cp -rf  $(LITEOSTOPDIR)/target/libs/sdk_bak/ALL/*.a  $(ALL_LIBS) ;\
	fi
	
	if [ -d "$(LITEOSTOPDIR)/target/libs/sdk_bak/$(HB_PRODUCT_NAME_LIST)" ] ;\
	then \
	cp -rf  $(LITEOSTOPDIR)/target/libs/sdk_bak/$(HB_PRODUCT_NAME_LIST)/*.a $(MAIN_OUT)/$(HB_PRODUCT_NAME_LIST)/lib ;\
	fi

link:prepare make_libs
	$(foreach X,$(APP_LIST), \
        $(LD) $(LDFLAGS) -Map=$(TARGET)/$(target)_$(X).map -o $(TARGET)/$(target)_$(X).out --start-group $(patsubst %,-l%,$(subst lib, ,$(basename $(notdir $(wildcard $(LIBS)/*.a $(ALL_LIBS)/*.a ) )))) -l$(X) --end-group; \
        $(OBJCOPY) -O binary $(TARGET)/$(target)_$(subst l,,$X).out $(TARGET)/$(target)_$(X).bin;\
        $(OBJDUMP) -d $(TARGET)/$(target)_$(X).out >$(TARGET)/$(target)_$(X).asm;\
	python map_parser.py $(TARGET)/$(target)_$(X).map;\
    	)

link_check:
	@for app in $(APP_LIST);\
	do if [ -f $(TARGET)/$(target)_$$app.bin ] && [ -f $(TARGET)/$(target)_$$app.out ] && [ -f $(TARGET)/$(target)_$$app.map ]; \
	then \
	exit 0;\
	else \
	echo "$(TARGET)/$(target)_$$app.bin" ;\
	echo -n "HB BUILD $(MODDIR) $$dir [FAILURE]" ;\
	echo "`date "+%Y-%m-%d %H:%M:%S"`" ; \
	exit 1;\
	fi;\
	done  
	@echo -n "HB BUILD $(MODDIR) [SUCCESS]"
	@echo "`date "+%Y-%m-%d %H:%M:%S"`"

hupg:
	-@rm -rf $(MAIN_OUT)/mk_img
	$(foreach X,$(APP_LIST), \
	$(foreach Y,$(PRODUCT_LIST), \
	rm -rf $(BINSRCDIR)/hi3911_$Y_$X.bin;\
	rm -rf $(HUPGDIR)/*$Y*.hupg;\
	rm -rf $(HUPGDIR)/*$Y*.hbin;))
	python pkt_builder.py

hupg_check:
#在遍历前，增加对遍历文件所在的imagelog.txt是否已经生成进行校验. w00291248/20170804
	@if [ -f $(FILE) ]; \
	then \
	echo -n "imagelist already generated"; \
	else \
	echo -n "imagelist not exist"; \
	echo -n "HB BUILD sdk_compress [FAILURE]"; \
	exit 1;\
	fi;
	@for image in $(IMAGELIST);\
	do if [ -f $(HUPGDIR)/$$image ]; \
	then \
	echo $$image;\
	else \
	echo -n "HB BUILD $(MODDIR) $$dir [FAILURE]" ;\
	echo "`date "+%Y-%m-%d %H:%M:%S"`" ; \
	exit 1;\
	fi;\
	done
	@echo -n "HB BUILD $(MODDIR) [SUCCESS]" 
	@echo "`date "+%Y-%m-%d %H:%M:%S"`" 
	python filerename.py
clean:
	$(RM) $(MAIN_OUT)
	
.PHONY:prepare link clean hupg link_check hupg_check

