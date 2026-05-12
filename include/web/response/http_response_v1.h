#ifndef __HTTP_RESPONSE_V1_H__
#define __HTTP_RESPONSE_V1_H__

#include "basic_http_response.h"

// concrete class
class http_response_v1 : public basic_http_response_interface{
    public:
        http_response_v1();
        http_response_v1(http_response_v1 &);
        http_response_v1(http_response_v1 &&);
        // will decide later
        http_response_v1(http_status & status);

        ~http_response_v1() = default;

        // set methods
        http_response_v1 & set_status(http_status &status) override;
        http_response_v1 & set_version(std::string version) override;
        http_response_v1 & set_header(std::string key , std::string value) override;
        http_response_v1 & set_body(basic_object_interface &body) override;


        // injections
        http_response_v1 & set_headers(basic_http_headers_interface & headers) override;
        http_response_v1 & set_response_line(basic_http_response_line_interface & response_line) override;

        // get method
        const http_status & status() override;
        std::string & version() override;
        const std::string & operator[](std::string key) override;
        std::shared_ptr<basic_http_headers_interface> headers() override;
        std::shared_ptr<basic_object_interface> body() override;
        std::shared_ptr<basic_http_response_line_interface> response_line() override;

        // copy , move , clone
        std::shared_ptr<basic_http_response_interface> clone() override;
        http_response_v1 & operator=(http_response_v1 & obj) ;
        http_response_v1 & operator=(http_response_v1 && obj) ;

        //parser methods
        void parse(buffer_type * buffer) override;
        void at_eof(buffer_type * buffer) override;
        buffer_type * buffer() override;
        bool continue_reading() override;

        //serialize , deserialize methods
        std::string serialize() override;
        std::string serialize(basic_formatter_interface & formatter) override;
        
        // reset and clean
        http_response_v1 & clear() override;
    private:
        class impl;
        std::shared_ptr<impl> impl_;
};
#endif
