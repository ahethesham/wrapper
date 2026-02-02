#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <unistd.h>
#include "json.h"
#include "request.h"
#include "socket.h"
#include "response.h"
#include "address.h"
#include <openssl/ssl.h>

class Client {
    public:
        virtual Response & send(Request &req) = 0; 
};

namespace http{
    class client : public Client
    {
        Stream * reader_;
        Stream * writer_;
        Socket * socket_;
        public:
            // connect to this host on the given socket 
            client(const char * hostname , int port);
            Response & send(Request & req);
            int getFd(){return socket_->getFd();}
    };
}
namespace https{
    class client : public http::client{

        SSL_CTX *context_;
        SSL * ssl_;
        SSL_METHOD *method_;
        Stream *reader_;
        Stream *writer_;
        public:
            client(const char * hostname , int port) : http::client(hostname , port){
                // it will have to take care of making this a https connnection
                // add assert and logs wherever necessary
                SSL_load_error_strings();
                OpenSSL_add_ssl_algorithms();
                method_ = (SSL_METHOD *)TLS_client_method();
                context_ = SSL_CTX_new(method_);
                ssl_ = SSL_new(context_);
                SSL_set_fd(ssl_ , getFd());
                SSL_connect(ssl_);
            }
            ~client(){
                // close and cleanup all the ssl contexts 
            }
            Response & send(Request & req) override;
    };
}

Response & https::client::send(Request & req)
{
    reader_ = new https::stream();
    writer_ = new https::stream();
    // read all the headers and json body into the writer stream
    req >> *writer_;
    (*writer_) >> ((void *)ssl_);
    (*reader_) << ((void *)ssl_);
    // extract the headers and jsonbody from the response 
    http::headers *responseHeaders = new http::headers(*reader_);
    json * json = new ::json(*reader_);
    Response *resp = new http::response(*responseHeaders , *json);

    return *resp;

}
/*
 * There should only be one concrete implementation
 */
http::client::client(const char * hostname , int port)
{
    // no socket options neede for now 
    Socket * tcpSocket = new tcp::socket(0);
    Address * ipv4Address = new ipv4::address(hostname , port);
    // bind the socket to this address 
    tcpSocket->bind(*ipv4Address);
    // connect with the remote client ...for now lets have it blocking
    tcpSocket->connect();

    return ;
}

Response & http::client::send(Request & req){
    // send the request to the client and then read the response and create a response object  for it
    Stream &_stream = *(new lineStream());
    // should write the entire request into this stream 
    req >> (_stream);
    _stream >> (socket_->getFd()) ;
    Stream & responseStream = *(new lineStream());
    
    responseStream << (socket_->getFd());
    // have an iterator here and then parse the response line by line and seperate each header and body
    Stream::iterator itr ;
    headers *resHeaders = new headers();
    body *   resBody = new body();
    while(itr.hasNext()){
        // have logic to get the stream line by line and then seperate headers and body
    }
    return *new response(*resHeaders  , *resBody);
}

#endif
