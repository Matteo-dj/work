# A simple Makefile for lib(libxxx.a)
# By Late Lee(http://www.latelee.org)

include $(LITEOSTOPDIR)/config.mk

ARFLAGS = cr
LIBOUT = $(BUILD)/drivers
LIB = $(OUT)/lib/libhispiflash.a

LIBDEP:= --start-group \
		-larm926 -lgcc -lgcc_eh -lhi3911 -llinuxadp -llitekernel -lposix -lsupc++ -lc -lm \
		--end-group

RM = -rm -rf

SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,$(LIBOUT)/%.o,$(SRCS))

all: $(LIBOUT) $(LIB)

$(OBJS): $(LIBOUT)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

$(LIBOUT):
	mkdir -p $(LIBOUT)/hispiflash

clean:
	@$(RM) $(OBJS) $(LIB) $(LIBOUT) *.bak *~

.PHONY: all clean
