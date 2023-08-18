//
// Created by Shawn Zhang on 8/11/23.
//

#include <poll.h>

#include "naiveSTL/net/poll_poller.h"
#include "naiveSTL/algorithm.h"

#include "naiveSTL/net/event_loop.h"
#include "naiveSTL/net/common.h"

namespace NaiveSTL::Net {
    void PollPoller::poll(vector<shared_ptr<Channel>> &channels, vector<shared_ptr<Channel>> &active) {
        constexpr int timeout = 500;
        int numEvents = ::poll(&*pollfds_.begin(), pollfds_.size(), timeout);
        int savedErrno = errno;
        if (numEvents > 0) {
            fillActiveChannels(channels,numEvents, active);
        } else if (numEvents == 0) {

        } else {
            if (savedErrno != EINTR) {
                errno = savedErrno;
            }
        }
    }

    auto PollPoller::fillActiveChannels(
            vector<shared_ptr<Channel>> &channels,
            int numEvents,
            vector<shared_ptr<NaiveSTL::Net::Channel>> &activeChannels) const -> void {

        for (auto *pollfd = pollfds_.begin(); pollfd != pollfds_.end() && numEvents > 0; pollfd++, numEvents--) {
            if (pollfd->revents > 0) {
                for (auto&& ch: channels) {
                    if (ch->fd() == pollfd->fd) {
                        ch->setRevents(pollfd->revents);
                        activeChannels.push_back(ch);
                        break;
                    }
                }
            }
        }

    }

    auto PollPoller::hasChannel(const vector<shared_ptr<Channel>> &channels, int fd) const -> bool {
        for (auto ch: channels) {
            if (ch->fd() == fd) {
                return true;
            }
        }
        return false;
    }

    auto PollPoller::notifyUpdateChannel(const shared_ptr<Channel> &channel) -> void {
        for (auto & pollfd : pollfds_) {
            if (pollfd.fd == channel->fd()) {
                pollfd.events = static_cast<short>(channel->getEventMask());
                pollfd.revents = static_cast<short>(channel->getRevents());
                break;
            }
        }
    }

    auto PollPoller::getChannelByFd(const vector<shared_ptr<Channel>> &channels, int fd) -> shared_ptr<Channel> {
        for (auto & ch : channels) {
            if (ch->fd() == fd) {
                return ch;
            }
        }
        throw std::runtime_error("Channel not found");
    }

    auto PollPoller::notifyNewChannel(const shared_ptr<Channel>& channel) -> void {
        pollfd_t pollfd{};

        pollfd.fd = channel->fd();
        pollfd.events = static_cast<short>(channel->getEventMask());
        pollfd.revents = static_cast<short>(channel->getRevents());
        pollfds_.push_back(pollfd);
    }

    auto PollPoller::notifyRemoveChannel(const shared_ptr<Channel>& channel) -> void {
        auto *polyIter = find_if(pollfds_.begin(), pollfds_.end(), [&](auto && poly){return poly.fd == channel->fd();});
        if(polyIter != pollfds_.end()) {
            pollfds_.erase(polyIter);
        }
    }
}