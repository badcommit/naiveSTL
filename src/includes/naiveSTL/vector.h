//
// Created by Shawn Zhang on 7/25/23.
//

#ifndef NAIVESTL_VECTOR_H
#define NAIVESTL_VECTOR_H

#include "naiveSTL/allocator.h"
#include "naiveSTL/iterator.h"
#include "type_traits.h"

namespace NaiveSTL {
    template<class T, class Alloc=allocator<T>>
            requires Concept::Allocable<Alloc, T>
    class vector {
    private:
        T *start_;
        T *finish_;
        T *end_of_storage_;


        typedef Alloc data_allocator;
    public:
        typedef T value_type;

        typedef T* iterator;
        typedef const T* const_iterator;

        typedef iterator  pointer;
        typedef T& reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef const T& const_reference;



        vector(): start_(0), finish_(0), end_of_storage_(0) {}
        explicit vector(size_type n);

        vector(size_type n, const value_type& value);


        template<class InputIterator>
        requires Concept::Iteratorable<InputIterator>
        vector(InputIterator first, InputIterator last);

        vector(const vector& v);

        vector(vector && v) noexcept;


        [[nodiscard]] difference_type capacity() const { return end_of_storage_ - start_; }

        vector& operator = (const vector& v);

        vector& operator = (vector &&v) noexcept;


        auto empty() const noexcept { return start_ == finish_; }

        auto size() const noexcept { return finish_ - start_; }

        auto reserve(size_type n) -> void;

        ~vector();

        bool operator == (const vector& v) const;

        bool operator != (const vector& v) const;

        reference operator[](size_type n) { return *(start_ + n); }
        const_reference operator[](size_type n) const { return *(start_ + n); }

        iterator begin() const noexcept { return start_; }
        const_iterator cbegin() const noexcept { return start_; }

        iterator end() const noexcept { return finish_; }
        const_iterator cend() const noexcept { return finish_; }

        reference front() { return *start_; }
        const_reference front() const { return *start_; }

        reference back() { return *(finish_ - 1); }
        const_reference back() const { return *(finish_ - 1); }

        void push_back(const value_type& value);
        void pop_back();

        iterator insert(iterator position, const value_type& val);
        iterator erase(iterator position);

        void clear() noexcept;


    private:
        template<class InputIterator>
        void allocateAndCopy(InputIterator first, InputIterator last);

        void destroyAndDeallocateAll();

        void allocateAndFillN(size_t n, const value_type &v);

        iterator insert_aux(iterator position, const value_type &value);
    };
}


#endif //NAIVESTL_VECTOR_H
