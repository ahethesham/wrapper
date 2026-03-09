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

template <typename creational_policy > // should have implementation for opening and closing the  
class basic_socket : public creational_policy{


    public:
        typedef creational_policy::sockaddr_type     sockaddr_type;
        typedef creational_policy::fd_type       fd_type ;
        using creational_policy::network;
    private:
        //creational_policy * socket_creator_;
    public:

        basic_socket( )  : creational_policy()
        {
        };

        basic_socket(fd_type fd )  : creational_policy(fd) {}

        basic_socket(fd_type fd , sockaddr_type addr) : creational_policy(fd) {}
#if 0
        void open(){
            fd_ = ::socket(network , communication_type::value_type  , protocol_type::value_type);
            assert(fd_ > 0);
            return ;
        }
#endif
        void close(){
            creational_policy::close();
            return ;
        }
        fd_type get(){
            return creational_policy::get();
        }
        virtual ~basic_socket(){
            close();
        }
};



#endif
