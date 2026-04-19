#ifndef __FILE_IO_HANDLE_H__
#define __FILE_IO_HANDLE_H__
#include "io_handles_fwd.h"
typedef enum file_operations{
    READ_ONLY ,
    READ_AND_WRITE ,
    READ_BINARY ,
    WRITE,
    APPEND
}file_operations;

template<enum file_operations f_op >
class file_io_handle :public file_io_handle_interface{

    public:
        using base_type = file_io_handle_interface;
        using endpoint_reference_type = base_type::endpoint_reference_type;
        using fd_type = base_type::fd_type;
        using self_type = file_io_handle;

        file_io_handle(endpoint_reference_type endpoint) ;

        fd_type get() override;
        fd_type open() override;
        bool is_closed() override;
        self_type & close() override;
    private:
        file_io_handle() =delete;
        endpoint_reference_type  path_; // will be a pointer type
        fd_type fd_;
};

template class file_io_handle<WRITE>;
template class file_io_handle<READ_ONLY>;

#endif
