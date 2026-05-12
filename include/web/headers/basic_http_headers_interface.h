#ifndef __BASIC_HTTP_HEADERS_INTERFACE__
#define __BASIC_HTTP_HEADERS_INTERFACE__

#include "basic_formatter_interface.h"
#include "basic_parser_interface.h"
#include "buffer.h"
#include <memory>
#include <string>
class basic_http_headers_interface : public basic_parser_interface<buffer_v1>{

    public:
        const virtual std::string & get(std::string key) = 0;
        virtual basic_http_headers_interface & set(std::string key , std::string value) = 0;
        virtual basic_http_headers_interface & clear() = 0;
        virtual std::string serialize() = 0;
        virtual std::string serialize(basic_formatter_interface & formatter) = 0;

        virtual std::shared_ptr<basic_http_headers_interface> clone() = 0;
        virtual const std::string & operator[](std::string key) = 0;
};
#endif
