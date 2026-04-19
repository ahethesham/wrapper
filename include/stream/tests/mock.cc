#include "stream_fwd.h"
#include "io_handles_fwd.h"
#include <memory>

int main(int argc , const char *argv[]){
	
    auto handle = file_io_handle_interface::build<file_io_handle<>>(argv[1]); 
    auto freader = (file_reader *)ini_file_reader_interface::build<file_reader>(handle);

    
    
	return 0;

}
