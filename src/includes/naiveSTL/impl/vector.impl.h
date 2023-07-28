//
// Created by Shawn Zhang on 7/27/23.
//

#ifndef NAIVESTL_VECTOR_IMPL_H
#define NAIVESTL_VECTOR_IMPL_H

#include "naiveSTL/vector.h"
#include "naiveSTL/uninitialized_functions.h"


namespace NaiveSTL {


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

#endif //NAIVESTL_VECTOR_IMPL_H
