#ifndef __ACCEPTOR_V1_H__
#define __ACCEPTOR_V1_H__

#include "basic_acceptor_interface.h"


/*
 * will be responsible for accepting incoming requests and giving us the file descriptor for it
 */
template<typename protocol>
class acceptor_v1 : public basic_acceptor_interface<protocol>{
    public:
        using client_handle_type = protocol::client_handle_type;
        using server_handle_type = protocol::server_handle_type;

        acceptor_v1(std::shared_ptr<server_handle_type> handle) ;
        acceptor_v1(int fd);

        int current_req_count() override;
        int close_connection(std::shared_ptr<client_handle_type>  handle) override;
        std::shared_ptr<client_handle_type>  accept() override;
    private:
        class impl;
        std::shared_ptr<impl> impl_;
};



#endif

