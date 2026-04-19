
#ifndef __BASIC_HTTP_REQUEST_LINE_INTERFACE_H__
#define __BASIC_HTTP_REQUEST_LINE_INTERFACE_H__
#include "basic_formatter_interface.h"
#include <string>

class basic_http_request_line_interface{
    public:
        virtual basic_http_request_line_interface & set_method(std::string method) = 0;
        virtual basic_http_request_line_interface & set_uri(std::string uri) = 0;
        virtual basic_http_request_line_interface & set_version(std::string version) = 0;
        virtual basic_http_request_line_interface & set_query_param(std::string key , std::string value) = 0;
        //getters
        virtual const std::string & get_method() = 0;
        virtual const std::string & get_uri() = 0;
        virtual const std::string & get_version() = 0;
        virtual const std::string & get_query_param(std::string key ) = 0;

        virtual std::string serialize(basic_formatter_interface & formatter) = 0;
};
#endif
