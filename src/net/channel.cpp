//
// Created by Shawn Zhang on 8/11/23.
//

#include <sys/poll.h>
#include "naiveSTL/net/channel.h"


namespace NaiveSTL::Net {


    void Channel::handleEvent(timestamp receiveTime) {
        if(revents_ & POLLHUP && !(revents_ & POLLIN)) {
            if(closeCallback_) closeCallback_();
        }
        if (revents_ & (POLLERR | POLLNVAL))
        {
            if (errorCallback_) errorCallback_();
        }

        if (revents_ & (POLLIN | POLLPRI ))
        {
            if (readCallback_) readCallback_(receiveTime);
        }

        if (revents_ & POLLOUT)
        {
            if (writeCallback_) writeCallback_();
        }
    }
}
