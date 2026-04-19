
#include "json_string_v1.h"
#include "basic_json_tokenizer_interface.h"
#include "file_logger_v1.h"

json_string_v1::json_string_v1(basic_json_tokenizer_interface & tokenizer) : value_(new std::string()){
    parse(tokenizer);
}

json_string_v1::json_string_v1(json_string_v1 & obj) : value_(nullptr){
    value_ = new std::string(*obj.value_);
}
json_string_v1::json_string_v1(json_string_v1 && obj) : value_(nullptr){
    value_ = obj.value_;
    obj.value_ = nullptr;
}
json_string_v1::json_string_v1(const char * input) : value_(new std::string(input)){}

std::string & json_string_v1::value()  {
    return *value_;
}

std::string json_string_v1::serialize(){
    return std::string("\"" + *value_ + "\"");
}
std::string json_string_v1::serialize(basic_formatter_interface & formatter){
    return std::string("\"" + *value_ + "\"");
}

void json_string_v1::parse(basic_json_tokenizer_interface & tokenizer){
    auto token = tokenizer.getNext();
    assert(token->compare('\"'));
    //++(tokenizer);
    char ch;
    do{
        ch = tokenizer.get_next_char();
        if(ch == '\"')break;
        (*value_) += ch;
    }while(1);

    return ;
}

/*
 * copy assignment 
 */
json_string_v1 & json_string_v1::operator=(json_string_v1 & obj){
    if(&obj == this)return *this;

    if(value_ != nullptr)
        delete value_;
    value_ = new std::string(*obj.value_);

    return *this;
}

/*
 * move assignment 
 */
json_string_v1 & json_string_v1::operator=(json_string_v1 && obj) {
    if(& obj == this)
        return *this;

    if(value_ != nullptr)delete value_;

    value_ = obj.value_;

    obj.value_ = nullptr;
    return *this;
}

std::unique_ptr<basic_object_interface > json_string_v1::clone(){
    return std::make_unique<json_string_v1 >(*this);
}
