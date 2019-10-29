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

/** @defgroup stat Stat
 *  @ingroup filesystem
 */

#ifndef _HWLITEOS_POSIX_SYS_STAT_H
#define _HWLITEOS_POSIX_SYS_STAT_H

#include "types.h"
#include "time.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


#define st_atime    st_atim.tv_sec
#define st_mtime    st_mtim.tv_sec
#define st_ctime    st_ctim.tv_sec

struct stat
{
    dev_t st_dev;               /* Device ID of device containing file. */
    unsigned short int __pad1;
    ino_t st_ino;               /* File serial number. */
    mode_t st_mode;             /* Mode of file. */
    nlink_t st_nlink;           /* Number of hard links to the file. */
    uid_t st_uid;               /* User ID of file. */
    gid_t st_gid;               /* Group ID of file. */
    dev_t st_rdev;              /* Device ID(if file is character or block special). */
    unsigned short int __pad2;
    off_t st_size;              /* For regular files, the file size in bytes.
                                   For symbolic links, the length in bytes of the
                                   pathname contained in the symbolic link.
                                   For a shared memory object, the length in bytes.
                                   For a typed memory object, the length in bytes.
                                   For other file types, the use of this field is unspecified. */
    blksize_t st_blksize;       /* A file system-specific preferred I/O block size for
                                   this object. In some file system types, this may
                                   vary from file to file. */

    blkcnt_t st_blocks;         /* Number of blocks allocated for this object. */

    struct timespec st_atim;    /* Time of last access. */
    struct timespec st_mtim;    /* Time of last data modification. */
    struct timespec st_ctim;    /* Time of last status change. */

    unsigned long int __unused4;
    unsigned long int __unused5;
};

struct utimbuf {
    time_t actime;    /* access time */
    time_t modtime;    /* modification time */
};

#define	S_IFMT	0170000	/* Type of file.  */

#define	S_IFDIR     0040000	/* Directory. */
#define	S_IFCHR     0020000	/* Character special. */
#define	S_IFBLK     0060000	/* Block special. */
#define	S_IFREG     0100000	/* Regular. */
#define	S_IFIFO     0010000	/* FIFO special. */
#define	S_IFLNK     0120000	/* Symbolic link. */
#define	S_IFSOCK    0140000	/* Socket. */

/* Test macros for file types.	*/

#define	__S_ISTYPE(mode, mask)		(((mode) & S_IFMT) == (mask))

#define	S_ISBLK(mode)	 __S_ISTYPE((mode), S_IFBLK) /* Test for a block special file. */
#define	S_ISCHR(mode)	 __S_ISTYPE((mode), S_IFCHR) /* Test for a character special file. */
#define	S_ISDIR(mode)	 __S_ISTYPE((mode), S_IFDIR) /* Test for a directory */
#define S_ISFIFO(mode)	 __S_ISTYPE((mode), S_IFIFO) /* Test for a pipe or FIFO special file. */
#define	S_ISREG(mode)	 __S_ISTYPE((mode), S_IFREG) /* Test for a regular file. */
#define S_ISLNK(mode)	 __S_ISTYPE((mode), S_IFLNK) /* Test for a symbolic link. */
#define S_ISSOCK(mode)   __S_ISTYPE((mode), S_IFSOCK) /* Test for a socket. */

#define	S_IRUSR	0400	/* Read permission, owner. */
#define	S_IWUSR	0200	/* Write permission, owner. */
#define	S_IXUSR	0100	/* Execute/search permission, owner.  */
/* Read, write, execute/search by owner */
#define	S_IRWXU	(S_IRUSR | S_IWUSR | S_IXUSR)

#define	S_ISUID	04000	/* Set-user-ID on execution. */
#define	S_ISGID	02000	/* Set-group-ID on execution. */
#define	S_ISVTX	01000	/* On directories, restricted deletion flag. */

#define	S_IRGRP	(S_IRUSR >> 3)	/* Read permission, group. */
#define	S_IWGRP	(S_IWUSR >> 3)	/* Write permission, group. */
#define	S_IXGRP	(S_IXUSR >> 3)	/* Execute/search permission, group. */
/* Read, write, execute/search by group. */
#define	S_IRWXG	(S_IRGRP | S_IWGRP | S_IXGRP)

#define	S_IROTH	(S_IRGRP >> 3)	/* Read permission, others. */
#define	S_IWOTH	(S_IWGRP >> 3)	/* Write permission, others. */
#define	S_IXOTH	(S_IXGRP >> 3)	/* Execute/search permission, others. */
/* Read, write, execute/search by others. */
#define	S_IRWXO	(S_IROTH | S_IWOTH | S_IXOTH)

#define S_TYPEISMQ(buf)  ((buf)->st_mode - (buf)->st_mode) /* Test for a message queue. */
#define S_TYPEISSEM(buf) ((buf)->st_mode - (buf)->st_mode) /* Test for a semaphore. */
#define S_TYPEISSHM(buf) ((buf)->st_mode - (buf)->st_mode) /* Test for a shared memory object. */

#define S_IREAD	S_IRUSR
#define S_IWRITE	S_IWUSR
#define S_IEXEC	S_IXUSR

#define ACCESSPERMS	(S_IRWXU | S_IRWXG | S_IRWXO)
#define ALLPERMS	(S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)
#define DEFFILEMODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)
#define S_BLKSIZE	512

/* Fat File attribute bits for directory entry */

#define    F_RDO    0x01    /* Read only */
#define    F_HID    0x02    /* Hidden */
#define    F_SYS    0x04    /* System */
#define    F_ARC    0x20    /* Archive */


/**
 * @ingroup  stat
 * @brief  return information about a file, in the buffer .
 *
 * @par Description:
 * This API is used to return information about the file pointed to
 *      by pathname, in the buffer pointed to by buf.
 * @attention
 * <ul>
 * <li>The parameter path should be a valid string.</li>
 * <li>The parameter buf must point a valid memory.</li>
 * </ul>
 *
 * @param  uwParam1 [IN] Type #const char * The first parameter is a file  pathname.
 * @param  uwParam2 [OUT] Type #struct stat * The second parameter passed to a stat buffer to be obtained.
 *
 * @retval EFAULT     NULL parameter.
 * @retval ENOENT     Invalid pathname.
 * @retval VFS_ERROR  Stat failed.
 * @retval OK         Get the information of the file successfully.
 * @par Dependency:
 * <ul><li>stat.h: the header file that contains the API declaration.</li></ul>
 * @see
 * @since Huawei LiteOS V100R001C00
 */
extern int stat(const char *, struct stat *);

/**
 * @ingroup  stat
 * @brief  return information about a file, in the buffer .
 *
 * @par Description:
 * This API is used to return information about the file pointed to
        by the file descriptor fd, in the buffer pointed to by buf.
 * @attention
 * <ul>
 * <li>The parameter fildes should be a valid file descriptor.</li>
 * <li>The parameter buf must point a valid memory.</li>
 * </ul>
 *
 * @param  uwParam1 [IN] Type #int
 * The first parameter is a file descriptor fd.
 * @param  uwParam2 [OUT] Type #struct stat
 * The second parameter passed to a stat buffer to be obtained.
 *
 * @retval EFAULT    NULL parameter.
 * @retval ENOENT    Invalid pathname.
 * @retval VFS_ERROR Stat failed.
 * @retval OK        Get the information of the file successfully.
 * @par Dependency
 * <ul><li>stat.h: the header file that contains the API declaration.</li></ul>
 * @see
 * @since Huawei LiteOS V100R001C00
 */
extern int fstat(int, struct stat *);

/**
 * @ingroup  stat
 * @brief Create a directory.
 *
 * @par Description:
 * This API used to create a new directory.
 *
 * @attention
 * <ul>
 * <li>The parameter pathname should be a valid string.</li>
 * </ul>
 *
 * @param  pathname  [IN] Type #const char *   The path of the new directory.
 * @param  mode      [IN] Type #mode_t         The mode to create.
 *
 * @retval #0                    On success.
 * @retval #-1                   On failed.
 * @par Dependency:
 * <ul><li>stat.h: the header file that contains the API declaration.</li></ul>
 * @see none
 * @since Huawei LiteOS V100R001C00
 */
extern int mkdir(const char *pathname, mode_t mode);

/**
 * @ingroup  stat
 * @brief  return information about a file, in the buffer .
 *
 * @par Description:
 * This API is used to return information about the file pointed to
        by pathname, in the buffer pointed to by buf.
 * @attention
 * <ul>
 * <li>The parameter path should be a valid string.</li>
 * <li>The parameter buffer must point a valid memory.</li>
 * </ul>
 *
 * @param  uwParam1 [IN] Type #const char* The first parameter is a file  pathname.
 * @param  uwParam2 [OUT] Type #struct stat * The second parameter passed to a stat buffer  to be obtained.
 *
 * @retval EFAULT NULL parameter .
 * @retval ENOENT Invalid pathname.
 * @retval VFS_ERROR Stat failed.
 * @retval OK  Get the information of the file successfully.
 * @par Dependency:
 * <ul><li>stat.h: the header file that contains the API declaration.</li></ul>
 * @see stat
 * @since Huawei LiteOS V100R001C00
 */
extern int lstat(const char *, struct stat *);
/**
 * @ingroup  stat
 * @brief Set the attribute of file.
 *
 * @par Description:
 * This API is used to set a attribute of specified file.
 *
 * @attention
 * <ul>
 * <li>The parameter path should be a valid string.</li>
 * </ul>
 *
 * @param  path [IN] Type #const char *  The path of the file.
 * @param  mode [IN] Type #mode_t        The attributo to be set.
 *
 * @retval #0                   Set successfully.
 * @retval #-1                  Set failed.
 * @par Dependency:
 * <ul><li>stat.h: the header file that contains the API declaration.</li></ul>
 * @see none
 * @since Huawei LiteOS V100R001C00
 */
extern int chattr(const char *path, mode_t mode);

/**
 * @ingroup  stat
 * @brief  Set File Access and Modification Times.
 *
 * @par Description:
 *	 The utime() function sets the access and modification
 *     times of path to the values in the utimbuf structure.
 *	 If times is a NULL pointer, the access and modification
 *     times are set to the current time.
 *
 * @attention
 * <ul>
 * <li>The parameter path should be a valid string.</li>
 * <li>The parameter ptimes must point a valid memory.</li>
 * </ul>
 *
 * @param path      [IN]  Type #const char*  A pointer to the null-terminated path name of the file.
 * @param ptimes    [IN]  Type #const struct utimbuf *  A pointer to a structure utimbuf.
 *
 * @retval #VFS_ERROR                             Error.
 * @retval #OK                                    Success.
 * @retval #ENOENT                                No such file or directory.
 * @retval #EACCES                                Permission denied.
 * @retval #EINVAL                                Path is invalid argument.
 * @retval #ENOSUPP                               Operation not supported.
 *
 * @par Dependency:
 * <ul><li>stat.h: the header file that contains the API declaration.</li></ul>
 * @see utime
 * @since Huawei LiteOS V100R001C00
 */
 extern int utime(const char *path,  const struct utimbuf *ptimes);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
