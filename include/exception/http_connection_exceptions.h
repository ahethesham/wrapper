#ifndef __HTTP_CONNECTION_EXCEPTIONS_H__
#define __HTTP_CONNECTION_EXCEPTIONS_H__

#include <stdexcept>

class connection_timeout :public std::runtime_error{
    public:
        connection_timeout(const char * fmt , ...) : std::runtime_error("connection timed out"){
        }
};

class socket_open_exception : public std::runtime_error{
    public:
        socket_open_exception(const char * fmt , ...) : std::runtime_error("socket open exception"){}
};

class read_timeout_exception: public std::runtime_error{
    public:
        read_timeout_exception(const char * fmt , ... ) : std::runtime_error("connection timedout at read "){};
};

class write_timeout_exception: public std::runtime_error{
    public:
        write_timeout_exception(const char * fmt , ... ) : std::runtime_error("connection timedout at write"){};
};

class socket_closed_exception: public std::runtime_error{
    public:
        socket_closed_exception(const char * fmt , ...) : std::runtime_error("socket closed unexpectedly ") {};
};
class host_not_found_exception : public std::runtime_error{
    public:
        host_not_found_exception(const char * fmt , ...) : std::runtime_error("host not found "){};
        host_not_found_exception() : std::runtime_error("host not found"){};
};
#endif
