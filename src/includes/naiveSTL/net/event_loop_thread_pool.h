//
// Created by Shawn Zhang on 8/13/23.
//

#ifndef NAIVESTL_EVENT_LOOP_THREAD_POOL_H
#define NAIVESTL_EVENT_LOOP_THREAD_POOL_H

#include "naiveSTL/net/event_loop_thread.h"
#include "naiveSTL/net/poller.h"

namespace NaiveSTL::Net{

    class EventLoopThreadPool {
    public:
        EventLoopThreadPool(unique_ptr<Poller> && poller, int threadNum):poller_(std::move(poller)), threadNum_(threadNum) {

        };

        void start();

        auto getNextLoop() -> EventLoop&;
    private:
        unique_ptr<Poller> poller_;
        vector<unique_ptr<EventLoopThread>> threads_;
        int threadNum_;
        int nxt_;
    };

}



#endif //NAIVESTL_EVENT_LOOP_THREAD_POOL_H
