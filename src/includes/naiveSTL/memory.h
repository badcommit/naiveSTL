//
// Created by Zhang Shawn on 7/28/23.
//

#ifndef NAIVESTL_MEMORY_H
#define NAIVESTL_MEMORY_H

#include <concepts>

#include "naiveSTL/utility.h"
#include "naiveSTL/ref.h"

namespace NaiveSTL {

    namespace Concept {
        template<class T, class D>
        concept Deleter = requires(D d, T *ptr){
            d(ptr);
        };
    }

    template<class T>
    class cow_ptr;

    template<class T>
    struct default_delete {

        default_delete() = default;

        template<class U>
        requires std::derived_from<U, T>
        default_delete(const default_delete<U> &other __attribute__((unused))) {}

        virtual void operator()(T *ptr) {
            delete ptr;
        }
    };


    template<class T>
    struct default_delete<T[]> {
        void operator()(T *ptr) {
            delete[] ptr;
        }

        default_delete() = default;
    };

    template<class T, class D=default_delete<T>> requires Concept::Deleter<T, D>
    class unique_ptr {
    public:
        typedef T element_type;
        typedef D deleter_type;
        typedef element_type *pointer;
    private:
        element_type *data_;
        deleter_type deleter_;
    public:
        explicit unique_ptr(pointer data = nullptr) : data_(data) {

        }

        unique_ptr(pointer data, const deleter_type &del) : data_(data), deleter_(del) {}

        explicit unique_ptr(T &&data) : data_(nullptr) {
            data_(std::forward<T>(data));
            data = nullptr;
        }

        explicit unique_ptr(unique_ptr<T> &&data) : data_(std::move(data.data_)), deleter_(std::move(data.deleter_)) {
            data.data_ = nullptr;
        }

        template<class U, class K> requires Concept::Deleter<U, K>
        friend
        class unique_ptr;


        template<class U,class DU=default_delete<U>>
        requires std::derived_from<U, T> &&Concept::Deleter<U, DU> && std::is_assignable_v<D, DU>
        unique_ptr(unique_ptr<U, DU> &&data): data_(std::move(data.release())), deleter_(std::move(data.get_deleter())) {}

        unique_ptr &operator=(unique_ptr<T> &&data) {
            if (this != &data) {
                clean();
                data_ = std::move(data.data_);
                deleter_ = std::move(data.deleter_);
                data.data_ = nullptr;
                data.~unique_ptr();
            }
            return *this;
        }

        unique_ptr(const unique_ptr &) = delete;

        unique_ptr &operator=(const unique_ptr &) = delete;

        const pointer &get() const {
            return data_;
        }

        explicit operator bool() const { return get() != nullptr; }


        void reset(pointer data = nullptr) {
            clean();
            data_ = data;
        }

        auto release() -> pointer {
            T *p = data_;
            data_ = nullptr;
            return p;
        }

        auto get() -> pointer {
            return data_;
        }

        deleter_type &get_deleter() { return deleter_; }

        const deleter_type &get_deleter() const { return deleter_; }

        const element_type &operator*() const { return *data_; }

        const pointer &operator->() const { return data_; }

        element_type &operator*() { return *data_; }

        pointer operator->() { return data_; }


        ~unique_ptr() {
            clean();
        }

    private:
        inline void clean() {
            deleter_(data_);
            data_ = nullptr;
        }

    };

    template<class T1, class D1, class T2, class D2>
    bool operator==(const unique_ptr<T1, D1> &lhs, const unique_ptr<T2, D2> &rhs) {
        return lhs.get() == rhs.get();
    }


    template<class T, class D>
    bool operator==(const unique_ptr<T, D> &up, nullptr_t p) {
        return up.get() == p;
    }

    template<class T, class D>
    bool operator==(nullptr_t p, const unique_ptr<T, D> &up) {
        return up.get() == p;
    }

    template<class T1, class D1, class T2, class D2>
    bool operator!=(const unique_ptr<T1, D1> &lhs, const unique_ptr<T2, D2> &rhs) {
        return !(lhs == rhs);
    }

    template<class T, class D>
    bool operator!=(const unique_ptr<T, D> &up, nullptr_t p) {
        return up.get() != p;
    }

    template<class T, class D>
    bool operator!=(nullptr_t p, const unique_ptr<T, D> &up) {
        return up.get() != p;
    }

    template<class T, class ...Args>
    unique_ptr<T> make_unique(Args &&... args) {
        return unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

    template<class T, class D=default_delete<T>> requires Concept::Deleter<T, D>
    class shared_ptr {
    public:
        typedef T element_type;
    private:
        template<class Type>
        using ref_t = NaiveSTL::ref_cnt_t<Type>;
    public:
        explicit shared_ptr(T *p = nullptr) : ref_(new ref_t<T>(p)) {}

        template<class _T>
        friend
        class cow_ptr;

        shared_ptr(T *p, const D &del) : ref_(new ref_t<T>(p, del)) {}

        shared_ptr(const shared_ptr &sp) {
            copy_ref(sp.ref_);
        }

        shared_ptr &operator=(const shared_ptr &sp) {
            if (this != &sp) {
                decrease_ref();
                copy_ref(sp.ref_);
            }
            return *this;
        }

        shared_ptr(shared_ptr && other)noexcept {
            if(this!=&other){
                this->ref_ = other.ref_;
                other.ref_ = nullptr;
            }
        }

        const element_type &operator*() const {
            return *ref_->get_data();
        }

        element_type &operator*() {
            return *ref_->get_data();
        }

        const element_type *operator->() const { return ref_->get_data(); }

        element_type *operator->() { return ref_->get_data(); }

        element_type *get() const { return ref_->get_data(); }

        size_t use_count() const { return ref_->count(); }

        ~shared_ptr() { decrease_ref(); }

    private:
        ref_t<T> *ref_;

        void decrease_ref() {
            if (ref_->get_data()) {
                --(*ref_);
                if (use_count() == 0) {
                    delete ref_;
                }
            }

        }

        void copy_ref(ref_t<T> *r) {
            ref_ = r;
            ++(*ref_);
        }


    };


    template<class T1, class T2>
    bool operator==(const shared_ptr<T1> &lhs, const shared_ptr<T2> &rhs) {
        return lhs.get() == rhs.get();
    }

    template<class T>
    bool operator==(const shared_ptr<T> &sp, nullptr_t p) {
        return sp.get() == p;
    }

    template<class T>
    bool operator==(nullptr_t p, const shared_ptr<T> &sp) {
        return sp == p;
    }

    template<class T1, class T2>
    bool operator!=(const shared_ptr<T1> &lhs, const shared_ptr<T2> &rhs) {
        return !(lhs == rhs);
    }

    template<class T>
    bool operator!=(const shared_ptr<T> &sp, nullptr_t p) {
        return !(sp == p);
    }

    template<class T>
    bool operator!=(nullptr_t p, const shared_ptr<T> &sp) {
        return !(sp == p);
    }

    template<class T, class...Args>
    auto make_shared(Args... args) -> shared_ptr<T> {
        return shared_ptr<T>(new T(std::forward<Args>(args)...));
    }


}

#endif //NAIVESTL_MEMORY_H
