#ifndef __JSON_TOKEN_V1_H__
#define __JSON_TOKEN_V1_H__
#include "basic_token_interface.h"
/*
 * concrete class impl
 */
template<typename T>
class token_v1 : public basic_token_interface<T>  {

    public:
        using base_type = basic_token_interface<T>;
        using object_interface = base_type::object_interface;
        using tokenizer_interface = base_type::tokenizer_interface;
        using token_handler = typename base_type::token_handler;

        /*
         * we just can't make token  object depend on any level 0 objects 
         */
        token_v1(char ch ,token_handler cb) : ch_(ch) , cb_(cb) {}
    
        bool compare(char ch ) override{
            return ch_ == ch;
        }
        
        char get() override{
            return ch_;
        }

        std::shared_ptr<object_interface> cb(std::shared_ptr<typename base_type::tokenizer_interface> tokenizer)  override{
            return cb_(tokenizer);
        }

    private:
        char ch_;
        token_handler cb_;
};

#endif
