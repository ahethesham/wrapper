#ifndef __JSON_NULL_V1_H__
#define __JSON_NULL_V1_H__

#include "basic_object_interface.h"
class json_null_v1 : public basic_object_interface{

    public:
        json_null_v1(json_tokenizer & tokenizer) ;
        json_null_v1();
        json_null_v1(json_null_v1 & obj);
        json_null_v1(json_null_v1 && obj);
        //json_null_v1(const char * input);

        bool operator==(json_null_v1 & cmp);

        json_null_v1& operator=(json_null_v1 & obj);
        json_null_v1 & operator=(json_null_v1 && obj);


        std::string & value() ;

        std::string serialize() override;
        std::string serialize(basic_formatter_interface & formatter) override;
        std::shared_ptr<basic_object_interface> clone() override;

        void parse(json_tokenizer & tokenizer) override;


        std::string get_body_type() override;

        //parse methods 
        buffer_type * buffer() override;
        void parse(buffer_type * buffer) override;
        void at_eof(buffer_type * buffer) override;
        bool continue_reading( )override;

        json_null_v1 & clear() override;
    private:
        std::string *value_;
};


#endif
