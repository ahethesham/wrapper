#ifndef __TOKEN_H__
#define __TOKEN_H__
#include <cassert>
#include <functional>
#include <stdexcept>
#include <string>
#include "json_fwd.h"
#include <map>
#include "logger.h"


template<typename impl >
class basic_tokenizer{
    public:
        using token_type = impl::token_type;
        using buffer_type = impl::buffer_type;
        using token_handler = impl::token_handler;
        using impl_type    = impl;

        basic_tokenizer(const char * input) : impl_( new impl(input) ){} 
        basic_tokenizer(buffer_type * buffer): impl_( new impl(buffer) ){} 
        
        token_type * getNext(){
            return impl_->getNext();
        }

        token_type * peek(){
            return impl_->peek();
        }

        basic_tokenizer & operator++(){
            ++(*impl_);
            return *this;
        }
        basic_tokenizer & operator--(){
            (*impl_)--;
            return *this;
        }

        basic_tokenizer & register_token_handler(char character , token_handler cb){
            impl_->register_token_handler(character , cb);
            return * this;
        }

        basic_tokenizer & register_pre_processor(std::function<variants(basic_tokenizer<impl> *) > cb){
            impl_->register_pre_processor(cb);
            return *this;
        }
        
        char get_next_char(){
            return impl_->get_next_char();
        }

        bool hasNext(){
            return impl_->hasNext();
        }

        char peek_next_char(){
            return impl_->peek_next_char();
        }

    private:
        impl * impl_;
};

template<typename  token_policy,
        typename grammer_policy , 
         typename  buffer_policy >
class tokenizer_impl{
    public:
        using buffer_type   = buffer_policy;
        using token_type    = token_policy;
        using grammer_type = grammer_policy;
        using token_handler = std::function<variants(json_tokenizer *)>; 
        
        tokenizer_impl(const char * input) : data_(input) , pos_(0) , grammer_(new grammer_type()){}
        
        token_type * getNext(){

            do{
                char ch = *(data_ + pos_);
                if(grammer_->is_end_of_file(ch))throw std::runtime_error("EOF reached ERR_EVNT_0000002");

                if(grammer_->is_unexpected_char(ch))throw std::runtime_error("unexpected char received ERR_EVNT_0000004");
                if(grammer_->continue_reading(ch) ){
                    pos_++;
                    continue;
                }
                
                auto cb = handlers_[ch];
                assert(cb != nullptr);
                return new token_type(ch , cb);

            } while(1);
            
        }

        char get_next_char(){
            if(*(data_ + pos_) == '\0')throw std::runtime_error("EOF reached ERR_EVNT_0000003");
            return *(data_ + pos_++);
        }

        tokenizer_impl & register_token_handler(char character ,token_handler cb){
            this->handlers_[character] = cb;
            return * this;
        }

        char peek_next_char(){
            char ch = '\0';
            int temp = pos_;
            do{
                ch = *(data_ + temp);
                if(ch == '\0')throw std::runtime_error("ERR_EVNT_0000005");

                if(grammer_->is_end_of_file(ch))throw std::runtime_error("EOF reached ERR_EVNT_0000002");

                if(grammer_->is_unexpected_char(ch))throw std::runtime_error("unexpected char received ERR_EVNT_0000004");
                if(grammer_->continue_reading(ch) ){
                    temp++;
                    continue;
                }
                break;
            }while(1);

            return ch;
        }

        token_type * peek(){
            int temp = pos_;
            do{
                char ch = *(data_ + temp++);
                if(ch == '\0')throw std::runtime_error("ERR_EVNT_0000005");

                if(grammer_->is_end_of_file(ch))throw std::runtime_error("EOF reached ERR_EVNT_0000002");

                if(grammer_->is_unexpected_char(ch))throw std::runtime_error("unexpected char received ERR_EVNT_0000004");
                if(grammer_->continue_reading(ch) )continue;
                
                auto cb = handlers_[ch];
                assert(cb != nullptr);

                return new token_type(ch , cb);

            } while(1);
        }

        bool hasNext(){
            return *(data_ + pos_) != '\0';
        }

        tokenizer_impl  & operator++(){
            do{
                char ch = *(data_ + pos_++);
                if(grammer_->is_end_of_file(ch))throw std::runtime_error("EOF reached ERR_EVNT_0000002");

                if(grammer_->is_unexpected_char(ch))throw std::runtime_error("unexpected char received ERR_EVNT_0000004");
                if(grammer_->continue_reading(ch) )continue;
                auto cb = handlers_[ch];
                assert(cb != nullptr);
                break;
            } while(1);

            return *this;
        }

        tokenizer_impl & operator--(){

        }


    private:
        const char * data_;
        int pos_;
        std::map<char , token_handler> handlers_;
        grammer_type * grammer_;
};

class token_v1 {
    public:
        using handler = std::function<variants(json_tokenizer *)>;
        token_v1(char ch , handler cb) : ch_(ch) , cb_(cb){}
        bool compare(char ch){
            return ch_ == ch;
        }
        bool compare(std::string str){
            return (str.size() == 1  && ch_ == str[0]);
        }

        variants cb(json_tokenizer * tokenizer){
            return cb_(tokenizer);
        }
        char get(){
            return ch_;
        }

    private:
        handler cb_;
        char ch_;
};

class json_grammer{
    public:
        bool is_end_of_file(char ch){
            return ch == '\0';
        }
        bool is_unexpected_char(char ch){
            return false;
        }
        bool continue_reading(char ch){
            return ( ch == ':' ||
                    ch == ' '  ||
                    ch == '\t' ||
                    ch == ','  ||
                    ch == '\r' || 
                    ch == '\n' );
        }
};
#endif
