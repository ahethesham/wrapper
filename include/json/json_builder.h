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
#include <memory>

template< typename tokenizer_type>
inline std::shared_ptr<tokenizer_type> tokenizer_builder(const char * input)
{

    static Logger &  logger =  Logger::build();
    json_grammer_v1 & grammer = *new json_grammer_v1(" :\t\r\n," , "" , '\0');

    std::shared_ptr<tokenizer_type> tokenizer = std::make_shared<tokenizer_type>(grammer , input);

    tokenizer->register_token_handler('{' , [](std::shared_ptr<basic_tokenizer_interface<basic_object_interface>> tokenizer) {
            //return basic_object_interface::build<json_object_v1>(static_cast<tokenizer_type &>(tokenizer)); 
            return std::make_shared<json_object_v1>(tokenizer);
    });

    tokenizer->register_token_handler('[' , [](std::shared_ptr<basic_tokenizer_interface<basic_object_interface>> tokenizer) {
            return std::make_shared<json_array_v1> (tokenizer);
    });

    tokenizer->register_token_handler('\"' , [](std::shared_ptr<basic_tokenizer_interface<basic_object_interface>> tokenizer) {
            return std::make_shared<json_string_v1> (tokenizer);
    });
    for(int i = 0 ; i < 10 ; i++){
        tokenizer->register_token_handler(char('0' + i) , [](std::shared_ptr<basic_tokenizer_interface<basic_object_interface>> tokenizer)  {
            return std::make_shared<json_integer_v1> (tokenizer);
        });
    }

    tokenizer->register_token_handler('f' ,[](std::shared_ptr<basic_tokenizer_interface<basic_object_interface>> tokenizer){
            return std::make_shared<json_boolean_v1> (tokenizer);
    });
    tokenizer->register_token_handler('t' ,[]( std::shared_ptr<basic_tokenizer_interface<basic_object_interface>>tokenizer) {
            return std::make_shared<json_boolean_v1> (tokenizer);
    });
    tokenizer->register_token_handler('n' , [](std::shared_ptr<basic_tokenizer_interface<basic_object_interface>> tokenizer ) {
            return std::make_shared<json_null_v1> (tokenizer);
    });
    
    return tokenizer;
}

template<typename json_object_type>
json_object_type & json_builder(const char * input){
    auto  tokenizer = tokenizer_builder<json_tokenizer>(input);
    return *new json_object_type(tokenizer);
}

#endif
