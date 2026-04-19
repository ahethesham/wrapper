#ifndef __BASIC_FORMATTER_INTERFACE_H__
#define __BASIC_FORMATTER_INTERFACE_H__

#include <string>
class basic_formatter_interface{

    public:
        /*
         *should take care of adding all the necessary characters to the str before we go ahead  and add a line
         */
        virtual basic_formatter_interface &line_pre_processor(std::string & str) = 0;
        virtual basic_formatter_interface &line_post_processor(std::string & str) = 0;

        virtual basic_formatter_interface & increment_tab_count() = 0;
        virtual basic_formatter_interface & decrement_tab_count() = 0;

};


#endif
