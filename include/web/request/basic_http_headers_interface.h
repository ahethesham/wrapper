#ifndef __BASIC_HTTP_HEADERS_INTERFACE_H__
#define __BASIC_HTTP_HEADERS_INTERFACE_H__
#include "basic_formatter_interface.h"
#include <string>
class basic_http_headers_interface{

    public:
        virtual const std::string & operator[](std::string  key) = 0;
        virtual const std::string & get(std::string key) = 0;
        virtual basic_http_headers_interface & set(std::string key , std::string value)= 0;
    
        
        basic_http_headers_interface & operator=(basic_http_headers_interface &) = delete;
        basic_http_headers_interface & operator=(basic_http_headers_interface &&) = delete;

        basic_http_headers_interface(basic_http_headers_interface &) = delete;
        basic_http_headers_interface(basic_http_headers_interface &&) = delete;

        basic_http_headers_interface() = default;
        virtual std::string serialize(basic_formatter_interface & formatter) = 0;
};

#endif
