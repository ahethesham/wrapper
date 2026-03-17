#ifndef __JSON_FWD_H__
#define __JSON_FWD_H__

#include <memory>
#include <string>
#include <variant>
#include "buffer.h"
#include "logger.h"


 

template<typename impl>
class basic_object ;

template<typename impl >
class basic_tokenizer;

class token_v1;
class json_grammer;

template<typename  token_policy ,
         typename grammer_policy ,
         typename  buffer_policy>
class tokenizer_impl;

using  json_tokenizer = basic_tokenizer<tokenizer_impl<token_v1 ,json_grammer , buffer_v1>>;

template<typename T ,
         typename return_type >
class Iterator;

template< typename token_policy ,
          typename buffer_policy = buffer_v1 >
class json_array_impl;

template< typename token_policy ,
          typename buffer_policy = buffer_v1 >
class json_string_impl;

template< typename token_policy ,
          typename buffer_policy = buffer_v1 >
class json_null_impl;

template< typename token_policy ,
          typename buffer_policy = buffer_v1 >
class json_object_impl;

template<typename token_policy ,
         typename buffer_policy = buffer_v1>
 class json_int_impl;

template<typename token_policy ,
         typename buffer_policy = buffer_v1>
 class json_boolean_impl;



using json_array = basic_object<json_array_impl<json_tokenizer>>;
using json_object = basic_object<json_object_impl<json_tokenizer>>;
using json_string = basic_object<json_string_impl<json_tokenizer>>;
using json_null   = basic_object<json_null_impl<json_tokenizer>>;
using json_int    = basic_object<json_int_impl<json_tokenizer>>;
using json_bool    = basic_object<json_boolean_impl<json_tokenizer>>;

using variants = std::variant<json_array * , json_object * , json_string * , json_null *, json_int * , json_bool *>;

#endif


#include "basic_object.h"
#include "jsonArray.h"
#include "jsonObject.h"
#include "jsonString.h"
#include "token.h"
#include "buffer.h"


