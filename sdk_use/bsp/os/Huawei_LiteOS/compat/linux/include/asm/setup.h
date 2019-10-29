
#ifndef __ASMARM_SETUP_H
#define __ASMARM_SETUP_H

#include <linux/types.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef struct tagCmdLine {
    char    cmdline[1];    /* this is the minimum size */
} tag_cmdline;

typedef struct tagMem32 {
    u_int    size;
    u_int    start;    /* physical start address */
} tag_mem32;

typedef struct tagInitRd {
    u_int start;    /* physical start address */
    u_int size;    /* size of compressed ramdisk image in bytes */
} tag_initrd;

typedef struct tagCore {
    u_int flags;        /* bit 0 = read-only */
    u_int pagesize;
    u_int rootdev;
} tag_core;

typedef struct tagMemClk {
    u_int fmemclk;
} tag_memclk;

typedef struct tagRamDisk {
    u_int flags;    /* bit 0 = load, bit 1 = prompt */
    u_int size;    /* decompressed ramdisk size in _kilo_ bytes */
    u_int start;    /* starting block of floppy-based RAM disk image */
} tag_ramdisk;

typedef struct tagSerialNr {
    u_int low;
    u_int high;
} tag_serialnr;

typedef struct tagRevision {
    u_int rev;
} tag_revision;

typedef struct tagHeader {
    u_int size;
    u_int tag;
} tag_header;

typedef struct tagVideoLfb {
    u_short        lfb_width;
    u_short        lfb_height;
    u_short        lfb_depth;
    u_short        lfb_linelength;
    u_int        lfb_base;
    u_int        lfb_size;
    u_char        red_size;
    u_char        red_pos;
    u_char        green_size;
    u_char        green_pos;
    u_char        blue_size;
    u_char        blue_pos;
    u_char        rsvd_size;
    u_char        rsvd_pos;
} tag_videolfb;

typedef struct tagVideoText {
    u_char        x;
    u_char        y;
    u_short        video_page;
    u_char        video_mode;
    u_char        video_cols;
    u_short        video_ega_bx;
    u_char        video_lines;
    u_char        video_isvga;
    u_short        video_points;
} tag_videotext;

typedef struct tagAcorn {
    u_int memc_control_reg;
    u_int vram_pages;
    u_char sounddefault;
    u_char adfsdrives;
} tag_acorn;

struct tag {
    tag_header hdr;
    union {
        tag_core           core;
        tag_mem32       mem;
        tag_videotext    videotext;
        tag_ramdisk      ramdisk;
        tag_initrd          initrd;
        tag_serialnr      serialnr;
        tag_revision      revision;
        tag_videolfb      videolfb;
        tag_cmdline      cmdline;

        /*
         * Acorn specific
         */
        tag_acorn    acorn;

        /*
         * DC21285 specific
         */
        tag_memclk    memclk;
    } u;
};

typedef struct tagTable {
    u_int tag;
    int (*parse)(const struct tag *);
} tag_table;

#define __tag  __attribute__((__section__(".taglist.init")))
#define __tagtable(tag, fn) \
static const tag_table __tagtable_##fn __tag = { tag, fn }

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif
