//*****************************************************************************
//
//                  版权所有 (C), 1998-2010, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : hi_types.h
//  版 本 号   : V1.0
//  功能描述   : 数据类型定义和公用宏和结构定义
//  函数列表   : 无
//  修改历史   :
//
//*****************************************************************************


#ifndef __HI_TYPES_H__
#define __HI_TYPES_H__

#include <hi_product_config.h>
//*****************************************************************************
#if defined(OBC_PRODUCT_CFG_CHIP_PROD_TYPE_CCO)
#define HI_PROD_CFG_FILE  <hi_product_config_cco.h>
#elif defined(OBC_PRODUCT_CFG_CHIP_PROD_TYPE_STA)
#define HI_PROD_CFG_FILE  <hi_product_config_sta.h>
#elif defined(OBC_PRODUCT_CFG_CHIP_PROD_TYPE_NDM)
#define HI_PROD_CFG_FILE  <hi_product_config_ndm.h>
#elif defined(OBC_PRODUCT_CFG_CHIP_PROD_TYPE_BOOT)
#define HI_PROD_CFG_FILE  <hi_product_config_boot.h>
#endif

#define HI_PROD_CFG_FILE_DEFAULT  <hi_product_config.h>

#if !defined(HI_HAVE_NO_PRODUCT_CONFIG_FILE)
#if defined(__OBC_VX_OS_SRC_BUILD__)
#include HI_PROD_CFG_FILE
#else
#include HI_PROD_CFG_FILE_DEFAULT
#endif
#endif
//*****************************************************************************

#if defined(__CC_ARM)
#define HI_HAVE_CROSS_COMPILER_ARM_ARMCC
#elif defined(__GNUC__)
#define HI_HAVE_CROSS_COMPILER_ARM_GCC
#elif defined(__DCC__)
#define HI_HAVE_CROSS_COMPILER_DIAB
#if defined(__HI_ASM_LANGUAGE__) || defined(_ASMLANGUAGE)
#define HI_HAVE_CROSS_COMPILER_DIAB_AS
#endif
#elif defined(_MSC_VER)
#define HI_HAVE_NOTIVE_COMPILER_VC
#endif

#if !defined(HI_HAVE_CROSS_COMPILER_DIAB_AS)
#if defined(HI_HAVE_CROSS_COMPILER_ARM_GCC) || defined(HI_HAVE_CROSS_COMPILER_ARM_ARMCC) || defined(HI_HAVE_CROSS_COMPILER_DIAB)
#undef HI_HAVE_CROSS_COMPILER
#define HI_HAVE_CROSS_COMPILER
#endif

#ifdef PRODUCT_CFG_PLATFORM_HI3911
#define SAL_HAVE_HI3911_VER
#endif

#ifdef PRODUCT_CFG_OS_WIN
# undef SAL_HAVE_OS_WIN_VER
# undef SAL_HAVE_OS_NU_VER
# undef SAL_HAVE_OS_VX_VER
#define SAL_HAVE_OS_WIN_VER
#endif

#ifdef PRODUCT_CFG_OS_NU
# undef SAL_HAVE_OS_NU_VER
# define SAL_HAVE_OS_NU_VER
#elif defined (PRODUCT_CFG_OS_VX)
# undef SAL_HAVE_OS_VX_VER
# define SAL_HAVE_OS_VX_VER
#else
# if defined (_WIN32) && defined (_MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#  ifndef SAL_HAVE_SIMU_WIN_VER
#   define SAL_HAVE_SIMU_WIN_VER
#   ifdef _USRDLL
#    if !defined(PRODUCT_CFG_MSVC_HIDE_AUTOLINK_OUTPUT_INFO)
#     pragma message("Windows VC Simulator DLL Version ")
#    endif
#   endif
#   ifdef _LIB
#    if !defined(PRODUCT_CFG_MSVC_HIDE_AUTOLINK_OUTPUT_INFO)
#     pragma message("Windows VC Simulator lib Version ")
#    endif
#   endif
#   ifdef _CONSOLE
#    if !defined(PRODUCT_CFG_MSVC_HIDE_AUTOLINK_OUTPUT_INFO)
#     pragma message("Windows VC Simulator console Version ")
#    endif
#   endif
#  else
#   if !defined(PRODUCT_CFG_MSVC_HIDE_AUTOLINK_OUTPUT_INFO)
#    pragma message("Windows Version")
#   endif
#  endif
# endif
#endif

#undef HI_SYS_DEBUG
#if (defined(PRODUCT_CFG_VERSION_DEBUG) || defined(SAL_HAVE_DEBUG_VERSION)) && !defined(SAL_HAVE_RELEASE_VERSION)
# define HI_SYS_DEBUG
#endif

#if defined(PRODUCT_CFG_OS_WIN)
# pragma warning(disable:4200)  // disable nonstandard extension used : zero-sized array in struct/union.
# pragma warning(disable:4214)  // allows bitfield structure members to be of any integral type.
# pragma warning(disable:4201)
# pragma warning(disable:4514)
# pragma warning(disable:4127)
#endif

// 基本数据类型定义
typedef unsigned int           HI_U32;
typedef int                    HI_S32;
typedef unsigned short         HI_U16;
typedef signed   short         HI_S16;
typedef unsigned char          HI_U8;
typedef signed char            HI_S8;
typedef void                   HI_VOID;
typedef char                   HI_CHAR;
typedef unsigned char          HI_UCHAR;
typedef HI_U8                  HI_BOOL;
typedef void*                  HI_PVOID;
typedef HI_U8                  HI_BYTE;
typedef HI_PVOID               HI_HANDLE;
typedef HI_BYTE*               HI_PBYTE;
typedef HI_S32                 HI_SOCKET;
typedef float                  HI_FLOAT;
typedef double                 HI_DOUBLE;
typedef volatile HI_U32        HI_U32_REG;
typedef HI_PVOID               HI_FUNC_PTR;
typedef HI_U32                 HI_FUNC;
typedef unsigned int           HI_SiZE_T;//size_t
typedef int                    HI_SSiZE_T;//ssize_t
typedef int                    HI_OFFET_T;//__off_t


#undef ERROR
#define ERROR -1


#undef NULL
#ifndef NULL
#define NULL 0
#endif

#define HI_CONST               const
#define HI_REG                 register

#define HI_U32_MAX            0xFFFFFFFF
#define HI_U16_MAX            0xFFFF

typedef HI_PVOID (*HI_PVOID_CALLBACK_F)(HI_VOID);
typedef HI_VOID  (*HI_VOID_CALLBACK_F)(HI_VOID);
typedef HI_BOOL  (*HI_BOOL_CALLBACK_F)(HI_VOID);
typedef HI_VOID  (*HI_VOID_U32_CALLBACK_F)(HI_U32);
typedef HI_VOID  (*HI_TIMER_CALLBACK_F)(HI_U32);
typedef HI_U32   (*HI_U32_PVOID_CALLBACK_F)(HI_PVOID);
typedef HI_U32   (*HI_U32_U32_PVOID_CALLBACK_F)(HI_PVOID, HI_U32);
typedef int 	 (*FUNCPTR)(HI_VOID);     /* ptr to function returning int */
typedef void 	 (*VOIDFUNCPTR)(HI_VOID); /* ptr to function returning void */

typedef HI_CONST HI_CHAR*  HI_PCSTR;

#ifdef PRODUCT_CFG_OS_WIN
typedef unsigned __int64       HI_U64;
typedef __int64 HI_S64;
#elif defined(HI_HAVE_CROSS_COMPILER_ARM_ARMCC)
typedef unsigned __int64       HI_U64;
typedef __int64 HI_S64;
#elif defined(HI_HAVE_CROSS_COMPILER_ARM_GCC) || defined(HAVE_PCLINT_CHECK)
typedef unsigned long long     HI_U64;
typedef long long              HI_S64;
#elif defined(HI_HAVE_CROSS_COMPILER_DIAB)
typedef unsigned long long     HI_U64;
typedef long long              HI_S64;
#elif !defined(PRODUCT_CFG_HSO)
#error "unknown compiler"
#endif


#define HI_S32_BITS       32
#define HI_S32_MAX       (~(~0<<(HI_S32_BITS-1)))

#define HI_PUBLIC    extern
#if !defined(PRODUCT_CFG_FEATURE_UT)
# define HI_PRV   static
#else
# define HI_PRV
#endif

#endif // HI_HAVE_CROSS_COMPILER_DIAB_AS

#ifdef PRODUCT_CFG_OS_WIN
# define HI_API   _declspec(dllexport)
# define HI_INLINE  __inline
#elif defined(HI_HAVE_CROSS_COMPILER_ARM_GCC)
#  define HI_INLINE  inline
#  define HI_API
#elif defined(HI_HAVE_CROSS_COMPILER_ARM_ARMCC)
#  define HI_INLINE  inline
#  define HI_API
#elif defined(HI_HAVE_CROSS_COMPILER_DIAB)
#  define HI_INLINE  __inline__
#  define HI_API
# else
#  define HI_INLINE __inline
#  define HI_API
#endif


#define HI_PRVL HI_PRV HI_INLINE

#if defined(__ONEBUILDER__CROSS_COMPILER_PRODUCT_CONFIG__)
#if defined(HI_HAVE_CROSS_COMPILER_ARM_ARMCC) || defined(HI_HAVE_CROSS_COMPILER_ARM_GCC)
# define HI_SECTION(name_string) __attribute__ ((section(name_string)))
# define HI_PACKED               __attribute__((packed))
# define HI_ALIGNED4             __attribute__ ((aligned (4)))
#elif defined(HI_HAVE_CROSS_COMPILER_DIAB)
# define HI_SECTION(name_string) __attribute__ ((section(name_string)))
# define HI_PACKED               __attribute__((packed))
# define HI_ALIGNED4             __attribute__ ((aligned (4)))
#endif
#elif defined(SAL_HAVE_OS_WIN_VER) || defined(PRODUCT_CFG_HSO)
#  define HI_SECTION(name_string)
#  define HI_PACKED
#  define HI_ALIGNED4
#else
#  define HI_SECTION(name_string)
#  define HI_PACKED
#  define HI_ALIGNED4
#endif

#if defined(SAL_HAVE_OS_WIN_VER)
# if defined(_DEBUG) || defined(PRODUCT_CFG_VERSION_DEBUG)
#  define HI_DLL_LIB_NAME(x)        x ## "_debug.dll"
# else
#  define HI_DLL_LIB_NAME(x)        x ## "_release.dll"
# endif
#else
#if defined(HI_HAVE_CROSS_COMPILER_ARM_GCC)
# define HI_DLL_LIB_NAME(x)        x
#else
# define HI_DLL_LIB_NAME(x)        x ## ".lib"
#endif
#endif


#if defined(SAL_HAVE_NO_EXTERN_DEFINED)
# define HI_EXTERN
# define HI_EXTERN_C
#else
# if defined(PRODUCT_CFG_OS_WIN)
#  define HI_EXTERN      extern HI_API
#  define HI_EXTERN_C    HI_EXTERN
#  define HI_EAPI        extern HI_API
# else
#    define HI_EXTERN   extern
#    define HI_EAPI
#    define HI_EXTERN_C
# endif
#endif

#ifdef __cplusplus
# define HI_CPP_START    extern "C" {
# define HI_CPP_END      }
#else
# define HI_CPP_START
# define HI_CPP_END
#endif

#if defined(HI_HAVE_CROSS_COMPILER_ARM_ARMCC)
#define HI_NOP             __asm { nop }
#define HI_DBG_BREAK()     __asm { swi 0x14DEAD }
#elif defined(HI_HAVE_CROSS_COMPILER_DIAB)
#define HI_NOP
#define HI_DBG_BREAK()
#else
#define HI_NOP
#ifdef PRODUCT_CFG_OS_WIN
#define HI_DBG_BREAK()     _asm { int 3 }
#else
#define HI_DBG_BREAK()
#endif
#endif

#define HI_START_HEADER    HI_CPP_START
#define HI_END_HEADER      HI_CPP_END


#undef HI_OUT
#undef HI_IN
#undef HI_INOUT
#define HI_OUT
#define HI_IN
#define HI_INOUT

#define HI_FALSE   0
#define HI_TRUE    1

#ifdef __cplusplus
#define HI_NULL	   0
#else
#define HI_NULL	((void *)0)
#endif

#define HI_ARRAY_COUNT(x)    (sizeof(x)/sizeof(x[0]))


// TODO:临时放置
#define PRODUCT_CFG_PTCL_V100   1
#define PRODUCT_CFG_PTCL_SG     1

#if !defined(HI_UNREF_PARAM) && !defined(HI_HAVE_CROSS_COMPILER_DIAB)
#define HI_UNREF_PARAM(P)  P = P
#else
#define HI_UNREF_PARAM(P)
#endif


#if defined(PRODUCT_CFG_PLATFORM_HI3911)
#if defined(HI_HAVE_CROSS_COMPILER_ARM_ARMCC)
#define HI_VOLATILE   volatile
#define HI_SYS_GET_LR()                        __return_address()
#elif defined(HAVE_PCLINT_CHECK)
#define HI_VOLATILE
#define HI_SYS_GET_LR()                        0
#elif defined(HI_HAVE_CROSS_COMPILER_DIAB)
#define HI_VOLATILE   volatile
#define HI_SYS_GET_LR()                        0
#elif defined(MAC_V200_LLT)
#define HI_VOLATILE  volatile
#define HI_SYS_GET_LR()                        0
#else
#define HI_VOLATILE  __volatile__
#define HI_SYS_GET_LR()                        0
#endif
#else
#define HI_VOLATILE
#define HI_SYS_GET_LR()                        0
#endif

#define HI_ALIGIN_U32_SIZE(x)  ((x&(~3))+4) // 构造4个字节对齐
#define HI_IS_ALIGN_U32(x)     (!((x)&3))   // 判断是否为4字节对齐
#define HI_IS_UNALIGN_U32(x)   ((x)&3)      // 判断是否为4字节对齐
#if defined(HAVE_PCLINT_CHECK)
#define HI_FIELDOFFSET(s,m)    (0) // 结构成员偏移
#else
#define HI_FIELDOFFSET(s,m)    ((HI_U32)&(((s *)0)->m)) // 结构成员偏移
#endif

#define HI_CHAR_CR             '\r' // 0x0D
#define HI_CHAR_LF             '\n' // 0x0A

#define HI_MAKEU16(a, b)       ((HI_U16)(((HI_U8)(a))  | ((HI_U16)((HI_U8)(b))) << 8))
#define HI_MAKEU32(a, b)       ((HI_U32)(((HI_U16)(a)) | ((HI_U32)((HI_U16)(b))) << 16))
#define HI_MAKEU64(a, b)       ((HI_U64)(((HI_U32)(a)) | ((HI_U64)((HI_U32)(b))) <<32))

#define HI_HIU32(l)            ((HI_U32)(((HI_U64)(l) >> 32) & 0xFFFFFFFF))
#define HI_LOU32(l)            ((HI_U32)(l))

#define HI_HIU16(l)            ((HI_U16)(((HI_U32)(l) >> 16) & 0xFFFF))
#define HI_LOU16(l)            ((HI_U16)(l))
#define HI_HIU8(l)             ((HI_U8)(((HI_U16)(l) >> 8) & 0xFF))
#define HI_LOU8(l)             ((HI_U8)(l))

#define HI_ABS(a)              (((a) > 0) ? (a) : (-(a)))

#define HI_MAX(a,b)            (((a) > (b)) ? (a) : (b))
#define HI_MIN(a,b)            (((a) < (b)) ? (a) : (b))
#undef MIN_T
#define MIN_T  HI_MIN

#define HI_MAKE_IDENTIFIER(a,b,c,d)      HI_MAKEU32(HI_MAKEU16(a,b),HI_MAKEU16(c,d))
#define HI_MAKE_VER16(spc, b)           ((HI_U16)(((HI_U8)((spc)&0x0F))  | ((HI_U16)((HI_U8)(b))) << 12))

#define  HI_SET_BIT_I(val, n)               ((val)|=(1<<n))
#define  HI_CLR_BIT_I(val, n)               ((val)&=~(1<<n))
#define  HI_IS_BIT_SET_I(val, n)            ((val) & (1<<n))
#define  HI_IS_BIT_CLR_I(val, n)            (~((val) & (1<<n)))
#define  HI_SWITCH_BIT_I(val, n)            ((val)^=(1<<n))
#define  HI_GET_BIT_I(val, n)               (((val) >> n) & 1)

#define  HI_U8_BIT_VAL(b7,b6,b5,b4,b3,b2,b1,b0)                     (b7<<7)|(b6<<6)|(b5<<5) |(b4<<4)|(b3<<3)|(b2<<2)|(b1<<1)|(b0<<0)
#define  HI_U16_BIT_VAL(b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0)  (HI_U16)((b12<<12)|(b11<<11)|(b10<<10)|(b9<<9)|(b8<<8)|(b7<<7)|(b6<<6)|(b5<<5) |(b4<<4)|(b3<<3)|(b2<<2)|(b1<<1)|(b0<<0))

#if defined(__ONEBUILDER__CROSS_COMPILER_PRODUCT_CONFIG__)
#define HSO_ENUM    HI_ALIGNED4 enum
#define HI_U8A      HI_ALIGNED4 HI_U8
#define HI_U16A     HI_ALIGNED4 HI_U16
#define HI_CHARTA   HI_ALIGNED4 HI_CHAR
#else
#define HSO_ENUM    enum
#define HI_U8A      HI_U8
#define HI_U16A     HI_U16
#define HI_CHARTA   HI_CHAR
#endif

#if defined(PRODUCT_CFG_HSO) && defined(HI_HAVE_NOTIVE_COMPILER_VC)
#define __FUNCTION__   "NA"
#endif
//*****************************************************************************

typedef unsigned int           hi_u32;
typedef int                    hi_s32;
typedef unsigned short         hi_u16;
typedef signed   short         hi_s16;
typedef unsigned char          hi_u8;
typedef signed char            hi_s8;
typedef void                   hi_void;
typedef char                   hi_char;
typedef unsigned char          hi_uchar;
typedef HI_U8                  hi_bool;
typedef void*                  hi_pvoid;
typedef HI_U8                  hi_byte;
typedef HI_PVOID               hi_handle;
typedef HI_BYTE*               hi_pbyte;
typedef HI_S32                 hi_socket;
typedef float                  hi_float;
typedef double                 hi_double;
typedef volatile HI_U32        hi_u32_reg;
typedef HI_PVOID               hi_func_ptr;
typedef HI_U32                 hi_func;
typedef unsigned int           hi_size_t;
typedef int                    hi_ssize_t;
typedef int                    hi_off_t;
typedef unsigned int           uintptr_t;
typedef int                    intptr_t;

typedef hi_pvoid (*hi_pvoid_callback_f)(hi_void);
typedef hi_void  (*hi_void_callback_f)(hi_void);
typedef hi_bool  (*hi_bool_callback_f)(hi_void);
typedef hi_void  (*hi_void_u32_callback_f)(hi_u32);
typedef hi_void  (*hi_timer_callback_f)(hi_u32);
typedef hi_u32   (*hi_u32_pvoid_callback_f)(hi_pvoid);
typedef hi_u32   (*hi_u32_u32_pvoid_callback_f)(hi_pvoid, hi_u32);
typedef int 	 (*funcptr)(hi_void);     /* ptr to function returning int */
typedef void 	 (*voidfuncptr)(hi_void); /* ptr to function returning void */

typedef HI_CONST hi_char*  hi_pcstr;

#ifdef PRODUCT_CFG_OS_WIN
typedef unsigned __int64       hi_u64;
typedef __int64 hi_s64;
#elif defined(HI_HAVE_CROSS_COMPILER_ARM_ARMCC)
typedef unsigned __int64       hi_u64;
typedef __int64 hi_s64;
#elif defined(HI_HAVE_CROSS_COMPILER_ARM_GCC) || defined(HAVE_PCLINT_CHECK)
typedef unsigned long long     hi_u64;
typedef long long              hi_s64;
#elif defined(HI_HAVE_CROSS_COMPILER_DIAB)
typedef unsigned long long     hi_u64;
typedef long long              hi_s64;
#elif !defined(PRODUCT_CFG_HSO)
#error "unknown compiler"
#endif


//*****************************************************************************
#define HI_SET_U32_PTR_VAL(ptr,offset,val)  *((HI_U32*)(((HI_U8*)(ptr))+(offset)))= (val)
#define HI_GET_U32_PTR_VAL(ptr,offset)      *((HI_U32*)(((HI_U8*)(ptr))+(offset)))
//*****************************************************************************

//*****************************************************************************
#define HI_SIZE_1K        (1024)
#define HI_SIZE_1M        (1024*1024)
//*****************************************************************************
/*寄存器访问接口*/
#define HI_REG_WRITE32(addr, val) (*(volatile unsigned int *)(addr) = (val))
#define HI_REG_READ32(addr, val)  ((val) = *(volatile unsigned int *)(addr))
#define HI_REG_READ_VAL32(addr)   (*(volatile unsigned *) (addr))

//*****************************************************************************
#include <hi_errno.h>
#include <hi_config.h>

#endif //__HI_TYPES_H__

