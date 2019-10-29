include $(LITEOSTOPDIR)/config.mk

ARFLAGS = cr
LIBOUT = $(BUILD)/platform/bsp/hi3911
LIB = $(OUT)/lib/libhi3911.a

RM = -rm -rf

NET_SRC = net
EXTCMD_SRC = extcmd
SRCS = $(wildcard *.c) $(wildcard config/*.c) $(wildcard mmu/*.c)

ifeq ($(LOSCFG_DRIVERS_NETDEV), y)
SRCS += $(wildcard $(NET_SRC)/*.c)
endif
ifeq ($(LOSCFG_PLATFORM_ADAPT), y)
SRCS += $(wildcard os_adapt/*.c)
endif

ifeq ($(LOSCFG_SHELL), y)
SRCS += $(wildcard $(EXTCMD_SRC)/*.c)
endif

COBJS = $(patsubst %.c,$(LIBOUT)/%.o,$(SRCS))


ALL_ASSRCS = $(wildcard *.S)
ASSRCS = $(subst board.ld.S,,$(ALL_ASSRCS))
ASLIBOUTSRCS = $(patsubst %.S,$(LIBOUT)/%.s,$(ASSRCS))
ASOBJS = $(patsubst %.S,$(LIBOUT)/%.o,$(ASSRCS))


all: $(LIBOUT) $(LIB) $(COBJS) $(ASOBJS)

$(COBJS): $(LIBOUT)/%.o : %.c
ifeq ($(LOSCFG_THUMB), y)
	@$(CC) $(LITEOS_CFLAGS_NODEBUG) -c $< -o $@
else
	@$(CC) $(LITEOS_CFLAGS) -c $< -o $@
endif

$(ASLIBOUTSRCS): $(LIBOUT)/%.s : %.S
	@$(CC) -E $(LITEOS_CFLAGS) $< -o $@

$(ASOBJS): $(LIBOUT)/%.o : $(LIBOUT)/%.s
	@$(AS) $(LITEOS_ASFLAGS) -c $< -o $@

OBJS = $(COBJS) $(ASOBJS)

$(OBJS): $(LIBOUT)

$(LIB): $(OBJS)
	@$(AR) $(ARFLAGS) $@ $(OBJS)

$(LIBOUT):
	mkdir -p $(LIBOUT)
	mkdir -p $(LIBOUT)/config
	mkdir -p $(LIBOUT)/mmu
ifeq ($(LOSCFG_SHELL), y)
	mkdir -p $(LIBOUT)/extcmd
endif
ifeq ($(LOSCFG_DRIVERS_NETDEV), y)
	mkdir -p $(LIBOUT)/$(NET_SRC)
endif
ifeq ($(LOSCFG_PLATFORM_ADAPT), y)
	mkdir -p $(LIBOUT)/os_adapt
endif

clean:
	@$(RM) $(OBJS) $(LIB) $(LIBOUT) *.bak *~

.PHONY: all clean
