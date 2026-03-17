#ifndef __BASIC_HTTP_REQUEST_H__
#define __BASIC_HTTP_REQUEST_H__

#include "http_request_fwd.h"
#include <functional>

template< typename status_line_policy  ,
          typename header_policy ,
          typename body_policy ,
          typename buffer_policy >
class basic_http_request {

   
    public:
        using buffer_type = buffer_v1;
        using callback = std::function<bool(buffer_type *)>;
        using return_type = body_policy *;
        static constexpr char * end_of_file = nullptr;

        callback currentStatus_;

        std::string serialize() {
            std::string  req = "";
            req += status_line_->serialize();
            req += "\r\n";
            req += header_->serialize();
            req += "\r\n";
            req += body_->serialize();
            req += "\r\n";
            return req;
        }

        bool process_statusLine(buffer_type *buffer){
            if(buffer == nullptr)return true;


            if(strchr((buffer->data + buffer->head ) , '\n') != NULL){

                // we have a line to process 
                status_line_->process(buffer);
                if(!status_line_->continue_reading()){
                    currentStatus_ = [this](buffer_type * buff){
                       return process_headers(buff);
                    };
                    return currentStatus_(buffer);
                }
            }
            return true;
        }
        basic_http_request() : status_line_(new status_line_policy()) ,
                                header_(new header_policy()) ,
                                body_(new body_policy())
        { 

            currentStatus_ = [this](buffer_type * buffer){
                return process_statusLine(buffer);
            };
        }

        bool process_body(buffer_type * buffer){
            if(buffer == nullptr)return true;
            body_->process(buffer );
            if(!body_->continue_reading()){
                currentStatus_ = [](buffer_type * buffer)->bool{
                    return false;
                };
                return currentStatus_(buffer);
            }
            return true;
        }

        bool process_headers(buffer_type * buffer){
            if(buffer == nullptr)return true;

            if(strchr(buffer->data + buffer->head , '\n') != NULL){
                header_->process(buffer , status_line_);
                if(!header_->continue_reading()){
                    currentStatus_ = [this](buffer_type * buff){
                        return process_body(buff);
                    };
                    return currentStatus_(buffer);
                }
            }
            return true;
        }

        bool continue_reading(){
            return currentStatus_(nullptr);
        }
        void parse(buffer_type * buffer ){
            if((char *)buffer == nullptr){
                // client has closed the socket have logic to end the parsing 
            }
            currentStatus_(buffer);
        }

        return_type &  get(){
            return  *body_;
        }

        body_policy & body(){
            return *body_;
        }

        header_policy & header(){
            return * header_;
        }

        status_line_policy & headerLine(){
            return *status_line_;
        }

        status_line_policy & requestLine(){
            return this->headerLine();
        }
        buffer_policy * buffer(){
            buffer_ = new buffer_policy();
            std::string res = this->serialize();
            memcpy(buffer_->data , res.c_str() , res.size());
            buffer_->tail = res.size();
            buffer_->remainingBytes = res.size();
            return buffer_;
        }
    private:
       status_line_policy * status_line_;
       header_policy      * header_;
       body_policy        * body_;
       buffer_policy      * buffer_;

};


#endif
