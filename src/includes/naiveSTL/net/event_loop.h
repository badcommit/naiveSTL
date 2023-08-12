//
// Created by Shawn Zhang on 8/9/23.
//

#ifndef NAIVESTL_EVENT_LOOP_H
#define NAIVESTL_EVENT_LOOP_H

#include <functional>
#include <mutex>
#include <poll.h>

#include "naiveSTL/utility.h"
#include "naiveSTL/net/common.h"
#include "naiveSTL/net/channel.h"
#include "naiveSTL/vector.h"
#include "naiveSTL/net/poller.h"


namespace NaiveSTL::Net{
    template<class P>
            requires std::is_base_of_v<NaiveSTL::Net::Poller, P>
    class EventLoop{
    public:


        EventLoop(unique_ptr<P> && poller);

        EventLoop(const EventLoop&) = delete;

        EventLoop& operator=(const EventLoop&) = delete;

        EventLoop(EventLoop&&) = delete;

        EventLoop& operator=(EventLoop&&) = delete;

        ~EventLoop();


        void loop();

        void quit();



    private:



        auto handleUpdateChannel(const Channel::UpdateInfo& t __attribute__((unused))) -> void {};

        int wakeupFd_;
        unique_ptr<P> poller_;
        std::atomic<bool> quit_;
        shared_ptr<Channel> wakeupChannel_;

        vector<shared_ptr<Channel>> channels_;



        mutable std::mutex mutex_;


    };

    template<class P>
    requires std::is_base_of_v<Poller, P>
    EventLoop<P>::EventLoop(unique_ptr<P> && poller) : wakeupFd_(createEventfd()), poller_(std::move(poller)), quit_(false),
                                                       wakeupChannel_(make_shared<Channel>(wakeupFd_, [this](auto&& t){
                                                           this->handleUpdateChannel(t);
                                                       })) {

//        wakeupChannel_->setReadCallback([this](timestamp) { this->wakeup(); });
//        wakeupChannel_->enableReading();
    }
    template<class P>
    requires std::is_base_of_v<Poller, P>
    EventLoop<P>::~EventLoop() {
        wakeupChannel_->disableReading();
        wakeupChannel_->disableWriting();
    }
    template<class P>
    requires std::is_base_of_v<Poller, P>
    void EventLoop<P>::loop() {
        while (!quit_) {
            vector<shared_ptr<Channel>> active;
            poller_->poll(channels_, active);
            for (auto && channel : active) {
                channel->handleEvent(timestamp());
            }
        }
    }

    template<class P>
    requires std::is_base_of_v<Poller, P>
    void EventLoop<P>::quit() {
        quit_ = true;
    }
}

#endif //NAIVESTL_EVENT_LOOP_H
