#ifndef __HTTP_REQUEST_BUILDER_H__
#define __HTTP_REQUEST_BUILDER_H__
#include "basic_request_builder_interface.h"
#include "http_request_v1.h"

template<typename T>
class http_request_builder_v1 : public basic_request_builder_interface<T>{
    using base_type = basic_request_builder_interface<T>;
    using self_type = http_request_builder_v1<T>;
    public:
        T& build() override;

        self_type & set_method(std::string method) override;
        self_type & set_uri(std::string uri) override;
        self_type & add_query_param(std::vector<std::pair<std::string , std::string >> & ) override;
        self_type & add_query_param(std::string key , std::string value) override;
        self_type & set_version(std::string http_version) override;

        // set headers
        self_type & add_header(std::string key , std::string value) override;
        self_type & add_header(std::vector<std::pair<std::string , std::string >> &) override;
        self_type & set_headers(basic_http_headers_interface & headers) override;
        
        // set body
        self_type & set_body(basic_object_interface & body) override;

        http_request_builder_v1() ;
        http_request_builder_v1(http_request_builder_v1 & obj);
        http_request_builder_v1(http_request_builder_v1 && obj);
        http_request_builder_v1 & operator=(http_request_builder_v1 & obj) ;
        http_request_builder_v1 & operator=(http_request_builder_v1 && obj) ;

    private:
        class impl;
        impl * impl_;
};

template class http_request_builder_v1<http_request_v1>;

using http_builder = http_request_builder_v1<http_request_v1>;

#endif
