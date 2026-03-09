#ifndef __BASIC_HTTP_REQUEST_H__
#define __BASIC_HTTP_REQUEST_H__

#include "buffer.h"
#include "web_fwd.h"
#include "basic_status_line.h"
#include "basic_headers.h"

template< typename status_line_policy = http_1_0_request_line ,
          typename header_policy =  http_1_0_headers,
          typename body_policy = jsonObject>
class basic_http_request : public Serializable {

   status_line_policy * status_line_;
   header_policy      * header_;
   body_policy        * body_;
   
    public:
        using buffer_type = buffer_v1;
        using callback = std::function<bool(buffer_type *)>;
        using return_type = body_policy *;
        callback currentStatus_;

        serialize_type serialize(){
            serialize_type req = "";
            req += status_line_->serialize();
            req += "\r\n";
            req += header_->serialize();
            req += "\r\n";
            req += body_->serialize();
            return req;
        }
        basic_http_request() : currentStatus_(process_statusLine) ,
                                status_line_(new status_line_policy()) ,
                                header_(new header_policy()) ,
                                body_(new body_policy()){ }

        bool process_statusLine(buffer_type *buffer){
            if(buffer == nullptr)return true;

            if(strchr((buffer->data + buffer->head ), '\n') != NULL){
                // we have a line to process 
                status_line_->process(buffer);
                if(!status_line_->continue_reading()){
                    currentStatus_ = this->process_headers;
                    return currentStatus_(buffer);
                }
            }
            return true;
        }
        bool process_headers(buffer_type * buffer){
            if(buffer == nullptr)return true;

            if(strchr(buffer->data + buffer->head , '\n') != NULL){
                header_->process(buffer , status_line_);
                if(!header_->continue_reading()){
                    currentStatus_ = this->process_body();
                    return currentStatus_(buffer);
                }
            }
            return true;
        }
        bool process_body(buffer_type * buffer){
            if(buffer == nullptr)return true;
            
            body_->process(buffer , status_line_ , header_);
            if(!body_->continue_reading()){
                currentStatus_ = [](buffer_type * buffer)->bool{
                    return false;
                };
            }
            return true;
        }

        bool continue_reading(){
            return currentStatus_(nullptr);
        }
        void parse(buffer_type * buffer ){
            currentStatus_(buffer);
        }

        return_type &  get(){
            return  *body_;
        }

        header_policy & header(){
            return * header_;
        }

        status_line_policy & headerLine(){
            return * status_line_;
        }


};

using http_1_0_request_parser = basic_http_request<>;
#endif
