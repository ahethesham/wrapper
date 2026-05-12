#ifndef __SOCKET_IO_HANDLE_H__
#define __SOCKET_IO_HANDLE_H__

#include "basic_io_handle_interface.h"
#include "socket_options.h"
#include <cassert>
#include <functional>
#include <memory>
#include <openssl/ssl.h>
#include <sys/socket.h>

using socket_io_handle_interface  = basic_io_handle_interface< int , sockaddr *>;
using ssl_socket_io_handle_interface  = basic_io_handle_interface< SSL* , sockaddr *>;


namespace detail{
// creates a socket and calls given callback function
using socket_callback = std::function<int(int)>;
inline int socket_wrapper(int network , int protocol , int flags , socket_callback && cb){
    int rc = ::socket(network , protocol , flags );
    return cb(rc);
}
inline int close(int fd , socket_callback && cb ){
    int rc  = ::close(fd);
    return cb(rc);
}
}

template< auto network_type = AF_INET ,
          auto communication_type = SOCK_STREAM ,
          auto protocol_type = 0>
class socket_io_handle : public socket_io_handle_interface{
    public:
        socket_io_handle() ;
        socket_io_handle(fd_type fd);
        fd_type open() override;
        bool is_closed() override;
        socket_io_handle & close() override;
        fd_type get() override;
        int get_lowest_level_fd() override;
        int get_flags() override;
        int set_flags(int f) override;
        
    private:
        class impl;
        std::shared_ptr<impl> impl_;
};


template< auto network_type = AF_INET ,
         auto communication_type = SOCK_STREAM ,
         auto protocol_type = 0>
class ssl_socket_io_handle: public ssl_socket_io_handle_interface{
    public:
        ssl_socket_io_handle();
        ssl_socket_io_handle(socket_options & opt);
        fd_type open() override;
        bool is_closed() override;
        ssl_socket_io_handle & close() override;
        fd_type get() override;
        int get_lowest_level_fd() override;
        int get_flags() override;
        int set_flags(int f) override;
    private:
        class impl;
        std::shared_ptr<impl> impl_;
};


using tcp_socket = socket_io_handle<>;

using tcp_ssl_socket = ssl_socket_io_handle<>;
#endif
