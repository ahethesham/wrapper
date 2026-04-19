#ifndef __HTTP_RESPONSE_H__
#define __HTTP_RESPONSE_H__
#include "json_fwd.h"
#include "basic_logger.h"

template < typename status_line_policy ,
           typename header_policy ,
           typename body_policy ,
           typename buffer_policy>
class basic_http_response {
    public:
        using buffer_type = buffer_policy;

        using callback = std::function<bool(buffer_type *)>;
        // basically for http_1_0 requests json build up happens only when clients close the connection
        basic_http_response & at_eof(){
            status_line_->at_eof(); 
            headers_->at_eof(); 
            body_->at_eof(); 
            currentStatus_ = [this](buffer_type * buff)->bool{
                throw std::runtime_error("Why the hell would we call after eof");
                return false;
            };
            return *this;
        }

        bool process_statusLine(buffer_type *buffer){
            if(buffer == nullptr)return true;


            if(strchr((buffer->data + buffer->head ) , '\n') != NULL){

                // we have a line to process 
                status_line_->process(buffer);
                if(!status_line_->continue_reading()){
                    LogStatus(*gLogger);
                    currentStatus_ = [this](buffer_type * buff){
                       return process_headers(buff);
                    };
                    return currentStatus_(buffer);
                }
            }
            return true;
        }
        basic_http_response() : status_line_(new status_line_policy()) ,
                                headers_(new header_policy()) ,
                                body_(new body_policy())
        { 

            currentStatus_ = [this](buffer_type * buffer){
                return process_statusLine(buffer);
            };
        }

        bool process_body(buffer_type * buffer){
            if(buffer == nullptr || buffer->head == buffer->tail)return true;
            
            if(!body_->verify_content_type((*headers_)["Content-Type"])){
                /*
                 * Unexpected response type ....exit from here 
                 */
                gLogger->log("unexpected content type received %s " , (*headers_)["Content-Type"].c_str());
                (*gLogger) << buffer->data << endl;
                return false;
            }

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
            if(buffer == nullptr || buffer->head == buffer->tail)return true;

            if(strchr(buffer->data + buffer->head , '\n') != NULL){
                headers_->process(buffer);
                if(!headers_->continue_reading()){
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
            currentStatus_(buffer);
        }

        body_policy & body(){
            return *body_;
        }

        header_policy & headers(){
            return *headers_;
        }

        status_line_policy & status_line(){
            return *status_line_;
        }
    private:

        void LogStatus( basic_logger_interface & logger ){
           logger << "Response received from client " << endl;
           logger << "Status code = " << status_line_->status() << endl;
           logger << "Message = " << status_line_->message() << endl;
           logger << "Http version = " << status_line_->version() << endl;

           return ;
        }
        callback currentStatus_;
        status_line_policy * status_line_;
        header_policy * headers_;
        body_policy * body_;

};


#endif
