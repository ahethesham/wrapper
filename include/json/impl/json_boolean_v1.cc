#include "json_boolean_v1.h"
#include "basic_json_tokenizer_interface.h"

json_boolean_v1::json_boolean_v1(basic_json_tokenizer_interface &  tokenizer){
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


void json_boolean_v1::parse(basic_json_tokenizer_interface &   tokenizer){

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

std::unique_ptr<basic_object_interface> json_boolean_v1::clone(){
    return std::make_unique<json_boolean_v1>(*this);
}
