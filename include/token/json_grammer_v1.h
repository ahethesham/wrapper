#ifndef __JSON_GRAMMER_V1_H__
#define __JSON_GRAMMER_V1_H__
#include "basic_grammer_interface.h"

class json_grammer_v1 : public basic_grammer_interface{
    
    public:
        bool is_end_of_file(char ) override;

        bool is_unexpected_char(char ) override;

        bool continue_reading( char ) override;

        json_grammer_v1(const char * skippable_chars , const char * unexpected_chars , char eof);

    private:
        char * skippable_chars_;
        char * unexpected_chars_;
        char eof_;
};


#endif
