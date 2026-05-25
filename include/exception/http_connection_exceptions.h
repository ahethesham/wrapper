#ifndef __HTTP_CONNECTION_EXCEPTIONS_H__
#define __HTTP_CONNECTION_EXCEPTIONS_H__

#include <stdexcept>

class connection_timeout :public std::runtime_error{
    public:
        connection_timeout(const char * fmt , ...) : std::runtime_error("connection timed out"){ }
        const char * what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override{
            return std::runtime_error::what();
        }
};

class socket_open_exception : public std::runtime_error{
    public:
        socket_open_exception(const char * fmt , ...) : std::runtime_error("socket open exception"){}
        const char * what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override{
            return std::runtime_error::what();
        }
};

class read_timeout_exception: public std::runtime_error{
    public:
        read_timeout_exception(const char * fmt , ... ) : std::runtime_error("connection timedout at read "){};
        const char * what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override{
            return std::runtime_error::what();
        }
};

class write_timeout_exception: public std::runtime_error{
    public:
        write_timeout_exception(const char * fmt , ... ) : std::runtime_error("connection timedout at write"){};
        const char * what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override{
            return std::runtime_error::what();
        }
};

class socket_closed_exception: public std::runtime_error{
    public:
        socket_closed_exception(const char * fmt , ...) : std::runtime_error("socket closed unexpectedly ") {};
        const char * what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override{
            return std::runtime_error::what();
        }
};
class host_not_found_exception : public std::runtime_error{
    public:
        host_not_found_exception(const char * fmt , ...) : std::runtime_error("host not found "){};
        host_not_found_exception() : std::runtime_error("host not found"){};
        const char * what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override{
            return std::runtime_error::what();
        }
} ;
class unexpected_file_format : public std::runtime_error{
    public:
        unexpected_file_format(const char * fmt , ...) : std::runtime_error("unexpected file formater identified"){};
        const char * what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override{
            return std::runtime_error::what();
        }
};
#endif
