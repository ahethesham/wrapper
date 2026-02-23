#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__

#include <functional>
#include "web_fwd.h"

/*
 * This will be the base  server 
 */
class HttpServer{
    using handler = std::function<void(http::http_request & req , http::http_response & res )> ;
    // have a function handler for each uri
    std::map<std::string , handler> routers_;

    public:
        void process( http::http_request &req , http::http_response & res){
            auto uri = req.requestLine().getUri();
            auto itr = routers_.find(uri);

            if(itr == routers_.end()){
                // generate a response showing a 400 status bad request
                return ;
            }
            auto func = itr->second;
            func(req , res);
                return ;
        }
        void GET(const char * uri , handler func){

        }
        void POST(const char *uri, handler func){

        }
        void PUT(const char * uri, handler func){

        }
        void DELETE(const char *uri, handler func){
            
        }
};


#endif
