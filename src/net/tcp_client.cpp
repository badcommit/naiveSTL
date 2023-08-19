//
// Created by Zhang Shawn on 8/18/23.
//
#include "naiveSTL/net/tcp_client.h"
#include "naiveSTL/net/tcp_connection.h"
#include "naiveSTL/net/poll_poller.h"

namespace NaiveSTL::Net{
    TCPClient::TCPClient(const NaiveSTL::Net::InetAddress &serverAddr): serverAddr_(serverAddr) {

        event_loop_ = make_unique<EventLoopThread>(make_unique<PollPoller>());

    }

    void TCPClient::connect() {
        event_loop_->start();
        event_loop_->getLoop()->queueCallback([this](){
            int sockfd = SocketOps::createNonblockingOrDie(serverAddr_.family());
            Socket socket{sockfd};
            socket.connect(serverAddr_);
            connection_ = make_shared<TCPConnection>(event_loop_->getLoop(), socket.fd());
            connection_->setMessageCallback(this->onRecv_);
            if(this->onConnect_){
                this->onConnect_(*connection_, timestamp{});
            }
        });

    }

    void TCPClient::disconnect() {
        event_loop_->getLoop()->queueCallback([this](){
            if(connection_ != nullptr){
                connection_->close();
            }
        });
    }
}