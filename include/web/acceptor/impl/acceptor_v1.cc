#include "acceptor_v1.h"
#include <cassert>

inline int acceptor::accept(std::shared_ptr<acceptor::server_handle_type> handle){
    struct sockaddr address;
    socklen_t len = sizeof(address);
    return ::accept(handle->get() , &address , &len);
}
inline int ssl_acceptor::accept(std::shared_ptr<ssl_acceptor::server_handle_type> handle){
    return SSL_accept(handle->get());
}

template<typename T>
class acceptor_v1<T>::impl{
    public:
        impl(int fd)
            : total_req_count_(0) , handle_(std::make_shared<server_handle_type>(fd)){}

        impl(std::shared_ptr<server_handle_type> handle)
            : total_req_count_(0) , handle_(handle){}

        int close_connection(std::shared_ptr<client_handle_type> handle){
            ::close(handle.get()) ;
            total_req_count_-- ;
        }
        std::shared_ptr<client_handle_type> accept(){
            int fd =  T::accept();
            assert(fd > 0);
            total_req_count_++;
            return std::make_shared<client_handle_type>(fd);
        }
        int current_req_count(){
            return total_req_count_;
        }

    private:
        int total_req_count_;
        std::shared_ptr<server_handle_type> handle_;
};


template<typename T>
acceptor_v1<T>::acceptor_v1(std::shared_ptr<server_handle_type> handle)
    : impl_(std::make_shared<impl>(handle) ){}

template<typename T>
acceptor_v1<T>::acceptor_v1(int fd)
    : impl_(std::make_shared<impl>(fd)) {}

template<typename T>
std::shared_ptr<typename acceptor_v1<T>::client_handle_type>  acceptor_v1<T>::accept(){
    return impl_->accept();
}

template<typename T>
int acceptor_v1<T>::close_connection(std::shared_ptr<client_handle_type>  handle){
    impl_->close(handle);
}

template<typename T>
int acceptor_v1<T>::current_req_count(){
    return impl_->current_req_count();
}


