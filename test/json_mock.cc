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
        json _add;
        auto _obj1 = _add.get();
        _obj1.push("hello" , std::make_shared<jsonString>("world"));
        _obj1.push("for number with spaces" , std::make_shared<jsonInteger>(12345));
        jsonArray arr;
        auto t = &_obj1;
        std::shared_ptr<basic_json> t1(t);
        arr.push(t1).push(t1).push(std::make_shared<jsonInteger>(123)).push(std::make_shared<jsonString>("done")).push(std::make_shared<jsonBoolean>(true));
        jsonObject obj;
        auto tarr = &arr;
        std::shared_ptr<basic_json> t2(tarr);
        obj.push("testing array" , t2);
        std::cout << obj.serialize() << std::endl;
    }catch(std::exception &e){
        printf("%s \n" , e.what());
    }
    return 0;
}
