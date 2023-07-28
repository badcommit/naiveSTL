#ifndef STL_ALLOC_H

#define STL_ALLOC_H


#include <cstdlib>

namespace NaiveSTL{

    class alloc{
        private:

    public:
        static void *allocate(size_t bytes);

        static void deallocate(void *ptr);

        static void *reallocate(void *ptr, size_t new_sz);

    };
}

#endif