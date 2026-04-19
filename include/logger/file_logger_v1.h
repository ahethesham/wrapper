#ifndef __LOGGER_FWD_H__
#define __LOGGER_FWD_H__
#include "buffer.h"
#include <functional>
#include <memory>
#include <string>
#include "file_io_handle.h"
#include "basic_logger_interface.h"
#include "stream_fwd.h"

typedef     file_io_handle<WRITE> log_file_handle;
typedef     file_writer           log_writer;


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
};


using Logger = file_logger_v1< log_file_handle , buffer_v1 , log_writer>;


inline basic_logger_interface & endl(basic_logger_interface & os){
    return os.flush();
}

extern Logger * gLogger;

#endif
