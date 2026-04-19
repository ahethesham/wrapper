#include "file_writer_v1.h"
#include "stream_fwd.h"
#include <stdexcept>

using io_handle_type = file_writer_v1::io_handle_type;
using size_type = file_writer_v1::size_type;
using parser_type = file_writer_v1::parser_type;
using buffer_type = file_writer_v1::buffer_type;
using self_type = file_writer_v1;
using base_type = file_writer_v1::base_type;

file_writer_v1::file_writer_v1(io_handle_type * handle) : handle_(handle) , buffer_(new buffer_type()){}

size_type file_writer_v1::write(void * buffer , ssize_t size) {
    size_type totalBytes = 0;
    size_type current_idx = 0;
    int remaining_bytes = size;
    int rc;
    while(remaining_bytes > 0 && !handle_->is_closed()){
        rc = std_file_writer( handle_->get() , (void *)((char *)buffer + current_idx) , remaining_bytes);
        if(rc == 0){
            handle_->close();
            throw std::runtime_error("Unable to write data  to  file fd closed");
        }else if(rc < 0){
            throw std::runtime_error("Unable to write data  to  file");
        }
        remaining_bytes -=  rc;
        current_idx += rc;
    }
    return  current_idx;
}

size_type file_writer_v1::write(buffer_type * buffer) {
    size_type rc = write(buffer->data + buffer->head ,  buffer->remainingBytes);
    if(rc > 0){
        buffer->remainingBytes -= rc;
        buffer->head += rc;
    }
    return rc;
}

self_type & file_writer_v1::operator<<(parser_type & parser) {
    auto temp = parser.buffer();
    memcpy((buffer_->data + buffer_->tail) , temp->data + temp->head , temp->remainingBytes);
    buffer_->tail += temp->remainingBytes;
    temp->head += temp->remainingBytes;
    temp->remainingBytes = 0;
    return *this;
}

self_type &  file_writer_v1::operator<<( std::string & str)  {
    memcpy((buffer_->data + buffer_->tail) , str.c_str() , str.length());

    buffer_->tail +=  strlen(str.c_str());
    return *this;
}

self_type & file_writer_v1::operator<<(const char * str) {
    memcpy((buffer_->data + buffer_->tail) , str , strlen(str));
    buffer_->tail +=  strlen(str);  
    return *this;
}

self_type & file_writer_v1::operator<<(int num) {
    sprintf((buffer_->data + buffer_->tail) , "%d" , num);

    buffer_->tail  = strlen(buffer_->data);
    return *this;
}

self_type & file_writer_v1::flush() {
    // flush the buffer to the remote process
    buffer_->remainingBytes = buffer_->tail;
    write(buffer_);
    memset(buffer_->data , 0 , buffer_->tail);
    buffer_->tail = buffer_->remainingBytes = buffer_->head = 0;

    return *this;
}
base_type & file_writer_v1::operator<<(std::function<basic_writer_interface & (basic_writer_interface & ) >  func) {
    return func(*this);
}
