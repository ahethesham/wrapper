#ifndef __JSON_H__
#define __JSON_H__
#include "json_fwd.h"


json_object * json_builder(const char * input){
    json_tokenizer * tokenizer = new json_tokenizer(input);
    tokenizer->register_token_handler('{' , json_object::build );
    tokenizer->register_token_handler('[' , json_array::build );
    tokenizer->register_token_handler('\"' , json_string::build );
    for(int i = 0 ; i < 10 ; i++){
        tokenizer->register_token_handler(char('0' + i) , json_int::build);
    }

    tokenizer->register_token_handler('f' , json_bool::build );
    tokenizer->register_token_handler('t' , json_bool::build );
    tokenizer->register_token_handler('n' , json_null::build );
    
    return new json_object(tokenizer);
}

#endif
