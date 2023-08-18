//
// Created by Zhang Shawn on 8/17/23.
//

#include <array>

#include "naiveSTL/net/inet_address.h"
#include "naiveSTL/net/sockops.h"

namespace NaiveSTL::Net {
    static const in_addr_t kInaddrLoopback = INADDR_LOOPBACK;

    InetAddress::InetAddress(uint16_t port) {
        memset(&addr_, 0, sizeof addr_);
        addr_.sin_family = AF_INET;
        in_addr_t ip =  kInaddrLoopback;
        addr_.sin_addr.s_addr = ip;
        addr_.sin_port = port;
    }
    string InetAddress::toIpPort() const
    {
        char buf[64] = "";
        SocketOps::toIpPort(buf, sizeof buf, getSockAddr());
        return {buf};
    }

    string InetAddress::toIp() const
    {
        char buf[64];
        memset(buf, 0, sizeof buf);
        SocketOps::toIp(buf, sizeof buf, getSockAddr());
        return {buf};
    }


}