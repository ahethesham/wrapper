#ifndef __LISTENER_H__
#define __LISTENER_H__

#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/socket.h>
/*
 * listener to listen the incoming requests 
 */
template<typename socket >
class listener{
    
    typedef typename socket::socket_fd_type socket_type;
    socket * socket_;
    socket_type * socketFd_;
    public:
        typedef int    return_type;
        listener(socket * pSocket) : socket_(pSocket) {
                socketFd_ = pSocket->get();
        }

        return_type listen(int limit){
            // listen to the requests here now            
            // TODO ...add a check to verify if the underlying type is actually a int
            ::listen(*socketFd_ , limit);
        }
};

#endif
