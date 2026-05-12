#ifndef __BASIC_REQUEST_BUILDER_INTERFACE_H__
#define __BASIC_REQUEST_BUILDER_INTERFACE_H__

#include "basic_http_request_interface.h"
#include <vector>

template < typename T>
class basic_request_builder_interface {
    public: 
        // builds and returns reference to the object 
        virtual T& build() = 0;
        /*
         * Options to set the request line
         */
        virtual basic_request_builder_interface & set_method(std::string method) = 0;
        virtual basic_request_builder_interface & set_uri(std::string method) = 0;
        virtual basic_request_builder_interface & add_query_param(std::string key , std::string value) = 0;
        virtual basic_request_builder_interface & add_query_param(std::vector<std::pair<std::string ,   std::string >  > & params) = 0;
        virtual basic_request_builder_interface & set_version(std::string version) = 0;
        /*
         *   Options to set headers
         */
        virtual basic_request_builder_interface & add_header(std::string key , std::string value) = 0;
        virtual basic_request_builder_interface & add_header(std::vector<std::pair<std::string , std::string >> &) = 0;
        virtual basic_request_builder_interface & set_headers(basic_http_headers_interface & headers) = 0;
        /*
         * Options to set the body
         */
        virtual basic_request_builder_interface & set_body(basic_object_interface & body) = 0;
        
        // each implementation should declare their own copy , move constructors and assignments 
        basic_request_builder_interface(basic_request_builder_interface &) = delete;
        basic_request_builder_interface(basic_request_builder_interface &&) = delete;
        basic_request_builder_interface & operator=(basic_request_builder_interface &) = delete;
        basic_request_builder_interface & operator=(basic_request_builder_interface &&) = delete;

        basic_request_builder_interface() = default;

};

#endif
