#ifndef __BASIC_ACCEPTOR_INTERFACE_H__
#define __BASIC_ACCEPTOR_INTERFACE_H__

#include "io_handles_fwd.h"
#include <openssl/ssl.h>
#include <memory>
using socket_fd = int;

struct acceptor{
    using client_handle_type = socket_io_handle_interface;
    using server_handle_type = socket_io_handle_interface;

    inline int accept(std::shared_ptr<acceptor::server_handle_type>handle) ;
};

struct ssl_acceptor{
    using client_handle_type = ssl_socket_io_handle_interface;
    using server_handle_type = ssl_socket_io_handle_interface;

    inline int accept(std::shared_ptr<server_handle_type > handle) ;
};

/*
 * responsible for accepting incoming client fd from waiting queue
 */
template < typename accept_policy = acceptor>
class basic_acceptor_interface {
    public:
        using client_handle_type = typename accept_policy::client_handle_type;

        virtual std::shared_ptr<client_handle_type > accept() = 0;

        virtual int current_req_count() = 0;

        virtual int close_connection(client_handle_type & handle) = 0;
};

template class basic_acceptor_interface<ssl_acceptor>;

using basic_ssl_acceptor_interface = basic_acceptor_interface<ssl_acceptor>;

#endif
