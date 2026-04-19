#ifndef __JSON_TOKEN_V1_H__
#define __JSON_TOKEN_V1_H__
#include "basic_object_interface.h"
#include "basic_tokenizer_interface.h"

/*
 * concrete class impl
 */
template<typename T>
class token_v1 : public basic_token_interface<T>  {

    public:
        using base_type = basic_token_interface<basic_object_interface>;
        using object_interface = base_type::object_interface;
        using tokenizer_interface = base_type::tokenizer_interface;
        using token_handler = typename base_type::token_handler;


        token_v1(char ch ,token_handler cb);
    
        bool compare(char ch ) override;
        
        char get() override;

        object_interface & cb(tokenizer_interface & tokenizer) override;

    private:
        class impl;
        impl &impl_;
};



#endif
