#ifndef __BASIC_ENTITY_MAPPER_INTERFACE_H__
#define __BASIC_ENTITY_MAPPER_INTERFACE_H__

#include <memory>
template<typename T1 ,
        typename T2>
class basic_entity_mapper_interface{
    public:
        T1 & convert(T2 &);
        T2 & convert(T1 &);
        std::shared_ptr<T1> convert(std::shared_ptr<T2> );
        std::shared_ptr<T2> convert(std::shared_ptr<T1> );
};


#endif
