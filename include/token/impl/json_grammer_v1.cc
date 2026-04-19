#include "json_grammer_v1.h"
#include "file_logger_v1.h"
#include <cstring>
#include <stdlib.h>

/*
 * concrete implementation of json_grammer_v1 
 */
json_grammer_v1::json_grammer_v1( const char * skippable_chars , const char * unexpected_chars , char eof): eof_(eof)
{
    int len = strlen(skippable_chars) + 1;
    skippable_chars_ = (char *)malloc(len * sizeof(char));
    memcpy(skippable_chars_ , skippable_chars , len);
    *(skippable_chars_ + len - 1) = '\0';
    len = strlen(unexpected_chars) + 1;
    unexpected_chars_ = (char *)malloc(len * sizeof(char ));
    memcpy(unexpected_chars_ , unexpected_chars , len);
    *(unexpected_chars_ + len - 1) = '\0';
}


bool json_grammer_v1::is_end_of_file(char ch){
    return eof_ == ch;
}

bool json_grammer_v1::is_unexpected_char(char ch){

    return strchr(unexpected_chars_ , ch) != NULL;
}

bool json_grammer_v1::continue_reading(char ch ){
    Logger & logger = Logger::build();
    return strchr(skippable_chars_ , ch) != NULL;
}
