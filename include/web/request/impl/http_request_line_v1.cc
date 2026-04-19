#include "http_request_line_v1.h"
#include <cassert>
#include <functional>
#include <map>
#include <stdexcept>
#include <vector>


class http_request_line_v1::impl{
    using callback = std::function<bool (buffer_v1 *)>;
    public:

        impl() : method_(new std::string()) , version_(new std::string()) , uri_(new std::string()) , query_params_(new std::map<std::string , std::string >()){}
        
        void set_method(std::string &method){
            assert(method_ != nullptr);
            method_->clear();
            method_->append(method);
        }
        void set_uri(std::string uri){
            assert(uri_ != nullptr);
            uri_->clear();
            uri_->append(uri);
        }
        void set_version(std::string version){
            assert(version_ != nullptr);
            version_->clear();
            version_->append(version);
        }
        void set_query_param(std::string key , std::string value){
            assert(query_params_ != nullptr);
            query_params_->insert(make_pair(key , value));
        }

        void set_query_param(std::vector<std::pair<std::string , std::string > > & params){
            assert(query_params_ != nullptr);
            for(auto itr : params)
                query_params_->insert(make_pair(itr.first , itr.second));
            return ;
        }

        std::string & get_method(){
            return *method_;
        }
        std::string & get_uri(){
            return *uri_;
        }
        std::string & get_version(){
            return *version_;
        }

        const std::string & get_query_param(std::string key){
            return (*query_params_)[key];
        }
        void at_eof(buffer_v1 * buffer){
            //TODO
        }
        bool continue_reading(){
            return cb_(nullptr);
        }
        void parse(buffer_v1 * buffer){
            cb_(buffer);
        }
        buffer_v1 * buffer(){
            return nullptr;
        }
        std::string serialize(basic_formatter_interface &  formatter){
            std::string res = "";
            res = *method_ + ' ' + *uri_ + ' ' + *version_;
            return res;
        }

    private:
        bool extract_line(buffer_v1 * buffer){
            if(buffer == nullptr)return false;
            if(strstr(buffer->data + buffer->head , "\r\n") == NULL)
                return true;
            while(*(buffer->data + buffer->head) != ' ')
                method_ += *(buffer->data + buffer->head++);
            while(*(buffer->data + buffer->head) != ' ')
                uri_ += *(buffer->data + buffer->head++);
            while(*(buffer->data + buffer->head) != ' ')
                version_ += *(buffer->data + buffer->head++);

            extract_query_param(*uri_);
            cb_ = [this](buffer_v1 * buffer){
                return false;
            };

            return false;
        }
        void extract_query_param(std::string & str){
            //TODO
            return ;
        }
        std::string *method_;
        std::string *uri_;
        std::string *version_;
        callback cb_;
        std::map<std::string , std::string > *query_params_;
};

http_request_line_v1::http_request_line_v1() : impl_(new impl()){}

http_request_line_v1::http_request_line_v1(http_request_line_v1 &obj){
    throw std::runtime_error("yet to implement a copy constrcutor");
}
http_request_line_v1::http_request_line_v1(http_request_line_v1 && obj){
    if(impl_)
        delete impl_;
    impl_ = obj.impl_;
    obj.impl_ = nullptr;
}
http_request_line_v1::self_type & http_request_line_v1::set_method(std::string method){
    impl_->set_method(method);
    return *this;
}

http_request_line_v1::self_type & http_request_line_v1::set_uri(std::string uri){
     impl_->set_uri(uri);
     return *this;
}

http_request_line_v1::self_type & http_request_line_v1::set_version(std::string version){
    impl_->set_version(version);
    return *this;
}

http_request_line_v1::self_type & http_request_line_v1::set_query_param(std::string key , std::string value){
    impl_->set_query_param(key , value);
    return *this;
}

const std::string & http_request_line_v1::get_method(){
    return impl_->get_method();
}
const std::string & http_request_line_v1::get_uri(){
    return impl_->get_uri();
}
const std::string & http_request_line_v1::get_version(){
    return impl_->get_version();
}

const std::string & http_request_line_v1::get_query_param(std::string key){
    return impl_->get_query_param(key);
}

void http_request_line_v1::at_eof(buffer_v1 * buffer){
    impl_->at_eof(buffer);
    return ;
}
void http_request_line_v1::parse(buffer_v1 * buffer){
    impl_->parse(buffer);
    return ;
}
bool http_request_line_v1::continue_reading(){
    return impl_->continue_reading();
}
buffer_v1 * http_request_line_v1::buffer(){
    return impl_->buffer();
}

std::string http_request_line_v1::serialize(basic_formatter_interface & formatter){
    return impl_->serialize(formatter);
}
//copy assignment 
http_request_line_v1::self_type & http_request_line_v1::operator=(http_request_line_v1 & obj){
    return *this;
}
http_request_line_v1::self_type & http_request_line_v1::operator=(http_request_line_v1 && obj){
    if(this == &obj)return *this;
    if(impl_)
        delete impl_;
    impl_ = obj.impl_;
    obj.impl_ = nullptr;
    return *this;
}
