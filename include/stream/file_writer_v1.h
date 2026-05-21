#ifndef __FILE_WRITER_V1_H__
#define __FILE_WRITER_V1_H__
#include "basic_parser_interface.h"
#include "basic_writer_interface.h"
#include "file_io_handle.h"


namespace detail{
    using file_writer_callback = std::function<int(int rc)>;
    inline int std_file_writer(FILE * fptr , char * buffer , size_t size , file_writer_callback cb){
        int rc = ::fwrite(buffer ,sizeof(char), size ,fptr);
        fflush(fptr);
        return cb(rc);
    }
}

using file_writer_interface = basic_writer_interface< file_io_handle_interface , basic_parser_interface<buffer_v1> , buffer_v1>;

template class basic_writer_interface< file_io_handle_interface , basic_parser_interface<buffer_v1> , buffer_v1>;

class file_writer_v1 : public file_writer_interface{
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

        size_type write(parser_type & parser) override;

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
using file_writer = file_writer_v1;
#endif
