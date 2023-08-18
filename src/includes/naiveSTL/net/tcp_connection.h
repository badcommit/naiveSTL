//
// Created by Shawn Zhang on 8/9/23.
//

#ifndef NAIVESTL_TCP_CONNECTION_H
#define NAIVESTL_TCP_CONNECTION_H

#include <sys/socket.h>

#include "memory"


#include "naiveSTL/utility.h"
#include "event_loop.h"
#include "naiveSTL/string.h"
#include "naiveSTL/net/socket.h"


namespace NaiveSTL::Net {
    class TCPConnection : std::enable_shared_from_this<TCPConnection> {
    public:
        using Callback = std::function<void(const shared_ptr<TCPConnection>&)>;
        TCPConnection(shared_ptr<EventLoop>& loop,
                      const string & arg,
                      int socketfd,
                      const InetAddress& localAddr,
                      const InetAddress& peerAddr);

        void setConnectionCallback(const Callback& cb)
        { connectionCallback_ = cb; }

        void setMessageCallback(const Callback& cb)
        { messageCallback_ = cb; }

        void send(const string& message);

    private:

        enum class StateE {
            kDisconnected, kConnecting, kConnected, kDisconnecting
        };

        void handleRead(timestamp t);

        void handleWrite();

        void handleClose();

        void handleError();

        void setState(StateE stateE);

        void startReadInLoop();

        void stopReadInLoop();



        shared_ptr<EventLoop> event_loop_;

        const string name_;

        unique_ptr<Socket> socket_;

        shared_ptr<Channel> channel_;

        const InetAddress localAddr_;
        const InetAddress peerAddr_;

        Callback connectionCallback_;
        Callback messageCallback_;

        vector<char> input_buffer_{};
        vector<char> output_buffer_{};

    };
}

#endif //NAIVESTL_TCP_CONNECTION_H
