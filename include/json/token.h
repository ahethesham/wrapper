#ifndef __TOKEN_H__
#define __TOKEN_H__
#include <cassert>
#include <stdexcept>
#include <vector>
#include <string>

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
    std::vector<char > pass_;
    char *input_;
    int idx_;
    public:
        token * getNext();
        bool isCharToBeIgnored(char &ch){
            for(const char &  x : pass_)
                if(x == ch)return true;
            return false;
        }
        explicit Tokenizer(const char *input , std::vector<char > pass) : input_((char *)input) , idx_(0) , pass_(std::move(pass)) {}
        bool hasNext(){return *(input_ + idx_) != '\0';}
};

token * Tokenizer::getNext()
{
    // for now lets consider a " , ' ' as seperators 
    std::string str = "";
    TokenType type = TokenType::End;
    do{
        char ch = *(input_ + idx_++);
      //  printf("Next begining %c \n" , ch);
        //fflush(stdout);
        if(isCharToBeIgnored(ch)){
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
                        do{
                            str += ch;
                            ch = *(input_ + idx_++);
                        }while(ch >= '0' && ch <= '9');
                        type = TokenType::Number;
                    }else if(ch == 't' ||  ch == 'f' ){
                        do{
                            str += ch;
                            ch = *(input_ + idx_++);
                        }while(ch >= 'a' && ch <= 'z');
                        assert(str == "true" || str == "false" );
                        type = TokenType::Bool;
                    }else if(ch == 'n'){
                        do{
                            str += ch;
                            ch = *(input_ + idx_++);
                        }while(ch == 'n' || ch == 'u' || ch == 'l');
                        assert(str == "null");
                        type = TokenType::Null;
                    } else{
                        throw std::runtime_error("unexpected json file passed");
                    }
                    break;
            }
       }

    }while(type == TokenType::End);
    return new token(type , str);
}

#endif
