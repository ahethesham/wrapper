#ifndef __HTTPS_CONNECTION_V1_H__
#define __HTTPS_CONNECTION_V1_H__

#include "basic_connection_interface.h"

/*
 */
class https_connection_v1 : public basic_connection_interface{
    public:
        //https_connection_v1(std::string & host , int port = 443) ;
        https_connection_v1() ;
        https_connection_v1(std::shared_ptr<ssl_socket_io_handle_interface> socket , std::shared_ptr<basic_endpoint_interface> endpoint);
        bool is_alive() override;
        https_connection_v1 & close() override;
        https_connection_v1 & send(basic_http_request_interface & req , callback && cb) override;

       basic_http_response_interface & send(basic_http_request_interface & req ) override;
       https_connection_v1(https_connection_v1 &) = delete;
       https_connection_v1(https_connection_v1 &&) = delete;
       https_connection_v1 & operator=(https_connection_v1 &) = delete;
       https_connection_v1 & operator=(https_connection_v1 &&) = delete;

       // set methods
        
       https_connection_v1 & set_response_obj(std::shared_ptr<basic_http_response_interface> obj) override;
       https_connection_v1 & set_endpoint_obj(std::shared_ptr<basic_endpoint_interface> obj) override;
       https_connection_v1 & set_writer_obj(std::shared_ptr<basic_writer_interface<ssl_socket_io_handle_interface>> obj) override;
       https_connection_v1 & set_reader_obj(std::shared_ptr<basic_reader_interface<ssl_socket_io_handle_interface>> obj) override;
       https_connection_v1 & set_socket_obj(std::shared_ptr<ssl_socket_io_handle_interface> obj) override;

       // builders
       https_connection_v1 & set_request_builder(std::shared_ptr<basic_request_builder_interface<basic_http_request_interface> > builder) override;
       https_connection_v1 & set_response_builder(std::shared_ptr<basic_response_builder_interface> builder) override;


       https_connection_v1 & set_retry_attempts(uint64_t r) override;
       https_connection_v1 & set_read_timeout(uint64_t r) override;
       https_connection_v1 & set_write_timeout(uint64_t r) override;
       https_connection_v1 & set_keep_alive_timeout(uint64_t r) override;
       
       https_connection_v1 & connect() override ;      

       // set handlers
       https_connection_v1 & set_exception_handler(exception_handler handler) override;
    private:
       class impl;
       std::shared_ptr<impl> impl_;
};

using https_connection = https_connection_v1; 
#endif
