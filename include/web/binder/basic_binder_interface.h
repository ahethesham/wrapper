#ifndef __BASIC_BINDER_INTERFACE_H__
#define __BASIC_BINDER_INTERFACE_H__

/*
 * binds a spricific interface to the fd
 */
#include "basic_endpoint_interface.h"
#include "http_reader_v1.h"
#include <string>

/*
 * will take a endpoint object and iterate through it untill it successfully binds to a single interface 
 */
class basic_bind_and_listen_interface {
    public:
        virtual int bind(std::shared_ptr<socket_io_handle_interface>  handle , std::shared_ptr<basic_endpoint_interface>  endpoint) = 0;
        virtual int bind() = 0;
        virtual const std::string & ip_address() = 0;
        virtual const int protocol() = 0; // AF_INET or AF_INET6
        virtual const int stream() = 0 ; // TCP or UDP stream
        virtual int listen(int backlog) = 0;
        basic_bind_and_listen_interface() = default;
        ~basic_bind_and_listen_interface() = default;
};

#endif
