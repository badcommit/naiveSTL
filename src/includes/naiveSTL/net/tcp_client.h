//
// Created by Zhang Shawn on 8/18/23.
//

#ifndef NAIVESTL_TCP_CLIENT_H
#define NAIVESTL_TCP_CLIENT_H

#include "naiveSTL/memory.h"
#include "inet_address.h"
#include "naiveSTL/net/socket.h"
#include "naiveSTL/net/event_loop_thread.h"
#include "tcp_connection.h"

namespace NaiveSTL::Net {
    class TCPClient {

    public:
        using Callback = std::function<void(TCPConnection&, timestamp ts)>;
        explicit TCPClient(const InetAddress& serverAddr);
        void connect();
        void disconnect();

        void setConnectCallback(Callback && cb){
            onConnect_ = std::move(cb);
        };

        void setMessageCallBack(Callback &&cb) {
            onRecv_ = std::move(cb);
        }
    private:
        Callback onConnect_;
        Callback onRecv_;
        unique_ptr<EventLoopThread> event_loop_;
        shared_ptr<TCPConnection> connection_;
        InetAddress serverAddr_;

    };
}

#endif //NAIVESTL_TCP_CLIENT_H
