#ifndef __CONNECTOR_H__
#define __CONNECTOR_H__

#include "socket.h"
#include <cassert>
#include <sys/socket.h>
#include <stdexcept>
#include <__type_traits/is_same.h>
#ifdef __SSL
#include "openssl/ssl.h"
#endif
#include "logger.h"


template < typename connectorPolicy >
class basic_connector{
    public:
        typedef connectorPolicy::sockaddr_type sockaddr_type;
        using socket_type = connectorPolicy::socket_type;
        typedef connectorPolicy::endpoint_type   endpoint_type;
    private:
        socket_type socket_;
        endpoint_type endpoint_;
        connectorPolicy * pConnector_;
    public:
        basic_connector(socket_type & _socket , endpoint_type & _endpoint) {
            // get an iterator to the endpoint and iterate through all the 
            // interfaces untill atleasr one of them connects successfully
            pConnector_ = new connectorPolicy();
            auto itr = _endpoint.iterator();
            while(itr.hasNext()){
                auto current = itr.getNext();
                if(current->ai_family == socket_type::network){
                    if(pConnector_->connect(_socket , current) == 0){
                        log("connection has been successfully");
                        return ;
                    }
                }
            }
            assert(false);
        }

        virtual ~basic_connector(){

        }
};


template< typename socketPolicy ,
          typename endpointPolicy>
class tcp_connector{
    // make a tcp connection with the client
    public:
        using socket_type = socketPolicy;
        using endpoint_type = endpointPolicy;
        typedef typename socket_type::sockaddr_type sockaddr_type;
        typedef typename endpoint_type::sockaddr_type endpoint_sockaddr_type;
    private:
        sockaddr_type client_;
        bool isConnectionAlive_;
    public:
        tcp_connector(){
            assert((std::is_same< sockaddr_type , endpoint_sockaddr_type>::value));
        }
        int connect(socket_type & _socket , struct addrinfo * _address){
            log("Doing a non ssl connect ");
            return ::connect(_socket.get() , _address->ai_addr , _address->ai_addrlen);
        }
};

template < typename socketPolicy ,
          typename endpointPolicy>
class  tcp_ssl_connector  : public tcp_connector<typename socketPolicy::base_socket_type , endpointPolicy>{
    // do a SSL connection
    //
    public:
        using socket_type = socketPolicy;
        using endpoint_type = endpointPolicy;
        using base_connector = tcp_connector<typename socket_type::base_socket_type , endpoint_type>;
        using typename base_connector::sockaddr_type;

        tcp_ssl_connector() : tcp_connector<typename socket_type::base_socket_type , endpoint_type>(){}
        int connect(socket_type &socket , struct addrinfo * address){
            if(tcp_connector<typename socket_type::base_socket_type , endpoint_type>::connect(socket.getBaseTcpSocket() , address) == 0){
                log("doing a ssl connect");
                assert(SSL_connect(socket.get()) == 1);
                return 0;
            }
            return -1;
        }
        
};
#endif
