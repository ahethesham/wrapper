#ifndef __BASIC_ISTREAM_H__
#define __BASIC_ISTREAM_H__

#include <stdexcept>
#include "http_reader.h"
#include "logger.h"
#include "basic_http_request.h"

template <typename socket_policy = tcp_socket ,
          typename reading_policy = https_1_0_reader,
          typename parsing_policy = http_1_0_request_parser >
class basic_istream : public parsing_policy {
    
    public:
        using buffer_type = reading_policy::buffer_type ;
        using return_type = parsing_policy::return_type;
        using reader_size_type = reading_policy::size_type;
        using socket_type = socket_policy;


        // read input from this fd 
        basic_istream(socket_type::fd_type fd) : fd_(fd) , reader_(new reading_policy(fd)){

        }
        // read input from this file 
        basic_istream(const char * fpath){
            
        }
        // 

        return_type read(){
            bool continue_reading = true;
            buffer_type * rc;
            while(parsing_policy::continue_reading()){
                try{
                    rc = reader_->read(fd_);
                    parsing_policy::parse(rc);
                    if(rc  == reading_policy::end_of_file){
                        log("EOF reached ");
                        break;
                    }
                }catch(std::exception & e){
                    log("exception received in %s" , __func__);
                    break;
                }
            };
            return parsing_policy::get();
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

};

#endif
