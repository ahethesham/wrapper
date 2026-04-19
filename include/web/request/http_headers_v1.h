#ifndef __HTTP_HEADERS_V1_H__
#define __HTTP_HEADERS_V1_H__

#include "basic_http_headers_interface.h"
#include "basic_parser_interface.h"
#include "buffer.h"
class http_headers_v1 : public basic_http_headers_interface ,
                        public basic_parser_interface<buffer_v1>{

    public:
        using self_type = http_headers_v1;
        using base_type = basic_http_headers_interface;

        const std::string & operator[](std::string key) override;
        const std::string & get(std::string key) override;
        self_type & set(std::string key , std::string value) override;


        self_type & operator=(self_type & obj) ;
        self_type & operator=(self_type && obj) ;

        http_headers_v1(http_headers_v1 &);
        http_headers_v1(http_headers_v1 &&);
        http_headers_v1();
    // parsing interface
        
        bool continue_reading() override;
        void at_eof(buffer_v1 * buffer) override;
        buffer_v1 * buffer() override;
        void parse(buffer_v1 * buffer) override;

        std::string serialize(basic_formatter_interface &  formatter) override;
    private:
        class impl;
        impl * impl_;
};


#endif
