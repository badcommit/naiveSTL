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

    auto sockaddr_cast( void *const addr) -> struct sockaddr*
    {
        return static_cast<struct sockaddr*>(static_cast<void*>(addr));
    }

    const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr)
    {
        return static_cast<const struct sockaddr_in*>(static_cast<const void*>(addr));
    }

    void listenOrDie(int sockfd)
    {
        ::listen(sockfd, SOMAXCONN);
    }

    void bindOrDie(int sockfd, const struct sockaddr* addr)
    {
        auto ret = ::bind(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr)));
        assert(ret == 0);
    }

    int connect(int sockfd, const struct sockaddr* addr)
    {
        auto ret = ::connect(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr)));
        assert(ret == 0);
        return ret;
    }

    int accept(int sockfd, struct sockaddr_in* addr)
    {
        socklen_t addrlen = static_cast<socklen_t>(sizeof *addr);
        int connfd = ::accept(sockfd, const_cast<sockaddr *>(sockaddr_cast(addr)),
                              &addrlen);

        return connfd;
    }

    auto read(int sockfd, void *buf, size_t count) -> ssize_t
    {
        return ::read(sockfd, buf, count);
    }
    void close(int sockfd)
    {
        auto ret = ::close(sockfd);
        assert(ret == 0);
    }

    ssize_t write(int sockfd, const void *buf, size_t count)
    {
        return ::write(sockfd, buf, count);
    }

    void toIp(char* buf, size_t size,
              const struct sockaddr* addr)
    {

        const struct sockaddr_in* addr4 = sockaddr_in_cast(addr);
        ::inet_ntop(AF_INET, &addr4->sin_addr, buf, static_cast<socklen_t>(size));

    }

    void toIpPort(char* buf, size_t size,
                           const struct sockaddr* addr)
    {

        toIp(buf, size, addr);
        size_t end = ::strnlen(buf, 64);
        const struct sockaddr_in* addr4 = sockaddr_in_cast(addr);
        uint16_t port = (addr4->sin_port);
        assert(size > end);
        snprintf(buf+end, size-end, ":%u", port);
    }



}

#endif //NAIVESTL_SOCKOPS_H
