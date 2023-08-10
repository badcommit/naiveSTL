//
// Created by Shawn Zhang on 7/31/23.
//

#ifndef NAIVESTL_ALGORITHM_H
#define NAIVESTL_ALGORITHM_H

#include <cstring>
#include "iterator.h"
#include "utility.h"
#include "functional.h"

namespace NaiveSTL {
    template<class ForwardIterator, class T>
    void fill(ForwardIterator first, ForwardIterator last, const T &value) {
        for (; first != last; ++first) {
            *first = value;
        }
    }

    inline void fill(char *first, char *last, const char &value) {
        memset(first, static_cast<unsigned char>(value), last - first);
    }


    template<class T>
    const T &min(const T &a, const T &b) {
        return b < a ? b : a;
    }

    template<class T, class Comp>
    const T &min(const T &a, const T &b, Comp comp) {
        return comp(a, b) ? a : b;
    }

    template<class T>
    const T &max(const T &a, const T &b) {
        return max(a, b);
    }

    template<class T, class Comp>
    const T &max(const T &a, const T &b, Comp comp) {
        return comp(a, b) ? b : a;
    }

    template<class InputIterator, class UnaryPredicate>
    bool all_of(InputIterator first, InputIterator last, UnaryPredicate) {
        for (; first != last; ++first) {
            if (!unary_predicate(*first)) {
                return false;
            }
        }
        return true;
    }

    template<class InputIterator, class UnaryPredicate>
    bool any_of(InputIterator first, InputIterator last, UnaryPredicate pred) {
        for (; first != last; ++first) {
            if (pred(*first)) {
                return true;
            }
        }
        return false;
    }

    template<class InputIterator, class UnaryPredicate>
    bool none_of(InputIterator first, InputIterator last, UnaryPredicate pred) {
        for (; first != last; ++first) {
            if (pred(*first)) {
                return false;
            }
        }
        return true;
    }


    template<class InputIterator, class T>
    InputIterator find(InputIterator first, InputIterator last, const T &val) {
        for (; first != last; ++first) {
            if (*first == val) {
                return first;
            }
        }
        return last;
    }

    template<class InputIterator, class UnaryPredicate>
    InputIterator find_if(InputIterator first, InputIterator last, UnaryPredicate pred) {
        for (; first != last; ++first) {
            if (pred(*first)) {
                return first;
            }
        }
        return last;
    }

    template<class InputIterator, class T>
    typename iterator_traits<InputIterator>::difference_type
    count(InputIterator first, InputIterator last, const T &val) {
        typename iterator_traits<InputIterator>::difference_type n = 0;
        for (; first != last; ++first) {
            if (*first == val) {
                ++n;
            }
        }
        return n;
    }

    template<class InputIterator0, class InputIterator1>
    bool equal(InputIterator0 first0, InputIterator0 last0, InputIterator1 first1) {
        for (; first0 != last0; ++first0, ++first1) {
            if (*first0 != *first1) {
                return false;
            }
        }
        return true;
    }

    template<class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
    ForwardIterator1
    search(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2,
           BinaryPredicate pred) {
        auto d1 = distance(first1, last1);
        auto d2 = distance(first2, last2);
        if (d1 < d2) {
            return last1;
        }
        auto current1 = first1;
        auto current2 = first2;
        while (current2 != last2) {
            if (!pred(*current1, *current2)) {
                if (d1 == d2) {
                    return last1;
                } else {
                    current1 = ++first1;
                    current2 = first2;
                    --d1;
                }
            } else {
                ++current1;
                ++current2;
            }
        }
        return first1;
    }


    template<class RandomIterator>
    void sort(RandomIterator first, RandomIterator last) {
        return sort(first, last, less<typename iterator_traits<RandomIterator>::value_type>());
    }


    template<class RandomIterator, class BinaryPredicate>
    void bubble_sort(RandomIterator first, RandomIterator last,  BinaryPredicate pred) {
        for (auto i = first; i != last; ++i) {
            auto j = i + 1;
            for (; j != last; ++j) {
                if (pred(*(j), *(i))){
                    NaiveSTL::swap(i, j);
                }
            }
        }
    }

    template<class RandomIterator, class BinaryPredicate>
    void sort(RandomIterator first, RandomIterator last, BinaryPredicate pred) {
        bubble_sort(first, last, pred);
    }

    template<class RandomIterator>
    typename iterator_traits<RandomIterator>::difference_type
    _distance(RandomIterator first, RandomIterator last, random_access_iterator_tag){
        auto dist = last - first;
        return dist;
    }

    template<class Iterator>
    typename iterator_traits<Iterator>::difference_type
    distance(Iterator first, Iterator last) {
        typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
        return _distance(first, last, iterator_category());
    }

    template<class InputIterator, class Function>
    void generate(InputIterator first, InputIterator last, Function func) {
        for (; first != last; ++first) {
            *first = func();
        }


    }




}

#endif //NAIVESTL_ALGORITHM_H
