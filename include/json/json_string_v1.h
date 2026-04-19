#ifndef __JSON_STRING_H__
#define __JSON_STRING_H__
#include "basic_object_interface.h"
class json_string_v1 : public basic_object_interface{
    
    public:
        json_string_v1(basic_json_tokenizer_interface & tokenizer) ;
        json_string_v1(const char * input);
        json_string_v1(json_string_v1 &);
        json_string_v1(json_string_v1 &&);
       
        std::string & value() ;

        std::string serialize() override;
        std::string serialize(basic_formatter_interface & formatter) override;

        void parse(basic_json_tokenizer_interface & tokenizer) override;

        json_string_v1 & operator=(json_string_v1 &) ;
        json_string_v1 & operator=(json_string_v1 &&) ;
        std::unique_ptr<basic_object_interface> clone() override;


    private:
        std::string *value_;
};

#endif
