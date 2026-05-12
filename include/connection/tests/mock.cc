#include "http_response_builder_v1.h"
#include "https_connection_v1.h"
#include "io_handles_fwd.h"
#include "json_formatter_v1.h"
#include "v4_tcp_endpoint_v1.h"
#include "http_writer_v1.h"
#include "http_reader_v1.h"
#include "http_request_builder_v1.h"
#include "http_response_v1.h"
#include <iostream>
int main(){
    std::shared_ptr<tcp_ssl_socket> socket = std::make_shared<tcp_ssl_socket>();
    https_connection_v1 conn(socket , std::make_shared<v4_tcp_endpoint_v1>("generativelanguage.googleapis.com" , 443));
    conn.set_response_builder(std::make_shared<http_response_builder_v1<http_response_v1>>())
        .set_exception_handler([&](std::runtime_error & err)->int{ return 0; })
        .set_writer_obj(std::make_shared<https_writer>(socket))
        .set_reader_obj(std::make_shared<https_reader>())
        .connect();
    // create a json request for google 
    http_request_builder_v1<http_request_v1> builder ;
    json_object request_body;
    json_array contents;
    json_array parts;
    json_object parts_1 , contents_obj;
    json_string text("Explain how AI works in a few words");
    parts_1.push("text" , &text);
    parts.push(&parts_1);
    contents_obj.push("parts",&parts_1);
    contents.push(&contents_obj);
    request_body.push("contents" , &contents);

    
    auto req = builder.add_header("x-goog-api-key" , "AIzaSyAOlbF_vBYvUlIbRdt4hzCiVBwT7lHE6vs")
               .set_uri("/v1beta/models/gemini-3-flash-preview:generateContent")
               .set_body(request_body)
               .build();
    json_formatter_v1 formatter;
    std::cout << "\n" << req.serialize(formatter) << "\n" << std::endl;

}
