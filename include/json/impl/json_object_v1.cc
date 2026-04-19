#include "file_logger_v1.h"
#include <stdexcept>
#include "json_object_v1.h"
#include "json_builder.h"
#include "json_string_v1.h"
#include "json_tokenizer_v1.h"

json_object_v1::json_object_v1(basic_json_tokenizer_interface &  tokenizer) : buffer_(nullptr) , storage_(new std::map<std::string , basic_object_interface*>()){    
    this->parse(tokenizer);
}

json_object_v1::json_object_v1() : buffer_(nullptr) , storage_(new std::map<std::string , basic_object_interface*>()){}

/*
 * Move construct
 */
json_object_v1::json_object_v1(json_object_v1 && obj) : buffer_(nullptr) , storage_(nullptr){
    storage_ = obj.storage_;
    obj.storage_ = nullptr;
}
/*
 * Copy construct
 */
json_object_v1::json_object_v1(json_object_v1 & obj) : buffer_(nullptr) , storage_(nullptr){
    for(auto itr : *obj.storage_){
        (*storage_)[itr.first] = itr.second;
    }
}

json_object_v1::json_object_v1(const char * input) : buffer_(nullptr) , storage_(new std::map<std::string , basic_object_interface *>()){
    json_tokenizer & tokenizer = *tokenizer_builder<json_tokenizer>(input);
    parse(tokenizer);
}

std::map<std::string , basic_object_interface *> & json_object_v1::value(){
    return *storage_;
}

std::string json_object_v1::serialize(){
    std::string res = "{";
    for(auto itr : *storage_){
        res += "\r\n\t";
        res += '\"' + itr.first + '\"' + ':' + ' ';
        res += itr.second->serialize();
    }
    if(storage_->size() > 0)
        res += "\r\n\t}";
    else 
        res += '}';
    return res;
}

std::string json_object_v1::serialize(basic_formatter_interface & formatter){
    std::string res = "{";
    formatter.increment_tab_count();
    bool starting = true;
    for(auto itr : *storage_){
        if(!starting)
            res +=  ',';
        formatter.line_post_processor(res);
        formatter.line_pre_processor(res);

        starting = false;
        res += '\"' + itr.first + '\"' + ':' + ' ';
        res += itr.second->serialize(formatter);
    }
    formatter.line_post_processor(res);
    formatter.decrement_tab_count();
    formatter.line_pre_processor(res);
    res += '}';
    return res;
}

void json_object_v1::parse(basic_json_tokenizer_interface & tokenizer){

    Logger & logger = Logger::build();

    assert(tokenizer.getNext()->compare('{'));
    //++(tokenizer);


    while(tokenizer.hasNext()){

        if(tokenizer.peek_next_valid_char() == '}'){
            tokenizer.get_next_valid_char();
            break;
        }
        auto nextToken = tokenizer.peek();
        // key has to be a string 
        assert(nextToken->compare('\"'));
    

        std::string key = (nextToken->cb(tokenizer)).get<json_string_v1 >().value();
        // TODO once we are done with extracting the string from it ..destroy the object
        nextToken = tokenizer.peek();
        auto value = &nextToken->cb(tokenizer);
        (*storage_)[key] = value;
    }
    return ;
}

basic_object_interface &  json_object_v1::operator[](std::string key){
    return get(key);
}

basic_object_interface & json_object_v1::get(std::string key){
    auto itr = storage_->find(key);
    if( itr == storage_->end())
        throw std::runtime_error("Requested Key Not found in json object");
    return *itr->second;
}

json_object_v1 & json_object_v1::push(std::string key , basic_object_interface * object){
    (*storage_)[key] = object;
    return *this;
}

bool json_object_v1::continue_reading(){
    return true;
}

void json_object_v1::at_eof(buffer_type * buffer){
    // TODO have some logic to get the tokenizer impl
}

void json_object_v1::parse(buffer_type * buffer){
    // TODO have some logic to get the tokenizer impl
}

json_object_v1::buffer_type *json_object_v1::buffer(){
    //TODO serialize and put it into the buffer
    return nullptr;
}

/*
 * copy assignment  
 */
json_object_v1 & json_object_v1::operator=(json_object_v1 & obj){
    
    if(&obj == this)
        return *this;
    //TODO make sure to delete the pointers associated with this storage 
    storage_->clear();

    for(auto itr : *obj.storage_){
        (*storage_)[itr.first] = itr.second;
    }

    return *this;
}

json_object_v1 & json_object_v1::operator=(json_object_v1 && obj){

    if(&obj == this)
        return *this;
    if(storage_ != nullptr){
        storage_->clear();
        delete storage_;
    }
    storage_ = obj.storage_;
    obj.storage_ = nullptr;
    return *this;
}

std::unique_ptr<basic_object_interface> json_object_v1::clone(){
    return std::make_unique<json_object_v1>(*this);
}
