
#ifndef __BASIC_HTTP_REQUEST_LINE_INTERFACE_H__
#define __BASIC_HTTP_REQUEST_LINE_INTERFACE_H__
#include "basic_formatter_interface.h"
#include "basic_parser_interface.h"
#include "buffer.h"
#include <memory>
#include <string>

class basic_http_request_line_interface : public basic_parser_interface<buffer_v1>{
    public:
        virtual basic_http_request_line_interface & set_method(std::string method) = 0;
        virtual basic_http_request_line_interface & set_uri(std::string uri) = 0;
        virtual basic_http_request_line_interface & set_version(std::string version) = 0;
        virtual basic_http_request_line_interface & set_query_param(std::string key , std::string value) = 0;
        //getters
        virtual  std::string & get_method() = 0;
        virtual  std::string & get_uri() = 0;
        virtual  std::string & get_version() = 0;
        virtual  std::string & get_query_param(std::string key ) = 0;

        virtual std::string serialize(basic_formatter_interface & formatter) = 0;
        virtual std::string serialize( )= 0;

        /*
         * clone , copy , move 
         */
        virtual std::shared_ptr<basic_http_request_line_interface> clone() = 0;
        basic_http_request_line_interface(basic_http_request_line_interface &) = delete;
        basic_http_request_line_interface(basic_http_request_line_interface &&) = delete;
        basic_http_request_line_interface & operator=(basic_http_request_line_interface &) = delete;
        basic_http_request_line_interface & operator=(basic_http_request_line_interface &&) = delete;

        basic_http_request_line_interface() = default;
        virtual ~basic_http_request_line_interface() = default;

};
#endif
