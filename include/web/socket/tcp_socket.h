#ifndef __TCP_SOCKET_H__
#define __TCP_SOCKET_H__

#include <sys/socket.h>
#include "socket.h"
class TcpSocket{
    public:
        typedef int fd_type;
        static short network_type ;
        static short communication_type;
        static short protocol_type;
};
short TcpSocket::network_type = AF_INET;
short TcpSocket::communication_type = SOCK_STREAM;
short TcpSocket::protocol_type = 0;

typedef basic_socket<TcpSocket> tcp_socket;
#endif
