#ifndef __HTTP_RESPONSE_H__
#define __HTTP_RESPONSE_H__
#include "json_fwd.h"
#include "http_request.h"


namespace http{

    class HttpResponse : public Serializable{
        public:
            typedef jsonObject                                    body_type;
            typedef HttpHeaders                                   header_type;
            typedef QueryParameters                               query_param_type;
            typedef std::string                                   response_line_type;
            typedef std::string                                   serialize_type;
            typedef HttpResponse                                  value_type;
            
            HttpResponse(std::string & input){

            }
            HttpResponse(){}
            value_type & addBody(body_type & body){
                //body_ =  body;
                return *this;
            }
            value_type & addHeaders(std::basic_string<char> & input_key , std::basic_string<char> & input_value){
                headers_[input_key] = input_value;
                return *this;
            }
            value_type & addQueryParam(std::string & key , std::string &value){
                queryParams_[key] = value;
                return *this;
            }
            serialize_type  serialize() override{
                return serialized_;
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
            header_type         headers_;
            query_param_type    queryParams_;
            serialize_type      serialized_;
            response_line_type  responseLine_;


    };
}


#endif
