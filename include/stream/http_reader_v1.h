#ifndef __HTTP_READER_H__
#define __HTTP_READER_H__

#include "basic_parser_interface.h"
#include "basic_io_handle_interface.h"
#include "buffer.h"
#include "basic_reader_interface.h"
#include <sys/socket.h>
#include "openssl/ssl.h"

using http_parser_interface    = basic_parser_interface<buffer_v1>;


using socket_io_handle_interface  = basic_io_handle_interface< int , sockaddr *>;
using ssl_socket_io_handle_interface = basic_io_handle_interface< SSL * , sockaddr *>;




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
        http_reader_v1(io_handle_type * handle);


        size_type read(void * buffer , ssize_t size) override;
        self_type & read(http_parser_interface & parser) override;

        size_type read(buffer_type * buffer) override;

        self_type & operator>>(http_parser_interface & parser) override;
    private:
        io_handle_type * handle_;
        buffer_v1 * buffer_;
};

#endif
