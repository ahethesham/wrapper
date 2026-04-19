#ifndef __BASIC_GRAMMER_INTERFACE_H__
#define __BASIC_GRAMMER_INTERFACE_H__

class basic_grammer_interface{

    public:
        virtual bool is_end_of_file(char) = 0;

        virtual bool is_unexpected_char(char) = 0;

        virtual bool continue_reading(char) = 0;
};

#endif
