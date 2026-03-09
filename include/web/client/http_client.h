#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__

#include "connector.h"
#include "client.h"
#include "http_writer.h"
#if 0
/*
 * create and connect a tcp connection
 */
#include <sys/socket.h>
#include "response.h"
class tcp{
    static short socket_type ;
};
class tcp_resolver{};
class http_connector{};
class http_stream{};

/*
 * Only two possiblities could be a http client or a https client 
 */
template < typename resolver ,
           typename socket ,
           typename connector , // could be a http connector or a https connector 
           typename stream ,
           typename request ,
           typename response>
class ClientFactory{
    typedef resolver       resolver_type;
    typedef connector      connector_type;
    typedef stream         stream_type;
    typedef socket         socket_type;
    typedef request        request_type;
    typedef response       response_type;

    resolver_type  * resolver_;
    connector_type * connector_;
    stream_type    * stream_;
    socket_type    * socket_; // could be a tcp socket or a udp socket 
    public:
    /*
     * Initialize this client with the supplied templates
     */
        ClientFactory(const char * host , int port = 80){

            resolver_  = new resolver_type(host);
            socket_    = new socket_type();
            connector_ = new connector_type(socket_); 
            // only the write and read will be replaced with ssl_write and ssl_read
            stream_    = new stream_type(socket_);
        }
        response_type send(request_type & req){
            stream_->write(req.serialize());
            response_type resp(stream_->read());
            return resp;
        }

    protected:
        ClientFactory(){}
};
#endif

/*
 *Should take care of framing the http request
 */
class BaseClient{



};


#endif
