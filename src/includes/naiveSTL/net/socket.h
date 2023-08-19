//
// Created by Zhang Shawn on 8/17/23.
//

#ifndef NAIVESTL_SOCKET_H
#define NAIVESTL_SOCKET_H

#include <sys/socket.h>
#include "inet_address.h"

namespace NaiveSTL::Net {
    class Socket {
    public:
        explicit Socket(int fd): sockfd_(fd){}
        Socket(const Socket&) = delete;
        auto operator = (const Socket &) -> Socket& = delete;

        Socket(Socket && socket) noexcept : sockfd_(socket.sockfd_){
            socket.sockfd_ = -1;
        }

        [[nodiscard]] auto fd() const -> int {
            return sockfd_;
        }

        auto operator = (Socket && socket) noexcept -> Socket& {

            sockfd_ = socket.sockfd_;
            return *this;
        }

        void listen();

        int accept(InetAddress & peeraddr);

        int connect(InetAddress &serveraddr);

        void bindAddress(InetAddress& localaddr);

        void close();
    private:
        int sockfd_;
    };
}

#endif //NAIVESTL_SOCKET_H
