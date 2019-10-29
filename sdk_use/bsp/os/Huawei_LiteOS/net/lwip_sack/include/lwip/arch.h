/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/**********************************************************************************
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which
 * might include those applicable to Huawei LiteOS of U.S. and the country in which you
 * are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance
 * with such applicable export control laws and regulations.
 **********************************************************************************/

#ifndef __LWIP_ARCH_H__
#define __LWIP_ARCH_H__

#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN 1234
#endif

#ifndef BIG_ENDIAN
#define BIG_ENDIAN 4321
#endif

#include "arch/cc.h"
#include "lwip/opt.h"

/** Temporary: define format string for size_t if not defined in cc.h */
#ifndef SZT_F
#define SZT_F U32_F
#endif /* SZT_F */
/** Temporary upgrade helper: define format string for u8_t as hex if not
    defined in cc.h */
#ifndef X8_F
#define X8_F  "02x"
#endif /* X8_F */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PACK_STRUCT_BEGIN
#define PACK_STRUCT_BEGIN
#endif /* PACK_STRUCT_BEGIN */

#ifndef PACK_STRUCT_END
#define PACK_STRUCT_END
#endif /* PACK_STRUCT_END */

#ifndef PACK_STRUCT_FIELD
#define PACK_STRUCT_FIELD(x) x
#endif /* PACK_STRUCT_FIELD */

/* Used for struct fields of u8_t,
 * where some compilers warn that packing is not necessary */
#ifndef PACK_STRUCT_FLD_8
#define PACK_STRUCT_FLD_8(x) PACK_STRUCT_FIELD(x)
#endif /* PACK_STRUCT_FLD_8 */

#ifndef LWIP_UNUSED_ARG
#define LWIP_UNUSED_ARG(x) (void)x
#endif /* LWIP_UNUSED_ARG */

#if LWIP_PROVIDE_ERRNO

/**
* @defgroup errno
* @ingroup
* @par Prototype
* @code
* extern int errno;
* @endcode
*
* @datastruct below are the possible return values:
@datastruct #define  EPERM         1    Operation not permitted
@datastruct #define  ENOENT        2    No such file or directory
@datastruct #define  ESRCH         3    No such process
@datastruct #define  EINTR         4    Interrupted system call
@datastruct #define  EIO           5    I/O error
@datastruct #define  ENXIO         6    No such device or address
@datastruct #define  E2BIG         7    Arg list too long
@datastruct #define  ENOEXEC       8    Exec format error
@datastruct #define  EBADF         9    Bad file number
@datastruct #define  ECHILD       10    No child processes
@datastruct #define  EAGAIN       11    Try again
@datastruct #define  ENOMEM       12    Out of memory
@datastruct #define  EACCES       13    Permission denied
@datastruct #define  EFAULT       14    Bad address
@datastruct #define  ENOTBLK      15    Block device required
@datastruct #define  EBUSY        16    Device or resource busy
@datastruct #define  EEXIST       17    File exists
@datastruct #define  EXDEV        18    Cross-device link
@datastruct #define  ENODEV       19    No such device
@datastruct #define  ENOTDIR      20    Not a directory
@datastruct #define  EISDIR       21    Is a directory
@datastruct #define  EINVAL       22    Invalid argument
@datastruct #define  ENFILE       23    File table overflow
@datastruct #define  EMFILE       24    Too many open files
@datastruct #define  ENOTTY       25    Not a typewriter
@datastruct #define  ETXTBSY      26    Text file busy
@datastruct #define  EFBIG        27    File too large
@datastruct #define  ENOSPC       28    No space left on device
@datastruct #define  ESPIPE       29    Illegal seek
@datastruct #define  EROFS        30    Read-only file system
@datastruct #define  EMLINK       31    Too many links
@datastruct #define  EPIPE        32    Broken pipe
@datastruct #define  EDOM         33    Math argument out of domain of func
@datastruct #define  ERANGE       34    Math result not representable
@datastruct #define  EDEADLK      35    Resource deadlock would occur
@datastruct #define  ENAMETOOLONG 36    File name too long
@datastruct #define  ENOLCK       37    No rcrd locks available
@datastruct #define  ENOSYS       38    Function not implemented
@datastruct #define  ENOTEMPTY    39    Directory not empty
@datastruct #define  ELOOP        40    Too many symbolic links encountered
@datastruct #define  EWOULDBLOCK  EAGAIN    Operation would block
@datastruct #define  ENOMSG       42    No message of desired type
@datastruct #define  EIDRM        43    Identifier removed
@datastruct #define  ECHRNG       44    Channel number out of range
@datastruct #define  EL2NSYNC     45    Level 2 not synchronized
@datastruct #define  EL3HLT       46    Level 3 halted
@datastruct #define  EL3RST       47    Level 3 reset
@datastruct #define  ELNRNG       48    Link number out of range
@datastruct #define  EUNATCH      49    Protocol driver not attached
@datastruct #define  ENOCSI       50    No CSI structure available
@datastruct #define  EL2HLT       51    Level 2 halted
@datastruct #define  EBADE        52    Invalid exchange
@datastruct #define  EBADR        53    Invalid request descriptor
@datastruct #define  EXFULL       54    Exchange full
@datastruct #define  ENOANO       55    No anode
@datastruct #define  EBADRQC      56    Invalid request code
@datastruct #define  EBADSLT      57    Invalid slot

@datastruct #define  EDEADLOCK    EDEADLK

@datastruct #define  EBFONT       59    Bad font file format
@datastruct #define  ENOSTR       60    Device not a stream
@datastruct #define  ENODATA      61    No data available
@datastruct #define  ETIME        62    Timer expired
@datastruct #define  ENOSR        63    Out of streams resources
@datastruct #define  ENONET       64    Machine is not on the network
@datastruct #define  ENOPKG       65    Package not installed
@datastruct #define  EREMOTE      66    Object is remote
@datastruct #define  ENOLINK      67    Link has been severed
@datastruct #define  EADV         68    Advertise error
@datastruct #define  ESRMNT       69    Srmount error
@datastruct #define  ECOMM        70    Communication error on send
@datastruct #define  EPROTO       71    Protocol error
@datastruct #define  EMULTIHOP    72    Multihop attempted
@datastruct #define  EDOTDOT      73    RFS specific error
@datastruct #define  EBADMSG      74    Not a data message
@datastruct #define  EOVERFLOW    75    Value too large for defined data type
@datastruct #define  ENOTUNIQ     76    Name not unique on network
@datastruct #define  EBADFD       77    File descriptor in bad state
@datastruct #define  EREMCHG      78    Remote address changed
@datastruct #define  ELIBACC      79    Can not access a needed shared library
@datastruct #define  ELIBBAD      80    Accessing a corrupted shared library
@datastruct #define  ELIBSCN      81    .lib section in a.out corrupted
@datastruct #define  ELIBMAX      82    Attempting to link in too many shared libraries
@datastruct #define  ELIBEXEC     83    Cannot exec a shared library directly
@datastruct #define  EILSEQ       84    Illegal byte sequence
@datastruct #define  ERESTART     85    Interrupted system call should be restarted
@datastruct #define  ESTRPIPE     86    Streams pipe error
@datastruct #define  EUSERS       87    Too many users
@datastruct #define  ENOTSOCK     88    Socket operation on non-socket
@datastruct #define  EDESTADDRREQ 89    Destination address required
@datastruct #define  EMSGSIZE     90    Message too long
@datastruct #define  EPROTOTYPE   91    Protocol wrong type for socket
@datastruct #define  ENOPROTOOPT  92    Protocol not available
@datastruct #define  EPROTONOSUPPORT 93    Protocol not supported
@datastruct #define  ESOCKTNOSUPPORT 94    Socket type not supported
@datastruct #define  EOPNOTSUPP      95    Operation not supported on transport endpoint
@datastruct #define  EPFNOSUPPORT    96    Protocol family not supported
@datastruct #define  EAFNOSUPPORT    97    Address family not supported by protocol
@datastruct #define  EADDRINUSE      98    Address already in use
@datastruct #define  EADDRNOTAVAIL   99    Cannot assign requested address
@datastruct #define  ENETDOWN       100    Network is down
@datastruct #define  ENETUNREACH    101    Network is unreachable
@datastruct #define  ENETRESET      102    Network dropped connection because of reset
@datastruct #define  ECONNABORTED   103    Software caused connection abort
@datastruct #define  ECONNRESET     104    Connection reset by peer
@datastruct #define  ENOBUFS        105    No buffer space available
@datastruct #define  EISCONN        106    Transport endpoint is already connected
@datastruct #define  ENOTCONN       107    Transport endpoint is not connected
@datastruct #define  ESHUTDOWN      108    Cannot send after transport endpoint shutdown
@datastruct #define  ETOOMANYREFS   109    Too many references: cannot splice
@datastruct #define  ETIMEDOUT      110    Connection timed out
@datastruct #define  ECONNREFUSED   111    Connection refused
@datastruct #define  EHOSTDOWN      112    Host is down
@datastruct #define  EHOSTUNREACH   113    No route to host
@datastruct #define  EALREADY       114    Operation already in progress
@datastruct #define  EINPROGRESS    115    Operation now in progress
@datastruct #define  ESTALE         116    Stale NFS file handle
@datastruct #define  EUCLEAN        117    Structure needs cleaning
@datastruct #define  ENOTNAM        118    Not a XENIX named type file
@datastruct #define  ENAVAIL        119    No XENIX semaphores available
@datastruct #define  EISNAM         120    Is a named type file
@datastruct #define  EREMOTEIO      121    Remote I/O error
@datastruct #define  EDQUOT         122    Quota exceeded

@datastruct #define  ENOMEDIUM      123    No medium found
@datastruct #define  EMEDIUMTYPE    124    Wrong medium type

*/

#define  EPERM         1  /* Operation not permitted */
#define  ENOENT        2  /* No such file or directory */
#define  ESRCH         3  /* No such process */
#define  EINTR         4  /* Interrupted system call */
#define  EIO           5  /* I/O error */
#define  ENXIO         6  /* No such device or address */
#define  E2BIG         7  /* Arg list too long */
#define  ENOEXEC       8  /* Exec format error */
#define  EBADF         9  /* Bad file number */
#define  ECHILD       10  /* No child processes */
#define  EAGAIN       11  /* Try again */
#define  ENOMEM       12  /* Out of memory */
#define  EACCES       13  /* Permission denied */
#define  EFAULT       14  /* Bad address */
#define  ENOTBLK      15  /* Block device required */
#define  EBUSY        16  /* Device or resource busy */
#define  EEXIST       17  /* File exists */
#define  EXDEV        18  /* Cross-device link */
#define  ENODEV       19  /* No such device */
#define  ENOTDIR      20  /* Not a directory */
#define  EISDIR       21  /* Is a directory */
#define  EINVAL       22  /* Invalid argument */
#define  ENFILE       23  /* File table overflow */
#define  EMFILE       24  /* Too many open files */
#define  ENOTTY       25  /* Not a typewriter */
#define  ETXTBSY      26  /* Text file busy */
#define  EFBIG        27  /* File too large */
#define  ENOSPC       28  /* No space left on device */
#define  ESPIPE       29  /* Illegal seek */
#define  EROFS        30  /* Read-only file system */
#define  EMLINK       31  /* Too many links */
#define  EPIPE        32  /* Broken pipe */
#define  EDOM         33  /* Math argument out of domain of func */
#define  ERANGE       34  /* Math result not representable */
#define  EDEADLK      35  /* Resource deadlock would occur */
#define  ENAMETOOLONG 36  /* File name too long */
#define  ENOLCK       37  /* No rcrd locks available */
#define  ENOSYS       38  /* Function not implemented */
#define  ENOTEMPTY    39  /* Directory not empty */
#define  ELOOP        40  /* Too many symbolic links encountered */
#define  EWOULDBLOCK  EAGAIN  /* Operation would block */
#define  ENOMSG       42  /* No message of desired type */
#define  EIDRM        43  /* Identifier removed */
#define  ECHRNG       44  /* Channel number out of range */
#define  EL2NSYNC     45  /* Level 2 not synchronized */
#define  EL3HLT       46  /* Level 3 halted */
#define  EL3RST       47  /* Level 3 reset */
#define  ELNRNG       48  /* Link number out of range */
#define  EUNATCH      49  /* Protocol driver not attached */
#define  ENOCSI       50  /* No CSI structure available */
#define  EL2HLT       51  /* Level 2 halted */
#define  EBADE        52  /* Invalid exchange */
#define  EBADR        53  /* Invalid request descriptor */
#define  EXFULL       54  /* Exchange full */
#define  ENOANO       55  /* No anode */
#define  EBADRQC      56  /* Invalid request code */
#define  EBADSLT      57  /* Invalid slot */

#define  EDEADLOCK    EDEADLK

#define  EBFONT       59  /* Bad font file format */
#define  ENOSTR       60  /* Device not a stream */
#define  ENODATA      61  /* No data available */
#define  ETIME        62  /* Timer expired */
#define  ENOSR        63  /* Out of streams resources */
#define  ENONET       64  /* Machine is not on the network */
#define  ENOPKG       65  /* Package not installed */
#define  EREMOTE      66  /* Object is remote */
#define  ENOLINK      67  /* Link has been severed */
#define  EADV         68  /* Advertise error */
#define  ESRMNT       69  /* Srmount error */
#define  ECOMM        70  /* Communication error on send */
#define  EPROTO       71  /* Protocol error */
#define  EMULTIHOP    72  /* Multihop attempted */
#define  EDOTDOT      73  /* RFS specific error */
#define  EBADMSG      74  /* Not a data message */
#define  EOVERFLOW    75  /* Value too large for defined data type */
#define  ENOTUNIQ     76  /* Name not unique on network */
#define  EBADFD       77  /* File descriptor in bad state */
#define  EREMCHG      78  /* Remote address changed */
#define  ELIBACC      79  /* Can not access a needed shared library */
#define  ELIBBAD      80  /* Accessing a corrupted shared library */
#define  ELIBSCN      81  /* .lib section in a.out corrupted */
#define  ELIBMAX      82  /* Attempting to link in too many shared libraries */
#define  ELIBEXEC     83  /* Cannot exec a shared library directly */
#define  EILSEQ       84  /* Illegal byte sequence */
#define  ERESTART     85  /* Interrupted system call should be restarted */
#define  ESTRPIPE     86  /* Streams pipe error */
#define  EUSERS       87  /* Too many users */
#define  ENOTSOCK     88  /* Socket operation on non-socket */
#define  EDESTADDRREQ 89  /* Destination address required */
#define  EMSGSIZE     90  /* Message too long */
#define  EPROTOTYPE   91  /* Protocol wrong type for socket */
#define  ENOPROTOOPT  92  /* Protocol not available */
#define  EPROTONOSUPPORT 93  /* Protocol not supported */
#define  ESOCKTNOSUPPORT 94  /* Socket type not supported */
#define  EOPNOTSUPP      95  /* Operation not supported on transport endpoint */
#define  EPFNOSUPPORT    96  /* Protocol family not supported */
#define  EAFNOSUPPORT    97  /* Address family not supported by protocol */
#define  EADDRINUSE      98  /* Address already in use */
#define  EADDRNOTAVAIL   99  /* Cannot assign requested address */
#define  ENETDOWN       100  /* Network is down */
#define  ENETUNREACH    101  /* Network is unreachable */
#define  ENETRESET      102  /* Network dropped connection because of reset */
#define  ECONNABORTED   103  /* Software caused connection abort */
#define  ECONNRESET     104  /* Connection reset by peer */
#define  ENOBUFS        105  /* No buffer space available */
#define  EISCONN        106  /* Transport endpoint is already connected */
#define  ENOTCONN       107  /* Transport endpoint is not connected */
#define  ESHUTDOWN      108  /* Cannot send after transport endpoint shutdown */
#define  ETOOMANYREFS   109  /* Too many references: cannot splice */
#define  ETIMEDOUT      110  /* Connection timed out */
#define  ECONNREFUSED   111  /* Connection refused */
#define  EHOSTDOWN      112  /* Host is down */
#define  EHOSTUNREACH   113  /* No route to host */
#define  EALREADY       114  /* Operation already in progress */
#define  EINPROGRESS    115  /* Operation now in progress */
#define  ESTALE         116  /* Stale NFS file handle */
#define  EUCLEAN        117  /* Structure needs cleaning */
#define  ENOTNAM        118  /* Not a XENIX named type file */
#define  ENAVAIL        119  /* No XENIX semaphores available */
#define  EISNAM         120  /* Is a named type file */
#define  EREMOTEIO      121  /* Remote I/O error */
#define  EDQUOT         122  /* Quota exceeded */

#define  ENOMEDIUM      123  /* No medium found */
#define  EMEDIUMTYPE    124  /* Wrong medium type */

#ifndef errno
extern int errno;
#endif

#endif /* ERRNO */

#ifdef __cplusplus
}
#endif

#endif /* __LWIP_ARCH_H__ */
