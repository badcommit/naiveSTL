//
// Created by Shawn Zhang on 7/24/23.
//

#ifndef NAIVESTL_UTILITY_H
#define NAIVESTL_UTILITY_H

namespace NaiveSTL {
    template<class T>
    void swap(T &a, T &b) {
        T temp = a;
        a = b;
        b = temp;
    }

    template<class T1, class T2>
    struct pair {

    public:
        T1 first;
        T2 second;
    public:
        pair() = default;

        pair(const pair<T1, T2> &pr);
        pair(const T1 &a, const T2 &b);
        pair<T1, T2>& operator = (const pair<T1, T2>& pr);

    public:
        template <class U, class V>
        friend bool operator == (const pair<U, V> &lhs, const pair<U, V>& rhs);

        template <class U, class V>
        friend bool operator != (const pair<U, V> &lhs, const pair<U, V>& rhs);



    };
    template<class T1, class T2>
    bool operator==(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    }

    template<class T1, class T2>
    bool operator!=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
        return !(lhs == rhs);
    }

    template<class T1, class T2>
    pair<T1, T2>::pair(const pair<T1, T2> &pr) : first(pr.first), second(pr.second) {}

    template<class T1, class T2>
    pair<T1, T2>::pair(const T1 &a, const T2 &b) : first(a), second(b) {}

    template<class T1, class T2>
    pair<T1, T2>& pair<T1, T2>::operator = (const pair<T1, T2>& pr){
        if (this != &pr){
            first = pr.first;
            second = pr.second;
        }
        return *this;
    }


    template<class U, class V>
    pair<U, V> make_pair(const U& u, const V& v){
        return pair<U, V>(u, v);
    }


}

#endif //NAIVESTL_UTILITY_H
