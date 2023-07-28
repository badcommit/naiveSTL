//
// Created by Shawn Zhang on 7/27/23.
//

#include "naiveSTL/vector.h"
#include "naiveSTL/impl/vector.impl.h"
#include <gtest/gtest.h>


namespace NaiveSTL::vector_test {
// Demonstrate some basic assertions.
    TEST(VECTOR, CAP_TEST ) {
        using naiveVec = NaiveSTL::vector<int>;
        auto nv = naiveVec(42, 0);

        EXPECT_EQ(nv.capacity(), 42);
    }

    TEST(VECTOR, RESERVE_TEST) {
        using naiveVec = NaiveSTL::vector<int>;
        auto nv = naiveVec(42, 0);
        nv.reserve(100);

        EXPECT_EQ(nv.capacity(), 100);
    }

    TEST(VECTOR, INIT_TEST_BY_RESERVE) {
        using naiveVec = NaiveSTL::vector<int>;
        const int cap = 42, num = 17;
        auto nv = naiveVec(cap, num);
        for(int i=0; i <cap; i++ ){
            EXPECT_EQ(nv[i], num);
        }
    }

    TEST(VECTOR, INIT_TEST_BY_OTHERS) {
        using naiveVec = NaiveSTL::vector<int>;
        const int cap = 42, num = 17;
        auto nv = naiveVec(cap, num);
        auto nv2(nv);
        for(int i=0; i <cap; i++ ){
            EXPECT_EQ(nv2[i], num);
        }
    }

    TEST(VECTOR, INIT_TEST_BY_ITERATORS) {
        using naiveVec = NaiveSTL::vector<int>;
        const int cap = 42, num = 17;
        auto nv = naiveVec(cap, num);
        naiveVec nv2(nv.begin(), nv.end());
        for(int i=0; i <cap; i++ ){
            EXPECT_EQ(nv2[i], num);
        }
    }

    TEST(VECTOR, TEST_PUSH_BACK) {
        using naiveVec = NaiveSTL::vector<int>;
        const int cap = 42;
        naiveVec nv{};
        for(int i=0; i <cap; i++ ){
            nv.push_back(i);
        }
        for(int i=0; i <cap; i++ ){
            EXPECT_EQ(nv[i], i);
        }
    }

    TEST(VECTOR, TEST_POP_BACK) {
        using naiveVec = NaiveSTL::vector<int>;
        const int cap = 42;
        naiveVec nv{};
        for(int i=0; i <cap; i++ ){
            nv.push_back(i);
        }
        for(int i=0; i <cap; i++ ){
            EXPECT_EQ(nv[i], i);
        }
        for(int i=0; i < cap; i++){
            EXPECT_EQ(nv.back(), cap - i -1);
            EXPECT_EQ(nv.size(), cap - i);
            nv.pop_back();
        }

    }

    TEST(VECTOR, TEST_INSERT) {
        using naiveVec = NaiveSTL::vector<int>;
        const int cap = 42;
        naiveVec nv{};
        for(int i=0; i <cap; i++ ){
            nv.push_back(i);
        }
        int pivot = 10;
        auto ptr = nv.insert(nv.begin() + pivot, 100);

        EXPECT_EQ(*ptr, 100);

        for(int i=0; i <cap; i++ ){
            if(i < pivot){
                EXPECT_EQ(nv[i], i);
            }

            else if(i > pivot){
                EXPECT_EQ(nv[i], i - 1);
            }
        }
    }
}
