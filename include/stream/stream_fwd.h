#ifndef __STREAM_FWD_H__
#define __STREAM_FWD_H__
#include "web_fwd.h"
#include "buffer.h"
#include "http_request_fwd.h"

template < typename istream_policy ,
           typename ostream_policy >
class basic_stream;

struct buffer_v1;

inline ssize_t std_reader(int fd , void * buffer , ssize_t size);
inline ssize_t ssl_reader(SSL * , void * , ssize_t );

template < typename socket_policy = tcp_ssl_socket , 
           typename buffer_policy = buffer_v1  ,
           auto reader = ssl_reader>
class basic_reader;

using http_1_0_reader = basic_reader<tcp_socket , buffer_v1 , std_reader>;
using https_1_0_reader = basic_reader<tcp_ssl_socket , buffer_v1 , ssl_reader>;

template < typename impl>
class basic_http_writer;

template<typename socket_policy = tcp_ssl_socket ,
         typename buffer_policy = buffer_v1 ,
         auto writer = std_writer>
class http_1_0_writer_impl;

using http_1_0_writer = basic_http_writer<http_1_0_writer_impl<>>;
using https_1_0_writer = basic_http_writer<http_1_0_writer_impl<tcp_ssl_socket , buffer_v1 , ssl_writer > >;

template < typename socket_policy  ,
           typename writer_policy  ,
           typename parser_policy >
class basic_ostream;
template <typename socket_policy = tcp_socket ,
          typename reading_policy = https_1_0_reader,
          typename parsing_policy = http_request >
class basic_istream  ;


template < typename istream_policy ,
           typename ostream_policy >
class basic_stream;


using http_server_ostream = basic_ostream<tcp_socket , http_1_0_writer , http_response>;
using http_client_ostream = basic_ostream<tcp_socket , http_1_0_writer , http_request>;
using https_client_ostream = basic_ostream<tcp_ssl_socket , https_1_0_writer , http_request>;
using https_server_ostream = basic_ostream<tcp_ssl_socket , https_1_0_writer , http_response>;



using http_client_istream = basic_istream<tcp_socket ,http_1_0_reader ,  http_response >;
using http_server_istream = basic_istream<tcp_socket , http_1_0_reader , http_request >;
using https_client_istream = basic_istream<tcp_ssl_socket , https_1_0_reader , http_response >;
using https_server_istream = basic_istream<tcp_ssl_socket , https_1_0_reader , http_request >;


using http_client_stream = basic_stream < http_client_istream , http_client_ostream>;
using https_client_stream = basic_stream< https_client_istream , https_client_ostream>;
using http_server_stream = basic_stream< http_server_istream , http_server_ostream>;
using https_server_stream = basic_stream< https_server_istream , https_server_ostream>;

#endif

#include "basic_istream.h"
#include "basic_ostream.h"
#include "stream.h"

