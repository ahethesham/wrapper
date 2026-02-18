#ifndef __SERVER_H__
#define __SERVER_H__

#include <exception>
template< typename socket ,
          typename endpoint,
          typename listener ,
          typename acceptor ,
          typename stream  ,
          typename request ,
          typename response ,
          typename base_server>
class basic_server : public base_server{
    typedef socket          socket_type;
    typedef listener        listener_type;
    typedef acceptor        acceptor_type;
    typedef stream          stream_type;
    typedef request         request_type;
    typedef response        response_type;
    typedef endpoint        endpoint_type;
    typedef base_server     base_server_type;

    typedef typename acceptor_type::client_fd_type    client_fd_type;
    
    listener_type *      listener_;
    socket_type   *      socket_;
    acceptor_type *      acceptor_;
    stream_type   *      stream_;
    endpoint_type *      endpoint_;
    //base_server_type *   base_server_;

    client_fd_type  totalRequests_;

    public:
        basic_server(endpoint_type & _endpoint ){
            socket_ = new socket_type(_endpoint);
            listener_ = new listener_type(socket_);
            acceptor_ = new acceptor_type(socket_);
            stream_ = new  stream_type();
            endpoint_ = new endpoint_type(_endpoint);
      //      base_server_ = new base_server_type();
            totalRequests_ = 0;
        }
        // this will be a blocking call run the server here 
        client_fd_type run(){
            assert(listener_->listen(1024) == 0);
            while(1){
                try{
                    client_fd_type newClientFd = acceptor_->accept();
                    request_type req;
                    response_type res;
                    /*
                     * read and create a request for this client fd as per the protocol given by user 
                     */
                    stream_->read(newClientFd , req);
                    // call the process function for the base server given by the client 
                    process(req , res);
                    // write back the response back to the client 
                    stream_->write(res->serialize());

                }catch(std::exception  &e){
                    //log the error and shutdown 
                    break;
                }
            }
            return totalRequests_;
        }
};

#endif
