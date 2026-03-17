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
        json_object  json = *json_builder(input);
        auto t = *(std::get<json_object*>(json["preferences"]));
        //auto t = *std::get<json_object *>((*std::get<json_object *>(json["preferences"]))["notifications"]);
        std::cout << "cross 1" << std::endl;
        std::cout << std::get<json_string *>(t["theme"])->get() << std::endl;
    }catch(std::exception &e){
        printf("%s \n" , e.what());
    }
    return 0;
}
