#ifndef __HTTP_REQUEST_V1_H__
#define __HTTP_REQUEST_V1_H__

#include "basic_http_request_interface.h"
#include "basic_parser_interface.h"

class http_request_v1 : public basic_http_request_interface  {
    public:
        explicit http_request_v1();
        explicit http_request_v1(basic_http_request_line_interface &,
                basic_http_headers_interface & , basic_object_interface &);
        explicit http_request_v1(basic_http_request_line_interface & ,
                basic_http_headers_interface &);
        explicit http_request_v1(basic_http_request_line_interface &);

        using base_type =  basic_http_request_interface;      
        using self_type = http_request_v1;
        using buffer_type = buffer_v1;

        self_type & set_headers(basic_http_headers_interface & headers) override;
        self_type & set_request_line(basic_http_request_line_interface & line) override;
        self_type & set_body(basic_object_interface  & body) override;
        basic_http_request_interface & set_header(std::string key , std::string value) override;
        basic_http_request_interface & set_uri(std::string uri) override;
        basic_http_request_interface & set_query_param(std::string key , std::string value) override;
        basic_http_request_interface & set_method(std::string method) override;

        /** Get methods **/
         std::string & get_query_param(std::string key) override;
         basic_http_headers_interface * headers() override;
         basic_http_request_line_interface * request_line() override; 
         // could be a json body , html , xml or any other protocol
         basic_object_interface * body() override;
         std::string & method() override;
         std::string & uri() override;
         std::string & version() override;
         std::string & param(std::string key) override;
        // asssume this mf is referring to get the header value
         const std::string & operator[](std::string key) override;
         const std::string & get_header(std::string key) override;

        //********************** Parser methods while reading requests ***************************************/
        bool continue_reading() override;
        void at_eof(buffer_type * buffer) override;
        void parse(buffer_type * buffer) override;
        buffer_type * buffer() override;

        // move and copy constructors
        http_request_v1(http_request_v1 & req);
        http_request_v1(http_request_v1 && req);
        http_request_v1 & operator=(http_request_v1 &);
        http_request_v1 & operator=(http_request_v1 &&);
        std::shared_ptr<basic_http_request_interface> clone() override;

        // serialize to a string
        std::string serialize(basic_formatter_interface & formatter ) override;

    private:
        class impl;
        impl * impl_;
};

#endif
