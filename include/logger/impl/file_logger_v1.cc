#include "file_logger_v1.h"
#include "file_writer_v1.h"
#include "io_handles_fwd.h"
#include <cstdarg>
#include <ctime>
#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>

using impl = Logger;
using io_handle_type = impl::io_handle_type;
using buffer_type = impl::buffer_type;
using writer_type = impl::writer_type;

template<>
impl::file_logger_v1(io_handle_type & handle) : handle_(&handle),writer_ ( new file_writer(&handle)) , buffer_ ( new buffer_type()) , log_level_(LogLevel::DEBUG) {}



template<>
basic_logger_interface & impl::operator << (std::function<basic_logger_interface & (basic_logger_interface & ) > func) {
    return func(*this);
}

template<> const char * impl::get_level(){
   return nullptr;
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

    sprintf(buffer , "%d:%.2d:%.2d %.2d:%.2d:%.2d|%s|%s|%ld| " , tm_info->tm_year + 1900,
                                                     tm_info->tm_mon + 1 ,
                                                     tm_info->tm_mday ,
                                                     tm_info->tm_hour ,
                                                     tm_info->tm_min ,
                                                     tm_info->tm_sec ,
                                                     hostname ,
                                                     "" ,
                                                     pthread_self() );

    return ;

}



template<> LogLevel & impl::log_level(){
    return log_level_;
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

// this is a static method
template<>  impl & impl::build(const char * file_format){
    static impl * instance_ = nullptr;
    if(instance_ != nullptr){
        return * instance_;
    }
    char  file[1024] = {0};
    static char hostname[512] = {0};
    if(hostname[0] == '\0')
        gethostname(hostname , sizeof(hostname));
    
    if(file_format == nullptr){
        sprintf(file , "agent-dev-%s-%d-%lu.log" , hostname , getpid() , pthread_self());
    }else{
        sprintf(file , file_format , hostname , getpid() , pthread_self());
    }
    return * (instance_ = new impl(*new io_handle_type((const char *)file)));
}

template<> impl & impl::operator << (const std::string & str) {

    if(buffer_->tail == 0){
        set_metadata(buffer_->data );
        buffer_->tail = strlen(buffer_->data);
    }

    memcpy((buffer_->data + buffer_->tail) , str.c_str() , str.length());

    buffer_->tail =  strlen(buffer_->data);
    //buffer_->data[buffer_->tail++] = '\n';
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


template<>impl & impl::instance(LogLevel level){
    impl & instance_ = impl::build(nullptr);
    if(instance_.buffer_->tail != 0){
        // flush out exisiting log line
        instance_ << endl;
    }
    instance_.set_metadata(instance_.buffer_->data);
    switch(level)
    {
        case LogLevel::CRITICAL:
            instance_ << "CRITICAL |";
            break;
        case LogLevel::DEBUG:
            instance_ << "DEBUG |";
            break;
        case LogLevel::INFO:
            instance_ << "INFO |";
            break;
        case LogLevel::ERROR:
            instance_ << "ERROR |";
            break;
        case LogLevel::FATAL:
            instance_ << "FATAL |";
            break;
        case LogLevel::WARN:
            instance_ << "WARN |";
            break;
    }

    return instance_;

}
