#ifndef __HTTP_WRITER_H__
#define __HTTP_WRITER_H__

#include "web_fwd.h"
#include "stream_fwd.h"
#include <cstddef>
#include <cstring>
#include <unistd.h>
#include "openssl/ssl.h"

inline ssize_t std_writer(int fd, void * buffer, ssize_t size){
    return ::write(fd , buffer , size);

}

inline ssize_t ssl_writer(SSL * ssl , void * buffer , ssize_t size){
    return ::SSL_write(ssl , buffer , size);
}

#if 0 
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

#else

template < typename impl>
class basic_http_writer {
    impl * impl_;

    public:
    
    basic_http_writer(impl::socket_type::fd_type fd) :
        impl_(new impl(fd)){}
    impl::return_type write(impl::buffer_type * buffer){
         return impl_->write(buffer);   
    }

};

template<typename socket_policy  ,
         typename buffer_policy  ,
         auto writer >
class http_1_0_writer_impl {
    public:
        typedef socket_policy socket_type;
        typedef socket_policy::fd_type fd_type;
        typedef buffer_policy buffer_type;
        typedef ssize_t return_type;

        http_1_0_writer_impl(fd_type fd) : fd_(fd){}
        ssize_t write( buffer_type * buffer){
            ssize_t total = 0;
            do{
                int rc = writer(fd_ , (buffer->data + total) , buffer->remainingBytes - total);
                if(rc > 0){
                    buffer->head += rc;
                    total += rc;
                }else{
                    throw std::runtime_error("ERR_EVNT_SERVER_0000001");
                }
            }while(total < buffer->remainingBytes );
            log("returning from the core writer ");
            return total;
        }

        ssize_t write(std::string & data){
            ssize_t size = data.length();
            const char * pdata = data.c_str();
            int idx = 0;
            int remainingBytes = size;
            do{
                int rc = writer(fd_ , (pdata + idx) , remainingBytes);
                if(rc > 0){
                    remainingBytes -= rc;
                }else if(rc == 0){
                    throw std::runtime_error("client has closed connection");
                }
            }while(remainingBytes > 0);

            return size;
        }

        ssize_t write( const char * data , ssize_t size){
            ssize_t total = 0;
            do{
                int rc = writer(fd_ , data , size);
                if(rc > 0){
                    size -= rc;
                    total += rc;
                }
                else if(rc == 0)return 0;
                else if(errno != EINTR)return -1;
            }while(size > 0);
            return total;
        }

    private:
        fd_type fd_;
};

#endif
#endif
