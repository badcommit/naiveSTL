//
// Created by Shawn Zhang on 7/25/23.
//

#ifndef NAIVESTL_STL_STRING_H
#define NAIVESTL_STL_STRING_H
#include <cstdlib>

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
        iterator start_;
        iterator finish_;
        iterator end_of_storage_;

        typedef allocator<char> data_allocator;
    };
}

#endif //NAIVESTL_STL_STRING_H
