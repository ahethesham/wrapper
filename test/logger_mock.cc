#include "logger_fwd.h"
#include <iostream>
Logger * logger;

int main(int argc , char *argv[])
{
    const char * file_path = argv[1];

    
    Logger & loger = log_builder<Logger>(file_path);
    loger << "what has happened " ;
    loger << 123;

    return 0;


}
