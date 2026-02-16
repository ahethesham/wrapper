#ifndef __JSON_H__
#define __JSON_H__
#include <cassert>
#include <map>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "stream.h"
#include "token.h"
#include "json_fwd.h"


class json : public basic_json{
    char * input_;
    jsonObject * root_;
    public:
       json(Stream &stream);
       json(std::string &input);
       json() : root_(new jsonObject()){}
       json(const char *input){
           std::vector<char> pass = {' ' , '\r' ,'\n' , '\t' , ',' };
            Tokenizer tokenizer(input , pass);
            if(!tokenizer.hasNext() || tokenizer.getNext()->type_ != TokenType::OpenBracket)
                throw std::runtime_error("unexpected json file provided ");
            root_ = new jsonObject(tokenizer);
       }
       void create(const char * input){
           std::vector<char> pass = {' ' , '\r' ,'\n' , '\t' , ',' };
            Tokenizer tokenizer(input , pass);
            if(!tokenizer.hasNext() || tokenizer.getNext()->type_ != TokenType::OpenBracket)
                throw std::runtime_error("unexpected json file provided ");
            root_ = new jsonObject(tokenizer);
       }

        basic_json & operator[](const char *key){
           return (*root_)[key];
       }
        
       std::string serialize() override{
           return root_->serialize();
       }
       jsonObject & get(){return *root_;}

       template<typename T>
       jsonType<T> * get(const char *key);


};


#endif
