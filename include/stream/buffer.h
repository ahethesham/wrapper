#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <cstdint>
#include <string.h>

// remember this is not thread safe
struct buffer_v1{
    char data[1024 * 1024] ;
    uint64_t head;
    uint64_t tail;
    buffer_v1() {
        memset(data , 0 , sizeof(data));
        head = tail = 0;
    }
};

#endif
