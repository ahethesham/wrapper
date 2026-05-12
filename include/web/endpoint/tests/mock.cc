#include "v4_tcp_endpoint_v1.h"
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(){
    v4_tcp_endpoint_v1 endpoint("google.com" , 443);
    auto itr = endpoint.begin();
    while(itr.has_next()){
        auto addr = *itr;
        std::cout << "address = " << endpoint.ip_address(addr) << std::endl;
        ++itr ;
    }
    return 0;
}
