#include "connection_handle_v1.h"

class connection_handle_v1::impl{
    public:
        impl(std::shared_ptr<basic_connection_interface> conn , basic_connection_pool_interface & pool)
        : conn_(conn) , pool_(pool){}
 
        bool is_active(){
            return is_active_;
        }
        void mark_dead(){
            is_active_ = false;
            return ;
        }
        void release(){
            // can also do pool_.release(*this);
            pool_.release(conn_);
            return ;
        }
        std::shared_ptr<basic_connection_interface> get_connection(){
            return conn_;
        }

    private:
        std::shared_ptr<basic_connection_interface> conn_;
        basic_connection_pool_interface &pool_;
        bool is_active_;
};

connection_handle_v1::connection_handle_v1(std::shared_ptr<basic_connection_interface> conn , basic_connection_pool_interface & pool)
 : impl_(std::make_shared<impl>(conn , pool)) {}

basic_connection_handle_interface & connection_handle_v1::mark_dead(){
    impl_->mark_dead();
    return *this;
}

basic_connection_handle_interface & connection_handle_v1::release(){
    impl_->release();
    return *this;
}

bool connection_handle_v1::is_active(){
    return impl_->is_active();
}

std::shared_ptr<basic_connection_interface> connection_handle_v1::operator->(){
    return impl_->get_connection();
}

basic_connection_interface & connection_handle_v1::operator*(){
    return * impl_->get_connection();
}

connection_handle_v1::~connection_handle_v1(){
    if(impl_->is_active()){
        impl_->release();
    }
}
