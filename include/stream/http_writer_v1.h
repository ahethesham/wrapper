#ifndef __HTTP_WRITER_V1_H__
#define __HTTP_WRITER_V1_H__
#include "basic_parser_interface.h"
#include "io_handles_fwd.h"
#include "buffer.h"
#include "basic_writer_interface.h"


template< typename io_handle_policy ,
          auto func>
class http_writer_v1 : public basic_writer_interface< io_handle_policy >{
    public:
        using base_type      = basic_writer_interface< io_handle_policy >;
        using self_type      = http_writer_v1;
        using io_handle_type = io_handle_policy;
        using size_type      = base_type::size_type;
        using buffer_type    = base_type::buffer_type;
        using parser_type    = base_type::parser_type;
        
        http_writer_v1(io_handle_type * handle_);
        
        size_type write(void * buffer , ssize_t size) override;
        size_type write(buffer_type * buffer) override;
        size_type write(parser_type & parser) override;

        self_type & operator<<(parser_type & parser) override;
        self_type & operator<<(std::string & str) override;
        self_type & operator<<(const char * str) override;
        self_type & operator<<(int num) override;
        self_type & operator<<(std::function<base_type &( base_type & ) > flush) override;
        
        self_type & flush() override;
    private:
        io_handle_type * handle_;
        buffer_type    * buffer_;

};

#endif

