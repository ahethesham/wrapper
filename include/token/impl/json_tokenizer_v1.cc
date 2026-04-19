#include "json_tokenizer_v1.h"
#include <stdexcept>
#include "file_logger_v1.h"

using self_type = json_tokenizer::self_type;
using token_type = json_tokenizer::token_type;
using buffer_type = json_tokenizer::buffer_type;

template<>
json_tokenizer::json_tokenizer_v1(basic_grammer_interface & grammer , const char * input) :
    grammer_(grammer) ,
    buffer_(input) ,
    pos_(0) ,
    line_(0)
{
}

template<>
token_type * json_tokenizer::move_token_forward(int &idx){
    token_type * ptoken = nullptr;
    do{
        char ch = *(buffer_ + idx++);
        if(ch == '\0')throw std::runtime_error("ERR_EVNT_0000005");

        if(grammer_.is_end_of_file(ch))throw std::runtime_error("EOF reached ERR_EVNT_0000002");

        if(grammer_.is_unexpected_char(ch))throw std::runtime_error("unexpected char received ERR_EVNT_0000004");
        if(grammer_.continue_reading(ch) )continue;
        
        auto cb = handlers_[ch];
        if(nullptr == cb)
            Logger::build() << "We are unable to find callback for the token" << ch << endl;
        
        assert(cb != nullptr);

        ptoken = new token_type(ch , cb);
        break;

    } while(1);

    assert(ptoken != nullptr);
    return  ptoken;
}

template<>
char json_tokenizer::move_char_forward(int & idx){
    char ch = '\0';
    do{
        ch = *(buffer_ + idx++);
        if(ch == '\0')throw std::runtime_error("ERR_EVNT_0000005");

        if(grammer_.is_end_of_file(ch))throw std::runtime_error("EOF reached ERR_EVNT_0000002");
        if(grammer_.is_unexpected_char(ch))throw std::runtime_error("unexpected char received ERR_EVNT_0000004");
        if(grammer_.continue_reading(ch) )continue;
        
        break;
    }while(1);

    return ch;
}

template<>
token_type * json_tokenizer::getNext(){
    return move_token_forward(pos_);
}

template<>
token_type * json_tokenizer::peek() {
    int temp = pos_;
    return move_token_forward(temp);
}

template<>
token_type * json_tokenizer::operator++(){
    return move_token_forward(pos_);
}
template<>
token_type * json_tokenizer::operator--(){
    //TODO
    return nullptr;
}

template<>
self_type & json_tokenizer::register_token_handler(char ch , token_handler cb){
    handlers_[ch] = cb;
    return *this;
}

template<>
char json_tokenizer::get_next_char(){
    return *(buffer_ + pos_++);
}

template<>
char json_tokenizer::peek_next_char(){
    return *(buffer_ + pos_);
}


template<>
char json_tokenizer::peek_next_valid_char(){
    int temp = pos_;
    return move_char_forward(temp);
}

template<>
char json_tokenizer::get_next_valid_char(){
    return move_char_forward(pos_);
}

template<>
bool json_tokenizer::hasNext(){
    return *(buffer_ + pos_) != '\0';
}
