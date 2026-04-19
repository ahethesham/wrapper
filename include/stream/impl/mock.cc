#include "file_io_handle.h"
#include "stream_fwd.h"
#include "file_writer_v1.h"

using file_writer = file_writer_v1;
int main(){
    file_io_handle<WRITE> * handle = new file_io_handle<WRITE>((const char *)"./testing_file.txt");

    file_writer & writer = *new file_writer(handle);
    
    writer << "hello my dear" << file_writer_v1::endl;

    return 0;

}
