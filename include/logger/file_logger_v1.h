#ifndef __LOGGER_FWD_H__
#define __LOGGER_FWD_H__
#include "buffer.h"
#include <functional>
#include <memory>
#include <string>
#include "file_io_handle.h"
#include "file_writer_v1.h"
#include "basic_logger_interface.h"

typedef     file_io_handle<WRITE> log_file_handle;
typedef     file_writer           log_writer;

struct log_level{
    int log_number_;
    std::string str_;
    log_level(int n , std::string && str) : log_number_(n) , str_(str) {}
};

enum LogLevel{
    CRITICAL,
    FATAL,
    ERROR,
    WARN,
    INFO,
    DEBUG
};

template<typename io_handle_policy =  log_file_handle ,
         typename buffer_policy = buffer_v1 ,
         typename writer_policy = log_writer>
class file_logger_v1 : public basic_logger_interface{
    
    public:

        using base_type = basic_logger_interface;
        using self_type = file_logger_v1<io_handle_policy , buffer_policy , writer_policy>;
        using io_handle_type = io_handle_policy;
        using buffer_type = buffer_policy;
        using writer_type = writer_policy;

        /* to make sure we have only one file per process ...and multiple per thread */
        static file_logger_v1 & build(const char * f_path = nullptr);
        static file_logger_v1 & instance(LogLevel level);
        LogLevel & log_level();
        file_logger_v1(io_handle_type & handle);
        file_logger_v1(io_handle_type & handle , writer_type & writer);
        file_logger_v1 & log(const char * fmt , ...) override; 
        file_logger_v1 & operator << (const std::string & str) override;
        file_logger_v1 & operator << (int data) override;
        file_logger_v1 & operator << (const char * data) override;
        file_logger_v1 & operator << (char ch) override;
        base_type & operator << (std::function<basic_logger_interface & (basic_logger_interface &)> func) override;
        file_logger_v1& flush() override;
    private:
        file_logger_v1(const char * file); 
        void set_metadata(char *);
        const char * get_level();
        buffer_type * buffer_;
        io_handle_type * handle_;
        writer_type * writer_;
        LogLevel log_level_;
};


using Logger = file_logger_v1< log_file_handle , buffer_v1 , log_writer>;



inline basic_logger_interface & endl(basic_logger_interface & os){
    return os.flush();
}

extern Logger * gLogger;


#define LOG_INFO \
    if(Logger::build().log_level() >= LogLevel::INFO) \
    Logger::instance(LogLevel::INFO)

#define LOG_ERROR \
    if(Logger::build().log_level() >= LogLevel::DEBUG) \
    Logger::instance(LogLevel::DEBUG)

#define LOG_WARN \
    if(Logger::build().log_level() >= LogLevel::WARN) \
    Logger::instance(LogLevel::WARN)

#define LOG_CRITICAL \
    if(Logger::build().log_level() >= LogLevel::CRITICAL) \
    Logger::instance(LogLevel::CRITICAL)

#define LOG_DEBUG \
    if(Logger::build().log_level() >= LogLevel::DEBUG) \
    Logger::instance(LogLevel::DEBUG)

#define LOG_FATAL \
    if(Logger::build().log_level() >= LogLevel::FATAL) \
    Logger::instance(LogLevel::FATAL)

#endif
