#include "http_headers_v1.h"
#include <cassert>
#include <functional>
#include <map>
#include <stdexcept>

class http_headers_v1::impl{
    using callback = std::function<bool(buffer_v1 * buffer)>;
    public:
        impl() : storage_(new std::map<std::string , std::string >())  {
            cb_ = [this](buffer_v1 * buffer){
                return extract_header(buffer);
            };
        }

        const std::string & get(std::string key){
            auto itr = storage_->find(key);
            if(itr == storage_->end())throw std::runtime_error("key not found");

            return itr->second;
        }
        void set(std::string & key , std::string & value){
            (*storage_)[key] = value;
            return ;
        }
        bool continue_reading(){
            return cb_(nullptr);
        }
        void at_eof(buffer_v1 * buffer){
            cb_(buffer);
            return ;
        }
        void parse(buffer_v1 * buffer){
            cb_(buffer);
            return ;
        }
        std::string serialize(basic_formatter_interface & formatter){
            std::string res = "";
            bool temp = false;
            for(auto itr : *storage_){
                if(temp)
                    formatter.line_post_processor(res);
                temp = true;
                res += itr.first;
                res += ": ";
                res += itr.second;
            }
            return res;
        }
    private:
        bool extract_header(buffer_v1 * buffer){
            if(buffer == nullptr)
                return true;
            std::string key ;
            std::string value;
            while(strstr((buffer->data + buffer->head), "\r\n") != NULL){
                char * ptr = strstr((buffer->data + buffer->head) , "\r\n");
                if(ptr == (buffer->data + buffer->head)){
                    cb_ = [this](buffer_v1 * buffer){
                        return false;
                    };
                    return cb_(nullptr);
                }
                while(*(buffer->data + buffer->head) != ':')
                    key += *(buffer->data + buffer->head++);
                assert(*(buffer->data + buffer->head++) == ':' && *(buffer->data + buffer->head++) == ' ');
                while(*(buffer->data + buffer->head) != '\r')
                    value += *(buffer->data + buffer->head++);
                assert((buffer->data + buffer->head) == ptr);
                buffer->head += 2;
                storage_->insert(make_pair(key , value));
            }
            return true;
        }
        std::map<std::string , std::string > * storage_;
        // to extract the headers from the given buffer
        callback cb_;

};


http_headers_v1::http_headers_v1() : impl_(new impl()) {}

//copy constructor yet to be implemented TODO
http_headers_v1::http_headers_v1(http_headers_v1 & obj) {
    throw std::runtime_error("error constructor yet to be implemented ");
}
http_headers_v1::http_headers_v1(http_headers_v1 && obj) : impl_(obj.impl_){
    obj.impl_ = nullptr;
}

const std::string & http_headers_v1::get(std::string key){
    return impl_->get(key);
}
const std::string & http_headers_v1::operator[](std::string key){
    return get(key);
}

http_headers_v1::self_type & http_headers_v1::set(std::string key , std::string value){

    impl_->set(key , value);
    return *this;
}

http_headers_v1::self_type & http_headers_v1::operator=(http_headers_v1 & obj){
    // copy assignment
    // TODO
    return *this;
}

http_headers_v1::self_type & http_headers_v1::operator=(http_headers_v1 && obj){
    if(this == &obj) return *this;
    if(impl_)
        delete impl_;

    impl_  = obj.impl_;
    obj.impl_ = nullptr;
    return *this;
}

bool http_headers_v1::continue_reading(){
    return impl_->continue_reading();
}

void http_headers_v1::at_eof(buffer_v1 * buffer){
    return impl_->at_eof(buffer);
}

buffer_v1 *  http_headers_v1::buffer(){
    // TODO
    return nullptr;
}

void http_headers_v1::parse(buffer_v1 * buffer){
    return impl_->parse(buffer);
}

std::string http_headers_v1::serialize(basic_formatter_interface & formatter){
    return  impl_->serialize(formatter);
}
