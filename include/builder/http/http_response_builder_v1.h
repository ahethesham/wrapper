#ifndef __HTTP_RESPONSE_BUILDER_V1_H__
#define __HTTP_RESPONSE_BUILDER_V1_H__

#include "basic_response_builder.h"
#include "http_response_v1.h"

template<typename T>
class http_response_builder_v1 : public basic_response_builder_interface{

    public:
        using base_type = basic_response_builder_interface;
        using self_type = http_response_builder_v1;

        http_response_builder_v1(basic_http_response_interface * obj);
        http_response_builder_v1(std::shared_ptr<basic_http_response_interface > obj);
        http_response_builder_v1(basic_http_response_interface & obj);
        http_response_builder_v1(http_response_builder_v1 &obj);
        http_response_builder_v1()  ;

        // set methods
        self_type & set_status(http_status & status) override;
        self_type & set_header(std::string key , std::string value) override;
        self_type & set_header(std::vector<std::pair<std::string , std::string >> & headers) override;
        self_type & set_body(basic_object_interface & obj) override;
        self_type & set_body(std::shared_ptr<basic_object_interface> obj) override;
    
        // create method
        self_type & create(basic_http_response_interface & obj) override;
        self_type & create(std::shared_ptr<basic_http_response_interface>  obj) override;

        //get method
        const http_status & status() override;
        const std::string & header(std::string key) override;
        std::string & version() override;
        const std::string & operator[](std::string key) override;
        basic_object_interface & body() override;
        // clone      
        std::shared_ptr<basic_response_builder_interface> clone() override;

        // build 
        std::shared_ptr<basic_http_response_interface>  build() override;
        std::shared_ptr<basic_http_response_interface > build(std::shared_ptr<basic_reader_interface<socket_io_handle_interface>> reader) override;
        std::shared_ptr<basic_http_response_interface > build(std::shared_ptr<basic_reader_interface<ssl_socket_io_handle_interface>> reader) override;

    private:
        class impl;
        std::shared_ptr<impl> impl_;

};

template class http_response_builder_v1<http_response_v1>;

#endif
