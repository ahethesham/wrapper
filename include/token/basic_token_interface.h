#ifndef __BASIC_TOKEN_INTERFACE_H__
#define __BASIC_TOKEN_INTERFACE_H__

#include <functional>

template<typename T>
class basic_tokenizer_interface;
class basic_object_interface;

template<typename T >
class basic_token_interface{
    public:
        using object_interface = T;
        using tokenizer_interface = basic_tokenizer_interface<T>;

        using token_handler = std::function< object_interface & (tokenizer_interface &) > ;

        virtual bool compare( char ch) = 0;

        virtual object_interface & cb(tokenizer_interface &  tokenizer) = 0;

        virtual char get() = 0;

};

#endif
