//
// Created by Shawn Zhang on 8/11/23.
//

#include "naiveSTL/net/event_loop.h"
#include "naiveSTL/net/channel.h"

namespace NaiveSTL::Net {
    EventLoop::EventLoop(unique_ptr<Poller> && poller) : poller_(std::move(poller)), quit_(false) {}

    EventLoop::~EventLoop() = default;

    void EventLoop::loop() {
        while (!quit_) {

            vector<shared_ptr<Channel>> active_channels;
            mutex_.lock();
            vector<shared_ptr<Channel>> chans = channels_;
            mutex_.unlock();
            poller_->poll(chans, active_channels);
            for (auto && channel : active_channels) {
                channel->handleEvent(timestamp());
            }
        }
    }

    auto EventLoop::handleUpdateChannel(const Channel::UpdateInfo &info) -> void {
        if(poller_->hasChannel(channels_, info.fd)) {
            auto chan = poller_->getChannelByFd(channels_, info.fd);
            poller_->notifyUpdateChannel(chan);
        }
    }

    auto EventLoop::attachChannel(shared_ptr<Channel>& channel) -> void {
        mutex_.lock();
        channels_.push_back(channel);
        mutex_.unlock();
        channel->setUpdateCallback([this](auto && PH1) { handleUpdateChannel(std::forward<decltype(PH1)>(PH1)); });
        poller_->notifyNewChannel(channel);
    }

    auto EventLoop::removeChannel(shared_ptr<Channel>& channel) -> void {
        mutex_.lock();
        channels_.erase(std::find(channels_.begin(), channels_.end(), channel));
        mutex_.unlock();
        poller_->notifyRemoveChannel(channel);
    }


    void EventLoop::quit() {
        quit_ = true;
    }
}