#ifndef __HTTP_WRITER_H__
#define __HTTP_WRITER_H__

#include "web_fwd.h"
#include <cstddef>
#include <cstring>
#include <unistd.h>
class http_writer{
    int fd_;
    public:
        typedef size_t size_type;
        typedef http::http_response & custom_protocol;

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

        size_type write(custom_protocol res){
            return this->write(res.serialize().c_str());
        } 

};

#endif
