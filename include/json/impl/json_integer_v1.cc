
#include "json_integer_v1.h"
#include "basic_object_interface.h"
#include "json_builder.h"

json_integer_v1::json_integer_v1(json_tokenizer & tokenizer) : value_(0){
    throw std::runtime_error("depreciated constructor for json integer ");
}
json_integer_v1::json_integer_v1(std::shared_ptr<basic_tokenizer_interface<basic_object_interface>> tokenizer) : value_(0){
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

void  json_integer_v1::parse(json_tokenizer & tokenizer){
#if 0
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
#endif
     return ;
}
void json_integer_v1::parse(std::shared_ptr<basic_tokenizer_interface<basic_object_interface>> tokenizer){
     char ch = tokenizer->getNext()->get();
     bool isNegetive = false;
     assert((ch >= '0' && ch <= '9') || ch == '-');
     if(ch == '-'){
         isNegetive = true;
         tokenizer->get_next_char();
     }else{
         value_ = ch - '0';
     }
     do{
         ch = tokenizer->peek_next_char();
         if(!(ch >= '0' && ch <= '9'))break;
         ch = tokenizer->get_next_char();
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

std::shared_ptr<basic_object_interface> json_integer_v1::clone(){
    return std::make_shared<json_integer_v1>(*this);
}

std::string json_integer_v1::get_body_type(){
    return "application/integer";
}

//parse mthods 
json_integer_v1::buffer_type * json_integer_v1::buffer(){
    assert(false);
    return nullptr;
}
void json_integer_v1::parse(json_integer_v1::buffer_type * buffer){
    char ch = *(buffer->data + buffer->head);
    assert(ch == '-' || ( ch >= '0' && ch <= '9'));
    auto tokenizer = tokenizer_builder<json_tokenizer>(buffer->data + buffer->head);
    parse(tokenizer);
}
void json_integer_v1::at_eof(json_integer_v1::buffer_type * buffer){
    // TODO
    return ;
}
bool json_integer_v1::continue_reading(){
    return false;
}

json_integer_v1 & json_integer_v1::clear(){
    value_ = -1;
    return *this;
}

int json_integer_v1::size(){
    return (std::to_string(value_).size());
}

bool json_integer_v1::verify_body_type(const std::string & type){
    return get_body_type() == type;
}
