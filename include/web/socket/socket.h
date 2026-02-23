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

/***********************************************************************************************************************/
/* Whenever a process wants to communicate to the outer world it has to open a file descripter for it ...               *
 *  it could  be through pipes , unix socket , network socket whatever it is all the sockets has to implement this and  *
 *  the base operations will always be  these                                                                           *
 *  ********************************************************************************************************************/

template <typename network_type , 
          typename communication_type , 
          typename protocol_type >
class basic_socket {


    public:
        typedef network_type::sockaddr_type     sockaddr_type;
    private:
        int fd_;
        sockaddr_type * sockaddr_;
    public:

        basic_socket( sockaddr_type * pClientAddress = nullptr) : sockaddr_(pClientAddress) 
        {
            open();
        };

        basic_socket(int fd , sockaddr_type * pClientAddress = nullptr)  : fd_(fd) , sockaddr_(pClientAddress) {}

        void open(){
            fd_ = ::socket(network_type::value_type , communication_type::value_type  , protocol_type::value_type);
            assert(fd_ > 0);
            return ;
        }
        void close(){
            fd_ = ::close(fd_);
            return ;
        }
        int & get(){
            return fd_;
        }
        virtual ~basic_socket(){
            close();
        }
};



#endif
