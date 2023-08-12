//
// Created by Shawn Zhang on 8/11/23.
//

#ifndef NAIVESTL_POLL_POLLER_H
#define NAIVESTL_POLL_POLLER_H

#include <poll.h>

#include "naiveSTL/net/poller.h"

namespace NaiveSTL::Net {
    using pollfd_t = struct pollfd;
    class PollPoller:  Poller {
    public:
        PollPoller() {};

        ~PollPoller() override = default;

        virtual auto poll(vector<shared_ptr<Channel>>&,  vector<shared_ptr<Channel>>& active) -> void override;

        virtual void updateChannel(vector<shared_ptr<Channel>>&, int fd) override;


        [[nodiscard]] virtual auto hasChannel(vector<shared_ptr<Channel>>&, int fd) const -> bool  override;
    private:

        void fillActiveChannels(int numEvents,
                                vector<shared_ptr<Channel>> &,
                                vector<shared_ptr<Channel>>& ) const;

        vector<pollfd_t> pollfds_;



    };
}

#endif //NAIVESTL_POLL_POLLER_H
