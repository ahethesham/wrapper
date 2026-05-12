#include "io_handles_fwd.h"
#include "file_logger_v1.h"

int main(int argc , char ** argv){
    const char * file_path = "/home/ahethesham/agent/logs/agent_testing-%s-%d-%lu.log";
    //auto handle = *new log_file_handle(file_path);
    auto logger = Logger::build();

    logger << "Testing for the log file are we good " << endl;

}

