#ifndef __JSON_TOKENIZER_V1_H__
#define __JSON_TOKENIZER_V1_H__

#include "basic_grammer_interface.h"
#include "basic_object_interface.h"
#include "basic_tokenizer_interface.h"
#include "json_token_v1.h"
#include "buffer.h"
#include <map>

template < typename token_policy ,
           typename buffer_policy >
class json_tokenizer_v1 : public basic_json_tokenizer_interface{
    
    public:
        using token_type = token_policy;
        using buffer_type = buffer_policy;
        using self_type   = json_tokenizer_v1<token_type , buffer_type>;
        using token_handler = typename token_type::token_handler;


        json_tokenizer_v1(basic_grammer_interface & grammer , const char * input);
        json_tokenizer_v1(basic_grammer_interface & grammer , buffer_type * buffer);
        
        token_type *  getNext() override;
        token_type *  peek()    override;
        token_type *  operator++() override;
        token_type *  operator--()override;

        self_type & register_token_handler(char ch , token_handler cb) override;

        char get_next_char() override;
        bool hasNext() override;
        char peek_next_char() override;
        char peek_next_valid_char() override;
        char get_next_valid_char() override;
        
    private:
        token_type * move_token_forward(int &idx);
        char move_char_forward(int &idx);
        basic_grammer_interface & grammer_;
        std::map<char , token_handler > handlers_;
        int pos_;
        int line_;
        const char  * buffer_;
};

template class json_tokenizer_v1<token_v1<basic_object_interface> , buffer_v1>;

using json_tokenizer = json_tokenizer_v1< token_v1<basic_object_interface> , buffer_v1 >;

#endif
