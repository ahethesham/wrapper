#ifndef __BASIC_HEADERS_H__
#define __BASIC_HEADERS_H__

#include <map>
#include <string>
#include "basic_status_line.h"
#include "buffer.h"

template<typename impl>
class basic_http_headers : public Serializable{
    impl * impl_;
    public:
        using buffer_policy = impl::buffer_policy;
        using key_type = impl::key_type;
        using value_type = impl::value_type;
        basic_http_headers() : impl_(new impl()){}
        /*
         * will be used while creating responses 
         */
        basic_http_headers(buffer_policy * buffer) : impl_(buffer) {}

        basic_http_headers & create(buffer_policy * buffer) {
            impl_->create(buffer);
            return *this;
        }

        basic_http_headers & operator[](key_type key , value_type value){
            (*impl_)[key] = value;
            return *this;
        }

        value_type & operator[](key_type key){
            return (*impl_)[key];
        }

        serialize_type serialize() override{
            return impl_->serialize();
        }
};

template<typename buffer_type = buffer_v1>
class http_1_0_headers_impl : public Serializable{
    public:
        typedef std::string key_type;
        typedef std::string value_type;
        using buffer_policy = buffer_type;

        http_1_0_headers_impl(){
            // add few default headers 
        }

        http_1_0_headers_impl(http_1_0_headers_impl && headers) {}

        bool process(){}

        http_1_0_headers_impl & operator[](key_type & key , value_type & value){
            storage_[key] = value;
            return *this;
        }

        value_type & operator[](key_type & key){
            auto itr = storage_.find(key);
            if(itr == storage_.end())throw std::runtime_error("key not found in http headers");
            return itr->second;
        }
        bool process(http_1_0_request_line *reqline_ , buffer_type * buffer){
            assert(reqLine_ && reqline_->version() == "HTTP/1.0");

        }

        serialize_type serialize() override{
            serialize_type res = "";
            for(auto itr : storage_){
                res += '\"';
                res += itr->first;
                res += '\"';
                res += ": ";
                res += '\"';
                res += itr->second ;
                res += '\"';
                res += "\r\n";
            }
            return res;
        }

    private:
        std::map<key_type , value_type> storage_;
        http_1_0_request_line * reqLine_;
};

using http_1_0_headers = basic_http_headers<http_1_0_headers_impl<>>;

#endif
