#ifndef __BASIC_CONNECTION_HANDLE_INTERFACE_H__
#define __BASIC_CONNECTION_HANDLE_INTERFACE_H__


#include "basic_connection_interface.h"
#include <memory>
class basic_connection_handle_interface{
    public:
        virtual std::shared_ptr<basic_connection_interface> operator->() = 0;

        // this is highly depreciated please avoid doing this 
        virtual basic_connection_interface & operator*() = 0;

        virtual basic_connection_handle_interface & mark_dead() = 0;

        virtual basic_connection_handle_interface & release() = 0;

        virtual bool is_active() = 0;

        basic_connection_handle_interface() = default;
};

#endif
