#include "http_request_builder_v1.h"
#include "basic_object_interface.h"
#ifdef __DEBUG
#include "file_logger_v1.h"
#endif

template<typename T>
class http_request_builder_v1<T>::impl{
    public:
        T& build(){
            // implement a std::move() here 
            set_default_headers();
#ifdef __DEBUG
            Logger::build(nullptr) << "request " << endl << instance_->serialize() << endl;
#endif
            return *instance_;
        }
        impl() : instance_(new T()) {
            memset(hostname_ , 0x00 , sizeof(hostname_));
            assert(gethostname(hostname_ , sizeof(hostname_)) == 0);
        }

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
            for(auto & itr : params)
                instance_->set_query_param(itr.first , itr.second);

            return ;
        }

        void query_param(std::string & key ,std::string & value)
        {
            instance_->set_query_param(key , value);
            return ;
        }
        void version(std::string & http_version){
            instance_->request_line()->set_version(http_version);
            return ;
        }

        /// setting headers
        void header(std::string & key , std::string & value){
            instance_->set_header(key , value);
            return ;
        }
        void header(std::vector<std::pair<std::string , std::string > > & headers){
            for(auto & itr : headers)
                instance_->set_header(itr.first , itr.second);
            return ;
        }

        /// setting body
        void body(basic_object_interface & body){
            instance_->set_body(body);
            return ;
        }

    private:
        void set_default_headers(){
            instance_->set_header("Host" , std::string(hostname_));
            instance_->set_header("Content-Type" , "application/json");
            instance_->set_header("Accept" , "*/*");
            
            return ;
        }
        char hostname_[256];
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
http_request_builder_v1<T>::self_type & http_request_builder_v1<T>::add_query_param(std::vector<std::pair<std::string , std::string > > & params){
    impl_->query_param(params);
    return *this;
}

template<typename T>
http_request_builder_v1<T>::self_type & http_request_builder_v1<T>::add_query_param(std::string key , std::string value){
    impl_->query_param(key , value);
    return *this;
}


template<typename T>
http_request_builder_v1<T>::self_type & http_request_builder_v1<T>::set_version(std::string version){
    impl_->version(version);
    return *this;
}


template<typename T>
http_request_builder_v1<T>::self_type & http_request_builder_v1<T>::add_header(std::string key , std::string value){
    impl_->header(key , value);
    return *this;
}

template<typename T>
http_request_builder_v1<T>::self_type & http_request_builder_v1<T>::add_header(std::vector<std::pair<std::string , std::string > > & headers){
    impl_->header(headers);
    return *this;
}

template<typename T>
http_request_builder_v1<T>::self_type & http_request_builder_v1<T>::set_body(basic_object_interface &body){
    impl_->body(body);
    return *this;
}

template<typename T>
http_request_builder_v1<T>::self_type & http_request_builder_v1<T>::set_headers(basic_http_headers_interface & headers){
    assert(0);
    return *this;
}
