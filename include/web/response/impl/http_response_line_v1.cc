#include "http_response_line_v1.h"
#include "file_logger_v1.h"
#include <cassert>


class http_response_line_v1::impl{
    public:
        impl() : status_(::status(200)) , version_("HTTP/1.1"){}
        impl(http_status & status) : status_(status) , version_("HTTP/1.1"){}
        impl(http_status & status , std::string &version) : status_(status) , version_(version){}

        std::string & version(){
            return version_;
        }
        const http_status & status(){
            return status_;
        }
        impl & set_version(std::string & version){
            version_ = version;
            return *this;
        }
        impl & set_status(const http_status & status){
            status_ = status;
            return *this;
        }
        //parser methods
        void parse(buffer_type * buffer){
            return ;
        }
        void at_eof(buffer_type * buffer){
            return;
        }
        void clear(){
            // reset to default versions
            status_ = ::status(200);
            version_ = std::string("HTTP/1.1");
            return ;
        }
        bool continue_reading(){
            return false;
        }
        buffer_type * buffer(){
            return nullptr;
        }

        // serialize , deserialize
        std::string serialize() {
            std::string res = "";
            res += std::to_string(status_.code);
            res += " ";
            res += status_.message;
            res += " ";
            res += version_;
            res += "\r\n";
            return res;
        }
        std::string serialize(basic_formatter_interface & formatter){
            std::string res = "";
            res += std::to_string(status_.code);
            res += " ";
            res += status_.message;
            res += " ";
            res += version_;
            formatter.line_post_processor(res);
            return res;
        }
    private:
        http_status &status_;
        std::string version_;
};

//constructors
http_response_line_v1::http_response_line_v1(http_status & status) 
    : impl_(std::make_shared< impl>(status)){}

http_response_line_v1::http_response_line_v1()
    : impl_(std::make_shared< impl>(::status(200))) {
}

http_response_line_v1::http_response_line_v1( http_response_line_v1 &obj)
    : impl_(obj.impl_) {
}

http_response_line_v1::http_response_line_v1(http_response_line_v1 &&obj)
    : impl_(obj.impl_) {
    obj.impl_ = nullptr;
}

// get methods 
const http_status & http_response_line_v1::status() const{
    return impl_->status();
}
std::string & http_response_line_v1::version() const{
    return impl_->version();
}

// set method
http_response_line_v1 & http_response_line_v1::set_status(http_status & status){
    impl_->set_status(status);
    return *this;
}
http_response_line_v1 & http_response_line_v1::set_version(std::string version){
    impl_->set_version(version);
    return *this;
}

// operators , clone
http_response_line_v1 & http_response_line_v1::operator=(http_response_line_v1 & obj){
    impl_->set_status(obj.status());
    return *this;
}
http_response_line_v1 & http_response_line_v1::operator=(http_response_line_v1 && obj){
    impl_ = nullptr;
    impl_ = obj.impl_;
    obj.impl_ = nullptr;
    return *this;
}

//parser methods
void http_response_line_v1::parse(buffer_type * buffer){
    impl_->parse(buffer);
    return ;
}
void http_response_line_v1::at_eof(buffer_type * buffer){
    impl_->at_eof(buffer);
    return ;
}
bool http_response_line_v1::continue_reading(){
    return impl_->continue_reading();
}
http_response_line_v1::buffer_type* http_response_line_v1::buffer(){
    return nullptr;
}


std::shared_ptr<basic_http_response_line_interface> http_response_line_v1::clone(){
    return std::make_shared<http_response_line_v1>(*this);
}

//serialize and deserialize
std::string http_response_line_v1::serialize(){
    return impl_->serialize();
}
std::string http_response_line_v1::serialize(basic_formatter_interface & formatter){
    return impl_->serialize(formatter);
}

http_response_line_v1 & http_response_line_v1::clear(){
    impl_->clear();
    return *this;
}


static http_status & find_bs(http_status arr[] , int low , int high , int code){
    int mid;
    while(low <= high){
        mid = (low + high)/2;
        // found it
        if(arr[mid].code == code)
            return arr[mid];

        if(arr[mid].code < code){
            low = mid + 1;
        }else{
            high = mid - 1;
        }
    }
    throw std::runtime_error("Http status code not found");
}

http_status & status(int status){
    static http_status arr[] = {
        { 200 , "OK"} ,
        { 400 , "BAD_REQUEST"} ,
        { 500 , "INTERNAL_SERVER_ERROR"},
        { 503 , "SERVICE_UNAVAILABLE"}
    };
    
    return find_bs(arr , 0 , 3 , status);
}


