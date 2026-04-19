#ifndef __BASIC_CONFIG_INTERFACE_H__
#define __BASIC_CONFIG_INTERFACE_H__


#include "buffer.h"
#include "stream_fwd.h"
#include <string>

template<typename key_policy ,
        typename value_policy >
class basic_container_interface{
    public:
        using key_type = key_policy;
        using value_type = value_policy;
        using self_type = basic_container_interface<key_type , value_type>;

        virtual value_type & operator[](key_type  key) = 0;
        virtual self_type & push(key_type & key ,  value_type & value ) = 0;
};

template< typename reader_interface_policy ,
        typename container_policy  = basic_container_interface<std::string , std::string>,      
    typename buffer_interface_policy = buffer_v1 >
class basic_config_interface{

    public:
        using reader_type = reader_interface_policy;
        using self_type = basic_config_interface<reader_type , container_policy , buffer_interface_policy>;
        using buffer_type = buffer_interface_policy;
        using container_type = container_policy;
        using key_type = container_type::key_type;
        using value_type = container_type::value_type;

        virtual ~basic_config_interface() = default;
        
        /*
         * These two API shall be used by the stream 
         */
        //virtual bool continue_reading() = 0;
        //virtual self_type & parse( buffer_type * buffer) = 0;
        //virtual self_type & at_eof(buffer_type * buffer) = 0;

        virtual container_type &  operator[](key_type key) = 0;

        virtual container_type &  get(key_type key) = 0;

};



#endif
