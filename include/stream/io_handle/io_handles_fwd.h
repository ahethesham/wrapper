#ifndef __IO_HANDLE_FWD_H__
#define __IO_HANDLE_FWD_H__

#include <cstdio>
#include <sys/socket.h>
#include <openssl/ssl.h>
#include "basic_io_handle_interface.h"

/*
 * If anybody wants to use file fd then use this interface
 */
using file_io_handle_interface = basic_io_handle_interface<FILE * , const char *>;


/*
 * If anybody wants to use socket fd  then use this interface
 */
using socket_io_handle_interface  = basic_io_handle_interface< int , sockaddr *>;
using ssl_socket_io_handle_interface = basic_io_handle_interface< SSL * , sockaddr *>;



/*
 * Implementation to open a tcp socket 
 */
template< auto network_type = AF_INET ,
          auto communication_type = SOCK_STREAM ,
          auto protocol_type = 0>
class socket_io_handle;

/*
 * Implementation to open a tcp ssl socket 
 */
template< auto network_type = AF_INET ,
          auto communication_type = SOCK_STREAM ,
          auto protocol_type = 0>
class ssl_socket_io_handle;

using tcp_socket = socket_io_handle<>;

using tcp_ssl_socket = ssl_socket_io_handle<>;

#endif
