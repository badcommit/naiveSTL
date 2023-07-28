//
// Created by Shawn Zhang on 7/25/23.
//

#ifndef NAIVESTL_ITERATOR_H
#define NAIVESTL_ITERATOR_H

#include <cstddef>

namespace NaiveSTL {

    struct input_iterator_tag {
    };
    struct output_iterator_tag {
    };
    struct forward_iterator_tag : public input_iterator_tag {
    };
    struct bidirectional_iterator_tag : public forward_iterator_tag {
    };
    struct random_access_iterator_tag : public bidirectional_iterator_tag {
    };


    template<class T, class Distance>
    struct input_iterator {
        typedef input_iterator_tag iterator_category;
        typedef T value_type;
        typedef Distance difference_type;
        typedef T *pointer;
        typedef T &reference;
    };

    struct output_iterator {
        typedef output_iterator_tag iterator_category;
        typedef void value_type;
        typedef void difference_type;
    };

    template<class T, class Distance>
    struct forward_iterator {
        typedef forward_iterator_tag iterator_category;
        typedef T value_type;
        typedef Distance difference_type;
    };

    template<class T, class Distance>
    struct bidirectional_iterator {
        typedef bidirectional_iterator_tag iterator_category;
        typedef T value_type;
        typedef Distance difference_type;
    };

    template<class T, class Distance>
    struct random_access_iterator {
        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef Distance difference_type;

    };

    template<class Category, class T, class Distance = ptrdiff_t,
            class Pointer = T *, class Reference = T &>
    struct iterator {
        typedef Category iterator_category;
        typedef T value_type;
        typedef Distance difference_type;

    };

    template<class Iterator>
    struct iterator_traits {
        typedef typename Iterator::iterator_category iterator_category;
        typedef typename Iterator::value_type value_type;
        typedef typename Iterator::difference_type difference_type;

    };

    template<class T>
    struct iterator_traits<T *> {
        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
    };

    template<class T>
    struct iterator_traits<const T *> {
        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
    };

    template<class Iterator>
    inline typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator &) {
        typedef typename iterator_traits<Iterator>::iterator_category category;
        return category();
    }

    template<class Iterator>
    inline typename iterator_traits<Iterator>::value_type* value_type(const Iterator&){
        return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
    }

    template<class Iterator>
    inline typename iterator_traits<Iterator>::difference_type*
    difference_type(__attribute__((unused)) const Iterator& It){
        return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
    }

    namespace Concept {
        template<class InputIterator>
        concept Iteratorable = requires(InputIterator it) {
            it++;
            *it;
        };

        template<class Size>
        concept Sizeable = requires(Size size) {
            size--;
            --size;
        };
    }

}



#endif //NAIVESTL_ITERATOR_H
