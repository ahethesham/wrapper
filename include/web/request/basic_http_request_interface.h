#ifndef __BASIC_HTTP_REQUEST_H__
#define __BASIC_HTTP_REQUEST_H__

#include "basic_http_headers_interface.h"
#include "basic_http_request_line_interface.h"
#include "basic_object_interface.h"
#include "buffer.h"


/*
 *A http request object has a request line , headers and the body
 */
class basic_http_request_interface{
    public:
		virtual basic_http_request_interface & set_headers( basic_http_headers_interface & headers ) = 0;
		virtual basic_http_request_interface & set_request_line( basic_http_request_line_interface & line ) = 0; 

		virtual basic_http_request_interface & set_body( basic_object_interface & body) = 0;

		virtual basic_http_headers_interface * headers() = 0;
		virtual basic_http_request_line_interface * request_line() = 0 ;
		virtual basic_object_interface * body() = 0 ;

		virtual basic_http_request_interface & set_header( std::string key , std::string value ) = 0;
		virtual basic_http_request_interface & set_uri(std::string uri) = 0;
		virtual basic_http_request_interface & set_query_param(std::string key , std::string value) = 0;
		virtual basic_http_request_interface & set_method(std::string method) = 0;

        virtual const std::string & get_query_param(std::string key) = 0;

        virtual std::string serialize(basic_formatter_interface & formatter) = 0;
};


#endif
