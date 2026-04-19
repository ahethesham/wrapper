#ifndef __BASIC_TOKENIZER_INTERFACE_H__
#define __BASIC_TOKENIZER_INTERFACE_H__

#include "basic_token_interface.h"

/*
 * Which protocol  are we going to tokenize for 
 */
template<typename T>
class basic_tokenizer_interface{
    public:
        
        using self_type = basic_tokenizer_interface<T>;
        /*
         * Should give reference to the next valid token and increment to the next token internally
         */
        virtual basic_token_interface<T> * getNext() = 0;
        /*
         * Should give reference to the next valid token and should not increment to next token at all
         */
        virtual basic_token_interface<T> * peek() = 0;
        /*
         * similar to getNext() 
         * Should give reference to the next valid token and increment to the next token internally
         */
        virtual basic_token_interface<T> * operator++() = 0;

        virtual basic_token_interface<T> * operator--() = 0;

        virtual self_type & register_token_handler(char ch , basic_token_interface<T>::token_handler cb) = 0;

        virtual char get_next_char() = 0;

        virtual bool hasNext() = 0;

        virtual char peek_next_char() = 0;

        virtual char peek_next_valid_char() = 0;

        virtual char get_next_valid_char() = 0;
};

#endif
