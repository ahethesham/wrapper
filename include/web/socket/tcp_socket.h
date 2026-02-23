#ifndef __TCP_SOCKET_H__
#define __TCP_SOCKET_H__

#include <sys/socket.h>
#include "web_fwd.h"


struct ipv4_network_domain{
    static constexpr short value_type = AF_INET;
    typedef struct sockaddr_in sockaddr_type;
};

struct tcp_communication_type{
    static constexpr short value_type = SOCK_STREAM;
};

struct default_protocol_type{
    static constexpr short value_type = 0;
};

typedef basic_socket<ipv4_network_domain ,
                     tcp_communication_type ,
                     default_protocol_type > tcp_socket;
#endif
