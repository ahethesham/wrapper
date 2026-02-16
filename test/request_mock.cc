#include "request.h"
#include <memory>
#include <iostream>

#include "json_fwd.h"
int main(){
    http::http_request request;
    request.header()["key"] = "value";
    request.requestLine() = "GET / HTTP/1.1\n";
    request.body().push("testing for body" , std::make_shared<jsonString>("for the value"));
    std::cout  << request.serialize() << std::endl;
    return 0;
}
