#ifndef __JSON_ARRAY_H__
#define __JSON_ARRAY_H__

#include "token.h"
#include "json_fwd.h"
#include <memory>

// custom data type for the json array
struct list{
    std::vector<std::shared_ptr<basic_json >> children_;
    basic_json & operator[](int idx){
        return *children_[idx];
    }
    list & push_back(std::shared_ptr<basic_json> ptr){
        children_.push_back(ptr);
        return  *this;
    }
    std::vector<std::shared_ptr<basic_json>> & get() {return children_;}
    list() : children_(std::vector< std::shared_ptr<basic_json> > (0,nullptr)){}
};

basic_json & jsonArray::operator[](int idx) { return (value_[idx]); }

list & jsonArray::operator=(list &arr) {return value_ = arr;}

list & jsonArray::get() {return value_;}

jsonObject & jsonArray::getAsObject(int idx) { return (dynamic_cast<jsonObject&>(value_[idx])); }

jsonArray & jsonArray::getAsArray(int idx) { return dynamic_cast<jsonArray&>(value_[idx]); }

jsonInteger & jsonArray::getAsInteger(int idx) { return dynamic_cast<jsonInteger&>(value_[idx]); }

jsonString & jsonArray::getAsString(int idx) { return dynamic_cast<jsonString&>(value_[idx]); }

jsonBoolean & jsonArray::getAsBoolean(int idx) { return dynamic_cast<jsonBoolean &>(value_[idx]); }

jsonArray & jsonArray::push(std::shared_ptr<basic_json> obj) { 
    assert(obj); 
    value_.push_back(obj);
    return *this;  
}


jsonArray::jsonArray(Tokenizer & tokenizer  ) : value_(*new list()){

        while(tokenizer.hasNext()){
            auto next = tokenizer.getNext();
            if(next->type_ == TokenType::ClosedBrace)break;
            switch(next->type_){
                case TokenType::OpenBracket:
                    value_.push_back(std::make_shared<jsonObject>(tokenizer ));
                    break;
                case TokenType::OpenBrace:
                    value_.push_back(std::make_shared<jsonArray>(tokenizer ));
                    break;
                case TokenType::String:
                    value_.push_back(std::make_shared<jsonString>(tokenizer , next));
                    break;
                case TokenType::Number:
                    value_.push_back(std::make_shared<jsonInteger>(tokenizer , next));
                    break;
                case TokenType::Bool:
                    value_.push_back( std::make_shared<jsonBoolean>(tokenizer , next));
                    break;
                default:
                    throw std::runtime_error("unexpected json file");
                    break;

            }
        }
}
jsonArray::jsonArray(  ) : value_(*new list()){}



std::string jsonArray::serialize(){
    // must serialize and return a string 
    std::string res = "";
    res += '[';
    for(auto  child : value_.get()){
        res += (child)->serialize();
        res += ',';
    }
    if(res[res.size() - 1] == ',')res.erase(res.size() - 1 , 1);

    res += ']';
    return res;
    
}

#endif
