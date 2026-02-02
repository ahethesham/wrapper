#ifndef __ITERATOR_H__
#define __ITERATOR_H__

template<typename T>
class Iterator{
    T * root_;
    T * current_;
    bool hasNext_;
    public:
        virtual T & getNext() ; 
        virtual bool hasNext() ;
};

#endif
