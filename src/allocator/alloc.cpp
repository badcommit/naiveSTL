//
// Created by Shawn Zhang on 7/25/23.
//

#include "naiveSTL/alloc.h"

namespace NaiveSTL{
    //do not want to bother too much since already implement one in cmu csapp

    void *alloc::allocate(size_t bytes) {
        return malloc(bytes);
    }

    void alloc::deallocate(void *ptr) {
         ::free(ptr);
    }

    void *alloc::reallocate(void *ptr, size_t new_sz){
        return realloc(ptr, new_sz);
    }

}