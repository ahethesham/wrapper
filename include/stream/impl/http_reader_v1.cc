
#include "http_reader_v1.h"
#include "basic_parser_interface.h"
#include "stream_fwd.h"
#include <stdexcept>

template<typename IO ,
         auto func>
http_reader_v1<IO , func>::http_reader_v1(io_handle_type * io_handle) : handle_(io_handle){}

template<typename IO , auto func>
http_reader_v1<IO, func>::size_type http_reader_v1<IO , func>::read(void * buffer , ssize_t size) {
   int rc ;
   int bufferIdx = 0;
   size_type totalBytes = 0;
   int remainingBytes = size;
   while(remainingBytes > 0){
        rc = func(handle_->get() ,(void *)( (char *)buffer + bufferIdx ), remainingBytes);
        if(rc == 0){
            break;
        }else if(rc < 0){
            if(errno == EINTR)continue;
            throw std::runtime_error("read interrupted");
        }
        remainingBytes -= rc;
   }

   return totalBytes;
}

template<typename IO , auto func>
http_reader_v1<IO, func>::self_type & http_reader_v1<IO, func>::read(http_parser_interface & parser) {
    buffer_v1 * buffer = new buffer_v1();
    int rc ;
    while(parser.continue_reading())
    {
        rc = reader_func(handle_->get() , buffer->data + buffer->tail , 64 * 1024);
        if(rc == 0){
            parser.at_eof(buffer);
            break;
        }else if(rc < 0){
            if(errno == EINTR)continue;
            throw std::runtime_error("read interrupted");
        }
        buffer->tail += rc;
        parser.parse(buffer);
    }
    return *this;
}

template<typename IO , auto func>
http_reader_v1<IO , func>::size_type http_reader_v1<IO , func>::read(buffer_type * buffer) {
    int rc ;
    int totalBytes = 0;
    while(1)
    {
        rc = reader_func(handle_->get() , buffer->data + buffer->tail , 64 * 1024);
        if(rc == 0){
            // client closed the socket 
            break;
        }else if(rc < 0){
            if(errno == EINTR)continue;
            throw std::runtime_error("read interrupted ");
        }
        totalBytes += rc;
        buffer->tail += rc;
    }
    return totalBytes;
}

template<typename IO , auto func>
http_reader_v1<IO , func>::self_type & http_reader_v1<IO , func>::operator>>(http_parser_interface & parser) {
    return read(parser);
}



