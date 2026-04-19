#ifndef __HTTP_REQUEST_FWD_H__
#define __HTTP_REQUEST_FWD_H__

#include "json_fwd.h"


struct buffer_v1;

template< typename impl>
class basic_status_line;

template<typename buffer_Policy = buffer_v1>
class statusLine_1_0;

template< typename impl>
class basic_request_line;

template<typename buffer_Policy = buffer_v1>
class requestLine_1_0;

template<typename impl>
class basic_http_headers;

using http_1_0_status_line  = basic_status_line<statusLine_1_0<>>;

using http_1_0_request_line = basic_request_line<requestLine_1_0 <> >;

template<typename buffer_Policy = buffer_v1 ,
         typename status_line_policy = http_1_0_status_line>
class http_1_0_headers_impl;


using http_1_0_request_headers = basic_http_headers<http_1_0_headers_impl<buffer_v1 , http_1_0_request_line>>;

using http_1_0_response_headers = basic_http_headers<http_1_0_headers_impl<>>;


template< typename status_line_policy = http_1_0_request_line ,
          typename header_policy =  http_1_0_request_headers,
          typename body_policy = json_object ,
          typename buffer_Policy = buffer_v1>
class basic_http_request;

template< typename status_line_policy = http_1_0_status_line ,
          typename header_policy =  http_1_0_request_headers,
          typename body_policy = json_object ,
          typename buffer_Policy = buffer_v1>
class basic_http_response;



using http_request = basic_http_request< http_1_0_request_line , http_1_0_request_headers , json_object  > ;

using  http_response = basic_http_response< http_1_0_status_line , http_1_0_response_headers , json_object >;

#endif

#include "buffer.h"
#include "basic_status_line.h"
#include "basic_headers.h"
#include "basic_http_request.h"
#include "basic_http_response.h"
