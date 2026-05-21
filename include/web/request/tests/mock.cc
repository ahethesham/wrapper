#include "http_formatter_v1.h"
#include "http_headers_v1.h"
#include "http_request_line_v1.h"
#include "http_request_v1.h"
#include "json_array_v1.h"
#include "json_object_v1.h"
#include "json_string_v1.h"
#include "buffer.h"
#include <iostream>
#include <ostream>

#define DEBUG std::cout << "testing line no " << __LINE__ << std::endl;

#ifndef LOGGER
#define LOG(str) std::cout << str ;
#else
#define LOG(str) Logger::build(nullptr) << str ;
#endif


bool warmup(){

    http_request_v1 req;
    http_headers_v1 headers;
    json_object_v1 obj ;
    http_formatter_v1 formatter;
    LOG("starting warmup");
    LOG(endl)
    obj.push("Testing this internally" , new json_array_v1("[ \"t1\" , \"t2\" ]"));
    req.set_body(obj);
    req.set_method("GET");
    req.set_query_param("param1" , "testing");
    headers.set("Authorization" , "testing 2");
    headers.set("Content-Length" , std::to_string(obj.serialize().size()));
    req.set_headers(headers);
    LOG(req.serialize(formatter) )
    LOG(std::endl)

    return true ;
}
bool test_parsing_logic_for_headers();
bool test_parsing_logic_for_request_line();
bool test_parsing_logic_for_request();

int main()
{
#if 0
    warmup();
    test_parsing_logic_for_headers();
    test_parsing_logic_for_request_line();
#endif
    test_parsing_logic_for_request();
    return 0;
}
bool test_parsing_logic_for_headers(){
    http_headers_v1 headers;
    buffer_v1 buffer;
    const char * test_headers = "Host: example.com\r\nContent-Type: application/json\r\nContent-Length: 48\r\n\r\n";
    memcpy(buffer.data , test_headers, strlen(test_headers));
    headers.parse(&buffer);
    LOG(headers.get("Host"))
    LOG(std::endl)
    assert(headers.continue_reading() == false);
    return true;
}
#if 0
bool test_parsing_logic_for_request_line(){
    http_request_line_v1 line;
    const char * test_line = "GET /this/is/the/uri?var1=val1&var2=123 HTTP/1.1\r\n";
    buffer_v1 buffer;
    memcpy(buffer.data , test_line , strlen(test_line));
    line.parse(&buffer);
    LOG(line.get_method());
    LOG(std::endl)
    LOG(line.get_uri());
    LOG(std::endl)
    LOG(line.get_version());
    LOG(std::endl)
    LOG(line.get_query_param("var1"))
    LOG(std::endl)
    LOG(line.get_query_param("var2"))

    return true;
}
#endif

bool test_parsing_logic_for_request(){
    char * test = "POST /api/v1/users HTTP/1.1\r\nHost: example.com\r\nContent-Type: application/json\r\nContent-Length: 48\r\n\r\n{\r\n\"name\": \"John Doe\", \"email\": \"john@example.com\" }" ;
    buffer_v1 buffer;
    memcpy(buffer.data , test , strlen(test));
    http_request_v1 req;
    Logger::build() << "starting parsing " << endl;
    req.parse(&buffer);
    LOG("Parsing successfull checking values now \n");
    LOG(req["Content-Length"]);
    LOG(std::endl)
    return true;
}
