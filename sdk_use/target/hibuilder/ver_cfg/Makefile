# A simple Makefile for lib(libxxx.a)
# By Late Lee(http://www.latelee.org)

include $(LITEOSTOPDIR)/config.mk

MODDIR = target/hibuilder/ver_cfg/
LIBOUT = $(BUILD)/target/hibuilder/ver_cfg/
LIB = $(OUT)/lib/libhbver.a


SRCS = $(wildcard  ./*.c)
OBJS = $(patsubst %.c,$(LIBOUT)/%.o,$(SRCS))


all: clean $(LIBOUT)  $(LIB)

$(OBJS): $(LIBOUT)/%.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	
$(LIBOUT):
	mkdir -p $(LIBOUT)
	
$(LIB):$(OBJS)
	@$(AR) $(ARFLAGS) $@ $(OBJS) 

clean:
	@$(RM) $(OBJS) $(LIB)  *.bak *~

check:
ifeq ($(LIB),$(wildcard $(LIB)))
	@echo -n "HB BUILD $(MODDIR) [SUCCESS]" 
	@echo "`date "+%Y-%m-%d %H:%M:%S"`"
else
	@echo -n "HB BUILD $(MODDIR) [FAILURE]" 
	@echo "`date "+%Y-%m-%d %H:%M:%S"`"
endif

.PHONY: all clean check
