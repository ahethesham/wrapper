#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include "logger.h"

template <typename socket>
class basic_acceptor{
    public:
        typedef  socket socket_type;
        typedef  typename socket::sockaddr_type sockaddr_type;
    private:  
        int  socketFd_;
        socket * socket_;

    public:
        basic_acceptor(socket * psocket) : socket_(psocket) , socketFd_(psocket->get()) { }
        socket_type accept(){
            sockaddr_type * clientaddr = new sockaddr_type();
            memset(clientaddr , 0 ,sizeof(sockaddr) );
            socklen_t len = 0;
            //log("Ready to accept requests");
            int newClient = ::accept(socketFd_ , (struct sockaddr *)clientaddr , &len);
            //log("accepted new client");
            return socket_type(newClient , *clientaddr);
        }
};

#endif

