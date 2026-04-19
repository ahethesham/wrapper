#ifndef __BASIC_OBJECT_H
#define __BASIC_OBJECT_H
#include "basic_formatter_interface.h"
#include "file_logger_v1.h"
#include "basic_tokenizer_interface.h"
#include <memory>
#include <cassert>
#include <string>

using basic_json_tokenizer_interface = basic_tokenizer_interface<basic_object_interface>;
/*
 * Basic interface for all the json object types 
 */
class basic_object_interface{
    public:
         template<typename T>
            T & get() {
                return (T&)*this;
            }

          template<typename T>
          static T& build(basic_json_tokenizer_interface & tokenizer) {
                return *(new T(tokenizer));
             }
          template<typename T>
              T& value() ;

          virtual std::string serialize() = 0;
          virtual std::string serialize(basic_formatter_interface & formatter) = 0;

          virtual void parse(basic_json_tokenizer_interface & tokenizer) = 0;

          //no copy assignment
          virtual basic_object_interface & operator=(basic_object_interface &) = delete;
        //no move assignment 
          virtual basic_object_interface & operator=(basic_object_interface && ) = delete;

          basic_object_interface(basic_object_interface &) = delete;
          basic_object_interface(basic_object_interface &&) = delete;
          basic_object_interface() = default;

          virtual ~basic_object_interface() = default;

          virtual std::unique_ptr<basic_object_interface> clone() = 0;
};
#endif
