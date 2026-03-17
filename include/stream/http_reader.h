#ifndef __HTTP_READER_H__
#define __HTTP_READER_H__

#include <cstring>
#include <string.h>
#include <unistd.h>
#include "web_fwd.h"
#include "logger.h"

inline ssize_t std_reader(int fd , void * buffer , ssize_t size){
        return ::read(fd , buffer , size);
}
inline ssize_t ssl_reader(SSL * fd , void * buffer , ssize_t size){
        return ::SSL_read(fd , buffer , size);
}

#if 0
namespace {
#define requestLine 0
#define headers     1
#define body        2
#define completed   3

class parsing_status{
    public:
    private:
        short currentParsingStatus_;
    public:
        void setStatus(short status = requestLine){
            currentParsingStatus_ = requestLine;
        }
        parsing_status(int status = requestLine) : currentParsingStatus_(status){}
        short getStatus(){
            return currentParsingStatus_;
        }
};
}


template <typename custom_http_protocol , // could be parsed as request for servers and response from clients
          typename fd_type ,
          auto reader >
class http_reader {
    typedef enum {
        json ,
        unknown
    }contentType;


    fd_type  fd_;
    char buffer_[1024*1024];
    custom_http_protocol * req;
    int lineStartIdx_;
    int lineEndingIdx_;
    contentType type_;
    int  contentLength_;

    parsing_status status_;

    public:
        typedef size_t size_type;
        typedef custom_http_protocol custom_protocol;
        
        http_reader(fd_type fd ) : fd_(fd) {
            ::memset(buffer_ , 0 , 1024 * 1024 );
            type_ = contentType::unknown;
            lineStartIdx_ = lineEndingIdx_ =  0;
            contentLength_ = 0;
            req  =  new custom_http_protocol();
        }

        custom_protocol  read(){
            int rc;
            size_type bufferIdx = 0;
            size_type totalBytesRead = 0;
            size_type remainingBytes = 1024 * 1024;
            lineStartIdx_ = lineEndingIdx_ = 0;
            size_type previousIdx = 0;

            do{
                rc = reader(fd_ , (void *)(buffer_ + bufferIdx) , remainingBytes);
                if(rc > 0){
                    bufferIdx += rc;    
                    try{
                        process(previousIdx , bufferIdx);
                    }catch(std::exception &e){
                        log("What has happened %s " , e.what());
                        exit(1);
                    }
                    remainingBytes -= rc;
                    if(status_.getStatus() == completed)break;
                }
            }while(remainingBytes > 0 &&  (rc > 0 || (rc < 0 && errno == EINTR)));
            log("completed reading request");
            return *req;
        }
        void reset(){
            ::memset(buffer_ , 0 , 1024 * 1024 );
            type_ = contentType::unknown;
            status_.setStatus(requestLine) ;
            lineStartIdx_ = lineEndingIdx_ =  0;
            if(req)delete req;
            req  =  new custom_http_protocol();
        }

    private:
        void process(size_type & previousIdx , size_type & currentIdx){

            do{
               switch(status_.getStatus()){ 
                   case headers:
                            processHeaders(previousIdx , currentIdx);
                        break;
                   case body:
                        if(currentIdx - previousIdx >= contentLength_){
                            // generate request body
                            processBody(previousIdx , currentIdx);
                            status_.setStatus(completed);
                            return ;
                        }else{
                            log("Body not fully received yet reading more \n");
                        }
                        break ;
                   case requestLine:
                            processRequestLine(previousIdx , currentIdx);
                            break;
                   case completed:
                        break;
               }
            }while(previousIdx < currentIdx);
            if(status_.getStatus() == body && contentLength_ == 0){
                // no json body only the headers 
                status_.setStatus( completed);
            }
        }
        void processHeaders(size_type & previousIdx , size_type & currentIdx) {

            static std::string key = "";
            static std::string value = "";
            static int currentKeyValueStatus = 0;

            if(currentIdx - previousIdx >= 2 && strncmp((buffer_ + previousIdx) , "\r\n" , 2) == 0 ){
                log("Setting the status to read body ");
                status_.setStatus(body);
                previousIdx += 2;
                return ;
            }

            do{
                if(currentKeyValueStatus == 0 || currentKeyValueStatus == 1){
                    while(previousIdx < currentIdx && buffer_[previousIdx] != ':'){
                        key += buffer_[previousIdx++];
                    }
                    if(previousIdx == currentIdx){
                        currentKeyValueStatus = 1;
                        break;
                    }else{
                        // extract the value;
                        currentKeyValueStatus = 2;
                        if( previousIdx + 1 < currentIdx)
                            assert(buffer_[++previousIdx] == ' ');
                        else
                            break;
                    }
                }if(currentKeyValueStatus == 2 && previousIdx < currentIdx){
                    currentKeyValueStatus = 3;
                }if(currentKeyValueStatus == 3){
                    while(++previousIdx < currentIdx && buffer_[previousIdx] != '\r')
                        value += buffer_[previousIdx];
                    if(previousIdx < currentIdx){
                        req->addHeader(key , value);
                        if(key == "Content-Type"){
                            if(value != "application/json")throw std::runtime_error("unexpected application format ");
                            type_ = contentType::json;
                        }else if(key == "Content-Length"){
                            contentLength_ = std::atoi(value.c_str());
                        }
                        key = value = "";
                        currentKeyValueStatus = 4;
                    }
                }if(currentKeyValueStatus == 4){
                    // continue untill we reach the end of the line
                    while(previousIdx < currentIdx && buffer_[previousIdx++] != '\r');
                    if(previousIdx > 0 && buffer_[previousIdx - 1] == '\r'){
                        if(previousIdx < currentIdx ){
                            assert(buffer_[previousIdx++] == '\n');
                            currentKeyValueStatus = 0;
                        }
                    }
                }
            }while(0);
            return ;
        }
        void processRequestLine(size_type & previousIdx , size_type & currentIdx){
            // forward the string as is to the req
            static std::string line = "";
            static bool foundEnd = false;
            do{
                line += buffer_[previousIdx++];
            }while(previousIdx < currentIdx  && buffer_[previousIdx] != '\r');
            if(buffer_[previousIdx] == '\r'){
                if(previousIdx + 1 < currentIdx)
                    assert(buffer_[++previousIdx] == '\n');
                previousIdx++;
                foundEnd = true;
            }
            if(foundEnd){
                req->addStatusLine(line);
                status_.setStatus(headers);
                line = "";
                foundEnd = false;
            }
            return ;
        }
        void processBody(size_type & previousIdx , size_type & currentIdx){
            buffer_[currentIdx] = '\0';
            req->addBody((buffer_ + previousIdx) );
        }

    
};


#else

template < typename socket_policy  , 
           typename buffer_policy   ,
           auto reader >
class basic_reader{
    public:
        using buffer_type = buffer_policy;
        using socket_type = socket_policy;

        basic_reader(socket_type::fd_type fd) : fd_(fd)  ,  buffer_(new buffer_policy()) {}
        buffer_type * read(){
            int rc = reader(fd_ , (buffer_->data + buffer_->tail) , 64 * 1024);
            if(rc > 0){
                buffer_->tail += rc;
                buffer_->remainingBytes += rc;
            }else {
                return nullptr;
            }
            return buffer_;
        }
    private:
        buffer_type * buffer_;
        socket_type::fd_type fd_;

};

using http_1_0_reader = basic_reader<tcp_socket , buffer_v1 , std_reader>;
using https_1_0_reader = basic_reader<tcp_ssl_socket , buffer_v1 , ssl_reader>;
#endif
#endif
