#include "socket_io_handle.h"
#include "socket_options.h"
#include <cassert>
#include "file_logger_v1.h"

template< auto network_type ,
    auto communication_type ,
    auto protocol_type >
class ssl_socket_io_handle<network_type , communication_type , protocol_type>::impl{
    public:
        impl(){
            init_ctx();
            assert(detail::socket_wrapper(network_type , communication_type , protocol_type , [&](int fd){
                        return fd <= 0 ? fd : SSL_set_fd(ssl_fd_ , fd) == 1 ? 0 : 1;
            }) > 0);
        }
        impl(socket_options & opts)
            : ssl_fd_(nullptr) , ctx_(nullptr) , fd_(-1){
            init_ctx();
            assert(socket_wrapper(network_type , communication_type , protocol_type , [&](int fd){
                        if(fd <= 0) return fd;
                        opts.build(fd);
                        return SSL_set_fd(ssl_fd_ , fd) == 1 ? 0 : 1;
            }) > 0);
        }
        bool is_closed(){
            if(fd_ == -1)return true;
            return false;
        }

        fd_type open(){
            assert(0);
            return ssl_fd_;
        }

        void close(){
            SSL_shutdown(ssl_fd_);
            SSL_free(ssl_fd_);
            detail::close(fd_ , [this](int rc){
                    if(ctx_)
                        free( ctx_);
                    LOG_DEBUG << "socket closed gracefully" << endl;
                    return rc;
            });
            return ;
        }
        fd_type get(){
            return ssl_fd_;
        }
        int get_lowest_level_fd(){
            return fd_;
        }
        int get_flags(){
            return 0;
        }
        int set_flags(int f){
            return 0;
        }
    private:
        void init_ctx(){
            ctx_ = SSL_CTX_new(TLS_client_method());
            assert(ctx_ != nullptr);
            ssl_fd_ = SSL_new(ctx_);
        }
        fd_type  ssl_fd_;
        SSL_CTX * ctx_;
        int fd_;
};

template<auto network ,
    auto communication ,
    auto protocol>
ssl_socket_io_handle<network , communication , protocol>::ssl_socket_io_handle() : impl_(std::make_shared<impl>()){}

template<auto network ,
    auto communication ,
    auto protocol>
ssl_socket_io_handle<network , communication , protocol>::ssl_socket_io_handle(socket_options & opt) : impl_(std::make_shared<impl>(opt)){}


template<auto network ,
    auto communication ,
    auto protocol>
ssl_socket_io_handle<network , communication , protocol >::fd_type ssl_socket_io_handle<network , communication , protocol>::open(){
    return impl_->open();
}

template<auto network ,
    auto communication ,
    auto protocol>
int ssl_socket_io_handle<network , communication , protocol>::get_lowest_level_fd(){
    return impl_->get_lowest_level_fd();
}

template<auto network ,
    auto communication ,
    auto protocol>
int ssl_socket_io_handle<network , communication , protocol>::get_flags(){
    return impl_->get_flags();
}

template<auto network ,
    auto communication ,
    auto protocol>
int ssl_socket_io_handle<network , communication , protocol>::set_flags(int f){
    return impl_->set_flags(f);
}

template<auto network ,
    auto communication ,
    auto protocol>
ssl_socket_io_handle<network , communication , protocol> & ssl_socket_io_handle<network , communication , protocol>::close(){
     impl_->close();
     return *this;
}


template<auto network ,
    auto communication ,
    auto protocol>
bool ssl_socket_io_handle<network , communication , protocol>::is_closed(){
    return impl_->is_closed();
}
