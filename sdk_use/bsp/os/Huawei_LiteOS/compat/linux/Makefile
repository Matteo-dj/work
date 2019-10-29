include $(LITEOSTOPDIR)/config.mk

ARFLAGS = cr
LIBOUT = $(BUILD)/compat/linux
LIB = $(OUT)/lib/liblinuxadp.a

RM = -rm -rf

LOCAL_INCLUDE := \
        -I $(LITEOSTOPDIR)/kernel/base/include \

SRCS = $(wildcard src/*.c)
COBJS = $(patsubst %.c,$(LIBOUT)/%.o,$(SRCS))

all: $(LIBOUT) $(LIB)

$(COBJS): $(LIBOUT)/%.o : %.c
	@$(CC) $(LITEOS_CFLAGS) $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS) -c $< -o $@

OBJS = $(COBJS)

$(OBJS): $(LIBOUT)

$(LIB): $(OBJS)
	@$(AR) $(ARFLAGS) $@ $(OBJS)

$(LIBOUT):
	mkdir -p $(LIBOUT)/src

clean:
	@$(RM) $(OBJS) $(LIB) $(LIBOUT) *.bak *~

.PHONY: all clean
