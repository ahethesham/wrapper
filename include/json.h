#ifndef __JSON_H__
#define __JSON_H__
#include <cassert>
#include <map>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "stream.h"

enum class TokenType{
    Number , 
    String ,
    OpenBrace , // '['
    ClosedBrace , // ']'
    OpenBracket , // '{'
    ClosedBracket ,
    Bool,
    Null ,
    Colon ,
    Comma,
    End
};
struct token{
    TokenType type_;
    std::string  value_;
    token(TokenType type , std::string & value) : type_(type) , value_(std::move(value)){}
};
class Tokenizer{
    char *input_;
    int idx_;
    char seperator_;
    public:
        token * getNext();
        token *operator++(); // skip the token
        explicit Tokenizer(const char *input) : input_((char *)input) , idx_(0) {}
        bool hasNext(){return *(input_ + idx_) != '\0';}
};

template<typename T>
class jsonValue{
    int type_;
    public:
    // each corresponding concrete class should take care of initializing its value with the given 
    virtual T& operator=(T &value) = 0;
    virtual T& get() = 0;
};
class jsonBoolean : public jsonValue<bool>{
    bool value_;
    public:
    explicit jsonBoolean(Tokenizer &tokenizer , token *current){
       if(current == nullptr || current->type_ != TokenType::Bool)
           throw std::runtime_error("unexpected boolean value");
       value_ = (current->value_ == "true" ) ? true : false;
    }
    bool & operator=(bool &val) override {return value_;}
    bool & get() override {return value_;}
};
class jsonInteger : public jsonValue<int>{
    int value_;
    public:
    explicit jsonInteger(Tokenizer &tokenizer , token * current){
        if(current == nullptr || current->type_ != TokenType::Number)
            throw std::runtime_error("unexpected json integer");
        value_ = std::atoi((const char *)current->value_.c_str());
    }
    int & operator=(int & a) override {return value_;}
    int & get() override {return value_;}
};


class jsonString : public jsonValue<std::string>{
    std::string  value_;
    public:
    explicit jsonString(Tokenizer & tokenizer , token * current){
        if(current->type_ != TokenType::String)
            throw std::runtime_error("unexpected json file 001 ");
        value_ = std::move(current->value_);
    }
    explicit jsonString(Tokenizer & tokenizer){
        auto token = tokenizer.getNext();
        assert(token->type_ == TokenType::String);
        value_ = std::move(token->value_);
    }
    std::string & operator=(std::string &str) override {return value_;}
    std::string & get() override {return value_;}
};

class jsonArray : public jsonValue<std::vector<void *>>{
    std::vector<void *> array_;
    public:
    explicit jsonArray(Tokenizer &tokenizer);
    template<typename T>
    T* operator[](int idx){
        return (T*)array_[idx];
    }
    std::vector<void *> & operator=(std::vector<void *> &arr) override {return array_;}
    std::vector<void* > & get() override {return array_;}
};

class jsonObject : public jsonValue<std::map<std::string , void*> >{
    std::map<std::string , void *> nodes_;
    public:
    explicit jsonObject(Tokenizer &tokenizer){
       nodes_ = std::map<std::string , void *>(); 
       
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
                    nodes_[key] = (new jsonObject(tokenizer));
                    break;
                case TokenType::OpenBrace:
                    nodes_[key] = (new jsonArray(tokenizer));
                    break;
                case TokenType::String:
                    nodes_[key] = (new jsonString(tokenizer,next));
                    break;
                case TokenType::Number:
                    nodes_[key] = (new jsonInteger(tokenizer , next));
                    break;
                case TokenType::Bool:
                    nodes_[key] = (new jsonBoolean(tokenizer , next));
                    break;
                default:
                    throw std::runtime_error("unexpected json file");
                    break;
            }
       }
    }
    template<typename T>
    jsonValue<T> *operator[]  (const char * key)  {
        auto itr = nodes_.find(std::string(key));
        if(itr == nodes_.end()){
            throw std::runtime_error("key not found");
        }
        return (jsonValue<T>*)(itr->second);
    }
    std::map<std::string , void*>  & operator=(std::map<std::string , void*> & value) override {return nodes_;}
    std::map<std::string , void*> & get() override {return nodes_;}
};


class json{
    char * input_;
    jsonObject * root_;
    public:
       json(Stream &stream);
       json(std::string &input);
       json(const char *input){
            Tokenizer tokenizer(input);
            if(!tokenizer.hasNext() || tokenizer.getNext()->type_ != TokenType::OpenBracket)
                throw std::runtime_error("unexpected json file provided ");
            root_ = new jsonObject(tokenizer);
       }

       template<typename T>
       jsonValue<T> * operator[](const char *key){
           return root_->template operator[]<T>(key);
       }

       template<typename T>
       jsonValue<T> * get(const char *key);
};

jsonArray::jsonArray(Tokenizer & tokenizer){
        array_ = std::vector<void *>();
        while(tokenizer.hasNext()){
            auto next = tokenizer.getNext();
            if(next->type_ == TokenType::ClosedBrace)break;
            switch(next->type_){
                case TokenType::OpenBracket:
                    array_.push_back(new jsonObject(tokenizer ));
                    break;
                case TokenType::OpenBrace:
                    array_.push_back(new jsonArray(tokenizer ));
                    break;
                case TokenType::String:
                    array_.push_back(new jsonString(tokenizer , next));
                    break;
                case TokenType::Number:
                    array_.push_back(new jsonInteger(tokenizer , next));
                    break;
                case TokenType::Bool:
                    array_.push_back(new jsonBoolean(tokenizer , next));
                    break;
                default:
                    throw std::runtime_error("unexpected json file");
                    break;

            }
        }
}

token * Tokenizer::getNext()
{
    // for now lets consider a " , ' ' as seperators 
    std::string str = "";
    TokenType type = TokenType::End;
    do{
        char ch = *(input_ + idx_++);
      //  printf("Next begining %c \n" , ch);
        //fflush(stdout);
        if(ch == ' ' ||
           ch == '\n'|| 
           ch == ',' ||
           ch == '\r'||
           ch == '\t'){
            // skip these 
        }else{
            switch(ch){
                case '{':
                    str = "{";
                    type = TokenType::OpenBracket;
                    break;
                case '}':
                    str = "}";
                    type = TokenType::ClosedBracket;
                    break;
                case '[':
                    str = "[";
                    type = TokenType::OpenBrace;
                    break;
                case ']':
                    str = "]";
                    type = TokenType::ClosedBrace;
                    break;
                case ':':
                    str = ":";
                    type = TokenType::Colon;
                    break;
                case '\"':
                    do{
                        if(*(input_ + idx_) != '\"')
                            str += *(input_ + idx_);
                    }while(*(input_ + idx_++) != '\"');
                    type = TokenType::String;
                    break;
                default:
                    if(ch >= '0' && ch <= '9'){
                        // this token is a number 
                        do{
                            str += ch;
                            ch = *(input_ + idx_++);
                        }while(ch >= '0' && ch <= '9');
                        type = TokenType::Number;
                    }else if(ch == 't' || ch == 'T' || ch == 'f' || ch == 'F'){
                        do{
                            str += ch;
                            ch = *(input_ + idx_++);
                        }while(ch >= 'a' && ch <= 'z');
                        assert(str == "true" || str == "True" || str == "false" || str == "False");
                        type = TokenType::Bool;
                    }else{
                        throw std::runtime_error("unexpected json file passed");
                    }
                    break;
            }
        }

    }while(type == TokenType::End);
    std::cout << "Ahethesham returning next token type " << (short)type << " " << str <<" setting next index to " << idx_ << std::endl;
    return new token(type , str);
}
#endif
