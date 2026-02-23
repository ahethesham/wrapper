
#ifndef __RESPONSE_H__
#define __RESPONSE_H__
#include <unistd.h>
#include "json_fwd.h"
//#include "web_fwd.h"

template< typename Protocol ,
          typename Parser  ,
          typename Allocator >
class basic_response : public Serializable{
    
    typedef typename Protocol::value_type           value_type;
    typedef typename Protocol::header_type          header_type;
    typedef typename Protocol::body_type            body_type;
    typedef typename Protocol::serialize_type       serialize_type;
    typedef typename Protocol::response_line_type   response_line_type;
    typedef typename Protocol::query_param_type     query_param_type;

    value_type & protocol_;

    public:
    basic_response(value_type  res) : protocol_(std::move(res)) {}
    basic_response(std::string & input) : protocol_(*new value_type(input)) {}
    basic_response() : protocol_(*new value_type()){}

    template<typename ... Args>
        basic_response & addBody(Args ... args){
            protocol_.addBody(std::forward<Args>(args)...);
            return *this;
        }
    template<typename ... Args>
        basic_response & addHeaders(Args ... args){
            protocol_.addHeaders(std::forward<Args>(args)...);
            return *this;
        }
    template<typename ... Args>
        basic_response & addQueryParam(Args ... args){
            protocol_.addQueryParam(std::forward<Args>(args)...);
            return *this;
        }
    serialize_type  serialize(){
        return protocol_.serialize();
    }
    header_type & header(){
        return protocol_.header();
    }
    body_type & body(){
        return protocol_.body();
    }
    response_line_type & responseLine(){
        return protocol_.requestLine();
    }
};
#endif
