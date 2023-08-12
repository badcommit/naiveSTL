//
// Created by Shawn Zhang on 8/9/23.
//

#ifndef NAIVESTL_CHANNEL_H
#define NAIVESTL_CHANNEL_H


#include <functional>
#include <utility>

#include "naiveSTL/utility.h"
#include "naiveSTL/memory.h"
#include "naiveSTL/net/common.h"

namespace NaiveSTL::Net {

    class Channel {
    public:

        struct UpdateInfo {
            int fd;
        };

        using EventCallback = std::function<void()>;
        using ReadEventCallback = std::function<void(timestamp)>;
        using UpdateCallback = std::function<void(UpdateInfo)>;


        enum class Event {
            kNoneEvent = 0,
            kReadEvent = 1,
            kWriteEvent = 2,
        };

        Channel(int fd, UpdateCallback updateCallback) : fd_(fd), updateCallback_(std::move(updateCallback)) {};

        Channel(const Channel &) = delete;

        Channel &operator=(const Channel &) = delete;

        Channel(Channel &&) = default;


        ~Channel() = default;

        void handleEvent(timestamp receiveTime);


        void setReadCallback(ReadEventCallback &&callback) {

            readCallback_ = std::move(callback);
            updateCallback_({fd()});
        }

        void setWriteCallback(EventCallback &&callback) {
            writeCallback_ = std::move(callback);
            updateCallback_({fd()});
        }

        void setCloseCallback(EventCallback &&callback) {
            closeCallback_ = std::move(callback);
            updateCallback_({fd()});
        }

        void setErrorCallback(EventCallback &&callback) {
            errorCallback_ = std::move(callback);
            updateCallback_({fd()});
        }

        [[nodiscard]] auto fd() const -> int { return fd_; }

        auto enableReading() { event_mask_ = event_mask_ | as_integer(Event::kReadEvent); }

        auto disableReading() { event_mask_ = event_mask_ & ~as_integer(Event::kReadEvent); }

        auto enableWriting() { event_mask_ = event_mask_ | as_integer(Event::kWriteEvent); }

        auto disableWriting() { event_mask_ = event_mask_ & ~as_integer(Event::kWriteEvent); }

        auto setRevents(int revents) { revents_ = revents; }


    private:

        const int fd_;
        int revents_;

        int event_mask_{0};
        ReadEventCallback readCallback_;
        EventCallback writeCallback_;
        EventCallback closeCallback_;
        EventCallback errorCallback_;

        UpdateCallback updateCallback_;
    };
}

#endif //NAIVESTL_CHANNEL_H
