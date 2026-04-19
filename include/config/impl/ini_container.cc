#include "ini_config_parser.h"
#include <cassert>
#include <iostream>
#include <ostream>


ini_container::ini_container(std::string & parent) : parent_(parent){}

ini_container::value_type &  ini_container::operator[](key_type  key){
            auto itr = children_.find(key);
            if(itr == children_.end())throw std::runtime_error("key not found");
            return itr->second;
}

ini_container &  ini_container::push(key_type & key  , value_type & value){
            if(children_.find(key) != children_.end())throw std::runtime_error("Duplicate entry found");
            children_[key] = value;
            return *this;
}

bool ini_container::parse(buffer_v1 * buffer){
    
    if(!buffer)return true;

    if(buffer == nullptr || strchr( (buffer->data + buffer->head) , ']') == nullptr)return true;

    trim(buffer , (char *)" \n\r\t");
    skip_comment(buffer);

    assert(*(buffer->data + buffer->head++) == '[' );
    parent_ = "";
    do{
        if(*(buffer->data + buffer->head) == ']'){
            buffer->head++;
            cb_ = [this](buffer_v1 * buffer){
                return extract_pair(buffer);
            };
            return cb_(buffer);
        }
        parent_ += *(buffer->data + buffer->head++);
    }while(buffer->head < buffer->tail);
    
    return true;
}

bool ini_container::extract_pair(buffer_v1 * buffer){

    if(buffer == nullptr || strchr((buffer->data + buffer->head ), '\n') == nullptr )return true;
    key_type key = "";
    value_type value = "";
    short turn = 0;
    skip_comment(buffer);

    do{
        // this section has been completed
        if(*(buffer->data + buffer->head) == '['){
            cb_ = [this](buffer_v1 * buffer){
                return false;
            };
            return cb_(buffer);
        }

        trim(buffer , (char *)"\r");
        
        if(*(buffer->data + buffer->head) == '\n'){
            // done adding 
            if(key == "" && value != "" ||
               value == "" && key != "" )throw std::runtime_error("error in config section ");
            buffer->head++;
            skip_comment(buffer);
            if(key == "" && value == "")continue;
            children_[key ] = value;
            if(strchr((buffer->data + buffer->head) , '\n') == nullptr)break;
            key = "";
            value = "";
            turn = 0;
            continue;
        }
        if(*(buffer->data + buffer->head) == '='){
            turn = 1;
            buffer->head++;
            continue;
        }

        if(!turn )
            key += *(buffer->data + buffer->head ++);
        else
            value += *(buffer->data + buffer->head ++);
    }while(1);

    return true;
}

std::string ini_container::parent(){
    return parent_;
}

void ini_container::trim(buffer_v1 * buffer , char * sep){
    while(buffer->head < buffer->tail && strchr(sep , *(buffer->data + buffer->head)) != NULL)buffer->head++;
    return ;
}

void ini_container::skip_comment(buffer_v1 * buffer){
    if(*(buffer->data + buffer->head) != '#')return ;
    if(strchr((buffer->data + buffer->head) , '\n') == NULL)return ;
    while(*(buffer->data + buffer->head) == '#'){
        while(*(buffer->data + buffer->head++) != '\n');
        trim(buffer , (char *)"\r\n");
    }
    return ;
}

void ini_container::at_eof(buffer_v1 * buffer){
    if(buffer->head == buffer->tail)return ;
    if(strchr((buffer->data + buffer->head) , '\n') == NULL)
        buffer->data[buffer->tail++] = '\n';
    
    extract_pair(buffer);
}

bool ini_container::continue_reading(){
    return true;
}


