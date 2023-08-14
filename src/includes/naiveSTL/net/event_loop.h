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

    class EventLoop{
    public:


        EventLoop(unique_ptr<Poller> && poller);

        EventLoop(const EventLoop&) = delete;

        EventLoop& operator=(const EventLoop&) = delete;

        EventLoop(EventLoop&&) = delete;

        EventLoop& operator=(EventLoop&&) = delete;


        ~EventLoop();

        void attachChannel(shared_ptr<Channel>& channel);

        void removeChannel(shared_ptr<Channel>& channel);

        void loop();

        void quit();



    private:



        auto handleUpdateChannel(const Channel::UpdateInfo& t __attribute__((unused))) -> void;

        unique_ptr<Poller> poller_;
        std::atomic<bool> quit_;

        vector<shared_ptr<Channel>> channels_;


        mutable std::mutex mutex_;


    };



}

#endif //NAIVESTL_EVENT_LOOP_H
