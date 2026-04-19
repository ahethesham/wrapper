#ifndef __JSON_FORMATTER_H__
#define __JSON_FORMATTER_H__
#include "basic_formatter_interface.h"

class json_formatter_v1 : public basic_formatter_interface{

    public:
        json_formatter_v1();
        json_formatter_v1 & line_pre_processor(std::string &str) override;
        json_formatter_v1 & line_post_processor(std::string &str) override;

        json_formatter_v1 & increment_tab_count() override;
        json_formatter_v1 & decrement_tab_count() override;
    private:
        int tab_count_;
};


#endif
