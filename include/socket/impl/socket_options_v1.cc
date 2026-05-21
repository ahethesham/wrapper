#include "socket_options.h"
#include "socket_io_handle.h"
#include <netinet/in.h>
#include <netinet/tcp.h>

class socket_options_v1::impl{
    struct option;
    public:
        using callback = std::function<int(option option)> ;

        impl(std::shared_ptr<socket_io_handle_interface> socket) 
            : impl(socket->get_lowest_level_fd()) {}

        impl(std::shared_ptr<ssl_socket_io_handle_interface> ssl_socket)
            : impl(ssl_socket->get_lowest_level_fd()){}

        impl()
            : fd_(-1) ,  cb_([this](option opt){
                    return process_queued(opt);
        }){}

        impl(int fd)
            : fd_(fd) , cb_([this](option opt){
                    return process_realtime(opt);
        }){}
        
        void reuse_address(bool val){
            int opt = val ? 1 : 0;
            cb_( option( SOL_SOCKET , SO_REUSEADDR , &opt , sizeof(opt)) );
            return ;
        }
        void reuse_port(bool val){
            int opt = val ? 1 : 0;
            cb_( option( SOL_SOCKET , SO_REUSEPORT , &opt , sizeof(opt)) );
            return ;
        }
        void set_receive_buffer_size(size_t size){
            cb_( option( SOL_SOCKET , SO_RCVBUF , &size , sizeof(size)) );
            return ;
        }
        void set_send_buffer_size(size_t size){
            cb_( option( SOL_SOCKET , SO_SNDBUF , &size , sizeof(size)) );
            return ;
        }
        void set_tcp_nodelay(bool val){
            int opt = val ? 1 : 0;
            cb_( option( IP_PROTOCOL , TCP_NODELAY , &opt , sizeof(opt)) );
            return ;
        }
        void build(int fd){
            fd_ = fd;
            for(auto x : vec_)
                process_realtime(x);
            return ;
        }
    private:
        int process_realtime(option & opt){
            assert(fd_ > 0);
            assert(::setsockopt(fd_ , opt.optname , opt.flagname , opt.flag , opt.flagsize ) == 0);
            return 0;
        }
        int process_queued(option & opt){
            vec_.push_back(opt);
            return 0;
        }


        int fd_;
        std::vector<option> vec_;
        struct option{
            int optname ;
            int flagname ;
            void * flag ;
            socklen_t flagsize;
            option(int oname , int fname , void *f, socklen_t size)
                : optname(oname) , flagname(fname) , flag(f) , flagsize(size) {}
        };
        callback cb_;
        
};

template<typename T>
socket_options_v1::socket_options_v1(std::shared_ptr<T> sock)
    : impl_(std::make_shared<impl>(sock)){}

socket_options_v1::socket_options_v1()
    : impl_(std::make_shared<impl>()){}

socket_options_v1::socket_options_v1(int fd)
    : impl_(std::make_shared<impl>(fd)){}

socket_options_v1&socket_options_v1::reuse_address(bool val ){
    impl_->reuse_address(val);
    return (*this);
}

socket_options_v1& socket_options_v1::reuse_port(bool val ){
    impl_->reuse_port(val);
    return (*this);
}

socket_options_v1& socket_options_v1::set_receive_buffer_size(size_t size){
    impl_->set_receive_buffer_size(size);
    return *this;
}

socket_options_v1& socket_options_v1::set_send_buffer_size(size_t size){
     impl_->set_send_buffer_size(size);
    return  *this;
}

socket_options_v1& socket_options_v1::set_tcp_nodelay(bool val){
    impl_->set_tcp_nodelay(val);
    return *this;
}

socket_options_v1& socket_options_v1::build(int fd){
    impl_->build(fd);
    return *this;
}



