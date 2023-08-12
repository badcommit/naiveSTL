//
// Created by Shawn Zhang on 7/25/23.
//

#ifndef NAIVESTL_VECTOR_H
#define NAIVESTL_VECTOR_H
#include <concepts>
#include "naiveSTL/allocator.h"
#include "naiveSTL/iterator.h"
#include "naiveSTL/uninitialized_functions.h"
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
        auto cbegin() const noexcept -> const_iterator { return start_; }

        iterator end() const noexcept { return finish_; }
        auto cend() const noexcept -> const_iterator { return finish_; }

        reference front() { return *start_; }
        const_reference front() const { return *start_; }

        reference back() { return *(finish_ - 1); }
        const_reference back() const { return *(finish_ - 1); }

        void push_back(const value_type& value);
        void push_back(value_type&& value);
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

        iterator insert_aux(iterator position, value_type &&value);

    };

    template<class T, class Alloc>
    requires Concept::Allocable<Alloc, T>
    void vector<T, Alloc>::destroyAndDeallocateAll() {
        if (capacity() > 0) {
            destroy(start_, finish_);
            data_allocator::deallocate(start_, capacity());
        }
    }

    template<class T, class Alloc>
    requires Concept::Allocable<Alloc, T>
    void vector<T, Alloc>::allocateAndFillN(const size_type n, const value_type &v) {
        start_ = data_allocator::allocate(n);
        finish_ = uninitialized_fill_n(start_, n, v);
        end_of_storage_ = start_ + n;
    }

    template<class T, class Alloc>
    requires Concept::Allocable<Alloc, T>
    template<class InputIterator>
    void vector<T, Alloc>::allocateAndCopy(InputIterator first, InputIterator last) {
        start_ = data_allocator::allocate(last - first);
        finish_ = uninitialized_copy(first, last, start_);
        end_of_storage_ = start_ + (last - first);
    }

    template<class T, class Alloc>
    requires Concept::Allocable<Alloc, T>
    vector<T, Alloc>::vector(size_type n) {
        allocateAndFillN(n, value_type());
    }

    template<class T, class Alloc>
    requires Concept::Allocable<Alloc, T>
    vector<T, Alloc>::vector(const size_type n, const value_type &value) {
        allocateAndFillN(n, value);
    }

    template<class T, class Alloc>
    requires Concept::Allocable<Alloc, T>
    template<class InputIterator>
    requires Concept::Iteratorable<InputIterator>
    vector<T, Alloc>::vector(InputIterator first, InputIterator last) {
        allocateAndCopy(first, last);
    }

    template<class T, class Alloc>
    requires Concept::Allocable<Alloc, T>
    vector<T, Alloc>::vector(const vector &v) {
        allocateAndCopy(v.start_, v.finish_);
    }

    template<class T, class Alloc>
    requires Concept::Allocable<Alloc, T>
    vector<T, Alloc>::vector(vector<T, Alloc> &&v) noexcept {
        if (this != &v) {
            start_ = v.start_;
            end_of_storage_ = v.end_of_storage_;
            finish_ = v.finish_;

            v.start_ = v.finish_ = v.end_of_storage_ = nullptr;
        }
    }

    template<class T, class Alloc>
    requires Concept::Allocable<Alloc, T>
    vector<T, Alloc> &vector<T, Alloc>::operator=(const vector &v) {
        if (this != &v) {
            allocateAndCopy(v.start_, v.finish_);
        }
        return *this;
    }

    template<class T, class Alloc>
    requires Concept::Allocable<Alloc, T>
    vector<T, Alloc> &vector<T, Alloc>::operator=(vector &&v) noexcept {
        if (this != &v) {
            start_ = v.start_;
            end_of_storage_ = v.end_of_storage_;
            finish_ = v.finish_;

            v.start_ = v.finish_ = v.end_of_storage_ = nullptr;
        }
        return *this;
    }

    template<class T, class Alloc>
    requires Concept::Allocable<Alloc, T>
    void vector<T, Alloc>::reserve(size_type n) {
        if (capacity() < (difference_type) n) {
            const auto old_size = size();
            auto temp_start = data_allocator::allocate(n);
            uninitialized_copy(start_, finish_, temp_start);

            destroyAndDeallocateAll();

            start_ = temp_start;
            finish_ = temp_start + old_size;
            end_of_storage_ = start_ + n;
        }
    }



    template<class T, class Alloc>
    requires Concept::Allocable<Alloc, T>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::insert_aux(iterator position, const value_type &value){
        auto diff = position - begin();
        if(capacity() == size()){
            const auto old_size = size();
            const auto new_size = old_size != 0 ? 2 * old_size : 1;
            auto temp_start = data_allocator::allocate(new_size);
            auto temp_finish = temp_start;

            uninitialized_copy(start_, position, temp_start);
            uninitialized_fill_n(temp_start + diff, 1, value);
            temp_finish = uninitialized_copy(position, finish_, temp_start + diff + 1);
            destroyAndDeallocateAll();
            start_ = temp_start;
            finish_ = temp_finish;
            end_of_storage_ = temp_start + new_size;
        } else {
            finish_ = uninitialized_copy_backwards(position, finish_, position + 1);
            uninitialized_fill_n(position, 1, value);
        }
        return begin() + diff;

    }

    template<class T, class Alloc>
    requires Concept::Allocable<Alloc, T>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::insert_aux(iterator position, value_type &&value){
        auto diff = position - begin();
        if(capacity() == size()){
            const auto old_size = size();
            const auto new_size = old_size != 0 ? 2 * old_size : 1;
            auto temp_start = data_allocator::allocate(new_size);
            auto temp_finish = temp_start;

            uninitialized_move(start_, position, temp_start);
            uninitialized_fill_n(temp_start + diff, 1, std::move(value));
            temp_finish = uninitialized_move(position, finish_, temp_start + diff + 1);
            data_allocator::deallocate(start_, capacity());
            start_ = temp_start;
            finish_ = temp_finish;
            end_of_storage_ = temp_start + new_size;
        } else {
            finish_ = uninitialized_move_backwards(position, finish_, position + 1);
            uninitialized_fill_n(position, 1,  std::forward<value_type>(value));
        }
        return begin() + diff;

    }



    template<class T, class Alloc>
    requires Concept::Allocable<Alloc, T>
    bool vector<T, Alloc>::operator==(const vector<T, Alloc> &v) const {
        if (size() != v.size()) return false;
        auto i1 = start_;
        auto i2 = v.start_;
        for (; i1 != finish_ && i2 != v.finish_; ++i1, ++i2) {
            if (*i1 != *i2) return false;
        }
        return true;
    }

    template<class T, class Alloc>
    requires Concept::Allocable<Alloc, T>
    bool vector<T, Alloc>::operator!=(const vector<T, Alloc> &v) const {
        auto d = *this == v;
        return !d;
    }


    template<class T, class Alloc>
    requires Concept::Allocable<Alloc, T>
    void vector<T, Alloc>::push_back(const value_type &v) {
        insert_aux(end(), v);
    }

    template<class T, class Alloc>
    requires Concept::Allocable<Alloc, T>
    void vector<T, Alloc>::push_back(value_type &&v) {
        insert_aux(end(), std::move(v));
    }

    template<class T, class Alloc>
    requires Concept::Allocable<Alloc, T>
    void vector<T, Alloc>::pop_back() {
        finish_--;
        destroy(finish_, finish_+1);
    }

    template<class T, class Alloc>
    requires Concept::Allocable<Alloc, T>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator position) {
        destroy(position, position+1);
        finish_ = uninitialized_copy_backwards(position + 1, finish_, position);
        return position;
    }

    template<class T, class Alloc>
    requires Concept::Allocable<Alloc, T>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, const value_type &val) {
        return insert_aux(position, val);
    }

    template<class T, class Alloc>
    requires Concept::Allocable<Alloc, T>
    void vector<T, Alloc>::clear() noexcept {
        destroy(start_, finish_);
        finish_ = start_;
    }

    template<class T, class Alloc>
    requires Concept::Allocable<Alloc, T>
    vector<T, Alloc>::~vector() {
        destroyAndDeallocateAll();
    }
}


#endif //NAIVESTL_VECTOR_H
