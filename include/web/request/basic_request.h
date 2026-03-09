#ifndef __REQUEST_H__
#define __REQUEST_H__

#include <unistd.h>
#include <sys/types.h>
#include "json_fwd.h"
#include "web_fwd.h"

#if 0
template<typename Protocol ,
         typename Allocator >
class basic_request : public Serializable ,
                      public Protocol
{
    typedef typename Protocol::value_type        value_type;
    typedef typename Protocol::header_type       header_type;
    typedef typename Protocol::body_type         body_type;
    typedef typename Protocol::serialize_type    serialize_type;
    typedef typename Protocol::request_line_type request_line_type;   
    typedef typename Protocol::query_param_type  query_param_type;

    value_type   * protocol_;

    public:
        basic_request(const char *payload) : Protocol(payload){}
        basic_request() : Protocol() {}
        template<typename ... Args>
            basic_request & addHeader(Args ... args){
                Protocol::addHeader(std::forward<Args>(args)...);
                return  *this;
            }
        template<typename ... Args>
            basic_request & addBody(Args ... args){
                Protocol::addBody(std::forward<Args>(args) ... );
                return *this;
            }
        template<typename ... Args>
            basic_request & addQueryParam(Args ... args){
                Protocol::addQueryParam(std::forward<Args>(args) ... );
                return *this;
            }
        template<typename ... Args>
            basic_request & addStatusLine(Args ... args){
                Protocol::addStatusLine(std::forward<Args>(args) ... );
                return *this;
            }
        serialize_type  serialize() override{
            return Protocol::serialize();
        }

        // so that users can get the reference to underlying header type and use it as per client abstraction
        header_type & header(){
            return Protocol::header();
        }
        body_type & body(){
            return Protocol::body();
        }
        request_line_type & requestLine(){
            return Protocol::requestLine();
        }
};
#endif

#endif
