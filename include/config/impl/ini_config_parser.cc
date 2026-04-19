#include "ini_config_parser.h"
#include <cassert>
#include <iostream>
#include <ostream>
using ini_config_parser_impl = ini_config_parser<ini_container>;
using container_type = ini_config_parser_impl::container_type;


template<> ini_config_parser_impl::ini_config_parser(file_reader_interface *   file_reader ) : file_reader_(file_reader){
    current_context_ = new container_type();
    file_reader_->read(*this);
}

template<> 
bool ini_config_parser_impl::continue_reading(){
    return current_context_->continue_reading();
}

template<>
void ini_config_parser_impl::parse(buffer_v1 * buffer){
    do{
        if(current_context_->parse(buffer))break;   
        storage_[current_context_->parent()] = current_context_;
        current_context_ = new container_type();
    }while(1);
    return ;
}

template<>
void ini_config_parser_impl::at_eof(buffer_v1 * buffer) {
    current_context_->at_eof(buffer);
    storage_[current_context_->parent()] = current_context_;
    return ;
}

template<>
buffer_v1 * ini_config_parser_impl::buffer(){
    return nullptr;
}

template<>
container_type & ini_config_parser_impl::operator[](std::string key){
    auto itr = storage_.find(key);
    if(itr == storage_.end())throw std::runtime_error("Key not found in Application config data source ");
    return *itr->second;
}
template<>
container_type & ini_config_parser_impl::get(std::string key){
    auto itr = storage_.find(key);
    if(itr == storage_.end())throw std::runtime_error("Key not found in Application config data source ");
    return *itr->second;
}
