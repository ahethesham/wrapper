#ifndef __JSON_INTEGER_V1_H__
#define __JSON_INTEGER_V1_H__

#include "basic_object_interface.h"

class json_integer_v1 : public basic_object_interface{

    public:
        json_integer_v1(basic_json_tokenizer_interface & tokenizer) ;
        json_integer_v1(json_integer_v1 &);
        json_integer_v1(json_integer_v1 &&);
        json_integer_v1(int val);

        json_integer_v1 & operator=(json_integer_v1 & obj);
        json_integer_v1 & operator=(json_integer_v1 && obj);

        int & value() ;

        std::string serialize() override;
        std::string serialize(basic_formatter_interface & formatter) override;

        void parse(basic_json_tokenizer_interface & tokenizer) override;
        std::unique_ptr<basic_object_interface> clone() override;
    private:
        int value_;
};


#endif
