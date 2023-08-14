//
// Created by Shawn Zhang on 8/13/23.
//

#include "naiveSTL/net/event_loop_thread.h"

namespace NaiveSTL::Net{
    void EventLoopThread::start() {
        thread_.start();
    }

    void EventLoopThread::stop() {
        loop_->quit();
        thread_.join();
    }

    void EventLoopThread::threadFunc() {
        loop_->loop();
    }
}