#ifndef __BASIC_OBJECT_H
#define __BASIC_OBJECT_H
#include <cassert>
#include "json_fwd.h"

template<typename impl>
class basic_object : public impl{

    public:
        using buffer_type = impl::buffer_type;
        using tokenizer_type = impl::tokenizer_type;
        using value_type     = impl::value_type;


        bool process(buffer_type * buffer){
            return impl::process(buffer);
        }

        std::string serialize(){
            return impl::serialize();
        }

        basic_object(tokenizer_type * tokenizer) : impl(tokenizer) {}

        void parse(buffer_type * buffer){
            //impl_->parse(buffer);
            return ;
        }

        void parse(tokenizer_type * tokenizer){
            impl::parse(tokenizer);
            return ;
        }
        static inline variants  build(json_tokenizer * tokenizer){
            return new basic_object<impl>(tokenizer);
        }

        const value_type & get() {
            return impl::get();
        }


};


#endif
