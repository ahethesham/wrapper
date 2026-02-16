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
#include "address.h"


template <typename socket_type>
class basic_socket{

    typedef socket_type value_type;

    int fd_;
    
    public:
        basic_socket(int port , std::string &address ) 
        {
            open();
        };

        void open(){
            /*
             * Open the socket based on the socket_type and stream type provided in the template 
             */
            fd_ = ::socket(value_type::socket_type , value_type::stream_type , 0);
            assert(fd_ > 0);
            return ;
        }
        int get(){
            return fd_;
        }
};



#endif
