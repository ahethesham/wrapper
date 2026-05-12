#ifndef __BASIC_HTTP_RESPONSE_LINE_INTERFACE_H__
#define __BASIC_HTTP_RESPONSE_LINE_INTERFACE_H__

#include "basic_parser_interface.h"
#include "buffer.h"
#include "http_status_codes.h"
#include "basic_formatter_interface.h"
#include <memory>

class basic_http_response_line_interface : public basic_parser_interface <buffer_v1>{
    public:
        basic_http_response_line_interface() = default;
        // set methods
        virtual basic_http_response_line_interface & set_status(http_status &) = 0;
        virtual basic_http_response_line_interface & set_version(std::string version) = 0;
        
        //get methods 
        virtual const http_status & status() const = 0;
        virtual std::string & version() const = 0;
        
        // copy , move , clone 
        basic_http_response_line_interface(basic_http_response_line_interface&) = delete;
        basic_http_response_line_interface(basic_http_response_line_interface &&) = delete;
        virtual basic_http_response_line_interface & operator=(basic_http_response_line_interface &) = delete;
        virtual basic_http_response_line_interface & operator=(basic_http_response_line_interface &&) = delete ;
        virtual std::shared_ptr<basic_http_response_line_interface> clone() = 0 ;

        // serialize , deserialize
        virtual std::string serialize() = 0;
        virtual std::string serialize(basic_formatter_interface & formatter) = 0;

        // reset , clear
        virtual basic_http_response_line_interface & clear() = 0;
};

#endif
