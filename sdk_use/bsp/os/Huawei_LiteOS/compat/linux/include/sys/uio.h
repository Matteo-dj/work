/** @defgroup uio Uio
 *  @ingroup filesystem
*/

#ifndef _SYS_UIO_H
#define _SYS_UIO_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


/* Structure for scatter/gather I/O. */
struct iovec{
    void *iov_base; /* Pointer to data. */
    size_t iov_len; /* Length of data. */
};

/**
 * @ingroup  uio
 * @brief Read into multiple buffers.
 *
 * @par Description:
 * The readv() function reads iovcnt buffers from the file associated with the
 * file descriptor fd into the buffers described by iov ("scatter input").The
 * readv() function works just like read() except that multiple buffers are filled.
 *
 * @attention
 * <ul>
 * <li>The parameter fd should be a valid file descriptor.</li>
 * <li>The parameter iov must point a valid memory.</li>
 * </ul>
 *
 * @param  fd       [IN]  Type #int             File descriptor (or socket descriptor) to read.
 * @param  iov      [IN]  Type #struct iovec *  A pointer to a data buf.
 * @param  iovcnt   [IN]  Type #int  The read vector counter.
 *
 * @retval #>0    Number of bytes read success.
 * @retval #0     End-of-file condition.
 * @retval #-1    Failure and errno is set appropriately.
 *
 * @par Dependency:
 * <ul><li>uio.h: the header file that contains the API declaration.</li></ul>
 * @see
 *
 * @since Huawei LiteOS V100R001C00
 */
extern ssize_t readv(int fd, const struct iovec *iov, int iovcnt);

/**
 * @ingroup uio
 * @brief  write a vector.
 *
 * @par Description:
 *  The writev() function shall be equivalent to write(), except as described below.
 *  The writev() function shall gather output data from the iovcnt buffers specified by the members of the iov array.
 *
 * @attention
 * <ul>
 * <li>The parameter fd should be a valid file descriptor.</li>
 * <li>The parameter iov must point a valid memory.</li>
 * </ul>
 *
 * @param fd    [IN]  Type #int A file descriptor
 * @param iov      [IN]  Type #const struct iovec *  A pointer to a data buf
 * @param iovcnt    [IN]  Type #int The write vector counter.
 *
 * @retval #VFS_ERROR                          Error.
 * @retval #>0                                 Number of bytes written.
 *
 * @par Dependency:
 * <ul><li>uio.h: the header file that contains the API declaration.</li></ul>
 * @see writev
 * @since Huawei LiteOS V100R001C00*/
extern ssize_t writev(int fd, const struct iovec *iov, int iovcnt);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _SYS_UIO_H */
