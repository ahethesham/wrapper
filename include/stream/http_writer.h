#ifndef __HTTP_WRITER_H__
#define __HTTP_WRITER_H__

#include <cstddef>
#include <cstring>
#include <unistd.h>
class http_writer{
    int fd_;
    public:
        typedef size_t size_type;
        http_writer(int fd) : fd_(fd) {}

        size_t write(const char * payload){
            size_t inputSize = strlen(payload);
            return write(payload , inputSize);
        }

        size_type write(const char * payload , int size){
            size_t inputSize = size;
            int rc ;
            size_t remainingBytes = inputSize;
            size_t bufferIdx = 0;
            do{
                rc = ::write(fd_ , (void *)(payload + bufferIdx) , remainingBytes);
                if(rc > 0){
                    remainingBytes -= rc;
                    bufferIdx += rc;
                }
            }while(bufferIdx < inputSize);

            return bufferIdx;
        }
};

#endif
