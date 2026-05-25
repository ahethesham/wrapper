#include "http_request_v1.h"
#include "http_formatter_v1.h"
#include "json_object_v1.h"
#include "http_headers_v1.h"
#include "http_request_line_v1.h"
#include "buffer.h"
#include "basic_logger_interface.h"
/*
 * actual implementation ... this shall be isolated to the .cc file only
 */
class http_request_v1::impl{
    using callback = std::function<bool(buffer_type * buffer )>;
    public:

        impl() : headers_(std::make_shared<http_headers_v1>()) , request_line_(std::make_shared<http_request_line_v1>()) , body_(std::make_shared<json_object_v1>()) , buffer_(*new http_request_v1::buffer_type()){
            cb_ = [this](buffer_type * buffer){
                return parse_request_line(buffer);
            };
        }


        impl(basic_http_request_line_interface & req_line , basic_http_headers_interface & headers , basic_object_interface & body) : headers_(headers.clone()) , request_line_(req_line.clone()) , body_(body.clone()) , buffer_(* new http_request_v1::buffer_type()){
            cb_ = [this](buffer_type * buffer){
                return parse_request_line(buffer);
            };
        }
        impl &set_headers(basic_http_headers_interface & headers){
            headers_ = headers.clone();
            return *this;
        }

        impl &set_request_line(basic_http_request_line_interface & line){
            request_line_ = line.clone();
            return *this;
        }

        impl &set_body(basic_object_interface  & body){
            // will return a pointer to the copy object  of the body 
            body_ = body.clone();
            return *this;
        }

        // set methods
        impl & set_header(std::string & key , std::string &value){
            headers_->set(key  , value);
            return *this;
        }
        impl & set_uri(std::string & uri){
            request_line_->set_uri(uri);
            return *this;
        }
        impl & set_query_param(std::string & key , std::string & value) {
            request_line_->set_query_param(key , value);
            return *this;
        }
        impl & set_method(  std::string & method) {
            request_line_->set_method(method);
            return *this;
        }

        // get methods
        std::string & get_query_param(  std::string & key) {
           return request_line_->get_query_param(key);
        }
        basic_http_headers_interface * headers() {
            return headers_.get();
        }
        basic_http_request_line_interface * request_line() {
            return request_line_.get();
        }
        std::shared_ptr<basic_object_interface>  body() {
            return body_;
        }
        std::string & method(){
           return request_line_->get_method();
        }
        std::string & uri(){
            return request_line_->get_uri();
        }
        std::string & version(){
            return request_line_->get_version();
        }
        std::string & param(std::string key){
            return request_line_->get_query_param(key);
        }
        const std::string & get_header(std::string key){
            return headers_->get(key);
        }

        //parse methods
        bool continue_reading(){
            return cb_(nullptr);
            return true;
        }
        void at_eof(buffer_type * buffer){
            cb_(buffer);
            return ;
        }
        void parse(buffer_type * buffer) {
            cb_(buffer);
            return ;
        }
        http_request_v1::buffer_type * buffer() {
            memset(buffer_.data , 0 , buffer_.tail);
            buffer_.head = buffer_.tail = 0;
            std::string  ser = serialize();
            memcpy(buffer_.data , ser.c_str() , ser.size());
            buffer_.data[ser.size()] = '\0';
            buffer_.tail = buffer_.remainingBytes  = ser.size();
            return &buffer_;
        }
        std::string  serialize(basic_formatter_interface & formatter){
            std::string res;
            res += request_line_->serialize(formatter);
            res += headers_->serialize(formatter);
            res += body_->serialize(formatter);
            return res;
        }
        std::string serialize(){
            std::string res;
            res += request_line_->serialize();
            res += headers_->serialize();
            res += body_->serialize();
            return res;
        }

    private:
        bool parse_request_line(buffer_type * buffer){
            if(!buffer)return true;

            request_line_->parse(buffer);
            if(!request_line_->continue_reading()){
                cb_ = [this](buffer_type * buffer){
                    return parse_headers(buffer);
                };
                return cb_(buffer);
            }
            return true;
        }
        bool parse_headers(buffer_type * buffer){
            if(!buffer)return true;

            headers_->parse(buffer);
            if(!headers_->continue_reading()){
                assert(memcmp((buffer->data + buffer->head) , "\r\n" , 2) == 0);
                buffer->head += 2;
                assert(*(buffer->data + buffer->head) == '{');
                cb_ = [this](buffer_type * buffer){
                    return parse_body(buffer);
                };
                return cb_(buffer);
            }
            return true;
        }
        bool parse_body(buffer_type * buffer){
            if(!buffer)return true;

            body_->parse(buffer);
            if(!body_->continue_reading()){
                cb_ = [this](buffer_type * buffer){
                    return false;
                };
                return cb_(buffer);
            }
            return true;
        }

        std::shared_ptr<basic_http_headers_interface>  headers_;
        std::shared_ptr<basic_http_request_line_interface>   request_line_;
        std::shared_ptr<basic_object_interface>  body_;
        http_request_v1::buffer_type & buffer_;
        callback cb_;
};

http_request_v1::http_request_v1() 
    : impl_(new impl()){}

// copy constrcutor
http_request_v1::http_request_v1(http_request_v1 &req) 
    : impl_(new impl(*req.request_line() , *req.headers() , *req.body())) {}

http_request_v1::http_request_v1(http_request_v1 && req)
    : impl_(req.impl_){
        req.impl_ = nullptr;
}

http_request_v1::self_type & http_request_v1::operator=(http_request_v1 & req){
    if(impl_ != nullptr)
        delete impl_;
    impl_ = (new impl(*req.request_line() , *req.headers() , *req.body()));
    return *this;
}
http_request_v1::self_type & http_request_v1::operator=(http_request_v1 && req){
    if(impl_ != nullptr)
        delete impl_;
    impl_ = req.impl_;
    req.impl_ = nullptr;
    return *this;
}

http_request_v1::http_request_v1(basic_http_request_line_interface & request_line ,
        basic_http_headers_interface & headers ,
        basic_object_interface & body) : impl_(new impl(request_line , headers , body)){}

http_request_v1::self_type & http_request_v1::set_headers(basic_http_headers_interface & headers){
    impl_->set_headers(headers);
    return *this;
}

http_request_v1::self_type & http_request_v1::set_request_line(basic_http_request_line_interface & line){
    impl_->set_request_line(line);
    return *this;
}

http_request_v1::self_type & http_request_v1::set_body(basic_object_interface & body){
    // shall we implement a std::move() ?? 

    impl_->set_body(body);
    return *this;
}

basic_http_request_interface & http_request_v1::set_header(std::string key , std::string value) {
     impl_->set_header(key , value);
     return *this;
}
basic_http_request_interface & http_request_v1::set_uri(std::string uri) {
    impl_->set_uri(uri);
    return *this;
}
basic_http_request_interface & http_request_v1::set_query_param(std::string key , std::string value) {
    impl_->set_query_param(key , value);
    return *this;
}
basic_http_request_interface & http_request_v1::set_method(std::string method){
    impl_->set_method(method);
    return  *this;
}

/***********************************************************/
/*                GET methods                               /
 ***********************************************************/
std::string & http_request_v1::get_query_param(std::string key){
    return impl_->get_query_param(key);
}
basic_http_headers_interface * http_request_v1::headers() {
    return impl_->headers();
}
basic_http_request_line_interface * http_request_v1::request_line() {
    return impl_->request_line();
}
basic_object_interface * http_request_v1::body() {
    return impl_->body().get();
}
std::string & http_request_v1::method(){
    return impl_->method();
}
std::string & http_request_v1::uri(){
    return impl_->uri();
}
std::string & http_request_v1::version(){
    return impl_->version();
}
std::string & http_request_v1::param(std::string key){
    return impl_->param(key);
}
const std::string & http_request_v1::operator[](std::string key){
    return impl_->get_header(key);
}
const std::string & http_request_v1::get_header(std::string key){
    return impl_->get_header(key);
}



/*************************************************/
/*         Parser methods                         /
 *************************************************/
bool http_request_v1::continue_reading(){
    return impl_->continue_reading();
}
void http_request_v1::at_eof(http_request_v1::buffer_type * buffer){
    impl_->at_eof(buffer);
    return;
}
void http_request_v1::parse(http_request_v1::buffer_type * buffer ){
    impl_->parse(buffer);
    return ;
}
http_request_v1::buffer_type * http_request_v1::buffer(){
    return impl_->buffer();
}
std::string http_request_v1::serialize(basic_formatter_interface & formatter){
    return impl_->serialize(formatter);
}
std::string http_request_v1::serialize(){
    return impl_->serialize();
}


std::shared_ptr<basic_http_request_interface> http_request_v1::clone(){
    return std::make_shared<http_request_v1>(*this);
}


