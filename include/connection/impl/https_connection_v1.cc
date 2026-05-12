#include "https_connection_v1.h"
#include "basic_endpoint_interface.h"
#include "file_logger_v1.h"
#include "http_connection_exceptions.h"


/*
 * WARNING : this code is not thread safe ....we have a connection pool which takes care of maintaining
 * the owndership of per connection and takes care of mainting concurrecny
 */
class https_connection_v1::impl{
    public:
        impl() : response_(nullptr), writer_(nullptr) , reader_(nullptr) , socket_(nullptr){
            LOG_INFO << "Deprecated constructor called " << endl;
        }

        impl(std::shared_ptr<ssl_socket_io_handle_interface> socket , std::shared_ptr<basic_endpoint_interface> endpoint)
            : socket_(socket) , endpoint_(endpoint) , response_(nullptr) , writer_(nullptr) , reader_(nullptr) { }

        bool is_alive(){
            assert(socket_ != nullptr);
            return !socket_->is_closed();
        }

        bool connect(std::shared_ptr<basic_endpoint_interface > endpoint , std::shared_ptr<ssl_socket_io_handle_interface> socket){
            assert(endpoint_ == nullptr && socket_ == nullptr );
            endpoint_ = endpoint;
            socket_ = socket;
            connect();
            return true;
        }

        basic_http_response_interface & send(basic_http_request_interface & req){
            assert(writer_ != nullptr && reader_ != nullptr && response_ != nullptr);
            *writer_ << req;
            auto res = response_->clone();
            res->clear();
            reader_->read(*res);
            return *res;
        }
        void send(basic_http_request_interface & req , callback &&cb){
            assert(writer_ != nullptr && reader_ != nullptr && response_ != nullptr);
            // TODO
        }
        void close(){
            // TODO have logic to clear all the requests in flight
            socket_->close();
            return ;
        }
        void set_response_obj(std::shared_ptr<basic_http_response_interface> obj){
            assert(response_ == nullptr);
            response_ = obj;
            return ;
        }
        void set_endpoint_obj(std::shared_ptr<basic_endpoint_interface> obj){
            assert( endpoint_ == nullptr);
            endpoint_ = obj;           
            return ;
        }
        void set_writer_obj(std::shared_ptr<basic_writer_interface<ssl_socket_io_handle_interface>> obj){
            assert(writer_ == nullptr);
            writer_ = obj;
            return ;
        }
        void set_reader_obj(std::shared_ptr<basic_reader_interface<ssl_socket_io_handle_interface>> obj){
            assert(reader_ == nullptr);
            reader_ = obj;
            return ;
        }
        void set_socket_obj(std::shared_ptr<ssl_socket_io_handle_interface> obj){
            assert(socket_ == nullptr);
            socket_ = obj;
            return ;
        }
        void set_retry_attempts(uint64_t r){
            retry_attempts_ = r;
            return ;
        }
        void set_read_timeout(uint64_t r){
            read_timeout_ = r;
            return ;
        }
        void set_write_timeout(uint64_t r){
            write_timeout_ = r;
            return ;
        }
        void set_keep_alive_timeout(uint64_t r){
            keep_alive_timeout_ = r;
            return ;
        }
        void set_exception_handler(exception_handler handler){
            exception_handler_  = handler;
            return ;
        }
        void set_request_builder(std::shared_ptr<basic_request_builder_interface<basic_http_request_interface> > builder){
        }
        void set_response_builder(std::shared_ptr<basic_response_builder_interface > builder){

        }
        void connect(){
            auto itr = endpoint_->begin();
            while(itr->has_next()){
                auto addr_info = itr->get_next();
                if(detail::connect_wrapper(socket_->get_lowest_level_fd() , addr_info  , [&](int rc){
                    LOG_DEBUG << "Attempting SSL connect with rc = " << rc << endl;
                    return rc != 0 ? 1 : ::SSL_connect(socket_->get()) == 1 ? 0 : 1;
                }) == 0){
                    address_ = addr_info;
                    LOG_DEBUG << "endpoint " << endpoint_->hostname() << "is connected to " << endpoint_->ip_address(addr_info) << endl;
                    break;
                }
            }
            if(address_ == nullptr){
                LOG_FATAL << "Host Not Found " << endpoint_->hostname() <<endl;
                    throw host_not_found_exception("Host %s Not Found" , endpoint_->hostname().c_str());
            }
            return ;
        }
    private:
        std::shared_ptr<basic_http_response_interface> response_;
        std::shared_ptr<ssl_socket_io_handle_interface> socket_;
        std::shared_ptr<basic_endpoint_interface> endpoint_;
        std::shared_ptr<basic_response_builder_interface> response_builder_;
        std::shared_ptr<basic_request_builder_interface<basic_http_request_interface>> request_builder;
        exception_handler exception_handler_;
        std::shared_ptr<basic_writer_interface<ssl_socket_io_handle_interface>>  writer_;
        std::shared_ptr<basic_reader_interface<ssl_socket_io_handle_interface>> reader_;
        struct addrinfo * address_;
        pthread_t current_thread_id_;
        std::string hostname_;
        int port_;
        uint64_t retry_attempts_;
        uint64_t read_timeout_;
        uint64_t write_timeout_;
        uint64_t keep_alive_timeout_;
        std::mutex lock_;
};

https_connection_v1::https_connection_v1() : impl_(std::make_shared<impl>()){ 
    LOG_INFO << "default constrcutor for " << __func__ << "is deprecated make sure we inject socket and endpoint objects explicitly" << endl;
}


https_connection_v1::https_connection_v1(std::shared_ptr<ssl_socket_io_handle_interface> socket , std::shared_ptr<basic_endpoint_interface> endpoint)
    : impl_(std::make_shared<impl>(socket, endpoint)) {}

bool  https_connection_v1::is_alive(){
    return impl_->is_alive();
}

https_connection_v1 & https_connection_v1::close(){
    impl_->close();
    return *this;
 }

basic_http_response_interface  & https_connection_v1::send(basic_http_request_interface & req){
    return impl_->send(req);
}

https_connection_v1 & https_connection_v1::set_response_obj(std::shared_ptr<basic_http_response_interface> obj){
     impl_->set_response_obj(obj);
     return *this;
}

https_connection_v1 & https_connection_v1::set_endpoint_obj(std::shared_ptr<basic_endpoint_interface> obj){
    impl_->set_endpoint_obj(obj);
    return *this;
}

https_connection_v1 & https_connection_v1::set_writer_obj(std::shared_ptr<basic_writer_interface<ssl_socket_io_handle_interface>> obj){
     impl_->set_writer_obj(obj);
     return *this;
}

https_connection_v1 & https_connection_v1::set_reader_obj(std::shared_ptr<basic_reader_interface<ssl_socket_io_handle_interface>> obj){
    impl_->set_reader_obj(obj);
    return *this;
}

https_connection_v1 & https_connection_v1::set_socket_obj(std::shared_ptr<ssl_socket_io_handle_interface> obj){
    impl_->set_socket_obj(obj);
    return *this;
}


https_connection_v1 & https_connection_v1::set_retry_attempts(uint64_t r) {
    impl_->set_retry_attempts(r);
    return  *this;
}

https_connection_v1 & https_connection_v1::set_read_timeout(uint64_t r) {
    impl_->set_read_timeout(r);
    return  *this;
}

https_connection_v1 & https_connection_v1::set_write_timeout(uint64_t r) {
    impl_->set_write_timeout(r);
    return  *this;
}

https_connection_v1 & https_connection_v1::set_keep_alive_timeout(uint64_t r) {
    impl_->set_keep_alive_timeout(r);
    return  *this;
}

https_connection_v1 & https_connection_v1::set_exception_handler(exception_handler handler){
    impl_->set_exception_handler(handler);
    return *this;
}
https_connection_v1 & https_connection_v1::connect(){
    impl_->connect();
    return *this;
}

// builders
https_connection_v1 & https_connection_v1::set_response_builder(std::shared_ptr<basic_response_builder_interface> builder){
    impl_->set_response_builder(builder);
    return *this;
}
https_connection_v1 & https_connection_v1::set_request_builder(std::shared_ptr<basic_request_builder_interface<basic_http_request_interface> > builder){
    impl_->set_request_builder(builder);
    return *this;
}
