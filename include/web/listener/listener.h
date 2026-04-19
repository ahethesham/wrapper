#ifndef __LISTENER_H__
#define __LISTENER_H__

#include <cassert>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "logger.h"
/*
 * listener to listen the incoming requests 
 */
template<typename socket >
class basic_listener{
    
    socket * socket_;
    int      socketFd_;
    public:
        typedef socket socket_type;
        typedef int    return_type;
        basic_listener(socket_type * pSocket) : socket_(pSocket)  , socketFd_(pSocket->get()) { }

        return_type listen(int limit){
            // listen to the requests here now            
            // TODO ...add a check to verify if the underlying type is actually a int
            assert(::listen(socketFd_ , limit) == 0);
            log("Started listening ");
            return 0;
        }
};

#endif
