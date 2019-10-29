include $(LITEOSTOPDIR)/config.mk

ARFLAGS = cr
LIBOUT = $(BUILD)/net/lwip
LIB = $(OUT)/lib/liblwip.a
DIR = $(LIBOUT)/src/arch $(LIBOUT)/src/core/snmp $(LIBOUT)/src/netif $(LIBOUT)/src/api $(LIBOUT)/src/core/ipv4

LITEOS_PLATFORM_CPU = $(subst /,_,$(LITEOS_PLATFORM))

ifeq ($(strip $(LITEOS_PLATFORM)), hi3516a)
	LIBTCP = src/core/liblwip_tcp_hi3516a.a
else ifeq ($(strip $(LITEOS_PLATFORM)), hi3518ev200)
	LIBTCP = src/core/liblwip_tcp_hi3518ev200.a
else ifeq ($(strip $(LITEOS_PLATFORM)), hi3911)
	LIBTCP = src/core/liblwip_tcp_hi3911.a
else ifeq ($(strip $(LITEOS_PLATFORM)), hi3731)
	LIBTCP = src/core/liblwip_tcp_hi3518ev200.a
else ifeq ($(strip $(LITEOS_PLATFORM)), him5v100)
	LIBTCP = src/core/liblwip_tcp_hi3518ev200.a
else ifeq ($(strip $(LITEOS_PLATFORM)), hi3516cv300)
	LIBTCP = src/core/liblwip_tcp_hi3518ev200.a
else ifeq ($(strip $(LITEOS_PLATFORM)), hi3559/cortex-a7)
	LIBTCP = src/core/liblwip_tcp_hi3519_cortex-a7.a
else ifeq ($(strip $(LITEOS_PLATFORM)), hi3559/cortex-a17)
	LIBTCP = src/core/liblwip_tcp_hi3519_cortex-a17.a
else ifeq ($(strip $(LITEOS_PLATFORM)), hi3519v101/cortex-a7)
	LIBTCP = src/core/liblwip_tcp_hi3519_cortex-a7.a
else ifeq ($(strip $(LITEOS_PLATFORM)), hi3519v101/cortex-a17)
	LIBTCP = src/core/liblwip_tcp_hi3519_cortex-a17.a
else ifeq ($(strip $(LITEOS_PLATFORM)), hi3519/cortex-a7)
	LIBTCP = src/core/liblwip_tcp_hi3519_cortex-a7.a
else ifeq ($(strip $(LITEOS_PLATFORM)), hi3519/cortex-a17)
	LIBTCP = src/core/liblwip_tcp_hi3519_cortex-a17.a
else
    LIBTCP = src/core/liblwip_tcp_$(LITEOS_CPU_TYPE).a
endif

RM = -rm -rf

LOCAL_INCLUDE := \
        -I $(LITEOSTOPDIR)/kernel/base/include \
		-I $(LITEOSTOPDIR)/net/lwip_sack/src/int
ENABLE_LWIP_SACKLIB=y

SRCS = $(wildcard src/arch/*.c) $(wildcard src/api/*.c) $(wildcard src/core/ipv4/*.c) \
			$(wildcard src/core/snmp/*.c) $(wildcard src/netif/*.c)	\
			$(wildcard src/core/def.c src/core/dhcps.c src/core/init.c src/core/mem.c src/core/netif.c src/core/raw.c src/core/stats.c src/core/udp.c) \
			$(wildcard src/core/dhcp.c src/core/dns.c src/core/memp.c src/core/pbuf.c src/core/sntp.c src/core/sys.c src/core/timers.c)
ifeq ($(strip $(LOSCFG_NET_LWIP_SACK_TFTP)),y)
   SRCS += $(wildcard src/core/tftpc.c)
endif
OBJS = $(patsubst %.c,$(LIBOUT)/%.o,$(SRCS))

ifeq ($(strip $(ENABLE_LWIP_SACKLIB)), y)
#LITEOS_LWIP_CFLAGS  := -DLWIP_SACK=1 -DLWIP_WND_SCALE=0
TCPSRCS = src/core/tcp.c src/core/tcp_in.c src/core/tcp_out.c src/core/tcp_sack.c src/core/tcp_ws.c
TCPOBJS = $(patsubst %.c,$(LIBOUT)/%.o,$(TCPSRCS))
$(TCPOBJS): $(LIBOUT)/%.o : %.c
	@$(CC) $(LITEOS_CFLAGS) $(LITEOS_LWIP_CFLAGS) $(LOCAL_INCLUDE) -c $< -o $@
#	$(AR) $(ARFLAGS) $(LIBTCP) $(TCPOBJS)
tcplib:	$(TCPOBJS)
	@$(AR) $(ARFLAGS) $(LIBTCP) $(TCPOBJS)

else ifeq ($(strip $(ENABLE_LWIP_SACKLIB)), n)
LITEOS_LWIP_CFLAGS  := -DLWIP_SACK=0 -DLWIP_WND_SCALE=0
TCPSRCS = src/core/tcp.c src/core/tcp_in.c src/core/tcp_out.c
TCPOBJS = $(patsubst %.c,$(LIBOUT)/%.o,$(TCPSRCS))
$(TCPOBJS): $(LIBOUT)/%.o : %.c
	@$(CC) $(LITEOS_CFLAGS) $(LITEOS_LWIP_CFLAGS) $(LOCAL_INCLUDE) -c $< -o $@
.PHONY: tcplib
tcplib:	$(TCPOBJS)

else
LITEOS_LWIP_CFLAGS  := -DLWIP_SACK=1 -DLWIP_WND_SCALE=0
TCPSRCS = src/core/tcp.c src/core/tcp_in.c src/core/tcp_out.c src/core/tcp_sack.c src/core/tcp_ws.c
TCPOBJS = $(patsubst %.c,$(LIBOUT)/%.o,$(TCPSRCS))
.PHONY: TCPOBJS_AR
TCPOBJS_AR: $(DIR) $(LIBTCP)
	$(AR) -x -o $(LIBTCP)
	mv  *.o $(LIBOUT)/src/core/
$(TCPOBJS): TCPOBJS_AR
.PHONY: tcplib
tcplib:	$(TCPOBJS)
endif

all: $(DIR) $(LIB) #tcplib

$(OBJS): $(LIBOUT)/%.o : %.c
	@$(CC) $(LITEOS_CFLAGS) $(LITEOS_LWIP_CFLAGS) $(LOCAL_INCLUDE) -c $< -o $@

$(LIB):  $(TCPOBJS) $(OBJS)
	@$(AR) $(ARFLAGS) $@ $(OBJS) $(TCPOBJS)

$(DIR):
	@mkdir -p $@
	
clean:
	@$(RM) $(LIBOUT)  $(LIB)  $(OBJS) $(TCPOBJS) *.bak *.o *~

.PHONY: all clean
