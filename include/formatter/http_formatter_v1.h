#ifndef __HTTP_FORMATTER_V1_H__
#define __HTTP_FORMATTER_V1_H__

#include "basic_formatter_interface.h"

class http_formatter_v1 : public basic_formatter_interface{
    public:
        using base_type = basic_formatter_interface;
        using self_type = http_formatter_v1;

        self_type & line_pre_processor(std::string &str)override;
        self_type & line_post_processor(std::string &str)override;
        self_type & increment_tab_count() override;
        self_type & decrement_tab_count() override;

        http_formatter_v1();
    private:
        int tab_count_;
};

#endif
