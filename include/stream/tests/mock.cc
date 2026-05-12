#include "http_writer_v1.h"
#include "socket_io_handle.h"
#include <memory>

class mock_socket : public socket_io_handle_interface{
    public:
        mock_socket(int fd) : fd_(fd){ }
        int get_lowest_level_fd()override{
            return fd_;
        }
        int get() override{
            return fd_;
        }
        int open() override{
            return fd_;
        }
        bool is_closed() override{
            return false;
        }
        socket_io_handle_interface & close() override{
            return *this;
        }
        int get_flags() override{
            return 0;
        }
        int set_flags(int f) override{
            return f;
        }
    private:
        int fd_;
};

int main(int argc , const char *argv[]){
	
    //stdoutput
    std::shared_ptr<mock_socket> socket = std::make_shared<mock_socket>(1);
    http_writer writer(socket);
    writer << "{\n\t\"testing\": \"done\";\n}";
    fflush(stdout);
	return 0;
}
