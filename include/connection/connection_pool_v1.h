#ifndef __CONNECTION_POOL_H__
#define __CONNECTION_POOL_H__


#include "basic_connection_handle_interface.h"
#include "basic_connection_pool_interface.h"
#include "connection_handle_v1.h"

class connection_pool_v1 : public basic_connection_pool_interface{
    public:
         struct config{
            int keep_alive_timeout_;
            int max_connections_;
            int min_connections_;
            int read_timeout_;
            int write_timeout_;
            bool tls_;
            std::string host_;
            // default value will be 80
            int port_;
        };
        connection_pool_v1(config  cfg);
        // connection can only be made after use calls connect with config for this constructor
        connection_pool_v1();
        connection_pool_v1(connection_pool_v1 & ) = delete;
        connection_pool_v1(connection_pool_v1 && ) = delete;
        connection_pool_v1 & operator=(connection_pool_v1 &) = delete;
        connection_pool_v1 & operator=(connection_pool_v1 &&) = delete;
        
        std::shared_ptr<basic_connection_handle_interface> acquire() override;

        basic_connection_pool_interface & release(std::shared_ptr<basic_connection_handle_interface>) override;
        basic_connection_pool_interface & release(std::shared_ptr<basic_connection_interface>) override;

        basic_connection_pool_interface & resize(int size) override;

        int id() override;
    private:
        class impl;
        std::shared_ptr<impl> impl_;
};

#endif
