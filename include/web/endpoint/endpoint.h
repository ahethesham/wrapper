#ifndef __ENDPOINT_H__
#define __ENDPOINT_H__

#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include "logger.h"

template <typename endpoint  >
class basic_endpoint {
    public:
        typedef typename endpoint::ip_type          ip_type;
        typedef typename endpoint::port_type        port_type;
        typedef typename endpoint::sockaddr_type    sockaddr_type;
    private:
        ip_type ip_;
        port_type port_;
        sockaddr_type * remoteHostAddress_;
        std::string host_;
        struct addrinfo *address_;
    public:
        basic_endpoint(port_type port , std::string host) : host_(std::move(host)) , port_(port) {
            log("calling resolver ");
            resolve();
        }
        basic_endpoint(sockaddr_type * clientAddress) : remoteHostAddress_(clientAddress) {}

        basic_endpoint(){}

        bool resolve(){
            struct addrinfo hint;
            memset(&hint , 0x00 , sizeof(hint));
            hint.ai_family = endpoint::IP_TYPE ; 
            hint.ai_socktype = endpoint::SOCK_TYPE;
            hint.ai_flags = endpoint::AI_FLAGS;

            if(::getaddrinfo(host_.c_str() , std::to_string(port_).c_str() , &hint , &address_) != 0){
                throw std::runtime_error("unknown host ");
            }
            log("resolver  has completed successfully ");
            return true;
        }
        struct addrinfo * getinfo(){
            return address_;
        }
        class basic_iterator{
                struct addrinfo * root_;
                struct addrinfo * current_;
            public:
                basic_iterator(struct addrinfo * address) : root_(address) , current_(address){}
                basic_iterator & operator++(){
                    current_ = (struct addrinfo *)current_->ai_next;
                    return *this;
                }
                struct addrinfo * operator->(){
                    return current_;
                }
                struct addrinfo * getNext(){
                    auto temp = current_;
                    ++(*this);
                    return temp;
                }
                bool hasNext(){
                    return current_ != nullptr;
                }
                virtual ~basic_iterator(){
                    // call a class in parent function to free our memory 
                }
        };
        basic_iterator & iterator(){
            return * new basic_iterator(address_); 
        }
};

/*
 * resolve the host name and get the ip address 
 */
struct endpoint{
        typedef int         port_type;
        typedef std::string ip_type;
        typedef sockaddr_in sockaddr_type;

        static constexpr uint8_t IP_TYPE      = AF_INET;
        static constexpr uint8_t SOCK_TYPE    = SOCK_STREAM  ;
        static constexpr uint8_t AI_FLAGS     = 0;
};

typedef basic_endpoint<endpoint> tcp_endpoint;
#endif
