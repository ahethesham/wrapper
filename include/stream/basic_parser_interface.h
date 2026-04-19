#ifndef __BASIC_PARSER_INTERFACE_H__
#define __BASIC_PARSER_INTERFACE_H__

template<typename buffer_policy>
class basic_parser_interface{
    public:
        using buffer_type = buffer_policy;
        virtual bool continue_reading() = 0;
        virtual void  at_eof(buffer_type *  buffer) = 0;

        virtual void parse(buffer_type * buffer) = 0;

        virtual buffer_type * buffer() = 0;

};

#endif
