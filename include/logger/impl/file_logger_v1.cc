#include "file_logger_v1.h"
#include "file_writer_v1.h"
#include "io_handles_fwd.h"
#include "stream_fwd.h"
#include <cstdarg>
#include <ctime>
#include <pthread.h>
#include <string>
#include <unistd.h>

using impl = Logger;
using io_handle_type = impl::io_handle_type;
using buffer_type = impl::buffer_type;
using writer_type = impl::writer_type;

template<>
impl::file_logger_v1(io_handle_type & handle) : handle_(&handle),writer_ ( new file_writer(&handle)) , buffer_ ( new buffer_type()) {}



template<>
basic_logger_interface & impl::operator << (std::function<basic_logger_interface & (basic_logger_interface & ) > func) {
    return func(*this);
}

template<> const char * impl::get_level(){
    static const char info[5] = "INFO";
    return info;
}

template<> void impl::set_metadata(char * buffer)
{
    struct tm * tm_info;
    time_t   t ;
    time(&t);

    tm_info = localtime(&t);

    static char hostname[1024] = {0};
    if(hostname[0] == '\0')
        gethostname(hostname , 1024);

    sprintf(buffer , "%d:%.2d:%.2d %.2d:%.2d:%.2d|%s|%s|%ld|%s| " , tm_info->tm_year + 1900,
                                                     tm_info->tm_mon + 1 ,
                                                     tm_info->tm_mday ,
                                                     tm_info->tm_hour ,
                                                     tm_info->tm_min ,
                                                     tm_info->tm_sec ,
                                                     hostname ,
                                                     "" ,
                                                     pthread_self() ,
                                                     get_level());

    return ;

}




template<> inline impl & impl::log(const char * fmt , ...)  {
    set_metadata(buffer_->data );

    int idx = strlen(buffer_->data);
    buffer_->tail = idx;

    va_list list;
    
    va_start(list , fmt);
    vsprintf((buffer_->data + buffer_->tail) ,fmt , list);
    va_end(list);
    buffer_->tail = strlen(buffer_->data);
    buffer_->data[buffer_->tail++] = '\n';
    buffer_->remainingBytes = buffer_->tail;
    writer_->write(buffer_);

    memset(buffer_->data , 0 , buffer_->tail);
    buffer_->tail = buffer_->head = buffer_->remainingBytes = 0;

    return *this ;
}

template<> impl & impl::build(const char * file_format){
    static impl * instance_ = nullptr;
    if(instance_ != nullptr){
        return * instance_;
    }
    char  file[1024] = {0};
    static char hostname[1024] = {0};
    if(hostname[0] == '\0')
        gethostname(hostname , 1024);

    sprintf(file , file_format , hostname , getpid() , pthread_self());

    return * (instance_ = new impl(*new io_handle_type((const char *)file)));
}

template<> impl & impl::operator << (const std::string & str) {

    if(buffer_->tail == 0){
        set_metadata(buffer_->data );
        buffer_->tail = strlen(buffer_->data);
    }

    memcpy((buffer_->data + buffer_->tail) , str.c_str() , str.length());

    buffer_->tail =  strlen(buffer_->data);
    buffer_->data[buffer_->tail++] = '\n';
    buffer_->remainingBytes = buffer_->tail;


    return *this;
    
}

template<> inline impl & impl::operator << (int data) {
    

    if(buffer_->tail == 0){
        set_metadata(buffer_->data );
        buffer_->tail = strlen(buffer_->data);
    }

    sprintf((buffer_->data + buffer_->tail) , "%d" , data);

    buffer_->tail  = strlen(buffer_->data);

    return *this;
}

template<> inline impl & impl::operator << (const char * data) {
    
    if(buffer_->tail == 0){
        set_metadata(buffer_->data );
        buffer_->tail = strlen(buffer_->data);
    }

    memcpy((buffer_->data + buffer_->tail) , data , strlen(data));

    buffer_->tail =  strlen(buffer_->data);  
    //buffer_->data[buffer_->tail++] = '\n';

    return *this;
}

template<> inline impl & impl::operator << (char ch) {
    
    if(buffer_->tail == 0){
        set_metadata(buffer_->data );
        buffer_->tail = strlen(buffer_->data);
    }

    memcpy((buffer_->data + buffer_->tail) , &ch , sizeof(char));

    buffer_->tail +=  sizeof(char);  
    //buffer_->data[buffer_->tail++] = '\n';

    return *this;
}


template<> impl & impl::flush() {
    buffer_->data[buffer_->tail ++ ] = '\n';
    buffer_->remainingBytes = buffer_->tail;
    writer_->write(buffer_);
    memset(buffer_->data , 0 , buffer_->tail);
    buffer_->tail = buffer_->remainingBytes = buffer_->head = 0;

    return *this;
}


