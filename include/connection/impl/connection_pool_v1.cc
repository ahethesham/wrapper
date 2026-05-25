#include "connection_pool_v1.h"
#include "basic_connection_handle_interface.h"
#include "basic_connection_pool_interface.h"
#include "connection_handle_v1.h"
#include "http_reader_v1.h"
#include "http_response_builder_v1.h"
#include "http_response_v1.h"
#include "http_writer_v1.h"
#include "https_connection_v1.h"
#include "http_connection_v1.h"
#include "socket_io_handle.h"
#include "v4_tcp_endpoint_v1.h"
#include <queue>

using https_connection = https_connection_v1;
using http_connection = http_connection_v1;
using connection_handle = connection_handle_v1;

class connection_pool_v1::impl{
    public:
        impl(config cfg , connection_pool_v1 & parent):
            cfg_(cfg) , parent_(parent){
                // we will start with min connections and if more and more threads keep asking for a connection we will have to increase one by one
             create_connection();
        }

        std::shared_ptr<basic_connection_handle_interface> acquire(){
            std::shared_ptr<basic_connection_interface> conn = nullptr;
            {
                std::lock_guard<std::mutex> lock(pool_lock_);
                if(!idle_pool_.empty()){
                    conn = idle_pool_.front();
                    idle_pool_.pop();
                }
            }
            return std::make_shared<connection_handle>(conn , parent_);
        }
        void release(std::shared_ptr<basic_connection_handle_interface> handle){
            handle->release();   
        }
        void release(std::shared_ptr<basic_connection_interface >  conn){
            {
                std::lock_guard<std::mutex> lock(pool_lock_);
                idle_pool_.push(conn);
            }
            return ;
        }
        void reset(config  cfg){
            // reload config and reset all the limits
        }
        void resize(int size){
            // TODO
        }
        
        int id(){
            return -1;
        }

    private:
        void create_connection(){
            {
                std::lock_guard<std::mutex > lock(pool_lock_);
                for(int count = 0 ; count < cfg_.min_connections_ ; count++){
                    std::shared_ptr<v4_tcp_endpoint_v1> endpoint = std::make_shared<v4_tcp_endpoint_v1>(cfg_.host_ , cfg_.port_);
                    if(cfg_.tls_){
                        std::shared_ptr<tcp_ssl_socket> sock = std::make_shared<tcp_ssl_socket>();
                        auto conn = std::make_shared<https_connection>(sock , endpoint);
                        conn->set_response_builder(std::make_shared<http_response_builder_v1<http_response_v1>>())
                            .set_exception_handler([&](std::runtime_error & err)->int{ return 0; })
                            .set_writer_obj(std::make_shared<https_writer>(sock))
                            .set_reader_obj(std::make_shared<https_reader>(sock))
                            // connection will not be established untill we call connect explicitly
                            .connect();

                        idle_pool_.push(conn);
                    }
                }
            }
            return ;
        }
        // TODO have all the logic to scale the connection count in between min and max at  runtime
        std::queue<std::shared_ptr<basic_connection_interface>> idle_pool_;
        // not used
        std::queue<std::shared_ptr<basic_connection_interface>> busy_pool_;
        config   cfg_;
        std::mutex pool_lock_;
        basic_connection_pool_interface & parent_;
};

connection_pool_v1::connection_pool_v1(config cfg )
    : impl_(std::make_shared<impl>(cfg , *this)){}

connection_pool_v1::connection_pool_v1() {}

std::shared_ptr<basic_connection_handle_interface> connection_pool_v1::acquire(){
    return impl_->acquire();
}

basic_connection_pool_interface & connection_pool_v1::release(std::shared_ptr<basic_connection_interface> conn){
    impl_->release(conn);
    return *this;
}
basic_connection_pool_interface & connection_pool_v1::release(std::shared_ptr<basic_connection_handle_interface> conn_handle){
    impl_->release(conn_handle);
    return *this;
}

basic_connection_pool_interface & connection_pool_v1::resize(int size){
    impl_->resize(size);
    return *this;
}
int connection_pool_v1::id(){
    return impl_->id();
}

