#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include <cstring>
#include <unistd.h>
#include <sys/socket.h>

template <typename socket>
class acceptor{
    typedef typename socket::socket_type socket_type;
      
    socket_type *socketFd_;
    socket * socket_;

    public:
        acceptor(socket * psocket) : socket_(psocket)  {socketFd_(*psocket->get());}
        socket_type accept(){
            struct sockaddr clientaddr;
            memset(&clientaddr , 0 ,sizeof(sockaddr) );
            socklen_t len = 0;
            socket_type newClient = ::accept(*socketFd_ , &clientaddr , &len);

            return newClient;
        }

     


};


#endif

