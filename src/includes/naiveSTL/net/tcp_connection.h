//
// Created by Shawn Zhang on 8/9/23.
//

#ifndef NAIVESTL_TCP_CONNECTION_H
#define NAIVESTL_TCP_CONNECTION_H

#include <sys/socket.h>



#include "naiveSTL/utility.h"
#include "event_loop.h"
#include "naiveSTL/string.h"
#include "naiveSTL/net/socket.h"


namespace NaiveSTL::Net {
    //using std because need to implement weak ptr and atomic operation
    class TCPConnection {
    public:
        //using tcp because
        using Callback = std::function<void(TCPConnection&, timestamp)>;
        TCPConnection(shared_ptr<EventLoop> loop,
                      int socketfd);

        TCPConnection(const TCPConnection &) = delete;
        TCPConnection & operator = (const TCPConnection &) = delete;

        void close();


        void setMessageCallback(const Callback& cb)
        { messageCallback_ = cb; }



        void send(const string& message);

    private:


        void handleChannelRead(timestamp t);

        void handleChannelWrite();

        void handleChannelClose();

        void handleChannelError();


        shared_ptr<EventLoop> event_loop_;

        shared_ptr<Channel> channel_;


        Callback messageCallback_;

        vector<char> input_buffer_{};
        vector<char> output_buffer_{};


    };
}

#endif //NAIVESTL_TCP_CONNECTION_H
