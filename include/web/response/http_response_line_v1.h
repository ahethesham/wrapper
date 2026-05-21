#ifndef __HTTP_RESPONSE_LINE_V1_H__
#define __HTTP_RESPONSE_LINE_V1_H__

#include "basic_http_response_line_interface.h"

class http_response_line_v1 : public basic_http_response_line_interface{
    public:
        // constructors 
        http_response_line_v1(http_status  &status);
        http_response_line_v1() ;
        http_response_line_v1(const http_response_line_v1 &) ;
        http_response_line_v1( http_response_line_v1 &) ;
        http_response_line_v1(http_response_line_v1 &&);
        
        // get methods
        const http_status & status() const override;
        std::string & version() const override;
        
        // set method
        http_response_line_v1 & set_status(http_status & status) override;
        http_response_line_v1 & set_version(std::string version) override;

        // operator , clone
        http_response_line_v1 & operator=(http_response_line_v1 &) ;
        http_response_line_v1 & operator=(http_response_line_v1 &&) ;
        std::shared_ptr<basic_http_response_line_interface> clone() override;

        //parsers
        void parse(buffer_type * buffer) override;
        bool continue_reading() override;
        void at_eof(buffer_type * buffer) override;
        buffer_type * buffer() override;

        // serialize , deserialize
        std::string serialize() override;
        std::string serialize(basic_formatter_interface & formatter) override;

        http_response_line_v1 & clear() override;

    private:
        class impl;
        std::shared_ptr<impl> impl_;
};

#endif
