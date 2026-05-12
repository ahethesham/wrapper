#ifndef __BINDER_LISTENER_V1_H__
#define __BINDER_LISTENER_V1_H__

#include "basic_binder_interface.h"
#include <memory>
class bind_and_listen_v1 : public basic_bind_and_listen_interface{
    public:
        bind_and_listen_v1() ;
        bind_and_listen_v1(std::shared_ptr<socket_io_handle_interface> handle , std::shared_ptr<basic_endpoint_interface > endpoint )  ;
        
        int bind() override;
        int bind(std::shared_ptr<socket_io_handle_interface>  handle , std::shared_ptr<basic_endpoint_interface>  ) override;
        const std::string & ip_address() override;
        const int protocol() override;
        const int stream() override;
        int listen(int backlog) override;
        
    private:
        class impl;
        std::shared_ptr<impl> impl_;
};

#endif
