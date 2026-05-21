#ifndef __JSON_INTEGER_V1_H__
#define __JSON_INTEGER_V1_H__

#include "basic_object_interface.h"

class json_integer_v1 : public basic_object_interface{

    public:
        json_integer_v1(json_tokenizer & tokenizer) ;
        json_integer_v1(std::shared_ptr<basic_tokenizer_interface<basic_object_interface>> tokenizer) ;
        json_integer_v1(json_integer_v1 &);
        json_integer_v1(json_integer_v1 &&);
        json_integer_v1(int val);

        json_integer_v1 & operator=(json_integer_v1 & obj);
        json_integer_v1 & operator=(json_integer_v1 && obj);

        int & value() ;

        std::string serialize() override;
        std::string serialize(basic_formatter_interface & formatter) override;

        void parse(json_tokenizer & tokenizer) override;
        void parse(std::shared_ptr<basic_tokenizer_interface<basic_object_interface>> tokenizer) override;
        std::shared_ptr<basic_object_interface> clone() override;

        std::string get_body_type() override;

        //parse methods 
        buffer_type * buffer() override;
        void parse(buffer_type * buffer) override;
        void at_eof(buffer_type * buffer) override;
        bool continue_reading( )override;

        json_integer_v1 & clear() override;

        int size() override;
    private:
        int value_;
};


#endif
