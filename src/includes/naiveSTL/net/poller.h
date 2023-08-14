//
// Created by Shawn Zhang on 8/11/23.
//

#ifndef NAIVESTL_POLLER_H
#define NAIVESTL_POLLER_H


#include <unordered_map>


#include "naiveSTL/net/common.h"
#include "naiveSTL/net/channel.h"
#include "naiveSTL/vector.h"

namespace NaiveSTL::Net {
    class Poller {
    public:
        Poller() = default;

        Poller(Poller&&) = default;


        virtual ~Poller() = default;

        virtual auto poll(vector<shared_ptr<Channel>>&, vector<shared_ptr<Channel>>& active) -> void = 0;

        virtual void notifyUpdateChannel(const shared_ptr<Channel>&) = 0;
        virtual void notifyNewChannel(const shared_ptr<Channel>&) = 0;
        virtual void notifyRemoveChannel(const shared_ptr<Channel>&) = 0;

        virtual shared_ptr<Channel> getChannelByFd(const vector<shared_ptr<Channel>>&, int fd) = 0;


        [[nodiscard]] virtual auto hasChannel(const vector<shared_ptr<Channel>>&, int fd) const -> bool  = 0;
    private:

    };
}

#endif //NAIVESTL_POLLER_H
