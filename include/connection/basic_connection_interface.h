#ifndef __BASIC_CONNECTION_INTERFACE__H__
#define __BASIC_CONNECTION_INTERFACE__H__

#include "basic_endpoint_interface.h"
#include "basic_http_request_interface.h"
#include "basic_request_builder_interface.h"
#include "basic_response_builder.h"
#include "basic_exception.h"
#include "basic_http_response.h"
#include "basic_writer_interface.h"
#include "socket_io_handle.h"
#include "basic_reader_interface.h"
#include <functional>
#include <netdb.h>

namespace detail{
using connect_cb = std::function<int(int rc  )>;
inline int connect_wrapper(int fd , struct addrinfo * address_info ,  connect_cb &&cb ){
    struct sockaddr * address = address_info->ai_addr;
    int rc = ::connect(fd , address , address_info->ai_addrlen);
    return cb(rc );
}
}

class basic_connection_interface{
    public:
        using exception_handler = std::function<int(std::runtime_error &)>;
        using callback = std::function<void(basic_http_response_interface & , basic_exception_interface & ec)>;
        // for async IO
        virtual basic_connection_interface & send(basic_http_request_interface & req , callback && cb) = 0;
        // for sync IO
        virtual basic_http_response_interface & send(basic_http_request_interface & req) = 0;
        virtual bool is_alive() = 0;
        virtual basic_connection_interface & close() = 0;

        // set methods ... builder object has to set these objects before calling connect function
        virtual basic_connection_interface & set_response_obj(std::shared_ptr<basic_http_response_interface> obj) = 0 ;
        virtual basic_connection_interface & set_response_builder(std::shared_ptr<basic_response_builder_interface> builder) = 0;
        virtual basic_connection_interface & set_request_builder(std::shared_ptr<basic_request_builder_interface<basic_http_request_interface>> builder) = 0;
        virtual basic_connection_interface & set_endpoint_obj(std::shared_ptr<basic_endpoint_interface> obj) = 0 ;
        virtual basic_connection_interface & set_socket_obj(std::shared_ptr<ssl_socket_io_handle_interface> obj) = 0 ;
        virtual basic_connection_interface & set_writer_obj(std::shared_ptr<basic_writer_interface<ssl_socket_io_handle_interface>> obj) = 0 ;
        virtual basic_connection_interface & set_reader_obj(std::shared_ptr<basic_reader_interface<ssl_socket_io_handle_interface>> obj) = 0 ;
        virtual basic_connection_interface & set_retry_attempts(uint64_t r) = 0;
        virtual basic_connection_interface & set_read_timeout(uint64_t r) = 0;
        virtual basic_connection_interface & set_write_timeout(uint64_t r) = 0;
        virtual basic_connection_interface & set_keep_alive_timeout(uint64_t r) = 0;

        virtual basic_connection_interface & connect() = 0;
        virtual basic_connection_interface & set_exception_handler(exception_handler handler) = 0;

        basic_connection_interface() = default;
};

#endif
