#ifndef __STREAM_FWD_H__
#define __STREAM_FWD_H__
#include <unistd.h>
#include "basic_parser_interface.h"
#include "openssl/ssl.h"
#include "io_handles_fwd.h"
#include "basic_reader_interface.h"
#include "file_writer_v1.h"
#include "file_reader_v1.h"
#include "http_reader_v1.h"
#include "http_writer_v1.h"

struct buffer_v1;

inline ssize_t std_reader(int fd , void * buffer , ssize_t size){
    return ::read(fd , buffer , size);
}
inline ssize_t ssl_reader(SSL * fd, void *buffer  , ssize_t size){
    return ::SSL_read(fd ,buffer , size );
}
inline ssize_t std_writer(int fd, void * buffer , ssize_t size){
    return ::write(fd , buffer , size);
}
inline ssize_t ssl_writer(SSL * fd , void * buffer , ssize_t size){
    return ::SSL_write(fd , buffer , size);
}
inline ssize_t std_file_reader(FILE * fptr, void *buffer , ssize_t size){
    return ::fread(buffer , sizeof(char) , size , fptr);
}
inline ssize_t std_file_writer(FILE * fptr , void * buffer, ssize_t size){
    ssize_t rc = ::fwrite(buffer , sizeof(char) , size , fptr);
    ::fflush(fptr);
    return rc;
}



//template<typename T>
//inline T & endl(T & object){
//    return object.flush();
//}


//using file_writer_interface = basic_writer_interface<file_io_handle_interface >;
using file_reader_interface = basic_reader_interface<file_io_handle_interface ,
                              basic_parser_interface<buffer_v1> ,
                              buffer_v1 >;


using http_reader = http_reader_v1<socket_io_handle_interface , std_reader>;
using https_reader = http_reader_v1<ssl_socket_io_handle_interface , ssl_reader>;

using http_writer = http_writer_v1<socket_io_handle_interface , std_writer>;
using https_writer = http_writer_v1<ssl_socket_io_handle_interface , ssl_writer>;

using file_reader = file_reader_v1;
using file_writer = file_writer_v1;

#endif
