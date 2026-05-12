#include "http_request_line_v1.h"
#include <cassert>
#include <functional>
#include <map>
#include <stdexcept>
#include <vector>


class http_request_line_v1::impl{
    using callback = std::function<bool (buffer_v1 *)>;
    public:

        impl() : method_(new std::string("GET")) , version_(new std::string("HTTP/1.1")) , uri_(new std::string("/")) , query_params_(new std::map<std::string , std::string >()){
            cb_ = [this](buffer_type * buffer){
                return extract_line(buffer);
            };
        }
        impl(std::string & method , std::string uri , std::string & version , std::map<std::string , std::string > & params)
            : method_(new std::string(method)) , version_(new std::string(version)) , uri_(new std::string(uri)) , query_params_(new std::map<std::string , std::string>(params)) {
            cb_ = [this](buffer_type * buffer){
                return extract_line(buffer);
            };
        }
        
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

         std::string & get_query_param(std::string key){
             auto itr = query_params_->find(key);
             if(itr == query_params_->end())
                 throw std::runtime_error("Param Not Found");

            return itr->second;
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
            if(uri_->size() == 0)
                *uri_ = '/';

           res = *method_ + ' ' +  *uri_ ;
            if(query_params_->size() > 0)
                res += '?';
            bool start = true;
            for(auto itr : *query_params_)
            {
                if(!start)
                    res += '&';
                start = false;
                res += itr.first ;
                res += '=' ;
                res += itr.second;
            }

            res += ' ' + *version_;
            formatter.line_post_processor(res);
            return res;
        }
        std::map<std::string , std::string > & get_query_params(){
            return *query_params_;
        }
    private:
        bool extract_line(buffer_v1 * buffer){
            if(buffer == nullptr)return false;
            if(strstr(buffer->data + buffer->head , "\r\n") == NULL)
                return true;
            std::string temp_str = "";

            while(*(buffer->data + buffer->head) != ' ')
                temp_str += *(buffer->data + buffer->head++);

            assert(*(buffer->data + buffer->head++) == ' ');

            set_method(temp_str);
            temp_str.clear();

            while(*(buffer->data + buffer->head) != ' '){
                if(*(buffer->data + buffer->head) == '?')
                    extract_query_param(buffer);
                else
                    temp_str += *(buffer->data + buffer->head++);
            }

            assert(*(buffer->data + buffer->head++) == ' ');
            set_uri(temp_str);
            temp_str.clear();

            while(*(buffer->data + buffer->head) != '\r')
                temp_str += *(buffer->data + buffer->head++);
            
            assert(*(buffer->data + buffer->head++) == '\r' &&
                    *(buffer->data + buffer->head++) == '\n');
            set_version(temp_str);
            temp_str.clear();

            cb_ = [this](buffer_v1 * buffer){
                return false;
            };
            return false;
        }
        void extract_query_param(buffer_type * buffer){
            assert(*(buffer->data + buffer->head++) == '?');
            std::string key , value;
            short turn = 0;
            while(*(buffer->data + buffer->head ) != ' '){
                if(*(buffer->data + buffer->head ) == '&'){
                    assert(turn == 1);
                    buffer->head++;
                    turn = 0;
                    (*query_params_)[key] = value;
                    key = value = "";
                    continue;
                }else if(*(buffer->data + buffer->head) == '='){
                    assert(turn == 0);
                    turn = 1;
                    buffer->head++;
                    continue;
                }
                turn == 0 ? key += *(buffer->data + buffer->head++) : value += *(buffer->data + buffer->head++);
            }
            if(turn == 1)
                (*query_params_)[key] = value;
            return ;
        }
        std::string *method_;
        std::string *uri_;
        std::string *version_;
        callback cb_;
        std::map<std::string , std::string > *query_params_;
};

http_request_line_v1::http_request_line_v1() : impl_(new impl()){}

http_request_line_v1::http_request_line_v1(http_request_line_v1 &obj) : impl_(new impl(obj.get_method() , obj.get_uri() , obj.get_version() , obj.impl_->get_query_params())) { }

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

std::string & http_request_line_v1::get_method(){
    return impl_->get_method();
}
 std::string & http_request_line_v1::get_uri(){
    return impl_->get_uri();
}
 std::string & http_request_line_v1::get_version(){
    return impl_->get_version();
}

 std::string & http_request_line_v1::get_query_param(std::string key){
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
    if(&obj == this)
        return *this;
    impl_->set_method(obj.get_method());
    impl_->set_uri(obj.get_uri());

    //impl_->set_query_param(obj.get_query_params());
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

std::shared_ptr<basic_http_request_line_interface> http_request_line_v1::clone(){
    return std::make_shared<http_request_line_v1>(*this);
}
