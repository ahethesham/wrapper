#ifndef __HTTP_REQUEST_LINE_V1_H__
#define __HTTP_REQUEST_LINE_V1_H__
#include "basic_http_request_line_interface.h"
#include "basic_parser_interface.h"
#include "buffer.h"
#include <vector>

class http_request_line_v1 : public basic_http_request_line_interface ,
                            public basic_parser_interface<buffer_v1>{
    public:
        http_request_line_v1() ;

        using self_type = http_request_line_v1;
        using base_type = basic_http_request_line_interface;

        self_type & set_method(std::string method) override;
        self_type & set_uri(std::string uri) override;
        self_type & set_version(std::string version) override;
        self_type & set_query_param(std::string key , std::string value) override;
        self_type & set_query_param(std::vector<std::pair<std::string , std::string > > & params);
        const std::string & get_method() override;
        const std::string  & get_uri() override;
        const std::string & get_version() override;

        const std::string &get_query_param(std::string key) override;

        //copy and move constructors
        http_request_line_v1(http_request_line_v1 &req);
        http_request_line_v1(http_request_line_v1 &&req);
        //copy and move assignment 
        self_type & operator=(http_request_line_v1 &req);
        self_type & operator=(http_request_line_v1 &&req);

        // parser interface implementation
        void at_eof(buffer_v1 * buffer) override;
        bool continue_reading() override;
        void parse(buffer_v1 * buffer) override;
        buffer_v1 * buffer() override;

        std::string serialize(basic_formatter_interface & formatter) override;


    private:
        class impl;
        impl  * impl_;
};

#endif
