#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <algorithm>
#include <cassert>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "address.h"

class Socket
{
    public:
        virtual int createSocket(const char * hostname , int port) = 0;
        virtual int bind(Address &);
        virtual int connect();
        virtual int getFd() = 0;
};

namespace tcp{
    class socket : public Socket
    {
        char hostname_[41];
        int port_;
        int fd_;
        public:
            socket(int sockopt){
                assert(fd_ = ::socket(AF_INET , SOCK_STREAM , 0) > 0);
                // TODO set provided socket options accordingly 
            }
            int getFd() override {return fd_;}
            int createSocket(const char * hostname , int port) override;
            int bind(Address &) override;
    };
}
#endif
