//
// Created by Zhang Shawn on 8/18/23.
//

#ifndef NAIVESTL_TCP_SERVER_H
#define NAIVESTL_TCP_SERVER_H

#include "naiveSTL/vector.h"
#include "naiveSTL/utility.h"

#include "naiveSTL/net/tcp_connection.h"
#include "event_loop_thread_pool.h"

namespace NaiveSTL::Net {
    class TCPServer {
    public:
        using Callback = std::function<void(TCPConnection&, timestamp ts)>;

        explicit TCPServer(
                InetAddress& listenAddr,
                string  nameArg,
                size_t thread_cnt,
                unique_ptr<Poller> &&poller,
                Callback &&messageCb
        );

        void start();

        ~TCPServer();

    private:

        void handleListenConnection(timestamp ts);

        unique_ptr<EventLoopThread> event_thread_loop_;
        shared_ptr<Channel> accept_channel_;
        InetAddress listenAddr_;
        const string name_;

        unique_ptr<EventLoopThreadPool> pools_;

        vector<shared_ptr<TCPConnection>> connections_;
        Callback messsage_cb_;

    };
}

#endif //NAIVESTL_TCP_SERVER_H
