#ifndef __SOCKET_OPTIONS_H__
#define __SOCKET_OPTIONS_H__

#include <memory>
class socket_options_v1 final{
    
    public:
        socket_options_v1& reuse_address(bool val) ;
        socket_options_v1& reuse_port(bool val) ;
        socket_options_v1& set_receive_buffer_size(size_t size);
        socket_options_v1& set_send_buffer_size(size_t size);
        socket_options_v1& set_tcp_nodelay(bool val);

        template<typename T>
        socket_options_v1(std::shared_ptr<T> sock );
        socket_options_v1();
        socket_options_v1(int fd);
        
        socket_options_v1& build(int fd);
    private:
        class impl;
        std::shared_ptr<impl> impl_;
};

using socket_options = socket_options_v1;
#endif
