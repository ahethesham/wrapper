
#ifndef __JSON_BOOLEAN_V1_H__
#define __JSON_BOOLEAN_V1_H__

#include "basic_object_interface.h"

class json_boolean_v1 : public basic_object_interface{

    public:
        json_boolean_v1(json_tokenizer & tokenizer) ;
        json_boolean_v1(std::shared_ptr<basic_tokenizer_interface<basic_object_interface>> tokenizer) ;
        json_boolean_v1(json_boolean_v1 & obj);
        json_boolean_v1(json_boolean_v1 &&obj);
        json_boolean_v1(bool val);

        bool & value() ;

        std::string serialize() override;
        std::string serialize(basic_formatter_interface & formatter) override;
        json_boolean_v1 & operator=(json_boolean_v1 &);
        json_boolean_v1 & operator=(json_boolean_v1 &&);

        std::shared_ptr<basic_object_interface> clone() override;
        void parse(json_tokenizer & tokenizer) override;
        void parse(std::shared_ptr<basic_tokenizer_interface<basic_object_interface>> tokenizer) override;

        std::string get_body_type() override;
        bool verify_body_type(const std::string & type) override;

        //parse methods 
        buffer_type * buffer() override;
        void parse(buffer_type * buffer) override;
        void at_eof(buffer_type * buffer) override;
        bool continue_reading( )override;

        json_boolean_v1 & clear() override;

        int size() override;
    private:
        bool value_;
};


#endif
