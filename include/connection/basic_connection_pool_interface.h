#ifndef __BASIC_CONNECTION_INTERFACE_H__
#define __BASIC_CONNECTION_INTERFACE_H__


#include "basic_connection_interface.h"
#include "basic_connection_handle_interface.h"
#include <memory>

/*
 * It is only responsible for maintaining pool of connections
 * not the connections itself , every thread should get a
 */

class basic_connection_pool_interface{

    public:
        // create , acquire , release , size ?
        virtual std::shared_ptr<basic_connection_handle_interface>  acquire() = 0;
        virtual basic_connection_pool_interface & release(std::shared_ptr<basic_connection_handle_interface> conn) = 0 ;
        virtual basic_connection_pool_interface & release(std::shared_ptr<basic_connection_interface> conn) = 0 ;

        virtual basic_connection_pool_interface & resize(int size) = 0;
        
        virtual int id() = 0;

        basic_connection_pool_interface() = default;


};

#endif
