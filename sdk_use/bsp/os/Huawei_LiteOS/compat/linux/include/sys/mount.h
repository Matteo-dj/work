
#ifndef _SYS_MOUNT_H
#define _SYS_MOUNT_H


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


#define MS_RDONLY 1 /* Mount file system read-only */

/* umount2 flags. */
#define MNT_FORCE    1    /* Forcibly unmount */
#define MNT_DETACH    2    /* Detach from tree only */
#define MNT_EXPIRE    4    /* Mark for expiry */

extern int mount(const char*, const char*, const char*, unsigned long, const void*);
extern int umount(const char*);
extern int umount2(const char*, int);



#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _SYS_MOUNT_H */

