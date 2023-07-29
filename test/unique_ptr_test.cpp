//
// Created by Zhang Shawn on 7/28/23.
//

#include <gtest/gtest.h>
#include "naiveSTL/memory.h"
namespace NaiveSTL::unique_ptr_test {



    TEST(UNIQUE_PTR, PTR_MAKE){

        auto a = make_unique<int>(5);

        EXPECT_EQ(*a, 5);

        struct b {
        public:
            int k_;
            int c_;
        public:
            b(int k, int c){
                k_ = k;
                c_ =c;
            }
        };

        auto c = make_unique<b>(2, 3);
        EXPECT_EQ(c.get()->k_, 2);
        EXPECT_EQ(c.get()->c_, 3);

    }



    TEST(UNIQUE_PTR, PTR_FREE){
        int counter = 0;
        auto called = [&counter](){
            counter += 1;
        };
        struct default_delete_mock : NaiveSTL::default_delete<int>{
            std::function<void()> called;

            void operator () (int *p) override{
                called();
                delete p;
            }
            default_delete_mock() = default;
        };
        auto d = default_delete_mock();
        d.called = called;
        {
            //auto a = unique_ptr<int, default_delete_mock>(6);

            auto a = unique_ptr<int, default_delete_mock>(new int(5), d);
        }
        EXPECT_EQ(counter, 1);
    }

    TEST(SHARED_PTR, PTR_FREE){
        int counter = 0;
        auto called = [&counter](){
            counter += 1;
        };
        struct default_delete_mock : NaiveSTL::default_delete<int>{
            std::function<void()> called;

            void operator () (int *p) override{
                called();
                delete p;
            }
            default_delete_mock() = default;
        };
        auto d = default_delete_mock();
        d.called = called;
        {
            auto a = shared_ptr<int, default_delete_mock>(new int(5), d);

            {
                //auto a = unique_ptr<int, default_delete_mock>(6);
                auto b = a;
                EXPECT_EQ(b.use_count(), 2);
                EXPECT_EQ(a.use_count(), 2);

            }
            EXPECT_EQ(a.use_count(), 1);
        }
        EXPECT_EQ(counter, 1);
    }



}