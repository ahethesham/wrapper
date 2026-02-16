#ifndef __JSON_STRING_H__
#define __JSON_STRING_H__

#include "token.h"
#include "json_fwd.h"

jsonString::jsonString(Tokenizer & tokenizer , token * current){
    if(current->type_ != TokenType::String)
        throw std::runtime_error("unexpected json token ");
    value_ = std::move(current->value_);
}
jsonString::jsonString(Tokenizer & tokenizer){
    auto token = tokenizer.getNext();
    assert(token->type_ == TokenType::String);
    value_ = std::move(token->value_);
}

jsonString::jsonString(const char * value) : value_(std::move(std::string(value))){}
    

std::string & jsonString::operator=(std::string &str) {
    return value_ = str;
}

std::string & jsonString::get() {
    return value_;
}

std::string jsonString::serialize(){
    return std::string('\"' + value_ + '\"');
}

#endif
