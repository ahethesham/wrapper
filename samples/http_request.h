#ifndef __HTTP_REQUEST_H__
#define __HTTP_REQUEST_H__

#include "basic_headers.h"
#include "basic_status_line.h"
#include "json.h"
#include <string>
#include "json_fwd.h"
#include "web_fwd.h"
#include "buffer.h"

#if 0
namespace http
{

#define HTTP_1_0 0
#define HTTP_1_1 1

    class HttpHeaders : public Serializable{
        std::vector<std::pair<std::string , std::string > > container_;
        public:
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
                    key += "\r\n";
                }
                if(key.size() > 1 && key[key.size() - 1] == '\n' && key[key.size() - 2] == '\r')
                    key.erase(key.size() - 2 , 2);
                log("Headers serialized %s" , key.c_str());
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
            std::string uri_;
            std::string method_;
            std::string httpVersion_;
            void setUri(int & idx){
                uri_ = "";
                while(idx < line_.size() && line_[idx] != ' '){
                    uri_ += line_[idx++];
                }
                if(idx == line_.size() || line_[idx] != ' ')
                    throw std::runtime_error("Unknown http request line");
                idx++;
                return ;
            }
            void setMethod(int  & idx){
                method_ = "";
                while(idx < line_.size() && line_[idx] != ' '){
                    method_ += line_[idx++];
                }
                if(idx == line_.size() || line_[idx] != ' ')
                    throw std::runtime_error("Unknown http request line");
                idx++;
                return ;
            }
            void setVersion(int & idx){
                httpVersion_ = "";
                while(idx < line_.size() && line_[idx] != '\r'){
                    httpVersion_ += line_[idx++];
                }
                return ;
            }
        public:
            std::string & getUri(){
                return uri_;
            }
            RequestLine & setUri(std::string & s){
                log("Ahethesham setting uri %s" , s.c_str());
                uri_ = s;
                return *this;
            }
            RequestLine & setMethod(std::string & s){
                log("Ahethesham setting method_ %s" , s.c_str());
                method_ = s;
                return *this;
            }
            RequestLine & setVersion(std::string & s){
                httpVersion_ = s;
                return *this;
            }

            std::string serialize() override{
                line_.clear();
                if(line_.size() == 0){
                    log("Ahethesham method = %s" , method_.c_str());
                    line_.append(method_);
                    line_ += " ";
                    log("Ahethesham uri = %s" , uri_.c_str());
                    line_.append(uri_);
                    line_ += " ";
                    line_.append(httpVersion_);
                }
                log("Ahethesham returning the request line  %s " , line_.c_str());
                return line_;
            }
            RequestLine & operator=(std::string & s){
                line_ = s;
                int idx = 0;
                setMethod(idx);
                setUri(idx);
                setVersion(idx);
                return *this;
            }
            RequestLine & operator=(const char * s){
                line_ = std::string(s);
                int idx = 0;
                setMethod(idx);
                setUri(idx);
                setVersion(idx);
                return *this;
            }
            // while sending request to external client ... set the defaults 
            RequestLine() : method_("GET") , uri_("/") , httpVersion_("HTTP/1.1") , line_(""){}
            int getType(){return 0;}
    };


    
   

    class HttpRequest : public Serializable{

        public:
            typedef json                                          body_type;
            // donot harcode the headers 
            typedef HttpHeaders                                   header_type;
            typedef QueryParameters                               query_param_type;
            typedef RequestLine                                   request_line_type;
            typedef HttpRequest                                   value_type;
            
            body_type body_;
            header_type header_;
            request_line_type requestLine_;
            query_param_type queryParams_;

        // implement a deep move 
            HttpRequest(HttpRequest && req) {} ;
        // implement a deep copy
           HttpRequest(HttpRequest &req) {};
           HttpRequest(int version = HTTP_1_1){}

           HttpRequest(const char *payload) {}

           void addQueryParam(std::string & key , std::string  & value){
                queryParams_[key] = value;
                return ;
           }
           void addHeader(std::basic_string<char > && key , std::basic_string<char >  && value){
                header_[key] = value;
                return ;
           }
           void addBody( const char *  input){
                body_.create(input);
                return ;
           }
           void addStatusLine(std::string  line){
               requestLine_ = line;
               return ;
           }
           // should take care of serializing itself into a string and returning the string 
           serialize_type serialize() {
                serialize_type res = "";
                res = requestLine_.serialize() + "\r\n" + header_.serialize() + "\r\n\r\n" + body_.serialize();
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
           HttpRequest & setUri(std::string  uri){
               requestLine_.setUri(uri);
               return *this;
           }
           HttpRequest & setMethod(std::string  uri){
               requestLine_.setMethod(uri);
               return *this;
           }
           HttpRequest & setHttpVersion(std::string  uri){
               requestLine_.setVersion(uri);
               return *this;
           }
    };
}




#endif
#endif
