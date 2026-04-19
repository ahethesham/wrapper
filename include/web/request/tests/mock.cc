#include "http_formatter_v1.h"
#include "http_request_v1.h"
#include "json_object_v1.h"
#include "json_string_v1.h"
#include <iostream>

int main()
{
    http_request_v1 req;
    req.set_method("GET");
    req.set_query_param("param 1 " , "testing ");
    req.set_header("Authorization" , "testing 2");

    json_object_v1 obj ;
    obj.push("Testing this internally" , new json_string_v1("successfulll"));
    req.set_body(obj);

    http_formatter_v1 formatter;
    std::cout << req.serialize(formatter) << std::endl;
    return 0;
}
