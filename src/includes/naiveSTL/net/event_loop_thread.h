//
// Created by Shawn Zhang on 8/13/23.
//

#ifndef NAIVESTL_EVENT_LOOP_THREAD_H
#define NAIVESTL_EVENT_LOOP_THREAD_H

#include "naiveSTL/memory.h"
#include "naiveSTL/net/event_loop.h"
#include "naiveSTL/thread.h"

namespace NaiveSTL::Net{
    class EventLoopThread {
    public:
        EventLoopThread(unique_ptr<Poller> && poller):loop_(make_unique<EventLoop>(std::move(poller))), thread_([this] { threadFunc(); }, "hello world") {

        };

        EventLoopThread(const EventLoopThread&) = delete;

        EventLoopThread& operator=(const EventLoopThread&) = delete;

        EventLoopThread(EventLoopThread&&) = delete;

        EventLoopThread& operator=(EventLoopThread&&) = delete;

        ~EventLoopThread() = default;

        void start();

        void stop();

        auto getLoop() -> EventLoop& {
            return *(loop_);
        };

    private:
        void threadFunc();

        unique_ptr<EventLoop> loop_;
        Thread thread_;
    };
}

#endif //NAIVESTL_EVENT_LOOP_THREAD_H
