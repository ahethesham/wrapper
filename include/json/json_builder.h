#ifndef __JSON_H__
#define __JSON_H__

#include "json_grammer_v1.h"
#include "json_tokenizer_v1.h"
#include "basic_object_interface.h"
#include "json_object_v1.h"
#include "json_string_v1.h"
#include "json_integer_v1.h"
#include "json_null_v1.h"
#include "json_array_v1.h"
#include "json_boolean_v1.h"
#include "file_logger_v1.h"
#if 0
template<typename buffer_type ,
         typename tokenizer_type>
inline tokenizer_type * tokenizer_builder(buffer_type * buffer)
{
    json_grammer_v1 & grammer = *new json_grammer_v1(" \t\r\n," , "" , '\0');

    json_tokenizer * tokenizer = new json_tokenizer(grammer , (buffer->data + buffer->head));

    tokenizer->register_token_handler('{' , basic_object_interface::build<json_object_v1> );
    tokenizer->register_token_handler('[' , basic_object_interface::build<json_array_v1>  );
    tokenizer->register_token_handler('\"' , basic_object_interface::build<json_string_v1>);
    for(int i = 0 ; i < 10 ; i++){
        tokenizer->register_token_handler(char('0' + i) , basic_object_interface::build<json_integer_v1>);
    }

    tokenizer->register_token_handler('f' , basic_object_interface::build<json_boolean_v1> );
    tokenizer->register_token_handler('t' , basic_object_interface::build<json_boolean_v1>  );
    tokenizer->register_token_handler('n' , basic_object_interface::build<json_null_v1> );
    
    return tokenizer;
}
#endif

 template< typename tokenizer_type>
inline tokenizer_type * tokenizer_builder(const char * input)
{

    static Logger &  logger =  Logger::build();


    json_grammer_v1 & grammer = *new json_grammer_v1(" :\t\r\n," , "" , '\0');

    tokenizer_type * tokenizer = new tokenizer_type(grammer , input);

    tokenizer->register_token_handler('{' , basic_object_interface::build<json_object_v1> );
    tokenizer->register_token_handler('[' , basic_object_interface::build<json_array_v1>  );
    tokenizer->register_token_handler('\"' , basic_object_interface::build<json_string_v1>);
    for(int i = 0 ; i < 10 ; i++){
        tokenizer->register_token_handler(char('0' + i) , basic_object_interface::build<json_integer_v1>);
    }

    tokenizer->register_token_handler('f' , basic_object_interface::build<json_boolean_v1> );
    tokenizer->register_token_handler('t' , basic_object_interface::build<json_boolean_v1>  );
    tokenizer->register_token_handler('n' , basic_object_interface::build<json_null_v1> );
    
    return tokenizer;
}

template<typename json_object_type>
json_object_type & json_builder(const char * input){
    json_tokenizer & tokenizer = *tokenizer_builder<json_tokenizer>(input);
    return *new json_object_type(tokenizer);
}

#endif
