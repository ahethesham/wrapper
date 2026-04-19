#ifndef __BASIC_REQUEST_BUILDER_INTERFACE_H__
#define __BASIC_REQUEST_BUILDER_INTERFACE_H__

#include "basic_http_request_interface.h"
#include <vector>

template < typename T>
class basic_request_builder_interface {
    public: 
        virtual T& build() = 0;
        /*
         * Options to set the request line
         */
        virtual basic_request_builder_interface & set_method(std::string method) = 0;
        virtual basic_request_builder_interface & set_uri(std::string method) = 0;
        virtual basic_request_builder_interface & set_query_param(std::string key , std::string value) = 0;
        virtual basic_request_builder_interface & set_query_param(std::vector<std::pair<std::string ,   std::string >  > & params) = 0;
        virtual basic_request_builder_interface & set_version(std::string version) = 0;
        /*
         *   Options to set headers
         */
        virtual basic_request_builder_interface & set_header(std::string key , std::string value) = 0;
        virtual basic_request_builder_interface & set_header(std::vector<std::pair<std::string , std::string >> &) = 0;
        /*
         * Options to set the body
         */
        virtual basic_request_builder_interface & set_body(basic_object_interface & body) = 0;

};

#endif
