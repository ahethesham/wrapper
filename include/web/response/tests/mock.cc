#include "http_response_v1.h"
#include "http_headers_v1.h"
#include <iostream>

using http_response = http_response_v1;

bool warmup_response();

int main(){
    warmup_response();
    return 0;
}

bool warmup_response(){
    http_response response(status(200));
    json_object_v1 resp_body;
    resp_body.push("Tested" , new json_string_v1("Okay"));
    response.set_body(resp_body);
    http_headers_v1 headers;
    headers.set("Auth" , "ok");
    response.set_headers(headers);
    std::cout << response.serialize() << std::endl;
    return true;
}
