#ifndef __REQUEST_H__
#define __REQUEST_H__

#include <unistd.h>
#include <sys/types.h>
#include "json_fwd.h"

/*
 * Protocol could be http , ftp , smtp ...or any other custom protocol
 * Parser could be http parser , json parser , ftp parser 
 * Allocator could be a dynamic memory management service
 */
template<typename Protocol ,
         typename Parser ,
         typename Allocator >
class basic_request : public Serializable
{
    typedef typename Protocol::value_type        value_type;
    typedef typename Protocol::header_type       header_type;
    typedef typename Protocol::body_type         body_type;
    typedef typename Protocol::serialize_type    serialize_type;
    typedef typename Protocol::request_line_type request_line_type;   
    typedef typename Protocol::query_param_type  query_param_type;

    value_type   * protocol_;

    public:
        basic_request(const char *payload) : protocol_(new value_type(payload)){}
        basic_request() : protocol_(new value_type()) {}
        template<typename ... Args>
            basic_request & addHeader(Args ... args){
                protocol_->addHeader(std::forward<Args>(args)...);
                return  *this;
            }
        template<typename ... Args>
            basic_request & addBody(Args ... args){
                protocol_->addBody(std::forward<Args>(args) ... );
                return *this;
            }
        template<typename ... Args>
            basic_request & addQueryParam(Args ... args){
                protocol_->addQueryParam(std::forward<Args>(args) ... );
                return *this;
            }
        template<typename ... Args>
            basic_request & addRequestLine(Args ... args){
                protocol_->addRequestLine(std::forward<Args>(args) ... );
            }
        serialize_type  serialize(){
            return protocol_->serialize();
        }

        // so that users can get the reference to underlying header type and use it as per client abstraction
        header_type & header(){
            return protocol_->header();
        }
        body_type & body(){
            return protocol_->body();
        }
        request_line_type & requestLine(){
            return protocol_->requestLine();
        }
};

#endif
