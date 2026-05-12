#include "binder_listener_v1.h"
#include "io_handles_fwd.h"
#include "v4_tcp_endpoint_v1.h"

int main(){
    bind_and_listen_v1(std::make_shared<tcp_socket>() , std::make_shared<v4_tcp_endpoint_v1>("localhost" , 3000));

}
