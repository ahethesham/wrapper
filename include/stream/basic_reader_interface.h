#ifndef __BASIC_READER_INTERFACE_H__
#define __BASIC_READER_INTERFACE_H__

#include <unistd.h>
#include "basic_parser_interface.h"
#include "buffer.h"

template < typename io_handle_policy , 
           typename parser_policy = basic_parser_interface<buffer_v1>,
          typename buffer_policy = buffer_v1  >
class basic_reader_interface{
    public:
        using io_handle_type = io_handle_policy;
        using parser_type = parser_policy ;
        using self_type = basic_reader_interface<io_handle_type,parser_type,buffer_policy>;

        virtual ~basic_reader_interface() = default;

        typedef  ssize_t size_type;

        typedef typename io_handle_policy::fd_type fd_type;
        typedef buffer_policy buffer_type;

        virtual size_type read(void * buffer , ssize_t size) = 0;

        virtual basic_reader_interface & read(parser_policy & parser) = 0;

        virtual size_type read(buffer_type * buffer) = 0;
        /*
         *  will read the input and pass it to the parser 
         */
        virtual basic_reader_interface & operator>>(parser_policy & parser ) = 0;

        template<typename impl>
            static self_type * build(io_handle_type *);
};

#endif
