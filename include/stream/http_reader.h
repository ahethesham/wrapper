#ifndef __HTTP_READER_H__
#define __HTTP_READER_H__

#include <cstring>
#include <unistd.h>
#include "web_fwd.h"


class http_reader {
    typedef enum {
        json ,
        unknown
    }contentType;

    typedef enum{
        requestLine ,
        headers ,
        body
    }currentParsingStatus;

    int  fd_;
    char buffer_[1024*1024];
    http::http_request * req;
    int lineStartIdx_;
    int lineEndingIdx_;
    contentType type_;
    currentParsingStatus currentParsingStatus_;
    int  contentLength_;
    int currentStatus_;

    public:
        typedef size_t size_type;
        typedef http::http_request & custom_protocol;
        
        http_reader(int fd ) : fd_(fd) {
            ::memset(buffer_ , 0 , 1024 * 1024 );
            type_ = contentType::unknown;
            currentParsingStatus_ = currentParsingStatus::requestLine;
            lineStartIdx_ = lineEndingIdx_ =  0;
            req  =  new http::http_request();
        }

        custom_protocol  read(){
            int rc;
            size_type bufferIdx = 0;
            size_type totalBytesRead = 0;
            size_type remainingBytes = 1024 * 1024;
            lineStartIdx_ = lineEndingIdx_ = 0;
            size_type previousIdx = 0;

            do{
                rc = ::read(fd_ , (void *)(buffer_ + bufferIdx) , remainingBytes);
                log("read %d bytes" , rc);
                if(rc > 0){
                    bufferIdx += rc;    
                //    process(previousIdx , bufferIdx);
                    remainingBytes -= rc;
                    log("Read \n %s" , buffer_);
                }
            }while(remainingBytes > 0 &&  (rc > 0 || (rc < 0 && errno == EINTR)));

            return *req;
        }
        void reset(){
            ::memset(buffer_ , 0 , 1024 * 1024 );
            type_ = contentType::unknown;
            currentParsingStatus_ = currentParsingStatus::requestLine;
            lineStartIdx_ = lineEndingIdx_ =  0;
            if(req)delete req;
            req  =  new http::http_request();
        }

    private:
        void process(size_type & previousIdx , size_type & currentIdx){

            if(previousIdx > 0 && buffer_[previousIdx - 1] == '\r' &&
                    buffer_[previousIdx] != '\n')
                throw std::runtime_error("unexpected json");

            do{
               switch(currentParsingStatus_){ 
                   case headers:
                            while(previousIdx < currentIdx &&
                                   ( buffer_[previousIdx] == '\r' || 
                                     buffer_[previousIdx] == '\n'))previousIdx++;
                            while(previousIdx < currentIdx &&
                                  buffer_[previousIdx] != '\r'){
                                // TODO : add logic to check for invalid characters
                                previousIdx ++;
                            }
                            lineStartIdx_ = previousIdx;
                            while(lineStartIdx_ < currentIdx && buffer_[lineStartIdx_] == '\r' || buffer_[lineStartIdx_] == '\n')lineStartIdx_++;
                            if(previousIdx < currentIdx){
                                lineEndingIdx_ = previousIdx;
                                processHeaders();
                            }
                        break;
                   case body:
                        if(currentIdx - previousIdx >= contentLength_){
                            // generate request body
                            processBody(previousIdx , currentIdx);
                        }
                        return ;
                   case requestLine:
                            processRequestLine(previousIdx , currentIdx);
                        break;

               }
            }while(previousIdx < currentIdx);
        }
        void processHeaders() {
            std::string key = "";
            std::string value = "";

            //assert(lineEndingIdx_ > lineStartIdx_);
            do{
                if(buffer_[lineStartIdx_] == ' ')continue;
                if(buffer_[lineStartIdx_] != '\"')throw std::runtime_error("unexpected key value pair");
                while(++lineStartIdx_ < lineEndingIdx_ && buffer_[lineStartIdx_] != '\"'){
                    key += buffer_[lineStartIdx_];
                }
                if(lineStartIdx_ == lineEndingIdx_  )throw std::runtime_error("unexpected key value pair");
                assert(buffer_[lineStartIdx_] == ':' && (lineStartIdx_ + 1 ) != lineEndingIdx_ && buffer_[lineStartIdx_ + 1] == ' ');
                
                lineStartIdx_ += 2;
                // remove the value
                
            }while(0);
            req->addHeader(key , value);
            if(key == "Content-Type"){
                if(value != "application/json")throw std::runtime_error("unexpected application format ");
                type_ = contentType::json;

            }else if(key == "Content-Length"){
                contentLength_ = std::atoi(value.c_str());
            }
            return ;
        }
        void processRequestLine(size_type & previousIdx , size_type & currentIdx){
            // forward the string as is to the req
            log("process %s start " , __func__);
            static std::string line = "";
            static bool foundEnd = false;
            do{
                line += buffer_[previousIdx++];
            }while(previousIdx < currentIdx  && buffer_[previousIdx] != '\r');
            if(buffer_[previousIdx] == '\r')foundEnd = true;
            if(foundEnd){
                req->addRequestLine(line);
                currentParsingStatus_ = currentParsingStatus::headers;
                line = "";
                foundEnd = false;
            }
            log("process %s end " , __func__);
            return ;
        }
        void processBody(size_type & previousIdx , size_type & currentIdx){
            buffer_[currentIdx] = '\0';
            req->addBody((buffer_ + previousIdx) );
        }

    
};

#endif
