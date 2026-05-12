#include  "json_null_v1.h"
#include "basic_object_interface.h"
#include "json_builder.h"

json_null_v1::json_null_v1(json_tokenizer &  tokenizer){
    parse(tokenizer);
}

json_null_v1::json_null_v1() : value_(new std::string("null")) {}

json_null_v1::json_null_v1(json_null_v1 & obj) {
    value_ = new std::string(*obj.value_);
}
json_null_v1::json_null_v1(json_null_v1 && obj){
    value_ = obj.value_;
    obj.value_ = nullptr;
    return ;
}

std::string & json_null_v1::value(){
    return  *value_;
}

void json_null_v1::parse(json_tokenizer &  tokenizer){
     do{
         char ch = tokenizer.peek_next_char();
         if(!(ch == 'n' || ch == 'u' || ch == 'l' || ch == 'l')){
             break;
         }
         ch = tokenizer.get_next_char();
         value_ += ch;
     }while(1);
}

std::string json_null_v1::serialize(){
    return *value_;
}

std::string json_null_v1::serialize(basic_formatter_interface & formatter){
    return serialize();
}

bool json_null_v1::operator==(json_null_v1 & cmp){
    return cmp.value() == *value_;
}

json_null_v1 & json_null_v1::operator=(json_null_v1 &  cmp){
    if(&cmp == this)return *this;

    if(value_ != nullptr)delete value_;
    value_ = new std::string(*cmp.value_);
    return *this;
}
json_null_v1 & json_null_v1::operator=(json_null_v1 && obj){
    if(&obj == this)return *this;
    if(value_ != nullptr)delete value_;
    value_ = obj.value_;
    obj.value_ = nullptr;
    return *this;
}



std::shared_ptr<basic_object_interface> json_null_v1::clone(){
    return std::make_shared<json_null_v1>(*this);
}

std::string json_null_v1::get_body_type(){
    return "application/null";
}

//parse methods
json_null_v1::buffer_type * json_null_v1::buffer(){
    assert(0);
    return nullptr;
}
void json_null_v1::parse(buffer_type * buffer){
    char ch = *(buffer->data + buffer->head);
    assert(ch == 'n' );
    json_tokenizer * tokenizer = tokenizer_builder<json_tokenizer>(buffer->data + buffer->head);
    parse(*tokenizer);
}
void json_null_v1::at_eof(buffer_type * buffer){
    //TODO
    return ;
}
bool json_null_v1::continue_reading(){
    return false;
}

json_null_v1 & json_null_v1::clear(){
    value_->clear();
    return *this;
}

