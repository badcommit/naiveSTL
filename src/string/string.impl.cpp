//
// Created by Shawn Zhang on 8/1/23.
//

#include <cstring>
#include "naiveSTL/string.h"
#include "naiveSTL/uninitialized_functions.h"
#include "naiveSTL/utility.h"

namespace NaiveSTL {
    string::string(const char *s){
        allocateAndCopy<string::const_iterator>(s, s + strlen(s));
    }

    string::string(const string &rhs){
        allocateAndCopy<string::const_iterator>(rhs.start_, rhs.finish_);
    }

    string::string(string && str) noexcept {
        moveData(std::move(str));
    }

    template<class InputIterator>
    string::string(InputIterator first, InputIterator last){
        allocateAndCopy(first, last);
    }

    string &string::operator=(const string &rhs){
        if (this != &rhs) {
            destroyAndDeallocateAll();
            allocateAndCopy(rhs.start_, rhs.finish_);
        }
        return *this;
    }

    string& string::operator=(NaiveSTL::string &&rhs) noexcept {
        if(this != &rhs){
            moveData(std::move(rhs));
        }
        return *this;
    }

    bool string::operator==(const NaiveSTL::string &rhs) const {
        if(size() != rhs.size()) return false;
        for(size_t i = 0; i < size(); ++i){
            if(start_[i] != rhs.start_[i]) return false;
        }
        return true;
    }

    string &string::operator=(const char *s){
        destroyAndDeallocateAll();
        allocateAndCopy(s, s + strlen(s));
        return *this;
    }

    void string::moveData(string &&str){
        start_ = str.start_;
        finish_ = str.finish_;
        end_of_storage_ = str.end_of_storage_;
        str.start_ = str.finish_ = str.end_of_storage_ = nullptr;
    }

    template<class InputIterator>
    void string::allocateAndCopy(InputIterator first, InputIterator last){
        start_ = data_allocator::allocate(last - first);
        finish_ = uninitialized_copy(first, last, start_);
        end_of_storage_ = finish_;
    }

    string::~string() {
        destroyAndDeallocateAll();
    }

    void string::swap(string &rhs) noexcept{
        if (this != &rhs) {
            NaiveSTL::swap(start_, rhs.start_);
            NaiveSTL::swap(finish_, rhs.finish_);
            NaiveSTL::swap(end_of_storage_, rhs.end_of_storage_);
        }
    }

    void string::destroyAndDeallocateAll() {
        if (start_) {
            destroy(start_, finish_);
            data_allocator::deallocate(start_);
        }
        start_ = finish_ = end_of_storage_ = nullptr;
    }
}