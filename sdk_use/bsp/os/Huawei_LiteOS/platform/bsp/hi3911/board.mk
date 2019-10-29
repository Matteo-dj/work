LITEOS_CPU_OPTS       := -mcpu=arm926ej-s
LITEOS_COPTS          += $(LITEOS_CPU_OPTS)
LITEOS_COPTS_NODEBUG  += $(LITEOS_CPU_OPTS)
LITEOS_ASOPTS         += $(LITEOS_CPU_OPTS)
LITEOS_CXXOPTS_BASE   += $(LITEOS_CPU_OPTS)

LITEOS_ARM_ARCH := -march=armv5te

PLATFORM_INCLUDE := -I $(LITEOSTOPDIR)/platform/bsp/hi3911 \
           -I $(LITEOSTOPDIR)/platform/cpu/arm/include \
           -I $(LITEOSTOPDIR)/platform/bsp/hi3911/config \
           -I $(LITEOSTOPDIR)/platform/bsp/hi3911/include \
           -I $(LITEOSTOPDIR)/platform/bsp/hi3911/mmu \
           -I $(LITEOSTOPDIR)/platform/bsp/common

LITEOS_SCATTERLIB += -larm926 -lhi3911

LITEOS_INCLUDE += $(PLATFORM_INCLUDE)

LITEOS_CXXINCLUDE += $(PLATFORM_INCLUDE)

LIB_SUBDIRS += platform/cpu/arm/arm926 platform/bsp/hi3911


LITEOS_GCCLIB :=armv5te_arm9_soft
