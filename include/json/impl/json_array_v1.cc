#include "basic_object_interface.h"
#include "json_array_v1.h"
#include "json_builder.h"

json_array_v1::json_array_v1(json_tokenizer & tokenizer):storage_(std::make_shared<std::vector<std::shared_ptr<basic_object_interface >>>()){
    parse(tokenizer);
}
json_array_v1::json_array_v1(std::shared_ptr<basic_tokenizer_interface<basic_object_interface>> tokenizer):storage_(std::make_shared<std::vector<std::shared_ptr<basic_object_interface >>>()){
    parse(tokenizer);
}

json_array_v1::json_array_v1(json_array_v1 & obj):storage_(std::make_shared<std::vector<std::shared_ptr<basic_object_interface >>>()){

    for(auto itr : *obj.storage_){
        // TODO
        storage_->push_back(itr->clone());
    }

}
json_array_v1::json_array_v1(json_array_v1 &&  obj):storage_(nullptr){

    storage_ = obj.storage_;
    obj.storage_ = nullptr;
}

/*
 * Will be mostly used for mocking tests
 */
json_array_v1::json_array_v1(const char * input):storage_(std::make_shared<std::vector<std::shared_ptr<basic_object_interface >>>()){

    auto tokenizer = *tokenizer_builder<json_tokenizer>(input);
    parse(tokenizer);
}

json_array_v1::json_array_v1() : storage_(std::make_shared<std::vector<std::shared_ptr<basic_object_interface >>>()){}


std::shared_ptr<std::vector<std::shared_ptr<basic_object_interface >>> json_array_v1::value(){
    return storage_;
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

void json_array_v1::parse(json_tokenizer & tokenizer){
#if 0
    assert(tokenizer.getNext()->compare('['));
    //++(tokenizer);
    while(tokenizer.hasNext()){
        if(tokenizer.peek_next_valid_char() == ']'){
            tokenizer.get_next_valid_char();
            break;
        }
        auto nextToken = tokenizer.peek();
        assert(nextToken != nullptr);
        storage_->push_back(nextToken->cb(tokenizer));
    }
#endif
    return ;
}
void json_array_v1::parse(std::shared_ptr<basic_tokenizer_interface<basic_object_interface>> tokenizer){
    assert(tokenizer->getNext()->compare('['));
    //++(tokenizer);
    while(tokenizer->hasNext()){
        if(tokenizer->peek_next_valid_char() == ']'){
            tokenizer->get_next_valid_char();
            break;
        }
        auto nextToken = tokenizer->peek();
        assert(nextToken != nullptr);
        storage_->push_back(nextToken->cb(tokenizer));
    }
}

basic_object_interface & json_array_v1::operator[](int idx){
    return *(*storage_)[idx];
}

json_array_v1 & json_array_v1::operator=(json_array_v1 & obj){

    if(&obj == this)return *this;

    storage_->clear();

    for(auto itr : *obj.storage_){
        storage_->push_back(itr->clone());
    }
    return *this;
}

json_array_v1 &  json_array_v1::operator=(json_array_v1 && obj){
    if(&obj == this)return *this;
    //TODO add logic to recursively call delete on each memory
    if(storage_ != nullptr){
        for(auto itr :* storage_)itr = nullptr;
         storage_ = nullptr;
    }
    storage_ = obj.storage_;
    obj.storage_ = nullptr;
    return *this;
}

std::shared_ptr<basic_object_interface>  json_array_v1::clone(){
    return std::make_shared<json_array_v1>(*this);
}

json_array_v1 & json_array_v1::push(std::shared_ptr<basic_object_interface>  obj){
    storage_->push_back(obj);
    return *this;
}

std::string json_array_v1::get_body_type(){
    return "application/string";
}

//parser methods
json_array_v1::buffer_type * json_array_v1::buffer(){
    //TODO
    assert(false);
    return nullptr;
}
void json_array_v1::parse(json_array_v1::buffer_type * buffer){
    assert(*(buffer->data + buffer->head) == '[');
    auto tokenizer = tokenizer_builder<json_tokenizer>((buffer->data + buffer->head));
    parse(tokenizer);
    return ;
}
void json_array_v1::at_eof(json_array_v1::buffer_type * buffer){
    // TODO
    return ;
}
bool json_array_v1::continue_reading(){
    return false;
}

json_array_v1 & json_array_v1::clear(){
    for(auto x : *storage_){
        x->clear();
        x = nullptr;
    }
    return *this;
}

int json_array_v1::size(){
    int total = 0;
    for(auto itr : *storage_){
        total += itr->size();
        total++; // for a ,
    }
    if(total > 0)
        total--;
    total += 2; // for []
    return total;
}

bool json_array_v1::verify_body_type(const std::string & body){
    return get_body_type() == body ;
}
