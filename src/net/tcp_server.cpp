//
// Created by Zhang Shawn on 8/18/23.
//

#include <utility>

#include "naiveSTL/net/tcp_server.h"
#include "naiveSTL/net/poll_poller.h"

namespace NaiveSTL::Net {
    TCPServer::TCPServer(NaiveSTL::Net::InetAddress &listenAddr,
                         NaiveSTL::string nameArg, size_t thread_cnt, unique_ptr<Poller> &&poller,
                         NaiveSTL::Net::TCPServer::Callback &&messageCb) :
            event_thread_loop_(make_unique<EventLoopThread>(make_unique<PollPoller>())),
            listenAddr_(listenAddr),
            name_(std::move(nameArg)),
            pools_(make_unique<EventLoopThreadPool>(std::move(poller), thread_cnt)),
            messsage_cb_(messageCb) {
        auto sock = SocketOps::createNonblockingOrDie(listenAddr_.family());
        accept_channel_ = make_shared<Channel>(Socket{sock});
        accept_channel_->socket().bindAddress(listenAddr_);
        accept_channel_->enableReading();
        accept_channel_->setReadCallback([this](timestamp ts){
            this->handleListenConnection(ts);
        });
        event_thread_loop_->getLoop()->attachChannel(accept_channel_);

    }

    void TCPServer::start() {
        event_thread_loop_->getLoop()->queueCallback([this](){
            this->accept_channel_->socket().listen();
        });
        pools_->start();
    }

    void TCPServer::handleListenConnection(timestamp ts __attribute__((unused))) {
        InetAddress peeraddr;
        auto fd = this->accept_channel_->socket().accept(peeraddr);
        shared_ptr<EventLoop> loop = this->pools_->getNextLoop();
        auto establish_read_connection = [this, loop, fd](){
            auto tcp_connection = make_shared<TCPConnection>(loop, fd);
            this->connections_.push_back(tcp_connection);
            tcp_connection->setMessageCallback([this](TCPConnection &tcp, timestamp ts){
                if(this->messsage_cb_){
                    this->messsage_cb_(tcp, ts);
                }
            });
        };
        loop->queueCallback(std::move(establish_read_connection));
    }

    TCPServer::~TCPServer() {
        event_thread_loop_->stop();
        pools_->stop();

    }
}