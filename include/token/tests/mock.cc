#include "buffer.h"
#include "token_v1.h"
#include "json_tokenizer_v1.h"


using json_tokenizer = json_tokenizer_v1<token_v1<int> , buffer_v1 , int>;


int main(){
    char ch = 's';
    auto handler = [ ](basic_tokenizer_interface<int> &)->int&{
        return *new int();
    };
    auto tokenizer = new token_v1<int>(ch , handler);
    
    return 0;
}
