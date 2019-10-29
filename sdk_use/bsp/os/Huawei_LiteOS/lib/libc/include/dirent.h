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

/** @defgroup dirent Dirent
 *  @ingroup filesystem
 */

#ifndef _HWLITEOS_POSIX_DIRENT_H
#define _HWLITEOS_POSIX_DIRENT_H

#include "sys/types.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


struct dirent
{
    ino_t d_ino;        /* File serial number. */
    off_t d_off;
    unsigned short int d_reclen;
    unsigned char d_type;
    char d_name[256];   /* Name of entry. */
};

/* File types for `d_type'.  */
enum
{
    DT_UNKNOWN = 0,
# define DT_UNKNOWN DT_UNKNOWN
    DT_FIFO = 1,
# define DT_FIFO    DT_FIFO
    DT_CHR = 2,
# define DT_CHR     DT_CHR
    DT_DIR = 4,
# define DT_DIR     DT_DIR
    DT_BLK = 6,
# define DT_BLK     DT_BLK
    DT_REG = 8,
# define DT_REG     DT_REG
    DT_LNK = 10,
# define DT_LNK     DT_LNK
    DT_SOCK = 12,
# define DT_SOCK    DT_SOCK
    DT_WHT = 14
# define DT_WHT     DT_WHT
};


/**
 * @ingroup  dirent
 * @brief close a directory
 *
 * @par Description:
 * This API is used to close the directory stream associated with dirp.
 *
 * @attention
 * <ul>
 * <li>none.</li>
 * </ul>
 *
 * @param  dirp [IN] Pointer of Directory object structure, which returned by function opendir.
 *
 * @retval #VFS_ERROR                        The directory dirp close unsuccessfully and an errno is set.
 * @retval #OK                               The directory dirp close successfully.
 * @par Dependency:
 * <ul><li>dirent.h: the header file that contains the API declaration.</li></ul>
 * @see opendir
 * @since Huawei LiteOS V100R001C00
 */
extern int closedir(DIR *dirp);

/**
 * @ingroup  dirent
 * @brief open a directory
 *
 * @par Description:
 * This API is used to open a directory stream corresponding to the directory name, and
 * returns a pointer to the directory stream.
 *
 * @attention
 * <ul>
 * <li>The length of the parameter path must be less than the value defined by PATH_MAX.</li>
 * </ul>
 *
 * @param  path [IN] the directory to open.
 *
 * @retval #NULL                          Open directory unsuccessfully and an errno is set.
 * @retval #DIR*                          A pointer to the directory stream.
 * @par Dependency:
 * <ul><li>dirent.h: the header file that contains the API declaration.</li></ul>
 * @see closedir
 * @since Huawei LiteOS V100R001C00
 */
extern DIR *opendir(const char *path);

/**
 * @ingroup  dirent
 * @brief read a directory
 *
 * @par Description:
 * This API is used to get a pointer to a dirent structure
 * representing the next directory entry in the directory stream pointed
 * to by dirp.
 *
 * @attention
 * <ul>
 * <li>none.</li>
 * </ul>
 *
 * @param  dirp [IN] An instance of type DIR returned by a previous call of opendir().
 *
 * @retval #NULL                Reaching the end of the directory stream or if an error occurred and an errno is set.
 * @retval #dirent*             A pointer to a dirent structure.
 * @par Dependency:
 * <ul><li>dirent.h: the header file that contains the API declaration.</li></ul>
 * @see opendir
 * @since Huawei LiteOS V100R001C00
 */
extern struct dirent *readdir(DIR *dirp);

/**
 * @ingroup  dirent
 * @brief  reset directory stream
 *
 * @par Description:
 * This API is used to reset the position of the
 * directory stream dir to the beginning of the directory.
 *
 * @attention
 * <ul>
 * <li>none.</li>
 * </ul>
 *
 * @param  dirp [IN] An instance of type DIR returned by a previous call of opendir().
 *
 * @retval #NONE                    An error occurred, the errno is set.
 * @par Dependency:
 * <ul><li>dirent.h: the header file that contains the API declaration.</li></ul>
 * @see telldir
 * @since Huawei LiteOS V100R001C00
 */
extern void rewinddir(DIR *dirp);

/**
 * @ingroup  dirent
 * @brief  set the position of the next readdir() call in the directory stream
 *
 * @par Description:
 * This API is used to set the location in the directory stream from
 * which the next readdir() call will start.  seekdir() should be used with
 * an offset returned by telldir().
 *
 * @attention
 * <ul>
 * <li>none.</li>
 * </ul>
 *
 * @param  dirp   [IN] An instance of type DIR returned by a previous call of opendir().
 * @param  offset [IN] offset to seek to
 *
 * @retval #NONE                      An error occurred, the errno is set.
 * @par Dependency:
 * <ul><li>dirent.h: the header file that contains the API declaration.</li></ul>
 * @see telldir
 * @since Huawei LiteOS V100R001C00
 */
extern void seekdir(DIR *dirp, off_t offset);

/**
 * @ingroup  dirent
 * @brief  get current location in directory stream.
 *
 * @par Description:
 * This API is used to get the current location
 * associated with the directory stream dirp.
 *
 * @attention
 * <ul>
 * <li>none.</li>
 * </ul>
 *
 * @param  dirp   [IN]An instance of type DIR returned by a previous call of opendir().
 *
 * @retval #-1                          An error occur, it will set errno.
 * @retval #off_t                       The current location in the directory stream.
 * @par Dependency:
 * <ul><li>dirent.h: the header file that contains the API declaration.</li></ul>
 * @see seekdir
 * @since Huawei LiteOS V100R001C00
 */
extern off_t telldir(DIR *dirp);

/**
 * @ingroup  dirent
 * @brief   string comparison using collating information.
 *
 * @par Description:
 *   The alphasort() function can be used as the comparison function(compar) for scandir.
 *
 * @attention
 * <ul>
 * <li>The parameter a and b can not be null, otherwise the system may crash.</li>
 * </ul>
 *
 * @param a [IN]    Type #const struct dirent**  a pointer to directory information pointer.
 * @param b [IN]    Type #const struct dirent**  a pointer to directory information pointer.
 *
 * @retval #>0      The 'a' name entry is greater than 'b'.
 * @retval #=0      The 'a' name entry is equal to 'b'.
 * @retval #<0      The 'a' name entry is less than 'b'.
 * @par Dependency:
 * <ul><li>dirent.h: the header file that contains the API declaration.</li></ul>
 * @see alphasort
 * @since Huawei LiteOS V100R001C00
 */
 extern int alphasort(const struct dirent** a, const struct dirent** b);

/**
 * @ingroup  dirent
 * @brief     scan a directory for matching entries.
 *
 * @par Description:
 *   The scandir() function scans the directory dirp, calling filter() on
 *   each directory entry.  Entries for which filter() returns nonzero are
 *   stored in strings allocated via malloc, sorted using qsort with
 *   the comparison function compar(), and collected in array namelist
 *   which is allocated via malloc.  If filter is NULL, all entries are
 *   selected.
 *
 * @attention
 * <ul>
 * <li>If return no error, don't forget to free the memory pointed by parameter namelist</li>
 * </ul>
 *
 * @param dir         [IN]    Type #const char*  The path of the directory, the length must be less than the value defined by PATH_MAX.
 * @param namelist    [OUT]   Type #const struct dirent***   a pointer to collected directory entries
 * @param filter      [IN]    Type #int(*filter)(const struct dirent*)    a filter type function
 * @param compar      [IN]    Type #int(*compar)(const struct dirent**,const struct dirent**)  a compar type function
 *
 * @retval #int         The number of directory entries selected.
 * @retval #<0          An error occur, it will set errno.
 * @par Dependency:
 * <ul><li>dirent.h: the header file that contains the API declaration.</li></ul>
 * @see scandir
 * @since Huawei LiteOS V100R001C00
 */
extern int scandir(const char *dir, struct dirent ***namelist,
        int (*filter)(const struct dirent *),
        int (*compar)(const struct dirent **,
            const struct dirent **));


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
