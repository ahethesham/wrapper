#include "http_response_builder_v1.h"
#include "http_headers_v1.h"
#include "http_response_line_v1.h"
#include "socket_io_handle.h"

template<typename T>
class http_response_builder_v1<T>::impl{
    
    public:
        impl(std::shared_ptr<basic_http_response_interface> obj)
        {
            response_ = std::make_shared<T>();
            response_->set_response_line(std::make_shared<http_response_line_v1>());
            response_->set_headers(std::make_shared<http_headers_v1>());
            response_->set_body(std::make_shared<json_object>());
        }
        impl(){
            response_ = std::make_shared<T>();
            response_->set_response_line(std::make_shared<http_response_line_v1>());
            response_->set_headers(std::make_shared<http_headers_v1>());
            response_->set_body(std::make_shared<json_object>());
        }

        // set methods
        void set_status(http_status & status) {
            response_->set_status(status);
            return ;
        }
        void set_header(std::string key , std::string value){
            response_->set_header(key , value);
            return ;
        }
        void set_header(std::vector<std::pair<std::string , std::string >> & headers){
            for(const auto & pair : headers)
                response_->set_header(pair.first , pair.second);
            return ;
        }
        void set_body(basic_object_interface & obj){
            assert(0);
            return ;
        }
        void set_body(std::shared_ptr<basic_object_interface > obj){
            response_->set_body(obj);
            return ;
        }

        //get method
        const http_status & status(){
            return response_->status();
        }
        const std::string & header(std::string key){
            return (*response_)[key];
        }
        std::string & version(){
            return response_->version();
        }
        const std::string & operator[](std::string key){
            return (*response_)[key];
        }
        basic_object_interface & body(){
            return *response_->body();
        }
        std::shared_ptr<basic_http_response_interface>  build(){
            LOG_DEBUG << "Entered " << __func__ << endl;
            auto resp_obj =  std::make_shared<T>() ;
            resp_obj->set_response_line(std::make_shared<http_response_line_v1>());
            resp_obj->set_body(std::make_shared<json_object>());
            resp_obj->set_headers(std::make_shared<http_headers_v1>());
            LOG_DEBUG << "Exiting " << __func__ << endl;
            return resp_obj;
        }
        std::shared_ptr<basic_http_response_interface> build(std::shared_ptr<basic_reader_interface<socket_io_handle_interface>> reader){
            LOG_DEBUG << "Entered " << __func__ << endl;
            auto response = build();
            reader->read(*response);
            LOG_DEBUG << "Exiting " << __func__ << endl;
            return response;
        }
        std::shared_ptr<basic_http_response_interface> build(std::shared_ptr<basic_reader_interface<ssl_socket_io_handle_interface>> reader){
            auto response = build();
            reader->read(*response);
            return response;
        }

    private:
        std::shared_ptr<T>  response_;
};


// constructors
template<typename T>
http_response_builder_v1<T>::http_response_builder_v1(basic_http_response_interface *obj)
    : impl_(std::make_shared<impl>(obj->clone())) {assert(0);}

    template<typename T>
http_response_builder_v1<T>::http_response_builder_v1(std::shared_ptr<basic_http_response_interface> obj)
    : impl_(std::make_shared<impl>(obj)) {assert(0);}

    template<typename T>
http_response_builder_v1<T>::http_response_builder_v1(basic_http_response_interface & obj)
    : impl_(std::make_shared<impl>(obj.clone())) {}

    template<typename T>
http_response_builder_v1<T>::http_response_builder_v1(http_response_builder_v1 & obj)
    : impl_(obj.impl_) {}

    template<typename T>
http_response_builder_v1<T>::http_response_builder_v1()
    : impl_(std::make_shared<impl>()) {}

//create methods
    template<typename T>
http_response_builder_v1 <T> & http_response_builder_v1<T>::create(basic_http_response_interface & obj){

    impl_ = std::make_shared<impl>(obj.clone());
    return *this;
}
    template<typename T>
http_response_builder_v1 <T> &  http_response_builder_v1<T>::create(std::shared_ptr<basic_http_response_interface>  obj){
    impl_ = std::make_shared<impl>(obj);
    return *this;
}

// set method
template<typename T>
http_response_builder_v1<T>::self_type & http_response_builder_v1<T>::set_status(http_status & status){
    impl_->set_status(status);
    return *this;
}
template<typename T>
http_response_builder_v1<T>::self_type & http_response_builder_v1<T>::set_header(std::vector<std::pair<std::string , std::string >> & headers){
    impl_->set_header(headers);
    return *this;
}
template<typename T>
http_response_builder_v1<T>::self_type & http_response_builder_v1<T>::set_header(std::string key , std::string value){
    impl_->set_header(key , value);
    return *this;
}
template<typename T>
http_response_builder_v1<T>::self_type & http_response_builder_v1<T>::set_body(basic_object_interface & obj){
    impl_->set_body(obj);
    return *this;
}
template<typename T>
http_response_builder_v1<T>::self_type & http_response_builder_v1<T>::set_body(std::shared_ptr<basic_object_interface> obj){
    impl_->set_body(obj);
    return *this;
}

// get method
template<typename T>
const http_status & http_response_builder_v1<T>::status(){
    return impl_->status();
}
template<typename T>
const std::string & http_response_builder_v1<T>::header(std::string key){
    return impl_->header(key);
}
template<typename T>
std::string & http_response_builder_v1<T>::version(){
    return impl_->version();
}
template<typename T>
const std::string & http_response_builder_v1<T>::operator[](std::string key){
    return impl_->header(key);
}
template<typename T>
basic_object_interface & http_response_builder_v1<T>::body(){
    return impl_->body();
}

//clone
template<typename T>
std::shared_ptr<basic_response_builder_interface> http_response_builder_v1<T>::clone(){
    return std::make_shared<http_response_builder_v1>(*this);
}

// build method
template<typename T>
std::shared_ptr<basic_http_response_interface> http_response_builder_v1<T>::build(){
    return impl_->build();
}
template<typename T>
std::shared_ptr<basic_http_response_interface> http_response_builder_v1<T>::build(std::shared_ptr<basic_reader_interface<ssl_socket_io_handle_interface>> reader){
    return impl_->build(reader);
}

template<typename T>
std::shared_ptr<basic_http_response_interface> http_response_builder_v1<T>::build(std::shared_ptr<basic_reader_interface<socket_io_handle_interface>> reader){
    return impl_->build(reader);
}
