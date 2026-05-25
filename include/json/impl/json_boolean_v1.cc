#include "json_boolean_v1.h"
#include "basic_object_interface.h"
#include "json_builder.h"

json_boolean_v1::json_boolean_v1(json_tokenizer &  tokenizer){
    parse(tokenizer);
}
json_boolean_v1::json_boolean_v1(std::shared_ptr<basic_tokenizer_interface<basic_object_interface>> tokenizer){
    parse(tokenizer);
}

json_boolean_v1::json_boolean_v1(bool val):value_(val){}
json_boolean_v1::json_boolean_v1(json_boolean_v1 & obj){
    value_ = obj.value_;
}
json_boolean_v1::json_boolean_v1(json_boolean_v1 && obj){
    value_ = obj.value_;
    obj.value_ = false;
}


bool & json_boolean_v1::value(){
    return value_;
}

std::string json_boolean_v1::serialize(){
    return  value_ ? "true" :  "false";
}


std::string json_boolean_v1::serialize(basic_formatter_interface & formatter){
    return serialize();
}


void json_boolean_v1::parse(json_tokenizer &   tokenizer){
#if 0
    char ch = tokenizer.peek()->get();
    assert(ch == 't' || ch == 'f');
    std::string temp = "";
    do{
        ch = tokenizer.peek_next_valid_char();
        if(!(ch >= 'a' && ch <= 'z'))break;
        ch = tokenizer.get_next_valid_char();
        temp += ch;
    }while(1);
    assert(temp == "true" ||
            temp == "false");
    value_ = (temp == "true") ? true : false;
#endif
    return ;
}

void json_boolean_v1::parse(std::shared_ptr<basic_tokenizer_interface<basic_object_interface>> tokenizer){
    char ch = tokenizer->peek()->get();
    assert(ch == 't' || ch == 'f');
    std::string temp = "";
    do{
        ch = tokenizer->peek_next_valid_char();
        if(!(ch >= 'a' && ch <= 'z'))break;
        ch = tokenizer->get_next_valid_char();
        temp += ch;
    }while(1);
    assert(temp == "true" ||
            temp == "false");
    value_ = (temp == "true") ? true : false;
    return ;
}

json_boolean_v1 & json_boolean_v1::operator=(json_boolean_v1 & obj){
    if(&obj == this)return *this;
    value_ = obj.value_;
    return *this;
}

json_boolean_v1 & json_boolean_v1::operator=(json_boolean_v1 && obj){
    if(&obj == this)return *this;
    value_ = obj.value_;
    obj.value_ = false;

    return *this;

    
}

std::shared_ptr<basic_object_interface> json_boolean_v1::clone(){
    return std::make_shared<json_boolean_v1>(*this);
}
std::string json_boolean_v1::get_body_type(){
    return "application/bool";
}

// parse methods
json_boolean_v1::buffer_type * json_boolean_v1::buffer(){
    assert(0);
    return nullptr;
}
void json_boolean_v1::parse(buffer_type * buffer){
    char ch = *(buffer->data + buffer->head);
    assert(ch == 't' || ch == 'f' );
    auto tokenizer = tokenizer_builder<json_tokenizer>(buffer->data + buffer->head);
    parse(tokenizer);
}
void json_boolean_v1::at_eof(buffer_type * buffer){
    //TODO
    return ;
}
bool json_boolean_v1::continue_reading(){
    return false;
}

json_boolean_v1 & json_boolean_v1::clear(){
    value_ = false;
    return *this;
}

int json_boolean_v1::size(){
    return value_ ? sizeof(char) * 4 : sizeof(char) * 5;
}

bool json_boolean_v1::verify_body_type(const std::string & str){
    return get_body_type() == str;
}
