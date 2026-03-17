#ifndef __JSON_ARRAY_H__
#define __JSON_ARRAY_H__

#include "token.h"
#include "json_fwd.h"
#include <memory>


template<typename T ,
        typename return_type>
class Iterator{
    public:
        return_type getNext(){
            //
            return impl_->storage_[idx_++];
        }
        Iterator(T * impl) : impl_(impl) {}
        bool hasNext(){
            return idx_ < impl_->storage_.size();
        }

        bool operator==(int size){
            return idx_ >= size;
        }

        bool operator==(Iterator<T,return_type> & itr){
            return itr.getIdx() == idx_;
        }

        int getIdx(){
            return idx_;
        }

    private:
        T * impl_;
        int idx_;
};

template< typename token_policy ,
          typename buffer_policy >
class json_array_impl{
    
    public:

        using tokenizer_type = token_policy;
        using value_type    = std::vector<variants> ;
        using buffer_type = buffer_policy;
        using token_type  = token_policy::token_type;
        using token_handler  = token_policy::token_handler;
        
        using iterator = Iterator<json_array_impl , variants> *;
        
        friend class Iterator<json_array_impl , variants> ;

        json_array_impl(tokenizer_type * tokenizer) {
            parse(tokenizer);
        }

        json_array_impl(buffer_type * buffer) {
            tokenizer_type * tokenizer(buffer->data + buffer->head);
            parse(tokenizer);
        }

        json_array_impl * parse(tokenizer_type * tokenizer){
            assert(tokenizer->getNext()->compare('['));
            ++(*tokenizer);
            while(tokenizer->hasNext()){
                if(tokenizer->peek_next_char() == ']'){
                    tokenizer->get_next_char();
                    break;
                }
                token_type * nextToken = tokenizer->getNext();
                storage_.push_back(nextToken->cb(tokenizer));
            }
            return this;
        }

        iterator begin(){
            return new iterator(this);
        }
        
        value_type & get(){
            return storage_;
        }

        ssize_t size(){
            return storage_.size();
        }

    private:
        value_type storage_;
        tokenizer_type * tokenizer_;
};


#endif
