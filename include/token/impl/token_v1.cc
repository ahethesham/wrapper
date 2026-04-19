#include "json_token_v1.h"


/*
 * json token v1 conrete class 
 */

class token_v1::impl{
    public:
        using handler = token_v1::token_handler;

        impl(char ch , handler cb): ch_(ch) , cb_(cb){}

        bool compare(char ch ){
            return ch == ch_;
        }

        char get(){
            return ch_;
        }

        basic_object_interface & cb(basic_tokenizer_interface & tokenizer) {
            return cb_(tokenizer);
        }

    private:
        handler cb_;
        char ch_;
};

token_v1::token_v1(char ch , token_v1::token_handler cb) : impl_(*new impl(ch , cb)){}

bool token_v1::compare(char ch) {
    return impl_.compare(ch);
}

char token_v1::get(){
    return impl_.get();
}

token_v1::object_interface & token_v1::cb(token_v1::tokenizer_interface & tokenizer) {
    return impl_.cb(tokenizer);
}


