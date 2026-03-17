#ifndef __BASIC_CONFIG_READER_H__
#define __BASIC_CONFIG_READER_H__

#include <functional>
#include "logger.h"
#include <map>
#include <stdexcept>
#include <string>
template<typename impl>
class basic_config_parser {
    
    public:
        using value_type = impl::value_type;
        using key_type = impl::key_type;
        using buffer_type = impl::buffer_type;

        basic_config_parser() : impl_(new impl()) {}

        basic_config_parser(buffer_type * buffer) : impl_(new impl(buffer)) {}

        bool continue_reading(){
            return impl_->continue_reading();
        }

        basic_config_parser & parse(buffer_type * buffer){
            impl_->parse(buffer);
            return *this;
        }

        value_type  operator[](key_type key){
            return (*impl_)[key];
        }

        
    private:
        impl * impl_;

};


template<typename container_impl>
class ini_parser_impl{
    public:
        using buffer_type = container_impl::buffer_type;
        using value_type  = container_impl ;
        using parent_type = container_impl::parent_type;
        using container_type = std::map<parent_type , container_impl *>;
        using key_type    = parent_type ;

        using callback = std::function<bool(buffer_type * buffer)>;
        
        bool extract(buffer_type * buffer){

            do{
                // continue reading 
                if(impl_->parse(buffer))break;
                containers_[impl_->get_id()] = impl_;
                impl_ = new container_impl();
            }while(1);

            return true;
        }

        ini_parser_impl(buffer_type * buffer) :   impl_(new container_impl(buffer)){ 
            cb = [this](buffer_type *buff){return extract(buff);};
            cb(buffer);
        }

        ini_parser_impl():   impl_(new container_impl()){
            cb = [this](buffer_type *buff){return extract(buff);};
        }

        bool continue_reading(){
            return impl_->continue_reading();
        }


        ini_parser_impl & parse(buffer_type * buffer){
            cb(buffer);
            return *this;
        }


        value_type  operator[](key_type key){
            return *containers_[key];
        }

    private:
        callback cb;
        container_type containers_;
        container_impl * impl_;
};

template<typename buffer_policy>
class ini_config_container_impl {
    public:
        using buffer_type = buffer_policy;
        using key_type    = std::string ;
        using value_type    = std::string ;
        using parent_type    = std::string ;
        using children_type = std::map< key_type ,  value_type>;
        using callback = std::function<bool(buffer_type * buffer)>;

        ini_config_container_impl(buffer_type * buffer) {
            cb_ = [this](buffer_type  * buff){
                return extract_parent(buff);
            };
            cb_(buffer);    
        }

        ini_config_container_impl() {
            cb_ = [this](buffer_type  * buff){
                return extract_parent(buff);
            };
        }

        bool parse(buffer_type * buffer){
            return cb_(buffer);
        }

        bool extract_parent(buffer_type * buffer){
            
            if(buffer == nullptr || strchr( (buffer->data + buffer->head) , ']') == nullptr)return true;

            assert(*(buffer->data + buffer->head++) == '[' );
            id_ = "";
            do{
                if(*(buffer->data + buffer->head) == ']'){
                    buffer->head++;
                    cb_ = [this](buffer_type * buffer){
                        return extract_children(buffer);
                    };
                    return cb_(buffer);
                }
                id_ += *(buffer->data + buffer->head++);
            }while(buffer->head < buffer->tail);

            return true;
        }

        bool extract_children(buffer_type * buffer){

            if(buffer == nullptr || strchr((buffer->data + buffer->head ), '\n') == nullptr )return true;
            key_type key = "";
            value_type value = "";
            short turn = 0;

            do{
                if(*(buffer->data + buffer->head) == '['){
                    cb_ = [this](buffer_type * buffer){
                        return false;
                    };
                    return cb_(buffer);
                }
                if(*(buffer->data + buffer->head) == '\r'){
                    buffer->head++;
                    continue;
                }
                if(*(buffer->data + buffer->head) == '\n'){
                    // done adding 
                    children_[key ] = value;
                    buffer->head++;
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

        parent_type & get_id(){
            return id_;
        }


        bool continue_reading(){
            return cb_(nullptr);
        }
        
        parent_type & parent(){
            return id_;
        }

        children_type & children(){
            return children_;
        }
        
        value_type  operator[](key_type key){
            auto itr = children_.find(key);
            if(itr == children_.end())
                throw std::runtime_error("Key Not found");

            return itr->second;
        }

    private:
        parent_type id_;
        children_type children_;
        callback cb_;

};

#endif
