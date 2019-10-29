#ifndef __HI_SOCKET_H
#define __HI_SOCKET_H

#include <string.h>

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#define AF_INET         2

#define SOCK_STREAM     1
#define SOCK_DGRAM      2

#define SOL_SOCKET      1
#define SO_REUSEADDR    2       /**< �������ñ��ص�ַ�Ͷ˿� */
#define SO_ERROR        4
#define SO_BROADCAST    6       /**< �����͹㲥���� */
#define SO_SNDBUF       7       /**< ���ͻ�������С */
#define SO_RCVBUF       8       /**< ���ջ�������С */
#define SO_KEEPALIVE    9       /**< �������� */
#define SO_NO_CHECK     11      /**< ������У��� */
#define SO_RCVTIMEO     20      /**< ���ͳ�ʱ */
#define SO_SNDTIMEO     21      /**< ���ճ�ʱ */

#define IPPROTO_IP      0
#define IPPROTO_ICMP    1
#define IPPROTO_TCP     6
#define IPPROTO_UDP     17

#define IP_HDRINCL      3       /**< �����ݰ��а���IP�ײ� */
#define IP_TOS          1       /**< �������� */
#define IP_TTL          2       /**< ����ʱ�� */

#define TCP_NODELAY     1       /**< ��ʹ��Nagle�㷨 */
#define TCP_KEEPIDLE    4
#define TCP_KEEPINTVL   5
#define TCP_KEEPCNT     6
#define TCP_QUEUE_SEQ   0x15

#define INADDR_ANY            ((unsigned int)0x00000000UL)
#define INADDR_BROADCAST      ((unsigned int)0xffffffffUL)
#define INADDR_NONE           ((unsigned int)0xffffffffUL)
#define INADDR_LOOPBACK       ((unsigned int)0x7f000001UL)

#define F_GETFL     3
#define F_SETFL     4

#ifndef O_NONBLOCK
#define O_NONBLOCK  04000
#endif

#ifndef O_NDELAY
#define O_NDELAY	O_NONBLOCK
#endif

#define IOCTLPARM_MASK    0x1fff
#define IOCTLPARM_LEN(x)  (((x) >> 16) & IOCTLPARM_MASK)
#define IOCTLBASECMD(x)   ((x) & ~(IOCTLPARM_MASK << 16))
#define IOCTLGROUP(x)     (((x) >> 8) & 0xff)

#define IOCTL_DIRMASK     0xe0000000
#define IOCTL_OUT         0x80000000
#define IOCTL_IN          0x40000000
#define IOCTL_VOID        0x00000000
#define IOCTL_INOUT       (IOCTL_IN|IOCTL_OUT)
#define IOC_OUT           IOCTL_OUT

#define _IOC(inout, group, num, len) \
    ((unsigned long)(inout | ((len & IOCTLPARM_MASK) << 16) | ((group) << 8) | (num)))
#define _IOW(g,n,t)     _IOC(IOCTL_IN,    (g), (n), sizeof(t))
#define _IOR(g,n,t)     _IOC(IOCTL_OUT,   (g), (n), sizeof(t))
#define _IO(g,n)        _IOC(IOCTL_VOID,  (g), (n), 0)
#define _IOWR(g,n,t)    _IOC(IOCTL_INOUT, (g), (n), sizeof(t))

#define SIOCADDRT   _IOW('R', 9, struct rtentry)    /**< ����·�� */
#define SIOCGIFFLAGS  _IOWR('i', 17, struct ifreq)  /**< ��ȡ�ӿڱ�־ */
#define SIOCSIFFLAGS   _IOW('i', 16, struct ifreq)  /**< ���ýӿڱ�־ */
#define SIOCGIFADDR _IOWR('i', 33, struct ifreq)  /**< ��ȡ�ӿڵ�ַ */
#define SIOCSIFADDR  _IOW('i', 12, struct ifreq)  /**< ���ýӿڵ�ַ */
#define SIOCGIFNETMASK  _IOWR('i', 37, struct ifreq)  /**< ��ȡ�������� */
#define SIOCSIFNETMASK   _IOW('i', 22, struct ifreq)  /**< ������������ */
#define SIOCSIFHWADDR  _IOW('i', 140, struct ifreq) /**< ��ȡӲ����ַ */
#define SIOCGIFHWADDR  _IOW('i', 141, struct ifreq) /**< ����Ӳ����ַ */
#define SIOCGIFNAME  _IOW('i', 142, struct ifreq) /**< ��ȡ�ӿ����� */
#define SIOCSIFNAME  _IOW('i', 40, struct ifreq)  /**< ���ýӿ����� */

#ifndef FIONREAD
#define FIONREAD    _IOR('f', 127, unsigned long) /**< ��ȡ���ջ������е��ֽ��� */
#endif
#ifndef FIONBIO
#define FIONBIO     _IOW('f', 126, unsigned long) /**< �������������I/O ��־ */
#endif

struct in_addr
{
    unsigned int s_addr;
};

struct sockaddr_in
{
    uint16_t     sin_family;  /**< ��ַ���� */
    uint16_t        sin_port;    /**< �����ֽ���˿ں� */
    struct in_addr  sin_addr;    /**< ��ַ */
    char            sin_zero[8]; /**< ����λ */
};

/**
* @ingroup �׽��ֽӿ�
* �����׽��ֵ�ַ��Ϣ
*/
struct sockaddr
{
    unsigned short sa_family;  /**< ��ַ���� */
    char sa_data[14];          /**< 14�ֽ�Э���ַ */
};

#define IFNAMSIZ	16
struct ifreq
{
    char  ifr_name[IFNAMSIZ];
    union
    {
        struct  sockaddr ifru_addr;
        struct  sockaddr ifru_dstaddr;
        struct  sockaddr ifru_broadaddr;
        short ifru_flags[2];
        short ifru_index;
        int ifru_jid;
        int ifru_metric;
        int ifru_mtu;
        int ifru_phys;
        int ifru_media;
        int ifru_cap[2];
    } ifr_ifru;
};

#define ifr_addr  ifr_ifru.ifru_addr
#define ifr_hwaddr  ifr_ifru.ifru_addr
#define ifr_netmask  ifr_ifru.ifru_addr
#define ifr_dstaddr ifr_ifru.ifru_dstaddr
#define ifr_broadaddr ifr_ifru.ifru_broadaddr
#define ifr_flags ifr_ifru.ifru_flags[0]
#define ifr_flagshigh ifr_ifru.ifru_flags[1]
#define ifr_jid   ifr_ifru.ifru_jid
#define ifr_metric  ifr_ifru.ifru_metric
#define ifr_mtu   ifr_ifru.ifru_mtu
#define ifr_phys  ifr_ifru.ifru_phys
#define ifr_media ifr_ifru.ifru_media
#define ifr_reqcap  ifr_ifru.ifru_cap[0]
#define ifr_curcap  ifr_ifru.ifru_cap[1]
#define ifr_index ifr_ifru.ifru_index

struct rtentry
{
    struct sockaddr rt_dst;
    struct sockaddr rt_gateway;
    struct sockaddr rt_genmask;
    unsigned int rt_flags;
};

enum
{
    SHUT_RD = 0,
#define SHUT_RD     SHUT_RD
    SHUT_WR,
#define SHUT_WR     SHUT_WR
    SHUT_RDWR
#define SHUT_RDWR   SHUT_RDWR
};

/**
* @ingroup �׽��ֽӿ�
* ����ʱ����Ϣ
*/
struct timeval
{
    long tv_sec;    /**< �� */
    long tv_usec;   /**< ΢�� */
};

struct ip_mreq
{
    struct in_addr imr_multiaddr;
    struct in_addr imr_interface;
};

typedef unsigned int socklen_t;

typedef unsigned int size_t;


static inline unsigned int ntohl (unsigned int x)
{
    unsigned int t;
    t = x ^ ((x << 16) | (x >> 16));
    x = (x << 24) | (x >> 8);
    t &= ~0x00FF0000;
    x ^= (t >> 8);

    return x;
}

static inline unsigned short ntohs (unsigned short x)
{
    return ((x & 0xff) << 8) | ((x & 0xff00) >> 8);
}

static inline unsigned int htonl (unsigned int x)
{
    return ntohl(x);
}

static inline unsigned short htons (unsigned short x)
{
    return ntohs(x);
}

#ifndef FD_SET
#undef  FD_SETSIZE
#define FD_SETSIZE    10 /**< ��С����socket�����趨 */
#define FD_SET(n, p)  ((p)->fd_bits[(n)/8] |=  (1 << ((unsigned int)n & 7)))
#define FD_CLR(n, p)  ((p)->fd_bits[(n)/8] &= ~(1 << ((unsigned int)n & 7)))
#define FD_ISSET(n,p) ((p)->fd_bits[(n)/8] &   (1 << ((unsigned int)n & 7)))

typedef struct fd_set
{
    unsigned char fd_bits [(FD_SETSIZE + 7) / 8];
} fd_set;
#undef  FD_SETSIZE
#endif

/**
* @ingroup �׽��ֽӿ�
*
* @par ����
*  ����һ���׽���.
*
* @attention
* <ul>
* <li>�˽ӿ�δ��ȫ����POSIX.1-2008</li>
* </ul>
*
* @param[in]    domain     �׽���Э����(ֻ֧��AF_INET)
* @param[in]    type	   �׽�������(ֻ֧��SOCK_STREAM��SOCK_DGRAM)
* @param[in]    protocol   �׽���Э��(IPPROTO_IP��IPPROTO_ICMP��IPPROTO_TCP��IPPROTO_UDP)
*
* @retval       #>=0       �Ϸ��׽���������
* @retval       #-1        ����ʧ��
*
* @par ������
*  \li ʧ�ܷ���-1�������뱻��Ϊ���µ�ĳ��ֵ
*     - <b> [EPROTONOSUPPORT]</b> : \n Э�鲻֧��
*     - <b> [ENFILE] </b>: \n �Ѵﵽ���ļ�������ϵͳ����
*     - <b> [EINVAL] </b>: \n ������Ч
*     - <b> [ENOBUFS] </b>: \n ��Դ���㣬�޷������׽���
*
* @par ����
* <ul><li>hi_socket.h: ��ͷ�ļ������ӿ�����</li></ul>
* @see ��
* @since ��
*/

int socket(int domain, int type, int protocol);

/**
* @ingroup �׽��ֽӿ�
*
* @par ����
*  ���׽���.
*
* @attention
* <ul>
* <li>�˽ӿ�δ��ȫ����POSIX.1-2008</li>
* </ul>
*
* @param[in]    sockfd     �׽���������
* @param[in]    addr	   ָ��ȷ���������ӵ�sockaddr�ṹ
* @param[in]    addrlen    sockaddr�ṹ����
*
* @retval       #=0        �ɹ�
* @retval       #-1        ʧ��
*
* @par ������
*  \li ʧ�ܷ���-1�������뱻��Ϊ���µ�ĳ��ֵ
*     - <b> [EBADF] </b>: \n �׽�����������Ч
*     - <b> [EINVAL] </b>: \n ������Ч
*     - <b> [EOPNOTSUPP] </b>: \n ָ����Э��㲻��ʶ��ѡ��
*     - <b> [EADDRINUSE] </b>: \n ��ַ�ѱ�ʹ��
*     - <b> [EADDRNOTAVAIL] </b>: \n ���󲻴��ڵĽӿڻ�����ĵ�ַ���Ǳ��ص�
*
* @par ����
* @see ��
* @since ��
*/

int bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen);

/**
* @ingroup �׽��ֽӿ�
*
* @par ����
*  �׽��ֽ�������.
*
* @attention
* <ul>
* <li>�˽ӿ�δ��ȫ����POSIX.1-2008</li>
* </ul>
*
* @param[in]    sockfd     �׽���������
* @param[in]    addr	   ָ��Զ�����ӵ�sockaddr�ṹ
* @param[in]    addrlen    sockaddr�ṹ����
*
* @retval       #=0        �ɹ�
* @retval       #-1        ʧ��
*
* @par ������
*  \li ʧ�ܷ���-1�������뱻��Ϊ���µ�ĳ��ֵ
*     - <b> [EBADF] </b>: \n �׽�����������Ч��
*     - <b> [EOPNOTSUPP] </b>: \n ָ����Э��㲻��ʶ��ѡ��
*     - <b> [EINVAL] </b>: \n ������Ч
*     - <b> [EINPROGRESS] </b>: \n �׽���Ϊ���������������޷��������
*     - <b> [EISCONN] </b>: \n �����ѽ���
*     - <b> [ENOTCONN] </b>: \n Զ���������ɴ�
*     - <b> [ECONNRESET] </b>: \n Զ������������������
*
* @par ����
* <ul><li>hi_socket.h: ��ͷ�ļ������ӿ�����</li></ul>
* @see ��
* @since ��
*/

int connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen);

/**
* @ingroup �׽��ֽӿ�
*
* @par ����
*  �׽��ֽ��뱻������״̬.
*
* @attention
* <ul>
* <li>�˽ӿ�δ��ȫ����POSIX.1-2008</li>
* </ul>
*
* @param[in]        sockfd     �׽���������
* @param[in]        backlog	   ���������󳤶�
*
* @retval           #0         �ɹ�
* @retval           #-1        ʧ��
*
* @par ������
*  \li ʧ�ܷ���-1�������뱻��Ϊ���µ�ĳ��ֵ
*     - <b> [EBADF] </b>: \n �׽�����������Ч
*     - <b> [EDESTADDRREQ] </b>: \n �׽���δ�󶨱��ص�ַ
*     - <b> [EINVAL] </b>: \n ������Ч
*     - <b> [EOPNOTSUPP] </b>: \n ָ����Э��㲻��ʶ��ѡ��.
*     - <b> [ENOBUFS] </b> : \n ϵͳû�ж�����Ч��Դ��Ӧ����.
*     - <b> [EMEM] </b> : \n �����ڴ�ʧ��.
*
* @par ����
* <ul><li>hi_socket.h: ��ͷ�ļ������ӿ�����</li></ul>
* @see ��
* @since ��
*/
int listen(int sockfd, int backlog);


/**
* @ingroup �׽��ֽӿ�
*
* @par ����
*  ���տͻ�����������.
*
* @attention
* <ul>
* <li>�˽ӿ�δ��ȫ����POSIX.1-2008</li>
* </ul>
*
* @param[in]        sockfd     �׽���������
* @param[in]        addr	   ���������׽���
* @param[in]        addrlen	   ���������׽��ֳ���
*
* @retval           #>=0       �ɹ�,�����µ��׽���������
* @retval           #-1        ʧ��
*
* @par ������
*  \li ʧ�ܷ���-1�������뱻��Ϊ���µ�ĳ��ֵ
*     - <b> [EWOULDBLOCK]</b> : \n �׽���������O_NONBLOCK����ָ���Ĳ���������
*     - <b> [EBADF] </b>: \n �׽�����������Ч.
*     - <b> [ECONNABORTED] </b>: \n ������ֹ.
*     - <b> [EINVAL] </b>: \n ������Ч
*     - <b> [ENFILE] </b>: \n �����׽�������.
*     - <b> [ENOBUFS] </b>: \n û�ж�����ڴ�ռ�.
*     - <b> [ENOMEM] </b>: \n ϵͳ�ڴ治��.
*     - <b> [EOPNOTSUPP] </b>: \n ָ����Э��㲻��ʶ��ѡ��
*     - <b> [ETIMEDOUT] </b>: \n �ȴ��µ����ӳ�ʱ.
*     - <b> [EIO] </b>: \n �ڲ�����
*     - <b> [ECONNECTED] </b>: \n ��Դ�����������ѹر�,
*
* @par ����
* <ul><li>hi_socket.h: ��ͷ�ļ������ӿ�����</li></ul>
* @see ��
* @since ��
*/
int accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen);

/**
* @ingroup �׽��ֽӿ�
*
* @par ����
*  ��������.
*
* @attention
* <ul>
* <li>�˽ӿ�δ��ȫ����POSIX.1-2008</li>
* </ul>
*
* @param[in]    sockfd     �׽���������
* @param[in]    buf	       ָ����Ҫ���͵����ݻ�����
* @param[in]    len        ��Ҫ���͵����ݻ���������
* @param[in]    flags      ��Ϣ�������ͣ���֧��MSG_MORE��MSG_DONTWAIT
*
* @retval       #>=0             ���͵������ֽ���
* @retval       #-1              ����ʧ��
*
* @par ������
*  \li ʧ�ܷ���-1�������뱻��Ϊ���µ�ĳ��ֵ
*     - <b> [EWOULDBLOCK]</b> : \n �׽���������O_NONBLOCK����ָ���Ĳ���������
*     - <b> [EBADF] </b>: \n �׽�����������Ч��
*     - <b> [EINVAL] </b>: \n ������Ч
*     - <b> [ECONNRESET] </b>: \n ���ӱ��Զ˹ر�
*     - <b> [EDESTADDRREQ] </b>: \n �׽���Ϊ������ģʽ���Զ˵�ַδ����
*     - <b> [EMSGSIZE] </b>: \n ���ݳ��ȳ��ޣ��޷�һ�η������
*     - <b> [ENOTCONN] </b>: \n �׽���û������
*     - <b> [EOPNOTSUPP] </b>: \n ָ����Э��㲻��ʶ��ѡ��
*     - <b> [EHOSTUNREACH] </b>: \n ����Ŀ��ip�����粻ͨ
*     - <b> [ENETDOWN] </b>: \n ����Ŀ��ip������δ����
*     - <b> [ENOBUFS] </b>: \n ϵͳû���㹻�ڴ���Դ
*     - <b> [EINPROGRESS] </b>: \n ����δ���
*
* @par ����
* <ul><li>hi_socket.h: ��ͷ�ļ������ӿ�����</li></ul>
* @see ��
* @since ��
*/

int send(int sockfd, const void* buf, size_t len, int flags);

/**
* @ingroup �׽��ֽӿ�
*
* @par ����
*  ��ָ��sockaddr��������.
*
* @attention
* <ul>
* <li>�˽ӿ�δ��ȫ����POSIX.1-2008</li>
* </ul>
*
* @param[in]    s          �׽���������
* @param[in]    data	   ָ����Ҫ���͵����ݻ�����
* @param[in]    size       ��Ҫ���͵����ݻ���������
* @param[in]    flags      ��Ϣ�������ͣ���֧��MSG_MORE��MSG_DONTWAIT
* @param[in]    to         ָ�룬ָ��sockaddr�ṹ�壬�ýṹ�����Ŀ��ip
* @param[in]    tolen      sockaddr�ṹ��Ĵ�С
*
* @retval       #>=0       ���͵������ֽ���
* @retval       #-1        ����ʧ��
*
* @par ������
*  \li ʧ�ܷ���-1�������뱻��Ϊ���µ�ĳ��ֵ
*     - <b> [EWOULDBLOCK]</b> : \n �׽���������O_NONBLOCK����ָ���Ĳ���������
*     - <b> [EBADF] </b>: \n �׽�����������Ч��
*     - <b> [EINVAL] </b>: \n ������Ч
*     - <b> [ECONNRESET] </b>: \n ���ӱ��Զ˹ر�
*     - <b> [EDESTADDRREQ] </b>: \n �׽���Ϊ������ģʽ���Զ˵�ַδ����
*     - <b> [EMSGSIZE] </b>: \n ���ݳ��ȳ��ޣ��޷�һ�η������
*     - <b> [ENOTCONN] </b>: \n �׽���û������
*     - <b> [EOPNOTSUPP] </b>: \n ָ����Э��㲻��ʶ��ѡ��
*     - <b> [EHOSTUNREACH] </b>: \n ����Ŀ��ip�����粻ͨ
*     - <b> [ENETDOWN] </b>: \n ����Ŀ��ip������δ����
*     - <b> [ENOBUFS] </b>: \n ϵͳû���㹻�ڴ���Դ
*     - <b> [EINPROGRESS] </b>: \n ����δ���
*
* @par ����
* <ul><li>hi_socket.h: ��ͷ�ļ������ӿ�����</li></ul>
* @see ��
* @since ��
*/

int sendto(int sockfd, const void* buf, size_t len, int flags,
           const struct sockaddr* to, socklen_t tolen);

/**
* @ingroup �׽��ֽӿ�
*
* @par ����
*  ��������.
*
* @attention
* <ul>
* <li>�˽ӿ�δ��ȫ����POSIX.1-2008</li>
* </ul>
*
* @param[in]    sockfd     �׽���������
* @param[in]    buf	       ָ��������ݵĴ洢��ָ��
* @param[in]    len        �洢���Ĵ�С
* @param[in]    flags      ��Ϣ�������ͣ�Ŀǰ֧��MSG_PEEK��MSG_DONTWAIT
*
* @retval       #>=0       ���յ������ֽ���
* @retval       #-1        ����ʧ��
*
* @par ������
*  \li ʧ�ܷ���-1�������뱻��Ϊ���µ�ĳ��ֵ
*     - <b> [EWOULDBLOCK]</b> : \n �׽���������O_NONBLOCK����ָ���Ĳ���������
*     - <b> [EBADF] </b>: \n �׽�����������Ч��
*     - <b> [ECONNRESET] </b>: \n ���ӱ��Զ˹ر�
*     - <b> [ENOTCONN] </b>: \n TCP�׽���δ����
*     - <b> [EOPNOTSUPP] </b>: \n ָ����Э��㲻��ʶ��ѡ��
*     - <b> [ETIMEDOUT] </b>: \n ���ճ�ʱ
*
* @par ����
* <ul><li>hi_socket.h: ��ͷ�ļ������ӿ�����</li></ul>
* @see ��
* @since ��
*/

int recv(int sockfd, void* buf, size_t len, int flags);

/**
* @ingroup �׽��ֽӿ�
*
* @par ����
*  ��ָ��sockaddr��������.
*
* @attention
* <ul>
* <li>�˽ӿ�δ��ȫ����POSIX.1-2008</li>
* </ul>
*
* @param[in]    sockfd     �׽���������
* @param[in]    buf	       ָ��������ݵĴ洢��ָ��
* @param[in]    len        �洢���Ĵ�С
* @param[in]    flags      ��Ϣ�������ͣ�Ŀǰ֧��MSG_PEEK��MSG_DONTWAIT
* @param[out]   from       ��ָ�����ָ��洢�Զ�sockaddr�ṹ�Ĵ洢��
* @param[out]   fromlen    ��ָ���ָ��socklen_t�������洢sockaddr�ṹ�Ĵ�С
*
* @retval       #>=0       ���յ������ֽ���
* @retval       #-1        ����ʧ��
*
* @par ������
*  \li ʧ�ܷ���-1�������뱻��Ϊ���µ�ĳ��ֵ
*     - <b> [EWOULDBLOCK]</b> : \n �׽���������O_NONBLOCK����ָ���Ĳ���������
*     - <b> [EBADF] </b>: \n �׽�����������Ч��
*     - <b> [ECONNRESET] </b>: \n ���ӱ��Զ˹ر�
*     - <b> [ENOTCONN] </b>: \n TCP�׽���δ����
*     - <b> [EOPNOTSUPP] </b>: \n ָ����Э��㲻��ʶ��ѡ��
*     - <b> [ETIMEDOUT] </b>: \n ���ճ�ʱ
*
* @par ����
* <ul><li>hi_socket.h: ��ͷ�ļ������ӿ�����</li></ul>
* @see ��
* @since ��
*/

int recvfrom(int sockfd, void* buf, size_t len, int flags,
             struct sockaddr* from, socklen_t* fromlen);

/**
* @ingroup �׽��ֽӿ�
*
* @par ����
*  �ر��׽��ַ��ͻ��߽��ղ���
*
* @attention
* <ul>
* <li>�˽ӿ�δ��ȫ����POSIX.1-2008</li>
* </ul>
*
* @param[in]    sockfd     �׽���������
* @param[in]    how	       ָ���رյĲ���
*
*
* @retval       #0         �ɹ�
* @retval       #-1        ʧ��
*
* @par ������
*  \li ʧ�ܷ���-1�������뱻��Ϊ���µ�ĳ��ֵ
*     - <b> [EBADF] </b>: \n �׽�����������Ч
*     - <b> [ENOTCONN] </b>: \n �׽�������δ����
*     - <b> [EOPNOTSUPP] </b>: \n ָ����Э��㲻��ʶ��ѡ��
*     - <b> [EINVAL] </b>: \n ������Ч
*
* @par ����
* <ul><li>hi_socket.h: ��ͷ�ļ������ӿ�����</li></ul>
* @see ��
* @since ��
*/

int shutdown(int sockfd, int how);

/**
* @ingroup �׽��ֽӿ�
*
* @par ����
*  �ر��׽���
*
* @attention
* <ul>
* <li>�˽ӿ�δ��ȫ����POSIX.1-2008</li>
* </ul>
*
* @param[in]    sockfd     �׽���������
*
* @retval       #0         �ɹ�
* @retval       #-1        ʧ��
*
* @par ������
*  \li ʧ�ܷ���-1�������뱻��Ϊ���µ�ĳ��ֵ
*     - <b> [EBADF] </b>: \n �׽�����������Ч
*
* @par ����
* <ul><li>hi_socket.h: ��ͷ�ļ������ӿ�����</li></ul>
* @see ��
* @since ��
*/

int close(int sockfd);

/**
* @ingroup �׽��ֽӿ�
*
* @par ����
*  �����׽��ֵ�ѡ��
*
* @attention
* <ul>
* <li>�˽ӿ�δ��ȫ����POSIX.1-2008</li>
* </ul>
*
* @param[in]    sockfd        �׽���������
* @param[in]    level         ָ��ѡ�����ڵ�Э�鼶��
* @param[in]    option        ָ����Ҫ���õ�ѡ��
* @param[out]   value         ָ��ָ��洢��
* @param[in]    value_len     value�ĳ���
*
* @retval       #0            �ɹ�
* @retval       #-1           ʧ��
*
* @par ������
*  \li ʧ�ܷ���-1�������뱻��Ϊ���µ�ĳ��ֵ:
*     - <b> [EBADF] </b>: \n �׽�����������Ч
*     - <b> [EINVAL] </b>: \n ������Ч
*     - <b> [ENOPROTOOPT] </b>: \n Э�鲻֧�ָò���
*     - <b> [EDOM] </b>: \n �����timevalֵ��Ч
*
* @par ����
* <ul><li>hi_socket.h: ��ͷ�ļ������ӿ�����</li></ul>
* @see ��
* @since ��
*/

int setsockopt(int sockfd, int level, int option,
               const void* value, socklen_t value_len);

/**
* @ingroup �׽��ֽӿ�
*
* @par ����
*  ��ȡ�׽��ֵ�ѡ��
*
* @attention
* <ul>
* <li>�˽ӿ�δ��ȫ����POSIX.1-2008</li>
* </ul>
*
* @param[in]        sockfd      �׽���������
* @param[in]        level       ָ��ѡ�����ڵ�Э�鼶��
* @param[in]        option      ָ����Ҫ��ȡ��ѡ��
* @param[out]       value       ָ��ָ��洢��
* @param[in,out]    value_len   value�ĳ���
*
* @retval           #0          �ɹ�
* @retval           #-1         ʧ��
*
* @par ������
*  \li ʧ�ܷ���-1�������뱻��Ϊ���µ�ĳ��ֵ:
*     - <b> [EBADF] </b>: \n �׽�����������Ч
*     - <b> [EINVAL] </b>: \n ������Ч
*     - <b> [ENOPROTOOPT] </b>: \n Э�鲻֧�ָò���
*
* @par ����
* <ul><li>hi_socket.h: ��ͷ�ļ������ӿ�����</li></ul>
* @see ��
* @since ��
*/

int getsockopt(int sockfd, int level, int option,
               void* value, socklen_t* value_len);

/**
* @ingroup �׽��ֽӿ�
*
* @par ����
*  ��ȡ���׽��ֹ����ı���Э���ַ.
*
* @attention
* <ul>
* <li>�˽ӿ�δ��ȫ����POSIX.1-2008</li>
* </ul>
*
* @param[in]        sockfd     �׽���������
* @param[out]       addr	   �׽��ֵ�ַ
* @param[in,out]    addrlen    �׽��ֵ�ַ����
*
* @retval           #0         ��ȡ���׽��ֹ����ı���Э���ַ�ɹ�
* @retval           #-1        ��ȡ���׽��ֹ����ı���Э���ַʧ��
*
* @par ������
*  \li ʧ�ܷ���-1�������뱻��Ϊ���µ�ĳ��ֵ:
*     - <b> [EBADF] </b>: \n �׽�����������Ч
*     - <b> [EOPNOTSUPP] </b>: \n ָ����Э��㲻��ʶ��ѡ��
*
* @par ����
* <ul><li>hi_socket.h: ��ͷ�ļ������ӿ�����</li></ul>
* @see ��
* @since ��
*/
int getsockname(int sockfd, struct sockaddr* addr,
                socklen_t* addrlen);

/**
* @ingroup �׽��ֽӿ�
*
* @par ����
*  ��ȡ���׽��ֹ�����Զ��Э���ַ.
*
* @attention
* <ul>
* <li>�˽ӿ�δ��ȫ����POSIX.1-2008</li>
* </ul>
*
* @param[in]        sockfd     �׽���������
* @param[out]       addr	   �׽��ֵ�ַ
* @param[in,out]    addrlen    �׽��ֵ�ַ����
*
* @retval           #0         ��ȡ���׽��ֹ�����Զ��Э���ַ�ɹ�
* @retval           #-1        ��ȡ���׽��ֹ�����Զ��Э���ַʧ��
*
* @par ������
*  \li ʧ�ܷ���-1�������뱻��Ϊ���µ�ĳ��ֵ
*     - <b> [EBADF] </b>: \n �׽�����������Ч
*     - <b> [ENOTCONN] </b>: \n ���ػ�Զ���׽���δ����
*     - <b> [EOPNOTSUPP] </b>: \n ָ����Э��㲻��ʶ��ѡ��
*     - <b> [EINVAL] </b>: \n ������Ч
*
* @par ����
* <ul><li>hi_socket.h: ��ͷ�ļ������ӿ�����</li></ul>
* @see ��
* @since ��
*/
int getpeername(int sockfd, struct sockaddr* addr,
                socklen_t* addrlen);


/**
* @ingroup �׽��ֽӿ�
*
* @par ����
*  ����һ�������׽ӿڵ�״̬�仯
*
* @attention
* <ul>
* <li>�˽ӿ�δ��ȫ����POSIX.1-2008</li>
* </ul>
*
* @param[in]        nfds       ���ӵ��ļ������
* @param[in,out]    readfds	   ���ӵĿɶ��ļ��������
* @param[in,out]    writefds   ���ӵĿ�д�ļ��������
* @param[in,out]    exceptfds  ���ӵ��쳣�ļ��������
* @param[in]        timeout    ��ʱ����ʱ��:��ΪNULL,��Ϊ����״̬;��Ϊ0��0���룬�ͱ��һ������ķ���������
							   �����ļ��������Ƿ��б仯�������̷��ؼ���ִ��;timeout��ֵ����0���ȴ��ĳ�ʱʱ��

* @retval           #>=0       �ɹ�
* @retval           #-1        ʧ��
*
* @par ������
*  \li ʧ�ܷ���-1�������뱻��Ϊ���µ�ĳ��ֵ
*     - <b> [EBADF] </b>: \n �׽�����������Ч
*     - <b> [EINVAL] </b>: \n ������Ч
*     - <b> [ENOMEM] </b>: \n ϵͳ�ڴ治��
*
* @par ����
* <ul><li>hi_socket.h: ��ͷ�ļ������ӿ�����</li></ul>
* @see ��
* @since ��
*/
int select(int nfds, fd_set* readfds, fd_set* writefds,
           fd_set* exceptfds, struct timeval* timeout);


/**
* @ingroup �׽��ֽӿ�
*
* @par ����
*  ���豸��I/Oͨ�����й���
*
* @attention
* <ul>
* <li>�˽ӿ�δ��ȫ����POSIX.1-2008</li>
* </ul>
*
* @param[in]        s      �׽���������
* @param[in]        cmd	   ��������
* @param[in]        argp   �������
*
* @retval           #>=0   �ɹ�
* @retval           #-1    ʧ��
*
* @par ������
*  \li ʧ�ܷ���-1�������뱻��Ϊ���µ�ĳ��ֵ
*     - <b> [EBADF] </b>: \n �׽�����������Ч
*     - <b> [EINVAL] </b>: \n ������Ч
*     - <b> [ENOMEM] </b>: \n ϵͳ�ڴ治��
*     - <b> [EAFNOSUPPORT] </b>: \n Э�鲻֧�ָò���
*
* @par ����
* <ul><li>hi_socket.h: ��ͷ�ļ������ӿ�����</li></ul>
* @see ��
* @since ��
*/
int ioctl(int s, long cmd, void* argp);

/**
* @ingroup �׽��ֽӿ�
*
* @par ����
*  �����ļ��������������ļ�������.
*
* @attention
* <ul>
* <li>�˽ӿ�δ��ȫ����POSIX.1-2008</li>
* </ul>
*
* @param[in]        s      �׽���������
* @param[in]        cmd	   ѡ����������(F_GETFL|F_SETFL)
* @param[in]        val    �������
*
* @retval           #>=0   �ɹ�
* @retval           #-1    ʧ��
*
* @par ������
*  \li ʧ�ܷ���-1�������뱻��Ϊ���µ�ĳ��ֵ
*     - <b> [EBADF] </b>: \n �׽�����������Ч
*     - <b> [EINVAL] </b>: \n ������Ч
*
* @par ����
* <ul><li>hi_socket.h: ��ͷ�ļ������ӿ�����</li></ul>
* @see ��
* @since ��
*/
int fcntl(int s, int cmd, int val);

/**
* @ingroup �׽��ֽӿ�
*
* @par ����
*  ���ʮ����IPת���ɳ�������.
*
* @attention
* <ul>
* <li>�˽ӿڴ��ڹ������⣬����ֵ-1����Ч�ĵ�ַ��255.255.255.255��</li>
* </ul>
*
* @param[in]    cp     ָ����ʮ����IPv4��ַ�ַ���ָ��
*
* @retval       #>=0   �ɹ�,����ֵΪunsigned int����
* @retval       #-1    ʧ��
*
* @par ����
* <ul><li>hi_socket.h: ��ͷ�ļ������ӿ�����</li></ul>
* @see ��
* @since ��
*/
unsigned int inet_addr (const char* cp);

/**
* @ingroup �׽��ֽӿ�
*
* @par ����
*  �����ַת���ɵ��ʮ����IP��ַ�ַ�����ʽ.
*
* @param[in]        in     ��������IP��ַ
*
* @retval #Valid pointer   �ɹ�,����ֵΪ��ʮ����IP��ַ�ַ�������
* @retval #NULL            ʧ��
*
* @par ����
* <ul><li>hi_socket.h: ��ͷ�ļ������ӿ�����</li></ul>
* @see ��
* @since ��
*/
char* inet_ntoa (struct in_addr in);

/**
* @ingroup �׽��ֽӿ�
*
* @par ����
*  ���ʮ����IP��ַת��������IP��ַ.
*
* @param[in]       cp     ָ����ʮ����IPv4��ַ�ַ���ָ��
* @param[out]      addr   ��������IP��ַ
*
* @retval          #0     �ɹ�
* @retval          #1     ʧ��
*
* @par ����
* <ul><li>hi_socket.h: ��ͷ�ļ������ӿ�����</li></ul>
* @see ��
* @since ��
*/
int inet_aton(const char* cp, struct in_addr* addr);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif /* __INCLUDE_SYS_SOCKET_H */
