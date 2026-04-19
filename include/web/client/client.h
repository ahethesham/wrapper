#ifndef __CLIENT_H__
#define __CLIENT_H__

/*
 * Whenever a process wants to interactr with the external client it has to go through this basic_client generic interface 
 */

#include <exception>
#include <string>
#include "file_logger_v1.h"

class BaseClient{};

template < typename socketPolicy ,
           typename endpointPolicy ,
           typename connectPolicy ,
           typename iStreamPolicy  ,
           typename oStreamPolicy ,
           typename requestPolicy ,
           typename responsePolicy ,
           typename clientBase >
class basic_client : public clientBase {
    typedef socketPolicy    socket_type;
    typedef endpointPolicy  endpoint_type;
    typedef iStreamPolicy    istream_type;
    typedef oStreamPolicy    ostream_type;
    typedef requestPolicy   request_type;
    typedef responsePolicy  response_type;
    typedef connectPolicy   connect_type;

    socket_type     * socket_;
    endpoint_type   * endpoint_;
    istream_type    * istream_;
    ostream_type    * ostream_;
    connect_type    * connector_;

    public:
        basic_client(endpoint_type  endpoint) {
            try{
                socket_ = new socket_type();
                connector_ = new connect_type(*socket_ , endpoint);
                istream_ = new istream_type(socket_->get());
                ostream_ = new ostream_type(socket_->get());
            }catch(std::exception & e){
                Logger::build().log("Failed to connect with the client %s" , e.what());
            }
            return ;
        }
        basic_client(int port , const char * host){
            try{
                endpoint_ = new endpoint_type(port , std::string(host));
                socket_ = new socket_type();
                connector_ = new connect_type(*socket_ , *endpoint_);
                istream_ = new istream_type(socket_);
                ostream_ = new ostream_type(socket_);
            }catch(std::exception & e){
                Logger::build().log("failed to connect with the client ");
            }
        }

        response_type & send(request_type & req){
            assert(ostream_ != nullptr);
            ostream_->write(req);
            response_type * resp = new response_type();
            return istream_->read(resp);
        }

        basic_client & close(){
            socket_->close();
            return *this;
        }
        virtual ~basic_client(){
            delete socket_;
            delete istream_;
            delete connector_;
        }
};

#endif
