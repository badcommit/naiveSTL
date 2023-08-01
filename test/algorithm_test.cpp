//
// Created by Shawn Zhang on 8/1/23.
//

#include <gtest/gtest.h>
#include "naiveSTL/memory.h"

#include "naiveSTL/algorithm.h"

namespace NaiveSTL::algorithm_test{
    TEST(ALGORITHM, FILL){
        int a[5] = {1, 2, 3, 4, 5};
        NaiveSTL::fill(a, a + 5, 6);
        for (int i = 0; i < 5; ++i) {
            EXPECT_EQ(a[i], 6);
        }
    }

    TEST(ALGORITHM, SORT){
        int a[5] = {5, 4, 3, 2, 1};
        NaiveSTL::sort(a, a + 5);
        for (int i = 0; i < 5; ++i) {
            EXPECT_EQ(a[i], i + 1);
        }
    }
}