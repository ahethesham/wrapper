#ifndef __WEB_FWD_H__
#define __WEB_FWD_H__

#include <unistd.h>
#include "openssl/ssl.h"
#include "json_fwd.h"

template<typename socket>
class basic_acceptor;

template<typename binder>
class basic_binder;
struct TcpBinder;
typedef basic_binder<TcpBinder> tcp_binder;

template <typename endpoint  >
class basic_endpoint ;
class endpoint;
typedef basic_endpoint<endpoint> tcp_endpoint;

template<typename socket>
class basic_listener;


template < typename socket_creator>
class basic_socket ;

struct ipv4_network_domain ;
struct tcp_communication_type ;
struct default_protocol_type;

template<typename network_type ,
        typename communication_type ,
        typename protocol_type >
class tcp_socket_creator;

template<typename network_type ,
        typename communication_type ,
        typename protocol_type >
class tcp_ssl_socket_creator;

using tcp_socket = basic_socket<tcp_socket_creator<ipv4_network_domain ,
                     tcp_communication_type ,
                     default_protocol_type> > ;

typedef basic_socket<tcp_ssl_socket_creator<ipv4_network_domain ,
                     tcp_communication_type ,
                     default_protocol_type> > tcp_ssl_socket;
template< typename socket ,
          typename endpoint,
          typename binder ,
          typename listener ,
          typename acceptor ,
          typename stream  ,
          typename request ,
          typename response ,
          typename base_server>
class basic_server ; 
template<class matcherBase>
class HttpServer;

class basic_matcher;


template < typename connectorPolicy >
class basic_connector;

class BaseClient;
template < typename socketPolicy ,
           typename endpointPolicy ,
           typename connectPolicy ,
           typename streamPolicy  ,
           typename requestPolicy ,
           typename responsePolicy ,
           typename clientBase >
class basic_client ;



#endif

#include "server.h"
#include "socket.h"
#include "tcp_socket.h"
#include "bind.h"
#include "endpoint.h"
#include "listener.h"
#include "client.h"
#include "buffer.h"
#include "http_server.h"
#include "http_request_fwd.h"
#include "connector.h"
