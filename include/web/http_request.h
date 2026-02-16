#ifndef __HTTP_REQUEST_H__
#define __HTTP_REQUEST_H__

#include "request.h"
#include "json_fwd.h"
#include <string>
namespace http
{
    class HttpRequest;
    class HttpResponse;

    typedef basic_request<HttpRequest  > http_request;

    class HttpHeaders : public Serializable{
        std::vector<std::pair<std::string , std::string > > container_;
        public:
            HttpHeaders(){}
            std::string & operator[](std::string key){
                container_.push_back({ key , ""});
                return container_[container_.size() - 1].second;
            }
            std::string serialize() override{
                std::string key = "";
                for(const auto & p : container_){
                    key += p.first;
                    key += ": ";
                    key += p.second;
                    key += '\n';
                }
                if(key.size() > 0 && key[key.size() - 1] == '\n')
                    key.erase(key.size() - 1 , 1);
                return key;
            }
    };
    class QueryParameters : public Serializable{
        std::vector<std::pair<std::string , std::string> > container_;
        public:
            QueryParameters(){}
            QueryParameters(std::string &buffer){}
            std::string& operator[](std::string key){
                container_.push_back({key , ""});
                return container_[container_.size() - 1].second;
            }
            std::string serialize(){
                std::string res = "";
                return res;
            }
    };
    
    // will have a json as body type
    // pair<string , string> as the header
    // std:;string as request  line
    class HttpRequest : public Serializable{

        public:
            typedef json                                          body_type;
            typedef HttpHeaders                                   header_type;
            typedef QueryParameters                               query_param_type;
            typedef std::string                                   request_line_type;
            typedef std::string                                   serialize_type;
            typedef HttpRequest                                   value_type;
            
            body_type body_;
            header_type header_;
            request_line_type requestLine_;
            query_param_type queryParams_;

        // implement a deep move 
            HttpRequest(HttpRequest && req) ;
        // implement a deep copy
           HttpRequest(HttpRequest &req) ;
           HttpRequest(){}
           HttpRequest(const char *payload) {}

           void addQueryParam(std::string & key , std::string  & value){
                queryParams_[key] = value;
                return ;
           }
           void addHeader(std::string & key , std::string  & value){
                header_[key] = value;
                return ;
           }
           void addBody( const char *  input){
                body_.create(input);
                return ;
           }
           // should take care of serializing itself into a string and returning the string 
           serialize_type serialize() {
                serialize_type res = "";
                res = requestLine_ + header_.serialize() + "\n\n" + body_.serialize();
                return res;
           }
           header_type & header(){
                return header_;
           }
           
           body_type & body(){
                return body_;
           }

           request_line_type & requestLine(){
                return requestLine_;
           }
    };
}

#endif
