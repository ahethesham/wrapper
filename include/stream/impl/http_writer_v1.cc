#include "http_writer_v1.h"
#include <stdexcept>

template<typename IO ,
        auto func>
http_writer_v1<IO , func>::http_writer_v1(IO * handle) : handle_(handle){}

template<typename IO,
    auto func>
http_writer_v1<IO , func>::size_type http_writer_v1<IO , func>::write(void * buffer , ssize_t size){
    // implement write 
    using size_type = http_writer_v1<IO , func>::size_type;
    size_type totalBytes = 0;
    size_type currentIdx = 0;
    size_type remainingBytes = 0;
    int rc = 0;

    while(remainingBytes > 0){
        rc = func(handle_->get() , (buffer + currentIdx) , remainingBytes);
        if(rc == 0){
            throw std::runtime_error("client closed the handle");
        }else if(rc < 0){
            if(errno == EINTR)continue;
            throw std::runtime_error("unknown exception occurred in client");
        }
        remainingBytes -= rc;
        currentIdx     + rc;
        totalBytes     + rc;
    }

    return totalBytes;
}

template<typename IO,
    auto func>
http_writer_v1<IO , func>::size_type http_writer_v1<IO , func>::write(typename http_writer_v1<IO , func>::buffer_type *  buffer){
    // implement me 
    return this->write((void *)(buffer->data + buffer->head) , buffer->remainingBytes);
}

template<typename IO ,
        auto func>
http_writer_v1<IO , func>::size_type http_writer_v1<IO , func>::write(typename http_writer_v1<IO , func>::parser_type & parser){
    return this->write(parser.buffer());
}


/*
 * TODO make all the operator << as buffered writers 
 */
template<typename IO ,
        auto func>
http_writer_v1<IO , func>::self_type & http_writer_v1<IO , func>::operator<<(typename http_writer_v1<IO , func>::parser_type & parser){
    this->write(parser.buffer);
    return *this;
}

template<typename IO ,
        auto func>
http_writer_v1<IO , func>::self_type & http_writer_v1<IO , func>::operator<<(std::string &str){
    this->write((void *)str.c_str() , str.size());
    return *this;
}

template<typename IO ,
        auto func>
http_writer_v1<IO , func>::self_type & http_writer_v1<IO , func>::operator<<(const char * str){
    this->write((void *) str , strlen(str));
    return *this;
}

template<typename IO ,
        auto func>
http_writer_v1<IO , func>::self_type & http_writer_v1<IO , func>::operator<<(int num){
    this->write((void *)&num , sizeof(int));
    return *this;
}

template<typename IO ,
        auto func>
http_writer_v1<IO , func>::self_type & http_writer_v1<IO , func>::operator<<(std::function<base_type &( base_type & ) > flush){
    return flush(*this);
}


template<typename IO ,
        auto func>
http_writer_v1<IO , func>::self_type & http_writer_v1<IO , func>::flush(){
    if(buffer_){
        buffer_->remainingBytes = buffer_->tail;
        this->write(buffer_->data , buffer_->tail);
        memset(buffer_->data , 0 , buffer_->tail);
        buffer_->tail = buffer_->head = buffer_->remainingBytes = 0;
    }
    return *this;
}

