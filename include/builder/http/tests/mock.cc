#include "http_request_builder_v1.h"
#include "file_logger_v1.h"
#include "http_request_v1.h"
#include "http_formatter_v1.h"
#include <iostream>

using http_request = http_request_v1;

int main(){
    http_request_builder_v1<http_request_v1> http_builder;
    http_request &req = http_builder.set_header("Authorization" , "Testing")
                                    .set_query_param("query1" , "param 1")
                                    .set_method("GET")
                                    .set_uri("testing/1/2/3")
                                    .build();
    http_formatter_v1 formatter;
    std::cout << req.serialize(formatter) << std::endl;

    return 0;
}
