#include "ini_config_parser.h"
#include "file_io_handle.h"
#include "file_reader_v1.h"
#include "file_logger_v1.h"
#include "basic_logger_interface.h"
#include "web.h"

int main(int argc , char * argv[]){
    
    Config & cfg = *new Config(new file_reader(new file_io_handle<READ_ONLY>(argv[1])));
    Logger & logger = Logger::build((const char *)cfg["Logging"]["file_path"].c_str());

    gLogger = &logger;

    logger << "Testing server " << endl;
    
#if 0
    http_server server(*(new tcp_endpoint(8080 , std::string("localhost"))  ));

    server.GET("/testing" , [](http_request & req , http_response & res){

            log("received requests at testing ");
            res.responseLine() = "HTTP/1.1 200 OK";
            auto hdrs = res.header();
            hdrs["Content-Type"] = "application/json";
            log("Added content type header ");
            res.body().get().push("hello" , std::make_shared<jsonString>("world"));
            log("Added body  ");
            hdrs["Content-Length"] = res.body().serialize().size();
            log("Complete proccessing the request ");
            return ;
    });
    server.GET("/testing2" , [](http_request & req , http_response & res){
            res.responseLine() = "HTTP/1.1 200 OK";
            auto hdrs = res.header();
            hdrs["Content-Type"] = "application/json";
            log("Added content type header ");
            res.body().get().push("hello" , std::make_shared<jsonInteger>(123));
            log("Added body  ");
            hdrs["Content-Length"] = res.body().serialize().size();
    });
    server.run();
#else
#if 1
    https_client client(443 , "generativelanguage.googleapis.com");
#endif
    logger.log("Starting the json buildup");
    http_request request;
    

    request.requestLine().uri() = "/v1beta/models/gemini-3-flash-preview:generateContent";
    request.requestLine().method() = "POST";




    auto headers = request.header();
    headers["x-goog-api-key"] = "AIzaSyAOlbF_vBYvUlIbRdt4hzCiVBwT7lHE6vs";
    headers["Content-Type"] = "application/json";
    headers["Accept"] = "*/*";

    
    json_object req ;
    json_object inner1 , inner2;
    inner1.push("text" , new json_string(std::string("reply to this request with a hello ")));
    json_array array1 , array2;
    array1.push((json_object*)&inner1);
    req.push("parts" , (json_array *)&array1);

    array2.push((json_object *) &req);
    inner2.push("contents" , (json_array *)&array2);
    headers["Content-Length"] = std::to_string(inner2.serialize().size());
    headers["User-agent"] = "testing";
    headers["Host"] = "generativelanguage.googleapis.com";
    request.body().get() = inner2.get();
    
    logger << "request \n " << request.serialize() << endl; 
#if 1
    auto response = client.send(request);

    //log("response received %s ", response.body().serialize().c_str());
#endif
#endif
    logger.log("Shutting Down Bye ");
    exit( 0);

}
