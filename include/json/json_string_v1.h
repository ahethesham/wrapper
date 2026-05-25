#ifndef __JSON_STRING_H__
#define __JSON_STRING_H__
#include "basic_object_interface.h"
class json_string_v1 : public basic_object_interface{
    
    public:
        json_string_v1(json_tokenizer & tokenizer) ;
        json_string_v1(std::shared_ptr<basic_tokenizer_interface<basic_object_interface>> tokenizer);
        json_string_v1(const char * input);
        json_string_v1(const std::string & input);
        json_string_v1(json_string_v1 &);
        json_string_v1(json_string_v1 &&);
       
        std::string & value() ;

        std::string serialize() override;
        std::string serialize(basic_formatter_interface & formatter) override;

        void parse(json_tokenizer & tokenizer) override;
        void parse(std::shared_ptr<basic_tokenizer_interface<basic_object_interface>> tokenizer ) override;

        json_string_v1 & operator=(json_string_v1 &) ;
        json_string_v1 & operator=(json_string_v1 &&) ;
        std::shared_ptr<basic_object_interface> clone() override;

        std::string get_body_type() override;
        bool verify_body_type(const std::string & body_type) override;

        // parse methods
        bool continue_reading() override;
        void at_eof(buffer_type * buffer) override;
        void parse(buffer_type * buffer) override;
        buffer_type * buffer() override;

        json_string_v1 & clear() override;

        int size() override;

    private:
        std::string *value_;
};

#endif
