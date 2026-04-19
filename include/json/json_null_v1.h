#ifndef __JSON_NULL_V1_H__
#define __JSON_NULL_V1_H__

#include "basic_object_interface.h"
class json_null_v1 : public basic_object_interface{

    public:
        json_null_v1(basic_json_tokenizer_interface & tokenizer) ;
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
        std::unique_ptr<basic_object_interface> clone() override;

        void parse(basic_json_tokenizer_interface & tokenizer) override;
    private:
        std::string *value_;
};


#endif
