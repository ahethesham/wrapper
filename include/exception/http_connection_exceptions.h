#ifndef __HTTP_CONNECTION_EXCEPTIONS_H__
#define __HTTP_CONNECTION_EXCEPTIONS_H__

#include <stdexcept>

class connection_timeout :public std::runtime_error{
    public:
        connection_timeout(const char * fmt , ...);
};

class socket_open_exception : public std::runtime_error{
    public:
        socket_open_exception(const char * fmt , ...);
};

class read_timeout_exception: public std::runtime_error{
    public:
        read_timeout_exception(const char * fmt , ... );
};

class write_timeout_exception: public std::runtime_error{
    public:
        write_timeout_exception(const char * fmt , ... );
};

class socket_closed_exception: public std::runtime_error{
    public:
        socket_closed_exception(const char * fmt , ...);
};
class host_not_found_exception : public std::runtime_error{
    public:
        host_not_found_exception(const char * fmt , ...);
        host_not_found_exception();
};
#endif
