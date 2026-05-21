#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <cstdint>
#include <string.h>

// remember this is not thread safe
struct buffer_v1{
    char data[1024 * 1024] ;
    uint64_t head;
    uint64_t tail;
    uint64_t remainingBytes;
    buffer_v1() {
        memset(data , 0 , sizeof(data));
        head = tail = remainingBytes = 0 ;
    }
    char * get_data();
    int &  get_tail();
    int &  get_head();
    bool continue_reading(){return false;}
    buffer_v1 & parse(const char * data) {return * this;}
    buffer_v1 & parse(buffer_v1 * buffer){return *this;}
    buffer_v1 & at_eof(){return * this;}
    buffer_v1 * buffer(){return this;}
};

#endif
