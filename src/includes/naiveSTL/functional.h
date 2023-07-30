//
// Created by Shawn Zhang on 7/30/23.
//

#ifndef NAIVESTL_FUNCTIONAL_H
#define NAIVESTL_FUNCTIONAL_H

namespace NaiveSTL{

    template<class T>
    requires requires(T x, T y) { x < y; }
    struct less {
        typedef T first_argument_type;
        typedef T second_argument_type;
        typedef bool result_type;

        result_type operator()(const first_argument_type &x, const second_argument_type &y) {
            return x < y;
        }
    };

    template<class T>
    requires requires(T x, T y) { x == y; }
    struct equal_to {
        typedef T first_argument_type;
        typedef T second_argument_type;
        typedef bool result_type;

        result_type operator()(const first_argument_type &x, const second_argument_type &y) {
            return x == y;
        }
    };
}

#endif //NAIVESTL_FUNCTIONAL_H
