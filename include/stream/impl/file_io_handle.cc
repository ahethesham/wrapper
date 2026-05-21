#include "io_handles_fwd.h"
#include "file_io_handle.h"
#include <fcntl.h>
#include <iostream>

template<enum file_operations f_op>
class file_io_handle<f_op>::impl{
    public:
        impl(endpoint_reference_type path) :
            path_(path){}

        file_io_handle<f_op>::fd_type open(){
            std::cout << "opening file for reading " << path_ << std::endl;
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
        file_io_handle<f_op>::fd_type get(){
            return fd_;
        }
        bool is_closed(){
            return fd_ == nullptr;
        }
        int get_lowest_level_fd(){
            return lowest_fd_;
        }
        int close(){
           return  ::fclose(fd_);
        }
        int set_flags(int f){
            return 0;
        }
        int get_flags(){
            return 0;
        }
    private:
        file_io_handle<f_op>::fd_type fd_;
        int lowest_fd_;
        file_io_handle<f_op>::endpoint_reference_type path_;
};

template<enum file_operations f_op>
file_io_handle<f_op>::fd_type file_io_handle<f_op>::open() {
    return impl_->open();
}

template<enum file_operations f_op> file_io_handle<f_op>::file_io_handle(endpoint_reference_type path) : impl_(std::make_shared<impl>(path)){
    open();
}

template<enum file_operations f_op> file_io_handle<f_op>::fd_type file_io_handle<f_op>::get() {
    return impl_->get();
}


template<enum file_operations f_op> bool file_io_handle<f_op>::is_closed() {
    return  impl_->is_closed();
}

template<enum file_operations f_op> file_io_handle<f_op>::self_type & file_io_handle<f_op>::close() {
    impl_->close();
    return *this;
}

template<enum file_operations f_op> 
int file_io_handle<f_op>::get_lowest_level_fd(){
    return impl_->get_lowest_level_fd();
}

template<enum file_operations f_op> 
int file_io_handle<f_op>::get_flags(){
    return impl_->get_flags();
}

template<enum file_operations f_op> 
int file_io_handle<f_op>::set_flags(int f){
    return impl_->set_flags(f);
}
