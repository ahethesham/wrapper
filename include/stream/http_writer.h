#ifndef __HTTP_WRITER_H__
#define __HTTP_WRITER_H__

#include "web_fwd.h"
#include <cstddef>
#include <cstring>
#include <unistd.h>
#include "openssl/ssl.h"

inline ssize_t std_writer(int fd, void * buffer, ssize_t size){
    return ::write(fd , buffer , size);

}

inline ssize_t ssl_writer(SSL * ssl , void * buffer , ssize_t size){
    log("Ahethesham writing data %s" , buffer);
    return ::SSL_write(ssl , buffer , size);
}

template <typename custom_http_procotol ,
         typename fd_type ,
         auto writer>
class http_writer{
    fd_type fd_;
    public:
        typedef size_t size_type;
        typedef custom_http_procotol custom_protocol;

        http_writer(fd_type fd) : fd_(fd) {}

        size_t write(const char * payload){
            size_t inputSize = strlen(payload);
            return write(payload , inputSize);
        }

        size_type write(const char * payload , int size){
            log("writing response %s" , payload);
            size_t inputSize = size;
            int rc ;
            size_t remainingBytes = inputSize;
            size_t bufferIdx = 0;
            do{
                rc = writer(fd_ , (void *)(payload + bufferIdx) , remainingBytes);
                if(rc > 0){
                    remainingBytes -= rc;
                    bufferIdx += rc;
                }
            }while(bufferIdx < inputSize);

            return bufferIdx;
        }

        size_type write(custom_protocol & res){
            return this->write(res.serialize().c_str());
        } 

};

#endif
