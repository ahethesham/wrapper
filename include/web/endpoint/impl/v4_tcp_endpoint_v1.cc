#include "v4_tcp_endpoint_v1.h"
#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <sys/socket.h>

class v4_tcp_endpoint_v1::impl{
    public:
        impl(const char * host , int port )
            :port_(port) , ai_flags_(0) {
                memset(host_ , 0x00 , sizeof(host_));
                if(host)
                    strcpy((char *)host_ , host);
        }
        bool resolve(){
            struct addrinfo hint;
            ::memset(&hint , 0x00 , sizeof(hint));
            hint.ai_family = AF_INET; 
            hint.ai_socktype = SOCK_STREAM;
            hint.ai_flags = 0;

            if(::getaddrinfo(host_ , std::to_string(port_).c_str() , &hint , &result_) != 0){
                throw std::runtime_error("unknown host ");
            }
            return true;
        }
        struct addrinfo * begin(){
            return result_;
        }
        struct addrinfo * end(){
            return nullptr;
        }
        struct addrinfo * next(struct addrinfo * addr){
            return addr->ai_next;
        }
        int get_ai_flags(){
            return ai_flags_;
        }
        int set_ai_flags(int flags){
            return ai_flags_ = flags;
        }
        const std::string & ip_address(struct addrinfo * addr){
            if(ip_address_.size() == 0)
            {
                char ip_str[246] = {0};
                auto ip = (struct sockaddr_in *)addr->ai_addr;
                struct in_addr *ipaddress = &(ip->sin_addr);
                inet_ntop(addr->ai_family , ipaddress , ip_str , sizeof(ip_str));
                ip_address_.append(ip_str) ;
            }
            return ip_address_;
        }
        std::string hostname(){
            return "";
        }
    private:
        struct addrinfo * result_;
        char host_[256];
        int port_;
        int ai_flags_;
        std::string ip_address_;
};

// iterator 
v4_tcp_endpoint_v1::iterator::iterator(struct addrinfo * list) : current_(list){}
v4_tcp_endpoint_v1::iterator::iterator(iterator & obj) : current_(obj.current_){}
v4_tcp_endpoint_v1::iterator::iterator(iterator && obj) : current_(obj.current_){
    obj.current_ = nullptr;
}
v4_tcp_endpoint_v1::iterator & v4_tcp_endpoint_v1::iterator::operator++(){
    current_ = current_->ai_next;
    return *this;
}
struct addrinfo * v4_tcp_endpoint_v1::iterator::operator* (){
    return get();
}
struct addrinfo * v4_tcp_endpoint_v1::iterator::get (){
    return current_;
}
bool v4_tcp_endpoint_v1::iterator::operator==(iterator_interface & itr){
    return get() == itr.get();
}
bool v4_tcp_endpoint_v1::iterator::has_next(){
    return current_ != nullptr;
}
struct addrinfo * v4_tcp_endpoint_v1::iterator::get_next(){
    auto temp = get();
    ++(*this);
    return temp;
}

v4_tcp_endpoint_v1::v4_tcp_endpoint_v1(const char * host , int port)
    : impl_(std::make_shared<impl>(host , port)){
        resolve();
}
bool v4_tcp_endpoint_v1::resolve(){
    return   impl_->resolve();
}
struct  addrinfo * v4_tcp_endpoint_v1::get_info(){
    return impl_->begin();
}
v4_tcp_endpoint_v1::iterator & v4_tcp_endpoint_v1::begin(){
    return *new iterator(impl_->begin());
}
v4_tcp_endpoint_v1::iterator & v4_tcp_endpoint_v1::end(){
    return * new iterator(impl_->end());
}
int v4_tcp_endpoint_v1::get_ai_flags(){
    return impl_->get_ai_flags();
}
int v4_tcp_endpoint_v1::set_ai_flags(int f){
    return impl_->set_ai_flags(f);
}
const std::string & v4_tcp_endpoint_v1::ip_address(struct addrinfo * addr){
    return impl_->ip_address(addr);
}
std::string v4_tcp_endpoint_v1::hostname(){
    return impl_->hostname();
}
