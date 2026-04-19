#include "http_request_builder_v1.h"

template<typename T>
class http_request_builder_v1<T>::impl{
    public:
        T& build(){
            // implement a std::move() here 
            return *instance_;
        }
        impl() : instance_(new T()) {}

        void method(std::string & method){
            instance_->set_method(method);
            return ;
        }

        void uri(std::string & uri){
            instance_->set_uri(uri);
            return ;
        }
        void query_param(std::vector<std::pair<std::string , std::string >> & params)
        {
            instance_->set_query_param(params);
            return ;
        }

        void query_param(std::string & key ,std::string & value)
        {
            instance_->set_query_param(key , value);
            return ;
        }
        void version(std::string & http_version){
            instance_->request_line().set_version(http_version);
            return ;
        }

        /// setting headers
        void header(std::string & key , std::string & value){
            instance_->headers().set_header(key , value);
            return ;
        }
        void header(std::vector<std::pair<std::string , std::string > > & headers){
            instance_->headers().set_header(headers);
            return ;
        }

        /// setting body
        void body(basic_object_interface & body){
            instance_->set_body(body);
            return ;
        }

    private:
        T * instance_;

};


template<typename T>
http_request_builder_v1<T>::http_request_builder_v1() : impl_(new impl()){}

template<typename T>
T & http_request_builder_v1<T>::build(){
    return impl_->build();
}

template<typename T>
http_request_builder_v1<T>::self_type & http_request_builder_v1<T>::set_method(std::string method){
    impl_->method(method);
    return *this;
}

template<typename T>
http_request_builder_v1<T>::self_type & http_request_builder_v1<T>::set_uri(std::string uri){
    impl_->uri(uri);
    return *this;
}
template<typename T>
http_request_builder_v1<T>::self_type & http_request_builder_v1<T>::set_query_param(std::vector<std::pair<std::string , std::string > > & params){
    impl_->query_param(params);
    return *this;
}

template<typename T>
http_request_builder_v1<T>::self_type & http_request_builder_v1<T>::set_query_param(std::string key , std::string value){
    impl_->query_param(key , value);
    return *this;
}


template<typename T>
http_request_builder_v1<T>::self_type & http_request_builder_v1<T>::set_version(std::string version){
    impl_->version(version);
    return *this;
}


template<typename T>
http_request_builder_v1<T>::self_type & http_request_builder_v1<T>::set_header(std::string key , std::string value){
    impl_->header(key , value);
    return *this;
}

template<typename T>
http_request_builder_v1<T>::self_type & http_request_builder_v1<T>::set_header(std::vector<std::pair<std::string , std::string > > & headers){
    impl_->header(headers);
    return *this;
}

template<typename T>
http_request_builder_v1<T>::self_type & http_request_builder_v1<T>::set_body(basic_object_interface &body){
    impl_->body(body);
    return *this;
}
