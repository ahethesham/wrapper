#ifndef __CLIENT_H__
#define __CLIENT_H__

/*
 * Whenever a process wants to interactr with the external client it has to go through this basic_client generic interface 
 */

#include "logger.h"
#include <exception>
#include <string>
template < typename socketPolicy ,
           typename endpointPolicy ,
           typename connectPolicy ,
           typename streamPolicy  ,
           typename requestPolicy ,
           typename responsePolicy ,
           typename clientBase >
class basic_client : public clientBase {
    typedef socketPolicy    socket_type;
    typedef endpointPolicy  endpoint_type;
    typedef streamPolicy    stream_type;
    typedef requestPolicy   request_type;
    typedef responsePolicy  response_type;
    typedef connectPolicy   connect_type;

    socket_type     * socket_;
    endpoint_type   * endpoint_;
    stream_type     * stream_;
    connect_type    * connector_;

    public:
        basic_client(endpoint_type  endpoint) {
            try{
                socket_ = new socket_type();
                connector_ = new connect_type(*socket_ , endpoint);
                stream_ = new stream_type(socket_->get());
            }catch(std::exception & e){
                log("Failed to connect with the client %s" , e.what());
            }
            return ;
        }
        basic_client(int port , const char * host){
            try{
                endpoint_ = new endpoint_type(port , std::string(host));
                socket_ = new socket_type();
                connector_ = new connect_type(*socket_ , *endpoint_);
                log("initializing stream ");
                stream_ = new stream_type(socket_->get());
                log("Done with setup");
            }catch(std::exception & e){
                log("failed to connect with the client ");
            }
        }

        response_type  send(request_type & req){
            assert(stream_ != nullptr);
            log("writing request");
            stream_->write(req);
            return stream_->read();
        }

        basic_client & close(){
            socket_->close();
            return *this;
        }
        virtual ~basic_client(){
            delete socket_;
            delete stream_;
            delete connector_;
        }
};

#endif
