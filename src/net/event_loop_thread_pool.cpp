//
// Created by Shawn Zhang on 8/13/23.
//

#include "naiveSTL/net/event_loop_thread_pool.h"

namespace NaiveSTL::Net{
    void EventLoopThreadPool::start() {
        for(int i = 0; i < threadNum_; i++){
            auto event_thread =  make_unique<EventLoopThread>(std::move(poller_));
            threads_.push_back(std::move(event_thread));
        }
        for (const auto & thread : threads_) {
            thread->start();
        }
    }

    auto EventLoopThreadPool::getNextLoop() -> EventLoop& {
        nxt_ = (nxt_ + 1) % (threads_.size());
        return threads_[nxt_]->getLoop();
    }
}