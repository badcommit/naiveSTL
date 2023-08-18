//
// Created by Zhang Shawn on 8/17/23.
//

#include "naiveSTL/net/socket.h"

namespace NaiveSTL::Net {
    void Socket::bindAddress(NaiveSTL::Net::InetAddress &localaddr) {
        return SocketOps::bindOrDie(sockfd_, localaddr.getSockAddr());
    }

    void Socket::listen() {
        SocketOps::listenOrDie(sockfd_);
    }

    int Socket::accept(InetAddress &peeraddr) {
        struct sockaddr_in addr;
        memset(&addr,0, sizeof addr);
        int connfd = SocketOps::accept(sockfd_, &addr);
        if (connfd >= 0)
        {
            peeraddr.setSockAddrInet(addr);
        }
        return connfd;
    }
}