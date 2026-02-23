#ifndef __CONNECTOR_H__
#define __CONNECTOR_H__

#include <cassert>
#include <sys/socket.h>
template < typename socket ,
            typename endpoint>
class basic_connector{
    public:
        typedef socket::sockaddr_type sockaddr_type;
        typedef socket     socket_type;
        typedef endpoint   endpoint_type;
    private:
        socket socket_;
        endpoint endpoint_;
    public:
        basic_connector(socket_type & _socket , endpoint_type & _endpoint){
            // get an iterator to the endpoint and iterate through all the 
            // interfaces untill atleasr one of them connects successfully
            auto itr = _endpoint.iterator();
            while(itr.hasNext()){
                if(itr->ai_family == socket_type::network_domain_type){
                    if(::connect(_socket.get() , itr->ai_addr , itr->ai_addrlen) == 0)
                        return ;
                }
            }
            assert(false);
        }

        virtual ~basic_connector(){
            close(socket_.get());
        }
};

#endif
