//
// Created by Shawn Zhang on 7/30/23.
//

#ifndef NAIVESTL_COW_PTR_H
#define NAIVESTL_COW_PTR_H

#include "naiveSTL/memory.h"


namespace NaiveSTL {
    template<class T>
    class cow_ptr;

    namespace Impl {
        template<class T>
        class proxy {
        public:
            explicit proxy(cow_ptr<T> *const cp) : cp_(cp) {}

            proxy(const proxy &rhs) = default;

            proxy &operator=(const proxy &rhs) = default;

            const T &operator*() const;

            T &operator*();

            const T *operator->() const;

            T *operator->();

            explicit operator T() const;

        private:
            cow_ptr<T> *const cp_;
        };


    }

    template<class T>
    class cow_ptr {
    public:
        typedef T element_type;
    private:
        template<class C>
        using proxy = Impl::proxy<C>;

    public:
        explicit cow_ptr(T *p = nullptr);

        template<class D>
                requires Concept::Deleter<D, T>
        cow_ptr(T *p, D d);

        cow_ptr(const cow_ptr &cp);

        cow_ptr &operator=(const cow_ptr &cp);

        const proxy<element_type>& operator*() const;

        proxy<element_type> operator*();

        proxy<element_type> operator->() const;

        proxy<element_type> operator->();

        element_type *get();

        const element_type *get() const;


    private:
        shared_ptr<element_type> ptr_;

    public:


    };

    template<class T1, class ... Args>
    cow_ptr<T1> make_cow(Args &&... args) {
        return cow_ptr<T1>(new T1(std::forward<Args>(args)...));
    }

    namespace Impl {
        template<class T>
        const T &proxy<T>::operator*() const {
            return *(cp_->ptr_);
        }

        template<class T>
        T &proxy<T>::operator*() {
            auto t = *(cp_->ptr_);
            cp_->ptr_ = make_shared<T>(t);
            return *(cp_->ptr_);
        }

        template<class T>
        const T *proxy<T>::operator->() const {
            return cp_->ptr_.get();
        }

        template<class T>
        T *proxy<T>::operator->() {
            auto t = *(cp_->ptr_);
            cp_->ptr_ = make_shared<T>(t);
            return cp_->ptr_.get();
        }


        template<class T>
        proxy<T>::operator T() const {
            return *(cp_->ptr_);
        }


    }
    template<class T>
    cow_ptr<T>::cow_ptr(T *p) : ptr_(p) {}

    template<class T>
    template<class D>
    requires Concept::Deleter<D, T>
    cow_ptr<T>::cow_ptr(T *p, D d) : ptr_(p, d) {}

    template<class T>
    cow_ptr<T>::cow_ptr(const cow_ptr &cp) : ptr_(cp.ptr_) {}

    template<class T>
    cow_ptr<T> &cow_ptr<T>::operator=(const cow_ptr &cp) {
        if(this != &cp){
            ptr_ = cp.ptr_;
        }

        return *this;
    }

    template<class T>
    typename cow_ptr<T>::element_type *cow_ptr<T>::get() {
        return ptr_.get();
    }

    template<class T>
    const typename cow_ptr<T>::element_type* cow_ptr<T>::get() const{
        return ptr_.get();
    }


    template<class T>
    const typename cow_ptr<T>::template proxy<T>& cow_ptr<T>::operator*() const {
        return proxy<T>(const_cast<cow_ptr<T> *>(this));
    }

    template<class T>
    typename cow_ptr<T>:: template proxy<T> cow_ptr<T>::operator ->(){
        return proxy<T>(this);
    }

    template<class T>
    typename cow_ptr<T>:: template proxy<T> cow_ptr<T>::operator *(){
        return proxy<T>(this);
    }

    template<class T>
     typename cow_ptr<T>:: template proxy<T> cow_ptr<T>::operator ->()const{
        return proxy<T>(const_cast<cow_ptr *const>(this));
    }

}


#endif //NAIVESTL_COW_PTR_H
