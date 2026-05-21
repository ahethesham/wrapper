#include "binder_listener_v1.h"
#include <cassert>
#include <netdb.h>

class bind_and_listen_v1::impl{
    
    public:
        impl() : handle_(nullptr) , endpoint_(nullptr) , address_(nullptr){}

        impl(std::shared_ptr<socket_io_handle_interface> handle , std::shared_ptr<basic_endpoint_interface > endpoint) : handle_(handle) , endpoint_(endpoint) , address_(nullptr) { }

        int bind(){
            auto itr = endpoint_->begin();
            while(itr->has_next()){
                auto addr = itr->get_next();
                if(::bind(handle_->get() , addr->ai_addr , addr->ai_addrlen) == 0){
                    // bind successfull
                    address_ = addr;
                    return 0;
                }
            }
            return -1;
        }
        int bind(std::shared_ptr<socket_io_handle_interface> handle , std::shared_ptr<basic_endpoint_interface > endpoint){
            assert(handle_ == nullptr && endpoint_ == nullptr);
            handle_ = handle;
            endpoint_ = endpoint;
            return bind();
        }
        const std::string & ip_address(){
            assert(address_ != nullptr);
            return endpoint_->ip_address(address_);
        }
        const int protocol(){
            return address_->ai_protocol;
        }
        const int stream(){
            return address_->ai_socktype;
        }
        int listen(int backlog){
            assert(::listen(handle_->get() , backlog) == 0);
            return 0;
        }

    private:
        std::shared_ptr<socket_io_handle_interface> handle_;
        std::shared_ptr<basic_endpoint_interface> endpoint_;
        struct addrinfo * address_;
};

bind_and_listen_v1::bind_and_listen_v1(std::shared_ptr<socket_io_handle_interface> handle , std::shared_ptr<basic_endpoint_interface> endpoint) : impl_(std::make_shared<impl>(handle , endpoint)){}

int bind_and_listen_v1::bind(){
    return impl_->bind();
}
int bind_and_listen_v1::bind(std::shared_ptr<socket_io_handle_interface> handle , std::shared_ptr<basic_endpoint_interface> endpoint){
    return impl_->bind(handle , endpoint);
}

const std::string & bind_and_listen_v1::ip_address(){
    return impl_->ip_address();
}
const int bind_and_listen_v1::protocol(){
    return impl_->protocol();
}
const int bind_and_listen_v1::stream(){
    return impl_->stream();
}
int bind_and_listen_v1::listen(int backlog){
    return impl_->listen(backlog);
}
