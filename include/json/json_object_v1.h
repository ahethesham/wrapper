#ifndef __JSON_OBJECT_H__
#define __JSON_OBJECT_H__


#include "basic_object_interface.h"
#include <map>
#include "basic_parser_interface.h"

class json_object_v1 : public basic_object_interface  {
    public:
        using buffer_type = basic_parser_interface::buffer_type;

        json_object_v1(json_tokenizer & tokenizer);
        json_object_v1(std::shared_ptr<basic_tokenizer_interface<basic_object_interface>> tokenizer);
        json_object_v1();
        json_object_v1(json_object_v1 && obj);
        json_object_v1(json_object_v1 & obj);
        json_object_v1(buffer_type * buffer);
        json_object_v1(const char *  input);

        std::map<std::string , std::shared_ptr<basic_object_interface>  > & value() ;
        std::string serialize() override;
        std::string serialize(basic_formatter_interface & formatter) override;

        void parse(json_tokenizer & tokenizer) override;
        void parse(std::shared_ptr<basic_tokenizer_interface<basic_object_interface>> tokenizer) override;

        basic_object_interface & operator[](std::string key) ;
    
        //copy
        json_object_v1 & operator=(json_object_v1 & copy_obj) ;

        json_object_v1 & operator=(json_object_v1 && move_obj) ;

        basic_object_interface & get(std::string   key);

        json_object_v1 & push(std::string key  , std::shared_ptr<basic_object_interface>  object);

        // parse methods
        bool continue_reading() override;
        void at_eof(buffer_type * buffer) override;
        void parse(buffer_type * buffer) override;
        buffer_type * buffer() override;

        std::shared_ptr<basic_object_interface> clone()  override;

        std::string get_body_type() override;
        json_object_v1 & clear() override;

        // should return the raw size of the body , ignore all the tabs , newlines and other formatters 
        int size() override;
        ~json_object_v1();
    private:
        std::shared_ptr<std::map<std::string , std::shared_ptr<basic_object_interface>>>  storage_;
        buffer_type * buffer_;
};
#endif
