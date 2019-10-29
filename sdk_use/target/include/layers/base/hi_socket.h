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
#define SO_REUSEADDR    2       /**< 允许重用本地地址和端口 */
#define SO_ERROR        4
#define SO_BROADCAST    6       /**< 允许发送广播数据 */
#define SO_SNDBUF       7       /**< 发送缓冲区大小 */
#define SO_RCVBUF       8       /**< 接收缓冲区大小 */
#define SO_KEEPALIVE    9       /**< 保持连接 */
#define SO_NO_CHECK     11      /**< 不创建校验和 */
#define SO_RCVTIMEO     20      /**< 发送超时 */
#define SO_SNDTIMEO     21      /**< 接收超时 */

#define IPPROTO_IP      0
#define IPPROTO_ICMP    1
#define IPPROTO_TCP     6
#define IPPROTO_UDP     17

#define IP_HDRINCL      3       /**< 在数据包中包含IP首部 */
#define IP_TOS          1       /**< 服务类型 */
#define IP_TTL          2       /**< 生存时间 */

#define TCP_NODELAY     1       /**< 不使用Nagle算法 */
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

#define SIOCADDRT   _IOW('R', 9, struct rtentry)    /**< 增加路径 */
#define SIOCGIFFLAGS  _IOWR('i', 17, struct ifreq)  /**< 获取接口标志 */
#define SIOCSIFFLAGS   _IOW('i', 16, struct ifreq)  /**< 设置接口标志 */
#define SIOCGIFADDR _IOWR('i', 33, struct ifreq)  /**< 获取接口地址 */
#define SIOCSIFADDR  _IOW('i', 12, struct ifreq)  /**< 设置接口地址 */
#define SIOCGIFNETMASK  _IOWR('i', 37, struct ifreq)  /**< 获取子网掩码 */
#define SIOCSIFNETMASK   _IOW('i', 22, struct ifreq)  /**< 设置子网掩码 */
#define SIOCSIFHWADDR  _IOW('i', 140, struct ifreq) /**< 获取硬件地址 */
#define SIOCGIFHWADDR  _IOW('i', 141, struct ifreq) /**< 设置硬件地址 */
#define SIOCGIFNAME  _IOW('i', 142, struct ifreq) /**< 获取接口名称 */
#define SIOCSIFNAME  _IOW('i', 40, struct ifreq)  /**< 设置接口名称 */

#ifndef FIONREAD
#define FIONREAD    _IOR('f', 127, unsigned long) /**< 获取接收缓存区中的字节数 */
#endif
#ifndef FIONBIO
#define FIONBIO     _IOW('f', 126, unsigned long) /**< 设置清除非阻塞I/O 标志 */
#endif

struct in_addr
{
    unsigned int s_addr;
};

struct sockaddr_in
{
    uint16_t     sin_family;  /**< 地址家族 */
    uint16_t        sin_port;    /**< 网络字节序端口号 */
    struct in_addr  sin_addr;    /**< 地址 */
    char            sin_zero[8]; /**< 保留位 */
};

/**
* @ingroup 套接字接口
* 定义套接字地址信息
*/
struct sockaddr
{
    unsigned short sa_family;  /**< 地址家族 */
    char sa_data[14];          /**< 14字节协议地址 */
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
* @ingroup 套接字接口
* 定义时间信息
*/
struct timeval
{
    long tv_sec;    /**< 秒 */
    long tv_usec;   /**< 微秒 */
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
#define FD_SETSIZE    10 /**< 大小根据socket数量设定 */
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
* @ingroup 套接字接口
*
* @par 描述
*  创建一个套接字.
*
* @attention
* <ul>
* <li>此接口未完全兼容POSIX.1-2008</li>
* </ul>
*
* @param[in]    domain     套接字协议族(只支持AF_INET)
* @param[in]    type	   套接字类型(只支持SOCK_STREAM、SOCK_DGRAM)
* @param[in]    protocol   套接字协议(IPPROTO_IP、IPPROTO_ICMP、IPPROTO_TCP和IPPROTO_UDP)
*
* @retval       #>=0       合法套接字描述符
* @retval       #-1        创建失败
*
* @par 错误码
*  \li 失败返回-1，错误码被设为以下的某个值
*     - <b> [EPROTONOSUPPORT]</b> : \n 协议不支持
*     - <b> [ENFILE] </b>: \n 已达到打开文件总数的系统限制
*     - <b> [EINVAL] </b>: \n 参数无效
*     - <b> [ENOBUFS] </b>: \n 资源不足，无法创建套接字
*
* @par 依赖
* <ul><li>hi_socket.h: 此头文件包含接口声明</li></ul>
* @see 无
* @since 无
*/

int socket(int domain, int type, int protocol);

/**
* @ingroup 套接字接口
*
* @par 描述
*  绑定套接字.
*
* @attention
* <ul>
* <li>此接口未完全兼容POSIX.1-2008</li>
* </ul>
*
* @param[in]    sockfd     套接字描述符
* @param[in]    addr	   指向确定本机连接的sockaddr结构
* @param[in]    addrlen    sockaddr结构长度
*
* @retval       #=0        成功
* @retval       #-1        失败
*
* @par 错误码
*  \li 失败返回-1，错误码被设为以下的某个值
*     - <b> [EBADF] </b>: \n 套接字描述符无效
*     - <b> [EINVAL] </b>: \n 参数无效
*     - <b> [EOPNOTSUPP] </b>: \n 指定的协议层不能识别选项
*     - <b> [EADDRINUSE] </b>: \n 地址已被使用
*     - <b> [EADDRNOTAVAIL] </b>: \n 请求不存在的接口或请求的地址不是本地的
*
* @par 依赖
* @see 无
* @since 无
*/

int bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen);

/**
* @ingroup 套接字接口
*
* @par 描述
*  套接字建立连接.
*
* @attention
* <ul>
* <li>此接口未完全兼容POSIX.1-2008</li>
* </ul>
*
* @param[in]    sockfd     套接字描述符
* @param[in]    addr	   指向远端连接的sockaddr结构
* @param[in]    addrlen    sockaddr结构长度
*
* @retval       #=0        成功
* @retval       #-1        失败
*
* @par 错误码
*  \li 失败返回-1，错误码被设为以下的某个值
*     - <b> [EBADF] </b>: \n 套接字描述符无效。
*     - <b> [EOPNOTSUPP] </b>: \n 指定的协议层不能识别选项
*     - <b> [EINVAL] </b>: \n 参数无效
*     - <b> [EINPROGRESS] </b>: \n 套接字为非阻塞，且连接无法立即完成
*     - <b> [EISCONN] </b>: \n 连接已建立
*     - <b> [ENOTCONN] </b>: \n 远端主机不可达
*     - <b> [ECONNRESET] </b>: \n 远端主机重置连接请求
*
* @par 依赖
* <ul><li>hi_socket.h: 此头文件包含接口声明</li></ul>
* @see 无
* @since 无
*/

int connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen);

/**
* @ingroup 套接字接口
*
* @par 描述
*  套接字进入被动监听状态.
*
* @attention
* <ul>
* <li>此接口未完全兼容POSIX.1-2008</li>
* </ul>
*
* @param[in]        sockfd     套接字描述符
* @param[in]        backlog	   请求队列最大长度
*
* @retval           #0         成功
* @retval           #-1        失败
*
* @par 错误码
*  \li 失败返回-1，错误码被设为以下的某个值
*     - <b> [EBADF] </b>: \n 套接字描述符无效
*     - <b> [EDESTADDRREQ] </b>: \n 套接字未绑定本地地址
*     - <b> [EINVAL] </b>: \n 参数无效
*     - <b> [EOPNOTSUPP] </b>: \n 指定的协议层不能识别选项.
*     - <b> [ENOBUFS] </b> : \n 系统没有多余有效资源响应请求.
*     - <b> [EMEM] </b> : \n 申请内存失败.
*
* @par 依赖
* <ul><li>hi_socket.h: 此头文件包含接口声明</li></ul>
* @see 无
* @since 无
*/
int listen(int sockfd, int backlog);


/**
* @ingroup 套接字接口
*
* @par 描述
*  接收客户端连接请求.
*
* @attention
* <ul>
* <li>此接口未完全兼容POSIX.1-2008</li>
* </ul>
*
* @param[in]        sockfd     套接字描述符
* @param[in]        addr	   服务器端套接字
* @param[in]        addrlen	   服务器端套接字长度
*
* @retval           #>=0       成功,返回新的套接字描述符
* @retval           #-1        失败
*
* @par 错误码
*  \li 失败返回-1，错误码被设为以下的某个值
*     - <b> [EWOULDBLOCK]</b> : \n 套接字设置了O_NONBLOCK，而指定的操作被阻塞
*     - <b> [EBADF] </b>: \n 套接字描述符无效.
*     - <b> [ECONNABORTED] </b>: \n 连接中止.
*     - <b> [EINVAL] </b>: \n 参数无效
*     - <b> [ENFILE] </b>: \n 申请套接字已满.
*     - <b> [ENOBUFS] </b>: \n 没有多余的内存空间.
*     - <b> [ENOMEM] </b>: \n 系统内存不足.
*     - <b> [EOPNOTSUPP] </b>: \n 指定的协议层不能识别选项
*     - <b> [ETIMEDOUT] </b>: \n 等待新的连接超时.
*     - <b> [EIO] </b>: \n 内部错误
*     - <b> [ECONNECTED] </b>: \n 资源分配是连接已关闭,
*
* @par 依赖
* <ul><li>hi_socket.h: 此头文件包含接口声明</li></ul>
* @see 无
* @since 无
*/
int accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen);

/**
* @ingroup 套接字接口
*
* @par 描述
*  发送数据.
*
* @attention
* <ul>
* <li>此接口未完全兼容POSIX.1-2008</li>
* </ul>
*
* @param[in]    sockfd     套接字描述符
* @param[in]    buf	       指向需要发送的数据缓冲区
* @param[in]    len        需要发送的数据缓冲区长度
* @param[in]    flags      消息传输类型，仅支持MSG_MORE和MSG_DONTWAIT
*
* @retval       #>=0             发送的数据字节数
* @retval       #-1              发送失败
*
* @par 错误码
*  \li 失败返回-1，错误码被设为以下的某个值
*     - <b> [EWOULDBLOCK]</b> : \n 套接字设置了O_NONBLOCK，而指定的操作被阻塞
*     - <b> [EBADF] </b>: \n 套接字描述符无效。
*     - <b> [EINVAL] </b>: \n 参数无效
*     - <b> [ECONNRESET] </b>: \n 连接被对端关闭
*     - <b> [EDESTADDRREQ] </b>: \n 套接字为非连接模式，对端地址未设置
*     - <b> [EMSGSIZE] </b>: \n 数据长度超限，无法一次发送完毕
*     - <b> [ENOTCONN] </b>: \n 套接字没有连接
*     - <b> [EOPNOTSUPP] </b>: \n 指定的协议层不能识别选项
*     - <b> [EHOSTUNREACH] </b>: \n 到达目的ip的网络不通
*     - <b> [ENETDOWN] </b>: \n 到达目的ip的网卡未开启
*     - <b> [ENOBUFS] </b>: \n 系统没有足够内存资源
*     - <b> [EINPROGRESS] </b>: \n 发送未完成
*
* @par 依赖
* <ul><li>hi_socket.h: 此头文件包含接口声明</li></ul>
* @see 无
* @since 无
*/

int send(int sockfd, const void* buf, size_t len, int flags);

/**
* @ingroup 套接字接口
*
* @par 描述
*  从指定sockaddr发送数据.
*
* @attention
* <ul>
* <li>此接口未完全兼容POSIX.1-2008</li>
* </ul>
*
* @param[in]    s          套接字描述符
* @param[in]    data	   指向需要发送的数据缓冲区
* @param[in]    size       需要发送的数据缓冲区长度
* @param[in]    flags      消息传输类型，仅支持MSG_MORE和MSG_DONTWAIT
* @param[in]    to         指针，指向sockaddr结构体，该结构体包含目的ip
* @param[in]    tolen      sockaddr结构体的大小
*
* @retval       #>=0       发送的数据字节数
* @retval       #-1        发送失败
*
* @par 错误码
*  \li 失败返回-1，错误码被设为以下的某个值
*     - <b> [EWOULDBLOCK]</b> : \n 套接字设置了O_NONBLOCK，而指定的操作被阻塞
*     - <b> [EBADF] </b>: \n 套接字描述符无效。
*     - <b> [EINVAL] </b>: \n 参数无效
*     - <b> [ECONNRESET] </b>: \n 连接被对端关闭
*     - <b> [EDESTADDRREQ] </b>: \n 套接字为非连接模式，对端地址未设置
*     - <b> [EMSGSIZE] </b>: \n 数据长度超限，无法一次发送完毕
*     - <b> [ENOTCONN] </b>: \n 套接字没有连接
*     - <b> [EOPNOTSUPP] </b>: \n 指定的协议层不能识别选项
*     - <b> [EHOSTUNREACH] </b>: \n 到达目的ip的网络不通
*     - <b> [ENETDOWN] </b>: \n 到达目的ip的网卡未开启
*     - <b> [ENOBUFS] </b>: \n 系统没有足够内存资源
*     - <b> [EINPROGRESS] </b>: \n 发送未完成
*
* @par 依赖
* <ul><li>hi_socket.h: 此头文件包含接口声明</li></ul>
* @see 无
* @since 无
*/

int sendto(int sockfd, const void* buf, size_t len, int flags,
           const struct sockaddr* to, socklen_t tolen);

/**
* @ingroup 套接字接口
*
* @par 描述
*  接收数据.
*
* @attention
* <ul>
* <li>此接口未完全兼容POSIX.1-2008</li>
* </ul>
*
* @param[in]    sockfd     套接字描述符
* @param[in]    buf	       指向接收数据的存储区指针
* @param[in]    len        存储区的大小
* @param[in]    flags      消息接收类型，目前支持MSG_PEEK和MSG_DONTWAIT
*
* @retval       #>=0       接收的数据字节数
* @retval       #-1        接收失败
*
* @par 错误码
*  \li 失败返回-1，错误码被设为以下的某个值
*     - <b> [EWOULDBLOCK]</b> : \n 套接字设置了O_NONBLOCK，而指定的操作被阻塞
*     - <b> [EBADF] </b>: \n 套接字描述符无效。
*     - <b> [ECONNRESET] </b>: \n 连接被对端关闭
*     - <b> [ENOTCONN] </b>: \n TCP套接字未连接
*     - <b> [EOPNOTSUPP] </b>: \n 指定的协议层不能识别选项
*     - <b> [ETIMEDOUT] </b>: \n 接收超时
*
* @par 依赖
* <ul><li>hi_socket.h: 此头文件包含接口声明</li></ul>
* @see 无
* @since 无
*/

int recv(int sockfd, void* buf, size_t len, int flags);

/**
* @ingroup 套接字接口
*
* @par 描述
*  从指定sockaddr接收数据.
*
* @attention
* <ul>
* <li>此接口未完全兼容POSIX.1-2008</li>
* </ul>
*
* @param[in]    sockfd     套接字描述符
* @param[in]    buf	       指向接收数据的存储区指针
* @param[in]    len        存储区的大小
* @param[in]    flags      消息接收类型，目前支持MSG_PEEK和MSG_DONTWAIT
* @param[out]   from       空指针或者指向存储对端sockaddr结构的存储区
* @param[out]   fromlen    空指针或指向socklen_t，用来存储sockaddr结构的大小
*
* @retval       #>=0       接收的数据字节数
* @retval       #-1        接收失败
*
* @par 错误码
*  \li 失败返回-1，错误码被设为以下的某个值
*     - <b> [EWOULDBLOCK]</b> : \n 套接字设置了O_NONBLOCK，而指定的操作被阻塞
*     - <b> [EBADF] </b>: \n 套接字描述符无效。
*     - <b> [ECONNRESET] </b>: \n 连接被对端关闭
*     - <b> [ENOTCONN] </b>: \n TCP套接字未连接
*     - <b> [EOPNOTSUPP] </b>: \n 指定的协议层不能识别选项
*     - <b> [ETIMEDOUT] </b>: \n 接收超时
*
* @par 依赖
* <ul><li>hi_socket.h: 此头文件包含接口声明</li></ul>
* @see 无
* @since 无
*/

int recvfrom(int sockfd, void* buf, size_t len, int flags,
             struct sockaddr* from, socklen_t* fromlen);

/**
* @ingroup 套接字接口
*
* @par 描述
*  关闭套接字发送或者接收操作
*
* @attention
* <ul>
* <li>此接口未完全兼容POSIX.1-2008</li>
* </ul>
*
* @param[in]    sockfd     套接字描述符
* @param[in]    how	       指定关闭的操作
*
*
* @retval       #0         成功
* @retval       #-1        失败
*
* @par 错误码
*  \li 失败返回-1，错误码被设为以下的某个值
*     - <b> [EBADF] </b>: \n 套接字描述符无效
*     - <b> [ENOTCONN] </b>: \n 套接字连接未建立
*     - <b> [EOPNOTSUPP] </b>: \n 指定的协议层不能识别选项
*     - <b> [EINVAL] </b>: \n 参数无效
*
* @par 依赖
* <ul><li>hi_socket.h: 此头文件包含接口声明</li></ul>
* @see 无
* @since 无
*/

int shutdown(int sockfd, int how);

/**
* @ingroup 套接字接口
*
* @par 描述
*  关闭套接字
*
* @attention
* <ul>
* <li>此接口未完全兼容POSIX.1-2008</li>
* </ul>
*
* @param[in]    sockfd     套接字描述符
*
* @retval       #0         成功
* @retval       #-1        失败
*
* @par 错误码
*  \li 失败返回-1，错误码被设为以下的某个值
*     - <b> [EBADF] </b>: \n 套接字描述符无效
*
* @par 依赖
* <ul><li>hi_socket.h: 此头文件包含接口声明</li></ul>
* @see 无
* @since 无
*/

int close(int sockfd);

/**
* @ingroup 套接字接口
*
* @par 描述
*  设置套接字的选项
*
* @attention
* <ul>
* <li>此接口未完全兼容POSIX.1-2008</li>
* </ul>
*
* @param[in]    sockfd        套接字描述符
* @param[in]    level         指定选项属于的协议级别
* @param[in]    option        指定需要设置的选项
* @param[out]   value         指针指向存储区
* @param[in]    value_len     value的长度
*
* @retval       #0            成功
* @retval       #-1           失败
*
* @par 错误码
*  \li 失败返回-1，错误码被设为以下的某个值:
*     - <b> [EBADF] </b>: \n 套接字描述符无效
*     - <b> [EINVAL] </b>: \n 参数无效
*     - <b> [ENOPROTOOPT] </b>: \n 协议不支持该操作
*     - <b> [EDOM] </b>: \n 传入的timeval值无效
*
* @par 依赖
* <ul><li>hi_socket.h: 此头文件包含接口声明</li></ul>
* @see 无
* @since 无
*/

int setsockopt(int sockfd, int level, int option,
               const void* value, socklen_t value_len);

/**
* @ingroup 套接字接口
*
* @par 描述
*  获取套接字的选项
*
* @attention
* <ul>
* <li>此接口未完全兼容POSIX.1-2008</li>
* </ul>
*
* @param[in]        sockfd      套接字描述符
* @param[in]        level       指定选项属于的协议级别
* @param[in]        option      指定需要获取的选项
* @param[out]       value       指针指向存储区
* @param[in,out]    value_len   value的长度
*
* @retval           #0          成功
* @retval           #-1         失败
*
* @par 错误码
*  \li 失败返回-1，错误码被设为以下的某个值:
*     - <b> [EBADF] </b>: \n 套接字描述符无效
*     - <b> [EINVAL] </b>: \n 参数无效
*     - <b> [ENOPROTOOPT] </b>: \n 协议不支持该操作
*
* @par 依赖
* <ul><li>hi_socket.h: 此头文件包含接口声明</li></ul>
* @see 无
* @since 无
*/

int getsockopt(int sockfd, int level, int option,
               void* value, socklen_t* value_len);

/**
* @ingroup 套接字接口
*
* @par 描述
*  获取与套接字关联的本地协议地址.
*
* @attention
* <ul>
* <li>此接口未完全兼容POSIX.1-2008</li>
* </ul>
*
* @param[in]        sockfd     套接字描述符
* @param[out]       addr	   套接字地址
* @param[in,out]    addrlen    套接字地址长度
*
* @retval           #0         获取与套接字关联的本地协议地址成功
* @retval           #-1        获取与套接字关联的本地协议地址失败
*
* @par 错误码
*  \li 失败返回-1，错误码被设为以下的某个值:
*     - <b> [EBADF] </b>: \n 套接字描述符无效
*     - <b> [EOPNOTSUPP] </b>: \n 指定的协议层不能识别选项
*
* @par 依赖
* <ul><li>hi_socket.h: 此头文件包含接口声明</li></ul>
* @see 无
* @since 无
*/
int getsockname(int sockfd, struct sockaddr* addr,
                socklen_t* addrlen);

/**
* @ingroup 套接字接口
*
* @par 描述
*  获取与套接字关联的远程协议地址.
*
* @attention
* <ul>
* <li>此接口未完全兼容POSIX.1-2008</li>
* </ul>
*
* @param[in]        sockfd     套接字描述符
* @param[out]       addr	   套接字地址
* @param[in,out]    addrlen    套接字地址长度
*
* @retval           #0         获取与套接字关联的远程协议地址成功
* @retval           #-1        获取与套接字关联的远程协议地址失败
*
* @par 错误码
*  \li 失败返回-1，错误码被设为以下的某个值
*     - <b> [EBADF] </b>: \n 套接字描述符无效
*     - <b> [ENOTCONN] </b>: \n 本地或远程套接字未连接
*     - <b> [EOPNOTSUPP] </b>: \n 指定的协议层不能识别选项
*     - <b> [EINVAL] </b>: \n 参数无效
*
* @par 依赖
* <ul><li>hi_socket.h: 此头文件包含接口声明</li></ul>
* @see 无
* @since 无
*/
int getpeername(int sockfd, struct sockaddr* addr,
                socklen_t* addrlen);


/**
* @ingroup 套接字接口
*
* @par 描述
*  监视一个或多个套接口的状态变化
*
* @attention
* <ul>
* <li>此接口未完全兼容POSIX.1-2008</li>
* </ul>
*
* @param[in]        nfds       监视的文件句柄数
* @param[in,out]    readfds	   监视的可读文件句柄集合
* @param[in,out]    writefds   监视的可写文件句柄集合
* @param[in,out]    exceptfds  监视的异常文件句柄集合
* @param[in]        timeout    超时结束时间:若为NULL,则为阻塞状态;若为0秒0毫秒，就变成一个纯粹的非阻塞函数
							   不管文件描述符是否有变化，都立刻返回继续执行;timeout的值大于0，等待的超时时间

* @retval           #>=0       成功
* @retval           #-1        失败
*
* @par 错误码
*  \li 失败返回-1，错误码被设为以下的某个值
*     - <b> [EBADF] </b>: \n 套接字描述符无效
*     - <b> [EINVAL] </b>: \n 参数无效
*     - <b> [ENOMEM] </b>: \n 系统内存不足
*
* @par 依赖
* <ul><li>hi_socket.h: 此头文件包含接口声明</li></ul>
* @see 无
* @since 无
*/
int select(int nfds, fd_set* readfds, fd_set* writefds,
           fd_set* exceptfds, struct timeval* timeout);


/**
* @ingroup 套接字接口
*
* @par 描述
*  对设备的I/O通道进行管理
*
* @attention
* <ul>
* <li>此接口未完全兼容POSIX.1-2008</li>
* </ul>
*
* @param[in]        s      套接字描述符
* @param[in]        cmd	   控制命令
* @param[in]        argp   命令参数
*
* @retval           #>=0   成功
* @retval           #-1    失败
*
* @par 错误码
*  \li 失败返回-1，错误码被设为以下的某个值
*     - <b> [EBADF] </b>: \n 套接字描述符无效
*     - <b> [EINVAL] </b>: \n 参数无效
*     - <b> [ENOMEM] </b>: \n 系统内存不足
*     - <b> [EAFNOSUPPORT] </b>: \n 协议不支持该操作
*
* @par 依赖
* <ul><li>hi_socket.h: 此头文件包含接口声明</li></ul>
* @see 无
* @since 无
*/
int ioctl(int s, long cmd, void* argp);

/**
* @ingroup 套接字接口
*
* @par 描述
*  根据文件描述词来操作文件的特性.
*
* @attention
* <ul>
* <li>此接口未完全兼容POSIX.1-2008</li>
* </ul>
*
* @param[in]        s      套接字描述符
* @param[in]        cmd	   选择属性命令(F_GETFL|F_SETFL)
* @param[in]        val    命令参数
*
* @retval           #>=0   成功
* @retval           #-1    失败
*
* @par 错误码
*  \li 失败返回-1，错误码被设为以下的某个值
*     - <b> [EBADF] </b>: \n 套接字描述符无效
*     - <b> [EINVAL] </b>: \n 参数无效
*
* @par 依赖
* <ul><li>hi_socket.h: 此头文件包含接口声明</li></ul>
* @see 无
* @since 无
*/
int fcntl(int s, int cmd, int val);

/**
* @ingroup 套接字接口
*
* @par 描述
*  点分十进制IP转化成长整型数.
*
* @attention
* <ul>
* <li>此接口存在功能问题，返回值-1是有效的地址（255.255.255.255）</li>
* </ul>
*
* @param[in]    cp     指向点分十进制IPv4地址字符串指针
*
* @retval       #>=0   成功,返回值为unsigned int类型
* @retval       #-1    失败
*
* @par 依赖
* <ul><li>hi_socket.h: 此头文件包含接口声明</li></ul>
* @see 无
* @since 无
*/
unsigned int inet_addr (const char* cp);

/**
* @ingroup 套接字接口
*
* @par 描述
*  网络地址转化成点分十进制IP地址字符串格式.
*
* @param[in]        in     网络主机IP地址
*
* @retval #Valid pointer   成功,返回值为分十进制IP地址字符串类型
* @retval #NULL            失败
*
* @par 依赖
* <ul><li>hi_socket.h: 此头文件包含接口声明</li></ul>
* @see 无
* @since 无
*/
char* inet_ntoa (struct in_addr in);

/**
* @ingroup 套接字接口
*
* @par 描述
*  点分十进制IP地址转化成网络IP地址.
*
* @param[in]       cp     指向点分十进制IPv4地址字符串指针
* @param[out]      addr   网络主机IP地址
*
* @retval          #0     成功
* @retval          #1     失败
*
* @par 依赖
* <ul><li>hi_socket.h: 此头文件包含接口声明</li></ul>
* @see 无
* @since 无
*/
int inet_aton(const char* cp, struct in_addr* addr);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif /* __INCLUDE_SYS_SOCKET_H */
