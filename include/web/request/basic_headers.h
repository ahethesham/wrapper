#ifndef __BASIC_HEADERS_H__
#define __BASIC_HEADERS_H__

#include <map>
#include <functional>
#include <string>
#include "http_request_fwd.h"

template<typename impl>
class basic_http_headers {
    impl * impl_;
    public:
        using buffer_policy = impl::buffer_policy;
        using key_type = impl::key_type;
        using value_type = impl::value_type;
        using status_line_type = impl::status_line_type;

        basic_http_headers() : impl_(new impl()){}
        /*
         * will be used while creating responses 
         */
        basic_http_headers(buffer_policy * buffer) : impl_(buffer) {}

        basic_http_headers  &  process( buffer_policy * buffer , status_line_type * status_line){
            impl_->process(buffer , status_line);
            return *this;
        }

        bool continue_reading(){
            return impl_->continue_reading();
        }

        basic_http_headers & process(buffer_policy * buffer) {
            impl_->process(buffer);
            return *this;
        }

        basic_http_headers & create(buffer_policy * buffer) {
            impl_->create(buffer);
            return *this;
        }


        value_type & operator[](key_type key){
            return (*impl_)[key];
        }

        std::string serialize() {
            return impl_->serialize();
        }
};

template<typename buffer_type ,
         typename status_line_policy >
class http_1_0_headers_impl {
    public:
        typedef std::string key_type;
        typedef std::string value_type;
        using buffer_policy = buffer_type;
        using status_line_type = status_line_policy;

        http_1_0_headers_impl(){
            // add few default headers 
            processor_ = [this](buffer_type * buffer){
                return    process_headers(buffer);
            };
        }

        http_1_0_headers_impl(http_1_0_headers_impl && headers) {}
        
        bool process_headers(buffer_type * buffer){
            if(buffer == nullptr || strstr(buffer->data + buffer->head , "\r\n") == nullptr)return true;

            do{
                ignore_spaces(buffer);

                if(check_for_eof(buffer))
                    return processor_(buffer);
                

                key_type key;
                value_type value;

                while(*(buffer->data + buffer->head) != ':')
                    key += *(buffer->data + buffer->head++);

                assert( *(buffer->data +  buffer->head++) == ':' &&
                        *(buffer->data +  buffer->head++) == ' ' );
                       
                
                while(*(buffer->data + buffer->head) != '\r')
                    value += *(buffer->data + buffer->head++);

                assert( *(buffer->data + buffer->head++) == '\r' &&
                        *(buffer->data + buffer->head++) == '\n');
                
                storage_[key] = value;

                log("adding key value %s , %s" , key.c_str() , value.c_str());

            }while(strstr(buffer->data + buffer->head , "\r\n") != nullptr);

            log("returning from process header");
            
        }


        bool continue_reading(){
            return processor_(nullptr);
        }

        value_type & operator[](key_type & key ){
            if(storage_.find(key) == storage_.end()){
                storage_.insert({key , value_type("")});
            }
            return storage_[key];
        }

        bool process( buffer_type * buffer , status_line_type *reqline_ ){
            assert(reqLine_ && reqline_->version() == "HTTP/1.0");
            // this is going to be a http/1.0 header
            return processor_(buffer);
        }

        bool process(buffer_type * buffer){

            return processor_(buffer);
        }

        std::string serialize() {
            std::string res = "";
            for(auto itr : storage_){
                res += '\"';
                res += itr.first;
                res += '\"';
                res += ": ";
                res += '\"';
                res += itr.second ;
                res += '\"';
                res += "\r\n";
           }
            return res;
        }

    private:
        void ignore_spaces(buffer_type * buffer){
            while(*(buffer->data + buffer->head) == ' ' || *(buffer->data + buffer->head) == '\t' )
                buffer->head++;
            return ;
        }
        bool check_for_eof(buffer_type * buffer){
            if(*(buffer->data + buffer->head) == '\r' ){
            
                assert(*(buffer->data + ++buffer->head) == '\n');
                buffer->head++;
                // headers have been completed ...now go ahead with the body    
                processor_ = [this](buffer_type * buffer){
                    return false;
                };
                return true;
            }
            return false;
        }
        using callback = std::function<bool(buffer_type *  buffer)>;
        callback processor_;
        std::map<key_type , value_type> storage_;
        status_line_type * reqLine_;
};


#endif
