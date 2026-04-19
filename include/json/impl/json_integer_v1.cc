
#include "json_integer_v1.h"
#include "basic_json_tokenizer_interface.h"

json_integer_v1::json_integer_v1(basic_json_tokenizer_interface & tokenizer) : value_(0){
    parse(tokenizer);
}
json_integer_v1::json_integer_v1(json_integer_v1 & obj) : value_(obj.value_){}
json_integer_v1::json_integer_v1(json_integer_v1 && obj) : value_(obj.value_){
    obj.value_ = 0;
}
json_integer_v1::json_integer_v1(int val) : value_(val){}
int & json_integer_v1::value(){
    return value_;
}

std::string json_integer_v1::serialize(){
    return std::to_string(value_);
}

std::string json_integer_v1::serialize(basic_formatter_interface & formatter){
    return serialize();
}

void  json_integer_v1::parse(basic_json_tokenizer_interface & tokenizer){
     char ch = tokenizer.getNext()->get();
     bool isNegetive = false;
     assert((ch >= '0' && ch <= '9') || ch == '-');
     value_ = 0;
     if(ch == '-'){
         isNegetive = true;
         tokenizer.get_next_char();
     }
     do{
         ch = tokenizer.peek_next_char();
         if(!(ch >= '0' && ch <= '9'))break;
         ch = tokenizer.get_next_char();
         value_ = value_ * 10  + (ch - '0');
     }while(1);
     return ;
}

json_integer_v1 & json_integer_v1::operator=(json_integer_v1 & obj){
    value_ = obj.value_;
    return *this;
}

json_integer_v1 & json_integer_v1::operator=(json_integer_v1 && obj){
    value_ = obj.value_;
    obj.value_ = 0;
    return *this;
}

std::unique_ptr<basic_object_interface> json_integer_v1::clone(){
    return std::make_unique<json_integer_v1>(*this);
}
