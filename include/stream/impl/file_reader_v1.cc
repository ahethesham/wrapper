#include <cassert>
#include <stdexcept>
#include "file_reader_v1.h"
#include "stream_fwd.h"

using size_type = file_reader_v1::size_type;

file_reader_v1::file_reader_v1(file_reader_v1::io_handle_type * handle  ) : handle_(*handle) { }

size_type file_reader_v1::read(void * buffer , ssize_t size) {
    fd_type  fd_ = handle_ .get();
    int currentIdx = 0;
    int remainingBytes = size;
    int totalBytes = 0;
    int rc;
    while(remainingBytes > 0){
        rc = std_file_reader(fd_ , (void *)((char *)buffer + currentIdx ), remainingBytes);
        if(rc == 0){
            handle_ .close();
            throw std::runtime_error("Host had closed unexpectedly");
        }else if(rc < 0){
            if(errno == EINTR)continue;
            throw std::runtime_error("unknown error while reading buffer");
        }
        remainingBytes -= rc;
        totalBytes += rc;
        currentIdx += rc;
    }

    return totalBytes;
}
        
file_reader_v1::self_type & file_reader_v1::read(file_reader_v1::parser_type & parser) {
    file_reader_v1::buffer_type * buffer = new file_reader_v1::buffer_type();
    int rc = 0 ;
    while( parser.continue_reading() ){
        if(handle_.is_closed()){
            throw std::runtime_error("handle closed unexpectedly");
            parser.at_eof(buffer);
            break;
        }
        rc = std_file_reader(handle_.get() , (buffer->data + buffer->tail ) , 64 * 1024);
        if(rc == 0){
            parser.at_eof(buffer);
            handle_.close();
            break;
        }else if(rc < 0){
            if(errno == EINTR)continue;
            throw std::runtime_error("unknown error while reading buffer");
        }
        buffer->tail += rc;
        parser.parse(buffer);
    }

    return *this;
}

file_reader_v1::size_type file_reader_v1::read(file_reader_v1::buffer_type * buffer) {
    int rc = 0 ;
    while( 1 ){
        if(handle_.is_closed()){
            break;
        }
        rc = std_file_reader(handle_.get() , (buffer->data + buffer->tail ) , 64 * 1024);
        if(rc == 0){
            handle_.close();
            break;
        }else if(rc < 0){
            if(errno == EINTR)continue;
            throw std::runtime_error("unknown error while reading buffer");
        }
        buffer->tail += rc;
    }

    return buffer->tail;
}

file_reader_v1::self_type & file_reader_v1::operator >> (parser_type & parser) {
    buffer_type * buffer = new buffer_type();
    int rc = 0 ;
    while( parser.continue_reading() ){
        if(handle_.is_closed()){
            parser.at_eof(buffer);
            break;
        }
        rc = std_file_reader(handle_.get() , (buffer->data + buffer->tail ) , 64 * 1024);
        if(rc == 0){
            parser.at_eof(buffer);
            handle_.close();
            break;
        }else if(rc < 0){
            if(errno == EINTR)continue;
            throw std::runtime_error("unknown error while reading buffer");
        }
        buffer->tail += rc;
        parser.parse(buffer);
    }

    return *this;
    
} 



