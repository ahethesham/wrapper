#ifndef __HTTP_REQUEST_H__
#define __HTTP_REQUEST_H__

#include "json.h"
#include <string>
#include "web_fwd.h"

namespace http
{

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
    class RequestLine : public Serializable{

        private:
            std::string line_;
        public:
            std::string & getUri();
            std::string serialize() override{
                return line_;
            }
            RequestLine & operator=(std::string & s){
                line_ = s;
                log("request line added %s" , s.c_str());
                return *this;
            }
    };
    
    class HttpRequest : public Serializable{

        public:
            typedef json                                          body_type;
            typedef HttpHeaders                                   header_type;
            typedef QueryParameters                               query_param_type;
            typedef RequestLine                                   request_line_type;
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
           void addHeader(std::string && key , std::string  && value){
                header_[key] = value;
                return ;
           }
           void addBody( const char *  input){
                body_.create(input);
                return ;
           }
           void addRequestLine(std::string  line){
               requestLine_ = line;
               return ;
           }
           // should take care of serializing itself into a string and returning the string 
           serialize_type serialize() {
                serialize_type res = "";
                res = requestLine_.serialize() + header_.serialize() + "\n\n" + body_.serialize();
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
