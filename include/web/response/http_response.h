#ifndef __HTTP_RESPONSE_H__
#define __HTTP_RESPONSE_H__
#include "json_fwd.h"
#include "http_request.h"
#include "web_fwd.h"

#if 0
namespace http{
    
    class StatusLine : public Serializable{
        serialize_type line_;
        int status_;
        std::string message_;
        std::string http_version_;
        public:
            StatusLine(serialize_type line ) : line_(line) {
            }
            StatusLine(){}
            serialize_type serialize() override{
                return line_;
            }
            StatusLine & operator = (serialize_type s){
                line_ = s;
                return *this;
            }
    };


    class HttpResponse : public Serializable{
        public:
            typedef json                                          body_type;
            typedef HttpHeaders                                   header_type;
            typedef QueryParameters                               query_param_type;
            typedef StatusLine                                    response_line_type;
            typedef std::string                                   serialize_type;
            typedef HttpResponse                                  value_type;
            
            HttpResponse(std::string & input){

            }
            HttpResponse(){}
            value_type & addBody(body_type & body){
                body_ =  body.get();
                return *this;
            }
            value_type & addBody(const char * body){
                body_.create(body);
                return *this;
            }
            value_type & addHeader(std::basic_string<char >  && input_key , std::basic_string<char> && input_value){
                headers_[input_key] = input_value;
                return *this;
            }
            value_type & addQueryParam(std::string & key , std::string &value){
                queryParams_[key] = value;
                return *this;
            }
            value_type & addStatusLine(std::string  s){
                responseLine_ = s;
                return *this;
            }
            serialize_type  serialize() override{
                serialize_type response = "";
                response += responseLine_.serialize() + "\r\n";
                response += headers_.serialize() + "\r\n\r\n";
                response += body_.serialize();
                return response;
            }
            header_type & header(){
                return headers_;
            }
            body_type & body(){
                return body_;
            }
            response_line_type & responseLine(){
                return responseLine_;
            }
        private:
            body_type           body_;
            HttpHeaders         headers_;
            query_param_type    queryParams_;
            response_line_type  responseLine_;
    };
}
#else


using response = basic_http_request<http_1_0_status_line >;


#endif


#endif
