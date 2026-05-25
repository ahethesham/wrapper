
#include "json_string_v1.h"
#include "basic_object_interface.h"
#include "file_logger_v1.h"
#include "json_builder.h"

json_string_v1::json_string_v1(json_tokenizer & tokenizer) : value_(new std::string()){
    parse(tokenizer);
}

json_string_v1::json_string_v1(std::shared_ptr<basic_tokenizer_interface<basic_object_interface>> tokenizer) : value_(new std::string()){
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
json_string_v1::json_string_v1(const std::string & input) : value_(new std::string(input)){}

std::string & json_string_v1::value()  {
    return *value_;
}

std::string json_string_v1::serialize(){
    return std::string("\"" + *value_ + "\"");
}
std::string json_string_v1::serialize(basic_formatter_interface & formatter){
    return std::string("\"" + *value_ + "\"");
}

void json_string_v1::parse(json_tokenizer & tokenizer){
    auto token = tokenizer.getNext();
    assert((token->compare('\'') || token->compare('\"') ) && value_ != nullptr);
    char ch;
    do{
        ch = tokenizer.get_next_char();
        if(ch == '\"')break;
        (*value_) += ch;
    }while(1);

    return ;
}
void json_string_v1::parse(std::shared_ptr<basic_tokenizer_interface<basic_object_interface>> tokenizer){
    auto token = tokenizer->getNext();
    assert((token->compare('\'') || token->compare('\"') )&& value_ != nullptr);
    char ch;
    do{
        ch = tokenizer->get_next_char();
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
    if(&obj == this)
        return *this;
    if(value_ != nullptr)delete value_;
    value_ = obj.value_;
    obj.value_ = nullptr;
    return *this;
}

std::shared_ptr<basic_object_interface > json_string_v1::clone(){
    return std::make_shared<json_string_v1 >(*this);
}

std::string json_string_v1::get_body_type(){
     return "application/string";
}


// parse methods
json_string_v1::buffer_type * json_string_v1::buffer(){
    assert(0);
    return nullptr;
}
void json_string_v1::parse(buffer_type * buffer){
    char ch = *(buffer->data + buffer->head);
    assert(ch == 't' || ch == 'f' );
    auto tokenizer = tokenizer_builder<json_tokenizer>(buffer->data + buffer->head);
    parse(tokenizer);
}

void json_string_v1::at_eof(buffer_type * buffer){
    //TODO
    return ;
}
bool json_string_v1::continue_reading(){
    return false;
}

json_string_v1 & json_string_v1::clear(){
    value_->clear();
    delete value_ ;
    value_ = nullptr;
    return *this;
}

int json_string_v1::size(){
    return value_->size() + 2;
}

bool json_string_v1::verify_body_type(const std::string & str){
    return get_body_type() == str ;

}
