#include "io_handles_fwd.h"
#include <cassert>
#include <cerrno>
#include <openssl/ssl.h>
#include <unistd.h>

template< auto network_type  ,
          auto communication_type  ,
          auto protocol_type >
class socket_io_handle : public socket_io_handle_interface{
    using socket_io_handle_interface::fd_type;
    using socket_io_handle_interface::endpoint_reference_type;
    using sockaddr_type = endpoint_reference_type;

    public:
        socket_io_handle() {
            open();
        };

        fd_type open() override{
            fd_ = ::socket(network_type , communication_type , protocol_type);
            assert(fd_ > 0);
            return fd_;
        }
        
        bool is_closed() override {
            if(fd_ < 0)return true;

            char ch;
            int ret = -1;
            do{
                int ret = ::recv(fd_  , &ch , sizeof(char) , MSG_PEEK);
            }while(ret < 0 && errno == EINTR);
            
            if(ret <= 0)fd_ = -1;

            return ret <= 0;
        }

        socket_io_handle & close() override{
            if(fd_  == -1)return *this;
            ::close(fd_);
            return *this;
        }

        fd_type get() override {
            return fd_;
        }


    private:
        endpoint_reference_type path_;
        fd_type fd_;
};



template<auto network_type ,
        auto communication_type ,
        auto protocol_type >
class ssl_socket_io_handle  : public ssl_socket_io_handle_interface{
    
    public:
        typedef socket_io_handle_interface base_type;
        using sockaddr_type = endpoint_reference_type;

    private:

        base_type * base_socket_;
    
        void createContext(){
            ctx_ = SSL_CTX_new(TLS_client_method());
            assert(ctx_ != nullptr);
        }
    public:
        typedef SSL * fd_type;
        /*
         * First creates a base socket and then makes a SSL * out of it 
         */
        ssl_socket_io_handle() : base_socket_(new tcp_socket() ){
            assert(base_socket_ != nullptr);
            createContext();
            ssl_fd_ = SSL_new(ctx_);
            assert( ssl_fd_ != nullptr);
            assert(SSL_set_fd(ssl_fd_ , base_socket_->get()) == 1);
        }
        ssl_socket_io_handle(base_type * tcp_socket) : base_socket_(tcp_socket){
            assert(base_socket_ != nullptr);
            createContext();
            ssl_fd_ = SSL_new(ctx_);
            assert( ssl_fd_ != nullptr);
            assert(SSL_set_fd(ssl_fd_ , base_socket_->get()) == 1);
        }


        ssl_socket_io_handle(base_type::fd_type fd ) = delete;

        fd_type get() override{
            return ssl_fd_;
        }

        ssl_socket_io_handle_interface & close() override {
            SSL_shutdown(ssl_fd_);
            SSL_free(ssl_fd_);
            base_socket_->close();
            return *this ;
        }
        ~ssl_socket_io_handle(){
            close();
            if(ctx_)
                free(ctx_);
        }

        base_type & getBaseTcpSocket(){
            return *base_socket_;
        }
    private:
        SSL_CTX * ctx_;
        fd_type ssl_fd_;

};
