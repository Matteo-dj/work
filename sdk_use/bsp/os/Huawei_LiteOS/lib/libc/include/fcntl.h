/*----------------------------------------------------------------------------
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

/** @defgroup fcntl Fcntl
 *  @ingroup filesystem
 */

#ifndef _HWLITEOS_POSIX_FCNTL_H
#define _HWLITEOS_POSIX_FCNTL_H

#include "sys/types.h"
#include "sys/ioctl.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


#define O_ACCMODE     0003   /* Mask for file access modes */
#define O_RDONLY        00   /* Open for reading only */
#define O_WRONLY        01   /* Open for writing only */
#define O_RDWR          02   /* Open for reading and writing */
#define O_CREAT       0100   /* Create file if it does not exist */
#define O_EXCL        0200   /* Exclusive use flag */
#define O_NOCTTY      0400   /* Do not assign controlling terminal */
#define O_TRUNC      01000   /* Truncate flag */
#define O_APPEND     02000  /* Set append mode */
#define O_SYNC      010000  /* Write according to synchronized I/O file integrity completion */
#define O_FSYNC     O_SYNC
#define O_ASYNC     020000

#define O_DIRECTORY 040000    /* Must be a directory */
#define O_NOFOLLOW  0100000   /* Do not follow links */
#define O_BINARY    0400000
#define O_DIRECT    0200000   /* Direct disk access */
#define O_NOATIME   01000000  /* Do not set atime */
#define O_CLOEXEC   02000000  /* Set close_on_exec */

#define O_RDOK      O_RDONLY
#define O_WROK      O_WRONLY

#define F_DUPFD     0   /* Duplicate file descriptor */
#define F_GETFD     1   /* Get file descriptor flags */
#define F_SETFD     2   /* Set file descriptor flags */
#define F_GETFL     3   /* Get file status flags */
#define F_SETFL     4   /* Set file status flags */
#define F_GETLK     5   /* Get record locking information */
#define F_SETLK     6   /* Set record locking information */
#define F_SETLKW    7   /* Set record locking information; wait if blocked */

#define F_SETOWN    8   /* Set process or process group ID to receive SIGURG signals */
#define F_GETOWN    9   /* Get process or process group ID to receive SIGURG signals */

#define FD_CLOEXEC  1   /* Close the file descriptor upon execution of an exec family function */

#define F_RDLCK     0   /* Shared or read lock */
#define F_WRLCK     1   /* Exclusive or write lock  */
#define F_UNLCK     2   /* Unlock */

struct flock
{
    short int l_type;   /* Type of lock; F_RDLCK, F_WRLCK, or F_UNLCK */
    short int l_whence; /* Flag for starting offset */
    off_t l_start;      /* Relative offset in bytes */
    off_t l_len;        /* Size; if 0 then until EOF */
    pid_t l_pid;        /* Process ID of the process holding the lock; returned with F_GETLK */
};

#define FAPPEND     O_APPEND
#define FFSYNC      O_FSYNC
#define FASYNC      O_ASYNC
#define FNONBLOCK   O_NONBLOCK
#define FNDELAY     O_NDELAY
#define FFCNTL      (FNONBLOCK | FNDELAY | FAPPEND | FFSYNC | FASYNC)
#define O_LARGEFILE 0400000

#ifndef O_NONBLOCK
#define O_NONBLOCK  04000     /* nonblocking I/O */
#endif

#ifndef O_NDELAY
#define O_NDELAY	O_NONBLOCK
#endif

struct flock64
{
    short int l_type;   /* Type of lock: F_RDLCK, F_WRLCK, or F_UNLCK.*/
    short int l_whence; /* Where `l_start' is relative to (like `lseek').*/
    long int l_start;   /* Offset where the lock begins.*/
    long int l_len;     /* Size of the locked area; zero means until EOF.*/
    int l_pid;          /* Process holding the lock.*/
};


/**
 * @ingroup  fcntl
 * @brief Open a file.
 *
 * @par Description:
 * This API used to open a file.
 *
 * @attention
 * <ul>
 * <li>The parameter path should be a valid string.</li>
 * </ul>
 *
 * @param  path     [IN] Type #const char *   The path of the file.
 * @param  oflags   [IN] Type #int            The limits to open.
 *
 * @retval #int                  On success, return the file descriptor.
 * @retval #-1                   On failed.
 * @par Dependency:
 * <ul><li>fcntl.h: the header file that contains the API declaration.</li></ul>
 * @see close
 * @since Huawei LiteOS V100R001C00
 */

extern int open(const char *path, int oflags, ...);
extern int close(int fd);

/**
 * @ingroup  fcntl
 * @brief The standard, POSIX read interface.
 *
 * @par Description:
 * read() attempts to read up to count bytes from file descriptor fd into the
 * buffer starting at buf.
 *
 * @attention
 * <ul>
 * None
 * </ul>
 *
 * @param  file     [IN]  Type #int     File descriptor (or socket descriptor) to read.
 * @param  buf      [IN]  Type #void *  User-provided to save the data.
 * @param  nbytes   [IN]  Type #size_t  The maximum size of the user-provided buffer.
 *
 * @retval #ssize_t    Number of bytes read success.
 * @retval #0          End-of-file condition.
 * @retval #-1         Failure and errno is set appropriately.
 *
 * @par Dependency:
 * <ul><li>fcntl.h: the header file that contains the API declaration.</li></ul>
 * @see
 *
 * @since Huawei LiteOS V100R001C00
 */
extern ssize_t read(int fd, void *buf, size_t nbytes);

/**
 * @ingroup  fcntl
 * @brief Create a file.
 *
 * @par Description:
 * This API used to create a file.
 *
 * @attention
 * <ul>
 * <li>The param mode is not supported.</li>
 * <li>creat() is equivalent to open() with flags equal to O_CREAT|O_WRONLY|O_TRUNC.</li>
 * </ul>
 *
 * @param  path     [IN] Type #const char *   The path of the file.
 * @param  mode   [IN] Type #mode_t            The mode of the file.
 *
 * @retval #int                  Succeed, return the file descriptor.
 * @retval #-1                   Failed.
 * @par Dependency:
 * <ul><li>fcntl.h: the header file that contains the API declaration.</li></ul>
 * @see close
 * @since Huawei LiteOS V100R001C00
 */
extern int creat(const char * path, mode_t mode);

/**
 * @ingroup  fcntl
 * @brief Performs one of the operations described below on the open file descriptor fd.
 * The operation is determined by cmd..
 *
 * @par Description:
 * This API can take an optional third argument.  Whether or not this argument is
 * required is determined by cmd.  The required argument type is indicated in parentheses
 * after each cmd name (in most cases, the required type is long, and we identify the
 * argument using the name arg), or void is specified if the argument is not required.
 *
 *
 * @attention
 * <ul>
 * <li>The parameter fd should be a valid file descriptor.</li>
 * </ul>
 *
 * @param  s  [IN] Type #int   The file/socket descriptor want to operate.
 * @param  cmd [IN] Type #int   The command for operation.
 * @param  val [IN] Type #none  The argument of the fcntl cmd. 
 *
 * @retval #int                 Operate success.
 * @retval #-1                  Operate failed.
 * @par Dependency:
 * <ul><li>fcntl.h: the header file that contains the API declaration.</li></ul>
 * @see none
 * @since Huawei LiteOS V100R001C00
 */
extern int fcntl(int s, int cmd, int val);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
