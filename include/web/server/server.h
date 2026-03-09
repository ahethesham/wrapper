#ifndef __SERVER_H__
#define __SERVER_H__

#include <exception>
#include <string>
#include "logger.h"
/*
 * Base prototype for all the server objects ... every kind of server whether it's a http server or a https server has to follow this
 * 
 */
template< typename socketPolicy ,
          typename endpointPolicy,
          typename binderPolicy ,
          typename listenerPolicy ,
          typename acceptorPolicy ,
          typename streamPolicy  ,
          typename requestPolicy ,
          typename responsePolicy ,
          typename base_server>
class basic_server : public base_server{
    typedef endpointPolicy        endpoint_type;
    typedef socketPolicy          socket_type;
    typedef listenerPolicy        listener_type;
    typedef acceptorPolicy        acceptor_type;
    typedef streamPolicy          stream_type;
    typedef requestPolicy         request_type;
    typedef responsePolicy        response_type;
    typedef binderPolicy          binder_type;
    typedef base_server           base_server_type;

    typedef typename acceptor_type::socket_type    client_fd_type;
    
    listener_type *      listener_;
    socket_type   *      socket_;
    acceptor_type *      acceptor_;
    stream_type   *      stream_;
    endpoint_type *      endpoint_;
    binder_type   *      binder_;
    //base_server_type *   base_server_;

     int  totalRequests_;

    public:
        basic_server(endpoint_type & _endpoint){
            try{

                socket_ = new socket_type();
                // bind the socket to the endpoint
                binder_ = new binder_type(*socket_ ,  _endpoint);
                listener_ = new listener_type(socket_);
                acceptor_ = new acceptor_type(socket_);
                totalRequests_ = 0;
            }catch(std::exception & e){
                // something has failed log it here 
            }
        }
        basic_server(std::string host , endpoint_type::port_type port) :  basic_server(*endpoint_){
        }
        // this will be a blocking call run the server here 
        client_fd_type run(){
            assert(listener_->listen(1024) == 0);
            while(1){
                try{
                    socket_type newClient = acceptor_->accept();
                    response_type res;
                    /*
                     * read and create a request for this client fd as per the protocol given by user 
                     */
                    stream_type * io_stream = new stream_type(newClient.get());
                    // call the process function for the base server given by the client 
                    auto req = io_stream->read();
                    // used defined handlers 
                    base_server_type::process( req , res);
                    // write back the response back to the client 
                    io_stream->write(res);

                }catch(std::exception  &e){
                    //log the error and shutdown 
                    break;
                }
            }
            return totalRequests_;
        }
};

#endif
