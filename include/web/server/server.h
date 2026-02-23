#ifndef __SERVER_H__
#define __SERVER_H__

#include <exception>
#include <string>
#include "logger.h"
/*
 * Base prototype for all the server objects ... every kind of server whether it's a http server or a https server has to follow this
 * 
 */
template< typename socket ,
          typename endpoint,
          typename binder ,
          typename listener ,
          typename acceptor ,
          typename stream  ,
          typename request ,
          typename response ,
          typename base_server>
class basic_server : public base_server{
    typedef endpoint        endpoint_type;
    typedef socket          socket_type;
    typedef listener        listener_type;
    typedef acceptor        acceptor_type;
    typedef stream          stream_type;
    typedef request         request_type;
    typedef response        response_type;
    typedef base_server     base_server_type;
    typedef binder          binder_type;

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

                log("Initializing socket ");
                socket_ = new socket_type();
                // bind the socket to the endpoint
                log("Initializing binder ");
                binder_ = new binder_type(*socket_ ,  _endpoint);
                log("Initializing listener ");
                listener_ = new listener_type(socket_);
                log("Initializing acceptor ");
                acceptor_ = new acceptor_type(socket_);
                totalRequests_ = 0;
            }catch(std::exception & e){
                // something has failed log it here 
            }
        }
        basic_server(std::string host , endpoint::port_type port) :  basic_server(*endpoint_){
        }
        // this will be a blocking call run the server here 
        client_fd_type run(){
            assert(listener_->listen(1024) == 0);
            while(1){
                try{
                    log("server  started listening ");
                    socket_type newClient = acceptor_->accept();
                    response_type res;
                    /*
                     * read and create a request for this client fd as per the protocol given by user 
                     */
                    log("new client fd %d " , newClient.get());
                    stream_type * io_stream = new stream_type(newClient.get());
                    // call the process function for the base server given by the client 
                    auto req = io_stream->read();
                    log("request received %s" , req.serialize().c_str() );
                    //process( io_stream->read() , res);
                    // write back the response back to the client 
                    stream_->write(res);

                }catch(std::exception  &e){
                    //log the error and shutdown 
                    break;
                }
            }
            return totalRequests_;
        }
};

#endif
