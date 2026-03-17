#include "web.h"
#include "json.h"

int main(){
    
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
    http_request request;
    http_1_0_request_line line;


    request.requestLine().uri() = "/v1beta/models/gemini-3-flash-preview:generateContent";
    request.requestLine().method() = "POST";




    auto headers = request.header();
    headers["x-goog-api-key"] = "AIzaSyAOlbF_vBYvUlIbRdt4hzCiVBwT7lHE6vs";
    headers["Content-Type"] = "application/json";
    headers["Accept"] = "*/*";

    
    jsonObject req ;
    jsonObject inner1 , inner2;
    inner1.push("text" , std::make_shared<jsonString>("reply to this request with a hello "));
    jsonArray array1 , array2;
    array1.push(std::make_shared<jsonObject>(inner1));
    req.push("parts" , std::make_shared<jsonArray>(array1));

    array2.push(req);
    inner2.push("contents" , array2);
    headers["Content-Length"] = std::to_string(inner2.serialize().size());
    headers["User-agent"] = "testing";
    headers["Host"] = "generativelanguage.googleapis.com";
    request.body() = inner2.get();
    log("sending request with headers %s " , request.serialize().c_str());
    
#if 1
    auto response = client.send(request);

    log("response received %s ", response.body().serialize().c_str());
#endif
#endif
    log("Shutting Down Bye ");
    exit( 0);

}
