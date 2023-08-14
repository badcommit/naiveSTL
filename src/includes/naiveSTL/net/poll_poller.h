//
// Created by Shawn Zhang on 8/11/23.
//

#ifndef NAIVESTL_POLL_POLLER_H
#define NAIVESTL_POLL_POLLER_H

#include <poll.h>

#include "naiveSTL/net/poller.h"

namespace NaiveSTL::Net {
    using pollfd_t = struct pollfd;
    class PollPoller: public Poller {
    public:
        PollPoller() {};

        ~PollPoller() override = default;

        auto poll(vector<shared_ptr<Channel>>& /*unused*/,  vector<shared_ptr<Channel>>& active) -> void override;

        void notifyUpdateChannel(const shared_ptr<Channel>& /*unused*/) override;

        auto getChannelByFd(const vector<shared_ptr<Channel>>&, int fd) -> shared_ptr<Channel> override;


        [[nodiscard]] auto hasChannel(const vector<shared_ptr<Channel>>&, int fd) const -> bool  override;

        auto  notifyNewChannel(const shared_ptr<Channel>& /*unused*/) -> void override;

        auto notifyRemoveChannel(const shared_ptr<Channel>& /*unused*/) -> void override;
    private:

        void fillActiveChannels(vector<shared_ptr<Channel>> &,
                                int numEvents,
                                vector<shared_ptr<Channel>>& ) const;



        vector<pollfd_t> pollfds_;



    };
}

#endif //NAIVESTL_POLL_POLLER_H
