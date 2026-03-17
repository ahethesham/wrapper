#ifndef __BASIC_OSTREAM_H__
#define __BASIC_OSTREAM_H__

#include "stream_fwd.h"
#include <sys/types.h>

template < typename socket_policy  ,
           typename writer_policy  ,
           typename parser_policy >
class basic_ostream {
    
    public:
        typedef socket_policy::fd_type fd_type;
        using buffer_policy = parser_policy::buffer_type;
        using protocol_type = parser_policy;
        using return_type = ssize_t;

        basic_ostream(socket_policy * socket) : socket_(*socket) , writer_(new writer_policy(socket->get())) {}
        basic_ostream(socket_policy & socket) : socket_(std::move(socket)) , writer_(new writer_policy(socket.get())) {}
        
        // could be request or response 
        ssize_t write(parser_policy * parser){
            buffer_policy * buffer = parser->buffer();
            while(buffer->remainingBytes > 0){
                int rc = writer_->write( buffer->data , buffer->remainingBytes);
                if(rc > 0){
                    buffer->remainingBytes -= rc;
                }else if(rc == 0){
                    socket_->close();
                    throw std::runtime_error("I/O exception host closed the socket ");
                }else if(errno != EINTR){
                    throw std::runtime_error("I/O exception ..write interupted");
                }
            };
            return 0;
        }
        ssize_t write(parser_policy & parser){
            buffer_policy * buffer = parser.buffer();
            log("Got the buffer pointer ");
            while(buffer->remainingBytes > 0){
                log("First attempt to write");
                int rc = writer_->write( buffer);
                if(rc > 0){
                    buffer->remainingBytes -= rc;
                }else if(rc == 0){
                    socket_.close();
                    throw std::runtime_error("I/O exception host closed the socket ");
                }else if(errno != EINTR){
                    throw std::runtime_error("I/O exception ..write interupted");
                }
            };
            return 0;
        }

        
    private:
        writer_policy * writer_;
        socket_policy  socket_;
        basic_ostream( fd_type fd) : writer_(new writer_policy(fd)){
        } 

};
#endif
