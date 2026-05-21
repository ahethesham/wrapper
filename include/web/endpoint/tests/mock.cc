#include "v4_tcp_endpoint_v1.h"
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(){
    v4_tcp_endpoint_v1 endpoint("generativelanguage.googleapis.com" , 443);
    auto itr = endpoint.begin();
    while(itr->has_next()){
        std::cout << "Next address " << endpoint.ip_address(itr->get_next());
    }
    return 0;
}
