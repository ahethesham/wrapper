#ifndef __HTTP_CONNECTION_V1_H__
#define __HTTP_CONNECTION_V1_H__

#include "basic_connection_interface.h"
#include "socket_options.h"
class http_connection_v1 : public basic_connection_interface{
    
    public:
        http_connection_v1() ;
        // should take care of opening a socket and connecting with the client on that socket 
        http_connection_v1(std::string &host , int port = 80);
        bool is_alive() override;
        http_connection_v1 & close() override;
        http_connection_v1 & send(basic_http_request_interface & req , callback && cb) override;
        basic_http_response_interface & send(basic_http_request_interface & req) override;
        http_connection_v1(http_connection_v1 &) = delete;
        http_connection_v1(http_connection_v1 &&) = delete;
        http_connection_v1 & operator=(http_connection_v1 &) = delete;
        http_connection_v1 & operator=(http_connection_v1 &&) = delete;

    private:
        class impl;
        std::shared_ptr<impl> impl_;
};

#endif
