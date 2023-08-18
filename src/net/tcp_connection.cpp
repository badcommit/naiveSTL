//
// Created by Zhang Shawn on 8/17/23.
//

#include "naiveSTL/algorithm.h"
#include "naiveSTL/net/tcp_connection.h"

namespace NaiveSTL::Net {
    void TCPConnection::send(const NaiveSTL::string &message) {

        copy(message.begin(), message.end(), output_buffer_.end());
        auto write = [this](){
            SocketOps::write(this->socket_->fd(), output_buffer_.begin(), output_buffer_.size());
            output_buffer_.clear();
        };

        event_loop_->queueCallback(write);
    }

    TCPConnection::TCPConnection(shared_ptr<NaiveSTL::Net::EventLoop> &loop, const NaiveSTL::string &arg, int socketfd,
                                 const NaiveSTL::Net::InetAddress &localAddr,
                                 const NaiveSTL::Net::InetAddress &peerAddr) : event_loop_(loop), name_(arg), socket_(
            make_unique<Socket>(socketfd)), channel_(make_shared<Channel>(socketfd)), localAddr_(localAddr), peerAddr_(peerAddr),
                                                                               input_buffer_(), output_buffer_() {
            channel_->setReadCallback([this](timestamp t){
                this->handleRead(t);
            });

            channel_->setCloseCallback([this](){
                this->handleClose();
            });

            channel_->setWriteCallback([this](){
                this->handleWrite();
            });

            channel_->setErrorCallback([this](){
                this->handleError();
            });

    }
}
