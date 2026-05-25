#ifndef __JSON_FWD_H__
#define __JSON_FWD_H__

#include "json_array_v1.h"
#include "json_formatter_v1.h"
#include "json_object_v1.h"
#include "json_boolean_v1.h"
#include "json_string_v1.h"
#include "json_integer_v1.h"

using json_integer = json_integer_v1;
using json_string = json_string_v1;
using json_object = json_object_v1;
using json_boolean = json_boolean_v1;
using json_array = json_array_v1;

class json_body{
    public:
        json_body() : obj_(std::make_shared<json_object>()){}

        inline json_body & put(const std::string & key , int val){
            obj_->push(key , std::make_shared<json_integer>(val));
            return *this;
        }
        inline json_body & put(const std::string & key ,const std::string & value){
            obj_->push(key , std::make_shared<json_string>(value));
            return *this;
        }
        inline json_body & put(std::string & key , bool val){
            obj_->push(key , std::make_shared<json_boolean>(val));
            return *this;
        }
        inline json_body & put(std::string & key , const json_body & val){
            obj_->push(key , val.obj_);
            return *this;
        }
        operator std::shared_ptr<basic_object_interface>(){
            return obj_;
        }
        inline json_body & put(std::string & key , std::shared_ptr<json_array> arr){
            obj_->push(key , arr);
            return *this;
        }

        inline json_body & put(const std::string & key , std::shared_ptr<basic_object_interface> val){
            obj_->push(key , val);
            return *this;
        }
        std::string  serialize(){
            return obj_->serialize();
        }
        std::string serialize(json_formatter_v1 formatter){
            return obj_->serialize(formatter);
        }
    private:
        std::shared_ptr<json_object_v1> obj_;
};
#endif

