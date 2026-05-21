#ifndef __BASIC_CONNECTION_INTERFACE_H__
#define __BASIC_CONNECTION_INTERFACE_H__


#include <memory>
class basic_connection_pool_interface{

    public:
        // create , acquire , release , size ?
        virtual basic_connection_pool_interface & create( std::string & host , int port = 443) = 0;
        virtual basic_connection_pool_interface & acquire() = 0;
        virtual basic_connection_pool_interface & release() = 0 ;

};

#endif
