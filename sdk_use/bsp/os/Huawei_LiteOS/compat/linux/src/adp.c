#include "los_sys.ph"

#include "linux/kernel.h"
#include "linux/module.h"
#include "localtime.h"
#ifdef LOSCFG_FS_VFS
#include "linux/seq_file.h"
#include "sys/statfs.h"
#endif
#include "math.h"
#ifdef LOSCFG_NET_LWIP_SACK
#include "lwip/sockets.h"
#include "lwip/api.h"
#include "lwip/netdb.h"
#endif


#ifdef LOSCFG_FS_VFS
extern int fstat(int fildes, struct stat *buf);
#endif
extern UINT32 ticks_read(void);
extern struct hostent* lwip_gethostbyname(const char *name);

void
__assert (const char *assertion, const char *file, int line)
{
  PRINT_ERR("__assert error\n");
  while(1)
  {
    ;
  }
}

#ifdef LOSCFG_FS_VFS
void fs_show(const char *path)
{
    int ret = 0;
    struct statfs fss;
    if (NULL == path)
    {
        PRINTK("path is NULL\n", path);
    }
    ret = statfs(path, &fss);
    PRINTK("Filesystem %s info: \n", path);
    PRINTK("----------------------------------------\n");
    if (ret == ENOERR)
    {
        PRINTK("  Total clusters: %u \n", fss.f_blocks);
        PRINTK("  Cluster size: %u \n", fss.f_bsize);
        PRINTK("  Free clusters: %u \n", fss.f_bfree);
    }
    else
    {
        ret = get_errno();
        PRINT_ERR("Get fsinfo failed: %d \n", ret);
    }
}
#endif
long int syscall(long int sysno, ...)
{
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
    return ENOSUPP;
}

int fs_fssync( const char *path )
{
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
    return ENOSUPP;
}

int __tls_get_addr(void * th,int map_address, size_t offset, void * address)
{
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
    return ENOSUPP; // FIXME: TODO
}

#define assert(a) \
    do \
    {  \
        if(0 == (a)) \
        { \
            printf("assert error : %s %d\n",__FUNCTION__,__LINE__); \
            return; \
        } \
    } while(0)



#define MAX_JIFFY_OFFSET ((LONG_MAX >> 1)-1)

#define    MAX_SCHEDULE_TIMEOUT    LONG_MAX

unsigned long msecs_to_jiffies(const unsigned int m)
{
    /*
     * Negative value, means infinite timeout:
     */
    if ((int)m < 0)
        return MAX_JIFFY_OFFSET;

    /*
     * HZ is equal to or smaller than 1000, and 1000 is a nice
     * round multiple of HZ, divide with the factor between them,
     * but round upwards:
     */
    return LOS_MS2Tick(m);
}

UINT64 jiffies_to_tick(unsigned long j)
{
    return j;
}

signed long schedule_timeout(signed long timeout)
{
    UINT32   uwRet = 0;

    if (OS_INT_ACTIVE) {
        PRINT_ERR("ERROR: OS_ERRNO_SWTMR_HWI_ACTIVE\n");
        return LOS_ERRNO_SWTMR_HWI_ACTIVE;
    }

    uwRet = LOS_TaskDelay(timeout);

    if (uwRet == LOS_OK) {
        return uwRet;
    }
    else {
        PRINT_ERR("ERROR: OS_ERRNO_SWTMR_NOT_STARTED\n");
        return LOS_ERRNO_SWTMR_NOT_STARTED;
    }

}

void linux_module_init(void)
{
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
}

// TODO:
UINT32 do_div_imp(UINT64 *n, UINT32 base)
{
    UINT32 remainder = 0;

    if ((NULL == n) || (0 == base))
    {
        PRINT_ERR("%s invalid input param, n:%p, base %d\n", __FUNCTION__, n, base);
        return 0;
    }

    remainder = *n % base;
    *n = *n / base;
    return remainder;
}

INT32 do_div_s64_imp(INT64 *n, INT32 base)
{
    INT32 remainder = 0;

    if ((NULL == n) || (0 == base))
    {
        PRINT_ERR("%s invalid input param, n:%p, base:%d\n", __FUNCTION__, n, base);
        return 0;
    }

    remainder = *n % base;
    *n = *n / base;
    return remainder;
}

    //mmap and munmap is
void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
    return (void *)offset;
}

int munmap(void *addr, size_t length)
{
    return 0;
}

int waitpid (int __pid, int *__stat_loc, int __options)
{
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
    return 0;
}

void closelog(void)
{
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
}

unsigned int alarm (unsigned int __seconds)
{
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
    return 0;
}

int daemon (int __nochdir, int __noclose)
{
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
    return 0;
}

typedef void __signalfn_t(int);
typedef __signalfn_t *__sighandler_t;
__sighandler_t sigset(int __sig, __sighandler_t __disp)
{
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
    return 0;
}

typedef unsigned long __gid_t;
typedef unsigned long __uid_t;
int setgroups(size_t __n, const __gid_t *__groups)
{
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
    return 0;
}

int initgroups(const char * user, __gid_t group)
{
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
    return 0;
}


int chroot (const char *__path)
{
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
    return 0;
}

void *getpwnam (const char *__name)
{
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
    return 0;
}

int fchown (int __fd, __uid_t __owner, __gid_t __group)
{
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
    return 0;
}
ssize_t readlink (const char * __path, char * __buf, size_t __len)
{
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
    return 0;
}

typedef uint32_t in_addr_t;

int gethostname (char *name, size_t len)
{
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
    return 0;
}

int fork (void)
{
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
    return 0;
}

int nice (int inc)
{
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
    return 0;
}

int getdtablesize (void)
{
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
    return 0;
}

int pipe (int pipedes[2])
{
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
    return 0;
}

int    execve(const char *name, char **argv, char **envv)
{
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
    return 0;
}

int getrlimit (int resource, void *rlimits)
{
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
    return 0;
}

int setrlimit (int resource, void *rlimits)
{
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
    return 0;
}
#ifdef LOSCFG_NET_LWIP_SACK
struct msghdr
  {
    void *msg_name;     /* Address to send to/receive from.  */
    socklen_t msg_namelen;  /* Length of address data.  */

    void *msg_iov;  /* Vector of data to send/receive into.  */
    size_t msg_iovlen;      /* Number of elements in the vector.  */

    void *msg_control;      /* Ancillary data (eg BSD filedesc passing). */
    size_t msg_controllen;  /* Ancillary data buffer length.
                   !! The type should be socklen_t but the
                   definition of the kernel is incompatible
                   with this.  */

    int msg_flags;      /* Flags on received message.  */
  };

ssize_t recvmsg (int fd, struct msghdr *message, int flags)
{
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
    return 0;
}

char *
stpcpy(char * to, const char * from)
{

    for (; (*to = *from); ++from, ++to);
    return(to);
}

/*
 * format an IPv4 address
 *
 */
 const char *inet_ntop4(const unsigned char *src, char *dst, socklen_t size)
{
    int octet;
    int chlen;
    char data[sizeof ("255.255.255.255") + 1] = "\0";
    if ((NULL == src) || (NULL == dst))
    {
        return NULL;
    }
    chlen = 0;
    for (octet = 0; octet <= 3; octet++) {
        data[chlen++] = '0' + src[octet] / 100;

        if (data[chlen - 1] != '0') {
            data[chlen++] = '0' + (src[octet] / 10 % 10);
        } else {
            data[chlen - 1] = '0' + (src[octet] / 10 % 10);

            if (data[chlen - 1] == '0')
                chlen--;
        }
        data[chlen++] = '0' + src[octet] % 10;
        data[chlen++] = '.';
    }
    data[chlen - 1] = '\0';

    if (((socklen_t)strlen(data)+1) <= size) {
        return strncpy(dst, data, strlen(data)+1);
    }
    else {
        return (NULL);
    }
}

/*
 * like inet_aton() but without all the hexadecimal and shorthand.
 */
int inet_pton4(const char *src,unsigned char *dst)
{
    int saw_digit, octets, ch;
    unsigned char tmp[4], *tp;
    if ((NULL == src) || (NULL == dst))
    {
        return 0;
    }
    saw_digit = 0;
    octets = 0;
    *(tp = tmp) = 0;
    while ((ch = *src++) != '\0') {
        if (ch >= '0' && ch <= '9') {
            u_int new_arg = *tp * 10 + (ch - '0');
            if (new_arg>255)
                return (0);
            *tp = new_arg;
            if (! saw_digit) {
                if (++octets>4)
                    return (0);
                saw_digit = 1;
                    }
        } else if (ch == '.' && saw_digit) {
            if (octets == 4)
                return (0);
            *++tp = 0;
            saw_digit = 0;
        } else
            return (0);
    }

    if (octets < 4)
        return (0);
    memcpy(dst, tmp, 4);
        return 1;

}

const char *inet_ntop(int af, const void *src, char *dst,socklen_t size)
{
    switch (af) {
    case AF_INET:
        return inet_ntop4(src, dst, size);
    default:
        /*__set_errno(EAFNOSUPPORT);*/
        return NULL;
    }
    /* NOTREACHED */
}

int inet_pton(int af, const char *src, void *dst)
{
    switch (af) {
    case AF_INET:
        return inet_pton4(src, dst);
    default:
        /*__set_errno(EAFNOSUPPORT);*/
        return -1;
    }
    /* NOTREACHED */
}
#endif
struct stat64 {
#define STAT64_HAS_BROKEN_ST_INO    1
    unsigned long long  st_dev;
    unsigned char   __pad0[4];

    unsigned long   st_uid;
    unsigned long   st_gid;

    unsigned long   st_ctime;
    unsigned long   st_ctime_nsec;

    unsigned long   st_atime;
    unsigned long   st_atime_nsec;

    unsigned long   st_mtime;
    unsigned long   st_mtime_nsec;
    unsigned long   __st_ino;
    unsigned int    st_mode;
    unsigned int    st_nlink;


    unsigned long long  st_rdev;
    unsigned char   __pad3[4];

    long long   st_size;
    unsigned long   st_blksize;
    unsigned long long st_blocks;   /* Number 512-byte blocks allocated. */

    unsigned long long  st_ino;
};
#ifdef LOSCFG_FS_VFS
int fstat64 (int fd, struct stat64 *buf)
{
    /* Fix ME: Need to correct return structure
     */
    return fstat(fd, (struct stat*)buf);
}
#endif
void *__dso_handle = 0;

void __cxa_atexit(void (*func)(void*), void* arg1, void* arg2)
{
    ;
}

__attribute__((weak)) void __aeabi_memclr(void *dest, size_t n)
{
  memset(dest, 0, n);
}

__attribute__((weak)) void __aeabi_memclr4(void *dest, size_t n)
{
  memset(dest, 0, n);
}

__attribute__((weak)) void __aeabi_memclr8(void *dest, size_t n)
{
  memset(dest, 0, n);
}
//TODO remove this
#ifdef gethostbyname
#undef gethostbyname
#endif
__attribute__((weak)) struct hostent*
gethostbyname(const char *name)
{
    return lwip_gethostbyname(name);
}

void asm_test(void)
{
     __asm__ __volatile__("ldr    r0, =0x20080000");
     __asm__ __volatile__("ldr    r1, =0x31");
     __asm__ __volatile__("str    r1, [r0]");
}
//--------------------------------------------------------------------------------

