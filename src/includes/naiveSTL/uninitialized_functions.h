//
// Created by Shawn Zhang on 7/27/23.
//

#ifndef NAIVESTL_UNINITIALIZED_FUNCTIONS_H
#define NAIVESTL_UNINITIALIZED_FUNCTIONS_H

#include "naiveSTL/construct.h"

namespace NaiveSTL {

    template<class ForwardIterator, class T>
    requires Concept::Iteratorable<ForwardIterator>
    ForwardIterator _uninitialized_fill_n(ForwardIterator first, ForwardIterator last, const T &x, _false_type) {
        for (; first != last; ++first) {
            construct(&*first, x);
        }
        return first;
    }

    template<class ForwardIterator, class T>
    requires Concept::Iteratorable<ForwardIterator>
    ForwardIterator _uninitialized_fill_n(ForwardIterator first, ForwardIterator last, const T &x, _true_type) {
        for (; first != last; ++first) {
            *first = x;
        }
        return first;
    }

    template<class ForwardIterator, class T, class Size>
    requires Concept::Iteratorable<ForwardIterator> && Concept::Sizeable<Size>
    ForwardIterator _uninitialized_fill_n(ForwardIterator first, Size t, const T &x, _true_type) {
        for (; t >0; t--, ++first) {
            *first = x;
        }
        return first;
    }

    template<class ForwardIterator, class T, class Size>
    requires Concept::Iteratorable<ForwardIterator> && Concept::Sizeable<Size>
    ForwardIterator _uninitialized_fill_n(ForwardIterator first, Size t, const T &x, _false_type) {
        for (; t >0; t--, ++first) {
            construct(&*first, x);
        }
        return first;
    }

    template<class ForwardIterator, class Size, class T>
    requires Concept::Iteratorable<ForwardIterator> && Concept::Sizeable<Size>
    ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x) {
        typedef typename _type_traits<ForwardIterator>::is_POD_type is_POD_type;
        return _uninitialized_fill_n(first, n, x, is_POD_type());
    }

    /**/

    template<class InputIterator, class ForwardIterator>
    requires Concept::Iteratorable<ForwardIterator>
    ForwardIterator _uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, _true_type) {
        memcpy(result, first, (last - first) * sizeof(*first));
        return result + (last - first);
    }

    template<class InputIterator, class ForwardIterator>
    requires Concept::Iteratorable<ForwardIterator>
    ForwardIterator _uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, _false_type) {
        int i = 0;
        for (; first != last; ++first, ++i){
            construct((result + i), *first);
        }
        return result + (last - first);
    }

    template<class ForwardIterator, class Size, class T>
    ForwardIterator uninitialized_copy(ForwardIterator first, Size n, const T& x) {
        typedef typename _type_traits<ForwardIterator>::is_POD_type is_POD_type;
        return _uninitialized_copy(first, n, x, is_POD_type());
    }

    template<class InputIterator, class ForwardIterator>
    requires Concept::Iteratorable<ForwardIterator>
    ForwardIterator _uninitialized_copy_backwards(InputIterator first, InputIterator last, ForwardIterator result, _false_type) {
        int i = 0;
        auto end  = last - 1;
        auto diff = last - first;
        if(diff == 0){
            return result;
        }
        for (; end+1 != first; --end, ++i){
            construct((result+ diff - i - 1), *end);
        }
        return result + (last - first);
    }

    template<class InputIterator, class ForwardIterator>
    requires Concept::Iteratorable<ForwardIterator>
    ForwardIterator _uninitialized_copy_backwards(InputIterator first, InputIterator last, ForwardIterator result, _true_type) {
        int i = 0;
        auto end  = last - 1;
        auto diff = last - first;
        if(diff == 0){
            return result;
        }
        for (; end+1 != first; --end, ++i){
            *(result + diff - i - 1) = *end;
        }
        return result + (last - first);
    }

    template<class ForwardIterator, class InputIterator>
    requires Concept::Iteratorable<ForwardIterator>
    ForwardIterator uninitialized_copy_backwards(InputIterator first, InputIterator last, ForwardIterator result) {
        typedef typename _type_traits<ForwardIterator>::is_POD_type is_POD_type;
        return _uninitialized_copy_backwards(first, last, result, is_POD_type());
    }
}

#endif //NAIVESTL_UNINITIALIZED_FUNCTIONS_H
