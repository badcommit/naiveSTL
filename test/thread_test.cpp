//
// Created by Shawn Zhang on 8/9/23.
//

#include <gtest/gtest.h>
#include <thread>

#include "naiveSTL/vector.h"
#include "naiveSTL/thread.h"
#include "naiveSTL/thread_pool.h"

namespace NaiveSTL::thread_test {


    TEST(THREAD, SUBMIT){
        auto threadpool = ThreadPool(4);
        std::atomic<int> counter = 0;
        threadpool.start();
        threadpool.submit([&](){
            counter ++;
            EXPECT_EQ(counter, 1);
        });

        threadpool.submit([&](){
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            counter ++;
            EXPECT_EQ(counter, 2);
        });
        threadpool.submit([&](){
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            counter ++;
            EXPECT_EQ(counter, 3);
        });


            threadpool.waitUntilAllTasksPicked();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        EXPECT_EQ(counter, 3);

    }

}