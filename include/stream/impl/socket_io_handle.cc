#include "socket_io_handle.h"
#include <cassert>
#include <cerrno>
#include <openssl/ssl.h>
#include <unistd.h>


template< auto network_type ,
    auto communication_type ,
    auto protocol_type >
class socket_io_handle<network_type , communication_type , protocol_type>::impl{
    public:
        impl(){
            open();
        }
        impl(fd_type fd ){
            fd_ = fd;
        }

        fd_type open(){
            assert(fd_ = socket_wrapper(network_type , communication_type , protocol_type , [](int rc){return rc;}) > 0);
            return fd_;
        }
        bool is_closed(){
            if(fd_ < 0)return true;

            char ch;
            int ret = -1;
            do{
                int ret = ::recv(fd_  , &ch , sizeof(char) , MSG_PEEK);
            }while(ret < 0 && errno == EINTR);
            
            if(ret <= 0)fd_ = -1;

            return ret <= 0;
        }
        fd_type get(){
            return fd_;
        }
        int get_lowest_level_fd(){
            return fd_;
        }
        void close(){
            assert(fd_ > 0);
            ::close(fd_);
            return ;
        }
        int get_flags(){
            return flags_ ;
        }
        int set_flags(int f){
            flags_ = f;
            return f;
        }
    private:
        fd_type fd_;
        int flags_;

};

template< auto network_type  ,
          auto communication_type  ,
          auto protocol_type >
socket_io_handle<network_type , communication_type , protocol_type>::socket_io_handle() :
impl_(std::make_shared<impl>()){ }

template< auto network_type  ,
          auto communication_type  ,
          auto protocol_type >
socket_io_handle<network_type , communication_type , protocol_type>::socket_io_handle(fd_type fd) :
impl_(std::make_shared<impl>(fd)){ }


template< auto network_type  ,
          auto communication_type  ,
          auto protocol_type >
socket_io_handle<network_type , communication_type , protocol_type>::fd_type socket_io_handle<network_type , communication_type , protocol_type>::open(){
    impl_->open();
}


template< auto network_type  ,
          auto communication_type  ,
          auto protocol_type >
bool socket_io_handle<network_type , communication_type , protocol_type>::is_closed(){
    return impl_->is_closed();
}

template< auto network_type  ,
          auto communication_type  ,
          auto protocol_type >
socket_io_handle<network_type , communication_type , protocol_type > & socket_io_handle<network_type , communication_type , protocol_type>::close(){
    impl_->close();
    return *this;
}

template< auto network_type  ,
          auto communication_type  ,
          auto protocol_type >
socket_io_handle<network_type , communication_type , protocol_type >::fd_type socket_io_handle<network_type , communication_type , protocol_type>::get(){
    return impl_->get();
}

template< auto network_type  ,
          auto communication_type  ,
          auto protocol_type >
socket_io_handle<network_type , communication_type , protocol_type >::fd_type socket_io_handle<network_type , communication_type , protocol_type>::get_lowest_level_fd(){
    return impl_->get_lowest_level_fd();
}

template< auto network_type  ,
          auto communication_type  ,
          auto protocol_type >
socket_io_handle<network_type , communication_type , protocol_type >::fd_type socket_io_handle<network_type , communication_type , protocol_type>::get_flags(){
    return impl_->get_flags();
}

template< auto network_type  ,
          auto communication_type  ,
          auto protocol_type >
socket_io_handle<network_type , communication_type , protocol_type >::fd_type socket_io_handle<network_type , communication_type , protocol_type>::set_flags(int f){
    return impl_->set_flags(f);
}


