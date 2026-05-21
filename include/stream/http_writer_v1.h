#ifndef __HTTP_WRITER_V1_H__
#define __HTTP_WRITER_V1_H__
#include "io_handles_fwd.h"
#include "buffer.h"
#include "basic_writer_interface.h"
#include "socket_io_handle.h"
#include <memory>
#include <openssl/ssl.h>

namespace detail{
    using writer_callback = std::function<int(int rc)>;
    inline int std_writer(int fd , void * buffer , ssize_t size , writer_callback cb){
        int rc = ::write(fd , buffer , size);
        return cb(rc);
    }
    inline int std_tls_writer(SSL *  fd , void * buffer , ssize_t size , writer_callback cb){
        int rc = ::SSL_write(fd , buffer , size);
        return cb(rc);
    }
}

template< typename io_handle_policy ,
          auto func>
class http_writer_v1 : public basic_writer_interface< io_handle_policy >{
    public:
        using base_type      = basic_writer_interface< io_handle_policy >;
        using self_type      = http_writer_v1;
        using io_handle_type = io_handle_policy;
        using size_type      = base_type::size_type;
        using buffer_type    = base_type::buffer_type;
        using parser_type    = base_type::parser_type;
        
        http_writer_v1(std::shared_ptr<io_handle_type> handle_);
        
        size_type write(void * buffer , ssize_t size) override;
        size_type write(buffer_type * buffer) override;
        size_type write(parser_type & parser) override;

        self_type & operator<<(parser_type & parser) override;
        self_type & operator<<(std::basic_string<char> & str) override;
        self_type & operator<<(const char * str) override;
        self_type & operator<<(int num) override;
        base_type & operator<<(std::function<base_type &( base_type & ) > flush) override;
        
        self_type & flush() override;
    private:
        std::shared_ptr<io_handle_type>  handle_;
        std::shared_ptr<buffer_type> buffer_;

};

template class http_writer_v1<socket_io_handle_interface , detail::std_writer>;
template class http_writer_v1<ssl_socket_io_handle_interface , detail::std_tls_writer>;

using http_writer = http_writer_v1<socket_io_handle_interface , detail::std_writer>;
using https_writer = http_writer_v1<ssl_socket_io_handle_interface , detail::std_tls_writer>;

#endif

