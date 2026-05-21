#include "http_headers_v1.h"
#include <cassert>
#include <iostream>

bool test_headers_parsing_logic();


int main(){
    test_headers_parsing_logic();
    return 0;
}

bool test_headers_parsing_logic(){

    http_headers_v1 headers;
    buffer_v1 buffer;
    const char * test_headers = "Host: example.com\r\nContent-Type: application/json\r\nContent-Length: 48\r\n\r\n";
    memcpy(buffer.data , test_headers, strlen(test_headers));
    headers.parse(&buffer);
    std::cout << headers.get("Host");
    std::cout << std::endl;
    assert(headers.continue_reading() == false);
    return true;
}
