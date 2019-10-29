
Contents
1   Configuration Macros
1.1 lwipopts.h
1.1.1   LWIP_NUM_SOCKETS
1.1.2   MEMP_NUM_NETCONN
1.1.3   MEMP_NUM_TCP_PCB_LISTEN
1.1.4   MEMP_NUM_TCP_PCB
1.1.5   MEMP_NUM_UDP_PCB
1.1.6   MEMP_NUM_RAW_PCB
1.1.7   MEMP_NUM_TCP_SEG
1.1.8   TCPIP_MBOX_SIZE
1.1.9   TCPIP_PRTY_MBOX_SIZE
1.1.10  DEFAULT_ACCEPTMBOX_SIZE
1.1.11  DEFAULT_TCP_RECVMBOX_SIZE
1.1.12  DEFAULT_UDP_RECVMBOX_SIZE
1.1.13  DEFAULT_RAW_RECVMBOX_SIZE



1   Configuration Macros
1.1 lwipopts.h
1.1.1   LWIP_NUM_SOCKETS
This Macro is used for specifying number of sockets to be used.
Related Macro:
 MEMP_NUM_NETCONN, MEMP_NUM_UDP_PCB, MEMP_NUM_TCP_PCB, MEMP_NUM_TCP_PCB_LISTEN

1.1.2   MEMP_NUM_NETCONN
This Macro is used for Specifying num of netconn.
Impact:
If number of netconn is less than number of sockets, than socket creation will fail after netconn are exhausted.
Example:
  #define LWIP_NUM_SOCKETS 20
  #define MEMP_NUM_NETCONN 10
  Here we can create max of 10 sockets only. Even though number of sockets are 20.
Related Macros:
LWIP_NUM_SOCKETS

1.1.3   MEMP_NUM_TCP_PCB_LISTEN
This Macro is used to specify the number of listening TCP pcbs.
 No of listen PCB  <=  MEMP_NUM_TCP_PCB/2.

     Impact:
     If no of listen PCB are more than half of the TCP pcbs. Then few of the listen PCB will fail in establishing the TCP connection due to less number of TCP pcbs.
Example:
  #define MEMP_NUM_TCP_PCB_LISTEN 11
  #define MEMP_NUM_TCP_PCB 20
If a server creates 11 listen sockets, and out of which 9 listen sockets are connected with clients. Then total of 20 PCBs are already used and remaining two listen PCB will never establish connection with clients.
Related Macro:
MEMP_NUM_TCP_PCB

1.1.4   MEMP_NUM_TCP_PCB
This macro is used to specify the number of TCP pcbs. TCP  pcbs should not be less than number of sockets
Impact:
If we are trying to create more than 20 TCP sockets, than TPC socket creation fails.
Example:
  #define LWIP_NUM_SOCKETS 32
  #define MEMP_NUM_TCP_PCB 20
Related Macros:
LWIP_NUM_SOCKETS

1.1.5   MEMP_NUM_UDP_PCB
This macro is used to specify the number of UDP pcbs. UDP pcbs should not be less than number of sockets
Impact:
If we are trying to create more than 20 UDP sockets, than UDP socket creation fails.
Example:
  #define LWIP_NUM_SOCKETS 32
  #define MEMP_NUM_UDP_PCB 20
Related Macros:
LWIP_NUM_SOCKETS



1.1.6   MEMP_NUM_RAW_PCB
This macro is used to specify the number of RAW pcbs. RAW pcbs should not be less than number of sockets
Impact:
If we are trying to create more than 20 RAW sockets, than RAW socket creation fails.
Example:
  #define LWIP_NUM_SOCKETS 32
  #define MEMP_NUM_RAW_PCB 20
Related Macros:
LWIP_NUM_SOCKETS

1.1.7   MEMP_NUM_TCP_SEG
This Macro is used to specify the number of TCP segments.

1.1.8   TCPIP_MBOX_SIZE
The mailbox size for the tcpip threads messages. MBOX size is based on number of possible message numbers can be received before tcpipthread can process it.
MBOX size = Number of messages can be received + total number of parallel messages for internal processing

1.1.9   TCPIP_PRTY_MBOX_SIZE
 The TCPIP PRIORITY MBOX size is for TCPIP priority thread for messages like driver stop and driver wake messages. The queue size value can has to be increased if numbers of interface are more.
Impact:
If size of priority mbox is less and interfaces are more. Then there is the chance that some of the driver status message can be dropped due to mbox is full with other interface events.
Example:
Number Interface: 10
#define TCPIP_PRTY_MBOX_SIZE 5
If an interfaces driver status is changing frequently due to huge data over it. Then priority mbox will be full will driver events. Then there is a chance that some of the events will be dropped.

1.1.10  DEFAULT_ACCEPTMBOX_SIZE
The mailbox size for the incoming connections.
Impact:
If accept mbox size is less than the incoming connection rate than new incoming connection will be dropped.

1.1.11  DEFAULT_TCP_RECVMBOX_SIZE
The mailbox size for the incoming packets on a TCP NETCONN.
mbox size is based on number of possible message numbers can be received before application processes it.

1.1.12  DEFAULT_UDP_RECVMBOX_SIZE
The mailbox size for the incoming packets on a UDP NETCONN.
mbox size is based on number of possible message numbers can be received before application processes it.

1.1.13  DEFAULT_RAW_RECVMBOX_SIZE
The mailbox size for the incoming packets on a RAW NETCONN.
mbox size is based on number of possible message numbers can be received before application processes it.




