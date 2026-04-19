#ifndef __JSON_OBJECT_H__
#define __JSON_OBJECT_H__


#include "basic_object_interface.h"
#include <map>
#include "basic_parser_interface.h"
#include "buffer.h"

class json_object_v1 : public basic_object_interface ,
                        public basic_parser_interface<buffer_v1>{
    public:
        using buffer_type = basic_parser_interface::buffer_type;

        json_object_v1(basic_json_tokenizer_interface & tokenizer);
        json_object_v1();
        json_object_v1(json_object_v1 && obj);
        json_object_v1(json_object_v1 & obj);
        json_object_v1(buffer_type * buffer);
        json_object_v1(const char *  input);

        std::map<std::string , basic_object_interface  *> & value() ;

        std::string serialize() override;
        std::string serialize(basic_formatter_interface & formatter) override;

        void parse(basic_json_tokenizer_interface & tokenizer) override;

        basic_object_interface & operator[](std::string key) ;
    
        //copy
        json_object_v1 & operator=(json_object_v1 & copy_obj) ;

        json_object_v1 & operator=(json_object_v1 && move_obj) ;

        basic_object_interface & get(std::string   key);

        json_object_v1 & push(std::string key  , basic_object_interface * object);

        bool continue_reading() override;

        void at_eof(buffer_type * buffer) override;

        void parse(buffer_type * buffer) override;

        buffer_type * buffer() override;

        std::unique_ptr<basic_object_interface> clone()  override;

    private:
        std::map<std::string , basic_object_interface *>  *storage_;
        buffer_type * buffer_;
};
#endif
