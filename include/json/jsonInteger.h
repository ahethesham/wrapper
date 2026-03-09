#ifndef __JSON_INTEGER_H__
#define __JSON_INTEGER_H__
#include "json_fwd.h"

jsonInteger::jsonInteger(Tokenizer &tokenizer , token * current):value_(*(int *)malloc(sizeof(int))){
    value_ = 0;
    if(current == nullptr || current->type_ != TokenType::Number)
        throw std::runtime_error("unexpected json integer");
    value_ = std::atoi((const char *)current->value_.c_str());
}
int & jsonInteger::operator=(int & a) {
    return value_ = a;
}
int & jsonInteger::get() {
    return value_;
}
std::string jsonInteger::serialize(){
    return std::to_string(value_);
}

jsonInteger & jsonInteger::clear(){
    value_ = 0;
    return *this;
}

#endif
