#include <cassert>
#include <vector>
#include <iostream>
#include "json_formatter_v1.h"
#include "file_logger_v1.h"
#include "ini_config_parser.h"
#include "json_builder.h"
#include "json_fwd.h"

Logger * gLogger;

using file_reader = file_reader_v1;
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
        std::shared_ptr<json_object> obj1 = std::make_shared<json_object>();
        std::shared_ptr<json_object> obj2= std::make_shared<json_object>();
 
        std::shared_ptr<json_array> array1 , array2;
        array1 = std::make_shared<json_array>();
        array2 = std::make_shared<json_array>();
        array2->push(std::make_shared<json_object>("{\"text\": \"Reply me with a hello bro \"}"  ) );
        obj2->push("parts" , array2);
        array1->push(obj2);
        obj1->push("contents" , array1);
        json_formatter_v1 formatter;
        logger << '\n' << obj1->serialize(formatter) << endl;

        json_object obj;
        obj.push("Testing this internally" , std::make_shared< json_array >("[ \"t1\" , \"t2\" ]"));
        logger << obj .serialize() << endl;

        logger << "test 2 " << endl;
        json_body body;
        body.put("testing json body" , "string")
            .put("testing json body integer" ,  7)
            .put("testing json body boolean" , std::make_shared<json_boolean>(true));

        logger << body.serialize(formatter) << endl;



    }catch(std::exception & e){
    }
    return 0;

}
