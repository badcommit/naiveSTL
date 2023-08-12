//
// Created by Shawn Zhang on 8/11/23.
//

#include <poll.h>

#include "naiveSTL/net/poll_poller.h"


#include "naiveSTL/net/event_loop.h"
#include "naiveSTL/net/common.h"

namespace NaiveSTL::Net {
    void PollPoller::poll(vector<shared_ptr<Channel>> &channels, vector<shared_ptr<Channel>> &active) {
        int numEvents = ::poll(&*pollfds_.begin(), pollfds_.size(), -1);
        int savedErrno = errno;
        if (numEvents > 0) {
            fillActiveChannels(numEvents, channels, active);
        } else if (numEvents == 0) {

        } else {
            if (savedErrno != EINTR) {
                errno = savedErrno;
            }
        }
    }

    auto PollPoller::fillActiveChannels(int numEvents,
                                        vector<shared_ptr<Channel>> &channels,
                                        vector<shared_ptr<NaiveSTL::Net::Channel>> &activeChannels) const -> void {

        for(auto pollfd = pollfds_.begin(); pollfd != pollfds_.end() && numEvents > 0; pollfd ++, numEvents--){
            if(pollfd->revents > 0){
                for(auto ch: channels){
                    if(ch->fd() == pollfd->fd){
                        ch->setRevents(pollfd->revents);
                        activeChannels.push_back(ch);
                        break;
                    }
                }
            }
        }

    }

    auto PollPoller::hasChannel(vector<shared_ptr<Channel>> &channels, int fd) const -> bool {
        for(auto ch: channels){
            if(ch->fd() == fd){
                return true;
            }
        }
        return false;
    }

    auto PollPoller::updateChannel(vector<shared_ptr<Channel>> &channels __attribute__((unused)), int fd __attribute__((unused))) -> void {
//        if(hasChannel(channels, fd)){
//            return;
//        }
//        pollfd_t pollfd{};
//        pollfd.fd = fd;
//        pollfd.events = static_cast<short>(Event::kNoneEvent);
//        pollfd.revents = static_cast<short>(Event::kNoneEvent);
//        pollfds_.push_back(pollfd);
    }
}