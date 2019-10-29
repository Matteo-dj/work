
include $(LITEOSTOPDIR)/hso/makescript/v10_p/MakefileInc

MAKE_HDB_PATH=$(HSO_PATH)/makescript/v10_p/hsoconfig

all:
	make -f $(MAKE_HDB_PATH)/MakeHDB HB_DEBUG_COMPILE_ENABLE=$(HB_DEBUG_COMPILE_ENABLE) HB_PROJECT_NAME_LIST=$(HB_PROJECT_NAME_LIST)
	
	
check:
	make -f $(MAKE_HDB_PATH)/MakeHDB check
	
clean:
	make -f $(MAKE_HDB_PATH)/MakeHDB clean