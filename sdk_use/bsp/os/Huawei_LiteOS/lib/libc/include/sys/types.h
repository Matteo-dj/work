//=============================================================================
//
//      types.h
//
//      POSIX types header
//
//=============================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):
// Contributors:
// Date:          2015-02-28
// Purpose:       POSIX types header
// Description:   This header contains various POSIX type definitions. These types
//                are implementation defined.
//
// Usage:         #include <sys/types.h>
//
//
//####DESCRIPTIONEND####
//
//=============================================================================
#ifndef LITEOS_LIBC_SYS_TYPES_H
#define LITEOS_LIBC_SYS_TYPES_H
#ifdef LOSCFG_FS_VFS
#include "vfs_config.h"
#include "compiler.h"
#endif
#include "los_typedef.h"
#include "los_base.h"
#include "los_sys.h"
#include "asm/platform.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/********************Start of uclibc basic typedef*************************/
typedef unsigned int    uintptr_t;

typedef long long int __quad_t;
typedef unsigned long long int __u_quad_t;

#define	__S16_TYPE		    short int
#define __U16_TYPE		    unsigned short int
#define	__S32_TYPE		    int
#define __U32_TYPE		    unsigned int
#define __SLONGWORD_TYPE	long int
#define __ULONGWORD_TYPE	unsigned long int
//#if __WORDSIZE == 32
#define __SQUAD_TYPE		__quad_t
#define __UQUAD_TYPE		__u_quad_t
#define __SWORD_TYPE		int
#define __UWORD_TYPE		unsigned int
#define __SLONG32_TYPE		long int
#define __ULONG32_TYPE		unsigned long int
#define __S64_TYPE		    __quad_t
#define __U64_TYPE		    __u_quad_t
#define __FSID_T_TYPE		struct { int __val[2]; }

#define PTHREAD_CREATE_JOINABLE	        0
#define PTHREAD_CREATE_DETACHED	        1

#define PTHREAD_SCOPE_SYSTEM            0
#define PTHREAD_SCOPE_PROCESS           1

#define PTHREAD_INHERIT_SCHED           0
#define PTHREAD_EXPLICIT_SCHED          1

typedef __UQUAD_TYPE dev_t;	/* Type of device numbers.  */
typedef __U32_TYPE   uid_t;	/* Type of user identifications.  */
typedef __U32_TYPE   gid_t;	/* Type of group identifications.  */
typedef __ULONGWORD_TYPE ino_t;	/* Type of file serial numbers.  */
typedef __UQUAD_TYPE ino64_t;	/* Type of file serial numbers (LFS).*/
typedef __U32_TYPE   mode_t;	/* Type of file attribute bitmasks.  */
typedef __UWORD_TYPE nlink_t;	/* Type of file link counts.  */
typedef __SLONGWORD_TYPE off_t;	/* Type of file sizes and offsets.  */
typedef __SQUAD_TYPE off64_t;	/* Type of file sizes and offsets (LFS).  */
typedef __S32_TYPE   pid_t;	/* Type of process identifications.  */
typedef __FSID_T_TYPE fsid_t;	/* Type of file system IDs.  */
typedef __SLONGWORD_TYPE clock_t;	/* Type of CPU usage counts.  */
typedef __ULONGWORD_TYPE rlim_t;	/* Type for resource measurement.  */
typedef __UQUAD_TYPE     rlim64_t;	/* Type for resource measurement (LFS).  */
typedef __U32_TYPE       id_t;		/* General type for IDs.  */
//typedef __SLONGWORD_TYPE time_t;	/* Seconds since the Epoch.  */
typedef __U32_TYPE       useconds_t; /* Count of microseconds.  */
typedef __SLONGWORD_TYPE suseconds_t; /* Signed count of microseconds.  */

typedef __S32_TYPE       daddr_t;	/* The type of a disk address.  */
typedef __SLONGWORD_TYPE swblk_t;	/* Type of a swap block maybe?  */
typedef __S32_TYPE       key_t;	/* Type of an IPC key.  */

/* Clock ID used in clock and timer functions.  */
//typedef __SLONGWORD_TYPE clockid_t;

/* Timer ID returned by `timer_create'.  */
//typedef void *timer_t;

/* Type to represent block size.  */
typedef __SLONGWORD_TYPE blksize_t;

/* Types from the Large File Support interface.  */

/* Type to count number of disk blocks.  */
typedef __SLONGWORD_TYPE blkcnt_t;
typedef __SQUAD_TYPE     blkcnt64_t;

/* Type to count file system blocks.  */
typedef __ULONGWORD_TYPE fsblkcnt_t;
typedef __UQUAD_TYPE     fsblkcnt64_t;

/* Type to count file system nodes.  */
typedef __ULONGWORD_TYPE fsfilcnt_t;
typedef __UQUAD_TYPE     fsfilcnt64_t;

/* These few don't really vary by system, they always correspond
   to one of the other defined types.  */
typedef off64_t     loff_t;	/* Type of file sizes and offsets (LFS).  */
typedef __quad_t *  qaddr_t;
typedef char *      caddr_t;

/* Duplicates info from stdint.h but this is used in unistd.h.  */
typedef __SWORD_TYPE intptr_t;

/* Duplicate info from sys/socket.h.  */
typedef __U32_TYPE   socklen_t;
typedef uint16_t     sa_family_t;

/* Number of descriptors that can fit in an `fd_set'.  */
#define	FD_SETSIZE		1024

typedef UINT32 pthread_t;
typedef int pthread_key_t;
typedef int pthread_once_t;

struct sched_param
{
    int sched_priority;
};

typedef struct __pthread_attr_s
{
    unsigned int    detachstate;
    unsigned int    schedpolicy;
    struct          sched_param schedparam;
    unsigned int    inheritsched;
    unsigned int    scope;
    unsigned int    stackaddr_set;
    void            *stackaddr;
    unsigned int    stacksize_set;
    size_t          stacksize;
} pthread_attr_t;

#ifndef TRUE
#  define TRUE  1
#endif

#ifndef FALSE
#  define FALSE 0
#endif

#ifndef HAVE_DIR_STRUCTURE
typedef void DIR;
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif
