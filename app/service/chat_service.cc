#include "json_object_v1.h"
#include "json_string_v1.h"
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
