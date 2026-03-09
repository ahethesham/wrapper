#ifndef __WEB_FWD_H__
#define __WEB_FWD_H__

#include <unistd.h>
#include "openssl/ssl.h"
extern struct Dummy;
/*
 * Should only contain all the declarations 
 */
//stream

template<typename fd ,
        typename reader ,
        typename writer>
class basic_stream;
inline ssize_t std_reader(int , void * , ssize_t);
inline ssize_t ssl_reader(SSL* , void * , ssize_t);
template < typename custom_protocol ,
            typename fd_type = int,
            auto reader = std_reader>
class http_reader;

inline ssize_t std_writer(int , void * , ssize_t);
inline ssize_t ssl_writer(SSL* , void * , ssize_t);
template < typename custom_protocol ,
            typename fd_type = int,
            auto writer = std_reader>
class http_writer;

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

template<typename Protocol ,
         typename Allocator = Dummy>
class basic_request ; 


template<typename Protocol ,
         typename Parser = Dummy,
         typename Allocator = Dummy>
class basic_response ; 

namespace http{
    class HttpHeaders;
    class QueryParameters;

    class RequestLine;
    class HttpRequest;
    
    class StatusLine;
    class HttpResponse;
}

typedef basic_request<http::HttpRequest  > http_request;
typedef basic_response<http::HttpResponse> http_response;

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

template < typename socketPolicy ,
           typename endpointPolicy ,
           typename connectPolicy ,
           typename streamPolicy  ,
           typename requestPolicy ,
           typename responsePolicy ,
           typename clientBase >
class basic_client ;

class BaseClient;


#endif
#include "http_reader.h"
#include "http_writer.h"
#include "http_server.h"
#include "http_response.h"
#include "basic_http_request.h"
#include "response.h"
#include "server.h"
#include "http_request.h"
#include "socket.h"
#include "tcp_socket.h"
#include "bind.h"
#include "acceptor.h"
#include "endpoint.h"
#include "listener.h"
#include "stream.h"
#include "client.h"

