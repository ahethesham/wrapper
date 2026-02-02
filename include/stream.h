#ifndef __STREAM_H__
#define __STREAM_H__
#include <string>
#include <unistd.h>
#include "iterator.h"

class Stream {
    public:
        typedef Iterator<Stream> iterator;
        virtual Stream & operator<<(const char * input) = 0;
        // dump the text into these buffers 
        virtual Stream & operator>>(char * burffer) = 0;
        virtual Stream & operator>>(std::string &buffer) = 0;
        virtual Stream & operator<<(int fd) = 0;
        virtual Stream & operator>>(int fd) = 0;
        virtual Stream & operator>>(void * fd) ;
        virtual Stream & operator<<(void * fd) ;
};

namespace http
{
    //have parsing of string algorithms here 
    class lineStream : public Stream
    {
            char buffer_[2024*2024] = {0};
            int  bufferIdx_;

        public:
            Stream & operator<<(const char * input) override;
            Stream & operator>>( char * input) override;
            Stream & operator>>(std::string &str) override;
            // basically read from the fooking fd per line
            Stream & operator<<(int fd) override;
            Stream & operator>>(int fd) override;
            lineStream();
    };
}
namespace https{

    // we will optimize it later by replacing the linestream with httpstream
    class stream : public Stream
    {
            char buffer_[2024*2024] = {0};
            int  bufferIdx_;

        public:
            Stream & operator<<(const char * input) override;
            Stream & operator>>( char * input) override;
            Stream & operator>>(std::string &str) override;
            // basically read from the fooking fd per line
            Stream & operator<<(int fd) override;
            Stream & operator>>(int fd) override;
            stream();
    };

}

// read the payload from the given fd line by line
Stream & http::lineStream::operator<<(int fd)
{
    char endingBytes[4] = {0};
    char ch;
    while(::read(fd , &ch , sizeof(ch)) == 1){
        buffer_[bufferIdx_++] = ch;
        if(ch == '\n' || ch == '\0'){
            // line is read break now
            break;
        }
    }
    return *this;
}

Stream & http::lineStream::operator>>(int fd){
    // write the entire buffer to the fd
    int size = bufferIdx_;
    int tempIdx = 0;
    int temp;
    while(size > 0){
        temp = ::write(fd , (buffer_ + tempIdx) , size);
        if((temp == 0) || (temp < 0 && errno == EINTR ))continue;
        size -= temp;
        tempIdx += temp;
    }

    return *this;
}

#endif
