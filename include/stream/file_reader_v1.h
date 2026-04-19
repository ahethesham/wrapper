#ifndef __FILE_READER_V1_H__
#define __FILE_READER_V1_H__

#include "io_handles_fwd.h"
#include "basic_parser_interface.h"
#include "buffer.h"
#include "basic_reader_interface.h"
/*
 * Any object which wants to utilize the file reader object must implement the parser interface 
 */

class file_reader_v1 : public basic_reader_interface<file_io_handle_interface ,
                              basic_parser_interface<buffer_v1> ,
                              buffer_v1 >
{

        public:
            using base_type = basic_reader_interface<file_io_handle_interface ,
                              basic_parser_interface<buffer_v1> ,
                              buffer_v1 >;
            using io_handle_type = base_type::io_handle_type;
            using parser_type = base_type::parser_type;
            using buffer_type =  base_type::buffer_type;
            using size_type = base_type::size_type;

            using self_type = file_reader_v1;

            file_reader_v1(io_handle_type *  handle) ;

            ~file_reader_v1() = default;

            size_type read(void * buffer , ssize_t size) override;
            self_type & read(parser_type & parser) override;

            size_type  read(buffer_type * buffer) override;
            self_type & operator>>(parser_type & parser) override;

        private:
            io_handle_type & handle_;
};


#endif
