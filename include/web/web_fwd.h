#ifndef __WEB_FWD_H__
#define __WEB_FWD_H__


extern struct Dummy;
/*
 * Should only contain all the declarations 
 */
//stream

template<typename fd ,
        typename reader ,
        typename writer>
class basic_stream;
class http_reader;
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
         typename Parser = Dummy,
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
    class HttpResponse;
    typedef basic_request<HttpRequest  > http_request;
    typedef basic_response<HttpResponse> http_response;
}

template <typename network_type , 
          typename communication_type , 
          typename protocol_type >
class basic_socket ;

struct ipv4_network_domain ;
struct tcp_communication_type ;
struct default_protocol_type;
typedef basic_socket<ipv4_network_domain ,
                     tcp_communication_type ,
                     default_protocol_type > tcp_socket;

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
class HttpServer;

typedef basic_server< tcp_socket,
                      tcp_endpoint ,
                      tcp_binder , 
                      basic_listener<tcp_socket> ,
                      basic_acceptor<tcp_socket> ,
                      basic_stream<int , http_reader , http_writer> ,
                      http::http_request ,
                      http::http_response ,
                      HttpServer> http_server; 

#endif
#include  "request.h"
#include "response.h"
#include "server.h"
#include "http_request.h"
#include "socket.h"
#include "tcp_socket.h"
#include "http_response.h"
#include "http_server.h"
#include "bind.h"
#include "acceptor.h"
#include "endpoint.h"
#include "listener.h"
#include "stream.h"
#include "http_reader.h"
#include "http_writer.h"


