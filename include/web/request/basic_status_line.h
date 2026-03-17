#ifndef __BASIC_STATUS_LINE_H__
#define __BASIC_STATUS_LINE_H__
#include <string>

#include "http_request_fwd.h"

template < typename impl >
class basic_request_line {
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
        std::string serialize() {
            return impl_->serialize();
        }
};

template<typename buffer_type  >
class requestLine_1_0  {
    std::string method_;
    std::string uri_;
    std::string version_;
    std::string line_;
    bool processed_;
    public:
        typedef buffer_type buffer_policy;
        requestLine_1_0() : processed_(false) , method_("GET") , uri_("/") , version_("HTTP/1.0"){}
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
            if(line_.empty()){
                line_ = method_ + " " + uri_ + " " +version_;
            }
            return line_;
        }

        std::string serialize() {
            if(line_.empty()){
                line_ = method_ + " " + uri_ + " " +version_;
            }
            return line_;
        }
};

template< typename impl>
class basic_status_line {
    impl * impl_;
    public:
        using buffer_type = impl::buffer_policy;
        basic_status_line() : impl_(new impl()) {}
        basic_status_line(int status) : impl_(new impl(status)) {}

        std::string serialize() {
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
        bool continue_reading(){
            return impl_->continue_reading();
        }
};

template<typename buffer_type >
class statusLine_1_0 {
    std::string status_code_;
    std::string version_;
    std::string status_message_;
    int status_;
    enum status_codes{
        HTTP_OK = 200,
        SERVICE_UNAVAILABLE = 503 ,
        BAD_REQUEST = 400
    } ;
    using callback = std::function<bool(buffer_type * buffer)>;
    public:
    using buffer_policy = buffer_type;
    callback processor_;
    statusLine_1_0()  {
        version_ = "HTTP/1.0";
        status_code_ = "200";
        status_message_ = "OK";
        processor_ = [this](buffer_type * buffer) -> bool{
            return process_version(buffer);
        };
    }
    statusLine_1_0(int status_code)  {
        version_ = "HTTP/1.0";
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
        processor_ = [this](buffer_type * buffer) -> bool{
            return process_version(buffer);
        };
    }

    bool process_message(buffer_type * buffer){
        if(buffer == nullptr || strstr(buffer->data + buffer->head , "\r\n") == nullptr)return true;

        status_message_.clear();
        while(*(buffer->data + buffer->head) != '\r'){
            status_message_ += *(buffer->data + buffer->head++);
        }
        
        assert(*(buffer->data  + buffer->head++ ) == '\r' && *(buffer->data + buffer->head++) == '\n');

        processor_ = [this](buffer_type * buffer){
            return  false;
        };

        return processor_(buffer);
    }

    bool process_status_code(buffer_type * buffer)
    {
        if(buffer == nullptr || strchr(buffer->data + buffer->head , ' ') == nullptr)return true;

        status_code_.clear();

        while(*(buffer->data + buffer->head) != ' '){
            status_code_ += *(buffer->data + buffer->head++);
        }
        assert(std::atoi(status_code_.c_str()) >= 200 &&
                std::atoi(status_code_.c_str()) <= 600);
        assert(*(buffer->data + buffer->head++) == ' ');
        
        processor_ = [this](buffer_type * buffer){
            return process_message(buffer);
        };

        return processor_(buffer);
    }

    bool process_version(buffer_type * buffer)
    {
        
        if(buffer == nullptr || strchr(buffer->data + buffer->head , ' ') == nullptr)return true;
        version_.clear();
        while(*(buffer->data + buffer->head) != ' '){
            version_ += *(buffer->data + buffer->head++);
        }
        assert(version_ == "HTTP/1.0" && *(buffer->data + buffer->head++) == ' ');
        processor_ = [this](buffer_type * buffer){
            return process_status_code(buffer);
        };
        return processor_(buffer);
    }

    std::string serialize() {
        std::string line_ = version_ + " " + status_code_ + " " + status_message_;
        return line_;
    }

    std::string & version(){
        return version_;
    }
    bool continue_reading(){
        return processor_(nullptr) ;
    }

    bool process(buffer_policy * buffer){
        // parse the buffer and get the status , message and http version
        return processor_(buffer);   
    }

};


#endif
