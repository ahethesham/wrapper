#ifndef __BASIC_OBJECT_H
#define __BASIC_OBJECT_H
#include "basic_formatter_interface.h"
#include "basic_tokenizer_interface.h"
#include "file_logger_v1.h"
#include "json_tokenizer_v1.h"
#include <memory>
#include <cassert>
#include <string>

class basic_object_interface;

template class json_tokenizer_v1< token_v1<basic_object_interface>, buffer_v1 ,basic_object_interface>;

using json_tokenizer = json_tokenizer_v1< token_v1<basic_object_interface>, buffer_v1 ,basic_object_interface>;
/*
 * Basic interface for all the json object types ...and each object should be parsable
 */
class basic_object_interface : public basic_parser_interface<buffer_v1>{
    public:
         template<typename T>
            T & get() {
                return (T&)*this;
            }

          template<typename T>
          static T& build(json_tokenizer & tokenizer) {
                return *(new T(tokenizer));
           }

          template<typename T>
              T& value() {
                  assert(false) ;
              }

          virtual std::string serialize() = 0;
          virtual std::string serialize(basic_formatter_interface & formatter) = 0;

          virtual void parse(json_tokenizer & tokenizer) = 0;

          //parser methods
           virtual void parse(buffer_type * buffer) override = 0 ;
           //virtual buffer_type * buffer() override = 0;
           //virtual void at_eof(buffer_type * buffer) override = 0 ;
           //virtual bool continue_reading() override = 0 ;

          //no copy assignment
          virtual basic_object_interface & operator=(basic_object_interface &) = delete;
        //no move assignment 
          virtual basic_object_interface & operator=(basic_object_interface && ) = delete;

          basic_object_interface(basic_object_interface &) = delete;
          basic_object_interface(basic_object_interface  &&) = delete;
          basic_object_interface() = default;

          virtual ~basic_object_interface() {};

          virtual std::shared_ptr<basic_object_interface> clone() = 0;

          virtual std::string get_body_type() = 0;

          virtual basic_object_interface & clear() = 0;
};
#endif
