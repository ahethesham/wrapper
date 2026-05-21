#include "file_io_handle.h"
#include "http_response_builder_v1.h"
#include "https_connection_v1.h"
#include "io_handles_fwd.h"
#include "json_formatter_v1.h"
#include "v4_tcp_endpoint_v1.h"
#include "http_writer_v1.h"
#include "http_reader_v1.h"
#include "http_request_builder_v1.h"
#include "http_response_v1.h"
#include "ini_config_parser.h"
#include <iostream>

int main(int argc , char *argv[]){
    Config cfg(new file_reader_v1(new file_io_handle<READ_ONLY>(argv[2])));

    std::string host = cfg["google"]["host"];
    int port = std::atoi(cfg["google"]["port"].c_str());
    
    // set logging object 
    auto logger = Logger::build(cfg["logging"]["file_path"].c_str());


    std::shared_ptr<tcp_ssl_socket> socket = std::make_shared<tcp_ssl_socket>();

    https_connection_v1 conn(socket , std::make_shared<v4_tcp_endpoint_v1>(host , port));

    conn.set_response_builder(std::make_shared<http_response_builder_v1<http_response_v1>>())
        .set_exception_handler([&](std::runtime_error & err)->int{ return 0; })
        .set_writer_obj(std::make_shared<https_writer>(socket))
        .set_reader_obj(std::make_shared<https_reader>(socket))
        // connection will not be established untill we call connect explicitly
        .connect();

    // create a json request for google 
    http_request_builder_v1<http_request_v1> builder;

    std::shared_ptr<json_object> obj1 = std::make_shared<json_object>();
    std::shared_ptr<json_object> obj2= std::make_shared<json_object>();

    std::shared_ptr<json_array> array1 , array2;
    array1 = std::make_shared<json_array>();
    array2 = std::make_shared<json_array>();
    array2->push(std::make_shared<json_object>("{\"text\": \"Reply me with a hello bro \"}"  ) );
    obj2->push("parts" , array2);
    array1->push(obj2);
    obj1->push("contents" , array1);
    
    auto req = builder.add_header("x-goog-api-key" , "AIzaSyAOlbF_vBYvUlIbRdt4hzCiVBwT7lHE6vs")
               .set_uri("/v1beta/models/gemini-3-flash-preview:generateContent")
               .set_body(*obj1)
               .build();
    json_formatter_v1 formatter;
    
    http_response_v1 & res = (http_response_v1 &)conn.send(req);

    LOG_DEBUG << "received response object triggering serialize " << endl;

    LOG_INFO << "\n" << res.serialize(formatter) << endl;

}
