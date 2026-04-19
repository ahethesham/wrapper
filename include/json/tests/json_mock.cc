#include <cassert>
#include <vector>
#include <iostream>
#include "json_boolean_v1.h"
#include "json_formatter_v1.h"
#include "json_integer_v1.h"
#include "json_null_v1.h"
#include "json_string_v1.h"
#include "file_logger_v1.h"
#include "ini_config_parser.h"
#include "json_builder.h"

Logger * gLogger;
using json_object = json_object_v1;
using json_string = json_string_v1;
using json_integer = json_integer_v1;
using json_boolean = json_boolean_v1;
using json_null = json_null_v1;
using json_array = json_array_v1;


int main(int argc , char **argv)
{

    Config & cfg =  *new Config(new file_reader(new file_io_handle<READ_ONLY>(argv[1])));
    Logger & logger = Logger::build((const char *)cfg["Logging"]["file_path"].c_str());

    gLogger = &logger;
    const char * sample_file = cfg["json_mock"]["file_path"].c_str();

    logger << "json mocking started ..." << endl;

    FILE *fptr = fopen(sample_file , "r");
    assert(fptr != NULL);
    char ch;
    char input[2024*2024] = {0};
    int idx = 0;
    while( (ch = getc(fptr) ) != EOF)
        input[idx++] = ch; 
    try{

        json_object  & json = json_builder<json_object>(input);
        json_object & p = json["preferences"].get<json_object>();
        std::string theme = p["theme"].get<json_string>().value();
        
        logger << "Theme in prefereneces is " << theme << endl;
               

    }catch(std::exception &e){

        printf("%s \n" , e.what());
    }

    /*
     *Try creating a json 
     */
    try{
        json_object obj1 , obj2 ;
        json_array array1 , array2;
        array2.push(new json_object("{\"text\": \"Reply me with a hello bro \"}"  ) );
        obj2.push("parts" , &array2);
        array1.push(&obj2);
        obj1.push("contents" , &array1);
        json_formatter_v1 formatter;
        logger << '\n' << obj1.serialize(formatter) << endl;
    }catch(std::exception & e){
        logger << "Fucked up" << endl;
    }
    return 0;

}
