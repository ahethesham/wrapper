#include "basic_connection_handle_interface.h"
#include "file_io_handle.h"
#include "http_response_builder_v1.h"
#include "https_connection_v1.h"
#include "io_handles_fwd.h"
#include "json_formatter_v1.h"
#include "json_object_v1.h"
#include "v4_tcp_endpoint_v1.h"
#include "http_writer_v1.h"
#include "http_reader_v1.h"
#include "http_request_builder_v1.h"
#include "http_response_v1.h"
#include "ini_config_parser.h"
#include "http_formatter_v1.h"
#include <iostream>
#include "connection_pool_v1.h"


using connection_pool = connection_pool_v1;

void mock_connection_pool(Config & cfg );
void mock_google_gemini(std::shared_ptr<basic_connection_handle_interface> handle_ , Config & );

int main(int argc , char *argv[]){
    Config cfg(new file_reader_v1(new file_io_handle<READ_ONLY>(argv[1])));

    std::string host = cfg["google"]["host"];
    int port = std::atoi(cfg["google"]["port"].c_str());
    
    // set logging object 
    auto logger = Logger::build(cfg["logging"]["file_path"].c_str());


    mock_connection_pool(cfg);   

}

void mock_connection_pool(Config & cfg ){
    std::string host = cfg["google"]["host"];
    int port = std::atoi(cfg["google"]["port"].c_str());
    static connection_pool pool(
                connection_pool::config{
                    .max_connections_ = 1,
                    .min_connections_ = 1,
                    .tls_ = true,
                    .host_ = host,
                    .port_ = port,
                }
            );
    auto connection = pool.acquire();
    LOG_INFO << "acquired connection" << endl;
    mock_google_gemini(connection , cfg);

    return ;
}

void mock_google_gemini(std::shared_ptr<basic_connection_handle_interface> handle, Config & cfg){
    
    LOG_INFO <<"triggering gemini call" << endl;
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
    
    auto req = builder.add_header("x-goog-api-key" , cfg["google"]["api-key"])
               .set_uri(cfg["google"]["uri"])
               .set_version("HTTP/1.0")
               .add_header("Host" , (*handle)->hostname() )
               .set_method("POST")
               .set_body(*obj1)
               .build();

    LOG_INFO << "raw request \n" << req.serialize() << endl;
    http_formatter_v1 formatter;
    
    auto res = (*handle)->send(req.clone());

    LOG_DEBUG << "received response object triggering serialize " << endl;
    auto res_body = res->body()->get<json_object_v1>();

    std::cout << res_body["candidates"].get<json_array_v1>()[0].get<json_object_v1>()["content"].get<json_object_v1>()["parts"].get<json_array>()[0].get<json_object_v1>()["text"].get<json_string_v1>().value() << std::endl;

    return ;
}
