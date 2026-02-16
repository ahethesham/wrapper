#ifndef __JSON_BOOL_H__
#define __JSON_BOOL_H__
#include "token.h"
#include "json_fwd.h"

jsonBoolean::jsonBoolean(Tokenizer &tokenizer , token *current) : value_(*new bool) {
   if(current == nullptr || current->type_ != TokenType::Bool)
       throw std::runtime_error("unexpected boolean value");
   value_ = (current->value_ == "true" ) ? true : false;
}
bool & jsonBoolean::operator=(bool &val)  {
    return value_;
}
bool & jsonBoolean::get() {
    return value_;
}
std::string jsonBoolean::serialize(){
    if(value_)return "true";
    return "false";
}
#endif
