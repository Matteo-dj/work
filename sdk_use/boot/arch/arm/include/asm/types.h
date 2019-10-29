#ifndef __ASM_ARM_TYPES_H
#define __ASM_ARM_TYPES_H

typedef unsigned short umode_t;

/*
 * __xx is ok: it doesn't pollute the POSIX namespace. Use these in the
 * header files exported to user space
 */

typedef __signed__ char __s8;
typedef unsigned char __u8;

typedef __signed__ short __s16;
typedef unsigned short __u16;

typedef __signed__ int __s32;
typedef unsigned int __u32;

#if defined(__GNUC__)
__extension__ typedef __signed__ long long __s64;
__extension__ typedef unsigned long long __u64;
#endif

/*
 * These aren't exported outside the kernel to avoid name space clashes
 */
typedef signed char s8;
typedef unsigned char u8;

typedef signed short s16;
typedef unsigned short u16;

typedef signed int s32;
typedef unsigned int u32;

typedef signed long long s64;
typedef unsigned long long u64;

#ifdef	CONFIG_ARM64
#define BITS_PER_LONG 64
#else	/* CONFIG_ARM64 */
#define BITS_PER_LONG 32
#endif	/* CONFIG_ARM64 */

#ifdef CONFIG_PHYS_64BIT
typedef unsigned long long phys_addr_t;
typedef unsigned long long phys_size_t;
#else
/* DMA addresses are 32-bits wide */
typedef unsigned long phys_addr_t;
typedef unsigned long phys_size_t;
#endif

typedef unsigned long long uint64_t;//32bit in ARM
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef long long int64_t;

typedef int ptrdiff_t;
typedef unsigned int size_t;
typedef unsigned int           uintptr_t;
/*
 * A dma_addr_t can hold any valid DMA address, i.e., any address returned
 * by the DMA API.
 *
 * If the DMA API only uses 32-bit addresses, dma_addr_t need only be 32
 * bits wide.  Bus addresses, e.g., PCI BARs, may be wider than 32 bits,
 * but drivers do memory-mapped I/O to ioremapped kernel virtual addresses,
 * so they don't care about the size of the actual bus addresses.
 */
#ifdef CONFIG_DMA_ADDR_T_64BIT
typedef unsigned long long dma_addr_t;
#else
typedef u32 dma_addr_t;
#endif

/* bsd */
typedef unsigned char           u_char;
typedef unsigned short          u_short;
typedef unsigned int            u_int;
typedef unsigned long           u_long;

/* sysv */
typedef unsigned char           unchar;
typedef unsigned short          ushort;
typedef unsigned int            uint;
typedef unsigned long           ulong;

#define TRUE  (1)
#define FALSE (0)

#ifndef NULL
#  define NULL         (void*)0
#endif
#  define HI_NULL		(void*)0

#define offsetof(type, field) ((u32)&((type *)0)->field)
#define MAKEU16(a, b)       ((u_short)(((u_char)(a))  | ((u_short)((u_char)(b))) << 8))

#ifndef HI_UNREF_PARAM
#define HI_UNREF_PARAM(P)  P = P
#endif

#endif
