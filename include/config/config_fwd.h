#ifndef __CONFIG_FWD_H__
#define __CONFIG_FWD_H__

#include "stream_fwd.h"
#include "basic_fd.h"

template<typename impl>
class basic_config_parser;

template<typename container_impl>
class ini_parser_impl;


template<typename buffer_policy>
class ini_config_container_impl ;

using config_fd = basic_socket<file_fd_impl>;

using ini_config_parser = basic_config_parser<ini_parser_impl<ini_config_container_impl<buffer_v1>> >;

inline ssize_t file_reader(FILE *fptr , char * data , ssize_t size){
    return ::fread(data , sizeof(char) , size , fptr);
}

using ini_config_reader = basic_istream<config_fd , basic_reader<file_fd_impl , buffer_v1 , file_reader> , ini_config_parser >;

#endif

#include "basic_config_reader.h"
#include "basic_fd.h"

