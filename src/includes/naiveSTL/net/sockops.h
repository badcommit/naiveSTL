//
// Created by Zhang Shawn on 8/17/23.
//

#ifndef NAIVESTL_SOCKOPS_H
#define NAIVESTL_SOCKOPS_H

#include <cassert>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdio>

namespace NaiveSTL::Net::SocketOps {

    auto sockaddr_cast( void *const addr) -> struct sockaddr*;

    const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr);

    int createNonblockingOrDie(sa_family_t family);

    void listenOrDie(int sockfd);

    void bindOrDie(int sockfd, const struct sockaddr* addr);

    int connect(int sockfd, const struct sockaddr* addr);

    int accept(int sockfd, struct sockaddr_in* addr);

    auto read(int sockfd, void *buf, size_t count) -> ssize_t;
    void close(int sockfd);

    ssize_t write(int sockfd, const void *buf, size_t count);

    void toIp(char* buf, size_t size,
              const struct sockaddr* addr);

    void toIpPort(char* buf, size_t size,
                           const struct sockaddr* addr);



}

#endif //NAIVESTL_SOCKOPS_H
