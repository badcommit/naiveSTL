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

        Channel(int fd) : fd_(fd) {};

        Channel(const Channel &) = delete;

        Channel &operator=(const Channel &) = delete;

        Channel(Channel &&) = default;


        ~Channel() = default;

        void handleEvent(timestamp receiveTime);


        void setReadCallback(ReadEventCallback &&callback) {

            readCallback_ = std::move(callback);

        }

        void setWriteCallback(EventCallback &&callback) {
            writeCallback_ = std::move(callback);

        }

        void setCloseCallback(EventCallback &&callback) {
            closeCallback_ = std::move(callback);

        }

        void setErrorCallback(EventCallback &&callback) {
            errorCallback_ = std::move(callback);

        }

        [[nodiscard]] auto fd() const -> int { return fd_; }

        auto enableReading() {
            event_mask_ = event_mask_ | as_integer(Event::kReadEvent);
            updateCallback_({fd()});
        }

        auto disableReading() {
            event_mask_ = event_mask_ & ~as_integer(Event::kReadEvent);
            updateCallback_({fd()});
        }

        auto enableWriting() {
            event_mask_ = event_mask_ | as_integer(Event::kWriteEvent);
            updateCallback_({fd()});
        }

        auto disableWriting() {
            event_mask_ = event_mask_ & ~as_integer(Event::kWriteEvent);
            updateCallback_({fd()});
        }

        auto setRevents(int revents) { revents_ = revents; }

        auto getRevents() const -> int { return revents_; }

        auto getEventMask() const -> int { return event_mask_; }

        auto setUpdateCallback(UpdateCallback &&callback) -> void { updateCallback_ = std::move(callback); }

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
