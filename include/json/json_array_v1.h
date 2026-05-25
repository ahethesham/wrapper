#ifndef __JSON_ARRAY_H__
#define __JSON_ARRAY_H__

#include "basic_object_interface.h"
#include <vector>

class json_array_v1 : public basic_object_interface{

    public:
        // have ur own tokenizer when testing
        json_array_v1(json_tokenizer & tokenizer) ;
        json_array_v1(std::shared_ptr<basic_tokenizer_interface<basic_object_interface>> tokenizer);
        json_array_v1(json_array_v1 & obj);
        json_array_v1(json_array_v1 && obj);
        json_array_v1(const char * input);
        json_array_v1();
        
        std::shared_ptr<std::vector<std::shared_ptr<basic_object_interface >>> value() ;
       
        std::string serialize() override;
        // have your own formatter when testing 
        std::string serialize(basic_formatter_interface & formatter) override;

        std::shared_ptr<basic_object_interface> clone()override;
        void parse(json_tokenizer & tokenizer) override;
        void parse(std::shared_ptr<basic_tokenizer_interface<basic_object_interface>> tokenizer) override;
        
        // specific to json array 
        json_array_v1 & push(std::shared_ptr<basic_object_interface> obj);
    
        basic_object_interface & operator[](int idx);
        json_array_v1 & operator=(json_array_v1 & obj);
        json_array_v1 & operator=(json_array_v1 && obj);

        std::string get_body_type() override;
        bool verify_body_type(const std::string & body_type) override;

        //parse methods 
        buffer_type * buffer() override;
        void parse(buffer_type * buffer) override;
        void at_eof(buffer_type * buffer) override;
        bool continue_reading( )override;
       
        json_array_v1 & clear() override;

        int size() override;
	
    private:
        std::shared_ptr<std::vector<std::shared_ptr<basic_object_interface >>> storage_;
};
#endif
