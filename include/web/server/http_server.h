#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__

#include <functional>
#include "web_fwd.h"


/*
 * This should be responsible for creating a uri to func mapper 
 */
class basic_matcher{
    public:
        using handler = std::function<void(http_request & req , http_response & res )> ;
    private:
        struct baseMatcher{
            std::string  uri_;
            std::string type_;
            handler handler_;
            baseMatcher(std::string & uri , std::string type , handler func) : uri_(uri) , type_(type) , handler_(func){}
        };
        std::vector< baseMatcher *> inventory_; 
    public:
        basic_matcher & push( std::string  uri , std::string requestType , handler func  ){
            // create a baceMatcher and push this 
            inventory_.push_back(new baseMatcher(uri , requestType , func));
            return * this;
        }
        void execute(std::string & uri , http_request & req , http_response & res){
            log("checking for the uri  and handler %s " , uri.c_str());
            // get the corresponding func handler and directly execute the function 
            for(auto itr = inventory_.begin() ; itr != inventory_.end() ; itr++){
                if(strcmp((*itr)->uri_.c_str() , uri.c_str()) == 0 && 
                   req.requestLine().method() == (*itr)->type_){
                    (*itr)->handler_(req , res);
                    break;
                }
            }
            return ;
        }
        // stop listening to these uri requests 
        basic_matcher & pop(std::string & uri , int requestType , handler func){
            return *this;
        }
};
/*
 * This will be the base  server 
 */
template <typename matcher>
class HttpServer : private matcher{
    typedef matcher::handler handler;
    // have a function handler for each uri
    std::map<std::string , handler> routers_;

    public:
        void process( http_request &req , http_response & res){
            matcher::execute(req.requestLine().uri() , req , res);
        }
        HttpServer & GET(const char * uri , handler func){
            matcher::push(std::string(uri) , std::string("GET") , func);
            return  *this;
        }
        HttpServer & POST(const char *uri, handler func){
            matcher::push(std::string(uri) , std::string("POST") , func);
            return *this;
        }
        HttpServer & PUT(const char * uri, handler func){
            matcher::push(std::string(uri) , std::string("PUT") , func);
            return *this;
        }
        HttpServer & DELETE(const char *uri, handler func){
            matcher::push(std::string(uri) , std::string("DELETE") , func);           
            return *this;
        }

};



#endif
