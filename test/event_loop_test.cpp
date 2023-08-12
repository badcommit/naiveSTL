//
// Created by Shawn Zhang on 8/12/23.
//


#include <gtest/gtest.h>
#include "naiveSTL/net/event_loop.h"
#include "naiveSTL/memory.h"
#include "naiveSTL/string.h"
#include "naiveSTL/net/poll_poller.h"
namespace NaiveSTL::unique_ptr_test {


    using namespace NaiveSTL::Net;
    TEST(EVENT_LOOP, CTR){
        auto abc = make_unique<PollPoller>();
//        EventLoop loop(std::move(abc));
    }

}