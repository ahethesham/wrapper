#include "http_request_builder_v1.h"
#include "file_logger_v1.h"
#include "http_request_v1.h"
#include "http_formatter_v1.h"
#include "json_object_v1.h"
#include "json_string_v1.h"
#include <iostream>
#include "http_response_v1.h"
#include "http_response_builder_v1.h"

using http_request = http_request_v1;
using http_response = http_response_v1;

int main(){

    http_request_builder_v1<http_request_v1> http_builder;
    json_object_v1 obj;

    obj.push("Testing http builder " , new json_string_v1("good"));
    http_request &req = http_builder.add_header("Authorization" , "Testing")
                                    .add_query_param("query1" , "param1")
                                    .set_method("GET")
                                    .set_uri("testing/1/2/3")
                                    .set_body(obj)
                                    .build();
    http_formatter_v1 formatter;
    Logger::build() << req.serialize(formatter) << endl;
    http_response_builder_v1<http_response> builder;
    auto res = builder.set_status(status(200))
               .set_header("Testing builder" , "successfull")
               .set_body(obj)
               .build();
    Logger::build() << "done with building object calling serialize " << endl;
    Logger::build() << res->serialize(formatter) << endl;

    Logger::build() << "Exiting" << endl;

    return 0;
}
