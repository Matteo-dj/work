#ifndef __LINUX_SLAB_H__
#define __LINUX_SLAB_H__

#include <stdlib.h>

#include <asm/page.h> /* Don't ask. Linux headers are a mess. */


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define GFP_KERNEL    (0)
#define GFP_ATOMIC    (0)

#define kmalloc(x, y) malloc(x)
#define kfree(x) free(x)
#define vmalloc(x) malloc(x)
#define vfree(x) free(x)


#define kzalloc(s, f)       calloc(1, s)


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __LINUX_SLAB_H__ */

