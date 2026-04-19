#ifndef __JSON_ARRAY_H__
#define __JSON_ARRAY_H__

#include "basic_object_interface.h"
#include <vector>

class json_array_v1 : public basic_object_interface{

    public:
        // have ur own tokenizer when testing
        json_array_v1(basic_json_tokenizer_interface & tokenizer) ;
        json_array_v1(json_array_v1 & obj);
        json_array_v1(json_array_v1 && obj);
        json_array_v1(const char * input);
        json_array_v1();
        
        std::vector<basic_object_interface *> & value() ;
       
        std::string serialize() override;
        // have your own formatter when testing 
        std::string serialize(basic_formatter_interface & formatter) override;

        std::unique_ptr<basic_object_interface> clone()override;
        void parse(basic_json_tokenizer_interface & tokenizer) override;
        json_array_v1 & push(basic_object_interface * obj);
    
        basic_object_interface & operator[](int idx);
        json_array_v1 & operator=(json_array_v1 & obj);
        json_array_v1 & operator=(json_array_v1 && obj);

    private:
        std::vector<basic_object_interface *> *storage_;
};
#endif
