#ifndef _LINUX_FB_H
#define _LINUX_FB_H

#include "stdlib.h"
#include <linux/kernel.h>
#include <linux/workqueue.h>
#include "linux/i2c.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


/* Definitions of frame buffers                        */
#define FB_MAX            32

#ifndef __iomem
#define __iomem
#endif

/* ioctls
   0x46 is 'F'                                */
#define FBIOPAN_DISPLAY        0x4606
#define FBIOPUTCMAP        0x4605
#define FBIOGETCMAP        0x4604
#define FBIOGET_VSCREENINFO    0x4600
#define FBIOPUT_VSCREENINFO    0x4601
#define FBIOGET_FSCREENINFO    0x4602

#define FB_TYPE_PACKED_PIXELS        0    /* Packed Pixels    */
#define FB_VISUAL_TRUECOLOR        2    /* True color    */
#define FB_ACCEL_NONE        0    /* no hardware accelerator    */




struct fb_bitfield {
    u_long offset;
    u_long length;
    u_long msb_right;
};

#define FB_ACTIVATE_NOW        0    /* set values immediately (or vbl)*/
#define FB_ACTIVATE_MASK       15
#define FB_ACTIVATE_VBL           16    /* activate values on next vbl  */
#define FB_ACTIVATE_FORCE     128    /* force apply even when no change*/
#define FB_ACTIVATE_INV_MODE  256       /* invalidate videomode */
#define FB_VMODE_YWRAP        256    /* ywrap instead of panning     */

struct fb_var_screeninfo {
    u_long xres;
    u_long yres;
    u_long xres_virtual;
    u_long yres_virtual;
    u_long xoffset;
    u_long yoffset;

    u_long bits_per_pixel;
    u_long grayscale;
    struct fb_bitfield red;
    struct fb_bitfield green;
    struct fb_bitfield blue;
    struct fb_bitfield transp;
    u_long nonstd;
    u_long activate;
    u_long height;
    u_long width;
    u_long accel_flags;
    u_long pixclock;
    u_long left_margin;
    u_long right_margin;
    u_long upper_margin;
    u_long lower_margin;
    u_long hsync_len;
    u_long vsync_len;
    u_long sync;
    u_long vmode;
    u_long rotate;
    u_long reserved[5];
};

struct fb_fix_screeninfo {
    char id[16];
    u_long smem_start;
    u_long smem_len;
    u_long type;
    u_long type_aux;
    u_long visual;
    u_short xpanstep;
    u_short ypanstep;
    u_short ywrapstep;
    u_long line_length;
    u_long mmio_start;
    u_long mmio_len;
    u_long accel;
    u_short reserved[3];
};
struct fb_cmap {
    u_long start;
    u_long len;
    u_short *red;
    u_short *green;
    u_short *blue;
    u_short *transp;            /* can be NULL */
};

struct vm_area_struct;
struct fb_info;
struct fb_ops {
    int (*fb_open)(struct fb_info *info, int user);
    int (*fb_release)(struct fb_info *info, int user);

    /* pan display */
    int (*fb_pan_display)(struct fb_var_screeninfo *var, struct fb_info *info);

    /* perform fb specific ioctl (optional) */
    int (*fb_ioctl)(struct fb_info *info, unsigned int cmd,
            unsigned long arg);
    /* checks var and eventually tweaks it to something supported,
     * DO NOT MODIFY PAR */
    int (*fb_check_var)(struct fb_var_screeninfo *var, struct fb_info *info);

    /* set the video mode according to info->var */
    int (*fb_set_par)(struct fb_info *info);
    /* set color registers in batch */
    int (*fb_setcmap)(struct fb_cmap *cmap, struct fb_info *info);
    /* set color register */
    int (*fb_setcolreg)(unsigned regno, unsigned red, unsigned green,
                unsigned blue, unsigned transp, struct fb_info *info);

    struct module *owner;
};


/* FBINFO_* = fb_info.flags bit flags */
#define FBINFO_MODULE                    0x0001    /* Low-level driver is a module */
#define FBINFO_HWACCEL_XPAN        0x1000 /* optional */
#define FBINFO_HWACCEL_YPAN        0x2000 /* optional */
#define FBINFO_MISC_USEREVENT      0x10000 /* event request from userspace */

struct fb_info {
    atomic_t count;
    int node;
    int flags;
    pthread_mutex_t lock;
    struct fb_var_screeninfo var;    /* Current var */
    struct fb_fix_screeninfo fix;    /* Current fix */
    struct fb_cmap cmap;        /* Current cmap */
    struct fb_ops *fbops;
    char __iomem *screen_base;    /* Virtual address */
    unsigned long screen_size;    /* Amount of ioremapped VRAM or 0 */
    void *par;
};

#ifdef MODULE
#define FBINFO_DEFAULT    FBINFO_MODULE
#else
#define FBINFO_DEFAULT    0
#endif

#define FBINFO_FLAG_DEFAULT    FBINFO_DEFAULT

/* drivers/video/fbmem.c */
extern int register_framebuffer(struct fb_info *info);
extern int unregister_framebuffer(struct fb_info *info);
extern int fb_set_var(struct fb_info *info, struct fb_var_screeninfo *var);
extern int fb_pan_display(struct fb_info *info, struct fb_var_screeninfo *var);
extern struct fb_info *framebuffer_alloc(size_t size, struct device *dev);
extern void framebuffer_release(struct fb_info *info);

/* drivers/video/fbcmap.c */
extern int fb_alloc_cmap(struct fb_cmap *cmap, int len, int transp);
extern void fb_dealloc_cmap(struct fb_cmap *cmap);
extern int fb_set_cmap(struct fb_cmap *cmap, struct fb_info *fb_info);
extern int fb_set_user_cmap(struct fb_cmap *cmap, struct fb_info *fb_info);
extern int fb_copy_cmap(const struct fb_cmap *from, struct fb_cmap *to);
extern int fb_cmap_to_user(const struct fb_cmap *from, struct fb_cmap *to);
extern const struct fb_cmap *fb_default_cmap(int len);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LINUX_FB_H */
