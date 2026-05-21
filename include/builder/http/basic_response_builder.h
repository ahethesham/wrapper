#ifndef __BASIC_RESPONSE_BUILDER_INTERFACE_H__
#define __BASIC_RESPONSE_BUILDER_INTERFACE_H__

#include "basic_http_response.h"
#include "basic_reader_interface.h"
#include "socket_io_handle.h"
class basic_response_builder_interface {

    public:
        basic_response_builder_interface() = default;
        virtual std::shared_ptr<basic_http_response_interface>  build() = 0;
        //set methods
        virtual basic_response_builder_interface & set_status(http_status & status) = 0;
        virtual basic_response_builder_interface & set_header(std::string key , std::string value) = 0;
        virtual basic_response_builder_interface & set_header(std::vector<std::pair<std::string , std::string> > & headers) = 0;
        virtual basic_response_builder_interface & set_body(basic_object_interface & obj) = 0;
        virtual basic_response_builder_interface & set_body(std::shared_ptr<basic_object_interface>  obj) = 0;
        
        // get methods
        virtual const http_status & status() = 0;
        virtual const std::string & header(std::string key) = 0;
        virtual std::string & version() = 0;
        virtual const std::string & operator[](std::string  key) = 0;
        virtual basic_object_interface & body() = 0;
       
        // create method
        virtual basic_response_builder_interface & create(std::shared_ptr<basic_http_response_interface > obj) = 0;
        virtual basic_response_builder_interface & create(basic_http_response_interface & obj) = 0 ;
        // clone
        virtual std::shared_ptr<basic_response_builder_interface > clone() = 0;


        virtual std::shared_ptr<basic_http_response_interface > build(std::shared_ptr<basic_reader_interface<ssl_socket_io_handle_interface>> reader) = 0;
        virtual std::shared_ptr<basic_http_response_interface > build(std::shared_ptr<basic_reader_interface<socket_io_handle_interface> > reader) = 0;
        // delete move and copy constructors
        basic_response_builder_interface & operator=(basic_response_builder_interface &) = delete;
        ~basic_response_builder_interface() = default;
        basic_response_builder_interface & operator=(basic_response_builder_interface &&) = delete;
};
#endif
