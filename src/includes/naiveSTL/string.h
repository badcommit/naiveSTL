//
// Created by Shawn Zhang on 7/25/23.
//

#ifndef NAIVESTL_STL_STRING_H
#define NAIVESTL_STL_STRING_H
#include <cstdlib>
#include "naiveSTL/allocator.h"

namespace NaiveSTL{
    class string {
    public:
        typedef char value_type;
        typedef char* iterator;
        typedef const char* const_iterator;

        typedef char& reference;
        typedef const char& const_reference;

        typedef size_t size_type;

        static const size_t npos = -1;

    private:
        iterator start_{};
        iterator finish_{};
        iterator end_of_storage_{};

        typedef NaiveSTL::allocator<char> data_allocator;

    public:
        string(): start_(nullptr), finish_(nullptr), end_of_storage_(nullptr) {}
        string(const char* s);
        string(const string &str);
        string(string &&str) noexcept;

        template<class InputIterator>
                string(InputIterator first, InputIterator last);


        string &operator=(const string &rhs);
        string &operator=(string &&rhs) noexcept;
        string &operator=(const char *s);

        bool operator==(const string &rhs) const;

        [[nodiscard]] size_type size() const { return finish_ - start_; }

        [[nodiscard]] size_t length() const { return size(); }

        [[nodiscard]] size_t capacity() const { return end_of_storage_ - start_; }

        [[nodiscard]] bool empty() const { return start_ == finish_; }

        const char& operator[](size_t pos) const { return *(start_ + pos); }

        ~string();

    private:
        void moveData(string &&str);
        void swap(string &str) noexcept;
        template<class InputIterator>
        void allocateAndCopy(InputIterator first, InputIterator last);
        void destroyAndDeallocateAll();
    };
}

#endif //NAIVESTL_STL_STRING_H
