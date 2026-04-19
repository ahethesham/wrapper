#ifndef __WEB_H__
#define __WEB_H__

#include "http_request_fwd.h"
#include "http_reader_v1.h"
#include "stream_fwd.h"
#include "client.h"
#include "web_fwd.h"


using http_1_0_server_reader = http_reader;
using http_1_0_server_writer = http_writer;
using https_1_0_client_reader = http_reader;
using https_1_0_client_writer = http_writer;




using http_server =  basic_server< tcp_socket,
                      tcp_endpoint ,
                      tcp_binder , 
                      basic_listener<tcp_socket> ,
                      basic_acceptor<tcp_socket> ,
                      http_server_stream , 
                      http_request ,
                      http_response ,
                      HttpServer<basic_matcher>> ;
#if 0
using http_client =  basic_client< tcp_socket ,
                      tcp_endpoint ,
                      basic_connector<tcp_connector < tcp_socket , tcp_endpoint > > ,
                      basic_stream<tcp_socket::fd_type , http_reader<http_response  > , http_writer<http_request> > ,
                      http_request ,
                      http_response ,
                      BaseClient > ;
#elif 0

using http_client =  basic_client< tcp_socket ,
                      tcp_endpoint ,
                      basic_connector<tcp_connector < tcp_socket , tcp_endpoint > > ,
                      http_client_stream , 
                      http_request ,
                      http_response ,
                      BaseClient > ;
#endif

#if 0

using https_client = basic_client<tcp_ssl_socket ,
                        tcp_endpoint ,
                        basic_connector<tcp_ssl_connector< tcp_ssl_socket , tcp_endpoint > > ,
                        basic_stream<tcp_ssl_socket::fd_type , http_reader<http_response , tcp_ssl_socket::fd_type , ssl_reader  >, http_writer<http_request , tcp_ssl_socket::fd_type , ssl_writer > > ,
                        http_request ,
                        http_response ,
                        BaseClient> ;
#else
using https_client = basic_client<tcp_ssl_socket ,
                        tcp_endpoint ,
                        basic_connector<tcp_ssl_connector< tcp_ssl_socket , tcp_endpoint > > ,
                        https_reader ,
                        https_writer ,
                        http_request ,
                        http_response ,
                        BaseClient> ;

#endif

#endif
