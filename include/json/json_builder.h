#ifndef __JSON_H__
#define __JSON_H__

#include "basic_tokenizer_interface.h"
#include "json_grammer_v1.h"
#include "basic_object_interface.h"
#include "json_object_v1.h"
#include "json_string_v1.h"
#include "json_integer_v1.h"
#include "json_null_v1.h"
#include "json_array_v1.h"
#include "json_boolean_v1.h"
#include "file_logger_v1.h"

 template< typename tokenizer_type>
inline tokenizer_type * tokenizer_builder(const char * input)
{

    static Logger &  logger =  Logger::build();


    json_grammer_v1 & grammer = *new json_grammer_v1(" :\t\r\n," , "" , '\0');

    tokenizer_type * tokenizer = new tokenizer_type(grammer , input);

    tokenizer->register_token_handler('{' , [](basic_tokenizer_interface<basic_object_interface> & tokenizer) ->  basic_object_interface &{
            return basic_object_interface::build<json_object_v1>(static_cast<json_tokenizer &>(tokenizer)); 
    });

    tokenizer->register_token_handler('[' , [](basic_tokenizer_interface<basic_object_interface> & tokenizer) -> basic_object_interface &{
            return basic_object_interface::build<json_array_v1>(static_cast<json_tokenizer &>(tokenizer));
    });

    tokenizer->register_token_handler('\"' , [](basic_tokenizer_interface<basic_object_interface> & tokenizer) ->  basic_object_interface&{
            return basic_object_interface::build<json_string_v1>(static_cast<json_tokenizer &>(tokenizer));
    });
    for(int i = 0 ; i < 10 ; i++){
        tokenizer->register_token_handler(char('0' + i) , [](basic_tokenizer_interface<basic_object_interface> & tokenizer) -> basic_object_interface & {
               return  basic_object_interface::build<json_integer_v1>(static_cast<json_tokenizer &>(tokenizer));
        });
    }

    tokenizer->register_token_handler('f' ,[](basic_tokenizer_interface<basic_object_interface> & tokenizer)-> basic_object_interface &{
            return basic_object_interface::build<json_boolean_v1>(static_cast<json_tokenizer &>(tokenizer)); 
    });
    tokenizer->register_token_handler('t' ,[](basic_tokenizer_interface<basic_object_interface> & tokenizer) -> basic_object_interface &{
            return basic_object_interface::build<json_boolean_v1>(static_cast<json_tokenizer &>(tokenizer));  
    });
    tokenizer->register_token_handler('n' , [](basic_tokenizer_interface<basic_object_interface> & tokenizer ) -> basic_object_interface &{
            return basic_object_interface::build<json_null_v1> (static_cast<json_tokenizer &>( tokenizer));
            });
    
    return tokenizer;
}

template<typename json_object_type>
json_object_type & json_builder(const char * input){
    json_tokenizer & tokenizer = *tokenizer_builder<json_tokenizer>(input);
    return *new json_object_type(tokenizer);
}

#endif
