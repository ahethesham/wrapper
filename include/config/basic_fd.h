#ifndef __BASIC_FD_H__
#define __BASIC_FD_H__
#include "socket.h"
#include <cstdio>
#include <stdexcept>
#include "logger.h"
class file_fd_impl{
    public:
        using sockaddr_type = const char * ;
        using fd_type       = FILE *;
        using network = int;

        file_fd_impl(sockaddr_type f_path) : file_path_(f_path){
            if(!open())throw std::runtime_error("File Not Found ");
        }
        bool open(){
            fd_ = fopen(file_path_ , "r");
            log("t3");
            return fd_ != nullptr;
        }

        fd_type get(){
            return fd_;
        }

        void close(){
            ::fclose(fd_);
        }

        bool is_closed(){
            return false;
        }
    private:
        fd_type fd_;
        sockaddr_type file_path_;
};



#endif
