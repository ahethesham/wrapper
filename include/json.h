#ifndef __JSON_H__
#define __JSON_H__
#include <cassert>
#include <map>
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
    virtual T& get() ;
    // each corresponding concrete class should take care of initializing its value with the given 
    virtual T& operator=(T &value);
};
class jsonBoolean : public jsonValue<bool>{
    bool value_;
    public:
    explicit jsonBoolean(Tokenizer &);
    bool & get() override{
        return value_;
    };
};
class jsonInteger : public jsonValue<int>{
    int value_;
    public:
    explicit jsonInteger(Tokenizer &);
    int & get() override{
        return value_;
    };
};
class jsonString : public jsonValue<std::string>{
    std::string  value_;
    public:
    explicit jsonString(std::string & str) : value_(std::move(str)) {}
    explicit jsonString(Tokenizer & tokenizer){
        auto token = tokenizer.getNext();
        assert(token->type_ == TokenType::String);
        value_ = std::move(token->value_);
    }
    std::string & get() override{return value_;}
};
class jsonArray : public jsonValue<jsonArray>{
    std::vector<void *> array_;
    public:
    explicit jsonArray(Tokenizer &tokenizer);
    template<typename T>
    T* operator[](int idx);
    jsonArray & get() override;
};

class jsonObject : public jsonValue<jsonObject>{
    std::map<std::string , void *> nodes_;
    public:
    explicit jsonObject(Tokenizer &tokenizer){
       nodes_ = std::map<std::string , void *>(); 
        
       while(tokenizer.hasNext()){
            auto current = ++tokenizer;
            // json has completed 
            if(current->type_ == TokenType::ClosedBracket)break;
            auto next = tokenizer.getNext();
            // keys always has to be strings 
            if(next->type_ != TokenType::String)throw std::runtime_error("unexpected json");
            std::string key = next->value_;
            ++tokenizer;
            next = tokenizer.getNext();
            if(next->type_ != TokenType::Colon)throw std::runtime_error("unexpected json");
            ++tokenizer;
            next = tokenizer.getNext();
            switch(next->type_){
                case TokenType::OpenBracket:
                    nodes_[key] = (new jsonObject(tokenizer));
                    break;
                case TokenType::OpenBrace:
                    nodes_[key] = (new jsonArray(tokenizer));
                    break;
                case TokenType::String:
                    nodes_[key] = (new jsonString(tokenizer));
                    break;
                case TokenType::Number:
                    nodes_[key] = (new jsonInteger(tokenizer));
                case TokenType::Bool:
                    nodes_[key] = (new jsonBoolean(tokenizer));
                default:
                    throw std::runtime_error("unexpected json file");
                    break;
            }

       }
    }
    template<typename T>
    jsonValue<T> *operator[](const char * key);

    jsonObject & operator=(jsonObject & value);
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
           return *root_->template operator[]<T>(key);
       }

       template<typename T>
       jsonValue<T> * get(const char *key);
};

jsonArray::jsonArray(Tokenizer & tokenizer){
        array_ = std::vector<void *>();
        while(tokenizer.hasNext()){
            auto current = tokenizer.getNext();
            if(current->type_ == TokenType::ClosedBrace)break;
            auto next = tokenizer.getNext();
            switch(next->type_){
                case TokenType::OpenBracket:
                    array_.push_back(new jsonObject(tokenizer));
                    break;
                case TokenType::OpenBrace:
                    array_.push_back(new jsonArray(tokenizer));
                    break;
                case TokenType::String:
                    array_.push_back(new jsonString(tokenizer));
                    break;
                case TokenType::Number:
                    array_.push_back(new jsonInteger(tokenizer));
                case TokenType::Bool:
                    array_.push_back(new jsonBoolean(tokenizer));
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
    TokenType type;
    do{
        if(*(input_ + idx_) == ' ' ||
           *(input_ + idx_) == '\n'|| 
           *(input_ + idx_) == '\r'){idx_++;continue;}
        else{
            char ch = *(input_ + idx_);
            switch(*(input_ + idx_++)){
                case '{':
                    str = "{";
                    break;
                case '}':
                    str = "}";
                    break;
                case '[':
                    str = "[";
                    break;
                case ']':
                    str = "]";
                    break;
                case ':':
                    str = ":";
                    break;
                case '\"':
                    do{
                        str += *(input_ + idx_);
                    }while(*(input_ + idx_++) != '\"');
                    break;
                default:
                    if(ch >= '0' && ch <= '9'){
                        // this token is a number 
                        do{
                            str += ch;
                            ch = *(input_ + idx_++);
                        }while(ch >= '0' && ch <= '9');
                    }else if(ch == 't' || ch == 'T' || ch == 'f' || ch == 'F'){
                        do{
                            str += ch;
                            ch = *(input_ + idx_++);
                        }while(ch >= 'a' && ch <= 'z');
                        assert(str == "true" || str == "True" || str == "false" || str == "False");
                    }else{
                        throw std::runtime_error("unexpected json file passed");
                    }
            }
        }

    }while(0);
    return new token(type , str);
}
#endif
