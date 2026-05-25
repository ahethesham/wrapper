#ifndef __CONNECTION_HANDLE_H__
#define __CONNECTION_HANDLE_H__

#include "basic_connection_handle_interface.h"
#include "basic_connection_pool_interface.h"
class connection_handle_v1 : public basic_connection_handle_interface{

    public:
        connection_handle_v1(std::shared_ptr<basic_connection_interface> conn , basic_connection_pool_interface & pool);
        connection_handle_v1(connection_handle_v1 &) = default;

        basic_connection_handle_interface & mark_dead() override;
        basic_connection_handle_interface & release() override;

        std::shared_ptr<basic_connection_interface> operator->() override;
        basic_connection_interface & operator*() override;

        bool is_active() override;
        ~connection_handle_v1();
    private:
        class impl;
        std::shared_ptr<impl> impl_;
};

#endif
