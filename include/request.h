#ifndef __REQUEST_H__
#define __REQUEST_H__

#include <unistd.h>
#include <vector>
#include <sys/types.h>
#include "stream.h"
#include <map>
#include "json.h"

class Request
{
    public:
       virtual Request & createRequest() = 0;
       virtual Request & operator>>(Stream & stream) = 0;

};

namespace http
{
    class headers;

    class headers{
            struct pair{
                std::string & key_;
                std::string & value_;
                pair & operator=(const char * value ){ value_ = std::string(value); return *this;}
            };
            std::vector<pair> pairs_;
        public:
            headers();
            headers(Stream &str);
            headers & add(const char * key , const char * value);
            pair & operator[](const char *key);
            headers & operator=(const char *value);
    };

    class request : Request
    {
        headers & headers_;
        json & body_;

        public:
        request(headers & requestHeaders , json & requestBody) : headers_(requestHeaders) , body_(requestBody){}
        Request & createRequest() override {return *this;}
        // serialize the request and put it into this fookin buffer 
        Request & operator>>(Stream & buffer ) override{
            return *this;
        };
    };
}

#endif
