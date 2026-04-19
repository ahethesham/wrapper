#ifndef __FILE_WRITER_V1_H__
#define __FILE_WRITER_V1_H__
#include "basic_parser_interface.h"
#include "basic_writer_interface.h"
#include "io_handles_fwd.h"
template class basic_writer_interface< file_io_handle_interface , basic_parser_interface<buffer_v1> , buffer_v1>;

class file_writer_v1 : public basic_writer_interface< file_io_handle_interface ,
                                                     basic_parser_interface<buffer_v1> ,
                                                     buffer_v1>{
    public:
        using base_type = basic_writer_interface<file_io_handle_interface , basic_parser_interface<buffer_v1> , buffer_v1>;
        using io_handle_type = base_type::io_handle_type;
        using  parser_type = base_type::parser_type;
        using self_type = file_writer_v1;
        using size_type = base_type::size_type;

        file_writer_v1(file_io_handle_interface * handle) ;
        ~file_writer_v1() = default;

        size_type write(void * buffer , ssize_t size) override;

        size_type write(buffer_type * buffer) override;

        self_type & operator<<(parser_type & parser) override;

        self_type &  operator<<( std::string & str) override ;

        self_type & operator<<(const char * str) override;

        self_type & operator<<(int num) override;

        self_type & flush() override;

        base_type & operator<<(std::function<basic_writer_interface & (basic_writer_interface & ) >  func) override;

    private:
        buffer_type *  buffer_;
        io_handle_type * handle_;

};

#endif
