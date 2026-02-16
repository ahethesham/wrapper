#ifndef __JSON_OBJECT_H__
#define __JSON_OBJECT_H__

#include "json_fwd.h"
#include "token.h"
#include <cstring>
#include <memory>
#include <iostream>
#include <map>
#include <ostream>

/* custom data type for the json object */
struct object{
    std::map<std::string , std::shared_ptr<basic_json> > children_;
    basic_json &operator[](const char *key){
        return *children_[std::string(key)];
    } 
    object() : children_(std::map<std::string , std::shared_ptr<basic_json> >()) {}
    object & add(std::string & key , std::shared_ptr<basic_json> value){
        children_[key] = value;
        return *this;
    }
    object & add(const char * key , std::shared_ptr<basic_json> value){
        children_[key] = value;
        return *this;
    }
    std::map<std::string , std::shared_ptr<basic_json> > & get(){return children_;}
};

jsonObject::jsonObject(Tokenizer &tokenizer ) : value_(*new object()){

       while(tokenizer.hasNext()){
            auto next = tokenizer.getNext();
            // json has completed 
            if(next->type_ == TokenType::ClosedBracket)break;
            // keys always has to be strings 
            if(next->type_ != TokenType::String)throw std::runtime_error("unexpected json 003");
            std::string key = next->value_;
            // got the key ... extract the value now
            next = tokenizer.getNext();
            if(next->type_ != TokenType::Colon)throw std::runtime_error("unexpected json 004");
            next = tokenizer.getNext();
            switch(next->type_){
                case TokenType::OpenBracket:
                    value_.add(key , std::make_shared<jsonObject>(tokenizer));
                    break;
                case TokenType::OpenBrace:
                    value_.add(key , std::make_shared<jsonArray>(tokenizer));
                    break;
                case TokenType::String:
                    value_.add(key , std::make_shared<jsonString>(tokenizer,next));
                    break;
                case TokenType::Number:
                    value_.add(key , std::make_shared<jsonInteger>(tokenizer , next));
                    break;
                case TokenType::Bool:
                    value_.add(key , std::make_shared<jsonBoolean>(tokenizer , next));
                    break;
                default:
                    throw std::runtime_error("unexpected json file");
                    break;
        }
   }
}

jsonObject::jsonObject( ) : value_(*new object()){}
    
basic_json & jsonObject::operator[](const char * key) {
    return  value_[key];
}

object & jsonObject::operator=(object & value) {return this->value_;}

object &  jsonObject::get() {return value_;}

jsonObject & jsonObject::getAsObject(const char * key)  { return (jsonObject &)value_[key]; }
jsonString & jsonObject::getAsString(const char * key) {return (jsonString &)value_[key];}
jsonInteger & jsonObject::getAsInteger(const char * key) {return (jsonInteger &)value_[key];}
jsonBoolean & jsonObject::getAsBoolean(const char * key) {return (jsonBoolean&)value_[key];}
jsonArray & jsonObject::getAsArray(const char * key) {return (jsonArray&)value_[key];}

jsonObject & jsonObject::push(const char *key, std::shared_ptr<basic_json> obj) {
    assert(obj != nullptr);
    value_.add(key , obj);
    return *this;
}

std::string  jsonObject::serialize() {
    std::string res  = "";
    res += "{";
    for(auto & itr : value_.get()){
        res += '\n';
        res += '\t';
        res += '\"';
        res += (itr.first) ;
        res += '\"';
        res += ": ";
        auto secondaryString = (itr.second)->serialize();
        std::string temp = "";
        for(int i = 0 ; i < secondaryString.size() ; i++){
            if(secondaryString[i] == '\t' || secondaryString[i] == '}'){
                temp += '\t';
            }
            temp+=secondaryString[i];
        }
        res += temp;
        res += ',';
    }
    if(res[res.size() - 1] == ',')res.erase(res.size() - 1 , 1);
    res += "\n}";
    return res;
}

#endif
