#ifndef __JSON_OBJECT_H__
#define __JSON_OBJECT_H__

#include "json_fwd.h"
#include <map>


template < typename token_policy ,
        typename buffer_policy >
class json_object_impl{
    public:
        using tokenizer_type = token_policy;
        using token_type     = typename token_policy::token_type;
        using iterator       = Iterator<json_object_impl , std::pair<std::string , variants> > *;
        using value_type     = std::map<std::string , variants>;
        using buffer_type    = buffer_policy;
        using token_handler  = typename token_policy::token_handler;

        json_object_impl(tokenizer_type * tokenizer){
            this->parse(tokenizer);
        }


        json_object_impl * parse(tokenizer_type * tokenizer){
            assert(tokenizer->getNext()->compare('{'));
            ++(*tokenizer);

            while(tokenizer->hasNext()){

                if(tokenizer->peek_next_char() == '}'){
                    tokenizer->get_next_char();
                    break;
                }
                token_type * nextToken = tokenizer->getNext();
                // key has to be a string 
                assert(nextToken->compare('\"'));
                json_string * key = std::get<json_string *>(nextToken->cb(tokenizer));
                nextToken = tokenizer->getNext();
                auto value = nextToken->cb(tokenizer);
                if(key->get() == "profile")
                    log("wtf %c" , tokenizer->peek_next_char());
                storage_[key->get()] = value;
            }
            return this;
        }
        
        json_object_impl & push(std::string key , variants & value){
            storage_[key] = value;
        }

        variants operator[](std::string key){
            return storage_[key];
        }

        iterator begin(){
            return new iterator(this);
        }

        int end(){
            return storage_.size();
        }

        value_type & get(){
            return storage_;
        }

        std::string serialize(){
            return "";
        }

        template<typename T >
            T  get(std::string & key){
                auto val = storage_[key];
                if(std::get_if<T*>(storage_[key]) == nullptr)
                    throw std::runtime_error("ERR_EVNT_0000006");
                return val;
            }


    private:
        value_type   storage_;
};

#endif
