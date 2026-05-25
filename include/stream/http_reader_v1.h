#ifndef __HTTP_READER_H__
#define __HTTP_READER_H__

#include "basic_parser_interface.h"
#include "basic_io_handle_interface.h"
#include "buffer.h"
#include "basic_reader_interface.h"
#include <iostream>
#include <memory>
#include <sys/socket.h>
#include "file_logger_v1.h"
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "socket_io_handle.h"

using http_parser_interface    = basic_parser_interface<buffer_v1>;


using socket_io_handle_interface  = basic_io_handle_interface< int , sockaddr *>;
using ssl_socket_io_handle_interface = basic_io_handle_interface< SSL * , sockaddr *>;

namespace detail{
    using reader_callback = std::function<int(int rc )>;
inline int std_reader(int fd , void * buffer , ssize_t size , reader_callback cb){
    int rc = ::read(fd , buffer , size);
    return cb(rc);
}
inline int std_tls_reader(SSL * fd , void * buffer , ssize_t size , reader_callback cb){
    int rc;
    do{
        rc = ::SSL_read(fd , buffer , size);
        if(rc < 0){
            int err = SSL_get_error( fd , rc);
            if(err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE){
                std::cout << "connection interrupted retrying " << std::endl;
                continue;
            }else if(err == SSL_ERROR_ZERO_RETURN ){
                // clean close 
                rc = 0;
                break;
            }else{
                // fatal error
                while ((err = ERR_get_error()) != 0) {
                    char buf[256];
                    ERR_error_string_n(err, buf, sizeof(buf));
                    printf("OpenSSL error: %s\n", buf);
                    fflush(stdout);
                }
                rc  = 0;
                break;
            }
        }
        break;
    }while(1);
    return cb(rc);
}
}



template< typename io_handle_policy ,
          auto     reader_func>
class http_reader_v1 : public basic_reader_interface< io_handle_policy>{
    public:
        using base_type = basic_reader_interface<io_handle_policy>;
        using self_type = http_reader_v1;
        using size_type = base_type::size_type;
        using io_handle_type = io_handle_policy;
        using buffer_type = base_type::buffer_type;

        http_reader_v1(io_handle_type & handle);
        http_reader_v1(std::shared_ptr<io_handle_type> handle);
        http_reader_v1();


        size_type read(void * buffer , ssize_t size) override;
        self_type & read(http_parser_interface & parser) override;

        size_type read(buffer_type * buffer) override;

        self_type & operator>>(http_parser_interface & parser) override;


    private:
        std::shared_ptr<io_handle_type> handle_;
        buffer_v1 * buffer_;
};

template class http_reader_v1<socket_io_handle_interface , detail::std_reader>;
template class http_reader_v1<ssl_socket_io_handle_interface , detail::std_tls_reader>;

using http_reader = http_reader_v1< socket_io_handle_interface, detail::std_reader>;
using https_reader = http_reader_v1<ssl_socket_io_handle_interface , detail::std_tls_reader>;
#endif
