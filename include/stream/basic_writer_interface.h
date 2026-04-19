#ifndef __BASIC_WRITER_INTERFACE_H__
#define __BASIC_WRITER_INTERFACE_H__

#include "basic_parser_interface.h"
#include "buffer.h"
#include <functional>
#include <string>
#include <unistd.h>

template < typename io_handle_policy ,
             typename parser_policy = basic_parser_interface<buffer_v1>,
          typename buffer_policy = buffer_v1  >
class basic_writer_interface{

    public:
        virtual ~basic_writer_interface()  {};

        typedef io_handle_policy io_handle_type;
        typedef buffer_policy    buffer_type;
        typedef parser_policy    parser_type;
        typedef ssize_t size_type;
        using self_type = basic_writer_interface<io_handle_type ,parser_type , buffer_type>;

        virtual size_type write(void * buffer , ssize_t size ) = 0;

        virtual size_type write(buffer_type * buffer) = 0;
        virtual size_type write(parser_type & parser) = 0;


        /*
         * this only copies data to local buffer and will not send untill flush is called 
         */
        virtual basic_writer_interface & operator<<( parser_policy & parser) = 0;
        virtual basic_writer_interface & operator<<(std::string & str ) = 0;
        virtual basic_writer_interface & operator<<( const char * str) = 0;
        virtual basic_writer_interface & operator<<(int num) = 0;
        virtual basic_writer_interface & operator<<(std::function<basic_writer_interface & (basic_writer_interface & ) >  func) = 0;

        /*
         * calling this function will flush the buffer to the other end
         */
        virtual basic_writer_interface & flush() = 0;

        static inline self_type & endl(self_type & os){
            return os.flush();
        }
};

#endif
