//
// Created by Zhang Shawn on 8/17/23.
//

#ifndef NAIVESTL_INET_ADDRESS_H
#define NAIVESTL_INET_ADDRESS_H

#include <cstdint>
#include <sys/socket.h>

#include "naiveSTL/string.h"
#include "naiveSTL/net/sockops.h"

namespace NaiveSTL::Net {
    class InetAddress  {
    public:
        explicit InetAddress(uint16_t port = 0);

        [[nodiscard]] auto getSockAddr() const -> const struct sockaddr*  { return SocketOps::sockaddr_cast(
                    const_cast<void *>(static_cast<const void *>(&addr_))); }

        [[nodiscard]] auto family() const -> sa_family_t { return addr_.sin_family; }
        auto toIp() const -> string;
        auto toIpPort() const -> string;

        void setSockAddrInet(const struct sockaddr_in& addr) { addr_ = addr; }

    private:
        struct sockaddr_in addr_;
    };
}

#endif //NAIVESTL_INET_ADDRESS_H
