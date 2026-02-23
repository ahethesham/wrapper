#ifndef __ITERATOR_H__
#define __ITERATOR_H__

template<typename T>
class Iterator{
    typedef T data_type;
    typedef typename T::value_type value_type;
    value_type * root_;
    value_type * current_;
    public:
        virtual bool hasNext(){
            return current_->hasNext();
        }
        virtual value_type * get(){
            return current_;
        }
};

#endif
