//
// Created by Shawn Zhang on 8/9/23.
//

#ifndef NAIVESTL_CHANNEL_H
#define NAIVESTL_CHANNEL_H


#include <functional>

#include "naiveSTL/utility.h"
#include "naiveSTL/net/event_loop.h"
#include "naiveSTL/memory.h"

namespace NaiveSTL {

    class Channel: noncopyable {
    public:
        using EventCallback = std::function<void()>;
        using ReadEventCallback = std::function<void(std::chrono::milliseconds)>;

        enum class Event {
            kNoneEvent = 0,
            kReadEvent = 1,
            kWriteEvent = 2,
        };
    public:
        Channel(shared_ptr<EventLoop> loop, int fd): fd_(fd), eventHandling_(false), addedToLoop_(false), loop_(loop) {};
        ~Channel();

        void handleEvent(std::chrono::milliseconds receiveTime);
        void setReadCallback(ReadEventCallback && cb) {
            readCallback_ = std::move(cb);
        }

        void setWriteCallback(EventCallback && cb) {
            writeCallback_ = std::move(cb);
        }

        void setCloseCallback(EventCallback && cb) {
            closeCallback_ = std::move(cb);
        }

        void setErrorCallback(EventCallback && cb) {
            errorCallback_ = std::move(cb);
        }

        [[nodiscard]] int fd() const { return fd_; }

        auto enableReading() { event_mask_ = event_mask_ | as_integer(Event::kReadEvent); }


    private:
        const int fd_;

        int event_mask_;

        bool eventHandling_;
        bool addedToLoop_;
        ReadEventCallback readCallback_;
        EventCallback writeCallback_;
        EventCallback closeCallback_;
        EventCallback errorCallback_;
        shared_ptr<EventLoop> loop_;
    };
}

#endif //NAIVESTL_CHANNEL_H
