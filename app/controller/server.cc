#include "httplib.h"
#include "json_object_v1.h"
#include "file_logger_v1.h"
#include "json_builder.h"

using json = json_object_v1;

class ChatService {

    public:
        std::shared_ptr<json> process_request(json & req){
            std::shared_ptr<json> res = std::make_shared<json>();
            Logger::build() << "request received at service \n" << req.serialize() << endl;
            res->push("status" , new json_string_v1("successfull"));
            return res;
        }
};


int main(){
    httplib::Server server;
    server.bind_to_port("0.0.0.0" , 8074);
    server.Get("/v1/chat" , [](const httplib::Request & req , httplib::Response &res){
        assert( strstr(req.get_header_value("Content-Type").c_str() , "application/json") != nullptr);
        
        json & body = json_builder<json>(req.body.c_str()); 
        
        ChatService service;
        auto response_json = service.process_request(body);
        res.set_content(response_json->serialize() , response_json->get_body_type());
        return ;
    });

    server.listen_after_bind();
    return 0;

}

