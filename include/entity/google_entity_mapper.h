#ifndef __GOOGLE_ENTITY_MAPPER_H__
#define __GOOGLE_ENTITY_MAPPER_H__

#include "basic_entity_mapper_interface.h"

/*
 * takes in json from the client and converts it to the user requested object
 */
template<typename T1 , typename T2>
class google_entity_mapper : basic_entity_mapper_interface<T1 , T2>{

    public:
        google_entity_mapper();
        std::shared_ptr<T1> convert(std::shared_ptr<T2> from) override;
        std::shared_ptr<T2> convert(std::shared_ptr<T1> from) override;

        T1 & convert(T2 & from ) override;
        T2 & convert(T1 & from ) override;
    private:
        class impl;
        std::shared_ptr<impl> impl_;
};

#endif
