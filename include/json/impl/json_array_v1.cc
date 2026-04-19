#include "basic_json_tokenizer_interface.h"
#include "json_array_v1.h"
#include "json_builder.h"

json_array_v1::json_array_v1(basic_json_tokenizer_interface & tokenizer):storage_(new std::vector<basic_object_interface *>()){
    parse(tokenizer);
}
json_array_v1::json_array_v1(json_array_v1 & obj):storage_(new std::vector<basic_object_interface*>()){

    for(auto itr : *obj.storage_){
        // TODO
        storage_->push_back(itr->clone().get());
    }

}
json_array_v1::json_array_v1(json_array_v1 &&  obj):storage_(nullptr){

    storage_ = obj.storage_;
    obj.storage_ = nullptr;
}

/*
 * Will be mostly used for mocking tests
 */
json_array_v1::json_array_v1(const char * input):storage_(nullptr){

    auto tokenizer = *tokenizer_builder<json_tokenizer>(input);
    parse(tokenizer);
}

json_array_v1::json_array_v1() : storage_(new std::vector<basic_object_interface *>()){}


std::vector<basic_object_interface *> &json_array_v1::value(){
    return *storage_;
}

std::string json_array_v1::serialize(){
    std::string res ;
    res = '[';
    for(int idx = 0 ; idx < storage_->size() ; idx++){
        res += (*storage_)[idx]->serialize();
        if(idx + 1 < storage_->size())
            res += ',';
    }
    res += ']';
    return res;
}

std::string json_array_v1::serialize(basic_formatter_interface & formatter){
    std::string res = "";
    res = '[';
    formatter.increment_tab_count();
    for(int idx = 0 ; idx < storage_->size() ; idx++){
        formatter.line_post_processor(res);
        formatter.line_pre_processor(res);
        res += (*storage_)[idx]->serialize(formatter);
        if(idx + 1 < storage_->size())
            res += ',';
    }
    formatter.line_post_processor(res);
    formatter.decrement_tab_count();
    formatter.line_pre_processor(res);
    res += ']';
    return res;
}

void json_array_v1::parse(basic_json_tokenizer_interface & tokenizer){
    assert(tokenizer.getNext()->compare('['));
    //++(tokenizer);
    while(tokenizer.hasNext()){
        if(tokenizer.peek_next_valid_char() == ']'){
            tokenizer.get_next_valid_char();
            break;
        }
        auto nextToken = tokenizer.peek();
        assert(nextToken != nullptr);
        storage_->push_back(&nextToken->cb(tokenizer));
    }
    return ;
}

basic_object_interface & json_array_v1::operator[](int idx){
    return *(*storage_)[idx];
}

json_array_v1 & json_array_v1::operator=(json_array_v1 & obj){

    if(&obj == this)return *this;

    storage_->clear();

    for(auto itr : *obj.storage_){
        storage_->push_back(itr->clone().get());
    }
    return *this;
}

json_array_v1 &  json_array_v1::operator=(json_array_v1 && obj){
    if(&obj == this)return *this;
    //TODO add logic to recursively call delete on each memory
    if(storage_ != nullptr){
        for(auto itr :* storage_)delete itr;
        delete storage_;
    }
    storage_ = obj.storage_;
    obj.storage_ = nullptr;
    return *this;
}

std::unique_ptr<basic_object_interface>  json_array_v1::clone(){
    return std::make_unique<json_array_v1>(*this);
}

json_array_v1 & json_array_v1::push(basic_object_interface * obj){
    storage_->push_back(obj);
    return *this;
}
