//
// Created by Shawn Zhang on 7/30/23.
//

#ifndef NAIVESTL_COW_PTR_H
#define NAIVESTL_COW_PTR_H

#include "naiveSTL/memory.h"


namespace NaiveSTL {
    template<class T>
    class cow_ptr;

    namespace Impl{
        template<class T>
        class proxy{
        public:
            explicit proxy(cow_ptr<T> *const cp): cp_(cp) {}
            proxy(const proxy& rhs) = default;
            proxy& operator = (const proxy& rhs) = default;

            const T& operator * () const;

            T& operator * ();

            const T *operator -> () const;

            T *operator -> ();

            cow_ptr<T> &operator=(const T &rhs);

            operator T() const;

        private:
            cow_ptr<T> *const cp_;
        };


    }

    template<class T>
    class cow_ptr {
    public:
        typedef T element_type;
    private:
        template<class _T>
                using proxy = Impl::proxy<_T>;

    public:
        explicit cow_ptr(T *p = nullptr);

        template<class D>
                cow_ptr(T *p, D d);

        cow_ptr(const cow_ptr &cp);

        cow_ptr &operator=(const cow_ptr &cp);

        const proxy<element_type>& operator*() const;

        proxy<element_type>& operator*();

        const proxy<element_type> operator->() const;

        proxy<element_type> operator->();

        element_type *get();

        const element_type *get() const;

        operator bool() const;

    private:
        shared_ptr<element_type> ptr_;

    };
}

#endif //NAIVESTL_COW_PTR_H
