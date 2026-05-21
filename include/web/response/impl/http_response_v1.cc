#include "http_response_v1.h"
#include "http_response_line_v1.h"
#include "json_formatter_v1.h"

class http_response_v1::impl{
    using callback = std::function<bool(buffer_type * buffer)>;
    public:
        impl() 
        : headers_(nullptr) , response_line_(nullptr) , body_(nullptr) {
            cb_ = [this](buffer_type * buffer){
                return extract_response_line(buffer);
            };
        }
        impl(std::shared_ptr<http_response_line_v1 > response_line)
            : headers_(nullptr) , response_line_(response_line) , body_(nullptr){
            cb_ = [this](buffer_type * buffer){
                return extract_response_line(buffer);
            };
        }

        void set_status(http_status & status){
            assert(response_line_ != nullptr);
            response_line_->set_status(status);
            return ;
        }
        void set_version(std::string &version){
            assert(response_line_ != nullptr);
            response_line_->set_version(version);
            return ;
        }

        void set_header(std::string key , std::string value){
            assert(headers_ != nullptr);
            headers_->set(key , value);
            return ;
        }
        // injections
        void set_body(std::shared_ptr<basic_object_interface > body){
            body_ = body;
            return ;
        }
        void set_headers(std::shared_ptr<basic_http_headers_interface > headers){
            headers_ = headers;
            return ;
        }
        void set_response_line(std::shared_ptr<basic_http_response_line_interface > response_line){
            response_line_ = response_line;
            return ;
        }
        
        // get methods
        const http_status & status(){
            assert(response_line_ != nullptr);
            return response_line_->status();
        }
        std::string & version(){
            assert(response_line_ != nullptr);
            return response_line_->version();
        }
        const std::string & header(std::string & key){
            assert(headers_ != nullptr);
            return headers_->get(key);
        }
        std::shared_ptr<basic_object_interface > body(){
            assert(body_ != nullptr);
            return body_;
        }
        std::shared_ptr<basic_http_headers_interface> headers(){
            return headers_;
        }
        std::shared_ptr<basic_http_response_line_interface> response_line(){
            return response_line_;
        }

        //parse methods
        void parse(buffer_type * buffer){
            cb_(buffer);
            return ;
        }
        bool continue_reading(){
            return cb_(nullptr);
        }
        buffer_type * buffer(){
            memset(buffer_->data , 0 , buffer_->tail);
            buffer_->head = buffer_->tail = 0;
            json_formatter_v1 formatter;
            std::string  ser = serialize(formatter);
            memcpy(buffer_->data , ser.c_str() , ser.size());
            buffer_->data[ser.size()] = '\0';
            buffer_->tail = ser.size();
            return buffer_.get();
        }
        void at_eof(buffer_type * buffer){
            cb_(buffer);
            return ;
        }

        //serialize and deserialize
        std::string serialize(){
            std::string res = "";
            res += response_line_->serialize();
            res += headers_->serialize();
            res += body_->serialize();
            return res;
        }
        std::string serialize(basic_formatter_interface & formatter){
            std::string res = "";
            LOG_DEBUG << "triggered serialiaze in " << __func__ << endl;
            res += response_line_->serialize(formatter);
            LOG_DEBUG << "response line serialized " << endl;
            res += headers_->serialize(formatter);
            LOG_DEBUG << "headers serialized " << endl;
            res += body_->serialize(formatter);
            LOG_DEBUG << "body serialized " << endl;
            return res;
        }
        void clear(){
            response_line_->clear();
            headers_->clear();
            body_->clear();
            return ;
        }

    private:
        bool extract_response_line(buffer_type * buffer){
            LOG_DEBUG << "extracting response line " << endl ;
            response_line_->parse(buffer);
            if(!response_line_->continue_reading()){
                // done with extracting reponse line
                LOG_DEBUG << "done extracting response line " << response_line_->serialize() << endl;
                cb_ = [this](buffer_type * buffer){
                    return extract_headers(buffer);
                };
                return cb_(buffer);
            }
            return true;
        }
        bool extract_headers(buffer_type * buffer){
            LOG_DEBUG << "extracting headers " << endl ;
            headers_->parse(buffer);
            if(!headers_->continue_reading()){
                // done with extracting headers
                LOG_DEBUG << "done extracting headers " << headers_->serialize() << endl;
                cb_ = [this ](buffer_type * buffer){
                    return extract_body(buffer);
                };
                return cb_(buffer);
            }
            return true;
        }
        bool extract_body(buffer_type * buffer){
            LOG_DEBUG << "extracting body " << endl ;
            if(response_line_->version() == "HTTP/1.1")
            {
                int size =  std::atoi(headers_->get("Content-Length").c_str());
                if(buffer->tail - buffer->head < size){
                    LOG_WARN << "full request body not yet received " << size << endl;
                    return true;
                }
            }
            body_->parse(buffer);
            if(!body_->continue_reading()){
                // done with extracting headers
                LOG_DEBUG << "done extracting body " << body_->serialize() << endl;
                cb_ = [this](buffer_type * buffer){
                    return false;
                };
                return cb_(buffer);
            }
            return true;
        }
        std::shared_ptr<basic_http_headers_interface> headers_;
        std::shared_ptr<basic_http_response_line_interface> response_line_;
        std::shared_ptr<basic_object_interface> body_;
        std::shared_ptr<buffer_type > buffer_;
        callback cb_;
};

/**********************************************
 *  user interface class implementations ******
 * *******************************************/

http_response_v1::http_response_v1() 
    : impl_(std::make_shared<impl>()){}

http_response_v1::http_response_v1(http_response_v1 & obj)
    : impl_(std::make_shared<impl>()){
        // add logic to copy the impl details
}

// set methods
http_response_v1::http_response_v1(http_response_v1 && obj)
    : impl_(obj.impl_){
    obj.impl_ = nullptr;
}

http_response_v1::http_response_v1(http_status & status)
    : impl_(std::make_shared<impl>(std::make_shared<http_response_line_v1>(status))){}

http_response_v1 & http_response_v1::set_status(http_status & status){
    impl_->set_status(status);
    return *this;
}

http_response_v1 & http_response_v1::set_version(std::string version){
    impl_->set_version(version);
    return  *this;
}

http_response_v1 & http_response_v1::set_header(std::string key , std::string value){
    impl_->set_header(key , value);
    return *this;
}

//parser methods
void http_response_v1::parse(buffer_type * buffer){
    impl_->parse(buffer);
    return ;
}
void http_response_v1::at_eof(buffer_type * buffer){
    impl_->at_eof(buffer);
    return ;
}
http_response_v1::buffer_type * http_response_v1::buffer(){
    return impl_->buffer();
}
bool http_response_v1::continue_reading(){
    return impl_->continue_reading();
}

// it will do a deep cloning
http_response_v1 & http_response_v1::set_body(std::shared_ptr<basic_object_interface > body){
    impl_->set_body(body);
    return *this;
}

// injections
http_response_v1 & http_response_v1::set_headers(std::shared_ptr<basic_http_headers_interface > headers){
    impl_->set_headers(headers);
    return *this;
}
http_response_v1 & http_response_v1::set_response_line(std::shared_ptr<basic_http_response_line_interface  > response_line){
    impl_->set_response_line(response_line);
    return *this;
}

// get methods
const http_status & http_response_v1::status(){
    return impl_->status();
}
std::string & http_response_v1::version(){
    return impl_->version();
}
const std::string & http_response_v1::operator[](std::string key){
    return impl_->header(key);
}
std::shared_ptr<basic_http_headers_interface> http_response_v1::headers(){
    return impl_->headers();
}
std::shared_ptr<basic_object_interface> http_response_v1::body(){
    return impl_->body();
}
std::shared_ptr<basic_http_response_line_interface> http_response_v1::response_line(){
    return impl_->response_line();
}

// copy , move , clone
std::shared_ptr<basic_http_response_interface> http_response_v1::clone(){
    return std::make_shared<http_response_v1>(*this);
}
http_response_v1 & http_response_v1::operator=(http_response_v1 & obj){
    assert(impl_ != nullptr && obj.impl_ != nullptr);
    auto status = obj.impl_->status();
    impl_->set_status(status);
    return *this;
}
http_response_v1 & http_response_v1::operator=(http_response_v1 && obj){
    assert(obj.impl_ != nullptr);
    impl_ = obj.impl_;
    obj.impl_ = nullptr;
    return *this;
}

// serialize and deserialize
std::string http_response_v1::serialize(){
    return impl_->serialize();
}
std::string http_response_v1::serialize(basic_formatter_interface & formatter){
    LOG_DEBUG << "triggered read in " << __func__ << endl;
    return impl_->serialize(formatter);
    LOG_DEBUG << " done " << __func__ << endl;
}

http_response_v1 & http_response_v1::clear(){
    impl_->clear();
    return *this;
}
