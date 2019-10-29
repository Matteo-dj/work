
################################################################################
# Main Makefile for HiBuilder 
#
# History
# Author                			Date          Version 
# chenghuanhuan/00132816   2016-05-19    V1.0  
################################################################################
export LITEOSTOPDIR := $(shell if ["$$PWD" != ""]; then echo $$PWD; else pwd; fi)
include config.mk

#app_check := app_check

MAKE = make
.PHONY: all mrs rmt csgmrs boot liteos osa  drv sal cmn init boardcfg phy mac upg nv link hupg mrs_clean rmt_clean csgmrs_clean boot_clean liteos_clean hal_clean platform_clean  \
drv_clean sal_clean cmn_clean init_clean boardcfg_clean phy_clean mac_clean upg_clean nv_clean link_clean \
mrs_check rmt_check csgmrs_check boot_check liteos_check hal_check platform_check  drv_check sal_check cmn_check init_check boardcfg_check phy_check mac_check upg_check nv_check link_check hupg_check
#红色打印
#all: mrs csgmrs upg
all: mrs rmt csgmrs boot osa drv sal cmn init boardcfg phy mac upg nv link
#platform
all_clean:mrs_clean rmt_clean csgmrs_clean boot_clean liteos_clean osa_clean drv_clean sal_clean cmn_clean init_clean boardcfg_clean phy_clean mac_clean upg_clean nv_clean link_clean clean_sdk
all_check:mrs_check rmt_check csgmrs_check boot_check liteos_check osa_check platform_check  drv_check sal_check cmn_check init_check boardcfg_check phy_check mac_check upg_check nv_check link_check hupg_check 

mrs: 
	$(MAKE) -C app\mrs all
    
mrs_clean:
	$(MAKE) -C app\mrs clean

mrs_check:
	$(MAKE) -C app\mrs check
	
rmt: 
	$(MAKE) -C app\rmt all
    
rmt_clean:
	$(MAKE) -C app\rmt clean

rmt_check:
	$(MAKE) -C app\rmt check
	
csgmrs: 
	$(MAKE) -C app\csgmrs all
    
csgmrs_clean:
	$(MAKE) -C app\csgmrs clean

csgmrs_check:
	$(MAKE) -C app\csgmrs check

boot:
	$(MAKE) -C boot all
boot_clean:
	$(MAKE) -C boot clean
boot_check:
	$(MAKE) -C boot check

liteos:
	$(MAKE) -C bsp/os all

liteos_clean:
	$(MAKE) -C bsp/os clean

liteos_check:
	$(MAKE) -C bsp/os check

osa:
	$(MAKE) -C bsp\osa all

osa_clean:
	$(MAKE) -C bsp\osa clean

osa_check:
	$(MAKE) -C bsp\osa check


drv:
	$(MAKE) -C bsp\drivers all

drv_clean:
	$(MAKE) -C bsp\drivers clean

drv_check:
	$(MAKE) -C bsp\drivers check

sal:
	$(MAKE) -C bsp\sal all

sal_clean:
	$(MAKE) -C bsp\sal clean

sal_check:
	$(MAKE) -C bsp\sal check

cmn:
	$(MAKE) -C bsp\cmn all

cmn_clean:
	$(MAKE) -C bsp\cmn clean

cmn_check:
	$(MAKE) -C bsp\cmn check

init:
	$(MAKE) -C bsp\init all

init_clean:
	$(MAKE) -C bsp\init clean

init_check:
	$(MAKE) -C bsp\init check

boardcfg:
	$(MAKE) -C bsp\boardcfg all

boardcfg_clean:
	$(MAKE) -C bsp\boardcfg clean

boardcfg_check:
	$(MAKE) -C bsp\boardcfg check	
	
phy:
	$(MAKE) -C mdm\phy all

phy_clean:
	$(MAKE) -C mdm\phy clean

phy_check:
	$(MAKE) -C mdm\phy check
	
mac:
	$(MAKE) -C mdm\mac all

mac_clean:
	$(MAKE) -C mdm\mac clean

mac_check:
	$(MAKE) -C mdm\mac check	

upg:
	$(MAKE) -C mdm\upg all

upg_clean:
	$(MAKE) -C mdm\upg clean

upg_check:
	$(MAKE) -C mdm\upg check
    
nv:
	$(MAKE) -C nv\scripts all 

nv_clean:
	$(MAKE) -C nv\scripts clean   
    
nv_check:
	$(MAKE) -C nv\scripts check  
	
link:
	$(MAKE) -C target\hibuilder\scripts link

link_clean:
	$(MAKE) -C target\hibuilder\scripts clean

link_check:
	$(MAKE) -C target\hibuilder\scripts link_check
	
hupg:
	$(MAKE) -C target\hibuilder\scripts hupg
	
hupg_check:
	$(MAKE) -C target\hibuilder\scripts hupg_check
	
	
make_sdk:
	$(MAKE) -C target\hibuilder\scripts make_sdk
compile_sdk:
	$(MAKE) -C target\hibuilder\scripts compile_sdk
clean_sdk:
	$(MAKE) -C target\hibuilder\scripts clean_sdk


