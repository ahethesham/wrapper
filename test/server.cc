#include "web_fwd.h"

int main(){
    http_server server(*(new tcp_endpoint(8080 , std::string("0.0.0.0"))  ));

    server.run();

    log("Shutting Down Bye ");

    return 0;

}
