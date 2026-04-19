#include "ini_config_parser.h"
#include "stream_fwd.h"
#include "io_handles_fwd.h"
#include "file_logger_v1.h"
#include "file_reader_v1.h"
#include <iostream>

using Config = ini_config_parser<ini_container>;

int main(int argc , char ** argv)
{
    /*
     * Open the file in read only mode
     */
    auto handle =  new file_io_handle<READ_ONLY>(argv[1]);
    /*
     * Initialize the reader stream
     */
    auto freader = new file_reader(handle);

    Config & parser = * new Config(freader);
    auto logger = Logger::build(parser["Logging"]["file_path"].c_str());

    logger << "Are we good with logging " << endl;

}
