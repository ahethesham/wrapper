#include "http_formatter_v1.h"

http_formatter_v1::http_formatter_v1() : tab_count_(0) {}

http_formatter_v1::self_type & http_formatter_v1::line_post_processor(std::string & str){
    // add "\r\n" to the str 
    str += "\r\n";
    return *this;
}

http_formatter_v1::self_type & http_formatter_v1::line_pre_processor(std::string & str){
    // nothing needs to be done 
    for(int i = 0 ; i < tab_count_ ; i++)
        str += "\t";
    return *this;
}

http_formatter_v1::self_type & http_formatter_v1::increment_tab_count(){
    tab_count_++;
    return *this;
}
http_formatter_v1::self_type & http_formatter_v1::decrement_tab_count(){
    tab_count_--;
    return *this;
}

http_formatter_v1::self_type & http_formatter_v1::header_post_processor(std::string & res){
    res += "\r\n";
    return *this;
}

http_formatter_v1::self_type & http_formatter_v1::body_post_processor(std::string & res){
    return *this;
}
