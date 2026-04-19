#include "io_handles_fwd.h"
#include "file_io_handle.h"
#include <fcntl.h>
#include <iostream>

template<enum file_operations f_op>
file_io_handle<f_op>::fd_type file_io_handle<f_op>::open() {
    switch(f_op){
        case READ_ONLY:
            fd_ = ::fopen(path_ , "r");
            break;
        case READ_AND_WRITE:
            fd_ = ::fopen(path_ , "rw");
        case WRITE:
            fd_ = ::fopen(path_ , "w");
    }
    if(fd_ == nullptr){
        throw std::runtime_error(" unable to open here ");
    }
    return fd_;
}

template<enum file_operations f_op> file_io_handle<f_op>::file_io_handle(endpoint_reference_type path) : path_(path){
    open();
}

template<enum file_operations f_op> file_io_handle<f_op>::fd_type file_io_handle<f_op>::get() {
    return fd_;
}


template<enum file_operations f_op> bool file_io_handle<f_op>::is_closed() {
    return  fd_ == nullptr;
}

template<enum file_operations f_op> file_io_handle<f_op>::self_type & file_io_handle<f_op>::close() {
    if(fd_ == nullptr)return *this;

    ::fclose(fd_);
    fd_ = nullptr;
    return *this;
}


//file_io_handle_interface * file_io_handle_interface::build(file_io_handle_interface::endpoint_reference_type path){
//    return new file_io_handle<WRITE>(path);
//}

