//
// Created by Shawn Zhang on 7/27/23.
//

#ifndef NAIVESTL_CONSTRUCT_H
#define NAIVESTL_CONSTRUCT_H

#include <new>

#include "naiveSTL/type_traits.h"
#include "naiveSTL/iterator.h"

namespace NaiveSTL {
    template<class T1, class ...T2>
    inline void construct(T1 *ptr1, T2&& ...value){
        new(ptr1) T1(std::forward<T2>(value)...);
    }


    template<class T1, class T2>
    inline void construct(T1 *ptr1, T2&& value){
        new(ptr1) T1(std::forward<T2>(value));
    }

    template<class T>
    inline void _destroy(T *ptr){
        ptr->~T();
    }

    //is not POD type
    template<class ForwardIterator>
    requires Concept::Iteratorable<ForwardIterator>
    inline void _destroy(ForwardIterator first, ForwardIterator last, _false_type /*unused*/){
        for(; first != last; ++first){
            _destroy(&*first);
        }
    }

    template<class ForwardIterator>
    requires Concept::Iteratorable<ForwardIterator>
    inline void _destroy(__attribute__((unused)) ForwardIterator first, __attribute__((unused)) ForwardIterator last, _true_type /*unused*/){

    }

    template<class ForwardIterator>
    requires Concept::Iteratorable<ForwardIterator>
    inline void destroy(ForwardIterator first, ForwardIterator last){
        typedef typename _type_traits<ForwardIterator>::is_POD_type is_POD_type;
        _destroy(first, last, is_POD_type());
    }


}

#endif //NAIVESTL_CONSTRUCT_H
