#include "config_fwd.h"
#include <iostream>

int main(int argc , char* argv[])
{
    const char * fpath = argv[1];
    
    ini_config_reader config(fpath);

    ini_config_parser &  parser = config.read();

    auto t = parser["testing 2"]["key2"];

    std::cout << "Testing \n" << t << std::endl;
    
    exit(0);
}
