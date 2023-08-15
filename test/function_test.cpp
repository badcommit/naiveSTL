//
// Created by Shawn Zhang on 8/14/23.
//


#include <gtest/gtest.h>
#include "naiveSTL/function.h"

namespace NaiveSTL::function_test {

    int add2(int x, int y)
    {
        return x + y;
    }

    TEST(FUNCTION, CTR){
        function<int(int, int)> func = std::move(add2);
        EXPECT_EQ(func(1, 2), 3);
    }

    TEST(FUNCTION, LAMBDA_CTR){
        function<int(int, int)> func = [](int a, int b){return a + b;};
        EXPECT_EQ(func(1, 2), 3);
    }

}