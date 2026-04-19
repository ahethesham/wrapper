#ifndef __BASIC_LOGGER_INTERFACE_H__
#define __BASIC_LOGGER_INTERFACE_H__
#include <string>
#include <functional>
class basic_logger_interface{
    public:
        using self_type = basic_logger_interface;

        virtual inline basic_logger_interface & log(const char * fmt , ...) = 0;
        
        virtual basic_logger_interface & operator << (const std::string & str) = 0;

        virtual basic_logger_interface & operator << (int data) = 0;

        virtual basic_logger_interface & operator << (const char * data) = 0;

        virtual basic_logger_interface & operator << (char ch) = 0;

        virtual basic_logger_interface & flush() = 0;

        virtual basic_logger_interface & operator << (std::function<basic_logger_interface&(basic_logger_interface &)> func) = 0;

        template<typename impl>
            static inline impl & log_builder(const char * f_path){
                return impl::build(f_path);
            }
};

#endif
