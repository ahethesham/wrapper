#ifndef __JSON_FWD_H__
#define __JSON_FWD_H__

#include <memory>
#include <string>
#include <variant>
#include "buffer.h"
#include "json_string_v1.h"
#include "json_object_v1.h"
#include "json_array_v1.h"
#include "json_integer_v1.h"
#include "json_boolean_v1.h"
#include "json_null_v1.h"

 

#if 0
template < typename json_interface_policy ,
           typename json_tokenizer_policy>
class basic_token ;

template < typename tokenizer_policy  ,
           typename buffer_policy>
class json_primitive_interface;

class json_grammer;

template<typename impl >
class basic_tokenizer;

template<typename impl>
class basic_object ;
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
template<typename  token_policy ,
         typename grammer_policy ,
         typename  buffer_policy>
class tokenizer_impl;

class token_v1;
class token_v2;

using  json_tokenizer = basic_tokenizer< tokenizer_impl<token_v1 ,json_grammer , buffer_v1>>;

using  json_tokenizer_2 = basic_tokenizer< tokenizer_impl<token_v2 ,json_grammer , buffer_v1>>;

using basic_json_interface = json_primitive_interface< json_tokenizer_2 , buffer_v1>;


template<typename T ,
         typename return_type >
class Iterator;


#endif
#if 0
using json_array = basic_object<json_array_impl<json_tokenizer>>;
using json_object = basic_object<json_object_impl<json_tokenizer>>;
using json_string = basic_object<json_string_impl<json_tokenizer>>;
using json_null   = basic_object<json_null_impl<json_tokenizer>>;
using json_int    = basic_object<json_int_impl<json_tokenizer>>;
using json_bool    = basic_object<json_boolean_impl<json_tokenizer>>;
#else
class json_array_v1;
class json_object_v1;
class json_string_v1;
class json_integer_v1;
class json_boolean_v1;
class json_null_v1;
using json_array = json_array_v1;
using json_object = json_object_v1;
using json_string = json_string_v1;
using json_null   = json_null_v1;
using json_int    = json_integer_v1;
using json_bool    = json_boolean_v1;
#endif
#if 0
using variants = std::variant<json_array * , json_object * , json_string * , json_null *, json_int * , json_bool *>;
#endif
#endif


