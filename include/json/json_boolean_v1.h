
#ifndef __JSON_BOOLEAN_V1_H__
#define __JSON_BOOLEAN_V1_H__

#include "basic_object_interface.h"

class json_boolean_v1 : public basic_object_interface{

    public:
        json_boolean_v1(basic_json_tokenizer_interface & tokenizer) ;

        json_boolean_v1(json_boolean_v1 & obj);
        json_boolean_v1(json_boolean_v1 &&obj);
        json_boolean_v1(bool val);

        bool & value() ;

        std::string serialize() override;
        std::string serialize(basic_formatter_interface & formatter) override;
        json_boolean_v1 & operator=(json_boolean_v1 &);
        json_boolean_v1 & operator=(json_boolean_v1 &&);

        std::unique_ptr<basic_object_interface> clone() override;
        void parse(basic_json_tokenizer_interface & tokenizer) override;
    private:
        bool value_;
};


#endif
