//
// Created by Shawn Zhang on 8/1/23.
//


#include <gtest/gtest.h>
#include "naiveSTL/memory.h"
#include "naiveSTL/string.h"
namespace NaiveSTL::unique_ptr_test {



    TEST(STRING, CTR){
        string ss("hello2");
        string ss2 = "hello2";
        std::cout << ss2.size() << std::endl;

        EXPECT_EQ(ss, ss2);
        EXPECT_NE(ss, "hello");
    }

}