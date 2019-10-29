/** @defgroup ioctl Ioctl
 *  @ingroup filesystem
 */
 
#ifndef LIBC_SYS_IOCTL_H
#define LIBC_SYS_IOCTL_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#include <net/route.h>
#include <net/if.h>

#define IOCTLPARM_MASK    0x1fff      /* parameter length, at most 13 bits */
#define IOCTLPARM_LEN(x)  (((x) >> 16) & IOCTLPARM_MASK)
#define IOCTLBASECMD(x)   ((x) & ~(IOCTLPARM_MASK << 16))
#define IOCTLGROUP(x)     (((x) >> 8) & 0xff)

#define IOCTL_DIRMASK     0xe0000000  /* mask for IN/OUT/VOID */
#define IOCTL_OUT         0x80000000  /* copy out parameters */
#define IOCTL_IN          0x40000000  /* copy in parameters */
#define IOCTL_VOID        0x00000000  /* no parameters */
#define IOCTL_INOUT       (IOCTL_IN|IOCTL_OUT)

#define IOC_OUT           IOCTL_OUT

#define _IOC(inout, group, num, len) \
    ((unsigned long)(inout | ((len & IOCTLPARM_MASK) << 16) | ((group) << 8) | (num)))
#define _IOW(g,n,t)     _IOC(IOCTL_IN,    (g), (n), sizeof(t))
#define _IOR(g,n,t)     _IOC(IOCTL_OUT,   (g), (n), sizeof(t))
#define _IO(g,n)        _IOC(IOCTL_VOID,  (g), (n), 0)
/* this should be _IORW, but stdio got there first */
#define _IOWR(g,n,t)    _IOC(IOCTL_INOUT, (g), (n), sizeof(t))

#ifdef CONFIG_LIBC_IOCTL_VARIADIC
extern int fs_ioctl(int fd, int req, ...);
#else
/**
 * @ingroup  ioctl
 * @brief Perform device specific operations.
 *
 * @par Description:
 * This API used to perform device specific operations.
 *
 * @attention
 * <ul>
 * <li>The parameter fd should be a valid file/socket descriptor.</li>
 * </ul>
 *
 * @param  s  [IN] Type #int   File/socket descriptor of device.
 * @param  cmd [IN] Type #long   The ioctl command.
 * @param  argp [IN] Type #none  The argument of the ioctl cmd.
 *
 * @retval #>=0                   On success(positive non-zero values are cmd-specific).
 * @retval #-1                    Failed.
 * @par Dependency:
 * <ul><li>ioctl.h: the header file that contains the API declaration.</li></ul>
 * @see none
 * @since Huawei LiteOS V100R001C00
 */
extern int ioctl(int s, long cmd, void *argp);

#endif

#define _IOC_NRBITS     8
#define _IOC_TYPEBITS   8
#ifndef _IOC_SIZEBITS
# define _IOC_SIZEBITS  14
#endif
#ifndef _IOC_DIRBITS
# define _IOC_DIRBITS   2
#endif
#define _IOC_NRMASK     ((1 << _IOC_NRBITS) - 1)
#define _IOC_TYPEMASK   ((1 << _IOC_TYPEBITS) - 1)
#define _IOC_SIZEMASK   ((1 << _IOC_SIZEBITS) - 1)
#define _IOC_DIRMASK    ((1 << _IOC_DIRBITS) - 1)
#define _IOC_NRSHIFT    0
#define _IOC_TYPESHIFT  (_IOC_NRSHIFT + _IOC_NRBITS)
#define _IOC_SIZESHIFT  (_IOC_TYPESHIFT + _IOC_TYPEBITS)
#define _IOC_DIRSHIFT   (_IOC_SIZESHIFT + _IOC_SIZEBITS)
#define _IOC_DIR(nr)    (((nr) >> _IOC_DIRSHIFT) & _IOC_DIRMASK)
#define _IOC_TYPE(nr)   (((nr) >> _IOC_TYPESHIFT) & _IOC_TYPEMASK)
#define _IOC_NR(nr)     (((nr) >> _IOC_NRSHIFT) & _IOC_NRMASK)
#define _IOC_SIZE(nr)   (((nr) >> _IOC_SIZESHIFT) & _IOC_SIZEMASK)

#define SIOCADDRT   _IOW('R', 9, struct rtentry)

#define SIOCGIFFLAGS  _IOWR('i', 17, struct ifreq)  /* get ifnet flags */
#define SIOCSIFFLAGS   _IOW('i', 16, struct ifreq)  /* set ifnet flags */
#define SIOCGIFADDR _IOWR('i', 33, struct ifreq)  /* get ifnet address */
#define SIOCSIFADDR  _IOW('i', 12, struct ifreq)  /* set ifnet address */
#define SIOCGIFNETMASK  _IOWR('i', 37, struct ifreq)  /* get net addr mask */
#define SIOCSIFNETMASK   _IOW('i', 22, struct ifreq)  /* set net addr mask */
#define SIOCSIFHWADDR  _IOW('i', 140, struct ifreq) /* set IF name */
#define SIOCGIFHWADDR  _IOW('i', 141, struct ifreq) /* set IF name */
#define SIOCGIFNAME  _IOW('i', 142, struct ifreq) /* set IF name */
#define SIOCSIFNAME  _IOW('i', 40, struct ifreq)  /* set IF name */

#ifndef FIONREAD
#define FIONREAD    _IOR('f', 127, unsigned long) /* get # bytes to read */
#endif
#ifndef FIONBIO
#define FIONBIO     _IOW('f', 126, unsigned long) /* set/clear non-blocking i/o */
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* LIBC_SYS_IOCTL_H */
