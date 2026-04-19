#ifndef __HTTP_REQUEST_BUILDER_H__
#define __HTTP_REQUEST_BUILDER_H__
#include "basic_request_builder_interface.h"

template<typename T>
class http_request_builder_v1 : public basic_request_builder_interface<T>{
    using base_type = basic_request_builder_interface<T>;
    using self_type = http_request_builder_v1<T>;
    public:
        T& build() override;

        self_type & set_method(std::string method) override;
        self_type & set_uri(std::string uri) override;
        self_type & set_query_param(std::vector<std::pair<std::string , std::string >> & ) override;
        self_type & set_query_param(std::string key , std::string value) override;
        self_type & set_version(std::string http_version) override;

        // set headers
        self_type & set_header(std::string key , std::string value) override;
        self_type & set_header(std::vector<std::pair<std::string , std::string >> &) override;

        // set body
        self_type & set_body(basic_object_interface & body) override;

        http_request_builder_v1() ;

    private:
        class impl;
        impl * impl_;
};

#endif
