#include <cassert>
#include <vector>
#include <iostream>
#include "json.h"

int main(int argc , char **argv)
{
    FILE *fptr = fopen(argv[1] , "r");
    assert(fptr != NULL);
    char ch;
    char input[2024*2024] = {0};
    int idx = 0;
    while( (ch = getc(fptr) ) != EOF)
        input[idx++] = ch; 
    try{
        json _json((const char *)input); 
        std::cout << "testing one " << (_json.operator[]<std::string>("lastLogin"))->get() << std::endl;
    }catch(std::exception &e){
        printf("Fucked up  %s \n" , e.what());
    }
    return 0;
}
