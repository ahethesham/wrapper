#ifndef __JSON_STRING_H__
#define __JSON_STRING_H__

#include "json_fwd.h"

template<typename token_policy ,
         typename buffer_policy>
 class json_string_impl{
     public:
         using value_type = std::string;
         using tokenizer_type = token_policy;
         using buffer_type  = buffer_policy;
        using token_handler  = token_policy::token_handler;

         json_string_impl(tokenizer_type * tokenizer ){
                auto token = tokenizer->getNext();
                assert(token->compare('\"'));
                ++(*tokenizer);
                char ch;
                do{
                    ch = tokenizer->get_next_char();
                    if(ch == '\"')break;
                    value_ += ch;
                }while(1);
         }
         json_string_impl(json_string_impl && impl) : value_(std::move(impl.get())){}

         value_type & get(){
            return value_;
         }
         
         bool operator==(value_type v){
            return (strcmp(value_.c_str() , v.c_str()) == 0);
         }

         std::string serialize(){
            return value_;
         }

         json_string_impl & operator=(value_type v){
            value_ = v;
            return *this;
         }
     private:
         value_type value_;
 };


template<typename token_policy ,
         typename buffer_policy>
 class json_null_impl{
     public:
         using value_type = std::string  ;
         using tokenizer_type = token_policy;
         using buffer_type = buffer_policy;
        using token_handler  = token_policy::token_handler;

         json_null_impl(tokenizer_type * tokenizer){
             do{
                 char ch = tokenizer->peek_next_char();
                 if(!(ch == 'n' || ch == 'u' || ch == 'l' || ch == 'l')){
                     break;
                 }
                 ch = tokenizer->get_next_char();
                 value_ += ch;
             }while(1);
         }

         value_type & get(){
            return value_;
         }
         json_null_impl & operator=(value_type v){
            value_ = v;
            return *this;
         }

         std::string serialize(){
            return value_;
         }
     private:
         value_type value_;
 };

template<typename token_policy ,
        typename buffer_policy>
class json_int_impl{
    public:
     using value_type = long long  ;
     using tokenizer_type = token_policy;
     using buffer_type  = buffer_policy;
    using token_handler  = token_policy::token_handler;

     json_int_impl(tokenizer_type *  tokenizer){
         char ch = tokenizer->getNext()->get();
         bool isNegetive = false;
         assert((ch >= '0' && ch <= '9') || ch == '-');
         value_ = 0;
         if(ch == '-'){
             isNegetive = true;
             tokenizer->get_next_char();
         }
         do{
             ch = tokenizer->peek_next_char();
             if(!(ch >= '0' && ch <= '9'))break;
             ch = tokenizer->get_next_char();
             value_ = value_ * 10  + (ch - '0');
         }while(1);
     }

     std::string serialize(){
        return std::to_string(value_);
     }

     value_type & get(){
        return value_;
     }
    private:
     value_type value_;

};

template<typename token_policy ,
         typename buffer_policy >
 class json_boolean_impl{
     public:
        using buffer_type = buffer_policy;
        using value_type  = bool;
        using tokenizer_type = token_policy;

        using token_handler  = token_policy::token_handler;

        json_boolean_impl(tokenizer_type * tokenizer){
            char ch = tokenizer->peek_next_char();
            assert(ch == 't' || ch == 'f');
            std::string temp = "";
            do{
                ch = tokenizer->peek_next_char();
                if(!(ch >= 'a' && ch <= 'z'))break;
                ch = tokenizer->get_next_char();
                temp += ch;
            }while(1);
            assert(temp == "true" ||
                    temp == "false");
            value_ = (temp == "true") ? true : false;
        }

        std::string serialize(){
            return value_ ? "true" : "false";
        }

        value_type & get(){
            return value_;
        }

     private:
        value_type value_;
 };

#endif
