#ifndef __BASIC_STATUS_LINE_H__
#define __BASIC_STATUS_LINE_H__
#include <string>

#include "buffer.h"
#include "json_fwd.h"
template < typename impl >
class basic_request_line : public Serializable{
    impl * impl_;
    using buffer_policy = impl::buffer_policy;
    public:
        basic_request_line() : impl_(new impl){ }
        basic_request_line(std::string line) : impl_(new impl(line)) {} 

        bool process(buffer_policy * buffer){
            return impl_->process(buffer);
        }
        bool continue_reading(){
            return impl_->continue_reading();
        }

        std::string & method(){
            return impl_->method();
        }
        std::string & uri(){
            return impl_->uri();
        }
        std::string & version(){
            return impl_->version();
        }
        std::string & line(){
            return impl_->line();
        }
        serialize_type serialize() override{
            return impl_->serialize();
        }
};

template<typename buffer_type = buffer_v1 >
class requestLine_1_0 : public Serializable{
    std::string method_;
    std::string uri_;
    std::string version_;
    std::string line_;
    bool processed_;
    public:
        typedef buffer_type buffer_policy;
        requestLine_1_0() : processed_(false){}
        requestLine_1_0(std::string line){}
        bool process(buffer_type * buffer){
            if(strchr( (buffer->data + buffer->head) , '\n' ) == NULL)return true;
            do
            {
                while(buffer->head < buffer->tail && *(buffer->data + buffer->head) != ' '){
                    method_ += *(buffer->data + buffer->head);
                    buffer->head++;
                }
                assert(buffer->head < buffer->tail && *(buffer->data + buffer->head) == ' ');
                while(buffer->head < buffer->tail && *(buffer->data + buffer->head) != ' '){
                    uri_ += *(buffer->data + buffer->head);
                    buffer->head++;
                }
                assert(buffer->head < buffer->tail && *(buffer->data + buffer->head) == ' ');
                while(buffer->head < buffer->tail && *(buffer->data + buffer->head) != '\r'){
                    uri_ += *(buffer->data + buffer->head);
                    buffer->head++;
                }
                assert(buffer->head < buffer->tail && *(buffer->data + buffer->head) == '\r');

            }while(0);
            assert(*(buffer->data + buffer->head++) == '\r' && *(buffer->data + buffer->head++) == '\n');
            processed_ = true;
            line_ = method_ + ' ' + uri_ + ' ' + version_;
            return false;
        }
        bool continue_reading(){
            /*
             * continue passing more data if not processed 
             */
            return processed_ ? false : true;
        }
        std::string & method(){
            return method_;
        }
        std::string & uri(){
            return uri_;
        }
        std::string & version(){
            return version_;
        }

        std::string & line(){
            return line_;
        }

        serialize_type serialize() override {
            return line_;
        }
};

template< typename impl>
class basic_status_line : public Serializable{
    impl * impl_;
    public:
        using buffer_type = impl::buffer_policy;
        basic_status_line() : impl_(new impl()) {}
        basic_status_line(int status) : impl_(new impl(status)) {}

        serialize_type serialize() override{
            return impl_->serialize();
        }

        std::string & version(){
            return impl_->version();
        }
        int & status(){
             return impl_->status();
        }
        std::string & message(){
            return impl_->message();
        }

        bool process(buffer_type * buffer){
            return impl_->process(buffer);
        }
};

template<typename buffer_type = buffer_v1>
class statusLine_1_0 : public Serializable{
    bool processed_;
    std::string status_code_;
    std::string version_;
    std::string status_message_;
    int status_;
    const enum{
        HTTP_OK = 200,
        SERVICE_UNAVAILABLE = 503 ,
        BAD_REQUEST = 400
    } status_codes;
    public:
    using buffer_policy = buffer_type;
    statusLine_1_0(){
        version_ = "HTTP/1.1";
        status_code_ = "200";
        status_message_ = "OK";
    }
    statusLine_1_0(int status_code)  {
        version_ = "HTTP/1.1";
            switch(status_code){
                case HTTP_OK:
                    status_code_ = "200";
                    status_message_ = "OK";
                    break;
                case SERVICE_UNAVAILABLE:
                    status_code_ = "503";
                    status_message_ = "SERVICE UNAVAILABLE";
                    break;
                case BAD_REQUEST:
                    status_code_ = "400";
                    status_message_ = "BAD REQUEST";
                    break;
            }
            version_ = "HTTP/1.1";
    }
    serialize_type serialize() override{
        std::string line_ = version_ + " " + status_code_ + " " + status_message_;
        return line_;
    }

    std::string & version(){
        return version_;
    }

};

using http_1_0_status_line  = basic_status_line<statusLine_1_0<>>;
using http_1_0_request_line = basic_request_line<requestLine_1_0 <> >;

#endif
