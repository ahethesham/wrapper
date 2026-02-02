
#ifndef __RESPONSE_H__
#define __RESPONSE_H__
#include <unistd.h>
#include "request.h"

class Response
{
    public:
        virtual Response & createResponse(const char * payload) = 0;
};

namespace http
{
    class response : public Response
    {
        headers  responseHeaders_;
        json responseBody_;
        public:
            response(headers & resHeaders , json & resBody) : responseHeaders_(resHeaders) , responseBody_(resBody){}
            response();
            Response & createResponse(const char * payload) override;
    };
}
#endif
