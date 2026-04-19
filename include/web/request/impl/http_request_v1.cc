#include "http_request_v1.h"

/*
 * actual implementation ... this shall be isolated to the .cc file only
 */
class http_request_v1::impl{
    using callback = std::function<bool(const char * data )>;
    public:
        impl &set_headers(basic_http_headers_interface * headers){
            headers_ = headers;
            return *this;
        }

        impl &set_request_line(basic_http_request_line_interface * line){
            request_line_ = line;
            return *this;
        }

        impl &set_body(basic_object_interface * body){
            body_ = body;
            return *this;
        }

        basic_http_headers_interface * headers() {
            return headers_;
        }
        basic_http_request_line_interface * request_line() {
            return request_line_;
        }
        basic_object_interface * body() {
            return body_;
        }

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
        const std::string & get_query_param(  std::string & key) {
            return request_line_->get_query_param(key);
        }

        bool continue_reading(){
            //TODO
            return true;
        }
        void at_eof(buffer_type * buffer){
            //TODO
        }
        void parse(buffer_type * buffer) {
            //TODO   
        }
        http_request_v1::buffer_type * buffer() {
            return  nullptr;
        }
        std::string  serialize(basic_formatter_interface & formatter){
            std::string res;
            res += request_line_->serialize(formatter);
            formatter.line_post_processor(res);
            res += headers_->serialize(formatter);
            formatter.line_post_processor(res);
            formatter.line_post_processor(res);
            res += body_->serialize(formatter);
            return res;
        }

        impl(impl &){}
        impl(impl &&){}
        impl() : headers_(nullptr) , request_line_(nullptr) , body_(nullptr) , buffer_(nullptr){}

    private:

        basic_http_headers_interface * headers_;
        basic_http_request_line_interface *  request_line_;
        basic_object_interface * body_;
        http_request_v1::buffer_type * buffer_;
};

http_request_v1::http_request_v1() : impl_(new impl()){}

http_request_v1::self_type & http_request_v1::set_headers(basic_http_headers_interface & headers){
    impl_->set_headers(&headers);
    return *this;
}

http_request_v1::self_type & http_request_v1::set_request_line(basic_http_request_line_interface & line){
    impl_->set_request_line(&line);
    return *this;
}

http_request_v1::self_type & http_request_v1::set_body(basic_object_interface & body){
    // shall we implement a std::move() ?? 
    impl_->set_body(&body);
    return *this;
}

basic_http_headers_interface * http_request_v1::headers() {
    return impl_->headers();
}

basic_http_request_line_interface * http_request_v1::request_line() {
    return impl_->request_line();
}

basic_object_interface * http_request_v1::body() {
    return impl_->body();
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

const std::string & http_request_v1::get_query_param(std::string key){
    return impl_->get_query_param(key);
}

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
