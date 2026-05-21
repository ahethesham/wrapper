#ifndef __JSON_TOKENIZER_V1_H__
#define __JSON_TOKENIZER_V1_H__

/*
 * Remember this entire module of tokenizer is a level 0 object 
 */
#include "basic_grammer_interface.h"
#include "basic_tokenizer_interface.h"
#include "buffer.h"
#include "token_v1.h"
#include <map>
#include <assert.h>
#include <stdexcept>

template < typename token_policy ,
           typename buffer_policy ,
           typename T>
class json_tokenizer_v1 : public basic_tokenizer_interface<T>{
    
    public:
        using token_type = token_policy;
        using buffer_type = buffer_policy;
        using self_type   = json_tokenizer_v1<token_type , buffer_type , T>;
        using token_handler = typename token_type::token_handler;


        json_tokenizer_v1(basic_grammer_interface & grammer , const char * input) :
        grammer_(grammer) ,
        buffer_(input) ,
        pos_(0) ,
        line_(0) {}

        json_tokenizer_v1(basic_grammer_interface & grammer , buffer_type * buffer) :
            grammer_(grammer) ,
            buffer_(buffer->data) ,
            pos_(0) ,
            line_(0){}
        
        token_type *  getNext() override{
            return move_token_forward(pos_);
        }

        token_type *  peek()    override{
            int temp = pos_;
            return move_token_forward(temp);
        }

        token_type *  operator++() override{
            return move_token_forward(pos_);
         }

        token_type *  operator--()override {
            //TODO
            throw std::runtime_error("yet to be implemented");
        }

        self_type & register_token_handler(char ch , token_handler cb) override{
            handlers_[ch] = cb;
            return *this;
        }

        char get_next_char() override{
            return *(buffer_ + pos_++);
        }
        bool hasNext() override{
            return *(buffer_ + pos_) != '\0';
        }
        char peek_next_char() override{
            return *(buffer_ + pos_);
        }
        char peek_next_valid_char() override{
            int temp = pos_;
            return move_char_forward(temp);
        }

        char get_next_valid_char() override{
            return move_char_forward(pos_);
        }
        
    private:
        token_type * move_token_forward(int &idx){
            token_type * ptoken = nullptr;
            do{
                char ch = *(buffer_ + idx++);
                if(ch == '\0')throw std::runtime_error("ERR_EVNT_0000005");

                if(grammer_.is_end_of_file(ch))throw std::runtime_error("EOF reached ERR_EVNT_0000002");

                if(grammer_.is_unexpected_char(ch))throw std::runtime_error("unexpected char received ERR_EVNT_0000004");
                if(grammer_.continue_reading(ch) )continue;
                
                auto cb = handlers_[ch];
#ifdef __DEBUG
                if(nullptr == cb)
                    Logger::build() << "We are unable to find callback for the token" << ch << endl;
#endif
                
                assert(cb != nullptr);

                ptoken = new token_type(ch , cb);
                break;

            } while(1);

            assert(ptoken != nullptr);
            return  ptoken;
        }
        char move_char_forward(int &idx){
            char ch = '\0';
            do{
                ch = *(buffer_ + idx++);
                if(ch == '\0')throw std::runtime_error("ERR_EVNT_0000005");

                if(grammer_.is_end_of_file(ch))throw std::runtime_error("EOF reached ERR_EVNT_0000002");
                if(grammer_.is_unexpected_char(ch))throw std::runtime_error("unexpected char received ERR_EVNT_0000004");
                if(grammer_.continue_reading(ch) )continue;
                
                break;
            }while(1);

            return ch;
        }

        basic_grammer_interface & grammer_;
        std::map<char , token_handler > handlers_;
        int pos_;
        int line_;
        const char  * buffer_;
};

/*
 *ANybody who wants to use the tokenizer should have to declare their own protocol in the template 
 */

#endif
