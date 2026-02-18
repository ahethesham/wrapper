#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <algorithm>
#include <cassert>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


template <typename socket_type>
class basic_socket{

    typedef socket_type value_type;

    typename value_type::fd_type        fd_;
    
    public:
        basic_socket( ) 
        {
            open();
        };

        void open(){
            /*
             * Open the socket based on the socket_type and stream type provided in the template 
             */
            fd_ = ::socket( value_type::network_type  , value_type::communication_type  , value_type::protocol_type);
            assert(fd_ > 0);
            return ;
        }
        typename value_type::fd_type get(){
            return fd_;
        }
};



#endif
