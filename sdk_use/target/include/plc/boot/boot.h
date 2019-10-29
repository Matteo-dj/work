#ifndef _BOOT_H_
#define _BOOT_H_

#include <types.h>
#include <product.h>
#include <led.h>


#ifndef PRODUCT_CFG_BUILD_DATE
#define PRODUCT_CFG_BUILD_DATE "2010-10-01"
#endif
#ifndef PRODUCT_CFG_BUILD_TIME
#define PRODUCT_CFG_BUILD_TIME "10:00:01"
#endif

#define ALIGN4          (0x4)
#define BOOT_SUCCESS    (0)
#define BOOT_FALURE     (-1)

/*malloc 测试代码开启宏*/
//#define CMD_TEST_MALLOC

//#define TEST_BOOT_TIME //性能测试启动时间宏
//#define TEST_BOOT_CLOSE_ELEC//断电测试

/* display debug info */
//#define DISPLAY_DEBUG_INFO

#define STRING_RIGHT "right"
#define STRING_WRONG "wrong"
  
static inline void DWB(void) /* drain write buffer */
{
    __asm__ __volatile__ (
        "mcr p15, 0, %0, c7, c10, 4\n" : : "r" (0)
        );
}

static inline void writel(unsigned val, unsigned addr)
{
    DWB();
    (*(volatile unsigned *) (addr)) = (val);
    DWB();
}

static inline void writew(unsigned val, unsigned addr)
{
    DWB();
    (*(volatile unsigned short *) (addr)) = (val);
    DWB();
}

static inline void writeb(unsigned val, unsigned addr)
{
    DWB();
    (*(volatile unsigned char *) (addr)) = (val);
    DWB();
}

static inline unsigned readl(unsigned addr)
{
    return (*(volatile unsigned *) (addr));
}

static inline unsigned readw(unsigned addr)
{
    return (*(volatile unsigned short *) (addr));
}

static inline unsigned char readb(unsigned addr)
{
    return (*(volatile unsigned char *) (addr));
}

/* main.c */
/* manage a list of functions to call */
//void boot_register_poll_func(void (*func)(void));
//void boot_poll(void);

/* misc.c */
//void *alloc(unsigned sz); /* alloc 32byte aligned memory */
void *alloc_page_aligned(unsigned sz);


/* string */
int memcmp(const void *cs, const void *ct, size_t count);
int strcmp(const char *cs, const char *ct);
int strncmp(const char *cs, const char *ct, size_t count);
char *strstr(const char *s1, const char *s2);
char *strnstr(const char *s1, const char *s2, size_t len);
size_t strlen(const char *s);
char *strchr(const char *s, int c);
size_t strnlen(const char *s, size_t count);

/* clock */
unsigned cycles_per_second(void);
void print_cpu_speed(void);
void mdelay(unsigned long msecs);
void udelay(unsigned long usecs);

void enable_mmu(void);
void stop_mmu(void);
void dcache_enable(void);
void dcache_disable(void);
void icache_enable(void);
void icache_disable(void);

#define DIGEST_SIZE 20
#define SIGNATURE_SIZE 256
//对应crypto文件
//void compute_digest(void *data, unsigned len, void *digest_out);
//int is_signature_okay(void *digest, void *signature, void *pubkey);
void enable_watchdog(void);
void disable_watchdog(void);

void* boot_malloc(u32 size);
u32 boot_free(void *addr);
#define xmalloc boot_malloc
#define xfree(x)    boot_free(x)

int run_command (const char *cmd, int flag);
void main_loop (void);
u32 crc32 (u32 crc, const char *buf, u32 len);
void io_remap(void);
s32 check_hupg(u32 addr, HI_UPG_FILE_HEADER_S *head);
void boot_malloc_init(void);
int ctrlc (void);
void serial_exit (void);
void check_ctrlc(void);
void led_status(LED_PHASE_E phase);

#endif
