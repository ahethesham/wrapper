#ifndef __BASIC_ISTREAM_H__
#define __BASIC_ISTREAM_H__

#include <stdexcept>
#include "logger.h"

template <typename socket_policy  ,
          typename reading_policy ,
          typename parsing_policy  >
class basic_istream  {
    
    public:
        using buffer_type = typename reading_policy::buffer_type ;
        using return_type = parsing_policy &;
        using socket_type = socket_policy;


        // read input from this fd 
        basic_istream(typename socket_type::fd_type fd) : fd_(fd) , reader_(new reading_policy(fd)){
            throw std::runtime_error("currently not supporting construcotr for fd ");
        }

        basic_istream(socket_type * socket ) : socket_(*socket) , reader_(new reading_policy(socket->get())) , fd_(socket->get()){}

        basic_istream(socket_type  socket ) : socket_(socket) , reader_(new reading_policy(socket->get())) , fd_(socket.get()){}

        // read input from this file 
        basic_istream(const char * fpath) : socket_(*new socket_type(fpath)){
            reader_ = new reading_policy(socket_.get());
            fd_     = socket_.get();

        }
        //
        return_type  read(){
            parsing_policy * data = new parsing_policy();
            return   this->read(data);
        }

        return_type  read(parsing_policy * parser){

            buffer_type * rc;
            while(parser->continue_reading() && !socket_.is_closed()){
                try{
                    rc = reader_->read();
                    if(rc == nullptr){
                        // reading has been completed ...close the socket 
                        socket_.close();
                        parser->parse(parsing_policy::end_of_file);
                        break;
                    }else{
                        parser->parse(rc);
                    }
                }catch(std::exception & e){
                    log("exception received in %s , %s" , __func__ , e.what());
                    break;
                }
            };
            // will mostly make it return the json or the core body type
            return *parser;
        }


        // TODO
        basic_istream & operator>>(return_type r){
            return *this;
        }

        basic_istream & reset(){
            reader_->reset();
            parsing_policy::reset();
            return *this;
        }
    private:
        socket_type::fd_type fd_;
        reading_policy * reader_ ;
        socket_type socket_;

};

#endif
