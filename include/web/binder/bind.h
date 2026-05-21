#ifndef __BIND_H__
#define __BIND_H__

/*
 * binder has to take the socket and a endpoint reference and get the iterator and run a loop
 * untill it  successfull binds to any of the interface 
 */
#include <cassert>
#include <stdexcept>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include "web_fwd.h"

template <typename binder>
class basic_binder{
    typedef binder binder_type;
    typedef binder::socket_type socket_type;
    typedef binder::endpoint_type endpoint_type;

    public:
        basic_binder(socket_type &  socket , endpoint_type & endpoint){
            auto itr = endpoint.iterator();
            while(itr.hasNext()){
                auto currentAddress = itr.getNext();
                if(::bind(socket.get() , currentAddress->ai_addr , currentAddress->ai_addrlen) == 0){
                    /*
                     * Bind is successfull return now
                     */
                    return ;
                }
            }
            assert(false );
        }
};

struct TcpBinder{
        typedef tcp_socket socket_type;
        typedef tcp_endpoint endpoint_type;
};

#endif
