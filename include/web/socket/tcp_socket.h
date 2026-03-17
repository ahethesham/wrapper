#ifndef __TCP_SOCKET_H__
#define __TCP_SOCKET_H__

#include <sys/socket.h>
#include "web_fwd.h"
#include "openssl/ssl.h"

struct ipv4_network_domain{
    static constexpr short value = AF_INET;
    typedef struct sockaddr_in sockaddr_type;
};

struct tcp_communication_type{
    static constexpr short value = SOCK_STREAM;
};

struct default_protocol_type{
    static constexpr short value = 0;
};


template<typename network_type ,
        typename communication_type ,
        typename protocol_type >
class tcp_socket_creator{
    public:
        typedef typename network_type::sockaddr_type sockaddr_type;
        static constexpr short network  = network_type::value;
        typedef  int fd_type;
    private:
        fd_type fd_;
    public:
        tcp_socket_creator() {
            fd_ = ::socket(network_type::value , communication_type::value , protocol_type::value);
            assert(fd_ > 0);
        }
        tcp_socket_creator(fd_type fd) : fd_(fd){
        }


        fd_type get(){
            return fd_;
        }

        void close(){
            ::close(fd_);
            return ;
        }
};

template<typename network_type ,
        typename communication_type ,
        typename protocol_type >
class tcp_ssl_socket_creator  : tcp_socket_creator<network_type , communication_type , protocol_type>{
    
    public:
        typedef tcp_socket_creator<network_type , communication_type , protocol_type> base_socket_type;
        using typename base_socket_type::sockaddr_type;
        using base_socket_type::network;

    private:

        base_socket_type * base_socket_;
    
        void createContext(){
            ctx_ = SSL_CTX_new(TLS_client_method());
            assert(ctx_ != nullptr);
        }
    public:
        typedef SSL * fd_type;
        /*
         * First creates a base socket and then makes a SSL * out of it 
         */
        tcp_ssl_socket_creator() : base_socket_(new base_socket_type() ){
            assert(base_socket_ != nullptr);
            createContext();
            ssl_fd_ = SSL_new(ctx_);
            assert( ssl_fd_ != nullptr);
            assert(SSL_set_fd(ssl_fd_ , base_socket_->get()) == 1);
        }

        tcp_ssl_socket_creator(base_socket_type::fd_type fd ):
        base_socket_(new base_socket_type(fd)){
            
            assert(base_socket_ != nullptr);
            createContext();
            ssl_fd_ = SSL_new(ctx_);
            assert( ssl_fd_ != nullptr);
            assert(SSL_set_fd(ssl_fd_ , base_socket_->get()) == 1);
        }
        fd_type get(){
            return ssl_fd_;
        }
        void close(){
            SSL_shutdown(ssl_fd_);
            SSL_free(ssl_fd_);
            base_socket_->close();
            return ;
        }
        ~tcp_ssl_socket_creator(){
            close();
            if(ctx_)
                free(ctx_);
        }

        base_socket_type & getBaseTcpSocket(){
            return *base_socket_;
        }
    private:
        SSL_CTX * ctx_;
        fd_type ssl_fd_;

};

#endif
