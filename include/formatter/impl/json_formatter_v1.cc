#include "json_formatter_v1.h"

json_formatter_v1::json_formatter_v1() : tab_count_(0){}

json_formatter_v1 & json_formatter_v1::line_pre_processor(std::string & str){

    for(int i = 0; i < tab_count_ ; i++){
        str += '\t';
    }
    
    return *this;

}

json_formatter_v1 & json_formatter_v1::line_post_processor(std::string & str){
    str += '\n';
    return *this;
}

json_formatter_v1 & json_formatter_v1::increment_tab_count(){
    tab_count_++;
    return *this;
}

json_formatter_v1 & json_formatter_v1::decrement_tab_count(){
    tab_count_--;
    return *this;
}
